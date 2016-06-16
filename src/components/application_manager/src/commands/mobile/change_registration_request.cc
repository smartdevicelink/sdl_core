/*

 Copyright (c) 2016, Ford Motor Company
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

#include <string.h>
#include <algorithm>
#include "application_manager/commands/mobile/change_registration_request.h"
#include "application_manager/application_manager.h"
#include "application_manager/application_impl.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace {
namespace custom_str = utils::custom_string;
struct IsSameNickname {
  IsSameNickname(const custom_str::CustomString& app_id) : app_id_(app_id) {}
  bool operator()(const policy::StringArray::value_type& nickname) const {
    return app_id_.CompareIgnoreCase(nickname.c_str());
  }

 private:
  const custom_str::CustomString& app_id_;
};
}

namespace application_manager {

namespace commands {

ChangeRegistrationRequest::ChangeRegistrationRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager)
    , ui_result_(hmi_apis::Common_Result::INVALID_ENUM)
    , vr_result_(hmi_apis::Common_Result::INVALID_ENUM)
    , tts_result_(hmi_apis::Common_Result::INVALID_ENUM) {}

ChangeRegistrationRequest::~ChangeRegistrationRequest() {}

void ChangeRegistrationRequest::Run() {
  SDL_AUTO_TRACE();
  using namespace smart_objects;

  const HMICapabilities& hmi_capabilities =
      application_manager_.hmi_capabilities();

  ApplicationSharedPtr app = application_manager_.application(connection_key());
  if (!app) {
    SDL_ERROR("NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (IsWhiteSpaceExist()) {
    SDL_INFO("Incoming request contains \t\n \\t \\n or whitespace");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (mobile_apis::Result::SUCCESS != CheckCoincidence()) {
    SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
    return;
  }

  if (!hmi_capabilities.is_ui_cooperating()) {
    ui_result_ = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  }

  if (!hmi_capabilities.is_vr_cooperating()) {
    vr_result_ = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  }

  if (!hmi_capabilities.is_tts_cooperating()) {
    tts_result_ = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  }

  SmartObject& msg_params = (*message_)[strings::msg_params];

  const int32_t hmi_language =
      msg_params[strings::hmi_display_language].asInt();

  const int32_t language = msg_params[strings::language].asInt();

  if (false == (IsLanguageSupportedByUI(hmi_language) &&
                IsLanguageSupportedByVR(language) &&
                IsLanguageSupportedByTTS(language))) {
    SDL_ERROR("Language is not supported");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  if (msg_params.keyExists(strings::app_name) &&
      !IsNicknameAllowed(msg_params[strings::app_name].asCustomString())) {
    SDL_ERROR("Nickname is not allowed.");
    SendResponse(false, mobile_apis::Result::DISALLOWED);
    return;
  }

  pending_requests_.Add(hmi_apis::FunctionID::UI_ChangeRegistration);
  pending_requests_.Add(hmi_apis::FunctionID::VR_ChangeRegistration);
  pending_requests_.Add(hmi_apis::FunctionID::TTS_ChangeRegistration);

  // UI processing
  SmartObject ui_params = SmartObject(SmartType_Map);

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

  SendHMIRequest(hmi_apis::FunctionID::UI_ChangeRegistration, &ui_params, true);

  // VR processing
  SmartObject vr_params = SmartObject(SmartType_Map);

  vr_params[strings::language] = msg_params[strings::language];

  vr_params[strings::app_id] = app->app_id();
  if (msg_params.keyExists(strings::vr_synonyms)) {
    vr_params[strings::vr_synonyms] = msg_params[strings::vr_synonyms];
    app->set_vr_synonyms(msg_params[strings::vr_synonyms]);
  }
  SendHMIRequest(hmi_apis::FunctionID::VR_ChangeRegistration, &vr_params, true);

  // TTS processing
  SmartObject tts_params = SmartObject(SmartType_Map);

  tts_params[strings::language] = msg_params[strings::language];

  tts_params[strings::app_id] = app->app_id();
  if (msg_params.keyExists(strings::tts_name)) {
    tts_params[strings::tts_name] = msg_params[strings::tts_name];
    app->set_tts_name(msg_params[strings::tts_name]);
  }

  SendHMIRequest(
      hmi_apis::FunctionID::TTS_ChangeRegistration, &tts_params, true);
}

bool ChangeRegistrationRequest::AllHmiResponsesSuccess(
    const hmi_apis::Common_Result::eType ui,
    const hmi_apis::Common_Result::eType vr,
    const hmi_apis::Common_Result::eType tts) {
  return hmi_apis::Common_Result::SUCCESS == ui &&
         hmi_apis::Common_Result::SUCCESS == vr &&
         hmi_apis::Common_Result::SUCCESS == tts;
}

void ChangeRegistrationRequest::on_event(const event_engine::Event& event) {
  SDL_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();

  hmi_apis::FunctionID::eType event_id = event.id();

  switch (event_id) {
    case hmi_apis::FunctionID::UI_ChangeRegistration: {
      SDL_INFO("Received UI_ChangeRegistration event");
      pending_requests_.Remove(event_id);
      ui_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      break;
    }
    case hmi_apis::FunctionID::VR_ChangeRegistration: {
      SDL_INFO("Received VR_ChangeRegistration event");
      pending_requests_.Remove(event_id);
      vr_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      break;
    }
    case hmi_apis::FunctionID::TTS_ChangeRegistration: {
      SDL_INFO("Received TTS_ChangeRegistration event");
      pending_requests_.Remove(event_id);
      tts_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      break;
    }
    default: {
      SDL_ERROR("Received unknown event" << event_id);
      return;
    }
  }

  if (pending_requests_.IsFinal(event_id)) {
    ApplicationSharedPtr application =
        application_manager_.application(connection_key());

    if (!application) {
      SDL_ERROR("NULL pointer");
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

    int32_t greates_result_code =
        std::max(std::max(ui_result_, vr_result_), tts_result_);

    (*message_)[strings::params][strings::function_id] =
        mobile_apis::FunctionID::eType::ChangeRegistrationID;

    SendResponse(AllHmiResponsesSuccess(ui_result_, vr_result_, tts_result_),
                 static_cast<mobile_apis::Result::eType>(greates_result_code),
                 NULL,
                 &(message[strings::msg_params]));
  } else {
    SDL_INFO(
        "There are some pending responses from HMI."
        "ChangeRegistrationRequest still waiting.");
  }
}

bool ChangeRegistrationRequest::IsLanguageSupportedByUI(
    const int32_t& hmi_display_lang) {
  const HMICapabilities& hmi_capabilities =
      application_manager_.hmi_capabilities();
  const smart_objects::SmartObject* ui_languages =
      hmi_capabilities.ui_supported_languages();

  if (!ui_languages) {
    SDL_ERROR("NULL pointer");
    return false;
  }

  for (size_t i = 0; i < ui_languages->length(); ++i) {
    if (hmi_display_lang == ui_languages->getElement(i).asInt()) {
      return true;
    }
  }

  SDL_ERROR("Language isn't supported by UI");

  return false;
}

bool ChangeRegistrationRequest::IsLanguageSupportedByVR(
    const int32_t& hmi_display_lang) {
  const HMICapabilities& hmi_capabilities =
      application_manager_.hmi_capabilities();
  const smart_objects::SmartObject* vr_languages =
      hmi_capabilities.vr_supported_languages();

  if (!vr_languages) {
    SDL_ERROR("NULL pointer");
    return false;
  }

  for (size_t i = 0; i < vr_languages->length(); ++i) {
    if (hmi_display_lang == vr_languages->getElement(i).asInt()) {
      return true;
    }
  }

  SDL_ERROR("Language isn't supported by VR");

  return false;
}

bool ChangeRegistrationRequest::IsLanguageSupportedByTTS(
    const int32_t& hmi_display_lang) {
  const HMICapabilities& hmi_capabilities =
      application_manager_.hmi_capabilities();
  const smart_objects::SmartObject* tts_languages =
      hmi_capabilities.tts_supported_languages();

  if (!tts_languages) {
    SDL_ERROR("NULL pointer");
    return false;
  }

  for (size_t i = 0; i < tts_languages->length(); ++i) {
    if (hmi_display_lang == tts_languages->getElement(i).asInt()) {
      return true;
      break;
    }
  }

  SDL_ERROR("Language isn't supported by TTS");
  return false;
}

bool ChangeRegistrationRequest::IsWhiteSpaceExist() {
  const char* str = NULL;

  if ((*message_)[strings::msg_params].keyExists(strings::app_name)) {
    str = (*message_)[strings::msg_params][strings::app_name].asCharArray();
    if (!CheckSyntax(str)) {
      SDL_ERROR("Invalid app_name syntax check failed");
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
        SDL_ERROR("Invalid tts_name syntax check failed");
        return true;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(
          strings::ngn_media_screen_app_name)) {
    str = (*message_)[strings::msg_params][strings::ngn_media_screen_app_name]
              .asCharArray();
    if (!CheckSyntax(str)) {
      SDL_ERROR("Invalid ngn_media_screen_app_name syntax check failed");
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
        SDL_ERROR("Invalid vr_synonyms syntax check failed");
        return true;
      }
    }
  }
  return false;
}

mobile_apis::Result::eType ChangeRegistrationRequest::CheckCoincidence() {
  SDL_AUTO_TRACE();

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  ApplicationSet accessor = application_manager_.applications().GetData();
  custom_str::CustomString app_name;
  uint32_t app_id = connection_key();
  if (msg_params.keyExists(strings::app_name)) {
    app_name = msg_params[strings::app_name].asCustomString();
  }

  ApplicationSetConstIt it = accessor.begin();
  for (; accessor.end() != it; ++it) {
    if (app_id == (*it)->app_id()) {
      continue;
    }

    const custom_str::CustomString& cur_name = (*it)->name();
    if (msg_params.keyExists(strings::app_name)) {
      if (app_name.CompareIgnoreCase(cur_name)) {
        SDL_ERROR("Application name is known already.");
        return mobile_apis::Result::DUPLICATE_NAME;
      }

      const smart_objects::SmartObject* vr = (*it)->vr_synonyms();
      const std::vector<smart_objects::SmartObject>* curr_vr = NULL;
      if (NULL != vr) {
        curr_vr = vr->asArray();
        CoincidencePredicateVR v(app_name);

        if (0 != std::count_if(curr_vr->begin(), curr_vr->end(), v)) {
          SDL_ERROR("Application name is known already.");
          return mobile_apis::Result::DUPLICATE_NAME;
        }
      }
    }

    // vr check
    if (msg_params.keyExists(strings::vr_synonyms)) {
      const std::vector<smart_objects::SmartObject>* new_vr =
          msg_params[strings::vr_synonyms].asArray();

      CoincidencePredicateVR v(cur_name);
      if (0 != std::count_if(new_vr->begin(), new_vr->end(), v)) {
        SDL_ERROR("vr_synonyms duplicated with app_name .");
        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }  // end vr check
  }    // application for end
  return mobile_apis::Result::SUCCESS;
}

bool ChangeRegistrationRequest::IsNicknameAllowed(
    const custom_str::CustomString& app_name) const {
  SDL_AUTO_TRACE();
  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_ERROR("Can't find appication with connection key " << connection_key());
    return false;
  }

  const std::string policy_app_id = app->policy_app_id();

  policy::StringArray app_nicknames;
  policy::StringArray app_hmi_types;

  bool init_result = application_manager_.GetPolicyHandler().GetInitialAppData(
      policy_app_id, &app_nicknames, &app_hmi_types);

  if (!init_result) {
    SDL_ERROR("Error during getting of nickname list for application "
              << policy_app_id);
    return false;
  }

  if (!app_nicknames.empty()) {
    IsSameNickname compare(app_name);
    policy::StringArray::const_iterator it =
        std::find_if(app_nicknames.begin(), app_nicknames.end(), compare);
    if (app_nicknames.end() == it) {
      SDL_WARN("Application name was not found in nicknames list.");

      usage_statistics::AppCounter count_of_rejections_nickname_mismatch(
          application_manager_.GetPolicyHandler().GetStatisticManager(),
          policy_app_id,
          usage_statistics::REJECTIONS_NICKNAME_MISMATCH);
      ++count_of_rejections_nickname_mismatch;

      return false;
    }
  }
  return true;
}

}  // namespace commands

}  // namespace application_manager
