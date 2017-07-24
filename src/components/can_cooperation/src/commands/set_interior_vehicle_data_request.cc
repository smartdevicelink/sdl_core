/*
 Copyright (c) 2017, Ford Motor Company
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

#include "can_cooperation/commands/set_interior_vehicle_data_request.h"
#include <algorithm>
#include "can_cooperation/can_module_constants.h"
#include "can_cooperation/message_helper.h"
#include "functional_module/function_ids.h"
#include "json/json.h"

namespace can_cooperation {

namespace commands {

using namespace json_keys;
using namespace message_params;

CREATE_LOGGERPTR_GLOBAL(logger_, "SetInteriorVehicleDataRequest")

SetInteriorVehicleDataRequest::SetInteriorVehicleDataRequest(
    const application_manager::MessagePtr& message,
    CANModuleInterface& can_module)
    : BaseCommandRequest(message, can_module) {}

SetInteriorVehicleDataRequest::~SetInteriorVehicleDataRequest() {}

void SetInteriorVehicleDataRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  const Json::Value request_params =
      MessageHelper::StringToValue(message_->json_message());
  const Json::Value module_data = request_params[kModuleData];
  const std::string module_type = module_data[kModuleType].asString();
  bool module_type_and_data_match = true;

  if (enums_value::kRadio == module_type) {
    module_type_and_data_match = !IsMember(module_data, kClimateControlData);
  }

  if (enums_value::kClimate == module_type) {
    module_type_and_data_match = !IsMember(module_data, kRadioControlData);
  }

  if (module_type_and_data_match) {
    application_manager::MessagePtr hmi_request = CreateHmiRequest(
        functional_modules::hmi_api::set_interior_vehicle_data, request_params);
    service()->RemoveHMIFakeParameters(hmi_request);
    service()->SendMessageToHMI(hmi_request);
  } else {
    LOG4CXX_WARN(logger_, "Request module type & data mismatch!");
    SendResponse(false,
                 result_codes::kInvalidData,
                 "Request module type & data mismatch!");
  }
}

void SetInteriorVehicleDataRequest::OnEvent(
    const can_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(
      (functional_modules::hmi_api::set_interior_vehicle_data == event.id()));

  application_manager::Message& hmi_response = *(event.event_message());
  const bool validate_result =
      application_manager::MessageValidationResult::SUCCESS ==
      service()->ValidateMessageBySchema(hmi_response);
  LOG4CXX_DEBUG(logger_,
                "HMI response validation result is " << validate_result);
  const Json::Value value =
      MessageHelper::StringToValue(hmi_response.json_message());

  std::string result_code;
  std::string info;

  const bool is_response_successful = ParseResultCode(value, result_code, info);

  if (is_response_successful) {
    response_params_[kModuleData] = value[kResult][kModuleData];
  }
  SendResponse(is_response_successful, result_code.c_str(), info);
}

std::string SetInteriorVehicleDataRequest::ModuleType(
    const Json::Value& message) {
  const Json::Value& module_data =
      message.get(message_params::kModuleData, Json::Value(Json::objectValue));
  return module_data.get(message_params::kModuleType, "").asString();
}
std::vector<std::string> SetInteriorVehicleDataRequest::ControlData(
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
