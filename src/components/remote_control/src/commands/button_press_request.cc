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

#include "remote_control/commands/button_press_request.h"
#include "remote_control/message_helper.h"
#include "remote_control/rc_module_constants.h"
#include "functional_module/function_ids.h"
#include "json/json.h"
#include "utils/helpers.h"

namespace remote_control {

namespace commands {

using namespace json_keys;
using namespace message_params;

CREATE_LOGGERPTR_GLOBAL(logger_, "ButtonPressRequest")

ButtonPressRequest::ButtonPressRequest(
    const application_manager::MessagePtr& message,
    RemotePluginInterface& rc_module)
    : BaseCommandRequest(message, rc_module) {}

ButtonPressRequest::~ButtonPressRequest() {}

const std::vector<std::string> buttons_climate() {
  std::vector<std::string> data;
  data.push_back(enums_value::kACMax);
  data.push_back(enums_value::kAC);
  data.push_back(enums_value::kRecirculate);
  data.push_back(enums_value::kFanUp);
  data.push_back(enums_value::kFanDown);
  data.push_back(enums_value::kTempUp);
  data.push_back(enums_value::kTempDown);
  data.push_back(enums_value::kDefrostMax);
  data.push_back(enums_value::kDefrost);
  data.push_back(enums_value::kDefrostRear);
  data.push_back(enums_value::kUpperVent);
  data.push_back(enums_value::kLowerVent);
  return data;
}

const std::vector<std::string> buttons_radio() {
  std::vector<std::string> data;
  data.push_back(enums_value::kVolumeUp);
  data.push_back(enums_value::kVolumeDown);
  data.push_back(enums_value::kEject);
  data.push_back(enums_value::kSource);
  data.push_back(enums_value::kShuffle);
  data.push_back(enums_value::kRepeat);
  return data;
}

bool CheckIfButtonExistInRCCaps(
    const smart_objects::SmartObject& rc_capabilities,
    const std::string& button_name) {
  if (rc_capabilities.keyExists(strings::kbuttonCapabilities)) {
    const smart_objects::SmartObject& button_caps =
        rc_capabilities[strings::kbuttonCapabilities];
    smart_objects::SmartArray::iterator it = button_caps.asArray()->begin();
    for (; it != button_caps.asArray()->end(); ++it) {
      smart_objects::SmartObject& so = *it;
      if (so[message_params::kName] == button_name) {
        LOG4CXX_TRACE(logger_,
                      "Button " << button_name << " exist in capabilities");
        return true;
      }
    }
  }
  LOG4CXX_TRACE(logger_,
                "Button " << button_name << " do not exist in capabilities");
  return false;
}

bool CheckButtonName(const std::string& module_type,
                     const std::string& button_name,
                     const smart_objects::SmartObject* rc_capabilities) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (rc_capabilities == NULL) {
    LOG4CXX_ERROR(logger_, "No remote controll capabilities available");
    return false;
  }

  if (enums_value::kRadio == module_type) {
    if (!helpers::in_range(buttons_radio(), button_name)) {
      LOG4CXX_WARN(logger_,
                   "Trying to acceess climate button with module type radio");
      return false;
    }
  }

  if (enums_value::kClimate == module_type) {
    if (!helpers::in_range(buttons_climate(), button_name)) {
      LOG4CXX_WARN(logger_,
                   "Trying to acceess radio button with module type climate");
      return false;
    }
  }
  return CheckIfButtonExistInRCCaps(*rc_capabilities, button_name);
}

void ButtonPressRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  const Json::Value request_params =
      MessageHelper::StringToValue(message_->json_message());

  const std::string button_name = request_params[kButtonName].asString();
  const std::string module_type = request_params[kModuleType].asString();
  const bool button_name_matches_module_type =
      CheckButtonName(module_type, button_name, service()->GetRCCapabilities());

  if (button_name_matches_module_type) {
    SendRequest(functional_modules::hmi_api::button_press, request_params);
  } else {
    LOG4CXX_WARN(logger_, "Request module type and button name mismatch!");
    SendResponse(false,
                 result_codes::kInvalidData,
                 "Request module type and button name mismatch!");
  }
}

AcquireResult::eType ButtonPressRequest::AcquireResource(
    const Json::Value& message) {
  ResourceAllocationManager& allocation_manager =
      rc_module_.resource_allocation_manager();
  const std::string& module_type = ModuleType(message);
  const uint32_t app_id = app()->app_id();
  return allocation_manager.AcquireResource(module_type, app_id);
}

bool ButtonPressRequest::IsResourceFree(const std::string& module_type) const {
  return rc_module_.resource_allocation_manager().IsResourceFree(module_type);
}

void ButtonPressRequest::SetResourceState(const Json::Value& message,
                                          const ResourceState::eType state) {
  const std::string& module_type = ModuleType(message);
  const uint32_t app_id = app()->app_id();

  ResourceAllocationManager& allocation_manager =
      rc_module_.resource_allocation_manager();
  allocation_manager.SetResourceState(module_type, app_id, state);
}

void ButtonPressRequest::OnEvent(
    const rc_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(
      (functional_modules::hmi_api::button_press == event.id()));

  application_manager::Message& hmi_response = *(event.event_message());
  const Json::Value value =
      MessageHelper::StringToValue(hmi_response.json_message());

  std::string result_code;
  std::string info;

  bool is_response_successful = ParseResultCode(value, result_code, info);

  if (remote_control::result_codes::kReadOnly == result_code) {
    is_response_successful = false;
    result_code = result_codes::kGenericError;
  }
  SendResponse(is_response_successful, result_code.c_str(), info);
}

std::string ButtonPressRequest::ModuleType(const Json::Value& message) {
  return message.get(message_params::kModuleType, Json::Value("")).asString();
}

}  // namespace commands

}  // namespace remote_control
