#include <string>
#include <vector>
#include <algorithm>

#include "application_manager/app_launch/device_apps_launcher.h"
#include "application_manager/app_launch/app_launch_data.h"
#include "application_manager/app_launch/apps_launcher.h"
#include "application_manager/resumption/resume_ctrl.h"

#include "utils/timer.h"
#include "utils/timer_task_impl.h"
#include <iostream>

namespace app_launch {
CREATE_LOGGERPTR_GLOBAL(logger_, "AppLaunch")

typedef std::pair<std::string, std::vector<ApplicationDataPtr> > AppsOnDevice;
typedef std::shared_ptr<AppsOnDevice> AppsOnDevicePtr;

class Launcher {
 public:
  Launcher(const resumption::ResumeCtrl& resume_ctrl,
           DeviceAppsLauncher& device_launcher,
           AppsLauncher& apps_launcher)
      : device_launcher_(device_launcher)
      , apps_launcher_(apps_launcher)
      , resume_ctrl_(resume_ctrl)
      , gap_between_app_timer_("GapBetweenLaunchTimer",
                               new timer::TimerTaskImpl<Launcher>(
                                   this, &Launcher::OnGapBetweenLaunchExpired))
      , wait_before_launch_timer_(
            "WaitBeforeLainchTimer",
            new timer::TimerTaskImpl<Launcher>(this, &Launcher::LaunchNext)) {}

  void Start(const AppsOnDevicePtr& apps_on_device) {
    DCHECK(!apps_on_device_);
    apps_on_device_ = apps_on_device;
    const time_t curr_time = time(NULL);
    const time_t sdl_launch_time = resume_ctrl_.LaunchTime();
    const double seconds_from_sdl_start = difftime(curr_time, sdl_launch_time);
    const uint32_t wait_time =
        device_launcher_.settings().resumption_delay_after_ign();
    const uint32_t wait_before_launch_timeout =
        seconds_from_sdl_start < wait_time
            ? wait_time - seconds_from_sdl_start
            : device_launcher_.settings().app_launch_wait_time();
    wait_before_launch_timer_.Start(wait_before_launch_timeout,
                                    timer::kSingleShot);
  }

  void LaunchNext() {
    std::vector<ApplicationDataPtr>& apps = apps_on_device_->second;
    std::vector<ApplicationDataPtr>::iterator it = apps.begin();
    if (it != apps.end()) {
      apps_launcher_.StartLaunching(*it);
      apps.erase(it);
      gap_between_app_timer_.Start(
          device_launcher_.settings().wait_time_between_apps(),
          timer::kSingleShot);
    } else {
      LOG4CXX_DEBUG(logger_,
                    "All Apps on " << apps_on_device_->first
                                   << " postponed launched");
      device_launcher_.StopLaunchingAppsOnDevice(apps_on_device_->first);
    }
  }

  void OnGapBetweenLaunchExpired() {
    LaunchNext();
  }

  void OnAppRegistered(const ApplicationDataPtr& app_data) {
    std::vector<ApplicationDataPtr>& apps = apps_on_device_->second;
    std::vector<ApplicationDataPtr>::iterator it =
        std::find(apps.begin(), apps.end(), app_data);
    if (it != apps.end()) {
      apps.erase(it);
    }
  }

  void Clear() {
    gap_between_app_timer_.Stop();
    wait_before_launch_timer_.Stop();
    apps_on_device_.reset();
  }

  DeviceAppsLauncher& device_launcher_;
  AppsLauncher& apps_launcher_;
  const resumption::ResumeCtrl& resume_ctrl_;

  timer::Timer gap_between_app_timer_;
  timer::Timer wait_before_launch_timer_;

  AppsOnDevicePtr apps_on_device_;
};

typedef std::shared_ptr<Launcher> LauncherPtr;
typedef std::vector<LauncherPtr> Launchers;

struct LauncherGenerator {
  LauncherGenerator(resumption::ResumeCtrl& resume_ctrl,
                    DeviceAppsLauncher& interface,
                    AppsLauncher& apps_launcher)
      : resume_ctrl_(resume_ctrl)
      , interface_(interface)
      , apps_launcher_(apps_launcher) {}

  LauncherPtr operator()() const {
    return std::make_shared<Launcher>(resume_ctrl_, interface_, apps_launcher_);
  }

  resumption::ResumeCtrl& resume_ctrl_;
  DeviceAppsLauncher& interface_;
  AppsLauncher& apps_launcher_;
};

// DeviceAppsLauncherImpl member function definitions
DeviceAppsLauncherImpl::DeviceAppsLauncherImpl(DeviceAppsLauncher& interface,
                                               AppsLauncher& apps_launcher)
    : interface_(interface) {
  sync_primitives::AutoLock lock(launchers_lock_);
  LauncherGenerator generate(
      interface.app_mngr_.resume_controller(), interface, apps_launcher);
  free_launchers_.reserve(interface.settings_.max_number_of_ios_device());
  std::generate_n(std::back_inserter(free_launchers_),
                  interface.settings_.max_number_of_ios_device(),
                  generate);
}

bool DeviceAppsLauncherImpl::LauncherFinder::operator()(
    const std::shared_ptr<Launcher>& launcher) const {
  return device_mac_ == launcher->apps_on_device_->first;
}

bool DeviceAppsLauncherImpl::LaunchAppsOnDevice(
    const std::string& device_mac,
    const std::vector<ApplicationDataPtr>& applications_to_launch) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "On Device " << device_mac << " will be launched "
                             << applications_to_launch.size() << " apps");
  AppsOnDevicePtr apps_on_device =
      std::make_shared<AppsOnDevice>(device_mac, applications_to_launch);
  sync_primitives::AutoLock lock(launchers_lock_);
  DCHECK_OR_RETURN(!free_launchers_.empty(), false)
  const Launchers::iterator it = free_launchers_.begin();
  LauncherPtr launcher = *it;
  works_launchers_.push_back(launcher);
  free_launchers_.erase(it);
  launcher->Start(apps_on_device);
  return true;
}

bool DeviceAppsLauncherImpl::StopLaunchingAppsOnDevice(
    const std::string& device_mac) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(launchers_lock_);
  const Launchers::iterator it = std::find_if(works_launchers_.begin(),
                                              works_launchers_.end(),
                                              LauncherFinder(device_mac));
  if (it == works_launchers_.end()) {
    return false;
  }
  LauncherPtr launcher = *it;
  launcher->Clear();
  free_launchers_.push_back(launcher);
  works_launchers_.erase(it);
  return true;
}

bool DeviceAppsLauncher::LaunchAppsOnDevice(
    const std::string& device_mac,
    const std::vector<ApplicationDataPtr>& applications_to_launch) {
  return impl_->LaunchAppsOnDevice(device_mac, applications_to_launch);
}

DeviceAppsLauncher::DeviceAppsLauncher(
    application_manager::ApplicationManager& app_mngr,
    app_launch::AppsLauncher& apps_launcher,
    const AppLaunchSettings& settings)
    : app_mngr_(app_mngr)
    , settings_(settings)
    , impl_(new DeviceAppsLauncherImpl(*this, apps_launcher)) {}

bool DeviceAppsLauncher::StopLaunchingAppsOnDevice(
    const std::string& device_mac) {
  return impl_->StopLaunchingAppsOnDevice(device_mac);
}

const AppLaunchSettings& DeviceAppsLauncher::settings() const {
  return settings_;
}

}  // namespace app_launch
