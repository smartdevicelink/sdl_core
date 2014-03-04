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
#include "connection_handler/connection_handler_impl.h"

using namespace connection_handler;

namespace test {
namespace components {
namespace connection_handle {

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
  void AddTestDeviceConenction() {
    const transport_manager::DeviceHandle device_handle = 0;
    const transport_manager::DeviceInfo device_info(
          device_handle, std::string("test_address"), std::string("test_name"));
    //Add Device and connection
    const bool result =
        connection_handler_->addDeviceConnection(device_info, uid);
    EXPECT_TRUE(result);
  }

  ConnectionHandlerImpl* connection_handler_;
  transport_manager::ConnectionUID uid;
};

TEST_F(ConnectionHandlerTest, SessionStarted_Fial_NoConnection) {
  //null sessionId for start new session
  const uint8_t sessionID = 0;
  //start new session with RPC service
  const int32_t result_fail =
      connection_handler_->OnSessionStartedCallback(uid, sessionID, protocol_handler::kRpc);
  //Unknown connection error is '-1'
  EXPECT_EQ(result_fail, -1);
  EXPECT_TRUE(connection_handler_->getConnectionList().empty());
}

TEST_F(ConnectionHandlerTest, SessionStarted_RPC) {
  //null sessionId for start new session
  const uint8_t sessionID = 0;

  //Add virtual device and connection
  AddTestDeviceConenction();

  //start new session with RPC service
  const int32_t session_id_on_rpc_secure =
      connection_handler_->OnSessionStartedCallback(uid, sessionID, protocol_handler::kRpc);
  EXPECT_NE(session_id_on_rpc_secure, -1);
  const ConnectionList& connection_list = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list.empty());
  const Connection* const connection = connection_list.begin()->second;
  EXPECT_NE(connection, static_cast<Connection*>(NULL));
  const SessionMap sessionMap = connection->session_map();
  EXPECT_EQ(sessionMap.size(), 1);
  EXPECT_EQ(sessionMap.begin()->first, session_id_on_rpc_secure);
  const ServiceList serviceList = sessionMap.begin()->second;
  const ServiceList::const_iterator it =
      std::find(serviceList.begin(), serviceList.end(), protocol_handler::kRpc);
  EXPECT_NE(it, serviceList.end());
}

TEST_F(ConnectionHandlerTest, SessionStarted_Secure) {
  //null sessionId for start new session
  const uint8_t start_session_id = 0;

  //Add virtual device and connection
  AddTestDeviceConenction();

  //start new session with RPC service
  const int32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, start_session_id, protocol_handler::kRpc);
  EXPECT_NE(session_id, -1);
  const ConnectionList& connection_list = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list.empty());
  EXPECT_EQ(connection_list.begin()->first, 0);

  //start new session with RPC service
  const int32_t session_id_on_start_secure =
      connection_handler_->OnSessionStartedCallback(uid, session_id, protocol_handler::kSecure);
  EXPECT_EQ(session_id_on_start_secure, session_id);
  ConnectionList& connection_list_new = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list_new.empty());
  EXPECT_EQ(connection_list_new.begin()->first, 0);
}

TEST_F(ConnectionHandlerTest, SessionEnded_Secure) {
  //null sessionId for start new session
  const uint8_t start_session_id = 0;

  //Add virtual device and connection
  AddTestDeviceConenction();

  //start new session with RPC service
  const int32_t session_id =
      connection_handler_->OnSessionStartedCallback(uid, start_session_id, protocol_handler::kRpc);
  EXPECT_NE(session_id, -1);
  const ConnectionList& connection_list = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list.empty());
  EXPECT_EQ(connection_list.begin()->first, 0);

  //start new session with RPC service
  const int32_t session_id_on_start_secure =
      connection_handler_->OnSessionStartedCallback(uid, session_id,
                                                    protocol_handler::kSecure);
  EXPECT_EQ(session_id_on_start_secure, session_id);
  ConnectionList& connection_list_start_secure = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list_start_secure.empty());
  EXPECT_EQ(connection_list_start_secure.begin()->first, 0);

  //start new session with RPC service
  const int32_t session_id_on_end_secure =
      connection_handler_->OnSessionEndedCallback(uid, session_id, 0,
                                                  protocol_handler::kSecure);
  EXPECT_EQ(session_id_on_end_secure, session_id);
  ConnectionList& connection_list_on_end_secure = connection_handler_->getConnectionList();
  EXPECT_FALSE(connection_list_on_end_secure.empty());
  EXPECT_EQ(connection_list_on_end_secure.begin()->first, 0);
}

} // connection_handle
} // namespace components
} // namespace test

#endif // CONNECTION_HANDLER_IMPL_TEST_H
