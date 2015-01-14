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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_H_

#ifdef ENABLE_LOG
  #include <errno.h>
  #include <string.h>
  #include <sstream>
  #include <apr_time.h>
  #include <log4cxx/propertyconfigurator.h>
  #include <log4cxx/spi/loggingevent.h>
  #include "utils/push_log.h"
  #include "utils/logger_status.h"
#endif  // ENABLE_LOG

#ifdef ENABLE_LOG

    #define CREATE_LOGGERPTR_GLOBAL(logger_var, logger_name) \
      namespace { \
        CREATE_LOGGERPTR_LOCAL(logger_var, logger_name); \
      }

    #define CREATE_LOGGERPTR_LOCAL(logger_var, logger_name) \
      log4cxx::LoggerPtr logger_var = log4cxx::LoggerPtr(log4cxx::Logger::getLogger(logger_name));

    #define INIT_LOGGER(file_name) \
      log4cxx::PropertyConfigurator::configure(file_name);

    // without this line log4cxx threads continue using some instances destroyed by exit()
    #define DEINIT_LOGGER() \
      log4cxx::Logger::getRootLogger()->closeNestedAppenders();

    #define LOG4CXX_IS_TRACE_ENABLED(logger) logger->isTraceEnabled()

    #define LOG_WITH_LEVEL(loggerPtr, logLevel, logEvent) \
    do { \
      if (logger::logger_status != logger::DeletingLoggerThread) { \
        if (loggerPtr->isEnabledFor(logLevel)) { \
          std::stringstream accumulator; \
          accumulator << logEvent; \
          logger::push_log(loggerPtr, logLevel, accumulator.str(), apr_time_now(), LOG4CXX_LOCATION, ::log4cxx::spi::LoggingEvent::getCurrentThreadName()); \
        } \
      } \
    } while (false)

    #undef LOG4CXX_INFO
    #define LOG4CXX_INFO(loggerPtr, logEvent) LOG_WITH_LEVEL(loggerPtr, ::log4cxx::Level::getInfo(), logEvent)

    #define LOG4CXX_INFO_EXT(logger, logEvent) LOG4CXX_INFO(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_INFO_STR_EXT(logger, logEvent) LOG4CXX_INFO_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #define LOG4CXX_TRACE_EXT(logger, logEvent) LOG4CXX_TRACE(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_TRACE_STR_EXT(logger, logEvent) LOG4CXX_TRACE_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #undef LOG4CXX_DEBUG
    #define LOG4CXX_DEBUG(loggerPtr, logEvent) LOG_WITH_LEVEL(loggerPtr, ::log4cxx::Level::getDebug(), logEvent)

    #define LOG4CXX_DEBUG_EXT(logger, logEvent) LOG4CXX_DEBUG(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_DEBUG_STR_EXT(logger, logEvent) LOG4CXX_DEBUG_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #undef LOG4CXX_WARN
    #define LOG4CXX_WARN(loggerPtr, logEvent) LOG_WITH_LEVEL(loggerPtr, ::log4cxx::Level::getWarn(), logEvent)

    #define LOG4CXX_WARN_EXT(logger, logEvent) LOG4CXX_WARN(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_WARN_STR_EXT(logger, logEvent) LOG4CXX_WARN_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #undef LOG4CXX_ERROR
    #define LOG4CXX_ERROR(loggerPtr, logEvent) LOG_WITH_LEVEL(loggerPtr, ::log4cxx::Level::getError(), logEvent)

    #define LOG4CXX_ERROR_EXT(logger, logEvent) LOG4CXX_ERROR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_ERROR_STR_EXT(logger, logEvent) LOG4CXX_ERROR_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #undef LOG4CXX_FATAL
    #define LOG4CXX_FATAL(loggerPtr, logEvent) LOG_WITH_LEVEL(loggerPtr, ::log4cxx::Level::getFatal(), logEvent)

    #define LOG4CXX_FATAL_EXT(logger, logEvent) LOG4CXX_FATAL(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_FATAL_STR_EXT(logger, logEvent) LOG4CXX_FATAL_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #undef LOG4CXX_TRACE
    #define LOG4CXX_TRACE(loggerPtr, logEvent) LOG_WITH_LEVEL(loggerPtr, ::log4cxx::Level::getTrace(), logEvent)

    #define LOG4CXX_TRACE_ENTER(logger) LOG4CXX_TRACE(logger, "ENTER: " << __PRETTY_FUNCTION__ )
    #define LOG4CXX_TRACE_EXIT(logger) LOG4CXX_TRACE(logger, "EXIT: " << __PRETTY_FUNCTION__ )

    #define LOG4CXX_ERROR_WITH_ERRNO(logger, message) \
      LOG4CXX_ERROR(logger, message << ", error code " << errno << " (" << strerror(errno) << ")")

     #define LOG4CXX_WARN_WITH_ERRNO(logger, message) \
       LOG4CXX_WARN(logger, message << ", error code " << errno << " (" << strerror(errno) << ")")

#else // ENABLE_LOG is OFF

    #define CREATE_LOGGERPTR_GLOBAL(logger_var, logger_name)

    #define CREATE_LOGGERPTR_LOCAL(logger_var, logger_name)

    #define INIT_LOGGER(file_name)

    #define DEINIT_LOGGER(file_name)

    #define LOG4CXX_IS_TRACE_ENABLED(logger) false

    #undef LOG4CXX_INFO
    #define LOG4CXX_INFO(x,y)

    #undef LOG4CXX_WARN
    #define LOG4CXX_WARN(x,y)

    #undef LOG4CXX_ERROR
    #define LOG4CXX_ERROR(x,y)

    #undef LOG4CXX_ERROR_WITH_ERRNO
    #define LOG4CXX_ERROR_WITH_ERRNO(x,y)

    #undef LOG4CXX_WARN_WITH_ERRNO
    #define LOG4CXX_WARN_WITH_ERRNO(x,y)

    #undef LOG4CXX_TRACE
    #define LOG4CXX_TRACE(x,y)

    #undef LOG4CXX_DEBUG
    #define LOG4CXX_DEBUG(x,y)

    #undef LOG4CXX_FATAL
    #define LOG4CXX_FATAL(x,y)

    #define LOG4CXX_INFO_EXT(logger, logEvent)
    #define LOG4CXX_INFO_STR_EXT(logger, logEvent)

    #define LOG4CXX_TRACE_EXT(logger, logEvent)
    #define LOG4CXX_TRACE_STR_EXT(logger, logEvent)

    #define LOG4CXX_DEBUG_EXT(logger, logEvent)
    #define LOG4CXX_DEBUG_STR_EXT(logger, logEvent)

    #define LOG4CXX_WARN_EXT(logger, logEvent)
    #define LOG4CXX_WARN_STR_EXT(logger, logEvent)

    #define LOG4CXX_ERROR_EXT(logger, logEvent)
    #define LOG4CXX_ERROR_STR_EXT(logger, logEvent)

    #define LOG4CXX_FATAL_EXT(logger, logEvent)
    #define LOG4CXX_FATAL_STR_EXT(logger, logEvent)

    #define LOG4CXX_TRACE_ENTER(logger)
    #define LOG4CXX_TRACE_EXIT(logger)
#endif  // ENABLE_LOG

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_H_
