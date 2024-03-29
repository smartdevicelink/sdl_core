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

#include "sdl_rpc_plugin/commands/mobile/change_registration_request.h"

#include <string.h>

#include <algorithm>

#include "application_manager/application_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

namespace {
namespace custom_str = utils::custom_string;
struct IsSameNickname {
  explicit IsSameNickname(const custom_str::CustomString& app_name)
      : app_name(app_name) {}
  bool operator()(const policy::StringArray::value_type& nickname) const {
    return app_name.CompareIgnoreCase(nickname.c_str());
  }

 private:
  const custom_str::CustomString& app_name;
};
}  // namespace

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

ChangeRegistrationRequest::ChangeRegistrationRequest(
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
    , ui_result_(hmi_apis::Common_Result::INVALID_ENUM)
    , vr_result_(hmi_apis::Common_Result::INVALID_ENUM)
    , tts_result_(hmi_apis::Common_Result::INVALID_ENUM) {}

ChangeRegistrationRequest::~ChangeRegistrationRequest() {}

void ChangeRegistrationRequest::SendVRRequest(
    ApplicationSharedPtr app, smart_objects::SmartObject& msg_params) {
  const HmiInterfaces& hmi_interfaces = application_manager_.hmi_interfaces();
  auto function = hmi_apis::FunctionID::VR_ChangeRegistration;
  smart_objects::SmartObject vr_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  vr_params[strings::language] = msg_params[strings::language];

  vr_params[strings::app_id] = app->app_id();
  if (msg_params.keyExists(strings::vr_synonyms)) {
    vr_params[strings::vr_synonyms] = msg_params[strings::vr_synonyms];
    app->set_vr_synonyms(msg_params[strings::vr_synonyms]);
  }
  StartAwaitForInterface(hmi_interfaces.GetInterfaceFromFunction(function));
  SendHMIRequest(function, &vr_params, true);
}

void ChangeRegistrationRequest::SendTTSRequest(
    ApplicationSharedPtr app, smart_objects::SmartObject& msg_params) {
  const HmiInterfaces& hmi_interfaces = application_manager_.hmi_interfaces();
  auto function = hmi_apis::FunctionID::TTS_ChangeRegistration;

  smart_objects::SmartObject tts_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  tts_params[strings::language] = msg_params[strings::language];

  tts_params[strings::app_id] = app->app_id();
  if (msg_params.keyExists(strings::tts_name)) {
    tts_params[strings::tts_name] = msg_params[strings::tts_name];
    app->set_tts_name(msg_params[strings::tts_name]);
  }
  StartAwaitForInterface(hmi_interfaces.GetInterfaceFromFunction(function));
  SendHMIRequest(function, &tts_params, true);
}

void ChangeRegistrationRequest::SendUIRequest(
    ApplicationSharedPtr app,
    smart_objects::SmartObject& msg_params,
    const int32_t hmi_language) {
  const HmiInterfaces& hmi_interfaces = application_manager_.hmi_interfaces();
  auto function = hmi_apis::FunctionID::UI_ChangeRegistration;
  // UI processing
  smart_objects::SmartObject ui_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  ui_params[strings::language] = hmi_language;
  ui_params[strings::app_id] = app->app_id();
  if (msg_params.keyExists(strings::app_name)) {
    ui_params[strings::app_name] = msg_params[strings::app_name];
    app->set_name(msg_params[strings::app_name].asCustomString());
  }
  if (msg_params.keyExists(strings::ngn_media_screen_app_name)) {
    ui_params[strings::ngn_media_screen_app_name] =
        msg_params[strings::ngn_media_screen_app_name];
    app->set_ngn_media_screen_name(
        msg_params[strings::ngn_media_screen_app_name]);
  }

  StartAwaitForInterface(hmi_interfaces.GetInterfaceFromFunction(function));
  SendHMIRequest(function, &ui_params, true);
}

void ChangeRegistrationRequest::Run() {
  SDL_LOG_AUTO_TRACE();
  using namespace smart_objects;

  ApplicationSharedPtr app = application_manager_.application(connection_key());
  if (!app) {
    SDL_LOG_ERROR("NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (IsWhiteSpaceExist()) {
    SDL_LOG_INFO("Incoming request contains \t\n \\t \\n or whitespace");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (mobile_apis::Result::SUCCESS != CheckCoincidence(app->device())) {
    SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
    return;
  }

  SmartObject& msg_params = (*message_)[strings::msg_params];

  const int32_t hmi_language =
      msg_params[strings::hmi_display_language].asInt();

  const int32_t language = msg_params[strings::language].asInt();

  if (false == (IsLanguageSupportedByUI(hmi_language) &&
                IsLanguageSupportedByVR(language) &&
                IsLanguageSupportedByTTS(language))) {
    SDL_LOG_ERROR("Language is not supported");
    SendResponse(false, mobile_apis::Result::UNSUPPORTED_RESOURCE);
    return;
  }

  if (msg_params.keyExists(strings::app_name) &&
      !IsNicknameAllowed(msg_params[strings::app_name].asCustomString())) {
    SDL_LOG_ERROR("Nickname is not allowed.");
    SendResponse(false, mobile_apis::Result::DISALLOWED);
    return;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::tts_name)) {
    smart_objects::SmartObject& tts_name =
        (*message_)[strings::msg_params][strings::tts_name];
    mobile_apis::Result::eType verification_result =
        MessageHelper::VerifyTtsFiles(tts_name, app, application_manager_);

    if (mobile_apis::Result::FILE_NOT_FOUND == verification_result) {
      SDL_LOG_ERROR("MessageHelper::VerifyTtsFiles return "
                    << verification_result);
      SendResponse(false,
                   mobile_apis::Result::FILE_NOT_FOUND,
                   "One or more files needed for tts_name are not present");
      return;
    }
  }

  const HmiInterfaces& hmi_interfaces = application_manager_.hmi_interfaces();

  const HmiInterfaces::InterfaceState vr_state =
      hmi_interfaces.GetInterfaceState(
          HmiInterfaces::InterfaceID::HMI_INTERFACE_VR);
  const HmiInterfaces::InterfaceState ui_state =
      hmi_interfaces.GetInterfaceState(
          HmiInterfaces::InterfaceID::HMI_INTERFACE_UI);
  const HmiInterfaces::InterfaceState tts_state =
      hmi_interfaces.GetInterfaceState(
          HmiInterfaces::InterfaceID::HMI_INTERFACE_TTS);

  using helpers::ALL;
  using helpers::Compare;
  using helpers::EQ;

  if (Compare<HmiInterfaces::InterfaceState, EQ, ALL>(
          HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE,
          vr_state,
          tts_state,
          ui_state)) {
    SendResponse(false, mobile_apis::Result::UNSUPPORTED_RESOURCE);
    return;
  }

  if (HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE != vr_state) {
    StartAwaitForInterface(HmiInterfaces::InterfaceID::HMI_INTERFACE_VR);
  }

  if (HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE != tts_state) {
    StartAwaitForInterface(HmiInterfaces::InterfaceID::HMI_INTERFACE_TTS);
  }

  if (HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE != ui_state) {
    StartAwaitForInterface(HmiInterfaces::InterfaceID::HMI_INTERFACE_UI);
  }

  if (IsInterfaceAwaited(HmiInterfaces::HMI_INTERFACE_UI)) {
    SendUIRequest(app, msg_params, hmi_language);
  }

  if (IsInterfaceAwaited(HmiInterfaces::HMI_INTERFACE_TTS)) {
    SendTTSRequest(app, msg_params);
  }

  if (IsInterfaceAwaited(HmiInterfaces::HMI_INTERFACE_VR)) {
    SendVRRequest(app, msg_params);
  }
}

void ChangeRegistrationRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();

  hmi_apis::FunctionID::eType event_id = event.id();

  switch (event_id) {
    case hmi_apis::FunctionID::UI_ChangeRegistration: {
      SDL_LOG_INFO("Received UI_ChangeRegistration event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      ui_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, ui_response_info_);
      break;
    }
    case hmi_apis::FunctionID::VR_ChangeRegistration: {
      SDL_LOG_INFO("Received VR_ChangeRegistration event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VR);
      vr_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, vr_response_info_);
      break;
    }
    case hmi_apis::FunctionID::TTS_ChangeRegistration: {
      SDL_LOG_INFO("Received TTS_ChangeRegistration event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
      tts_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, tts_response_info_);
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event " << event_id);
      return;
    }
  }

  if (IsPendingResponseExist()) {
    SDL_LOG_DEBUG("Command still wating for HMI response");
    return;
  }

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (!application) {
    SDL_LOG_ERROR("NULL pointer");
    return;
  }

  if (hmi_apis::Common_Result::SUCCESS == ui_result_) {
    application->set_ui_language(static_cast<mobile_api::Language::eType>(
        (*message_)[strings::msg_params][strings::hmi_display_language]
            .asInt()));
  }

  if (hmi_apis::Common_Result::SUCCESS == vr_result_ ||
      hmi_apis::Common_Result::SUCCESS == tts_result_) {
    application->set_language(static_cast<mobile_api::Language::eType>(
        (*message_)[strings::msg_params][strings::language].asInt()));
  }

  mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;
  std::string response_info;
  const bool result = PrepareResponseParameters(result_code, response_info);
  SendResponse(result,
               result_code,
               response_info.empty() ? NULL : response_info.c_str(),
               &(message[strings::msg_params]));
}  // namespace commands

namespace {
void CheckInfo(std::string& str) {
  if (std::string::npos != str.find("is not supported by system")) {
    str.clear();
  }
}
}  // namespace

bool ChangeRegistrationRequest::PrepareResponseParameters(
    mobile_apis::Result::eType& result_code, std::string& response_info) {
  SDL_LOG_AUTO_TRACE();
  using namespace helpers;
  const bool is_tts_succeeded_unsupported =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          tts_result_,
          hmi_apis::Common_Result::SUCCESS,
          hmi_apis::Common_Result::WARNINGS,
          hmi_apis::Common_Result::WRONG_LANGUAGE,
          hmi_apis::Common_Result::RETRY,
          hmi_apis::Common_Result::SAVED,
          hmi_apis::Common_Result::UNSUPPORTED_RESOURCE);

  const bool is_ui_succeeded_unsupported =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          ui_result_,
          hmi_apis::Common_Result::SUCCESS,
          hmi_apis::Common_Result::WARNINGS,
          hmi_apis::Common_Result::WRONG_LANGUAGE,
          hmi_apis::Common_Result::RETRY,
          hmi_apis::Common_Result::SAVED,
          hmi_apis::Common_Result::UNSUPPORTED_RESOURCE);

  const bool is_vr_succeeded_unsupported =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          vr_result_,
          hmi_apis::Common_Result::SUCCESS,
          hmi_apis::Common_Result::WARNINGS,
          hmi_apis::Common_Result::WRONG_LANGUAGE,
          hmi_apis::Common_Result::RETRY,
          hmi_apis::Common_Result::SAVED,
          hmi_apis::Common_Result::UNSUPPORTED_RESOURCE);

  const bool is_tts_ui_vr_unsupported =
      Compare<hmi_apis::Common_Result::eType, EQ, ALL>(
          hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
          tts_result_,
          ui_result_,
          vr_result_);

  const HmiInterfaces& hmi_interfaces = application_manager_.hmi_interfaces();
  const HmiInterfaces::InterfaceState tts_state =
      hmi_interfaces.GetInterfaceState(
          HmiInterfaces::InterfaceID::HMI_INTERFACE_TTS);
  const HmiInterfaces::InterfaceState vr_state =
      hmi_interfaces.GetInterfaceState(
          HmiInterfaces::InterfaceID::HMI_INTERFACE_VR);
  const HmiInterfaces::InterfaceState ui_state =
      hmi_interfaces.GetInterfaceState(
          HmiInterfaces::InterfaceID::HMI_INTERFACE_UI);

  app_mngr::commands::ResponseInfo ui_properties_info(
      ui_result_, HmiInterfaces::HMI_INTERFACE_UI, application_manager_);

  app_mngr::commands::ResponseInfo tts_properties_info(
      tts_result_, HmiInterfaces::HMI_INTERFACE_TTS, application_manager_);

  app_mngr::commands::ResponseInfo vr_properties_info(
      ui_result_, HmiInterfaces::HMI_INTERFACE_VR, application_manager_);

  bool result = ((!is_tts_ui_vr_unsupported) && is_tts_succeeded_unsupported &&
                 is_ui_succeeded_unsupported && is_vr_succeeded_unsupported);

  const bool is_tts_or_ui_or_vr_unsupported =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
          tts_result_,
          ui_result_,
          vr_result_);

  if ((result && is_tts_or_ui_or_vr_unsupported)) {
    result_code = mobile_apis::Result::UNSUPPORTED_RESOURCE;
    result =
        PrepareResultForMobileResponse(ui_properties_info,
                                       tts_properties_info) &&
        PrepareResultForMobileResponse(tts_properties_info, vr_properties_info);
  } else {
    // If response contains erroneous result code SDL need return erroneus
    // result code.
    hmi_apis::Common_Result::eType ui_result =
        hmi_apis::Common_Result::INVALID_ENUM;
    hmi_apis::Common_Result::eType vr_result =
        hmi_apis::Common_Result::INVALID_ENUM;
    hmi_apis::Common_Result::eType tts_result =
        hmi_apis::Common_Result::INVALID_ENUM;
    if (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE != ui_result_) {
      ui_result = ui_result_;
    }
    if (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE != vr_result_) {
      vr_result = vr_result_;
    }
    if (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE != tts_result_) {
      tts_result = tts_result_;
    }
    result_code = MessageHelper::HMIToMobileResult(
        std::max(std::max(ui_result, vr_result), tts_result));
    if (mobile_api::Result::INVALID_ENUM == result_code) {
      result_code = mobile_api::Result::UNSUPPORTED_RESOURCE;
    }
  }

  const bool is_tts_state_available =
      tts_state == HmiInterfaces::STATE_AVAILABLE;
  const bool is_vr_state_available = vr_state == HmiInterfaces::STATE_AVAILABLE;
  const bool is_ui_state_available = ui_state == HmiInterfaces::STATE_AVAILABLE;

  const bool is_tts_hmi_info =
      is_tts_state_available && !tts_response_info_.empty();
  const bool is_vr_hmi_info =
      is_vr_state_available && !vr_response_info_.empty();
  const bool is_ui_hmi_info =
      is_ui_state_available && !ui_response_info_.empty();

  if (is_tts_hmi_info || is_vr_hmi_info || is_ui_hmi_info) {
    if (!is_tts_hmi_info)
      CheckInfo(tts_response_info_);
    if (!is_vr_hmi_info)
      CheckInfo(ui_response_info_);
    if (!is_ui_hmi_info)
      CheckInfo(vr_response_info_);
  }

  response_info = app_mngr::commands::MergeInfos(
      ui_response_info_, vr_response_info_, tts_response_info_);
  return result;
}

bool ChangeRegistrationRequest::IsLanguageSupportedByUI(
    const int32_t& hmi_display_lang) {
  const HMICapabilities& hmi_capabilities = hmi_capabilities_;
  const auto ui_languages = hmi_capabilities.ui_supported_languages();

  if (!ui_languages) {
    SDL_LOG_ERROR("NULL pointer");
    return false;
  }

  for (size_t i = 0; i < ui_languages->length(); ++i) {
    if (hmi_display_lang == ui_languages->getElement(i).asInt()) {
      return true;
    }
  }

  SDL_LOG_ERROR("Language isn't supported by UI");

  return false;
}

bool ChangeRegistrationRequest::IsLanguageSupportedByVR(
    const int32_t& hmi_display_lang) {
  const HMICapabilities& hmi_capabilities = hmi_capabilities_;
  const auto vr_languages = hmi_capabilities.vr_supported_languages();

  if (!vr_languages) {
    SDL_LOG_ERROR("NULL pointer");
    return false;
  }

  for (size_t i = 0; i < vr_languages->length(); ++i) {
    if (hmi_display_lang == vr_languages->getElement(i).asInt()) {
      return true;
    }
  }

  SDL_LOG_ERROR("Language isn't supported by VR");

  return false;
}

bool ChangeRegistrationRequest::IsLanguageSupportedByTTS(
    const int32_t& hmi_display_lang) {
  const HMICapabilities& hmi_capabilities = hmi_capabilities_;
  const auto tts_languages = hmi_capabilities.tts_supported_languages();

  if (!tts_languages) {
    SDL_LOG_ERROR("NULL pointer");
    return false;
  }

  for (size_t i = 0; i < tts_languages->length(); ++i) {
    if (hmi_display_lang == tts_languages->getElement(i).asInt()) {
      return true;
    }
  }

  SDL_LOG_ERROR("Language isn't supported by TTS");
  return false;
}

bool ChangeRegistrationRequest::IsWhiteSpaceExist() {
  const char* str = NULL;

  if ((*message_)[strings::msg_params].keyExists(strings::app_name)) {
    str = (*message_)[strings::msg_params][strings::app_name].asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid app_name syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::tts_name)) {
    const smart_objects::SmartArray* tn_array =
        (*message_)[strings::msg_params][strings::tts_name].asArray();

    smart_objects::SmartArray::const_iterator it_tn = tn_array->begin();
    smart_objects::SmartArray::const_iterator it_tn_end = tn_array->end();

    for (; it_tn != it_tn_end; ++it_tn) {
      str = (*it_tn)[strings::text].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid tts_name syntax check failed");
        return true;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(
          strings::ngn_media_screen_app_name)) {
    str = (*message_)[strings::msg_params][strings::ngn_media_screen_app_name]
              .asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid ngn_media_screen_app_name syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::vr_synonyms)) {
    const smart_objects::SmartArray* vs_array =
        (*message_)[strings::msg_params][strings::vr_synonyms].asArray();

    smart_objects::SmartArray::const_iterator it_vs = vs_array->begin();
    smart_objects::SmartArray::const_iterator it_vs_end = vs_array->end();

    for (; it_vs != it_vs_end; ++it_vs) {
      str = (*it_vs).asCharArray();
      if (!CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid vr_synonyms syntax check failed");
        return true;
      }
    }
  }
  return false;
}

mobile_apis::Result::eType ChangeRegistrationRequest::CheckCoincidence(
    const connection_handler::DeviceHandle& device_id) {
  SDL_LOG_AUTO_TRACE();

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  auto compare_tts_name = [](const smart_objects::SmartObject& obj_1,
                             const smart_objects::SmartObject& obj_2) {
    return obj_1[application_manager::strings::text]
        .asCustomString()
        .CompareIgnoreCase(
            obj_2[application_manager::strings::text].asCustomString());
  };

  const auto& accessor = application_manager_.applications().GetData();
  custom_str::CustomString app_name;
  const uint32_t app_id = connection_key();
  if (msg_params.keyExists(strings::app_name)) {
    app_name = msg_params[strings::app_name].asCustomString();
  }

  for (const auto& app : accessor) {
    if (app->device() != device_id) {
      continue;
    }

    if (app->app_id() == app_id) {
      continue;
    }

    const auto& cur_name = app->name();
    if (msg_params.keyExists(strings::app_name)) {
      if (app_name.CompareIgnoreCase(cur_name)) {
        SDL_LOG_ERROR("Application name is known already.");
        return mobile_apis::Result::DUPLICATE_NAME;
      }
      const auto vr = app->vr_synonyms();
      if (vr) {
        const auto curr_vr = vr->asArray();
        CoincidencePredicateVR v(app_name);

        if (0 != std::count_if(curr_vr->begin(), curr_vr->end(), v)) {
          SDL_LOG_ERROR("Application name is known already.");
          return mobile_apis::Result::DUPLICATE_NAME;
        }
      }
    }

    // VR check
    if (msg_params.keyExists(strings::vr_synonyms)) {
      const auto new_vr = msg_params[strings::vr_synonyms].asArray();

      CoincidencePredicateVR v(cur_name);
      if (0 != std::count_if(new_vr->begin(), new_vr->end(), v)) {
        SDL_LOG_ERROR("vr_synonyms duplicated with app_name .");
        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }  // End vr check

    // TTS check
    if (msg_params.keyExists(strings::tts_name) && app->tts_name()) {
      const auto tts_array = msg_params[strings::tts_name].asArray();
      const auto tts_curr = app->tts_name()->asArray();
      const auto& it_tts = std::find_first_of(tts_array->begin(),
                                              tts_array->end(),
                                              tts_curr->begin(),
                                              tts_curr->end(),
                                              compare_tts_name);
      if (it_tts != tts_array->end()) {
        SDL_LOG_ERROR("TTS name: "
                      << (*it_tts)[strings::text].asCustomString().AsMBString()
                      << " is known already");
        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }  // End tts check

  }  // Application for end
  return mobile_apis::Result::SUCCESS;
}

bool ChangeRegistrationRequest::IsNicknameAllowed(
    const custom_str::CustomString& app_name) const {
  SDL_LOG_AUTO_TRACE();
  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("Can't find appication with connection key "
                  << connection_key());
    return false;
  }

  const std::string policy_app_id = app->policy_app_id();

  policy::StringArray app_nicknames;
  policy::StringArray app_hmi_types;

  bool init_result = policy_handler_.GetInitialAppData(
      policy_app_id, &app_nicknames, &app_hmi_types);

  if (!init_result) {
    SDL_LOG_ERROR("Error during getting of nickname list for application "
                  << policy_app_id);
    return false;
  }

  if (!app_nicknames.empty()) {
    IsSameNickname compare(app_name);
    policy::StringArray::const_iterator it =
        std::find_if(app_nicknames.begin(), app_nicknames.end(), compare);
    if (app_nicknames.end() == it) {
      SDL_LOG_WARN("Application name was not found in nicknames list.");

      usage_statistics::AppCounter count_of_rejections_nickname_mismatch(
          policy_handler_.GetStatisticManager(),
          policy_app_id,
          usage_statistics::REJECTIONS_NICKNAME_MISMATCH);
      ++count_of_rejections_nickname_mismatch;

      return false;
    }
  }
  return true;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
