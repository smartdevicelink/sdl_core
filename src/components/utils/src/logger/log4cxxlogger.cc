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

#include "utils/logger/log4cxxlogger.h"
#include <apr_pools.h>
#include <apr_thread_proc.h>
#include <atomic>
#include <sstream>
#include "assert.h"
#include "log4cxx/propertyconfigurator.h"

namespace logger {

Log4CXXLogger::Log4CXXLogger(const std::string& filename)
    : filename_(filename) {}

void Log4CXXLogger::Init() {
  log4cxx::PropertyConfigurator::configure(filename_);
}

void Log4CXXLogger::DeInit() {
  log4cxx::LoggerPtr rootLogger = log4cxx::Logger::getRootLogger();

  if (LOG4CXX_UNLIKELY(rootLogger->isDebugEnabled())) {
    ::log4cxx::helpers::MessageBuffer oss_;
    rootLogger->forcedLog(::log4cxx::Level::getDebug(),
                          oss_.str(oss_ << "Logger loop thread deinitialized"),
                          LOG4CXX_LOCATION);
  }

  if (LOG4CXX_UNLIKELY(rootLogger->isDebugEnabled())) {
    ::log4cxx::helpers::MessageBuffer oss_;
    rootLogger->forcedLog(::log4cxx::Level::getDebug(),
                          oss_.str(oss_ << "Logger calling removeAllAppenders"),
                          LOG4CXX_LOCATION);
  }

  log4cxx::spi::LoggerRepositoryPtr repository =
      rootLogger->getLoggerRepository();
  log4cxx::LoggerList loggers = repository->getCurrentLoggers();
  for (auto logger : loggers) {
    logger->removeAllAppenders();
  }

  rootLogger->removeAllAppenders();
}

log4cxx::LevelPtr getLogLevel(LogLevel log_level) {
  switch (log_level) {
    case LogLevel::TRACE_LEVEL:
      return log4cxx::Level::getTrace();
    case LogLevel::DEBUG_LEVEL:
      return log4cxx::Level::getDebug();
    case LogLevel::INFO_LEVEL:
      return log4cxx::Level::getInfo();
    case LogLevel::WARNING_LEVEL:
      return log4cxx::Level::getWarn();
    case LogLevel::ERROR_LEVEL:
      return log4cxx::Level::getError();
    case LogLevel::FATAL_LEVEL:
      return log4cxx::Level::getFatal();
    default:
      assert(false);
      return log4cxx::Level::getTrace();
  }
}

bool Log4CXXLogger::IsEnabledFor(const std::string& component,
                                 LogLevel log_level) const {
  log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(component);
  return logger->isEnabledFor(getLogLevel(log_level));
}

void Log4CXXLogger::PushLog(const LogMessage& log_message) {
  log4cxx::LoggerPtr logger =
      log4cxx::Logger::getLogger(log_message.component_);

  std::ostringstream ss;
  ss << log_message.thread_id_;

  auto time = std::chrono::duration_cast<std::chrono::microseconds>(
                  log_message.timestamp_.time_since_epoch())
                  .count();
  auto location =
      log4cxx::spi::LocationInfo(log_message.location_.file_name.c_str(),
                                 log_message.location_.function_name.c_str(),
                                 log_message.location_.line_number);
  logger->forcedLog(getLogLevel(log_message.log_level_),
                    log_message.log_event_,
                    time,
                    location,
                    ss.str());
}

}  // namespace logger
