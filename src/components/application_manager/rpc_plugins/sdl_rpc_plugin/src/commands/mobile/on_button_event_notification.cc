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

#include "sdl_rpc_plugin/commands/mobile/on_button_event_notification.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

namespace mobile {

OnButtonEventNotification::OnButtonEventNotification(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : ButtonNotificationToMobile(message,
                                 application_manager,
                                 rpc_service,
                                 hmi_capabilities,
                                 policy_handler) {}

OnButtonEventNotification::~OnButtonEventNotification() {}

void OnButtonEventNotification::SendButtonNotification(
    ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);

  mobile_apis::ButtonEventMode::eType btn_event_mode;

  if ((*message_)[strings::msg_params].keyExists(hmi_response::button_mode)) {
    btn_event_mode = static_cast<mobile_apis::ButtonEventMode::eType>(
        (*message_)[strings::msg_params][hmi_response::button_mode].asInt());

  } else if ((*message_)[strings::msg_params].keyExists(
                 strings::button_event_mode)) {
    btn_event_mode = static_cast<mobile_apis::ButtonEventMode::eType>(
        (*message_)[strings::msg_params][strings::button_event_mode].asInt());
  }

  message_ = MessageHelper::CreateButtonNotificationToMobile(
      application_manager_, app, *message_);

  (*message_)[strings::msg_params][strings::button_event_mode] = btn_event_mode;

  (*message_)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::eType::OnButtonEventID;

  SendNotification();
}

}  // namespace mobile

}  // namespace commands

}  // namespace sdl_rpc_plugin
