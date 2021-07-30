#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_SETTINGS_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_SETTINGS_H_

#include <cstdint>

namespace logger {
class LoggerSettings {
 public:
  virtual bool flush_log_messages_before_shutdown() const = 0;
  virtual uint16_t max_time_before_shutdown() const = 0;
};

}  // namespace logger

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_SETTINGS_H_
