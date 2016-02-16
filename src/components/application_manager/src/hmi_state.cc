/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "application_manager/hmi_state.h"
#include "application_manager/application_manager.h"
#include "utils/helpers.h"

namespace application_manager {

HmiState::HmiState(uint32_t app_id,
                   ApplicationManager* app_mngr,
                   StateID state_id)
    : app_id_(app_id)
    , state_id_(state_id)
    , app_mngr_(app_mngr)
    , hmi_level_(mobile_apis::HMILevel::INVALID_ENUM)
    , audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM)
    , system_context_(mobile_apis::SystemContext::INVALID_ENUM) {}

HmiState::HmiState(uint32_t app_id, ApplicationManager* app_mngr)
    : app_id_(app_id)
    , state_id_(STATE_ID_REGULAR)
    , app_mngr_(app_mngr)
    , hmi_level_(mobile_apis::HMILevel::INVALID_ENUM)
    , audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM)
    , system_context_(mobile_apis::SystemContext::INVALID_ENUM) {}

void HmiState::set_parent(HmiStatePtr parent) {
  DCHECK_OR_RETURN_VOID(parent);
  parent_ = parent;
}

bool HmiState::is_navi_app(const uint32_t app_id) const {
  DCHECK_OR_RETURN(app_mngr_, false);
  const ApplicationSharedPtr app = app_mngr_->application(app_id);
  DCHECK_OR_RETURN(app, false);
  return app ? app->is_navi() : false;
}

bool HmiState::is_media_app(const uint32_t app_id) const {
  DCHECK_OR_RETURN(app_mngr_, false);
  const ApplicationSharedPtr app = app_mngr_->application(app_id);
  DCHECK_OR_RETURN(app, false);
  return app ? app->is_media_application() : false;
}

bool HmiState::is_voice_communication_app(const uint32_t app_id) const {
  DCHECK_OR_RETURN(app_mngr_, false);
  const ApplicationSharedPtr app = app_mngr_->application(app_id);
  DCHECK_OR_RETURN(app, false);
  return app ? app->is_voice_communication_supported() : false;
}

mobile_apis::AudioStreamingState::eType VRHmiState::audio_streaming_state()
    const {
  using namespace mobile_apis;
  return AudioStreamingState::NOT_AUDIBLE;
}

VRHmiState::VRHmiState(uint32_t app_id, ApplicationManager* app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_VR_SESSION) {}

TTSHmiState::TTSHmiState(uint32_t app_id, ApplicationManager* app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_TTS_SESSION) {}

mobile_apis::AudioStreamingState::eType TTSHmiState::audio_streaming_state()
    const {
  using namespace helpers;
  using namespace mobile_apis;

  AudioStreamingState::eType expected_state = AudioStreamingState::NOT_AUDIBLE;
  if (app_mngr_->is_attenuated_supported() &&
      AudioStreamingState::NOT_AUDIBLE != parent()->audio_streaming_state() &&
      Compare<HMILevel::eType, EQ, ONE>(
          hmi_level(), HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)) {
    expected_state = AudioStreamingState::ATTENUATED;
  }
  return expected_state;
}

NaviStreamingHmiState::NaviStreamingHmiState(uint32_t app_id,
                                             ApplicationManager* app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_NAVI_STREAMING) {}

mobile_apis::AudioStreamingState::eType
NaviStreamingHmiState::audio_streaming_state() const {
  using namespace helpers;
  using namespace mobile_apis;

  AudioStreamingState::eType expected_state = parent()->audio_streaming_state();
  if (!is_navi_app(app_id_) && AudioStreamingState::AUDIBLE == expected_state) {
    if (app_mngr_->is_attenuated_supported()) {
      expected_state = AudioStreamingState::ATTENUATED;
    } else {
      expected_state = AudioStreamingState::NOT_AUDIBLE;
    }
  }
  return expected_state;
}

PhoneCallHmiState::PhoneCallHmiState(uint32_t app_id,
                                     ApplicationManager* app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_PHONE_CALL) {}

mobile_apis::HMILevel::eType PhoneCallHmiState::hmi_level() const {
  using namespace helpers;
  using namespace mobile_apis;
  if (Compare<HMILevel::eType, EQ, ONE>(parent()->hmi_level(),
                                        HMILevel::HMI_BACKGROUND,
                                        HMILevel::HMI_NONE)) {
    return parent()->hmi_level();
  }
  if (is_navi_app(app_id_)) {
    return HMILevel::HMI_LIMITED;
  }
  if (!is_media_app(app_id_)) {
    return parent()->hmi_level();
  }
  return HMILevel::HMI_BACKGROUND;
}

SafetyModeHmiState::SafetyModeHmiState(uint32_t app_id,
                                       ApplicationManager* app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_SAFETY_MODE) {}

DeactivateHMI::DeactivateHMI(uint32_t app_id, ApplicationManager* app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_DEACTIVATE_HMI) {}

mobile_apis::HMILevel::eType DeactivateHMI::hmi_level() const {
  using namespace helpers;
  using namespace mobile_apis;
  if (Compare<HMILevel::eType, EQ, ONE>(parent()->hmi_level(),
                                        HMILevel::HMI_BACKGROUND,
                                        HMILevel::HMI_NONE)) {
    return parent()->hmi_level();
  }
  return HMILevel::HMI_BACKGROUND;
}

AudioSource::AudioSource(uint32_t app_id, ApplicationManager* app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_AUDIO_SOURCE) {}

mobile_apis::HMILevel::eType AudioSource::hmi_level() const {
  using namespace mobile_apis;
  using namespace helpers;
  // TODO(AOleynik): That NONE check is necessary to avoid issue during
  // calculation of HMI level during setting default HMI level
  // Should be investigated (used in multiple places here), since looks weird
  if (Compare<HMILevel::eType, EQ, ONE>(parent()->hmi_level(),
                                        HMILevel::HMI_BACKGROUND,
                                        HMILevel::HMI_NONE)) {
    return parent()->hmi_level();
  }
  if (is_navi_app(app_id_)) {
    return HMILevel::HMI_LIMITED;
  }
  return HMILevel::HMI_BACKGROUND;
}

EmbeddedNavi::EmbeddedNavi(uint32_t app_id, ApplicationManager* app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_EMBEDDED_NAVI) {}

mobile_apis::HMILevel::eType EmbeddedNavi::hmi_level() const {
  using namespace mobile_apis;
  using namespace helpers;
  if (Compare<HMILevel::eType, EQ, ONE>(parent()->hmi_level(),
                                        HMILevel::HMI_BACKGROUND,
                                        HMILevel::HMI_NONE)) {
    return parent()->hmi_level();
  }
  if (is_media_app(app_id_)) {
    return HMILevel::HMI_LIMITED;
  }
  return HMILevel::HMI_BACKGROUND;
}
}
