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
  if(message->service_type() == protocol_handler::kSecure) {
      LOG4CXX_WARN(logger_, "Incorrect message service type "
                   << message->service_type());
      return;
    }
  if(!session_observer_) {
      LOG4CXX_ERROR(logger_, "No SessionObserver for usage.");
      return;
    }
  const SecureServiceMessage serviceQueryPtr(
        SecureMessageFromRawMessage(message));
  if(serviceQueryPtr) {
      secure_service_messages_.PostMessage(serviceQueryPtr);
    } else {
      LOG4CXX_WARN(logger_, "Incorrect message received");
    }
}

SecureServiceQueryPtr SecureServiceManager::SecureMessageFromRawMessage(
    const protocol_handler::RawMessagePtr &message) {
  LOG4CXX_INFO(logger_, "SecureServiceManager::SecureMessageFromRawMessage");
  DCHECK(message->service_type() == protocol_handler::kSecure);
  SecureServiceQueryPtr query(new SecureServiceQuery());
  const bool result = query->Parse(message->data(), message->data_size());
  if(!result) {
      LOG4CXX_ERROR(logger_, "Incorrect message received");
      return SecureServiceQueryPtr();
    }
  query->setConnectionKey(message->connection_key());
  return SecureServiceQueryPtr(query);
}

void SecureServiceManager::OnMobileMessageSent(
    const protocol_handler::RawMessagePtr &message) {
  LOG4CXX_INFO(logger_, "SecureServiceManager::OnMobileMessageSent");
  assert(!"not implemented");
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
  DCHECK(message); DCHECK(session_observer_);
  LOG4CXX_INFO(logger_, "Received Secure Service message from Mobile side");
  switch (message->getHeader().query_id_) {
    case SecureServiceQuery::ProtectServiceRequest:
      if(!ProtectServiceRequest(message)) {
          LOG4CXX_ERROR(logger_, "ProtectServiceRequest failed");
        }
      return;
    case SecureServiceQuery::ProtectServiceResponse:
      if(!ProtectServiceResponse(message)) {
          LOG4CXX_ERROR(logger_, "ProtectServiceRequest failed");
        }
      return;
    case SecureServiceQuery::SendHandshakeData:
      if(!SendHandshakeData(message)) {
          LOG4CXX_ERROR(logger_, "ProtectServiceRequest failed");
        }
      return;
    default: // SecureServiceQuery::InvalidSecureServiceQuery
      LOG4CXX_ERROR(logger_, "Secure Service message: unknown query");
    }
}

bool SecureServiceManager::ProtectServiceRequest(
    const SecureServiceMessage &requestMessage) {
  LOG4CXX_INFO(logger_, "ProtectServiceRequest processing");
  DCHECK(requestMessage->getHeader().query_id_ ==
         SecureServiceQuery::ProtectServiceRequest);
  DCHECK(requestMessage); DCHECK(session_observer_);
  if(requestMessage->getDataSize() != 1) {
      LOG4CXX_ERROR(logger_, "Wrong arguments size." <<
                    requestMessage->getDataSize());
      PostProtectServiceResponse(requestMessage,
                                 SecureServiceQuery::INTERNAL_ERROR);
      return false;
    }
  const uint8_t service_id = *requestMessage->getData();
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceTypeFromByte(service_id);
  if(service_type == protocol_handler::kInvalidServiceType) {
      LOG4CXX_ERROR(logger_, "Wrong Service type" << service_id);
      //Generate response query and post to secure_service_messages_
      PostProtectServiceResponse(requestMessage,
                                 SecureServiceQuery::SERVICE_ALREADY_PROTECTED);
      return false;
    }

  const secure_service_manager::SSLContext * const currentSSLContext =
      session_observer_->GetSSLContext(
        requestMessage->getConnectionKey(), service_type);
  //check current SSl context
  if(currentSSLContext){
    if(currentSSLContext->IsInitCompleted()) {
      LOG4CXX_WARN(logger_, "Service is already protected, service_id"
                   << service_id);
      //Generate response query and post to secure_service_messages_
      PostProtectServiceResponse(requestMessage,
                                 SecureServiceQuery::SERVICE_ALREADY_PROTECTED);
      }
    else {
      LOG4CXX_WARN(logger_, "Secure protection is pending, service_id"
                   << service_id);
      //Generate response query and post to secure_service_messages_
      PostProtectServiceResponse(requestMessage,
                                 SecureServiceQuery::SERVICE_ALREADY_PROTECTED);
      }
    return false;
  }

  secure_service_manager::SSLContext * const newSSLContext =
      crypto_manager_->CreateSSLContext();
  if(!newSSLContext) {
      LOG4CXX_ERROR(logger_, "CryptoManager could not create SSl context.");
      //Generate response query and post to secure_service_messages_
      PostProtectServiceResponse(requestMessage,
                                 SecureServiceQuery::INTERNAL_ERROR);
      return false;
    }
  const bool result = session_observer_->SetSSLContext(
        requestMessage->getConnectionKey(), service_type, newSSLContext);
  if(!result) {
      LOG4CXX_ERROR(logger_,
                    "SessionObserver: could not set SSL context.");
      //Generate response query and post to secure_service_messages_
      PostProtectServiceResponse(requestMessage,
                                 SecureServiceQuery::INTERNAL_ERROR);
      return false;
    }
  //Generate response query and post to secure_service_messages_
  PostProtectServiceResponse(requestMessage, SecureServiceQuery::SUCCESS);
  return true;
}

bool SecureServiceManager::ProtectServiceResponse(
    const SecureServiceMessage &responseMessage) {
  LOG4CXX_INFO(logger_, "ProtectServiceResponse processing");
  DCHECK(responseMessage->getHeader().query_id_ ==
         SecureServiceQuery::ProtectServiceResponse);
  DCHECK(responseMessage); DCHECK(session_observer_);
  if(responseMessage->getDataSize() != 1) {
      LOG4CXX_ERROR(logger_,
                    "ProtectServiceResponse: wrong arguments size." <<
                    responseMessage->getDataSize());
      return false;
    }

  uint8_t* rawData = new uint8_t[responseMessage->getDataSize()];
  memcpy(rawData, responseMessage->getData(), responseMessage->getDataSize());

  protocol_handler::RawMessagePtr rawMessagePtr(
        new protocol_handler::RawMessage( responseMessage->getConnectionKey(), 2,
                                          rawData, responseMessage->getDataSize(),
                                          protocol_handler::kSecure));

  if (!rawMessagePtr) {
    LOG4CXX_ERROR(logger_, "Failed to create raw message.");
    return false;
    }
  OnMobileMessageSent(rawMessagePtr);
  return true;
}

bool SecureServiceManager::SendHandshakeData(const SecureServiceMessage &inMessage) {
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
  PostSendHandshakeData(inMessage, out_data, out_data_size);
  return true;
  }

void SecureServiceManager::PostProtectServiceResponse(
    const SecureServiceMessage &requestMessage,
    const SecureServiceQuery::ProtectServiceResult response){
    LOG4CXX_INFO(logger_, "SecureServiceManager::GenerateProtectServiceResponse");
    DCHECK(requestMessage->getHeader().query_id_==
           SecureServiceQuery::ProtectServiceRequest);

    SecureServiceQuery::QueryHeader header;
    header.query_id_ = SecureServiceQuery::ProtectServiceResponse;
    header.seq_number_ = requestMessage->getHeader().seq_number_;

    SecureServiceMessage query(
          new SecureServiceQuery(header, requestMessage->getConnectionKey()));
    const uint8_t data = response;
    query->setData(&data, 1);
    secure_service_messages_.PostMessage(query);
  }

void SecureServiceManager::PostSendHandshakeData(
    const SecureServiceMessage &inMessage,
    const uint8_t * const data, const size_t data_size) {
    LOG4CXX_INFO(logger_, "SecureServiceManager::PostSendHandshakeData");
    DCHECK(data); DCHECK(data_size);
    DCHECK(inMessage->getHeader().query_id_ ==
           SecureServiceQuery::SendHandshakeData);

    SecureServiceQuery::QueryHeader header = inMessage->getHeader();

    SecureServiceMessage query(
          new SecureServiceQuery(header, inMessage->getConnectionKey()));
    query->setData(data, data_size);
    secure_service_messages_.PostMessage(query);
  }
