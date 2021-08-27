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

#include "sdl_rpc_plugin/commands/hmi/subscribe_button_request.h"
#include "application_manager/message_helper.h"
#include "application_manager/resumption/resume_ctrl.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

namespace hmi {

SubscribeButtonRequest::SubscribeButtonRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : RequestToHMI(message,
                   application_manager,
                   rpc_service,
                   hmi_capabilities,
                   policy_handle) {
  hmi_apis::Common_ButtonName::eType button_name =
      static_cast<hmi_apis::Common_ButtonName::eType>(
          (*message_)[app_mngr::strings::msg_params]
                     [app_mngr::strings::button_name]
                         .asInt());

  button_name_ = button_name;
}

SubscribeButtonRequest::~SubscribeButtonRequest() {}

app_mngr::ApplicationSharedPtr SubscribeButtonRequest::GetApplicationPtr() {
  const uint32_t app_id =
      (*message_)[strings::msg_params][strings::app_id].asUInt();

  return application_manager_.application_by_hmi_app(app_id);
}

void SubscribeButtonRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  // Specific case when app subscribes to CUSTOM_BUTTON upon registration and no
  // explicit mobile request exist when response arrives. In this case event
  // should be catched by HMI request itself.
  if (hmi_apis::Common_ButtonName::CUSTOM_BUTTON == button_name_) {
    ApplicationSharedPtr app = GetApplicationPtr();
    if (app) {
      // Application should also be subscribed to CUSTOM_BUTTON even before
      // response is received
      app->SubscribeToButton(mobile_apis::ButtonName::CUSTOM_BUTTON);
    }

    subscribe_on_event(hmi_apis::FunctionID::Buttons_SubscribeButton,
                       correlation_id());
  }

  SendRequest();
}

void SubscribeButtonRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  using namespace helpers;

  if (hmi_apis::FunctionID::Buttons_SubscribeButton != event.id()) {
    SDL_LOG_ERROR("Unexpected event id received: " << event.id());
    return;
  }

  unsubscribe_from_event(hmi_apis::FunctionID::Buttons_SubscribeButton);

  const smart_objects::SmartObject& message = event.smart_object();
  ApplicationSharedPtr app = GetApplicationPtr();

  if (!app) {
    SDL_LOG_ERROR("NULL pointer.");
    return;
  }

  const auto hmi_result = static_cast<hmi_apis::Common_Result::eType>(
      message[strings::params][hmi_response::code].asInt());

  if (CommandImpl::IsHMIResultSuccess(hmi_result,
                                      HmiInterfaces::HMI_INTERFACE_Buttons)) {
    SDL_LOG_DEBUG("Subscription to "
                  << button_name_ << " was successful. Subscribing internally");
    app->SubscribeToButton(
        static_cast<mobile_apis::ButtonName::eType>(button_name_));
  } else if (hmi_apis::Common_ButtonName::CUSTOM_BUTTON == button_name_) {
    // SDL should revert subscription in a specific case related to custom
    // button because it was subscribed when the request is sent to HMI
    SDL_LOG_ERROR(
        "Subscription to custom button was failed. Revert the internal "
        "subscription");
    app->UnsubscribeFromButton(
        static_cast<mobile_apis::ButtonName::eType>(button_name_));
  }
}

void SubscribeButtonRequest::OnTimeOut() {
  SDL_LOG_AUTO_TRACE();

  application_manager_.AddExpiredButtonRequest(
      application_id(), correlation_id(), button_name_);

  if (hmi_apis::Common_ButtonName::CUSTOM_BUTTON == button_name_) {
    ApplicationSharedPtr app = GetApplicationPtr();

    if (app) {
      // SDL should revert subscription in a specific case related to custom
      // button because it was subscribed when the request is sent to HMI
      SDL_LOG_ERROR(
          "Subscription to custom button was timed out. Revert the internal "
          "subscription");
      app->UnsubscribeFromButton(
          static_cast<mobile_apis::ButtonName::eType>(button_name_));
    }
  }

  auto& resume_ctrl = application_manager_.resume_controller();
  resume_ctrl.HandleOnTimeOut(
      correlation_id(),
      static_cast<hmi_apis::FunctionID::eType>(function_id()));
}

}  // namespace hmi
}  // namespace commands
}  // namespace sdl_rpc_plugin
