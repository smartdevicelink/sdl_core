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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_LOGGER_IMPL_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_LOGGER_IMPL_H_

#include "utils/ilogger.h"

#include <functional>

#include "utils/threads/message_loop_thread.h"

namespace logger {

typedef std::queue<LogMessage> LogMessageQueue;

typedef threads::MessageLoopThread<LogMessageQueue>
    LogMessageLoopThreadTemplate;

template <typename T>
using LoopThreadPtr = std::unique_ptr<T, std::function<void(T*)> >;

class LogMessageLoopThread : public LogMessageLoopThreadTemplate,
                             public LogMessageLoopThreadTemplate::Handler {
 public:
  LogMessageLoopThread(std::function<void(LogMessage)> handler);

  void Push(const LogMessage& message);

  void Handle(const LogMessage message);

  ~LogMessageLoopThread();

 private:
  std::function<void(LogMessage)> force_log_;
};

class LoggerImpl : public Logger, public LoggerInitializer {
 public:
  LoggerImpl(bool use_message_loop_thread = true);
  void Init(std::unique_ptr<ThirdPartyLoggerInterface>&& impl) override;
  void DeInit() override;
  void Flush() override;

  bool IsEnabledFor(const std::string& component,
                    LogLevel log_level) const override;
  void PushLog(const LogMessage& log_message) override;
  void InitLoggerSettings(
      std::unique_ptr<const LoggerSettings>& settings) override;
  void InitFlushLogsTimePoint(const TimePoint& time_point) override;

  std::unique_ptr<ThirdPartyLoggerInterface> impl_;
  std::unique_ptr<const LoggerSettings> settings_;
  LoopThreadPtr<LogMessageLoopThread> loop_thread_;
  bool use_message_loop_thread_;
  TimePoint flush_logs_time_point_;
};

}  // namespace logger

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_LOGGER_IMPL_H_
