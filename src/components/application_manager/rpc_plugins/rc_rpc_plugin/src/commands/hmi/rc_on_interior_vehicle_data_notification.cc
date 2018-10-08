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

#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/commands/hmi/rc_on_interior_vehicle_data_notification.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

RCOnInteriorVehicleDataNotification::RCOnInteriorVehicleDataNotification(
    const app_mngr::commands::MessageSharedPtr& message,
    const RCCommandParams& params)
    : application_manager::commands::NotificationFromHMI(
          message,
          params.application_manager_,
          params.rpc_service_,
          params.hmi_capabilities_,
          params.policy_handler_) {}

RCOnInteriorVehicleDataNotification::~RCOnInteriorVehicleDataNotification() {}

void RCOnInteriorVehicleDataNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  (*message_)[app_mngr::strings::params][app_mngr::strings::function_id] =
      static_cast<int>(mobile_apis::FunctionID::eType::OnInteriorVehicleDataID);

  smart_objects::SmartObject& module_data = (*message_)
      [application_manager::strings::msg_params][message_params::kModuleData];
  if (module_data.keyExists(rc_rpc_plugin::message_params::kAudioControlData)) {
    smart_objects::SmartObject& audio_control_data =
        module_data[message_params::kAudioControlData];
    if (audio_control_data.keyExists(message_params::kKeepContext)) {
      audio_control_data.erase(message_params::kKeepContext);
    }
  }

  SendNotificationToMobile(message_);
}

}  // namespace commands
}  // namespace rc_rpc_plugin
