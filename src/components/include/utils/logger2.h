#pragma once
#include <log4cxx/logger.h>
#include <string>
#include "utils/ilogger.h"

namespace logger {
class Log4CXXLogger;
}

typedef logger::Log4CXXLogger ExternalLogger;

#define LOCATTION_INFO LOG4CXX_LOCATION

#define CREATE_LOGGERPTR(logger_name)      \
  namespace {                              \
  static std::string logger_(logger_name); \
  }

#define LOG_WITH_LEVEL2(logLevel, logEvent)                               \
  do {                                                                    \
    auto location = LOCATTION_INFO;                                       \
    logger::SDLLogMessage message{                                        \
        logger_,                                                          \
        logLevel,                                                         \
        logEvent,                                                         \
        std::chrono::high_resolution_clock::now(),                        \
        logger::SDLLocationInfo{__FILE__, __PRETTY_FUNCTION__, __LINE__}, \
        std::this_thread::get_id()};                                      \
    logger::Logger<ExternalLogger>::instance().PushLog(message);          \
  } while (false)

#undef LOG4CXX_TRACE
#define LOG4CXX_TRACE(loggerPtr, logEvent) \
  LOG_WITH_LEVEL(loggerPtr, ::log4cxx::Level::getTrace(), logEvent)
