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

#include "can_cooperation/commands/on_interior_vehicle_data_notification.h"
#include <algorithm>
#include <vector>
#include "can_cooperation/validators/on_interior_vehicle_data_notification_validator.h"
#include "json/json.h"
#include "can_cooperation/can_module_constants.h"
#include "can_cooperation/message_helper.h"
#include "can_cooperation/can_module.h"
#include "can_cooperation/can_app_extension.h"

namespace can_cooperation {

namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "OnInteriorVehicleDataNotification")

OnInteriorVehicleDataNotification::OnInteriorVehicleDataNotification(
    const application_manager::MessagePtr& message,
    CANModuleInterface& can_module)
    : BaseCommandNotification(message, can_module) {}

OnInteriorVehicleDataNotification::~OnInteriorVehicleDataNotification() {}

void OnInteriorVehicleDataNotification::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value moduleDescription(Json::ValueType::objectValue);
  Json::Value json;

  application_manager::MessagePtr msg = message();

  json = MessageHelper::StringToValue(msg->json_message());

  moduleDescription[message_params::kModuleType] =
      json[message_params::kModuleData][message_params::kModuleType];

  moduleDescription[message_params::kModuleZone] =
      json[message_params::kModuleData][message_params::kModuleZone];

  std::vector<application_manager::ApplicationSharedPtr> applications =
      service_->GetApplications(can_module_.GetModuleID());

  for (size_t i = 0; i < applications.size(); ++i) {
    CANAppExtensionPtr extension =
        application_manager::AppExtensionPtr::static_pointer_cast<
            CANAppExtension>(
            applications[i]->QueryInterface(can_module_.GetModuleID()));
    DCHECK(extension);

    if (extension->IsSubscibedToInteriorVehicleData(moduleDescription)) {
      msg->set_connection_key(applications[i]->app_id());
      application_manager::MessagePtr message(
          new application_manager::Message(*msg));
      NotifyOneApplication(message);
    }
  }
}

bool OnInteriorVehicleDataNotification::Validate() {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::MessagePtr msg = message();
  std::string json = msg->json_message();

  validators::OnInteriorVehicleDataNotificationValidator validaror;
  if (validators::ValidationResult::SUCCESS == validaror.Validate(json)) {
    msg->set_json_message(json);
  } else {
    LOG4CXX_INFO(logger_, "Invalid notification from the vehicle!");
    return false;
  }

  return true;
}

std::string OnInteriorVehicleDataNotification::ModuleType(
    const Json::Value& message) {
  return message.get(message_params::kModuleData,
                     Json::Value(Json::objectValue))
      .get(message_params::kModuleType, Json::Value(""))
      .asString();
}

SeatLocation OnInteriorVehicleDataNotification::InteriorZone(
    const Json::Value& message) {
  Json::Value zone =
      message.get(message_params::kModuleData, Json::Value(Json::objectValue))
          .get(message_params::kModuleZone, Json::Value(Json::objectValue));
  return CreateInteriorZone(zone);
}

std::vector<std::string> OnInteriorVehicleDataNotification::ControlData(
    const Json::Value& message) {
  Json::Value data =
      message.get(message_params::kModuleData, Json::Value(Json::objectValue));
  const char* name_control_data;
  std::string module = ModuleType(message);
  if (module == enums_value::kRadio) {
    name_control_data = message_params::kRadioControlData;
  }
  if (module == enums_value::kClimate) {
    name_control_data = message_params::kClimateControlData;
  }
  Json::Value params =
      data.get(name_control_data, Json::Value(Json::objectValue));
  return params.getMemberNames();
}

}  // namespace commands

}  // namespace can_cooperation
