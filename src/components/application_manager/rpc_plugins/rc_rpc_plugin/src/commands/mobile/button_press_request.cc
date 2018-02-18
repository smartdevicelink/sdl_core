/*
 * Copyright (c) 2018, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "rc_rpc_plugin/commands/mobile/button_press_request.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/macro.h"
#include "json/json.h"
#include "utils/helpers.h"
#include "interfaces/MOBILE_API.h"

namespace rc_rpc_plugin {
namespace commands {

using namespace json_keys;
using namespace message_params;

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

typedef std::map<std::string, mobile_apis::ButtonName::eType> ButtonsMap;

ButtonPressRequest::ButtonPressRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle,
    ResourceAllocationManager& resource_allocation_manager)
    : RCCommandRequest(message,
                       application_manager,
                       rpc_service,
                       hmi_capabilities,
                       policy_handle,
                       resource_allocation_manager) {}

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

const ButtonsMap buttons_map() {
  using namespace mobile_apis;

  ButtonsMap buttons_map;
  buttons_map[enums_value::kACMax] = ButtonName::AC_MAX;
  buttons_map[enums_value::kAC] = ButtonName::AC;
  buttons_map[enums_value::kRecirculate] = ButtonName::RECIRCULATE;
  buttons_map[enums_value::kFanUp] = ButtonName::FAN_UP;
  buttons_map[enums_value::kFanDown] = ButtonName::FAN_DOWN;
  buttons_map[enums_value::kTempUp] = ButtonName::TEMP_UP;
  buttons_map[enums_value::kTempDown] = ButtonName::TEMP_DOWN;
  buttons_map[enums_value::kDefrostMax] = ButtonName::DEFROST_MAX;
  buttons_map[enums_value::kDefrost] = ButtonName::DEFROST;
  buttons_map[enums_value::kDefrostRear] = ButtonName::DEFROST_REAR;
  buttons_map[enums_value::kUpperVent] = ButtonName::UPPER_VENT;
  buttons_map[enums_value::kLowerVent] = ButtonName::LOWER_VENT;
  buttons_map[enums_value::kVolumeUp] = ButtonName::VOLUME_UP;
  buttons_map[enums_value::kVolumeDown] = ButtonName::VOLUME_DOWN;
  buttons_map[enums_value::kEject] = ButtonName::EJECT;
  buttons_map[enums_value::kSource] = ButtonName::SOURCE;
  buttons_map[enums_value::kShuffle] = ButtonName::SHUFFLE;
  buttons_map[enums_value::kRepeat] = ButtonName::REPEAT;

  return buttons_map;
}

bool CheckIfButtonExistInRCCaps(
    const smart_objects::SmartObject& rc_capabilities,
    const mobile_apis::ButtonName::eType button) {
  if (rc_capabilities.keyExists(strings::kbuttonCapabilities)) {
    const smart_objects::SmartObject& button_caps =
        rc_capabilities[strings::kbuttonCapabilities];
    auto it = button_caps.asArray()->begin();
    for (; it != button_caps.asArray()->end(); ++it) {
      smart_objects::SmartObject& so = *it;
      int64_t current_id = so[message_params::kName].asInt();
      if (-1 == current_id) {
        // capabilities received from HMI contains enum values
        // capabilities loaded from file contains string values
        // TODO : unificate capabilities storing
        const std::string& bt_name = so[message_params::kName].asString();
        static ButtonsMap btn_map = buttons_map();
        current_id = btn_map[bt_name];
      }
      const mobile_apis::ButtonName::eType current_button =
          static_cast<mobile_apis::ButtonName::eType>(current_id);
      if (current_button == button) {
        LOG4CXX_TRACE(logger_,
                      "Button id " << current_button
                                   << " exist in capabilities");
        return true;
      }
    }
  }
  LOG4CXX_TRACE(logger_,
                "Button id " << button << " do not exist in capabilities");
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
  return true;
}

void ButtonPressRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  const char* button_name;
  NsSmartDeviceLink::NsSmartObjects::
      EnumConversionHelper<mobile_apis::ButtonName::eType>::EnumToCString(
          static_cast<mobile_apis::ButtonName::eType>(
              (*message_)[app_mngr::strings::msg_params]
                         [message_params::kButtonName].asUInt()),
          &button_name);

  const std::string module_type = ModuleType();
  static ButtonsMap btn_map = buttons_map();
  mobile_apis::ButtonName::eType button_id =
      mobile_apis::ButtonName::INVALID_ENUM;
  if (btn_map.end() != btn_map.find(button_name)) {
    button_id = btn_map[button_name];
  }

  const smart_objects::SmartObject* rc_capabilities =
      application_manager_.hmi_capabilities().rc_capability();
  const bool button_name_matches_module_type =
      CheckButtonName(module_type, button_name, rc_capabilities);
  const bool button_id_exist_in_caps =
      rc_capabilities &&
      CheckIfButtonExistInRCCaps(*rc_capabilities, button_id);

  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(connection_key());

  (*message_)[app_mngr::strings::msg_params][app_mngr::strings::app_id] =
      app->app_id();

  if (button_name_matches_module_type && button_id_exist_in_caps) {
    SendHMIRequest(hmi_apis::FunctionID::Buttons_ButtonPress,
                   &(*message_)[app_mngr::strings::msg_params],
                   true);
  } else if (!button_name_matches_module_type) {
    LOG4CXX_WARN(logger_, "Request module type and button name mismatch!");
    SetResourceState(module_type, ResourceState::FREE);
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "Request module type and button name mismatch!");
  } else {
    LOG4CXX_WARN(logger_, "Requested button is not exists in capabilities!");
    SetResourceState(module_type, ResourceState::FREE);
    SendResponse(false,
                 mobile_apis::Result::UNSUPPORTED_RESOURCE,
                 "Requested button is not exists in capabilities!");
  }
}

AcquireResult::eType ButtonPressRequest::AcquireResource(
    const app_mngr::commands::MessageSharedPtr& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string module_type = ModuleType();
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());
  return resource_allocation_manager_.AcquireResource(module_type,
                                                      app->app_id());
}

bool ButtonPressRequest::IsResourceFree(const std::string& module_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return resource_allocation_manager_.IsResourceFree(module_type);
}

void ButtonPressRequest::SetResourceState(const std::string& module_type,
                                          const ResourceState::eType state) {
  LOG4CXX_AUTO_TRACE(logger_);
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());
  resource_allocation_manager_.SetResourceState(
      module_type, app->app_id(), state);
}

void ButtonPressRequest::on_event(const app_mngr::event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  RCCommandRequest::on_event(event);

  if (hmi_apis::FunctionID::Buttons_ButtonPress != event.id()) {
    return;
  }

  const smart_objects::SmartObject& message = event.smart_object();
  mobile_apis::Result::eType result_code =
      GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
          message[app_mngr::strings::params][app_mngr::hmi_response::code]
              .asUInt()));

  bool result =
      helpers::Compare<mobile_apis::Result::eType, helpers::EQ, helpers::ONE>(
          result_code,
          mobile_apis::Result::SUCCESS,
          mobile_apis::Result::WARNINGS);

  if (mobile_apis::Result::READ_ONLY == result_code) {
    result = false;
    result_code = mobile_apis::Result::GENERIC_ERROR;
  }
  std::string response_info;
  GetInfo(message, response_info);
  SetResourceState(ModuleType(), ResourceState::FREE);
  SendResponse(result, result_code, response_info.c_str());
}

std::string ButtonPressRequest::ModuleType() {
  mobile_apis::ModuleType::eType module_type = static_cast<
      mobile_apis::ModuleType::eType>(
      (*message_)[app_mngr::strings::msg_params][message_params::kModuleType]
          .asUInt());
  const char* str;
  const bool ok = NsSmartDeviceLink::NsSmartObjects::EnumConversionHelper<
      mobile_apis::ModuleType::eType>::EnumToCString(module_type, &str);
  return ok ? str : "unknown";
}

}  // namespace commands
}  // namespace rc_rpc_plugin
