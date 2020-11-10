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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_LOGGER_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_LOGGER_H_

#include "utils/ilogger.h"

#ifdef ENABLE_LOG
#include <sstream>
#include <string>

#include "utils/auto_trace.h"
#define SDL_CREATE_LOG_VARIABLE(component_name) \
  namespace {                                   \
  static std::string logger_(component_name);   \
  }

#define SDL_CREATE_LOCAL_LOG_VARIABLE(component_name) \
  std::string logger_(component_name);

// special macros to dump logs from queue
// it's needed, for example, when crash happened
#define SDL_FLUSH_LOGGER() logger::Logger::instance().Flush();

// Logger deinitilization function and macro, need to stop log4cxx writing
// without this deinitilization log4cxx threads continue using some instances
// destroyed by exit()
#define SDL_DEINIT_LOGGER() logger::Logger::instance().DeInit();

#define LOG_WITH_LEVEL(logLevel, logEvent)                               \
  do {                                                                   \
    if (logger::Logger::instance().IsEnabledFor(logger_, logLevel)) {    \
      std::stringstream accumulator;                                     \
      accumulator << logEvent;                                           \
      logger::LogMessage message{                                        \
          logger_,                                                       \
          logLevel,                                                      \
          accumulator.str(),                                             \
          std::chrono::high_resolution_clock::now(),                     \
          logger::LocationInfo{__FILE__, __PRETTY_FUNCTION__, __LINE__}, \
          std::this_thread::get_id()};                                   \
      logger::Logger::instance().PushLog(message);                       \
    }                                                                    \
  } while (false)

#define SDL_LOG_TRACE(logEvent) \
  LOG_WITH_LEVEL(logger::LogLevel::TRACE_LEVEL, logEvent)

#define SDL_LOG_AUTO_TRACE()    \
  logger::AutoTrace auto_trace( \
      logger_, logger::LocationInfo{__FILE__, __PRETTY_FUNCTION__, __LINE__})

#define SDL_LOG_DEBUG(logEvent) \
  LOG_WITH_LEVEL(logger::LogLevel::DEBUG_LEVEL, logEvent)

#define SDL_LOG_INFO(logEvent) \
  LOG_WITH_LEVEL(logger::LogLevel::INFO_LEVEL, logEvent)

#define SDL_LOG_WARN(logEvent) \
  LOG_WITH_LEVEL(logger::LogLevel::WARNING_LEVEL, logEvent)

#define SDL_LOG_ERROR(logEvent) \
  LOG_WITH_LEVEL(logger::LogLevel::ERROR_LEVEL, logEvent)

#define SDL_LOG_FATAL(logEvent) \
  LOG_WITH_LEVEL(logger::LogLevel::FATAL_LEVEL, logEvent)

#define SDL_LOG_ERROR_WITH_ERRNO(message)                                      \
  SDL_LOG_ERROR(message << ", error code " << errno << " (" << strerror(errno) \
                        << ")")

#define SDL_LOG_WARN_WITH_ERRNO(message)                                      \
  SDL_LOG_WARN(message << ", error code " << errno << " (" << strerror(errno) \
                       << ")")

#else  // ENABLE_LOG is OFF
#define SDL_CREATE_LOG_VARIABLE(x)

#define SDL_CREATE_LOCAL_LOG_VARIABLE(x)

#define SDL_DEINIT_LOGGER()

#define SDL_FLUSH_LOGGER()

#define SDL_LOG_TRACE(x)

#define SDL_LOG_AUTO_TRACE()

#define SDL_LOG_DEBUG(x)

#define SDL_LOG_INFO(x)

#define SDL_LOG_WARN(x)

#define SDL_LOG_ERROR(x)

#define SDL_LOG_ERROR_WITH_ERRNO(x)

#define SDL_LOG_WARN_WITH_ERRNO(x)

#define SDL_LOG_FATAL(x)

#endif  // ENABLE_LOG

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_LOGGER_H_
