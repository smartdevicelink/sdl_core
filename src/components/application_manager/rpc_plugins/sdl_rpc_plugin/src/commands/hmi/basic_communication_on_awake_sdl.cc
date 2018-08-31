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

#include "sdl_rpc_plugin/commands/hmi/basic_communication_on_awake_sdl.h"
#include "application_manager/message_helper.h"
#include "application_manager/resumption/resume_ctrl.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

OnAwakeSDLNotification::OnAwakeSDLNotification(
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

OnAwakeSDLNotification::~OnAwakeSDLNotification() {}

void OnAwakeSDLNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!application_manager_.resume_controller().is_suspended()) {
    return;
  }

  {
    DataAccessor<ApplicationSet> accessor = application_manager_.applications();
    ApplicationSetIt itBegin = accessor.GetData().begin();
    ApplicationSetIt itEnd = accessor.GetData().end();
    for (; itBegin != itEnd; ++itBegin) {
      const ApplicationSharedPtr app = *itBegin;
      if (app && app->IsHashChangedDuringSuspend()) {
        MessageHelper::SendHashUpdateNotification(app->app_id(),
                                                  application_manager_);
        app->SetHashChangedDuringSuspend(false);
      }
    }
  }

  application_manager_.resume_controller().OnAwake();

  event_engine::Event event(
      hmi_apis::FunctionID::BasicCommunication_OnAwakeSDL);
  event.set_smart_object(*message_);
  event.raise(application_manager_.event_dispatcher());
}

}  // namespace commands

}  // namespace application_manager
