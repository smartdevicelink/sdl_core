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
#include "sdl_rpc_plugin/commands/hmi/ui_get_capabilities_response.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

UIGetCapabilitiesResponse::UIGetCapabilitiesResponse(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : ResponseFromHMI(message,
                      application_manager,
                      rpc_service,
                      hmi_capabilities,
                      policy_handle) {}

UIGetCapabilitiesResponse::~UIGetCapabilitiesResponse() {}

void UIGetCapabilitiesResponse::Run() {
  SDL_LOG_AUTO_TRACE();

  const auto result_code = static_cast<hmi_apis::Common_Result::eType>(
      (*message_)[strings::params][hmi_response::code].asInt());

  if (hmi_apis::Common_Result::SUCCESS != result_code) {
    SDL_LOG_DEBUG("Request was not successful. Don't change HMI capabilities");
    hmi_capabilities_.UpdateRequestsRequiredForCapabilities(
        hmi_apis::FunctionID::UI_GetCapabilities);
    return;
  }

  std::vector<std::string> sections_to_update;

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  if (msg_params.keyExists(hmi_response::display_capabilities)) {
    sections_to_update.push_back(hmi_response::display_capabilities);
    hmi_capabilities_.set_display_capabilities(
        msg_params[hmi_response::display_capabilities]);
  }

  if (msg_params.keyExists(hmi_response::hmi_zone_capabilities)) {
    sections_to_update.push_back(hmi_response::hmi_zone_capabilities);
    hmi_capabilities_.set_hmi_zone_capabilities(
        msg_params[hmi_response::hmi_zone_capabilities]);
  }

  if (msg_params.keyExists(hmi_response::soft_button_capabilities)) {
    sections_to_update.push_back(hmi_response::soft_button_capabilities);
    hmi_capabilities_.set_soft_button_capabilities(
        msg_params[hmi_response::soft_button_capabilities]);
  }

  // use newer parameter "audioPassThruCapabilitiesList" when available
  if (msg_params.keyExists(strings::audio_pass_thru_capabilities_list)) {
    sections_to_update.push_back(strings::audio_pass_thru_capabilities);
    hmi_capabilities_.set_audio_pass_thru_capabilities(
        msg_params[strings::audio_pass_thru_capabilities_list]);
  } else if (msg_params.keyExists(strings::audio_pass_thru_capabilities)) {
    smart_objects::SmartObject audio_pass_thru_capabilities_list(
        smart_objects::SmartType_Array);
    audio_pass_thru_capabilities_list[0] =
        msg_params[strings::audio_pass_thru_capabilities];
    sections_to_update.push_back(strings::audio_pass_thru_capabilities);
    hmi_capabilities_.set_audio_pass_thru_capabilities(
        audio_pass_thru_capabilities_list);
  }

  if (msg_params.keyExists(strings::hmi_capabilities)) {
    if (msg_params[strings::hmi_capabilities].keyExists(strings::navigation)) {
      sections_to_update.push_back(strings::navigation);
      hmi_capabilities_.set_navigation_supported(
          msg_params[strings::hmi_capabilities][strings::navigation].asBool());
    }
    if (msg_params[strings::hmi_capabilities].keyExists(strings::phone_call)) {
      sections_to_update.push_back(strings::phone_call);
      hmi_capabilities_.set_phone_call_supported(
          msg_params[strings::hmi_capabilities][strings::phone_call].asBool());
    }
    if (msg_params[strings::hmi_capabilities].keyExists(
            strings::video_streaming)) {
      sections_to_update.push_back(strings::video_streaming);
      hmi_capabilities_.set_video_streaming_supported(
          msg_params[strings::hmi_capabilities][strings::video_streaming]
              .asBool());
    }
  }

  if (msg_params.keyExists(strings::system_capabilities)) {
    auto& system_capabilities_so = msg_params[strings::system_capabilities];

    if (system_capabilities_so.keyExists(strings::navigation_capability)) {
      sections_to_update.push_back(strings::navigation_capability);
      hmi_capabilities_.set_navigation_capability(
          system_capabilities_so[strings::navigation_capability]);
    }
    if (system_capabilities_so.keyExists(strings::phone_capability)) {
      sections_to_update.push_back(strings::phone_capability);
      hmi_capabilities_.set_phone_capability(
          system_capabilities_so[strings::phone_capability]);
    }
    if (system_capabilities_so.keyExists(strings::video_streaming_capability)) {
      sections_to_update.push_back(strings::video_streaming_capability);
      hmi_capabilities_.set_video_streaming_capability(
          system_capabilities_so[strings::video_streaming_capability]);
    }
    if (system_capabilities_so.keyExists(
            strings::driver_distraction_capability)) {
      if (!system_capabilities_so[strings::driver_distraction_capability]
               .empty()) {
        sections_to_update.push_back(strings::driver_distraction_capability);
        hmi_capabilities_.set_driver_distraction_capability(
            system_capabilities_so[strings::driver_distraction_capability]);
        hmi_capabilities_.set_driver_distraction_supported(true);
      }
    }
    if (system_capabilities_so.keyExists(strings::display_capabilities)) {
      sections_to_update.push_back(strings::display_capabilities);
      hmi_capabilities_.set_system_display_capabilities(
          system_capabilities_so[strings::display_capabilities]);
    }
  }

  if (msg_params.keyExists(strings::pcm_stream_capabilities)) {
    sections_to_update.push_back(strings::pcm_stream_capabilities);
    hmi_capabilities_.set_pcm_stream_capabilities(
        msg_params[strings::pcm_stream_capabilities]);
  }

  hmi_capabilities_.UpdateRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::UI_GetCapabilities);

  if (!hmi_capabilities_.SaveCachedCapabilitiesToFile(
          hmi_interface::ui, sections_to_update, message_->getSchema())) {
    SDL_LOG_ERROR("Failed to save UI.GetCapabilities response to cache");
  }
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
