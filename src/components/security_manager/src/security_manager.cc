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

#include "security_manager/security_manager.h"
#include "security_manager/crypto_manager_impl.h"
#include "utils/byte_order.h"
#include "json/json.h"

using namespace security_manager;
static const char* err_id ="ERROR_ID";

log4cxx::LoggerPtr SecurityManager::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("SecurityManager"));

SecurityManager::SecurityManager() :
  security_messages_("SecurityManager::security_messages_", this),
  crypto_manager_(NULL), session_observer_(NULL), protocol_handler_(NULL) {
}

void SecurityManager::OnMessageReceived(
    const protocol_handler::RawMessagePtr &message) {
  LOG4CXX_INFO(logger_, "OnMessageReceived");
  if(message->service_type() != protocol_handler::kControl) {
    LOG4CXX_WARN(logger_, "Incorrect message service type of income message "
                 << message->service_type());
    return;
  }

  SecurityMessage securityMessagePtr(new SecurityQuery());
  const bool result = securityMessagePtr->ParseQuery(
        message->data(), message->data_size());
  if(!result) {
    //result will be false only if data less then query header
    LOG4CXX_ERROR(logger_, "Incorrect message received");
    SendInternalError(message->connection_key(),
                      SecurityQuery::ERROR_INVALID_QUERY_SIZE);
    return;
  }
  securityMessagePtr->set_connection_key(message->connection_key());

  // Post message to message query for next processing in thread
  security_messages_.PostMessage(securityMessagePtr);
}

void SecurityManager::OnMobileMessageSent(
    const protocol_handler::RawMessagePtr &) {
//  LOG4CXX_INFO(logger_, "OnMobileMessageSent");
}

void SecurityManager::StartHandshake(uint32_t connection_key) {
  security_manager::SSLContext* ssl_context =
      session_observer_->GetSSLContext(connection_key,
                                       protocol_handler::kControl);
  if (!ssl_context && !ProtectConnection(connection_key)) {
    LOG4CXX_ERROR(logger_, "StartHandshake failed");
    return;
  }
  ssl_context = session_observer_->GetSSLContext(connection_key,
                                                 protocol_handler::kControl);
  DCHECK(ssl_context != NULL);

  if (!ssl_context->IsInitCompleted()) {
    size_t data_size;
    const uint8_t* data =
        static_cast<uint8_t*>(ssl_context->StartHandshake(&data_size));
    DCHECK(data);
    DCHECK(data_size);
    SendBinaryData(connection_key, data, data_size);
  }
}

void SecurityManager::set_session_observer(
    protocol_handler::SessionObserver *observer) {
  if (!observer) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to SessionObserver.");
    return;
  }
  session_observer_ = observer;
}

void SecurityManager::set_protocol_handler(
    protocol_handler::ProtocolHandler *handler) {
  if (!handler) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to ProtocolHandler.");
    return;
  }
  protocol_handler_ = handler;
}

void SecurityManager::set_crypto_manager(CryptoManager *crypto_manager) {
  if (!crypto_manager) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to CryptoManager.");
    return;
  }
  crypto_manager_ = crypto_manager;
}

void SecurityManager::Handle(const SecurityMessage &message) {
  DCHECK(message);DCHECK(session_observer_);
  LOG4CXX_INFO(logger_, "Received Security message from Mobile side");
  if(!crypto_manager_)  {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) CryptoManager.");
    SendInternalError(message->get_connection_key(),
                      SecurityQuery::ERROR_NOT_SUPPORTED);
    return;
  }
  switch (message->get_header().query_id) {
    case SecurityQuery::SEND_HANDSHAKE_DATA:
      if(!ProccessHandshakeData(message)) {
        LOG4CXX_ERROR(logger_, "Proccess HandshakeData failed");
      }
      break;
    case SecurityQuery::SEND_INTERNAL_ERROR:
      if(!ProccessInternalError(message)) {
        LOG4CXX_ERROR(logger_, "Processing income InternalError failed");
      }
      break;
    default: { // SecurityQuery::InvalidQuery
      LOG4CXX_ERROR(logger_, "Unknown query identifier.");
      SendInternalError(message->get_connection_key(),
                        SecurityQuery::ERROR_INVALID_QUERY_ID,
                        message->get_header().seq_number);
      }
      break;
    }
}

bool SecurityManager::ProtectConnection(const uint32_t& connection_key) {
  LOG4CXX_INFO(logger_, "ProtectService processing");
  DCHECK(session_observer_); DCHECK(crypto_manager_);

  if(session_observer_->GetSSLContext(connection_key,
                                      protocol_handler::kControl)) {
    LOG4CXX_WARN(logger_, "Connection is already protected, key "
                 << connection_key);
    SendInternalError(connection_key,
                      SecurityQuery::ERROR_SERVICE_ALREADY_PROTECTED);
    return false;
  }

  security_manager::SSLContext * newSSLContext = crypto_manager_->CreateSSLContext();
  if(!newSSLContext) {
      LOG4CXX_ERROR(logger_, "CryptoManager could not create SSL context.");
      // Generate response query and post to security_messages_
      SendInternalError(connection_key, SecurityQuery::ERROR_CREATE_SLL);
      return false;
  }

  const int result = session_observer_->SetSSLContext(connection_key, newSSLContext);
  if(SecurityQuery::ERROR_SUCCESS != result) {
    //delete SSLContex on any error
    crypto_manager_->ReleaseSSLContext(newSSLContext);
    SendInternalError(connection_key, result);
    return false;
  }
  return true;
}

bool SecurityManager::ProccessHandshakeData(const SecurityMessage &inMessage) {
  LOG4CXX_INFO(logger_, "SendHandshakeData processing");
  DCHECK(inMessage->get_header().query_id == SecurityQuery::SEND_HANDSHAKE_DATA);
  DCHECK(inMessage); DCHECK(session_observer_);
  const uint32_t seqNumber = inMessage->get_header().seq_number;
  const uint32_t connectionKey = inMessage->get_connection_key();

  if(!inMessage->get_data_size()) {
    const std::string error("SendHandshakeData: null arguments size.");
    LOG4CXX_ERROR(logger_, error);
    SendInternalError(connectionKey,
                      SecurityQuery::ERROR_INVALID_QUERY_SIZE, seqNumber);
    return false;
  }
  SSLContext * sslContext =
      session_observer_->GetSSLContext(connectionKey,
                                       protocol_handler::kControl);
  if(!sslContext) {
    const std::string error("SendHandshakeData: No ssl context.");
    LOG4CXX_ERROR(logger_, error);
    SendInternalError(connectionKey,
                      SecurityQuery::ERROR_SERVICE_NOT_PROTECTED, seqNumber);
    return false;
  }
  size_t out_data_size;
  const uint8_t * out_data = static_cast<uint8_t *>
      (sslContext->DoHandshakeStep(
         inMessage->get_data(), inMessage->get_data_size(), &out_data_size));
  if(!out_data || !out_data_size){
    std::string error("SendHandshakeData: Handshake failed.");
    error += LastError();
    LOG4CXX_WARN(logger_, error);
    SendInternalError(connectionKey,
                      SecurityQuery::ERROR_SSL_INVALID_DATA, seqNumber);
    return false;
  }

  //answer with the same header as income message
  SendData(connectionKey, inMessage->get_header(), out_data, out_data_size);
  return true;
}

bool SecurityManager::ProccessInternalError(
    const SecurityMessage &inMessage) {
  LOG4CXX_INFO(logger_,"Recieved InternalError with Json message"
                << inMessage->get_json_message());

  Json::Value root;
  Json::Reader reader;
  const bool parsingSuccessful =
      reader.parse(inMessage->get_json_message(), root );
  if(!parsingSuccessful)
    return false;
  Json::Value value(inMessage->get_json_message());
  LOG4CXX_ERROR(logger_,"Recieved InternalError id " << value[err_id].asString());
  return true;
}

void SecurityManager::SendInternalError(const int32_t connection_key,
                                        const int &error_id,
                                        const uint32_t seq_number) {
  Json::Value value;
  value[err_id]=error_id;
  const std::string error_str = value.toStyledString();
  SecurityQuery::QueryHeader header(SecurityQuery::NOTIFICATION,
                                    SecurityQuery::SEND_INTERNAL_ERROR,
                                    seq_number, error_str.size());
  std::vector<uint8_t> data_sending(error_str.size() + 1);
  memcpy(&data_sending[0], error_str.c_str(), error_str.size());
  data_sending[data_sending.size()-1]=error_id;

  SendData(connection_key, header, &data_sending[0], data_sending.size());
}

void SecurityManager::SendData(
    const int32_t connectionKey,
    SecurityQuery::QueryHeader header,
    const uint8_t * const data, const size_t data_size) {
  // FIXME(EZ): move to SecurityQuery class
  header.query_id  = LE_TO_BE32(header.query_id << 8);
  header.json_size = LE_TO_BE32(header.json_size);

  const size_t header_size = sizeof(header);
  std::vector<uint8_t> data_sending(header_size + data_size);
  memcpy(&data_sending[0], &header, header_size);
  //TODO (EZamakhov): Fix invalid read (by Valgrind)
  memcpy(&data_sending[header_size], data, data_size);

  SendBinaryData(connectionKey, &data_sending[0], data_sending.size());
}

void SecurityManager::SendBinaryData(const int32_t connectionKey,
                                     const uint8_t * const data,
                                     size_t data_size) {
//   LOG4CXX_INFO(logger_, "SendBinaryDataData");
  DCHECK(protocol_handler_)
  const protocol_handler::RawMessagePtr rawMessagePtr(
        new protocol_handler::RawMessage(connectionKey,
                                         protocol_handler::PROTOCOL_VERSION_2,
                                         data, data_size,
                                         protocol_handler::kControl));
  // Add RawMessage to ProtocolHandler message query
  // FIXME(EZ): final_message - false?
  protocol_handler_->SendMessageToMobileApp(rawMessagePtr, false);
}


const char *SecurityManager::ConfigSection() {
  return "Security Manager";
}
