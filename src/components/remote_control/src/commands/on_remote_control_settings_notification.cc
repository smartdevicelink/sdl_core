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

#include "remote_control/commands/on_remote_control_settings_notification.h"
#include <algorithm>
#include <vector>
#include "json/json.h"
#include "remote_control/rc_module_constants.h"
#include "remote_control/message_helper.h"
#include "remote_control/remote_control_plugin.h"
#include "remote_control/rc_app_extension.h"
#include "functional_module/function_ids.h"

namespace remote_control {

namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "OnRemoteControlSettingsNotification");

OnRemoteControlSettingsNotification::OnRemoteControlSettingsNotification(
    const application_manager::MessagePtr& message,
    RemotePluginInterface& rc_module)
    : BaseCommandNotification(message, rc_module) {}

void UnsubscribeFromInteriorVehicleDataForAllModules(
    RCAppExtensionPtr extension) {
  LOG4CXX_AUTO_TRACE(logger_);
  const Json::Value climate(enums_value::kClimate);
  extension->UnsubscribeFromInteriorVehicleData(climate);
  const Json::Value radio(enums_value::kRadio);
  extension->UnsubscribeFromInteriorVehicleData(radio);
}

void OnRemoteControlSettingsNotification::DisallowRCFunctionality() {
  LOG4CXX_AUTO_TRACE(logger_);
  typedef std::vector<application_manager::ApplicationSharedPtr> Apps;
  Apps apps = service_->GetApplications(rc_module_.GetModuleID());
  for (Apps::iterator it = apps.begin(); it != apps.end(); ++it) {
    application_manager::ApplicationSharedPtr app = *it;
    DCHECK(app);
    service_->ChangeNotifyHMILevel(app, mobile_apis::HMILevel::eType::HMI_NONE);

    const RCAppExtensionPtr extension =
        application_manager::AppExtensionPtr::static_pointer_cast<
            RCAppExtension>(app->QueryInterface(rc_module_.GetModuleID()));
    if (extension) {
      UnsubscribeFromInteriorVehicleDataForAllModules(extension);
    }
  }
}

void OnRemoteControlSettingsNotification::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);
  const Json::Value value =
      MessageHelper::StringToValue(message()->json_message());

  if (!value.isMember(message_params::kAllowed)) {
    LOG4CXX_DEBUG(logger_,
                  "Notification is ignored due to \"allow\" parameter absense");
    LOG4CXX_DEBUG(logger_, "RC Functionality remains unchanged");
    return;
  }
  const bool is_allowed = value[message_params::kAllowed].asBool();
  if (is_allowed) {
    LOG4CXX_DEBUG(logger_, "Allowing RC Functionality");
    const std::string access_mode =
        value.get(message_params::kAccessMode, enums_value::kAutoAllow)
            .asString();

    const hmi_apis::Common_RCAccessMode::eType access_mode_ =
        MessageHelper::AccessModeFromString(access_mode);
    ResourceAllocationManager& allocation_manager =
        rc_module_.resource_allocation_manager();
    LOG4CXX_DEBUG(logger_, "Setting up access mode : " << access_mode);
    allocation_manager.SetAccessMode(access_mode_);
  } else {
    LOG4CXX_DEBUG(logger_, "Disallowing RC Functionality");
    DisallowRCFunctionality();
  }
}

}  // namespace commands

}  // namespace remote_control
