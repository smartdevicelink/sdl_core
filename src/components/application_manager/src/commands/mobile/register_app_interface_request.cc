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

#include "application_manager/commands/mobile/register_app_interface_request.h"

#include <unistd.h>
#include <algorithm>

#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "config_profile/profile.h"
#include "interfaces/MOBILE_API.h"

namespace application_manager {

namespace commands {

RegisterAppInterfaceRequest::RegisterAppInterfaceRequest(
  const MessageSharedPtr& message)
  : CommandRequestImpl(message) {
}

RegisterAppInterfaceRequest::~RegisterAppInterfaceRequest() {
}

bool RegisterAppInterfaceRequest::Init() {
  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::Init");
  return true;
}

void RegisterAppInterfaceRequest::Run() {
  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::Run "<< connection_key());

  // wait till all HMI capabilities initialized
  const HMICapabilities& hmi_capabilities =
      ApplicationManagerImpl::instance()->hmi_capabilities();
  while (!hmi_capabilities.is_hmi_capabilities_initialized()) {
    sleep(1);
    // TODO(DK): timer_->StartWait(1);
    ApplicationManagerImpl::instance()->updateRequestTimeout(connection_key(),
                                                             correlation_id(),
                                                             default_timeout());
  }

  ApplicationSharedPtr application =
        ApplicationManagerImpl::instance()->application(connection_key());

  if (application) {
    SendResponse(false, mobile_apis::Result::APPLICATION_REGISTERED_ALREADY);
    return;
  }

  if (IsApplicationWithSameAppIdRegistered()) {
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  mobile_apis::Result::eType restriction_result = CheckRestrictions();
  if (mobile_apis::Result::SUCCESS != restriction_result) {
    LOG4CXX_ERROR_EXT(logger_, "Param names restrictions check failed.");
    SendResponse(false, restriction_result);
    return;
  }

  mobile_apis::Result::eType coincidence_result =
      CheckCoincidence();

  if (mobile_apis::Result::SUCCESS != coincidence_result) {
    LOG4CXX_ERROR_EXT(logger_, "Coincidence check failed.");
    SendResponse(false, coincidence_result);
    return;
  }

  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->RegisterApplication(message_);

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "Application " <<
       msg_params[strings::app_name].asString() << "  hasn't been registered!");
  } else {
    app->set_is_media_application(
        msg_params[strings::is_media_application].asBool());

    if (msg_params.keyExists(strings::vr_synonyms)) {
      app->set_vr_synonyms(msg_params[strings::vr_synonyms]);
    }

    if (msg_params.keyExists(strings::ngn_media_screen_app_name)) {
      app->set_ngn_media_screen_name(
          msg_params[strings::ngn_media_screen_app_name]);
    }

    if (msg_params.keyExists(strings::tts_name)) {
      app->set_tts_name(msg_params[strings::tts_name]);
    }

    if (msg_params.keyExists(strings::app_hmi_type)) {
      app->set_app_types(msg_params[strings::app_hmi_type]);

      // check if app is NAVI
      const int32_t is_navi_type = mobile_apis::AppHMIType::NAVIGATION;
      const smart_objects::SmartObject& app_type =
          msg_params.getElement(strings::app_hmi_type);

      for (size_t i = 0; i < app_type.length(); ++i) {
        if (is_navi_type == app_type.getElement(i).asInt()) {
          app->set_allowed_support_navigation(true);
        }
      }
    }

    MessageHelper::SendOnAppRegisteredNotificationToHMI(*app);
    SendRegisterAppInterfaceResponseToMobile();

  }
}

void RegisterAppInterfaceRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::on_event");
  switch (event.id()) {
    case hmi_apis::FunctionID::TTS_Speak: {
      const smart_objects::SmartObject& message = event.smart_object();

      mobile_apis::Result::eType tts_result =
                static_cast<mobile_apis::Result::eType>(
                message[strings::params][hmi_response::code].asInt());

      SendRegisterAppInterfaceResponseToMobile(tts_result);
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      break;
    }
  }
}


void RegisterAppInterfaceRequest::SendRegisterAppInterfaceResponseToMobile(
    mobile_apis::Result::eType result) {
  smart_objects::SmartObject* params = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  if (!params) {
    SendResponse(false, mobile_apis::Result::OUT_OF_MEMORY);
    return;
  }

  ApplicationManagerImpl* app_manager = ApplicationManagerImpl::instance();
  const HMICapabilities& hmi_capabilities = app_manager->hmi_capabilities();
  ApplicationSharedPtr application =
      ApplicationManagerImpl::instance()->application(connection_key());

  smart_objects::SmartObject& response_params = *params;

  response_params[strings::sync_msg_version][strings::major_version] =
    APIVersion::kAPIV3;
  response_params[strings::sync_msg_version][strings::minor_version] =
    APIVersion::kAPIV0;

  response_params[strings::language] = hmi_capabilities.active_vr_language();
  response_params[strings::hmi_display_language] =
      hmi_capabilities.active_ui_language();

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  if (msg_params[strings::language_desired].asInt() !=
          hmi_capabilities.active_vr_language() ||
      msg_params[strings::hmi_display_language_desired].asInt() !=
          hmi_capabilities.active_ui_language()) {

    LOG4CXX_WARN_EXT(
      logger_,
      "Wrong language on registering application " << application->name());

    LOG4CXX_ERROR_EXT(
      logger_,
      "vr "
      << msg_params[strings::language_desired].asInt()
      << " - "
      << hmi_capabilities.active_vr_language()
      << "ui "
      << msg_params[strings::hmi_display_language_desired].asInt()
      << " - "
      << hmi_capabilities.active_ui_language());

    result = mobile_apis::Result::WRONG_LANGUAGE;
  }

  if (hmi_capabilities.display_capabilities()) {

    response_params[hmi_response::display_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

    smart_objects::SmartObject& display_caps =
      response_params[hmi_response::display_capabilities];

    display_caps[hmi_response::display_type] =
        hmi_capabilities.display_capabilities()->getElement(
        hmi_response::display_type);

    display_caps[hmi_response::text_fields] =
        hmi_capabilities.display_capabilities()->getElement(
        hmi_response::text_fields);

    display_caps[hmi_response::image_fields] =
        hmi_capabilities.display_capabilities()->getElement(
        hmi_response::image_fields);

    display_caps[hmi_response::media_clock_formats] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::media_clock_formats);

    display_caps[hmi_response::templates_available] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::templates_available);

    display_caps[hmi_response::screen_params] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::screen_params);

    display_caps[hmi_response::num_custom_presets_available] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::num_custom_presets_available);

    if (hmi_capabilities.display_capabilities()->getElement(
          hmi_response::image_capabilities).length() > 0) {

      display_caps[hmi_response::graphic_supported] = true;
    } else {

      display_caps[hmi_response::graphic_supported] = false;
    }

    display_caps[hmi_response::templates_available] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::templates_available);

    display_caps[hmi_response::screen_params] =
        hmi_capabilities.display_capabilities()->getElement(
            hmi_response::screen_params);

    display_caps[hmi_response::num_custom_presets_available] =
        hmi_capabilities.display_capabilities()->getElement(
                hmi_response::num_custom_presets_available);
  }

  if (hmi_capabilities.button_capabilities()) {
    response_params[hmi_response::button_capabilities] =
        *hmi_capabilities.button_capabilities();
  }
  if (hmi_capabilities.soft_button_capabilities()) {
    response_params[hmi_response::soft_button_capabilities] =
        *hmi_capabilities.soft_button_capabilities();
  }
  if (hmi_capabilities.preset_bank_capabilities()) {
    response_params[hmi_response::preset_bank_capabilities] =
        *hmi_capabilities.preset_bank_capabilities();
  }
  if (hmi_capabilities.hmi_zone_capabilities()) {
    response_params[hmi_response::hmi_zone_capabilities] =
        *hmi_capabilities.hmi_zone_capabilities();
  }
  if (hmi_capabilities.speech_capabilities()) {
    response_params[strings::speech_capabilities] =
        *hmi_capabilities.speech_capabilities();
  }
  if (hmi_capabilities.vr_capabilities()) {
    response_params[strings::vr_capabilities] =
        *hmi_capabilities.vr_capabilities();
  }
  if (hmi_capabilities.audio_pass_thru_capabilities()) {
    response_params[strings::audio_pass_thru_capabilities] =
        *hmi_capabilities.audio_pass_thru_capabilities();
  }
  if (hmi_capabilities.vehicle_type()) {
    response_params[hmi_response::vehicle_type] =
        *hmi_capabilities.vehicle_type();
  }
  if (hmi_capabilities.prerecorded_speech()) {
    response_params[strings::prerecorded_speech] =
        *(hmi_capabilities.prerecorded_speech());
  }

  const std::vector<uint32_t>& diag_modes =
      profile::Profile::instance()->supported_diag_modes();
  if (!diag_modes.empty()) {
    std::vector<uint32_t>::const_iterator it = diag_modes.begin();
    uint32_t index = 0;
    for (; it != diag_modes.end(); ++it) {
      response_params[strings::supported_diag_modes][index] = *it;
      ++index;
    }
  }

  ResumeCtrl& resumer = ApplicationManagerImpl::instance()->resume_controller();
  uint32_t hash_id = 0;

  const char* add_info = "";
  if ((*message_)[strings::msg_params].keyExists(strings::hash_id)) {

    hash_id = (*message_)[strings::msg_params][strings::hash_id].asUInt();
    const std::string& mobile_app_id = (*application->mobile_app_id()).asString();
    if (!resumer.CheckApplicationHash(mobile_app_id,
                                      hash_id)) {
      result = mobile_apis::Result::RESUME_FAILED;
      LOG4CXX_WARN(logger_, "Hash does not maches");
      add_info = "Hash does not maches";
    } else if (!resumer.CheckPersistenceFilesForResumption(application)) {
      result = mobile_apis::Result::RESUME_FAILED;
      LOG4CXX_WARN(logger_, "Persistent data is missed");
      add_info = "Persistent data is missed";
    } else {
      add_info = " Resume Succesed";
    }
  }

  SendResponse(true, result, add_info, params);
  if (result != mobile_apis::Result::RESUME_FAILED) {
    resumer.StartResumption(application, hash_id);
  } else {
    resumer.StartResumptionOnlyHMILevel(application);
  }

}

mobile_apis::Result::eType
RegisterAppInterfaceRequest::CheckCoincidence() {

  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::CheckCoincidence ");

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  ApplicationManagerImpl* app_manager = ApplicationManagerImpl::instance();

  const std::set<ApplicationSharedPtr>& applications = app_manager->applications();
  std::set<ApplicationSharedPtr>::const_iterator it = applications.begin();
  const std::string app_name = msg_params[strings::app_name].asString();

  for (; applications.end() != it; ++it) {

    // name check
    const std::string &cur_name = (*it)->name();
    if (!strcasecmp(app_name.c_str(), cur_name.c_str())) {
      LOG4CXX_ERROR(logger_, "Application name is known already.");
      return mobile_apis::Result::DUPLICATE_NAME;
    }

    const smart_objects::SmartObject* tts = (*it)->tts_name();
    std::vector<smart_objects::SmartObject>* curr_tts = NULL;
    if (NULL != tts) {
      curr_tts = tts->asArray();
      CoincidencePredicateTTS t(app_name);

      if (0 != std::count_if((*curr_tts).begin(), (*curr_tts).end(), t)) {
        LOG4CXX_ERROR(logger_, "Application name is known already.");
        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }

    const smart_objects::SmartObject* vr = (*it)->vr_synonyms();
    const std::vector<smart_objects::SmartObject>* curr_vr = NULL;
    if (NULL != vr) {
      curr_vr = vr->asArray();
      CoincidencePredicateVR v(app_name);

      if (0 != std::count_if(curr_vr->begin(), curr_vr->end(), v )) {
        LOG4CXX_ERROR(logger_, "Application name is known already.");
        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }


    // tts check
    if (msg_params.keyExists(strings::tts_name)) {

      const std::vector<smart_objects::SmartObject>* new_tts =
          msg_params[strings::tts_name].asArray();

      std::vector<smart_objects::SmartObject>::const_iterator it_tts =
          new_tts->begin();

      std::vector<smart_objects::SmartObject>::const_iterator it_tts_End =
          new_tts->end();

      for (; it_tts != it_tts_End; ++it_tts) {
        std::string text = (*it_tts)[strings::text].asString();
        if (!strcasecmp(cur_name.c_str(), text.c_str())){
          LOG4CXX_ERROR(logger_,
                        "Some TTS parameters names are known already.");
          return mobile_apis::Result::DUPLICATE_NAME;
        }

        CoincidencePredicateTTS t((*it_tts)[strings::text].asString());
        if (NULL != curr_tts
            &&  0 != std::count_if(curr_tts->begin(), curr_tts->end(), t)) {
          LOG4CXX_ERROR(logger_,
                        "Some TTS parameters names are known already.");
          return mobile_apis::Result::DUPLICATE_NAME;
        }

        CoincidencePredicateVR v((*it_tts)[strings::text].asString());
        if (NULL != curr_vr
            &&  0 != std::count_if(curr_vr->begin(), curr_vr->end(), v)) {
          LOG4CXX_ERROR(logger_,
                        "Some TTS parameters names are known already.");
          return mobile_apis::Result::DUPLICATE_NAME;
        }
      }
    }  // end tts check

    if (msg_params.keyExists(strings::vr_synonyms)) {

      const std::vector<smart_objects::SmartObject>* new_vr =
          msg_params[strings::vr_synonyms].asArray();

      std::vector<smart_objects::SmartObject>::const_iterator it_vr =
          new_vr->begin();

      std::vector<smart_objects::SmartObject>::const_iterator it_vr_End =
          new_vr->end();

      for (; it_vr != it_vr_End; ++it_vr) {
        std::string vr_synonym = it_vr->asString();
        if (!strcasecmp(cur_name.c_str(), vr_synonym.c_str())) {
          LOG4CXX_ERROR(logger_, "Some VR synonyms are known already.");
          return mobile_apis::Result::DUPLICATE_NAME;
        }

        CoincidencePredicateTTS t(it_vr->asString());
        if (NULL !=curr_tts
            &&  0 != std::count_if(curr_tts->begin(), curr_tts->end(), t)) {
          LOG4CXX_ERROR(logger_, "Some VR synonyms are known already.");
          return mobile_apis::Result::DUPLICATE_NAME;
        }

        CoincidencePredicateVR v(it_vr->asString());
        if (NULL != curr_vr
            &&  0 != std::count_if(curr_vr->begin(), curr_vr->end(), v)) {
          LOG4CXX_ERROR(logger_, "Some VR synonyms are known already.");
          return mobile_apis::Result::DUPLICATE_NAME;
        }
      }
    }  // end vr check

  }  // application for end

  return mobile_apis::Result::SUCCESS;
}  // method end

mobile_apis::Result::eType
RegisterAppInterfaceRequest::CheckRestrictions() const {

  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::CheckRestrictions");

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  const std::string& app_name = msg_params[strings::app_name].asString();

  if (ClearParamName(app_name).empty()) {
    printf("Application name is empty.\n");
    return mobile_apis::Result::INVALID_DATA;
  }

  if ((app_name[0] == '\n') ||
      ((app_name[0] == '\\') && (app_name[1] == 'n'))) {

    printf("Application name has invalid characters.");
    return mobile_apis::Result::INVALID_DATA;
  }

  if (msg_params.keyExists(strings::tts_name)) {

    const smart_objects::SmartArray* tts =
        msg_params[strings::tts_name].asArray();

    smart_objects::SmartArray::const_iterator it = tts->begin();
    smart_objects::SmartArray::const_iterator it_end = tts->end();

    for (; it != it_end; ++it) {

      const std::string& tts_name = (*it)[strings::text].asString();

      if (ClearParamName(tts_name).empty()) {
        printf("TTS value is empty.");
        return mobile_apis::Result::INVALID_DATA;
      }

      if ((tts_name[0] == '\n') ||
          ((tts_name[0] == '\\') && (tts_name[1] == 'n'))) {

        printf("TTS value(s) has invalid characters.");
        return mobile_apis::Result::INVALID_DATA;
      }
    }
  }

  return mobile_apis::Result::SUCCESS;
}

std::string
RegisterAppInterfaceRequest::ClearParamName(std::string param_name) const {

  // Expecting for chars different from newlines and spaces in the appName
  //
  // There is an agreement, that "\n" is not allowed symbols, so we have to
  // check for this case also

  std::string newline = "\\n";
  while (std::string::npos != param_name.find(newline)) {
    param_name.erase(param_name.find(newline), newline.length());
  }

  std::string::iterator param_name_new_end =
      std::remove_if(param_name.begin(), param_name.end(), ::isspace);

  return std::string(param_name.begin(), param_name_new_end);
}

bool RegisterAppInterfaceRequest::IsApplicationWithSameAppIdRegistered() {

  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::IsApplicationRegistered");

  const std::string& mobile_app_id = (*message_)[strings::msg_params][strings::app_id].asString();

  const std::set<ApplicationSharedPtr>& applications =
      ApplicationManagerImpl::instance()->applications();

  std::set<ApplicationSharedPtr>::const_iterator it = applications.begin();
  std::set<ApplicationSharedPtr>::const_iterator it_end = applications.end();

  for (; it != it_end; ++it) {
    if (mobile_app_id == (*it)->mobile_app_id()->asString()) {
      return true;
    }
  }

  return false;
}


}  // namespace commands

}  // namespace application_manager
