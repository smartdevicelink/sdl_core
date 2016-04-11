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

#include <string.h>

#include "application_manager/message_helper.h"
#include "application_manager/application_impl.h"
#include "application_manager/application_manager_impl.h"
#include "utils/helpers.h"

namespace application_manager {

namespace commands {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

AlertRequest::AlertRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message),
      awaiting_ui_alert_response_(false),
      awaiting_tts_speak_response_(false),
      awaiting_tts_stop_speaking_response_(false),
      is_alert_succeeded_(false),
      is_ui_alert_sent_(false),
      alert_result_(mobile_apis::Result::INVALID_ENUM),
      tts_speak_result_(mobile_apis::Result::INVALID_ENUM) {
  subscribe_on_event(hmi_apis::FunctionID::UI_OnResetTimeout);
  subscribe_on_event(hmi_apis::FunctionID::TTS_OnResetTimeout);
}

AlertRequest::~AlertRequest() {
}

bool AlertRequest::Init() {
  /* Timeout in milliseconds.
     If omitted a standard value of 10000 milliseconds is used.*/
  if ((*message_)[strings::msg_params].keyExists(strings::duration)) {
    default_timeout_ =
        (*message_)[strings::msg_params][strings::duration].asUInt();
  } else {
    const int32_t def_value = 5000;
    default_timeout_ = def_value;
  }

  // If soft buttons are present, SDL will not use initiate timeout tracking for response.
  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    LOG4CXX_INFO(logger_, "Request contains soft buttons - request timeout "
                 "will be set to 0.");
    default_timeout_ = 0;
  }

  return true;
}

void AlertRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t app_id = (*message_)[strings::params][strings::connection_key]
      .asInt();

  if (!Validate(app_id)) {
    // Invalid command, abort execution
    return;
  }
  bool tts_chunks_exists = (*message_)[strings::msg_params].keyExists(strings::tts_chunks);
  size_t length_tts_chunks = 0;

  if (tts_chunks_exists) {
    length_tts_chunks = (*message_)[strings::msg_params][strings::tts_chunks].length();
  }

  if ((tts_chunks_exists && length_tts_chunks) ||
      ((*message_)[strings::msg_params].keyExists(strings::play_tone) &&
      (*message_)[strings::msg_params][strings::play_tone].asBool())) {
    awaiting_tts_speak_response_ = true;
  }

  SendAlertRequest(app_id);
  if (awaiting_tts_speak_response_) {
    SendSpeakRequest(app_id, tts_chunks_exists, length_tts_chunks);
  }
}

void AlertRequest::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (false == (*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    CommandRequestImpl::onTimeOut();
    return;
  }
  LOG4CXX_INFO(logger_, "Default timeout ignored. "
               "AlertRequest with soft buttons wait timeout on HMI side");
}

void AlertRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::TTS_OnResetTimeout:
    case hmi_apis::FunctionID::UI_OnResetTimeout: {
      LOG4CXX_INFO(logger_, "Received UI_OnResetTimeout event "
          " or TTS_OnResetTimeout event"
                   << awaiting_tts_speak_response_ << " "
                   << awaiting_tts_stop_speaking_response_ << " "
                   << awaiting_ui_alert_response_);
      ApplicationManagerImpl::instance()->updateRequestTimeout(
          connection_key(), correlation_id(), default_timeout());
      break;
    }
    case hmi_apis::FunctionID::UI_Alert: {
      LOG4CXX_INFO(logger_, "Received UI_Alert event");
      // Unsubscribe from event to avoid unwanted messages
      unsubscribe_from_event(hmi_apis::FunctionID::UI_Alert);
      awaiting_ui_alert_response_ = false;

      if (awaiting_tts_speak_response_) {
        awaiting_tts_stop_speaking_response_ = true;
        SendHMIRequest(hmi_apis::FunctionID::TTS_StopSpeaking, NULL, true);
      }

      mobile_apis::Result::eType result_code =
          static_cast<mobile_apis::Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      // Mobile Alert request is successful when UI_Alert is successful

      const bool is_alert_ok =
          Compare<mobile_api::Result::eType, EQ, ONE>(
            result_code,
            mobile_apis::Result::SUCCESS,
            mobile_apis::Result::UNSUPPORTED_RESOURCE,
            mobile_apis::Result::WARNINGS);

      is_alert_succeeded_ = is_alert_ok;
      alert_result_ = result_code;
      alert_response_params_ = message[strings::msg_params];
      break;
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      LOG4CXX_INFO(logger_, "Received TTS_Speak event");
      // Unsubscribe from event to avoid unwanted messages
      unsubscribe_from_event(hmi_apis::FunctionID::TTS_Speak);
      awaiting_tts_speak_response_ = false;
      tts_speak_result_ = static_cast<mobile_apis::Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      break;
    }
    case hmi_apis::FunctionID::TTS_StopSpeaking: {
      LOG4CXX_INFO(logger_, "Received TTS_StopSpeaking event");
      // Unsubscribe from event to avoid unwanted messages
      unsubscribe_from_event(hmi_apis::FunctionID::TTS_StopSpeaking);
      awaiting_tts_stop_speaking_response_ = false;
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      return;
    }
  }

  if (HasHmiResponsesToWait()) {
    return;
  }

  const bool is_tts_alert_unsupported =
      Compare<mobile_api::Result::eType, EQ, ALL>(
        mobile_api::Result::UNSUPPORTED_RESOURCE,
        tts_speak_result_,
        alert_result_);

  const bool is_alert_ok =
      Compare<mobile_api::Result::eType, EQ, ONE>(
        alert_result_,
        mobile_api::Result::SUCCESS,
        mobile_api::Result::WARNINGS);

  std::string response_info;
  if (mobile_apis::Result::UNSUPPORTED_RESOURCE == tts_speak_result_ &&
      !is_ui_alert_sent_) {
    is_alert_succeeded_ = false;
    alert_result_ = mobile_apis::Result::WARNINGS;
    response_info = "Unsupported phoneme type sent in a prompt";
  } else if (is_tts_alert_unsupported) {
    alert_result_ = mobile_apis::Result::WARNINGS;
    response_info = "Unsupported phoneme type sent in a prompt and "
        "unsupported image sent in soft buttons";
  } else if (mobile_apis::Result::UNSUPPORTED_RESOURCE == tts_speak_result_ &&
             is_alert_ok) {
    alert_result_ = mobile_apis::Result::WARNINGS;
    response_info = "Unsupported phoneme type sent in a prompt";
  } else if (mobile_apis::Result::SUCCESS == tts_speak_result_ &&
            (mobile_apis::Result::INVALID_ENUM == alert_result_ &&
            !is_ui_alert_sent_)) {
    alert_result_ = mobile_apis::Result::SUCCESS;
    is_alert_succeeded_ = true;
  }

  const bool is_tts_not_ok =
      Compare<mobile_api::Result::eType, EQ, ONE>(
        tts_speak_result_,
        mobile_api::Result::ABORTED,
        mobile_api::Result::REJECTED);

  if (is_tts_not_ok && !is_ui_alert_sent_) {
    is_alert_succeeded_ = false;
    alert_result_ = tts_speak_result_;
  }

  if (mobile_apis::Result::WARNINGS == tts_speak_result_) {
    alert_result_ = mobile_apis::Result::WARNINGS;
  }

  SendResponse(is_alert_succeeded_, alert_result_,
               response_info.empty() ? NULL : response_info.c_str(),
               &alert_response_params_);
}

bool AlertRequest::Validate(uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);

  if (!app) {
    LOG4CXX_ERROR(logger_, "No application associated with session key");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return false;
  }

  if (mobile_apis::HMILevel::HMI_BACKGROUND == app->hmi_level() &&
      app->IsCommandLimitsExceeded(
        static_cast<mobile_apis::FunctionID::eType>(function_id()),
        application_manager::TLimitSource::POLICY_TABLE)) {
    LOG4CXX_ERROR(logger_, "Alert frequency is too high.");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return false;
  }

  if (!CheckStringsOfAlertRequest()) {
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return false;
  }

  //ProcessSoftButtons checks strings on the contents incorrect character

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons((*message_)[strings::msg_params], app,
          application_manager::ApplicationManagerImpl::instance()->GetPolicyHandler());

  if (mobile_apis::Result::SUCCESS != processing_result) {
    LOG4CXX_ERROR(logger_, "INVALID_DATA!");
    SendResponse(false, processing_result);
    return false;
  }

  // check if mandatory params(alertText1 and TTSChunk) specified
  if ((!(*message_)[strings::msg_params].keyExists(strings::alert_text1))
      && (!(*message_)[strings::msg_params].keyExists(strings::alert_text2))
      && (!(*message_)[strings::msg_params].keyExists(strings::tts_chunks)
          && (1 > (*message_)[strings::msg_params]
                              [strings::tts_chunks].length()))) {
    LOG4CXX_ERROR(logger_, "Mandatory parameters are missing");
    SendResponse(false, mobile_apis::Result::INVALID_DATA,
                 "Mandatory parameters are missing");
    return false;
  }

  return true;
}

void AlertRequest::SendAlertRequest(int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  msg_params[hmi_request::alert_strings] = smart_objects::SmartObject(
      smart_objects::SmartType_Array);

  int32_t index = 0;
  if ((*message_)[strings::msg_params].keyExists(strings::alert_text1)) {
    msg_params[hmi_request::alert_strings][index][hmi_request::field_name] =
        hmi_apis::Common_TextFieldName::alertText1;
     msg_params[hmi_request::alert_strings][index][hmi_request::field_text] =
         (*message_)[strings::msg_params][strings::alert_text1];
     index++;
  }
  if ((*message_)[strings::msg_params].keyExists(strings::alert_text2)) {
    msg_params[hmi_request::alert_strings][index][hmi_request::field_name] =
        hmi_apis::Common_TextFieldName::alertText2;
    msg_params[hmi_request::alert_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::alert_text2];
    index++;
  }
  if ((*message_)[strings::msg_params].keyExists(strings::alert_text3)) {
    msg_params[hmi_request::alert_strings][index][hmi_request::field_name] =
        hmi_apis::Common_TextFieldName::alertText3;
    msg_params[hmi_request::alert_strings][index][hmi_request::field_text] =
         (*message_)[strings::msg_params][strings::alert_text3];
  }

  // softButtons
  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[hmi_request::soft_buttons] =
        (*message_)[strings::msg_params][strings::soft_buttons];
    MessageHelper::SubscribeApplicationToSoftButton(
        (*message_)[strings::msg_params], app, function_id());
  }
  // app_id
  msg_params[strings::app_id] = app_id;
  msg_params[strings::duration] = default_timeout_;

  // NAVI platform progressIndicator
  if ((*message_)[strings::msg_params].keyExists(strings::progress_indicator)) {
    msg_params[strings::progress_indicator] =
      (*message_)[strings::msg_params][strings::progress_indicator];
  }

  // PASA Alert type
  msg_params[strings::alert_type] = hmi_apis::Common_AlertType::UI;
  if (awaiting_tts_speak_response_) {
    msg_params[strings::alert_type] = hmi_apis::Common_AlertType::BOTH;
  }

  // check out if there are alert strings or soft buttons
  if (msg_params[hmi_request::alert_strings].length() > 0 ||
      msg_params.keyExists(hmi_request::soft_buttons)) {

    awaiting_ui_alert_response_ = true;
    is_ui_alert_sent_ = true;
    SendHMIRequest(hmi_apis::FunctionID::UI_Alert, &msg_params, true);
  }
}

void AlertRequest::SendSpeakRequest(int32_t app_id, bool tts_chunks_exists,
                                    size_t length_tts_chunks) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace hmi_apis;
  using namespace smart_objects;
  // crate HMI speak request
  SmartObject msg_params = smart_objects::SmartObject(SmartType_Map);
  if (tts_chunks_exists && length_tts_chunks) {
    msg_params[hmi_request::tts_chunks] =
        smart_objects::SmartObject(SmartType_Array);
    msg_params[hmi_request::tts_chunks] =
        (*message_)[strings::msg_params][strings::tts_chunks];
  }
  if ((*message_)[strings::msg_params].keyExists(strings::play_tone) &&
      (*message_)[strings::msg_params][strings::play_tone].asBool()) {
    msg_params[strings::play_tone] = true;
  }
  msg_params[strings::app_id] = app_id;
  msg_params[hmi_request::speak_type] = Common_MethodName::ALERT;
  SendHMIRequest(FunctionID::TTS_Speak, &msg_params, true);
}

bool AlertRequest::CheckStringsOfAlertRequest() {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  if ((*message_)[strings::msg_params].keyExists(strings::alert_text1)) {
    str = (*message_)[strings::msg_params][strings::alert_text1].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid alert_text_1 syntax check failed");
      return  false;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::alert_text2)) {
    str = (*message_)[strings::msg_params][strings::alert_text2].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid alert_text_2 syntax check failed");
      return  false;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::alert_text3)) {
    str = (*message_)[strings::msg_params][strings::alert_text3].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid alert_text_3 syntax check failed");
      return  false;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::tts_chunks)) {
    smart_objects::SmartObject& tts_chunks_array =
        (*message_)[strings::msg_params][strings::tts_chunks];
    for (size_t i = 0; i < tts_chunks_array.length(); ++i) {
      str = tts_chunks_array[i][strings::text].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_, "Invalid tts_chunks text syntax check failed");
        return false;
      }
    }
  }
  return true;
}

bool AlertRequest::HasHmiResponsesToWait() {
  LOG4CXX_AUTO_TRACE(logger_);
  return awaiting_ui_alert_response_ || awaiting_tts_speak_response_
      || awaiting_tts_stop_speaking_response_;
}

}  // namespace commands

}  // namespace application_manager
