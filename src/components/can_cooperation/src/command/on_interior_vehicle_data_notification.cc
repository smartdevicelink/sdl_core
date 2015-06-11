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
#include "can_cooperation/validators/on_interior_vehicle_data_notification_validator.h"
#include "json/json.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "OnInteriorVehicleDataNotification")

OnInteriorVehicleDataNotification::OnInteriorVehicleDataNotification(
  const application_manager::MessagePtr& message)
  : BaseCommandNotification(message) {
}

OnInteriorVehicleDataNotification::~OnInteriorVehicleDataNotification() {
}

void OnInteriorVehicleDataNotification::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);
}

bool OnInteriorVehicleDataNotification::Validate() {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::MessagePtr msg = message();
  std::string json = msg->json_message();

  if (validators::ValidationResult::SUCCESS ==
      validators::OnInteriorVehicleDataNotificationValidator::instance()->
      Validate(json)) {
    msg->set_json_message(json);
  } else {
    LOG4CXX_INFO(logger_, "HMI notification validation failed!");
    return false;
  }

  return true;
}

std::string OnInteriorVehicleDataNotification::ModuleType(
    const Json::Value& message) {
  // TODO(KKolodiy): Now notification from CAN(HMI) doesn't have moduleData
  // it contains list moduleType, moduleZone, radioControlData and
  // climateControlData
  return message.get(message_params::kModuleType, Json::Value("")).asString();
}

SeatLocation OnInteriorVehicleDataNotification::InteriorZone(
    const Json::Value& message) {
  Json::Value zone = message.get(message_params::kModuleZone,
                                 Json::Value(Json::objectValue));
  return CreateInteriorZone(zone);
}

namespace {
std::string ToString(const Json::Value& i) {
  return i.asString();
}
}  // namespace

std::vector<std::string> OnInteriorVehicleDataNotification::ControlData(
    const Json::Value& message) {
  Json::Value data = message.get(message_params::kModuleData,
                                   Json::Value(Json::objectValue));

  Json::Value radio = data.get(message_params::kRadioControlData,
                               Json::Value(Json::objectValue));
  Json::Value climate = data.get(message_params::kClimateControlData,
                                 Json::Value(Json::objectValue));

  std::vector<std::string> params(radio.size() + climate.size());
  std::vector<std::string>::iterator i = std::transform(radio.begin(),
                                                        radio.end(),
                                                        params.begin(),
                                                        &ToString);
  std::transform(climate.begin(), climate.end(), i, &ToString);

  return params;
}

}  // namespace commands

}  // namespace can_cooperation
