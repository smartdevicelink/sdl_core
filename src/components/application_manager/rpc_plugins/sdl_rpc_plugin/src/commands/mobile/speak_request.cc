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

#include <string.h>
#include "application_manager/commands/mobile/speak_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "utils/helpers.h"

namespace application_manager {

namespace commands {

SpeakRequest::SpeakRequest(const MessageSharedPtr& message,
                           ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {
  subscribe_on_event(hmi_apis::FunctionID::TTS_OnResetTimeout);
}

SpeakRequest::~SpeakRequest() {}

void SpeakRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (IsWhiteSpaceExist()) {
    LOG4CXX_ERROR(logger_,
                  "Incoming speak has contains \\t\\n \\\\t \\\\n "
                  " text contains only whitespace in ttsChunks");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  smart_objects::SmartObject& tts_chunks =
      (*message_)[strings::msg_params][strings::tts_chunks];
  mobile_apis::Result::eType verification_result =
      MessageHelper::VerifyTtsFiles(tts_chunks, app, application_manager_);

  if (mobile_apis::Result::FILE_NOT_FOUND == verification_result) {
    LOG4CXX_ERROR(logger_,
                  "MessageHelper::VerifyTtsFiles return "
                      << verification_result);
    SendResponse(false,
                 mobile_apis::Result::FILE_NOT_FOUND,
                 "One or more files needed for tts_chunks are not present");
    return;
  }

  (*message_)[strings::msg_params][strings::app_id] = app->app_id();
  (*message_)[strings::msg_params][hmi_request::speak_type] =
      hmi_apis::Common_MethodName::SPEAK;
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
  SendHMIRequest(hmi_apis::FunctionID::TTS_Speak,
                 &message_->getElement(strings::msg_params),
                 true);
}

void SpeakRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  switch (event.id()) {
    case hmi_apis::FunctionID::TTS_Speak: {
      LOG4CXX_INFO(logger_, "Received TTS_Speak event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
      ProcessTTSSpeakResponse(event.smart_object());
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
      break;
    }
  }
}

void SpeakRequest::ProcessTTSSpeakResponse(
    const smart_objects::SmartObject& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (!application) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  hmi_apis::Common_Result::eType hmi_result_code =
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());

  mobile_apis::Result::eType result_code =
      MessageHelper::HMIToMobileResult(hmi_result_code);

  const bool result = PrepareResultForMobileResponse(
      hmi_result_code, HmiInterfaces::HMI_INTERFACE_TTS);

  (*message_)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::SpeakID;

  const char* return_info = NULL;

  SendResponse(
      result, result_code, return_info, &(message[strings::msg_params]));
}

bool SpeakRequest::IsWhiteSpaceExist() {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  if ((*message_)[strings::msg_params].keyExists(strings::tts_chunks)) {
    const smart_objects::SmartArray* tc_array =
        (*message_)[strings::msg_params][strings::tts_chunks].asArray();

    smart_objects::SmartArray::const_iterator it_tc = tc_array->begin();
    smart_objects::SmartArray::const_iterator it_tc_end = tc_array->end();

    for (; it_tc != it_tc_end; ++it_tc) {
      str = (*it_tc)[strings::text].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_, "Invalid tts_chunks syntax check failed");
        return true;
      }
    }
  }
  return false;
}

}  // namespace commands

}  // namespace application_manager
