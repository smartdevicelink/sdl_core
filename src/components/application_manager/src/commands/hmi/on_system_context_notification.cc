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
    const MessageSharedPtr& message)
    : NotificationFromHMI(message) {
}

OnSystemContextNotification::~OnSystemContextNotification() {
}

void OnSystemContextNotification::Run() {
  LOG4CXX_INFO(logger_, "OnSystemContextNotification::Run");

  ApplicationManagerImpl::ApplicationListAccessor accessor;
  const std::set<ApplicationSharedPtr>& app_list = accessor.applications();
  std::set<ApplicationSharedPtr>::const_iterator it = app_list.begin();
  uint32_t app_id = 0;
  mobile_api::SystemContext::eType system_context =
      static_cast<mobile_api::SystemContext::eType>(
          (*message_)[strings::msg_params][hmi_notification::system_context].asInt());

  if (((*message_)[strings::msg_params].keyExists(strings::app_id)) &&
      (mobile_api::SystemContext::SYSCTXT_ALERT == system_context)) {
    app_id = (*message_)[strings::msg_params][strings::app_id].asUInt();
  }

  // SDLAQ-CRS-833 implementation
  for (; app_list.end() != it; ++it) {
    // send notification for background app
    if ((app_id != 0) && ((*it)->app_id() == app_id) &&
        (mobile_api::HMILevel::HMI_BACKGROUND == (*it)->hmi_level())) {
      if (system_context != (*it)->system_context()) {
        (*it)->set_alert_in_background(true);
        SendSystemContextNotification((*it), system_context);
      }
      // change system context for full app
      system_context = mobile_api::SystemContext::SYSCTXT_HMI_OBSCURED;
    } else if ((mobile_api::HMILevel::HMI_BACKGROUND == (*it)->hmi_level()) &&
        (true == (*it)->alert_in_background())) {
      (*it)->set_alert_in_background(false);
      SendSystemContextNotification((*it), system_context);
    } else if (((mobile_api::HMILevel::HMI_FULL == (*it)->hmi_level()) ||
        (mobile_api::HMILevel::HMI_LIMITED == (*it)->hmi_level())) &&
        (system_context != (*it)->system_context())) {
      // If context actually changed
      SendSystemContextNotification((*it), system_context);
    }
  }
}

void OnSystemContextNotification::SendSystemContextNotification(ApplicationSharedPtr app,
    mobile_api::SystemContext::eType system_context) {
  app->set_system_context(system_context);
  MessageHelper::SendHMIStatusNotification(*app);
}

}  // namespace commands

}  // namespace application_manager

