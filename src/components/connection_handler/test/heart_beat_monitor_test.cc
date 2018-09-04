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
#include "utils/test_async_waiter.h"

namespace {
const int32_t MILLISECONDS_IN_SECOND = 1000;
const int32_t MICROSECONDS_IN_MILLISECONDS = 1000;
const int32_t MICROSECONDS_IN_SECOND = 1000 * 1000;
const uint32_t kTime_offset = 20u;
}

namespace test {
namespace components {
namespace connection_handler_test {

using ::testing::DoAll;
using ::testing::_;
using ::testing::Return;

class HeartBeatMonitorTest : public testing::Test {
 public:
  HeartBeatMonitorTest()
      : connection_(NULL)
      , timeout_(100u)
      , time_allowance_multiplier_(1.5)
      , expiration_timeout_(timeout_ * 2u) {}

 protected:
  testing::NiceMock<MockConnectionHandler> connection_handler_mock_;
  connection_handler::Connection* connection_;
  const uint32_t timeout_;
  const float_t time_allowance_multiplier_;
  const uint32_t expiration_timeout_;
  static const connection_handler::ConnectionHandle connection_handle_ =
      0xABCDEF;
  static const transport_manager::ConnectionUID kDefaultConnectionHandle = 1;
  static const uint32_t kDefaultSessionId = 1;

  void SetUp() OVERRIDE {
    connection_ = new connection_handler::Connection(
        connection_handle_, 0, &connection_handler_mock_, timeout_);
  }

  void TearDown() OVERRIDE {
    delete connection_;
  }
};

ACTION_P2(RemoveSession, conn, session_id) {
  conn->RemoveSession(session_id);
}

TEST_F(HeartBeatMonitorTest, TimerNotStarted) {
  EXPECT_CALL(connection_handler_mock_, AddSession(_))
      .WillOnce(Return(kDefaultSessionId));
  EXPECT_CALL(connection_handler_mock_, RemoveSession(kDefaultSessionId))
      .WillOnce(Return(true));  // called by destructor of Connection

  // Whithout StartHeartBeat nothing to be call
  EXPECT_CALL(connection_handler_mock_, CloseSession(_, _)).Times(0);
  EXPECT_CALL(connection_handler_mock_, CloseConnection(_)).Times(0);
  EXPECT_CALL(connection_handler_mock_, SendHeartBeat(_, _)).Times(0);

  connection_->AddNewSession(kDefaultConnectionHandle);
}

TEST_F(HeartBeatMonitorTest, TimerNotElapsed) {
  EXPECT_CALL(connection_handler_mock_, AddSession(_))
      .WillOnce(Return(kDefaultSessionId));
  EXPECT_CALL(connection_handler_mock_, RemoveSession(kDefaultSessionId))
      .WillOnce(Return(true));

  EXPECT_CALL(connection_handler_mock_, SendHeartBeat(_, _)).Times(0);
  EXPECT_CALL(connection_handler_mock_, CloseSession(_, _)).Times(0);
  EXPECT_CALL(connection_handler_mock_, CloseConnection(_)).Times(0);

  const uint32_t session = connection_->AddNewSession(kDefaultConnectionHandle);
  connection_->StartHeartBeat(session);
}

TEST_F(HeartBeatMonitorTest, TimerElapsed) {
  EXPECT_CALL(connection_handler_mock_, AddSession(_))
      .WillOnce(Return(kDefaultSessionId));
  EXPECT_CALL(connection_handler_mock_, RemoveSession(kDefaultSessionId))
      .WillOnce(Return(true));  // invoked by RemoveSession action

  const uint32_t session = connection_->AddNewSession(kDefaultConnectionHandle);

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(connection_handler_mock_, CloseSession(_, session, _))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter),
                      RemoveSession(connection_, session)));
  times++;
  EXPECT_CALL(connection_handler_mock_, SendHeartBeat(_, session))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  connection_->StartHeartBeat(session);

  EXPECT_TRUE(waiter.WaitFor(
      times,
      2 * timeout_ * MICROSECONDS_IN_MILLISECONDS + MICROSECONDS_IN_SECOND));
}

TEST_F(HeartBeatMonitorTest, KeptAlive) {
  EXPECT_CALL(connection_handler_mock_, AddSession(_))
      .WillOnce(Return(kDefaultSessionId));
  EXPECT_CALL(connection_handler_mock_, RemoveSession(kDefaultSessionId))
      .WillOnce(Return(true));

  EXPECT_CALL(connection_handler_mock_, CloseSession(_, _)).Times(0);
  EXPECT_CALL(connection_handler_mock_, CloseConnection(_)).Times(0);
  EXPECT_CALL(connection_handler_mock_, SendHeartBeat(_, _)).Times(0);

  const uint32_t session = connection_->AddNewSession(kDefaultConnectionHandle);
  connection_->StartHeartBeat(session);
  usleep(timeout_);
  connection_->KeepAlive(session);
  usleep(timeout_);
  connection_->KeepAlive(session);
}

TEST_F(HeartBeatMonitorTest, NotKeptAlive) {
  EXPECT_CALL(connection_handler_mock_, AddSession(_))
      .WillOnce(Return(kDefaultSessionId));
  EXPECT_CALL(connection_handler_mock_, RemoveSession(kDefaultSessionId))
      .WillOnce(Return(true));

  const uint32_t session = connection_->AddNewSession(kDefaultConnectionHandle);

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(connection_handler_mock_, SendHeartBeat(_, session))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(connection_handler_mock_, CloseSession(_, session, _))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter),
                      RemoveSession(connection_, session)));
  times++;

  connection_->StartHeartBeat(session);
  usleep(timeout_);
  connection_->KeepAlive(session);

  EXPECT_TRUE(waiter.WaitFor(
      times,
      2 * timeout_ * MICROSECONDS_IN_MILLISECONDS + MICROSECONDS_IN_SECOND));
}

TEST_F(HeartBeatMonitorTest, TwoSessionsElapsed) {
  const uint32_t kMockSessionId1 = 1;
  const uint32_t kMockSessionId2 = 2;
  EXPECT_CALL(connection_handler_mock_, AddSession(_))
      .WillOnce(Return(kMockSessionId1))
      .WillOnce(Return(kMockSessionId2));
  EXPECT_CALL(connection_handler_mock_, RemoveSession(kMockSessionId1))
      .WillOnce(Return(true));
  EXPECT_CALL(connection_handler_mock_, RemoveSession(kMockSessionId2))
      .WillOnce(Return(true));

  const uint32_t kSession1 =
      connection_->AddNewSession(kDefaultConnectionHandle);

  const transport_manager::ConnectionUID kAnotherConnectionHandle = 2;
  const uint32_t kSession2 =
      connection_->AddNewSession(kAnotherConnectionHandle);

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(connection_handler_mock_, CloseSession(_, kSession1, _))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter),
                      RemoveSession(connection_, kSession1)));
  times++;
  EXPECT_CALL(connection_handler_mock_, CloseSession(_, kSession2, _))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter),
                      RemoveSession(connection_, kSession2)));
  times++;

  EXPECT_CALL(connection_handler_mock_, SendHeartBeat(_, kSession1))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;
  EXPECT_CALL(connection_handler_mock_, SendHeartBeat(_, kSession2))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  connection_->StartHeartBeat(kSession1);
  connection_->StartHeartBeat(kSession2);

  EXPECT_TRUE(waiter.WaitFor(
      times,
      2 * timeout_ * MICROSECONDS_IN_MILLISECONDS + MICROSECONDS_IN_SECOND));
}

TEST_F(HeartBeatMonitorTest, IncreaseHeartBeatTimeout) {
  EXPECT_CALL(connection_handler_mock_, AddSession(_))
      .WillOnce(Return(kDefaultSessionId));
  EXPECT_CALL(connection_handler_mock_, RemoveSession(kDefaultSessionId))
      .WillOnce(Return(true));

  const uint32_t kSession =
      connection_->AddNewSession(kDefaultConnectionHandle);

  EXPECT_CALL(connection_handler_mock_, CloseSession(_, _)).Times(0);
  EXPECT_CALL(connection_handler_mock_, CloseConnection(_)).Times(0);
  EXPECT_CALL(connection_handler_mock_, SendHeartBeat(_, _)).Times(0);

  const uint32_t kNewTimeout = timeout_ + MICROSECONDS_IN_MILLISECONDS;
  connection_->StartHeartBeat(kSession);
  connection_->SetHeartBeatTimeout(kNewTimeout, kSession);
}

TEST_F(HeartBeatMonitorTest, DecreaseHeartBeatTimeout) {
  EXPECT_CALL(connection_handler_mock_, AddSession(_))
      .WillOnce(Return(kDefaultSessionId));
  EXPECT_CALL(connection_handler_mock_, RemoveSession(kDefaultSessionId))
      .WillOnce(Return(true));

  const uint32_t kSession =
      connection_->AddNewSession(kDefaultConnectionHandle);

  TestAsyncWaiter waiter;
  uint32_t times = 0;
  EXPECT_CALL(connection_handler_mock_, CloseSession(_, kSession, _))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter),
                      RemoveSession(connection_, kSession)));
  times++;
  EXPECT_CALL(connection_handler_mock_, SendHeartBeat(_, kSession))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  times++;

  const uint32_t new_timeout = timeout_ - kTime_offset;
  connection_->StartHeartBeat(kSession);
  connection_->SetHeartBeatTimeout(new_timeout, kSession);

  EXPECT_TRUE(waiter.WaitFor(
      times,
      2 * timeout_ * MICROSECONDS_IN_MILLISECONDS + MICROSECONDS_IN_SECOND));
}

}  // namespace connection_handler_test
}  // namespace components
}  // namespace test
