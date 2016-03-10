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
#include "gtest/gtest.h"
#include <string>
#include "protocol_handler/protocol_handler.h"
#include "protocol_handler/protocol_handler_impl.h"
#include "protocol/common.h"
#include "protocol_handler/control_message_matcher.h"
#include "protocol_handler/protocol_handler_mock.h"
#include "protocol_handler/protocol_observer_mock.h"
#include "protocol_handler/mock_protocol_handler_settings.h"
#include "protocol_handler/mock_session_observer.h"
#include "connection_handler/mock_connection_handler.h"
#include "transport_manager/transport_manager_mock.h"

#include "utils/make_shared.h"

namespace test {
namespace components {
namespace protocol_handler_test {

// Id passed as NULL for new session establishing
#define NEW_SESSION_ID        0u
#define SESSION_START_REJECT  0u

using namespace ::protocol_handler;
using namespace ::transport_manager;
using ::transport_manager::TransportManagerListener;
using ::testing::Return;
using ::testing::ReturnRefOfCopy;
using ::testing::ReturnNull;
using ::testing::AnyOf;
using ::testing::_;
using ::testing::Invoke;

class ProtocolHandlerImplTest : public ::testing::Test {
 protected:
  void InitProtocolHandlerImpl(
      const size_t period_msec, const size_t max_messages,
      bool malformed_message_filtering = false,
      const size_t malformd_period_msec = 0u,
      const size_t malformd_max_messages = 0u,
                                     const int32_t multiframe_waiting_timeout = 0,
                                     const size_t maximum_payload_size = 0u) {
          ON_CALL(protocol_handler_settings_mock, maximum_payload_size())
              .WillByDefault(Return(maximum_payload_size));
          ON_CALL(protocol_handler_settings_mock, message_frequency_time())
              .WillByDefault(Return(period_msec));
          ON_CALL(protocol_handler_settings_mock, message_frequency_count())
              .WillByDefault(Return(max_messages));
          ON_CALL(protocol_handler_settings_mock, malformed_message_filtering())
              .WillByDefault(Return(malformed_message_filtering));
          ON_CALL(protocol_handler_settings_mock, malformed_frequency_time())
              .WillByDefault(Return(malformd_period_msec));
          ON_CALL(protocol_handler_settings_mock, malformed_frequency_count())
              .WillByDefault(Return(malformd_max_messages));
          ON_CALL(protocol_handler_settings_mock, multiframe_waiting_timeout())
              .WillByDefault(Return(multiframe_waiting_timeout));
          protocol_handler_impl.reset(new ProtocolHandlerImpl(protocol_handler_settings_mock,
                                      session_observer_mock,
                                      connection_handler_mock,
                                      transport_manager_mock));
    tm_listener = protocol_handler_impl.get();
  }
  void SetUp() OVERRIDE {
    InitProtocolHandlerImpl(0u, 0u);
    connection_id = 0xAu;
    session_id = 0xFFu;
    connection_key = 0xFF00AAu;
    message_id = 0xABCDEFu;
    some_data.resize(256, 0xAB);

    // Expect ConnectionHandler support methods call (conversion, check heartbeat)
    EXPECT_CALL(session_observer_mock,
        KeyFromPair(connection_id, _)).
    // Return some connection_key
    WillRepeatedly(Return(connection_key));
    EXPECT_CALL(session_observer_mock,
        IsHeartBeatSupported(connection_id, _)).
    // Return false to avoid call KeepConnectionAlive
    WillRepeatedly(Return(false));
  }

  void TearDown() OVERRIDE {
    // Wait call methods in thread
    testing::Mock::AsyncVerifyAndClearExpectations(10000);
  }

  // Emulate connection establish
  void AddConnection() {
    tm_listener->OnConnectionEstablished(
        DeviceInfo(DeviceHandle(1u), std::string("mac"), std::string("name"),
                   std::string("BTMAC")),
        connection_id);
  }
  void AddSession() {
    AddConnection();
    const ServiceType start_service = kRpc;
#ifdef ENABLE_SECURITY
    // For enabled protection callback shall use protection ON
    const bool callback_protection_flag = PROTECTION_ON;
#else
    // For disabled protection callback shall ignore protection income flad and use protection OFF
    const bool callback_protection_flag = PROTECTION_OFF;
#endif  // ENABLE_SECURITY
    // Expect ConnectionHandler check
    EXPECT_CALL(session_observer_mock,
        OnSessionStartedCallback(connection_id, NEW_SESSION_ID, start_service,
            callback_protection_flag, _)).
    // Return sessions start success
    WillOnce(Return(session_id));

    // Expect send Ack with PROTECTION_OFF (on no Security Manager)
    EXPECT_CALL(transport_manager_mock,
        SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF)))
        .WillOnce(Return(E_SUCCESS));

    SendControlMessage(PROTECTION_ON, start_service, NEW_SESSION_ID,
                       FRAME_DATA_START_SERVICE);
  }

#ifdef ENABLE_SECURITY
  // Emulate security manager initilization establish
  void AddSecurityManager() {
    protocol_handler_impl->set_security_manager(&security_manager_mock);
  }
#endif  // ENABLE_SECURITY
  void SendTMMessage(uint8_t connection_id, uint8_t version, bool protection,
                     uint8_t frameType, uint8_t serviceType, uint8_t frameData,
                     uint8_t sessionId, uint32_t dataSize, uint32_t messageID,
                     const uint8_t *data = 0) {
    // Create packet
    const ProtocolPacket packet(connection_id, version, protection, frameType,
                                serviceType, frameData, sessionId, dataSize,
                                messageID, data);
    // Emulate receive packet from transoprt manager
    tm_listener->OnTMMessageReceived(packet.serializePacket());
  }

  void SetProtocolVersion2() {
    // Set protocol version 2
    ON_CALL(protocol_handler_settings_mock, max_supported_protocol_version())
        .WillByDefault(Return(PROTOCOL_VERSION_2));
  }

  void SendControlMessage(bool protection, uint8_t service_type,
                          uint8_t sessionId, uint32_t frame_data,
                          uint32_t dataSize = 0u, const uint8_t *data = NULL) {
    SendTMMessage(connection_id, PROTOCOL_VERSION_3, protection,
                  FRAME_TYPE_CONTROL, service_type, frame_data, sessionId,
                  dataSize, message_id, data);
  }

  testing::NiceMock<MockProtocolHandlerSettings> protocol_handler_settings_mock;
  ::utils::SharedPtr<ProtocolHandlerImpl> protocol_handler_impl;
  TransportManagerListener* tm_listener;
  // Uniq connection
  ::transport_manager::ConnectionUID connection_id;
  // Id of established session
  uint8_t session_id;
  // Uniq id as connection_id and session_id in one
  uint32_t connection_key;
  uint32_t message_id;
  std::vector<uint8_t> some_data;
  // Strict mocks (same as all methods EXPECT_CALL().Times(0))
  testing::NiceMock<connection_handler_test::MockConnectionHandler> connection_handler_mock;
  testing::StrictMock<transport_manager_test::MockTransportManager> transport_manager_mock;
  testing::StrictMock<protocol_handler_test::MockSessionObserver> session_observer_mock;
#ifdef ENABLE_SECURITY
  testing::NiceMock<protocol_handler_test::SecurityManagerMock> security_manager_mock;
  testing::NiceMock<protocol_handler_test::SSLContextMock> ssl_context_mock;
#endif  // ENABLE_SECURITY
};

#ifdef ENABLE_SECURITY
class OnHandshakeDoneFunctor {
public:
  OnHandshakeDoneFunctor(const uint32_t connection_key,
                         security_manager::SSLContext::HandshakeResult error)
  : connection_key(connection_key), result(error) {}
  void operator()(security_manager::SecurityManagerListener * listener) const {
    listener->OnHandshakeDone(connection_key, result);
  }
private:
  const uint32_t connection_key;
  const security_manager::SSLContext::HandshakeResult result;
};
#endif  // ENABLE_SECURITY

/*
 * ProtocolHandler shall skip empty message
 */
TEST_F(ProtocolHandlerImplTest, RecieveEmptyRawMessage) {
  tm_listener->OnTMMessageReceived(RawMessagePtr());
}
/*
 * ProtocolHandler shall disconnect on no connection
 */
TEST_F(ProtocolHandlerImplTest, RecieveOnUnknownConnection) {
  EXPECT_CALL(transport_manager_mock, DisconnectForce(connection_id)).
      WillOnce(Return(E_SUCCESS));

  SendTMMessage(connection_id, PROTOCOL_VERSION_3, PROTECTION_OFF,
                FRAME_TYPE_CONTROL, kRpc, FRAME_DATA_START_SERVICE,
                NEW_SESSION_ID, 0, message_id);
}
/*
 * ProtocolHandler shall send NAck on session_observer rejection
 * Check protection flag OFF for all services from kControl to kBulk
 */
TEST_F(ProtocolHandlerImplTest, StartSession_Unprotected_SessionObserverReject) {
  const int call_times = 5;
  AddConnection();
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionStartedCallback(
          connection_id, NEW_SESSION_ID, AnyOf(kControl, kRpc, kAudio,
              kMobileNav, kBulk), PROTECTION_OFF, _)).Times(call_times).
  // Return sessions start rejection
      WillRepeatedly(Return(SESSION_START_REJECT));

  // Expect send NAck
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_NACK, PROTECTION_OFF)))
      .Times(call_times).WillRepeatedly(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_OFF, kControl, NEW_SESSION_ID,
                     FRAME_DATA_START_SERVICE);
  SendControlMessage(PROTECTION_OFF, kRpc, NEW_SESSION_ID,
                     FRAME_DATA_START_SERVICE);
  SendControlMessage(PROTECTION_OFF, kAudio, NEW_SESSION_ID,
                     FRAME_DATA_START_SERVICE);
  SendControlMessage(PROTECTION_OFF, kMobileNav, NEW_SESSION_ID,
                     FRAME_DATA_START_SERVICE);
  SendControlMessage(PROTECTION_OFF, kBulk, NEW_SESSION_ID,
                     FRAME_DATA_START_SERVICE);
}
/*
 * ProtocolHandler shall send NAck on session_observer rejection
 * Emulate getting PROTECTION_ON and check protection flag OFF in NAck
 * For ENABLE_SECURITY=OFF session_observer shall be called with protection flag OFF
 */
TEST_F(ProtocolHandlerImplTest, StartSession_Protected_SessionObserverReject) {
  const int call_times = 5;
  AddConnection();
#ifdef ENABLE_SECURITY
  // For enabled protection callback shall use protection ON
  const bool callback_protection_flag = PROTECTION_ON;
#else
  // For disabled protection callback shall ignore protection income flag and use protection OFF
  const bool callback_protection_flag = PROTECTION_OFF;
#endif  // ENABLE_SECURITY
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionStartedCallback(
          connection_id, NEW_SESSION_ID, AnyOf(kControl, kRpc, kAudio,
              kMobileNav, kBulk), callback_protection_flag, _)).Times(
      call_times).
  // Return sessions start rejection
      WillRepeatedly(Return(SESSION_START_REJECT));

  // Expect send NAck with encryption OFF
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_NACK, PROTECTION_OFF)))
      .Times(call_times).WillRepeatedly(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_ON, kControl, NEW_SESSION_ID,
                     FRAME_DATA_START_SERVICE);
  SendControlMessage(PROTECTION_ON, kRpc, NEW_SESSION_ID,
                     FRAME_DATA_START_SERVICE);
  SendControlMessage(PROTECTION_ON, kAudio, NEW_SESSION_ID,
                     FRAME_DATA_START_SERVICE);
  SendControlMessage(PROTECTION_ON, kMobileNav, NEW_SESSION_ID,
                     FRAME_DATA_START_SERVICE);
  SendControlMessage(PROTECTION_ON, kBulk, NEW_SESSION_ID,
                     FRAME_DATA_START_SERVICE);
}
/*
 * ProtocolHandler shall send Ack on session_observer accept
 * Check protection flag OFF
 */
TEST_F(ProtocolHandlerImplTest, StartSession_Unprotected_SessionObserverAccept) {
  AddConnection();
  const ServiceType start_service = kRpc;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionStartedCallback(connection_id, NEW_SESSION_ID, start_service, PROTECTION_OFF, _))
      .
      // Return sessions start success
  WillOnce(Return(session_id));

  SetProtocolVersion2();
  // Expect send Ack
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF)))
      .WillOnce(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_OFF, start_service, NEW_SESSION_ID,
                     FRAME_DATA_START_SERVICE);
}
/*
 * ProtocolHandler shall send Ack on session_observer accept
 * Emulate getting PROTECTION_ON and check protection flag OFF in Ack
 * For ENABLE_SECURITY=OFF session_observer shall be called with protection flag OFF
 */
TEST_F(ProtocolHandlerImplTest, StartSession_Protected_SessionObserverAccept) {
  SetProtocolVersion2();
  AddSession();
}
// TODO(EZamakhov): add test for get_hash_id/set_hash_id from protocol_handler_impl.cc
/*
 * ProtocolHandler shall send NAck on session_observer rejection
 */
TEST_F(ProtocolHandlerImplTest, EndSession_SessionObserverReject) {
  AddSession();
  const ServiceType service = kRpc;

  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionEndedCallback(connection_id, session_id, _, service)).
  // reject session start
  WillOnce(Return(SESSION_START_REJECT));

  SetProtocolVersion2();
  // Expect send NAck
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_END_SERVICE_NACK, PROTECTION_OFF)))
      .WillOnce(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_OFF, service, session_id,
                     FRAME_DATA_END_SERVICE);
}
/*
 * ProtocolHandler shall send NAck on wrong hash code
 */
TEST_F(ProtocolHandlerImplTest, EndSession_Success) {
  AddSession();
  const ServiceType service = kRpc;

  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionEndedCallback(connection_id, session_id, _, service)).
  // return sessions start success
  WillOnce(Return(connection_key));

  SetProtocolVersion2();
  // Expect send Ack
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_END_SERVICE_ACK, PROTECTION_OFF)))
      .WillOnce(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_OFF, service, session_id,
                     FRAME_DATA_END_SERVICE);
}

#ifdef ENABLE_SECURITY
/*
 * ProtocolHandler shall not call Security logics with Protocol version 1
 * Check session_observer with PROTECTION_OFF and Ack with PROTECTION_OFF
 */
TEST_F(ProtocolHandlerImplTest, SecurityEnable_StartSessionProtocoloV1) {
  AddConnection();
  // Add security manager
  AddSecurityManager();
  const ServiceType start_service = kRpc;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionStartedCallback(connection_id, NEW_SESSION_ID, start_service, PROTECTION_OFF, _)).
  // Return sessions start success
  WillOnce(Return(session_id));

  SetProtocolVersion2();
  // Expect send Ack with PROTECTION_OFF (on no Security Manager)
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF))).
  WillOnce(Return(E_SUCCESS));

  SendTMMessage(connection_id, PROTOCOL_VERSION_1, PROTECTION_ON, FRAME_TYPE_CONTROL,
      start_service, FRAME_DATA_START_SERVICE, NEW_SESSION_ID, 0, message_id);
}
/*
 * ProtocolHandler shall not call Security logics on start session with PROTECTION_OFF
 */
TEST_F(ProtocolHandlerImplTest, SecurityEnable_StartSessionUnprotected) {
  AddConnection();
  // Add security manager
  AddSecurityManager();
  const ServiceType start_service = kRpc;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionStartedCallback(connection_id, NEW_SESSION_ID, start_service, PROTECTION_OFF, _)).
  // Return sessions start success
  WillOnce(Return(session_id));

  SetProtocolVersion2();
  // Expect send Ack with PROTECTION_OFF (on no Security Manager)
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF))).
  WillOnce(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_OFF, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_OFF on fail SLL creation
 */
TEST_F(ProtocolHandlerImplTest, SecurityEnable_StartSessionProtected_Fail) {
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionStartedCallback(connection_id, NEW_SESSION_ID, start_service, PROTECTION_ON, _)).
  // Return sessions start success
  WillOnce(Return(session_id));

  SetProtocolVersion2();
  // Expect start protection for unprotected session
  EXPECT_CALL(security_manager_mock,
      CreateSSLContext(connection_key)).
  // Return fail protection
  WillOnce(ReturnNull());

  // Expect send Ack with PROTECTION_OFF (on fail SLL creation)
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF))).
  WillOnce(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_ON on already established and initialized SLLContext
 */
TEST_F(ProtocolHandlerImplTest,SecurityEnable_StartSessionProtected_SSLInitialized) {
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionStartedCallback(connection_id, NEW_SESSION_ID, start_service, PROTECTION_ON, _)).
  // Return sessions start success
  WillOnce(Return(session_id));

  SetProtocolVersion2();
  // call new SSLContext creation
  EXPECT_CALL(security_manager_mock,
      CreateSSLContext(connection_key)).
  // Return new SSLContext
  WillOnce(Return(&ssl_context_mock));

  // Initilization check
  EXPECT_CALL(ssl_context_mock,
      IsInitCompleted()).
  //emulate SSL is initilized
  WillOnce(Return(true));

  // Expect service protection enable
  EXPECT_CALL(session_observer_mock,
      SetProtectionFlag(connection_key, start_service));

  // Expect send Ack with PROTECTION_ON (on SSL is initilized)
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_ON))).
  WillOnce(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_OFF on session handshhake fail
 */
TEST_F(ProtocolHandlerImplTest, SecurityEnable_StartSessionProtected_HandshakeFail) {
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionStartedCallback(connection_id, NEW_SESSION_ID, start_service, PROTECTION_ON, _)).
  // Return sessions start success
  WillOnce(Return(session_id));

  std::vector<int> services;
  // TODO(AKutsan) : APPLINK-21398 use named constants instead of magic numbers
  services.push_back(0x0A);
  services.push_back(0x0B);
  ON_CALL(protocol_handler_settings_mock, force_protected_service())
      .WillByDefault(ReturnRefOfCopy(services));

  // call new SSLContext creation
  EXPECT_CALL(security_manager_mock,
      CreateSSLContext(connection_key)).
  // Return new SSLContext
  WillOnce(Return(&ssl_context_mock));

  // Initilization check
  EXPECT_CALL(ssl_context_mock,
      IsInitCompleted()).
  //emulate SSL is not initilized
  WillOnce(Return(false));

  // Pending handshake check
  EXPECT_CALL(ssl_context_mock,
      IsHandshakePending()).
  //emulate is pending
  WillOnce(Return(true));

  // Expect add listener for handshake result
  EXPECT_CALL(security_manager_mock,
      AddListener(_))
  // Emulate handshake fail
  .WillOnce(Invoke(OnHandshakeDoneFunctor(
                     connection_key,
                     security_manager::SSLContext::Handshake_Result_Fail)));

  // Listener check SSLContext
  EXPECT_CALL(session_observer_mock,
      GetSSLContext(connection_key, start_service)).
  // Emulate protection for service is not enabled
  WillOnce(ReturnNull());

  // Expect send Ack with PROTECTION_OFF (on fail handshake)
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF))).
  WillOnce(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_ON on session handshhake success
 */
TEST_F(ProtocolHandlerImplTest, SecurityEnable_StartSessionProtected_HandshakeSuccess) {
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;

  // No services are protected
  std::vector<int> services;
  ON_CALL(protocol_handler_settings_mock, force_protected_service())
      .WillByDefault(ReturnRefOfCopy(services));

  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionStartedCallback(connection_id, NEW_SESSION_ID, start_service, PROTECTION_ON, _)).
  // Return sessions start success
  WillOnce(Return(session_id));

  // call new SSLContext creation
  EXPECT_CALL(security_manager_mock,
      CreateSSLContext(connection_key)).
  // Return new SSLContext
  WillOnce(Return(&ssl_context_mock));

  // Initilization check
  EXPECT_CALL(ssl_context_mock,
      IsInitCompleted()).
  //emulate SSL is not initilized
  WillOnce(Return(false));

  // Pending handshake check
  EXPECT_CALL(ssl_context_mock,
      IsHandshakePending()).
  //emulate is pending
  WillOnce(Return(true));

  // Expect add listener for handshake result
  EXPECT_CALL(security_manager_mock,
      AddListener(_))
  // Emulate handshake fail
  .WillOnce(Invoke(OnHandshakeDoneFunctor(
                     connection_key,
                     security_manager::SSLContext::Handshake_Result_Success)));

  // Listener check SSLContext
  EXPECT_CALL(session_observer_mock,
      GetSSLContext(connection_key, start_service)).
  // Emulate protection for service is not enabled
  WillOnce(ReturnNull());

  // Expect service protection enable
  EXPECT_CALL(session_observer_mock,
      SetProtectionFlag(connection_key, start_service));

  // Expect send Ack with PROTECTION_OFF (on fail handshake)
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_ON))).
  WillOnce(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_ON on session handshhake success
 */
TEST_F(ProtocolHandlerImplTest,
    SecurityEnable_StartSessionProtected_HandshakeSuccess_ServiceProtectedBefore) {
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;

  std::vector<int> services;
  ON_CALL(protocol_handler_settings_mock, force_protected_service())
      .WillByDefault(ReturnRefOfCopy(services));

  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionStartedCallback(connection_id, NEW_SESSION_ID, start_service, PROTECTION_ON, _)).
  // Return sessions start success
  WillOnce(Return(session_id));

  // call new SSLContext creation
  EXPECT_CALL(security_manager_mock,
      CreateSSLContext(connection_key)).
  // Return new SSLContext
  WillOnce(Return(&ssl_context_mock));

  // Initilization check
  EXPECT_CALL(ssl_context_mock,
      IsInitCompleted()).
  //emulate SSL is not initilized
  WillOnce(Return(false));

  // Pending handshake check
  EXPECT_CALL(ssl_context_mock,
      IsHandshakePending()).
  //emulate is pending
  WillOnce(Return(true));

  // Expect add listener for handshake result
  EXPECT_CALL(security_manager_mock,
      AddListener(_))
  // Emulate handshake fail
  .WillOnce(Invoke(OnHandshakeDoneFunctor(
                     connection_key,
                     security_manager::SSLContext::Handshake_Result_Success)));

  // Listener check SSLContext
  EXPECT_CALL(session_observer_mock,
      GetSSLContext(connection_key, start_service)).
  // Emulate protection for service is not enabled
  WillOnce(ReturnNull());

  // Expect service protection enable
  EXPECT_CALL(session_observer_mock,
      SetProtectionFlag(connection_key, start_service));

  // Expect send Ack with PROTECTION_OFF (on fail handshake)
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_ON))).
  WillOnce(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_ON on session handshhake success
 */
TEST_F(ProtocolHandlerImplTest,
    SecurityEnable_StartSessionProtected_HandshakeSuccess_SSLIsNotPending) {
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;

  std::vector<int> services;
  ON_CALL(protocol_handler_settings_mock, force_protected_service())
      .WillByDefault(ReturnRefOfCopy(services));

  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
      OnSessionStartedCallback(connection_id, NEW_SESSION_ID, start_service, PROTECTION_ON, _)).
  // Return sessions start success
  WillOnce(Return(session_id));

  // call new SSLContext creation
  EXPECT_CALL(security_manager_mock,
      CreateSSLContext(connection_key)).
  // Return new SSLContext
  WillOnce(Return(&ssl_context_mock));

  // Initilization check
  EXPECT_CALL(ssl_context_mock,
      IsInitCompleted()).
  //emulate SSL is not initilized
  WillOnce(Return(false));

  // Pending handshake check
  EXPECT_CALL(ssl_context_mock,
      IsHandshakePending()).
  //emulate is pending
  WillOnce(Return(false));

  // Wait restart handshake operation
  EXPECT_CALL(security_manager_mock,
      StartHandshake(connection_key));

  // Expect add listener for handshake result
  EXPECT_CALL(security_manager_mock,
      AddListener(_))
  // Emulate handshake fail
  .WillOnce(Invoke(OnHandshakeDoneFunctor(
                     connection_key,
                     security_manager::SSLContext::Handshake_Result_Success)));

  // Listener check SSLContext
  EXPECT_CALL(session_observer_mock,
      GetSSLContext(connection_key, start_service)).
  // Emulate protection for service is not enabled
  WillOnce(ReturnNull());

  // Expect service protection enable
  EXPECT_CALL(session_observer_mock,
      SetProtectionFlag(connection_key, start_service));

  // Expect send Ack with PROTECTION_OFF (on fail handshake)
  EXPECT_CALL(transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_ON))).
  WillOnce(Return(E_SUCCESS));

  SendControlMessage(PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
}
#endif  // ENABLE_SECURITY

TEST_F(ProtocolHandlerImplTest,
    FloodVerification) {
  const size_t period_msec = 10000;
  const size_t max_messages = 1000;
  InitProtocolHandlerImpl(period_msec, max_messages);
  AddConnection();
  AddSession();

  // Expect flood notification to CH
  EXPECT_CALL(session_observer_mock,
      OnApplicationFloodCallBack(connection_key)).
  Times(1);

  ON_CALL(protocol_handler_settings_mock, message_frequency_time())
      .WillByDefault(Return(period_msec));
  ON_CALL(protocol_handler_settings_mock, message_frequency_count())
      .WillByDefault(Return(max_messages));

  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id, PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_SINGLE,
        kControl, FRAME_DATA_SINGLE, session_id,
        some_data.size(), message_id, &some_data[0]);
  }
}
TEST_F(ProtocolHandlerImplTest,
    FloodVerification_ThresholdValue) {
  const size_t period_msec = 10000;
  const size_t max_messages = 1000;
  InitProtocolHandlerImpl(period_msec, max_messages);
  AddConnection();
  AddSession();

  ON_CALL(protocol_handler_settings_mock, message_frequency_time())
      .WillByDefault(Return(period_msec));
  ON_CALL(protocol_handler_settings_mock, message_frequency_count())
      .WillByDefault(Return(max_messages));

  // Expect NO flood notification to CH
  EXPECT_CALL(session_observer_mock, OnApplicationFloodCallBack(connection_key))
      .Times(0);
  for (size_t i = 0; i < max_messages - 1; ++i) {
    SendTMMessage(connection_id, PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_SINGLE,
        kControl, FRAME_DATA_SINGLE, session_id,
        some_data.size(), message_id, &some_data[0]);
  }
}
TEST_F(ProtocolHandlerImplTest,
    FloodVerification_VideoFrameSkip) {
  const size_t period_msec = 10000;
  const size_t max_messages = 1000;
  InitProtocolHandlerImpl(period_msec, max_messages);
  AddConnection();
  AddSession();

  // Expect NO flood notification to CH on video data streaming
  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id, PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_SINGLE,
        kMobileNav, FRAME_DATA_SINGLE, session_id,
        some_data.size(), message_id, &some_data[0]);
  }
}
TEST_F(ProtocolHandlerImplTest,
    FloodVerification_AudioFrameSkip) {
  const size_t period_msec = 10000;
  const size_t max_messages = 1000;
  InitProtocolHandlerImpl(period_msec, max_messages);
  AddConnection();
  AddSession();

  // Expect NO flood notification to CH on video data streaming
  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id, PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_SINGLE,
        kAudio, FRAME_DATA_SINGLE, session_id,
        some_data.size(), message_id, &some_data[0]);
  }
}
TEST_F(ProtocolHandlerImplTest,
    FloodVerificationDisable) {
  const size_t period_msec = 0;
  const size_t max_messages = 0;
  InitProtocolHandlerImpl(period_msec, max_messages);
  AddConnection();
  AddSession();

  // Expect NO flood notification to session observer
  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id, PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_SINGLE,
        kControl, FRAME_DATA_SINGLE, session_id,
        some_data.size(), message_id, &some_data[0]);
  }
}


TEST_F(ProtocolHandlerImplTest,
       MalformedVerificationDisable) {
  const size_t period_msec = 10000;
  const size_t max_messages = 100;
  InitProtocolHandlerImpl(0u, 0u, false, period_msec, max_messages);
  AddConnection();
  AddSession();

  // Expect malformed notification to CH
  EXPECT_CALL(session_observer_mock,
              OnMalformedMessageCallback(connection_id)).
      Times(max_messages);

  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  for (size_t i = 0; i < max_messages; ++i) {
    SendTMMessage(connection_id, malformed_version, PROTECTION_OFF, FRAME_TYPE_SINGLE,
                  kControl, FRAME_DATA_SINGLE, session_id,
                  some_data.size(), message_id, &some_data[0]);
  }
}

TEST_F(ProtocolHandlerImplTest,
       MalformedLimitVerification) {
  const size_t period_msec = 10000;
  const size_t max_messages = 100;
  InitProtocolHandlerImpl(0u, 0u, true, period_msec, max_messages);
  AddConnection();
  AddSession();

  // Expect malformed notification to CH
  EXPECT_CALL(session_observer_mock,
              OnMalformedMessageCallback(connection_id)).
      Times(1);

  // Sending malformed packets
  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  for (size_t i = 0; i < max_messages * 2; ++i) {
    // Malformed message
    SendTMMessage(connection_id, malformed_version, PROTECTION_OFF, FRAME_TYPE_SINGLE,
                  kControl, FRAME_DATA_SINGLE, session_id,
                  some_data.size(), message_id, &some_data[0]);
    // Common message
    SendTMMessage(connection_id, PROTOCOL_VERSION_1, PROTECTION_OFF, FRAME_TYPE_SINGLE,
                  kControl, FRAME_DATA_SINGLE, session_id,
                  some_data.size(), message_id, &some_data[0]);
  }
}

TEST_F(ProtocolHandlerImplTest,
    MalformedLimitVerification_MalformedStock) {
  const size_t period_msec = 10000;
  const size_t max_messages = 100;
  InitProtocolHandlerImpl(0u, 0u, true, period_msec, max_messages);
  AddConnection();
  AddSession();

  // Expect malformed notification to CH
  EXPECT_CALL(session_observer_mock,
      OnMalformedMessageCallback(connection_id)).
  Times(1);

  // Sending malformed packets
  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  const uint8_t malformed_frame_type = FRAME_TYPE_MAX_VALUE;
  const uint8_t malformed_service_type = kInvalidServiceType;
  for (size_t i = 0; i < max_messages * 2; ++i) {
    // Malformed message 1
    SendTMMessage(connection_id, malformed_version, PROTECTION_OFF, FRAME_TYPE_SINGLE,
        kControl, FRAME_DATA_SINGLE, session_id,
        some_data.size(), message_id, &some_data[0]);
    // Malformed message 2
    SendTMMessage(connection_id, PROTOCOL_VERSION_1, PROTECTION_OFF, malformed_frame_type,
        kControl, FRAME_DATA_SINGLE, session_id,
        some_data.size(), message_id, &some_data[0]);
    // Malformed message 3
    SendTMMessage(connection_id, PROTOCOL_VERSION_1, PROTECTION_OFF, FRAME_TYPE_SINGLE,
        malformed_service_type, FRAME_DATA_SINGLE, session_id,
        some_data.size(), message_id, &some_data[0]);

    // Common message
    SendTMMessage(connection_id, PROTOCOL_VERSION_1, PROTECTION_OFF, FRAME_TYPE_SINGLE,
                  kControl, FRAME_DATA_SINGLE, session_id,
                  some_data.size(), message_id, &some_data[0]);
  }
}

TEST_F(ProtocolHandlerImplTest,
       MalformedLimitVerification_MalformedOnly) {
  const size_t period_msec = 10000;
  const size_t max_messages = 100;
  InitProtocolHandlerImpl(0u, 0u, true, period_msec, max_messages);
  AddConnection();
  AddSession();

  // Expect NO malformed notification to CH
  EXPECT_CALL(session_observer_mock,
              OnMalformedMessageCallback(connection_id)).
      Times(0);

  // Sending malformed packets
  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  const uint8_t malformed_frame_type = FRAME_TYPE_MAX_VALUE;
  const uint8_t malformed_service_type = kInvalidServiceType;
  for (size_t i = 0; i < max_messages * 2; ++i) {
    // Malformed message 1
    SendTMMessage(connection_id, malformed_version, PROTECTION_OFF, FRAME_TYPE_SINGLE,
                  kControl, FRAME_DATA_SINGLE, session_id,
                  some_data.size(), message_id, &some_data[0]);
    // Malformed message 2
    SendTMMessage(connection_id, PROTOCOL_VERSION_1, PROTECTION_OFF, malformed_frame_type,
                  kControl, FRAME_DATA_SINGLE, session_id,
                  some_data.size(), message_id, &some_data[0]);
    // Malformed message 3
    SendTMMessage(connection_id, PROTOCOL_VERSION_1, PROTECTION_OFF, FRAME_TYPE_SINGLE,
                  malformed_service_type, FRAME_DATA_SINGLE, session_id,
                  some_data.size(), message_id, &some_data[0]);

    // No common message
  }
}

TEST_F(ProtocolHandlerImplTest,
       MalformedLimitVerification_NullTimePeriod) {
  const size_t period_msec = 0;
  const size_t max_messages = 1000;
  InitProtocolHandlerImpl(0u, 0u, true, period_msec, max_messages);
  AddConnection();
  AddSession();

  // Expect no malformed notification to CH
  EXPECT_CALL(session_observer_mock,
              OnMalformedMessageCallback(connection_id)).
      Times(0);

  // Sending malformed packets
  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id, malformed_version, PROTECTION_OFF, FRAME_TYPE_SINGLE,
                  kControl, FRAME_DATA_SINGLE, session_id,
                  some_data.size(), message_id, &some_data[0]);
  }
}
TEST_F(ProtocolHandlerImplTest,
       MalformedLimitVerification_NullCount) {
  const size_t period_msec = 10000;
  const size_t max_messages = 0;
  InitProtocolHandlerImpl(0u, 0u, true, period_msec, max_messages);
  AddConnection();
  AddSession();

  // Expect no malformed notification to CH
  EXPECT_CALL(session_observer_mock,
              OnMalformedMessageCallback(connection_id)).
      Times(0);

  // Sending malformed packets
  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id, malformed_version, PROTECTION_OFF, FRAME_TYPE_SINGLE,
                  kControl, FRAME_DATA_SINGLE, session_id,
                  some_data.size(), message_id, &some_data[0]);
  }
}

}  // namespace test
}  // namespace components
}  // namespace protocol_handler_test
