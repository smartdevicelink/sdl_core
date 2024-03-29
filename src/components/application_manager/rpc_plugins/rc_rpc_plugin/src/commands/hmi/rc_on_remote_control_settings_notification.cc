/*
 Copyright (c) 2018, Ford Motor Company
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

#include "rc_rpc_plugin/commands/hmi/rc_on_remote_control_settings_notification.h"
#include "rc_rpc_plugin/interior_data_manager.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

namespace {
std::map<std::string, hmi_apis::Common_RCAccessMode::eType> access_modes{
    {enums_value::kAutoAllow, hmi_apis::Common_RCAccessMode::AUTO_ALLOW},
    {enums_value::kAutoDeny, hmi_apis::Common_RCAccessMode::AUTO_DENY},
    {enums_value::kAskDriver, hmi_apis::Common_RCAccessMode::ASK_DRIVER}};
}

SDL_CREATE_LOG_VARIABLE("Commands")

RCOnRemoteControlSettingsNotification::RCOnRemoteControlSettingsNotification(
    const app_mngr::commands::MessageSharedPtr& message,
    const RCCommandParams& params)
    : application_manager::commands::NotificationFromHMI(
          message,
          params.application_manager_,
          params.rpc_service_,
          params.hmi_capabilities_,
          params.policy_handler_)
    , resource_allocation_manager_(params.resource_allocation_manager_)
    , interior_data_manager_(params.interior_data_manager_)
    , rc_consent_manager_(params.rc_consent_manager_) {}

RCOnRemoteControlSettingsNotification::
    ~RCOnRemoteControlSettingsNotification() {}

std::string AccessModeToString(
    const hmi_apis::Common_RCAccessMode::eType access_mode) {
  std::map<std::string, hmi_apis::Common_RCAccessMode::eType>::const_iterator
      it = access_modes.begin();
  for (; access_modes.end() != it; ++it) {
    if (access_mode == it->second) {
      return it->first;
    }
  }
  const std::string error = "UNKNOW_ACCESS_MODE";
  DCHECK_OR_RETURN(false, error);
  return error;
}

void RCOnRemoteControlSettingsNotification::DisallowRCFunctionality() {
  SDL_LOG_AUTO_TRACE();
  interior_data_manager_.OnDisablingRC();
}

void RCOnRemoteControlSettingsNotification::Run() {
  SDL_LOG_AUTO_TRACE();

  if ((*message_)[app_mngr::strings::msg_params].empty()) {
    SDL_LOG_DEBUG("Notification is ignored due to absence of any parameters");
    SDL_LOG_DEBUG("RC Functionality remains unchanged");
    return;
  }

  ProcessAccessModeParam();
  ProcessAllowedParam();
}

void RCOnRemoteControlSettingsNotification::ProcessAccessModeParam() {
  if (!(*message_)[app_mngr::strings::msg_params].keyExists(
          message_params::kAccessMode)) {
    SDL_LOG_DEBUG(
        "No access mode received. Using last known: "
        << AccessModeToString(resource_allocation_manager_.GetAccessMode()));
    return;
  }

  const auto access_mode = static_cast<hmi_apis::Common_RCAccessMode::eType>(
      (*message_)[app_mngr::strings::msg_params][message_params::kAccessMode]
          .asUInt());
  SDL_LOG_DEBUG("Setting up access mode : " << AccessModeToString(access_mode));
  resource_allocation_manager_.SetAccessMode(access_mode);
}

void RCOnRemoteControlSettingsNotification::ProcessAllowedParam() {
  if (!(*message_)[app_mngr::strings::msg_params].keyExists(
          message_params::kAllowed)) {
    SDL_LOG_DEBUG("No allowed param received. Using last known: "
                  << std::boolalpha
                  << resource_allocation_manager_.is_rc_enabled());
    return;
  }

  const bool is_allowed =
      (*message_)[app_mngr::strings::msg_params][message_params::kAllowed]
          .asBool();

  if (is_allowed) {
    SDL_LOG_DEBUG("Allowing RC Functionality");
    resource_allocation_manager_.set_rc_enabled(true);
    return;
  }

  SDL_LOG_DEBUG("Disallowing RC Functionality");
  DisallowRCFunctionality();
  resource_allocation_manager_.ResetAllAllocations();
  resource_allocation_manager_.set_rc_enabled(false);
  rc_consent_manager_.RemoveAllConsents();
}

}  // namespace commands
}  // namespace rc_rpc_plugin
