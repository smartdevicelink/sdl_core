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
#ifndef SRC_COMPONENTS_INCLUDE_UTILS_LOGGER_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_LOGGER_H_

#ifdef ENABLE_LOG

#include <cerrno>
#include <string>
#include <sstream>

#if defined(LOG4CXX_LOGGER)
#include <log4cxx/logger.h>
#endif

#include "utils/pimpl.h"

namespace logger {

namespace LogLevel {
enum Type { LL_TRACE, LL_DEBUG, LL_INFO, LL_WARN, LL_ERROR, LL_FATAL };
}  // namespace LogLevel

struct LogLocation {
  LogLocation() {}
  LogLocation(const char* file_name,
              const char* function_name,
              unsigned long line_number)
      : file_name_(file_name)
      , function_name_(function_name)
      , line_number_(line_number) {}

  const char* file_name_;
  const char* function_name_;
  unsigned long line_number_;
};

#if defined(LOG4CXX_LOGGER)
typedef log4cxx::LoggerPtr LoggerType;
#else
typedef std::string LoggerType;
#endif

class Logger {
 public:
  class Impl;
  typedef utils::Pimpl<Impl, utils::CopyAssigner<Impl> > Pimpl;

  static bool InitLogger(const bool logs_enabled,
                         const std::string& ini_file_name);
  static bool InitLogger(const bool logs_enabled,
                         const LogLevel::Type log_level,
                         const std::string& log_file_name);
  static void DeinitLogger();

  static void FlushLogger();

  static bool logs_enabled();
  static void set_logs_enabled(const bool state);

  static LogLevel::Type log_level();
  static void set_log_level(const LogLevel::Type level);

  static bool PushLog(const LoggerType& logger,
                      const LogLevel::Type level,
                      const std::string& entry,
                      const LogLocation& location);

  static void SetLogger(Pimpl& impl);
  static Pimpl& GetLogger();

 private:
  Logger();
  Logger(const Logger&);
  Logger& operator=(const Logger&);
  ~Logger();

  static Pimpl impl_;
};

}  // namespace logger

#undef INIT_LOGGER
#if defined(LOG4CXX_LOGGER)
#define INIT_LOGGER(logs_enabled) \
  logger::Logger::InitLogger(logs_enabled, "log4cxx.properties");
#else
#define INIT_LOGGER(logs_enabled) \
  logger::Logger::InitLogger(     \
      logs_enabled, logger::LogLevel::LL_TRACE, "SmartDeviceLink.log");
#endif

#undef DEINIT_LOGGER
#define DEINIT_LOGGER() logger::Logger::DeinitLogger()

#undef FLUSH_LOGGER
#define FLUSH_LOGGER() logger::Logger::FlushLogger()

#undef SET_LOGGER
#define SET_LOGGER(logger) logger::Logger::SetLogger(logger)

#undef GET_LOGGER
#define GET_LOGGER() logger::Logger::GetLogger()

#undef CREATE_LOGGERPTR_LOCAL
#if defined(LOG4CXX_LOGGER)
#define CREATE_LOGGERPTR_LOCAL(logger_var, logger_name) \
  logger::LoggerType logger_var(log4cxx::Logger::getLogger(logger_name));
#else
#define CREATE_LOGGERPTR_LOCAL(logger_var, logger_name) \
  logger::LoggerType logger_var(logger_name);
#endif

#undef CREATE_LOGGERPTR_GLOBAL
#define CREATE_LOGGERPTR_GLOBAL(logger_var, logger_name) \
  namespace {                                            \
  CREATE_LOGGERPTR_LOCAL(logger_var, logger_name);       \
  }

#undef LOG_WITH_LEVEL
#define LOG_WITH_LEVEL(logger_var, level, message, line)    \
  do {                                                      \
    std::stringstream accumulator;                          \
    accumulator << message;                                 \
    logger::Logger::PushLog(                                \
        logger_var,                                         \
        level,                                              \
        accumulator.str(),                                  \
        logger::LogLocation(__FILE__, __FUNCTION__, line)); \
  } while (false)

#undef LOG_WITH_LEVEL_EXT
#define LOG_WITH_LEVEL_EXT(logger_var, level, message, location)             \
  do {                                                                       \
    std::stringstream accumulator;                                           \
    accumulator << message;                                                  \
    logger::Logger::PushLog(logger_var, level, accumulator.str(), location); \
  } while (false)

#undef LOGGER_TRACE
#define LOGGER_TRACE(logger_var, message) \
  LOG_WITH_LEVEL(logger_var, logger::LogLevel::LL_TRACE, message, __LINE__)

#undef LOGGER_DEBUG
#define LOGGER_DEBUG(logger_var, message) \
  LOG_WITH_LEVEL(logger_var, logger::LogLevel::LL_DEBUG, message, __LINE__)

#undef LOGGER_INFO
#define LOGGER_INFO(logger_var, message) \
  LOG_WITH_LEVEL(logger_var, logger::LogLevel::LL_INFO, message, __LINE__)

#undef LOGGER_WARN
#define LOGGER_WARN(logger_var, message) \
  LOG_WITH_LEVEL(logger_var, logger::LogLevel::LL_WARN, message, __LINE__)

#undef LOGGER_ERROR
#define LOGGER_ERROR(logger_var, message) \
  LOG_WITH_LEVEL(logger_var, logger::LogLevel::LL_ERROR, message, __LINE__)

#undef LOGGER_FATAL
#define LOGGER_FATAL(logger_var, message) \
  LOG_WITH_LEVEL(logger_var, logger::LogLevel::LL_FATAL, message, __LINE__)

#undef LOGGER_WARN_WITH_ERRNO
#define LOGGER_WARN_WITH_ERRNO(logger_var, message)                          \
  LOGGER_WARN(logger_var,                                                    \
              message << ", error code " << errno << " (" << strerror(errno) \
                      << ")")

#undef LOGGER_ERROR_WITH_ERRNO
#define LOGGER_ERROR_WITH_ERRNO(logger_var, message)                          \
  LOGGER_ERROR(logger_var,                                                    \
               message << ", error code " << errno << " (" << strerror(errno) \
                       << ")")

namespace logger {

class AutoTrace {
 public:
  AutoTrace(const LoggerType& logger, const LogLocation& location)
      : logger_(logger), location_(location) {
    LOG_WITH_LEVEL_EXT(logger_, LogLevel::LL_TRACE, "Enter", location_);
  }

  ~AutoTrace() {
    LOG_WITH_LEVEL_EXT(logger_, LogLevel::LL_TRACE, "Exit", location_);
  }

 private:
  const LoggerType logger_;
  const LogLocation location_;
};

}  // namespace logger

#undef LOGGER_AUTO_TRACE
#define LOGGER_AUTO_TRACE(logger_var) \
  logger::AutoTrace auto_trace(       \
      logger_var, logger::LogLocation(__FILE__, __FUNCTION__, __LINE__));

#else  // ENABLE_LOG is OFF

#undef INIT_LOGGER
#define INIT_LOGGER(ini_file_name)

#undef DEINIT_LOGGER
#define DEINIT_LOGGER()

#undef FLUSH_LOGGER
#define FLUSH_LOGGER()

#undef SET_LOGGER
#define SET_LOGGER(logger)

#undef GET_LOGGER
#define GET_LOGGER()

#undef CREATE_LOGGERPTR_LOCAL
#define CREATE_LOGGERPTR_LOCAL(logger_var, logger_name)

#undef CREATE_LOGGERPTR_GLOBAL
#define CREATE_LOGGERPTR_GLOBAL(logger_var, logger_name)

#undef LOG_WITH_LEVEL
#define LOG_WITH_LEVEL(logger_var, level, message, line)

#undef LOG_WITH_LEVEL_EXT
#define LOG_WITH_LEVEL_EXT(logger_var, level, message, file, function, line)

#undef LOGGER_TRACE
#define LOGGER_TRACE(logger_var, message)

#undef LOGGER_DEBUG
#define LOGGER_DEBUG(logger_var, message)

#undef LOGGER_INFO
#define LOGGER_INFO(logger_var, message)

#undef LOGGER_WARN
#define LOGGER_WARN(logger_var, message)

#undef LOGGER_ERROR
#define LOGGER_ERROR(logger_var, message)

#undef LOGGER_FATAL
#define LOGGER_FATAL(logger_var, message)

#undef LOGGER_WARN_WITH_ERRNO
#define LOGGER_WARN_WITH_ERRNO(logger_var, message)

#undef LOGGER_ERROR_WITH_ERRNO
#define LOGGER_ERROR_WITH_ERRNO(logger_var, message)

#undef LOGGER_AUTO_TRACE
#define LOGGER_AUTO_TRACE(logger_var)

#endif  // ENABLE_LOG

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_LOGGER_H_
