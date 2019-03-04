/*
 * Copyright (c) 2016, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_CTRL_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_CTRL_IMPL_H_

#include "application_manager/app_launch/app_launch_ctrl.h"
#include "application_manager/app_launch/app_launch_data.h"
#include "application_manager/app_launch/apps_launcher.h"
#include "application_manager/app_launch/device_apps_launcher.h"
#include "application_manager/app_launch_settings.h"

namespace connection_handler {
class ConnectionHandler;
}  // connection_handler

namespace resumption {
class ResumeCtrl;
}  // resumption

namespace app_launch {
// TODO(AK) Use unique pointer
typedef std::shared_ptr<timer::Timer> TimerPtr;
class MultipleAppsLauncherFactoryImpl;

class AppLaunchCtrlImpl : public AppLaunchCtrl {
 public:
  /**
   * @brief allows to create AppLaunchCtrlImpl object
   */
  AppLaunchCtrlImpl(AppLaunchData& data,
                    application_manager::ApplicationManager& app_mngr,
                    const AppLaunchSettings& settings);

  /**
   * @brief allows to destroy AppLaunchCtrlImpl object
   */
  ~AppLaunchCtrlImpl() {}

  void OnAppRegistered(const application_manager::Application& app) OVERRIDE;
  void OnDeviceConnected(const std::string& device_mac) OVERRIDE;
  void OnMasterReset() OVERRIDE;
  void Stop() OVERRIDE;

 private:
  const AppLaunchSettings& settings_;
  AppLaunchData& app_launch_data_;
  resumption::ResumeCtrl& resume_ctrl_;

  AppsLauncher apps_launcher_;
  DeviceAppsLauncher device_apps_launcher_;
  sync_primitives::Lock launch_ctrl_lock_;

  DISALLOW_COPY_AND_ASSIGN(AppLaunchCtrlImpl);
};

}  // namespace app_launch

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_CTRL_IMPL_H_
