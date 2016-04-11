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

#include "application_manager/application_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/commands/hmi/on_system_request_notification.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "interfaces/MOBILE_API.h"
#include "utils/macro.h"

using policy::PolicyHandler;

namespace application_manager {

namespace commands {

OnSystemRequestNotification::OnSystemRequestNotification(
  const MessageSharedPtr& message)
  : NotificationFromHMI(message) {
}

OnSystemRequestNotification::~OnSystemRequestNotification() {
}

void OnSystemRequestNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject& params = (*message_)[strings::params];
  smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];

  params[strings::function_id] =
    static_cast<int32_t>(mobile_apis::FunctionID::eType::OnSystemRequestID);

  const std::string app_id = msg_params[strings::app_id].asString();
  LOG4CXX_DEBUG(logger_, "Received OnSystemRequest for " << app_id );

  ApplicationSharedPtr app;
  if (strings::default_app_id == app_id) {
    const policy::PolicyHandlerInterface& policy_handler = 
	application_manager::ApplicationManagerImpl::instance()->GetPolicyHandler();
    const uint32_t selected_app_id = policy_handler.GetAppIdForSending();
    if (0 == selected_app_id) {
      LOG4CXX_WARN(logger_,
                   "Can't select application to forward OnSystemRequestNotification");
      return;
    }
    ApplicationManagerImpl* app_mgr = ApplicationManagerImpl::instance();
    app = app_mgr->application(selected_app_id);
  } else {
    app = ApplicationManagerImpl::instance()->application_by_policy_id(app_id);
  }

  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "Application with connection key " << app_id <<
                 "is not registered.");
    return;
  }

  params[strings::connection_key] = app->app_id();
  SendNotificationToMobile(message_);
}

}  // namespace commands

}  // namespace application_manager

