/*
 * Copyright (c) 2016, Ford Motor Company
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
#include <stddef.h>

#include <fstream>
#include <ctime>

#include "gtest/gtest.h"
#include "utils/logger.h"
#include "utils/log_message_loop_thread.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/file_system.h"
#include "utils/threads/thread.h"
#include "utils/date_time.h"
#include "utils/helpers.h"
#include "utils/appenders_loader.h"

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
  INIT_LOGGER(true);
  // DEINIT_LOGGER will be called in test_main.cc
}

void CreateDebugAndAutoTrace(const std::string& debug_message) {
  LOGGER_AUTO_TRACE(logger_);
  LOGGER_DEBUG(logger_, debug_message);
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
 * @brief LogWithLevel Log message with log level.
 * @first log level
 * @second log message
 */
typedef std::pair<std::string, std::string> LogWithLevel;

/**
 * @brief CheckIfIstreamContains chacks if file stream contains all log lines
 * from search for vector
 * @param stream file stream
 * @param search_for vector with logg messages that exist in stream
 * @return index of first element from search_for vector or -1 if all elements
 * exists in stream
 */
int32_t CheckIfIstreamContains(std::ifstream& stream,
                               const std::vector<LogWithLevel>& search_for) {
  std::vector<LogWithLevel>::const_iterator it = search_for.begin();
  std::string line;
  while (it != search_for.end() && getline(stream, line)) {
    if (IsLogLineContains(line, it->first, it->second)) {
      ++it;
    }
  }
  return it == search_for.end() ? -1 : std::distance(search_for.begin(), it);
}

// TODO(AKutsan) : Enable after resolve APPLINK-24336
TEST(AutoTraceTest, DISABLED_AutoTrace_WriteToFile_ReadCorrectString) {
  const std::string debug_log_level = "DEBUG";
  const std::string trace_log_level = "TRACE";
  const std::string enter_message = ": Enter";
  const std::string exit_message = ": Exit";
  const std::string debug_message =
      "Trying to debug AutoTrace_WriteToFile_ReadCorrectString";
  Preconditions();
  InitLogger();
  CreateDebugAndAutoTrace(debug_message);

  FLUSH_LOGGER();
  std::ifstream file_log(kFileName);
  ASSERT_TRUE(file_log.is_open()) << "Unable to open log file";

  std::vector<LogWithLevel> log_messages;
  log_messages.push_back(LogWithLevel(trace_log_level, enter_message));
  log_messages.push_back(LogWithLevel(debug_log_level, debug_message));
  log_messages.push_back(LogWithLevel(trace_log_level, exit_message));
  const int32_t index_of_missed_element =
      CheckIfIstreamContains(file_log, log_messages);
  if (index_of_missed_element != -1) {
    const std::string missed_log = log_messages[index_of_missed_element].first +
                                   " " +
                                   log_messages[index_of_missed_element].second;
    FAIL() << "Missed log :" << missed_log.c_str();
  }
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
