/*
 * Copyright (c) 2016, Ford Motor Company
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
#include "protocol/bson_object_keys.h"
#include "protocol/common.h"
#include "protocol_handler/control_message_matcher.h"
#include "protocol_handler/mock_protocol_handler.h"
#include "protocol_handler/mock_protocol_observer.h"
#include "protocol_handler/mock_protocol_handler_settings.h"
#include "protocol_handler/mock_session_observer.h"
#include "connection_handler/mock_connection_handler.h"
#include "connection_handler/connection_handler_impl.h"
#ifdef ENABLE_SECURITY
#include "security_manager/mock_security_manager.h"
#include "security_manager/mock_ssl_context.h"
#endif  // ENABLE_SECURITY
#include "transport_manager/mock_transport_manager.h"
#include "utils/mock_system_time_handler.h"
#include "utils/semantic_version.h"

#include "utils/test_async_waiter.h"
#include <bson/bson_object.h>

namespace transport_manager {
namespace transport_adapter {
// taken from transport_adapter_impl.cc
const char* tc_enabled = "enabled";
const char* tc_tcp_port = "tcp_port";
const char* tc_tcp_ip_address = "tcp_ip_address";
}
}

namespace test {
namespace components {
namespace protocol_handler_test {

// Id passed as NULL for new session establishing
#define NEW_SESSION_ID 0u
#define SESSION_START_REJECT 0u
// Protocol Handler Entities
using protocol_handler::ProtocolHandlerImpl;
using protocol_handler::ServiceType;
using protocol_handler::RawMessage;
using protocol_handler::RawMessagePtr;
using protocol_handler::PROTECTION_ON;
using protocol_handler::PROTECTION_OFF;
using protocol_handler::PROTOCOL_VERSION_1;
using protocol_handler::PROTOCOL_VERSION_2;
using protocol_handler::PROTOCOL_VERSION_3;
using protocol_handler::PROTOCOL_VERSION_4;
using protocol_handler::PROTOCOL_VERSION_5;
using protocol_handler::PROTOCOL_VERSION_MAX;
using protocol_handler::FRAME_TYPE_CONTROL;
using protocol_handler::FRAME_TYPE_SINGLE;
using protocol_handler::FRAME_TYPE_FIRST;
using protocol_handler::FRAME_TYPE_CONSECUTIVE;
using protocol_handler::FRAME_TYPE_MAX_VALUE;
using protocol_handler::MAXIMUM_FRAME_DATA_V2_SIZE;
using protocol_handler::FRAME_DATA_START_SERVICE;
using protocol_handler::FRAME_DATA_START_SERVICE_ACK;
using protocol_handler::FRAME_DATA_END_SERVICE_NACK;
using protocol_handler::FRAME_DATA_END_SERVICE_ACK;
using protocol_handler::FRAME_DATA_END_SERVICE;
using protocol_handler::FRAME_DATA_HEART_BEAT;
using protocol_handler::FRAME_DATA_HEART_BEAT_ACK;
using protocol_handler::FRAME_DATA_SERVICE_DATA_ACK;
using protocol_handler::FRAME_DATA_SINGLE;
using protocol_handler::FRAME_DATA_FIRST;
using protocol_handler::FRAME_DATA_LAST_CONSECUTIVE;
using protocol_handler::FRAME_DATA_REGISTER_SECONDARY_TRANSPORT;
using protocol_handler::FRAME_DATA_REGISTER_SECONDARY_TRANSPORT_ACK;
using protocol_handler::FRAME_DATA_REGISTER_SECONDARY_TRANSPORT_NACK;
using protocol_handler::FRAME_DATA_TRANSPORT_EVENT_UPDATE;
using protocol_handler::kRpc;
using protocol_handler::kControl;
using protocol_handler::kAudio;
using protocol_handler::kMobileNav;
using protocol_handler::kBulk;
using protocol_handler::kInvalidServiceType;
// For TM states
using transport_manager::TransportManagerListener;
using test::components::security_manager_test::MockSystemTimeHandler;
using transport_manager::E_SUCCESS;
using transport_manager::DeviceInfo;
#ifdef ENABLE_SECURITY
// For security
using ContextCreationStrategy =
    security_manager::SecurityManager::ContextCreationStrategy;
#endif  // ENABLE_SECURITY
// For CH entities
using connection_handler::DeviceHandle;
// Google Testing Framework Entities
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::ReturnRefOfCopy;
using ::testing::ReturnNull;
using ::testing::An;
using ::testing::AnyOf;
using ::testing::AtLeast;
using ::testing::ByRef;
using ::testing::DoAll;
using ::testing::SaveArg;
using ::testing::Eq;
using ::testing::_;
using ::testing::Invoke;
using ::testing::SetArgReferee;
using ::testing::SetArgPointee;

typedef std::vector<uint8_t> UCharDataVector;

// custom action to call a member function with 6 arguments
ACTION_P4(InvokeMemberFuncWithArg2, ptr, memberFunc, a, b) {
  (ptr->*memberFunc)(a, b);
}

namespace {
const uint32_t kAsyncExpectationsTimeout = 10000u;
const uint32_t kMicrosecondsInMillisecond = 1000u;
const uint32_t kAddSessionWaitTimeMs = 100u;
}

class ProtocolHandlerImplTest : public ::testing::Test {
 protected:
  void InitProtocolHandlerImpl(const size_t period_msec,
                               const size_t max_messages,
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
#ifdef ENABLE_SECURITY
    ON_CALL(protocol_handler_settings_mock, force_protected_service())
        .WillByDefault(ReturnRefOfCopy(force_protected_services));
    ON_CALL(protocol_handler_settings_mock, force_unprotected_service())
        .WillByDefault(ReturnRefOfCopy(force_unprotected_services));
#endif

    protocol_handler_impl.reset(
        new ProtocolHandlerImpl(protocol_handler_settings_mock,
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

    // Expect ConnectionHandler support methods call (conversion, check
    // heartbeat)
    EXPECT_CALL(session_observer_mock, KeyFromPair(connection_id, _))
        .
        // Return some connection_key
        WillRepeatedly(Return(connection_key));
    EXPECT_CALL(session_observer_mock, IsHeartBeatSupported(connection_id, _))
        .
        // Return false to avoid call KeepConnectionAlive
        WillRepeatedly(Return(false));

    session_connection_map_lock_ptr_ =
        std::make_shared<sync_primitives::Lock>();
  }

  void TearDown() OVERRIDE {
    const_cast<protocol_handler::impl::FromMobileQueue&>(
        protocol_handler_impl->get_from_mobile_queue()).WaitDumpQueue();
    const_cast<protocol_handler::impl::ToMobileQueue&>(
        protocol_handler_impl->get_to_mobile_queue()).WaitDumpQueue();
  }

  // Emulate connection establish
  void AddConnection() {
    tm_listener->OnConnectionEstablished(DeviceInfo(DeviceHandle(1u),
                                                    std::string("mac"),
                                                    std::string("name"),
                                                    std::string("BTMAC")),
                                         connection_id);
  }

  protocol_handler::SessionContext GetSessionContext(
      const transport_manager::ConnectionUID connection_id,
      const uint8_t initial_session_id,
      const uint8_t new_session_id,
      const protocol_handler::ServiceType service_type,
      const uint32_t hash_id,
      const bool protection_flag) {
    return protocol_handler::SessionContext(connection_id,
                                            connection_id,
                                            initial_session_id,
                                            new_session_id,
                                            service_type,
                                            hash_id,
                                            protection_flag);
  }

  void AddSession(const std::shared_ptr<TestAsyncWaiter>& waiter,
                  uint32_t& times) {
    using namespace protocol_handler;
    ASSERT_TRUE(NULL != waiter.get());

    AddConnection();
    const ServiceType start_service = kRpc;

#ifdef ENABLE_SECURITY
    // For enabled protection callback shall use protection ON
    const bool callback_protection_flag = PROTECTION_ON;
#else
    // For disabled protection callback shall ignore protection income flad and
    // use protection OFF
    const bool callback_protection_flag = PROTECTION_OFF;
#endif  // ENABLE_SECURITY

    const protocol_handler::SessionContext context =
        GetSessionContext(connection_id,
                          NEW_SESSION_ID,
                          session_id,
                          start_service,
                          HASH_ID_WRONG,
                          callback_protection_flag);

    // Expect ConnectionHandler check
    EXPECT_CALL(session_observer_mock,
                OnSessionStartedCallback(connection_id,
                                         NEW_SESSION_ID,
                                         start_service,
                                         callback_protection_flag,
                                         An<const BsonObject*>()))
        .
        // Return sessions start success
        WillOnce(DoAll(
            NotifyTestAsyncWaiter(waiter),
            InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                     &ProtocolHandler::NotifySessionStarted,
                                     context,
                                     ByRef(empty_rejected_param_))));
    times++;

    // Expect send Ack with PROTECTION_OFF (on no Security Manager)
    EXPECT_CALL(transport_manager_mock,
                SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK,
                                                   PROTECTION_OFF)))
        .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
    times++;

    SendControlMessage(
        PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);

    usleep(kAddSessionWaitTimeMs * kMicrosecondsInMillisecond);
  }

#ifdef ENABLE_SECURITY
  // Emulate security manager initilization establish
  void AddSecurityManager() {
    protocol_handler_impl->set_security_manager(&security_manager_mock);
  }
#endif  // ENABLE_SECURITY

  void SendTMMessage(uint8_t connection_id,
                     uint8_t version,
                     bool protection,
                     uint8_t frameType,
                     uint8_t serviceType,
                     uint8_t frameData,
                     uint8_t sessionId,
                     uint32_t dataSize,
                     uint32_t messageID,
                     const uint8_t* data = 0) {
    // Create packet
    const ProtocolPacket packet(connection_id,
                                version,
                                protection,
                                frameType,
                                serviceType,
                                frameData,
                                sessionId,
                                dataSize,
                                messageID,
                                data);
    // Emulate receive packet from transoprt manager
    tm_listener->OnTMMessageReceived(packet.serializePacket());
  }

  void SetProtocolVersion2() {
    // Set protocol version 2
    ON_CALL(protocol_handler_settings_mock, max_supported_protocol_version())
        .WillByDefault(Return(PROTOCOL_VERSION_2));
  }

  void SendControlMessage(bool protection,
                          uint8_t service_type,
                          uint8_t sessionId,
                          uint32_t frame_data,
                          uint8_t protocol_version = PROTOCOL_VERSION_3,
                          uint32_t dataSize = 0u,
                          const uint8_t* data = NULL) {
    SendTMMessage(connection_id,
                  protocol_version,
                  protection,
                  FRAME_TYPE_CONTROL,
                  service_type,
                  frame_data,
                  sessionId,
                  dataSize,
                  message_id,
                  data);
  }

  void VerifySecondaryTransportParamsInStartSessionAck(
      bool config_multiple_transports_enabled,
      const std::vector<std::string>& config_secondary_transports_for_usb,
      const std::vector<std::string>& config_secondary_transports_for_bluetooth,
      const std::vector<std::string>& config_secondary_transports_for_wifi,
      const std::vector<std::string>& config_audio_service_transports,
      const std::vector<std::string>& config_video_service_transports,
      const std::string& connection_type_string,
      const std::vector<std::string>& expected_transport_strings,
      const std::vector<int32_t>& expected_audio_service_transports,
      const std::vector<int32_t>& expected_video_service_transports);

  testing::NiceMock<MockProtocolHandlerSettings> protocol_handler_settings_mock;
  std::shared_ptr<ProtocolHandlerImpl> protocol_handler_impl;
  TransportManagerListener* tm_listener;
  // Uniq connection
  ::transport_manager::ConnectionUID connection_id;
  // Id of established session
  uint8_t session_id;
  // Uniq id as connection_id and session_id in one
  uint32_t connection_key;
  uint32_t message_id;
  UCharDataVector some_data;
  // Strict mocks (same as all methods EXPECT_CALL().Times(0))
  testing::NiceMock<connection_handler_test::MockConnectionHandler>
      connection_handler_mock;
  testing::StrictMock<transport_manager_test::MockTransportManager>
      transport_manager_mock;
  testing::StrictMock<protocol_handler_test::MockSessionObserver>
      session_observer_mock;
#ifdef ENABLE_SECURITY
  testing::NiceMock<security_manager_test::MockSecurityManager>
      security_manager_mock;
  testing::NiceMock<security_manager_test::MockSSLContext> ssl_context_mock;
  std::vector<int> force_protected_services;
  std::vector<int> force_unprotected_services;
#endif  // ENABLE_SECURITY
  std::vector<std::string> empty_rejected_param_;
  // Used by OnTransportConfigUpdated() tests. The lifetime of these objects
  // should be longer than that of a test case.
  connection_handler::SessionConnectionMap session_connection_map_;
  std::shared_ptr<sync_primitives::Lock> session_connection_map_lock_ptr_;
};

#ifdef ENABLE_SECURITY
class OnHandshakeDoneFunctor {
 public:
  OnHandshakeDoneFunctor(const uint32_t connection_key,
                         security_manager::SSLContext::HandshakeResult error)
      : connection_key(connection_key), result(error) {}
  void operator()(security_manager::SecurityManagerListener* listener) const {
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
  EXPECT_CALL(transport_manager_mock, DisconnectForce(connection_id))
      .WillOnce(Return(E_SUCCESS));

  SendTMMessage(connection_id,
                PROTOCOL_VERSION_3,
                PROTECTION_OFF,
                FRAME_TYPE_CONTROL,
                kRpc,
                FRAME_DATA_START_SERVICE,
                NEW_SESSION_ID,
                0,
                message_id);
}
/*
 * ProtocolHandler shall send NAck on session_observer rejection
 * Check protection flag OFF for all services from kControl to kBulk
 */
TEST_F(ProtocolHandlerImplTest,
       StartSession_Unprotected_SessionObserverReject) {
  using namespace protocol_handler;
  const int call_times = 5;
  AddConnection();

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  ServiceType service_type;
  // Expect ConnectionHandler check
  EXPECT_CALL(
      session_observer_mock,
      OnSessionStartedCallback(connection_id,
                               NEW_SESSION_ID,
                               AnyOf(kControl, kRpc, kAudio, kMobileNav, kBulk),
                               PROTECTION_OFF,
                               An<const BsonObject*>()))
      .Times(call_times)
      .
      // Return sessions start rejection
      WillRepeatedly(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                SaveArg<2>(&service_type),
                InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                         &ProtocolHandler::NotifySessionStarted,
                                         GetSessionContext(connection_id,
                                                           NEW_SESSION_ID,
                                                           SESSION_START_REJECT,
                                                           service_type,
                                                           HASH_ID_WRONG,
                                                           PROTECTION_OFF),
                                         ByRef(empty_rejected_param_))));
  times += call_times;

  // Expect send NAck
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_NACK,
                                                 PROTECTION_OFF)))
      .Times(call_times)
      .WillRepeatedly(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times += call_times;

  SendControlMessage(
      PROTECTION_OFF, kControl, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
  SendControlMessage(
      PROTECTION_OFF, kRpc, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
  SendControlMessage(
      PROTECTION_OFF, kAudio, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
  SendControlMessage(
      PROTECTION_OFF, kMobileNav, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
  SendControlMessage(
      PROTECTION_OFF, kBulk, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * ProtocolHandler shall send NAck on session_observer rejection
 * Emulate getting PROTECTION_ON and check protection flag OFF in NAck
 * For ENABLE_SECURITY=OFF session_observer shall be called with protection flag
 * OFF
 */
TEST_F(ProtocolHandlerImplTest, StartSession_Protected_SessionObserverReject) {
  using namespace protocol_handler;
  const int call_times = 5;
  AddConnection();
#ifdef ENABLE_SECURITY
  // For enabled protection callback shall use protection ON
  const bool callback_protection_flag = PROTECTION_ON;
#else
  // For disabled protection callback shall ignore protection income flag and
  // use protection OFF
  const bool callback_protection_flag = PROTECTION_OFF;
#endif  // ENABLE_SECURITY

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  ServiceType service_type;
  // Expect ConnectionHandler check
  EXPECT_CALL(
      session_observer_mock,
      OnSessionStartedCallback(connection_id,
                               NEW_SESSION_ID,
                               AnyOf(kControl, kRpc, kAudio, kMobileNav, kBulk),
                               callback_protection_flag,
                               An<const BsonObject*>()))
      .Times(call_times)
      .
      // Return sessions start rejection
      WillRepeatedly(DoAll(
          NotifyTestAsyncWaiter(&waiter),
          SaveArg<2>(&service_type),
          InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                   &ProtocolHandler::NotifySessionStarted,
                                   GetSessionContext(connection_id,
                                                     NEW_SESSION_ID,
                                                     SESSION_START_REJECT,
                                                     service_type,
                                                     HASH_ID_WRONG,
                                                     callback_protection_flag),
                                   ByRef(empty_rejected_param_))));
  times += call_times;

  // Expect send NAck with encryption OFF
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_NACK,
                                                 PROTECTION_OFF)))
      .Times(call_times)
      .WillRepeatedly(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times += call_times;

  SendControlMessage(
      PROTECTION_ON, kControl, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
  SendControlMessage(
      PROTECTION_ON, kRpc, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
  SendControlMessage(
      PROTECTION_ON, kAudio, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
  SendControlMessage(
      PROTECTION_ON, kMobileNav, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);
  SendControlMessage(
      PROTECTION_ON, kBulk, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * ProtocolHandler shall send Ack on session_observer accept
 * Check protection flag OFF
 */
TEST_F(ProtocolHandlerImplTest,
       StartSession_Unprotected_SessionObserverAccept) {
  using namespace protocol_handler;
  AddConnection();
  const ServiceType start_service = kRpc;

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
              OnSessionStartedCallback(connection_id,
                                       NEW_SESSION_ID,
                                       start_service,
                                       PROTECTION_OFF,
                                       An<const BsonObject*>()))
      .
      // Return sessions start success
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                         &ProtocolHandler::NotifySessionStarted,
                                         GetSessionContext(connection_id,
                                                           NEW_SESSION_ID,
                                                           session_id,
                                                           start_service,
                                                           HASH_ID_WRONG,
                                                           PROTECTION_OFF),
                                         ByRef(empty_rejected_param_))));
  times++;

  SetProtocolVersion2();
  // Expect send Ack
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  SendControlMessage(
      PROTECTION_OFF, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * ProtocolHandler shall send Ack on session_observer accept
 * Emulate getting PROTECTION_ON and check protection flag OFF in Ack
 * For ENABLE_SECURITY=OFF session_observer shall be called with protection flag
 * OFF
 */
TEST_F(ProtocolHandlerImplTest, StartSession_Protected_SessionObserverAccept) {
  SetProtocolVersion2();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

static std::vector<uint8_t> CreateVectorFromBsonObject(const BsonObject* bo) {
  std::vector<uint8_t> output;
  if (bo != NULL) {
    size_t len = bson_object_size(const_cast<BsonObject*>(bo));
    uint8_t* bytes = bson_object_to_bytes(const_cast<BsonObject*>(bo));
    output.assign(bytes, bytes + len);
    free(bytes);
  }
  return output;
}

/*
 * Simulate two StartService messages of video service from mobile.
 * Session observer accepts the first message with delay, while rejects the
 * second message immediately.
 */
TEST_F(ProtocolHandlerImplTest,
       StartSession_Unprotected_Multiple_SessionObserverAcceptAndReject) {
  using namespace protocol_handler;

  ON_CALL(protocol_handler_settings_mock, max_supported_protocol_version())
      .WillByDefault(Return(PROTOCOL_VERSION_5));

  const size_t maximum_payload_size = 1000;
  InitProtocolHandlerImpl(0u, 0u, false, 0u, 0u, 0, maximum_payload_size);

  const ServiceType start_service = kMobileNav;
  const ::transport_manager::ConnectionUID connection_id1 = 0xAu;
  const uint8_t session_id1 = 1u;
  const ::transport_manager::ConnectionUID connection_id2 = 0xBu;
  const uint8_t session_id2 = 2u;

#ifdef ENABLE_SECURITY
  AddSecurityManager();

  EXPECT_CALL(session_observer_mock, KeyFromPair(connection_id2, session_id2))
      .WillOnce(Return(connection_key));

  EXPECT_CALL(session_observer_mock,
              GetSSLContext(connection_key, start_service))
      .Times(2)
      .WillRepeatedly(ReturnNull());
#endif  // ENABLE_SECURITY

  EXPECT_CALL(session_observer_mock, IsHeartBeatSupported(connection_id1, _))
      .WillRepeatedly(Return(false));
  EXPECT_CALL(session_observer_mock, IsHeartBeatSupported(connection_id2, _))
      .WillRepeatedly(Return(false));

  // Add two connections
  tm_listener->OnConnectionEstablished(DeviceInfo(DeviceHandle(1u),
                                                  std::string("mac"),
                                                  std::string("name"),
                                                  std::string("BTMAC")),
                                       connection_id1);
  tm_listener->OnConnectionEstablished(DeviceInfo(DeviceHandle(2u),
                                                  std::string("mac"),
                                                  std::string("name"),
                                                  std::string("BTMAC")),
                                       connection_id2);

  TestAsyncWaiter waiter;
  uint32_t times = 0;

  BsonObject bson_params1;
  bson_object_initialize_default(&bson_params1);
  bson_object_put_string(&bson_params1,
                         protocol_handler::strings::video_protocol,
                         const_cast<char*>("RAW"));
  bson_object_put_string(&bson_params1,
                         protocol_handler::strings::video_codec,
                         const_cast<char*>("H264"));
  std::vector<uint8_t> params1 = CreateVectorFromBsonObject(&bson_params1);

  uint8_t generated_session_id1 = 100;

  EXPECT_CALL(session_observer_mock,
              OnSessionStartedCallback(connection_id1,
                                       session_id1,
                                       start_service,
                                       PROTECTION_OFF,
                                       An<const BsonObject*>()))
      // don't call NotifySessionStartedContext() immediately, instead call it
      // after second OnSessionStartedCallback()
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  BsonObject bson_params2;
  bson_object_initialize_default(&bson_params2);
  bson_object_put_string(&bson_params2,
                         protocol_handler::strings::video_protocol,
                         const_cast<char*>("RTP"));
  bson_object_put_string(&bson_params2,
                         protocol_handler::strings::video_codec,
                         const_cast<char*>("H265"));
  std::vector<uint8_t> params2 = CreateVectorFromBsonObject(&bson_params2);

  std::vector<std::string> rejected_param_list;
  rejected_param_list.push_back(protocol_handler::strings::video_codec);

  EXPECT_CALL(session_observer_mock,
              OnSessionStartedCallback(connection_id2,
                                       session_id2,
                                       start_service,
                                       PROTECTION_OFF,
                                       An<const BsonObject*>()))
      .WillOnce(DoAll(
          NotifyTestAsyncWaiter(&waiter),
          InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                   &ProtocolHandler::NotifySessionStarted,
                                   GetSessionContext(connection_id2,
                                                     session_id2,
                                                     SESSION_START_REJECT,
                                                     start_service,
                                                     HASH_ID_WRONG,
                                                     PROTECTION_OFF),
                                   ByRef(rejected_param_list)),
          InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                   &ProtocolHandler::NotifySessionStarted,
                                   GetSessionContext(connection_id1,
                                                     session_id1,
                                                     generated_session_id1,
                                                     start_service,
                                                     HASH_ID_WRONG,
                                                     PROTECTION_OFF),
                                   ByRef(empty_rejected_param_))));
  times++;

  BsonObject bson_ack_params;
  bson_object_initialize_default(&bson_ack_params);
  bson_object_put_int64(
      &bson_ack_params, protocol_handler::strings::mtu, maximum_payload_size);
  bson_object_put_string(&bson_ack_params,
                         protocol_handler::strings::video_protocol,
                         const_cast<char*>("RAW"));
  bson_object_put_string(&bson_ack_params,
                         protocol_handler::strings::video_codec,
                         const_cast<char*>("H264"));
  std::vector<uint8_t> ack_params =
      CreateVectorFromBsonObject(&bson_ack_params);
  bson_object_deinitialize(&bson_ack_params);

  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK,
                                                 PROTECTION_OFF,
                                                 connection_id1,
                                                 Eq(ack_params))))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  BsonArray bson_arr;
  bson_array_initialize(&bson_arr, rejected_param_list.size());
  for (unsigned int i = 0; i < rejected_param_list.size(); i++) {
    bson_array_add_string(&bson_arr,
                          const_cast<char*>(rejected_param_list[i].c_str()));
  }
  BsonObject bson_nack_params;
  bson_object_initialize_default(&bson_nack_params);
  bson_object_put_array(
      &bson_nack_params, protocol_handler::strings::rejected_params, &bson_arr);
  std::vector<uint8_t> nack_params =
      CreateVectorFromBsonObject(&bson_nack_params);
  bson_object_deinitialize(&bson_nack_params);

  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_NACK,
                                                 PROTECTION_OFF,
                                                 connection_id2,
                                                 Eq(nack_params))))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  SendTMMessage(connection_id1,
                PROTOCOL_VERSION_5,
                PROTECTION_OFF,
                FRAME_TYPE_CONTROL,
                start_service,
                FRAME_DATA_START_SERVICE,
                session_id1,
                params1.size(),
                message_id,
                params1.size() > 0 ? &params1[0] : NULL);

  SendTMMessage(connection_id2,
                PROTOCOL_VERSION_5,
                PROTECTION_OFF,
                FRAME_TYPE_CONTROL,
                start_service,
                FRAME_DATA_START_SERVICE,
                session_id2,
                params2.size(),
                message_id,
                params2.size() > 0 ? &params2[0] : NULL);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));

  bson_object_deinitialize(&bson_params1);
  bson_object_deinitialize(&bson_params2);
}

// TODO(EZamakhov): add test for get_hash_id/set_hash_id from
// protocol_handler_impl.cc
/*
 * ProtocolHandler shall send NAck on session_observer rejection
 */
TEST_F(ProtocolHandlerImplTest, EndSession_SessionObserverReject) {
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);
  const ServiceType service = kRpc;

  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
              OnSessionEndedCallback(
                  connection_id, session_id, An<uint32_t*>(), service))
      .
      // reject session start
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(waiter), Return(SESSION_START_REJECT)));
  times++;

  SetProtocolVersion2();
  // Expect send NAck
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_END_SERVICE_NACK, PROTECTION_OFF)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
  times++;

  SendControlMessage(
      PROTECTION_OFF, service, session_id, FRAME_DATA_END_SERVICE);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * ProtocolHandler shall send NAck on wrong hash code
 */
TEST_F(ProtocolHandlerImplTest, EndSession_Success) {
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  const ServiceType service = kRpc;

  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
              OnSessionEndedCallback(
                  connection_id, session_id, An<uint32_t*>(), service))
      .
      // return sessions start success
      WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(connection_key)));
  times++;

  SetProtocolVersion2();
  // Expect send Ack
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_END_SERVICE_ACK, PROTECTION_OFF)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
  times++;

  SendControlMessage(
      PROTECTION_OFF, service, session_id, FRAME_DATA_END_SERVICE);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

#ifdef ENABLE_SECURITY
TEST_F(ProtocolHandlerImplTest, SecurityEnable_StartSessionProtocoloV1) {
  using namespace protocol_handler;
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Add security manager
  AddSecurityManager();
  const ServiceType start_service = kRpc;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
              OnSessionStartedCallback(connection_id,
                                       NEW_SESSION_ID,
                                       start_service,
                                       PROTECTION_OFF,
                                       An<const BsonObject*>()))
      .
      // Return sessions start success
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(waiter),
                InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                         &ProtocolHandler::NotifySessionStarted,
                                         GetSessionContext(connection_id,
                                                           NEW_SESSION_ID,
                                                           session_id,
                                                           start_service,
                                                           HASH_ID_WRONG,
                                                           PROTECTION_OFF),
                                         ByRef(empty_rejected_param_))));
  times++;

  SetProtocolVersion2();
  // Expect send Ack with PROTECTION_OFF (on no Security Manager)
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)))
      .RetiresOnSaturation();
  times++;

  SendTMMessage(connection_id,
                PROTOCOL_VERSION_1,
                PROTECTION_ON,
                FRAME_TYPE_CONTROL,
                start_service,
                FRAME_DATA_START_SERVICE,
                NEW_SESSION_ID,
                0,
                message_id);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * ProtocolHandler shall not call Security logics on start session with
 * PROTECTION_OFF
 */
TEST_F(ProtocolHandlerImplTest, SecurityEnable_StartSessionUnprotected) {
  using namespace protocol_handler;
  AddConnection();
  // Add security manager
  AddSecurityManager();
  const ServiceType start_service = kRpc;

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
              OnSessionStartedCallback(connection_id,
                                       NEW_SESSION_ID,
                                       start_service,
                                       PROTECTION_OFF,
                                       An<const BsonObject*>()))
      .
      // Return sessions start success
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                         &ProtocolHandler::NotifySessionStarted,
                                         GetSessionContext(connection_id,
                                                           NEW_SESSION_ID,
                                                           session_id,
                                                           start_service,
                                                           HASH_ID_WRONG,
                                                           PROTECTION_OFF),
                                         ByRef(empty_rejected_param_))));
  times++;

  SetProtocolVersion2();
  // Expect send Ack with PROTECTION_OFF (on no Security Manager)
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  SendControlMessage(
      PROTECTION_OFF, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_OFF on fail SLL creation
 */
TEST_F(ProtocolHandlerImplTest, SecurityEnable_StartSessionProtected_Fail) {
  using namespace protocol_handler;
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;

  TestAsyncWaiter waiter;
  uint32_t times = 0;

  protocol_handler::SessionContext context = GetSessionContext(connection_id,
                                                               NEW_SESSION_ID,
                                                               session_id,
                                                               start_service,
                                                               HASH_ID_WRONG,
                                                               PROTECTION_ON);
  context.is_new_service_ = true;

  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
              OnSessionStartedCallback(connection_id,
                                       NEW_SESSION_ID,
                                       start_service,
                                       PROTECTION_ON,
                                       An<const BsonObject*>()))
      .
      // Return sessions start success
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                         &ProtocolHandler::NotifySessionStarted,
                                         context,
                                         ByRef(empty_rejected_param_))));
  times++;

  SetProtocolVersion2();
  // Expect start protection for unprotected session
  EXPECT_CALL(security_manager_mock,
              CreateSSLContext(connection_key,
                               security_manager::SecurityManager::
                                   ContextCreationStrategy::kUseExisting))
      .
      // Return fail protection
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), ReturnNull()));
  times++;

  // Expect send Ack with PROTECTION_OFF (on fail SLL creation)
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  SendControlMessage(
      PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_ON on already established and
 * initialized SLLContext
 */
TEST_F(ProtocolHandlerImplTest,
       SecurityEnable_StartSessionProtected_SSLInitialized) {
  using namespace protocol_handler;
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
              OnSessionStartedCallback(connection_id,
                                       NEW_SESSION_ID,
                                       start_service,
                                       PROTECTION_ON,
                                       An<const BsonObject*>()))
      .
      // Return sessions start success
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                         &ProtocolHandler::NotifySessionStarted,
                                         GetSessionContext(connection_id,
                                                           NEW_SESSION_ID,
                                                           session_id,
                                                           start_service,
                                                           HASH_ID_WRONG,
                                                           PROTECTION_ON),
                                         ByRef(empty_rejected_param_))));
  times++;

  SetProtocolVersion2();
  // call new SSLContext creation
  EXPECT_CALL(security_manager_mock, CreateSSLContext(connection_key, _))
      .
      // Return new SSLContext
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter), Return(&ssl_context_mock)));
  times++;

  // Initilization check
  EXPECT_CALL(ssl_context_mock, IsInitCompleted())
      .
      // emulate SSL is initilized
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times++;

  // Expect service protection enable
  EXPECT_CALL(session_observer_mock,
              SetProtectionFlag(connection_key, start_service))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  // Expect send Ack with PROTECTION_ON (on SSL is initilized)
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_ON)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  SendControlMessage(
      PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_OFF on session handshhake fail
 */
TEST_F(ProtocolHandlerImplTest,
       SecurityEnable_StartSessionProtected_HandshakeFail) {
  using namespace protocol_handler;
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  protocol_handler::SessionContext context = GetSessionContext(connection_id,
                                                               NEW_SESSION_ID,
                                                               session_id,
                                                               start_service,
                                                               HASH_ID_WRONG,
                                                               PROTECTION_ON);
  context.is_new_service_ = true;

  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
              OnSessionStartedCallback(connection_id,
                                       NEW_SESSION_ID,
                                       start_service,
                                       PROTECTION_ON,
                                       An<const BsonObject*>()))
      .
      // Return sessions start success
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                         &ProtocolHandler::NotifySessionStarted,
                                         context,
                                         ByRef(empty_rejected_param_))));
  times++;

  std::vector<int> services;
  // TODO(AKutsan) : APPLINK-21398 use named constants instead of magic numbers
  services.push_back(0x0A);
  services.push_back(0x0B);
  EXPECT_CALL(protocol_handler_settings_mock, force_protected_service())
      .WillOnce(ReturnRefOfCopy(services));

  // call new SSLContext creation
  EXPECT_CALL(security_manager_mock, CreateSSLContext(connection_key, _))
      .
      // Return new SSLContext
      WillOnce(Return(&ssl_context_mock));

  // Initilization check
  EXPECT_CALL(ssl_context_mock, IsInitCompleted())
      .
      // emulate SSL is not initilized
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(false)));
  times++;

  // Pending handshake check
  EXPECT_CALL(ssl_context_mock, IsHandshakePending())
      .
      // emulate is pending
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times++;

  // Expect add listener for handshake result
  EXPECT_CALL(security_manager_mock, AddListener(_))
      // Emulate handshake fail
      .WillOnce(Invoke(OnHandshakeDoneFunctor(
          connection_key,
          security_manager::SSLContext::Handshake_Result_Fail)));

  // Expect send Ack with PROTECTION_OFF (on fail handshake)
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  SendControlMessage(
      PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_ON on session handshhake
 * success
 */
TEST_F(ProtocolHandlerImplTest,
       SecurityEnable_StartSessionProtected_HandshakeSuccess) {
  using namespace protocol_handler;
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;

  // No services are protected
  std::vector<int> services;
  ON_CALL(protocol_handler_settings_mock, force_protected_service())
      .WillByDefault(ReturnRefOfCopy(services));

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
              OnSessionStartedCallback(connection_id,
                                       NEW_SESSION_ID,
                                       start_service,
                                       PROTECTION_ON,
                                       An<const BsonObject*>()))
      .
      // Return sessions start success
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                         &ProtocolHandler::NotifySessionStarted,
                                         GetSessionContext(connection_id,
                                                           NEW_SESSION_ID,
                                                           session_id,
                                                           start_service,
                                                           HASH_ID_WRONG,
                                                           PROTECTION_ON),
                                         ByRef(empty_rejected_param_))));
  times++;

  // call new SSLContext creation
  EXPECT_CALL(security_manager_mock, CreateSSLContext(connection_key, _))
      .
      // Return new SSLContext
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter), Return(&ssl_context_mock)));
  times++;

  // Initilization check
  EXPECT_CALL(ssl_context_mock, IsInitCompleted())
      .
      // emulate SSL is not initilized
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(false)));
  times++;

  // Pending handshake check
  EXPECT_CALL(ssl_context_mock, IsHandshakePending())
      .
      // emulate is pending
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times++;

  // Expect add listener for handshake result
  EXPECT_CALL(security_manager_mock, AddListener(_))
      // Emulate handshake fail
      .WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                Invoke(OnHandshakeDoneFunctor(
                    connection_key,
                    security_manager::SSLContext::Handshake_Result_Success))));
  times++;

  // Listener check SSLContext
  EXPECT_CALL(session_observer_mock,
              GetSSLContext(connection_key, start_service))
      .
      // Emulate protection for service is not enabled
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), ReturnNull()));
  times++;

  // Expect service protection enable
  EXPECT_CALL(session_observer_mock,
              SetProtectionFlag(connection_key, start_service))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  // Expect send Ack with PROTECTION_OFF (on fail handshake)
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_ON)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  SendControlMessage(
      PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_ON on session handshhake
 * success
 */
TEST_F(
    ProtocolHandlerImplTest,
    SecurityEnable_StartSessionProtected_HandshakeSuccess_ServiceProtectedBefore) {
  using namespace protocol_handler;
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;

  std::vector<int> services;
  ON_CALL(protocol_handler_settings_mock, force_protected_service())
      .WillByDefault(ReturnRefOfCopy(services));

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
              OnSessionStartedCallback(connection_id,
                                       NEW_SESSION_ID,
                                       start_service,
                                       PROTECTION_ON,
                                       An<const BsonObject*>()))
      .
      // Return sessions start success
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                         &ProtocolHandler::NotifySessionStarted,
                                         GetSessionContext(connection_id,
                                                           NEW_SESSION_ID,
                                                           session_id,
                                                           start_service,
                                                           HASH_ID_WRONG,
                                                           PROTECTION_ON),
                                         ByRef(empty_rejected_param_))));
  times++;

  // call new SSLContext creation
  EXPECT_CALL(security_manager_mock, CreateSSLContext(connection_key, _))
      .
      // Return new SSLContext
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter), Return(&ssl_context_mock)));
  times++;

  // Initilization check
  EXPECT_CALL(ssl_context_mock, IsInitCompleted())
      .
      // emulate SSL is not initilized
      WillOnce(Return(false));

  // Pending handshake check
  EXPECT_CALL(ssl_context_mock, IsHandshakePending())
      .
      // emulate is pending
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times++;

  // Expect add listener for handshake result
  EXPECT_CALL(security_manager_mock, AddListener(_))
      // Emulate handshake fail
      .WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                Invoke(OnHandshakeDoneFunctor(
                    connection_key,
                    security_manager::SSLContext::Handshake_Result_Success))));
  times++;

  // Listener check SSLContext
  EXPECT_CALL(session_observer_mock,
              GetSSLContext(connection_key, start_service))
      .
      // Emulate protection for service is not enabled
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), ReturnNull()));
  times++;

  // Expect service protection enable
  EXPECT_CALL(session_observer_mock,
              SetProtectionFlag(connection_key, start_service))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  // Expect send Ack with PROTECTION_OFF (on fail handshake)
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_ON)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  SendControlMessage(
      PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}
/*
 * ProtocolHandler shall send Ack with PROTECTION_ON on session handshhake
 * success
 */
TEST_F(ProtocolHandlerImplTest,
       SecurityEnable_StartSessionProtected_HandshakeSuccess_SSLIsNotPending) {
  using namespace protocol_handler;
  AddConnection();
  AddSecurityManager();
  const ServiceType start_service = kRpc;

  std::vector<int> services;
  ON_CALL(protocol_handler_settings_mock, force_protected_service())
      .WillByDefault(ReturnRefOfCopy(services));

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  // Expect ConnectionHandler check
  EXPECT_CALL(session_observer_mock,
              OnSessionStartedCallback(connection_id,
                                       NEW_SESSION_ID,
                                       start_service,
                                       PROTECTION_ON,
                                       An<const BsonObject*>()))
      .
      // Return sessions start success
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                InvokeMemberFuncWithArg2(protocol_handler_impl.get(),
                                         &ProtocolHandler::NotifySessionStarted,
                                         GetSessionContext(connection_id,
                                                           NEW_SESSION_ID,
                                                           session_id,
                                                           start_service,
                                                           HASH_ID_WRONG,
                                                           PROTECTION_ON),
                                         ByRef(empty_rejected_param_))));
  times++;

  // call new SSLContext creation
  EXPECT_CALL(security_manager_mock,
              CreateSSLContext(connection_key,
                               security_manager::SecurityManager::
                                   ContextCreationStrategy::kUseExisting))
      .
      // Return new SSLContext
      WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter), Return(&ssl_context_mock)));
  times++;

  // Initilization check
  EXPECT_CALL(ssl_context_mock, IsInitCompleted())
      .
      // emulate SSL is not initilized
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(false)));
  times++;

  // Pending handshake check
  EXPECT_CALL(ssl_context_mock, IsHandshakePending())
      .
      // emulate is pending
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(false)));
  times++;

  // Wait restart handshake operation
  EXPECT_CALL(security_manager_mock, StartHandshake(connection_key))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  // Expect add listener for handshake result
  EXPECT_CALL(security_manager_mock, AddListener(_))
      // Emulate handshake
      .WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter),
                Invoke(OnHandshakeDoneFunctor(
                    connection_key,
                    security_manager::SSLContext::Handshake_Result_Success))));
  times++;

  // Listener check SSLContext
  EXPECT_CALL(session_observer_mock,
              GetSSLContext(connection_key, start_service))
      .
      // Emulate protection for service is not enabled
      WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), ReturnNull()));
  times++;

  EXPECT_CALL(security_manager_mock, IsSystemTimeProviderReady())
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(true)));
  times++;

  EXPECT_CALL(session_observer_mock,
              SetProtectionFlag(connection_key, start_service))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  //   Expect send Ack with PROTECTION_ON (on successfull handshake)
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_START_SERVICE_ACK, PROTECTION_ON)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));

  times++;

  SendControlMessage(
      PROTECTION_ON, start_service, NEW_SESSION_ID, FRAME_DATA_START_SERVICE);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}
#endif  // ENABLE_SECURITY

void ProtocolHandlerImplTest::VerifySecondaryTransportParamsInStartSessionAck(
    bool config_multiple_transports_enabled,
    const std::vector<std::string>& config_secondary_transports_for_usb,
    const std::vector<std::string>& config_secondary_transports_for_bluetooth,
    const std::vector<std::string>& config_secondary_transports_for_wifi,
    const std::vector<std::string>& config_audio_service_transports,
    const std::vector<std::string>& config_video_service_transports,
    const std::string& connection_type_string,
    const std::vector<std::string>& expected_transport_strings,
    const std::vector<int32_t>& expected_audio_service_transports,
    const std::vector<int32_t>& expected_video_service_transports) {
  const size_t maximum_rpc_payload_size = 1500;
  EXPECT_CALL(protocol_handler_settings_mock, maximum_rpc_payload_size())
      .WillRepeatedly(Return(maximum_rpc_payload_size));
  InitProtocolHandlerImpl(0u, 0u);

  TestAsyncWaiter waiter;
  uint32_t times = 0;

  const uint8_t input_protocol_version = 5;
  const uint32_t hash_id = 123456;
  utils::SemanticVersion full_version(5, 1, 0);
  char full_version_string[] = "5.1.0";

  // configuration setup
  EXPECT_CALL(protocol_handler_settings_mock, max_supported_protocol_version())
      .WillRepeatedly(Return(PROTOCOL_VERSION_5));
  EXPECT_CALL(protocol_handler_settings_mock, multiple_transports_enabled())
      .WillRepeatedly(Return(config_multiple_transports_enabled));
  EXPECT_CALL(protocol_handler_settings_mock, secondary_transports_for_usb())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_usb));
  EXPECT_CALL(protocol_handler_settings_mock,
              secondary_transports_for_bluetooth())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_bluetooth));
  EXPECT_CALL(protocol_handler_settings_mock, secondary_transports_for_wifi())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_wifi));
  EXPECT_CALL(protocol_handler_settings_mock, audio_service_transports())
      .WillOnce(ReturnRef(config_audio_service_transports));
  EXPECT_CALL(protocol_handler_settings_mock, video_service_transports())
      .WillOnce(ReturnRef(config_video_service_transports));

  EXPECT_CALL(session_observer_mock,
              TransportTypeProfileStringFromConnHandle(connection_id))
      .WillRepeatedly(Return(connection_type_string));

  // Prepare expected BSON parameters. When we add another param in Start
  // Service ACK frame in future, it should be also added here.
  BsonObject expected_obj;
  bson_object_initialize_default(&expected_obj);
  // mtu
  bson_object_put_int64(&expected_obj,
                        protocol_handler::strings::mtu,
                        static_cast<int64_t>(maximum_rpc_payload_size));
  // hashId
  bson_object_put_int32(&expected_obj,
                        protocol_handler::strings::hash_id,
                        static_cast<int32_t>(hash_id));
  // protocolVersion
  bson_object_put_string(&expected_obj,
                         protocol_handler::strings::protocol_version,
                         full_version_string);
  // secondaryTransports
  BsonArray secondary_transports;
  bson_array_initialize(&secondary_transports,
                        expected_transport_strings.size());
  for (std::vector<std::string>::const_iterator it =
           expected_transport_strings.begin();
       it != expected_transport_strings.end();
       ++it) {
    // note: if there is no transport allowed, we can either make the array
    // empty, or completely omit the array. (The spec allows both cases.) In
    // this test case we make the array empty.
    bson_array_add_string(&secondary_transports,
                          const_cast<char*>(it->c_str()));
  }
  bson_object_put_array(&expected_obj,
                        protocol_handler::strings::secondary_transports,
                        &secondary_transports);
  // audioServiceTransports
  BsonArray audio_service_transports;
  if (expected_audio_service_transports.size() > 0) {
    bson_array_initialize(&audio_service_transports,
                          expected_audio_service_transports.size());
    for (std::vector<int32_t>::const_iterator it =
             expected_audio_service_transports.begin();
         it != expected_audio_service_transports.end();
         ++it) {
      bson_array_add_int32(&audio_service_transports, *it);
    }
    bson_object_put_array(&expected_obj,
                          protocol_handler::strings::audio_service_transports,
                          &audio_service_transports);
  }
  // videoServiceTransports
  BsonArray video_service_transports;
  if (expected_video_service_transports.size() > 0) {
    bson_array_initialize(&video_service_transports,
                          expected_video_service_transports.size());
    for (std::vector<int32_t>::const_iterator it =
             expected_video_service_transports.begin();
         it != expected_video_service_transports.end();
         ++it) {
      bson_array_add_int32(&video_service_transports, *it);
    }
    bson_object_put_array(&expected_obj,
                          protocol_handler::strings::video_service_transports,
                          &video_service_transports);
  }

  std::vector<uint8_t> expected_param =
      CreateVectorFromBsonObject(&expected_obj);

  bson_object_deinitialize(&expected_obj);

  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK,
                                                 PROTECTION_OFF,
                                                 connection_id,
                                                 Eq(expected_param))))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

#ifdef ENABLE_SECURITY
  AddSecurityManager();

  EXPECT_CALL(session_observer_mock, KeyFromPair(connection_id, session_id))
      .WillOnce(Return(connection_key));

  EXPECT_CALL(session_observer_mock, GetSSLContext(connection_key, kRpc))
      .WillOnce(ReturnNull());
#endif  // ENABLE_SECURITY

  protocol_handler_impl->SendStartSessionAck(connection_id,
                                             session_id,
                                             input_protocol_version,
                                             hash_id,
                                             protocol_handler::SERVICE_TYPE_RPC,
                                             false /* protection */,
                                             full_version);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest,
       StartSessionAck_SecondaryTransportParams_Enabled) {
  // config allows secondary transport only when connected through Bluetooth,
  // and the secondary is Wi-Fi
  std::vector<std::string> secondary_transports_for_usb;  // empty
  std::vector<std::string> secondary_transports_for_bluetooth;
  secondary_transports_for_bluetooth.push_back("WiFi");
  std::vector<std::string> secondary_transports_for_wifi;  // empty
  // config allows video and audio services to run on all transports except
  // Bluetooth
  std::vector<std::string> audio_service_transports;
  audio_service_transports.push_back("IAP_USB");
  audio_service_transports.push_back("IAP_USB_HOST_MODE");
  audio_service_transports.push_back("IAP_USB_DEVICE_MODE");
  audio_service_transports.push_back("IAP_CARPLAY");
  audio_service_transports.push_back("AOA_USB");
  audio_service_transports.push_back("TCP_WIFI");
  std::vector<std::string> video_service_transports;
  video_service_transports.push_back("IAP_USB");
  video_service_transports.push_back("IAP_USB_HOST_MODE");
  video_service_transports.push_back("IAP_USB_DEVICE_MODE");
  video_service_transports.push_back("IAP_CARPLAY");
  video_service_transports.push_back("AOA_USB");
  video_service_transports.push_back("TCP_WIFI");

  // assume the device is Android and is connected through Bluetooth SPP
  std::string connection_type_string("SPP_BLUETOOTH");

  // Core should specify WiFi for secondary transport, and should allow video
  // and audio services only on secondary transport
  std::vector<std::string> expected_transport_strings;
  expected_transport_strings.push_back("TCP_WIFI");
  std::vector<int32_t> expected_audio_service_transports;
  expected_audio_service_transports.push_back(2);
  std::vector<int32_t> expected_video_service_transports;
  expected_video_service_transports.push_back(2);

  // A TransportUpdateEvent is also issued after Start Service ACK. We don't
  // check it in this test case.
  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(Return(false));

  VerifySecondaryTransportParamsInStartSessionAck(
      true,
      secondary_transports_for_usb,
      secondary_transports_for_bluetooth,
      secondary_transports_for_wifi,
      audio_service_transports,
      video_service_transports,
      connection_type_string,
      expected_transport_strings,
      expected_audio_service_transports,
      expected_video_service_transports);
}

TEST_F(ProtocolHandlerImplTest,
       StartSessionAck_SecondaryTransportParams_NoSecondaryTransport) {
  // config allows secondary transport only when connected through Bluetooth,
  // and the secondary is Wi-Fi
  std::vector<std::string> secondary_transports_for_usb;  // empty
  std::vector<std::string> secondary_transports_for_bluetooth;
  secondary_transports_for_bluetooth.push_back("WiFi");
  std::vector<std::string> secondary_transports_for_wifi;  // empty
  // config allows video and audio services to run on all transports except
  // Bluetooth
  std::vector<std::string> audio_service_transports;
  audio_service_transports.push_back("IAP_USB");
  audio_service_transports.push_back("IAP_USB_HOST_MODE");
  audio_service_transports.push_back("IAP_USB_DEVICE_MODE");
  audio_service_transports.push_back("IAP_CARPLAY");
  audio_service_transports.push_back("AOA_USB");
  audio_service_transports.push_back("TCP_WIFI");
  std::vector<std::string> video_service_transports;
  video_service_transports.push_back("IAP_USB");
  video_service_transports.push_back("IAP_USB_HOST_MODE");
  video_service_transports.push_back("IAP_USB_DEVICE_MODE");
  video_service_transports.push_back("IAP_CARPLAY");
  video_service_transports.push_back("AOA_USB");
  video_service_transports.push_back("TCP_WIFI");

  // assume the device is iOS and is connected through iAP over USB
  std::string connection_type_string("IAP_USB");

  // Core should not offer any secondary transport. It will allow both video
  // and audio services on primary transport.
  std::vector<std::string> expected_transport_strings;  // empty
  std::vector<int32_t> expected_audio_service_transports;
  expected_audio_service_transports.push_back(1);
  std::vector<int32_t> expected_video_service_transports;
  expected_video_service_transports.push_back(1);

  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(Return(false));

  VerifySecondaryTransportParamsInStartSessionAck(
      true,
      secondary_transports_for_usb,
      secondary_transports_for_bluetooth,
      secondary_transports_for_wifi,
      audio_service_transports,
      video_service_transports,
      connection_type_string,
      expected_transport_strings,
      expected_audio_service_transports,
      expected_video_service_transports);
}

TEST_F(ProtocolHandlerImplTest,
       StartSessionAck_SecondaryTransportParams_MultipleSecondaryTransports) {
  // config allows secondary transport only when connected through Bluetooth,
  // and the secondary is Wi-Fi and USB
  std::vector<std::string> secondary_transports_for_usb;  // empty
  std::vector<std::string> secondary_transports_for_bluetooth;
  secondary_transports_for_bluetooth.push_back("WiFi");
  secondary_transports_for_bluetooth.push_back("USB");
  std::vector<std::string> secondary_transports_for_wifi;  // empty
  // config allows video and audio services to run on all transports except
  // Bluetooth
  std::vector<std::string> audio_service_transports;
  audio_service_transports.push_back("IAP_USB");
  audio_service_transports.push_back("IAP_USB_HOST_MODE");
  audio_service_transports.push_back("IAP_USB_DEVICE_MODE");
  audio_service_transports.push_back("IAP_CARPLAY");
  audio_service_transports.push_back("AOA_USB");
  audio_service_transports.push_back("TCP_WIFI");
  std::vector<std::string> video_service_transports;
  video_service_transports.push_back("IAP_USB");
  video_service_transports.push_back("IAP_USB_HOST_MODE");
  video_service_transports.push_back("IAP_USB_DEVICE_MODE");
  video_service_transports.push_back("IAP_CARPLAY");
  video_service_transports.push_back("AOA_USB");
  video_service_transports.push_back("TCP_WIFI");

  // assume the device is iOS and is connected through iAP over Bluetooth
  std::string connection_type_string("IAP_BLUETOOTH");

  // Core should offer both Wi-Fi and USB for secondary transport. Since the
  // device is iOS, Core should specify "IAP_USB".
  std::vector<std::string> expected_transport_strings;
  expected_transport_strings.push_back("TCP_WIFI");
  expected_transport_strings.push_back("IAP_USB");
  std::vector<int32_t> expected_audio_service_transports;
  expected_audio_service_transports.push_back(2);
  std::vector<int32_t> expected_video_service_transports;
  expected_video_service_transports.push_back(2);

  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(Return(false));

  VerifySecondaryTransportParamsInStartSessionAck(
      true,
      secondary_transports_for_usb,
      secondary_transports_for_bluetooth,
      secondary_transports_for_wifi,
      audio_service_transports,
      video_service_transports,
      connection_type_string,
      expected_transport_strings,
      expected_audio_service_transports,
      expected_video_service_transports);
}

TEST_F(
    ProtocolHandlerImplTest,
    StartSessionAck_SecondaryTransportParams_ServiceAllowedOnBothTransports) {
  std::vector<std::string> secondary_transports_for_usb;
  secondary_transports_for_usb.push_back("WiFi");
  std::vector<std::string> secondary_transports_for_bluetooth;
  secondary_transports_for_bluetooth.push_back("USB");
  std::vector<std::string> secondary_transports_for_wifi;  // empty
  // config allows video service to run on Wi-Fi transports only, and audio
  // service to run on all transports
  std::vector<std::string> audio_service_transports;
  audio_service_transports.push_back("IAP_BLUETOOTH");
  audio_service_transports.push_back("IAP_USB");
  audio_service_transports.push_back("IAP_USB_HOST_MODE");
  audio_service_transports.push_back("IAP_USB_DEVICE_MODE");
  audio_service_transports.push_back("IAP_CARPLAY");
  audio_service_transports.push_back("SPP_BLUETOOTH");
  audio_service_transports.push_back("AOA_USB");
  audio_service_transports.push_back("TCP_WIFI");
  std::vector<std::string> video_service_transports;
  video_service_transports.push_back("IAP_CARPLAY");
  video_service_transports.push_back("TCP_WIFI");

  // assume the device is Android and is connected through AOA
  std::string connection_type_string("AOA_USB");

  // Core should offer Wi-Fi for secondary transport. It should allow audio
  // service to run on both primary and secondary, while video service to run
  // on secondary only. Since the list specifies AOA_USB then TCP_WIFI, the
  // priority is primary > secondary.
  std::vector<std::string> expected_transport_strings;
  expected_transport_strings.push_back("TCP_WIFI");
  std::vector<int32_t> expected_audio_service_transports;
  expected_audio_service_transports.push_back(1);  // primary preferred
  expected_audio_service_transports.push_back(2);
  std::vector<int32_t> expected_video_service_transports;
  expected_video_service_transports.push_back(2);

  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(Return(false));

  VerifySecondaryTransportParamsInStartSessionAck(
      true,
      secondary_transports_for_usb,
      secondary_transports_for_bluetooth,
      secondary_transports_for_wifi,
      audio_service_transports,
      video_service_transports,
      connection_type_string,
      expected_transport_strings,
      expected_audio_service_transports,
      expected_video_service_transports);
}

TEST_F(ProtocolHandlerImplTest,
       StartSessionAck_SecondaryTransportParams_SecondaryDisabled) {
  std::vector<std::string> secondary_transports_for_usb;        // empty
  std::vector<std::string> secondary_transports_for_bluetooth;  // empty
  std::vector<std::string> secondary_transports_for_wifi;       // empty
  // config allows video and audio services to run on all transports
  std::vector<std::string> audio_service_transports;
  audio_service_transports.push_back("IAP_BLUETOOTH");
  audio_service_transports.push_back("IAP_USB");
  audio_service_transports.push_back("IAP_USB_HOST_MODE");
  audio_service_transports.push_back("IAP_USB_DEVICE_MODE");
  audio_service_transports.push_back("IAP_CARPLAY");
  audio_service_transports.push_back("SPP_BLUETOOTH");
  audio_service_transports.push_back("AOA_USB");
  audio_service_transports.push_back("TCP_WIFI");
  std::vector<std::string> video_service_transports;
  video_service_transports.push_back("IAP_BLUETOOTH");
  video_service_transports.push_back("IAP_USB");
  video_service_transports.push_back("IAP_USB_HOST_MODE");
  video_service_transports.push_back("IAP_USB_DEVICE_MODE");
  video_service_transports.push_back("IAP_CARPLAY");
  video_service_transports.push_back("SPP_BLUETOOTH");
  video_service_transports.push_back("AOA_USB");
  video_service_transports.push_back("TCP_WIFI");

  // assume the device is iOS and is connected through iAP over Bluetooth
  std::string connection_type_string("IAP_BLUETOOTH");

  // Core should not offer any secondary transport. It should still send
  // the video/audio service transport lists.
  std::vector<std::string> expected_transport_strings;  // empty
  std::vector<int32_t> expected_audio_service_transports;
  expected_audio_service_transports.push_back(1);
  std::vector<int32_t> expected_video_service_transports;
  expected_video_service_transports.push_back(1);

  connection_handler::SessionTransports dummy_st = {0, 0};
  EXPECT_CALL(connection_handler_mock,
              SetSecondaryTransportID(_, kDisabledSecondary))
      .WillOnce(Return(dummy_st));

  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(Return(false));

  VerifySecondaryTransportParamsInStartSessionAck(
      false, /* disabled */
      secondary_transports_for_usb,
      secondary_transports_for_bluetooth,
      secondary_transports_for_wifi,
      audio_service_transports,
      video_service_transports,
      connection_type_string,
      expected_transport_strings,
      expected_audio_service_transports,
      expected_video_service_transports);
}

TEST_F(ProtocolHandlerImplTest,
       StartSessionAck_SecondaryTransportParams_ServicesMapEmpty) {
  std::vector<std::string> secondary_transports_for_usb;  // empty
  std::vector<std::string> secondary_transports_for_bluetooth;
  secondary_transports_for_bluetooth.push_back("USB");
  std::vector<std::string> secondary_transports_for_wifi;
  secondary_transports_for_wifi.push_back("USB");
  // config does not specify video and audio services
  std::vector<std::string> audio_service_transports;  // empty
  std::vector<std::string> video_service_transports;  // empty

  // assume the device is connected through Wi-Fi (so not sure if it's iOS or
  // Android)
  std::string connection_type_string("TCP_WIFI");

  // Core should offer USB transport for secondary transport. (Since the OS type
  // is unknown, it will offer both IAP_USB and AOA_USB.) Also, it should allow
  // video/audio services on all transports.
  std::vector<std::string> expected_transport_strings;
  expected_transport_strings.push_back("IAP_USB");
  expected_transport_strings.push_back("AOA_USB");
  std::vector<int32_t> expected_audio_service_transports;
  expected_audio_service_transports.push_back(1);
  expected_audio_service_transports.push_back(2);
  std::vector<int32_t> expected_video_service_transports;
  expected_video_service_transports.push_back(1);
  expected_video_service_transports.push_back(2);

  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(Return(false));

  VerifySecondaryTransportParamsInStartSessionAck(
      true,
      secondary_transports_for_usb,
      secondary_transports_for_bluetooth,
      secondary_transports_for_wifi,
      audio_service_transports,
      video_service_transports,
      connection_type_string,
      expected_transport_strings,
      expected_audio_service_transports,
      expected_video_service_transports);
}

TEST_F(
    ProtocolHandlerImplTest,
    StartSessionAck_SecondaryTransportParams_SecondaryDisabled_ServicesMapEmpty) {
  std::vector<std::string> secondary_transports_for_usb;        // empty
  std::vector<std::string> secondary_transports_for_bluetooth;  // empty
  std::vector<std::string> secondary_transports_for_wifi;       // empty
  // config does not specify video and audio services
  std::vector<std::string> audio_service_transports;  // empty
  std::vector<std::string> video_service_transports;  // empty

  std::string connection_type_string("IAP_BLUETOOTH");

  // Core should not offer any secondary transport. It should still send
  // the video/audio service transport lists.
  std::vector<std::string> expected_transport_strings;  // empty
  std::vector<int32_t> expected_audio_service_transports;
  expected_audio_service_transports.push_back(1);
  std::vector<int32_t> expected_video_service_transports;
  expected_video_service_transports.push_back(1);

  connection_handler::SessionTransports dummy_st = {0, 0};
  EXPECT_CALL(connection_handler_mock,
              SetSecondaryTransportID(_, kDisabledSecondary))
      .WillOnce(Return(dummy_st));

  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(Return(false));

  VerifySecondaryTransportParamsInStartSessionAck(
      false, /* disabled */
      secondary_transports_for_usb,
      secondary_transports_for_bluetooth,
      secondary_transports_for_wifi,
      audio_service_transports,
      video_service_transports,
      connection_type_string,
      expected_transport_strings,
      expected_audio_service_transports,
      expected_video_service_transports);
}

// Secondary transport param should not be included for apps with v5.0.0
TEST_F(ProtocolHandlerImplTest,
       StartSessionAck_Unprotected_NoSecondaryTransportParamsForV5) {
  TestAsyncWaiter waiter;
  uint32_t times = 0;

  const uint8_t input_protocol_version = 5;
  const uint32_t hash_id = 123456;
  utils::SemanticVersion full_version(5, 0, 0);
  char full_version_string[] = "5.0.0";

  const size_t maximum_rpc_payload_size = 1500;
  EXPECT_CALL(protocol_handler_settings_mock, maximum_rpc_payload_size())
      .WillRepeatedly(Return(maximum_rpc_payload_size));
  InitProtocolHandlerImpl(0u, 0u);

  // configuration
  std::vector<std::string> config_secondary_transports_for_usb;  // empty
  std::vector<std::string> config_secondary_transports_for_bluetooth;
  config_secondary_transports_for_bluetooth.push_back("USB");
  std::vector<std::string> config_secondary_transports_for_wifi;
  config_secondary_transports_for_wifi.push_back("USB");

  // assume the device is iOS and is connected through iAP over Bluetooth
  std::string connection_type_string("IAP_BLUETOOTH");

  // configuration setup
  EXPECT_CALL(protocol_handler_settings_mock, max_supported_protocol_version())
      .WillRepeatedly(Return(PROTOCOL_VERSION_5));
  EXPECT_CALL(protocol_handler_settings_mock, multiple_transports_enabled())
      .WillRepeatedly(Return(true));
  EXPECT_CALL(protocol_handler_settings_mock, secondary_transports_for_usb())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_usb));
  EXPECT_CALL(protocol_handler_settings_mock,
              secondary_transports_for_bluetooth())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_bluetooth));
  EXPECT_CALL(protocol_handler_settings_mock, secondary_transports_for_wifi())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_wifi));

  EXPECT_CALL(session_observer_mock,
              TransportTypeProfileStringFromConnHandle(connection_id))
      .WillRepeatedly(Return(connection_type_string));

  // BSON params should not include any of "secondaryTransports",
  // "audioServiceTransports" and "videoServiceTransports" since v5.0.0 app
  // does not understand them
  BsonObject expected_obj;
  bson_object_initialize_default(&expected_obj);
  // mtu
  bson_object_put_int64(&expected_obj,
                        protocol_handler::strings::mtu,
                        static_cast<int64_t>(maximum_rpc_payload_size));
  // hashId
  bson_object_put_int32(&expected_obj,
                        protocol_handler::strings::hash_id,
                        static_cast<int32_t>(hash_id));
  // protocolVersion
  bson_object_put_string(&expected_obj,
                         protocol_handler::strings::protocol_version,
                         full_version_string);

  std::vector<uint8_t> expected_param =
      CreateVectorFromBsonObject(&expected_obj);

  bson_object_deinitialize(&expected_obj);

  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ControlMessage(FRAME_DATA_START_SERVICE_ACK,
                                                 PROTECTION_OFF,
                                                 connection_id,
                                                 Eq(expected_param))))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  connection_handler::SessionTransports dummy_st = {0, 0};
  EXPECT_CALL(connection_handler_mock,
              SetSecondaryTransportID(_, kDisabledSecondary))
      .WillOnce(Return(dummy_st));

  // Since the protocol version is less than 5.1.0, Core should not issue
  // TransportEventUpdate frame. Enable ProtocolVersionUsed() call and verify
  // that transport_manager_mock will NOT receive another SendMessageToDevice()
  // call.
  ON_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillByDefault(Return(true));

#ifdef ENABLE_SECURITY
  AddSecurityManager();

  EXPECT_CALL(session_observer_mock, KeyFromPair(connection_id, session_id))
      .WillOnce(Return(connection_key));

  EXPECT_CALL(session_observer_mock, GetSSLContext(connection_key, kRpc))
      .WillOnce(ReturnNull());
#endif  // ENABLE_SECURITY

  protocol_handler_impl->SendStartSessionAck(connection_id,
                                             session_id,
                                             input_protocol_version,
                                             hash_id,
                                             protocol_handler::SERVICE_TYPE_RPC,
                                             false /* protection */,
                                             full_version);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, StartSessionAck_PrimaryTransportUSBHostMode) {
  // config allows secondary transport only when connected through Bluetooth,
  // and the secondary is Wi-Fi
  std::vector<std::string> secondary_transports_for_usb;
  secondary_transports_for_usb.push_back("WiFi");
  std::vector<std::string> secondary_transports_for_bluetooth;  // empty
  std::vector<std::string> secondary_transports_for_wifi;       // empty
  // config allows video and audio services to run on all transports except
  // Bluetooth
  std::vector<std::string> audio_service_transports;
  audio_service_transports.push_back("IAP_USB_DEVICE_MODE");
  audio_service_transports.push_back("IAP_CARPLAY");
  audio_service_transports.push_back("AOA_USB");
  audio_service_transports.push_back("TCP_WIFI");
  std::vector<std::string> video_service_transports;
  video_service_transports.push_back("IAP_USB");
  video_service_transports.push_back("IAP_CARPLAY");
  video_service_transports.push_back("AOA_USB");
  video_service_transports.push_back("TCP_WIFI");

  // assume the device is IOS and is connected through USB Host Mode
  std::string connection_type_string("IAP_USB_HOST_MODE");

  // Core should specify WiFi for secondary transport, and should allow video
  // services on both transports, and audio only on secondary transport
  std::vector<std::string> expected_transport_strings;
  expected_transport_strings.push_back("TCP_WIFI");
  std::vector<int32_t> expected_audio_service_transports;
  expected_audio_service_transports.push_back(2);
  std::vector<int32_t> expected_video_service_transports;
  expected_video_service_transports.push_back(1);
  expected_video_service_transports.push_back(2);

  // A TransportUpdateEvent is also issued after Start Service ACK. We don't
  // check it in this test case.
  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(Return(false));

  VerifySecondaryTransportParamsInStartSessionAck(
      true,
      secondary_transports_for_usb,
      secondary_transports_for_bluetooth,
      secondary_transports_for_wifi,
      audio_service_transports,
      video_service_transports,
      connection_type_string,
      expected_transport_strings,
      expected_audio_service_transports,
      expected_video_service_transports);
}

TEST_F(ProtocolHandlerImplTest,
       TransportEventUpdate_afterVersionNegotiation_TCPEnabled) {
  TestAsyncWaiter waiter;
  uint32_t times = 0;

  const uint8_t input_protocol_version = 5;
  const uint32_t hash_id = 123456;
  utils::SemanticVersion full_version(5, 1, 0);

  const size_t maximum_rpc_payload_size = 1500;
  EXPECT_CALL(protocol_handler_settings_mock, maximum_rpc_payload_size())
      .WillRepeatedly(Return(maximum_rpc_payload_size));
  InitProtocolHandlerImpl(0u, 0u);

  // TCP configuration setup
  bool tcp_enabled = true;
  char tcp_address[] = "192.168.1.1";
  int32_t tcp_port = 12345;
  std::string tcp_port_str = "12345";
  protocol_handler_impl->set_tcp_config(
      tcp_enabled, std::string(tcp_address), tcp_port_str);

  // configuration setup
  std::vector<std::string> config_secondary_transports_for_usb;  // empty
  std::vector<std::string> config_secondary_transports_for_bluetooth;
  config_secondary_transports_for_bluetooth.push_back("WiFi");
  std::vector<std::string> config_secondary_transports_for_wifi;  // empty
  std::vector<std::string> config_audio_service_transports;
  config_audio_service_transports.push_back("IAP_USB");
  config_audio_service_transports.push_back("IAP_USB_HOST_MODE");
  config_audio_service_transports.push_back("IAP_USB_DEVICE_MODE");
  config_audio_service_transports.push_back("IAP_CARPLAY");
  config_audio_service_transports.push_back("AOA_USB");
  config_audio_service_transports.push_back("TCP_WIFI");
  std::vector<std::string> config_video_service_transports;
  config_video_service_transports.push_back("IAP_USB");
  config_video_service_transports.push_back("IAP_USB_HOST_MODE");
  config_video_service_transports.push_back("IAP_USB_DEVICE_MODE");
  config_video_service_transports.push_back("IAP_CARPLAY");
  config_video_service_transports.push_back("AOA_USB");
  config_video_service_transports.push_back("TCP_WIFI");

  EXPECT_CALL(protocol_handler_settings_mock, max_supported_protocol_version())
      .WillRepeatedly(Return(PROTOCOL_VERSION_5));
  EXPECT_CALL(protocol_handler_settings_mock, multiple_transports_enabled())
      .WillRepeatedly(Return(true));
  EXPECT_CALL(protocol_handler_settings_mock, secondary_transports_for_usb())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_usb));
  EXPECT_CALL(protocol_handler_settings_mock,
              secondary_transports_for_bluetooth())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_bluetooth));
  EXPECT_CALL(protocol_handler_settings_mock, secondary_transports_for_wifi())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_wifi));
  EXPECT_CALL(protocol_handler_settings_mock, audio_service_transports())
      .WillOnce(ReturnRef(config_audio_service_transports));
  EXPECT_CALL(protocol_handler_settings_mock, video_service_transports())
      .WillOnce(ReturnRef(config_video_service_transports));

  // assume the device is iOS and is connected through iAP over Bluetooth
  std::string connection_type_string("IAP_BLUETOOTH");

  EXPECT_CALL(session_observer_mock,
              TransportTypeProfileStringFromConnHandle(connection_id))
      .WillRepeatedly(Return(connection_type_string));

  EXPECT_CALL(
      transport_manager_mock,
      SendMessageToDevice(ControlMessage(
          FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF, connection_id, _)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(
          DoAll(SetArgReferee<2>(PROTOCOL_VERSION_5), Return(true)));

  BsonObject expected_obj;
  bson_object_initialize_default(&expected_obj);
  // IP address
  bson_object_put_string(
      &expected_obj, protocol_handler::strings::tcp_ip_address, tcp_address);
  // TCP port number
  bson_object_put_int32(
      &expected_obj, protocol_handler::strings::tcp_port, tcp_port);

  std::vector<uint8_t> expected_param =
      CreateVectorFromBsonObject(&expected_obj);

  bson_object_deinitialize(&expected_obj);

  EXPECT_CALL(
      transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_TRANSPORT_EVENT_UPDATE,
                                         PROTECTION_OFF,
                                         connection_id,
                                         Eq(expected_param))))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

#ifdef ENABLE_SECURITY
  AddSecurityManager();

  EXPECT_CALL(session_observer_mock, KeyFromPair(connection_id, session_id))
      .WillOnce(Return(connection_key));

  EXPECT_CALL(session_observer_mock, GetSSLContext(connection_key, kRpc))
      .WillOnce(ReturnNull());
#endif  // ENABLE_SECURITY

  protocol_handler_impl->SendStartSessionAck(connection_id,
                                             session_id,
                                             input_protocol_version,
                                             hash_id,
                                             protocol_handler::SERVICE_TYPE_RPC,
                                             false /* protection */,
                                             full_version);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest,
       TransportEventUpdate_afterVersionNegotiation_TCPDisabled) {
  TestAsyncWaiter waiter;
  uint32_t times = 0;

  const uint8_t input_protocol_version = 5;
  const uint32_t hash_id = 123456;
  utils::SemanticVersion full_version(5, 1, 0);

  const size_t maximum_rpc_payload_size = 1500;
  EXPECT_CALL(protocol_handler_settings_mock, maximum_rpc_payload_size())
      .WillRepeatedly(Return(maximum_rpc_payload_size));
  InitProtocolHandlerImpl(0u, 0u);

  // TCP configuration setup
  bool tcp_enabled = false;
  char tcp_address[] = "192.168.2.3";
  std::string tcp_port_str = "12345";
  protocol_handler_impl->set_tcp_config(
      tcp_enabled, std::string(tcp_address), tcp_port_str);

  std::vector<std::string> config_secondary_transports_for_usb;  // empty
  std::vector<std::string> config_secondary_transports_for_bluetooth;
  config_secondary_transports_for_bluetooth.push_back("WiFi");
  std::vector<std::string> config_secondary_transports_for_wifi;  // empty
  std::vector<std::string> config_audio_service_transports;
  config_audio_service_transports.push_back("IAP_USB");
  config_audio_service_transports.push_back("IAP_USB_HOST_MODE");
  config_audio_service_transports.push_back("IAP_USB_DEVICE_MODE");
  config_audio_service_transports.push_back("IAP_CARPLAY");
  config_audio_service_transports.push_back("AOA_USB");
  config_audio_service_transports.push_back("TCP_WIFI");
  std::vector<std::string> config_video_service_transports;
  config_video_service_transports.push_back("IAP_USB");
  config_video_service_transports.push_back("IAP_USB_HOST_MODE");
  config_video_service_transports.push_back("IAP_USB_DEVICE_MODE");
  config_video_service_transports.push_back("IAP_CARPLAY");
  config_video_service_transports.push_back("AOA_USB");
  config_video_service_transports.push_back("TCP_WIFI");

  EXPECT_CALL(protocol_handler_settings_mock, max_supported_protocol_version())
      .WillRepeatedly(Return(PROTOCOL_VERSION_5));
  EXPECT_CALL(protocol_handler_settings_mock, multiple_transports_enabled())
      .WillRepeatedly(Return(true));
  EXPECT_CALL(protocol_handler_settings_mock, secondary_transports_for_usb())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_usb));
  EXPECT_CALL(protocol_handler_settings_mock,
              secondary_transports_for_bluetooth())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_bluetooth));
  EXPECT_CALL(protocol_handler_settings_mock, secondary_transports_for_wifi())
      .Times(AtLeast(0))
      .WillRepeatedly(ReturnRef(config_secondary_transports_for_wifi));
  EXPECT_CALL(protocol_handler_settings_mock, audio_service_transports())
      .WillOnce(ReturnRef(config_audio_service_transports));
  EXPECT_CALL(protocol_handler_settings_mock, video_service_transports())
      .WillOnce(ReturnRef(config_video_service_transports));

  // assume the device is iOS and is connected through iAP over Bluetooth
  std::string connection_type_string("IAP_BLUETOOTH");

  EXPECT_CALL(session_observer_mock,
              TransportTypeProfileStringFromConnHandle(connection_id))
      .WillRepeatedly(Return(connection_type_string));

  EXPECT_CALL(
      transport_manager_mock,
      SendMessageToDevice(ControlMessage(
          FRAME_DATA_START_SERVICE_ACK, PROTECTION_OFF, connection_id, _)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(
          DoAll(SetArgReferee<2>(PROTOCOL_VERSION_5), Return(true)));

  BsonObject expected_obj;
  bson_object_initialize_default(&expected_obj);
  // IP address
  char empty_ip_address[] = "";
  bson_object_put_string(&expected_obj,
                         protocol_handler::strings::tcp_ip_address,
                         empty_ip_address);
  // TCP port number should be omitted

  std::vector<uint8_t> expected_param =
      CreateVectorFromBsonObject(&expected_obj);

  bson_object_deinitialize(&expected_obj);

  EXPECT_CALL(
      transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_TRANSPORT_EVENT_UPDATE,
                                         PROTECTION_OFF,
                                         connection_id,
                                         Eq(expected_param))))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

#ifdef ENABLE_SECURITY
  AddSecurityManager();

  EXPECT_CALL(session_observer_mock, KeyFromPair(connection_id, session_id))
      .WillOnce(Return(connection_key));

  EXPECT_CALL(session_observer_mock, GetSSLContext(connection_key, kRpc))
      .WillOnce(ReturnNull());
#endif  // ENABLE_SECURITY

  protocol_handler_impl->SendStartSessionAck(connection_id,
                                             session_id,
                                             input_protocol_version,
                                             hash_id,
                                             protocol_handler::SERVICE_TYPE_RPC,
                                             false /* protection */,
                                             full_version);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest,
       OnTransportConfigUpdated_TransportEventUpdate_TCPEnabled) {
  using connection_handler::SessionConnectionMap;
  using connection_handler::SessionTransports;

  TestAsyncWaiter waiter;
  uint32_t times = 0;

  char tcp_address[] = "172.16.2.3";
  int32_t tcp_port = 23456;
  std::string tcp_port_str = "23456";

  transport_manager::transport_adapter::TransportConfig configs;
  configs[transport_manager::transport_adapter::tc_enabled] =
      std::string("true");
  configs[transport_manager::transport_adapter::tc_tcp_port] = tcp_port_str;
  configs[transport_manager::transport_adapter::tc_tcp_ip_address] =
      std::string(tcp_address);

  transport_manager::ConnectionUID device1_primary_connection_id = 100;
  transport_manager::ConnectionUID device2_primary_connection_id = 101;
  transport_manager::ConnectionUID device2_secondary_connection_id = 150;

  SessionTransports st1 = {device1_primary_connection_id, kDisabledSecondary};
  SessionTransports st2 = {device2_primary_connection_id,
                           device2_secondary_connection_id};
  session_connection_map_[0x11] = st1;
  session_connection_map_[0x22] = st2;

  EXPECT_CALL(connection_handler_mock, session_connection_map())
      .WillOnce(Return(DataAccessor<SessionConnectionMap>(
          session_connection_map_, session_connection_map_lock_ptr_)));

  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(
          DoAll(SetArgReferee<2>(PROTOCOL_VERSION_5), Return(true)));

  BsonObject expected_obj;
  bson_object_initialize_default(&expected_obj);
  // IP address
  bson_object_put_string(
      &expected_obj, protocol_handler::strings::tcp_ip_address, tcp_address);
  // TCP port number
  bson_object_put_int32(
      &expected_obj, protocol_handler::strings::tcp_port, tcp_port);

  std::vector<uint8_t> expected_param =
      CreateVectorFromBsonObject(&expected_obj);

  bson_object_deinitialize(&expected_obj);

  // since device 1 doesn't support secondary transport feature,
  // TransportEvetUpdate should be delivered only to device 2
  EXPECT_CALL(
      transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_TRANSPORT_EVENT_UPDATE,
                                         PROTECTION_OFF,
                                         device2_primary_connection_id,
                                         Eq(expected_param))))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  tm_listener->OnTransportConfigUpdated(configs);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest,
       OnTransportConfigUpdated_TransportEventUpdate_TCPDisabled) {
  using connection_handler::SessionConnectionMap;
  using connection_handler::SessionTransports;

  TestAsyncWaiter waiter;
  uint32_t times = 0;

  char tcp_address[] = "172.16.2.3";
  std::string tcp_port_str = "23456";

  transport_manager::transport_adapter::TransportConfig configs;
  configs[transport_manager::transport_adapter::tc_enabled] =
      std::string("false");
  configs[transport_manager::transport_adapter::tc_tcp_port] = tcp_port_str;
  configs[transport_manager::transport_adapter::tc_tcp_ip_address] =
      std::string(tcp_address);

  transport_manager::ConnectionUID device1_primary_connection_id = 100;
  transport_manager::ConnectionUID device1_secondary_connection_id = 150;
  transport_manager::ConnectionUID device2_primary_connection_id = 101;
  transport_manager::ConnectionUID device3_primary_connection_id = 102;
  transport_manager::ConnectionUID device3_secondary_connection_id = 151;

  SessionTransports st1 = {device1_primary_connection_id,
                           device1_secondary_connection_id};
  SessionTransports st2 = {device2_primary_connection_id, kDisabledSecondary};
  SessionTransports st3 = {device3_primary_connection_id,
                           device3_secondary_connection_id};
  session_connection_map_[0x11] = st1;
  session_connection_map_[0x22] = st2;
  session_connection_map_[0x33] = st3;

  EXPECT_CALL(connection_handler_mock, session_connection_map())
      .WillOnce(Return(DataAccessor<SessionConnectionMap>(
          session_connection_map_, session_connection_map_lock_ptr_)));

  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(_, _, _))
      .WillRepeatedly(
          DoAll(SetArgReferee<2>(PROTOCOL_VERSION_5), Return(true)));

  BsonObject expected_obj;
  bson_object_initialize_default(&expected_obj);
  // IP address
  char empty_ip_address[] = "";
  bson_object_put_string(&expected_obj,
                         protocol_handler::strings::tcp_ip_address,
                         empty_ip_address);
  // TCP port number should be omitted

  std::vector<uint8_t> expected_param =
      CreateVectorFromBsonObject(&expected_obj);

  bson_object_deinitialize(&expected_obj);

  // both device 1 and device 3 should receive TransportEventUpdate frames
  EXPECT_CALL(
      transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_TRANSPORT_EVENT_UPDATE,
                                         PROTECTION_OFF,
                                         device1_primary_connection_id,
                                         Eq(expected_param))))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;
  EXPECT_CALL(
      transport_manager_mock,
      SendMessageToDevice(ControlMessage(FRAME_DATA_TRANSPORT_EVENT_UPDATE,
                                         PROTECTION_OFF,
                                         device3_primary_connection_id,
                                         Eq(expected_param))))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  tm_listener->OnTransportConfigUpdated(configs);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, RegisterSecondaryTransport_SUCCESS) {
  AddConnection();

  TestAsyncWaiter waiter;
  uint32_t times = 0;

  transport_manager::ConnectionUID primary_connection_id = 123;

  EXPECT_CALL(session_observer_mock,
              ProtocolVersionUsed(primary_connection_id, _, _))
      .WillRepeatedly(
          DoAll(SetArgReferee<2>(PROTOCOL_VERSION_5), Return(true)));

  EXPECT_CALL(connection_handler_mock,
              OnSecondaryTransportStarted(_, connection_id, session_id))
      .WillOnce(DoAll(SetArgReferee<0>(primary_connection_id), Return(true)));

  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_REGISTER_SECONDARY_TRANSPORT_ACK,
                                 PROTECTION_OFF,
                                 connection_id,
                                 _)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  SendControlMessage(PROTECTION_OFF,
                     kControl,
                     session_id,
                     FRAME_DATA_REGISTER_SECONDARY_TRANSPORT,
                     PROTOCOL_VERSION_5);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, RegisterSecondaryTransport_FAILURE) {
  AddConnection();

  TestAsyncWaiter waiter;
  uint32_t times = 0;

  transport_manager::ConnectionUID primary_connection_id = 123;

  EXPECT_CALL(session_observer_mock,
              ProtocolVersionUsed(primary_connection_id, _, _))
      .WillRepeatedly(
          DoAll(SetArgReferee<2>(PROTOCOL_VERSION_5), Return(true)));

  // check the behavior when OnSecondaryTransportStarted() returns false
  EXPECT_CALL(connection_handler_mock,
              OnSecondaryTransportStarted(_, connection_id, session_id))
      .WillOnce(DoAll(SetArgReferee<0>(primary_connection_id), Return(false)));

  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(
                  ControlMessage(FRAME_DATA_REGISTER_SECONDARY_TRANSPORT_NACK,
                                 PROTECTION_OFF,
                                 connection_id,
                                 _)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter), Return(E_SUCCESS)));
  times++;

  SendControlMessage(PROTECTION_OFF,
                     kControl,
                     session_id,
                     FRAME_DATA_REGISTER_SECONDARY_TRANSPORT,
                     PROTOCOL_VERSION_5);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, DISABLED_FloodVerification) {
  const size_t period_msec = 10000;
  const size_t max_messages = 1000;
  InitProtocolHandlerImpl(period_msec, max_messages);
  AddConnection();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect flood notification to CH
  EXPECT_CALL(session_observer_mock, OnApplicationFloodCallBack(connection_key))
      .WillOnce(NotifyTestAsyncWaiter(waiter));
  times++;

  ON_CALL(protocol_handler_settings_mock, message_frequency_time())
      .WillByDefault(Return(period_msec));
  ON_CALL(protocol_handler_settings_mock, message_frequency_count())
      .WillByDefault(Return(max_messages));

  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id,
                  PROTOCOL_VERSION_3,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
  }

  EXPECT_TRUE(waiter->WaitFor(times, period_msec));
}

TEST_F(ProtocolHandlerImplTest, DISABLED_FloodVerification_ThresholdValue) {
  const size_t period_msec = 10000;
  const size_t max_messages = 1000;
  InitProtocolHandlerImpl(period_msec, max_messages);
  AddConnection();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  ON_CALL(protocol_handler_settings_mock, message_frequency_time())
      .WillByDefault(Return(period_msec));
  ON_CALL(protocol_handler_settings_mock, message_frequency_count())
      .WillByDefault(Return(max_messages));

  // Expect NO flood notification to CH
  EXPECT_CALL(session_observer_mock, OnApplicationFloodCallBack(connection_key))
      .Times(0);

  for (size_t i = 0; i < max_messages - 1; ++i) {
    SendTMMessage(connection_id,
                  PROTOCOL_VERSION_3,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
  }

  EXPECT_TRUE(waiter->WaitFor(times, period_msec));
}

TEST_F(ProtocolHandlerImplTest, DISABLED_FloodVerification_VideoFrameSkip) {
  const size_t period_msec = 10000;
  const size_t max_messages = 1000;
  InitProtocolHandlerImpl(period_msec, max_messages);
  AddConnection();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect NO flood notification to CH on video data streaming
  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id,
                  PROTOCOL_VERSION_3,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kMobileNav,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
  }

  EXPECT_TRUE(waiter->WaitFor(times, period_msec));
}

TEST_F(ProtocolHandlerImplTest, DISABLED_FloodVerification_AudioFrameSkip) {
  const size_t period_msec = 10000;
  const size_t max_messages = 1000;
  InitProtocolHandlerImpl(period_msec, max_messages);
  AddConnection();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect NO flood notification to CH on video data streaming
  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id,
                  PROTOCOL_VERSION_3,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kAudio,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
  }

  EXPECT_TRUE(waiter->WaitFor(times, period_msec));
}

TEST_F(ProtocolHandlerImplTest, DISABLED_FloodVerificationDisable) {
  const size_t period_msec = 0;
  const size_t max_messages = 0;
  InitProtocolHandlerImpl(period_msec, max_messages);
  AddConnection();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect NO flood notification to session observer
  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id,
                  PROTOCOL_VERSION_3,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
  }

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, MalformedVerificationDisable) {
  const size_t period_msec = 10000;
  const size_t max_messages = 100;
  InitProtocolHandlerImpl(0u, 0u, false, period_msec, max_messages);
  AddConnection();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect malformed notification to CH
  EXPECT_CALL(session_observer_mock, OnMalformedMessageCallback(connection_id))
      .Times(max_messages);

  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  for (size_t i = 0; i < max_messages; ++i) {
    SendTMMessage(connection_id,
                  malformed_version,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
  }

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, DISABLED_MalformedLimitVerification) {
  const size_t period_msec = 10000;
  const size_t max_messages = 100;
  InitProtocolHandlerImpl(0u, 0u, true, period_msec, max_messages);
  AddConnection();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect malformed notification to CH
  EXPECT_CALL(session_observer_mock, OnMalformedMessageCallback(connection_id))
      .WillOnce(NotifyTestAsyncWaiter(waiter));
  times++;

  // Sending malformed packets
  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  for (size_t i = 0; i < max_messages * 2; ++i) {
    // Malformed message
    SendTMMessage(connection_id,
                  malformed_version,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
    // Common message
    SendTMMessage(connection_id,
                  PROTOCOL_VERSION_1,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
  }

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest,
       DISABLED_MalformedLimitVerification_MalformedStock) {
  const size_t period_msec = 10000;
  const size_t max_messages = 100;
  InitProtocolHandlerImpl(0u, 0u, true, period_msec, max_messages);
  AddConnection();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect malformed notification to CH
  EXPECT_CALL(session_observer_mock, OnMalformedMessageCallback(connection_id))
      .WillOnce(NotifyTestAsyncWaiter(waiter));
  times++;

  // Sending malformed packets
  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  const uint8_t malformed_frame_type = FRAME_TYPE_MAX_VALUE;
  const uint8_t malformed_service_type = kInvalidServiceType;
  for (size_t i = 0; i < max_messages * 2; ++i) {
    // Malformed message 1
    SendTMMessage(connection_id,
                  malformed_version,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
    // Malformed message 2
    SendTMMessage(connection_id,
                  PROTOCOL_VERSION_1,
                  PROTECTION_OFF,
                  malformed_frame_type,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
    // Malformed message 3
    SendTMMessage(connection_id,
                  PROTOCOL_VERSION_1,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  malformed_service_type,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);

    // Common message
    SendTMMessage(connection_id,
                  PROTOCOL_VERSION_1,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
  }

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, MalformedLimitVerification_MalformedOnly) {
  const size_t period_msec = 10000;
  const size_t max_messages = 100;
  InitProtocolHandlerImpl(0u, 0u, true, period_msec, max_messages);
  AddConnection();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect NO malformed notification to CH
  EXPECT_CALL(session_observer_mock, OnMalformedMessageCallback(connection_id))
      .Times(0);

  // Sending malformed packets
  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  const uint8_t malformed_frame_type = FRAME_TYPE_MAX_VALUE;
  const uint8_t malformed_service_type = kInvalidServiceType;
  for (size_t i = 0; i < max_messages * 2; ++i) {
    // Malformed message 1
    SendTMMessage(connection_id,
                  malformed_version,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
    // Malformed message 2
    SendTMMessage(connection_id,
                  PROTOCOL_VERSION_1,
                  PROTECTION_OFF,
                  malformed_frame_type,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
    // Malformed message 3
    SendTMMessage(connection_id,
                  PROTOCOL_VERSION_1,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  malformed_service_type,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);

    // No common message
  }

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, MalformedLimitVerification_NullTimePeriod) {
  const size_t period_msec = 0;
  const size_t max_messages = 1000;
  InitProtocolHandlerImpl(0u, 0u, true, period_msec, max_messages);
  AddConnection();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect no malformed notification to CH
  EXPECT_CALL(session_observer_mock, OnMalformedMessageCallback(connection_id))
      .Times(0);

  // Sending malformed packets
  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id,
                  malformed_version,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
  }

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, MalformedLimitVerification_NullCount) {
  const size_t period_msec = 10000;
  const size_t max_messages = 0;
  InitProtocolHandlerImpl(0u, 0u, true, period_msec, max_messages);
  AddConnection();

  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect no malformed notification to CH
  EXPECT_CALL(session_observer_mock, OnMalformedMessageCallback(connection_id))
      .Times(0);

  // Sending malformed packets
  const uint8_t malformed_version = PROTOCOL_VERSION_MAX;
  for (size_t i = 0; i < max_messages + 1; ++i) {
    SendTMMessage(connection_id,
                  malformed_version,
                  PROTECTION_OFF,
                  FRAME_TYPE_SINGLE,
                  kControl,
                  FRAME_DATA_SINGLE,
                  session_id,
                  some_data.size(),
                  message_id,
                  &some_data[0]);
  }

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest,
       SendEndServicePrivate_NoConnection_MessageNotSent) {
  // Expect check connection with ProtocolVersionUsed
  EXPECT_CALL(session_observer_mock,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(false));
  // Expect not send End Service
  EXPECT_CALL(transport_manager_mock, SendMessageToDevice(_)).Times(0);

  // Act
  protocol_handler_impl->SendEndSession(connection_id, session_id);
}

TEST_F(ProtocolHandlerImplTest,
       DISABLED_SendEndServicePrivate_EndSession_MessageSent) {
  // Arrange
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect check connection with ProtocolVersionUsed
  EXPECT_CALL(session_observer_mock,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(true));
  // Expect send End Service
  EXPECT_CALL(
      transport_manager_mock,
      SendMessageToDevice(ExpectedMessage(
          FRAME_TYPE_CONTROL, FRAME_DATA_END_SERVICE, PROTECTION_OFF, kRpc)))
      .WillOnce(Return(E_SUCCESS));
  // Act
  protocol_handler_impl->SendEndSession(connection_id, session_id);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest,
       SendEndServicePrivate_ServiceTypeControl_MessageSent) {
  // Arrange
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect check connection with ProtocolVersionUsed
  EXPECT_CALL(session_observer_mock,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(true));
  // Expect send End Service
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ExpectedMessage(FRAME_TYPE_CONTROL,
                                                  FRAME_DATA_END_SERVICE,
                                                  PROTECTION_OFF,
                                                  kControl)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
  times++;

  // Act
  protocol_handler_impl->SendEndService(
      connection_id, connection_id, session_id, kControl);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, SendHeartBeat_NoConnection_NotSent) {
  // Expect check connection with ProtocolVersionUsed
  EXPECT_CALL(session_observer_mock,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(false));
  // Expect not send HeartBeat
  EXPECT_CALL(transport_manager_mock, SendMessageToDevice(_)).Times(0);

  // Act
  protocol_handler_impl->SendHeartBeat(connection_id, session_id);
}

TEST_F(ProtocolHandlerImplTest, SendHeartBeat_Successful) {
  // Arrange
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect check connection with ProtocolVersionUsed
  EXPECT_CALL(session_observer_mock,
              ProtocolVersionUsed(connection_id, session_id, _))
      .WillOnce(Return(true));
  // Expect send HeartBeat
  EXPECT_CALL(
      transport_manager_mock,
      SendMessageToDevice(ExpectedMessage(
          FRAME_TYPE_CONTROL, FRAME_DATA_HEART_BEAT, PROTECTION_OFF, kControl)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
  times++;

  // Act
  protocol_handler_impl->SendHeartBeat(connection_id, session_id);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, SendHeartBeatAck_Successful) {
  // Arrange
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect double check connection and protocol version with
  // ProtocolVersionUsed
  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(connection_id, _, _))
      .WillRepeatedly(
          DoAll(SetArgReferee<2>(PROTOCOL_VERSION_3), Return(true)));
  // Expect send HeartBeatAck
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ExpectedMessage(FRAME_TYPE_CONTROL,
                                                  FRAME_DATA_HEART_BEAT_ACK,
                                                  PROTECTION_OFF,
                                                  kControl)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
  times++;

  // Act
  SendControlMessage(
      PROTECTION_OFF, kControl, session_id, FRAME_DATA_HEART_BEAT);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest,
       DISABLED_SendHeartBeatAck_WrongProtocolVersion_NotSent) {
  // Arrange
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  // Expect two checks of connection and protocol version with
  // ProtocolVersionUsed
  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(connection_id, _, _))
      .Times(2)
      .WillRepeatedly(
          DoAll(SetArgReferee<2>(PROTOCOL_VERSION_1), Return(true)));
  // Expect not send HeartBeatAck
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ExpectedMessage(FRAME_TYPE_CONTROL,
                                                  FRAME_DATA_HEART_BEAT_ACK,
                                                  PROTECTION_OFF,
                                                  kControl))).Times(0);
  // Act
  SendControlMessage(
      PROTECTION_OFF, kControl, session_id, FRAME_DATA_HEART_BEAT);
  SendControlMessage(
      PROTECTION_OFF, kControl, session_id, FRAME_DATA_HEART_BEAT);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest,
       SendMessageToMobileApp_SendSingleControlMessage) {
  // Arrange
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  const bool is_final = true;
  const uint32_t total_data_size = 1;
  UCharDataVector data(total_data_size);
  RawMessagePtr message = std::make_shared<RawMessage>(
      connection_key, PROTOCOL_VERSION_3, &data[0], total_data_size, kControl);
  // Expect getting pair from key from session observer
  EXPECT_CALL(session_observer_mock,
              PairFromKey(message->connection_key(), _, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(connection_id), SetArgPointee<2>(session_id)));
#ifdef ENABLE_SECURITY
  // Expect getting ssl context
  EXPECT_CALL(session_observer_mock,
              GetSSLContext(message->connection_key(), message->service_type()))
      .WillOnce(Return(&ssl_context_mock));
#endif  // ENABLE_SECURITY
  // Expect send message to mobile
  EXPECT_CALL(
      transport_manager_mock,
      SendMessageToDevice(ExpectedMessage(
          FRAME_TYPE_SINGLE, FRAME_DATA_SINGLE, PROTECTION_OFF, kControl)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
  times++;

  // Act
  protocol_handler_impl->SendMessageToMobileApp(message, is_final);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest,
       SendMessageToMobileApp_SendSingleNonControlMessage) {
  // Arrange
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  const bool is_final = true;
  const uint32_t total_data_size = 1;
  UCharDataVector data(total_data_size);
  RawMessagePtr message = std::make_shared<RawMessage>(
      connection_key, PROTOCOL_VERSION_3, &data[0], total_data_size, kRpc);
  // Expect getting pair from key from session observer
  EXPECT_CALL(session_observer_mock,
              PairFromKey(message->connection_key(), _, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(connection_id), SetArgPointee<2>(session_id)));

#ifdef ENABLE_SECURITY
  // Expect getting ssl context
  EXPECT_CALL(session_observer_mock,
              GetSSLContext(message->connection_key(), message->service_type()))
      .Times(2)
      .WillRepeatedly(
          DoAll(NotifyTestAsyncWaiter(waiter), Return(&ssl_context_mock)));
  times += 2;
  AddSecurityManager();
#endif  // ENABLE_SECURITY

  // Expect send message to mobile
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ExpectedMessage(
                  FRAME_TYPE_SINGLE, FRAME_DATA_SINGLE, PROTECTION_OFF, kRpc)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
  times++;

  // Act
  protocol_handler_impl->SendMessageToMobileApp(message, is_final);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, SendMessageToMobileApp_SendMultiframeMessage) {
  // Arrange
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  const bool is_final = true;
  const uint32_t total_data_size = MAXIMUM_FRAME_DATA_V2_SIZE * 2;
  UCharDataVector data(total_data_size);
  const uint8_t first_consecutive_frame = 0x01;
  RawMessagePtr message = std::make_shared<RawMessage>(
      connection_key, PROTOCOL_VERSION_3, &data[0], total_data_size, kBulk);
  // Expect getting pair from key from session observer
  EXPECT_CALL(session_observer_mock,
              PairFromKey(message->connection_key(), _, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(connection_id), SetArgPointee<2>(session_id)));
#ifdef ENABLE_SECURITY
  // Expect getting ssl context
  EXPECT_CALL(session_observer_mock,
              GetSSLContext(message->connection_key(), message->service_type()))
      .Times(4)
      .WillRepeatedly(
          DoAll(NotifyTestAsyncWaiter(waiter), Return(&ssl_context_mock)));
  times += 4;
  AddSecurityManager();
#endif  // ENABLE_SECURITY
  // Expect sending message frame by frame to mobile
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ExpectedMessage(
                  FRAME_TYPE_FIRST, FRAME_DATA_FIRST, PROTECTION_OFF, kBulk)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
  times++;
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ExpectedMessage(FRAME_TYPE_CONSECUTIVE,
                                                  first_consecutive_frame,
                                                  PROTECTION_OFF,
                                                  kBulk)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
  times++;
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ExpectedMessage(FRAME_TYPE_CONSECUTIVE,
                                                  FRAME_DATA_LAST_CONSECUTIVE,
                                                  PROTECTION_OFF,
                                                  kBulk)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
  times++;

  // Act
  protocol_handler_impl->SendMessageToMobileApp(message, is_final);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, SendServiceDataAck_PreVersion5) {
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  EXPECT_CALL(session_observer_mock, PairFromKey(connection_key, _, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(connection_id), SetArgPointee<2>(session_id)));
  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(connection_id, _, _))
      .WillRepeatedly(
          DoAll(SetArgReferee<2>(PROTOCOL_VERSION_4), Return(true)));

  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ExpectedMessage(FRAME_TYPE_CONTROL,
                                                  FRAME_DATA_SERVICE_DATA_ACK,
                                                  PROTECTION_OFF,
                                                  kMobileNav)))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(E_SUCCESS)));
  times++;

  protocol_handler_impl->SendFramesNumber(connection_key, 0);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ProtocolHandlerImplTest, SendServiceDataAck_AfterVersion5) {
  std::shared_ptr<TestAsyncWaiter> waiter = std::make_shared<TestAsyncWaiter>();
  uint32_t times = 0;

  AddSession(waiter, times);

  EXPECT_CALL(session_observer_mock, PairFromKey(connection_key, _, _))
      .WillOnce(
          DoAll(SetArgPointee<1>(connection_id), SetArgPointee<2>(session_id)));
  EXPECT_CALL(session_observer_mock, ProtocolVersionUsed(connection_id, _, _))
      .WillRepeatedly(
          DoAll(SetArgReferee<2>(PROTOCOL_VERSION_5), Return(true)));

  // It is expected that Service Data ACK is NOT sent for version 5+
  EXPECT_CALL(transport_manager_mock,
              SendMessageToDevice(ExpectedMessage(FRAME_TYPE_CONTROL,
                                                  FRAME_DATA_SERVICE_DATA_ACK,
                                                  PROTECTION_OFF,
                                                  kMobileNav))).Times(0);

  protocol_handler_impl->SendFramesNumber(connection_key, 0);

  EXPECT_TRUE(waiter->WaitFor(times, kAsyncExpectationsTimeout));
}

}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test
