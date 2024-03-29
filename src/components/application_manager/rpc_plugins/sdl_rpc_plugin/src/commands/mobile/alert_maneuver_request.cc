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

#include "sdl_rpc_plugin/commands/mobile/alert_maneuver_request.h"

#include <cstring>
#include <string>

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

AlertManeuverRequest::AlertManeuverRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromMobileImpl(message,
                            application_manager,
                            rpc_service,
                            hmi_capabilities,
                            policy_handler)
    , tts_speak_result_code_(hmi_apis::Common_Result::INVALID_ENUM)
    , navi_alert_maneuver_result_code_(hmi_apis::Common_Result::INVALID_ENUM) {}

AlertManeuverRequest::~AlertManeuverRequest() {}

void AlertManeuverRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  if ((!(*message_)[strings::msg_params].keyExists(strings::soft_buttons)) &&
      (!(*message_)[strings::msg_params].keyExists(strings::tts_chunks))) {
    SDL_LOG_ERROR("AlertManeuverRequest::Request without parameters!");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  ApplicationSharedPtr app = application_manager_.application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (NULL == app.get()) {
    SDL_LOG_ERROR("Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (IsWhiteSpaceExist()) {
    SDL_LOG_ERROR(
        "Incoming alert maneuver has contains \\t\\n \\\\t \\\\n"
        "text contains only whitespace in ttsChunks");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  // ProcessSoftButtons checks strings on the contents incorrect character

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons((*message_)[strings::msg_params],
                                        app,
                                        policy_handler_,
                                        application_manager_);

  if (mobile_apis::Result::SUCCESS != processing_result) {
    SDL_LOG_ERROR("Wrong soft buttons parameters!");
    SendResponse(false, processing_result);
    return;
  }

  // Checking parameters and how many HMI requests should be sent
  bool tts_is_ok = false;

  // check TTSChunk parameter
  if ((*message_)[strings::msg_params].keyExists(strings::tts_chunks)) {
    smart_objects::SmartObject& tts_chunks =
        (*message_)[strings::msg_params][strings::tts_chunks];
    mobile_apis::Result::eType verification_result =
        MessageHelper::VerifyTtsFiles(tts_chunks, app, application_manager_);

    if (mobile_apis::Result::FILE_NOT_FOUND == verification_result) {
      SDL_LOG_ERROR("MessageHelper::VerifyTtsFiles return "
                    << verification_result);
      SendResponse(false,
                   mobile_apis::Result::FILE_NOT_FOUND,
                   "One or more files needed for tts_chunks are not present");
      return;
    }

    if (0 < (*message_)[strings::msg_params][strings::tts_chunks].length()) {
      pending_requests_.Add(hmi_apis::FunctionID::TTS_Speak);
      tts_is_ok = true;
    }
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::app_id] = app->app_id();

  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[hmi_request::soft_buttons] =
        (*message_)[strings::msg_params][strings::soft_buttons];
    MessageHelper::SubscribeApplicationToSoftButton(
        (*message_)[strings::msg_params], app, function_id());
  }

  pending_requests_.Add(hmi_apis::FunctionID::Navigation_AlertManeuver);
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
  SendHMIRequest(
      hmi_apis::FunctionID::Navigation_AlertManeuver, &msg_params, true);

  if (tts_is_ok) {
    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    msg_params[strings::app_id] = app->app_id();
    msg_params[hmi_request::tts_chunks] =
        (*message_)[strings::msg_params][strings::tts_chunks];
    msg_params[hmi_request::speak_type] =
        hmi_apis::Common_MethodName::ALERT_MANEUVER;

    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
    SendHMIRequest(hmi_apis::FunctionID::TTS_Speak, &msg_params, true);
  }
}

void AlertManeuverRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();
  hmi_apis::FunctionID::eType event_id = event.id();
  switch (event_id) {
    case hmi_apis::FunctionID::Navigation_AlertManeuver: {
      SDL_LOG_INFO("Received Navigation_AlertManeuver event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
      pending_requests_.Remove(event_id);
      navi_alert_maneuver_result_code_ =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      GetInfo(message, info_navi_);
      break;
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      SDL_LOG_INFO("Received TTS_Speak event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);

      pending_requests_.Remove(event_id);
      tts_speak_result_code_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, info_tts_);
      break;
    }

    default: {
      SDL_LOG_ERROR("Received unknown event " << event.id());
      SendResponse(
          false, mobile_apis::Result::INVALID_ENUM, "Received unknown event ");
      return;
    }
  }

  if (IsPendingResponseExist()) {
    SDL_LOG_DEBUG("Command still wating for HMI response");
    return;
  }

  std::string return_info;
  mobile_apis::Result::eType result_code;
  const bool result = PrepareResponseParameters(result_code, return_info);
  bool must_be_empty_info = false;
  if (return_info.find("\n") != std::string::npos ||
      return_info.find("\t") != std::string::npos) {
    must_be_empty_info = true;
  }
  SendResponse(result,
               result_code,
               (must_be_empty_info) ? NULL : return_info.c_str(),
               &(message[strings::msg_params]));
}

bool AlertManeuverRequest::PrepareResponseParameters(
    mobile_apis::Result::eType& result_code, std::string& return_info) {
  SDL_LOG_AUTO_TRACE();
  using namespace helpers;

  app_mngr::commands::ResponseInfo navigation_alert_info(
      navi_alert_maneuver_result_code_,
      HmiInterfaces::HMI_INTERFACE_Navigation,
      application_manager_);

  app_mngr::commands::ResponseInfo tts_alert_info(
      tts_speak_result_code_,
      HmiInterfaces::HMI_INTERFACE_TTS,
      application_manager_);
  const bool result =
      PrepareResultForMobileResponse(navigation_alert_info, tts_alert_info);

  if (result && (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE ==
                     tts_speak_result_code_ &&
                 (HmiInterfaces::STATE_AVAILABLE ==
                  application_manager_.hmi_interfaces().GetInterfaceState(
                      HmiInterfaces::HMI_INTERFACE_TTS)))) {
    result_code = mobile_apis::Result::WARNINGS;
    return_info = app_mngr::commands::MergeInfos(
        navigation_alert_info, info_navi_, tts_alert_info, info_tts_);
    return result;
  }
  result_code =
      PrepareResultCodeForResponse(navigation_alert_info, tts_alert_info);
  return_info = app_mngr::commands::MergeInfos(
      navigation_alert_info, info_navi_, tts_alert_info, info_tts_);
  return result;
}

bool AlertManeuverRequest::IsWhiteSpaceExist() {
  SDL_LOG_AUTO_TRACE();
  using smart_objects::SmartArray;

  if ((*message_)[strings::msg_params].keyExists(strings::tts_chunks)) {
    const SmartArray* tts_chunks_arr =
        (*message_)[strings::msg_params][strings::tts_chunks].asArray();

    SmartArray::const_iterator it_tts_chunk = tts_chunks_arr->begin();

    for (; it_tts_chunk != tts_chunks_arr->end(); ++it_tts_chunk) {
      const char* tts_chunk_text = (*it_tts_chunk)[strings::text].asCharArray();
      if (strlen(tts_chunk_text) && !CheckSyntax(tts_chunk_text)) {
        SDL_LOG_ERROR("Invalid tts_chunks syntax check failed");
        return true;
      }
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    DCHECK_OR_RETURN(
        (*message_)[strings::msg_params][strings::soft_buttons].getType() ==
            smart_objects::SmartType_Array,
        true);
    const smart_objects::SmartArray* soft_button_array =
        (*message_)[strings::msg_params][strings::soft_buttons].asArray();

    SmartArray::const_iterator it_soft_button = soft_button_array->begin();

    for (; it_soft_button != soft_button_array->end(); ++it_soft_button) {
      const char* soft_button_text =
          (*it_soft_button)[strings::text].asCharArray();
      if (!CheckSyntax(soft_button_text)) {
        SDL_LOG_ERROR("Invalid soft_buttons syntax check failed");
        return true;
      }
    }
  }

  return false;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
