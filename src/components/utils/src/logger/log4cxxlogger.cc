#include "utils/logger/log4cxxlogger.h"
#include "assert.h"
#include "log4cxx/propertyconfigurator.h"

Log4CXXLogger::Log4CXXLogger(std::string filename) : filename_(filename) {}

void Log4CXXLogger::Init() {
  log4cxx::PropertyConfigurator::configure(filename_);
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
void Log4CXXLogger::PushLog(LogMessage<LocationInfo> log_message) {
  log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(log_message.logger_);

  logger->forcedLog(getLogLevel(log_message.log_level_),
                    log_message.log_event_,
                    log_message.location_);
}
