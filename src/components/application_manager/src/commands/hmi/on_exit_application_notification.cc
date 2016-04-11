/*
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
  LOG4CXX_AUTO_TRACE(logger_);

  using namespace mobile_apis;
  using namespace hmi_apis;

  ApplicationManagerImpl* app_mgr = ApplicationManagerImpl::instance();
  uint32_t app_id = (*message_)[strings::msg_params][strings::app_id].asUInt();
  ApplicationSharedPtr app_impl = app_mgr->application(app_id);
      
  if (!(app_impl.valid())) {
    LOG4CXX_ERROR(logger_, "Application does not exist");
    return;
  }

  Common_ApplicationExitReason::eType reason;
  reason = static_cast<Common_ApplicationExitReason::eType>
      ((*message_)[strings::msg_params][strings::reason].asInt());

  switch (reason) {
    case Common_ApplicationExitReason::DRIVER_DISTRACTION_VIOLATION: {
      MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
          app_impl->app_id(),
              AppInterfaceUnregisteredReason::DRIVER_DISTRACTION_VIOLATION);
      break;
    }
    case Common_ApplicationExitReason::USER_EXIT: {
      break;
    }
    case Common_ApplicationExitReason::UNAUTHORIZED_TRANSPORT_REGISTRATION: {
      MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
          app_id, AppInterfaceUnregisteredReason::APP_UNAUTHORIZED);
      app_mgr->UnregisterApplication(app_id, Result::SUCCESS);
      return;
    }
    case Common_ApplicationExitReason::UNSUPPORTED_HMI_RESOURCE: {
      MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
          app_id, AppInterfaceUnregisteredReason::UNSUPPORTED_HMI_RESOURCE);
      app_mgr->UnregisterApplication(app_id, Result::SUCCESS);
      return;
    }
    default: {
      LOG4CXX_WARN(logger_, "Unhandled reason");
      return;
    }
  }

  ApplicationManagerImpl::instance()->SetState<false>(
      app_id, HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE);
}

}  // namespace commands

}  // namespace application_manager
