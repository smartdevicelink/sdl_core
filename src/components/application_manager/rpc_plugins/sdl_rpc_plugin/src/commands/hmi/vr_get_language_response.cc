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
#include "sdl_rpc_plugin/commands/hmi/vr_get_language_response.h"

#include "application_manager/event_engine/event.h"
#include "interfaces/HMI_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

VRGetLanguageResponse::VRGetLanguageResponse(
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

VRGetLanguageResponse::~VRGetLanguageResponse() {}

void VRGetLanguageResponse::Run() {
  SDL_LOG_AUTO_TRACE();
  using namespace hmi_apis;
  const Common_Result::eType result_code = static_cast<Common_Result::eType>(
      (*message_)[strings::params][hmi_response::code].asInt());

  if (Common_Result::SUCCESS != result_code) {
    SDL_LOG_DEBUG("Request was not successful. Don't change HMI capabilities");
    hmi_capabilities_.UpdateRequestsRequiredForCapabilities(
        hmi_apis::FunctionID::VR_GetLanguage);
    return;
  }

  Common_Language::eType language = Common_Language::INVALID_ENUM;

  if ((*message_).keyExists(strings::msg_params) &&
      (*message_)[strings::msg_params].keyExists(hmi_response::language)) {
    language = static_cast<Common_Language::eType>(
        (*message_)[strings::msg_params][hmi_response::language].asInt());
  }

  hmi_capabilities_.set_active_vr_language(language);

  hmi_capabilities_.UpdateRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::VR_GetLanguage);

  std::vector<std::string> sections_to_update{hmi_response::language};
  if (!hmi_capabilities_.SaveCachedCapabilitiesToFile(
          hmi_interface::vr, sections_to_update, message_->getSchema())) {
    SDL_LOG_ERROR("Failed to save VR.GetLanguage response to cache");
  }

  SDL_LOG_DEBUG("Raising event for function_id "
                << function_id() << " and correlation_id " << correlation_id());
  event_engine::Event event(FunctionID::VR_GetLanguage);
  event.set_smart_object(*message_);
  event.raise(application_manager_.event_dispatcher());
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
