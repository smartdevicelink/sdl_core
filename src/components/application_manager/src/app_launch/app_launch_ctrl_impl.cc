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
#include <iterator>
#include <algorithm>
#include <utility>
#include <vector>
#include "application_manager/app_launch/app_launch_ctrl_impl.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "connection_handler/connection_handler.h"
#include "application_manager/application.h"
#include "utils/timer_task_impl.h"
#include "utils/make_shared.h"

namespace app_launch {
CREATE_LOGGERPTR_GLOBAL(logger_, "AppLaunch")

AppLaunchCtrlImpl::AppLaunchCtrlImpl(
    AppLaunchData& data,
    application_manager::ApplicationManager& app_mngr,
    const AppLaunchSettings& settings)
    : settings_(settings)
    , app_launch_data_(data)
    , resume_ctrl_(app_mngr.resume_controller())
    , apps_launcher_(app_mngr.connection_handler(),
                     settings.max_number_of_ios_device(),
                     settings.app_launch_max_retry_attempt(),
                     settings.app_launch_retry_wait_time())
    , device_apps_launcher_(app_mngr, apps_launcher_, settings) {}

void AppLaunchCtrlImpl::OnAppRegistered(
    const application_manager::Application& app) {
  LOG4CXX_AUTO_TRACE(logger_);
  // TODO (AKutsan) : get device mac
  ApplicationDataPtr app_data = std::make_shared<ApplicationData>(
      app.policy_app_id(), app.bundle_id(), app.mac_address());
  apps_launcher_.OnLaunched(app_data);
  app_launch_data_.AddApplicationData(*app_data);
}

ApplicationDataPtr GetAppFromHmiLevelPair(
    const std::pair<int32_t, ApplicationDataPtr>& pair) {
  return pair.second;
}

bool HmiLevelSorter(const std::pair<int32_t, ApplicationDataPtr>& lval,
                    const std::pair<int32_t, ApplicationDataPtr>& rval) {
  if (lval.first == -1) {
    return false;
  }
  if (rval.first == -1) {
    return true;
  }
  return lval.first < rval.first;
}

void AppLaunchCtrlImpl::OnDeviceConnected(const std::string& device_mac) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<ApplicationDataPtr> apps_on_device =
      app_launch_data_.GetApplicationDataByDevice(device_mac);
  std::vector<std::pair<int32_t, ApplicationDataPtr> > apps_hmi_levels;
  std::vector<ApplicationDataPtr>::iterator it = apps_on_device.begin();
  for (; it != apps_on_device.end(); ++it) {
    const ApplicationDataPtr& app_data = *it;
    const int32_t hmi_level = resume_ctrl_.GetSavedAppHmiLevel(
        app_data->mobile_app_id_, app_data->device_mac_);
    const std::pair<int32_t, ApplicationDataPtr> hmi_level_app(hmi_level,
                                                               app_data);
    apps_hmi_levels.push_back(hmi_level_app);
  }
  std::sort(apps_hmi_levels.begin(), apps_hmi_levels.end(), HmiLevelSorter);
  apps_on_device.clear();
  std::transform(apps_hmi_levels.begin(),
                 apps_hmi_levels.end(),
                 std::back_inserter(apps_on_device),
                 GetAppFromHmiLevelPair);
  if (apps_on_device.size() > 0) {
    device_apps_launcher_.LaunchAppsOnDevice(device_mac, apps_on_device);
  } else {
    LOG4CXX_DEBUG(logger_, "No apps in saved for device " << device_mac);
  }
}

void AppLaunchCtrlImpl::OnMasterReset() {
  LOG4CXX_AUTO_TRACE(logger_);
  app_launch_data_.Clear();
}
}  // namespace app_launch
