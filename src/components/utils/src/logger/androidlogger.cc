/*
 * Copyright (c) 2021, Ford Motor Company
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

#include "utils/logger/androidlogger.h"

#include <android/log.h>

namespace{
    std::chrono::high_resolution_clock::time_point init_time_point;
}

namespace logger {

AndroidLogger::AndroidLogger(){
}

void AndroidLogger::Init() {
    init_time_point = std::chrono::high_resolution_clock::now();
}

void AndroidLogger::DeInit() {
}

android_LogPriority getLogLevel(LogLevel log_level) {
    switch (log_level) {
    case LogLevel::TRACE_LEVEL:
      return android_LogPriority::ANDROID_LOG_VERBOSE;
    case LogLevel::DEBUG_LEVEL:
      return android_LogPriority::ANDROID_LOG_DEBUG;
    case LogLevel::INFO_LEVEL:
      return android_LogPriority::ANDROID_LOG_INFO;
    case LogLevel::WARNING_LEVEL:
      return android_LogPriority::ANDROID_LOG_WARN;
    case LogLevel::ERROR_LEVEL:
      return android_LogPriority::ANDROID_LOG_ERROR;
    case LogLevel::FATAL_LEVEL:
      return android_LogPriority::ANDROID_LOG_FATAL;
    default:
      assert(false);
  }
}

bool AndroidLogger::IsEnabledFor(const std::string& component,
                                 LogLevel log_level) const {
  return true;
}

void AndroidLogger::PushLog(const LogMessage& log_message) {

  auto timeInMicroSec =
    std::chrono::duration_cast<std::chrono::microseconds>
    (log_message.timestamp_ - init_time_point).count();

  std::string time_stamp;
  time_stamp += std::to_string((timeInMicroSec % 1000000000) / 1000000 ); // sec
  time_stamp += "::";
  time_stamp += std::to_string((timeInMicroSec % 1000000) / 1000 );       // mls
  time_stamp += "::";
  time_stamp += std::to_string(timeInMicroSec % 1000);                    // mcs

  __android_log_print(
    getLogLevel(log_message.log_level_),
    time_stamp.c_str(),
    "%s: %s:%d %s",
    log_message.log_event_.c_str(),
    log_message.location_.file_name.c_str(),
    log_message.location_.line_number,
    log_message.location_.function_name.c_str()
    );
}

}  // namespace logger
