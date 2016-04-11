/*
 * Copyright (c) 2013-2014, Ford Motor Company
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

#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "connection_handler/heartbeat_monitor.h"
#include "connection_handler/connection.h"
#include "connection_handler/connection_handler.h"
#include "connection_handler/mock_connection_handler.h"

namespace {
const int32_t MILLISECONDS_IN_SECOND = 1000;
const int32_t MICROSECONDS_IN_MILLISECONDS = 1000;
const int32_t MICROSECONDS_IN_SECOND = 1000 * 1000;
}

namespace test {
namespace components {
namespace connection_handler_test {
using ::testing::_;

class HeartBeatMonitorTest : public testing::Test {
 public:
  HeartBeatMonitorTest() : conn(NULL) {
    kTimeout = 5000u;
  }

 protected:
  testing::NiceMock<MockConnectionHandler> connection_handler_mock;
  connection_handler::Connection* conn;
  uint32_t kTimeout;
  static const connection_handler::ConnectionHandle kConnectionHandle =
      0xABCDEF;

  virtual void SetUp() {
    conn = new connection_handler::Connection(
        kConnectionHandle, 0, &connection_handler_mock, kTimeout);
  }

  virtual void TearDown() { delete conn; }
};

ACTION_P2(RemoveSession, conn, session_id) { conn->RemoveSession(session_id); }

TEST_F(HeartBeatMonitorTest, TimerNotStarted) {
  // Whithout StartHeartBeat nothing to be call
  EXPECT_CALL(connection_handler_mock, CloseSession(_, _)).Times(0);
  EXPECT_CALL(connection_handler_mock, CloseConnection(_)).Times(0);
  EXPECT_CALL(connection_handler_mock, SendHeartBeat(_, _)).Times(0);

  conn->AddNewSession();
  testing::Mock::AsyncVerifyAndClearExpectations(kTimeout * MICROSECONDS_IN_MILLISECONDS + MICROSECONDS_IN_SECOND);
}

TEST_F(HeartBeatMonitorTest, TimerNotElapsed) {
  EXPECT_CALL(connection_handler_mock, SendHeartBeat(_, _)).Times(0);
  EXPECT_CALL(connection_handler_mock, CloseSession(_, _)).Times(0);
  EXPECT_CALL(connection_handler_mock, CloseConnection(_)).Times(0);

  const uint32_t session = conn->AddNewSession();
  conn->StartHeartBeat(session);
  testing::Mock::AsyncVerifyAndClearExpectations(kTimeout * MICROSECONDS_IN_MILLISECONDS - MICROSECONDS_IN_SECOND);
}

TEST_F(HeartBeatMonitorTest, TimerElapsed) {
  const uint32_t session = conn->AddNewSession();

  EXPECT_CALL(connection_handler_mock, CloseSession(_, session, _))
      .WillOnce(RemoveSession(conn, session));
  EXPECT_CALL(connection_handler_mock, CloseConnection(_));
  EXPECT_CALL(connection_handler_mock, SendHeartBeat(_, session));

  conn->StartHeartBeat(session);
  testing::Mock::AsyncVerifyAndClearExpectations(2 * kTimeout * MICROSECONDS_IN_MILLISECONDS + MICROSECONDS_IN_SECOND);
}

TEST_F(HeartBeatMonitorTest, KeptAlive) {
  EXPECT_CALL(connection_handler_mock, CloseSession(_, _)).Times(0);
  EXPECT_CALL(connection_handler_mock, CloseConnection(_)).Times(0);
  EXPECT_CALL(connection_handler_mock, SendHeartBeat(_, _)).Times(0);

  const uint32_t session = conn->AddNewSession();
  conn->StartHeartBeat(session);
  usleep(kTimeout * MICROSECONDS_IN_MILLISECONDS - MICROSECONDS_IN_SECOND);
  conn->KeepAlive(session);
  usleep(kTimeout * MICROSECONDS_IN_MILLISECONDS - MICROSECONDS_IN_SECOND);
  conn->KeepAlive(session);
  usleep(kTimeout * MICROSECONDS_IN_MILLISECONDS - MICROSECONDS_IN_SECOND);
  conn->KeepAlive(session);
  usleep(kTimeout * MICROSECONDS_IN_MILLISECONDS - MICROSECONDS_IN_SECOND);
}

TEST_F(HeartBeatMonitorTest, NotKeptAlive) {
  const uint32_t session = conn->AddNewSession();

  EXPECT_CALL(connection_handler_mock, SendHeartBeat(_, session));
  EXPECT_CALL(connection_handler_mock, CloseSession(_, session, _))
      .WillOnce(RemoveSession(conn, session));
  EXPECT_CALL(connection_handler_mock, CloseConnection(_));

  conn->StartHeartBeat(session);
  usleep(kTimeout * MICROSECONDS_IN_MILLISECONDS - MICROSECONDS_IN_SECOND);
  conn->KeepAlive(session);
  usleep(kTimeout * MICROSECONDS_IN_MILLISECONDS - MICROSECONDS_IN_SECOND);
  conn->KeepAlive(session);
  usleep(kTimeout * MICROSECONDS_IN_MILLISECONDS - MICROSECONDS_IN_SECOND);
  conn->KeepAlive(session);
  usleep(2 * kTimeout * MICROSECONDS_IN_MILLISECONDS + MICROSECONDS_IN_SECOND);
}

TEST_F(HeartBeatMonitorTest, TwoSessionsElapsed) {
  const uint32_t kSession1 = conn->AddNewSession();
  const uint32_t kSession2 = conn->AddNewSession();

  EXPECT_CALL(connection_handler_mock, CloseSession(_, kSession1, _))
      .WillOnce(RemoveSession(conn, kSession1));
  EXPECT_CALL(connection_handler_mock, CloseSession(_, kSession2, _))
      .WillOnce(RemoveSession(conn, kSession2));
  EXPECT_CALL(connection_handler_mock, CloseConnection(_));
  EXPECT_CALL(connection_handler_mock, SendHeartBeat(_, kSession1));
  EXPECT_CALL(connection_handler_mock, SendHeartBeat(_, kSession2));

  conn->StartHeartBeat(kSession1);
  conn->StartHeartBeat(kSession2);
  testing::Mock::AsyncVerifyAndClearExpectations(2 * kTimeout * MICROSECONDS_IN_MILLISECONDS + MICROSECONDS_IN_SECOND);
}

TEST_F(HeartBeatMonitorTest, IncreaseHeartBeatTimeout) {
  const uint32_t kSession = conn->AddNewSession();

  EXPECT_CALL(connection_handler_mock, CloseSession(_, _)).Times(0);
  EXPECT_CALL(connection_handler_mock, CloseConnection(_)).Times(0);
  EXPECT_CALL(connection_handler_mock, SendHeartBeat(_, _)).Times(0);

  const uint32_t kNewTimeout = kTimeout + MICROSECONDS_IN_MILLISECONDS;
  conn->StartHeartBeat(kSession);
  conn->SetHeartBeatTimeout(kNewTimeout, kSession);
  // new timeout greater by old timeout so mock object shouldn't be invoked
  testing::Mock::AsyncVerifyAndClearExpectations(kTimeout * MICROSECONDS_IN_MILLISECONDS);
}

TEST_F(HeartBeatMonitorTest, DecreaseHeartBeatTimeout) {
  const uint32_t kSession = conn->AddNewSession();

  EXPECT_CALL(connection_handler_mock, CloseSession(_, kSession, _))
      .WillOnce(RemoveSession(conn, kSession));
  EXPECT_CALL(connection_handler_mock, CloseConnection(_));
  EXPECT_CALL(connection_handler_mock, SendHeartBeat(_, kSession));

  const uint32_t kNewTimeout = kTimeout - MICROSECONDS_IN_MILLISECONDS;
  conn->StartHeartBeat(kSession);
  conn->SetHeartBeatTimeout(kNewTimeout, kSession);
  // new timeout less than old timeout so mock object should be invoked
  testing::Mock::AsyncVerifyAndClearExpectations(kTimeout * 2 * MICROSECONDS_IN_MILLISECONDS);
}

}  // namespace connection_handler_test
}  // namespace components
}  // namespace test
