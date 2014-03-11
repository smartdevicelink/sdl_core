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

//TODO: add test on get correct/wrong InternalError
//TODO: check connection_key the same and seq_number
namespace test  {
namespace components  {
namespace security_manager_test {

  using ::protocol_handler::RawMessage;
  using ::protocol_handler::RawMessagePtr;
  using ::security_manager::SecurityQuery;
  using ::testing::_;
  using ::testing::Return;

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

    /*
    * Wrapper for fast send query
    */
    void EmulateMobileMessage(SecurityQuery::QueryHeader header,
                              const uint8_t* const data, const uint32_t data_size ){
      //convert to Little-Endian (network) order
      header.query_id = LE_TO_BE32(header.query_id << 8);

      const size_t data_sending_size = sizeof(header) + data_size;
      uint8_t* data_sending = new uint8_t[data_sending_size];
      memcpy(data_sending, &header, sizeof(header));
      memcpy(data_sending + sizeof(header), data, data_size);

      call_OnMessageReceived(data_sending, data_sending_size,
                             protocol_handler::kSecure);
      delete[] data_sending;
    }

    ::utils::SharedPtr<security_manager::SecurityManager> security_manager_;
    // Strict mocks (same as all methods EXPECT_CALL().Times(0))
    testing::StrictMock<SessionObserverMock>   mock_session_observer;
    testing::StrictMock<ProtocoloObserverMock> mock_protocol_observer;
    testing::StrictMock<SSLContextMock>        mock_ssl_context;
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
    ASSERT_EQ(sizeof(SecurityQuery::QueryHeader)*8,
              protocol_handler::ProtocolPayloadV2SizeBits());
  }

  /*
   * SecurityManger shall skip all not-Secure messages
   */
  TEST_F(SecurityManagerTest, GetWrongServiceType) {
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
  TEST_F(SecurityManagerTest, GetEmptyQuery) {
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_INVALID_QUERY_SIZE), is_final)).Times(1);
    // Call with NULL data
    call_OnMessageReceived(NULL, 0, secureServiceType);
    // Wait call methods in thread
    sleep(1);
  }

  /*
   * SecurityManger shall send InternallError on INVALID_QUERY_ID
   */
  TEST_F(SecurityManagerTest, GetInvalidQueryId) {
    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_INVALID_QUERY_ID),is_final)).Times(1);
    const SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          SecurityQuery::INVALID_QUERY_ID, seq_number);
    EmulateMobileMessage(header, NULL, 0);
    // Wait call methods in thread
    sleep(1);
  }

  /*
   * SecurityManger shall send InternallError on
   * getting PROTECT_SERVICE_RESPONSE from mobile side
   */
  TEST_F(SecurityManagerTest, GetProtectServiceResponse) {
    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_OTHER_INTERNAL_ERROR), is_final)).
        Times(1);
    const SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          SecurityQuery::PROTECT_SERVICE_RESPONSE, seq_number);
    EmulateMobileMessage(header, NULL, 0);
    // Wait call methods in thread
    sleep(1);
  }

  /*
   * SecurityManger shall send ProtectServiceResponse with INTERNAL_ERROR
   * on getting PROTECT_SERVICE_RESPONSE with wrong data size
   */
  TEST_F(SecurityManagerTest, GetProtectServiceRequest_WrongDataSize) {
    // Expect ProtectServiceResponse with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  ProtectServiceResponseWithId(
                    SecurityQuery::INTERNAL_ERROR),is_final)).Times(1);
    const SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          SecurityQuery::PROTECT_SERVICE_REQUEST, seq_number);
    EmulateMobileMessage(header, NULL, 0);
    // Wait call methods in thread
    sleep(1);
  }

  /*
   * SecurityManger shall send ProtectServiceResponse with SERVICE_NOT_FOUND
   * on getting PROTECT_SERVICE_RESPONSE with invalid service type
   * Check few service types: kInvalidServiceType, ... , kInvalidServiceType+N
   */
  TEST_F(SecurityManagerTest, GetProtectServiceRequest_InvalidServiceType) {
    const int invalid_requests = 10;
    DCHECK(protocol_handler::kInvalidServiceType + invalid_requests <= UCHAR_MAX);

    // Expect ProtectServiceResponse with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  ProtectServiceResponseWithId(
                    SecurityQuery::SERVICE_NOT_FOUND),is_final)).
        //invalid_requests + kSecure
        Times(invalid_requests + 1);
    const SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          SecurityQuery::PROTECT_SERVICE_REQUEST, seq_number);
    for (uint8_t i = protocol_handler::kInvalidServiceType;
         i < protocol_handler::kInvalidServiceType + invalid_requests; ++i) {
      EmulateMobileMessage(header, &i, 1);
    }
    //Secure service shall not be encrypted
    const uint8_t service_type = ::protocol_handler::kSecure;
    EmulateMobileMessage(header, &service_type, 1);
    // Wait call methods in thread
    sleep(1);
  }

  /*
   * SecurityManger shall send ProtectServiceResponse with INTERNAL_ERROR
   * on error create SSLContext
   */
  TEST_F(SecurityManagerTest, GetProtectServiceRequest_ErrorCreateSSLContext) {
    //FIXME(EZ): "move CryptoManager as extended ot remove test")
  }

  /*
   * SecurityManger shall send ProtectServiceResponse with SERVICE_NOT_FOUND
   * on getting PROTECT_SERVICE_RESPONSE for not start service
   */
  TEST_F(SecurityManagerTest, GetProtectServiceRequest_ServiceNotFound) {
    // Expect ProtectServiceResponse with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  ProtectServiceResponseWithId(
                    SecurityQuery::SERVICE_NOT_FOUND),is_final)).Times(1);
    const ::protocol_handler::ServiceType encryption_service_type =
        ::protocol_handler::kMobileNav;
    // Expect SessionObserver::SetSSLContext
    EXPECT_CALL(mock_session_observer,
                SetSSLContext( key, encryption_service_type,_)).
        WillOnce(Return(SecurityQuery::SERVICE_NOT_FOUND));
    const SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          SecurityQuery::PROTECT_SERVICE_REQUEST, seq_number);
    const uint8_t service_type_uint8 = encryption_service_type;
    EmulateMobileMessage(header, &service_type_uint8, 1);
    // Wait call methods in thread
    sleep(1);
  }

  /*
   * SecurityManger shall send ProtectServiceResponse with SERVICE_ALREADY_PROTECTED
   * on getting PROTECT_SERVICE_RESPONSE for already protected service
   */
  TEST_F(SecurityManagerTest,
         GetProtectServiceRequest_ServiceAlreadyProtected) {
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  ProtectServiceResponseWithId(
                    SecurityQuery::SERVICE_ALREADY_PROTECTED),is_final)).Times(1);
    const ::protocol_handler::ServiceType encryption_service_type =
        ::protocol_handler::kMobileNav;
    EXPECT_CALL(mock_session_observer,
                SetSSLContext(key, encryption_service_type,_)).
        WillOnce(Return(SecurityQuery::SERVICE_ALREADY_PROTECTED));
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, encryption_service_type)).
        WillOnce(Return(&mock_ssl_context));
    EXPECT_CALL(mock_ssl_context,
                IsInitCompleted()).
        WillOnce(Return(true));

    const SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          SecurityQuery::PROTECT_SERVICE_REQUEST, seq_number);
    const uint8_t service_type_uint8 = encryption_service_type;
    EmulateMobileMessage(header, &service_type_uint8, 1);
    // Wait call methods in thread
    sleep(1);
  }

  /*
   * SecurityManger shall send ProtectServiceResponse with PENDING
   * on getting PROTECT_SERVICE_RESPONSE for already protecting service
   */
  TEST_F(SecurityManagerTest,
         GetProtectServiceRequest_ServiceProtectionPending) {
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  ProtectServiceResponseWithId(
                    SecurityQuery::PENDING),is_final)).Times(1);
    const ::protocol_handler::ServiceType encryption_service_type =
        ::protocol_handler::kMobileNav;
    EXPECT_CALL(mock_session_observer,
                SetSSLContext(key, encryption_service_type,_)).
        WillOnce(Return(SecurityQuery::SERVICE_ALREADY_PROTECTED));
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, encryption_service_type)).
        WillOnce(Return(&mock_ssl_context));
    EXPECT_CALL(mock_ssl_context,
                IsInitCompleted()).
        WillOnce(Return(false));

    const SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          SecurityQuery::PROTECT_SERVICE_REQUEST, seq_number);
    const uint8_t service_type_uint8 = encryption_service_type;
    EmulateMobileMessage(header, &service_type_uint8, 1);
    // Wait call methods in thread
    sleep(1);
  }

  /*
   * SecurityManger shall send ProtectServiceResponse with SUCCESS
   * on getting correct PROTECT_SERVICE_RESPONSE
   */
  TEST_F(SecurityManagerTest, GetProtectServiceRequest_Success) {
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  ProtectServiceResponseWithId(
                    SecurityQuery::SUCCESS),is_final)).Times(1);
    const ::protocol_handler::ServiceType encryption_service_type =
        ::protocol_handler::kMobileNav;
    EXPECT_CALL(mock_session_observer,
                SetSSLContext(key, encryption_service_type,_)).
        WillOnce(Return(SecurityQuery::SUCCESS));

    const SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          SecurityQuery::PROTECT_SERVICE_REQUEST, seq_number);
    const uint8_t service_type_uint8 = encryption_service_type;
    EmulateMobileMessage(header, &service_type_uint8, 1);
    // Wait call methods in thread
    sleep(1);
  }

  /*
   * SecurityManger shall send InternallError on
   * getting SEND_HANDSHAKE_DATA from mobile side before
   * PROTECT_SERVICE_RESPONSE
   */
  TEST_F(SecurityManagerTest, SendHandshakeData_WrongDataSize) {
    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_INVALID_QUERY_SIZE),is_final)).Times(1);
    const SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          SecurityQuery::SEND_HANDSHAKE_DATA, seq_number);
    EmulateMobileMessage(header, NULL, 0);
    // Wait call methods in thread
    sleep(1);
  }

} // connection_handle
} // namespace components
} // namespace test
#endif // SECURITY_MANAGER_TEST_H
