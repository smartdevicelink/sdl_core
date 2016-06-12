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

#include "application_manager/commands/hmi/navi_start_stream_request.h"
#include "application_manager/message_helper.h"
#include "protocol_handler/protocol_handler.h"

namespace application_manager {

namespace commands {

NaviStartStreamRequest::NaviStartStreamRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : RequestToHMI(message, application_manager)
    , EventObserver(application_manager.event_dispatcher())
    , retry_number_(0) {
  LOGGER_AUTO_TRACE(logger_);
  std::pair<uint32_t, int32_t> stream_retry =
      application_manager_.get_settings().start_stream_retry_amount();
  default_timeout_ = stream_retry.second;
  retry_number_ = stream_retry.first;
  LOGGER_DEBUG(logger_,
               "default_timeout_ = " << default_timeout_
                                     << "; retry_number_ = " << retry_number_);
}

NaviStartStreamRequest::~NaviStartStreamRequest() {}

void NaviStartStreamRequest::Run() {
  LOGGER_AUTO_TRACE(logger_);

  SetAllowedToTerminate(false);
  subscribe_on_event(hmi_apis::FunctionID::Navigation_StartStream,
                     correlation_id());

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());
  if (app) {
    app->set_video_streaming_allowed(true);
    SendRequest();
  } else {
    LOGGER_ERROR(logger_,
                 "Applcation with hmi_app_id " << application_id()
                                               << "does not exist");
  }
}

void NaviStartStreamRequest::on_event(const event_engine::Event& event) {
  using namespace protocol_handler;
  LOGGER_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());
  if (!app) {
    LOGGER_ERROR(logger_,
                 "NaviStartStreamRequest aborted. Application not found");
    return;
  }

  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_StartStream: {
      LOGGER_DEBUG(logger_, "Received StartStream event");

      const hmi_apis::Common_Result::eType code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());

      if (hmi_apis::Common_Result::SUCCESS == code) {
        LOGGER_INFO(logger_, "NaviStartStreamResponse SUCCESS");
        if (application_manager_.HMILevelAllowsStreaming(
                app->app_id(), ServiceType::kMobileNav)) {
          app->set_video_streaming_approved(true);
        } else {
          LOGGER_DEBUG(
              logger_,
              "NaviStartStreamRequest aborted. Application can not stream");
        }
        break;
      }
      if (hmi_apis::Common_Result::REJECTED == code) {
        LOGGER_INFO(logger_, "StartStream response REJECTED ");
        RetryStartSession();
        application_manager_.TerminateRequest(connection_key(),
                                              correlation_id());
        break;
      }
    }
    default: {
      LOGGER_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }
}

void NaviStartStreamRequest::onTimeOut() {
  RetryStartSession();
  application_manager_.TerminateRequest(connection_key(), correlation_id());
}

void NaviStartStreamRequest::RetryStartSession() {
  LOGGER_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());
  if (!app) {
    LOGGER_ERROR(logger_,
                 "NaviStartStreamRequest aborted. Application not found");
    return;
  }

  if (!app->video_streaming_allowed()) {
    LOGGER_WARN(logger_, "Video streaming not allowed");
    return;
  }

  if (app->video_streaming_approved()) {
    LOGGER_DEBUG(logger_,
                 "NaviStartStream retry sequence stopped. "
                     << "Video streaming is approved");
    app->set_video_stream_retry_number(0);
    return;
  }

  uint32_t curr_retry_number = app->video_stream_retry_number();
  if (curr_retry_number < retry_number_ - 1) {
    LOGGER_DEBUG(
        logger_,
        "Send NaviStartStream retry. retry_number = " << curr_retry_number);
    MessageHelper::SendNaviStartStream(app->app_id(), application_manager_);
    app->set_video_stream_retry_number(++curr_retry_number);
  } else {
    LOGGER_DEBUG(logger_,
                 "NaviStartStream retry sequence stopped. "
                     << "Attempts expired");
    application_manager_.EndNaviServices(app->app_id());
  }
}

}  // namespace commands

}  // namespace application_manager
