/*
 * Copyright (c) 2013, Ford Motor Company
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

#ifndef SECURITY_MANAGER_TEST_H
#define SECURITY_MANAGER_TEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "security_manager/security_manager.h"
#include "connection_handler/connection_handler_impl.h"
#include "protocol_handler/protocol_handler_impl.h"

using ::testing::_;
using ::testing::NotNull;
using ::testing::Pointee;
using ::protocol_handler::RawMessage;
using ::protocol_handler::RawMessagePtr;

namespace test  {
namespace components  {
namespace security_manager_test {

  //MOCK implementation of SessionObserver interface
  class SessionObserver: public protocol_handler::SessionObserver {
  public:
    MOCK_METHOD3(SetSSLContext,
                 int( const uint32_t& key,
                      protocol_handler::ServiceType service_type,
                      security_manager::SSLContext* context));
    MOCK_METHOD2(GetSSLContext,
                 security_manager::SSLContext* (
                   const uint32_t& key,
                   protocol_handler::ServiceType service_type));
    MOCK_METHOD3(OnSessionStartedCallback,
                 int32_t(
                   const transport_manager::ConnectionUID& connection_handle,
                   const uint8_t& sessionId,
                   const protocol_handler::ServiceType& service_type));
    MOCK_METHOD4(OnSessionEndedCallback,
                 uint32_t(
                   const transport_manager::ConnectionUID& connection_handle,
                   const uint8_t& sessionId,
                   const uint32_t& hashCode,
                   const protocol_handler::ServiceType& service_type));
    MOCK_METHOD2(KeyFromPair,
                 uint32_t(
                   transport_manager::ConnectionUID connection_handle,
                   uint8_t sessionId));
    MOCK_METHOD3(PairFromKey,
                 void(
                   uint32_t key,
                   transport_manager::ConnectionUID* connection_handle,
                   uint8_t* sessionId));
    MOCK_METHOD4(GetDataOnSessionKey,
                 int32_t(uint32_t key,
                         uint32_t* app_id,
                         std::list<int32_t>* sessions_list,
                         uint32_t* device_id));
    MOCK_METHOD4(GetDataOnDeviceID,
                 int32_t(
                   uint32_t device_handle,
                   std::string* device_name,
                   std::list<uint32_t>* applications_list,
                   std::string* mac_address));
  };
  //MOCK implementation of ProtocolObserver interface
  class ProtocoloObserver: public protocol_handler::ProtocolHandler {
  public:
    MOCK_METHOD2(SendMessageToMobileApp,
                 void(const RawMessagePtr& message, bool final_message));
    MOCK_METHOD1(AddProtocolObserver,
                 void(protocol_handler::ProtocolObserver* observer));
    MOCK_METHOD1(RemoveProtocolObserver,
                 void(protocol_handler::ProtocolObserver* observer));
    MOCK_METHOD2(SendFramesNumber,
                 void(int32_t connection_key, int32_t number_of_frames));
  };

  //Mock matcher for check equal RawMessages
  MATCHER_P2(RawMessageEq, exp_data, exp_data_size,
             std::string(negation ? "is not" : "is") + " equal ") {
    const size_t header_size = sizeof(security_manager::SecuityQuery::QueryHeader);
    const size_t arg_data_size = arg->data_size();
    if(arg_data_size != exp_data_size) {
//      result_listener << std::string("Got ") << arg_data_size << " bytes"
//                      << "expected " << exp_data_size << " bytes";
      return false;
      }
    const uint8_t * arg_data = arg->data();
    for (int i = 0; i < arg_data_size; ++i) {
      if(arg_data[i] != exp_data[i]) {
//        result_listener << std::string("Fail in ") << i << "byte";
        return false;
        }
      }
    return true;
  }

  //Mock matcher for check RawMessage as InternalError message with substring
  MATCHER_P(InternalErrorHasSubstr, expected,
             std::string(negation ? "is not" : "is") + " equal " + expected) {
    const size_t header_size = sizeof(security_manager::SecuityQuery::QueryHeader);
    if(arg->data_size() < header_size)
      return false;
    if(security_manager::SecuityQuery::InternalError != arg->data()[0])
      return false;
    const char* const string_data = reinterpret_cast<char*>(arg->data() + header_size);
    const std::string string(string_data, arg->data_size() - header_size);
    return std::string::npos != string.find(expected);
  }

  ///////////////////

  class SecurityManagerTest: public ::testing::Test {
   protected:
    void SetUp() OVERRIDE {
      security_manager_.reset(new security_manager::SecurityManager());
      security_manager_->set_session_observer(&mock_session_observer);
      security_manager_->set_protocol_handler(&mock_protocol_observer);
    }

    //SecurityManager::OnMessageReceived Wrapper
    void call_OnMessageReceived(const uint8_t* const data, uint32_t dataSize,
                                const protocol_handler::ServiceType serviceType) {
      const RawMessagePtr rawMessagePtr(
            new RawMessage( key, protocolVersion, data, dataSize, serviceType));
      security_manager_->OnMessageReceived(rawMessagePtr);
    }

    ::utils::SharedPtr<security_manager::SecurityManager> security_manager_;
    //Strict mocks (same as all methods EXPECT_CALL().Times(0))
    testing::StrictMock<SessionObserver>   mock_session_observer;
    testing::StrictMock<ProtocoloObserver> mock_protocol_observer;
    //const values
    const int32_t key = 0;
    const protocol_handler::ServiceType secureServiceType = protocol_handler::kSecure;
    const uint32_t protocolVersion = protocol_handler::PROTOCOL_VERSION_2;
    const bool is_final = false;
    const uint32_t seq_number = 1;
  };

  TEST_F(SecurityManagerTest, OnMessageReceived_WrongService) {
    //Call with wrong Service type
    call_OnMessageReceived(NULL, 0, protocol_handler::kZero);
    call_OnMessageReceived(NULL, 0, protocol_handler::kRpc);
    call_OnMessageReceived(NULL, 0, protocol_handler::kAudio);
    call_OnMessageReceived(NULL, 0, protocol_handler::kMobileNav);
    call_OnMessageReceived(NULL, 0, protocol_handler::kBulk);
    call_OnMessageReceived(NULL, 0, protocol_handler::kInvalidServiceType);
    //Wait call methods in thread
    sleep(1);
  }

  TEST_F(SecurityManagerTest, OnMessageReceived_NullData) {
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(InternalErrorHasSubstr("Incorrect message"),
                                       is_final)).Times(1);
    //Call with NULL data
    call_OnMessageReceived(NULL, 0, secureServiceType);
    //Wait call methods in thread
    sleep(1);
  }

  TEST_F(SecurityManagerTest, OnMessageReceived_InvalidQuery) {
    const security_manager::SecuityQuery::QueryHeader header(
          security_manager::SecuityQuery::InvalidQuery, seq_number);
    const void* data = &header;
    uint32_t data_size = sizeof(header);

    //Expect eror message with string
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(InternalErrorHasSubstr("Unknown query"),
                                       is_final)) .Times(1);

    call_OnMessageReceived(static_cast<const uint8_t*>(data),
                           data_size, secureServiceType);
    //Wait call methods in thread
    sleep(1);
  }

  TEST_F(SecurityManagerTest, OnMessageReceived_ProtectServiceRequest) {
//    const security_manager::SecuityQuery::QueryHeader header(
//          security_manager::SecuityQuery::ProtectServiceRequest, seq_number);
//    const void* data = &header;
//    uint32_t data_size = sizeof(header);
//    const uint8_t* uint8_data = static_cast<const uint8_t*>(data);

//    EXPECT_CALL(mock_protocol_observer,
//                SendMessageToMobileApp(RawMessageEq(uint8_data, data_size), is_final))
//        .Times(1);

//    call_OnMessageReceived(static_cast<const uint8_t*>(data),
//                           data_size, secureServiceType);
//    //Wait call methods in thread
//    sleep(1);
  }

} // connection_handle
} // namespace components
} // namespace test
#endif // SECURITY_MANAGER_TEST_H
