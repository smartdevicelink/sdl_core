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

#include "sdl_rpc_plugin/commands/mobile/delete_command_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "utils/helpers.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

DeleteCommandRequest::DeleteCommandRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromMobileImpl(message,
                            application_manager,
                            rpc_service,
                            hmi_capabilities,
                            policy_handler)
    , ui_result_(hmi_apis::Common_Result::INVALID_ENUM)
    , vr_result_(hmi_apis::Common_Result::INVALID_ENUM) {}

DeleteCommandRequest::~DeleteCommandRequest() {}

void DeleteCommandRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (!application) {
    SDL_LOG_ERROR("Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const int32_t cmd_id =
      (*message_)[strings::msg_params][strings::cmd_id].asInt();

  smart_objects::SmartObject command = application->FindCommand(cmd_id);

  if (smart_objects::SmartType::SmartType_Null == command.getType()) {
    SDL_LOG_ERROR("Command with id " << cmd_id << " is not found.");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::cmd_id] =
      (*message_)[strings::msg_params][strings::cmd_id];
  msg_params[strings::app_id] = application->app_id();

  /* Need to set all flags before sending request to HMI
   * for correct processing this flags in method on_event */
  if (command.keyExists(strings::menu_params)) {
    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
    SendHMIRequest(hmi_apis::FunctionID::UI_DeleteCommand, &msg_params, true);
  }
  // check vr params
  if (command.keyExists(strings::vr_commands)) {
    // VR params
    msg_params[strings::grammar_id] = application->get_grammar_id();
    msg_params[strings::type] = hmi_apis::Common_VRCommandType::Command;
    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VR);
    SendHMIRequest(hmi_apis::FunctionID::VR_DeleteCommand, &msg_params, true);
  }
}

bool DeleteCommandRequest::PrepareResponseParameters(
    mobile_apis::Result::eType& result_code, std::string& info) {
  using namespace helpers;
  app_mngr::commands::ResponseInfo ui_delete_info(
      ui_result_, HmiInterfaces::HMI_INTERFACE_UI, application_manager_);
  app_mngr::commands::ResponseInfo vr_delete_info(
      vr_result_, HmiInterfaces::HMI_INTERFACE_VR, application_manager_);
  const bool result =
      PrepareResultForMobileResponse(ui_delete_info, vr_delete_info);

  const bool is_vr_or_ui_warning =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          hmi_apis::Common_Result::WARNINGS, ui_result_, vr_result_);
  info = app_mngr::commands::MergeInfos(
      ui_delete_info, ui_info_, vr_delete_info, vr_info_);
  if (is_vr_or_ui_warning && !ui_delete_info.is_unsupported_resource &&
      !vr_delete_info.is_unsupported_resource) {
    SDL_LOG_DEBUG("VR or UI result is warning");
    result_code = mobile_apis::Result::WARNINGS;
    return result;
  }
  result_code = PrepareResultCodeForResponse(ui_delete_info, vr_delete_info);
  SDL_LOG_DEBUG("Result is " << (result ? "true" : "false"));
  return result;
}

void DeleteCommandRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();

  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::UI_DeleteCommand: {
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      ui_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      SDL_LOG_DEBUG("Received UI_DeleteCommand event with result "
                    << EnumToString(ui_result_));
      GetInfo(message, ui_info_);
      break;
    }
    case hmi_apis::FunctionID::VR_DeleteCommand: {
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VR);
      vr_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      SDL_LOG_DEBUG("Received VR_DeleteCommand event with result "
                    << EnumToString(vr_result_));
      GetInfo(message, vr_info_);
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event" << event.id());
      return;
    }
  }

  if (IsPendingResponseExist()) {
    SDL_LOG_DEBUG("Still awaiting for other responses.");
    return;
  }

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (!application) {
    SDL_LOG_ERROR("Application is not registered");
    return;
  }
  smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];

  const uint32_t cmd_id = msg_params[strings::cmd_id].asUInt();

  const auto command = application->FindCommand(cmd_id);

  if (smart_objects::SmartType_Null == command.getType()) {
    SDL_LOG_ERROR("Command id " << cmd_id
                                << " not found for "
                                   "application with connection key "
                                << connection_key());
    return;
  }
  mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;
  std::string info;
  const bool result = PrepareResponseParameters(result_code, info);
  if (result) {
    application->RemoveCommand(cmd_id);
    application->help_prompt_manager().OnVrCommandDeleted(cmd_id, false);
  }
  SendResponse(
      result, result_code, info.empty() ? NULL : info.c_str(), &msg_params);
}

bool DeleteCommandRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
