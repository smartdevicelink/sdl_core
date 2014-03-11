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
using namespace security_manager;

log4cxx::LoggerPtr SecurityManager::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("SecurityManager"));

SecurityManager::SecurityManager() :
  security_messages_("SecurityManager::security_messages_", this),
  crypto_manager_(new security_manager::CryptoManagerImpl()),
  session_observer_(NULL), protocol_handler_(NULL) {
  if(!crypto_manager_->Init()) {
      LOG4CXX_ERROR(logger_, "CryptoManager initialization fail.");
    }
}

void SecurityManager::OnMessageReceived(
    const protocol_handler::RawMessagePtr &message) {
  LOG4CXX_INFO(logger_, "SecurityManager::OnMessageReceived");
  if(message->service_type() != protocol_handler::kSecure) {
    LOG4CXX_WARN(logger_, "Incorrect message service type of income message"
                 << message->service_type());
    return;
  }

  SecurityMessage securityMessagePtr(new SecurityQuery());
  const bool result = securityMessagePtr->Parse(message->data(),
                                             message->data_size());
  if(!result) {
    //result will be false only if data less then query header
    const std::string error("Incorrect message received");
    LOG4CXX_ERROR(logger_, error);
    SendInternalError(message->connection_key(), 0,
                      SecurityQuery::ERROR_INVALID_QUERY_SIZE, error);
    return;
  }
  securityMessagePtr->setConnectionKey(message->connection_key());

  const SecurityQuery::QueryHeader &header = securityMessagePtr->getHeader();
  if(header.query_id  == SecurityQuery::INVALID_QUERY_ID) {
    const std::string error("Unknown query identifier.");
    LOG4CXX_ERROR(logger_, error);
    SendInternalError(message->connection_key(), header.seq_number,
                       SecurityQuery::ERROR_INVALID_QUERY_ID, error);
    }
  // Post message to message query for next processing in thread
  security_messages_.PostMessage(securityMessagePtr);
}

void SecurityManager::OnMobileMessageSent(
    const protocol_handler::RawMessagePtr &message) {
  LOG4CXX_INFO(logger_, "SecurityManager::OnMobileMessageSent");
  LOG4CXX_ERROR(logger_, "OnMobileMessageSent not supported in SecurityManager.");
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

void SecurityManager::Handle(const SecurityMessage &message) {
  DCHECK(message);DCHECK(session_observer_);
  LOG4CXX_INFO(logger_, "Received Security message from Mobile side");
  switch (message->getHeader().query_id) {
    case SecurityQuery::PROTECT_SERVICE_REQUEST:
      if(!ParseProtectServiceRequest(message)) {
          LOG4CXX_ERROR(logger_, "ProtectServiceRequest failed");
        }
      return;
    case SecurityQuery::SEND_HANDSHAKE_DATA:
      if(!ParseHandshakeData(message)) {
          LOG4CXX_ERROR(logger_, "SendHandshakeData failed");
        }
      return;
    case SecurityQuery::PROTECT_SERVICE_RESPONSE:
      {
      const std::string error("ProtectServiceResponse couldn't be received from mobile side.");
      LOG4CXX_ERROR(logger_, error);
      SendInternalError(message->getConnectionKey(),
                        message->getHeader().seq_number,
                        //FIXME(EZ): ERROR_OTHER_INTERNAL_ERROR or ERROR_INVALID_QUERY_ID
                        SecurityQuery::ERROR_OTHER_INTERNAL_ERROR, error);
      }
      return;
    case SecurityQuery::SEND_INTERNAL_ERROR:
      {
      const char* const error_str = reinterpret_cast<const char*>(message->getData());
      const std::string error(error_str, message->getDataSize());
      LOG4CXX_ERROR(logger_, "InternalError error received" << error);
      }
      return;
    default: // SecurityQuery::InvalidQuery
      LOG4CXX_ERROR(logger_, "Security message: unknown query");
    }
}

bool SecurityManager::ParseProtectServiceRequest(
    const SecurityMessage &requestMessage) {
  LOG4CXX_INFO(logger_, "ProtectServiceRequest processing");
  DCHECK(requestMessage); DCHECK(session_observer_);
  DCHECK(requestMessage->getHeader().query_id ==
         SecurityQuery::PROTECT_SERVICE_REQUEST);
  if(requestMessage->getDataSize() != 1) {
    LOG4CXX_ERROR(logger_, "ProtectServiceRequest wrong arguments size.");
    SendProtectServiceResponse(requestMessage,
                               SecurityQuery::INTERNAL_ERROR);
    return false;
  }
  const uint8_t service_id = *requestMessage->getData();
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceTypeFromByte(service_id);
  if(service_type == protocol_handler::kInvalidServiceType) {
      LOG4CXX_ERROR(logger_, "Invalid Service type.");
      SendProtectServiceResponse(requestMessage,
                                 SecurityQuery::SERVICE_NOT_FOUND);
      return false;
    }

  DCHECK(crypto_manager_);
  security_manager::SSLContext * const newSSLContext =
      crypto_manager_->CreateSSLContext();
  if(!newSSLContext) {
      LOG4CXX_ERROR(logger_, "CryptoManager could not create SSl context.");
      //Generate response query and post to security_messages_
      SendProtectServiceResponse(requestMessage,
                                 SecurityQuery::INTERNAL_ERROR);
      return false;
  }
  const int result = session_observer_->SetSSLContext(
        requestMessage->getConnectionKey(), service_type, newSSLContext);
  //delete SSLContex on any error
  if(SecurityQuery::SUCCESS != result)
  //FIXME(EZ): return DeleteSSLContext
//        crypto_manager_->DeleteSSLContext(newSSLContext);
    ;
  switch (result) {
    case SecurityQuery::SUCCESS:
      //Generate response query and post to secure_service_messages_
      SendProtectServiceResponse(requestMessage, SecurityQuery::SUCCESS);
      break;
    case SecurityQuery::SERVICE_ALREADY_PROTECTED:
      {
      const security_manager::SSLContext * const currentSSLContext =
          session_observer_->GetSSLContext(
            requestMessage->getConnectionKey(), service_type);
      if(currentSSLContext) {
        if(currentSSLContext->IsInitCompleted()) {
          LOG4CXX_WARN(logger_, "Service is already protected, service_id"
                       << service_id);
          SendProtectServiceResponse(requestMessage,
                                     SecurityQuery::SERVICE_ALREADY_PROTECTED);
        }
        else {
          LOG4CXX_WARN(logger_, "Secure protection is pending, service_id"
                       << service_id);
          //Generate response query and post to secure_service_messages_
          SendProtectServiceResponse(requestMessage,
                                     SecurityQuery::PENDING);
        }
      }
      }
      break;
    case SecurityQuery::SERVICE_NOT_FOUND:
      LOG4CXX_WARN(logger_, "Secure protection is pending, service_id"
                   << service_id);
      //Generate response query and post to secure_service_messages_
      SendProtectServiceResponse(requestMessage,
                                 SecurityQuery::SERVICE_NOT_FOUND);
      break;
    default: // SecurityQuery::INTERNAL_ERROR
      LOG4CXX_ERROR(logger_,
                    "SessionObserver: could not set SSL context.");
      SendProtectServiceResponse(requestMessage,
                                 SecurityQuery::INTERNAL_ERROR);
      return false;
    }
  return true;
}

bool SecurityManager::ParseHandshakeData(const SecurityMessage &inMessage) {
  LOG4CXX_INFO(logger_, "SendHandshakeData processing");
  DCHECK(inMessage->getHeader().query_id == SecurityQuery::SEND_HANDSHAKE_DATA);
  DCHECK(inMessage);
  const uint32_t seqNumber = inMessage->getHeader().seq_number;
  const uint32_t connectionKey = inMessage->getConnectionKey();

  if(inMessage->getDataSize() > 0) {
    const std::string error("SendHandshakeData: null arguments size.");
    LOG4CXX_ERROR(logger_, error);
    SendInternalError(connectionKey, seqNumber,
                      SecurityQuery::ERROR_INVALID_QUERY_SIZE, error);
    return false;
  }
  const uint8_t service_id = *inMessage->getData();
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceTypeFromByte(service_id);
  if(service_type == protocol_handler::kInvalidServiceType) {
    const std::string error("SendHandshakeData: Invalid Service Type.");
    LOG4CXX_ERROR(logger_, error);
    SendInternalError(connectionKey, seqNumber,
                      SecurityQuery::ERROR_INVALID_SERVICE_TYPE, error);
    return false;
  }
  SSLContext * sslContext =
      session_observer_->GetSSLContext(connectionKey, service_type);
  if(!sslContext) {
    const std::string error("SendHandshakeData: No ssl context.");
    LOG4CXX_ERROR(logger_, error);
    SendInternalError(connectionKey, seqNumber,
                      SecurityQuery::ERROR_PROTECTION_NOT_REQUESTED, error);
    return false;
  }
  const uint8_t * data = inMessage->getData();
  const size_t data_size = inMessage->getDataSize();
  size_t out_data_size;
  const uint8_t * out_data = static_cast<uint8_t *>
      (sslContext->DoHandshakeStep(data, data_size, &out_data_size));
  if(!out_data){
    const std::string error("SendHandshakeData: Handshake failed.");
    LOG4CXX_WARN(logger_, error + LastError());
    SendInternalError(connectionKey, seqNumber,
                      SecurityQuery::ERROR_SSL_INVALID_DATA,
                      error + LastError());
    return false;
  }
  //answer with the same header as income message
  SendData(connectionKey, inMessage->getHeader(), out_data, out_data_size);
  return true;
}

void SecurityManager::SendProtectServiceResponse(
    const SecurityMessage &requestMessage,
    const SecurityQuery::ProtectServiceResult result){
  LOG4CXX_INFO(logger_, "SecurityManager::SendProtectServiceResponse");
  DCHECK(requestMessage->getHeader().query_id==
         SecurityQuery::PROTECT_SERVICE_REQUEST);

  const SecurityQuery::QueryHeader header(
        SecurityQuery::RESPONSE,
        SecurityQuery::PROTECT_SERVICE_RESPONSE,
        requestMessage->getHeader().seq_number);

  const uint8_t data_result = result;
  SendData(requestMessage->getConnectionKey(), header, &data_result, 1);
}

void SecurityManager::SendInternalError(
    const int32_t connection_key, const uint32_t seq_number,
    const SecurityQuery::InternalErrors &error_id,
    const std::string &error_str) {
  const SecurityQuery::QueryHeader header(
        SecurityQuery::NOTIFICATION, SecurityQuery::SEND_INTERNAL_ERROR, seq_number);

  const size_t data_sending_size = error_str.size() + 1;
  uint8_t* data_sending = new uint8_t[data_sending_size];
  data_sending[0] = error_id;
  memcpy(data_sending + 1, error_str.c_str(), error_str.size());

  SendData(connection_key, header, data_sending, data_sending_size);

  delete[] data_sending;
}

void SecurityManager::SendData(
    const int32_t connectionKey,
    SecurityQuery::QueryHeader header,
    const uint8_t * const data, const size_t data_size) {
  //FIXME(EZ): move to SecurityQuery class
  header.query_id = LE_TO_BE32(header.query_id << 8);

  const size_t data_sending_size = sizeof(header) + data_size;
  uint8_t* data_sending = new uint8_t[data_sending_size];
  memcpy(data_sending, &header, sizeof(header));
  memcpy(data_sending + sizeof(header), data, data_size);

  SendBinaryData(connectionKey, data_sending, data_sending_size);

  delete[] data_sending;
}

void SecurityManager::SendBinaryData(const int32_t connectionKey,
                                     const uint8_t * const data,
                                     size_t data_size) {
  LOG4CXX_INFO(logger_, "SecurityManager::SendBinaryDataData");
  if(!protocol_handler_) {
    LOG4CXX_ERROR(logger_, "No ProtocolHandler for usage.");
    return;
  }
  const protocol_handler::RawMessagePtr rawMessagePtr(
        new protocol_handler::RawMessage( connectionKey,
                                          protocol_handler::PROTOCOL_VERSION_2,
                                          data, data_size,
                                          protocol_handler::kSecure));
  //Add RawMessage to ProtocolHandler messsage query
  //FIXME(EZ): final_message - false?
  protocol_handler_->SendMessageToMobileApp(rawMessagePtr, false);
}
