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
 * Neither the names of the copyright holders nor the names of its contributors
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

#include "sdl_rpc_plugin/commands/hmi/navi_set_video_config_request.h"
#include <string>
#include <vector>

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

NaviSetVideoConfigRequest::NaviSetVideoConfigRequest(
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
    , EventObserver(application_manager.event_dispatcher()) {}

NaviSetVideoConfigRequest::~NaviSetVideoConfigRequest() {}

void NaviSetVideoConfigRequest::Run() {
  SDL_LOG_AUTO_TRACE();
  if (!app_mngr::commands::CheckAvailabilityHMIInterfaces(
          application_manager_, HmiInterfaces::HMI_INTERFACE_Navigation)) {
    SDL_LOG_WARN("HMI interface Navigation is not supported");
    return;
  }

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());
  if (!app) {
    SDL_LOG_ERROR("Application with hmi_app_id " << application_id()
                                                 << " does not exist");
    return;
  }

  subscribe_on_event(hmi_apis::FunctionID::Navigation_SetVideoConfig,
                     correlation_id());
  SendRequest();
}

void NaviSetVideoConfigRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());
  if (!app) {
    SDL_LOG_ERROR("Application is not found, abort NaviSetVideoConfigRequest");
    return;
  }

  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_SetVideoConfig: {
      const hmi_apis::Common_Result::eType code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());

      if (code == hmi_apis::Common_Result::SUCCESS) {
        SDL_LOG_DEBUG("Received SetVideoConfig success response");
        application_manager_.OnStreamingConfigurationSuccessful(
            app->app_id(), protocol_handler::ServiceType::kMobileNav);
      } else {
        SDL_LOG_DEBUG("Received SetVideoConfig failure response (" << event.id()
                                                                   << ")");
        std::vector<std::string> rejected_params;
        if (message[strings::msg_params].keyExists(strings::rejected_params)) {
          const smart_objects::SmartArray* list =
              message[strings::msg_params][strings::rejected_params].asArray();
          if (list != NULL) {
            for (unsigned int i = 0; i < list->size(); ++i) {
              const std::string& param = (*list)[i].asString();
              // Make sure that we actually sent the parameter in the request
              if ((*message_)[strings::msg_params].keyExists(strings::config) &&
                  (*message_)[strings::msg_params][strings::config].keyExists(
                      param)) {
                rejected_params.push_back(param);
              }
            }
          }
        }

        application_manager_.OnStreamingConfigurationFailed(
            app->app_id(),
            rejected_params,
            "Received SetVideoConfig failure response");

        break;
      }
    }
    default:
      SDL_LOG_ERROR("Received unknown event " << event.id());
      break;
  }
}

void NaviSetVideoConfigRequest::onTimeOut() {
  SDL_LOG_WARN("Timed out while waiting for SetVideoConfig response");

  ApplicationSharedPtr app =
      application_manager_.application_by_hmi_app(application_id());
  if (!app) {
    SDL_LOG_ERROR("Application is not found");
    return;
  }

  std::vector<std::string> empty;
  application_manager_.OnStreamingConfigurationFailed(
      app->app_id(),
      empty,
      "Timed out while waiting for SetVideoConfig response");

  application_manager_.TerminateRequest(
      connection_key(), correlation_id(), function_id());
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
