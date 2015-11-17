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

#include <gtest/gtest.h>
#include <fstream>
#include <cstdint>
#include "connection_handler/connection_handler_impl.h"
#include "protocol/common.h"
#include "config_profile/profile.h"
// TODO(EZamakhov): move security test
#include "security_manager_mock.h"
#include "protocol_handler_mock.h"
#include "connection_handler_observer_mock.h"
#include "transport_manager_mock.h"

namespace test {
namespace components {
namespace connection_handle_test {

using namespace ::connection_handler;
using ::protocol_handler::ServiceType;
using namespace ::protocol_handler;
using ::testing::_;
using ::testing::InSequence;
// For service types and PROTECTION_ON/OFF

class ConnectionHandlerTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    connection_handler_ = ConnectionHandlerImpl::instance();
    uid_ = 1u;
    connection_key_ = connection_handler_->KeyFromPair(0, 0u);
  }
  void TearDown() OVERRIDE {
    ConnectionHandlerImpl::destroy();
  }
  // Additional SetUp
  void AddTestDeviceConnection() {
    device_handle_ = 0;

    connection_type_ = "BTMAC";
    device_name_ = "test_name";
    mac_address_ = "test_address";

    const transport_manager::DeviceInfo device_info(device_handle_,
                                                    mac_address_,
                                                    device_name_,
                                                    connection_type_);
    // Add Device and connection
    connection_handler_->addDeviceConnection(device_info, uid_);
    connection_key_ = connection_handler_->KeyFromPair(uid_, 0u);
    // Remove all specific services
    SetSpecificServices("", "");
  }
  void AddTestSession() {
    start_session_id_ = connection_handler_->OnSessionStartedCallback(
                       uid_, 0, kRpc, PROTECTION_OFF, &out_hash_id_);
    EXPECT_NE(0u, start_session_id_);
    EXPECT_EQ(SessionHash(uid_, start_session_id_), out_hash_id_);
    connection_key_ = connection_handler_->KeyFromPair(uid_, start_session_id_);
    CheckSessionExists(uid_, start_session_id_);
  }
  uint32_t SessionHash(const uint32_t connection, const uint32_t session) {
    return connection_handler_->KeyFromPair(connection, session);
  }
  void AddTestService(ServiceType service_type) {
    EXPECT_NE(0u, start_session_id_);
    EXPECT_EQ(SessionHash(uid_, start_session_id_), out_hash_id_);
    connection_key_ = connection_handler_->KeyFromPair(uid_, start_session_id_);
    CheckSessionExists(uid_, start_session_id_);
    uint32_t session_id = connection_handler_->OnSessionStartedCallback(
        uid_, start_session_id_, service_type, PROTECTION_OFF, 0);
    EXPECT_EQ(session_id, start_session_id_);
  }

  // Additional SetUp
  void SetSpecificServices(const std::string& protect, const std::string& not_protect) {
    const char* config_file = "config.ini";
    std::ofstream file_config(config_file);
    ASSERT_TRUE(file_config.is_open());
    const std::string non("NON");
    file_config << "[Security Manager]" << std::endl
        << "; Force protected services (could be id's from 0x01 to 0xFF)"
        << std::endl << "ForceProtectedService = "
        << (protect.empty() ? non : protect) << std::endl
        << "; Force unprotected services" << std::endl
        << "ForceUnprotectedService = "
        << (not_protect.empty() ? non : not_protect) << std::endl;
    file_config.close();
    profile::Profile::instance()->config_file_name(config_file);
  }
  // Check Service Wrapper
  // If session_id is NULL - check that there is no sessions in connection
  void CheckSessionExists(const int connectionId, const int session_id) {
    // Check all tree to find Session and check own protected value
    const ConnectionList& connection_list = connection_handler_->getConnectionList();
    ASSERT_FALSE(connection_list.empty());
    ConnectionList::const_iterator conn_it = connection_list.find(connectionId);
    ASSERT_NE(conn_it, connection_list.end());
    const Connection& connection = *connection_list.begin()->second;

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
      ASSERT_NE(service_list.end(), std::find(service_list.begin(), service_list.end(), kRpc));
      ASSERT_NE(service_list.end(), std::find(service_list.begin(), service_list.end(), kBulk));
    }
  }

  // Check Service Wrapper
  void CheckServiceExists(const int connectionId, const int session_id,
                          const ::protocol_handler::ServiceType serviceId,
                          const bool exists) {
    // Check all trees to find Service and check own protected value
    const ConnectionList& connection_list = connection_handler_->getConnectionList();
    ASSERT_FALSE(connection_list.empty());
    ConnectionList::const_iterator conn_it = connection_list.find(connectionId);
    ASSERT_NE(conn_it, connection_list.end());
    const Connection& connection = *connection_list.begin()->second;

    const SessionMap& session_map = connection.session_map();
    ASSERT_FALSE(session_map.empty());
    SessionMap::const_iterator sess_it = session_map.find(session_id);
    ASSERT_NE(sess_it, session_map.end());
    const Session& session = sess_it->second;
    const ServiceList& service_list = session.service_list;
    ASSERT_FALSE(service_list.empty());
    ServiceList::const_iterator serv_it = std::find(service_list.begin(), service_list.end(), serviceId);
    if (exists) {
      ASSERT_NE(serv_it, service_list.end());
    } else {
      ASSERT_EQ(serv_it, service_list.end());
    }
  }
  // Check Service Wrapper
  void CheckService(const int connectionId, const int session_id,
                    const ::protocol_handler::ServiceType serviceId,
                    const ::security_manager::SSLContext* ssl_context,
                    const bool is_protected) {
    // Check all tree to find Service and check own protected value
    const ConnectionList& connection_list = connection_handler_->getConnectionList();
    ASSERT_FALSE(connection_list.empty());
    ConnectionList::const_iterator conn_it = connection_list.find(connectionId);
    ASSERT_NE(conn_it, connection_list.end());
    const Connection& connection = *connection_list.begin()->second;

    const SessionMap& session_map = connection.session_map();
    ASSERT_FALSE(session_map.empty());
    SessionMap::const_iterator sess_it = session_map.find(session_id);
    ASSERT_NE(sess_it, session_map.end());
    const Session& session = sess_it->second;
#ifdef ENABLE_SECURITY
    ASSERT_EQ(session.ssl_context, ssl_context);
#endif  // ENABLE_SECURITY
    const ServiceList& service_list = session.service_list;
    ASSERT_FALSE(service_list.empty());
    ServiceList::const_iterator serv_it = std::find(service_list.begin(), service_list.end(), serviceId);
    ASSERT_NE(serv_it, service_list.end());

    const Service& service = *serv_it;
    EXPECT_EQ(PROTECTION_OFF, service.is_protected_);
#ifdef ENABLE_SECURITY
    if (is_protected) {
      // Emulate success protection - check enable service flag
      const uint32_t connection_key_ = connection_handler_->KeyFromPair(
          connectionId, session_id);
      connection_handler_->SetProtectionFlag(connection_key_, serviceId);
    }
#endif  // ENABLE_SECURITY
  }

  void ChangeProtocol(const int connectionId, const int session_id, const uint8_t protocol_version) {
    ConnectionList connection_list = connection_handler_->getConnectionList();

    ConnectionList::const_iterator conn_it = (connection_handler_->getConnectionList()).find(connectionId);
    ASSERT_NE(conn_it, connection_list.end());
    Connection * connection = conn_it->second;
    ASSERT_TRUE(connection != NULL);
    connection->UpdateProtocolVersionSession(session_id, protocol_version);
    uint8_t check_protocol_version;
    EXPECT_TRUE(connection->ProtocolVersion(session_id, check_protocol_version));
    EXPECT_EQ(check_protocol_version,protocol_version);

  }

  ConnectionHandlerImpl* connection_handler_;
  transport_manager::DeviceHandle device_handle_;
  transport_manager::ConnectionUID uid_;
  uint32_t connection_key_;
  uint32_t start_session_id_;
  uint32_t out_hash_id_;

  std::string connection_type_;
  std::string device_name_;
  std::string mac_address_;

};

TEST_F(ConnectionHandlerTest, StartSession_NoConnection) {
  // Null sessionId for start new session
  const uint8_t sessionID = 0;
  // Start new session with RPC service
  const uint32_t result_fail = connection_handler_->OnSessionStartedCallback(uid_, sessionID, kRpc, PROTECTION_ON, &out_hash_id_);
  // Unknown connection error is '0'
  EXPECT_EQ(0u, result_fail);
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, out_hash_id_);
  ASSERT_TRUE(connection_handler_->getConnectionList().empty());
}

TEST_F(ConnectionHandlerTest, StartSession) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  // Start new session with RPC service
  AddTestSession();
}

TEST_F(ConnectionHandlerTest, GetConnectionSessionsCount) {
  AddTestDeviceConnection();
  EXPECT_EQ(0u, connection_handler_->GetConnectionSessionsCount(connection_key_));

  AddTestSession();
  EXPECT_EQ(1u, connection_handler_->GetConnectionSessionsCount(connection_key_));
}

TEST_F(ConnectionHandlerTest, GetAppIdOnSessionKey) {
  AddTestDeviceConnection();
  AddTestSession();

  uint32_t app_id = 0;
  const uint32_t testid = SessionHash(uid_, start_session_id_);

  EXPECT_EQ(0, connection_handler_->GetDataOnSessionKey(connection_key_, &app_id, NULL, NULL));
  EXPECT_EQ(testid, app_id);
}

TEST_F(ConnectionHandlerTest,GetDeviceID) {
  AddTestDeviceConnection();
  AddTestSession();

  DeviceHandle test_handle;
  const DeviceMap & devmap = connection_handler_->getDeviceList();
  DeviceMap::const_iterator pos = devmap.find(device_handle_);
  ASSERT_NE(pos, devmap.end());
  const Device & devres = pos->second;
  std::string test_mac_address = devres.mac_address();

  EXPECT_TRUE(connection_handler_->GetDeviceID(test_mac_address, &test_handle));
  EXPECT_EQ(device_handle_, test_handle);
}

TEST_F(ConnectionHandlerTest,GetDeviceName) {
  AddTestDeviceConnection();
  AddTestSession();

  std::string test_device_name;
  DeviceHandle handle = 0;
  EXPECT_EQ(0, connection_handler_->GetDataOnDeviceID(handle, &test_device_name));
  EXPECT_EQ(device_name_, test_device_name);
}

TEST_F(ConnectionHandlerTest,GetConnectionType) {
  AddTestDeviceConnection();
  AddTestSession();

  const DeviceHandle handle = 0;
  std::string test_connection_type;
  EXPECT_EQ(0, connection_handler_->GetDataOnDeviceID(handle, NULL, NULL, NULL, &test_connection_type));
  EXPECT_EQ(connection_type_, test_connection_type);
}

TEST_F(ConnectionHandlerTest, GetDefaultProtocolVersion) {
  AddTestDeviceConnection();
  AddTestSession();

  uint8_t protocol_version = 0;
  EXPECT_TRUE(connection_handler_->ProtocolVersionUsed(uid_, start_session_id_, protocol_version));

  EXPECT_EQ(PROTOCOL_VERSION_2, protocol_version);
}

TEST_F(ConnectionHandlerTest, GetProtocolVersion) {
  AddTestDeviceConnection();
  AddTestSession();
  ChangeProtocol(uid_, start_session_id_, PROTOCOL_VERSION_3);

  uint8_t protocol_version = 0;
  EXPECT_TRUE(connection_handler_->ProtocolVersionUsed(uid_, start_session_id_, protocol_version));

  EXPECT_EQ(PROTOCOL_VERSION_3, protocol_version);
}

TEST_F(ConnectionHandlerTest, GetProtocolVersionAfterBinding) {
  AddTestDeviceConnection();
  AddTestSession();
  uint8_t protocol_version = 0;
  EXPECT_TRUE(connection_handler_->ProtocolVersionUsed(uid_, start_session_id_, protocol_version));
  EXPECT_EQ(PROTOCOL_VERSION_2, protocol_version);

  connection_handler_->BindProtocolVersionWithSession(connection_key_, PROTOCOL_VERSION_3);

  EXPECT_TRUE(connection_handler_->ProtocolVersionUsed(uid_, start_session_id_, protocol_version));
  EXPECT_EQ(PROTOCOL_VERSION_3, protocol_version);
}

TEST_F(ConnectionHandlerTest, GetPairFromKey) {
  AddTestDeviceConnection();
  AddTestSession();

  uint8_t session_id = 0;
  uint32_t test_uid = 0;
  connection_handler_->PairFromKey(connection_key_, &test_uid, &session_id);
  EXPECT_EQ(uid_, test_uid);
  EXPECT_EQ(start_session_id_, session_id);
}

TEST_F(ConnectionHandlerTest, IsHeartBeatSupported) {
  AddTestDeviceConnection();
  AddTestSession();

  ChangeProtocol(uid_, start_session_id_, PROTOCOL_VERSION_3);
  EXPECT_TRUE(connection_handler_->IsHeartBeatSupported(uid_, start_session_id_));
}

TEST_F(ConnectionHandlerTest,SendEndServiceWithoutSetProtocolHandler) {
  AddTestDeviceConnection();
  AddTestSession();
  protocol_handler_test::ProtocolHandlerMock mock_protocol_handler;

  EXPECT_CALL(mock_protocol_handler, SendEndService(_,_,kRpc)).Times(0);
  connection_handler_->SendEndService(connection_key_, kRpc);
}

TEST_F(ConnectionHandlerTest,SendEndService) {
  AddTestDeviceConnection();
  AddTestSession();
  protocol_handler_test::ProtocolHandlerMock mock_protocol_handler;

  connection_handler_->set_protocol_handler(&mock_protocol_handler);
  EXPECT_CALL(mock_protocol_handler, SendEndService(_,_,kRpc));
  connection_handler_->SendEndService(connection_key_, kRpc);
}

TEST_F(ConnectionHandlerTest,OnFindNewApplicationsRequest) {
  AddTestDeviceConnection();
  AddTestSession();
  connection_handler_test::ConnectionHandlerObserverMock mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(&mock_connection_handler_observer);

  EXPECT_CALL(mock_connection_handler_observer, OnFindNewApplicationsRequest());
  connection_handler_->OnFindNewApplicationsRequest();
}

TEST_F(ConnectionHandlerTest,OnFindNewApplicationsRequestWithoutObserver) {
  connection_handler_test::ConnectionHandlerObserverMock mock_connection_handler_observer;
  EXPECT_CALL(mock_connection_handler_observer, OnFindNewApplicationsRequest()).Times(0);
  connection_handler_->OnFindNewApplicationsRequest();
}

TEST_F(ConnectionHandlerTest,OnFindNewApplicationsRequestWithoutSession) {
  connection_handler_test::ConnectionHandlerObserverMock mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(&mock_connection_handler_observer);

  EXPECT_CALL(mock_connection_handler_observer, OnFindNewApplicationsRequest());
  connection_handler_->OnFindNewApplicationsRequest();
}

TEST_F(ConnectionHandlerTest, OnMalformedMessageCallback) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::ConnectionHandlerObserverMock mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(&mock_connection_handler_observer);

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kMobileNav, kMalformed)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kAudio, kMalformed)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kBulk, kMalformed)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kRpc, kMalformed)).Times(1);
  connection_handler_->OnMalformedMessageCallback(uid_);
}

TEST_F(ConnectionHandlerTest, OnApplicationFloodCallBack) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::ConnectionHandlerObserverMock mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(&mock_connection_handler_observer);

  protocol_handler_test::ProtocolHandlerMock mock_protocol_handler;
  connection_handler_->set_protocol_handler(&mock_protocol_handler);

  EXPECT_CALL(mock_protocol_handler, SendEndSession(uid_,start_session_id_));

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kMobileNav, kCommon)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kAudio, kCommon)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kBulk, kCommon)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kRpc, kCommon)).Times(1);
  connection_handler_->OnApplicationFloodCallBack(uid_);
}

TEST_F(ConnectionHandlerTest, StartDevicesDiscovery) {
  AddTestDeviceConnection();
  AddTestSession();
  transport_manager_test::TransportManagerMock mock_transport_manager;
  connection_handler_->set_transport_manager(&mock_transport_manager);
  connection_handler_test::ConnectionHandlerObserverMock mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(&mock_connection_handler_observer);

  EXPECT_CALL(mock_transport_manager, SearchDevices());
  EXPECT_CALL(mock_connection_handler_observer, OnDeviceListUpdated(_));
  connection_handler_->StartDevicesDiscovery();
}

TEST_F(ConnectionHandlerTest, StartTransportManager) {
  AddTestDeviceConnection();
  AddTestSession();
  transport_manager_test::TransportManagerMock mock_transport_manager;
  connection_handler_->set_transport_manager(&mock_transport_manager);
  EXPECT_CALL(mock_transport_manager, Visibility(true));
  connection_handler_->StartTransportManager();
}

TEST_F(ConnectionHandlerTest, CloseConnection) {
  AddTestDeviceConnection();
  AddTestSession();
  transport_manager_test::TransportManagerMock mock_transport_manager;
  connection_handler_->set_transport_manager(&mock_transport_manager);
  EXPECT_CALL(mock_transport_manager, DisconnectForce(uid_));
  connection_handler_->CloseConnection(uid_);
}

TEST_F(ConnectionHandlerTest, CloseRevokedConnection) {
  AddTestDeviceConnection();
  AddTestSession();
  transport_manager_test::TransportManagerMock mock_transport_manager;
  connection_handler_->set_transport_manager(&mock_transport_manager);
  EXPECT_CALL(mock_transport_manager, DisconnectForce(uid_));
  connection_handler_->CloseRevokedConnection(connection_key_);
}

TEST_F(ConnectionHandlerTest, CloseSessionWithCommonReason) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::ConnectionHandlerObserverMock mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(&mock_connection_handler_observer);

  protocol_handler_test::ProtocolHandlerMock mock_protocol_handler;
  connection_handler_->set_protocol_handler(&mock_protocol_handler);

  EXPECT_CALL(mock_protocol_handler, SendEndSession(uid_,start_session_id_));

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kMobileNav, kCommon)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kAudio, kCommon)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kBulk, kCommon)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kRpc, kCommon)).Times(1);
  connection_handler_->CloseSession(connection_key_, kCommon);
}

TEST_F(ConnectionHandlerTest, CloseSessionWithFloodReason) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::ConnectionHandlerObserverMock mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(&mock_connection_handler_observer);

  protocol_handler_test::ProtocolHandlerMock mock_protocol_handler;
  connection_handler_->set_protocol_handler(&mock_protocol_handler);

  EXPECT_CALL(mock_protocol_handler, SendEndSession(uid_,start_session_id_));

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kMobileNav, kFlood)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kAudio, kFlood)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kBulk, kFlood)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kRpc, kFlood)).Times(1);
  connection_handler_->CloseSession(connection_key_, kFlood);
}

TEST_F(ConnectionHandlerTest, CloseSessionWithMalformedMessage) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::ConnectionHandlerObserverMock mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(&mock_connection_handler_observer);

  protocol_handler_test::ProtocolHandlerMock mock_protocol_handler;
  connection_handler_->set_protocol_handler(&mock_protocol_handler);

  EXPECT_CALL(mock_protocol_handler, SendEndSession(uid_,start_session_id_)).Times(0);

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kMobileNav, kMalformed)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kAudio, kMalformed)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kBulk, kMalformed)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kRpc, kMalformed)).Times(1);
  connection_handler_->CloseSession(connection_key_, kMalformed);
}

TEST_F(ConnectionHandlerTest, CloseConnectionSessionsWithMalformedMessage) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::ConnectionHandlerObserverMock mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(&mock_connection_handler_observer);

  protocol_handler_test::ProtocolHandlerMock mock_protocol_handler;
  connection_handler_->set_protocol_handler(&mock_protocol_handler);

  EXPECT_CALL(mock_protocol_handler, SendEndSession(uid_,start_session_id_)).Times(0);

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kMobileNav, kMalformed)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kAudio, kMalformed)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kBulk, kMalformed)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kRpc, kMalformed)).Times(1);
  connection_handler_->CloseConnectionSessions(uid_, kMalformed);
}

TEST_F(ConnectionHandlerTest, CloseConnectionSessionsWithCommonReason) {
  AddTestDeviceConnection();
  AddTestSession();
  AddTestService(kAudio);
  AddTestService(kMobileNav);
  connection_handler_test::ConnectionHandlerObserverMock mock_connection_handler_observer;
  connection_handler_->set_connection_handler_observer(&mock_connection_handler_observer);

  protocol_handler_test::ProtocolHandlerMock mock_protocol_handler;
  connection_handler_->set_protocol_handler(&mock_protocol_handler);

  EXPECT_CALL(mock_protocol_handler, SendEndSession(uid_,start_session_id_));

  InSequence seq;
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kMobileNav, kCommon)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kAudio, kCommon)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kBulk, kCommon)).Times(1);
  EXPECT_CALL(mock_connection_handler_observer,
              OnServiceEndedCallback(connection_key_,kRpc, kCommon)).Times(1);
  connection_handler_->CloseConnectionSessions(uid_, kCommon);
}

TEST_F(ConnectionHandlerTest, StartService_withServices) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  AddTestSession();

  // Start Audio service
  const uint32_t start_audio = connection_handler_->OnSessionStartedCallback(
        uid_, start_session_id_, kAudio, PROTECTION_OFF, &out_hash_id_);
  EXPECT_EQ(start_session_id_, start_audio);
  CheckServiceExists(uid_, start_session_id_, kAudio, true);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, out_hash_id_);

  // Start Audio service
  const uint32_t start_video = connection_handler_->OnSessionStartedCallback(
      uid_, start_session_id_, kMobileNav, PROTECTION_OFF, &out_hash_id_);
  EXPECT_EQ(start_session_id_, start_video);
  CheckServiceExists(uid_, start_session_id_, kMobileNav, true);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, out_hash_id_);
}

TEST_F(ConnectionHandlerTest, ServiceStop_UnExistSession) {
  AddTestDeviceConnection();

  const uint32_t end_session_result = connection_handler_->OnSessionEndedCallback(
        uid_, 0u, 0u, kAudio);
  EXPECT_EQ(0u, end_session_result);
  CheckSessionExists(uid_, 0);
}

TEST_F(ConnectionHandlerTest, ServiceStop_UnExistService) {
  AddTestDeviceConnection();
  AddTestSession();
  const uint32_t end_session_result = connection_handler_->OnSessionEndedCallback(
        uid_, start_session_id_, 0u, kAudio);
  EXPECT_EQ(0u, end_session_result);
  CheckServiceExists(uid_, start_session_id_, kAudio, false);
}

TEST_F(ConnectionHandlerTest, ServiceStop) {
  AddTestDeviceConnection();
  AddTestSession();
  // Check ignoring hash_id on stop non-rpc service
  for (uint32_t some_hash_id = 0; some_hash_id < 0xFF; ++some_hash_id) {
    // Start audio service
    const uint32_t start_audio = connection_handler_->OnSessionStartedCallback(
          uid_, start_session_id_, kAudio, PROTECTION_OFF, &out_hash_id_);
    EXPECT_EQ(start_session_id_, start_audio);
    EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, out_hash_id_);

    const uint32_t end_session_result = connection_handler_->OnSessionEndedCallback(
          uid_, start_session_id_, some_hash_id, kAudio);
    EXPECT_EQ(connection_key_, end_session_result);
    CheckServiceExists(uid_, start_session_id_, kAudio, false);
  }
}

TEST_F(ConnectionHandlerTest, SessionStop_CheckHash) {
  AddTestDeviceConnection();
  for (uint32_t session = 0; session < 0xFF; ++session) {
    AddTestSession();

    const uint32_t hash = connection_key_;
    const uint32_t wrong_hash = hash + 1;

    const uint32_t end_audio_wrong_hash = connection_handler_->OnSessionEndedCallback(
          uid_, start_session_id_, wrong_hash, kRpc);
    EXPECT_EQ(0u, end_audio_wrong_hash);
    CheckSessionExists(uid_, start_session_id_);

    const uint32_t end_audio = connection_handler_->OnSessionEndedCallback(
          uid_, start_session_id_, hash, kRpc);
    EXPECT_EQ(connection_key_, end_audio);
    CheckSessionExists(uid_, 0);
  }
}

TEST_F(ConnectionHandlerTest, SessionStop_CheckSpecificHash) {
  AddTestDeviceConnection();
  for (uint32_t session = 0; session < 0xFF; ++session) {
    AddTestSession();

    const uint32_t wrong_hash = protocol_handler::HASH_ID_WRONG;
    const uint32_t hash = protocol_handler::HASH_ID_NOT_SUPPORTED;

    const uint32_t end_audio_wrong_hash = connection_handler_->OnSessionEndedCallback(
          uid_, start_session_id_, wrong_hash, kRpc);
    EXPECT_EQ(0u, end_audio_wrong_hash);
    CheckSessionExists(uid_, start_session_id_);

    const uint32_t end_audio = connection_handler_->OnSessionEndedCallback(
          uid_, start_session_id_, hash, kRpc);
    EXPECT_EQ(connection_key_, end_audio);
    CheckSessionExists(uid_, 0);
  }
}

TEST_F(ConnectionHandlerTest, SessionStarted_StartSession_SecureSpecific_Unprotect) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  // Forbid start kRPC without encryption
  SetSpecificServices("0x07", "");
  // Start new session with RPC service
  const uint32_t session_id_fail = connection_handler_->OnSessionStartedCallback(
        uid_, 0, kRpc, PROTECTION_OFF, &out_hash_id_);
#ifdef ENABLE_SECURITY
  EXPECT_EQ(0u, session_id_fail);
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, out_hash_id_);
#else
  EXPECT_EQ(1u, session_id_fail);
  EXPECT_EQ(SessionHash(uid_, session_id_fail), out_hash_id_);
#endif  // ENABLE_SECURITY

  // Allow start kRPC without encryption
  SetSpecificServices("0x00, Non", "");
  // Start new session with RPC service
  const uint32_t session_id = connection_handler_->OnSessionStartedCallback(
        uid_, 0, kRpc, PROTECTION_OFF, &out_hash_id_);
  EXPECT_NE(0u, session_id);
  CheckService(uid_, session_id, kRpc, NULL, PROTECTION_OFF);
  EXPECT_EQ(SessionHash(uid_, session_id), out_hash_id_);
}

TEST_F(ConnectionHandlerTest, SessionStarted_StartSession_SecureSpecific_Protect) {
  // Add virtual device and connection
  AddTestDeviceConnection();
  // Forbid start kRPC with encryption
  SetSpecificServices("", "0x06, 0x07, 0x08, Non");
  // Start new session with RPC service
  const uint32_t session_id_fail = connection_handler_->OnSessionStartedCallback(
        uid_, 0, kRpc, PROTECTION_ON, NULL);
#ifdef ENABLE_SECURITY
  EXPECT_EQ(0u, session_id_fail);
#else
  EXPECT_EQ(1u, session_id_fail);
#endif  // ENABLE_SECURITY

  // Allow start kRPC with encryption
  SetSpecificServices("", "0x00, 0x05, Non");
  // Start new session with RPC service
  const uint32_t session_id = connection_handler_->OnSessionStartedCallback(
        uid_, 0, kRpc, PROTECTION_ON, &out_hash_id_);
  EXPECT_NE(0u, session_id);
  EXPECT_EQ(SessionHash(uid_, session_id), out_hash_id_);

  // Protection steal FALSE because of APPlink Protocol implementation
  CheckService(uid_, session_id, kRpc, NULL, PROTECTION_OFF);
}

TEST_F(ConnectionHandlerTest, SessionStarted_StartService_SecureSpecific_Unprotect) {
  AddTestDeviceConnection();
  AddTestSession();

  // Audio is 0x0A
  ASSERT_EQ(0x0A, kAudio);

  // Forbid start kAudio without encryption
  SetSpecificServices("0x06, 0x0A, 0x08, Non", "");
  // Start new session with Audio service
  const uint32_t session_id2 = connection_handler_->OnSessionStartedCallback(
        uid_, start_session_id_, kAudio, PROTECTION_OFF, NULL);
#ifdef ENABLE_SECURITY
  EXPECT_EQ(0u, session_id2);
#else
  EXPECT_EQ(1u, session_id2);
#endif  // ENABLE_SECURITY
  // Allow start kAudio without encryption
  SetSpecificServices("0x06, 0x0B, 0x08, Non", "");
  const uint32_t session_id3 = connection_handler_->OnSessionStartedCallback(
        uid_, start_session_id_, kAudio, PROTECTION_OFF, &out_hash_id_);
  // Returned original session id
#ifdef ENABLE_SECURITY
  EXPECT_EQ(start_session_id_, session_id3);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, out_hash_id_);
  CheckService(uid_, session_id3, kRpc,
      NULL,
      PROTECTION_OFF);
#else
  EXPECT_EQ(0u, session_id3);
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, out_hash_id_);
#endif  // ENABLE_SECURITY
}

TEST_F(ConnectionHandlerTest, SessionStarted_StartService_SecureSpecific_Protect) {
  AddTestDeviceConnection();
  AddTestSession();

  // Audio is 0x0A
  ASSERT_EQ(0x0A, kAudio);
  // Forbid start kAudio with encryption
  SetSpecificServices("", "0x06, 0x0A, 0x08, Non");
  // Start new session with Audio service
  const uint32_t session_id_reject = connection_handler_->OnSessionStartedCallback(
        uid_, start_session_id_, kAudio, PROTECTION_ON, NULL);
#ifdef ENABLE_SECURITY
  EXPECT_EQ(0u, session_id_reject);
#else
  EXPECT_EQ(1u, session_id_reject);
#endif  // ENABLE_SECURITY
  // Allow start kAudio with encryption
  SetSpecificServices("", "Non");
  const uint32_t session_id3 = connection_handler_->OnSessionStartedCallback(
        uid_, start_session_id_, kAudio, PROTECTION_ON, &out_hash_id_);
  // Returned original session id
#ifdef ENABLE_SECURITY
  EXPECT_EQ(start_session_id_, session_id3);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, out_hash_id_);
  CheckService(uid_, session_id3, kAudio,
      NULL,
      PROTECTION_ON);
#else
  EXPECT_EQ(0u, session_id3);
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, out_hash_id_);
  CheckService(uid_, start_session_id_, kAudio, NULL, PROTECTION_OFF);
#endif  // ENABLE_SECURITY
}

TEST_F(ConnectionHandlerTest, SessionStarted_DealyProtect) {
  AddTestDeviceConnection();
  AddTestSession();

  // Start RPC protection
  const uint32_t session_id_new = connection_handler_->OnSessionStartedCallback(
        uid_, start_session_id_, kRpc, PROTECTION_ON, &out_hash_id_);
#ifdef ENABLE_SECURITY
  EXPECT_EQ(start_session_id_, session_id_new);
  // Post protection nedd no hash
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, out_hash_id_);
  CheckService(uid_, start_session_id_, kRpc,
      NULL,
      PROTECTION_ON);
#else
  EXPECT_EQ(0u, session_id_new);
  // Post protection nedd no hash
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, out_hash_id_);
  CheckService(uid_, start_session_id_, kRpc, NULL, PROTECTION_OFF);
#endif  // ENABLE_SECURITY

  // Start Audio session without protection
  const uint32_t session_id2 = connection_handler_->OnSessionStartedCallback(
        uid_, start_session_id_, kAudio, PROTECTION_OFF, &out_hash_id_);
  EXPECT_EQ(start_session_id_, session_id2);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, out_hash_id_);
  CheckService(uid_, start_session_id_, kAudio, NULL, PROTECTION_OFF);

  // Start Audio protection
  const uint32_t session_id3 = connection_handler_->OnSessionStartedCallback(
        uid_, start_session_id_, kAudio, PROTECTION_ON, &out_hash_id_);
#ifdef ENABLE_SECURITY
  EXPECT_EQ(start_session_id_, session_id3);
  EXPECT_EQ(protocol_handler::HASH_ID_NOT_SUPPORTED, out_hash_id_);
  CheckService(uid_, start_session_id_, kAudio,
      NULL,
      PROTECTION_ON);
#else
  EXPECT_EQ(0u, session_id3);
  EXPECT_EQ(protocol_handler::HASH_ID_WRONG, out_hash_id_);
  CheckService(uid_, start_session_id_, kAudio, NULL, PROTECTION_OFF);
#endif  // ENABLE_SECURITY
}

TEST_F(ConnectionHandlerTest, SessionStarted_DealyProtectBulk) {
  AddTestDeviceConnection();
  AddTestSession();

  const uint32_t session_id_new = connection_handler_->OnSessionStartedCallback(
        uid_, start_session_id_, kBulk, PROTECTION_ON, NULL);
#ifdef ENABLE_SECURITY
  EXPECT_EQ(start_session_id_, session_id_new);
  CheckService(uid_, start_session_id_, kRpc,
      NULL,
      PROTECTION_ON);
#else
  EXPECT_EQ(0u, session_id_new);
  CheckService(uid_, start_session_id_, kRpc, NULL, PROTECTION_OFF);
#endif  // ENABLE_SECURITY
}

#ifdef ENABLE_SECURITY
TEST_F(ConnectionHandlerTest, SetSSLContext_Null) {
  // No SSLContext on start up
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
      reinterpret_cast<security_manager::SSLContext *>(NULL));
  EXPECT_EQ(::security_manager::SecurityManager::ERROR_INTERNAL,
      connection_handler_->SetSSLContext(connection_key_, NULL));
  // No SSLContext after error
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
      reinterpret_cast<security_manager::SSLContext *>(NULL));

  AddTestDeviceConnection();
  EXPECT_EQ(::security_manager::SecurityManager::ERROR_INTERNAL,
      connection_handler_->SetSSLContext(connection_key_, NULL));
  // No SSLContext after error
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
      reinterpret_cast<security_manager::SSLContext *>(NULL));

  AddTestSession();
  EXPECT_EQ(::security_manager::SecurityManager::ERROR_SUCCESS,
      connection_handler_->SetSSLContext(connection_key_, NULL));
  // NULL SSLContext after success
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
      reinterpret_cast<security_manager::SSLContext *>(NULL));
}
TEST_F(ConnectionHandlerTest, SetSSLContext) {
  // No SSLContext on start up
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
      reinterpret_cast<security_manager::SSLContext *>(NULL));

  testing::StrictMock<security_manager_test::SSLContextMock> mock_ssl_context;
  // Error on no connection
  EXPECT_EQ(
      connection_handler_->SetSSLContext(connection_key_, &mock_ssl_context),
      ::security_manager::SecurityManager::ERROR_INTERNAL);
  // No SSLContext after error
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
      reinterpret_cast<security_manager::SSLContext *>(NULL));

  AddTestDeviceConnection();
  // Error on no session
  EXPECT_EQ(
      connection_handler_->SetSSLContext(connection_key_, &mock_ssl_context),
      ::security_manager::SecurityManager::ERROR_INTERNAL);
  // No SSLContext after error
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
      reinterpret_cast<security_manager::SSLContext *>(NULL));
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
      reinterpret_cast<security_manager::SSLContext *>(NULL));
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kBulk),
      reinterpret_cast<security_manager::SSLContext *>(NULL));
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kAudio),
      reinterpret_cast<security_manager::SSLContext *>(NULL));
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kMobileNav),
      reinterpret_cast<security_manager::SSLContext *>(NULL));
}

TEST_F(ConnectionHandlerTest, GetSSLContext_ByProtectedService) {
  // No SSLContext on start up
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
      reinterpret_cast<security_manager::SSLContext *>(NULL));

  testing::StrictMock<security_manager_test::SSLContextMock> mock_ssl_context;
  AddTestDeviceConnection();
  AddTestSession();
  EXPECT_EQ(
      connection_handler_->SetSSLContext(connection_key_, &mock_ssl_context),
      ::security_manager::SecurityManager::ERROR_SUCCESS);
  // kControl service mean - return for all connection
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
      &mock_ssl_context);

  // kAudio is not exists yet
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kAudio),
      reinterpret_cast<security_manager::SSLContext *>(NULL));
  // Open kAudio service
  const uint32_t session_id = connection_handler_->OnSessionStartedCallback(
      uid_, start_session_id_, kAudio, PROTECTION_ON, NULL);
  EXPECT_EQ(session_id, start_session_id_);
  CheckService(uid_, session_id, kAudio, &mock_ssl_context, PROTECTION_ON);

  // kAudio is not exists yet
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kAudio),
      &mock_ssl_context);
}
TEST_F(ConnectionHandlerTest, GetSSLContext_ByDealyProtectedRPC) {
  testing::StrictMock<security_manager_test::SSLContextMock> mock_ssl_context;
  AddTestDeviceConnection();
  AddTestSession();
  EXPECT_EQ(
      connection_handler_->SetSSLContext(connection_key_, &mock_ssl_context),
      ::security_manager::SecurityManager::ERROR_SUCCESS);
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
      &mock_ssl_context);

  // kRpc is not protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kRpc),
      reinterpret_cast<security_manager::SSLContext *>(NULL));

  // Protect kRpc (Bulk will be protect also)
  const uint32_t session_id = connection_handler_->OnSessionStartedCallback(
      uid_, start_session_id_, kRpc, PROTECTION_ON, NULL);
  EXPECT_EQ(start_session_id_, session_id);
  CheckService(uid_, session_id, kRpc, &mock_ssl_context, PROTECTION_ON);

  // kRpc is protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kRpc),
      &mock_ssl_context);
  // kBulk is protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kBulk),
      &mock_ssl_context);
}
TEST_F(ConnectionHandlerTest, GetSSLContext_ByDealyProtectedBulk) {
  testing::StrictMock<security_manager_test::SSLContextMock> mock_ssl_context;
  AddTestDeviceConnection();
  AddTestSession();
  EXPECT_EQ(
      connection_handler_->SetSSLContext(connection_key_, &mock_ssl_context),
      ::security_manager::SecurityManager::ERROR_SUCCESS);
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kControl),
      &mock_ssl_context);

  // kRpc is not protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kRpc),
      reinterpret_cast<security_manager::SSLContext *>(NULL));

  // Protect Bulk (kRpc will be protected also)
  const uint32_t session_id = connection_handler_->OnSessionStartedCallback(
      uid_, start_session_id_, kBulk, PROTECTION_ON, NULL);
  EXPECT_EQ(start_session_id_, session_id);
  CheckService(uid_, session_id, kRpc, &mock_ssl_context, PROTECTION_ON);

  // kRpc is protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kRpc),
      &mock_ssl_context);
  // kBulk is protected
  EXPECT_EQ(connection_handler_->GetSSLContext(connection_key_, kBulk),
      &mock_ssl_context);
}
#endif  // ENABLE_SECURITY

}  // namespace connection_handle_test
}  // namespace components
}  // namespace test
