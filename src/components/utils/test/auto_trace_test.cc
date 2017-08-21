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
#include "utils/log_message_loop_thread.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/file_system.h"
#include "utils/threads/thread.h"
#include "utils/date_time.h"
#include "utils/logger_status.h"
#include "utils/helpers.h"

namespace test {
namespace components {
namespace utils_test {

using namespace ::logger;

CREATE_LOGGERPTR_GLOBAL(logger_, "AutoTraceTestLog")

namespace {
const std::string kFileName =
    file_system::CurrentWorkingDirectory() + "/AutoTraceTestLogFile.log";
}  // namespace

void Preconditions() {
  // Delete file with previous logs
  if (file_system::FileExists(kFileName)) {
    // If logger is active now deleting log file cause undefined befaviour.
    DEINIT_LOGGER();
    ASSERT_TRUE(file_system::DeleteFile(kFileName))
        << "Can't delete AutoTraceTestLogFile.log";
  }
}

void InitLogger() {
  // Set enabled logs
  INIT_LOGGER("log4cxx.properties", true);
  // DEINIT_LOGGER will be called in test_main.cc
}

void CreateDeleteAutoTrace(const std::string& testlog) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, testlog);
}

/**
 * @brief IsLogLineContains cheks if log line contains debug message with
 * specified debug message
 * @param log_line line to search in
 * @param debug_level expected debug level
 * @param debug_log_message expected debug message
 * @return true if debug_message exist in log_line with debug_level
 */
bool IsLogLineContains(const std::string& log_line,
                       const std::string& debug_level,
                       const std::string& debug_message) {
  return log_line.find(debug_level) != std::string::npos &&
         log_line.find(debug_message) != std::string::npos;
}

/**
 * @brief CheckAutoTraceDebugInFile chacks if logfile contains autotrace and
 * debug for test AutoTrace_WriteToFile_ReadCorrectString
 * @param debug_message message that should be logged with DEBUG level
 * @return true if trace enter, trace exit, debug message exist in log file
 */
bool CheckAutoTraceDebugInFile(const std::string& debug_message) {
  using namespace helpers;
  const std::string debug_log_level = "DEBUG";
  const std::string trace_log_level = "TRACE";
  const std::string enter_message = ": Enter";
  const std::string exit_message = ": Exit";
  std::ifstream file_log(kFileName);
  if (!file_log.is_open()) {
    return false;
  }

  bool debug_found = false;
  bool trace_enter = false;
  bool trace_exit = false;
  for (std::string line;
       Compare<bool, EQ, ONE>(false, debug_found, trace_enter, trace_exit) &&
           getline(file_log, line);) {
    debug_found = debug_found
                      ? debug_found
                      : IsLogLineContains(line, debug_log_level, debug_message);
    trace_enter = trace_enter
                      ? trace_enter
                      : IsLogLineContains(line, trace_log_level, enter_message);
    trace_exit = trace_exit
                     ? trace_exit
                     : IsLogLineContains(line, trace_log_level, exit_message);
  }
  file_log.close();
  return Compare<bool, EQ, ALL>(true, debug_found, trace_enter, trace_exit);
}
// TODO(DTrunov) : Enable after APPLINK-25006 will be resolved
TEST(AutoTraceTest, DISABLED_AutoTrace_WriteToFile_ReadCorrectString) {
  const std::string testlog = "Test trace is working!";
  Preconditions();
  InitLogger();
  CreateDeleteAutoTrace(testlog);

  FLUSH_LOGGER();
  ASSERT_TRUE(CheckAutoTraceDebugInFile(testlog));
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
