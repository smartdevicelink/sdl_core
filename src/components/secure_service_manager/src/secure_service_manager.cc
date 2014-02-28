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
  session_observer_(0){
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
  const SecureServiceQueryPtr serviceQueryPtr(
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
  const bool result = query->setData(message->data(), message->data_size());
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
}

void SecureServiceManager::set_session_observer(
    protocol_handler::SessionObserver *observer) {
  if (!observer) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to ISessionObserver.");
    return;
  }
  session_observer_ = observer;
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
    const SecureServiceMessage &message) {
  LOG4CXX_INFO(logger_, "ProtectServiceRequest processing");
  DCHECK(message->getHeader().query_id_ == SecureServiceQuery::ProtectServiceRequest);
  DCHECK(message); DCHECK(session_observer_);
  if(message->getDataSize() != 1) {
      LOG4CXX_ERROR(logger_,
                    "ProtectServiceRequest: wrong arguments size." <<
                    message->getDataSize());
      return false;
    }
  const uint8_t service_id = *message->getData();
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceTypeFromByte(service_id);
  if(service_type == protocol_handler::kInvalidServiceType) {
      LOG4CXX_ERROR(logger_,
                    "ProtectServiceRequest: Wrong ServiceType "
                    << service_id);
      return false;
    }
  secure_service_manager::SSLContext * const sslContext =
      crypto_manager_->CreateSSLContext();
  if(!sslContext) {
      LOG4CXX_ERROR(logger_,
                    "CryptoManager: could not create SSl context.");
      return false;
    }
  const bool result = session_observer_->SetSSLContext(
        message->getConnectionKey(), service_type, sslContext);
  if(!result) {
      LOG4CXX_ERROR(logger_,
                    "SessionObserver: could not set SSL context.");
      return false;
    }
  return true;
}

bool SecureServiceManager::ProtectServiceResponse(const SecureServiceMessage &message) {
  LOG4CXX_INFO(logger_, "ProtectServiceResponse processing");
  DCHECK(message->getHeader().query_id_ == SecureServiceQuery::ProtectServiceResponse);
  DCHECK(message); DCHECK(session_observer_);
  if(message->getDataSize() != 1) {
      LOG4CXX_ERROR(logger_,
                    "ProtectServiceResponse: wrong arguments size." <<
                    message->getDataSize());
      return false;
    }
//  const uint8_t service_id = *message->getData();
//  const protocol_handler::ServiceType service_type =
//      protocol_handler::ServiceTypeFromByte(service_id);
//  if(service_type == protocol_handler::kInvalidServiceType) {
//      LOG4CXX_ERROR(logger_,
//                    "ProtectServiceRequest: Wrong ServiceType "
//                    << service_id);
//      return false;
//    }
  assert(!"Not implemented push logics");
  return true;
}

bool SecureServiceManager::SendHandshakeData(const SecureServiceMessage &message) {
  LOG4CXX_INFO(logger_, "SendHandshakeData processing");
  DCHECK(message->getHeader().query_id_ == SecureServiceQuery::SendHandshakeData);
  DCHECK(message);
  if(message->getDataSize() > 0) {
      LOG4CXX_ERROR(logger_, "SendHandshakeData: null arguments size.");
      return false;
    }
  const uint8_t service_id = *message->getData();
  const protocol_handler::ServiceType service_type =
      protocol_handler::ServiceTypeFromByte(service_id);
  if(service_type == protocol_handler::kInvalidServiceType) {
      LOG4CXX_ERROR(logger_,
                    "ProtectServiceRequest: Wrong ServiceType "
                    << service_id);
      return false;
    }
  SSLContext * sslContext =
      session_observer_->GetSSLContext(message->getConnectionKey(), service_type);
  if(!sslContext) {
      LOG4CXX_ERROR(logger_, "SendHandshakeData: No ssl context");
      return false;
    }
  const uint8_t * data = message->getData();
  const size_t data_size = message->getDataSize();
  size_t out_data_size;
  const uint8_t * new_data = sslContext->DoHandshake(data, data_size, out_data_size);
  if(!new_data){
      LOG4CXX_WARN(logger_, "Handshake fail: " << getError());
      return false;
    }
  assert(!"Not implemented push logics");
  return true;
}
