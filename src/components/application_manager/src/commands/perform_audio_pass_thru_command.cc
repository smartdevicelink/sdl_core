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

#include "application_manager/commands/perform_audio_pass_thru_command.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_chaining.h"
#include "application_manager/application_impl.h"
#include "JSONHandler/SDLRPCObjects/V2/HMILevel.h"
#include "utils/logger.h"

namespace application_manager {

namespace commands {

namespace str = strings;

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Commands"));

PerformAudioPassThruCommandRequest::PerformAudioPassThruCommandRequest(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

PerformAudioPassThruCommandRequest::~PerformAudioPassThruCommandRequest() {
}

void PerformAudioPassThruCommandRequest::Run() {
  LOG4CXX_INFO(logger_, "PerformAudioPassThruCommandRequest::Run ");

  if (ApplicationManagerImpl::instance()->audio_pass_thru_flag()) {
    LOG4CXX_ERROR_EXT(logger_, "TOO_MANY_PENDING_REQUESTS");
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::TOO_MANY_PENDING_REQUESTS);
    return;
  }

  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->
      application((*message_)[str::params][str::connection_key]));

  if (NULL == app) {
    LOG4CXX_ERROR_EXT(logger_, "APPLICATION_NOT_REGISTERED");
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  SendSpeakRequest();

  // crate HMI TTS speak request
  smart_objects::CSmartObject* ui_audio = new smart_objects::CSmartObject();
  // TODO(DK): HMI tts request Id
  const int audio_cmd_id = 61;
  (*ui_audio)[str::params][str::function_id] = audio_cmd_id;
  (*ui_audio)[str::params][str::message_type] = MessageType::kRequest;

  int i = 0;
  if ((*message_)[str::msg_params].keyExists(str::audio_pass_display_text1)) {
    (*ui_audio)[str::msg_params][hmi_request::audio_pass_display_texts][i] =
        smart_objects::CSmartObject();
    (*ui_audio)[str::msg_params][hmi_request::audio_pass_display_texts][i]
        [hmi_request::field_name] = TextFieldName::AUDIO_DISPLAY_TEXT1;
    (*ui_audio)[str::msg_params][hmi_request::audio_pass_display_texts][i]
        [hmi_request::field_text] =
            (*message_)[str::msg_params][str::audio_pass_display_text1];
    // increment index
    ++i;
  }

  if ((*message_)[str::msg_params].keyExists(str::audio_pass_display_text2)) {
    (*ui_audio)[str::msg_params][hmi_request::audio_pass_display_texts][i] =
        smart_objects::CSmartObject();
    (*ui_audio)[str::msg_params][hmi_request::audio_pass_display_texts][i]
        [hmi_request::field_name] = TextFieldName::AUDIO_DISPLAY_TEXT2;
    (*ui_audio)[str::msg_params][hmi_request::audio_pass_display_texts][i]
        [hmi_request::field_text] =
            (*message_)[str::msg_params][str::audio_pass_display_text2];
  }

  // duration
  (*ui_audio)[strings::msg_params][hmi_request::max_duration] =
      (*message_)[str::msg_params][str::max_duration];

  const int corellation_id =
      (*message_)[strings::params][strings::correlation_id];
  const int connection_key =
      (*message_)[strings::params][strings::connection_key];

  MessageChaining * chain = NULL;
  chain = ApplicationManagerImpl::instance()->AddMessageChain(chain,
      connection_key, corellation_id, audio_cmd_id);

  ApplicationManagerImpl::instance()->SendMessageToHMI(ui_audio);
  ApplicationManagerImpl::instance()->StartAudioPassThruThread(connection_key,
      corellation_id, (*message_)[str::msg_params][str::max_duration].asInt(),
      (*message_)[str::msg_params][str::sampling_rate].asInt(),
      (*message_)[str::msg_params][str::bits_per_sample].asInt(),
      (*message_)[str::msg_params][str::audio_type].asInt());
}

void PerformAudioPassThruCommandRequest::SendSpeakRequest() const {
  // crate HMI TTS speak request
  smart_objects::CSmartObject* tts_speak = new smart_objects::CSmartObject();
  // TODO(DK): HMI tts request Id
  const int tts_cmd_id = 51;
  (*tts_speak)[str::params][str::function_id] = tts_cmd_id;
  (*tts_speak)[str::params][str::message_type] = MessageType::kRequest;

  int i = 0;
  if ((*message_)[str::msg_params].keyExists(str::audio_pass_display_text1)) {
    (*tts_speak)[str::msg_params][hmi_request::tts_chunks][i++] =
        (*message_)[str::msg_params][str::audio_pass_display_text1];
  }

  if ((*message_)[str::msg_params].keyExists(str::audio_pass_display_text2)) {
    (*tts_speak)[str::msg_params][hmi_request::tts_chunks][i] =
        (*message_)[str::msg_params][str::audio_pass_display_text2];
  }

  // app_id
  (*tts_speak)[strings::msg_params][strings::app_id] =
      (*message_)[strings::params][strings::connection_key];

    if (0 < tts_speak->length()) {
      ApplicationManagerImpl::instance()->SendMessageToHMI(tts_speak);
    }
}

}  // namespace commands

}  // namespace application_manager
