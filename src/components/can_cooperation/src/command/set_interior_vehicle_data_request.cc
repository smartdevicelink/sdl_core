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
  const application_manager::MessagePtr& message)
  : BaseCommandRequest(message) {
}

SetInteriorVehicleDataRequest::~SetInteriorVehicleDataRequest() {
}

void SetInteriorVehicleDataRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value params;

  Json::Reader reader;
  reader.parse(message_->json_message(), params);

  SendRequest(
      functional_modules::hmi_api::set_interior_vehicle_data, params, true);
}

void SetInteriorVehicleDataRequest::OnEvent(
    const event_engine::Event<application_manager::MessagePtr,
    std::string>& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (functional_modules::hmi_api::set_interior_vehicle_data == event.id()) {
    std::string result_code;
    std::string info;

    Json::Value value;
    Json::Reader reader;
    reader.parse(event.event_message()->json_message(), value);

    bool success = ParseResultCode(value, result_code, info);

    // TOD(VS): Create SetInteriorVehicleDataResponseValidator
    validators::ValidationResult validation_result = validators::SUCCESS;

    if (success) {
      if (value[kResult].isMember(kModuleData)) {
        validation_result =
            validators::ModuleDataValidator::instance()->Validate(
                                              value[kResult][kModuleData],
                                              response_params_[kModuleData]);
      } else {
        validation_result = validators::INVALID_DATA;
      }

      if (validators::SUCCESS != validation_result) {
        success = false;
        info = "Response validation failed";
        result_code = result_codes::kInvalidData;
      }
    }

    SendResponse(success, result_code.c_str(), info);
  } else {
    LOG4CXX_ERROR(logger_, "Received unknown event: " << event.id());
  }
}

bool SetInteriorVehicleDataRequest::Validate() {

  Json::Value json;

  json = MessageHelper::StringToValue(message_->json_message());
  Json::Value outgoing_json;

  if (validators::ValidationResult::SUCCESS !=
    validators::SetInteriorVehicleDataRequestValidator::instance()->
                                                Validate(json, outgoing_json)) {
    LOG4CXX_INFO(logger_,
                 "SetInteriorVehicleDataRequest validation failed!");
    SendResponse(false, result_codes::kInvalidData,
                 "Mobile request validation failed!");
    return false;
  }

  return true;
}

std::string SetInteriorVehicleDataRequest::ModuleType(
    const Json::Value& message) {
  return message.get(message_params::kModuleData,
                     Json::Value(Json::objectValue))
      .get(message_params::kModuleType, Json::Value("")).asString();
}

SeatLocation SetInteriorVehicleDataRequest::InteriorZone(
    const Json::Value& message) {
  Json::Value zone = message.get(message_params::kModuleData,
                                 Json::Value(Json::objectValue)).get(
      message_params::kModuleZone, Json::Value(Json::objectValue));
  return CreateInteriorZone(zone);
}

namespace {
std::string ToString(const Json::Value& i) {
  return i.asString();
}
}  // namespace

std::vector<std::string> SetInteriorVehicleDataRequest::ControlData(
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
