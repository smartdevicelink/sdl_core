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

#ifndef CONNECTION_HANDLER_IMPL_TEST_H
#define CONNECTION_HANDLER_IMPL_TEST_H

#include <gtest/gtest.h>
#include <fstream>
#include "connection_handler/connection_handler_impl.h"
#include "config_profile/profile.h"

using namespace connection_handler;

namespace test {
namespace components {
namespace connection_handle_test {

class ConnectionHandlerTest: public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    connection_handler_ = ConnectionHandlerImpl::instance();
    uid = 0;
  }
  void TearDown() OVERRIDE {
    ConnectionHandlerImpl::destroy();
  }
  //Additional SetUp
  void AddTestDeviceConnection() {
    const transport_manager::DeviceHandle device_handle = 0;
    const transport_manager::DeviceInfo device_info(
          device_handle, std::string("test_address"), std::string("test_name"));
    //Add Device and connection
    connection_handler_->addDeviceConnection(device_info, uid);
    //Remove all specifis services
    SetSpecificServices("", "");
  }
  //Additional SetUp
  void SetSpecificServices(const std::string& protect,
                           const std::string& not_protect) {
    const char* config_file = "config.ini";
    std::ofstream file_config(config_file);
    ASSERT_TRUE(file_config.is_open());
    const std::string non("NON");
    file_config
        << "[Security Manager]" << std::endl
        << "; Force protected services (could be id's from 0x01 to 0xFF)"
        << std::endl
        << "ForceProtectedService = "   << (protect.empty() ? non : protect) << std::endl
        << "; Force unprotected services" << std::endl
        << "ForceUnprotectedService = " << (not_protect.empty() ? non : not_protect)
        << std::endl;
    file_config.close();
    profile::Profile::instance()->config_file_name(config_file);
  }
  //Check Service Wrapper
  void CheckService(const int connectionId, const int session_id,
                    const protocol_handler::ServiceType service,
                    const security_manager::SSLContext* ssl_context,
                    const bool is_protected) {
    //check all tree to find Service and check own protected value
    const ConnectionList& connection_list = connection_handler_->getConnectionList();
    ASSERT_FALSE(connection_list.empty());
    ConnectionList::const_iterator conn_it = connection_list.find(connectionId);
    ASSERT_NE(conn_it, connection_list.end());
    const Connection& connection = *connection_list.begin()->second;

    const SessionMap& session_map = connection.session_map();
    ASSERT_FALSE(session_map.empty());
    // Hint: session id handled in connection with +1
    SessionMap::const_iterator sess_it = session_map.find(session_id + 1);
    ASSERT_NE(sess_it, session_map.end());
    const Session& session = sess_it->second;

    ASSERT_EQ(session.ssl_context, ssl_context);

    const ServiceList& service_list = session.service_list;
    ASSERT_FALSE(service_list.empty());
    ServiceList::const_iterator serv_it =
        std::find(service_list.begin(), service_list.end(), service);
    ASSERT_NE(serv_it, service_list.end());

    const Service& service = *serv_it;
    EXPECT_EQ(service.is_protected_, is_protected);
  }

  ConnectionHandlerImpl* connection_handler_;
  transport_manager::ConnectionUID uid;
};

TEST_F(ConnectionHandlerTest, SessionStarted_Fial_NoConnection) {
  //null sessionId for start new session
  const uint8_t sessionID = 0;
  //start new session with RPC service
  const int32_t result_fail =
      connection_handler_->OnSessionStartedCallback(uid, sessionID, protocol_handler::kRpc, true);
  //Unknown connection error is '-1'
  EXPECT_EQ(result_fail, -1);
  EXPECT_TRUE(connection_handler_->getConnectionList().empty());
}

TEST_F(ConnectionHandlerTest, SessionStarted_RPC) {
  //null sessionId for start new session
  const uint8_t sessionID = 0;

  //Add virtual device and connection
  AddTestDeviceConnection();

  //start new session with RPC service
  const int32_t session_id_on_rpc_secure =
      connection_handler_->OnSessionStartedCallback(uid, sessionID, protocol_handler::kRpc,
                                                    false);
  EXPECT_NE(session_id_on_rpc_secure, -1);
  const ConnectionList& connection_list = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list.empty());
  const Connection* const connection = connection_list.begin()->second;
  EXPECT_NE(connection, static_cast<Connection*>(NULL));
  const SessionMap sessionMap = connection->session_map();
  EXPECT_EQ(sessionMap.size(), 1);
  EXPECT_EQ(sessionMap.begin()->first, session_id_on_rpc_secure);
  const ServiceList serviceList = sessionMap.begin()->second.service_list;
  const ServiceList::const_iterator it =
      std::find(serviceList.begin(), serviceList.end(), protocol_handler::kRpc);
  EXPECT_NE(it, serviceList.end());
}

TEST_F(ConnectionHandlerTest, SessionStarted_Audio) {
  //null sessionId for start new session
  const uint8_t start_session_id = 0;

  //Add virtual device and connection
  AddTestDeviceConnection();

  //start new session with RPC service
  const int32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, start_session_id,
                                                    protocol_handler::kRpc,
                                                    false);
  EXPECT_NE(session_id, -1);
  const ConnectionList& connection_list = connection_handler_->getConnectionList();
  EXPECT_EQ(connection_list.size(), 1);
  EXPECT_EQ(connection_list.begin()->first, 0);

  //start new session with RPC service
  const int32_t session_id_on_start_secure =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kAudio,
                                                    false);
  EXPECT_EQ(session_id_on_start_secure, session_id);
  ConnectionList& connection_list_new = connection_handler_->getConnectionList();
  EXPECT_EQ(connection_list.size(), 1);
  EXPECT_EQ(connection_list_new.begin()->first, 0);
}

TEST_F(ConnectionHandlerTest, SessionEnded_Audio) {
  //null sessionId for start new session
  const uint8_t start_session_id = 0;

  //Add virtual device and connection
  AddTestDeviceConnection();

  //start new session with RPC service
  const int32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, start_session_id,
                                                    protocol_handler::kRpc,
                                                    false);
  EXPECT_NE(session_id, -1);
  const ConnectionList& connection_list = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list.empty());
  EXPECT_EQ(connection_list.begin()->first, 0);

  //start new session with RPC service
  const int32_t session_id_on_start_secure =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kAudio,
                                                    false);
  EXPECT_EQ(session_id_on_start_secure, session_id);
  ConnectionList& connection_list_start_secure = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list_start_secure.empty());
  EXPECT_EQ(connection_list_start_secure.begin()->first, 0);

  //start new session with RPC service
  const int32_t session_id_on_end_secure =
      connection_handler_->OnSessionEndedCallback(uid, session_id, 0,
                                                  protocol_handler::kAudio);
  EXPECT_EQ(session_id_on_end_secure, session_id);
  ConnectionList& connection_list_on_end_secure = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list_on_end_secure.empty());
  EXPECT_EQ(connection_list_on_end_secure.begin()->first, 0);
}
// FIXME (EZamakhov) Add test with create secured services and create kContorl Service

TEST_F(ConnectionHandlerTest, SessionStarted_SecureSpecific_StartSessionProtect) {
  //Add virtual device and connection
  AddTestDeviceConnection();

  //Forbid start kRPC without encryption
  SetSpecificServices("0x07", "");
  //null sessionId for start new session
  const uint8_t start_session_id = 0;
  //start new session with RPC service
  const int32_t session_id_fail =
      connection_handler_->OnSessionStartedCallback(uid, start_session_id,
                                                    protocol_handler::kRpc,
                                                    false);
  EXPECT_EQ(session_id_fail, -1);

  //Allow start kRPC without encryption
  SetSpecificServices("Non", "");
  //null sessionId for start new session
  //start new session with RPC service
  const int32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, start_session_id,
                                                    protocol_handler::kRpc,
                                                    false);
  EXPECT_NE(session_id, -1);
  CheckService(uid, start_session_id, protocol_handler::kRpc, NULL, false);
}
TEST_F(ConnectionHandlerTest, SessionStarted_SecureSpecific_StartSessionUnprotect) {
  //Add virtual device and connection
  AddTestDeviceConnection();

  //Forbid start kRPC with encryption
  SetSpecificServices("", "0x07");
  //null sessionId for start new session
  const uint8_t start_session_id = 0;
  //start new session with RPC service
  const int32_t session_id_fail =
      connection_handler_->OnSessionStartedCallback(uid, start_session_id,
                                                    protocol_handler::kRpc,
                                                    true);
  EXPECT_EQ(session_id_fail, -1);

  //Allow start kRPC with encryption
  SetSpecificServices("Non", "");
  //null sessionId for start new session
  //start new session with RPC service
  const int32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, start_session_id,
                                                    protocol_handler::kRpc,
                                                    true);
  EXPECT_NE(session_id, -1);

  //Protection steal FALSE because of APPlink Protocol implementation
  CheckService(uid, start_session_id, protocol_handler::kRpc, NULL, false);
}

} // connection_handle_test
} // namespace components
} // namespace test
#endif // CONNECTION_HANDLER_IMPL_TEST_H
