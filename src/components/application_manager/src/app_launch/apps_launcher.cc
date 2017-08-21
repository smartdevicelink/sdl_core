#include <algorithm>
#include "application_manager/app_launch/apps_launcher.h"
#include "utils/make_shared.h"
#include "utils/timer_task_impl.h"
#include <iostream>

namespace app_launch {
struct LauncherGenerator {
  LauncherGenerator(AppsLauncher& apps_laucnher,
                    connection_handler::ConnectionHandler& connection_handler,
                    const uint16_t app_launch_max_retry_attempt,
                    const uint16_t app_launch_retry_wait_time)
      : apps_laucnher_(apps_laucnher)
      , connection_handler_(connection_handler)
      , app_launch_max_retry_attempt_(app_launch_max_retry_attempt)
      , app_launch_retry_wait_time_(app_launch_retry_wait_time) {}
  AppsLauncher::LauncherPtr operator()() {
    return utils::MakeShared<AppsLauncher::Launcher>(
        apps_laucnher_,
        connection_handler_,
        app_launch_max_retry_attempt_,
        app_launch_retry_wait_time_);
  }

  AppsLauncher& apps_laucnher_;
  connection_handler::ConnectionHandler& connection_handler_;
  const uint16_t app_launch_max_retry_attempt_;
  const uint16_t app_launch_retry_wait_time_;
};

CREATE_LOGGERPTR_GLOBAL(logger_, "AppLaunch")
AppsLauncher::AppsLauncher(
    connection_handler::ConnectionHandler& connection_handler,
    const uint16_t max_number_of_ios_device,
    const uint16_t app_launch_max_retry_attempt,
    const uint16_t app_launch_retry_wait_time) {
  sync_primitives::AutoLock lock(launchers_lock_);
  free_launchers_.resize(max_number_of_ios_device);
  std::generate(free_launchers_.begin(),
                free_launchers_.end(),
                LauncherGenerator(*this,
                                  connection_handler,
                                  app_launch_max_retry_attempt,
                                  app_launch_retry_wait_time));
}

void AppsLauncher::StartLaunching(ApplicationDataPtr app_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(launchers_lock_);
  DCHECK_OR_RETURN_VOID(!free_launchers_.empty())
  const AppLaunchers::iterator it = free_launchers_.begin();
  LauncherPtr app_launcher = *it;
  works_launchers_.push_back(app_launcher);
  free_launchers_.erase(it);
  app_launcher->PosponedLaunch(app_data);
}

struct AppLauncherFinder {
  AppLauncherFinder(const ApplicationDataPtr& app_data) : app_data_(app_data) {}
  bool operator()(const AppsLauncher::LauncherPtr& launcher) const {
    DCHECK_OR_RETURN(launcher->app_data_ && app_data_, false)
    return *launcher->app_data_ == *app_data_;
  }
  const ApplicationDataPtr& app_data_;
};

void AppsLauncher::StopLaunching(ApplicationDataPtr app_data) {
  sync_primitives::AutoLock lock(launchers_lock_);
  const AppLaunchers::iterator it = std::find_if(works_launchers_.begin(),
                                                 works_launchers_.end(),
                                                 AppLauncherFinder(app_data));
  if (it != works_launchers_.end()) {
    LauncherPtr launcher = *it;
    launcher->Clear();
    free_launchers_.push_back(launcher);
    works_launchers_.erase(it);
  } else {
    LOG4CXX_DEBUG(logger_,
                  "Unable to StopLaunching" << app_data->mobile_app_id_);
  }
}

void AppsLauncher::OnLaunched(ApplicationDataPtr app_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  StopLaunching(app_data);
}

void AppsLauncher::OnRetryAttemptsExhausted(ApplicationDataPtr app_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  StopLaunching(app_data);
}

AppsLauncher::Launcher::Launcher(
    AppsLauncher& parent,
    connection_handler::ConnectionHandler& connection_handler,
    const uint16_t app_launch_max_retry_attempt,
    const uint16_t app_launch_retry_wait_time)
    : retry_index_(0)
    , retry_timer_(
          "AppsLauncherTimer",
          new timer::TimerTaskImpl<Launcher>(this, &Launcher::LaunchNow))
    , app_launch_max_retry_attempt_(app_launch_max_retry_attempt)
    , app_launch_retry_wait_time_(app_launch_retry_wait_time)
    , connection_handler_(connection_handler)
    , parent_(parent) {}

void AppsLauncher::Launcher::PosponedLaunch(
    const app_launch::ApplicationDataPtr& app_data) {
  DCHECK(!app_data_);
  app_data_ = app_data;
  retry_index_ = 0;
  retry_timer_.Start(app_launch_retry_wait_time_, timer::kPeriodic);
  LOG4CXX_DEBUG(logger_,
                "Applicaiton " << app_data->mobile_app_id_ << " on device "
                               << app_data->device_mac_
                               << " will be launched in "
                               << app_launch_retry_wait_time_ << " ms");
}

void AppsLauncher::Launcher::Clear() {
  retry_timer_.Stop();
  app_data_.reset();
  retry_index_ = 0;
}

void AppsLauncher::Launcher::LaunchNow() {
  if (retry_index_++ < app_launch_max_retry_attempt_) {
    LOG4CXX_DEBUG(logger_,
                  "Run App " << app_data_->mobile_app_id_ << "with bundle "
                             << app_data_->bundle_id_ << " On Device "
                             << app_data_->device_mac_);

    connection_handler_.RunAppOnDevice(app_data_->device_mac_,
                                       app_data_->bundle_id_);
  } else {
    parent_.OnRetryAttemptsExhausted(app_data_);
  }
}

}  // namespace app_launch
