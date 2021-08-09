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

#include "utils/logger/logger_impl.h"
#include <assert.h>
#include <iostream>

namespace logger {

Logger* Logger::instance_ = nullptr;

LoggerImpl::LoggerImpl(bool use_message_loop_thread)
    : impl_(nullptr), use_message_loop_thread_(use_message_loop_thread) {}

void LoggerImpl::Init(std::unique_ptr<ThirdPartyLoggerInterface>&& impl) {
  assert(impl_ == nullptr);
  impl_ = std::move(impl);

  impl_->Init();

  if (use_message_loop_thread_) {
    auto deinit_logger = [](LogMessageLoopThread* logMsgThread) {
      delete logMsgThread;
    };

    if (!loop_thread_) {
      loop_thread_ = LoopThreadPtr<LogMessageLoopThread>(
          new LogMessageLoopThread(
              [this](LogMessage message) { impl_->PushLog(message); }),
          deinit_logger);
    }
  }
}

void LoggerImpl::DeInit() {
  if (use_message_loop_thread_) {
    Flush();
    loop_thread_.reset();
  }

  if (impl_) {
    impl_->DeInit();
  }

  instance_ = nullptr;
}

void LoggerImpl::Flush() {
  if (use_message_loop_thread_) {
    if (loop_thread_) {
      loop_thread_->WaitDumpQueue();
    }
  }
}

bool LoggerImpl::IsEnabledFor(const std::string& component,
                              LogLevel log_level) const {
  return impl_ ? impl_->IsEnabledFor(component, log_level) : false;
}

void LoggerImpl::PushLog(const LogMessage& log_message) {
  if (impl_) {
    if (use_message_loop_thread_) {
      if (loop_thread_) {
        loop_thread_->Push(log_message);
      }
    } else {
      impl_->PushLog(log_message);
    }
  }
}

Logger& Logger::instance(Logger* pre_init) {
  if (pre_init) {
    assert(instance_ == nullptr);
    instance_ = pre_init;
  }
  return *instance_;
}

LogMessageLoopThread::LogMessageLoopThread(
    std::function<void(LogMessage)> handler)
    : LogMessageLoopThreadTemplate("Logger", this), force_log_(handler) {}

void LogMessageLoopThread::Push(const LogMessage& message) {
  PostMessage(message);
}

void LogMessageLoopThread::Handle(const LogMessage message) {
  force_log_(message);
}

LogMessageLoopThread::~LogMessageLoopThread() {
  // we'll have to drop messages
  // while deleting logger thread
  LogMessageLoopThreadTemplate::Shutdown();
}

}  // namespace logger
