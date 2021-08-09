/*
 * Copyright (c) 2020, Ford Motor Company
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

#pragma once

#include <chrono>
#include <string>
#include <thread>

namespace logger {

enum class LogLevel {
  TRACE_LEVEL,
  DEBUG_LEVEL,
  INFO_LEVEL,
  WARNING_LEVEL,
  ERROR_LEVEL,
  FATAL_LEVEL
};

struct LocationInfo {
  std::string file_name;
  std::string function_name;
  int line_number;
};

typedef std::chrono::high_resolution_clock::time_point TimePoint;

struct LogMessage {
  std::string component_;  // <- component_name
  LogLevel log_level_;
  std::string log_event_;
  TimePoint timestamp_;
  LocationInfo location_;
  std::thread::id thread_id_;
};

class Logger {
 public:
  virtual bool IsEnabledFor(const std::string& component,
                            LogLevel log_level) const = 0;
  virtual void DeInit() = 0;
  virtual void Flush() = 0;
  virtual void PushLog(const LogMessage& log_message) = 0;
  static Logger& instance(Logger* pre_init = nullptr);

 protected:
  static Logger* instance_;
};

class ThirdPartyLoggerInterface {
 public:
  virtual ~ThirdPartyLoggerInterface() {}
  virtual void Init() = 0;
  virtual void DeInit() = 0;
  virtual bool IsEnabledFor(const std::string& component,
                            LogLevel log_level) const = 0;
  virtual void PushLog(const LogMessage& log_message) = 0;
};

class LoggerInitializer {
 public:
  virtual void Init(std::unique_ptr<ThirdPartyLoggerInterface>&& impl) = 0;
};
}  // namespace logger
