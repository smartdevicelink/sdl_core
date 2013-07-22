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
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

ShowRequest::ShowRequest(const MessageSharedPtr& message)
  : CommandRequestImpl(message) {
}

ShowRequest::~ShowRequest() {
}

void ShowRequest::Run() {
  LOG4CXX_INFO(logger_, "ShowRequest::Run");

  Application* app = application_manager::ApplicationManagerImpl::instance()->
    application((*message_)[strings::params][strings::connection_key].asInt());

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "An application " << app->name() <<
                      " is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  smart_objects::SmartObject msg_params =
    smart_objects::SmartObject(smart_objects::SmartType_Map);


  msg_params[hmi_request::show_strings] =
    smart_objects::SmartObject(smart_objects::SmartType_Array);
  msg_params[strings::app_id] = app->app_id();

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_1)) {
    msg_params[hmi_request::show_strings][0][hmi_request::field_name] =
      TextFieldName::MAIN_FILED1;
    msg_params[hmi_request::show_strings][0][hmi_request::field_text] =
      (*message_)[strings::msg_params][strings::main_field_1];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_2)) {
    msg_params[hmi_request::show_strings][1][hmi_request::field_name] =
      TextFieldName::MAIN_FILED2;
    msg_params[hmi_request::show_strings][1][hmi_request::field_text] =
      (*message_)[strings::msg_params][strings::main_field_2];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_3)) {
    msg_params[hmi_request::show_strings][2][hmi_request::field_name] =
      TextFieldName::MAIN_FILED3;
    msg_params[hmi_request::show_strings][2][hmi_request::field_text] =
      (*message_)[strings::msg_params][strings::main_field_3];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_4)) {
    msg_params[hmi_request::show_strings][3][hmi_request::field_name] =
      TextFieldName::MAIN_FILED4;
    msg_params[hmi_request::show_strings][3][hmi_request::field_text] =
      (*message_)[strings::msg_params][strings::main_field_4];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::media_clock)) {
    msg_params[hmi_request::show_strings][4][hmi_request::field_name] =
      TextFieldName::MEDIA_CLOCK;
    msg_params[hmi_request::show_strings][4][hmi_request::field_text] =
      (*message_)[strings::msg_params][strings::media_clock];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::media_track)) {
    msg_params[hmi_request::show_strings][5][hmi_request::field_name] =
      TextFieldName::MEDIA_TRACK;
    msg_params[hmi_request::show_strings][5][hmi_request::field_text] =
      (*message_)[strings::msg_params][strings::media_track];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::status_bar)) {
    msg_params[hmi_request::show_strings][6][hmi_request::field_name] =
      TextFieldName::STATUS_BAR;
    msg_params[hmi_request::show_strings][6][hmi_request::field_text] =
      (*message_)[strings::msg_params][strings::status_bar];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::alignment)) {
    msg_params[strings::alignment] =
      (*message_)[strings::msg_params][strings::alignment];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::graphic)) {
    msg_params[strings::graphic] =
      (*message_)[strings::msg_params][strings::graphic];
    std::string file_path = file_system::FullPath(app->name());
    file_path += "/";
    file_path += (*message_)[strings::msg_params][strings::graphic]
                         [strings::value].asString();

    msg_params[strings::graphic][strings::value] = file_path;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[strings::soft_buttons] =
      (*message_)[strings::msg_params][strings::soft_buttons];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::custom_presets)) {
    msg_params[strings::custom_presets] =
      (*message_)[strings::msg_params][strings::custom_presets];
  }

  CreateHMIRequest(hmi_apis::FunctionID::UI_Show, msg_params, true, 1);

  MessageSharedPtr persistentData =
    new smart_objects::SmartObject((*message_)[strings::msg_params]);
  app->set_show_command(*persistentData);
}

}  // namespace commands

}  // namespace application_manager
