/**
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

#ifndef TEST_COMPONENTS_CONNECTION_HANDLER_INCLUDE_HEART_BEAT_MONITOR_TEST_H_
#define TEST_COMPONENTS_CONNECTION_HANDLER_INCLUDE_HEART_BEAT_MONITOR_TEST_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "connection_handler/heartbeat_monitor.h"
#include "connection_handler/connection.h"
#include "connection_handler/connection_handler.h"

namespace test  {
namespace components  {
namespace protocol_handler_test {
using namespace connection_handler;

class ConnectionHandlerMock: public ConnectionHandler {
 public:
  MOCK_METHOD1(set_connection_handler_observer, void(ConnectionHandlerObserver*));
  MOCK_METHOD1(set_transport_manager, void(transport_manager::TransportManager*));
  MOCK_METHOD1(set_resume_session_map, void(const ResumeSessionMap&));
  MOCK_METHOD0(StartTransportManager,void());
  MOCK_METHOD1(ConnectToDevice, void(DeviceHandle device_handle));
  MOCK_METHOD1(CloseConnection, void(ConnectionHandle connection_handle));
};

class HeartBeatMonitorTest: public testing::Test {
 public:
  static const uint32_t kTimeout = 2;
  ConnectionHandlerMock connection_handler_mock;
  connection_handler::Connection connection;
  HeartBeatMonitorTest()
  : connection(42, 0, &connection_handler_mock, kTimeout) {
  }
  ~HeartBeatMonitorTest() {
  }
};

TEST_F(HeartBeatMonitorTest, TimerNotElapsed){
  EXPECT_CALL(connection_handler_mock, CloseConnection(42))
      .Times(0);
  connection.AddNewSession();
  sleep(kTimeout - 1);
}

TEST_F(HeartBeatMonitorTest, TimerElapsed){
  EXPECT_CALL(connection_handler_mock, CloseConnection(42));
  connection.AddNewSession();
  sleep(kTimeout + 1);
}
TEST_F(HeartBeatMonitorTest, KeptAlive){
  EXPECT_CALL(connection_handler_mock, CloseConnection(42))
      .Times(0);
  connection.AddNewSession();
  sleep(kTimeout - 1);
  connection.KeepAlive();
  sleep(kTimeout - 1);
  connection.KeepAlive();
  sleep(kTimeout - 1);
  connection.KeepAlive();
  sleep(kTimeout - 1);
}

TEST_F(HeartBeatMonitorTest, NotKeptAlive){
  EXPECT_CALL(connection_handler_mock, CloseConnection(42))
      .Times(1);
  connection.AddNewSession();
  sleep(kTimeout - 1);
  connection.KeepAlive();
  sleep(kTimeout - 1);
  connection.KeepAlive();
  sleep(kTimeout - 1);
  connection.KeepAlive();
  sleep(kTimeout + 1);
}

}
}
}

#endif  // TEST_COMPONENTS_CONNECTION_HANDLER_INCLUDE_HEART_BEAT_MONITOR_TEST_H_
