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

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "utils/log_message_loop_thread.h"
#include "utils/logger_status.h"

namespace test {
namespace components {
namespace utils {

using namespace ::logger;
using ::testing::_;

TEST(LogMessageLoopThread, DestroyLogMessage_loggerStatusDeletingLogger) {
  logger::logger_status = CreatingLoggerThread;
  LogMessageLoopThread* loop_thread = new LogMessageLoopThread();
  //assert
  EXPECT_EQ(CreatingLoggerThread, logger::logger_status);

  //act
  delete loop_thread;

  //assert
  EXPECT_EQ(DeletingLoggerThread, logger::logger_status);

  logger::logger_status = LoggerThreadNotCreated;
}

class MockLogMessageTest : public LogMessageHandler {
 public:
  MOCK_CONST_METHOD1(Handle, void(const LogMessage message));
};

TEST(LogMessageLoopThread, HandleNeverCalled) {
  logger::logger_status = CreatingLoggerThread;

  MockLogMessageTest mmock;
  EXPECT_CALL(mmock,Handle(_)).Times(0);
  LogMessageLoopThread* loop_thread = new LogMessageLoopThread();

  delete loop_thread;
  logger::logger_status = LoggerThreadNotCreated;
}

}  // namespace utils
}  // namespace components
}  // namespace test
