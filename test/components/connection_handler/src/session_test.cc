/*
 * \file test_schema.cc
 * \brief
 *
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

#include <gtest/gtest.h>
#include "connection_handler/connection_handler_impl.h"

using namespace connection_handler;

namespace test {
namespace components {
namespace connection_handle {

class ConnectionHandlerImplWrapper : public ConnectionHandlerImpl{
public:
  ConnectionHandlerImplWrapper() : ConnectionHandlerImpl() {}

  ConnectionList& getConnectionList(){
    return connection_list_;
  }
};

class SessionTest: public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    session_observer_.reset(new ConnectionHandlerImplWrapper);

    const transport_manager::DeviceHandle device_handle = 0;
    const transport_manager::DeviceInfo device_info(
          device_handle, std::string("test_address"), std::string("test_name"));
    std::vector<transport_manager::DeviceInfo> device_list;
    device_list.push_back(device_info);
    session_observer_->OnDeviceListUpdated(device_list);

    const transport_manager::ConnectionUID uid = 0;

    session_observer_->OnConnectionEstablished(device_info, uid);
  }
  utils::SharedPtr<ConnectionHandlerImplWrapper> session_observer_;
};

TEST_F(SessionTest, OnSessionStartedCallback_OnNoConnection) {

  utils::SharedPtr<ConnectionHandlerImplWrapper> session_observer(new ConnectionHandlerImplWrapper);
  const transport_manager::ConnectionUID uid = 0;
  //new session
  const uint8_t sessionID = 0;
  //start new session with RPC
  session_observer->OnSessionStartedCallback(uid, sessionID, protocol_handler::kRpc);
  ConnectionList& connection_list = session_observer->getConnectionList();
  EXPECT_TRUE(connection_list.empty());
  EXPECT_EQ(connection_list.begin()->first, 0);
}

TEST_F(SessionTest, OnSessionStartedCallback) {
  const transport_manager::ConnectionUID uid = 0;
  //new session
  const uint8_t sessionID = 0;
  //start new session with RPC
  session_observer_->OnSessionStartedCallback(uid, sessionID, protocol_handler::kRpc);
  ConnectionList& connection_list = session_observer_->getConnectionList();
  EXPECT_FALSE(connection_list.empty());
  EXPECT_EQ(connection_list.begin()->first, 0);
}

} // connection_handle
} // namespace components
} // namespace test

