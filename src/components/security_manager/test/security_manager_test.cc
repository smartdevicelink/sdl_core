/*
 * Copyright (c) 2015, Ford Motor Company
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

#include <string>
#include <memory>

#include "gtest/gtest.h"
#include "utils/byte_order.h"
#include "protocol/common.h"
#include "security_manager/security_manager_impl.h"

#include "protocol_handler/mock_protocol_handler.h"
#include "protocol_handler/mock_session_observer.h"
#include "security_manager/mock_security_manager.h"
#include "security_manager/mock_ssl_context.h"
#include "security_manager/mock_crypto_manager.h"
#include "security_manager/mock_security_manager_listener.h"
#include "utils/mock_system_time_handler.h"
#include "utils/make_shared.h"
#include "utils/test_async_waiter.h"

namespace test {
namespace components {
namespace security_manager_test {

using protocol_handler::PROTOCOL_VERSION_2;
using protocol_handler::ServiceType;
using protocol_handler::kControl;
using protocol_handler::kRpc;
using protocol_handler::kAudio;
using protocol_handler::kMobileNav;
using protocol_handler::kBulk;
using protocol_handler::kInvalidServiceType;
using protocol_handler::RawMessagePtr;
using protocol_handler::RawMessage;

using security_manager::SecurityQuery;
using security_manager::SSLContext;
using security_manager::SecurityManager;
using security_manager::SecurityManagerImpl;

using security_manager_test::InternalErrorWithErrId;
using ::testing::Return;
using ::testing::ReturnNull;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::_;

namespace {
// Sample data for handshake data emulation
const int32_t kKey = 0x1;
const int32_t kSeqNumber = 0x2;
const ServiceType kSecureServiceType = kControl;
const uint32_t kProtocolVersion = PROTOCOL_VERSION_2;
const bool kIsFinal = false;

const uint8_t handshake_data[] = {0x1, 0x2, 0x3, 0x4, 0x5};
const size_t handshake_data_size =
    sizeof(handshake_data) / sizeof(handshake_data[0]);

uint8_t handshake_data_out[] = {0x6, 0x7, 0x8};
uint8_t* handshake_data_out_pointer = handshake_data_out;
const size_t handshake_data_out_size =
    sizeof(handshake_data_out) / sizeof(handshake_data_out[0]);
const uint32_t kAsyncExpectationsTimeout = 10000u;
}

class SecurityManagerTest : public ::testing::Test {
 protected:
  SecurityManagerTest()
      : mock_system_time_handler(
            std::unique_ptr<MockSystemTimeHandler>(new MockSystemTimeHandler()))
      , security_manager_(
            new SecurityManagerImpl(std::move(mock_system_time_handler))) {}
  void SetUp() OVERRIDE {
    security_manager_->set_session_observer(&mock_session_observer);
    security_manager_->set_protocol_handler(&mock_protocol_handler);
    mock_sm_listener.reset(new testing::StrictMock<
        security_manager_test::MockSecurityManagerListener>());
    security_manager_->AddListener(mock_sm_listener.get());
  }

  void SetMockCryptoManager() {
    EXPECT_CALL(mock_crypto_manager, IsCertificateUpdateRequired(_, _))
        .WillRepeatedly(Return(false));
    security_manager_->set_crypto_manager(&mock_crypto_manager);
  }
  /*
   * Wrapper for fast emulate recieve SecurityManager::OnMessageReceived
   */
  void call_OnMessageReceived(const uint8_t* const data,
                              uint32_t dataSize,
                              const ServiceType serviceType) {
    const RawMessagePtr rawMessagePtr(utils::MakeShared<RawMessage>(
        kKey, kProtocolVersion, data, dataSize, serviceType));
    security_manager_->OnMessageReceived(rawMessagePtr);
  }
  /*
   * Wrapper for fast emulate recieve query
   */
  void EmulateMobileMessage(SecurityQuery::QueryHeader header,
                            const uint8_t* const data,
                            const uint32_t data_size) {
    // convert to Big-Endian (network) order
    const uint32_t query_id = header.query_id << 8;
    header.query_id = LE_TO_BE32(query_id);
    header.json_size = LE_TO_BE32(header.json_size);
    header.seq_number = LE_TO_BE32(header.seq_number);

    const size_t data_sending_size = sizeof(header) + data_size;
    uint8_t* data_sending = new uint8_t[data_sending_size];
    memcpy(data_sending, &header, sizeof(header));
    memcpy(data_sending + sizeof(header), data, data_size);

    call_OnMessageReceived(data_sending, data_sending_size, kControl);
    delete[] data_sending;
  }
  /*
   * Wrapper for fast emulate recieve Handshake
   */
  void EmulateMobileMessageHandshake(const uint8_t* const data,
                                     const uint32_t data_size,
                                     const int repeat_count = 1) {
    const SecurityQuery::QueryHeader header(SecurityQuery::NOTIFICATION,
                                            SecurityQuery::SEND_HANDSHAKE_DATA,
                                            kSeqNumber);
    for (int c = 0; c < repeat_count; ++c) {
      EmulateMobileMessage(header, data, data_size);
    }
  }

  // Strict mocks (same as all methods EXPECT_CALL().Times(0))

  testing::StrictMock<protocol_handler_test::MockSessionObserver>
      mock_session_observer;
  testing::StrictMock<protocol_handler_test::MockProtocolHandler>
      mock_protocol_handler;
  testing::StrictMock<security_manager_test::MockCryptoManager>
      mock_crypto_manager;
  testing::StrictMock<security_manager_test::MockSSLContext>
      mock_ssl_context_new;
  testing::StrictMock<security_manager_test::MockSSLContext>
      mock_ssl_context_exists;
  std::unique_ptr<testing::StrictMock<
      security_manager_test::MockSecurityManagerListener> > mock_sm_listener;
  std::unique_ptr<MockSystemTimeHandler> mock_system_time_handler;
  std::shared_ptr<SecurityManagerImpl> security_manager_;
};
// Test Bodies

/*
 * SecurityManager shall not set NULL interfaces
 * and shall not call any methodes
 */
TEST_F(SecurityManagerTest, SetNULL_Intefaces) {
  security_manager_->set_session_observer(NULL);
  security_manager_->set_protocol_handler(NULL);
  security_manager_->set_crypto_manager(NULL);
  security_manager_->AddListener(NULL);
  // additional check not null config section
  EXPECT_TRUE(SecurityManagerImpl::ConfigSection());
}
/*
 * Add/Remove NULL listeners do not any additional logics
 */
TEST_F(SecurityManagerTest, Listeners_NULL) {
  security_manager_->AddListener(NULL);
  security_manager_->RemoveListener(NULL);
}
/*
 * Twice remove listener
 */
TEST_F(SecurityManagerTest, Listeners_TwiceRemoveListeners) {
  security_manager_->RemoveListener(mock_sm_listener.get());
  security_manager_->RemoveListener(mock_sm_listener.get());
}
/*
 * Add and remove listeners
 */
TEST_F(SecurityManagerTest, Listeners_NoListeners) {
  // Check correct removing listener
  security_manager_->RemoveListener(mock_sm_listener.get());

  // Expect no calls
  testing::StrictMock<MockSecurityManagerListener> mock_listener2;
  security_manager_->AddListener(&mock_listener2);
  security_manager_->RemoveListener(&mock_listener2);

  security_manager_->NotifyListenersOnHandshakeDone(
      kKey, SSLContext::Handshake_Result_Success);
  security_manager_->NotifyListenersOnHandshakeDone(
      kKey, SSLContext::Handshake_Result_Fail);
}
/*
 * Notifying two listeners
 */
TEST_F(SecurityManagerTest, Listeners_Notifying) {
  // Check correct removing listener
  security_manager_->RemoveListener(mock_sm_listener.get());

  testing::StrictMock<MockSecurityManagerListener>* mock_listener1 =
      new testing::StrictMock<MockSecurityManagerListener>();
  testing::StrictMock<MockSecurityManagerListener>* mock_listener2 =
      new testing::StrictMock<MockSecurityManagerListener>();

  const SSLContext::HandshakeResult first_call_value =
      SSLContext::Handshake_Result_Success;
  // Expect call both listeners on 1st call
  EXPECT_CALL(*mock_listener1, OnHandshakeDone(kKey, first_call_value))
      .
      // Emulate false (reject) result
      WillOnce(Return(false));
  EXPECT_CALL(*mock_listener2, OnHandshakeDone(kKey, first_call_value))
      .
      // Emulate true (accept) result
      WillOnce(Return(true));

  // First listener was not removed from listener list
  // So this callback wil lbe either call.
  EXPECT_CALL(*mock_listener1, OnCertificateUpdateRequired());

  const SSLContext::HandshakeResult second_call_value =
      SSLContext::Handshake_Result_Fail;
  // Expect call last listener on 2d call
  EXPECT_CALL(*mock_listener1, OnHandshakeDone(kKey, second_call_value))
      .
      // Emulate false (reject) result
      WillOnce(Return(true));

  // Expect no call 3d call

  security_manager_->AddListener(mock_listener1);
  security_manager_->AddListener(mock_listener2);
  // 1st call
  security_manager_->NotifyListenersOnHandshakeDone(kKey, first_call_value);
  security_manager_->NotifyOnCertificateUpdateRequired();
  // 2nd call
  security_manager_->NotifyListenersOnHandshakeDone(kKey, second_call_value);
  security_manager_->NotifyOnCertificateUpdateRequired();
  // 3nd call
  security_manager_->NotifyListenersOnHandshakeDone(
      kKey, SSLContext::Handshake_Result_Fail);
  security_manager_->NotifyOnCertificateUpdateRequired();
}

/*
 * SecurityManager with NULL CryptoManager shall send
 * InternallError (ERROR_NOT_SUPPORTED) on any Query
 */
TEST_F(SecurityManagerTest, SecurityManager_NULLCryptoManager) {
  // Expect InternalError with ERROR_ID
  uint32_t connection_id = 0;
  uint8_t session_id = 0;

  TestAsyncWaiter waiter;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _));
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(true));

  EXPECT_CALL(mock_protocol_handler,
              SendMessageToMobileApp(
                  InternalErrorWithErrId(SecurityManager::ERROR_NOT_SUPPORTED),
                  kIsFinal)).WillOnce(NotifyTestAsyncWaiter(&waiter));
  const SecurityQuery::QueryHeader header(SecurityQuery::REQUEST,
                                          // It could be any query id
                                          SecurityQuery::INVALID_QUERY_ID);
  const uint8_t data = 0;
  EmulateMobileMessage(header, &data, 1);

  EXPECT_TRUE(waiter.WaitFor(1, kAsyncExpectationsTimeout));
}
/*
 * Shall skip all OnMobileMessageSent
 */
TEST_F(SecurityManagerTest, OnMobileMessageSent) {
  const uint8_t* data_param = NULL;
  const RawMessagePtr rawMessagePtr(
      utils::MakeShared<RawMessage>(kKey, kProtocolVersion, data_param, 0));
  security_manager_->OnMobileMessageSent(rawMessagePtr);
}
/*
 * Shall skip all not-Secure messages
 */
TEST_F(SecurityManagerTest, GetWrongServiceType) {
  // Call with wrong Service type
  call_OnMessageReceived(NULL, 0, kRpc);
  call_OnMessageReceived(NULL, 0, kAudio);
  call_OnMessageReceived(NULL, 0, kMobileNav);
  call_OnMessageReceived(NULL, 0, kBulk);
  call_OnMessageReceived(NULL, 0, kInvalidServiceType);
}
/*
 * Shall send InternallError on null data recieved
 */
TEST_F(SecurityManagerTest, GetEmptyQuery) {
  uint32_t connection_id = 0;
  uint8_t session_id = 0;
  // uint8_t protocol_version = 0;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _));
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(true));

  EXPECT_CALL(
      mock_protocol_handler,
      SendMessageToMobileApp(
          InternalErrorWithErrId(SecurityManager::ERROR_INVALID_QUERY_SIZE),
          kIsFinal));
  // Call with NULL data
  call_OnMessageReceived(NULL, 0, kSecureServiceType);
}
/*
 * Shall send InternallError on null data recieved
 */
TEST_F(SecurityManagerTest, GetWrongJSONSize) {
  SetMockCryptoManager();
  uint32_t connection_id = 0;
  uint8_t session_id = 0;
  // uint8_t protocol_version = 0;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _));
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(true));
  // Expect InternalError with ERROR_ID
  EXPECT_CALL(
      mock_protocol_handler,
      SendMessageToMobileApp(
          InternalErrorWithErrId(SecurityManager::ERROR_INVALID_QUERY_SIZE),
          kIsFinal));
  SecurityQuery::QueryHeader header(SecurityQuery::REQUEST,
                                    SecurityQuery::INVALID_QUERY_ID);
  header.json_size = 0x0FFFFFFF;
  EmulateMobileMessage(header, NULL, 0);
}
/*
 * Shall send InternallError on INVALID_QUERY_ID
 */
TEST_F(SecurityManagerTest, GetInvalidQueryId) {
  SetMockCryptoManager();
  uint32_t connection_id = 0;
  uint8_t session_id = 0;

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times++;

  // Expect InternalError with ERROR_ID
  EXPECT_CALL(
      mock_protocol_handler,
      SendMessageToMobileApp(
          InternalErrorWithErrId(SecurityManager::ERROR_INVALID_QUERY_ID),
          kIsFinal)).WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  const SecurityQuery::QueryHeader header(SecurityQuery::REQUEST,
                                          SecurityQuery::INVALID_QUERY_ID);
  const uint8_t data = 0;
  EmulateMobileMessage(header, &data, 1);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * Shall send Internall Error on call
 * CreateSSLContext for already protected connections
 */
TEST_F(SecurityManagerTest, CreateSSLContext_ServiceAlreadyProtected) {
  SetMockCryptoManager();

  // Return mock SSLContext
  EXPECT_CALL(mock_session_observer, GetSSLContext(kKey, kControl))
      .WillOnce(Return(&mock_ssl_context_new));

  const SSLContext* result = security_manager_->CreateSSLContext(
      kKey,
      security_manager::SecurityManager::ContextCreationStrategy::kUseExisting);
  EXPECT_EQ(&mock_ssl_context_new, result);
}
/*
 * Shall send Internall Error on error create SSL
 */
TEST_F(SecurityManagerTest, CreateSSLContext_ErrorCreateSSL) {
  SetMockCryptoManager();
  // Expect InternalError with ERROR_ID
  uint32_t connection_id = 0;
  uint8_t session_id = 0;
  // uint8_t protocol_version = 0;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _));
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_protocol_handler,
      SendMessageToMobileApp(
          InternalErrorWithErrId(SecurityManager::ERROR_INTERNAL), kIsFinal));

  // Emulate SessionObserver and CryptoManager result
  EXPECT_CALL(mock_session_observer, GetSSLContext(kKey, kControl))
      .WillOnce(ReturnNull());
  EXPECT_CALL(mock_crypto_manager, CreateSSLContext()).WillOnce(ReturnNull());

  const SSLContext* result = security_manager_->CreateSSLContext(
      kKey,
      security_manager::SecurityManager::ContextCreationStrategy::kUseExisting);
  EXPECT_EQ(NULL, result);
}
/*
 * Shall send InternalError with SERVICE_NOT_FOUND
 * on getting any Error with call SetSSLContext
 */
TEST_F(SecurityManagerTest, CreateSSLContext_SetSSLContextError) {
  SetMockCryptoManager();
  // Expect InternalError with ERROR_ID
  uint32_t connection_id = 0;
  uint8_t session_id = 0;
  // uint8_t protocol_version = 0;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _));
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(true));

  EXPECT_CALL(
      mock_protocol_handler,
      SendMessageToMobileApp(
          InternalErrorWithErrId(SecurityManager::ERROR_UNKNOWN_INTERNAL_ERROR),
          kIsFinal));

  // Emulate SessionObserver and CryptoManager result
  EXPECT_CALL(mock_session_observer, GetSSLContext(kKey, kControl))
      .WillOnce(ReturnNull());
  EXPECT_CALL(mock_crypto_manager, CreateSSLContext())
      .WillOnce(Return(&mock_ssl_context_new));
  EXPECT_CALL(mock_crypto_manager, ReleaseSSLContext(&mock_ssl_context_new));
  EXPECT_CALL(mock_session_observer, SetSSLContext(kKey, &mock_ssl_context_new))
      .WillOnce(Return(SecurityManager::ERROR_UNKNOWN_INTERNAL_ERROR));

  const SSLContext* result = security_manager_->CreateSSLContext(
      kKey,
      security_manager::SecurityManager::ContextCreationStrategy::kUseExisting);
  EXPECT_EQ(NULL, result);
}
/*
 * Shall protect connection on correct call CreateSSLContext
 */
TEST_F(SecurityManagerTest, CreateSSLContext_Success) {
  SetMockCryptoManager();
  // Expect no Errors
  // Expect no notifying listeners - it will be done after handshake

  // Emulate SessionObserver and CryptoManager result
  EXPECT_CALL(mock_session_observer, GetSSLContext(kKey, kControl))
      .WillOnce(ReturnNull())
      .
      // additional check for debug code
      WillOnce(Return(&mock_ssl_context_exists));
  EXPECT_CALL(mock_crypto_manager, CreateSSLContext())
      .WillOnce(Return(&mock_ssl_context_new));
  EXPECT_CALL(mock_session_observer, SetSSLContext(kKey, &mock_ssl_context_new))
      .WillOnce(Return(SecurityManager::ERROR_SUCCESS));

  const SSLContext* result = security_manager_->CreateSSLContext(
      kKey,
      security_manager::SecurityManager::ContextCreationStrategy::kUseExisting);
  EXPECT_EQ(&mock_ssl_context_new, result);
}
/*
 * Shall send InternallError on call StartHandshake for uprotected service
 */
TEST_F(SecurityManagerTest, StartHandshake_ServiceStillUnprotected) {
  SetMockCryptoManager();
  uint32_t connection_id = 0;
  uint8_t session_id = 0;
  // uint8_t protocol_version = 0;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _));
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(true));
  // Expect InternalError with ERROR_INTERNAL
  EXPECT_CALL(
      mock_protocol_handler,
      SendMessageToMobileApp(
          InternalErrorWithErrId(SecurityManager::ERROR_INTERNAL), kIsFinal));
  // Expect notifying listeners (unsuccess)
  EXPECT_CALL(*mock_sm_listener,
              OnHandshakeDone(kKey, SSLContext::Handshake_Result_Fail))
      .WillOnce(Return(true));

  // Emulate SessionObserver result
  EXPECT_CALL(mock_session_observer, GetSSLContext(kKey, kControl))
      .WillOnce(ReturnNull());

  security_manager_->StartHandshake(kKey);

  // Listener was destroyed after OnHandshakeDone call
  mock_sm_listener.release();
}
/*
 * Shall send InternallError on SSL error and notify listeners
 */
TEST_F(SecurityManagerTest, StartHandshake_SSLInternalError) {
  SetMockCryptoManager();

  uint32_t connection_id = 0;
  uint8_t session_id = 0;

  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _));
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(true));
  // Expect notifying listeners (unsuccess)
  EXPECT_CALL(*mock_sm_listener,
              OnHandshakeDone(kKey, SSLContext::Handshake_Result_Fail))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_session_observer, GetSSLContext(kKey, kControl))
      .WillOnce(ReturnNull());
  // Expect InternalError with ERROR_ID
  EXPECT_CALL(
      mock_protocol_handler,
      SendMessageToMobileApp(
          InternalErrorWithErrId(SecurityManager::ERROR_INTERNAL), kIsFinal));

  security_manager_->StartHandshake(kKey);
  mock_sm_listener.release();
}

/*
 * Shall send InternallError on
 * getting SEND_HANDSHAKE_DATA with NULL data
 */
TEST_F(SecurityManagerTest, ProccessHandshakeData_WrongDataSize) {
  SetMockCryptoManager();
  uint32_t connection_id = 0;
  uint8_t session_id = 0;

  TestAsyncWaiter waiter;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _));
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(true));

  // Expect InternalError with ERROR_ID
  EXPECT_CALL(
      mock_protocol_handler,
      SendMessageToMobileApp(
          InternalErrorWithErrId(SecurityManager::ERROR_INVALID_QUERY_SIZE),
          kIsFinal)).WillOnce(NotifyTestAsyncWaiter(&waiter));

  EmulateMobileMessageHandshake(NULL, 0);

  EXPECT_TRUE(waiter.WaitFor(1, kAsyncExpectationsTimeout));
}
/*
 * Shall send InternallError on
 * getting SEND_HANDSHAKE_DATA from mobile side
 * for service which is not protected
 */
TEST_F(SecurityManagerTest,
       DISABLED_ProccessHandshakeData_ServiceNotProtected) {
  SetMockCryptoManager();
  // Expect InternalError with ERROR_ID
  uint32_t connection_id = 0;
  uint8_t session_id = 0;

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times++;
  EXPECT_CALL(
      mock_protocol_handler,
      SendMessageToMobileApp(
          InternalErrorWithErrId(SecurityManager::ERROR_SERVICE_NOT_PROTECTED),
          kIsFinal)).WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  // Expect notifying listeners (unsuccess)
  EXPECT_CALL(*mock_sm_listener,
              OnHandshakeDone(kKey, SSLContext::Handshake_Result_Fail))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times++;

  // Emulate SessionObserver result
  EXPECT_CALL(mock_session_observer, GetSSLContext(kKey, kControl))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), ReturnNull()));
  times++;

  const uint8_t data[] = {0x1, 0x2};
  EmulateMobileMessageHandshake(data, sizeof(data) / sizeof(data[0]));

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));

  // Listener was destroyed after OnHandshakeDone call
  mock_sm_listener.release();
}
/*
 * Shall send InternallError on getting
 * SEND_HANDSHAKE_DATA from mobile side with invalid handshake
 * data (DoHandshakeStep return NULL pointer)
 */
TEST_F(SecurityManagerTest, ProccessHandshakeData_InvalidData) {
  SetMockCryptoManager();

  // Count handshake calls
  const int handshake_emulates = 4;

  uint32_t connection_id = 0;
  uint8_t session_id = 0;

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _))
      .Times(handshake_emulates)
      .WillRepeatedly(NotifyTestAsyncWaiter(&waiter));
  times += handshake_emulates;
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .Times(handshake_emulates)
      .WillRepeatedly(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times += handshake_emulates;

  // Expect InternalError with ERROR_ID
  EXPECT_CALL(
      mock_protocol_handler,
      SendMessageToMobileApp(
          InternalErrorWithErrId(SecurityManager::ERROR_SSL_INVALID_DATA),
          kIsFinal))
      .Times(handshake_emulates)
      .WillRepeatedly(NotifyTestAsyncWaiter(&waiter));
  times += handshake_emulates;
  // Expect notifying listeners (unsuccess)
  EXPECT_CALL(*mock_sm_listener,
              OnHandshakeDone(kKey, SSLContext::Handshake_Result_Fail))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times++;

  // Emulate SessionObserver and CryptoManager result
  EXPECT_CALL(mock_session_observer, GetSSLContext(kKey, kControl))
      .Times(handshake_emulates)
      .WillRepeatedly(Return(&mock_ssl_context_exists));

  // Emulate DoHandshakeStep fail logics
  EXPECT_CALL(
      mock_ssl_context_exists,
      DoHandshakeStep(HandshakeStepEq(handshake_data, handshake_data_size),
                      handshake_data_size,
                      _,
                      _))
      .WillOnce(DoAll(SetArgPointee<2>(handshake_data_out_pointer),
                      SetArgPointee<3>(handshake_data_out_size),
                      NotifyTestAsyncWaiter(&waiter),
                      Return(SSLContext::Handshake_Result_AbnormalFail)))
      .WillOnce(DoAll(SetArgPointee<2>((uint8_t*)NULL),
                      SetArgPointee<3>(handshake_data_out_size),
                      NotifyTestAsyncWaiter(&waiter),
                      Return(SSLContext::Handshake_Result_AbnormalFail)))
      .WillOnce(DoAll(SetArgPointee<2>(handshake_data_out_pointer),
                      SetArgPointee<3>(0),
                      NotifyTestAsyncWaiter(&waiter),
                      Return(SSLContext::Handshake_Result_AbnormalFail)))
      .WillOnce(DoAll(SetArgPointee<2>((uint8_t*)NULL),
                      SetArgPointee<3>(0),
                      NotifyTestAsyncWaiter(&waiter),
                      Return(SSLContext::Handshake_Result_AbnormalFail)));
  times += 4;  // matches to each single call above

  // On each wrong handshake will be asked error
  EXPECT_CALL(mock_ssl_context_exists, LastError()).Times(handshake_emulates);

  // Emulate handshare #handshake_emulates times for 5 cases
  EmulateMobileMessageHandshake(
      handshake_data, handshake_data_size, handshake_emulates);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));

  // Listener was destroyed after OnHandshakeDone call
  mock_sm_listener.release();
}
/*
 * Shall send HandshakeData on getting SEND_HANDSHAKE_DATA from mobile side
 * with correct handshake data Check Fail and sussecc states
 */
TEST_F(SecurityManagerTest, ProccessHandshakeData_Answer) {
  SetMockCryptoManager();
  // Count handshake calls
  const int handshake_emulates = 2;

  uint32_t connection_id = 0;
  uint8_t session_id = 0;

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _))
      .Times(handshake_emulates)
      .WillRepeatedly(NotifyTestAsyncWaiter(&waiter));
  times += handshake_emulates;
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .Times(handshake_emulates)
      .WillRepeatedly(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times += handshake_emulates;

  // Get size of raw message after
  const size_t raw_message_size = 15;
  EXPECT_CALL(
      mock_protocol_handler,
      SendMessageToMobileApp(RawMessageEqSize(raw_message_size), kIsFinal))
      .Times(handshake_emulates)
      .WillRepeatedly(NotifyTestAsyncWaiter(&waiter));
  times += handshake_emulates;

  // Expect notifying listeners (unsuccess)
  EXPECT_CALL(*mock_sm_listener,
              OnHandshakeDone(kKey, SSLContext::Handshake_Result_Fail))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times++;

  // Emulate SessionObserver and CryptoManager result
  EXPECT_CALL(mock_ssl_context_exists, IsInitCompleted())
      .Times(handshake_emulates)
      .WillRepeatedly(DoAll(NotifyTestAsyncWaiter(&waiter), Return(false)));
  times += handshake_emulates;
  EXPECT_CALL(mock_session_observer, GetSSLContext(kKey, kControl))
      .Times(handshake_emulates)
      .WillRepeatedly(DoAll(NotifyTestAsyncWaiter(&waiter),
                            Return(&mock_ssl_context_exists)));
  times += handshake_emulates;

  // Emulate DoHandshakeStep correct logics
  EXPECT_CALL(
      mock_ssl_context_exists,
      DoHandshakeStep(HandshakeStepEq(handshake_data, handshake_data_size),
                      handshake_data_size,
                      _,
                      _))
      .WillOnce(DoAll(SetArgPointee<2>(handshake_data_out_pointer),
                      SetArgPointee<3>(handshake_data_out_size),
                      NotifyTestAsyncWaiter(&waiter),
                      Return(SSLContext::Handshake_Result_Success)))
      .WillOnce(DoAll(SetArgPointee<2>(handshake_data_out_pointer),
                      SetArgPointee<3>(handshake_data_out_size),
                      NotifyTestAsyncWaiter(&waiter),
                      Return(SSLContext::Handshake_Result_Fail)));
  times += 2;  // matches to each single call above

  EmulateMobileMessageHandshake(
      handshake_data, handshake_data_size, handshake_emulates);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));

  // Listener was destroyed after OnHandshakeDone call
  mock_sm_listener.release();
}
/*
 * Shall call all listeners on success end handshake
 * and return handshake data
 * Check Fail and sussecc states
 */
TEST_F(SecurityManagerTest, ProccessHandshakeData_HandshakeFinished) {
  SetMockCryptoManager();
  // Count handshake calls
  const int handshake_emulates = 6;

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  // Expect no errors
  // Expect notifying listeners (success)
  EXPECT_CALL(*mock_sm_listener,
              OnHandshakeDone(kKey, SSLContext::Handshake_Result_Success))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times++;

  // Emulate SessionObserver and CryptoManager result
  EXPECT_CALL(mock_session_observer, GetSSLContext(kKey, kControl))
      .Times(handshake_emulates)
      .WillRepeatedly(DoAll(NotifyTestAsyncWaiter(&waiter),
                            Return(&mock_ssl_context_exists)));
  times += handshake_emulates;
  EXPECT_CALL(mock_ssl_context_exists, IsInitCompleted())
      .Times(handshake_emulates)
      .WillRepeatedly(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times += handshake_emulates;

  EXPECT_CALL(
      mock_ssl_context_exists,
      DoHandshakeStep(HandshakeStepEq(handshake_data, handshake_data_size),
                      handshake_data_size,
                      _,
                      _))
      .
      // two states with correct out data
      WillOnce(DoAll(SetArgPointee<2>(handshake_data_out_pointer),
                     SetArgPointee<3>(handshake_data_out_size),
                     NotifyTestAsyncWaiter(&waiter),
                     Return(SSLContext::Handshake_Result_Success)))
      .WillOnce(DoAll(SetArgPointee<2>(handshake_data_out_pointer),
                      SetArgPointee<3>(handshake_data_out_size),
                      NotifyTestAsyncWaiter(&waiter),
                      Return(SSLContext::Handshake_Result_Fail)))
      .
      // two states with with null pointer data
      WillOnce(DoAll(SetArgPointee<2>((uint8_t*)NULL),
                     SetArgPointee<3>(handshake_data_out_size),
                     NotifyTestAsyncWaiter(&waiter),
                     Return(SSLContext::Handshake_Result_Success)))
      .WillOnce(DoAll(SetArgPointee<2>((uint8_t*)NULL),
                      SetArgPointee<3>(handshake_data_out_size),
                      NotifyTestAsyncWaiter(&waiter),
                      Return(SSLContext::Handshake_Result_Fail)))
      .
      // two states with with null data size
      WillOnce(DoAll(SetArgPointee<2>(handshake_data_out_pointer),
                     SetArgPointee<3>(0),
                     NotifyTestAsyncWaiter(&waiter),
                     Return(SSLContext::Handshake_Result_Success)))
      .WillOnce(DoAll(SetArgPointee<2>(handshake_data_out_pointer),
                      SetArgPointee<3>(0),
                      NotifyTestAsyncWaiter(&waiter),
                      Return(SSLContext::Handshake_Result_Success)));
  times += 6;  // matches to each single call above

  // Expect send two message (with correct pointer and size data)

  uint32_t connection_id = 0;
  uint8_t session_id = 0;
  // uint8_t protocol_version = 0;
  EXPECT_CALL(mock_session_observer, PairFromKey(kKey, _, _)).Times(2);
  EXPECT_CALL(mock_session_observer,
              ProtocolVersionUsed(connection_id, session_id, _))
      .Times(2)
      .WillRepeatedly(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times += 2;  // matches to the number above

  EXPECT_CALL(mock_protocol_handler, SendMessageToMobileApp(_, kIsFinal))
      .Times(2)
      .WillRepeatedly(NotifyTestAsyncWaiter(&waiter));
  times += 2;  // matches to the number above

  // Expect NO InternalError with ERROR_ID
  EmulateMobileMessageHandshake(
      handshake_data, handshake_data_size, handshake_emulates);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));

  // Listener was destroyed after OnHandshakeDone call
  mock_sm_listener.release();
}
/*
 * Shall not any query on getting empty SEND_INTERNAL_ERROR
 */
TEST_F(SecurityManagerTest, GetInternalError_NullData) {
  SetMockCryptoManager();

  const SecurityQuery::QueryHeader header(
      SecurityQuery::NOTIFICATION, SecurityQuery::SEND_INTERNAL_ERROR, 0);
  EmulateMobileMessage(header, NULL, 0);
}
/*
 * Shall not send any query on getting SEND_INTERNAL_ERROR
 */
TEST_F(SecurityManagerTest, GetInternalError) {
  SetMockCryptoManager();

  const SecurityQuery::QueryHeader header(
      SecurityQuery::NOTIFICATION, SecurityQuery::SEND_INTERNAL_ERROR, 0);
  const uint8_t data[] = {0x1, 0x2};
  EmulateMobileMessage(header, data, sizeof(data) / sizeof(data[0]));
}
/*
 * Shall not send any query on getting SEND_INTERNAL_ERROR with error string
 */
TEST_F(SecurityManagerTest, GetInternalError_WithErrText) {
  SetMockCryptoManager();

  SecurityQuery::QueryHeader header(
      SecurityQuery::NOTIFICATION, SecurityQuery::SEND_INTERNAL_ERROR, 0);
  std::string error("JSON wrong string");
  header.json_size = error.size();
  EmulateMobileMessage(
      header, reinterpret_cast<const uint8_t*>(error.c_str()), error.size());
}
/*
 * Shall not send any query on getting SEND_INTERNAL_ERROR with error string
 */
TEST_F(SecurityManagerTest, GetInternalError_WithErrJSONText) {
  SetMockCryptoManager();

  SecurityQuery::QueryHeader header(
      SecurityQuery::NOTIFICATION, SecurityQuery::SEND_INTERNAL_ERROR, 0);
  std::string error(" { \"id\": 1 } ");
  header.json_size = error.size();
  EmulateMobileMessage(
      header, reinterpret_cast<const uint8_t*>(error.c_str()), error.size());
}

}  // namespace security_manager_test
}  // namespace components
}  // namespace test
