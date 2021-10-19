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

#include "sdl_rpc_plugin/commands/hmi/unsubscribe_button_request.h"
#include "application_manager/event_engine/event_dispatcher_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/resumption/resume_ctrl.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {
SDL_CREATE_LOG_VARIABLE("Commands")

namespace hmi {

UnsubscribeButtonRequest::UnsubscribeButtonRequest(
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

UnsubscribeButtonRequest::~UnsubscribeButtonRequest() {}

void UnsubscribeButtonRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  subscribe_on_event(hmi_apis::FunctionID::Buttons_UnsubscribeButton,
                     correlation_id());
  SendRequest();
}

void UnsubscribeButtonRequest::OnTimeOut() {
  SDL_LOG_AUTO_TRACE();

  application_manager_.AddExpiredButtonRequest(
      application_id(), correlation_id(), button_name_);

  auto& resume_ctrl = application_manager_.resume_controller();
  resume_ctrl.HandleOnTimeOut(
      correlation_id(),
      static_cast<hmi_apis::FunctionID::eType>(function_id()));
}

void UnsubscribeButtonRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();

  if (hmi_apis::FunctionID::Buttons_UnsubscribeButton != event.id()) {
    SDL_LOG_ERROR("Unexpected event id received: " << event.id());
    return;
  }

  unsubscribe_from_event(hmi_apis::FunctionID::Buttons_UnsubscribeButton);

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());

  const smart_objects::SmartObject& message = event.smart_object();

  if (!app) {
    SDL_LOG_ERROR("NULL pointer.");
    return;
  }

  hmi_apis::Common_Result::eType hmi_result =
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());

  if (CommandImpl::IsHMIResultSuccess(hmi_result,
                                      HmiInterfaces::HMI_INTERFACE_Buttons)) {
    const mobile_apis::ButtonName::eType btn_id =
        static_cast<mobile_apis::ButtonName::eType>(
            (*message_)[strings::msg_params][strings::button_name].asInt());
    app->UnsubscribeFromButton(
        static_cast<mobile_apis::ButtonName::eType>(btn_id));
  }
}

}  // namespace hmi
}  // namespace commands
}  // namespace sdl_rpc_plugin
