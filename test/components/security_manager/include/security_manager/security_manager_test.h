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

#ifndef SECURITY_MANAGER_TEST_H
#define SECURITY_MANAGER_TEST_H

#include <gtest/gtest.h>
#include "security_manager/security_manager.h"
#include "security_manager/security_manager_mock.h"
#include "connection_handler/connection_handler_impl.h"
#include "protocol_handler/protocol_payload.h"

namespace test  {
namespace components  {
namespace security_manager_test {

  using ::protocol_handler::RawMessage;
  using ::protocol_handler::RawMessagePtr;
  using ::security_manager::SecuityQuery;

  class SecurityManagerTest: public ::testing::Test {
   protected:
    void SetUp() OVERRIDE {
      security_manager_.reset(new security_manager::SecurityManager());
      security_manager_->set_session_observer(&mock_session_observer);
      security_manager_->set_protocol_handler(&mock_protocol_observer);
    }
    /*
    * Wrapper for fast send SecurityManager::OnMessageReceived
    */
    void call_OnMessageReceived(const uint8_t* const data, uint32_t dataSize,
                                const protocol_handler::ServiceType serviceType) {
      const RawMessagePtr rawMessagePtr(
            new RawMessage( key, protocolVersion, data, dataSize, serviceType));
      security_manager_->OnMessageReceived(rawMessagePtr);
    }

    ::utils::SharedPtr<security_manager::SecurityManager> security_manager_;
    // Strict mocks (same as all methods EXPECT_CALL().Times(0))
    testing::StrictMock<SessionObserverMock>   mock_session_observer;
    testing::StrictMock<ProtocoloObserverMock> mock_protocol_observer;
    // constants
    const int32_t key = 0;
    const protocol_handler::ServiceType secureServiceType = protocol_handler::kSecure;
    const uint32_t protocolVersion = protocol_handler::PROTOCOL_VERSION_2;
    const bool is_final = false;
    const uint32_t seq_number = 1;
  };

  /*
   * Security QueryHeader shall be the same size as RPC header
   * for correct working on Mobile side
   */
  TEST_F(SecurityManagerTest, SecurityHeader_eq_RPCHeader) {
    ASSERT_EQ(sizeof(security_manager::SecuityQuery::QueryHeader)*8,
              protocol_handler::ProtocolPayloadV2SizeBits());
  }

  /*
   * SecurityManger shall skip all not-Secure messages
   */
  TEST_F(SecurityManagerTest, OnMessageReceived_WrongService) {
    // Call with wrong Service type
    call_OnMessageReceived(NULL, 0, protocol_handler::kZero);
    call_OnMessageReceived(NULL, 0, protocol_handler::kRpc);
    call_OnMessageReceived(NULL, 0, protocol_handler::kAudio);
    call_OnMessageReceived(NULL, 0, protocol_handler::kMobileNav);
    call_OnMessageReceived(NULL, 0, protocol_handler::kBulk);
    call_OnMessageReceived(NULL, 0, protocol_handler::kInvalidServiceType);
    // Wait call methods in thread
    sleep(1);
    //Strict mocks are the same as EXPECT_CALL(ALL).Times(0)
  }

  /*
   * SecurityManger shall send InternallError on null data recieved
   */
  TEST_F(SecurityManagerTest, OnMessageReceived_NullData) {
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecuityQuery::ERROR_NULL_DATA), is_final)).Times(1);
    // Call with NULL data
    call_OnMessageReceived(NULL, 0, secureServiceType);
    // Wait call methods in thread
    sleep(1);
  }

  /*
   * SecurityManger shall send InternallError on INVALID_QUERY_ID
   */
  TEST_F(SecurityManagerTest, OnMessageReceived_InvalidQuery) {
    const security_manager::SecuityQuery::QueryHeader header(
          security_manager::SecuityQuery::REQUEST,
          security_manager::SecuityQuery::INVALID_QUERY_ID,
          seq_number);
    const void* data = &header;
    uint32_t data_size = sizeof(header);

    // Expect error message with string
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecuityQuery::ERROR_INVALID_QUERY_ID),is_final)) .Times(1);

    call_OnMessageReceived(static_cast<const uint8_t*>(data),
                           data_size, secureServiceType);
    // Wait call methods in thread
    sleep(1);
  }

  TEST_F(SecurityManagerTest, OnMessageReceived_ProtectServiceRequest_NULLData) {

    const security_manager::SecuityQuery::QueryHeader header(
          security_manager::SecuityQuery::REQUEST,
          security_manager::SecuityQuery::PROTECT_SERVICE_REQUEST,
          seq_number);
    const void* data = &header;
    uint32_t data_size = sizeof(header);
    const uint8_t* uint8_data = static_cast<const uint8_t*>(data);

    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(RawMessageEq(uint8_data, data_size), is_final))
        .Times(1);

    call_OnMessageReceived(static_cast<const uint8_t*>(data),
                           data_size, secureServiceType);
    //Wait call methods in thread
    sleep(1);
  }

} // connection_handle
} // namespace components
} // namespace test
#endif // SECURITY_MANAGER_TEST_H
