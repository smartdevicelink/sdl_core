/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Copyright (c) 2017 Xevo Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the copyright holders nor the names of their contributors
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

#include "rc_rpc_plugin/rc_command_factory.h"
#include "rc_rpc_plugin/commands/mobile/button_press_request.h"
#include "rc_rpc_plugin/commands/mobile/button_press_response.h"
#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_request.h"
#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_response.h"
#include "rc_rpc_plugin/commands/mobile/set_interior_vehicle_data_request.h"
#include "rc_rpc_plugin/commands/mobile/set_interior_vehicle_data_response.h"
#include "rc_rpc_plugin/commands/mobile/on_interior_vehicle_data_notification.h"
#include "rc_rpc_plugin/commands/mobile/on_remote_control_settings_notification.h"

#include "interfaces/MOBILE_API.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "RCCommandFactory")

namespace rc_rpc_plugin {
using namespace application_manager;

RCCommandFactory::RCCommandFactory(
    app_mngr::ApplicationManager& app_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : app_manager_(app_manager)
    , rpc_service_(rpc_service)
    , hmi_capabilities_(hmi_capabilities)
    , policy_handler_(policy_handler) {}

CommandSharedPtr RCCommandFactory::CreateCommand(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::commands::Command::CommandSource source) {
  if (app_mngr::commands::Command::SOURCE_HMI == source) {
    return CreateHMICommand(message, source);
  } else {
    return CreateMobileCommand(message, source);
  }
}

CommandSharedPtr RCCommandFactory::CreateMobileCommand(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::commands::Command::CommandSource source) {
  CommandSharedPtr command;
  const int function_id =
      (*message)[strings::params][strings::function_id].asInt();
  LOG4CXX_DEBUG(
      logger_,
      "MobileCommandFactory::CreateCommand function_id: " << function_id);
  switch (function_id) {
    case mobile_apis::FunctionID::ButtonPressID: {
      if ((*message)[strings::params][strings::message_type] ==
          static_cast<int>(application_manager::MessageType::kRequest)) {
        command.reset(new commands::ButtonPressRequest(message, app_manager_));
      } else {
        command.reset(new commands::ButtonPressRequest(message, app_manager_));
      }
    }

    break;
    default:
      break;
  }
  return command;
}

CommandSharedPtr RCCommandFactory::CreateHMICommand(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::commands::Command::CommandSource source) {
  CommandSharedPtr command;
  return command;
}

}  // namespace application_manager
