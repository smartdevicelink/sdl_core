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


#include <gtest/gtest.h>
#include <algorithm>
#include "connection_handler/connection.h"
#include "connection_handler/connection_handler_impl.h"
#include "protocol_handler/service_type.h"
#include "utils/shared_ptr.h"

using namespace connection_handler;

namespace test {
namespace components {
namespace connection_handle {

class ConnectionTest: public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    connection_handler_ = ConnectionHandlerImpl::instance();
    const ConnectionHandle connectionHandle = 0;
    const DeviceHandle device_handle = 0;
    connection_.reset(new Connection(connectionHandle, device_handle,
                                     connection_handler_, 10000));
  }

  void TearDown() OVERRIDE {
    ConnectionHandlerImpl::destroy();
  }

  utils::SharedPtr<Connection> connection_;
  ConnectionHandlerImpl* connection_handler_;
};

TEST_F(ConnectionTest, Session_AddNewSession) {
  const int32_t session_id = connection_->AddNewSession();
  EXPECT_NE(session_id, -1);
  const SessionMap sessionMap = connection_->session_map();
  EXPECT_FALSE(sessionMap.empty());
  const ServiceList serviceList = sessionMap.begin()->second;
  EXPECT_FALSE(serviceList.empty());
  const ServiceList::const_iterator it =
      std::find(serviceList.begin(), serviceList.end(), protocol_handler::kRpc);
  EXPECT_NE(it, serviceList.end());
}

TEST_F(ConnectionTest, Session_SecureService) {
  const int32_t session_id = connection_->AddNewSession();
  EXPECT_NE(session_id, -1);
  const SessionMap sessionMap = connection_->session_map();
  EXPECT_FALSE(sessionMap.empty());
  const ServiceList serviceList = sessionMap.begin()->second;
  EXPECT_FALSE(serviceList.empty());
  const ServiceList::const_iterator it =
      std::find(serviceList.begin(), serviceList.end(), protocol_handler::kRpc);
  EXPECT_NE(it, serviceList.end());

  //TODO: check Secure plugin enable

  const bool result = connection_->AddNewService(session_id, protocol_handler::kSecure);
  EXPECT_TRUE(result);
  const SessionMap newSessionMap = connection_->session_map();
  EXPECT_FALSE(newSessionMap.empty());
  const ServiceList newServiceList = newSessionMap.begin()->second;
  EXPECT_FALSE(newServiceList.empty());
  const ServiceList::const_iterator it1 =
      std::find(newServiceList.begin(), newServiceList.end(), protocol_handler::kSecure);
  EXPECT_NE(it1, newServiceList.end());

  const bool result2 = connection_->RemoveService(session_id, protocol_handler::kSecure);
  EXPECT_TRUE(result2);
  const SessionMap newSessionMap2 = connection_->session_map();
  EXPECT_FALSE(newSessionMap2.empty());
  const ServiceList newServiceList2 = newSessionMap2.begin()->second;
  EXPECT_FALSE(newServiceList2.empty());
  const ServiceList::const_iterator it2 =
      std::find(newServiceList2.begin(), newServiceList2.end(), protocol_handler::kSecure);
  EXPECT_EQ(it2, newServiceList2.end());
}

} // connection_handle
} // namespace components
} // namespace test
