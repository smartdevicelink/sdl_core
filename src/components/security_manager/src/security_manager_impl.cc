/*
 * Copyright (c) 2014, Ford Motor Company
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
#include "security_manager/crypto_manager_impl.h"
#include "protocol_handler/protocol_packet.h"
#include "utils/logger.h"
#include "utils/byte_order.h"
#include "utils/json_utils.h"
#include "utils/convert_utils.h"

namespace security_manager {

SDL_CREATE_LOGGER("SecurityManager")

static const char* kErrId = "id";
static const char* kErrText = "text";

SecurityManagerImpl::SecurityManagerImpl()
    : security_messages_("SecurityManager", this)
    , session_observer_(NULL)
    , crypto_manager_(NULL)
    , protocol_handler_(NULL) {}

void SecurityManagerImpl::OnMessageReceived(
    const ::protocol_handler::RawMessagePtr message) {
  if (message->service_type() != protocol_handler::kControl) {
    return;
  }

  SecurityMessage securityMessagePtr(new SecurityQuery());
  const bool result =
      securityMessagePtr->SerializeQuery(message->data(), message->data_size());
  if (!result) {
    // result will be false only if data less then query header
    const std::string error_text("Incorrect message received");
    SDL_ERROR(error_text);
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
    SDL_ERROR("Invalid (NULL) pointer to SessionObserver.");
    return;
  }
  session_observer_ = observer;
}

void SecurityManagerImpl::set_protocol_handler(
    protocol_handler::ProtocolHandler* handler) {
  if (!handler) {
    SDL_ERROR("Invalid (NULL) pointer to ProtocolHandler.");
    return;
  }
  protocol_handler_ = handler;
}

void SecurityManagerImpl::set_crypto_manager(CryptoManager* crypto_manager) {
  if (!crypto_manager) {
    SDL_ERROR("Invalid (NULL) pointer to CryptoManager.");
    return;
  }
  crypto_manager_ = crypto_manager;
}

void SecurityManagerImpl::Handle(const SecurityMessage message) {
  DCHECK(message);
  SDL_INFO("Received Security message from Mobile side");
  if (!crypto_manager_) {
    const std::string error_text("Invalid (NULL) CryptoManager.");
    SDL_ERROR(error_text);
    SendInternalError(
        message->get_connection_key(), ERROR_NOT_SUPPORTED, error_text);
    return;
  }
  switch (message->get_header().query_id) {
    case SecurityQuery::SEND_HANDSHAKE_DATA:
      if (!ProccessHandshakeData(message)) {
        SDL_ERROR("Proccess HandshakeData failed");
      }
      break;
    case SecurityQuery::SEND_INTERNAL_ERROR:
      if (!ProccessInternalError(message)) {
        SDL_ERROR("Processing income InternalError failed");
      }
      break;
    default: {
      // SecurityQuery::InvalidQuery
      const std::string error_text("Unknown query identifier.");
      SDL_ERROR(error_text);
      SendInternalError(message->get_connection_key(),
                        ERROR_INVALID_QUERY_ID,
                        error_text,
                        message->get_header().seq_number);
    } break;
  }
}

security_manager::SSLContext* SecurityManagerImpl::CreateSSLContext(
    const uint32_t& connection_key) {
  SDL_INFO("ProtectService processing");
  DCHECK(session_observer_);
  DCHECK(crypto_manager_);

  security_manager::SSLContext* ssl_context = session_observer_->GetSSLContext(
      connection_key, protocol_handler::kControl);
  // return exists SSLCOntext for current connection/session
  if (ssl_context) {
    return ssl_context;
  }

  ssl_context = crypto_manager_->CreateSSLContext();
  if (!ssl_context) {
    const std::string error_text("CryptoManager could not create SSL context.");
    SDL_ERROR(error_text);
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
  SDL_DEBUG("Set SSL context to connection_key " << connection_key);
  return ssl_context;
}

void SecurityManagerImpl::StartHandshake(uint32_t connection_key) {
  DCHECK(session_observer_);
  SDL_INFO("StartHandshake: connection_key " << connection_key);
  security_manager::SSLContext* ssl_context = session_observer_->GetSSLContext(
      connection_key, protocol_handler::kControl);
  if (!ssl_context) {
    const std::string error_text(
        "StartHandshake failed, "
        "connection is not protected");
    SDL_ERROR(error_text);
    SendInternalError(connection_key, ERROR_INTERNAL, error_text);
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Fail);
    return;
  }

  if (crypto_manager_->IsCertificateUpdateRequired()) {
    NotifyOnCertififcateUpdateRequired();
  }

  if (ssl_context->IsInitCompleted()) {
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Success);
    return;
  }

  ssl_context->SetHandshakeContext(
      session_observer_->GetHandshakeContext(connection_key));

  size_t data_size = 0;
  const uint8_t* data = NULL;

  const security_manager::SSLContext::HandshakeResult result =
      ssl_context->StartHandshake(&data, &data_size);
  if (security_manager::SSLContext::Handshake_Result_Success != result) {
    const std::string error_text("StartHandshake failed, handshake step fail");
    SDL_ERROR(error_text);
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
void SecurityManagerImpl::AddListener(SecurityManagerListener* const listener) {
  if (!listener) {
    SDL_ERROR("Invalid (NULL) pointer to SecurityManagerListener.");
    return;
  }
  listeners_.push_back(listener);
}
void SecurityManagerImpl::RemoveListener(
    SecurityManagerListener* const listener) {
  if (!listener) {
    SDL_ERROR("Invalid (NULL) pointer to SecurityManagerListener.");
    return;
  }
  listeners_.remove(listener);
}
void SecurityManagerImpl::NotifyListenersOnHandshakeDone(
    const uint32_t& connection_key, SSLContext::HandshakeResult error) {
  SDL_AUTO_TRACE();
  std::list<SecurityManagerListener*>::iterator it = listeners_.begin();
  while (it != listeners_.end()) {
    if ((*it)->OnHandshakeDone(connection_key, error)) {
      // On get notification remove listener
      it = listeners_.erase(it);
    } else {
      ++it;
    }
  }
}

void SecurityManagerImpl::NotifyOnCertififcateUpdateRequired() {
  SDL_AUTO_TRACE();
  std::list<SecurityManagerListener*>::iterator it = listeners_.begin();
  while (it != listeners_.end()) {
    (*it)->OnCertificateUpdateRequired();
    ++it;
  }
}

bool SecurityManagerImpl::ProccessHandshakeData(
    const SecurityMessage& inMessage) {
  SDL_INFO("SendHandshakeData processing");
  DCHECK(inMessage);
  DCHECK(inMessage->get_header().query_id ==
         SecurityQuery::SEND_HANDSHAKE_DATA);
  const uint32_t seqNumber = inMessage->get_header().seq_number;
  const uint32_t connection_key = inMessage->get_connection_key();

  SDL_DEBUG("Received " << inMessage->get_data_size()
                        << " bytes handshake data ");

  if (!inMessage->get_data_size()) {
    const std::string error_text("SendHandshakeData: null arguments size.");
    SDL_ERROR(error_text);
    SendInternalError(
        connection_key, ERROR_INVALID_QUERY_SIZE, error_text, seqNumber);
    return false;
  }
  DCHECK(session_observer_);
  SSLContext* sslContext = session_observer_->GetSSLContext(
      connection_key, protocol_handler::kControl);
  if (!sslContext) {
    const std::string error_text("SendHandshakeData: No ssl context.");
    SDL_ERROR(error_text);
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
    const std::string erorr_text(sslContext->LastError());
    SDL_ERROR("SendHandshakeData: Handshake failed: " << erorr_text);
    SendInternalError(
        connection_key, ERROR_SSL_INVALID_DATA, erorr_text, seqNumber);
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Fail);
    // no handshake data to send
    return false;
  }
  if (sslContext->IsInitCompleted()) {
    // On handshake success
    SDL_DEBUG("SSL initialization finished success.");
    NotifyListenersOnHandshakeDone(connection_key,
                                   SSLContext::Handshake_Result_Success);
  } else if (handshake_result != SSLContext::Handshake_Result_Success) {
    // On handshake fail
    SDL_WARN("SSL initialization finished with fail.");
    NotifyListenersOnHandshakeDone(connection_key, handshake_result);
  }

  if (out_data && out_data_size) {
    // answer with the same seqNumber as income message
    SendHandshakeBinData(connection_key, out_data, out_data_size, seqNumber);
  }
  return true;
}

bool SecurityManagerImpl::ProccessInternalError(
    const SecurityMessage& inMessage) {
  std::string json_message = inMessage->get_json_message();
  SDL_INFO("Received InternalError with Json message" << json_message);
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(json_message);
  if (!parse_result.second) {
    return false;
  }
#if defined(ENABLE_LOG)
  JsonValue& root_json = parse_result.first;
#endif
  SDL_DEBUG("Received InternalError id "
            << root_json[kErrId].AsString()
            << ", text: " << root_json[kErrText].AsString());
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
  SDL_DEBUG("Sent " << data_size << " bytes handshake data ");
}

void SecurityManagerImpl::SendInternalError(const uint32_t connection_key,
                                            const uint8_t& error_id,
                                            const std::string& erorr_text,
                                            const uint32_t seq_number) {
  using namespace utils::json;
  JsonValue value;
  value[kErrId] = utils::ConvertUInt64ToLongLongUInt(error_id);
  value[kErrText] = erorr_text;
  const std::string error_str = value.ToJson();
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
  SDL_DEBUG("Sent Internal error id " << static_cast<int>(error_id) << " : \""
                                      << erorr_text << "\".");
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
                                         protocol_handler::kControl));
    DCHECK(protocol_handler_);
    // Add RawMessage to ProtocolHandler message query
    protocol_handler_->SendMessageToMobileApp(rawMessagePtr, false);
  }
}

const char* SecurityManagerImpl::ConfigSection() {
  return "Security Manager";
}

}  // namespace security_manager
