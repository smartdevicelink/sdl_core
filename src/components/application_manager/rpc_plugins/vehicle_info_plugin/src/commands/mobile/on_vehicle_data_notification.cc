/*

 Copyright (c) 2018, Ford Motor Company
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

#include "vehicle_info_plugin/commands/mobile/on_vehicle_data_notification.h"

#include "application_manager/application_impl.h"
#include "application_manager/helpers/application_helper.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "utils/macro.h"
#include "vehicle_info_plugin/vehicle_info_app_extension.h"

namespace vehicle_info_plugin {
using namespace application_manager;

namespace commands {

OnVehicleDataNotification::OnVehicleDataNotification(
    const application_manager::commands::MessageSharedPtr& message,
    const VehicleInfoCommandParams& params)
    : CommandNotificationImpl(message,
                              params.application_manager_,
                              params.rpc_service_,
                              params.hmi_capabilities_,
                              params.policy_handler_)
    , custom_vehicle_data_manager_(params.custom_vehicle_data_manager_) {}

OnVehicleDataNotification::~OnVehicleDataNotification() {}

void OnVehicleDataNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  std::vector<ApplicationSharedPtr> notify_apps;
  std::vector<ApplicationSharedPtr>::iterator notified_app_it =
      notify_apps.begin();
  std::vector<smart_objects::SmartObject> appSO;

  custom_vehicle_data_manager_.CreateMobileMessageParams(
      (*message_)[strings::msg_params]);

  const auto& param_names = (*message_)[strings::msg_params].enumerate();
  for (const auto& name : param_names) {
    LOG4CXX_DEBUG(logger_, "vehicle_data name: " << name);
    auto vehicle_data_value = (*message_)[strings::msg_params][name].asInt();
    application_manager_.IviInfoUpdated(name, vehicle_data_value);

    auto subscribed_to_ivi_predicate = [&name](const ApplicationSharedPtr app) {
      DCHECK_OR_RETURN(app, false);
      auto& ext = VehicleInfoAppExtension::ExtractVIExtension(*app);
      return ext.isSubscribedToVehicleInfo(name);
    };

    const auto& applications = application_manager::FindAllApps(
        application_manager_.applications(), subscribed_to_ivi_predicate);
    for (const auto& app : applications) {
      if (!app) {
        LOG4CXX_ERROR(logger_, "NULL pointer");
        continue;
      }
      notified_app_it = find(notify_apps.begin(), notify_apps.end(), app);
      if (notified_app_it == notify_apps.end()) {
        notify_apps.push_back(app);
        smart_objects::SmartObject msg_param =
            smart_objects::SmartObject(smart_objects::SmartType_Map);
        msg_param[name] = (*message_)[strings::msg_params][name];
        appSO.push_back(msg_param);
      } else {
        size_t idx = std::distance(notify_apps.begin(), notified_app_it);
        appSO[idx][name] = (*message_)[strings::msg_params][name];
      }
    }
  }

  LOG4CXX_DEBUG(logger_,
                "Number of Notifications to be send: " << notify_apps.size());

  for (size_t idx = 0; idx < notify_apps.size(); idx++) {
    LOG4CXX_INFO(logger_,
                 "Send OnVehicleData PRNDL notification to "
                     << notify_apps[idx]->name().c_str() << " application id "
                     << notify_apps[idx]->app_id());
    (*message_)[strings::params][strings::connection_key] =
        notify_apps[idx]->app_id();
    (*message_)[strings::msg_params] = appSO[idx];
    SendNotification();
  }
}

}  // namespace commands
}  // namespace vehicle_info_plugin
