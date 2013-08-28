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

#include "application_manager/commands/mobile/alert_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

AlertRequest::AlertRequest(
  const MessageSharedPtr& message): CommandRequestImpl(message) {
}

AlertRequest::~AlertRequest() {
}

void AlertRequest::Run() {
  LOG4CXX_INFO(logger_, "AlertRequest::Run");

  unsigned int app_id =
    (*message_)[strings::params][strings::connection_key].asInt();
  Application* app = ApplicationManagerImpl::instance()->application(app_id);

  if (NULL == app) {
    LOG4CXX_ERROR_EXT(logger_, "No application associated with session key");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  //check if mandatory params(alertText1 and TTSChunk) specified
  if ((!(*message_)[strings::msg_params].keyExists(strings::alert_text1)) ||
      (!(*message_)[strings::msg_params].keyExists(strings::tts_chunks) ||
      (1 > (*message_)[strings::msg_params][strings::tts_chunks].length()))) {
    LOG4CXX_ERROR_EXT(logger_, "Mandatoty parameters omitted");
    SendResponse(false, mobile_apis::Result::INVALID_DATA,
                 "Mandatoty parameters omitted");
    return;
  }

  mobile_apis::Result::eType verification_result =
      MessageHelper::VerifyImageFiles((*message_)[strings::msg_params], app);

  if (mobile_apis::Result::SUCCESS != verification_result) {
    LOG4CXX_ERROR_EXT(logger_, "MessageHelper::VerifyImageFiles return " <<
                          verification_result);
    SendResponse(false, verification_result);
    return;
  }

  SendAlertRequest(app->app_id());
  SendPlayToneNotification(app->app_id());
  SendSpeakRequest(app->app_id());
}

void AlertRequest::SendAlertRequest(int app_id) {
  smart_objects::SmartObject msg_params =
    smart_objects::SmartObject(smart_objects::SmartType_Map);

  // alert1
  msg_params[hmi_request::alert_strings] =
    smart_objects::SmartObject(smart_objects::SmartType_Array);
  msg_params[hmi_request::alert_strings][0]
  [hmi_request::field_name] = TextFieldName::ALERT_TEXT1;
  msg_params[hmi_request::alert_strings][0]
  [hmi_request::field_text] =
    (*message_)[strings::msg_params][strings::alert_text1];

  // alert2
  msg_params[hmi_request::alert_strings][1][hmi_request::field_name] =
    TextFieldName::ALERT_TEXT2;
  msg_params[hmi_request::alert_strings][1][hmi_request::field_text] =
    (*message_)[strings::msg_params][strings::alert_text2];

  // alert3
  msg_params[hmi_request::alert_strings][2][hmi_request::field_name] =
    TextFieldName::ALERT_TEXT3;
  msg_params[hmi_request::alert_strings][2][hmi_request::field_text] =
    (*message_)[strings::msg_params][strings::alert_text3];

  // softButtons
  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[hmi_request::soft_buttons] =
      (*message_)[strings::msg_params][strings::soft_buttons];
  }
  // app_id
  msg_params[strings::app_id] = app_id;

  if ((*message_)[strings::msg_params].keyExists(strings::duration)) {
    msg_params[strings::duration] =
        (*message_)[strings::msg_params][strings::duration];
  }
  else {
    msg_params[strings::duration] = 5000;
  }

  CreateHMIRequest(hmi_apis::FunctionID::UI_Alert, msg_params, true, 1);
}

void AlertRequest::SendSpeakRequest(int app_id) {
  // check TTSChunk parameter
  if ((*message_)[strings::msg_params].keyExists(strings::tts_chunks)) {
    if (0 < (*message_)[strings::msg_params][strings::tts_chunks].length()) {
      // crate HMI basic communication playtone request
      smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

      msg_params[hmi_request::tts_chunks] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
      msg_params[hmi_request::tts_chunks] =
        (*message_)[strings::msg_params][strings::tts_chunks];
      msg_params[strings::app_id] = app_id;
      CreateHMIRequest(hmi_apis::FunctionID::TTS_Speak, msg_params);
    }
  }
}

void AlertRequest::SendPlayToneNotification(int app_id) {
  LOG4CXX_INFO(logger_, "AlertRequest::SendPlayToneNotification");

  // check playtone parameter
  if ((*message_)[strings::msg_params].keyExists(strings::play_tone)) {
    if ((*message_)[strings::msg_params][strings::play_tone].asBool()) {
      // crate HMI basic communication playtone request
      smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

      CreateHMINotification(hmi_apis::FunctionID::BasicCommunication_PlayTone,
                            msg_params);
    }
  }
}

}  // namespace commands

}  // namespace application_manager
