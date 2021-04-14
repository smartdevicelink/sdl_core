/*
 Copyright (c) 2020, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

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

#include "sdl_rpc_plugin/commands/hmi/bc_set_app_properties_request.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;
namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

BCSetAppPropertiesRequest::BCSetAppPropertiesRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromHMI(message,
                     application_manager,
                     rpc_service,
                     hmi_capabilities,
                     policy_handler) {}

void BCSetAppPropertiesRequest::Run() {
  SDL_LOG_AUTO_TRACE();
  const auto& properties =
      (*message_)[strings::msg_params][strings::properties];

  const auto policy_app_id(properties[strings::policy_app_id].asString());

  const auto properties_change_status =
      policy_handler_.GetAppPropertiesStatus(properties, policy_app_id);

  using AppPropertiesState = policy::PolicyHandlerInterface::AppPropertiesState;
  const bool are_properties_changed =
      AppPropertiesState::NO_CHANGES != properties_change_status;

  const bool is_new_app = policy_handler_.IsNewApplication(policy_app_id);

  policy_handler_.OnSetAppProperties(properties);
  SendResponse(true,
               (*message_)[strings::params][strings::correlation_id].asUInt(),
               hmi_apis::FunctionID::BasicCommunication_SetAppProperties,
               hmi_apis::Common_Result::SUCCESS);

  if (are_properties_changed || is_new_app) {
    const auto notification =
        MessageHelper::CreateOnAppPropertiesChangeNotification(
            policy_app_id, application_manager_);
    application_manager_.GetRPCService().ManageHMICommand(notification);
  }
  if (is_new_app) {
    SDL_LOG_ERROR("Message contains unknow policyAppId: "
                  << policy_app_id << ", PTU will be triggered");
    policy_handler_.OnLocalAppAdded();
  }

  auto app_enabled = [this]() -> bool {
    auto& properties = (*message_)[strings::msg_params][strings::properties];
    if (properties.keyExists(strings::enabled)) {
      return properties[strings::enabled].asBool();
    }
    return false;
  };

  const bool enable_flag_switch =
      AppPropertiesState::ENABLED_FLAG_SWITCH == properties_change_status;

  if (app_enabled() && (enable_flag_switch || is_new_app)) {
    application_manager_.CreatePendingLocalApplication(policy_app_id);
    application_manager_.SendUpdateAppList();
    return;
  }

  if (enable_flag_switch) {
    application_manager_.RemovePendingApplication(policy_app_id);
    application_manager_.SendUpdateAppList();
  }
}

}  // namespace commands
}  // namespace sdl_rpc_plugin
