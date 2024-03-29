/*

 Copyright (c) 2018, Ford Motor Company
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

#include "sdl_rpc_plugin/commands/mobile/slider_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "config_profile/profile.h"
#include "utils/helpers.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

SliderRequest::SliderRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromMobileImpl(message,
                            application_manager,
                            rpc_service,
                            hmi_capabilities,
                            policy_handler) {}

SliderRequest::~SliderRequest() {}

bool SliderRequest::Init() {
  /* Timeout in milliseconds.
     If omitted a standard value of 10000 milliseconds is used.*/
  if ((*message_)[strings::msg_params].keyExists(strings::timeout)) {
    default_timeout_ +=
        (*message_)[strings::msg_params][strings::timeout].asUInt();
  }

  return true;
}

void SliderRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr application = application_manager_.application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (!application) {
    SDL_LOG_ERROR("Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if ((*message_)[strings::msg_params][strings::num_ticks].asInt() <
      (*message_)[strings::msg_params][strings::position].asInt()) {
    SDL_LOG_ERROR("INVALID_DATA");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::slider_footer)) {
    if (1 < (*message_)[strings::msg_params][strings::slider_footer].length()) {
      if ((*message_)[strings::msg_params][strings::num_ticks].asUInt() !=
          (*message_)[strings::msg_params][strings::slider_footer].length()) {
        SDL_LOG_ERROR("INVALID_DATA");
        SendResponse(false, mobile_apis::Result::INVALID_DATA);
        return;
      }
    }
  }

  if (IsWhiteSpaceExist()) {
    SDL_LOG_ERROR("Incoming slider has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  smart_objects::SmartObject msg_params = (*message_)[strings::msg_params];
  msg_params[strings::app_id] = application->app_id();

  if (!(*message_)[strings::msg_params].keyExists(strings::timeout)) {
    msg_params[strings::timeout] = default_timeout_;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::cancel_id)) {
    msg_params[strings::cancel_id] =
        (*message_)[strings::msg_params][strings::cancel_id].asInt();
  }

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(hmi_apis::FunctionID::UI_Slider, &msg_params, true);
}

void SliderRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  using namespace helpers;
  using namespace smart_objects;
  using namespace hmi_apis;

  const SmartObject& message = event.smart_object();

  const event_engine::Event::EventID event_id = event.id();

  if (event_id != FunctionID::UI_Slider) {
    SDL_LOG_ERROR("Received unknown event " << event.id());
    return;
  }

  SDL_LOG_DEBUG("Received UI_Slider event");
  EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  const Common_Result::eType response_code = static_cast<Common_Result::eType>(
      message[strings::params][hmi_response::code].asInt());

  SmartObject response_msg_params = message[strings::msg_params];

  std::string response_info;
  GetInfo(message, response_info);
  const bool is_response_success = PrepareResultForMobileResponse(
      response_code, HmiInterfaces::HMI_INTERFACE_UI);

  SendResponse(is_response_success,
               MessageHelper::HMIToMobileResult(response_code),
               response_info.empty() ? NULL : response_info.c_str(),
               &response_msg_params);
}

bool SliderRequest::IsWhiteSpaceExist() {
  SDL_LOG_AUTO_TRACE();

  const char* str =
      (*message_)[strings::msg_params][strings::slider_header].asCharArray();
  if (!CheckSyntax(str)) {
    SDL_LOG_ERROR("Invalid slider_header value syntax check failed");
    return true;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::slider_footer)) {
    const smart_objects::SmartArray* sf_array =
        (*message_)[strings::msg_params][strings::slider_footer].asArray();

    smart_objects::SmartArray::const_iterator it_sf = sf_array->begin();
    smart_objects::SmartArray::const_iterator it_sf_end = sf_array->end();

    for (; it_sf != it_sf_end; ++it_sf) {
      str = (*it_sf).asCharArray();
      if (!CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid slider_footer syntax check failed");
        return true;
      }
    }
  }
  return false;
}

}  // namespace commands
}  // namespace sdl_rpc_plugin
