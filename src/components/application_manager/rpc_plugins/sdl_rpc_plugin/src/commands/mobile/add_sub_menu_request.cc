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

#include "sdl_rpc_plugin/commands/mobile/add_sub_menu_request.h"

#include "application_manager/application.h"
#include "application_manager/message_helper.h"
#include "utils/helpers.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

AddSubMenuRequest::AddSubMenuRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

AddSubMenuRequest::~AddSubMenuRequest() {}

void AddSubMenuRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  smart_objects::SmartObject received_msg_params =
      (*message_)[strings::msg_params];
  mobile_apis::Result::eType verification_result =
      mobile_apis::Result::INVALID_ENUM;

  if (received_msg_params.keyExists(strings::menu_icon)) {
    verification_result = MessageHelper::VerifyImage(
        received_msg_params[strings::menu_icon], app, application_manager_);

    if (mobile_apis::Result::INVALID_DATA == verification_result) {
      SDL_LOG_ERROR("MessageHelper::VerifyImage return "
                    << verification_result);
      SendResponse(false, verification_result);
      return;
    }
  }

  const int32_t menu_id = received_msg_params[strings::menu_id].asInt();

  const auto sub_menu = app->FindSubMenu(menu_id);

  if (smart_objects::SmartType_Null != sub_menu.getType()) {
    SDL_LOG_ERROR("Menu with id " << menu_id << " already exists.");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  const uint32_t parent_id =
      received_msg_params.keyExists(strings::parent_id)
          ? received_msg_params[strings::parent_id].asUInt()
          : 0;

  if (0 != parent_id) {
    smart_objects::SmartObject parent = app->FindSubMenu(parent_id);
    if (smart_objects::SmartType_Null == parent.getType()) {
      SDL_LOG_ERROR("Parent ID " << parent_id << " doesn't exist");
      SendResponse(
          false, mobile_apis::Result::INVALID_ID, "Parent ID doesn't exist");
      return;
    }
  }

  const std::string& menu_name =
      received_msg_params[strings::menu_name].asString();

  if (!CheckSubMenuName()) {
    SDL_LOG_ERROR("Sub-menu name is not valid.");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  if (received_msg_params.keyExists(strings::menu_layout)) {
    auto menu_layout = static_cast<mobile_apis::MenuLayout::eType>(
        received_msg_params[strings::menu_layout].asUInt());
    if (app->menu_layout_supported(menu_layout)) {
      msg_params[strings::menu_layout] =
          received_msg_params[strings::menu_layout];
    } else {
      is_menu_layout_available_ = false;
    }
  }

  msg_params[strings::menu_id] = received_msg_params[strings::menu_id];
  if (received_msg_params.keyExists(strings::position)) {
    msg_params[strings::menu_params][strings::position] =
        received_msg_params[strings::position];
  }
  if (received_msg_params.keyExists(strings::menu_icon)) {
    msg_params[strings::menu_icon] = received_msg_params[strings::menu_icon];
  }
  msg_params[strings::menu_params][strings::menu_name] =
      received_msg_params[strings::menu_name];
  if (received_msg_params.keyExists(strings::parent_id)) {
    msg_params[strings::menu_params][strings::parent_id] = parent_id;
  }

  msg_params[strings::app_id] = app->app_id();

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(hmi_apis::FunctionID::UI_AddSubMenu, &msg_params, true);
}

void AddSubMenuRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_AddSubMenu: {
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      std::string response_info;
      GetInfo(message, response_info);
      const bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_UI);

      ApplicationSharedPtr application =
          application_manager_.application(connection_key());

      if (!application) {
        SDL_LOG_ERROR("NULL pointer");
        return;
      }

      if (result) {
        application->AddSubMenu(
            (*message_)[strings::msg_params][strings::menu_id].asInt(),
            (*message_)[strings::msg_params]);
        response_info =
            "The MenuLayout specified is unsupported, the "
            "default MenuLayout will be used." +
            response_info;
        SendResponse(result,
                     is_menu_layout_available_
                         ? MessageHelper::HMIToMobileResult(result_code)
                         : mobile_apis::Result::WARNINGS,
                     is_menu_layout_available_ ? NULL : response_info.c_str(),
                     &(message[strings::msg_params]));
      } else {
        SendResponse(result,
                     MessageHelper::HMIToMobileResult(result_code),
                     response_info.empty() ? NULL : response_info.c_str(),
                     &(message[strings::msg_params]));
      }
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event" << event.id());
      return;
    }
  }
}

bool AddSubMenuRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

bool AddSubMenuRequest::CheckSubMenuName() {
  SDL_LOG_AUTO_TRACE();

  const char* str =
      (*message_)[strings::msg_params][strings::menu_name].asCharArray();
  if (!CheckSyntax(str)) {
    SDL_LOG_INFO("Invalid subMenu name.");
    return false;
  }
  return true;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
