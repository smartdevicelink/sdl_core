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
#include "protocol_handler/protocol_packet.h"

//TODO: add test on get correct/wrong InternalError
//TODO: check connection_key the same and seq_number
//TODO: check ::SendData with correct query_id and query_type
namespace test  {
namespace components  {
namespace security_manager_test {

  using ::protocol_handler::RawMessage;
  using ::protocol_handler::RawMessagePtr;
  using ::security_manager::SecurityQuery;
  using ::testing::Return;
  using ::testing::ReturnNull;
  using ::testing::ReturnPointee;
  using ::testing::DoAll;
  using ::testing::ElementsAreArray;
  using ::testing::SetArgPointee;
  using ::testing::_;

  class SecurityManagerTest: public ::testing::Test {
   protected:
    void SetUp() OVERRIDE {
      security_manager_.reset(new security_manager::SecurityManager());
      security_manager_->set_session_observer(&mock_session_observer);
      security_manager_->set_protocol_handler(&mock_protocol_observer);
      security_manager_->AddListener(&mock_sm_listener);
    }
    void TearDown() OVERRIDE {
      // Wait call methods in thread
      usleep(5000);
      //Strict mocks are the same as EXPECT_CALL(ALL).Times(0)
    }

    void SetMockCryptoManger() {
      security_manager_->set_crypto_manager(&mock_crypto_manager);
    }
    /*
    * Wrapper for fast emulate recieve SecurityManager::OnMessageReceived
    */
    void call_OnMessageReceived(const uint8_t* const data, uint32_t dataSize,
                                const protocol_handler::ServiceType serviceType) {
      const RawMessagePtr rawMessagePtr(
            new RawMessage( key, protocolVersion, data, dataSize, serviceType));
      security_manager_->OnMessageReceived(rawMessagePtr);
    }
    /*
    * Wrapper for fast emulate recieve query
    */
    void EmulateMobileMessage(SecurityQuery::QueryHeader header,
                              const uint8_t* const data, const uint32_t data_size ){
      //convert to Big-Endian (network) order
      header.query_id  = LE_TO_BE32(header.query_id << 8);
      header.json_size = LE_TO_BE32(header.json_size);

      const size_t data_sending_size = sizeof(header) + data_size;
      uint8_t* data_sending = new uint8_t[data_sending_size];
      memcpy(data_sending, &header, sizeof(header));
      memcpy(data_sending + sizeof(header), data, data_size);

      call_OnMessageReceived(data_sending, data_sending_size,
                             protocol_handler::kControl);
      delete[] data_sending;
    }
    /*
    * Wrapper for fast emulate recieve Handshake
    */
    void EmulateMobileMessageHandShake(const uint8_t* const data,
                                       const uint32_t data_size,
                                       const int repeat_count = 1){
      const SecurityQuery::QueryHeader header(
            SecurityQuery::NOTIFICATION,
            SecurityQuery::SEND_HANDSHAKE_DATA, seq_number);
      for (int c = 0; c < repeat_count; ++c) {
        EmulateMobileMessage(header, data, data_size);
      }
    }
    ::utils::SharedPtr<security_manager::SecurityManager> security_manager_;
    // Strict mocks (same as all methods EXPECT_CALL().Times(0))
    testing::StrictMock<SessionObserverMock>   mock_session_observer;
    testing::StrictMock<ProtocoloObserverMock> mock_protocol_observer;
    testing::StrictMock<CryptoManagerMock>     mock_crypto_manager;
    testing::StrictMock<SSLContextMock>        mock_ssl_context_new;
    testing::StrictMock<SSLContextMock>        mock_ssl_context_exists;
    testing::StrictMock<SMListenerMock>        mock_sm_listener;
    SMListenerMock                             mock_sm_listener2;
    // constants
    const int32_t key = 0x1;
    const int32_t seq_number = 0x2;
    const protocol_handler::ServiceType secureServiceType = protocol_handler::kControl;
    const uint32_t protocolVersion = protocol_handler::PROTOCOL_VERSION_2;
    const bool is_final = false;
  };

  // Sample data for handshake data emulation
  namespace {
      const uint8_t handshake_data[] = {0x1, 0x2, 0x3, 0x4, 0x5};
      const size_t handshake_data_size =
          sizeof(handshake_data)/sizeof(handshake_data[0]);

      uint8_t handshake_data_out[] = {0x6, 0x7, 0x8};
      uint8_t *handshake_data_out_pointer = handshake_data_out;
      const size_t handshake_data_out_size =
          sizeof(handshake_data_out)/sizeof(handshake_data_out[0]);
    }
  //Test Bodies

  /*
   * SecurityManager shall not set NULL interfaces
   * and shall not call any methodes
   */
  TEST_F(SecurityManagerTest, SetNULL_Intefaces) {
    security_manager_.reset(new security_manager::SecurityManager());
    security_manager_->set_session_observer(NULL);
    security_manager_->set_protocol_handler(NULL);
    security_manager_->set_crypto_manager(NULL);
    security_manager_->AddListener(NULL);
    //additional check not null config section
    EXPECT_TRUE(security_manager::SecurityManager::ConfigSection());
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
    security_manager_->RemoveListener(&mock_sm_listener);
    security_manager_->RemoveListener(&mock_sm_listener);
  }
  /*
   * Add and remove listeners
   */
  TEST_F(SecurityManagerTest, Listeners_NoListeners) {
    //Check correct removing listener
    security_manager_->RemoveListener(&mock_sm_listener);

    //Expect no calls
    testing::StrictMock<SMListenerMock> mock_listener2;
    security_manager_->AddListener(&mock_listener2);
    security_manager_->RemoveListener(&mock_listener2);

    security_manager_->NotifyListenersOnHandshakeDone(key, true);
    security_manager_->NotifyListenersOnHandshakeDone(key, false);
  }
  /*
   * Notifying two listeners
   */
  TEST_F(SecurityManagerTest, Listeners_Notifying) {
    //Check correct removing listener
    security_manager_->RemoveListener(&mock_sm_listener);

    testing::StrictMock<SMListenerMock> mock_listener1;
    testing::StrictMock<SMListenerMock> mock_listener2;

    const bool first_call_value = true;
    //Expect call both listeners on 1st call
    EXPECT_CALL(mock_listener1,
                OnHandshakeDone(key, first_call_value)).
        //Emulate false (reject) result
        WillOnce(Return(false));
    EXPECT_CALL(mock_listener2,
                OnHandshakeDone(key, first_call_value)).
        //Emulate true (assept) result
        WillOnce(Return(true));

    const bool second_call_value = false;
    //Expect call last listener on 2d call
    EXPECT_CALL(mock_listener1,
                OnHandshakeDone(key, second_call_value)).
        //Emulate false (reject) result
        WillOnce(Return(true));

    //Expect no call 3d call

    security_manager_->AddListener(&mock_listener1);
    security_manager_->AddListener(&mock_listener2);
    //1st call
    security_manager_->NotifyListenersOnHandshakeDone(key, first_call_value);
    //2nd call
    security_manager_->NotifyListenersOnHandshakeDone(key, second_call_value);
    //3nd call
    security_manager_->NotifyListenersOnHandshakeDone(key, false);

  }
  /*
   * SecurityManager with NULL CryptoManager shall send
   * InternallError (ERROR_NOT_SUPPORTED) on any Query
   */
  TEST_F(SecurityManagerTest, SecurityManager_NULLCryptoManager) {
    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_NOT_SUPPORTED),is_final)).
        Times(1);
    const SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          //It could be any query id
          SecurityQuery::INVALID_QUERY_ID);
    const uint8_t data = 0;
    EmulateMobileMessage(header, &data, 1);
  }
  /*
   * Shall skip all OnMobileMessageSent
   */
  TEST_F(SecurityManagerTest, OnMobileMessageSent) {
    const RawMessagePtr rawMessagePtr(
          new RawMessage( key, protocolVersion, NULL, 0));
    security_manager_->OnMobileMessageSent(rawMessagePtr);
    //Strict mocks are the same as EXPECT_CALL(ALL).Times(0)
  }
  /*
   * Shall skip all not-Secure messages
   */
  TEST_F(SecurityManagerTest, GetWrongServiceType) {
    // Call with wrong Service type
    call_OnMessageReceived(NULL, 0, protocol_handler::kRpc);
    call_OnMessageReceived(NULL, 0, protocol_handler::kAudio);
    call_OnMessageReceived(NULL, 0, protocol_handler::kMobileNav);
    call_OnMessageReceived(NULL, 0, protocol_handler::kBulk);
    call_OnMessageReceived(NULL, 0, protocol_handler::kInvalidServiceType);
    //Strict mocks are the same as EXPECT_CALL(ALL).Times(0)
  }
  /*
   * Shall send InternallError on null data recieved
   */
  TEST_F(SecurityManagerTest, GetEmptyQuery) {
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_INVALID_QUERY_SIZE), is_final)).
        Times(1);
    // Call with NULL data
    call_OnMessageReceived(NULL, 0, secureServiceType);
  }
  /*
   * Shall send InternallError on null data recieved
   */
  TEST_F(SecurityManagerTest, GetWrongJSONSize) {
    SetMockCryptoManger();
    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_INVALID_QUERY_SIZE),is_final)).
        Times(1);
    SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          SecurityQuery::INVALID_QUERY_ID);
    header.json_size = 0x0FFFFFFF;
    EmulateMobileMessage(header, NULL, 0);
  }
  /*
   * Shall send InternallError on INVALID_QUERY_ID
   */
  TEST_F(SecurityManagerTest, GetInvalidQueryId) {
    SetMockCryptoManger();
    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_INVALID_QUERY_ID),is_final)).
        Times(1);
    const SecurityQuery::QueryHeader header(
          SecurityQuery::REQUEST,
          SecurityQuery::INVALID_QUERY_ID);
    const uint8_t data = 0;
    EmulateMobileMessage(header, &data, 1);
  }
  /*
   * Shall send Internall Error on call
   * ProtectConnection for already protected connections
   */
  TEST_F(SecurityManagerTest, ProtectConnection_ServiceAlreadyProtected) {
    SetMockCryptoManger();
    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_SERVICE_ALREADY_PROTECTED),is_final)).
        Times(1);
    //Expect notifying listeners (unsuccess)
    EXPECT_CALL(mock_sm_listener,
                OnHandshakeDone(key, false)).
        WillOnce(Return(true));

    //Emulate SessionObserver result
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, protocol_handler::kControl)).
        WillOnce(Return(&mock_ssl_context_new));

    const bool rezult = security_manager_->ProtectConnection(key);
    EXPECT_FALSE(rezult);
  }
  /*
   * Shall send Internall Error on error create SSL
   */
  TEST_F(SecurityManagerTest, ProtectConnection_ErrorCreateSSL) {
    SetMockCryptoManger();
    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_CREATE_SLL),is_final)).
        Times(1);
    //Expect notifying listeners (unsuccess)
    EXPECT_CALL(mock_sm_listener,
                OnHandshakeDone(key, false)).
        WillOnce(Return(true));

    //Emulate SessionObserver and CryptoManager result
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, protocol_handler::kControl)).
        WillOnce(ReturnNull());
    EXPECT_CALL(mock_crypto_manager,
                CreateSSLContext()).
        WillOnce(ReturnNull());

    const bool rezult = security_manager_->ProtectConnection(key);
    EXPECT_FALSE(rezult);
  }
  /*
   * Shall send InternalError with SERVICE_NOT_FOUND
   * on getting any Error with call SetSSLContext
   */
  TEST_F(SecurityManagerTest, ProtectConnection_SetSSLContextError) {
    SetMockCryptoManger();
    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_UNKWOWN_INTERNAL_ERROR),is_final)).
        Times(1);
    //Expect notifying listeners (unsuccess)
    EXPECT_CALL(mock_sm_listener,
                OnHandshakeDone(key, false)).
        WillOnce(Return(true));

    //Emulate SessionObserver and CryptoManager result
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, protocol_handler::kControl)).
        WillOnce(ReturnNull());
    EXPECT_CALL(mock_crypto_manager,
                CreateSSLContext()).
        WillOnce(Return(&mock_ssl_context_new));
    EXPECT_CALL(mock_crypto_manager,
                ReleaseSSLContext(&mock_ssl_context_new)).
        Times(1);
    EXPECT_CALL(mock_session_observer,
                SetSSLContext(key, &mock_ssl_context_new)).
        WillOnce(Return(SecurityQuery::ERROR_UNKWOWN_INTERNAL_ERROR));

    const bool rezult = security_manager_->ProtectConnection(key);
    EXPECT_FALSE(rezult);
  }
  /*
   * Shall protect connection on correct call ProtectConnection
   */
  TEST_F(SecurityManagerTest, ProtectConnection_Success) {
    SetMockCryptoManger();
    // Expect no Errors
    //Expect no notifying listeners - it will be done after handshake

    //Emulate SessionObserver and CryptoManager result
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, protocol_handler::kControl)).
        WillOnce(ReturnNull()).
        //additional check for debug code
        WillOnce(Return(&mock_ssl_context_exists));
    EXPECT_CALL(mock_crypto_manager,
                CreateSSLContext()).
        WillOnce(Return(&mock_ssl_context_new));
    EXPECT_CALL(mock_session_observer,
                SetSSLContext(key, &mock_ssl_context_new)).
        WillOnce(Return(SecurityQuery::ERROR_SUCCESS));

    const bool rezult = security_manager_->ProtectConnection(key);
    EXPECT_TRUE(rezult);
  }
  /*
   * Shall send InternallError on call StartHandshake for uprotected service
   */
  TEST_F(SecurityManagerTest, StartHandshake_ServiceStillUnprotected) {
    SetMockCryptoManger();
    // Expect InternalError with ERROR_INTERNAL
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_INTERNAL),is_final)).
        Times(1);
    //Expect notifying listeners (unsuccess)
    EXPECT_CALL(mock_sm_listener,
                OnHandshakeDone(key, false)).
        WillOnce(Return(true));

    //Emulate SessionObserver result
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, protocol_handler::kControl)).
        WillOnce(ReturnNull());

    security_manager_->StartHandshake(key);
  }
  /*
   * Shall send InternallError on call StartHandshake for uprotected service
   */
  TEST_F(SecurityManagerTest, StartHandshake_InitIsNotComplete) {
    SetMockCryptoManger();

    // Expect send two message ( with correct pointer and size data)
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(_,is_final)).
        Times(1);

    //Emulate SessionObserver result
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, protocol_handler::kControl)).
        WillOnce(Return(&mock_ssl_context_exists));
    EXPECT_CALL(mock_ssl_context_exists,
                IsInitCompleted()).
        WillOnce(Return(false));
    EXPECT_CALL(mock_ssl_context_exists,
                StartHandshake(_,_)).
        WillOnce(DoAll( SetArgPointee<0>(handshake_data_out_pointer),
                        SetArgPointee<1>(handshake_data_out_size),
                        Return(security_manager::SSLContext::
                               Handshake_Result_Success)));

    security_manager_->StartHandshake(key);
  }
  /*
   * Shall send InternallError on call StartHandshake for uprotected service
   */
  TEST_F(SecurityManagerTest, StartHandshake_InitIsComplete) {
    SetMockCryptoManger();
    // Expect no message send
    //Expect notifying listeners (success)
    EXPECT_CALL(mock_sm_listener,
                OnHandshakeDone(key, true)).
        WillOnce(Return(true));

    //Emulate SessionObserver result
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, protocol_handler::kControl)).
        WillOnce(Return(&mock_ssl_context_exists));
    EXPECT_CALL(mock_ssl_context_exists,
                IsInitCompleted()).
        WillOnce(Return(true));

    security_manager_->StartHandshake(key);
  }
  /*
   * Shall send InternallError on
   * getting SEND_HANDSHAKE_DATA with NULL data
   */
  TEST_F(SecurityManagerTest, ProccessHandshakeData_WrongDataSize) {
    SetMockCryptoManger();
    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_INVALID_QUERY_SIZE),is_final)).
        Times(1);
    EmulateMobileMessageHandShake(NULL, 0);
  }
  /*
   * Shall send InternallError on
   * getting SEND_HANDSHAKE_DATA from mobile side
   * for service which is not protected
   */
  TEST_F(SecurityManagerTest, ProccessHandshakeData_ServiceNotProtected) {
    SetMockCryptoManger();
    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_SERVICE_NOT_PROTECTED),is_final)).
        Times(1);
    //Expect notifying listeners (unsuccess)
    EXPECT_CALL(mock_sm_listener,
                OnHandshakeDone(key, false)).
        WillOnce(Return(true));

    //Emulate SessionObserver result
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, protocol_handler::kControl)).
        WillOnce(ReturnNull());

    const uint8_t data[] = {0x1, 0x2};
    EmulateMobileMessageHandShake(data, sizeof(data)/sizeof(data[0]));
  }
  /*
   * Shall send InternallError on getting
   * SEND_HANDSHAKE_DATA from mobile side with invalid handshake
   * data (DoHandshakeStep return NULL pointer)
   */
  TEST_F(SecurityManagerTest, ProccessHandshakeData_InvalidData) {
    SetMockCryptoManger();

    //Count handshake calls
    const int handshake_emulates = 4;

    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  InternalErrorWithErrId(
                    SecurityQuery::ERROR_SSL_INVALID_DATA),is_final)).
        Times(handshake_emulates);
    //Expect notifying listeners (unsuccess)
    EXPECT_CALL(mock_sm_listener,
                OnHandshakeDone(key, false)).
        WillOnce(Return(true));

    //Emulate SessionObserver and CryptoManager result
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, protocol_handler::kControl)).
        Times(handshake_emulates).
        WillRepeatedly(Return(&mock_ssl_context_exists));
    //Emulate DoHandshakeStep fail logics
    EXPECT_CALL(mock_ssl_context_exists,
                DoHandshakeStep(_, handshake_data_size, _, _)).
         WillOnce(DoAll( SetArgPointee<2>(handshake_data_out_pointer),
                         SetArgPointee<3>(handshake_data_out_size),
                         Return(security_manager::SSLContext::
                                Handshake_Result_AbnormalFail))).
         WillOnce(DoAll( SetArgPointee<2>((uint8_t*)NULL),
                         SetArgPointee<3>(handshake_data_out_size),
                         Return(security_manager::SSLContext::
                                Handshake_Result_AbnormalFail))).
         WillOnce(DoAll( SetArgPointee<2>(handshake_data_out_pointer),
                         SetArgPointee<3>(0),
                         Return(security_manager::SSLContext::
                                Handshake_Result_AbnormalFail))).
         WillOnce(DoAll( SetArgPointee<2>((uint8_t*)NULL),
                         SetArgPointee<3>(0),
                         Return(security_manager::SSLContext::
                                Handshake_Result_AbnormalFail)));

    // Emulate handshare #handshake_emulates times for 5 cases
    EmulateMobileMessageHandShake(handshake_data, handshake_data_size, handshake_emulates);
  }
  /*
   * Shall send HandshakeData on getting SEND_HANDSHAKE_DATA from mobile side
   * with correct handshake data Check Fail and sussecc states
   */
  TEST_F(SecurityManagerTest, ProccessHandshakeData_Answer) {
    SetMockCryptoManger();
    //Count handshake calls
    const int handshake_emulates = 2;

    // Expect InternalError with ERROR_ID
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(
                  // FIXME : !!!
                  _, is_final)).
        Times(handshake_emulates);
    //Expect notifying listeners (unsuccess)
    EXPECT_CALL(mock_sm_listener,
                OnHandshakeDone(key, false)).
        WillOnce(Return(true));

    //Emulate SessionObserver and CryptoManager result
    EXPECT_CALL(mock_ssl_context_exists,
                IsInitCompleted()).
        Times(handshake_emulates).
        WillRepeatedly(Return(false));
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, protocol_handler::kControl)).
        Times(handshake_emulates).
        WillRepeatedly(Return(&mock_ssl_context_exists));

    //Emulate DoHandshakeStep correct logics
    EXPECT_CALL(mock_ssl_context_exists,
                DoHandshakeStep(_, handshake_data_size, _, _)).
        WillOnce(DoAll( SetArgPointee<2>(handshake_data_out_pointer),
                        SetArgPointee<3>(handshake_data_out_size),
                        Return(security_manager::SSLContext::
                               Handshake_Result_Success))).
        WillOnce(DoAll( SetArgPointee<2>(handshake_data_out_pointer),
                        SetArgPointee<3>(handshake_data_out_size),
                        Return(security_manager::SSLContext::
                               Handshake_Result_Fail)));

    EmulateMobileMessageHandShake(handshake_data, handshake_data_size, handshake_emulates);
  }
  /*
   * Shall call all listeners on success end handshake
   * and return handshake data
   * Check Fail and sussecc states
   */
  TEST_F(SecurityManagerTest, ProccessHandshakeData_HandShakeFinished) {
    SetMockCryptoManger();
    //Count handshake calls
    const int handshake_emulates = 6;
    //Expect no errors
    //Expect notifying listeners (success)
    EXPECT_CALL(mock_sm_listener,
                OnHandshakeDone(key, true)).
        WillOnce(Return(true));

    //Emulate SessionObserver and CryptoManager result
    EXPECT_CALL(mock_session_observer,
                GetSSLContext(key, protocol_handler::kControl)).
        Times(handshake_emulates).
        WillRepeatedly(Return(&mock_ssl_context_exists));
    EXPECT_CALL(mock_ssl_context_exists,
                IsInitCompleted()).
        Times(handshake_emulates).
        WillRepeatedly(Return(true));
    // FIXME (EZamakhov) : add DoHandshakeStep matcher for compare handshake data
    EXPECT_CALL(mock_ssl_context_exists,
                DoHandshakeStep(_, handshake_data_size, _, _)).
        // two states with correct out data
         WillOnce(DoAll( SetArgPointee<2>(handshake_data_out_pointer),
                         SetArgPointee<3>(handshake_data_out_size),
                         Return(security_manager::SSLContext::
                                Handshake_Result_Success))).
         WillOnce(DoAll( SetArgPointee<2>(handshake_data_out_pointer),
                         SetArgPointee<3>(handshake_data_out_size),
                         Return(security_manager::SSLContext::
                                Handshake_Result_Fail))).
        // two states with with null pointer data
         WillOnce(DoAll( SetArgPointee<2>((uint8_t*)NULL),
                         SetArgPointee<3>(handshake_data_out_size),
                         Return(security_manager::SSLContext::
                                Handshake_Result_Success))).
         WillOnce(DoAll( SetArgPointee<2>((uint8_t*)NULL),
                         SetArgPointee<3>(handshake_data_out_size),
                         Return(security_manager::SSLContext::
                                Handshake_Result_Fail))).
        // two states with with null data size
         WillOnce(DoAll( SetArgPointee<2>(handshake_data_out_pointer),
                         SetArgPointee<3>(0),
                         Return(security_manager::SSLContext::
                                Handshake_Result_Success))).
         WillOnce(DoAll( SetArgPointee<2>(handshake_data_out_pointer),
                         SetArgPointee<3>(0),
                         Return(security_manager::SSLContext::
                                Handshake_Result_Success)));

    // Expect send two message ( with correct pointer and size data)
    EXPECT_CALL(mock_protocol_observer,
                SendMessageToMobileApp(_,is_final)).
        Times(2);

    // Expect NO InternalError with ERROR_ID
    EmulateMobileMessageHandShake(handshake_data, handshake_data_size, handshake_emulates);
  }
  /*
   * Shall not any query on getting empty SEND_INTERNAL_ERROR
   */
  TEST_F(SecurityManagerTest, GetInternalError_NullData) {
    SetMockCryptoManger();

    const SecurityQuery::QueryHeader header(
          SecurityQuery::NOTIFICATION,
          SecurityQuery::SEND_INTERNAL_ERROR, 0);
    EmulateMobileMessage(header, NULL, 0);
  }
  /*
   * Shall not send any query on getting SEND_INTERNAL_ERROR
   */
  TEST_F(SecurityManagerTest, GetInternalError) {
    SetMockCryptoManger();

    const SecurityQuery::QueryHeader header(
          SecurityQuery::NOTIFICATION,
          SecurityQuery::SEND_INTERNAL_ERROR, 0);
    const uint8_t data[] = {0x1, 0x2};
    EmulateMobileMessage(header, data, sizeof(data)/sizeof(data[0]));
  }
  /*
   * Shall not send any query on getting SEND_INTERNAL_ERROR with error string
   */
  TEST_F(SecurityManagerTest, GetInternalError_WithErrText) {
    SetMockCryptoManger();

    SecurityQuery::QueryHeader header(
          SecurityQuery::NOTIFICATION,
          SecurityQuery::SEND_INTERNAL_ERROR, 0);
    std::string error("JSON wrong string");
    header.json_size = error.size();
    EmulateMobileMessage(header,
                         reinterpret_cast<const uint8_t*>(error.c_str()),
                         error.size());
  }
  /*
   * Shall not send any query on getting SEND_INTERNAL_ERROR with error string
   */
  TEST_F(SecurityManagerTest, GetInternalError_WithErrJSONText) {
    SetMockCryptoManger();

    SecurityQuery::QueryHeader header(
          SecurityQuery::NOTIFICATION,
          SecurityQuery::SEND_INTERNAL_ERROR, 0);
    std::string error(" { \"id\": 1 } ");
    header.json_size = error.size();
    EmulateMobileMessage(header,
                         reinterpret_cast<const uint8_t*>(error.c_str()),
                         error.size());
  }
} // security_manager_test
} // namespace components
} // namespace test
#endif // SECURITY_MANAGER_TEST_H
