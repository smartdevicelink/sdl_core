#ifndef SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_SETTINGS_H_

#include <stdint.h>
#include <string>
namespace app_launch {
class AppLaunchSettings {
 public:
  virtual const uint16_t app_launch_wait_time() const = 0;
  virtual const uint16_t app_launch_max_retry_attempt() const = 0;
  virtual const uint16_t app_launch_retry_wait_time() const = 0;
  virtual const uint16_t remove_bundle_id_attempts() const = 0;
  virtual const uint16_t max_number_of_ios_device() const = 0;
  virtual const uint16_t wait_time_between_apps() const = 0;
  virtual const bool enable_app_launch_ios() const = 0;
  virtual const uint32_t resumption_delay_after_ign() const = 0;
  virtual const std::string& app_storage_folder() const = 0;
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_SETTINGS_H_
