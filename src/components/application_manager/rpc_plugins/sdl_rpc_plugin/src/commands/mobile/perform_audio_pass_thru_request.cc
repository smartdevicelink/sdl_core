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
#include "sdl_rpc_plugin/commands/mobile/perform_audio_pass_thru_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "utils/helpers.h"

namespace application_manager {

namespace commands {

namespace str = strings;

PerformAudioPassThruRequest::PerformAudioPassThruRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager)
    , result_tts_speak_(hmi_apis::Common_Result::INVALID_ENUM)
    , result_ui_(hmi_apis::Common_Result::INVALID_ENUM) {
  subscribe_on_event(hmi_apis::FunctionID::TTS_OnResetTimeout);
}

PerformAudioPassThruRequest::~PerformAudioPassThruRequest() {}

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

  ApplicationSharedPtr app = application_manager_.application(connection_key());

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
  // According with new implementation processing of UNSUPPORTE_RESOURCE
  // need set flag before sending to hmi

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
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
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);

      result_ui_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asUInt());
      GetInfo(message, ui_info_);

      // in case perform audio is started by other request skip stopping
      if (hmi_apis::Common_Result::REJECTED == result_ui_) {
        LOG4CXX_ERROR(logger_, "Request was rejected");
        SendResponse(false,
                     MessageHelper::HMIToMobileResult(result_ui_),
                     NULL,
                     &(message[strings::msg_params]));
        return;
      }
      FinishTTSSpeak();
      break;
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      LOG4CXX_INFO(logger_, "Received TTS_Speak event");
      result_tts_speak_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asUInt());
      GetInfo(message, tts_info_);
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
      const bool is_tts_speak_success_unsuported =
          Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
              result_tts_speak_,
              hmi_apis::Common_Result::SUCCESS,
              hmi_apis::Common_Result::WARNINGS,
              hmi_apis::Common_Result::WRONG_LANGUAGE,
              hmi_apis::Common_Result::RETRY,
              hmi_apis::Common_Result::SAVED,
              hmi_apis::Common_Result::UNSUPPORTED_RESOURCE);

      if (is_tts_speak_success_unsuported) {
        SendRecordStartNotification();
        StartMicrophoneRecording();

        // update request timeout to get time for perform audio recording
        application_manager_.updateRequestTimeout(
            connection_key(), correlation_id(), default_timeout());
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_OnResetTimeout: {
      LOG4CXX_INFO(logger_, "Received TTS_OnResetTimeout event");

      application_manager_.updateRequestTimeout(
          connection_key(), correlation_id(), default_timeout());
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }
  if (IsWaitingHMIResponse()) {
    return;
  }

  const ResponseParams response_params = PrepareResponseParameters();

  SendResponse(
      response_params.success,
      response_params.result_code,
      response_params.info.empty() ? NULL : response_params.info.c_str(),
      &(message[strings::msg_params]));
}

const PerformAudioPassThruRequest::ResponseParams&
PerformAudioPassThruRequest::PrepareResponseParameters() {
  LOG4CXX_AUTO_TRACE(logger_);

  ResponseInfo ui_perform_info(
      result_ui_, HmiInterfaces::HMI_INTERFACE_UI, application_manager_);
  ResponseInfo tts_perform_info(result_tts_speak_,
                                HmiInterfaces::HMI_INTERFACE_TTS,
                                application_manager_);

  // Note(dtrunov): According to requirment "WARNINGS, success:true on getting
  // UNSUPPORTED_RESOURCE for "ttsChunks"
  if (ui_perform_info.is_ok && tts_perform_info.is_unsupported_resource &&
      HmiInterfaces::STATE_AVAILABLE == tts_perform_info.interface_state) {
    response_params_.result_code = mobile_apis::Result::WARNINGS;
    tts_info_ = "Unsupported phoneme type sent in a prompt";
    response_params_.info =
        MergeInfos(ui_perform_info, ui_info_, tts_perform_info, tts_info_);
    response_params_.success = true;
    return response_params_;
  }

  response_params_.success =
      PrepareResultForMobileResponse(ui_perform_info, tts_perform_info);
  if (IsResultCodeUnsupported(ui_perform_info, tts_perform_info)) {
    response_params_.result_code = mobile_apis::Result::UNSUPPORTED_RESOURCE;
  } else {
    AudioPassThruResults results = PrepareAudioPassThruResultCodeForResponse(
        ui_perform_info, tts_perform_info);
    response_params_.success = results.second;
    response_params_.result_code = results.first;
  }
  response_params_.info =
      MergeInfos(ui_perform_info, ui_info_, tts_perform_info, tts_info_);

  return response_params_;
}

bool PerformAudioPassThruRequest::PrepareResponseParameters(
    mobile_apis::Result::eType& result_code, std::string& info) {
  LOG4CXX_AUTO_TRACE(logger_);
  NOTREACHED();
  return false;
}

void PerformAudioPassThruRequest::SendSpeakRequest() {
  LOG4CXX_AUTO_TRACE(logger_);

  using namespace hmi_apis;
  using namespace smart_objects;

  SmartObject msg_params = smart_objects::SmartObject(SmartType_Map);
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
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
  SendHMIRequest(FunctionID::TTS_Speak, &msg_params, true);
}

void PerformAudioPassThruRequest::SendPerformAudioPassThruRequest() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[str::app_id] = connection_key();

  msg_params[hmi_request::max_duration] =
      (*message_)[str::msg_params][str::max_duration];

  msg_params[hmi_request::audio_pass_display_texts] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  if ((*message_)[str::msg_params].keyExists(str::audio_pass_display_text1)) {
    msg_params[hmi_request::audio_pass_display_texts][0]
              [hmi_request::field_name] = static_cast<int32_t>(
                  hmi_apis::Common_TextFieldName::audioPassThruDisplayText1);
    msg_params[hmi_request::audio_pass_display_texts][0]
              [hmi_request::field_text] =
                  (*message_)[str::msg_params][str::audio_pass_display_text1];
  }

  if ((*message_)[str::msg_params].keyExists(str::audio_pass_display_text2)) {
    msg_params[hmi_request::audio_pass_display_texts][1]
              [hmi_request::field_name] = static_cast<int32_t>(
                  hmi_apis::Common_TextFieldName::audioPassThruDisplayText2);
    msg_params[hmi_request::audio_pass_display_texts][1]
              [hmi_request::field_text] =
                  (*message_)[str::msg_params][str::audio_pass_display_text2];
  }

  if ((*message_)[str::msg_params].keyExists(str::mute_audio)) {
    msg_params[str::mute_audio] =
        (*message_)[str::msg_params][str::mute_audio].asBool();
  } else {
    // If omitted, the value is set to true
    msg_params[str::mute_audio] = true;
  }

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(
      hmi_apis::FunctionID::UI_PerformAudioPassThru, &msg_params, true);
}

void PerformAudioPassThruRequest::SendRecordStartNotification() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = connection_key();

  CreateHMINotification(hmi_apis::FunctionID::UI_OnRecordStart, msg_params);
}

void PerformAudioPassThruRequest::StartMicrophoneRecording() {
  LOG4CXX_AUTO_TRACE(logger_);

  application_manager_.BeginAudioPassThrough();

  application_manager_.StartAudioPassThruThread(
      connection_key(),
      correlation_id(),
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

  if ((*message_)[strings::msg_params].keyExists(
          strings::audio_pass_display_text1)) {
    str = (*message_)[strings::msg_params][strings::audio_pass_display_text1]
              .asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(
          logger_,
          "Invalid audio_pass_display_text1 value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(
          strings::audio_pass_display_text2)) {
    str = (*message_)[strings::msg_params][strings::audio_pass_display_text2]
              .asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(
          logger_,
          "Invalid audio_pass_display_text2 value syntax check failed");
      return true;
    }
  }
  return false;
}

void PerformAudioPassThruRequest::FinishTTSSpeak() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (application_manager_.EndAudioPassThrough()) {
    LOG4CXX_DEBUG(logger_, "Stop AudioPassThru.");
    application_manager_.StopAudioPassThru(connection_key());
  }
  if (!IsInterfaceAwaited(HmiInterfaces::HMI_INTERFACE_TTS)) {
    LOG4CXX_WARN(logger_, "TTS Speak is inactive.");
    return;
  }
  SendHMIRequest(hmi_apis::FunctionID::TTS_StopSpeaking, NULL);
}

PerformAudioPassThruRequest::AudioPassThruResults
PerformAudioPassThruRequest::PrepareAudioPassThruResultCodeForResponse(
    const ResponseInfo& ui_response, const ResponseInfo& tts_response) {
  mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;

  hmi_apis::Common_Result::eType common_result =
      hmi_apis::Common_Result::INVALID_ENUM;
  const hmi_apis::Common_Result::eType ui_result = ui_response.result_code;
  const hmi_apis::Common_Result::eType tts_result = tts_response.result_code;
  bool result = false;

  if ((ui_result == hmi_apis::Common_Result::SUCCESS) &&
      (tts_result == hmi_apis::Common_Result::SUCCESS)) {
    result = true;
  }

  if ((ui_result == hmi_apis::Common_Result::SUCCESS) &&
      (tts_result == hmi_apis::Common_Result::INVALID_ENUM)) {
    result = true;
  }

  if ((ui_result == hmi_apis::Common_Result::SUCCESS) &&
      (tts_result != hmi_apis::Common_Result::SUCCESS) &&
      (tts_result != hmi_apis::Common_Result::INVALID_ENUM)) {
    common_result = hmi_apis::Common_Result::WARNINGS;
    result = true;
  } else if (ui_response.is_ok &&
             tts_result == hmi_apis::Common_Result::WARNINGS) {
    common_result = hmi_apis::Common_Result::WARNINGS;
    result = true;
  } else if (ui_result == hmi_apis::Common_Result::INVALID_ENUM) {
    common_result = tts_result;
  } else {
    common_result = ui_result;
  }
  result_code = MessageHelper::HMIToMobileResult(common_result);
  return std::make_pair(result_code, result);
}

bool PerformAudioPassThruRequest::IsWaitingHMIResponse() {
  LOG4CXX_AUTO_TRACE(logger_);
  return IsInterfaceAwaited(HmiInterfaces::HMI_INTERFACE_TTS) ||
         IsInterfaceAwaited(HmiInterfaces::HMI_INTERFACE_UI);
}

}  // namespace commands

}  // namespace application_manager
