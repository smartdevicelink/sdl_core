/*
 * Copyright (c) 2021, Ford Motor Company
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

#include "application_manager/commands/button_notification_to_mobile.h"
#include "interfaces/MOBILE_API.h"
namespace application_manager {

namespace commands {
namespace app_mngr = application_manager;

SDL_CREATE_LOG_VARIABLE("ButtonNotificationToMobile")

ButtonNotificationToMobile::ButtonNotificationToMobile(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandNotificationImpl(message,
                              application_manager,
                              rpc_service,
                              hmi_capabilities,
                              policy_handler) {}

ButtonNotificationToMobile::~ButtonNotificationToMobile() {}

bool ButtonNotificationToMobile::DoesParamExist(
    const std::string& param_name) const {
  SDL_LOG_AUTO_TRACE();
  using namespace application_manager::strings;
  return (*message_).keyExists(msg_params) &&
         (*message_)[msg_params].keyExists(param_name);
}

void ButtonNotificationToMobile::HandleCustomButton(
    app_mngr::ApplicationSharedPtr app) {
  SDL_LOG_AUTO_TRACE();
  using namespace application_manager::strings;

  if (!app) {
    SDL_LOG_ERROR("Application doesn't exist.");
    return;
  }

  // app_id is mandatory for CUSTOM_BUTTON notification
  if (!DoesParamExist(strings::app_id)) {
    SDL_LOG_ERROR("CUSTOM_BUTTON mobile notification without app_id.");
    return;
  }

  // custom_button_id is mandatory for CUSTOM_BUTTON notification
  if (!DoesParamExist(hmi_response::custom_button_id)) {
    SDL_LOG_ERROR(
        "CUSTOM_BUTTON mobile notification without custom_button_id.");
    return;
  }

  const uint32_t custom_btn_id =
      (*message_)[msg_params][hmi_response::custom_button_id].asUInt();

  if (!app->IsSubscribedToSoftButton(custom_btn_id)) {
    SDL_LOG_ERROR("Application doesn't subscribed to this custom_button_id.");
    return;
  }

  const auto window_id = app->GetSoftButtonWindowID(custom_btn_id);
  (*message_)[strings::msg_params][strings::window_id] = window_id;

  if (mobile_api::HMILevel::HMI_NONE == app->hmi_level(window_id)) {
    SDL_LOG_WARN("CUSTOM_BUTTON mobile notification is allowed only "
                 << "in FULL, LIMITED or BACKGROUND hmi level");
    return;
  }

  SendButtonNotification(app);
  return;
}

void ButtonNotificationToMobile::HandleOKButton(
    app_mngr::ApplicationSharedPtr app) {
  SDL_LOG_AUTO_TRACE();
  using namespace application_manager;
  SDL_LOG_DEBUG("OK button received");

  const auto subscribed_apps = SubscribedApps();
  if (app) {
    const auto app_ptr =
        std::find_if(subscribed_apps.begin(),
                     subscribed_apps.end(),
                     [&app](const ApplicationSharedPtr subscribed_app) {
                       return app->app_id() == subscribed_app->app_id();
                     });
    if (app_ptr != subscribed_apps.end()) {
      SDL_LOG_DEBUG(
          "Sending button press for this app id: " << (*app_ptr)->app_id());
      SendButtonNotification(*app_ptr);
      return;
    }
  }

  const auto app_ptr =
      std::find_if(subscribed_apps.begin(),
                   subscribed_apps.end(),
                   [](const ApplicationSharedPtr subscribed_app) {
                     return subscribed_app->IsFullscreen();
                   });

  if (app_ptr != subscribed_apps.end()) {
    SDL_LOG_DEBUG("Sending button press for app in FULL with app id: "
                  << (*app_ptr)->app_id());
    SendButtonNotification(*app_ptr);
    return;
  } else {
    SDL_LOG_ERROR("No application found");
  }
}

std::vector<ApplicationSharedPtr> ButtonNotificationToMobile::SubscribedApps()
    const {
  const uint32_t btn_id = static_cast<uint32_t>(
      (*message_)[strings::msg_params][hmi_response::button_name].asInt());
  return application_manager_.applications_by_button(btn_id);
}

void ButtonNotificationToMobile::HandleMediaButton(
    app_mngr::ApplicationSharedPtr app) {
  SDL_LOG_AUTO_TRACE();
  using namespace application_manager;

  const auto subscribed_apps = SubscribedApps();

  if (app) {
    const auto app_ptr =
        std::find_if(subscribed_apps.begin(),
                     subscribed_apps.end(),
                     [&app](const ApplicationSharedPtr subscribed_app) {
                       return app->app_id() == subscribed_app->app_id();
                     });
    if (app_ptr != subscribed_apps.end()) {
      SendButtonNotification(*app_ptr);
      return;
    }
  }

  const auto app_ptr =
      std::find_if(subscribed_apps.begin(),
                   subscribed_apps.end(),
                   [](const ApplicationSharedPtr subscribed_app)

                   {
                     return helpers::Compare<mobile_api::HMILevel::eType,
                                             helpers::EQ,
                                             helpers::ONE>(
                         subscribed_app->hmi_level(
                             mobile_apis::PredefinedWindows::DEFAULT_WINDOW),
                         mobile_api::HMILevel::HMI_FULL,
                         mobile_api::HMILevel::HMI_LIMITED);
                   }

      );

  if (app_ptr != subscribed_apps.end()) {
    SendButtonNotification(*app_ptr);
    return;
  } else {
    SDL_LOG_ERROR("No application found");
  }
}

void ButtonNotificationToMobile::Run() {
  SDL_LOG_AUTO_TRACE();
  using namespace application_manager::strings;

  const uint32_t btn_id = static_cast<uint32_t>(
      (*message_)[msg_params][hmi_response::button_name].asInt());

  SDL_LOG_DEBUG("Received button id: " << btn_id);

  ApplicationSharedPtr app;

  if (DoesParamExist(strings::app_id)) {
    app = application_manager_.application(
        (*message_)[msg_params][app_id].asUInt());
  }

  if (static_cast<uint32_t>(mobile_apis::ButtonName::CUSTOM_BUTTON) == btn_id) {
    HandleCustomButton(app);
  } else if (static_cast<uint32_t>(mobile_apis::ButtonName::OK) == btn_id) {
    HandleOKButton(app);
  } else {
    HandleMediaButton(app);
  }
}

}  // namespace commands
}  // namespace application_manager
