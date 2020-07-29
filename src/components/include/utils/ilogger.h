#ifndef ILOGGER_H
#define ILOGGER_H
#pragma once
#include <chrono>
#include <string>
#include <thread>

namespace logger {

enum class LogLevel {
  TRACE_LEVEL,
  DEBUG_LEVEL,
  INFO_LEVEL,
  WARNIGN_LEVEL,
  ERROR_LEVEL,
  FATAL_LEVEL
};

struct SDLLocationInfo {
  std::string file_name;
  std::string function_name;
  int line_number;
};

typedef std::chrono::high_resolution_clock::time_point TimePoint;

struct SDLLogMessage {
  std::string logger_;  // <- component_name
  LogLevel log_level_;
  std::string log_event_;
  TimePoint timestamp_;
  SDLLocationInfo location_;
  std::thread::id thread_id_;
};

template <class ThirdPartyLogger>
class Logger {
 public:
  virtual void Init(ThirdPartyLogger* impl) = 0;
  virtual void DeInit() = 0;
  virtual void Enable() = 0;
  virtual bool Enabled() = 0;
  virtual void Disable() = 0;
  virtual bool IsEnabledFor(const std::string& logger, LogLevel log_level) = 0;
  virtual void Flush() = 0;
  virtual void PushLog(const SDLLogMessage& log_message) = 0;
  static Logger<ThirdPartyLogger>& instance();
};

}  // namespace logger

#endif  // ILOGGER_H
