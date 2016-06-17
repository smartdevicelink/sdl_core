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
#include "appenders_loader.h"
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

class LoggerAutoPtr {
 public:
  LoggerAutoPtr(const bool logs_enabled) {
#if defined(LOG4CXX_LOGGER)
    if (logs_enabled && utils::appenders_loader.Loaded()) {
      logger::Logger::InitLogger(logs_enabled, "log4cxx.properties");
    }
#else
    logger::Logger::InitLogger(
        logs_enabled, logger::LogLevel::LL_TRACE, "SmartDeviceLink.log");
#endif
  }
  ~LoggerAutoPtr() {
    logger::Logger::DeinitLogger();
  }
};

}  // namespace logger

#undef SDL_EMPTY_WHILE
#define SDL_EMPTY_WHILE(body) \
  do {                        \
    body                      \
  } while (false);

#undef SDL_FLUSH_LOGGER
#define SDL_FLUSH_LOGGER() SDL_EMPTY_WHILE(logger::Logger::FlushLogger();)

#undef SDL_SET_LOGGER
#define SET_SDL_LOGGER(logger) \
  SDL_EMPTY_WHILE(logger::Logger::SetLogger(logger);)

#undef SDL_GET_LOGGER
#define SDL_GET_LOGGER() logger::Logger::GetLogger()

#undef SDL_CREATE_LOGGER
#if defined(LOG4CXX_LOGGER)
#define SDL_CREATE_LOGGER(logger_name)         \
  static logger::LoggerType logger_module_ptr( \
      log4cxx::Logger::getLogger(logger_name));
#else
#define SDL_CREATE_LOGGER(logger_name) \
  static logger::LoggerType logger_module_ptr(logger_name);
#endif

#undef SDL_LOG_WITH_LEVEL
#define SDL_LOG_WITH_LEVEL(logger_var, level, message, line) \
  do {                                                       \
    std::stringstream accumulator;                           \
    accumulator << message;                                  \
    logger::Logger::PushLog(                                 \
        logger_var,                                          \
        level,                                               \
        accumulator.str(),                                   \
        logger::LogLocation(__FILE__, __FUNCTION__, line));  \
  } while (false)

#undef SDL_LOG_WITH_LEVEL_EXT
#define SDL_LOG_WITH_LEVEL_EXT(logger_var, level, message, location)         \
  do {                                                                       \
    std::stringstream accumulator;                                           \
    accumulator << message;                                                  \
    logger::Logger::PushLog(logger_var, level, accumulator.str(), location); \
  } while (false)

#undef SDL_LOG_MESSAGE
#define SDL_LOG_MESSAGE(log_level, message) \
  SDL_LOG_WITH_LEVEL(                       \
      logger_module_ptr, logger::LogLevel::log_level, message, __LINE__)

#undef SDL_TRACE
#define SDL_TRACE(message) SDL_LOG_MESSAGE(LL_TRACE, message)

#undef SDL_DEBUG
#define SDL_DEBUG(message) SDL_LOG_MESSAGE(LL_DEBUG, message)

#undef SDL_INFO
#define SDL_INFO(message) SDL_LOG_MESSAGE(LL_INFO, message)

#undef SDL_WARN
#define SDL_WARN(message) SDL_LOG_MESSAGE(LL_WARN, message)

#undef SDL_ERROR
#define SDL_ERROR(message) SDL_LOG_MESSAGE(LL_ERROR, message)

#undef SDL_FATAL
#define SDL_FATAL(message) SDL_LOG_MESSAGE(LL_FATAL, message)

#undef SDL_WARN_WITH_ERRNO
#define SDL_WARN_WITH_ERRNO(message)                                      \
  SDL_WARN(message << ", error code " << errno << " (" << strerror(errno) \
                   << ")")

#undef SDL_ERROR_WITH_ERRNO
#define SDL_ERROR_WITH_ERRNO(message)                                      \
  SDL_ERROR(message << ", error code " << errno << " (" << strerror(errno) \
                    << ")")

namespace logger {

class AutoTrace {
 public:
  AutoTrace(const LoggerType& logger, const LogLocation& location)
      : logger_(logger), location_(location) {
    SDL_LOG_WITH_LEVEL_EXT(logger_, LogLevel::LL_TRACE, "Enter", location_);
  }

  ~AutoTrace() {
    SDL_LOG_WITH_LEVEL_EXT(logger_, LogLevel::LL_TRACE, "Exit", location_);
  }

 private:
  const LoggerType logger_;
  const LogLocation location_;
};

}  // namespace logger

#undef SDL_AUTO_TRACE
#define SDL_AUTO_TRACE()        \
  logger::AutoTrace auto_trace( \
      logger_module_ptr,        \
      logger::LogLocation(__FILE__, __FUNCTION__, __LINE__));

#else  // ENABLE_LOG is OFF

#undef SDL_FLUSH_LOGGER
#define SDL_FLUSH_LOGGER()

#undef SDL_SET_LOGGER
#define SDL_SET_LOGGER(logger)

#undef SDL_GET_LOGGER
#define SDL_GET_LOGGER()

#undef SDL_CREATE_LOGGER
#define SDL_CREATE_LOGGER(logger_name)

#undef SDL_LOG_WITH_LEVEL
#define SDL_LOG_WITH_LEVEL(level, message, line)

#undef SDL_LOG_WITH_LEVEL_EXT
#define SDL_LOG_WITH_LEVEL_EXT(level, message, file, function, line)

#undef SDL_TRACE
#define SDL_TRACE(message)

#define SDL_AUTO_TRACE()

#undef SDL_DEBUG
#define SDL_DEBUG(message)

#undef SDL_INFO
#define SDL_INFO(message)

#undef SDL_WARN
#define SDL_WARN(message)

#undef SDL_ERROR
#define SDL_ERROR(message)

#undef SDL_FATAL
#define SDL_FATAL(message)

#undef SDL_WARN_WITH_ERRNO
#define SDL_WARN_WITH_ERRNO(message)

#undef SDL_ERROR_WITH_ERRNO
#define SDL_ERROR_WITH_ERRNO(message)

#undef SDL_AUTO_TRACE
#define SDL_AUTO_TRACE()
#endif  // ENABLE_LOG

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_LOGGER_H_
