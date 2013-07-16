/*

 Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/mobile/on_button_event_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "interfaces/MOBILE_API.h"

namespace application_manager {

namespace commands {

OnButtonEventNotification::OnButtonEventNotification(
  const MessageSharedPtr& message): CommandNotificationImpl(message) {
}

OnButtonEventNotification::~OnButtonEventNotification() {
}

void OnButtonEventNotification::Run() {
  LOG4CXX_INFO(logger_, "OnButtonEventNotification::Run");

  if ((*message_)[strings::msg_params].keyExists(
        hmi_response::custom_button_id)) {
    LOG4CXX_INFO_EXT(logger_, "No subscription for custom buttons requires");

    ApplicationImpl* app = static_cast<ApplicationImpl*>(
        ApplicationManagerImpl::instance()->active_application());

    if (NULL == app) {
      LOG4CXX_WARN_EXT(logger_, "OnButtonEvent came but no app is active.");
      return;
    }
    SendButtonEvent(app, true);
    return;
  }

  const unsigned int btn_id = static_cast<unsigned int>(
                                (*message_)[strings::msg_params]
                                [hmi_response::button_name].asInt());

  const std::vector<Application*>& subscribedApps =
    ApplicationManagerImpl::instance()->applications_by_button(btn_id);

  std::vector<Application*>::const_iterator it = subscribedApps.begin();
  for (; subscribedApps.end() != it; ++it) {
    ApplicationImpl* subscribed_app = static_cast<ApplicationImpl*>(*it);
    if (!subscribed_app) {
      LOG4CXX_WARN_EXT(logger_, "Null pointer to subscribed app.");
      continue;
    }

    if ((mobile_api::HMILevel::HMI_FULL == subscribed_app->hmi_level()) ||
        (mobile_api::HMILevel::HMI_LIMITED == subscribed_app->hmi_level()
         && mobile_apis::ButtonName::OK != btn_id)) {
      SendButtonEvent(subscribed_app, false);
    } else {
      LOG4CXX_WARN_EXT(logger_, "OnButtonEvent in HMI_BACKGROUND or NONE");
      continue;
    }
  }
}

void OnButtonEventNotification::SendButtonEvent(const ApplicationImpl* app,
    bool is_custom_btn_id) {
  smart_objects::SmartObject* on_btn_event =
    new smart_objects::SmartObject();

  if (!on_btn_event) {
    LOG4CXX_ERROR_EXT(logger_, "OnButtonEvent NULL pointer");
    return;
  }

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "OnButtonEvent NULL pointer");
    return;
  }

  const int correlation_id =
    (*message_)[strings::params][strings::correlation_id];
  const int connection_key =
    (*message_)[strings::params][strings::connection_key];

  (*on_btn_event)[strings::params][strings::message_type] =
    MessageType::kNotification;
  (*on_btn_event)[strings::params][strings::correlation_id] =
    correlation_id;

  (*on_btn_event)[strings::params][strings::app_id] =
    app->app_id();

  (*on_btn_event)[strings::params][strings::connection_key] =
    connection_key;
  (*on_btn_event)[strings::params][strings::function_id] =
    mobile_apis::FunctionID::eType::OnButtonEventID;
  (*on_btn_event)[strings::msg_params][strings::button_name] =
    (*message_)[strings::msg_params][hmi_response::button_name];
  (*on_btn_event)[strings::msg_params][strings::button_event_mode] =
    (*message_)[strings::msg_params][hmi_response::button_mode];

  if (is_custom_btn_id) {
    (*on_btn_event)[strings::msg_params][strings::custom_button_id] =
      (*message_)[strings::msg_params][strings::custom_button_id];
  } else {
    (*on_btn_event)[strings::msg_params][strings::custom_button_id] = 0;
  }

  (*on_btn_event)[strings::msg_params][strings::success] = true;
  (*on_btn_event)[strings::msg_params][strings::result_code] =
    mobile_apis::Result::SUCCESS;

  message_.reset(on_btn_event);
  SendNotification();
}

}  // namespace commands

}  // namespace application_manager
