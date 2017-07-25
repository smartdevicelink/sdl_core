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

#include "can_cooperation/commands/button_press_request.h"
#include "can_cooperation/message_helper.h"
#include "can_cooperation/can_module_constants.h"
#include "functional_module/function_ids.h"
#include "json/json.h"

namespace can_cooperation {

namespace commands {

using namespace json_keys;
using namespace message_params;

CREATE_LOGGERPTR_GLOBAL(logger_, "ButtonPressRequest")

ButtonPressRequest::ButtonPressRequest(
    const application_manager::MessagePtr& message,
    CANModuleInterface& can_module)
    : BaseCommandRequest(message, can_module) {}

ButtonPressRequest::~ButtonPressRequest() {}

const bool CheckButtonName(const std::string& module_type,
                           const std::string& button_name) {
  if (enums_value::kRadio == module_type) {
    return (button_name == enums_value::kVolumeUp) ||
           (button_name == enums_value::kVolumeDown) ||
           (button_name == enums_value::kEject) ||
           (button_name == enums_value::kSource) ||
           (button_name == enums_value::kShuffle) ||
           (button_name == enums_value::kRepeat);
  }

  if (enums_value::kClimate == module_type) {
    return (button_name == enums_value::kACMax) ||
           (button_name == enums_value::kAC) ||
           (button_name == enums_value::kRecirculate) ||
           (button_name == enums_value::kFanUp) ||
           (button_name == enums_value::kFanDown) ||
           (button_name == enums_value::kTempUp) ||
           (button_name == enums_value::kTempDown) ||
           (button_name == enums_value::kDefrostMax) ||
           (button_name == enums_value::kDefrost) ||
           (button_name == enums_value::kDefrostRear) ||
           (button_name == enums_value::kUpperVent) ||
           (button_name == enums_value::kLowerVent);
  }
  return false;
}

void ButtonPressRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  const Json::Value request_params =
      MessageHelper::StringToValue(message_->json_message());

  const std::string button_name = request_params[kButtonName].asString();
  const std::string module_type = request_params[kModuleType].asString();
  const bool button_name_matches_module_type =
      CheckButtonName(module_type, button_name);

  if (button_name_matches_module_type) {
    SendRequest(functional_modules::hmi_api::button_press, request_params);
  } else {
    LOG4CXX_WARN(logger_, "Request module type and button name mismatch!");
    SendResponse(false,
                 result_codes::kInvalidData,
                 "Request module type and button name mismatch!");
  }
}

void ButtonPressRequest::OnEvent(
    const can_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(
      (functional_modules::hmi_api::button_press == event.id()));

  application_manager::Message& hmi_response = *(event.event_message());
  const Json::Value value =
      MessageHelper::StringToValue(hmi_response.json_message());

  std::string result_code;
  std::string info;

  const bool is_response_successful = ParseResultCode(value, result_code, info);

  SendResponse(is_response_successful, result_code.c_str(), info);
}

std::string ButtonPressRequest::ModuleType(const Json::Value& message) {
  return message.get(message_params::kModuleType, Json::Value("")).asString();
}

}  // namespace commands

}  // namespace can_cooperation
