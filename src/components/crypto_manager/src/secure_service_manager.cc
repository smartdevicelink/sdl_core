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

#include "crypto_manager/secure_service_manager.h"
#include "crypto_manager/crypto_manager_impl.h"
using namespace crypto_manager;

log4cxx::LoggerPtr SecureServiceManager::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("SecureServiceManager"));

SecureServiceManager::SecureServiceManager() :
  secure_service_messages_("SecureServiceManager::secure_service_messages_", this),
  crypto_manager_(new crypto_manager::CryptoManagerImpl()){
}

void SecureServiceManager::OnMessageReceived(
    const protocol_handler::RawMessagePtr &message) {
  LOG4CXX_INFO(logger_, "SecureServiceManager::OnMessageReceived");
  if(message->service_type() == protocol_handler::kSecure) {
      LOG4CXX_WARN(logger_, "Incorrect message service type "
                   << message->service_type());
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
  return SecureServiceQueryPtr(query);
}

void SecureServiceManager::OnMobileMessageSent(
    const protocol_handler::RawMessagePtr &message) {
  LOG4CXX_INFO(logger_, "SecureServiceManager::OnMobileMessageSent");
  assert(!"not implemented");
}

void SecureServiceManager::Handle(const SecureServiceMessageLoop::Message &message) {
  LOG4CXX_INFO(logger_, "Received Secure Service message from Mobile side");
  assert(!"not implemented");
}

//void SecureServiceManager::ProcessMessageFromMobile(const utils::SharedPtr<Message> &message)
//{

//}

//void SecureServiceManager::ProcessMessageFromHMI(const utils::SharedPtr<Message> &message)
//{

//}
