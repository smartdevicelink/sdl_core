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
#include <string>
#include <sstream>
#include <stdio.h>

#include "utils/winhdr.h"
#include "utils/macro.h"
#include "utils/log_message_loop_thread.h"
#include "utils/file_system.h"
#include "utils/string_utils.h"

namespace {

std::string GetLogTypeStringByLevel(const logger::LogLevel::Type level) {
  using namespace logger;
  switch (level) {
    case LogLevel::LL_TRACE:
      return "TRACE";
    case LogLevel::LL_DEBUG:
      return "DEBUG";
    case LogLevel::LL_INFO:
      return "INFO ";
    case LogLevel::LL_WARN:
      return "WARN ";
    case LogLevel::LL_ERROR:
      return "ERROR";
    case LogLevel::LL_FATAL:
      return "FATAL";
    default:
      NOTREACHED();
  }
  return "TRACE";
}

}  // namespace

namespace logger {

void LogMessageHandler::Handle(const LogMessage message) {
  char time_buf[15];
  _snprintf_s(time_buf,
              sizeof(time_buf),
              "%i:%i:%i:%i",
              message.time_.wHour,
              message.time_.wMinute,
              message.time_.wSecond,
              message.time_.wMilliseconds);

  std::stringstream entry;
  entry << GetLogTypeStringByLevel(message.level_) << " [" << time_buf << "]"
        << " [" << message.thread_id_ << "]"
        << " [" << message.logger_ << "] "
        << file_system::RetrieveFileNameFromPath(message.location_.file_name_)
        << ":" << message.location_.line_number_ << " "
        << message.location_.function_name_ << ": "
        // we have to replace % character with %% because
        // printf() below recognizes % character as formatter start
        << utils::ReplaceString(message.entry_, "%", "%%");

  // dump log string to console
  printf(entry.str().c_str());
  printf("\n");
  // dump log string to file
  if (message.output_file_) {
    fprintf(message.output_file_, entry.str().c_str());
    fprintf(message.output_file_, "\n");
    fflush(message.output_file_);
  }
}

}  // namespace logger
