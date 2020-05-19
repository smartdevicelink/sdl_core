/*
 * Copyright (c) 2017, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "protocol_handler/handshake_handler.h"

#include <bson_object.h>

#include "protocol_handler/protocol_handler_impl.h"
#include "protocol_handler/protocol_packet.h"
#include "protocol_handler/session_observer.h"
#include "security_manager/security_manager.h"
#include "utils/helpers.h"

namespace protocol_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "ProtocolHandler")

HandshakeHandler::HandshakeHandler(
    ProtocolHandlerImpl& protocol_handler,
    SessionObserver& session_observer,
    utils::SemanticVersion& full_version,
    const SessionContext& context,
    const uint8_t protocol_version,
    std::shared_ptr<BsonObject> payload,
    ServiceStatusUpdateHandler& service_status_update_handler)
    : protocol_handler_(protocol_handler)
    , session_observer_(session_observer)
    , context_(context)
    , full_version_(full_version)
    , protocol_version_(protocol_version)
    , payload_(payload)
    , service_status_update_handler_(service_status_update_handler) {}

HandshakeHandler::~HandshakeHandler() {
  LOG4CXX_DEBUG(logger_, "Destroying of HandshakeHandler: " << this);
}

uint32_t HandshakeHandler::connection_key() const {
  return session_observer_.KeyFromPair(context_.connection_id_,
                                       context_.new_session_id_);
}

uint32_t HandshakeHandler::primary_connection_key() const {
  return session_observer_.KeyFromPair(context_.primary_connection_id_,
                                       context_.new_session_id_);
}

bool HandshakeHandler::GetPolicyCertificateData(std::string& data) const {
  return false;
}

void HandshakeHandler::OnCertificateUpdateRequired() {
  LOG4CXX_AUTO_TRACE(logger_);
}

#if defined(EXTERNAL_PROPRIETARY_MODE) && defined(ENABLE_SECURITY)
bool HandshakeHandler::OnCertDecryptFailed() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (payload_) {
    ProcessFailedHandshake(*payload_, ServiceStatus::CERT_INVALID);
  }

  return true;
}
#endif

bool HandshakeHandler::OnGetSystemTimeFailed() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (payload_) {
    ProcessFailedHandshake(*payload_, ServiceStatus::INVALID_TIME);
  } else {
    BsonObject params;
    bson_object_initialize_default(&params);
    ProcessFailedHandshake(params, ServiceStatus::INVALID_TIME);
    bson_object_deinitialize(&params);
  }

  return true;
}

bool HandshakeHandler::OnPTUFailed() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (payload_) {
    ProcessFailedHandshake(*payload_, ServiceStatus::PTU_FAILED);
  }

  return true;
}

bool HandshakeHandler::OnHandshakeDone(
    uint32_t connection_key,
    security_manager::SSLContext::HandshakeResult result) {
  LOG4CXX_AUTO_TRACE(logger_);

  LOG4CXX_DEBUG(logger_,
                "OnHandshakeDone for service : " << context_.service_type_);

  if (connection_key != this->primary_connection_key()) {
    LOG4CXX_DEBUG(logger_,
                  "Listener " << this
                              << " expects notification for connection id: "
                              << this->primary_connection_key()
                              << ". Received notification for connection id "
                              << connection_key << " will be ignored");
    return false;
  }

  const bool success =
      result == security_manager::SSLContext::Handshake_Result_Success;

  if (payload_) {
    if (success) {
      ProcessSuccessfulHandshake(connection_key, *payload_);
    } else {
      ProcessFailedHandshake(*payload_, ServiceStatus::CERT_INVALID);
    }
  } else {
    BsonObject params;
    bson_object_initialize_default(&params);
    if (success) {
      ProcessSuccessfulHandshake(connection_key, params);
    } else {
      ProcessFailedHandshake(params, ServiceStatus::CERT_INVALID);
    }
    bson_object_deinitialize(&params);
  }

  return true;
}

bool HandshakeHandler::CanBeProtected() const {
  const auto& force_unprotected =
      protocol_handler_.get_settings().force_unprotected_service();

  return !(helpers::in_range(force_unprotected, context_.service_type_));
}

bool HandshakeHandler::IsAlreadyProtected() const {
  return (session_observer_.GetSSLContext(this->primary_connection_key(),
                                          context_.service_type_) != NULL);
}

void HandshakeHandler::ProcessSuccessfulHandshake(const uint32_t connection_key,
                                                  BsonObject& params) {
  LOG4CXX_AUTO_TRACE(logger_);

  const bool is_service_already_protected = IsAlreadyProtected();

  const bool can_be_protected = CanBeProtected();

  LOG4CXX_DEBUG(logger_,
                "Service can be protected: " << can_be_protected
                                             << " and service was protected: "
                                             << is_service_already_protected);

  if (can_be_protected && !is_service_already_protected) {
    session_observer_.SetProtectionFlag(connection_key, context_.service_type_);
    service_status_update_handler_.OnServiceUpdate(
        this->connection_key(),
        context_.service_type_,
        ServiceStatus::SERVICE_ACCEPTED);
    protocol_handler_.SendStartSessionAck(context_.connection_id_,
                                          context_.new_session_id_,
                                          protocol_version_,
                                          context_.hash_id_,
                                          context_.service_type_,
                                          can_be_protected,
                                          full_version_,
                                          params);
  } else {
    service_status_update_handler_.OnServiceUpdate(
        this->connection_key(),
        context_.service_type_,
        ServiceStatus::SERVICE_START_FAILED);
    protocol_handler_.SendStartSessionNAck(context_.connection_id_,
                                           context_.new_session_id_,
                                           protocol_version_,
                                           context_.service_type_);
  }
}

void HandshakeHandler::ProcessFailedHandshake(BsonObject& params,
                                              ServiceStatus service_status) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Handshake failed");
  const std::vector<int>& force_protected =
      protocol_handler_.get_settings().force_protected_service();

  const bool can_be_unprotected =
      std::find(force_protected.begin(),
                force_protected.end(),
                context_.service_type_) == force_protected.end();

  LOG4CXX_DEBUG(logger_,
                "Service can be unprotected: " << can_be_unprotected
                                               << " and this is a new service: "
                                               << context_.is_new_service_);

  if (can_be_unprotected && context_.is_new_service_) {
    service_status_update_handler_.OnServiceUpdate(
        this->connection_key(),
        context_.service_type_,
        ServiceStatus::PROTECTION_DISABLED);
    protocol_handler_.SendStartSessionAck(context_.connection_id_,
                                          context_.new_session_id_,
                                          protocol_version_,
                                          context_.hash_id_,
                                          context_.service_type_,
                                          PROTECTION_OFF,
                                          full_version_,
                                          params);
  } else {
    service_status_update_handler_.OnServiceUpdate(
        this->connection_key(), context_.service_type_, service_status);
    protocol_handler_.SendStartSessionNAck(context_.connection_id_,
                                           context_.new_session_id_,
                                           protocol_version_,
                                           context_.service_type_);
  }
}

}  // namespace protocol_handler
