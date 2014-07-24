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

#ifndef TEST_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_HEART_BEAT_MONITOR_TEST_H_
#define TEST_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_HEART_BEAT_MONITOR_TEST_H_

#include <string>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "connection_handler/heartbeat_monitor.h"
#include "connection_handler/connection.h"
#include "connection_handler/connection_handler.h"

namespace test  {
namespace components  {
namespace protocol_handler_test {
using ::testing::_;

class ConnectionHandlerMock: public connection_handler::ConnectionHandler {
 public:
  MOCK_METHOD1(set_connection_handler_observer,
               void(connection_handler::ConnectionHandlerObserver*));
  MOCK_METHOD1(set_transport_manager,
               void(transport_manager::TransportManager*));
  MOCK_METHOD0(StartTransportManager,
               void());
  MOCK_METHOD1(ConnectToDevice,
               void(connection_handler::DeviceHandle device_handle));
  MOCK_METHOD0(ConnectToAllDevices,
               void());
  MOCK_METHOD1(CloseConnection,
               void(connection_handler::ConnectionHandle connection_handle));
  MOCK_METHOD1(GetConnectionSessionsCount, uint32_t(uint32_t connection_key));
  MOCK_METHOD2(GetDeviceID,
               bool(const std::string& mac_address,
                    connection_handler::DeviceHandle* device_handle));
  MOCK_METHOD1(CloseSession,
               void(uint32_t key));
  MOCK_METHOD2(CloseSession,
               void(connection_handler::ConnectionHandle connection_handle,
                    uint8_t session_id));
  MOCK_METHOD1(StartSessionHeartBeat,
               void(uint32_t key));
  MOCK_METHOD2(SendHeartBeat,
               void(connection_handler::ConnectionHandle connection_handle,
                    uint8_t session_id));
  MOCK_METHOD2(BindProtocolVersionWithSession,
               void(uint32_t connection_key,
                    uint8_t protocol_version));
};

class HeartBeatMonitorTest: public testing::Test {
protected:
  virtual void SetUp() {
  }
 public:
  testing::NiceMock<ConnectionHandlerMock>   connection_handler_mock;
  connection_handler::Connection connection;
  static const int32_t kTimeout = 2;
  static const connection_handler::ConnectionHandle connection_handle = 0xABCDEF;
  HeartBeatMonitorTest()
    : connection(connection_handle, 0, &connection_handler_mock, kTimeout) {
  }
};

TEST_F(HeartBeatMonitorTest, TimerNotStarted) {
  // Whithout StartHeartBeat nothing to be call
  EXPECT_CALL(connection_handler_mock, CloseConnection(_)).Times(0);
  connection.AddNewSession();
  sleep(kTimeout + 1);
}

TEST_F(HeartBeatMonitorTest, TimerNotElapsed) {
  EXPECT_CALL(connection_handler_mock, CloseConnection(_)).Times(0);
  const uint32_t session = connection.AddNewSession();
  connection.StartHeartBeat(session);
  sleep(kTimeout - 1);
}

TEST_F(HeartBeatMonitorTest, TimerElapsed) {
  const uint32_t session = connection.AddNewSession();
  connection.StartHeartBeat(session);

  // FIXME(EZamakhov): not called
  EXPECT_CALL(connection_handler_mock, CloseConnection(connection_handle));
  sleep(kTimeout + 1);
}
TEST_F(HeartBeatMonitorTest, KeptAlive) {
  EXPECT_CALL(connection_handler_mock, CloseConnection(_)).Times(0);
  const uint32_t session = connection.AddNewSession();
  connection.StartHeartBeat(session);
  sleep(kTimeout - 1);
  connection.KeepAlive(session);
  sleep(kTimeout - 1);
  connection.KeepAlive(session);
  sleep(kTimeout - 1);
  connection.KeepAlive(session);
  sleep(kTimeout - 1);
}

TEST_F(HeartBeatMonitorTest, NotKeptAlive) {
  const uint32_t session = connection.AddNewSession();
  connection.StartHeartBeat(session);

  // FIXME(EZamakhov): not called
  EXPECT_CALL(connection_handler_mock, CloseConnection(session)).Times(1);
  sleep(kTimeout - 1);
  connection.KeepAlive(session);
  sleep(kTimeout - 1);
  connection.KeepAlive(session);
  sleep(kTimeout - 1);
  connection.KeepAlive(session);
  sleep(kTimeout + 13);
}

}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test
#endif  // TEST_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_HEART_BEAT_MONITOR_TEST_H_
