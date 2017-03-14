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

#include "can_cooperation/commands/set_interior_vehicle_data_request.h"
#include <algorithm>
#include "can_cooperation/validators/set_interior_vehicle_data_request_validator.h"
#include "can_cooperation/validators/struct_validators/module_data_validator.h"
#include "can_cooperation/validators/struct_validators/radio_control_data_validator.h"
#include "can_cooperation/validators/struct_validators/climate_control_data_validator.h"
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

  Json::Value params;

  Json::Reader reader;
  reader.parse(message_->json_message(), params);

  SendRequest(
      functional_modules::hmi_api::set_interior_vehicle_data, params, true);
}

void SetInteriorVehicleDataRequest::OnEvent(
    const can_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (functional_modules::hmi_api::set_interior_vehicle_data == event.id()) {
    std::string result_code;
    std::string info;

    Json::Value value;
    Json::Reader reader;
    reader.parse(event.event_message()->json_message(), value);

    bool success = false;
    if (IsMember(value, kError) && IsMember(value[kError], kCode) &&
        (hmi_apis::Common_Result::READ_ONLY == value[kError][kCode].asInt())) {
      result_code = result_codes::kReadOnly;

      if (IsMember(value[kError], kMessage)) {
        info = value[kError][kMessage].asCString();
      }
    } else {
      success = ParseResultCode(value, result_code, info);
    }

    // TOD(VS): Create SetInteriorVehicleDataResponseValidator
    validators::ValidationResult validation_result = validators::SUCCESS;
    validators::ModuleDataValidator validator;
    if (success) {
      if (IsMember(value[kResult], kModuleData)) {
        validation_result = validator.Validate(value[kResult][kModuleData],
                                               response_params_[kModuleData]);
      } else {
        validation_result = validators::INVALID_DATA;
      }

      if (validators::SUCCESS != validation_result) {
        success = false;
        info = "Invalid response from the vehicle";
        result_code = result_codes::kGenericError;
      }
    }

    SendResponse(success, result_code.c_str(), info);
  } else {
    LOG4CXX_ERROR(logger_, "Received unknown event: " << event.id());
  }
}

bool SetInteriorVehicleDataRequest::Validate() {
  Json::Value json;
  if (!this->ParseJsonString(&json)) {
    return false;
  }

  Json::Value outgoing_json;
  validators::SetInteriorVehicleDataRequestValidator set_interior_validator;
  if (validators::ValidationResult::SUCCESS !=
      set_interior_validator.Validate(json, outgoing_json)) {
    LOG4CXX_INFO(logger_, "SetInteriorVehicleDataRequest validation failed!");
    SendResponse(
        false, result_codes::kInvalidData, "Mobile request validation failed!");
    return false;
  }

  // TODO(VS): Create function for read only validation and move there similar
  // code
  validators::RadioControlDataValidator radio_control_validator;
  validators::ClimateControlDataValidator climat_control_validator;
  if (outgoing_json[kModuleData].isMember(kRadioControlData)) {
    radio_control_validator.RemoveReadOnlyParams(
        outgoing_json[kModuleData][kRadioControlData]);

    if (0 == outgoing_json[kModuleData][kRadioControlData].size()) {
      SendResponse(false,
                   result_codes::kReadOnly,
                   "Request contains just read only params!");
      return false;
    }
  } else if (outgoing_json[kModuleData].isMember(kClimateControlData)) {
    climat_control_validator.RemoveReadOnlyParams(
        outgoing_json[kModuleData][kClimateControlData]);
    if (0 == outgoing_json[kModuleData][kClimateControlData].size()) {
      SendResponse(false,
                   result_codes::kReadOnly,
                   "Request contains just read only params!");
      return false;
    }
  } else {
    DCHECK(false);
  }

  message_->set_json_message(MessageHelper::ValueToString(outgoing_json));

  return true;
}

std::string SetInteriorVehicleDataRequest::ModuleType(
    const Json::Value& message) {
  return message.get(message_params::kModuleData,
                     Json::Value(Json::objectValue))
      .get(message_params::kModuleType, Json::Value(""))
      .asString();
}

Json::Value SetInteriorVehicleDataRequest::GetInteriorZone(
    const Json::Value& message) {
  return message.get(message_params::kModuleData,
                     Json::Value(Json::objectValue))
      .get(message_params::kModuleZone, Json::Value(Json::objectValue));
}

SeatLocation SetInteriorVehicleDataRequest::InteriorZone(
    const Json::Value& message) {
  Json::Value zone =
      message.get(message_params::kModuleData, Json::Value(Json::objectValue))
          .get(message_params::kModuleZone, Json::Value(Json::objectValue));
  return CreateInteriorZone(zone);
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
