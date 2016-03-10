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

#ifndef TEST_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_SESSION_OBSERVER_MOCK_H_
#define TEST_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_SESSION_OBSERVER_MOCK_H_

#include <gmock/gmock.h>
#include <string>
#include <list>
#include "protocol_handler/session_observer.h"

namespace test {
namespace components {
namespace protocol_handler_test {
/*
 * MOCK implementation of ::protocol_handler::SessionObserver interface
 */
class MockSessionObserver : public ::protocol_handler::SessionObserver {
 public:
  MOCK_METHOD5(
      OnSessionStartedCallback,
      uint32_t(const transport_manager::ConnectionUID connection_handle,
               const uint8_t sessionId,
               const protocol_handler::ServiceType& service_type,
               const bool is_protected,
               uint32_t* hash_id));
  MOCK_METHOD4(
      OnSessionEndedCallback,
      uint32_t(const transport_manager::ConnectionUID connection_handle,
               const uint8_t sessionId,
               const uint32_t& hashCode,
               const protocol_handler::ServiceType& service_type));
  MOCK_METHOD1(OnApplicationFloodCallBack,
               void(const uint32_t& connection_key));
  MOCK_METHOD1(OnMalformedMessageCallback,
               void(const uint32_t& connection_key));
  MOCK_CONST_METHOD2(
      KeyFromPair,
      uint32_t(transport_manager::ConnectionUID connection_handle,
               uint8_t session_id));
  MOCK_CONST_METHOD3(PairFromKey,
                     void(uint32_t key,
                          transport_manager::ConnectionUID* connection_handle,
                          uint8_t* sessionId));
  MOCK_CONST_METHOD4(GetDataOnSessionKey,
                     int32_t(uint32_t key,
                             uint32_t* app_id,
                             std::list<int32_t>* sessions_list,
                             uint32_t* device_id));
  MOCK_CONST_METHOD5(GetDataOnDeviceID,
                     int32_t(uint32_t device_handle,
                             std::string* device_name,
                             std::list<uint32_t>* applications_list,
                             std::string* mac_address,
                             std::string* connection_type));
  MOCK_CONST_METHOD2(IsHeartBeatSupported,
                     bool(transport_manager::ConnectionUID connection_handle,
                          uint8_t session_id));
  MOCK_CONST_METHOD3(ProtocolVersionUsed,
                     bool(uint32_t connection_id,
                          uint8_t session_id,
                          uint8_t& protocol_version));

#ifdef ENABLE_SECURITY
  MOCK_METHOD2(SetSSLContext,
               int(const uint32_t& key, security_manager::SSLContext* context));
  MOCK_METHOD2(GetSSLContext,
               security_manager::SSLContext*(
                   const uint32_t& key,
                   const protocol_handler::ServiceType& service_type));
  MOCK_METHOD2(SetProtectionFlag,
               void(const uint32_t& key,
                    const protocol_handler::ServiceType& service_type));
  MOCK_CONST_METHOD1(
      GetHandshakeContext,
      security_manager::SSLContext::HandshakeContext(uint32_t key));
#endif  // ENABLE_SECURITY
};
}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test
#endif  // TEST_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_SESSION_OBSERVER_MOCK_H_
