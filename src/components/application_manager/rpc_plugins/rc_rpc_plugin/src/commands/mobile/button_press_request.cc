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
#include "interfaces/MOBILE_API.h"
#include "json/json.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/helpers.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

using namespace json_keys;
using namespace message_params;

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

ButtonPressRequest::ButtonPressRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    const RCCommandParams& params)
    : RCCommandRequest(message, params) {}

ButtonPressRequest::~ButtonPressRequest() {}

std::string ButtonPressRequest::GetButtonName() const {
  mobile_apis::ButtonName::eType button_name =
      static_cast<mobile_apis::ButtonName::eType>(
          (*message_)[app_mngr::strings::msg_params]
                     [message_params::kButtonName]
                         .asUInt());
  const char* str;
  const bool ok = ns_smart_device_link::ns_smart_objects::EnumConversionHelper<
      mobile_apis::ButtonName::eType>::EnumToCString(button_name, &str);
  return ok ? str : "unknown";
}

const mobile_apis::ButtonName::eType ButtonPressRequest::GetButtonId() const {
  const auto button_name = GetButtonName();
  static RCHelpers::ButtonsMap btn_map = RCHelpers::buttons_map();
  mobile_apis::ButtonName::eType button_id =
      mobile_apis::ButtonName::INVALID_ENUM;
  if (btn_map.end() != btn_map.find(button_name)) {
    button_id = btn_map[button_name];
  }
  return button_id;
}

void ButtonPressRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string module_type = ModuleType();

  const bool button_name_matches_module_type =
      rc_capabilities_manager_.CheckButtonName(module_type, GetButtonName());

  const std::string module_id = ModuleId();
  const ModuleUid module(module_type, module_id);
  const bool is_module_exists =
      rc_capabilities_manager_.CheckIfModuleExistsInCapabilities(module);

  const bool button_valid_by_caps =
      is_module_exists &&
      rc_capabilities_manager_.CheckIfButtonExistInRCCaps(GetButtonId());

  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(connection_key());

  (*message_)[app_mngr::strings::msg_params][app_mngr::strings::app_id] =
      app->app_id();

  if (button_name_matches_module_type && button_valid_by_caps) {
    (*message_)[app_mngr::strings::msg_params][message_params::kModuleId] =
        module_id;
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
    LOG4CXX_WARN(logger_,
                 "Requested button or module does not exist in capabilities!");
    SetResourceState(module_type, ResourceState::FREE);
    SendResponse(false,
                 mobile_apis::Result::UNSUPPORTED_RESOURCE,
                 "Requested button or module does not exist in capabilities!");
  }
}

AcquireResult::eType ButtonPressRequest::AcquireResource(
    const app_mngr::commands::MessageSharedPtr& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string module_type = ModuleType();
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());

  return resource_allocation_manager_.AcquireResource(
      module_type, ModuleId(), app->app_id());
}

bool ButtonPressRequest::IsResourceFree(const std::string& module_type,
                                        const std::string& module_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return resource_allocation_manager_.IsResourceFree(module_type, module_id);
}

void ButtonPressRequest::SetResourceState(const std::string& module_type,
                                          const ResourceState::eType state) {
  LOG4CXX_AUTO_TRACE(logger_);
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());

  resource_allocation_manager_.SetResourceState(
      module_type, ModuleId(), app->app_id(), state);
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

  const std::string module_type = ModuleType();
  const std::string module_id = ModuleId();

  const rc_rpc_types::ModuleUid resource{module_type, module_id};
  auto app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED, "");
    return;
  }

  const auto app_id = app->app_id();

  bool is_resource_acquired = false;

  if (result && !resource_allocation_manager_.IsResourceAlreadyAcquiredByApp(
                    resource, app_id)) {
    resource_allocation_manager_.SetResourceAcquired(
        module_type, module_id, app_id);

    is_resource_acquired = true;
  }

  std::string response_info;
  GetInfo(message, response_info);
  SendResponse(result, result_code, response_info.c_str());

  if (is_resource_acquired) {
    resource_allocation_manager_.SendOnRCStatusNotifications(
        NotificationTrigger::MODULE_ALLOCATION,
        std::shared_ptr<application_manager::Application>());
  }
}

std::string ButtonPressRequest::ModuleType() const {
  mobile_apis::ModuleType::eType module_type =
      static_cast<mobile_apis::ModuleType::eType>(
          (*message_)[app_mngr::strings::msg_params]
                     [message_params::kModuleType]
                         .asUInt());
  const char* str;
  const bool ok = ns_smart_device_link::ns_smart_objects::EnumConversionHelper<
      mobile_apis::ModuleType::eType>::EnumToCString(module_type, &str);
  return ok ? str : "unknown";
}

std::string ButtonPressRequest::ModuleId() const {
  LOG4CXX_AUTO_TRACE(logger_);
  auto msg_params = (*message_)[app_mngr::strings::msg_params];
  if (msg_params.keyExists(message_params::kModuleId)) {
    return msg_params[message_params::kModuleId].asString();
  }
  const std::string module_id =
      rc_capabilities_manager_.GetDefaultModuleIdFromCapabilities(ModuleType());
  return module_id;
}

}  // namespace commands
}  // namespace rc_rpc_plugin
