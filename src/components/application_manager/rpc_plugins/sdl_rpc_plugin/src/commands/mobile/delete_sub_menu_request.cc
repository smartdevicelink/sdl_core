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

#include "sdl_rpc_plugin/commands/mobile/delete_sub_menu_request.h"

#include "application_manager/application.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"
#include "utils/helpers.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

DeleteSubMenuRequest::DeleteSubMenuRequest(
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
    , requests_list_()
    , pending_request_corr_id_(0) {}

DeleteSubMenuRequest::~DeleteSubMenuRequest() {}

void DeleteSubMenuRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    SDL_LOG_ERROR("Application is not registered");
    return;
  }

  const int32_t menu_id =
      (*message_)[strings::msg_params][strings::menu_id].asInt();

  const auto sub_menu = app->FindSubMenu(menu_id);

  if (smart_objects::SmartType_Null == sub_menu.getType()) {
    SDL_LOG_ERROR("Menu with id " << menu_id << " is not found.");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  {
    const DataAccessor<SubMenuMap> accessor = app->sub_menu_map();
    const SubMenuMap& sub_menus = accessor.GetData();
    DeleteNestedSubMenus(app, menu_id, sub_menus);
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::menu_id] =
      (*message_)[strings::msg_params][strings::menu_id];
  msg_params[strings::app_id] = app->app_id();
  requests_list_.push_back(msg_params);

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendNextRequest();
}

void DeleteSubMenuRequest::SendNextRequest() {
  SDL_LOG_AUTO_TRACE();

  const auto request_params = requests_list_.front();
  auto function_id = hmi_apis::FunctionID::UI_DeleteSubMenu;

  if (request_params.keyExists(strings::cmd_id)) {
    function_id = request_params.keyExists(strings::grammar_id)
                      ? hmi_apis::FunctionID::VR_DeleteCommand
                      : hmi_apis::FunctionID::UI_DeleteCommand;
  }

  pending_request_corr_id_ = SendHMIRequest(function_id, &request_params, true);
  SDL_LOG_DEBUG(
      "Waiting for response for corr_id = " << pending_request_corr_id_);
}

void DeleteSubMenuRequest::DeleteNestedSubMenus(ApplicationSharedPtr const app,
                                                const uint32_t parentID,
                                                const SubMenuMap& subMenus) {
  SDL_LOG_AUTO_TRACE();

  SubMenuMap::const_iterator it = subMenus.begin();
  SDL_LOG_DEBUG("Delete Submenus with Parent ID: " << parentID);
  while (subMenus.end() != it) {
    if (!(*it->second).keyExists(strings::parent_id)) {
      SDL_LOG_ERROR("parent ID does not exist");
      ++it;
      continue;
    }

    if (parentID == (*it->second)[strings::parent_id].asUInt()) {
      const uint32_t menuID = (*it->second)[strings::menu_id].asUInt();
      DeleteNestedSubMenus(app, menuID, subMenus);

      smart_objects::SmartObject msg_params =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
      msg_params[strings::menu_id] = menuID;
      msg_params[strings::app_id] = app->app_id();
      requests_list_.push_back(msg_params);
    }

    ++it;
  }

  SDL_LOG_DEBUG("Delete commands with Parent ID: " << parentID);
  DeleteSubMenuVRCommands(app, parentID);
  DeleteSubMenuUICommands(app, parentID);
}

void DeleteSubMenuRequest::DeleteSubMenuVRCommands(
    ApplicationConstSharedPtr app, const uint32_t parentID) {
  SDL_LOG_AUTO_TRACE();

  const DataAccessor<CommandsMap> accessor = app->commands_map();
  const CommandsMap& commands = accessor.GetData();
  CommandsMap::const_iterator it = commands.begin();

  for (; commands.end() != it; ++it) {
    if (!(*it->second).keyExists(strings::vr_commands)) {
      continue;
    }

    if (parentID ==
        (*it->second)[strings::menu_params][hmi_request::parent_id].asInt()) {
      smart_objects::SmartObject msg_params =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
      msg_params[strings::cmd_id] = (*it->second)[strings::cmd_id].asInt();
      msg_params[strings::app_id] = app->app_id();
      msg_params[strings::grammar_id] = app->get_grammar_id();
      msg_params[strings::type] = hmi_apis::Common_VRCommandType::Command;

      requests_list_.push_back(msg_params);
    }
  }
}

void DeleteSubMenuRequest::DeleteSubMenuUICommands(
    ApplicationSharedPtr const app, uint32_t parentID) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Delete UI Commands with Parent ID: " << parentID);
  const DataAccessor<CommandsMap> accessor(app->commands_map());
  const CommandsMap& commands = accessor.GetData();
  CommandsMap::const_iterator it = commands.begin();

  while (commands.end() != it) {
    if (!(*it->second).keyExists(strings::menu_params)) {
      SDL_LOG_ERROR("menu_params not exist");
      ++it;
      continue;
    }

    if (parentID ==
        (*it->second)[strings::menu_params][hmi_request::parent_id].asUInt()) {
      smart_objects::SmartObject msg_params =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
      const uint32_t cmd_id = (*it->second)[strings::cmd_id].asUInt();
      msg_params[strings::app_id] = app->app_id();
      msg_params[strings::cmd_id] = cmd_id;

      requests_list_.push_back(msg_params);
    }

    ++it;
  }
}

void DeleteSubMenuRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_DeleteCommand: {
      SDL_LOG_DEBUG("Received UI_DeleteCommand response");

      const auto corr_id =
          message[strings::params][strings::correlation_id].asUInt();
      if (pending_request_corr_id_ == corr_id) {
        auto msg_params = requests_list_.front();

        const auto result_code = static_cast<hmi_apis::Common_Result::eType>(
            message[strings::params][hmi_response::code].asInt());

        auto app = application_manager_.application(connection_key());
        if (!app) {
          SDL_LOG_ERROR("Application not found");
          return;
        }

        if (application_manager::commands::IsHMIResultSuccess(result_code)) {
          const auto cmd_id = msg_params[strings::cmd_id].asUInt();
          SDL_LOG_DEBUG("Removing UI Command: " << cmd_id);
          app->RemoveCommand(cmd_id);
          app->help_prompt_manager().OnVrCommandDeleted(cmd_id, false);
        }

        requests_list_.pop_front();
      }

      break;
    }

    case hmi_apis::FunctionID::VR_DeleteCommand: {
      SDL_LOG_DEBUG("Received VR_DeleteCommand response");

      const auto corr_id =
          message[strings::params][strings::correlation_id].asUInt();

      if (corr_id == pending_request_corr_id_) {
        auto app = application_manager_.application(connection_key());
        if (!app) {
          SDL_LOG_ERROR("Application not found");
          return;
        }

        requests_list_.pop_front();
      }

      break;
    }

    case hmi_apis::FunctionID::UI_DeleteSubMenu: {
      SDL_LOG_DEBUG("Received UI_DeleteSubMenu response");

      const auto corr_id =
          message[strings::params][strings::correlation_id].asUInt();
      if (corr_id == pending_request_corr_id_) {
        auto msg_params = requests_list_.front();

        const auto result_code = static_cast<hmi_apis::Common_Result::eType>(
            message[strings::params][hmi_response::code].asInt());

        auto app = application_manager_.application(connection_key());
        if (!app) {
          SDL_LOG_ERROR("Application not found");
          return;
        }

        if (application_manager::commands::IsHMIResultSuccess(result_code)) {
          const auto menu_id = msg_params[strings::menu_id].asUInt();
          SDL_LOG_DEBUG("Removing submenuID: " << menu_id);
          app->RemoveSubMenu(menu_id);
        }

        requests_list_.pop_front();
      }

      break;
    }

    default: {
      SDL_LOG_ERROR("Received unknown event" << event.id());
      return;
    }
  }

  if (!requests_list_.empty()) {
    SDL_LOG_DEBUG("Still waiting for another requests");
    SendNextRequest();
    return;
  }

  EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);

  hmi_apis::Common_Result::eType result_code =
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
  std::string response_info;
  GetInfo(message, response_info);
  const bool result = PrepareResultForMobileResponse(
      result_code, HmiInterfaces::HMI_INTERFACE_UI);

  SendResponse(result,
               MessageHelper::HMIToMobileResult(result_code),
               response_info.empty() ? NULL : response_info.c_str(),
               &(message[strings::msg_params]));
}

bool DeleteSubMenuRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
