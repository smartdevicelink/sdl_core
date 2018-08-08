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

#include "sdl_rpc_plugin/commands/hmi/on_system_context_notification.h"

#include "application_manager/application_impl.h"
#include "application_manager/state_controller.h"
#include "application_manager/message_helper.h"

namespace sdl_rpc_plugin {
using namespace application_manager;
namespace commands {

OnSystemContextNotification::OnSystemContextNotification(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : NotificationFromHMI(message,
                          application_manager,
                          rpc_service,
                          hmi_capabilities,
                          policy_handle) {}

OnSystemContextNotification::~OnSystemContextNotification() {}

void OnSystemContextNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  mobile_api::SystemContext::eType system_context =
      static_cast<mobile_api::SystemContext::eType>(
          (*message_)[strings::msg_params][hmi_notification::system_context]
              .asInt());

  ApplicationSharedPtr app;
  if ((mobile_api::SystemContext::SYSCTXT_VRSESSION == system_context) ||
      (mobile_api::SystemContext::SYSCTXT_MENU == system_context) ||
      (mobile_api::SystemContext::SYSCTXT_HMI_OBSCURED == system_context)) {
    app = application_manager_.active_application();
  } else if ((mobile_api::SystemContext::SYSCTXT_ALERT == system_context) ||
             (mobile_api::SystemContext::SYSCTXT_MAIN == system_context)) {
    if ((*message_)[strings::msg_params].keyExists(strings::app_id)) {
      app = application_manager_.application(
          (*message_)[strings::msg_params][strings::app_id].asUInt());
    }
  }

  if (app && mobile_api::SystemContext::INVALID_ENUM != system_context) {
    application_manager_.state_controller().SetRegularState(app,
                                                            system_context);
  } else {
    LOG4CXX_ERROR(logger_, "Application does not exist");
  }
}

}  // namespace commands

}  // namespace application_manager
