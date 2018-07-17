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

#include "utils/shared_ptr_helpers.h"
#include <memory>
#include "remote_control/rc_command_factory.h"
#include "functional_module/function_ids.h"
#include "remote_control/commands/get_interior_vehicle_data_request.h"
#include "remote_control/commands/set_interior_vehicle_data_request.h"
#include "remote_control/commands/button_press_request.h"
#include "remote_control/commands/on_interior_vehicle_data_notification.h"
#include "remote_control/commands/on_remote_control_settings_notification.h"

namespace remote_control {

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControl")

using functional_modules::RCFunctionID;

std::shared_ptr<commands::Command> RCCommandFactory::CreateCommand(
    const application_manager::MessagePtr& msg,
    RemotePluginInterface& rc_module) {
  switch (msg->function_id()) {
    case RCFunctionID::GET_INTERIOR_VEHICLE_DATA: {
      return std::make_shared<commands::GetInteriorVehicleDataRequest>(
          msg, rc_module);
      break;
    }
    case RCFunctionID::SET_INTERIOR_VEHICLE_DATA: {
      return std::make_shared<commands::SetInteriorVehicleDataRequest>(
          msg, rc_module);
      break;
    }
    case RCFunctionID::BUTTON_PRESS: {
      return std::make_shared<commands::ButtonPressRequest>(msg, rc_module);
      break;
    }
    case RCFunctionID::ON_INTERIOR_VEHICLE_DATA: {
      return std::make_shared<commands::OnInteriorVehicleDataNotification>(
          msg, rc_module);
      break;
    }
    case RCFunctionID::ON_REMOTE_CONTROL_SETTINGS: {
      return std::make_shared<commands::OnRemoteControlSettingsNotification>(
          msg, rc_module);
      break;
    }
    default: {
      std::shared_ptr<commands::Command> invalid_command;
      LOG4CXX_DEBUG(logger_,
                    "RSDL unable to proces function " << msg->function_id());
      return invalid_command;
    }
  }
}

}  // namespace remote_control
