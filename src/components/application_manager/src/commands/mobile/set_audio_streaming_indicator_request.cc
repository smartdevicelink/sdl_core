/*
 Copyright (c) 2017, Ford Motor Company
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

#include "application_manager/commands/mobile/set_audio_streaming_indicator_request.h"
#include "application_manager/application.h"
#include "application_manager/message_helper.h"

namespace application_manager {
namespace commands {

SetAudioStreamingIndicatorRequest::SetAudioStreamingIndicatorRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager)
    , audio_streaming_indicator_(
          mobile_apis::AudioStreamingIndicator::INVALID_ENUM) {}

void SetAudioStreamingIndicatorRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());
  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (!app->is_media_application() || app->is_navi() ||
      app->is_voice_communication_supported()) {
    LOG4CXX_DEBUG(logger_, "Application has incorrect type");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  audio_streaming_indicator_ =
      static_cast<mobile_apis::AudioStreamingIndicator::eType>(
          (*message_)[strings::msg_params][strings::audio_streaming_indicator]
              .asInt());
  if (!(app->AddIndicatorWaitForResponse(audio_streaming_indicator_))) {
    LOG4CXX_DEBUG(
        logger_,
        "Application is subscribed or waiting for response for this indicator");
    SendResponse(false, mobile_apis::Result::IGNORED);
    return;
  }

  const smart_objects::SmartObject msg_params((*message_)[strings::msg_params]);

  SendHMIRequest(
      hmi_apis::FunctionID::UI_SetAudioStreamingIndicator, &msg_params, true);
}

void SetAudioStreamingIndicatorRequest::onTimeOut() {
  ApplicationSharedPtr app = application_manager_.application(connection_key());
  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
  } else {
    app->RemoveIndicatorWaitForResponse(audio_streaming_indicator_);
  }
  CommandRequestImpl::onTimeOut();
}

void SetAudioStreamingIndicatorRequest::on_event(
    const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_SetAudioStreamingIndicator: {
      LOG4CXX_INFO(logger_, "Received UI_SetAudioStreamingIndicator event");
      ProcessResultFromHMI(event.smart_object());
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      break;
    }
  }
}

void SetAudioStreamingIndicatorRequest::ProcessResultFromHMI(
    const smart_objects::SmartObject& message) {
  const hmi_apis::Common_Result::eType result_code =
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
  std::string response_info;
  GetInfo(message, response_info);
  const bool result = PrepareResultForMobileResponse(
      result_code, HmiInterfaces::HMI_INTERFACE_UI);
  ApplicationSharedPtr app = application_manager_.application(connection_key());
  if (app) {
    if (result) {
      app->set_audio_streaming_indicator(audio_streaming_indicator_);
    }
    app->RemoveIndicatorWaitForResponse(audio_streaming_indicator_);
  } else {
    LOG4CXX_ERROR(logger_, "Application doesn't exist");
  }

  SendResponse(result,
               MessageHelper::HMIToMobileResult(result_code),
               response_info.empty() ? NULL : response_info.c_str(),
               NULL);
}

}  // namespace commands
}  // namespace application_manager
