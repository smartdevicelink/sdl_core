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

#include "secure_service_manager/secure_service_manager.h"
#include "secure_service_manager/crypto_manager_impl.h"
using namespace secure_service_manager;

log4cxx::LoggerPtr SecureServiceManager::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("SecureServiceManager"));

SecureServiceManager::SecureServiceManager() :
  secure_service_messages_("SecureServiceManager::secure_service_messages_", this),
  crypto_manager_(new secure_service_manager::CryptoManagerImpl()),
  session_observer_(NULL), protocol_handler_(NULL) {
  if(!crypto_manager_->Init()) {
      LOG4CXX_ERROR(logger_, "CryptoManager initialization fail.");
    }
}

void SecureServiceManager::OnMessageReceived(
    const protocol_handler::RawMessagePtr &message) {
  LOG4CXX_INFO(logger_, "SecureServiceManager::OnMessageReceived");
  if(message->service_type() != protocol_handler::kSecure) {
    LOG4CXX_WARN(logger_, "Incorrect message service type "
                 << message->service_type());
    return;
  }

  SecureServiceMessage serviceQueryPtr(new SecureServiceQuery());
  const bool result = serviceQueryPtr->Parse(message->data(),
                                             message->data_size());
  if(!result) {
    //result will be false only if data less then query header
    const std::string error("Incorrect message received");
    LOG4CXX_ERROR(logger_, error);
    SendInternalError(message->connection_key(), 0, error);
    return;
  }
  serviceQueryPtr->setConnectionKey(message->connection_key());

  const SecureServiceQuery::QueryHeader &header = serviceQueryPtr->getHeader();
  if(header.query_id_ == SecureServiceQuery::InvalidQuery) {
    const std::string error("Unknown query identifier.");
    LOG4CXX_ERROR(logger_, error);
    SendInternalError(message->connection_key(), header.seq_number_, error);
    }
  //Post message to messqge query for next processong in thread
  secure_service_messages_.PostMessage(serviceQueryPtr);
}

void SecureServiceManager::OnMobileMessageSent(
    const protocol_handler::RawMessagePtr &message) {
  LOG4CXX_INFO(logger_, "SecureServiceManager::OnMobileMessageSent");
  if(!protocol_handler_) {
    LOG4CXX_ERROR(logger_, "No ProtocolHandler for usage.");
    return;
  }
  //FIXME(EZ): final_message - true / false?
  protocol_handler_->SendMessageToMobileApp(message, false);
}

void SecureServiceManager::set_session_observer(
    protocol_handler::SessionObserver *observer) {
  if (!observer) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to SessionObserver.");
    return;
  }
  session_observer_ = observer;
}

void SecureServiceManager::set_protocol_handler(
    protocol_handler::ProtocolHandler *handler) {
  if (!handler) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to ProtocolHandler.");
    return;
  }
  protocol_handler_ = handler;
}

void SecureServiceManager::Handle(const SecureServiceMessage &message) {
  DCHECK(message);DCHECK(session_observer_);
  LOG4CXX_INFO(logger_, "Received Secure Service message from Mobile side");
  switch (message->getHeader().query_id_) {
    case SecureServiceQuery::ProtectServiceRequest:
      if(!ParseProtectServiceRequest(message)) {
          LOG4CXX_ERROR(logger_, "ProtectServiceRequest failed");
        }
      return;
    case SecureServiceQuery::SendHandshakeData:
      if(!ParseHandshakeData(message)) {
          LOG4CXX_ERROR(logger_, "SendHandshakeData failed");
        }
      return;
    case SecureServiceQuery::ProtectServiceResponse:
      {
      const std::string error("ProtectServiceResponse couldn't be received from mobile side.");
      LOG4CXX_ERROR(logger_, error);
      SendInternalError(message->getConnectionKey(),
                        message->getHeader().seq_number_, error);
      }
      return;
    case SecureServiceQuery::InternalError:
      {
      const char* const error_str = reinterpret_cast<const char*>(message->getData());
      const std::string error(error_str, message->getDataSize());
      LOG4CXX_ERROR(logger_, "InternalError error received" << error);
      }
      return;
    default: // SecureServiceQuery::InvalidQuery
      LOG4CXX_ERROR(logger_, "Secure Service message: unknown query");
    }
}

bool SecureServiceManager::ParseProtectServiceRequest(
    const SecureServiceMessage &requestMessage) {
  LOG4CXX_INFO(logger_, "ProtectServiceRequest processing");
  DCHECK(requestMessage); DCHECK(session_observer_);
  DCHECK(requestMessage->getHeader().query_id_ ==
         SecureServiceQuery::ProtectServiceRequest);
  if(requestMessage->getDataSize() != 1) {
    LOG4CXX_ERROR(logger_, "ProtectServiceRequest wrong arguments size.");
    SendProtectServiceResponse(requestMessage,
                               SecureServiceQuery::INTERNAL_ERROR);
    return false;
  }
  const uint8_t service_id = *requestMessage->getData();
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceTypeFromByte(service_id);
  if(service_type == protocol_handler::kInvalidServiceType) {
      LOG4CXX_ERROR(logger_, "Invalid Service type.");
      SendProtectServiceResponse(requestMessage,
                                 SecureServiceQuery::INTERNAL_ERROR);
      return false;
    }

  const secure_service_manager::SSLContext * const currentSSLContext =
      session_observer_->GetSSLContext(
        requestMessage->getConnectionKey(), service_type);
  if(currentSSLContext) {
    if(currentSSLContext->IsInitCompleted()) {
      LOG4CXX_WARN(logger_, "Service is already protected, service_id"
                   << service_id);
      SendProtectServiceResponse(requestMessage,
                                 SecureServiceQuery::SERVICE_ALREADY_PROTECTED);
      }
    else {
      LOG4CXX_WARN(logger_, "Secure protection is pending, service_id"
                   << service_id);
      //Generate response query and post to secure_service_messages_
      SendProtectServiceResponse(requestMessage,
                                 SecureServiceQuery::PENDING);
      }
    return false;
  }

  secure_service_manager::SSLContext * const newSSLContext =
      crypto_manager_->CreateSSLContext();
  if(!newSSLContext) {
      LOG4CXX_ERROR(logger_, "CryptoManager could not create SSl context.");
      //Generate response query and post to secure_service_messages_
      SendProtectServiceResponse(requestMessage,
                                 SecureServiceQuery::INTERNAL_ERROR);
      return false;
    }
  const bool result = session_observer_->SetSSLContext(
        requestMessage->getConnectionKey(), service_type, newSSLContext);
  if(!result) {
      LOG4CXX_ERROR(logger_,
                    "SessionObserver: could not set SSL context.");
      //Generate response query and post to secure_service_messages_
      SendProtectServiceResponse(requestMessage,
                                 SecureServiceQuery::SERVICE_ALREADY_PROTECTED);
      return false;
    }
  //Generate response query and post to secure_service_messages_
  SendProtectServiceResponse(requestMessage, SecureServiceQuery::SUCCESS);
  return true;
}

bool SecureServiceManager::ParseHandshakeData(const SecureServiceMessage &inMessage) {
  LOG4CXX_INFO(logger_, "SendHandshakeData processing");
  DCHECK(inMessage->getHeader().query_id_ == SecureServiceQuery::SendHandshakeData);
  DCHECK(inMessage);
  if(inMessage->getDataSize() > 0) {
      LOG4CXX_ERROR(logger_, "SendHandshakeData: null arguments size.");
      return false;
    }
  const uint8_t service_id = *inMessage->getData();
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceTypeFromByte(service_id);
  if(service_type == protocol_handler::kInvalidServiceType) {
      LOG4CXX_ERROR(logger_,
                    "ProtectServiceRequest: Wrong ServiceType "
                    << service_id);
      return false;
    }
  SSLContext * sslContext =
      session_observer_->GetSSLContext(inMessage->getConnectionKey(), service_type);
  if(!sslContext) {
      LOG4CXX_ERROR(logger_, "SendHandshakeData: No ssl context");
      return false;
    }
  const uint8_t * data = inMessage->getData();
  const size_t data_size = inMessage->getDataSize();
  size_t out_data_size;
  const uint8_t * out_data  = static_cast<uint8_t *>
      (sslContext->DoHandshakeStep(data, data_size, &out_data_size));
  if(!out_data){
      LOG4CXX_WARN(logger_, "Handshake fail: " << LastError());
      return false;
    }

  //FIXME(EZ): add header to data array
  uint8_t* rawData = new uint8_t[data_size];
  memcpy(rawData, data, data_size);

  SendBinaryData(inMessage->getConnectionKey(), rawData, data_size);

  return true;
}

void SecureServiceManager::SendProtectServiceResponse(
    const SecureServiceMessage &requestMessage,
    const SecureServiceQuery::ProtectServiceResult result){
  LOG4CXX_INFO(logger_, "SecureServiceManager::SendProtectServiceResponse");
  DCHECK(requestMessage->getHeader().query_id_==
         SecureServiceQuery::ProtectServiceRequest);

  const SecureServiceQuery::QueryHeader header(
        SecureServiceQuery::ProtectServiceResponse,
        requestMessage->getHeader().seq_number_);

  const uint8_t data_result = result;
  SendData(requestMessage->getConnectionKey(), header, &data_result, 1);
}

void SecureServiceManager::SendInternalError(const int32_t connectionKey,
                                             const uint32_t seq_umber,
                                             const std::string &error) {
  const SecureServiceQuery::QueryHeader header(
        SecureServiceQuery::InvalidQuery, seq_umber);
  const uint8_t* const error_str = reinterpret_cast<const uint8_t*>(error.c_str());
  SendData(connectionKey, header, error_str, error.size());
}

void SecureServiceManager::SendData(
    const int32_t connectionKey,
    const SecureServiceQuery::QueryHeader& header,
    const uint8_t * const data, const size_t data_size) {
  const size_t data_sending_size = sizeof(header) + data_size;
  uint8_t* data_sending = new uint8_t[data_sending_size];

  memcpy(data_sending, &header, sizeof(header));
  memcpy(data_sending + sizeof(header), data, data_size);

  SendBinaryData(connectionKey, data_sending, data_sending_size);
}

void SecureServiceManager::SendBinaryData(const int32_t connectionKey,
                                          uint8_t * data,
                                          size_t data_size)
{
  LOG4CXX_INFO(logger_, "SecureServiceManager::SendBinaryDataData");
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
