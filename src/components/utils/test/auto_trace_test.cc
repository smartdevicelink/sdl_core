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

#include <fstream>
#include <ctime>

#include "gtest/gtest.h"
#include "utils/auto_trace.h"
#include "utils/logger.h"
#include "config_profile/profile.h"
#include "utils/log_message_loop_thread.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/file_system.h"
#include "utils/threads/thread.h"
#include "utils/date_time.h"
#include "utils/logger_status.h"

namespace test {
namespace components {
namespace utils_test {

using namespace ::logger;

CREATE_LOGGERPTR_GLOBAL(logger_, "AutoTraceTestLog")

void Preconditions() {
  const char* file_name = "AutoTraceTestLogFile.log";
  // Delete file with previous logs
  if (file_system::FileExists(file_name)) {
    ASSERT_TRUE(file_system::DeleteFile(file_name))
        << "Can't delete AutoTraceTestLogFile.log";
  }
}

void InitLogger() {
  // Set enabled logs
  profile::Profile::instance()->config_file_name("smartDeviceLink.ini");
  profile::Profile::instance()->UpdateValues();
  INIT_LOGGER("log4cxx.properties", true); //DEINIT_LOGGER will be called in test_main.cc
}

void CreateDeleteAutoTrace(const std::string & testlog) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, testlog);
}

bool CheckAutoTraceDebugInFile(const std::string & testlog) {
  bool isLogFound = false;
  std::string line;

  std::ifstream file_log("AutoTraceTestLogFile.log");

  if (file_log.is_open()) {
    while (getline(file_log, line)) {
      std::size_t found = line.find(testlog);
      std::size_t founddebug = line.find("DEBUG");
      if ((found != std::string::npos) && (founddebug != std::string::npos)) {
        isLogFound = true;
        break;
      }
    }
    file_log.close();
  } else {
    std::cout << "file cannot be opened \n";
  }
  return isLogFound;
}

TEST(AutoTraceTest, AutoTrace_WriteToFile_ReadCorrectString) {
  const std::string testlog =
      "Test trace is working!";
  Preconditions();
  InitLogger();
  CreateDeleteAutoTrace(testlog);

  const TimevalStruct startTime = date_time::DateTime::getCurrentTime();
  const int64_t timeout_msec = 10000;
  // Waiting for empty Logger MessageQueue 10 seconds
  LogMessageLoopThread loop_thread;
  while (loop_thread.GetMessageQueueSize()) {
    ASSERT_LT(date_time::DateTime::calculateTimeDiff(
        date_time::DateTime::getCurrentTime(), startTime), timeout_msec);
    threads::Thread::yield();
  }
  ASSERT_TRUE(CheckAutoTraceDebugInFile(testlog));
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
