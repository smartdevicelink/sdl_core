/**
 * Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/hmi/on_exit_application_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

OnExitApplicationNotification::OnExitApplicationNotification(
    const MessageSharedPtr& message) : NotificationFromHMI(message) {
}

OnExitApplicationNotification::~OnExitApplicationNotification() {
}

void OnExitApplicationNotification::Run() {
  LOG4CXX_INFO(logger_, "OnExitApplicationNotification::Run");

  ApplicationManagerImpl* app_mgr = ApplicationManagerImpl::instance();
  ApplicationSharedPtr app_impl = app_mgr->application(
      (*message_)[strings::msg_params][strings::app_id].asUInt());
  if (!(app_impl.valid())) {
    LOG4CXX_ERROR(logger_, "Application does not exist");
    return;
  }
  hmi_apis::Common_ApplicationToNONEReason::eType reason;
  reason = static_cast<hmi_apis::Common_ApplicationToNONEReason::eType>
                       ((*message_)[strings::msg_params][strings::reason].asInt());
  switch (reason) {
    case hmi_apis::Common_ApplicationToNONEReason::USER_EXIT : {
      break;
    }
    default : {
      LOG4CXX_WARN(logger_, "Unhandled reason");
      break;
    }
  }
  app_impl->set_hmi_level(mobile_apis::HMILevel::HMI_NONE);
  app_impl->set_audio_streaming_state(mobile_apis::AudioStreamingState::NOT_AUDIBLE);
  app_impl->set_system_context(mobile_api::SystemContext::SYSCTXT_MAIN);
  MessageHelper::SendHMIStatusNotification(*app_impl);
}

}  // namespace commands

}  // namespace application_manager
