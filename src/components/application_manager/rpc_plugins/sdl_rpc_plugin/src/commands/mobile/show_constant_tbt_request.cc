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

#include "sdl_rpc_plugin/commands/mobile/show_constant_tbt_request.h"
#include <cstring>

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

ShowConstantTBTRequest::ShowConstantTBTRequest(
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

ShowConstantTBTRequest::~ShowConstantTBTRequest() {}

void ShowConstantTBTRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (!app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    SDL_LOG_ERROR("Application is not registered");
    return;
  }
  // SDLAQ-CRS-664, VC3.1
  if ((*message_)[strings::msg_params].empty()) {
    SDL_LOG_ERROR("INVALID_DATA!");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  smart_objects::SmartObject msg_params = (*message_)[strings::msg_params];

  if (IsWhiteSpaceExist()) {
    SDL_LOG_ERROR("Incoming show constant TBT has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  // ProcessSoftButtons checks strings on the contents incorrect character

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons(
          msg_params, app, policy_handler_, application_manager_);

  if (mobile_apis::Result::SUCCESS != processing_result) {
    SDL_LOG_ERROR("INVALID_DATA!");
    SendResponse(false, processing_result);
    return;
  }

  mobile_apis::Result::eType verification_result = mobile_apis::Result::SUCCESS;
  if (msg_params.keyExists(strings::turn_icon)) {
    verification_result = MessageHelper::VerifyImage(
        msg_params[strings::turn_icon], app, application_manager_);
    if (mobile_apis::Result::INVALID_DATA == verification_result) {
      SDL_LOG_ERROR("VerifyImage INVALID_DATA!");
      SendResponse(false, verification_result);
      return;
    }
  }

  if (msg_params.keyExists(strings::next_turn_icon)) {
    verification_result = MessageHelper::VerifyImage(
        msg_params[strings::next_turn_icon], app, application_manager_);
    if (mobile_apis::Result::INVALID_DATA == verification_result) {
      SDL_LOG_ERROR("VerifyImage INVALID_DATA!");
      SendResponse(false, verification_result);
      return;
    }
  }

  msg_params[strings::app_id] = app->app_id();

  msg_params[hmi_request::navi_texts] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  int32_t index = 0;
  if (msg_params.keyExists(strings::navigation_text_1)) {
    // erase useless parametr
    msg_params.erase(strings::navigation_text_1);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::navigationText1);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::navigation_text_1];
    ++index;
  }

  if (msg_params.keyExists(strings::navigation_text_2)) {
    // erase useless param
    msg_params.erase(strings::navigation_text_2);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::navigationText2);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::navigation_text_2];
    ++index;
  }

  if (msg_params.keyExists(strings::eta)) {
    // erase useless param
    msg_params.erase(strings::eta);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::ETA);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::eta];
    ++index;
  }

  if (msg_params.keyExists(strings::total_distance)) {
    // erase useless param
    msg_params.erase(strings::total_distance);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::totalDistance);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::total_distance];
    ++index;
  }

  if (msg_params.keyExists(strings::time_to_destination)) {
    // erase useless param
    msg_params.erase(strings::time_to_destination);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::timeToDestination);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::time_to_destination];
    ++index;
  }

  if (msg_params.keyExists(strings::soft_buttons)) {
    MessageHelper::SubscribeApplicationToSoftButton(
        msg_params, app, function_id());
  }

  app->set_tbt_show_command(msg_params);
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
  SendHMIRequest(
      hmi_apis::FunctionID::Navigation_ShowConstantTBT, &msg_params, true);
}

void ShowConstantTBTRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  using namespace hmi_apis;
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_ShowConstantTBT: {
      SDL_LOG_INFO("Received Navigation_ShowConstantTBT event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
      const Common_Result::eType result_code =
          static_cast<Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      std::string response_info;
      GetInfo(message, response_info);
      const bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_Navigation);
      SendResponse(result,
                   MessageHelper::HMIToMobileResult(result_code),
                   response_info.empty() ? NULL : response_info.c_str(),
                   &(message[strings::msg_params]));
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event " << event.id());
      break;
    }
  }
}

bool ShowConstantTBTRequest::IsWhiteSpaceExist() {
  SDL_LOG_AUTO_TRACE();
  const char* str = NULL;

  if ((*message_)[strings::msg_params].keyExists(strings::turn_icon)) {
    str = (*message_)[strings::msg_params][strings::turn_icon][strings::value]
              .asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid turn_icon value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::next_turn_icon)) {
    str = (*message_)[strings::msg_params][strings::next_turn_icon]
                     [strings::value]
                         .asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid next_turn_icon value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::navigation_text_1)) {
    str = (*message_)[strings::msg_params][strings::navigation_text_1]
              .asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid navigation_text_1 value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::navigation_text_2)) {
    str = (*message_)[strings::msg_params][strings::navigation_text_2]
              .asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid navigation_text_2 value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::eta)) {
    str = (*message_)[strings::msg_params][strings::eta].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid eta value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::total_distance)) {
    str =
        (*message_)[strings::msg_params][strings::total_distance].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid total_distance value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(
          strings::time_to_destination)) {
    str = (*message_)[strings::msg_params][strings::time_to_destination]
              .asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid time_to_destination value syntax check failed");
      return true;
    }
  }
  return false;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
