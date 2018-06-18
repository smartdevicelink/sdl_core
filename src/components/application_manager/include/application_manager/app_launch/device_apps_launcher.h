#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_DEVICE_APPS_LAUNCHER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_DEVICE_APPS_LAUNCHER_H_
#include <stdint.h>
#include <vector>
#include <string>
#include <memory>
#include "application_manager/app_launch/app_launch_data.h"
#include "application_manager/application_manager.h"
#include "application_manager/app_launch_settings.h"

namespace app_launch {
class AppLaunchCtrlImpl;
class DeviceAppsLauncherImpl;
class AppsLauncher;
class AppLaunchSettings;
/**
 * @brief The MultipleAppsLauncher struct
 * should manage launching applications and gaps between launching application
 * on one device
 * When all apps launched it will notify AppLaunchCtrlImpl that all apps
 * launched
 */
class DeviceAppsLauncher {
 public:
  DeviceAppsLauncher(application_manager::ApplicationManager& app_mngr,
                     app_launch::AppsLauncher& apps_launcher,
                     const AppLaunchSettings& settings);

  bool LaunchAppsOnDevice(
      const std::string& device_mac,
      const std::vector<ApplicationDataPtr>& applications_to_launch);
  bool StopLaunchingAppsOnDevice(const std::string& device_mac);

  const AppLaunchSettings& settings() const;

 private:
  application_manager::ApplicationManager& app_mngr_;
  const AppLaunchSettings& settings_;
  std::auto_ptr<DeviceAppsLauncherImpl> impl_;
  friend class DeviceAppsLauncherImpl;
  DISALLOW_COPY_AND_ASSIGN(DeviceAppsLauncher);
};

}  // namespace app_launch

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_DEVICE_APPS_LAUNCHER_H_
