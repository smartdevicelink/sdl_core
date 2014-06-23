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

#ifndef CONNECTION_HANDLER_IMPL_TEST_H
#define CONNECTION_HANDLER_IMPL_TEST_H

#include <gtest/gtest.h>
#include <fstream>
#include "connection_handler/connection_handler_impl.h"
#include "protocol_handler/protocol_packet.h"
#include "config_profile/profile.h"
#include "security_manager/security_query.h"
#include "security_manager/security_manager_mock.h"
#include "security_manager/security_manager_mock.h"

namespace test {
namespace components {
namespace connection_handle_test {
  using namespace connection_handler;
  using namespace protocol_handler;

class ConnectionHandlerTest: public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    connection_handler_ = ConnectionHandlerImpl::instance();
    uid = 1;
    connection_key = connection_handler_->KeyFromPair(0, 0u);
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
    connection_key = connection_handler_->KeyFromPair(uid, 0u);
    //Remove all specifis services
    SetSpecificServices("", "");
  }
  void AddTestSession() {
    start_session_id =
        connection_handler_->OnSessionStartedCallback(uid, 0,
                                                      protocol_handler::kRpc,
                                                      PROTECTION_OFF);
    EXPECT_NE(start_session_id, 0u);
    connection_key = connection_handler_->KeyFromPair(uid,
                                                      start_session_id);
    CheckService(uid, start_session_id, protocol_handler::kRpc, NULL, PROTECTION_OFF);
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
                    const protocol_handler::ServiceType serviceId,
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
    EXPECT_EQ(service.is_protected_, is_protected);
  }

  ConnectionHandlerImpl* connection_handler_;
  transport_manager::ConnectionUID uid;
  uint32_t connection_key;
  uint32_t start_session_id;
};

TEST_F(ConnectionHandlerTest, SessionStarted_Fial_NoConnection) {
  //null sessionId for start new session
  const uint8_t sessionID = 0;
  //start new session with RPC service
  const uint32_t result_fail =
      connection_handler_->OnSessionStartedCallback(uid, sessionID,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_ON);
  //Unknown connection error is '0'
  EXPECT_EQ(result_fail, 0u);
  EXPECT_TRUE(connection_handler_->getConnectionList().empty());
}

TEST_F(ConnectionHandlerTest, SessionStarted_RPC) {
  //Add virtual device and connection
  AddTestDeviceConnection();

  //start new session with RPC service
  const uint32_t session_id_on_rpc_secure =
      connection_handler_->OnSessionStartedCallback(uid, 0,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_OFF);
  EXPECT_NE(session_id_on_rpc_secure, 0u);
  const ConnectionList& connection_list = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list.empty());
  const Connection* const connection = connection_list.begin()->second;
  EXPECT_NE(connection, static_cast<Connection*>(NULL));
  const SessionMap sessionMap = connection->session_map();
  EXPECT_EQ(sessionMap.size(), 1u);
  EXPECT_EQ(sessionMap.begin()->first, session_id_on_rpc_secure);
  const ServiceList serviceList = sessionMap.begin()->second.service_list;
  const ServiceList::const_iterator it =
      std::find(serviceList.begin(), serviceList.end(), protocol_handler::kRpc);
  EXPECT_NE(it, serviceList.end());
}

TEST_F(ConnectionHandlerTest, SessionStarted_Audio) {
  //Add virtual device and connection
  AddTestDeviceConnection();

  //start new session with RPC service
  const uint32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, 0,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_OFF);
  EXPECT_NE(session_id, 0u);
  const ConnectionList& connection_list = connection_handler_->getConnectionList();
  EXPECT_EQ(connection_list.size(), 1u);
  EXPECT_EQ(connection_list.begin()->first, uid);

  //start new session with Audio service
  const uint32_t session_id_on_start_secure =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kAudio,
                                                    PROTECTION_OFF);
  EXPECT_EQ(session_id_on_start_secure, session_id);
  ConnectionList& connection_list_new = connection_handler_->getConnectionList();
  EXPECT_EQ(connection_list.size(), 1u);
  EXPECT_EQ(connection_list_new.begin()->first, uid);
}

TEST_F(ConnectionHandlerTest, SessionEnded_Audio) {
  //Add virtual device and connection
  AddTestDeviceConnection();

  //start new session with RPC service
  const uint32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, 0,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_OFF);
  EXPECT_NE(session_id, 0u);
  const ConnectionList& connection_list = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list.empty());
  EXPECT_EQ(connection_list.begin()->first, uid);

  //start new session with RPC service
  const uint32_t session_id_on_start_secure =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kAudio,
                                                    PROTECTION_OFF);
  EXPECT_EQ(session_id_on_start_secure, session_id);
  ConnectionList& connection_list_start_secure = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list_start_secure.empty());
  EXPECT_EQ(connection_list_start_secure.begin()->first, uid);

  //start new session with RPC service
  const uint32_t session_key_on_end_secure =
      connection_handler_->OnSessionEndedCallback(uid, session_id, 0,
                                                  protocol_handler::kAudio);
  transport_manager::ConnectionUID connection_id_on_end_secure;
  uint8_t session_id_on_end_secure = 0;
  connection_handler_->PairFromKey(session_key_on_end_secure,
                                   &connection_id_on_end_secure, &session_id_on_end_secure);
  EXPECT_EQ(session_id_on_end_secure, session_id);
  ConnectionList& connection_list_on_end_secure = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list_on_end_secure.empty());
  EXPECT_EQ(connection_list_on_end_secure.begin()->first, uid);
}

TEST_F(ConnectionHandlerTest, SessionStarted_StartSession_SecureSpecific_Unprotect) {
  //Add virtual device and connection
  AddTestDeviceConnection();

  //Forbid start kRPC without encryption
  SetSpecificServices("0x07", "");
  //start new session with RPC service
  const uint32_t session_id_fail =
      connection_handler_->OnSessionStartedCallback(uid, 0,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_OFF);
  EXPECT_EQ(session_id_fail, 0u);

  //Allow start kRPC without encryption
  SetSpecificServices("0x00, Non", "");
  //start new session with RPC service
  const uint32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, 0,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_OFF);
  EXPECT_NE(session_id, 0u);
  CheckService(uid, session_id, protocol_handler::kRpc, NULL, PROTECTION_OFF);
}
TEST_F(ConnectionHandlerTest, SessionStarted_StartSession_SecureSpecific_Protect) {
  //Add virtual device and connection
  AddTestDeviceConnection();

  //Forbid start kRPC with encryption
  SetSpecificServices("", "0x06, 0x07, 0x08, Non");
  //start new session with RPC service
  const uint32_t session_id_fail =
      connection_handler_->OnSessionStartedCallback(uid, 0,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_ON);
  EXPECT_EQ(session_id_fail, 0u);

  //Allow start kRPC with encryption
  SetSpecificServices("", "0x00, 0x05, Non");
  //start new session with RPC service
  const uint32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, 0,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_ON);
  EXPECT_NE(session_id, 0u);

  //Protection steal FALSE because of APPlink Protocol implementation
  CheckService(uid, session_id, protocol_handler::kRpc, NULL, PROTECTION_OFF);
  // TODO(EZamakhov)add test - protect kRPC
}
TEST_F(ConnectionHandlerTest, SessionStarted_StartService_SecureSpecific_Unprotect) {
  AddTestDeviceConnection();

  ASSERT_EQ(0x0A, protocol_handler::kAudio);
  const uint32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, 0,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_OFF);
  EXPECT_NE(session_id, 0u);
  CheckService(uid, session_id, protocol_handler::kRpc, NULL, PROTECTION_OFF);

  //Audio is 0x0A
  ASSERT_EQ(0x0A, protocol_handler::kAudio);

  //Forbid start kAudio without encryption
  SetSpecificServices("0x06, 0x0A, 0x08, Non", "");
  //start new session with Audio service
  const uint32_t session_id2 =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kAudio,
                                                    PROTECTION_OFF);
  EXPECT_EQ(session_id2, 0u);

  //Allow start kAudio without encryption
  SetSpecificServices("0x06, 0x0B, 0x08, Non", "");
  const uint32_t session_id3 =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kAudio,
                                                    PROTECTION_OFF);
  EXPECT_NE(session_id3, 0u);
  EXPECT_EQ(session_id3, session_id);
  CheckService(uid, session_id3, protocol_handler::kRpc, NULL, PROTECTION_OFF);
}
TEST_F(ConnectionHandlerTest, SessionStarted_StartService_SecureSpecific_Protect) {
  AddTestDeviceConnection();

  ASSERT_EQ(0x0A, protocol_handler::kAudio);
  const uint32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, 0,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_OFF);
  EXPECT_NE(session_id, 0u);
  CheckService(uid, session_id, protocol_handler::kRpc, NULL, PROTECTION_OFF);

  //Audio is 0x0A
  ASSERT_EQ(0x0A, protocol_handler::kAudio);

  //Forbid start kAudio with encryption
  SetSpecificServices("", "0x06, 0x0A, 0x08, Non");
  //start new session with Audio service
  const uint32_t session_id2 =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kAudio,
                                                    PROTECTION_ON);
  EXPECT_EQ(session_id2, 0u);

  //Allow start kAudio without encryption
  SetSpecificServices("", "Non");
  const uint32_t session_id3 =
      connection_handler_->OnSessionStartedCallback( uid, session_id,
                                                     protocol_handler::kAudio,
                                                     PROTECTION_ON);
  EXPECT_NE(session_id3, 0u);
  EXPECT_EQ(session_id3, session_id);
  CheckService(uid, session_id3, protocol_handler::kAudio, NULL, PROTECTION_ON);
}
TEST_F(ConnectionHandlerTest, SessionStarted_DealyProtect) {
  AddTestDeviceConnection();

  // Start RPC session without protection
  const uint32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, 0,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_OFF);
  EXPECT_NE(session_id, 0u);
  CheckService(uid, session_id, protocol_handler::kRpc, NULL, PROTECTION_OFF);

  // Start RPC protection
  const uint32_t session_id_new =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_ON);
  EXPECT_EQ(session_id_new, session_id);
  CheckService(uid, session_id, protocol_handler::kRpc, NULL, PROTECTION_ON);

  // Start Audio session without protection
  const uint32_t session_id2 =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kAudio,
                                                    PROTECTION_OFF);
  EXPECT_EQ(session_id2, session_id);
  CheckService(uid, session_id, protocol_handler::kAudio, NULL, PROTECTION_OFF);

  // Start Audio protection
  const uint32_t session_id3 =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kAudio,
                                                    PROTECTION_ON);
  EXPECT_EQ(session_id3, session_id);
  CheckService(uid, session_id, protocol_handler::kAudio, NULL, PROTECTION_ON);
}
TEST_F(ConnectionHandlerTest, SessionStarted_DealyProtectBulk) {
  AddTestDeviceConnection();

  const uint32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, 0,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_OFF);
  EXPECT_NE(session_id, 0u);
  CheckService(uid, session_id, protocol_handler::kRpc, NULL, PROTECTION_OFF);

  const uint32_t session_id_new =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kBulk,
                                                    PROTECTION_ON);
  EXPECT_EQ(session_id_new, session_id);
  CheckService(uid, session_id, protocol_handler::kRpc, NULL, PROTECTION_ON);
}
TEST_F(ConnectionHandlerTest, SetSSLContext_Null) {
  //No SSLContext on start up
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             reinterpret_cast<security_manager::SSLContext *>(NULL));
  EXPECT_EQ (::security_manager::SecurityQuery::ERROR_INTERNAL,
             connection_handler_->SetSSLContext(connection_key, NULL));
  //No SSLContext after error
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             reinterpret_cast<security_manager::SSLContext *>(NULL));

  AddTestDeviceConnection();
  EXPECT_EQ (::security_manager::SecurityQuery::ERROR_INTERNAL,
             connection_handler_->SetSSLContext(connection_key, NULL));
  //No SSLContext after error
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             reinterpret_cast<security_manager::SSLContext *>(NULL));

  AddTestSession();
  EXPECT_EQ (::security_manager::SecurityQuery::ERROR_SUCCESS,
             connection_handler_->SetSSLContext(connection_key, NULL));
  //NULL SSLContext after success
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             reinterpret_cast<security_manager::SSLContext *>(NULL));
}
TEST_F(ConnectionHandlerTest, SetSSLContext) {
  //No SSLContext on start up
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             reinterpret_cast<security_manager::SSLContext *>(NULL));

  testing::StrictMock<security_manager_test::SSLContextMock> mock_ssl_context;
  //Error on no connection
  EXPECT_EQ (connection_handler_->SetSSLContext(connection_key, &mock_ssl_context),
             ::security_manager::SecurityQuery::ERROR_INTERNAL);
  //No SSLContext after error
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             reinterpret_cast<security_manager::SSLContext *>(NULL));

  AddTestDeviceConnection();
  //Error on no session
  EXPECT_EQ (connection_handler_->SetSSLContext(connection_key, &mock_ssl_context),
             ::security_manager::SecurityQuery::ERROR_INTERNAL);
  //No SSLContext after error
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             reinterpret_cast<security_manager::SSLContext *>(NULL));
  AddTestSession();
  //success
  EXPECT_EQ (connection_handler_->SetSSLContext(connection_key, &mock_ssl_context),
             ::security_manager::SecurityQuery::ERROR_SUCCESS);
  //SSLContext set on Success
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             &mock_ssl_context);
  //Null SSLContext for unprotected services
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kRpc),
             reinterpret_cast<security_manager::SSLContext *>(NULL));
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kBulk),
             reinterpret_cast<security_manager::SSLContext *>(NULL));
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kAudio),
             reinterpret_cast<security_manager::SSLContext *>(NULL));
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kMobileNav),
             reinterpret_cast<security_manager::SSLContext *>(NULL));
}
TEST_F(ConnectionHandlerTest, GetSSLContext_ByProtectedService) {
  //No SSLContext on start up
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             reinterpret_cast<security_manager::SSLContext *>(NULL));

  testing::StrictMock<security_manager_test::SSLContextMock> mock_ssl_context;
  AddTestDeviceConnection();
  AddTestSession();
  EXPECT_EQ (connection_handler_->SetSSLContext(connection_key, &mock_ssl_context),
             ::security_manager::SecurityQuery::ERROR_SUCCESS);
  //kControl service mean - return for all connection
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             &mock_ssl_context);

  //kAudio is not exists yet
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kAudio),
             reinterpret_cast<security_manager::SSLContext *>(NULL));
  //Open kAudio service
  const uint32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, start_session_id,
                                                    protocol_handler::kAudio,
                                                    PROTECTION_ON);
  EXPECT_EQ(session_id, start_session_id);
  CheckService(uid, start_session_id, protocol_handler::kAudio, &mock_ssl_context, true);

  //kAudio is not exists yet
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kAudio),
             &mock_ssl_context);
}
TEST_F(ConnectionHandlerTest, GetSSLContext_ByDealyProtecteRPC) {
  testing::StrictMock<security_manager_test::SSLContextMock> mock_ssl_context;
  AddTestDeviceConnection();
  AddTestSession();
  EXPECT_EQ (connection_handler_->SetSSLContext(connection_key, &mock_ssl_context),
             ::security_manager::SecurityQuery::ERROR_SUCCESS);
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             &mock_ssl_context);

  //kRpc is not protected
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kRpc),
             reinterpret_cast<security_manager::SSLContext *>(NULL));

  //Protect kRpc (Bulk will be protect also)
  const uint32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, start_session_id,
                                                    protocol_handler::kRpc,
                                                    PROTECTION_ON);
  EXPECT_EQ(session_id, start_session_id);
  CheckService(uid, start_session_id, protocol_handler::kRpc, &mock_ssl_context, true);

  //kRpc is protecte
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kRpc),
             &mock_ssl_context);
  //kBulk is protecte
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kBulk),
             &mock_ssl_context);
}
TEST_F(ConnectionHandlerTest, GetSSLContext_ByDealyProtecteBulk) {
  testing::StrictMock<security_manager_test::SSLContextMock> mock_ssl_context;
  AddTestDeviceConnection();
  AddTestSession();
  EXPECT_EQ (connection_handler_->SetSSLContext(connection_key, &mock_ssl_context),
             ::security_manager::SecurityQuery::ERROR_SUCCESS);
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kControl),
             &mock_ssl_context);

  //kRpc is not protected
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kRpc),
             reinterpret_cast<security_manager::SSLContext *>(NULL));

  //Protect Bulk (kRpc will be protect also)
  const uint32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, start_session_id,
                                                    protocol_handler::kBulk,
                                                    PROTECTION_ON);
  EXPECT_EQ(session_id, start_session_id);
  CheckService(uid, start_session_id, protocol_handler::kRpc, &mock_ssl_context, true);

  //kRpc is protected
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kRpc),
             &mock_ssl_context);
  //kBulk is protected
  EXPECT_EQ (connection_handler_->GetSSLContext(
               connection_key, protocol_handler::kBulk),
             &mock_ssl_context);
}
} // connection_handle_test
} // namespace components
} // namespace test
#endif  // CONNECTION_HANDLER_IMPL_TEST_H
