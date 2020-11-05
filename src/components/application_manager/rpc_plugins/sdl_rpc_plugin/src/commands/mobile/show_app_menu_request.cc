/*
 * Copyright (c) 2018, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sdl_rpc_plugin/commands/mobile/show_app_menu_request.h"

#include "application_manager/message_helper.h"
#include "utils/helpers.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
SDL_CREATE_LOG_VARIABLE("Commands")

ShowAppMenuRequest::ShowAppMenuRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

ShowAppMenuRequest::~ShowAppMenuRequest() {}

void ShowAppMenuRequest::Run() {
  using namespace app_mngr;
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("Application with id " << connection_key()
                                         << " is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (mobile_apis::HMILevel::HMI_FULL !=
          app->hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW) ||
      helpers::Compare<mobile_apis::SystemContext::eType,
                       helpers::NEQ,
                       helpers::ALL>(
          app->system_context(mobile_apis::PredefinedWindows::DEFAULT_WINDOW),
          mobile_apis::SystemContext::SYSCTXT_MAIN,
          mobile_apis::SystemContext::SYSCTXT_MENU)) {
    SDL_LOG_ERROR("Application with id " << connection_key()
                                         << " is not activated.");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  auto msg_params = smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();

  const auto& received_msg_params = (*message_)[strings::msg_params];
  if (received_msg_params.keyExists(strings::menu_id)) {
    const int32_t menu_id = received_msg_params[strings::menu_id].asInt();

    const auto sub_menu = app->FindSubMenu(menu_id);

    if (smart_objects::SmartType_Null == sub_menu.getType()) {
      SDL_LOG_ERROR("Menu with id " << menu_id << " is not found.");
      SendResponse(false, mobile_apis::Result::INVALID_ID);
      return;
    }
    msg_params[strings::menu_id] = menu_id;
  }

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(hmi_apis::FunctionID::UI_ShowAppMenu, &msg_params, true);
}

void ShowAppMenuRequest::on_event(const app_mngr::event_engine::Event& event) {
  using namespace app_mngr;
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_ShowAppMenu: {
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      const auto result_code = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      std::string response_info;
      GetInfo(message, response_info);
      const bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_UI);

      ApplicationSharedPtr app =
          application_manager_.application(connection_key());

      if (!app) {
        SDL_LOG_ERROR("Application with id " << connection_key()
                                             << " is not registered.");
        return;
      }

      SendResponse(result,
                   MessageHelper::HMIToMobileResult(result_code),
                   response_info.empty() ? NULL : response_info.c_str(),
                   &message[strings::msg_params]);
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event: " << event.id());
      return;
    }
  }
}

}  // namespace commands
}  // namespace sdl_rpc_plugin
