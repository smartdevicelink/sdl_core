/*

 Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/mobile/change_registration_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include <algorithm>

namespace application_manager {

namespace commands {

ChangeRegistrationRequest::ChangeRegistrationRequest(
    const MessageSharedPtr& message)
    : CommandRequestImpl(message),
      is_ui_send_(false),
      is_vr_send_(false),
      is_tts_send_(false),
      is_ui_received_(false),
      is_vr_received_(false),
      is_tts_received_(false),
      ui_result_(hmi_apis::Common_Result::INVALID_ENUM),
      tts_result_(hmi_apis::Common_Result::INVALID_ENUM),
      vr_result_(hmi_apis::Common_Result::INVALID_ENUM) {
}

ChangeRegistrationRequest::~ChangeRegistrationRequest() {
}

void ChangeRegistrationRequest::Run() {
  LOG4CXX_INFO(logger_, "ChangeRegistrationRequest::Run");

  ApplicationManagerImpl* instance = ApplicationManagerImpl::instance();

  Application* app = instance->application(
      (*message_)[strings::params][strings::connection_key]);

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const int hmi_language =
      (*message_)[strings::msg_params][strings::hmi_display_language].asInt();

  const int language =
      (*message_)[strings::msg_params][strings::language].asInt();

  hmi_apis::Common_Result::eType result = hmi_apis::Common_Result::INVALID_ENUM;

  bool ui_flag = false;
  bool vr_flag = false;
  bool tts_flag = false;

  if (instance->is_ui_cooperating()) {
    if (false == IsLanguageSupportedByUI(hmi_language)) {
      LOG4CXX_ERROR(logger_, "Language is not supported");
      SendResponse(false, mobile_apis::Result::REJECTED);
      return;
    }

    if (app->ui_language() != hmi_language) {
      ui_flag = true;
    }
  } else {
   ui_result_ = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  }

  if (instance->is_vr_cooperating()) {
    if (false == IsLanguageSupportedByVR(language)) {
      LOG4CXX_ERROR(logger_, "Language is not supported");
      SendResponse(false, mobile_apis::Result::REJECTED);
      return;
    }
    if (app->language() != language) {
      vr_flag = true;
    }
  } else {
    vr_result_ = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  }

  if (instance->is_tts_cooperating()) {
    if (false == IsLanguageSupportedByTTS(language)) {
      LOG4CXX_ERROR(logger_, "Language is not supported");
      SendResponse(false, mobile_apis::Result::REJECTED);
      return;
    }
    if (app->language() != language) {
      tts_flag = true;
    }
  } else {
    tts_result_ = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  }

  if (vr_flag) {
    is_vr_send_ = true;
  }

  if (tts_flag) {
    is_tts_send_ = true;
  }

  if (ui_flag) {
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);

    msg_params[strings::language] = hmi_language;
    msg_params[strings::app_id] = app->app_id();

    is_ui_send_ = true;
    SendHMIRequest(hmi_apis::FunctionID::UI_ChangeRegistration,
                   &msg_params, true);
  }

  if (vr_flag) {
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);
    msg_params[strings::language] =
        (*message_)[strings::msg_params][strings::language];
    msg_params[strings::app_id] = app->app_id();

    SendHMIRequest(hmi_apis::FunctionID::VR_ChangeRegistration,
                   &msg_params, true);
  }

  if (tts_flag) {
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);
    msg_params[strings::language] =
        (*message_)[strings::msg_params][strings::language];
    msg_params[strings::app_id] = app->app_id();

    SendHMIRequest(hmi_apis::FunctionID::TTS_ChangeRegistration,
                   &msg_params, true);
  }

  if (!ui_flag && !vr_flag && !tts_flag) {
    LOG4CXX_ERROR(logger_, "Current language is the same");
    SendResponse(false, mobile_apis::Result::REJECTED,
                 "Current language is the same");
    return;
  }
}

bool ChangeRegistrationRequest::WasAnySuccess(const hmi_apis::Common_Result::eType ui,
                   const hmi_apis::Common_Result::eType vr,
                   const hmi_apis::Common_Result::eType tts) {
  if (hmi_apis::Common_Result::SUCCESS == ui) {
    return true;
  }
  if (hmi_apis::Common_Result::SUCCESS == vr) {
    return true;
  }
  if (hmi_apis::Common_Result::SUCCESS == tts) {
    return true;
  }
  return false;
}

void ChangeRegistrationRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "ChangeRegistrationRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_ChangeRegistration: {
      LOG4CXX_INFO(logger_, "Received UI_ChangeRegistration event");
      is_ui_received_ = true;
      ui_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      break;
    }
    case hmi_apis::FunctionID::VR_ChangeRegistration: {
      LOG4CXX_INFO(logger_, "Received VR_ChangeRegistration event");
      is_vr_received_ = true;
      vr_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      break;
    }
    case hmi_apis::FunctionID::TTS_ChangeRegistration: {
      LOG4CXX_INFO(logger_, "Received TTS_ChangeRegistration event");
      is_tts_received_ = true;
      tts_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      return;
    }
  }

  if (!IsPendingResponseExist()) {
    Application* application = ApplicationManagerImpl::instance()->application(
        (*message_)[strings::params][strings::connection_key]);

    if (NULL == application) {
      LOG4CXX_ERROR(logger_, "NULL pointer");
      return;
    }

    if (hmi_apis::Common_Result::SUCCESS == ui_result_) {
      application->set_ui_language(static_cast<mobile_api::Language::eType>(
          (*message_)[strings::msg_params][strings::hmi_display_language].asInt()));
    }

    if (hmi_apis::Common_Result::SUCCESS == vr_result_
        || hmi_apis::Common_Result::SUCCESS == tts_result_) {
      application->set_language(
          static_cast<mobile_api::Language::eType>(
              (*message_)[strings::msg_params]
                          [strings::language].asInt()));
    }

    int greates_result_code = std::max(std::max(ui_result_, vr_result_),
                                       tts_result_);

    (*message_)[strings::params][strings::function_id] =
          mobile_apis::FunctionID::eType::ChangeRegistrationID;

    SendResponse(WasAnySuccess(ui_result_, vr_result_, tts_result_),
                 static_cast<mobile_apis::Result::eType>(greates_result_code),
                 NULL, &(message[strings::msg_params]));
  }
}

bool ChangeRegistrationRequest::IsLanguageSupportedByUI(
    const int& hmi_display_lang) {
  const smart_objects::SmartObject* ui_languages =
      ApplicationManagerImpl::instance()->ui_supported_languages();

  if (!ui_languages) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return false;
  }

  bool is_language_supported = false;
  for (size_t i = 0; i < ui_languages->length(); ++i) {
    if (hmi_display_lang == ui_languages->getElement(i).asInt()) {
      is_language_supported = true;
      break;
    }
  }

  if (false == is_language_supported) {
    LOG4CXX_ERROR(logger_, "Language isn't supported by UI");
  }
  return is_language_supported;
}

bool ChangeRegistrationRequest::IsLanguageSupportedByVR(
    const int& hmi_display_lang) {
  const smart_objects::SmartObject* vr_languages =
      ApplicationManagerImpl::instance()->vr_supported_languages();

  if (!vr_languages) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return false;
  }

  bool is_language_supported = false;
  for (size_t i = 0; i < vr_languages->length(); ++i) {
    if (hmi_display_lang == vr_languages->getElement(i).asInt()) {
      is_language_supported = true;
      break;
    }
  }

  if (false == is_language_supported) {
    LOG4CXX_ERROR(logger_, "Language isn't supported by VR");
  }
  return is_language_supported;
}

bool ChangeRegistrationRequest::IsLanguageSupportedByTTS(
    const int& hmi_display_lang) {
  const smart_objects::SmartObject* tts_languages =
      ApplicationManagerImpl::instance()->tts_supported_languages();

  if (!tts_languages) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return false;
  }

  bool is_language_supported = false;
  for (size_t i = 0; i < tts_languages->length(); ++i) {
    if (hmi_display_lang == tts_languages->getElement(i).asInt()) {
      is_language_supported = true;
      break;
    }
  }

  if (false == is_language_supported) {
    LOG4CXX_ERROR(logger_, "Language isn't supported by TTS");
  }
  return is_language_supported;
}

bool ChangeRegistrationRequest::IsPendingResponseExist() {
  if (is_ui_send_ != is_ui_received_) {
    return true;
  }

  if (is_vr_send_ != is_vr_received_) {
    return true;
  }

  if (is_tts_send_ != is_tts_received_) {
    return true;
  }

  return false;
}

}  // namespace commands

}  // namespace application_manager
