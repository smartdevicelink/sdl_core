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

#include <string.h>
#include <algorithm>
#include "application_manager/commands/mobile/register_app_interface_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"

namespace application_manager {

namespace commands {

RegisterAppInterfaceRequest::RegisterAppInterfaceRequest(
  const MessageSharedPtr& message)
  : CommandRequestImpl(message),
    timer_(NULL) {
}

RegisterAppInterfaceRequest::~RegisterAppInterfaceRequest() {
  if (timer_) {
    delete timer_;
  }
}

bool RegisterAppInterfaceRequest::Init() {
  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::Init");

  synchronisation_.init();
  timer_ = new sync_primitives::Timer(&synchronisation_);
  if (!timer_) {
    LOG4CXX_ERROR_EXT(logger_, "Init NULL pointer");
    return false;
  }
  return true;
}

void RegisterAppInterfaceRequest::Run() {
  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::Run "<< connection_key());

  // wait till all HMI capabilities initialized
  while (!ApplicationManagerImpl::instance()->IsHMICapabilitiesInitialized()) {
    sleep(1);
    // TODO(DK): timer_->StartWait(1);
    ApplicationManagerImpl::instance()->updateRequestTimeout(connection_key(),
                                                             correlation_id(),
                                                             default_timeout());
  }

  if (CheckCoincidence()) {
    LOG4CXX_ERROR_EXT(logger_, "DUPLICATE_NAME");
    SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
    return;
  }

  Application* app =
      ApplicationManagerImpl::instance()->RegisterApplication(message_);

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "Application " <<
                      msg_params[strings::app_name].asString() <<
                      "  hasn't been registered!");
  } else {
    app->set_mobile_app_id(msg_params[strings::app_id]);
    app->set_is_media_application(msg_params[strings::is_media_application].asBool());

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
      const int is_navi_type = mobile_apis::AppHMIType::NAVIGATION;
      const smart_objects::SmartObject& app_type =
          msg_params.getElement(strings::app_hmi_type);

      for (size_t i = 0; i < app_type.length(); ++i) {
        if (is_navi_type == app_type.getElement(i).asInt()) {
          app->set_allowed_support_navigation(true);
        }
      }
    }

    SendRegisterAppInterfaceResponseToMobile(*app);
    MessageHelper::SendOnAppRegisteredNotificationToHMI(*app);
    MessageHelper::SendHMIStatusNotification(*app);
    MessageHelper::SendVrCommandsOnRegisterAppToHMI(app);
  }
}

void RegisterAppInterfaceRequest::SendRegisterAppInterfaceResponseToMobile(
  const Application& application_impl) {
  mobile_apis::Result::eType result = mobile_apis::Result::SUCCESS;
  smart_objects::SmartObject* params = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  if (!params) {
    SendResponse(false, mobile_apis::Result::OUT_OF_MEMORY);
    return;
  }

  smart_objects::SmartObject& response_params = *params;

  ApplicationManagerImpl* app_manager = ApplicationManagerImpl::instance();

  response_params[strings::sync_msg_version][strings::major_version] =
    APIVersion::kAPIV2;
  response_params[strings::sync_msg_version][strings::minor_version] =
    APIVersion::kAPIV2;

  response_params[strings::language] = app_manager->active_vr_language();
  response_params[strings::hmi_display_language] = app_manager
      ->active_ui_language();

  if ((*message_)[strings::msg_params][strings::language_desired].asInt()
      != app_manager->active_vr_language()
      || (*message_)[strings::msg_params][strings::hmi_display_language_desired]
      .asInt() != app_manager->active_ui_language()) {
    LOG4CXX_WARN_EXT(
      logger_,
      "Wrong language on registering application " << application_impl.name());
    LOG4CXX_ERROR_EXT(
      logger_,
      "vr " << (*message_)[strings::msg_params] [strings::language_desired].asInt() << " - " << app_manager->active_vr_language() << "ui " << (*message_) [strings::msg_params][strings::hmi_display_language_desired].asInt() << " - " << app_manager->active_ui_language());
    result = mobile_apis::Result::WRONG_LANGUAGE;
  }

  if (app_manager->display_capabilities()) {
    response_params[hmi_response::display_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
    smart_objects::SmartObject& display_caps =
      response_params[hmi_response::display_capabilities];

    display_caps[hmi_response::display_type] =
      app_manager->display_capabilities()->getElement(
        hmi_response::display_type);
    display_caps[hmi_response::text_fields] =
      app_manager->display_capabilities()->getElement(
        hmi_response::text_fields);
    display_caps[hmi_response::media_clock_formats] = app_manager
        ->display_capabilities()->getElement(hmi_response::media_clock_formats);
    if (app_manager->display_capabilities()->getElement(
          hmi_response::image_capabilities).length() > 0) {
      display_caps[hmi_response::graphic_supported] = true;
    } else {
      display_caps[hmi_response::graphic_supported] = false;
    }
  }

  if (app_manager->button_capabilities()) {
    response_params[hmi_response::button_capabilities] = *app_manager
        ->button_capabilities();
  }
  if (app_manager->soft_button_capabilities()) {
    response_params[hmi_response::soft_button_capabilities] = *app_manager
        ->soft_button_capabilities();
  }
  if (app_manager->preset_bank_capabilities()) {
    response_params[hmi_response::preset_bank_capabilities] = *app_manager
        ->preset_bank_capabilities();
  }
  if (app_manager->hmi_zone_capabilities()) {
    response_params[hmi_response::hmi_zone_capabilities] = *app_manager
        ->hmi_zone_capabilities();
  }
  if (app_manager->speech_capabilities()) {
    response_params[strings::speech_capabilities] = *app_manager
        ->speech_capabilities();
  }
  if (app_manager->vr_capabilities()) {
    response_params[strings::vr_capabilities] = *app_manager->vr_capabilities();
  }
  if (app_manager->audio_pass_thru_capabilities()) {
    response_params[strings::audio_pass_thru_capabilities] = *app_manager
        ->audio_pass_thru_capabilities();
  }
  if (app_manager->vehicle_type()) {
    response_params[hmi_response::vehicle_type] = *app_manager->vehicle_type();
  }

  SendResponse(true, result, "", params);
}

bool RegisterAppInterfaceRequest::CheckCoincidence() {

  LOG4CXX_INFO(logger_, "RegisterAppInterfaceRequest::CheckCoincidence ");

    const smart_objects::SmartObject& msg_params =
        (*message_)[strings::msg_params];

    ApplicationManagerImpl* app_manager = ApplicationManagerImpl::instance();

    const std::set<Application*>& applications = app_manager->applications();
    std::set<Application*>::const_iterator it = applications.begin();

    for (; applications.end() != it; ++it) {

      // name check
      const std::string &curName = (*it)->name();
      const std::string appName = msg_params[strings::app_name].asString();
      if (appName == curName) {
        LOG4CXX_ERROR(logger_, "Application name is known already.");
        return true;
      }

      const smart_objects::SmartObject *tts = (*it)->tts_name();
      std::vector<smart_objects::SmartObject> *curTTS = NULL;
      if (NULL != tts) {
        curTTS = tts->asArray();
        CoincidencePredicateTTS t(appName);

        if (std::any_of((*curTTS).begin(), (*curTTS).end(), t) ) {
          LOG4CXX_ERROR(logger_, "Application name is known already.");
          return true;
        }
      }

      const smart_objects::SmartObject *vr = (*it)->vr_synonyms();
      const std::vector<smart_objects::SmartObject>* curVR = NULL;
      if (NULL != vr) {
        curVR = vr->asArray();
        CoincidencePredicateVR v(appName);

        if (std::any_of(curVR->begin(), curVR->end(), v )) {
          LOG4CXX_ERROR(logger_, "Application name is known already.");
          return true;
        }
      }


      // tts check
      if (    msg_params.keyExists(strings::tts_name)) {

        const std::vector<smart_objects::SmartObject> *newTTS = msg_params[strings::tts_name].asArray();

        std::vector<smart_objects::SmartObject>::const_iterator it = newTTS->begin();
        std::vector<smart_objects::SmartObject>::const_iterator itEnd = newTTS->end();

        for (; it != itEnd; ++it) {
          if (curName == (*it)[strings::text].asString()) {
            LOG4CXX_ERROR(logger_, "Some TTS parameters names are known already.");
            return true;
          }

          CoincidencePredicateTTS t((*it)[strings::text].asString());
          if (    NULL != curTTS
              &&  std::any_of(curTTS->begin(), curTTS->end(), t)) {
            LOG4CXX_ERROR(logger_, "Some TTS parameters names are known already.");
            return true;
          }

          CoincidencePredicateVR v((*it)[strings::text].asString());
          if (    NULL != curVR
              &&  std::any_of(curVR->begin(), curVR->end(), v)) {
            LOG4CXX_ERROR(logger_, "Some TTS parameters names are known already.");
            return true;
          }
        }
      } // end tts check

      if (msg_params.keyExists(strings::vr_synonyms)) {

        const std::vector<smart_objects::SmartObject> *newVR = msg_params[strings::vr_synonyms].asArray();

        std::vector<smart_objects::SmartObject>::const_iterator it = newVR->begin();
        std::vector<smart_objects::SmartObject>::const_iterator itEnd = newVR->end();

        for (; it != itEnd; ++it) {
          if (curName == it->asString()) {
            LOG4CXX_ERROR(logger_, "Some VR synonyms are known already.");
            return true;
          }

          CoincidencePredicateTTS t(it->asString());
          if (    NULL !=curTTS
              &&  std::any_of(curTTS->begin(), curTTS->end(), t)){
            LOG4CXX_ERROR(logger_, "Some VR synonyms are known already.");
            return true;
          }

          CoincidencePredicateVR v(it->asString());
          if (    NULL != curVR
              &&  std::any_of(curVR->begin(), curVR->end(), v)) {
            LOG4CXX_ERROR(logger_, "Some VR synonyms are known already.");
            return true;
          }
        }
      } // end vr check

    }// application for end

    return false;
} // method end


}  // namespace commands

}  // namespace application_manager
