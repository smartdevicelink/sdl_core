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
class DeviceAppsLauncher;
class AppsLauncher;
class AppLaunchSettings;
class Launcher;
class LauncherGenerator;

// impl class must be defined for unique_ptr in DeviceAppsLauncher
class DeviceAppsLauncherImpl {
 public:
  DeviceAppsLauncherImpl(DeviceAppsLauncher& interface,
                         AppsLauncher& apps_launcher);

  bool LaunchAppsOnDevice(
      const std::string& device_mac,
      const std::vector<ApplicationDataPtr>& applications_to_launch);

  struct LauncherFinder {
    LauncherFinder(const std::string& device_mac) : device_mac_(device_mac) {}

    bool operator()(const utils::SharedPtr<Launcher>& launcher) const;

    std::string device_mac_;
  };

  bool StopLaunchingAppsOnDevice(const std::string& device_mac);

 private:
  sync_primitives::Lock launchers_lock_;
  std::vector<utils::SharedPtr<Launcher> > free_launchers_;
  std::vector<utils::SharedPtr<Launcher> > works_launchers_;
  DeviceAppsLauncher& interface_;
};

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
  std::unique_ptr<DeviceAppsLauncherImpl> impl_;
  friend class DeviceAppsLauncherImpl;
  DISALLOW_COPY_AND_ASSIGN(DeviceAppsLauncher);
};

}  // namespace app_launch

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_DEVICE_APPS_LAUNCHER_H_
