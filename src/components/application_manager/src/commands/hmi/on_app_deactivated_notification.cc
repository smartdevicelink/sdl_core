/**
 * Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/hmi/on_app_deactivated_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "config_profile/profile.h"

namespace application_manager {

namespace commands {

OnAppDeactivatedNotification::OnAppDeactivatedNotification(
    const MessageSharedPtr& message)
    : NotificationFromHMI(message) {
}

OnAppDeactivatedNotification::~OnAppDeactivatedNotification() {
}

void OnAppDeactivatedNotification::Run() {
  LOG4CXX_INFO(logger_, "OnAppDeactivatedNotification::Run");
  uint32_t app_id = (*message_)[strings::msg_params][strings::app_id].asUInt();
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Application not found, id="<<app_id);
    return;
  }

  if (!(((hmi_apis::Common_DeactivateReason::AUDIO ==
      (*message_)[strings::msg_params][hmi_request::reason].asInt()) ||
      (hmi_apis::Common_DeactivateReason::PHONECALL ==
            (*message_)[strings::msg_params][hmi_request::reason].asInt())) &&
      (app->hmi_level() == mobile_api::HMILevel::eType::HMI_LIMITED))) {
    app = ApplicationManagerImpl::instance()->active_application();
    if (!app.valid()) {
      LOG4CXX_ERROR_EXT(logger_, "OnAppDeactivatedNotification no active app!");
      return;
    }
    if (app_id != app->app_id()) {
      LOG4CXX_ERROR_EXT(logger_, "Wrong application id!");
      return;
    }
  }

  if (mobile_api::HMILevel::eType::HMI_NONE == app->hmi_level()) {
    return;
  }

  switch ((*message_)[strings::msg_params][hmi_request::reason].asInt()) {
    case hmi_apis::Common_DeactivateReason::AUDIO: {
      if (app->is_media_application()) {
        if (profile::Profile::instance()->is_mixing_audio_supported() &&
            (ApplicationManagerImpl::instance()->vr_session_started() ||
             app->tts_speak_state())) {
          app->set_audio_streaming_state(mobile_api::AudioStreamingState::ATTENUATED);
        } else {
          app->set_audio_streaming_state(mobile_api::AudioStreamingState::NOT_AUDIBLE);
        }
      }
      // switch HMI level for all applications in FULL or LIMITED
      ApplicationManagerImpl::ApplicationListAccessor accessor;
      ApplicationManagerImpl::TAppList applications =
          accessor.applications();
      ApplicationManagerImpl::TAppListIt it =
          applications.begin();
      for (; applications.end() != it; ++it) {
          ApplicationSharedPtr app = *it;
          if (app.valid()) {
            if (mobile_apis::HMILevel::eType::HMI_FULL == app->hmi_level() ||
                mobile_apis::HMILevel::eType::HMI_LIMITED == app->hmi_level()) {
                app->set_hmi_level(mobile_api::HMILevel::HMI_BACKGROUND);
                MessageHelper::SendHMIStatusNotification(*app);
            }
          }
      }
      break;
    }
    case hmi_apis::Common_DeactivateReason::PHONECALL: {
      app->set_audio_streaming_state(mobile_api::AudioStreamingState::NOT_AUDIBLE);
      app->set_hmi_level(mobile_api::HMILevel::HMI_BACKGROUND);
      break;
    }
    case hmi_apis::Common_DeactivateReason::NAVIGATIONMAP:
    case hmi_apis::Common_DeactivateReason::PHONEMENU:
    case hmi_apis::Common_DeactivateReason::SYNCSETTINGS:
    case hmi_apis::Common_DeactivateReason::GENERAL: {
      if ((!app->IsAudioApplication()) ||
          ApplicationManagerImpl::instance()->
          DoesAudioAppWithSameHMITypeExistInFullOrLimited(app)) {
        app->set_hmi_level(mobile_api::HMILevel::HMI_BACKGROUND);
      } else {
        app->set_hmi_level(mobile_api::HMILevel::HMI_LIMITED);
      }
      break;
    }
    default: {
      LOG4CXX_ERROR_EXT(logger_, "Unknown reason of app deactivation");
      return;
    }
  }

  MessageHelper::SendHMIStatusNotification(*app);
}

}  // namespace commands

}  // namespace application_manager

