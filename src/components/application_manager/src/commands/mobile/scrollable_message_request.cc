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

#include "application_manager/commands/mobile/scrollable_message_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

ScrollabelMessageRequest::ScrollabelMessageRequest(
  const MessageSharedPtr& message): CommandRequestImpl(message) {

}

ScrollabelMessageRequest::~ScrollabelMessageRequest(){
}

void ScrollabelMessageRequest::Run() {
  LOG4CXX_INFO(logger_, "ScrollabelMessageRequest::Run");

  Application* app = application_manager::ApplicationManagerImpl::instance()->
    application((*message_)[strings::params][strings::connection_key]);

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  smart_objects::SmartObject msg_params =
    smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[hmi_request::message_text][hmi_request::field_name] =
    TextFieldName::SCROLLABLE_MSG_BODY;
  msg_params[hmi_request::message_text][hmi_request::field_text] =
    (*message_)[strings::msg_params][strings::scroll_message_body];
  msg_params[strings::app_id] = app->app_id();

  if ((*message_)[strings::msg_params].keyExists(strings::timeout)) {
    msg_params[strings::timeout] =
        (*message_)[strings::msg_params][strings::timeout];
  }
  else {
    msg_params[strings::timeout] = 30000;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[strings::soft_buttons] =
        (*message_)[strings::msg_params][strings::soft_buttons];
  }

  CreateHMIRequest(hmi_apis::FunctionID::UI_ScrollableMessage,
                   msg_params, true, 1);
}

}  // namespace commands
}  // namespace application_manager

