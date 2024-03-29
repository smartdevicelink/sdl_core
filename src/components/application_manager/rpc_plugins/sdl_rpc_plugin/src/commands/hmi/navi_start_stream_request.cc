/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "sdl_rpc_plugin/commands/hmi/navi_start_stream_request.h"

#include "application_manager/message_helper.h"
#include "protocol_handler/protocol_handler.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

NaviStartStreamRequest::NaviStartStreamRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : RequestToHMI(message,
                   application_manager,
                   rpc_service,
                   hmi_capabilities,
                   policy_handle)
    , retry_number_(0) {
  SDL_LOG_AUTO_TRACE();
  std::pair<uint32_t, int32_t> stream_retry =
      application_manager_.get_settings().start_stream_retry_amount();
  default_timeout_ =
      stream_retry.second +
      application_manager_.get_settings().default_timeout_compensation();
  retry_number_ = stream_retry.first;
  SDL_LOG_DEBUG("default_timeout_ = " << stream_retry.second
                                      << "; retry_number_ = " << retry_number_);
}

NaviStartStreamRequest::~NaviStartStreamRequest() {
  // unsubscribe_from_all_hmi_events() in EventObserver's destructor isn't
  // enough; we must unsubscribe before this NaviStartStreamRequest instance is
  // removed
  unsubscribe_from_event(hmi_apis::FunctionID::Navigation_StartStream);
}

void NaviStartStreamRequest::Run() {
  SDL_LOG_AUTO_TRACE();
  if (!app_mngr::commands::CheckAvailabilityHMIInterfaces(
          application_manager_, HmiInterfaces::HMI_INTERFACE_Navigation)) {
    SDL_LOG_INFO("Interface Navi is not supported by system");
    return;
  }
  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());
  if (!app) {
    SDL_LOG_ERROR("Application with hmi_app_id " << application_id()
                                                 << " does not exist");
    return;
  }
  SetAllowedToTerminate(false);
  subscribe_on_event(hmi_apis::FunctionID::Navigation_StartStream,
                     correlation_id());
  app->set_video_streaming_allowed(true);
  SendRequest();
}

void NaviStartStreamRequest::on_event(const event_engine::Event& event) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());
  if (!app) {
    SDL_LOG_ERROR("NaviStartStreamRequest aborted. Application not found");
    return;
  }

  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_StartStream: {
      SDL_LOG_DEBUG("Received StartStream event");

      const hmi_apis::Common_Result::eType code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());

      if (hmi_apis::Common_Result::SUCCESS == code) {
        SDL_LOG_INFO("NaviStartStreamResponse SUCCESS");
        if (application_manager_.HMIStateAllowsStreaming(
                app->app_id(), ServiceType::kMobileNav)) {
          app->set_video_streaming_approved(true);
        } else {
          SDL_LOG_DEBUG(
              "NaviStartStreamRequest aborted. Application can not stream");
        }
        application_manager_.TerminateRequest(
            connection_key(), correlation_id(), function_id());
        break;
      }
      if (hmi_apis::Common_Result::REJECTED == code) {
        SDL_LOG_INFO("StartStream response REJECTED");
        RetryStartSession();
        break;
      }
    }
    default: {
      SDL_LOG_ERROR("Received unknown event " << event.id());
      return;
    }
  }
}

void NaviStartStreamRequest::OnTimeOut() {
  SDL_LOG_AUTO_TRACE();
  RetryStartSession();
}

void NaviStartStreamRequest::RetryStartSession() {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  auto retry_start_session = [this](const uint32_t hmi_app_id) {
    ApplicationSharedPtr app =
        application_manager_.application_by_hmi_app(hmi_app_id);

    if (!app) {
      SDL_LOG_ERROR("NaviStartStreamRequest aborted. Application not found");
      return;
    }

    if (!app->video_streaming_allowed()) {
      SDL_LOG_WARN("Video streaming not allowed");
      return;
    }

    if (app->video_streaming_approved()) {
      SDL_LOG_INFO("NaviStartStream retry sequence stopped. "
                   << "SUCCESS received");
      app->set_video_stream_retry_number(0);
      return;
    }

    uint32_t curr_retry_number = app->video_stream_retry_number() + 1;

    if (curr_retry_number <= retry_number_) {
      SDL_LOG_DEBUG("Retry number " << curr_retry_number << " of "
                                    << retry_number_);
      MessageHelper::SendNaviStartStream(app->app_id(), application_manager_);
      app->set_video_stream_retry_number(curr_retry_number);
    } else {
      SDL_LOG_DEBUG("NaviStartStream retry sequence stopped. "
                    << "Attempts expired");

      application_manager_.EndService(app->app_id(), ServiceType::kMobileNav);
    }
  };

  retry_start_session(application_id());

  application_manager_.TerminateRequest(
      connection_key(), correlation_id(), function_id());
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
