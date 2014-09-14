/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "utils/log_message_loop_thread.h"

namespace logger {

void LogMessageHandler::Handle(const LogMessage message) {
// these are basically log4cxx macros
// they cannot be used directly here
// because they are shadowed by our redefinitions
// and because location is subject to substitute
  switch (message.level) {
    case Fatal:
      if (message.logger->isFatalEnabled()) {
        ::log4cxx::helpers::MessageBuffer oss_;
        message.logger->forcedLog(::log4cxx::Level::getFatal(), oss_.str(oss_ << message.entry), message.location);
      }
      break;
    case Error:
      if (message.logger->isErrorEnabled()) {
        ::log4cxx::helpers::MessageBuffer oss_;
        message.logger->forcedLog(::log4cxx::Level::getError(), oss_.str(oss_ << message.entry), message.location);
      }
      break;
    case Warn:
      if (message.logger->isWarnEnabled()) {
        ::log4cxx::helpers::MessageBuffer oss_;
        message.logger->forcedLog(::log4cxx::Level::getWarn(), oss_.str(oss_ << message.entry), message.location);
      }
      break;
    case Info:
      if (message.logger->isInfoEnabled()) {
        ::log4cxx::helpers::MessageBuffer oss_;
        message.logger->forcedLog(::log4cxx::Level::getInfo(), oss_.str(oss_ << message.entry), message.location);
      }
      break;
    case Debug:
      if (LOG4CXX_UNLIKELY(message.logger->isDebugEnabled())) {
        ::log4cxx::helpers::MessageBuffer oss_;
        message.logger->forcedLog(::log4cxx::Level::getDebug(), oss_.str(oss_ << message.entry), message.location);
      }
      break;
    case Trace:
      if (LOG4CXX_UNLIKELY(message.logger->isTraceEnabled())) {
        ::log4cxx::helpers::MessageBuffer oss_;
        message.logger->forcedLog(::log4cxx::Level::getTrace(), oss_.str(oss_ << message.entry), message.location);
      }
      break;
  }
}

LogMessageLoopThread::LogMessageLoopThread() :
  LogMessageLoopThreadTemplate("Logger", new LogMessageHandler()) {
}

}  // namespace logger
