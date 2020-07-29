#ifndef LOG4CXXLOGGER_H
#define LOG4CXXLOGGER_H
#include <log4cxx/logger.h>
#include "utils/ilogger.h"

namespace logger {

class Log4CXXLogger {
 public:
  typedef log4cxx::spi::LocationInfo LocationInfo;
  Log4CXXLogger(std::string filename);
  void Init();
  void DeInit();
  bool IsEnabledFor(const std::string& logger_name, LogLevel log_level);
  void PushLog(SDLLogMessage log_message);

 private:
  std::string filename_;
};

}  // namespace logger

#endif  // LOG4CXXLOGGER_H
