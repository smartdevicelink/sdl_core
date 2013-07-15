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

#include "application_manager/commands/mobile/show_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

ShowRequest::ShowRequest(const MessageSharedPtr& message)
  : CommandRequestImpl(message) {
}

ShowRequest::~ShowRequest() {
}

void ShowRequest::Run() {
  LOG4CXX_INFO(logger_, "ShowRequest::Run");

  ApplicationImpl* app = static_cast<ApplicationImpl*>
      (application_manager::ApplicationManagerImpl::instance()->
      application((*message_)[strings::msg_params][strings::connection_key]));

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "An application " << app->name() <<
                      " is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  smart_objects::CSmartObject msg_params =
      smart_objects::CSmartObject(smart_objects::SmartType_Map);

  msg_params = (*message_)[strings::msg_params];

  msg_params[hmi_request::show_strings] =
      smart_objects::CSmartObject(smart_objects::SmartType_Array);

  if (msg_params.keyExists(strings::main_field_1)) {
    // erase useless parametr
    msg_params.erase(strings::main_field_1);
    msg_params[hmi_request::show_strings][0][hmi_request::field_name] =
        TextFieldName::MAIN_FILED1;
    msg_params[hmi_request::show_strings][0][hmi_request::field_text] =
       (*message_)[strings::msg_params][strings::main_field_1];

  }

  if (msg_params.keyExists(strings::main_field_2)) {
    // erase useless param
    msg_params.erase(strings::main_field_2);
    msg_params[hmi_request::show_strings][1][hmi_request::field_name] =
        TextFieldName::MAIN_FILED2;
    msg_params[hmi_request::show_strings][1][hmi_request::field_text] =
       (*message_)[strings::msg_params][strings::main_field_2];

  }

  if (msg_params.keyExists(strings::main_field_3)) {
    // erase useless param
    msg_params.erase(strings::main_field_3);
    msg_params[hmi_request::show_strings][2][hmi_request::field_name] =
        TextFieldName::MAIN_FILED3;
    msg_params[hmi_request::show_strings][2][hmi_request::field_text] =
       (*message_)[strings::msg_params][strings::main_field_3];

  }

  if (msg_params.keyExists(strings::main_field_4)) {
    // erase useless param
    msg_params.erase(strings::main_field_4);
    msg_params[hmi_request::show_strings][3][hmi_request::field_name] =
        TextFieldName::MAIN_FILED4;
    msg_params[hmi_request::show_strings][3][hmi_request::field_text] =
       (*message_)[strings::msg_params][strings::main_field_4];

  }

  CreateHMIRequest(hmi_apis::FunctionID::UI_Show, msg_params, true);

  MessageSharedPtr persistentData =
      new smart_objects::CSmartObject((*message_)[strings::msg_params]);
  app->set_show_command(*persistentData);
}

}  // namespace commands

}  // namespace application_manager
