/*

 Copyright (c) 2015, Ford Motor Company
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

#include "application_manager/commands/mobile/on_interior_vehicle_data_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "interfaces/MOBILE_API.h"
namespace application_manager {

namespace commands {

namespace mobile {

OnInteriorVehicleDataNotification::OnInteriorVehicleDataNotification(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandNotificationImpl(message, application_manager) {}

OnInteriorVehicleDataNotification::~OnInteriorVehicleDataNotification() {}

void OnInteriorVehicleDataNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject moduleDescription =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject moduleZone =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject moduleType =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  moduleZone = (*message_)[strings::msg_params]["moduleData"]["moduleZone"];

  moduleType = (*message_)[strings::msg_params]["moduleData"]["moduleType"];

  moduleDescription["moduleZone"] = moduleZone;
  moduleDescription["moduleType"] = moduleType;

  const std::vector<ApplicationSharedPtr>& subscribedApps =
      application_manager_.applications_by_interior_vehicle_data(
          moduleDescription);

  std::vector<ApplicationSharedPtr>::const_iterator it = subscribedApps.begin();
  for (; subscribedApps.end() != it; ++it) {
    ApplicationSharedPtr subscribed_app = *it;
    if (!subscribed_app) {
      LOG4CXX_WARN(logger_, "Null pointer to subscribed app.");
      continue;
    }

    smart_objects::SmartObjectSPtr on_interior_vehicle_data_notification =
        new smart_objects::SmartObject();

    if (!on_interior_vehicle_data_notification) {
      LOG4CXX_ERROR(logger_, "OnButtonPress NULL pointer");
      return;
    }

    (*on_interior_vehicle_data_notification)
        [strings::params][strings::connection_key] = subscribed_app->app_id();

    (*on_interior_vehicle_data_notification)
        [strings::params][strings::function_id] = static_cast<int32_t>(
            mobile_apis::FunctionID::eType::OnInteriorVehicleDataID);

    (*on_interior_vehicle_data_notification)
        [strings::msg_params]["moduleData"] =
            (*message_)[strings::msg_params]["moduleData"];

    message_ = on_interior_vehicle_data_notification;
    //(*message_)[strings::params][strings::connection_key] =
    // subscribed_app->app_id();
    SendNotification();
  }
}

/*void
OnInteriorVehicleDataNotification::SendNotificationHelper(ApplicationConstSharedPtr
app){

 if(!app){
  LOG4CXX_ERROR_EXT(logger_, "OnButtonPress NULL pointer");
    return;
 }
  smart_objects::SmartObjectSPtr on_interior_vehicle_data_notification = new
smart_objects::SmartObject();

  if (!on_interior_vehicle_data_notification) {
    LOG4CXX_ERROR_EXT(logger_, "OnButtonPress NULL pointer");
    return;
  }


  (*on_interior_vehicle_data_notification)[strings::params][strings::connection_key]
= app->app_id();

  (*on_interior_vehicle_data_notification)[strings::params][strings::function_id]
=
      static_cast<int32_t>(mobile_apis::FunctionID::eType::OnInteriorVehicleDataID);

  (*on_interior_vehicle_data_notification)[strings::msg_params][strings::moduleDescription]
=
      (*message_)[strings::msg_params][hmi_response::button_name];
  (*on_interior_vehicle_data_notification)[strings::msg_params][strings::button_press_mode]
=
      (*message_)[strings::msg_params][hmi_response::button_mode];


  message_ = on_interior_vehicle_data_notification;
  SendNotification();
}*/

}  // namespace mobile

}  // namespace commands

}  // namespace application_manager
