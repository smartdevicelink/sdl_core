/*
 * Copyright (c) 2016, Ford Motor Company
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

#include "application_manager/application_manager_impl.h"
#include "application_manager/hmi_language_handler.h"
#include "application_manager/message_helper.h"
#include "application_manager/hmi_capabilities.h"
#include "utils/helpers.h"
#include "resumption/last_state.h"

static const std::string LanguagesKey = "Languages";
static const std::string UIKey = "UI";
static const std::string VRKey = "VR";
static const std::string TTSKey = "TTS";

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

HMILanguageHandler::HMILanguageHandler()
  : is_ui_language_received_(false),
    is_vr_language_received_(false),
    is_tts_language_received_(false) {
}

void HMILanguageHandler::set_ui_language(
      hmi_apis::Common_Language::eType language) {
  resumption::LastState::instance()->dictionary[LanguagesKey][UIKey] = language;
}

void HMILanguageHandler::set_vr_language(
      hmi_apis::Common_Language::eType language) {
  resumption::LastState::instance()->dictionary[LanguagesKey][VRKey] = language;
}

void HMILanguageHandler::set_tts_language(
      hmi_apis::Common_Language::eType language) {
  resumption::LastState::instance()->dictionary[LanguagesKey][TTSKey] = language;
}

hmi_apis::Common_Language::eType HMILanguageHandler::get_ui_language() const {
  using namespace resumption;
  using namespace hmi_apis;
  if (LastState::instance()->dictionary.isMember(LanguagesKey)) {
    if (LastState::instance()->dictionary[LanguagesKey].isMember(UIKey)) {
      Common_Language::eType ui_language =
          static_cast<Common_Language::eType>(
          LastState::instance()->dictionary[LanguagesKey][UIKey].asUInt());

      return ui_language;
    }
  }
  return Common_Language::INVALID_ENUM;
}

hmi_apis::Common_Language::eType HMILanguageHandler::get_vr_language() const {
  using namespace resumption;
  using namespace hmi_apis;
  if (LastState::instance()->dictionary.isMember(LanguagesKey)) {
    if (LastState::instance()->dictionary[LanguagesKey].isMember(VRKey)) {
      Common_Language::eType vr_language =
          static_cast<Common_Language::eType>(
          LastState::instance()->dictionary[LanguagesKey][VRKey].asUInt());

      return vr_language;
    }
  }
  return Common_Language::INVALID_ENUM;
}

hmi_apis::Common_Language::eType HMILanguageHandler::get_tts_language() const {
  using namespace resumption;
  using namespace hmi_apis;
  if (LastState::instance()->dictionary.isMember(LanguagesKey)) {
    if (LastState::instance()->dictionary[LanguagesKey].isMember(TTSKey)) {
      // Web HMI returns -1 which causes assert for debug
      Common_Language::eType tts_language = //Common_Language::EN_US;
          static_cast<Common_Language::eType>(
          LastState::instance()->dictionary[LanguagesKey][TTSKey].asUInt());

      return tts_language;
    }
  }
  return Common_Language::INVALID_ENUM;
}

void HMILanguageHandler::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  switch (event.id()) {
  case hmi_apis::FunctionID::UI_GetLanguage:
    LOG4CXX_DEBUG(logger_, "Got UI language response.");
    is_ui_language_received_ = true;
    break;
  case hmi_apis::FunctionID::VR_GetLanguage:
    LOG4CXX_DEBUG(logger_, "Got VR language response.");
    is_vr_language_received_ = true;
    break;
  case hmi_apis::FunctionID::TTS_GetLanguage:
    LOG4CXX_DEBUG(logger_, "Got TTS language response.");
    is_tts_language_received_ = true;
    break;
  default:
    return;
  }

  if (is_ui_language_received_ && is_vr_language_received_ &&
      is_tts_language_received_) {
    VerifyRegisteredApps();
  }
}

void HMILanguageHandler::set_handle_response_for(
    const event_engine::smart_objects::SmartObject& request) {
  using namespace helpers;
  if (!request.keyExists(strings::params)) {
    return;
  }

  if (!request[strings::params].keyExists(strings::function_id)) {
    return;
  }

  if (!request[strings::params].keyExists(strings::correlation_id)) {
    return;
  }

  hmi_apis::FunctionID::eType function_id =
      static_cast<hmi_apis::FunctionID::eType>(
        request[strings::params][strings::function_id].asInt());

  if (!Compare<hmi_apis::FunctionID::eType, EQ, ONE>(
        function_id,
        hmi_apis::FunctionID::UI_GetLanguage,
        hmi_apis::FunctionID::VR_GetLanguage,
        hmi_apis::FunctionID::TTS_GetLanguage)) {
    return;
  }

  uint32_t correlation_id =
      request[strings::params][strings::correlation_id].asUInt();

  subscribe_on_event(function_id, correlation_id);

  LOG4CXX_DEBUG(logger_, "Subscribed for function_id " << function_id <<
                " and correlation_id " << correlation_id);
}

void HMILanguageHandler::VerifyRegisteredApps() const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  HMICapabilities& hmi_capabilities =
        ApplicationManagerImpl::instance()->hmi_capabilities();

  hmi_apis::Common_Language::eType ui_language =
          hmi_capabilities.active_ui_language();
  hmi_apis::Common_Language::eType vr_language =
          hmi_capabilities.active_vr_language();
  hmi_apis::Common_Language::eType tts_language =
          hmi_capabilities.active_tts_language();
  ApplicationManagerImpl::ApplicationListAccessor accessor;
  ApplicationSetIt it = accessor.begin();
  for (; accessor.end() != it;) {
    ApplicationSharedPtr app = *it++;
    if (app->ui_language() !=
        MessageHelper::CommonToMobileLanguage(ui_language) ||
        !Compare<mobile_apis::Language::eType, EQ, ALL>(
          app->language(),
          MessageHelper::CommonToMobileLanguage(vr_language),
          MessageHelper::CommonToMobileLanguage(tts_language))) {
      MessageHelper::SendOnLanguageChangeToMobile(app->app_id());
      MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
                  app->app_id(),
                  mobile_api::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE);
      ApplicationManagerImpl::instance()->UnregisterApplication(
                  app->app_id(), mobile_apis::Result::SUCCESS, false);
    }
  }
}

} // namespace application_manager
