/*
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/commands/hmi/navi_audio_start_stream_request.h"
#include "application_manager/application_manager_impl.h"
#include "protocol_handler/protocol_handler.h"
#include "config_profile/profile.h"

namespace application_manager {

namespace commands {

AudioStartStreamRequest::AudioStartStreamRequest(
    const MessageSharedPtr& message)
    : RequestToHMI(message),
    retry_number_(0) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::pair<uint32_t, int32_t> stream_retry =
      profile::Profile::instance()->start_stream_retry_amount();
  default_timeout_ = stream_retry.second;
  retry_number_ = stream_retry.first;
  LOG4CXX_DEBUG(logger_, "default_timeout_ = " << default_timeout_
                <<"; retry_number_ = " << retry_number_);
}

AudioStartStreamRequest::~AudioStartStreamRequest() {
}

void AudioStartStreamRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  SetAllowedToTerminate(false);
  subscribe_on_event(hmi_apis::FunctionID::Navigation_StartAudioStream,
                     correlation_id());

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->
      application_by_hmi_app(application_id());
  if (app) {
    app->set_audio_streaming_allowed(true);
    SendRequest();
  } else {
    LOG4CXX_ERROR(logger_, "Applcation with hmi_app_id "
                  << application_id() << " does not exist");
  }
}

void AudioStartStreamRequest::on_event(const event_engine::Event& event) {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->
      application_by_hmi_app(application_id());
  if (!app) {
    LOG4CXX_ERROR(logger_,
        "StartAudioStreamRequest aborted. Application not found");
    return;
  }

  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_StartAudioStream: {
      LOG4CXX_DEBUG(logger_, "Received StartStream event");

      const hmi_apis::Common_Result::eType code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());

      if (hmi_apis::Common_Result::SUCCESS == code) {
        LOG4CXX_INFO(logger_, "StartAudioStream response SUCCESS");
        if (ApplicationManagerImpl::instance()->
                HMILevelAllowsStreaming(app->app_id(), ServiceType::kAudio)) {
          app->set_audio_streaming_approved(true);
        } else {
          LOG4CXX_WARN(logger_,
                       "StartAudioStreamRequest aborted. Application can not stream");
        }
        break;
      }
      if (hmi_apis::Common_Result::REJECTED == code) {
        LOG4CXX_INFO(logger_, "StartAudioStream response REJECTED");
        RetryStartSession();
        break;
      }
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      return;
    }
  }
}

void AudioStartStreamRequest::onTimeOut() {
  RetryStartSession();

  ApplicationManagerImpl::instance()->TerminateRequest(
      connection_key(), correlation_id());
}

void AudioStartStreamRequest::RetryStartSession() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->
      application_by_hmi_app(application_id());
  if (!app) {
    LOG4CXX_ERROR(logger_,
        "StartAudioStreamRequest aborted. Application not found");
    return;
  }

  if (!app->audio_streaming_allowed()) {
    LOG4CXX_WARN(logger_, "Audio streaming not allowed");
    return;
  }

  if (app->audio_streaming_approved()) {
    LOG4CXX_INFO(logger_, "AudioStartStream retry sequence stopped. "
                 << "SUCCESS received");
    app->set_audio_stream_retry_number(0);
    return;
  }

  uint32_t curr_retry_number =  app->audio_stream_retry_number();
  if (curr_retry_number < retry_number_ - 1) {
    LOG4CXX_DEBUG(logger_, "Send AudioStartStream retry. retry_number = "
                 << curr_retry_number);
    MessageHelper::SendAudioStartStream(app->app_id());
    app->set_audio_stream_retry_number(++curr_retry_number);
  } else {
    LOG4CXX_DEBUG(logger_, "Audio start stream retry sequence stopped. "
                 << "Attempts expired.");

    ApplicationManagerImpl::instance()->EndNaviServices(app->app_id());
  }
}

}  // namespace commands

}  // namespace application_manager
