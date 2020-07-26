#include "utils/logger/std_logger.h"
#include <iostream>
#include <map>

STDLogger::STDLogger() : enable_(true) {}

void STDLogger::Init(void* unused) {
  std::cout << "std:logger init" << std::endl;
}

void STDLogger::DeInit() {
  std::cout << "std:logger deinit" << std::endl;
}

void STDLogger::Enable() {
  enable_ = true;
}

void STDLogger::Disable() {
  enable_ = false;
}

void STDLogger::Flush() {
  std::cout << "std:logger flusheed" << std::endl;
}

std::string time_to_str(std::chrono::high_resolution_clock::time_point point) {
  using namespace std::chrono;
  static_assert(system_clock::time_point::period::den == 1000000000 &&
                system_clock::time_point::period::num == 1);
  std::string out(29, '0');
  char* buf = &out[0];
  std::time_t now_c = system_clock::to_time_t(point);
  std::strftime(buf, 21, "%Y-%m-%dT%H:%M:%S.", std::localtime(&now_c));
  sprintf(buf + 20, "%09ld", point.time_since_epoch().count() % 1000000000);
  return out;
}

std::string suverity(LogLevel l) {
  std::map<LogLevel, std::string> convert_map;
  convert_map[LogLevel::TRACE_LEVEL] = "trace";
  convert_map[LogLevel::DEBUG_LEVEL] = "debug";
  convert_map[LogLevel::INFO_LEVEL] = "info";
  convert_map[LogLevel::WARNIGN_LEVEL] = "warn";
  convert_map[LogLevel::ERROR_LEVEL] = "error";
  convert_map[LogLevel::FATAL_LEVEL] = "FATAL";
  return convert_map.at(l);
}

void STDLogger::PushLog(const STDLogger::LogMessageImpl& log_message) {
  if (enable_) {
    std::cout << "[" << suverity(log_message.log_level_) << "]"
              << "[" << log_message.logger_ << "]"
              << "[" << log_message.thread_id_ << "]"
              << "[" << time_to_str(log_message.timestamp_) << "]"
              << "[" << log_message.location_ << "]"
              << " " << log_message.log_event_ << std::endl;
  }
}
