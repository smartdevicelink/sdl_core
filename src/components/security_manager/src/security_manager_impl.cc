/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "security_manager/security_manager_impl.h"
#include <functional>
#include "json/json.h"
#include "protocol_handler/protocol_packet.h"
#include "security_manager/crypto_manager_impl.h"
#include "utils/byte_order.h"
#include "utils/helpers.h"
#include "utils/jsoncpp_reader_wrapper.h"
#include "utils/logger.h"

namespace security_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "SecurityManager")

static const char* kErrId = "id";
static const char* kErrText = "text";

SecurityManagerImpl::SecurityManagerImpl(
    std::unique_ptr<utils::SystemTimeHandler>&& system_time_handler)
    : security_messages_("SecurityManager", this)
    , session_observer_(NULL)
    , crypto_manager_(NULL)
    , protocol_handler_(NULL)
    , system_time_handler_(std::move(system_time_handler))
    , waiting_for_certificate_(false)
    , waiting_for_time_(false) {
  DCHECK(system_time_handler_);
  system_time_handler_->SubscribeOnSystemTime(this);
}

SecurityManagerImpl::~SecurityManagerImpl() {
  system_time_handler_->UnsubscribeFromSystemTime(this);
}

void SecurityManagerImpl::OnMessageReceived(
    const ::protocol_handler::RawMessagePtr message) {
  if (message->service_type() != protocol_handler::kControl) {
    return;
  }

  SecurityMessage securityMessagePtr(std::make_shared<SecurityQuery>());
  const bool result =
      securityMessagePtr->SerializeQuery(message->data(), message->data_size());
  if (!result) {
    // result will be false only if data less then query header
    const std::string error_text("Incorrect message received");
    LOG4CXX_ERROR(logger_, error_text);
    SendInternalError(
        message->connection_key(), ERROR_INVALID_QUERY_SIZE, error_text);
    return;
  }
  securityMessagePtr->set_connection_key(message->connection_key());

  // Post message to message query for next processing in thread
  security_messages_.PostMessage(securityMessagePtr);
}

void SecurityManagerImpl::OnMobileMessageSent(
    const ::protocol_handler::RawMessagePtr) {}

void SecurityManagerImpl::set_session_observer(
    protocol_handler::SessionObserver* observer) {
  if (!observer) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to SessionObserver.");
    return;
  }
  session_observer_ = observer;
}

void SecurityManagerImpl::set_protocol_handler(
    protocol_handler::ProtocolHandler* handler) {
  if (!handler) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to ProtocolHandler.");
    return;
  }
  protocol_handler_ = handler;
}

void SecurityManagerImpl::set_crypto_manager(CryptoManager* crypto_manager) {
  if (!crypto_manager) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to CryptoManager.");
    return;
  }
  crypto_manager_ = crypto_manager;
}

void SecurityManagerImpl::Handle(const SecurityMessage message) {
  DCHECK(message);
  LOG4CXX_INFO(logger_, "Received Security message from Mobile side");
  if (!crypto_manager_) {
    const std::string error_text("Invalid (NULL) CryptoManager.");
    LOG4CXX_ERROR(logger_, error_text);
    SendInternalError(
        message->get_connection_key(), ERROR_NOT_SUPPORTED, error_text);
    return;
  }
  switch (message->get_header().query_id) {
    case SecurityQuery::SEND_HANDSHAKE_DATA:
      if (!ProcessHandshakeData(message)) {
        LOG4CXX_ERROR(logger_, "Process HandshakeData failed");
      }
      break;
    case SecurityQuery::SEND_INTERNAL_ERROR:
      if (!ProcessInternalError(message)) {
        LOG4CXX_ERROR(logger_, "Processing income InternalError failed");
      }
      break;
    default: {
      // SecurityQuery::InvalidQuery
      const std::string error_text("Unknown query identifier.");
      LOG4CXX_ERROR(logger_, error_text);
      SendInternalError(message->get_connection_key(),
                        ERROR_INVALID_QUERY_ID,
                        error_text,
                        message->get_header().seq_number);
    } break;
  }
}

security_manager::SSLContext* SecurityManagerImpl::CreateSSLContext(
    const uint32_t& connection_key, ContextCreationStrategy cc_strategy) {
  LOG4CXX_INFO(logger_, "ProtectService processing");
  DCHECK(session_observer_);
  DCHECK(crypto_manager_);

  if (kUseExisting == cc_strategy) {
    security_manager::SSLContext* ssl_context =
        session_observer_->GetSSLContext(connection_key,
                                         protocol_handler::kControl);
    // If SSLContext for current connection/session exists - return it
    if (ssl_context) {
      return ssl_context;
    }
  }

  security_manager::SSLContext* ssl_context =
      crypto_manager_->CreateSSLContext();
  if (!ssl_context) {
    const std::string error_text("CryptoManager could not create SSL context.");
    LOG4CXX_ERROR(logger_, error_text);
    // Generate response query and post to security_messages_
    SendInternalError(connection_key, ERROR_INTERNAL, error_text);
    return NULL;
  }

  const int result =
      session_observer_->SetSSLContext(connection_key, ssl_context);
  if (ERROR_SUCCESS != result) {
    // delete SSLContext on any error
    crypto_manager_->ReleaseSSLContext(ssl_context);
    SendInternalError(connection_key, result, "");
    return NULL;
  }
  DCHECK(session_observer_->GetSSLContext(connection_key,
                                          protocol_handler::kControl));
  LOG4CXX_DEBUG(logger_,
                "Set SSL context to connection_key " << connection_key);
  return ssl_context;
}

void SecurityManagerImpl::PostponeHandshake(const uint32_t connection_key) {
  LOG4CXX_TRACE(logger_, "Handshake postponed");
  sync_primitives::AutoLock lock(connections_lock_);
  if (waiting_for_certificate_) {
    awaiting_certificate_connections_.insert(connection_key);
  }
  if (waiting_for_time_) {
    awaiting_time_connections_.insert(connection_key);
  }
}

void SecurityManagerImpl::ResumeHandshake(uint32_t connection_key) {
  LOG4CXX_TRACE(logger_, "Handshake resumed");

  security_manager::SSLContext* ssl_context =
      CreateSSLContext(connection_key, kForceRecreation);

  if (!ssl_context) {
    LOG4CXX_WARN(logger_,
                 "Unable to resume handshake. No SSL context for key "
                     << connection_key);
    return;
  }

  LOG4CXX_DEBUG(logger_,
                "Connection key : "
                    << connection_key
                    << " is waiting for certificate: " << std::boolalpha
                    << waiting_for_certificate_ << " and has certificate: "
                    << ssl_context->HasCertificate());

  ssl_context->ResetConnection();
  if (!waiting_for_certificate_ && !ssl_context->HasCertificate()) {
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Fail);
    return;
  }

  ProceedHandshake(ssl_context, connection_key);
}

void SecurityManagerImpl::StartHandshake(uint32_t connection_key) {
  DCHECK(session_observer_);
  LOG4CXX_INFO(logger_, "StartHandshake: connection_key " << connection_key);
  security_manager::SSLContext* ssl_context = session_observer_->GetSSLContext(
      connection_key, protocol_handler::kControl);

  if (!ssl_context) {
    const std::string error_text(
        "StartHandshake failed, "
        "connection is not protected");
    LOG4CXX_ERROR(logger_, error_text);
    SendInternalError(connection_key, ERROR_INTERNAL, error_text);
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Fail);
    return;
  }
  if (!ssl_context->HasCertificate()) {
    LOG4CXX_ERROR(logger_, "Security certificate is absent");
    sync_primitives::AutoLock lock(waiters_lock_);
    waiting_for_certificate_ = true;
    NotifyOnCertificateUpdateRequired();
  }

  {
    sync_primitives::AutoLock lock(waiters_lock_);
    waiting_for_time_ = true;
  }

  PostponeHandshake(connection_key);
  system_time_handler_->QuerySystemTime();
}

bool SecurityManagerImpl::IsSystemTimeProviderReady() const {
  return system_time_handler_->system_time_can_be_received();
}

void SecurityManagerImpl::ProceedHandshake(
    security_manager::SSLContext* ssl_context, uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!ssl_context) {
    LOG4CXX_WARN(logger_,
                 "Unable to process handshake. No SSL context for key "
                     << connection_key);
    return;
  }

  if (ssl_context->IsInitCompleted()) {
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Success);
    return;
  }

  time_t cert_due_date;
  if (!ssl_context->GetCertificateDueDate(cert_due_date)) {
    LOG4CXX_ERROR(logger_, "Failed to get certificate due date!");
    PostponeHandshake(connection_key);
    return;
  }

  if (crypto_manager_->IsCertificateUpdateRequired(
          system_time_handler_->GetUTCTime(), cert_due_date)) {
    LOG4CXX_DEBUG(logger_, "Host certificate update required");
    if (helpers::in_range(awaiting_certificate_connections_, connection_key)) {
      NotifyListenersOnHandshakeDone(connection_key,
                                     SSLContext::Handshake_Result_CertExpired);
      return;
    }
    {
      sync_primitives::AutoLock lock(waiters_lock_);
      waiting_for_certificate_ = true;
    }
    PostponeHandshake(connection_key);
    NotifyOnCertificateUpdateRequired();
    return;
  }

  SSLContext::HandshakeContext handshake_context =
      session_observer_->GetHandshakeContext(connection_key);
  handshake_context.system_time = system_time_handler_->GetUTCTime();
  ssl_context->SetHandshakeContext(handshake_context);

  size_t data_size = 0;
  const uint8_t* data = NULL;

  const security_manager::SSLContext::HandshakeResult result =
      ssl_context->StartHandshake(&data, &data_size);
  if (security_manager::SSLContext::Handshake_Result_Success != result) {
    const std::string error_text("StartHandshake failed, handshake step fail");
    LOG4CXX_ERROR(logger_, error_text);
    SendInternalError(connection_key, ERROR_INTERNAL, error_text);
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Fail);
    return;
  }
  // for client mode will be generated output data
  if (data != NULL && data_size != 0) {
    SendHandshakeBinData(connection_key, data, data_size);
  }
}

bool SecurityManagerImpl::IsCertificateUpdateRequired(
    const uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  security_manager::SSLContext* ssl_context =
      CreateSSLContext(connection_key, kUseExisting);
  DCHECK_OR_RETURN(ssl_context, true);
  LOG4CXX_DEBUG(logger_,
                "Set SSL context to connection_key " << connection_key);
  time_t cert_due_date;
  if (!ssl_context->GetCertificateDueDate(cert_due_date)) {
    LOG4CXX_ERROR(logger_, "Failed to get certificate due date!");
    return true;
  }
  return crypto_manager_->IsCertificateUpdateRequired(
      system_time_handler_->GetUTCTime(), cert_due_date);
}

void SecurityManagerImpl::AddListener(SecurityManagerListener* const listener) {
  if (!listener) {
    LOG4CXX_ERROR(logger_,
                  "Invalid (NULL) pointer to SecurityManagerListener.");
    return;
  }
  listeners_.push_back(listener);
}

void SecurityManagerImpl::RemoveListener(
    SecurityManagerListener* const listener) {
  if (!listener) {
    LOG4CXX_ERROR(logger_,
                  "Invalid (NULL) pointer to SecurityManagerListener.");
    return;
  }
  listeners_.remove(listener);
}

bool SecurityManagerImpl::OnCertificateUpdated(const std::string& data) {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock lock(waiters_lock_);
    waiting_for_certificate_ = false;
  }
  crypto_manager_->OnCertificateUpdated(data);
  std::for_each(
      awaiting_certificate_connections_.begin(),
      awaiting_certificate_connections_.end(),
      std::bind1st(std::mem_fun(&SecurityManagerImpl::ResumeHandshake), this));

  awaiting_certificate_connections_.clear();
  return true;
}

void SecurityManagerImpl::OnSystemTimeArrived(const time_t utc_time) {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock lock(waiters_lock_);
    waiting_for_time_ = false;
  }

  std::for_each(
      awaiting_time_connections_.begin(),
      awaiting_time_connections_.end(),
      std::bind1st(std::mem_fun(&SecurityManagerImpl::ResumeHandshake), this));

  awaiting_time_connections_.clear();
}

void SecurityManagerImpl::OnSystemTimeFailed() {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock lock(waiters_lock_);
    waiting_for_time_ = false;
  }

  NotifyListenersOnGetSystemTimeFailed();

  awaiting_time_connections_.clear();
}

void SecurityManagerImpl::ProcessFailedPTU() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (listeners_.empty()) {
    LOG4CXX_DEBUG(logger_, "listeners arrays IS EMPTY!");
    return;
  }

  std::list<SecurityManagerListener*> listeners_to_remove;
  for (auto listener : listeners_) {
    if (listener->OnPTUFailed()) {
      listeners_to_remove.push_back(listener);
    }
  }

  for (auto& listener : listeners_to_remove) {
    auto it = std::find(listeners_.begin(), listeners_.end(), listener);
    DCHECK(it != listeners_.end());
    LOG4CXX_DEBUG(logger_, "Destroying listener: " << *it);
    delete (*it);
    listeners_.erase(it);
  }
}

#ifdef EXTERNAL_PROPRIETARY_MODE
void SecurityManagerImpl::ProcessFailedCertDecrypt() {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock lock(waiters_lock_);
    waiting_for_certificate_ = false;
  }

  std::list<SecurityManagerListener*> listeners_to_remove;
  for (auto listener : listeners_) {
    if (listener->OnCertDecryptFailed()) {
      listeners_to_remove.push_back(listener);
    }
  }

  for (auto& listener : listeners_to_remove) {
    auto it = std::find(listeners_.begin(), listeners_.end(), listener);
    DCHECK(it != listeners_.end());
    LOG4CXX_DEBUG(logger_, "Destroying listener: " << *it);
    delete (*it);
    listeners_.erase(it);
  }

  awaiting_certificate_connections_.clear();
}
#endif

void SecurityManagerImpl::NotifyListenersOnHandshakeDone(
    const uint32_t& connection_key, SSLContext::HandshakeResult error) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::list<SecurityManagerListener*>::iterator it = listeners_.begin();
  while (it != listeners_.end()) {
    if ((*it)->OnHandshakeDone(connection_key, error)) {
      LOG4CXX_DEBUG(logger_, "Destroying listener: " << *it);
      delete (*it);
      it = listeners_.erase(it);
    } else {
      ++it;
    }
  }
}

void SecurityManagerImpl::NotifyOnCertificateUpdateRequired() {
  LOG4CXX_AUTO_TRACE(logger_);
  std::list<SecurityManagerListener*>::iterator it = listeners_.begin();
  while (it != listeners_.end()) {
    (*it)->OnCertificateUpdateRequired();
    ++it;
  }
}

void SecurityManagerImpl::ResetPendingSystemTimeRequests() {
  system_time_handler_->ResetPendingSystemTimeRequests();
}

void SecurityManagerImpl::NotifyListenersOnGetSystemTimeFailed() {
  LOG4CXX_AUTO_TRACE(logger_);
  std::list<SecurityManagerListener*>::iterator it = listeners_.begin();
  while (it != listeners_.end()) {
    if ((*it)->OnGetSystemTimeFailed()) {
      LOG4CXX_DEBUG(logger_, "Destroying listener: " << *it);
      delete (*it);
      it = listeners_.erase(it);
    } else {
      ++it;
    }
  }
}

bool SecurityManagerImpl::IsPolicyCertificateDataEmpty() {
  LOG4CXX_AUTO_TRACE(logger_);

  std::string certificate_data;
  for (auto it = listeners_.begin(); it != listeners_.end(); ++it) {
    if ((*it)->GetPolicyCertificateData(certificate_data)) {
      LOG4CXX_DEBUG(logger_, "Certificate data received from listener");
      return certificate_data.empty();
    }
  }
  return false;
}

bool SecurityManagerImpl::ProcessHandshakeData(
    const SecurityMessage& inMessage) {
  LOG4CXX_INFO(logger_, "SendHandshakeData processing");
  DCHECK(inMessage);
  DCHECK(inMessage->get_header().query_id ==
         SecurityQuery::SEND_HANDSHAKE_DATA);
  const uint32_t seqNumber = inMessage->get_header().seq_number;
  const uint32_t connection_key = inMessage->get_connection_key();

  LOG4CXX_DEBUG(
      logger_,
      "Received " << inMessage->get_data_size() << " bytes handshake data ");

  if (!inMessage->get_data_size()) {
    const std::string error_text("SendHandshakeData: null arguments size.");
    LOG4CXX_ERROR(logger_, error_text);
    SendInternalError(
        connection_key, ERROR_INVALID_QUERY_SIZE, error_text, seqNumber);
    return false;
  }
  DCHECK(session_observer_);
  SSLContext* sslContext = session_observer_->GetSSLContext(
      connection_key, protocol_handler::kControl);
  if (!sslContext) {
    const std::string error_text("SendHandshakeData: No ssl context.");
    LOG4CXX_ERROR(logger_, error_text);
    SendInternalError(
        connection_key, ERROR_SERVICE_NOT_PROTECTED, error_text, seqNumber);
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Fail);
    return false;
  }
  size_t out_data_size;
  const uint8_t* out_data;
  const SSLContext::HandshakeResult handshake_result =
      sslContext->DoHandshakeStep(inMessage->get_data(),
                                  inMessage->get_data_size(),
                                  &out_data,
                                  &out_data_size);
  if (handshake_result == SSLContext::Handshake_Result_AbnormalFail) {
    // Do not return handshake data on AbnormalFail or null returned values
    const std::string error_text(sslContext->LastError());
    LOG4CXX_ERROR(logger_,
                  "SendHandshakeData: Handshake failed: " << error_text);
    SendInternalError(
        connection_key, ERROR_SSL_INVALID_DATA, error_text, seqNumber);
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Fail);
    // no handshake data to send
    return false;
  }
  if (sslContext->IsInitCompleted()) {
    // On handshake success
    LOG4CXX_DEBUG(logger_, "SSL initialization finished success.");
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Success);
  } else if (handshake_result != SSLContext::Handshake_Result_Success) {
    // On handshake fail
    LOG4CXX_WARN(logger_, "SSL initialization finished with fail.");
    NotifyListenersOnHandshakeDone(connection_key, handshake_result);
  }

  if (out_data && out_data_size) {
    // answer with the same seqNumber as income message
    SendHandshakeBinData(connection_key, out_data, out_data_size, seqNumber);
  }
  return true;
}

bool SecurityManagerImpl::ProcessInternalError(
    const SecurityMessage& inMessage) {
  std::string str = inMessage->get_json_message();
  const uint32_t connection_key = inMessage->get_connection_key();
  LOG4CXX_INFO(logger_, "Received InternalError with Json message" << str);
  Json::Value root;
  utils::JsonReader reader;

  if (!reader.parse(str, &root)) {
    LOG4CXX_DEBUG(logger_, "Json parsing fails.");
    return false;
  }
  uint8_t id = root[kErrId].asInt();
  LOG4CXX_DEBUG(logger_,
                "Received InternalError id " << std::to_string(id) << ", text: "
                                             << root[kErrText].asString());
  if (ERROR_SSL_INVALID_DATA == id || ERROR_NOT_SUPPORTED == id) {
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Fail);
  }
  return true;
}

void SecurityManagerImpl::SendHandshakeBinData(const uint32_t connection_key,
                                               const uint8_t* const data,
                                               const size_t data_size,
                                               const uint32_t seq_number) {
  const SecurityQuery::QueryHeader header(SecurityQuery::NOTIFICATION,
                                          SecurityQuery::SEND_HANDSHAKE_DATA,
                                          seq_number);
  DCHECK(data_size < 1024 * 1024 * 1024);
  const SecurityQuery query =
      SecurityQuery(header, connection_key, data, data_size);
  SendQuery(query, connection_key);
  LOG4CXX_DEBUG(logger_, "Sent " << data_size << " bytes handshake data ");
}

void SecurityManagerImpl::SendInternalError(const uint32_t connection_key,
                                            const uint8_t& error_id,
                                            const std::string& error_text,
                                            const uint32_t seq_number) {
  Json::Value value;
  value[kErrId] = error_id;
  value[kErrText] = error_text;
  const std::string error_str = value.toStyledString();
  SecurityQuery::QueryHeader header(
      SecurityQuery::NOTIFICATION,
      SecurityQuery::SEND_INTERNAL_ERROR,
      // header save json size only (exclude last byte)
      seq_number,
      error_str.size());

  // Raw data is json string and error id at last byte
  std::vector<uint8_t> data_sending(error_str.size() + 1);
  memcpy(&data_sending[0], error_str.c_str(), error_str.size());
  data_sending[data_sending.size() - 1] = error_id;

  const SecurityQuery query(
      header, connection_key, &data_sending[0], data_sending.size());
  SendQuery(query, connection_key);
  LOG4CXX_DEBUG(logger_,
                "Sent Internal error id " << static_cast<int>(error_id)
                                          << " : \"" << error_text << "\".");
}

void SecurityManagerImpl::SendQuery(const SecurityQuery& query,
                                    const uint32_t connection_key) {
  const std::vector<uint8_t> data_sending = query.DeserializeQuery();
  uint32_t connection_handle = 0;
  uint8_t sessionID = 0;
  uint8_t protocol_version;
  session_observer_->PairFromKey(
      connection_key, &connection_handle, &sessionID);
  if (session_observer_->ProtocolVersionUsed(
          connection_handle, sessionID, protocol_version)) {
    const ::protocol_handler::RawMessagePtr rawMessagePtr(
        new protocol_handler::RawMessage(connection_key,
                                         protocol_version,
                                         &data_sending[0],
                                         data_sending.size(),
                                         false,
                                         protocol_handler::kControl));
    DCHECK(protocol_handler_);
    // Add RawMessage to ProtocolHandler message query
    protocol_handler_->SendMessageToMobileApp(rawMessagePtr, false, false);
  }
}

const char* SecurityManagerImpl::ConfigSection() {
  return "Security Manager";
}

}  // namespace security_manager
