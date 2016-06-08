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
#include "utils/macro.h"
#include "utils/log_message_loop_thread.h"

#if defined(LOG4CXX_LOGGER)
#include <log4cxx/logger.h>

namespace {

log4cxx::LevelPtr GetLog4cxxLogLevel(const logger::LogLevel::Type level) {
  using namespace logger;
  switch (level) {
    case LogLevel::LL_TRACE:
      return log4cxx::Level::getTrace();
    case LogLevel::LL_DEBUG:
      return log4cxx::Level::getDebug();
    case LogLevel::LL_INFO:
      return log4cxx::Level::getInfo();
    case LogLevel::LL_WARN:
      return log4cxx::Level::getWarn();
    case LogLevel::LL_ERROR:
      return log4cxx::Level::getError();
    case LogLevel::LL_FATAL:
      return log4cxx::Level::getFatal();
    default:
      NOTREACHED();
  }
}

}  // namespace

namespace logger {

void LogMessageHandler::Handle(const LogMessage message) {
  message.logger_->forcedLog(
      GetLog4cxxLogLevel(message.level_),
      message.entry_,
      message.time_,
      log4cxx::spi::LocationInfo(
          message.location_.file_name_,
          message.location_.function_name_,
          static_cast<int>(message.location_.line_number_)),
      message.thread_name_);
}

}  // namespace logger

#endif  // LOG4CXX_LOGGER
