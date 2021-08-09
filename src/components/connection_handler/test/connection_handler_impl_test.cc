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

#include "connection_handler/connection_handler_impl.h"
#include <stdint.h>
#include <fstream>
#include <vector>
#include "gtest/gtest.h"
#include "protocol/common.h"
// TODO(EZamakhov): move security test
#ifdef ENABLE_SECURITY
#include "security_manager/mock_security_manager.h"
#include "security_manager/mock_ssl_context.h"
#endif  // ENABLE_SECURITY
#include "connection_handler/mock_connection_handler_observer.h"
#include "connection_handler/mock_connection_handler_settings.h"
#include "encryption/hashing.h"
#include "protocol_handler/mock_protocol_handler.h"
#include "transport_manager/mock_transport_manager.h"
#include "utils/test_async_waiter.h"

namespace test {
namespace components {
namespace connection_handler_test {

using namespace ::connection_handler;
using ::protocol_handler::ServiceType;
using namespace ::protocol_handler;
using ::testing::_;
using ::testing::ByRef;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRefOfCopy;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;

// custom action to call a member function with 4 arguments
ACTION_P6(InvokeMemberFuncWithArg4, ptr, memberFunc, a, b, c, d) {
  (ptr->*memberFunc)(a, b, c, d);
}

namespace {
const uint32_t kAsyncExpectationsTimeout = 10000u;
}

// For service types and PROTECTION_ON/OFF

enum UnnamedService { kServedService1 = 0x06, kServedService2 = 0x08 };

class ConnectionHandlerTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    device_handle_ = 0;

    connection_type_ = "BTMAC";
    device_name_ = "test_name";
    mac_address_ = "test_address";

    connection_handler_ = new ConnectionHandlerImpl(
        mock_connection_handler_settings_, mock_transport_manager_);
    uid_ = 1u;
    connection_key_ = connection_handler_->KeyFromPair(0, 0u);
    protected_services_.clear();
    unprotected_services_.clear();
    SetSpecificServices();
  }
  void TearDown() OVERRIDE {
    delete connection_handler_;
  }

  void SetSpecificServices() {
#ifdef ENABLE_SECURITY
    ON_CALL(mock_connection_handler_settings_, force_protected_service())
        .WillByDefault(ReturnRefOfCopy(protected_services_));
    ON_CALL(mock_connection_handler_settings_, force_unprotected_service())
        .WillByDefault(ReturnRefOfCopy(unprotected_services_));
#endif  // ENABLE_SECURITY
  }

  // Additional SetUp
  void AddTestDeviceConnection() {
    const transport_manager::DeviceInfo device_info(
        device_handle_, mac_address_, device_name_, connection_type_);
    // Add Device and connection
    ON_CALL(mock_connection_handler_settings_, heart_beat_timeout())
        .WillByDefault(Return(1000u));
    connection_handler_->addDeviceConnection(device_info, uid_);
    connection_key_ = connection_handler_->KeyFromPair(uid_, 0u);
    // Remove all specific services
  }

  /**
   * @brief Adds test session
   * @return session_id
   */
  void AddTestSession() {
    protocol_handler_test::MockProtocolHandler temp_protocol_handler;
    connection_handler_->set_protocol_handler(&temp_protocol_handler);
    EXPECT_CALL(temp_protocol_handler, NotifySessionStarted(_, _, _))
        .WillOnce(SaveArg<0>(&out_context_));

    connection_handler_->OnSessionStartedCallback(
        uid_, 0, kRpc, PROTECTION_OFF, static_cast<BsonObject*>(NULL));
    connection_handler_->set_protocol_handler(NULL);
    EXPECT_NE(0u, out_context_.new_session_id_);
    EXPECT_EQ(SessionHash(uid_, out_context_.new_session_id_),
              out_context_.hash_id_);
    connection_key_ =
        connection_handler_->KeyFromPair(uid_, out_context_.new_session_id_);
    CheckSessionExists(uid_, out_context_.new_session_id_);
  }

  uint32_t SessionHash(const uint32_t connection, const uint32_t session) {
    return connection_handler_->KeyFromPair(connection, session);
  }

  void AddTestService(ServiceType service_type) {
    EXPECT_NE(0u, out_context_.new_session_id_);
    EXPECT_EQ(SessionHash(uid_, out_context_.new_session_id_),
              out_context_.hash_id_);
    connection_key_ =
        connection_handler_->KeyFromPair(uid_, out_context_.new_session_id_);
    CheckSessionExists(uid_, out_context_.new_session_id_);

    // Set protocol version to 3 if audio or video service should be tested
    if (service_type == ServiceType::kAudio ||
        service_type == ServiceType::kMobileNav) {
      ChangeProtocol(uid_,
                     out_context_.new_session_id_,
                     protocol_handler::PROTOCOL_VERSION_3);
    }

    SessionContext context;
    protocol_handler_test::MockProtocolHandler temp_protocol_handler;
    connection_handler_->set_protocol_handler(&temp_protocol_handler);
    EXPECT_CALL(temp_protocol_handler, NotifySessionStarted(_, _, _))
        .WillOnce(SaveArg<0>(&context));

    connection_handler_->OnSessionStartedCallback(uid_,
                                                  out_context_.new_session_id_,
                                                  service_type,
                                                  PROTECTION_OFF,
                                                  static_cast<BsonObject*>(0));
    connection_handler_->set_protocol_handler(NULL);
    EXPECT_EQ(context.new_session_id_, out_context_.new_session_id_);
  }

  // Check Service Wrapper
  // If session_id is NULL - check that there is no sessions in connection
  void CheckSessionExists(const int connectionId, const int session_id) {
    // Check all tree to find Session and check own protected value
    const ConnectionList& connection_list =
        connection_handler_->getConnectionList();
    ASSERT_FALSE(connection_list.empty());
    ConnectionList::const_iterator conn_it = connection_list.find(connectionId);
    ASSERT_NE(conn_it, connection_list.end());
    const Connection& connection = *conn_it->second;

    const SessionMap& session_map = connection.session_map();
    SessionMap::const_iterator sess_it = session_map.find(session_id);
    if (session_id == 0) {
      ASSERT_TRUE(session_map.empty());
    } else {
      ASSERT_FALSE(session_map.empty());
      ASSERT_NE(sess_it, session_map.end());
      const Session& session = sess_it->second;
      const ServiceList& service_list = session.service_list;
      ASSERT_FALSE(service_list.empty());
      // Check RPC and bulk services in session
      ASSERT_NE(service_list.end(),
                std::find(service_list.begin(), service_list.end(), kRpc));
      ASSERT_NE(service_list.end(),
                std::find(service_list.begin(), service_list.end(), kBulk));
    }
  }

  // Check Service Wrapper
  void CheckServiceExists(
      const int connectionId,
      const int session_id,
      const ::protocol_handler::ServiceType serviceId,
      const ::transport_manager::ConnectionUID serviceConnectionId,
      const bool exists) {
    // Check all trees to find Service and check own protected value
    const ConnectionList& connection_list =
        connection_handler_->getConnectionList();
    ASSERT_FALSE(connection_list.empty());
    ConnectionList::const_iterator conn_it = connection_list.find(connectionId);
    ASSERT_NE(conn_it, connection_list.end());
    const Connection& connection = *conn_it->second;

    const SessionMap& session_map = connection.session_map();
    ASSERT_FALSE(session_map.empty());
    SessionMap::const_iterator sess_it = session_map.find(session_id);
    ASSERT_NE(sess_it, session_map.end());
    const Session& session = sess_it->second;
    const ServiceList& service_list = session.service_list;
    ASSERT_FALSE(service_list.empty());
    ServiceList::const_iterator serv_it =
        std::find(service_list.begin(), service_list.end(), serviceId);
    if (exists) {
      ASSERT_NE(serv_it, service_list.end());
      ASSERT_EQ(serv_it->connection_id, serviceConnectionId);
    } else {
      ASSERT_EQ(serv_it, service_list.end());
    }
  }

// Check Service Wrapper
#ifdef ENABLE_SECURITY
  void CheckService(const int connectionId,
                    const int session_id,
                    const ::protocol_handler::ServiceType serviceId,
                    const ::security_manager::SSLContext* ssl_context,
                    const bool is_protected) {
    // Check all tree to find Service and check own protected value
    const ConnectionList& connection_list =
        connection_handler_->getConnectionList();
    ASSERT_FALSE(connection_list.empty());
    ConnectionList::const_iterator conn_it = connection_list.find(connectionId);
    ASSERT_NE(conn_it, connection_list.end());
    const Connection& connection = *conn_it->second;

    const SessionMap& session_map = connection.session_map();
    ASSERT_FALSE(session_map.empty());
    SessionMap::const_iterator sess_it = session_map.find(session_id);
    ASSERT_NE(sess_it, session_map.end());
    const Session& session = sess_it->second;
    ASSERT_EQ(session.ssl_context, ssl_context);
    const ServiceList& service_list = session.service_list;
    ASSERT_FALSE(service_list.empty());
    ServiceList::const_iterator serv_it =
        std::find(service_list.begin(), service_list.end(), serviceId);
    ASSERT_NE(serv_it, service_list.end());

    const Service& service = *serv_it;
    EXPECT_EQ(PROTECTION_OFF, service.is_protected_);
    if (is_protected) {
      // Emulate success protection - check enable service flag
      const uint32_t connection_key_ =
          connection_handler_->KeyFromPair(connectionId, session_id);
      connection_handler_->SetProtectionFlag(connection_key_, serviceId);
    }
  }
#endif  // ENABLE_SECURITY

  void CheckService(const int connectionId,
                    const int session_id,
                    const ::protocol_handler::ServiceType serviceId) {
    // Check all tree to find Service and check own protected value
    const ConnectionList& connection_list =
        connection_handler_->getConnectionList();
    ASSERT_FALSE(connection_list.empty());
    ConnectionList::const_iterator conn_it = connection_list.find(connectionId);
    ASSERT_NE(conn_it, connection_list.end());
    const Connection& connection = *conn_it->second;

    const SessionMap& session_map = connection.session_map();
    ASSERT_FALSE(session_map.empty());
    SessionMap::const_iterator sess_it = session_map.find(session_id);
    ASSERT_NE(sess_it, session_map.end());
    const Session& session = sess_it->second;
    const ServiceList& service_list = session.service_list;
    ASSERT_FALSE(service_list.empty());
    ServiceList::const_iterator serv_it =
        std::find(service_list.begin(), service_list.end(), serviceId);
    ASSERT_NE(serv_it, service_list.end());

    const Service& service = *serv_it;
    EXPECT_EQ(PROTECTION_OFF, service.is_protected_);
  }

  void ChangeProtocol(const int connectionId,
                      const int session_id,
                      const uint8_t protocol_version) {
    ConnectionList connection_list = connection_handler_->getConnectionList();

    ConnectionList::const_iterator conn_it =
        (connection_handler_->getConnectionList()).find(connectionId);
    ASSERT_NE(conn_it, connection_list.end());
    Connection* connection = conn_it->second;
    ASSERT_TRUE(connection != NULL);
    connection->UpdateProtocolVersionSession(session_id, protocol_version);
    uint8_t check_protocol_version;
    EXPECT_TRUE(
        connection->ProtocolVersion(session_id, check_protocol_version));
    EXPECT_EQ(check_protocol_version, protocol_version);
  }
  void AddSecondaryTestService(ServiceType service_type) {
    EXPECT_NE(0u, out_context_.new_session_id_);
    EXPECT_EQ(SessionHash(uid_, out_context_.new_session_id_),
              out_context_.hash_id_);
    CheckSessionExists(uid_, out_context_.new_session_id_);

    // Set protocol version to 5
    ChangeProtocol(uid_,
                   out_context_.new_session_id_,
                   protocol_handler::PROTOCOL_VERSION_5);

    connection_handler_test::MockConnectionHandlerObserver
        temp_connection_handler_observer;
    connection_handler_->set_connection_handler_observer(
        &temp_connection_handler_observer);
    EXPECT_CALL(temp_connection_handler_observer,
                OnServiceStartedCallback(_, _, service_type, _))
        .Times(1);

    connection_handler_->OnSessionStartedCallback(2u,
                                                  out_context_.new_session_id_,
                                                  service_type,
                                                  PROTECTION_OFF,
                                                  static_cast<BsonObject*>(0));
    connection_handler_->set_connection_handler_observer(NULL);
  }

  ConnectionHandlerImpl* connection_handler_;
  testing::NiceMock<transport_manager_test::MockTransportManager>
      mock_transport_manager_;
  testing::NiceMock<MockConnectionHandlerSettings>
      mock_connection_handler_settings_;
  protocol_handler_test::MockProtocolHandler mock_protocol_handler_;
  transport_manager::DeviceHandle device_handle_;
  transport_manager::ConnectionUID uid_;
  uint32_t connection_key_;
  protocol_handler::SessionContext out_context_;

  std::string connection_type_;
  std::string device_name_;
  std::string mac_address_;

  const uint32_t heartbeat_timeout = 100u;
  std::vector<int> protected_services_;
  std::vector<int> unprotected_services_;
};

TEST_F(ConnectionHandlerTest, StartSession_NoConnection) {
  // Null sessionId for start new session
  const uint8_t sessionID = 0;
  // Start new session with RPC service
  protocol_handler::SessionContext context;

  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&context));

  connection_handler_->OnSessionStartedCallback(
      uid_, sessionID, kRpc, PROTECTION_ON, static_cast<BsonObject*>(NULL));
  // Unknown connection error is '0'
  EXPECT_EQ(0u, context.new_session_id_);
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, context.hash_id_);
  ASSERT_TRUE(connection_handler_->getConnectionList().empty());
}

TEST_F(ConnectionHandlerTest, StartSession) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  // Start new session with RPC service
  AddTestSession();
}

TEST_F(ConnectionHandlerTest, AddConnection_StopConnection) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  EXPECT_FALSE(connection_handler_->getConnectionList().empty());
  // Stop connection
  connection_handler_->Stop();
  EXPECT_TRUE(connection_handler_->getConnectionList().empty());
}

TEST_F(ConnectionHandlerTest, GetConnectionSessionsCount) {
  ASSERT_TRUE(connection_handler_->getConnectionList().empty());
  EXPECT_EQ(0u,
            connection_handler_->GetConnectionSessionsCount(connection_key_));

  AddTestDeviceConnection();
  EXPECT_EQ(0u,
            connection_handler_->GetConnectionSessionsCount(connection_key_));

  AddTestSession();
  EXPECT_EQ(1u,
            connection_handler_->GetConnectionSessionsCount(connection_key_));
}

TEST_F(ConnectionHandlerTest, GetAppIdOnSessionKey) {
  AddTestDeviceConnection();
  AddTestSession();

  uint32_t app_id = 0;
  const uint32_t test_id = SessionHash(uid_, out_context_.new_session_id_);

  EXPECT_EQ(0,
            connection_handler_->GetDataOnSessionKey(
                connection_key_, &app_id, nullptr, nullptr));
  EXPECT_EQ(test_id, app_id);
}

TEST_F(ConnectionHandlerTest, GetAppIdOnSessionKey_InvalidConnectionId) {
  uint8_t invalid_id = 0;
  EXPECT_EQ(-1,
            connection_handler_->GetDataOnSessionKey(
                invalid_id, nullptr, nullptr, nullptr));
}

TEST_F(ConnectionHandlerTest, GetAppIdOnSessionKey_SessionNotStarted) {
  AddTestDeviceConnection();

  uint32_t app_id = 0;
  connection_handler::DeviceHandle* device_id = NULL;
  EXPECT_EQ(-1,
            connection_handler_->GetDataOnSessionKey(
                connection_key_, &app_id, NULL, device_id));
}

TEST_F(ConnectionHandlerTest, GetDeviceID) {
  AddTestDeviceConnection();
  AddTestSession();

  DeviceHandle test_handle;
  const DeviceMap& devmap = connection_handler_->getDeviceList();
  DeviceMap::const_iterator pos = devmap.find(device_handle_);
  ASSERT_NE(pos, devmap.end());
  const Device& devres = pos->second;
  std::string test_mac_address = devres.mac_address();

  std::string invalid_mac_address;
  EXPECT_FALSE(
      connection_handler_->GetDeviceID(invalid_mac_address, &test_handle));

  EXPECT_TRUE(connection_handler_->GetDeviceID(test_mac_address, &test_handle));
  EXPECT_EQ(device_handle_, test_handle);
}

TEST_F(ConnectionHandlerTest, GetDataOnDeviceID_InvalidDeviceHandle) {
  const DeviceHandle handle = 0;
  EXPECT_EQ(-1,
            connection_handler_->GetDataOnDeviceID(
                handle, nullptr, nullptr, nullptr, nullptr));
}

TEST_F(ConnectionHandlerTest, GetDeviceName) {
  AddTestDeviceConnection();
  AddTestSession();

  std::string test_device_name;
  DeviceHandle handle = 0;
  EXPECT_EQ(0,
            connection_handler_->GetDataOnDeviceID(handle, &test_device_name));
  EXPECT_EQ(device_name_, test_device_name);
}

TEST_F(ConnectionHandlerTest, GetConnectionType) {
  AddTestDeviceConnection();
  AddTestSession();

  const DeviceHandle handle = 0;
  std::string test_connection_type;
  EXPECT_EQ(0,
            connection_handler_->GetDataOnDeviceID(
                handle, nullptr, nullptr, nullptr, &test_connection_type));
  EXPECT_EQ(connection_type_, test_connection_type);
}

TEST_F(ConnectionHandlerTest, GetApplicationsOnDevice) {
  AddTestDeviceConnection();
  AddTestSession();

  const DeviceHandle handle = 0;
  std::list<uint32_t> applications_list;
  EXPECT_EQ(
      0,
      connection_handler_->GetDataOnDeviceID(handle, NULL, &applications_list));

  uint32_t test_id =
      connection_handler_->KeyFromPair(uid_, out_context_.new_session_id_);
  EXPECT_EQ(1u, applications_list.size());

  EXPECT_EQ(test_id, applications_list.front());
}

TEST_F(ConnectionHandlerTest, ProtocolVersionUsed_Default_Success) {
  AddTestDeviceConnection();
  AddTestSession();

  uint8_t protocol_version = 0;
  EXPECT_TRUE(connection_handler_->ProtocolVersionUsed(
      uid_, out_context_.new_session_id_, protocol_version));

  EXPECT_EQ(PROTOCOL_VERSION_2, protocol_version);
}

TEST_F(ConnectionHandlerTest, ProtocolVersionUsed_Success) {
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(uid_, out_context_.new_session_id_, PROTOCOL_VERSION_3);

  uint8_t protocol_version = 0;
  EXPECT_TRUE(connection_handler_->ProtocolVersionUsed(
      uid_, out_context_.new_session_id_, protocol_version));

  EXPECT_EQ(PROTOCOL_VERSION_3, protocol_version);
}

TEST_F(ConnectionHandlerTest, ProtocolVersionUsed_AfterBinding_Success) {
  AddTestDeviceConnection();
  AddTestSession();
  uint8_t protocol_version = 0;
  EXPECT_TRUE(connection_handler_->ProtocolVersionUsed(
      uid_, out_context_.new_session_id_, protocol_version));
  EXPECT_EQ(PROTOCOL_VERSION_2, protocol_version);

  connection_handler_->BindProtocolVersionWithSession(connection_key_,
                                                      PROTOCOL_VERSION_3);

  EXPECT_TRUE(connection_handler_->ProtocolVersionUsed(
      uid_, out_context_.new_session_id_, protocol_version));
  EXPECT_EQ(PROTOCOL_VERSION_3, protocol_version);
}

TEST_F(ConnectionHandlerTest, ProtocolVersionUsed_WrongConnection_Fail) {
  AddTestDeviceConnection();
  AddTestSession();
  uint8_t connection_id = 0;
  uint8_t protocol_version = 0;
  EXPECT_FALSE(connection_handler_->ProtocolVersionUsed(
      connection_id, out_context_.new_session_id_, protocol_version));
}

TEST_F(ConnectionHandlerTest, GetPairFromKey) {
  AddTestDeviceConnection();
  AddTestSession();

  uint8_t session_id = 0;
  uint32_t test_uid = 0;
  connection_handler_->PairFromKey(connection_key_, &test_uid, &session_id);
  EXPECT_EQ(uid_, test_uid);
  EXPECT_EQ(out_context_.new_session_id_, session_id);
}

MATCHER_P(SameDevice, device, "") {
  return arg.device_handle() == device.device_handle() &&
         arg.user_friendly_name() == device.user_friendly_name() &&
         arg.mac_address() == device.mac_address() &&
         arg.connection_type() == device.connection_type();
}

TEST_F(ConnectionHandlerTest, IsHeartBeatSupported_Success) {
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(uid_, out_context_.new_session_id_, PROTOCOL_VERSION_3);

  EXPECT_TRUE(connection_handler_->IsHeartBeatSupported(
      uid_, out_context_.new_session_id_));
}

TEST_F(ConnectionHandlerTest, IsHeartBeatSupported_Fail) {
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(uid_, out_context_.new_session_id_, PROTOCOL_VERSION_3);

  uint8_t invalid_id = 0u;
  EXPECT_FALSE(connection_handler_->IsHeartBeatSupported(
      invalid_id, out_context_.new_session_id_));
}

TEST_F(ConnectionHandlerTest, SendEndServiceWithoutSetProtocolHandler) {
  AddTestDeviceConnection();
  AddTestSession();
  EXPECT_CALL(mock_protocol_handler_, SendEndService(_, _, _, kRpc)).Times(0);
  connection_handler_->SendEndService(connection_key_, kRpc);
}

TEST_F(ConnectionHandlerTest, SendEndService_NoSession_Fail) {
  AddTestDeviceConnection();
  EXPECT_CALL(mock_protocol_handler_, SendEndService(_, _, _, kRpc)).Times(0);
  connection_handler_->SendEndService(connection_key_, kRpc);
}

TEST_F(ConnectionHandlerTest, SendEndService) {
  AddTestDeviceConnection();
  AddTestSession();
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, SendEndService(_, _, _, kRpc));
  connection_handler_->SendEndService(connection_key_, kRpc);
}

TEST_F(ConnectionHandlerTest, OnFindNewApplicationsRequest) {
  AddTestDeviceConnection();
  AddTestSession();
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  EXPECT_CALL(mock_connection_handler_observer, OnFindNewApplicationsRequest());
  connection_handler_->OnFindNewApplicationsRequest();
}

TEST_F(ConnectionHandlerTest, OnFindNewApplicationsRequestWithoutObserver) {
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  EXPECT_CALL(mock_connection_handler_observer, OnFindNewApplicationsRequest())
      .Times(0);
  connection_handler_->OnFindNewApplicationsRequest();
}

TEST_F(ConnectionHandlerTest, OnFindNewApplicationsRequestWithoutSession) {
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  EXPECT_CALL(mock_connection_handler_observer, OnFindNewApplicationsRequest());
  connection_handler_->OnFindNewApplicationsRequest();
}

TEST_F(ConnectionHandlerTest, OnMalformedMessageCallback) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kMobileNav, kMalformed))
      .Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kAudio, kMalformed))
      .Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kMalformed))
      .Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kMalformed))
      .Times(1);
  connection_handler_->OnMalformedMessageCallback(uid_);
}

TEST_F(ConnectionHandlerTest, OnApplicationFloodCallBack) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  connection_handler_->set_protocol_handler(&mock_protocol_handler_);

  EXPECT_CALL(mock_protocol_handler_,
              SendEndSession(uid_, out_context_.new_session_id_))
      .Times(1);
  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kMobileNav, kCommon));
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kAudio, kCommon));
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kCommon));
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kCommon));
  connection_handler_->OnApplicationFloodCallBack(uid_);
}

TEST_F(ConnectionHandlerTest, OnApplicationFloodCallBack_SessionFound) {
  // Precondition
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_,
              SendEndSession(uid_, out_context_.new_session_id_));
  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kMobileNav, kFlood));
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kAudio, kFlood));
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kFlood));
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kFlood));

  connection_handler_->OnApplicationFloodCallBack(connection_key_);
}

TEST_F(ConnectionHandlerTest, StartDevicesDiscovery) {
  AddTestDeviceConnection();
  AddTestSession();

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  EXPECT_CALL(mock_transport_manager_, SearchDevices());
  EXPECT_CALL(mock_connection_handler_observer, OnDeviceListUpdated(_));
  connection_handler_->StartDevicesDiscovery();
}

MATCHER_P(CheckDevList, check_device_map, "") {
  DeviceMap device_list = arg;
  if (device_list.size() != check_device_map.size()) {
    return false;
  }
  DeviceMap::const_iterator it = device_list.begin();
  DeviceMap::const_iterator end = device_list.end();

  DeviceMap::const_iterator check_it = check_device_map.begin();
  for (; it != end; ++it, ++check_it) {
    if (it->first != check_it->first) {
      return false;
    }
    if ((*it).second.device_handle() != (*check_it).second.device_handle()) {
      return false;
    }
    if ((*it).second.mac_address() != (*check_it).second.mac_address()) {
      return false;
    }
    if ((*it).second.connection_type() !=
        (*check_it).second.connection_type()) {
      return false;
    }
  }
  return true;
}

TEST_F(ConnectionHandlerTest, UpdateDeviceList) {
  // Precondition
  AddTestDeviceConnection();
  AddTestSession();
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  connection_type_ = "BTMAC";
  device_name_ = "test_name";
  mac_address_ = "test_address";
  const transport_manager::DeviceInfo added_info(
      0, "test_address", "test_name", "BTMAC");
  connection_handler_->OnDeviceAdded(added_info);

  // Prepare map with DeviceInfo that sets in OnDeviceListUpdated
  const transport_manager::DeviceInfo unused_parameter(
      0, "mac_address_", "device_name_", "connection_type_");
  std::vector<transport_manager::DeviceInfo> unused_info;
  unused_info.push_back(unused_parameter);
  DeviceMap map_with_unused_var;
  map_with_unused_var.insert(
      DeviceMap::value_type(unused_parameter.device_handle(),
                            Device(unused_parameter.device_handle(),
                                   unused_parameter.name(),
                                   unused_parameter.mac_address(),
                                   unused_parameter.connection_type())));

  // Only previously added devices is updated
  EXPECT_CALL(
      mock_connection_handler_observer,
      OnDeviceListUpdated(CheckDevList(connection_handler_->getDeviceList())));

  // New value that sets in OnDeviceListUpdated does not add
  EXPECT_CALL(mock_connection_handler_observer,
              OnDeviceListUpdated(CheckDevList(map_with_unused_var)))
      .Times(0);

  connection_handler_->OnDeviceListUpdated(unused_info);
}

TEST_F(ConnectionHandlerTest, GetConnectedDevicesMAC) {
  // Precondition
  const std::string mac_address1 = "test_address1";
  const std::string mac_address2 = "test_address2";

  const transport_manager::DeviceInfo device1(
      0, mac_address1, device_name_, connection_type_);
  const transport_manager::DeviceInfo device2(
      1, mac_address2, device_name_, connection_type_);
  connection_handler_->OnDeviceAdded(device1);
  connection_handler_->OnDeviceAdded(device2);

  // Expect mac adress was hashed and saved
  const std::string check_mac1 = encryption::MakeHash(mac_address1);
  const std::string check_mac2 = encryption::MakeHash(mac_address2);
  std::vector<std::string> device_macs;
  connection_handler_->GetConnectedDevicesMAC(device_macs);
  ASSERT_EQ(2u, device_macs.size());
  EXPECT_EQ(check_mac1, device_macs.at(0));
  EXPECT_EQ(check_mac2, device_macs.at(1));
}

TEST_F(ConnectionHandlerTest, StartTransportManager) {
  AddTestDeviceConnection();
  AddTestSession();

  EXPECT_CALL(mock_transport_manager_,
              PerformActionOnClients(
                  transport_manager::TransportAction::kVisibilityOn));
  connection_handler_->StartTransportManager();
}

TEST_F(ConnectionHandlerTest, OnDeviceRemoved_ServiceNotStarted) {
  // Precondition
  const uint32_t dev_handle1 = 1;
  const uint32_t dev_handle2 = 2;

  const transport_manager::DeviceInfo device1(
      dev_handle1, mac_address_, device_name_, connection_type_);
  const transport_manager::DeviceInfo device2(
      dev_handle2, mac_address_, device_name_, connection_type_);
  connection_handler_->OnDeviceAdded(device1);
  connection_handler_->OnDeviceAdded(device2);

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  EXPECT_CALL(mock_connection_handler_observer, RemoveDevice(dev_handle1));
  EXPECT_CALL(mock_connection_handler_observer, OnServiceEndedCallback(_, _, _))
      .Times(0);
  connection_handler_->OnDeviceRemoved(device1);
}

TEST_F(ConnectionHandlerTest, OnDeviceRemoved_ServiceStarted) {
  // Precondition
  AddTestDeviceConnection();
  AddTestSession();

  const transport_manager::DeviceInfo device1(
      device_handle_, mac_address_, device_name_, connection_type_);

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  EXPECT_CALL(mock_connection_handler_observer, RemoveDevice(device_handle_));

  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kCommon));
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kCommon));
  connection_handler_->OnDeviceRemoved(device1);
}

TEST_F(ConnectionHandlerTest, OnConnectionClosed_InvalidConnectionId_Fail) {
  AddTestDeviceConnection();
  AddTestSession();

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kCommon))
      .Times(0);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kCommon))
      .Times(0);

  uint8_t invalid_id = 0;
  connection_handler_->OnConnectionClosed(invalid_id);
}

TEST_F(ConnectionHandlerTest, OnConnectionClosed_Success) {
  AddTestDeviceConnection();
  AddTestSession();

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kCommon));
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kCommon));

  connection_handler_->OnConnectionClosed(uid_);
}

TEST_F(ConnectionHandlerTest, OnFinalMessageCallback_OnConnectionClosed) {
  AddTestDeviceConnection();
  AddTestSession();

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kFinalMessage));
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kFinalMessage));

  connection_handler_->OnFinalMessageCallback(connection_key_);
  connection_handler_->OnConnectionClosed(uid_);
}

TEST_F(ConnectionHandlerTest, OnUnexpectedDisconnect) {
  AddTestDeviceConnection();
  AddTestSession();

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);
  transport_manager::CommunicationError err;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(
                  connection_key_, kBulk, CloseSessionReason::kCommon));
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(
                  connection_key_, kRpc, CloseSessionReason::kCommon));

  connection_handler_->OnUnexpectedDisconnect(uid_, err);
}

TEST_F(ConnectionHandlerTest, OnFinalMessageCallback_OnUnexpectedDisconnect) {
  AddTestDeviceConnection();
  AddTestSession();

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(
                  connection_key_, kBulk, CloseSessionReason::kFinalMessage));
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(
                  connection_key_, kRpc, CloseSessionReason::kFinalMessage));

  connection_handler_->OnFinalMessageCallback(connection_key_);
  transport_manager::CommunicationError err;
  connection_handler_->OnUnexpectedDisconnect(uid_, err);
}

TEST_F(ConnectionHandlerTest, ConnectToDevice) {
  // Precondition
  const uint32_t dev_handle1 = 1;
  const uint32_t dev_handle2 = 2;

  const transport_manager::DeviceInfo device1(
      dev_handle1, mac_address_, device_name_, connection_type_);
  const transport_manager::DeviceInfo device2(
      dev_handle2, mac_address_, device_name_, connection_type_);
  connection_handler_->OnDeviceAdded(device1);
  connection_handler_->OnDeviceAdded(device2);

  EXPECT_CALL(mock_transport_manager_, ConnectDevice(dev_handle1))
      .WillOnce(Return(transport_manager::E_SUCCESS));
  EXPECT_CALL(mock_transport_manager_, ConnectDevice(dev_handle2)).Times(0);
  connection_handler_->ConnectToDevice(dev_handle1);
}

TEST_F(ConnectionHandlerTest, ConnectToAllDevices) {
  // Precondition
  const uint32_t dev_handle1 = 1;
  const uint32_t dev_handle2 = 2;

  const transport_manager::DeviceInfo device1(
      dev_handle1, mac_address_, device_name_, connection_type_);
  const transport_manager::DeviceInfo device2(
      dev_handle2, mac_address_, device_name_, connection_type_);
  connection_handler_->OnDeviceAdded(device1);
  connection_handler_->OnDeviceAdded(device2);

  EXPECT_CALL(mock_transport_manager_, ConnectDevice(dev_handle1))
      .WillOnce(Return(transport_manager::E_SUCCESS));
  EXPECT_CALL(mock_transport_manager_, ConnectDevice(dev_handle2))
      .WillOnce(Return(transport_manager::E_SUCCESS));
  connection_handler_->ConnectToAllDevices();
}

TEST_F(ConnectionHandlerTest, CloseConnection) {
  AddTestDeviceConnection();
  AddTestSession();

  EXPECT_CALL(mock_transport_manager_, DisconnectForce(uid_));
  connection_handler_->CloseConnection(uid_);
}

TEST_F(ConnectionHandlerTest, CloseRevokedConnection) {
  AddTestDeviceConnection();
  AddTestSession();

  EXPECT_CALL(mock_transport_manager_, DisconnectForce(uid_));
  connection_handler_->CloseRevokedConnection(connection_key_);
}

TEST_F(ConnectionHandlerTest, CloseSessionWithCommonReason) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  connection_handler_->set_protocol_handler(&mock_protocol_handler_);

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(mock_protocol_handler_,
              SendEndSession(uid_, out_context_.new_session_id_))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kMobileNav, kCommon))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kAudio, kCommon))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kCommon))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kCommon))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  connection_handler_->CloseSession(connection_key_, kCommon);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ConnectionHandlerTest, CloseSessionWithFloodReason) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  connection_handler_->set_protocol_handler(&mock_protocol_handler_);

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(mock_protocol_handler_,
              SendEndSession(uid_, out_context_.new_session_id_))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kMobileNav, kFlood))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kAudio, kFlood))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kFlood))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kFlood))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  connection_handler_->CloseSession(connection_key_, kFlood);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ConnectionHandlerTest, CloseSessionWithMalformedMessage) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  connection_handler_->set_protocol_handler(&mock_protocol_handler_);

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(mock_protocol_handler_,
              SendEndSession(uid_, out_context_.new_session_id_))
      .Times(0);

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kMobileNav, kMalformed))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kAudio, kMalformed))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kMalformed))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kMalformed))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  connection_handler_->CloseSession(connection_key_, kMalformed);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ConnectionHandlerTest, CloseConnectionSessionsWithMalformedMessage) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  connection_handler_->set_protocol_handler(&mock_protocol_handler_);

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(mock_protocol_handler_,
              SendEndSession(uid_, out_context_.new_session_id_))
      .Times(0);

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kMobileNav, kMalformed))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kAudio, kMalformed))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kMalformed))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kMalformed))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  connection_handler_->CloseConnectionSessions(uid_, kMalformed);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ConnectionHandlerTest, CloseConnectionSessionsInvalidConnectionId) {
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  connection_handler_->set_protocol_handler(&mock_protocol_handler_);

  EXPECT_CALL(mock_protocol_handler_, SendEndSession(_, _)).Times(0);
  EXPECT_CALL(mock_connection_handler_observer, OnServiceEndedCallback(_, _, _))
      .Times(0);
  EXPECT_CALL(mock_connection_handler_observer, OnServiceEndedCallback(_, _, _))
      .Times(0);

  uint8_t invalid_id = 0u;
  connection_handler_->CloseConnectionSessions(invalid_id, kCommon);
}

TEST_F(ConnectionHandlerTest, CloseConnectionSessionsWithCommonReason) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  connection_handler_->set_protocol_handler(&mock_protocol_handler_);

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(mock_protocol_handler_,
              SendEndSession(uid_, out_context_.new_session_id_))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kMobileNav, kCommon))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kAudio, kCommon))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kBulk, kCommon))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_, kRpc, kCommon))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  connection_handler_->CloseConnectionSessions(uid_, kCommon);

  EXPECT_TRUE(waiter.WaitFor(times, kAsyncExpectationsTimeout));
}

TEST_F(ConnectionHandlerTest, StartService_withServices) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(
      uid_, out_context_.new_session_id_, protocol_handler::PROTOCOL_VERSION_3);

  SessionContext audio_context, video_context;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&audio_context))
      .WillOnce(SaveArg<0>(&video_context));

  // Start Audio service
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kAudio,
                                                PROTECTION_OFF,
                                                static_cast<BsonObject*>(NULL));
  EXPECT_NE(0u, audio_context.new_session_id_);
  CheckServiceExists(uid_, audio_context.new_session_id_, kAudio, uid_, true);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, audio_context.hash_id_);

  // Start Audio service
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kMobileNav,
                                                PROTECTION_OFF,
                                                static_cast<BsonObject*>(NULL));
  EXPECT_NE(0u, video_context.new_session_id_);
  CheckServiceExists(
      uid_, video_context.new_session_id_, kMobileNav, uid_, true);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, video_context.hash_id_);

  connection_handler_->set_protocol_handler(NULL);
}

TEST_F(ConnectionHandlerTest, StartService_withServices_withParams) {
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(
      uid_, out_context_.new_session_id_, protocol_handler::PROTOCOL_VERSION_3);

  SessionContext video_context;

  // create a dummy pointer
  int dummy = 0;
  std::vector<std::string> empty;
  BsonObject* dummy_param = reinterpret_cast<BsonObject*>(&dummy);
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, empty, _))
      .WillOnce(SaveArg<0>(&video_context));

  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kMobileNav,
                                                PROTECTION_OFF,
                                                dummy_param);
  EXPECT_EQ(out_context_.new_session_id_, video_context.new_session_id_);
  CheckServiceExists(
      uid_, out_context_.new_session_id_, kMobileNav, uid_, true);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, video_context.hash_id_);

  connection_handler_->set_protocol_handler(NULL);
}

TEST_F(ConnectionHandlerTest, ServiceStop_UnExistSession) {
  AddTestDeviceConnection();
  uint32_t dummy_hash = 0u;
  const uint32_t end_session_result =
      connection_handler_->OnSessionEndedCallback(
          uid_, 0u, &dummy_hash, kAudio);
  EXPECT_EQ(0u, end_session_result);
  CheckSessionExists(uid_, 0);
}

TEST_F(ConnectionHandlerTest, ServiceStop_UnExistService) {
  AddTestDeviceConnection();
  AddTestSession();
  uint32_t dummy_hash = 0u;
  const uint32_t end_session_result =
      connection_handler_->OnSessionEndedCallback(
          uid_, out_context_.new_session_id_, &dummy_hash, kAudio);
  EXPECT_EQ(0u, end_session_result);
  CheckServiceExists(uid_, out_context_.new_session_id_, kAudio, uid_, false);
}

TEST_F(ConnectionHandlerTest, ServiceStop) {
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(
      uid_, out_context_.new_session_id_, protocol_handler::PROTOCOL_VERSION_3);

  SessionContext audio_context;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillRepeatedly(SaveArg<0>(&audio_context));

  // Check ignoring hash_id on stop non-rpc service
  for (uint32_t some_hash_id = 0; some_hash_id < 0xFF; ++some_hash_id) {
    // Start audio service
    connection_handler_->OnSessionStartedCallback(
        uid_,
        out_context_.new_session_id_,
        kAudio,
        PROTECTION_OFF,
        static_cast<BsonObject*>(NULL));
    EXPECT_EQ(out_context_.new_session_id_, audio_context.new_session_id_);
    EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, audio_context.hash_id_);

    const uint32_t end_session_result =
        connection_handler_->OnSessionEndedCallback(
            uid_, out_context_.new_session_id_, &some_hash_id, kAudio);
    EXPECT_EQ(connection_key_, end_session_result);
    CheckServiceExists(uid_, out_context_.new_session_id_, kAudio, uid_, false);
  }
}

TEST_F(ConnectionHandlerTest, SessionStop_CheckHash) {
  AddTestDeviceConnection();
  for (uint32_t session = 0; session < 0xFF; ++session) {
    AddTestSession();

    uint32_t hash = connection_key_;
    uint32_t wrong_hash = hash + 1;

    const uint32_t end_audio_wrong_hash =
        connection_handler_->OnSessionEndedCallback(
            uid_, out_context_.new_session_id_, &wrong_hash, kRpc);
    EXPECT_EQ(0u, end_audio_wrong_hash);
    EXPECT_EQ(protocol_handler::HASH_ID_WRONG, wrong_hash);
    CheckSessionExists(uid_, out_context_.new_session_id_);

    const uint32_t end_audio = connection_handler_->OnSessionEndedCallback(
        uid_, out_context_.new_session_id_, &hash, kRpc);
    EXPECT_EQ(connection_key_, end_audio);
    CheckSessionExists(uid_, 0);
  }
}

TEST_F(ConnectionHandlerTest, SessionStop_CheckSpecificHash) {
  AddTestDeviceConnection();
  for (uint32_t session = 0; session < 0xFF; ++session) {
    AddTestSession();

    uint32_t hash = connection_key_;
    uint32_t wrong_hash = hash + 1;

    const uint32_t end_audio_wrong_hash =
        connection_handler_->OnSessionEndedCallback(
            uid_, out_context_.new_session_id_, &wrong_hash, kRpc);
    EXPECT_EQ(0u, end_audio_wrong_hash);
    EXPECT_EQ(protocol_handler::HASH_ID_WRONG, wrong_hash);
    CheckSessionExists(uid_, out_context_.new_session_id_);

    const uint32_t end_audio = connection_handler_->OnSessionEndedCallback(
        uid_, out_context_.new_session_id_, &hash, kRpc);
    EXPECT_EQ(connection_key_, end_audio);
    CheckSessionExists(uid_, 0);
  }
}

TEST_F(ConnectionHandlerTest, SessionStarted_WithRpc) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  out_context_.initial_session_id_ = 1u;
  // Expect that rpc service has started
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);
  std::vector<std::string> empty;
  std::string reason;
  uint32_t session_key =
      connection_handler_->KeyFromPair(uid_, out_context_.initial_session_id_);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceStartedCallback(device_handle_, _, kRpc, NULL))
      .WillOnce(InvokeMemberFuncWithArg4(
          connection_handler_,
          &ConnectionHandler::NotifyServiceStartedResult,
          session_key,
          true,
          ByRef(empty),
          reason));

  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&out_context_));

  // Start new session with RPC service
  connection_handler_->OnSessionStartedCallback(
      uid_, 0, kRpc, PROTECTION_OFF, static_cast<BsonObject*>(NULL));

  EXPECT_NE(0u, out_context_.new_session_id_);
}

TEST_F(ConnectionHandlerTest, ServiceStarted_Video_SUCCESS) {
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(
      uid_, out_context_.new_session_id_, protocol_handler::PROTOCOL_VERSION_3);

  int dummy = 0;
  BsonObject* dummy_params = reinterpret_cast<BsonObject*>(&dummy);

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);
  uint32_t session_key =
      connection_handler_->KeyFromPair(uid_, out_context_.new_session_id_);
  std::vector<std::string> empty;
  std::string reason;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceStartedCallback(
                  device_handle_, session_key, kMobileNav, dummy_params))
      .WillOnce(InvokeMemberFuncWithArg4(
          connection_handler_,
          &ConnectionHandler::NotifyServiceStartedResult,
          session_key,
          true,
          ByRef(empty),
          reason));

  // confirm that NotifySessionStarted() is called
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, empty, _))
      .WillOnce(SaveArg<0>(&out_context_));

  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kMobileNav,
                                                PROTECTION_OFF,
                                                dummy_params);

  EXPECT_NE(0u, out_context_.new_session_id_);
}

TEST_F(ConnectionHandlerTest, ServiceStarted_Video_FAILURE) {
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(
      uid_, out_context_.new_session_id_, protocol_handler::PROTOCOL_VERSION_3);

  int dummy = 0;
  BsonObject* dummy_params = reinterpret_cast<BsonObject*>(&dummy);

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);
  uint32_t session_key =
      connection_handler_->KeyFromPair(uid_, out_context_.new_session_id_);
  std::vector<std::string> empty;
  std::string reason;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceStartedCallback(
                  device_handle_, session_key, kMobileNav, dummy_params))
      .WillOnce(InvokeMemberFuncWithArg4(
          connection_handler_,
          &ConnectionHandler::NotifyServiceStartedResult,
          session_key,
          false,
          ByRef(empty),
          reason));

  // confirm that NotifySessionStarted() is called
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, empty, _))
      .WillOnce(SaveArg<0>(&out_context_));

  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kMobileNav,
                                                PROTECTION_OFF,
                                                dummy_params);

  EXPECT_EQ(0u, out_context_.new_session_id_);
}

/*
 * Simulate two OnSessionStartedCallback calls, and connection handler observer
 * returns a positive reply for the first call with delay and a negative reply
 * for the second call immediately.
 */
TEST_F(ConnectionHandlerTest, ServiceStarted_Video_Multiple) {
  AddTestDeviceConnection();

  SessionContext context_first, context_second;

  protocol_handler_test::MockProtocolHandler temp_protocol_handler;
  connection_handler_->set_protocol_handler(&temp_protocol_handler);
  EXPECT_CALL(temp_protocol_handler, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&context_first))
      .WillOnce(SaveArg<0>(&context_second));

  // add two sessions
  connection_handler_->OnSessionStartedCallback(
      uid_, 0, kRpc, PROTECTION_OFF, static_cast<BsonObject*>(NULL));
  connection_handler_->OnSessionStartedCallback(
      uid_, 0, kRpc, PROTECTION_OFF, static_cast<BsonObject*>(NULL));

  EXPECT_NE(0u, context_first.new_session_id_);
  EXPECT_NE(0u, context_second.new_session_id_);
  EXPECT_EQ(SessionHash(uid_, context_first.new_session_id_),
            context_first.hash_id_);
  EXPECT_EQ(SessionHash(uid_, context_second.new_session_id_),
            context_second.hash_id_);
  CheckSessionExists(uid_, context_first.new_session_id_);
  CheckSessionExists(uid_, context_second.new_session_id_);

  connection_handler_->set_protocol_handler(NULL);

  int dummy = 0;
  BsonObject* dummy_params = reinterpret_cast<BsonObject*>(&dummy);

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);
  uint32_t session_key1 =
      connection_handler_->KeyFromPair(uid_, context_first.new_session_id_);
  uint32_t session_key2 =
      connection_handler_->KeyFromPair(uid_, context_second.new_session_id_);

  std::vector<std::string> empty;
  std::string reason;

  ChangeProtocol(uid_,
                 context_first.new_session_id_,
                 protocol_handler::PROTOCOL_VERSION_3);
  ChangeProtocol(uid_,
                 context_second.new_session_id_,
                 protocol_handler::PROTOCOL_VERSION_3);

  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceStartedCallback(
                  device_handle_, session_key1, kMobileNav, dummy_params))
      // don't call NotifyServiceStartedResult() with this event
      .Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceStartedCallback(
                  device_handle_, session_key2, kMobileNav, dummy_params))
      // call NotifyServiceStartedResult() twice, first for the second session
      // then for the first session
      .WillOnce(DoAll(InvokeMemberFuncWithArg4(
                          connection_handler_,
                          &ConnectionHandler::NotifyServiceStartedResult,
                          session_key2,
                          false,
                          ByRef(empty),
                          reason),
                      InvokeMemberFuncWithArg4(
                          connection_handler_,
                          &ConnectionHandler::NotifyServiceStartedResult,
                          session_key1,
                          true,
                          ByRef(empty),
                          reason)));

  // verify that connection handler will not mix up the two results
  SessionContext new_context_first, new_context_second;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, empty, _))
      .WillOnce(SaveArg<0>(&new_context_second))
      .WillOnce(SaveArg<0>(&new_context_first));

  connection_handler_->OnSessionStartedCallback(uid_,
                                                context_first.new_session_id_,
                                                kMobileNav,
                                                PROTECTION_OFF,
                                                dummy_params);
  connection_handler_->OnSessionStartedCallback(uid_,
                                                context_second.new_session_id_,
                                                kMobileNav,
                                                PROTECTION_OFF,
                                                dummy_params);

  EXPECT_NE(0u, new_context_first.new_session_id_);   // result is positive
  EXPECT_EQ(0u, new_context_second.new_session_id_);  // result is negative
}

TEST_F(ConnectionHandlerTest,
       SessionStarted_StartSession_SecureSpecific_Unprotect) {
  EXPECT_CALL(mock_connection_handler_settings_, heart_beat_timeout())
      .WillOnce(Return(heartbeat_timeout));
  // Add virtual device and connection
  AddTestDeviceConnection();
  // Forbid start kRPC without encryption
  protected_services_.push_back(kRpc);
  SetSpecificServices();

  SessionContext fail_context;
  SessionContext positive_context;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&fail_context))
      .WillOnce(SaveArg<0>(&positive_context));

  // Start new session with RPC service
  connection_handler_->OnSessionStartedCallback(
      uid_, 0, kRpc, PROTECTION_OFF, static_cast<BsonObject*>(NULL));
#ifdef ENABLE_SECURITY
  EXPECT_EQ(0u, fail_context.new_session_id_);
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, fail_context.hash_id_);
#else
  EXPECT_EQ(1u, fail_context.new_session_id_);
  EXPECT_EQ(SessionHash(uid_, fail_context.new_session_id_),
            fail_context.hash_id_);
#endif  // ENABLE_SECURITY

  // Allow start kRPC without encryption
  protected_services_.clear();
  protected_services_.push_back(kControl);
  SetSpecificServices();
  // Start new session with RPC service
  connection_handler_->OnSessionStartedCallback(
      uid_, 0, kRpc, PROTECTION_OFF, static_cast<BsonObject*>(NULL));
  EXPECT_NE(0u, positive_context.new_session_id_);
  CheckService(uid_, positive_context.new_session_id_, kRpc);
  EXPECT_EQ(SessionHash(uid_, positive_context.new_session_id_),
            positive_context.hash_id_);
}

TEST_F(ConnectionHandlerTest,
       SessionStarted_StartSession_SecureSpecific_Protect) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  // Forbid start kRPC with encryption
  unprotected_services_.push_back(UnnamedService::kServedService1);
  unprotected_services_.push_back(kRpc);
  unprotected_services_.push_back(UnnamedService::kServedService2);
  unprotected_services_.push_back(kControl);
  SetSpecificServices();

  SessionContext fail_context;
  SessionContext positive_context;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&fail_context))
      .WillOnce(SaveArg<0>(&positive_context));

  // Start new session with RPC service
  connection_handler_->OnSessionStartedCallback(
      uid_, 0, kRpc, PROTECTION_ON, static_cast<BsonObject*>(NULL));
#ifdef ENABLE_SECURITY
  EXPECT_EQ(0u, fail_context.new_session_id_);
#else
  EXPECT_EQ(1u, fail_context.new_session_id_);
#endif  // ENABLE_SECURITY

  // Allow start kRPC with encryption
  unprotected_services_.clear();
  unprotected_services_.push_back(kControl);
  SetSpecificServices();
  // Start new session with RPC service
  connection_handler_->OnSessionStartedCallback(
      uid_, 0, kRpc, PROTECTION_ON, static_cast<BsonObject*>(NULL));
  EXPECT_NE(0u, positive_context.new_session_id_);
  EXPECT_EQ(SessionHash(uid_, positive_context.new_session_id_),
            positive_context.hash_id_);
#ifdef ENABLE_SECURITY
  // Protection steal FALSE because of APPlink Protocol implementation
  CheckService(uid_, positive_context.new_session_id_, kRpc);
#endif  // ENABLE_SECURITY
}

TEST_F(ConnectionHandlerTest,
       SessionStarted_StartService_SecureSpecific_Unprotect) {
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(
      uid_, out_context_.new_session_id_, protocol_handler::PROTOCOL_VERSION_3);

  // Forbid start kAudio without encryption
  protected_services_.push_back(UnnamedService::kServedService1);
  protected_services_.push_back(kAudio);
  protected_services_.push_back(UnnamedService::kServedService2);
  protected_services_.push_back(kControl);
  SetSpecificServices();

  SessionContext context_first, context_second;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&context_first))
      .WillOnce(SaveArg<0>(&context_second));

  // Start new session with Audio service
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kAudio,
                                                PROTECTION_OFF,
                                                static_cast<BsonObject*>(NULL));
#ifdef ENABLE_SECURITY
  EXPECT_EQ(0u, context_first.new_session_id_);
#else
  EXPECT_EQ(1u, context_first.new_session_id_);
#endif  // ENABLE_SECURITY
  // Allow start kAudio without encryption
  protected_services_.clear();
  protected_services_.push_back(UnnamedService::kServedService1);
  protected_services_.push_back(kMobileNav);
  protected_services_.push_back(UnnamedService::kServedService2);
  protected_services_.push_back(kControl);
  SetSpecificServices();
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kAudio,
                                                PROTECTION_OFF,
                                                static_cast<BsonObject*>(NULL));
// Returned original session id
#ifdef ENABLE_SECURITY
  EXPECT_EQ(out_context_.new_session_id_, context_second.new_session_id_);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, context_second.hash_id_);
  CheckService(
      uid_, context_second.new_session_id_, kRpc, NULL, PROTECTION_OFF);
#else
  EXPECT_EQ(0u, context_second.new_session_id_);
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, context_second.hash_id_);
#endif  // ENABLE_SECURITY
}

TEST_F(ConnectionHandlerTest,
       SessionStarted_StartService_SecureSpecific_Protect) {
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(
      uid_, out_context_.new_session_id_, protocol_handler::PROTOCOL_VERSION_3);

  // Forbid start kAudio with encryption
  unprotected_services_.push_back(UnnamedService::kServedService1);
  unprotected_services_.push_back(kAudio);
  unprotected_services_.push_back(UnnamedService::kServedService2);
  unprotected_services_.push_back(kControl);
  SetSpecificServices();

  SessionContext rejected_context, positive_context;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&rejected_context))
      .WillOnce(SaveArg<0>(&positive_context));

  // Start new session with Audio service
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kAudio,
                                                PROTECTION_ON,
                                                static_cast<BsonObject*>(NULL));
#ifdef ENABLE_SECURITY
  EXPECT_EQ(0u, rejected_context.new_session_id_);
#else
  EXPECT_EQ(1u, rejected_context.new_session_id_);
#endif  // ENABLE_SECURITY
  // Allow start kAudio with encryption
  unprotected_services_.clear();
  SetSpecificServices();
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kAudio,
                                                PROTECTION_ON,
                                                static_cast<BsonObject*>(NULL));
// Returned original session id
#ifdef ENABLE_SECURITY
  EXPECT_EQ(out_context_.new_session_id_, positive_context.new_session_id_);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, positive_context.hash_id_);
  CheckService(
      uid_, positive_context.new_session_id_, kAudio, NULL, PROTECTION_ON);
#else
  EXPECT_EQ(0u, positive_context.new_session_id_);
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, positive_context.hash_id_);
#endif  // ENABLE_SECURITY
}

TEST_F(ConnectionHandlerTest, SessionStarted_DelayProtect) {
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(
      uid_, out_context_.new_session_id_, protocol_handler::PROTOCOL_VERSION_3);

  SessionContext context_new, context_second, context_third;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&context_new))
      .WillOnce(SaveArg<0>(&context_second))
      .WillOnce(SaveArg<0>(&context_third));

  // Start RPC protection
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kRpc,
                                                PROTECTION_ON,
                                                static_cast<BsonObject*>(NULL));
#ifdef ENABLE_SECURITY
  EXPECT_EQ(out_context_.new_session_id_, context_new.new_session_id_);
  // Post protection nedd no hash
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, context_new.hash_id_);
  CheckService(uid_, context_new.new_session_id_, kRpc, NULL, PROTECTION_ON);
#else
  EXPECT_EQ(0u, context_new.new_session_id_);
  // Post protection nedd no hash
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, context_new.hash_id_);
#endif  // ENABLE_SECURITY

  // Start Audio session without protection
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kAudio,
                                                PROTECTION_OFF,
                                                static_cast<BsonObject*>(NULL));
  EXPECT_EQ(out_context_.new_session_id_, context_second.new_session_id_);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, context_second.hash_id_);
  CheckService(uid_, context_second.new_session_id_, kAudio);

  // Start Audio protection
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kAudio,
                                                PROTECTION_ON,
                                                static_cast<BsonObject*>(NULL));
#ifdef ENABLE_SECURITY
  EXPECT_EQ(out_context_.new_session_id_, context_third.new_session_id_);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, context_third.hash_id_);
  CheckService(
      uid_, context_third.new_session_id_, kAudio, NULL, PROTECTION_ON);
#else
  EXPECT_EQ(0u, context_third.new_session_id_);
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, context_third.hash_id_);
#endif  // ENABLE_SECURITY
}

TEST_F(ConnectionHandlerTest, SessionStarted_DelayProtectBulk) {
  AddTestDeviceConnection();
  AddTestSession();

  SessionContext new_context;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&new_context));
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kBulk,
                                                PROTECTION_ON,
                                                static_cast<BsonObject*>(NULL));
#ifdef ENABLE_SECURITY
  EXPECT_EQ(out_context_.new_session_id_, new_context.new_session_id_);
  CheckService(uid_, new_context.new_session_id_, kRpc, NULL, PROTECTION_ON);
#else
  EXPECT_EQ(0u, new_context.new_session_id_);
#endif  // ENABLE_SECURITY
}

#ifdef ENABLE_SECURITY
TEST_F(ConnectionHandlerTest, SetSSLContext_Null) {
  // No SSLContext on start up
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            reinterpret_cast<security_manager::SSLContext*>(NULL));
  EXPECT_EQ(::security_manager::SecurityManager::ERROR_INTERNAL,
            connection_handler_->SetSSLContext(connection_key_, NULL));
  // No SSLContext after error
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            reinterpret_cast<security_manager::SSLContext*>(NULL));

  AddTestDeviceConnection();
  EXPECT_EQ(::security_manager::SecurityManager::ERROR_INTERNAL,
            connection_handler_->SetSSLContext(connection_key_, NULL));
  // No SSLContext after error
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            reinterpret_cast<security_manager::SSLContext*>(NULL));

  AddTestSession();
  EXPECT_EQ(::security_manager::SecurityManager::ERROR_SUCCESS,
            connection_handler_->SetSSLContext(connection_key_, NULL));
  // NULL SSLContext after success
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            reinterpret_cast<security_manager::SSLContext*>(NULL));
}

TEST_F(ConnectionHandlerTest, SetSSLContext) {
  // No SSLContext on start up
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            reinterpret_cast<security_manager::SSLContext*>(NULL));

  testing::StrictMock<security_manager_test::MockSSLContext> mock_ssl_context;
  // Error on no connection
  EXPECT_EQ(
      connection_handler_->SetSSLContext(connection_key_, &mock_ssl_context),
      ::security_manager::SecurityManager::ERROR_INTERNAL);
  // No SSLContext after error
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            reinterpret_cast<security_manager::SSLContext*>(NULL));

  AddTestDeviceConnection();
  // Error on no session
  EXPECT_EQ(
      connection_handler_->SetSSLContext(connection_key_, &mock_ssl_context),
      ::security_manager::SecurityManager::ERROR_INTERNAL);
  // No SSLContext after error
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            reinterpret_cast<security_manager::SSLContext*>(NULL));
  AddTestSession();
  // Success
  EXPECT_EQ(
      connection_handler_->SetSSLContext(connection_key_, &mock_ssl_context),
      ::security_manager::SecurityManager::ERROR_SUCCESS);
  // SSLContext set on Success
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            &mock_ssl_context);
  // Null SSLContext for unprotected services
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kRpc),
            reinterpret_cast<security_manager::SSLContext*>(NULL));
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kBulk),
            reinterpret_cast<security_manager::SSLContext*>(NULL));
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kAudio),
            reinterpret_cast<security_manager::SSLContext*>(NULL));
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kMobileNav),
            reinterpret_cast<security_manager::SSLContext*>(NULL));
}

TEST_F(ConnectionHandlerTest, GetSSLContext_ByProtectedService) {
  // No SSLContext on start up
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            reinterpret_cast<security_manager::SSLContext*>(NULL));

  testing::StrictMock<security_manager_test::MockSSLContext> mock_ssl_context;
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(
      uid_, out_context_.new_session_id_, protocol_handler::PROTOCOL_VERSION_3);

  EXPECT_EQ(
      connection_handler_->SetSSLContext(connection_key_, &mock_ssl_context),
      ::security_manager::SecurityManager::ERROR_SUCCESS);
  // kControl service mean - return for all connection
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            &mock_ssl_context);

  // kAudio is not exists yet
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kAudio),
            reinterpret_cast<security_manager::SSLContext*>(NULL));

  SessionContext new_context;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&new_context));

  // Open kAudio service
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kAudio,
                                                PROTECTION_ON,
                                                static_cast<BsonObject*>(NULL));
  EXPECT_EQ(new_context.new_session_id_, out_context_.new_session_id_);
  CheckService(uid_,
               new_context.new_session_id_,
               kAudio,
               &mock_ssl_context,
               PROTECTION_ON);

  // kAudio is not exists yet
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kAudio),
            &mock_ssl_context);
}

TEST_F(ConnectionHandlerTest, GetSSLContext_ByDealyProtectedRPC) {
  testing::StrictMock<security_manager_test::MockSSLContext> mock_ssl_context;
  AddTestDeviceConnection();
  AddTestSession();
  EXPECT_EQ(
      connection_handler_->SetSSLContext(connection_key_, &mock_ssl_context),
      ::security_manager::SecurityManager::ERROR_SUCCESS);
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            &mock_ssl_context);

  // kRpc is not protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kRpc),
            reinterpret_cast<security_manager::SSLContext*>(NULL));

  SessionContext new_context;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&new_context));

  // Protect kRpc (Bulk will be protect also)
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kRpc,
                                                PROTECTION_ON,
                                                static_cast<BsonObject*>(NULL));
  EXPECT_EQ(out_context_.new_session_id_, new_context.new_session_id_);
  CheckService(uid_,
               new_context.new_session_id_,
               kRpc,
               &mock_ssl_context,
               PROTECTION_ON);

  // kRpc is protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kRpc),
            &mock_ssl_context);
  // kBulk is protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kBulk),
            &mock_ssl_context);
}

TEST_F(ConnectionHandlerTest, GetSSLContext_ByDealyProtectedBulk) {
  testing::StrictMock<security_manager_test::MockSSLContext> mock_ssl_context;
  AddTestDeviceConnection();
  AddTestSession();
  EXPECT_EQ(
      connection_handler_->SetSSLContext(connection_key_, &mock_ssl_context),
      ::security_manager::SecurityManager::ERROR_SUCCESS);
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
            &mock_ssl_context);

  // kRpc is not protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kRpc),
            reinterpret_cast<security_manager::SSLContext*>(NULL));

  SessionContext new_context;
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_, NotifySessionStarted(_, _, _))
      .WillOnce(SaveArg<0>(&new_context));

  // Protect Bulk (kRpc will be protected also)
  connection_handler_->OnSessionStartedCallback(uid_,
                                                out_context_.new_session_id_,
                                                kBulk,
                                                PROTECTION_ON,
                                                static_cast<BsonObject*>(NULL));
  EXPECT_EQ(out_context_.new_session_id_, new_context.new_session_id_);
  CheckService(uid_,
               new_context.new_session_id_,
               kRpc,
               &mock_ssl_context,
               PROTECTION_ON);

  // kRpc is protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kRpc),
            &mock_ssl_context);
  // kBulk is protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kBulk),
            &mock_ssl_context);
}
#endif  // ENABLE_SECURITY

TEST_F(ConnectionHandlerTest, SendHeartBeat) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  AddTestSession();
  connection_handler_->set_protocol_handler(&mock_protocol_handler_);
  EXPECT_CALL(mock_protocol_handler_,
              SendHeartBeat(uid_, out_context_.new_session_id_));
  connection_handler_->SendHeartBeat(uid_, out_context_.new_session_id_);
}

TEST_F(ConnectionHandlerTest, RunAppOnDevice_NoAppOnDevice_UNSUCCESS) {
  const std::string bundle_id = "test_bundle_id";
  const std::string mac_address0 = "test_mac_address0";
  // All MAC addresses were stored in device list of ConnectionHandler in form
  // of hashed string.
  const std::string hash_of_mac_address0 = encryption::MakeHash(mac_address0);
  const std::string hash_of_mac_address1 =
      encryption::MakeHash("test_mac_address1");

  // By default device list of ConnectionHandler was empty.
  EXPECT_CALL(mock_transport_manager_, RunAppOnDevice(_, _)).Times(0);
  connection_handler_->RunAppOnDevice(hash_of_mac_address0, bundle_id);

  transport_manager::DeviceInfo device_info(
      device_handle_, mac_address0, device_name_, connection_type_);
  connection_handler_->OnDeviceAdded(device_info);

  connection_handler_->RunAppOnDevice(hash_of_mac_address1, bundle_id);
}

TEST_F(ConnectionHandlerTest, RunAppOnDevice_AppOnDevice_SUCCESS) {
  const std::string bundle_id = "test_bundle_id";

  transport_manager::DeviceInfo device_info(
      device_handle_, mac_address_, device_name_, connection_type_);
  connection_handler_->OnDeviceAdded(device_info);

  const std::string hash_of_mac_address = encryption::MakeHash(mac_address_);

  EXPECT_CALL(mock_transport_manager_,
              RunAppOnDevice(device_handle_, bundle_id));
  connection_handler_->RunAppOnDevice(hash_of_mac_address, bundle_id);
}

TEST_F(ConnectionHandlerTest, OnDeviceConnectionSwitching) {
  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  const transport_manager::DeviceInfo device_info_1(
      device_handle_, mac_address_, device_name_, connection_type_);

  connection_handler_->OnDeviceAdded(device_info_1);

  const auto second_mac_address = "second_mac_address";
  const transport_manager::DeviceInfo device_info_2(device_handle_ + 1,
                                                    second_mac_address,
                                                    "second_device_name",
                                                    "second_connection_type");

  connection_handler_->OnDeviceAdded(device_info_2);

  connection_handler::Device d1(device_info_1.device_handle(),
                                device_info_1.name(),
                                device_info_1.mac_address(),
                                device_info_1.connection_type());

  connection_handler::Device d2(device_info_2.device_handle(),
                                device_info_2.name(),
                                device_info_2.mac_address(),
                                device_info_2.connection_type());

  EXPECT_CALL(mock_connection_handler_observer,
              OnDeviceSwitchingStart(SameDevice(d1), SameDevice(d2)));

  connection_handler_->OnDeviceSwitchingStart(mac_address_, second_mac_address);

  EXPECT_CALL(mock_connection_handler_observer,
              OnDeviceSwitchingFinish(encryption::MakeHash(mac_address_)));
  connection_handler_->OnDeviceSwitchingFinish(mac_address_);

  EXPECT_CALL(
      mock_connection_handler_observer,
      OnDeviceSwitchingFinish(encryption::MakeHash(second_mac_address)));
  connection_handler_->OnDeviceSwitchingFinish(second_mac_address);
}

TEST_F(ConnectionHandlerTest, OnConnectionEstablishedForUnknownDevice) {
  const transport_manager::DeviceInfo device_info(
      device_handle_, mac_address_, device_name_, std::string("WIFI"));
  const transport_manager::ConnectionUID uid = 2u;

  connection_handler_->OnConnectionEstablished(device_info, uid);
  ASSERT_TRUE(connection_handler_->getDeviceList().empty());
  ASSERT_TRUE(connection_handler_->getConnectionList().empty());
}

TEST_F(ConnectionHandlerTest, StartStopSecondarySession) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  // Start new session with RPC service
  AddTestSession();

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  // Start a session on a secondary transport
  const transport_manager::DeviceInfo secondary_device_info(
      device_handle_, mac_address_, device_name_, std::string("WIFI"));
  const transport_manager::ConnectionUID secondary_uid = 2u;
  // Add Device and connection
  ON_CALL(mock_connection_handler_settings_, heart_beat_timeout())
      .WillByDefault(Return(1000u));
  connection_handler_->addDeviceConnection(secondary_device_info,
                                           secondary_uid);

  EXPECT_CALL(mock_connection_handler_observer,
              OnSecondaryTransportStartedCallback(device_handle_, _))
      .Times(1);

  connection_handler_->OnSecondaryTransportStarted(
      uid_, secondary_uid, out_context_.new_session_id_);

  SessionTransports st =
      connection_handler_->GetSessionTransports(out_context_.new_session_id_);
  EXPECT_EQ(st.primary_transport, uid_);
  EXPECT_EQ(st.secondary_transport, secondary_uid);

  AddSecondaryTestService(kAudio);
  AddSecondaryTestService(kMobileNav);

  CheckServiceExists(
      uid_, out_context_.new_session_id_, kAudio, secondary_uid, true);
  CheckServiceExists(
      uid_, out_context_.new_session_id_, kMobileNav, secondary_uid, true);

  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);
  EXPECT_CALL(mock_connection_handler_observer,
              OnSecondaryTransportEndedCallback(_))
      .Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(_, kAudio, _))
      .Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(_, kMobileNav, _))
      .Times(1);

  connection_handler_->OnSecondaryTransportEnded(uid_, secondary_uid);

  st = connection_handler_->GetSessionTransports(out_context_.new_session_id_);
  EXPECT_EQ(st.primary_transport, uid_);
  EXPECT_EQ(st.secondary_transport, 0u);

  CheckServiceExists(
      uid_, out_context_.new_session_id_, kAudio, secondary_uid, false);
  CheckServiceExists(
      uid_, out_context_.new_session_id_, kMobileNav, secondary_uid, false);
}

TEST_F(ConnectionHandlerTest, StopSecondarySession_NoService) {
  AddTestDeviceConnection();
  AddTestSession();

  connection_handler_test::MockConnectionHandlerObserver
      mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(
      &mock_connection_handler_observer);

  const transport_manager::DeviceInfo secondary_device_info(
      device_handle_, mac_address_, device_name_, std::string("WIFI"));
  const transport_manager::ConnectionUID secondary_uid = 123u;
  ON_CALL(mock_connection_handler_settings_, heart_beat_timeout())
      .WillByDefault(Return(1000u));
  connection_handler_->addDeviceConnection(secondary_device_info,
                                           secondary_uid);

  EXPECT_CALL(mock_connection_handler_observer,
              OnSecondaryTransportStartedCallback(device_handle_, _))
      .Times(1);
  connection_handler_->OnSecondaryTransportStarted(
      uid_, secondary_uid, out_context_.new_session_id_);

  // check if OnSecondaryTransportEndedCallback is triggered with correct
  // session ID even if we don't have any services
  EXPECT_CALL(mock_connection_handler_observer,
              OnSecondaryTransportEndedCallback(_));
  EXPECT_CALL(mock_connection_handler_observer, OnServiceEndedCallback(_, _, _))
      .Times(0);

  connection_handler_->OnSecondaryTransportEnded(uid_, secondary_uid);

  SessionTransports st =
      connection_handler_->GetSessionTransports(out_context_.new_session_id_);
  EXPECT_EQ(st.primary_transport, uid_);
  EXPECT_EQ(st.secondary_transport, 0u);
}

TEST_F(ConnectionHandlerTest, GetSessionTransports_InvalidSessionId) {
  uint8_t invalid_id = 0;
  SessionTransports st = connection_handler_->GetSessionTransports(invalid_id);
  EXPECT_EQ(st.primary_transport, 0u);
  EXPECT_EQ(st.secondary_transport, 0u);
}

TEST_F(ConnectionHandlerTest, ConnectionType_valid) {
  AddTestDeviceConnection();
  AddTestSession();

  std::string ret =
      connection_handler_->TransportTypeProfileStringFromConnHandle(uid_);
  EXPECT_EQ(connection_type_, ret);
}

TEST_F(ConnectionHandlerTest, ConnectionType_invalid) {
  AddTestDeviceConnection();

  transport_manager::ConnectionUID invalid_uid = 12345;
  ASSERT_TRUE(invalid_uid != uid_);
  std::string ret =
      connection_handler_->TransportTypeProfileStringFromConnHandle(
          invalid_uid);
  EXPECT_EQ(std::string(), ret);
}

TEST_F(ConnectionHandlerTest, SetSecondaryTransportID_UpdateSuccess) {
  uint8_t session_id = 123;
  transport_manager::ConnectionUID primary_uid = 100;
  transport_manager::ConnectionUID secondary_uid = 0;

  SessionConnectionMap& session_connection_map =
      connection_handler_->getSessionConnectionMap();
  // secondary transport's ID is 0
  SessionTransports st = {primary_uid, secondary_uid};
  session_connection_map[session_id] = st;

  secondary_uid = 200;
  st = connection_handler_->SetSecondaryTransportID(session_id, secondary_uid);
  EXPECT_EQ(primary_uid, st.primary_transport);
  EXPECT_EQ(secondary_uid, st.secondary_transport);
}

TEST_F(ConnectionHandlerTest, SetSecondaryTransportID_UpdateFailure) {
  uint8_t session_id = 123;
  transport_manager::ConnectionUID primary_uid = 100;
  transport_manager::ConnectionUID secondary_uid = 300;

  SessionConnectionMap& session_connection_map =
      connection_handler_->getSessionConnectionMap();
  // secondary transport's ID is already assigned
  SessionTransports st = {primary_uid, secondary_uid};
  session_connection_map[session_id] = st;

  st = connection_handler_->SetSecondaryTransportID(session_id, 500);
  EXPECT_EQ(primary_uid, st.primary_transport);
  // secondary transport's ID is NOT updated
  EXPECT_EQ(secondary_uid, st.secondary_transport);
}

TEST_F(ConnectionHandlerTest, SetSecondaryTransportID_OverwirteSecondaryUID) {
  uint8_t session_id = 123;
  transport_manager::ConnectionUID primary_uid = 200;
  transport_manager::ConnectionUID secondary_uid = 500;

  SessionConnectionMap& session_connection_map =
      connection_handler_->getSessionConnectionMap();
  SessionTransports st = {primary_uid, secondary_uid};
  session_connection_map[session_id] = st;

  secondary_uid = kDisabledSecondary;
  st = connection_handler_->SetSecondaryTransportID(session_id, secondary_uid);
  EXPECT_EQ(primary_uid, st.primary_transport);
  // secondary transport's ID is updated
  EXPECT_EQ(secondary_uid, st.secondary_transport);
}

TEST_F(ConnectionHandlerTest, SetSecondaryTransportID_Failure) {
  uint8_t session_id = 123;
  transport_manager::ConnectionUID primary_uid = 100;
  transport_manager::ConnectionUID secondary_uid = 0;

  SessionConnectionMap& session_connection_map =
      connection_handler_->getSessionConnectionMap();
  SessionTransports st = {primary_uid, secondary_uid};
  session_connection_map[session_id] = st;

  uint8_t invalid_session_id = 10;
  secondary_uid = 300;
  st = connection_handler_->SetSecondaryTransportID(invalid_session_id,
                                                    secondary_uid);
  EXPECT_EQ(0u, st.primary_transport);
  EXPECT_EQ(0u, st.secondary_transport);
}

TEST_F(ConnectionHandlerTest, CloseSession_TwoSessions_ConnectionOpened) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  AddTestSession();

  connection_handler::ConnectionList& connection_list =
      connection_handler_->getConnectionList();
  ASSERT_EQ(1u, connection_list.size());

  connection_handler::Connection* connection = connection_list.begin()->second;
  EXPECT_EQ(1u, connection->session_map().size());

  AddTestSession();
  EXPECT_EQ(2u, connection->session_map().size());

  connection_handler_->CloseSession(
      uid_, out_context_.new_session_id_, kCommon);

  EXPECT_EQ(1u, connection->session_map().size());
  EXPECT_EQ(1u, connection_list.size());
}

TEST_F(ConnectionHandlerTest, CloseSession_LastSession_ConnectionOpened) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  AddTestSession();

  connection_handler::ConnectionList& connection_list =
      connection_handler_->getConnectionList();
  ASSERT_EQ(1u, connection_list.size());

  connection_handler::Connection* connection = connection_list.begin()->second;
  EXPECT_EQ(1u, connection->session_map().size());

  connection_handler_->CloseSession(
      uid_, out_context_.new_session_id_, kCommon);

  EXPECT_EQ(0u, connection->session_map().size());
  EXPECT_EQ(1u, connection_list.size());
}
}  // namespace connection_handler_test
}  // namespace components
}  // namespace test
