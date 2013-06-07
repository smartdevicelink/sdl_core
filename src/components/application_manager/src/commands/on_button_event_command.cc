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

#include "application_manager/commands/on_button_event_command.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "v4_protocol_v2_0_revT.h"
#include "utils/logger.h"

namespace application_manager {

namespace commands {

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Commands"));

OnButtonEventCommand::OnButtonEventCommand(
    const MessageSharedPtr& message): CommandResponseImpl(message) {
}

OnButtonEventCommand::~OnButtonEventCommand() {
}

void OnButtonEventCommand::Run() {
  LOG4CXX_INFO(logger_, "OnButtonEventCommand::Run ");

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
      (*message_)[strings::msg_params][hmi_response::button_name].asInt());

  std::vector<Application*> subscribedApps =
      ApplicationManagerImpl::instance()->applications_by_button(btn_id);

  std::vector<Application*>::const_iterator it = subscribedApps.begin();
  for (; subscribedApps.end() != it; ++it) {
    ApplicationImpl* subscribed_app = static_cast<ApplicationImpl*>(*it);
    if (!subscribed_app) {
      LOG4CXX_WARN_EXT(logger_, "Null pointer to subscribed app.");
      continue;
    }

    if (!subscribed_app->IsAudible()) {
        LOG4CXX_WARN_EXT(logger_, "OnButtonEvent in HMI_BACKGROUND or NONE");
        continue;
    }
    SendButtonEvent(subscribed_app, false);
  }
}

void OnButtonEventCommand::SendButtonEvent(const ApplicationImpl* app,
                                           bool is_custom_btn_id) {
  smart_objects::CSmartObject* on_btn_event =
      new smart_objects::CSmartObject();

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
      NsSmartDeviceLinkRPC::V2::FunctionID::eType::OnButtonEventID;
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
      NsSmartDeviceLinkRPC::V2::Result::SUCCESS;

  message_.reset(on_btn_event);
  SendResponse();
}

}  // namespace commands

}  // namespace application_manager
