#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_SETTINGS_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_SETTINGS_H_

#include <stdint.h>

namespace logger {
class LoggerSettings {
 public:
  virtual const bool flush_log_messages_before_shutdown() const = 0;
  virtual const uint16_t max_time_before_shutdown() const = 0;
};

}  // namespace logger

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_SETTINGS_H_
