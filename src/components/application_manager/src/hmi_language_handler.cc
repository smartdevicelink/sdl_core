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

#include "application_manager/hmi_language_handler.h"
#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"
#include "application_manager/hmi_capabilities.h"
#include "utils/helpers.h"
#include "resumption/last_state.h"
#include "smart_objects/smart_object.h"

static const std::string LanguagesKey = "Languages";
static const std::string UIKey = "UI";
static const std::string VRKey = "VR";
static const std::string TTSKey = "TTS";

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")
namespace application_manager {

HMILanguageHandler::HMILanguageHandler(ApplicationManager& application_manager)
    : event_engine::EventObserver(application_manager.event_dispatcher())
    , capabilities_ui_language_(hmi_apis::Common_Language::INVALID_ENUM)
    , capabilities_vr_language_(hmi_apis::Common_Language::INVALID_ENUM)
    , capabilities_tts_language_(hmi_apis::Common_Language::INVALID_ENUM)
    , is_ui_language_received_(false)
    , is_vr_language_received_(false)
    , is_tts_language_received_(false)
    , application_manager_(application_manager) {
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnAppRegistered);
}

void HMILanguageHandler::set_language_for(
    HMILanguageHandler::Interface interface,
    hmi_apis::Common_Language::eType language) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string key = "UNKNOWN";
  switch (interface) {
    case INTERFACE_UI:
      key = UIKey;
      break;
    case INTERFACE_VR:
      key = VRKey;
      break;
    case INTERFACE_TTS:
      key = TTSKey;
      break;
    default:
      LOG4CXX_WARN(logger_, "Unknown interface has been passed " << interface);
      return;
  }
  LOG4CXX_DEBUG(logger_,
                "Setting language " << language << " for interface "
                                    << interface);
  last_state_->dictionary[LanguagesKey][key] = language;
  return;
}

hmi_apis::Common_Language::eType HMILanguageHandler::get_language_for(
    HMILanguageHandler::Interface interface) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace resumption;
  using namespace hmi_apis;
  std::string key = "UNKNOWN";
  switch (interface) {
    case INTERFACE_UI:
      key = UIKey;
      break;
    case INTERFACE_VR:
      key = VRKey;
      break;
    case INTERFACE_TTS:
      key = TTSKey;
      break;
    default:
      LOG4CXX_WARN(logger_, "Unknown interfcase has been passed " << interface);
      return Common_Language::INVALID_ENUM;
  }

  if (last_state_->dictionary.isMember(LanguagesKey)) {
    if (last_state_->dictionary[LanguagesKey].isMember(key)) {
      Common_Language::eType language = static_cast<Common_Language::eType>(
          last_state_->dictionary[LanguagesKey][key].asInt());
      return language;
    }
  }
  return Common_Language::INVALID_ENUM;
}

void HMILanguageHandler::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject msg = event.smart_object();
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
    case hmi_apis::FunctionID::BasicCommunication_OnAppRegistered:
      if (!(msg[strings::params].keyExists(strings::app_id))) {
        LOG4CXX_ERROR(logger_, "Message doesn't contain parameter app_id");
        return;
      }
      CheckApplication(
          std::make_pair(msg[strings::params][strings::app_id].asUInt(), true));
      return;
    default:
      return;
  }

  if (is_ui_language_received_ && is_vr_language_received_ &&
      is_tts_language_received_) {
    LOG4CXX_DEBUG(logger_, "All GetLanguages responses gotten.");
    VerifyWithPersistedLanguages();
  }
}

void HMILanguageHandler::set_handle_response_for(
    const smart_objects::SmartObject& request) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  if (!request.keyExists(strings::params)) {
    LOG4CXX_ERROR(logger_,
                  "Object does not have " << strings::params << " key.");
    return;
  }

  if (!request[strings::params].keyExists(strings::function_id)) {
    LOG4CXX_ERROR(logger_,
                  "Object does not have " << strings::function_id << " key.");
    return;
  }

  if (!request[strings::params].keyExists(strings::correlation_id)) {
    LOG4CXX_ERROR(
        logger_, "Object does not have " << strings::correlation_id << " key.");
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
    LOG4CXX_ERROR(logger_,
                  "Only *GetLanguage request are allowed to be subscribed.");
    return;
  }

  uint32_t correlation_id =
      request[strings::params][strings::correlation_id].asUInt();

  subscribe_on_event(function_id, correlation_id);

  LOG4CXX_DEBUG(logger_,
                "Subscribed for function_id "
                    << function_id << " and correlation_id " << correlation_id);
}

void HMILanguageHandler::set_default_capabilities_languages(
    hmi_apis::Common_Language::eType ui,
    hmi_apis::Common_Language::eType vr,
    hmi_apis::Common_Language::eType tts) {
  capabilities_ui_language_ = ui;
  if (hmi_apis::Common_Language::INVALID_ENUM == persisted_ui_language_) {
    persisted_ui_language_ = ui;
  }

  capabilities_vr_language_ = vr;
  if (hmi_apis::Common_Language::INVALID_ENUM == persisted_vr_language_) {
    persisted_vr_language_ = vr;
  }

  capabilities_tts_language_ = tts;
  if (hmi_apis::Common_Language::INVALID_ENUM == persisted_tts_language_) {
    persisted_tts_language_ = tts;
  }
}

void HMILanguageHandler::SendOnLanguageChangeToMobile(
    const uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObjectSPtr notification = new smart_objects::SmartObject;
  DCHECK_OR_RETURN_VOID(notification);
  smart_objects::SmartObject& message = *notification;
  message[strings::params][strings::function_id] =
      static_cast<int32_t>(mobile_api::FunctionID::OnLanguageChangeID);
  message[strings::params][strings::message_type] =
      static_cast<int32_t>(kNotification);
  message[strings::params][strings::connection_key] = connection_key;

  HMICapabilities& hmi_capabilities = application_manager_.hmi_capabilities();
  message[strings::msg_params][strings::hmi_display_language] =
      hmi_capabilities.active_ui_language();
  message[strings::msg_params][strings::language] =
      hmi_capabilities.active_vr_language();
  if (application_manager_.ManageMobileCommand(notification,
                                               commands::Command::ORIGIN_SDL)) {
    LOG4CXX_INFO(logger_, "Mobile command sent");
  } else {
    LOG4CXX_WARN(logger_, "Cannot send mobile command");
  }
}

void HMILanguageHandler::VerifyWithPersistedLanguages() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  const HMICapabilities& hmi_capabilities =
      application_manager_.hmi_capabilities();

  // Updated values compared with persisted
  if (hmi_capabilities.active_ui_language() == persisted_ui_language_ &&
      hmi_capabilities.active_vr_language() == persisted_vr_language_ &&
      hmi_capabilities.active_tts_language() == persisted_tts_language_) {
    LOG4CXX_INFO(logger_,
                 "All languages gotten from HMI match to persisted values.");
    return;
  }

  LOG4CXX_INFO(logger_,
               "Some languages gotten from HMI have "
               "mismatch with persisted values.");

  const ApplicationSet& accessor =
      application_manager_.applications().GetData();
  ApplicationSetIt it = accessor.begin();
  for (; accessor.end() != it;) {
    ApplicationConstSharedPtr app = *it++;

    LOG4CXX_INFO(logger_,
                 "Application with app_id "
                     << app->app_id() << " will be unregistered because of "
                                         "HMI language(s) mismatch.");

    CheckApplication(std::make_pair(app->app_id(), false));
  }

  sync_primitives::AutoLock lock(apps_lock_);
  if (0 == apps_.size()) {
    LOG4CXX_DEBUG(logger_,
                  "No registered apps found. HMILanguageHandler unsubscribed "
                  "from all events.");
    unsubscribe_from_all_events();
  }
}

void HMILanguageHandler::HandleWrongLanguageApp(const Apps::value_type& app) {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock lock(apps_lock_);
    Apps::iterator it = apps_.find(app.first);
    if (apps_.end() == it) {
      LOG4CXX_DEBUG(logger_,
                    "Application id "
                        << app.first
                        << " is not found within apps with wrong language.");
      return;
    }
    apps_.erase(it);
    if (0 == apps_.size()) {
      LOG4CXX_DEBUG(logger_,
                    "HMILanguageHandler unsubscribed from all events.");
      unsubscribe_from_all_events();
    }
  }
  SendOnLanguageChangeToMobile(app.first);
  application_manager_.ManageMobileCommand(
      MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
          app.first,
          mobile_api::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE),
      commands::Command::ORIGIN_SDL);
  application_manager_.UnregisterApplication(
      app.first, mobile_apis::Result::SUCCESS, false);
  LOG4CXX_INFO(logger_,
               "Unregistering application with app_id "
                   << app.first << " because of HMI language(s) mismatch.");
}

void HMILanguageHandler::CheckApplication(const Apps::value_type app) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool is_need_handle_wrong_language = false;
  {
    sync_primitives::AutoLock lock(apps_lock_);
    Apps::iterator it = apps_.find(app.first);
    if (apps_.end() == it) {
      LOG4CXX_INFO(logger_,
                   "Adding application id "
                       << app.first
                       << " Application registered: " << app.second);
      apps_.insert(app);
      return;
    }
    is_need_handle_wrong_language = apps_[app.first];
  }
  if (is_need_handle_wrong_language) {
    HandleWrongLanguageApp(app);
  }
}

void HMILanguageHandler::Init(resumption::LastState* value) {
  last_state_ = value;
  persisted_ui_language_ = get_language_for(INTERFACE_UI);
  persisted_vr_language_ = get_language_for(INTERFACE_VR);
  persisted_tts_language_ = get_language_for(INTERFACE_TTS);
}

void HMILanguageHandler::OnUnregisterApplication(uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(apps_lock_);
  apps_.erase(app_id);
}

}  // namespace application_manager
