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

#include "sdl_rpc_plugin/commands/hmi/tts_is_ready_request.h"
#include "application_manager/rpc_service.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

TTSIsReadyRequest::TTSIsReadyRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestToHMI(message,
                   application_manager,
                   rpc_service,
                   hmi_capabilities,
                   policy_handler)
    , EventObserver(application_manager.event_dispatcher()) {}

TTSIsReadyRequest::~TTSIsReadyRequest() {}

void TTSIsReadyRequest::Run() {
  SDL_LOG_AUTO_TRACE();
  subscribe_on_event(hmi_apis::FunctionID::TTS_IsReady, correlation_id());
  SendRequest();
}

void TTSIsReadyRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();

  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::TTS_IsReady: {
      SDL_LOG_DEBUG("Received TTS_IsReady event");
      unsubscribe_from_event(hmi_apis::FunctionID::TTS_IsReady);
      const bool is_available = app_mngr::commands::ChangeInterfaceState(
          application_manager_, message, HmiInterfaces::HMI_INTERFACE_TTS);
      HMICapabilities& hmi_capabilities = hmi_capabilities_;
      hmi_capabilities.set_is_tts_cooperating(is_available);
      if (!app_mngr::commands::CheckAvailabilityHMIInterfaces(
              application_manager_, HmiInterfaces::HMI_INTERFACE_TTS)) {
        UpdateRequiredInterfaceCapabilitiesRequests(hmi_interface::tts);
        SDL_LOG_INFO("HmiInterfaces::HMI_INTERFACE_TTS isn't available");
        return;
      }

      RequestInterfaceCapabilities(hmi_interface::tts);
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event " << event.id());
      return;
    }
  }
}

void TTSIsReadyRequest::onTimeOut() {
  // Note(dtrunov): According to new requirment  APPLINK-27956
  RequestInterfaceCapabilities(hmi_interface::tts);
}
}  // namespace commands

}  // namespace sdl_rpc_plugin
