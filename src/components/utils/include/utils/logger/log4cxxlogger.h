#ifndef LOG4CXXLOGGER_H
#define LOG4CXXLOGGER_H
#include <log4cxx/logger.h>
#include "utils/ilogger.h"

class Log4CXXLogger {
 public:
  typedef log4cxx::spi::LocationInfo LocationInfo;
  Log4CXXLogger(std::string filename);
  void Init();
  void DeInit() {}
  void Enable() {}
  bool Enabled() {
    return true;
  }
  void Disable() {}
  void Flush() {}
  void PushLog(LogMessage<LocationInfo> log_message);
  std::string filename_;
};

#endif  // LOG4CXXLOGGER_H
