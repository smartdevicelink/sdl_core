#include "utils/logger/log4cxxlogger.h"
#include <apr_pools.h>
#include <apr_thread_proc.h>
#include <atomic>
#include <sstream>
#include "assert.h"
#include "log4cxx/propertyconfigurator.h"

namespace logger {

Log4CXXLogger::Log4CXXLogger(std::string filename) : filename_(filename) {}

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
  for (log4cxx::LoggerList::iterator i = loggers.begin(); i != loggers.end();
       ++i) {
    log4cxx::LoggerPtr logger = *i;
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
    case LogLevel::WARNIGN_LEVEL:
      return log4cxx::Level::getWarn();
    case LogLevel::ERROR_LEVEL:
      return log4cxx::Level::getError();
    case LogLevel::FATAL_LEVEL:
      return log4cxx::Level::getFatal();
    default:
      assert(false);
  }
}

bool Log4CXXLogger::IsEnabledFor(const std::string& logger_name,
                                 LogLevel log_level) {
  log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(logger_name);
  logger->isEnabledFor(getLogLevel(log_level));
  return true;
}

void Log4CXXLogger::PushLog(SDLLogMessage log_message) {
  log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(log_message.logger_);

  std::ostringstream ss;
  ss << log_message.thread_id_;

  logger->forcedLog(getLogLevel(log_message.log_level_),
                    log_message.log_event_,
                    std::chrono::duration_cast<std::chrono::microseconds>(
                        log_message.timestamp_.time_since_epoch())
                        .count() /*apr_time_now()*/,
                    LocationInfo(log_message.location_.file_name.c_str(),
                                 log_message.location_.function_name.c_str(),
                                 log_message.location_.line_number),
                    ss.str());
}

}  // namespace logger
