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
#include "security_manager/security_manager.h"
#include "protocol_handler/session_observer.h"

namespace protocol_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "ProtocolHandler")

HandshakeHandler::HandshakeHandler(
    ProtocolHandlerImpl& protocol_handler,
    SessionObserver& session_observer,
    uint32_t connection_key,
    ConnectionID connection_id,
    uint8_t session_id,
    uint8_t protocol_version,
    uint32_t hash_id,
    ServiceType service_type,
    const std::vector<int>& force_protected_service,
    const bool is_new_service,
    ProtocolPacket::ProtocolVersion& full_version,
    std::shared_ptr<uint8_t> payload)
    : protocol_handler_(protocol_handler)
    , session_observer_(session_observer)
    , context_()
    , full_version_(full_version)
    , protocol_version_(protocol_version)
    , payload_(payload) {}

HandshakeHandler::HandshakeHandler(
    ProtocolHandlerImpl& protocol_handler,
    SessionObserver& session_observer,
    ProtocolPacket::ProtocolVersion& full_version,
    const SessionContext& context,
    const uint8_t protocol_version,
    std::shared_ptr<uint8_t> payload)
    : protocol_handler_(protocol_handler)
    , session_observer_(session_observer)
    , context_(context)
    , full_version_(full_version)
    , protocol_version_(protocol_version)
    , payload_(payload) {}

HandshakeHandler::~HandshakeHandler() {
  LOG4CXX_DEBUG(logger_, "Destroying of HandshakeHandler: " << this);
}

uint32_t HandshakeHandler::connection_key() const {
  return session_observer_.KeyFromPair(context_.connection_id_,
                                       context_.new_session_id_);
}

void HandshakeHandler::OnCertificateUpdateRequired() {}

bool HandshakeHandler::OnHandshakeFailed() {
  BsonObject params;
  if (payload_) {
    params = bson_object_from_bytes(payload_.get());
  } else {
    bson_object_initialize_default(&params);
  }
  ProcessFailedHandshake(params);
  bson_object_deinitialize(&params);
  return true;
}

bool HandshakeHandler::OnHandshakeDone(
    uint32_t connection_key,
    security_manager::SSLContext::HandshakeResult result) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (connection_key != this->connection_key()) {
    LOG4CXX_DEBUG(logger_,
                  "Listener " << this
                              << " expects notification for connection id: "
                              << this->connection_key()
                              << ". Received notification for connection id "
                              << connection_key << " will be ignored");
    return false;
  }

  const bool success =
      result == security_manager::SSLContext::Handshake_Result_Success;

  BsonObject params;
  if (payload_) {
    params = bson_object_from_bytes(payload_.get());
  } else {
    bson_object_initialize_default(&params);
  }

  if (success) {
    ProcessSuccessfulHandshake(connection_key, params);
  } else {
    ProcessFailedHandshake(params);
  }

  bson_object_deinitialize(&params);
  return true;
}

void HandshakeHandler::ProcessSuccessfulHandshake(const uint32_t connection_key,
                                                  BsonObject& params) {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::vector<int>& force_unprotected =
      protocol_handler_.get_settings().force_unprotected_service();

  const bool can_be_protected =
      std::find(force_unprotected.begin(),
                force_unprotected.end(),
                context_.service_type_) == force_unprotected.end();

  const bool is_service_already_protected =
      session_observer_.GetSSLContext(connection_key, context_.service_type_) !=
      NULL;

  LOG4CXX_DEBUG(logger_,
                "Service can be protected: " << can_be_protected
                                             << " and service was protected: "
                                             << is_service_already_protected);

  if (can_be_protected && !is_service_already_protected) {
    session_observer_.SetProtectionFlag(connection_key, context_.service_type_);
    protocol_handler_.SendStartSessionAck(context_.connection_id_,
                                          context_.new_session_id_,
                                          protocol_version_,
                                          context_.hash_id_,
                                          context_.service_type_,
                                          can_be_protected,
                                          full_version_,
                                          params);
  } else {
    protocol_handler_.SendStartSessionNAck(context_.connection_id_,
                                           context_.new_session_id_,
                                           protocol_version_,
                                           context_.service_type_);
  }
}

void HandshakeHandler::ProcessFailedHandshake(BsonObject& params) {
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
    protocol_handler_.SendStartSessionAck(context_.connection_id_,
                                          context_.new_session_id_,
                                          protocol_version_,
                                          context_.hash_id_,
                                          context_.service_type_,
                                          PROTECTION_OFF,
                                          full_version_,
                                          params);
  } else {
    protocol_handler_.SendStartSessionNAck(context_.connection_id_,
                                           context_.new_session_id_,
                                           protocol_version_,
                                           context_.service_type_);
  }
}

}  // namespace protocol_handler
