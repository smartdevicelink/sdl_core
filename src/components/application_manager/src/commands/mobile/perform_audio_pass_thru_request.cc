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

#include <cstring>
#include "application_manager/commands/mobile/perform_audio_pass_thru_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "config_profile/profile.h"
#include "utils/helpers.h"

namespace application_manager {

namespace commands {

namespace str = strings;

PerformAudioPassThruRequest::PerformAudioPassThruRequest(
    const MessageSharedPtr& message)
    : CommandRequestImpl(message),
      is_active_tts_speak_(false),
      result_tts_speak_(mobile_apis::Result::SUCCESS) {
  subscribe_on_event(hmi_apis::FunctionID::TTS_OnResetTimeout);
}

PerformAudioPassThruRequest::~PerformAudioPassThruRequest() {
}

void PerformAudioPassThruRequest::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);

  FinishTTSSpeak();
  CommandRequestImpl::onTimeOut();
}

bool PerformAudioPassThruRequest::Init() {
  default_timeout_ +=
      (((*message_)[str::msg_params][str::max_duration].asUInt()));
  return true;
}

void PerformAudioPassThruRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "APPLICATION_NOT_REGISTERED");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (mobile_api::HMILevel::HMI_NONE == app->hmi_level()) {
    LOG4CXX_ERROR(logger_, "application isn't activated");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  if (IsWhiteSpaceExist()) {
    LOG4CXX_ERROR(logger_,
                  "Incoming perform audio pass thru has contains "
                  "\\t\\n \\\\t \\\\n"
                  " text contains only whitespace in initialPrompt");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if ((*message_)[str::msg_params].keyExists(str::initial_prompt) &&
      (0 < (*message_)[str::msg_params][str::initial_prompt].length())) {
    // In case TTS Speak, subscribe on notification
    SendSpeakRequest();
    SendPerformAudioPassThruRequest();
  } else {
    SendPerformAudioPassThruRequest();
    SendRecordStartNotification();
    StartMicrophoneRecording();
  }
}

void PerformAudioPassThruRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_PerformAudioPassThru: {
      LOG4CXX_TRACE(logger_, "Received UI_PerformAudioPassThru");

      if (!WaitTTSSpeak()) {
        LOG4CXX_DEBUG(logger_, "TTS.Speak is absent");
        return;
      }

      mobile_apis::Result::eType mobile_code =
          GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asUInt()));

      // in case perform audio is started by other request skip stopping
      if (mobile_apis::Result::REJECTED == mobile_code) {
        LOG4CXX_ERROR(logger_, "Request was rejected");
        SendResponse(false, mobile_code, NULL, &(message[strings::msg_params]));
        return;
      }

      FinishTTSSpeak();

      std::string return_info;
      const bool result =
          Compare<mobile_api::Result::eType, EQ, ONE>(
            mobile_code,
            mobile_apis::Result::SUCCESS,
            mobile_apis::Result::RETRY,
            mobile_apis::Result::WARNINGS);

      const bool is_result_ok =
          Compare<mobile_api::Result::eType, EQ, ONE>(
            mobile_code,
            mobile_apis::Result::SUCCESS,
            mobile_apis::Result::WARNINGS);

      if (is_result_ok &&
          mobile_apis::Result::UNSUPPORTED_RESOURCE == result_tts_speak_) {
        mobile_code = mobile_apis::Result::WARNINGS;
        return_info = "Unsupported phoneme type sent in a prompt";
      }

      SendResponse(result, mobile_code, return_info.c_str(),
                   &(message[strings::msg_params]));
      break;
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      LOG4CXX_INFO(logger_, "Received TTS_Speak event");
      result_tts_speak_ = GetMobileResultCode(
            static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asUInt()));
      is_active_tts_speak_ = false;
      if (mobile_apis::Result::SUCCESS == result_tts_speak_) {
        SendRecordStartNotification();
        StartMicrophoneRecording();

        // update request timeout to get time for perform audio recording
        ApplicationManagerImpl::instance()->
            updateRequestTimeout(connection_key(),
                                 correlation_id(),
                                 default_timeout());
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_OnResetTimeout: {
      LOG4CXX_INFO(logger_, "Received TTS_OnResetTimeout event");

      ApplicationManagerImpl::instance()->updateRequestTimeout(
          connection_key(), correlation_id(), default_timeout());
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }
}

void PerformAudioPassThruRequest::SendSpeakRequest() {
  LOG4CXX_AUTO_TRACE(logger_);

  using namespace hmi_apis;
  using namespace smart_objects;

  SmartObject msg_params = smart_objects::SmartObject(SmartType_Map);

  if ((*message_)[str::msg_params].keyExists(str::initial_prompt) &&
      (0 < (*message_)[str::msg_params][str::initial_prompt].length())) {
    for (uint32_t i = 0;
        i < (*message_)[str::msg_params][str::initial_prompt].length();
        ++i) {
      msg_params[hmi_request::tts_chunks][i][str::text] =
          (*message_)[str::msg_params][str::initial_prompt][i][str::text];
      msg_params[hmi_request::tts_chunks][i][str::type] =
          (*message_)[str::msg_params][str::initial_prompt][i][str::type];
    }
    // app_id
    msg_params[strings::app_id] = connection_key();
    msg_params[hmi_request::speak_type] = Common_MethodName::AUDIO_PASS_THRU;
    is_active_tts_speak_ = true;
    SendHMIRequest(FunctionID::TTS_Speak, &msg_params, true);
  }
}

void PerformAudioPassThruRequest::SendPerformAudioPassThruRequest() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  msg_params[str::app_id] = connection_key();

  msg_params[hmi_request::max_duration] =
      (*message_)[str::msg_params][str::max_duration];

  msg_params[hmi_request::audio_pass_display_texts] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  if ((*message_)[str::msg_params].keyExists(str::audio_pass_display_text1)) {
    msg_params[hmi_request::audio_pass_display_texts]
               [0][hmi_request::field_name] = static_cast<int32_t>
    (hmi_apis::Common_TextFieldName::audioPassThruDisplayText1);
    msg_params[hmi_request::audio_pass_display_texts]
               [0][hmi_request::field_text] =
        (*message_)[str::msg_params][str::audio_pass_display_text1];
  }

  if ((*message_)[str::msg_params].keyExists(str::audio_pass_display_text2)) {
    msg_params[hmi_request::audio_pass_display_texts]
               [1][hmi_request::field_name] = static_cast<int32_t>
    (hmi_apis::Common_TextFieldName::audioPassThruDisplayText2);
    msg_params[hmi_request::audio_pass_display_texts]
               [1][hmi_request::field_text] =
        (*message_)[str::msg_params][str::audio_pass_display_text2];
  }

  if ((*message_)[str::msg_params].keyExists(str::mute_audio)) {
    msg_params[str::mute_audio] =
        (*message_)[str::msg_params][str::mute_audio].asBool();
  } else {
    // If omitted, the value is set to true
    msg_params[str::mute_audio] = true;
  }

  SendHMIRequest(hmi_apis::FunctionID::UI_PerformAudioPassThru,
                 &msg_params, true);
}

void PerformAudioPassThruRequest::SendRecordStartNotification() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  msg_params[strings::app_id] = connection_key();

  CreateHMINotification(hmi_apis::FunctionID::UI_OnRecordStart, msg_params);
}

void PerformAudioPassThruRequest::StartMicrophoneRecording() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationManagerImpl::instance()->begin_audio_pass_thru();

  ApplicationManagerImpl::instance()->StartAudioPassThruThread(
      connection_key(), correlation_id(),
      (*message_)[str::msg_params][str::max_duration].asInt(),
      (*message_)[str::msg_params][str::sampling_rate].asInt(),
      (*message_)[str::msg_params][str::bits_per_sample].asInt(),
      (*message_)[str::msg_params][str::audio_type].asInt());
}

bool PerformAudioPassThruRequest::IsWhiteSpaceExist() {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  if ((*message_)[strings::msg_params].keyExists(strings::initial_prompt)) {
    const smart_objects::SmartArray* ip_array =
        (*message_)[strings::msg_params][strings::initial_prompt].asArray();

    smart_objects::SmartArray::const_iterator it_ip = ip_array->begin();
    smart_objects::SmartArray::const_iterator it_ip_end = ip_array->end();

    for (; it_ip != it_ip_end; ++it_ip) {
      str = (*it_ip)[strings::text].asCharArray();
      if (std::strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_, "Invalid initial_prompt syntax check failed");
        return true;
      }
    }
  }

  if ((*message_)[strings::msg_params].
      keyExists(strings::audio_pass_display_text1)) {

    str = (*message_)[strings::msg_params]
                     [strings::audio_pass_display_text1].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
          "Invalid audio_pass_display_text1 value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].
      keyExists(strings::audio_pass_display_text2)) {

    str = (*message_)[strings::msg_params]
                     [strings::audio_pass_display_text2].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
          "Invalid audio_pass_display_text2 value syntax check failed");
      return true;
    }
  }
  return false;
}

void PerformAudioPassThruRequest::FinishTTSSpeak() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (ApplicationManagerImpl::instance()->end_audio_pass_thru()) {
    LOG4CXX_DEBUG(logger_, "Stop AudioPassThru.");
    ApplicationManagerImpl::instance()->
        StopAudioPassThru(connection_key());
  }
  if (!is_active_tts_speak_) {
    LOG4CXX_WARN(logger_, "TTS Speak is inactive.");
    return;
  }
  is_active_tts_speak_ = false;
  SendHMIRequest(hmi_apis::FunctionID::TTS_StopSpeaking, NULL);
}

bool PerformAudioPassThruRequest::WaitTTSSpeak() {
  LOG4CXX_AUTO_TRACE(logger_);
  uint64_t default_timeout_msec =
      profile::Profile::instance()->default_timeout();
  const TimevalStruct start_time = date_time::DateTime::getCurrentTime();

  // Waiting for TTS_Speak
  while (is_active_tts_speak_) {
    uint64_t difference_between_start_current_time
        = date_time::DateTime::calculateTimeSpan(start_time);
    // Send GENERIC_ERROR after default timeout
    if (difference_between_start_current_time > default_timeout_msec) {
      LOG4CXX_WARN(logger_, "Expired timeout for TTS.Speak response");
      // Don't use onTimeOut(), because default_timeout_ is bigger than
      // Default time in *.ini file
      FinishTTSSpeak();
      SendResponse(false, mobile_apis::Result::eType::GENERIC_ERROR,
        "Expired timeout for TTS.Speak response");
      return false;
    }
  }
  return true;
}

}  // namespace commands

}  // namespace application_manager
