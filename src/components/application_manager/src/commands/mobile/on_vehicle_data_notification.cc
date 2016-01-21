/*

 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/commands/mobile/on_vehicle_data_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"

namespace application_manager {

namespace commands {

OnVehicleDataNotification::OnVehicleDataNotification(
  const MessageSharedPtr& message)
  : CommandNotificationImpl(message) {
}

OnVehicleDataNotification::~OnVehicleDataNotification() {
}

void OnVehicleDataNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  std::vector<ApplicationSharedPtr> appNotification;
  std::vector<ApplicationSharedPtr>::iterator appNotification_it =
    appNotification.begin();
  std::vector<smart_objects::SmartObject> appSO;

  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();

  for (; vehicle_data.end() != it; ++it) {
    if (true == (*message_)[strings::msg_params].keyExists(it->first)) {
      const std::vector<ApplicationSharedPtr>& applications =
        ApplicationManagerImpl::instance()->IviInfoUpdated(it->second,
                                                           (*message_)[strings::msg_params][it->first].asInt());

      std::vector<ApplicationSharedPtr>::const_iterator app_it =
        applications.begin();

      for (; applications.end() != app_it; ++app_it) {
        const ApplicationSharedPtr app = *app_it;
        if (!app) {
          LOG4CXX_ERROR(logger_, "NULL pointer");
          continue;
}

        appNotification_it = find(appNotification.begin(), appNotification.end(), app);
        if (appNotification_it == appNotification.end()) {
          appNotification.push_back(app);
          smart_objects::SmartObject msg_param = smart_objects::SmartObject(
                                                   smart_objects::SmartType_Map);
          msg_param[it->first] = (*message_)[strings::msg_params][it->first];
          appSO.push_back(msg_param);
        } else {
          size_t idx = std::distance(appNotification.begin(), appNotification_it);
          appSO[idx][it->first] = (*message_)[strings::msg_params][it->first];

        }
      }
    }
  }

  LOG4CXX_DEBUG(logger_, "Number of Notifications to be send: " <<
                appNotification.size());

  for (size_t idx = 0; idx < appNotification.size(); idx++) {
    LOG4CXX_INFO(
      logger_,
      "Send OnVehicleData PRNDL notification to " << appNotification[idx]->name().c_str()
      << " application id " << appNotification[idx]->app_id());
    (*message_)[strings::params][strings::connection_key] =
      appNotification[idx]->app_id();
    (*message_)[strings::msg_params] = appSO[idx];
    SendNotification();
  }
}

}  // namespace commands

}  // namespace application_manager
