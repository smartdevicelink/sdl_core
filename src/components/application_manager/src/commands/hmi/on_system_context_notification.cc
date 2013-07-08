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

#include "application_manager/commands/hmi/on_system_context_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"

namespace application_manager {

namespace commands {

OnSystemContextNotification::OnSystemContextNotification(
  const MessageSharedPtr& message): NotificationFromHMI(message) {
}

OnSystemContextNotification::~OnSystemContextNotification() {
}

void OnSystemContextNotification::Run() {
  LOG4CXX_INFO(logger_, "OnSystemContextNotification::Run");
  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->active_application());

  if (NULL == app) {
    LOG4CXX_ERROR_EXT(logger_, "NULL pointer");
    return;
  }

  mobile_api::SystemContext::eType system_context =
    static_cast<mobile_api::SystemContext::eType>((*message_)
    [strings::msg_params][hmi_notification::system_context].asInt());

  if ((mobile_api::SystemContext::SYSCTXT_VRSESSION != system_context) &&
      (true == ApplicationManagerImpl::instance()->vr_session_started()) &&
      app->is_media_application()) {
       app->set_audio_streaming_state(mobile_api::AudioStreamingState::AUDIBLE);
       ApplicationManagerImpl::instance()->set_vr_session_started(false);
  } else if ((mobile_api::SystemContext::SYSCTXT_VRSESSION == system_context) &&
         (false == ApplicationManagerImpl::instance()->vr_session_started()) &&
      app->is_media_application()) {
      if (true == ApplicationManagerImpl::instance()->attenuated_supported()) {
        app->set_audio_streaming_state(
            mobile_api::AudioStreamingState::ATTENUATED);
      } else {
        app->set_audio_streaming_state(
            mobile_api::AudioStreamingState::NOT_AUDIBLE);
      }
      ApplicationManagerImpl::instance()->set_vr_session_started(true);
  }

  if (mobile_api::SystemContext::SYSCTXT_ALERT == system_context) {
    // TODO(DK): Find initiator of alert.
    system_context = mobile_api::SystemContext::SYSCTXT_HMI_OBSCURED;
  }

  if (system_context != app->system_context()) {
    app->set_system_context(system_context);

    // If main we need to activate application and send HMI level notification
    if (mobile_api::SystemContext::SYSCTXT_MAIN == system_context
        && mobile_api::HMILevel::HMI_FULL != app->hmi_level()) {
      ApplicationManagerImpl::instance()->ActivateApplication(app);
    } else {
      NotifyMobileApp(app);
    }
  }
}

void OnSystemContextNotification::NotifyMobileApp(ApplicationImpl* const app) {
  MessageHelper::SendHMIStatusNotification(*app);
}

}  // namespace commands

}  // namespace application_manager

