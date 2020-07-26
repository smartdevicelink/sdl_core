#ifndef ILOGGER_H
#define ILOGGER_H
#pragma once
#include <chrono>
#include <string>
#include <thread>
enum class LogLevel { TRACE_LEVEL, DEBUG_LEVEL, INFO_LEVEL, WARNIGN_LEVEL, ERROR_LEVEL, FATAL_LEVEL };

typedef std::chrono::high_resolution_clock::time_point TimePoint;
template <typename LocationInfo>
struct LogMessage {
  std::string logger_;  // <- component_name
  LogLevel log_level_;
  std::string log_event_;
  TimePoint timestamp_;
  //  std::string timestamp_;
  LocationInfo location_;
  std::thread::id thread_id_;
};

template <typename LocationInfo, class ThirdPartyLogger>
class Logger {
 public:
  typedef LogMessage<LocationInfo> LogMessageImpl;

  virtual void Init(ThirdPartyLogger* impl) = 0;
  virtual void DeInit() = 0;
  virtual void Enable() = 0;
  virtual bool Enabled() = 0;
  virtual void Disable() = 0;
  virtual void Flush() = 0;
  virtual void PushLog(const LogMessageImpl& log_message) = 0;
  static Logger<LocationInfo, ThirdPartyLogger>& instance();
};

#endif  // ILOGGER_H
