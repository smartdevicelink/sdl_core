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

#include "sdl_rpc_plugin/commands/hmi/unsubscribe_button_request.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/event_engine/event_dispatcher_impl.h"
#include "application_manager/message_helper.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

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
                   policy_handle)
    , EventObserver(application_manager.event_dispatcher()) {
  hmi_apis::Common_ButtonName::eType button_name = static_cast<
      hmi_apis::Common_ButtonName::eType>(
      (*message_)[app_mngr::strings::msg_params][app_mngr::strings::button_name]
          .asInt());

  button_name_ = button_name;
}

UnsubscribeButtonRequest::~UnsubscribeButtonRequest() {
  unsubscribe_from_event(hmi_apis::FunctionID::Buttons_UnsubscribeButton);
}

void UnsubscribeButtonRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  subscribe_on_event(hmi_apis::FunctionID::Buttons_UnsubscribeButton,
                     correlation_id());

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());

  if (!app) {
    LOG4CXX_ERROR(logger_,
                  "Application for connection key: " << application_id()
                                                     << " was not found");
    return;
  }

  app->AddPendingButtonUnsubscription(correlation_id(), button_name_);

  SendRequest();
}

void UnsubscribeButtonRequest::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager_.updateRequestTimeout(
      connection_key(), correlation_id(), 0);
  auto& resume_ctrl = application_manager_.resume_controller();

  resume_ctrl.HandleOnTimeOut(
      correlation_id(),
      static_cast<hmi_apis::FunctionID::eType>(function_id()));

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());

  if (!app) {
    LOG4CXX_ERROR(logger_,
                  "Application for connection key: " << application_id()
                                                     << " was not found");
    return;
  }

  app->RemovePendingButtonUnsubscription(correlation_id());
}

void UnsubscribeButtonRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());

  const smart_objects::SmartObject& message = event.smart_object();

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer.");
    return;
  }

  const auto pending_button_unsubscriptions =
      app->PendingButtonUnsubscriptions();

  const auto it =
      pending_button_unsubscriptions.find(event.smart_object_correlation_id());

  const bool is_in_pending = it != pending_button_unsubscriptions.end();

  hmi_apis::Common_Result::eType hmi_result =
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());

  if (hmi_apis::Common_Result::SUCCESS == hmi_result && is_in_pending) {
    const mobile_apis::ButtonName::eType btn_id =
        static_cast<mobile_apis::ButtonName::eType>(
            (*message_)[strings::msg_params][strings::button_name].asInt());
    app->UnsubscribeFromButton(
        static_cast<mobile_apis::ButtonName::eType>(btn_id));
    app->RemovePendingButtonUnsubscription(correlation_id());
  } else if (!is_in_pending) {
    smart_objects::SmartObjectSPtr msg =
        MessageHelper::CreateButtonSubscriptionHandlingRequestToHmi(
            application_id(),
            static_cast<hmi_apis::Common_ButtonName::eType>(button_name_),
            hmi_apis::FunctionID::Buttons_SubscribeButton,
            application_manager_);

    rpc_service_.SendMessageToHMI(msg);
  }
}

}  // namespace hmi
}  // namespace commands
}  // namespace sdl_rpc_plugin
