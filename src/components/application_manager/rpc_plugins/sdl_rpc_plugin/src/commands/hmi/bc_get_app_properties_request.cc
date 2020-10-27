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

#include "sdl_rpc_plugin/commands/hmi/bc_get_app_properties_request.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;
namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

BCGetAppPropertiesRequest::BCGetAppPropertiesRequest(
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

void BCGetAppPropertiesRequest::FillAppProperties(
    const std::string& policy_app_id,
    smart_objects::SmartObject& out_properties) const {
  SDL_LOG_AUTO_TRACE();

  policy::AppProperties app_properties;
  const bool result =
      policy_handler_.GetAppProperties(policy_app_id, app_properties);

  if (!result) {
    SDL_LOG_DEBUG(
        "Failed to get app parameters for policy_app_id: " << policy_app_id);
    return;
  }

  out_properties[strings::policy_app_id] = policy_app_id;
  out_properties[strings::enabled] = app_properties.enabled;

  policy::StringArray nicknames;
  policy::StringArray app_hmi_types;

  policy_handler_.GetInitialAppData(policy_app_id, &nicknames, &app_hmi_types);

  smart_objects::SmartObject nicknames_array(smart_objects::SmartType_Array);
  size_t i = 0;
  for (const auto& nickname : nicknames) {
    nicknames_array[i++] = nickname;
  }
  out_properties[strings::nicknames] = nicknames_array;

  if (!app_properties.auth_token.empty()) {
    out_properties[strings::auth_token] = app_properties.auth_token;
  }
  if (!app_properties.transport_type.empty()) {
    out_properties[strings::transport_type] = app_properties.transport_type;
  }
  if (!app_properties.hybrid_app_preference.empty()) {
    out_properties[strings::hybrid_app_preference] =
        app_properties.hybrid_app_preference;
  }
  if (!app_properties.endpoint.empty()) {
    out_properties[strings::endpoint] = app_properties.endpoint;
  }
}

void BCGetAppPropertiesRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  const auto& msg_params = (*message_)[strings::msg_params];
  smart_objects::SmartObject response_params(smart_objects::SmartType_Map);

  if (msg_params.keyExists(strings::policy_app_id)) {
    const auto policy_app_id = msg_params[strings::policy_app_id].asString();
    smart_objects::SmartObject properties(smart_objects::SmartType_Map);
    FillAppProperties(policy_app_id, properties);
    if (!properties.empty()) {
      response_params[strings::properties][0] = properties;
    }
  } else {
    SDL_LOG_DEBUG(
        "policyAppID was absent in request, all apps properties "
        "will be returned.");
    const auto app_ids = policy_handler_.GetApplicationPolicyIDs();
    int i = 0;
    for (auto& app_id : app_ids) {
      smart_objects::SmartObject properties(smart_objects::SmartType_Map);
      FillAppProperties(app_id, properties);
      response_params[strings::properties][i++] = properties;
    }
  }

  if (response_params[strings::properties].empty()) {
    SendErrorResponse(
        (*message_)[strings::params][strings::correlation_id].asUInt(),
        hmi_apis::FunctionID::BasicCommunication_GetAppProperties,
        hmi_apis::Common_Result::DATA_NOT_AVAILABLE,
        "Requested data not available",
        application_manager::commands::Command::SOURCE_SDL_TO_HMI);
    return;
  }

  SendResponse(true,
               (*message_)[strings::params][strings::correlation_id].asUInt(),
               hmi_apis::FunctionID::BasicCommunication_GetAppProperties,
               hmi_apis::Common_Result::SUCCESS,
               &response_params);
}

}  // namespace commands
}  // namespace sdl_rpc_plugin
