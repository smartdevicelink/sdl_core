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
using namespace crypto_manager;
//TODO add info
inline uint32_t getUInt8Value(const uint8_t* const data){
  DCHECK(data);
  return
      data[0] << 24 | data[1] << 16 |
      data[2] <<  8 | data[3];
}

log4cxx::LoggerPtr SecureServiceManager::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("SecureServiceManager"));

SecureServiceManager::SecureServiceManager() :
  secure_message_message_("SecureManager::message_for_encryption", this),
  secure_manager_(new crypto_manager::SecureServiceManager()){
}

void SecureServiceManager::OnMessageReceived(
    const protocol_handler::RawMessagePtr &message) {
  if(message->service_type() == protocol_handler::kSecure) {
      //LOG4CXX_WARN(logger_, "Incorrect message received");
      return;
    }
  const SecureServiceMessagePtr serviceQueryPtr(
        SecureMessageFromRawMessage(message));
  if(serviceQueryPtr) {
      secure_message_message_.PostMessage(serviceQueryPtr);
    } else {
      LOG4CXX_WARN(logger_, "Incorrect message received");
    }
}

SecureServiceQueryPtr SecureServiceManager::SecureMessageFromRawMessage(
    const protocol_handler::RawMessagePtr &message) {
  assert(!"not implemented");
  DCHECK(message->service_type() == protocol_handler::kSecure);
  const uint8_t* const binary_data = message->data();

  if(message->data_size() < sizeof(SecureServiceQuery::QueryHeader)) {
      LOG4CXX_ERROR(logger_, "Received invalid message: wrong query size.");
      return SecureServiceQueryPtr();
    }

  SecureServiceQuery::QueryHeader header;
  header.query_id_ = binary_data[0];
  header.seq_number_ = getUInt8Value(binary_data + 1);
  header.seq_number_ = getUInt8Value(binary_data + 5);
  return SecureServiceMessagePtr(
        new SecureServiceQuery(header,
                               binary_data + sizeof(SecureServiceQuery::QueryHeader)));
}

void SecureServiceManager::OnMobileMessageSent(
    const protocol_handler::RawMessagePtr &message) {
  assert(!"not implemented");
}

void SecureServiceManager::Handle(const SecureServiceMessagePtr &message) {
  assert(!"not implemented");
}
