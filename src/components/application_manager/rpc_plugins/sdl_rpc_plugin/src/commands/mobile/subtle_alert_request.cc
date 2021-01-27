/*
 Copyright (c) 2020, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

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

#include "sdl_rpc_plugin/commands/mobile/subtle_alert_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

SubtleAlertRequest::SubtleAlertRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler)
    , awaiting_ui_subtle_alert_response_(false)
    , awaiting_tts_speak_response_(false)
    , awaiting_tts_stop_speaking_response_(false)
    , is_ui_subtle_alert_sent_(false)
    , is_tts_stop_speaking_sent_(false)
    , subtle_alert_result_(hmi_apis::Common_Result::INVALID_ENUM)
    , tts_speak_result_(hmi_apis::Common_Result::INVALID_ENUM) {
  subscribe_on_event(hmi_apis::FunctionID::UI_OnResetTimeout);
  subscribe_on_event(hmi_apis::FunctionID::TTS_OnResetTimeout);
}

SubtleAlertRequest::~SubtleAlertRequest() {}

bool SubtleAlertRequest::Init() {
  /* Timeout in milliseconds.
    If omitted a standard value of 10000 milliseconds is used.*/
  auto& msg_params = (*message_)[strings::msg_params];
  uint32_t duration_timeout = msg_params[strings::duration].asUInt();

  default_timeout_ += duration_timeout;

  // If soft buttons are present, SDL will not use initiate timeout tracking for
  // response.
  if (msg_params.keyExists(strings::soft_buttons)) {
    SDL_LOG_INFO(
        "Request contains soft buttons - request timeout "
        "will be set to 0.");
    default_timeout_ = 0;
  }

  return true;
}

void SubtleAlertRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  uint32_t app_id =
      (*message_)[strings::params][strings::connection_key].asInt();

  if (!Validate(app_id)) {
    // Invalid command, abort execution
    return;
  }
  bool tts_chunks_exists =
      (*message_)[strings::msg_params].keyExists(strings::tts_chunks);
  size_t length_tts_chunks = 0;

  if (tts_chunks_exists) {
    length_tts_chunks =
        (*message_)[strings::msg_params][strings::tts_chunks].length();
  }

  awaiting_tts_speak_response_ = (tts_chunks_exists && length_tts_chunks);

  SendSubtleAlertRequest(app_id);
  if (awaiting_tts_speak_response_) {
    SendSpeakRequest(app_id, tts_chunks_exists, length_tts_chunks);
  }
}

void SubtleAlertRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::TTS_OnResetTimeout:
    case hmi_apis::FunctionID::UI_OnResetTimeout: {
      SDL_LOG_INFO(
          "Received UI_OnResetTimeout event "
          " or TTS_OnResetTimeout event"
          << awaiting_tts_speak_response_ << " "
          << awaiting_tts_stop_speaking_response_ << " "
          << awaiting_ui_subtle_alert_response_);
      application_manager_.updateRequestTimeout(
          connection_key(), correlation_id(), default_timeout());
      break;
    }
    case hmi_apis::FunctionID::UI_SubtleAlert: {
      SDL_LOG_INFO("Received UI_SubtleAlert event");
      // Unsubscribe from event to avoid unwanted messages
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      unsubscribe_from_event(hmi_apis::FunctionID::UI_SubtleAlert);
      awaiting_ui_subtle_alert_response_ = false;
      HmiInterfaces::InterfaceState ui_interface_state =
          application_manager_.hmi_interfaces().GetInterfaceState(
              HmiInterfaces::HMI_INTERFACE_UI);

      if (awaiting_tts_speak_response_ &&
          HmiInterfaces::STATE_NOT_AVAILABLE != ui_interface_state) {
        awaiting_tts_stop_speaking_response_ = true;
        StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
        SendHMIRequest(hmi_apis::FunctionID::TTS_StopSpeaking, NULL, true);
        is_tts_stop_speaking_sent_ = true;
      }
      subtle_alert_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());

      // Mobile Alert request is successful when UI_SubtleAlert is successful
      subtle_alert_response_params_ = message[strings::msg_params];
      GetInfo(message, ui_response_info_);
      break;
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      SDL_LOG_INFO("Received TTS_Speak event");
      // Unsubscribe from event to avoid unwanted messages
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
      unsubscribe_from_event(hmi_apis::FunctionID::TTS_Speak);
      awaiting_tts_speak_response_ = false;
      tts_speak_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, tts_response_info_);
      break;
    }
    case hmi_apis::FunctionID::TTS_StopSpeaking: {
      SDL_LOG_INFO("Received TTS_StopSpeaking event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
      // Unsubscribe from event to avoid unwanted messages
      unsubscribe_from_event(hmi_apis::FunctionID::TTS_StopSpeaking);
      awaiting_tts_stop_speaking_response_ = false;
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event" << event.id());
      return;
    }
  }

  if (HasHmiResponsesToWait()) {
    return;
  }
  mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;
  std::string info;
  const bool result = PrepareResponseParameters(result_code, info);
  SendResponse(result,
               result_code,
               info.empty() ? NULL : info.c_str(),
               &subtle_alert_response_params_);
}

bool SubtleAlertRequest::PrepareResponseParameters(
    mobile_apis::Result::eType& result_code, std::string& info) {
  app_mngr::commands::ResponseInfo ui_subtle_alert_info(
      subtle_alert_result_,
      HmiInterfaces::HMI_INTERFACE_UI,
      application_manager_);
  app_mngr::commands::ResponseInfo tts_alert_info(
      tts_speak_result_,
      HmiInterfaces::HMI_INTERFACE_TTS,
      application_manager_);

  bool result =
      PrepareResultForMobileResponse(ui_subtle_alert_info, tts_alert_info);

  result_code = mobile_apis::Result::WARNINGS;
  if ((ui_subtle_alert_info.is_ok || ui_subtle_alert_info.is_not_used) &&
      tts_alert_info.is_unsupported_resource &&
      HmiInterfaces::STATE_AVAILABLE == tts_alert_info.interface_state) {
    info = app_mngr::commands::MergeInfos(ui_subtle_alert_info,
                                          ui_response_info_,
                                          tts_alert_info,
                                          tts_response_info_);
    return result;
  }
  // Ignore TTS.Speak result if ABORTED due to TTS.StopSpeaking request
  if (is_tts_stop_speaking_sent_ &&
      hmi_apis::Common_Result::ABORTED == tts_alert_info.result_code) {
    result_code =
        MessageHelper::HMIToMobileResult(ui_subtle_alert_info.result_code);
    info = app_mngr::commands::MergeInfos(ui_subtle_alert_info,
                                          ui_response_info_,
                                          tts_alert_info,
                                          tts_response_info_);
    return ui_subtle_alert_info.is_ok;
  }
  result_code =
      PrepareResultCodeForResponse(ui_subtle_alert_info, tts_alert_info);
  info = app_mngr::commands::MergeInfos(ui_subtle_alert_info,
                                        ui_response_info_,
                                        tts_alert_info,
                                        tts_response_info_);
  // Mobile Alert request is successful when UI_SubtleAlert is successful
  bool has_unsupported_data = ui_subtle_alert_info.is_unsupported_resource &&
                              HmiInterfaces::STATE_NOT_AVAILABLE !=
                                  ui_subtle_alert_info.interface_state;
  if (is_ui_subtle_alert_sent_ && !ui_subtle_alert_info.is_ok &&
      !has_unsupported_data) {
    return false;
  }
  return result;
}

bool SubtleAlertRequest::Validate(uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();
  ApplicationSharedPtr app = application_manager_.application(app_id);

  if (!app) {
    SDL_LOG_ERROR("No application associated with session key");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return false;
  }

  if (mobile_apis::HMILevel::HMI_BACKGROUND ==
          app->hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW) &&
      app->AreCommandLimitsExceeded(
          static_cast<mobile_apis::FunctionID::eType>(function_id()),
          application_manager::TLimitSource::POLICY_TABLE)) {
    SDL_LOG_ERROR("SubtleAlert frequency is too high.");
    SendResponse(false,
                 mobile_apis::Result::REJECTED,
                 "SubtleAlert frequency is too high.");
    return false;
  }

  if (!CheckStrings()) {
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return false;
  }

  // ProcessSoftButtons checks strings on the contents incorrect character

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons((*message_)[strings::msg_params],
                                        app,
                                        policy_handler_,
                                        application_manager_);

  if (mobile_apis::Result::SUCCESS != processing_result) {
    SDL_LOG_ERROR("INVALID_DATA!");
    SendResponse(false, processing_result);
    return false;
  }

  // check if mandatory params(alertText1 and TTSChunk) specified
  if ((!(*message_)[strings::msg_params].keyExists(strings::alert_text1)) &&
      (!(*message_)[strings::msg_params].keyExists(strings::alert_text2)) &&
      (!(*message_)[strings::msg_params].keyExists(strings::tts_chunks))) {
    SDL_LOG_ERROR("Mandatory parameters are missing");
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "Mandatory parameters are missing");
    return false;
  }

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
      return false;
    }
  }

  return true;
}

void SubtleAlertRequest::SendSubtleAlertRequest(int32_t app_id) {
  SDL_LOG_AUTO_TRACE();
  ApplicationSharedPtr app = application_manager_.application(app_id);

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[hmi_request::alert_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  if ((*message_)[strings::msg_params].keyExists(strings::cancel_id)) {
    msg_params[strings::cancel_id] =
        (*message_)[strings::msg_params][strings::cancel_id].asInt();
  }

  int32_t index = 0;
  if ((*message_)[strings::msg_params].keyExists(strings::alert_text1)) {
    msg_params[hmi_request::alert_strings][index][hmi_request::field_name] =
        hmi_apis::Common_TextFieldName::subtleAlertText1;
    msg_params[hmi_request::alert_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::alert_text1];
    index++;
  }
  if ((*message_)[strings::msg_params].keyExists(strings::alert_text2)) {
    msg_params[hmi_request::alert_strings][index][hmi_request::field_name] =
        hmi_apis::Common_TextFieldName::subtleAlertText2;
    msg_params[hmi_request::alert_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::alert_text2];
  }

  // softButtons
  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[hmi_request::soft_buttons] =
        (*message_)[strings::msg_params][strings::soft_buttons];
    MessageHelper::SubscribeApplicationToSoftButton(
        (*message_)[strings::msg_params], app, function_id());
  } else {
    msg_params[strings::duration] =
        (*message_)[strings::msg_params][strings::duration].asUInt();
  }

  if ((*message_)[strings::msg_params].keyExists(strings::alert_icon)) {
    auto verification_result = MessageHelper::VerifyImage(
        (*message_)[strings::msg_params][strings::alert_icon],
        app,
        application_manager_);

    if (mobile_apis::Result::INVALID_DATA == verification_result) {
      SDL_LOG_ERROR("Image verification failed.");
      SendResponse(false, verification_result);
      return;
    }

    msg_params[strings::alert_icon] =
        (*message_)[strings::msg_params][strings::alert_icon];
  }

  // app_id
  msg_params[strings::app_id] = app_id;

  // PASA Alert type
  msg_params[strings::alert_type] = hmi_apis::Common_AlertType::UI;
  if (awaiting_tts_speak_response_) {
    msg_params[strings::alert_type] = hmi_apis::Common_AlertType::BOTH;
  }

  // check out if there are alert strings or soft buttons
  if (msg_params[hmi_request::alert_strings].length() > 0 ||
      msg_params.keyExists(hmi_request::soft_buttons)) {
    awaiting_ui_subtle_alert_response_ = true;
    is_ui_subtle_alert_sent_ = true;
    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
    SendHMIRequest(hmi_apis::FunctionID::UI_SubtleAlert, &msg_params, true);
  }
}

void SubtleAlertRequest::SendSpeakRequest(int32_t app_id,
                                          bool tts_chunks_exists,
                                          size_t length_tts_chunks) {
  SDL_LOG_AUTO_TRACE();
  using namespace hmi_apis;
  using namespace smart_objects;
  // crate HMI speak request
  SmartObject msg_params = smart_objects::SmartObject(SmartType_Map);
  if (tts_chunks_exists && length_tts_chunks) {
    msg_params[hmi_request::tts_chunks] =
        (*message_)[strings::msg_params][strings::tts_chunks];
  }
  msg_params[strings::app_id] = app_id;
  msg_params[hmi_request::speak_type] = Common_MethodName::SUBTLE_ALERT;
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
  SendHMIRequest(FunctionID::TTS_Speak, &msg_params, true);
}

bool SubtleAlertRequest::CheckStrings() {
  SDL_LOG_AUTO_TRACE();
  const char* str = NULL;

  if ((*message_)[strings::msg_params].keyExists(strings::alert_text1)) {
    str = (*message_)[strings::msg_params][strings::alert_text1].asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid alert_text_1 syntax check failed");
      return false;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::alert_text2)) {
    str = (*message_)[strings::msg_params][strings::alert_text2].asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid alert_text_2 syntax check failed");
      return false;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::tts_chunks)) {
    smart_objects::SmartObject& tts_chunks_array =
        (*message_)[strings::msg_params][strings::tts_chunks];
    for (size_t i = 0; i < tts_chunks_array.length(); ++i) {
      str = tts_chunks_array[i][strings::text].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid tts_chunks text syntax check failed");
        return false;
      }
    }
  }
  return true;
}

bool SubtleAlertRequest::HasHmiResponsesToWait() {
  SDL_LOG_AUTO_TRACE();
  return awaiting_ui_subtle_alert_response_ || awaiting_tts_speak_response_ ||
         awaiting_tts_stop_speaking_response_;
}

}  // namespace commands
}  // namespace sdl_rpc_plugin
