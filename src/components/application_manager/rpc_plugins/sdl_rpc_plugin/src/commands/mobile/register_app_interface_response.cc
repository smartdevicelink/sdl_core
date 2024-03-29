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

#include "sdl_rpc_plugin/commands/mobile/register_app_interface_response.h"
#include "application_manager/application_manager.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "connection_handler/connection_handler.h"
#include "interfaces/MOBILE_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

void RegisterAppInterfaceResponse::Run() {
  SDL_LOG_AUTO_TRACE();

  mobile_apis::Result::eType result_code = mobile_apis::Result::SUCCESS;
  bool success = (*message_)[strings::msg_params][strings::success].asBool();
  bool last_message = !success;
  // Do not close connection in case of APPLICATION_NOT_REGISTERED despite it is
  // an error
  if (!success &&
      (*message_)[strings::msg_params].keyExists(strings::result_code)) {
    result_code = static_cast<mobile_apis::Result::eType>(
        (*message_)[strings::msg_params][strings::result_code].asInt());
    if (result_code == mobile_apis::Result::APPLICATION_REGISTERED_ALREADY) {
      last_message = false;
    }
  }

  application_manager::ApplicationSharedPtr app =
      application_manager_.application(connection_key());

  if (app && app->msg_version() < utils::rpc_version_5 &&
      app->is_media_application() &&
      (*message_)[strings::msg_params].keyExists(
          hmi_response::button_capabilities)) {
    const smart_objects::SmartObject& button_caps =
        (*message_)[strings::msg_params][hmi_response::button_capabilities];
    auto it = button_caps.asArray()->begin();
    auto ok_btn_it = it;
    bool ok_btn_exists = false;
    bool play_pause_btn_exists = false;
    for (; it != button_caps.asArray()->end(); ++it) {
      smart_objects::SmartObject& so = *it;
      int64_t current_id = so[strings::name].asInt();
      if (current_id == -1) {
        continue;
      }
      const mobile_apis::ButtonName::eType current_button =
          static_cast<mobile_apis::ButtonName::eType>(current_id);
      if (current_button == mobile_apis::ButtonName::PLAY_PAUSE) {
        play_pause_btn_exists = true;
        so[strings::name] = mobile_apis::ButtonName::OK;
      } else if (current_button == mobile_apis::ButtonName::OK) {
        ok_btn_exists = true;
        ok_btn_it = it;
      }
    }
    if (ok_btn_exists && play_pause_btn_exists) {
      button_caps.asArray()->erase(ok_btn_it);
    }
  }

  SendResponse(success, result_code, last_message);
  if (app && success) {
    app->set_is_ready(true);
  }
  event_engine::MobileEvent event(
      mobile_apis::FunctionID::RegisterAppInterfaceID);
  smart_objects::SmartObject event_msg(*message_);
  event_msg[strings::params][strings::correlation_id] = 0;
  event.set_smart_object(event_msg);
  event.raise(application_manager_.event_dispatcher());

  if (mobile_apis::Result::SUCCESS != result_code) {
    return;
  }

  // Add registered application to the policy db right after response sent to
  // mobile to be able to check all other API according to app permissions
  if (!app) {
    SDL_LOG_ERROR("Application with connection key " << connection_key()
                                                     << " is not registered.");
    return;
  }

  SetHeartBeatTimeout(connection_key(), app->policy_app_id());
}

void RegisterAppInterfaceResponse::SetHeartBeatTimeout(
    uint32_t connection_key, const std::string& mobile_app_id) {
  SDL_LOG_AUTO_TRACE();
  policy::PolicyHandlerInterface& policy_handler = policy_handler_;
  if (policy_handler.PolicyEnabled()) {
    const uint32_t timeout = policy_handler.HeartBeatTimeout(mobile_app_id);
    if (timeout > 0) {
      application_manager_.connection_handler().SetHeartBeatTimeout(
          connection_key, timeout);
    }
  } else {
    SDL_LOG_INFO("Policy is turn off");
  }
}

}  // namespace commands
}  // namespace sdl_rpc_plugin
