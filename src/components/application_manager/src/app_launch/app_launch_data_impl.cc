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
#include "application_manager/app_launch/app_launch_data_impl.h"
#include "utils/logger.h"

namespace app_launch {

CREATE_LOGGERPTR_GLOBAL(logger_, "AppLaunch")

AppLaunchDataImpl::AppLaunchDataImpl(const AppLaunchSettings& settings)
    : settings_(settings)
    , kMaxNumberOfiOSdevice(settings.max_number_of_ios_device()) {}

AppLaunchDataImpl::~AppLaunchDataImpl() {}

bool AppLaunchDataImpl::AddApplicationData(const ApplicationData& app_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool retVal = true;

  if (app_data.device_mac_.empty() == false &&
      app_data.mobile_app_id_.empty() == false &&
      app_data.bundle_id_.empty() == false) {
    if (IsAppDataAlreadyExisted(app_data)) {
      LOG4CXX_INFO(logger_, "This application data already existed");
      retVal &= RefreshAppSessionTime(app_data);
    } else {
      if (GetCurentNumberOfAppData() >= get_max_number_iOS_devs()) {
        LOG4CXX_INFO(logger_,
                     "Max number of application data have. It will be deleted "
                     "the oldest one");
        retVal &= DeleteOldestAppData();
      }
      retVal &= AddNewAppData(app_data);
      LOG4CXX_INFO(logger_, "Added new application data to DB");
    }
  } else {
    retVal = false;
  }
  return retVal;
}

std::vector<ApplicationDataPtr> AppLaunchDataImpl::GetApplicationDataByDevice(
    const std::string& dev_mac) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<ApplicationDataPtr> apps = GetAppDataByDevMac(dev_mac);

  if (apps.empty()) {
    LOG4CXX_DEBUG(logger_, "No application founded by mac" << dev_mac);
  }

  return apps;
}

}  // namespace app_launch
