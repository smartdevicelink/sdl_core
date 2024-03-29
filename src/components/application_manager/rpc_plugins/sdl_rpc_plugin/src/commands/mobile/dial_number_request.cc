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

#include "sdl_rpc_plugin/commands/mobile/dial_number_request.h"
#include <algorithm>
#include <string>

#include "application_manager/application_impl.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

DialNumberRequest::DialNumberRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromMobileImpl(message,
                            application_manager,
                            rpc_service,
                            hmi_capabilities,
                            policy_handler) {}

DialNumberRequest::~DialNumberRequest() {}

bool DialNumberRequest::Init() {
  SDL_LOG_AUTO_TRACE();

  default_timeout_ = 0;

  return true;
}

void DialNumberRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (!application) {
    SDL_LOG_ERROR("NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  std::string number =
      (*message_)[strings::msg_params][strings::number].asString();
  if (!CheckSyntax(number)) {
    SDL_LOG_ERROR("Invalid incoming data");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  StripNumberParam(number);
  if (number.empty()) {
    SDL_LOG_ERROR("After strip number param is empty. Invalid incoming data");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::number] =
      (*message_)[strings::msg_params][strings::number].asString();
  msg_params[strings::app_id] = application->hmi_app_id();

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_BasicCommunication);
  SendHMIRequest(
      hmi_apis::FunctionID::BasicCommunication_DialNumber, &msg_params, true);
}

void DialNumberRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (!application) {
    SDL_LOG_ERROR("NULL pointer");
    return;
  }

  const smart_objects::SmartObject& message = event.smart_object();
  mobile_apis::Result::eType result_code = mobile_apis::Result::SUCCESS;
  switch (event.id()) {
    case hmi_apis::FunctionID::BasicCommunication_DialNumber: {
      SDL_LOG_INFO("Received DialNumber event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_BasicCommunication);
      result_code = RequestFromMobileImpl::GetMobileResultCode(
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt()));
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event " << event.id());
      return;
    }
  }

  const bool is_success = mobile_apis::Result::SUCCESS == result_code;
  const bool is_info_valid =
      message[strings::msg_params].keyExists(strings::info);

  if (is_info_valid) {
    const char* info_char_array =
        message[strings::msg_params][strings::info].asCharArray();
    SendResponse(is_success, result_code, info_char_array);
    return;
  }

  SendResponse(is_success, result_code);
}

void DialNumberRequest::StripNumberParam(std::string& number) {
  std::size_t found = 0;
  while (std::string::npos !=
         (found = number.find_first_not_of("0123456789*#,;+"))) {
    number.erase(number.begin() + found);
  }
  (*message_)[strings::msg_params][strings::number] = number;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
