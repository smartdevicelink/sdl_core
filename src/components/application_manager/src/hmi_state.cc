
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
#include <ostream>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include "application_manager/application_manager.h"
#include "utils/helpers.h"

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "HmiState")

HmiState::HmiState(std::shared_ptr<Application> app,
                   const ApplicationManager& app_mngr,
                   StateID state_id)
    : hmi_app_id_(app->hmi_app_id())
    , state_id_(state_id)
    , app_mngr_(app_mngr)
    , hmi_level_(mobile_apis::HMILevel::INVALID_ENUM)
    , audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM)
    , video_streaming_state_(mobile_apis::VideoStreamingState::INVALID_ENUM)
    , system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
  LOG4CXX_DEBUG(logger_, *this);
}

HmiState::HmiState(std::shared_ptr<Application> app,
                   const ApplicationManager& app_mngr)
    : hmi_app_id_(app->hmi_app_id())
    , state_id_(STATE_ID_REGULAR)
    , app_mngr_(app_mngr)
    , hmi_level_(mobile_apis::HMILevel::INVALID_ENUM)
    , audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM)
    , video_streaming_state_(mobile_apis::VideoStreamingState::INVALID_ENUM)
    , system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
  LOG4CXX_DEBUG(logger_, *this);
}

void HmiState::set_parent(HmiStatePtr parent) {
  DCHECK_OR_RETURN_VOID(parent);
  parent_ = parent;
}

bool HmiState::is_navi_app() const {
  const ApplicationSharedPtr app =
      app_mngr_.application_by_hmi_app(hmi_app_id_);
  return app ? app->is_navi() : false;
}

bool HmiState::is_media_app() const {
  const ApplicationSharedPtr app =
      app_mngr_.application_by_hmi_app(hmi_app_id_);
  return app ? app->is_media_application() : false;
}

bool HmiState::is_voice_communication_app() const {
  const ApplicationSharedPtr app =
      app_mngr_.application_by_hmi_app(hmi_app_id_);
  return app ? app->is_voice_communication_supported() : false;
}

bool HmiState::is_mobile_projection_app() const {
  const ApplicationSharedPtr app =
      app_mngr_.application_by_hmi_app(hmi_app_id_);
  return app ? app->mobile_projection_enabled() : false;
}

mobile_apis::AudioStreamingState::eType VRHmiState::audio_streaming_state()
    const {
  using namespace mobile_apis;
  return AudioStreamingState::NOT_AUDIBLE;
}

VRHmiState::VRHmiState(std::shared_ptr<Application> app,
                       const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_VR_SESSION) {}

TTSHmiState::TTSHmiState(std::shared_ptr<Application> app,
                         const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_TTS_SESSION) {}

mobile_apis::AudioStreamingState::eType TTSHmiState::audio_streaming_state()
    const {
  using namespace helpers;
  using namespace mobile_apis;

  AudioStreamingState::eType expected_state = AudioStreamingState::NOT_AUDIBLE;
  if (app_mngr_.is_attenuated_supported() &&
      AudioStreamingState::NOT_AUDIBLE != parent()->audio_streaming_state() &&
      Compare<HMILevel::eType, EQ, ONE>(
          hmi_level(), HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)) {
    expected_state = AudioStreamingState::ATTENUATED;
  }
  return expected_state;
}

VideoStreamingHmiState::VideoStreamingHmiState(
    std::shared_ptr<Application> app, const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_VIDEO_STREAMING) {}

mobile_apis::VideoStreamingState::eType
VideoStreamingHmiState::video_streaming_state() const {
  const ApplicationSharedPtr app =
      app_mngr_.application_by_hmi_app(hmi_app_id_);
  if (app && app->IsVideoApplication()) {
    return parent()->video_streaming_state();
  }

  return mobile_apis::VideoStreamingState::NOT_STREAMABLE;
}

NaviStreamingHmiState::NaviStreamingHmiState(std::shared_ptr<Application> app,
                                             const ApplicationManager& app_mngr)
    : VideoStreamingHmiState(app, app_mngr) {
  set_state_id(STATE_ID_NAVI_STREAMING);
}

mobile_apis::AudioStreamingState::eType
NaviStreamingHmiState::audio_streaming_state() const {
  using namespace helpers;
  using namespace mobile_apis;

  AudioStreamingState::eType expected_state = parent()->audio_streaming_state();
  if (!is_navi_app() && Compare<AudioStreamingState::eType, EQ, ONE>(
                            expected_state,
                            AudioStreamingState::AUDIBLE,
                            AudioStreamingState::ATTENUATED)) {
    if (app_mngr_.is_attenuated_supported()) {
      expected_state = AudioStreamingState::ATTENUATED;
    } else {
      expected_state = AudioStreamingState::NOT_AUDIBLE;
    }
  }

  return expected_state;
}

PhoneCallHmiState::PhoneCallHmiState(std::shared_ptr<Application> app,
                                     const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_PHONE_CALL) {}

mobile_apis::HMILevel::eType PhoneCallHmiState::hmi_level() const {
  using namespace helpers;
  using namespace mobile_apis;
  if (Compare<HMILevel::eType, EQ, ONE>(parent()->hmi_level(),
                                        HMILevel::HMI_BACKGROUND,
                                        HMILevel::HMI_NONE)) {
    return parent()->hmi_level();
  }
  if (is_navi_app() || is_mobile_projection_app()) {
    return HMILevel::HMI_LIMITED;
  }
  if (!is_media_app()) {
    return parent()->hmi_level();
  }
  return HMILevel::HMI_BACKGROUND;
}

SafetyModeHmiState::SafetyModeHmiState(std::shared_ptr<Application> app,
                                       const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_SAFETY_MODE) {}

DeactivateHMI::DeactivateHMI(std::shared_ptr<Application> app,
                             const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_DEACTIVATE_HMI) {}

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

AudioSource::AudioSource(std::shared_ptr<Application> app,
                         const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_AUDIO_SOURCE)
    , keep_context_(app->keep_context()) {
  app->set_keep_context(false);
}

mobile_apis::HMILevel::eType AudioSource::hmi_level() const {
  // Checking for NONE  is necessary to avoid issue during
  // calculation of HMI level during setting default HMI level
  if (keep_context_ ||
      mobile_apis::HMILevel::HMI_NONE == parent()->hmi_level()) {
    return parent()->hmi_level();
  }

  return mobile_apis::HMILevel::HMI_BACKGROUND;
}

EmbeddedNavi::EmbeddedNavi(std::shared_ptr<Application> app,
                           const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_EMBEDDED_NAVI) {}

mobile_apis::HMILevel::eType EmbeddedNavi::hmi_level() const {
  using namespace mobile_apis;
  using namespace helpers;
  if (Compare<HMILevel::eType, EQ, ONE>(parent()->hmi_level(),
                                        HMILevel::HMI_BACKGROUND,
                                        HMILevel::HMI_NONE)) {
    return parent()->hmi_level();
  }
  return HMILevel::HMI_BACKGROUND;
}

namespace {
typedef boost::bimap<HmiState::StateID, std::string> StateID2StrMap;
const StateID2StrMap kStateID2StrMap =
    boost::assign::list_of<StateID2StrMap::relation>(
        HmiState::StateID::STATE_ID_CURRENT, "CURRENT")(
        HmiState::StateID::STATE_ID_REGULAR, "REGULAR")(
        HmiState::StateID::STATE_ID_POSTPONED, "POSTPONED")(
        HmiState::StateID::STATE_ID_PHONE_CALL, "PHONE_CALL")(
        HmiState::StateID::STATE_ID_SAFETY_MODE, "SAFETY_MODE")(
        HmiState::StateID::STATE_ID_VR_SESSION, "VR_SESSION")(
        HmiState::StateID::STATE_ID_TTS_SESSION, "TTS_SESSION")(
        HmiState::StateID::STATE_ID_VIDEO_STREAMING, "VIDEO_STREAMING")(
        HmiState::StateID::STATE_ID_NAVI_STREAMING, "NAVI_STREAMING")(
        HmiState::StateID::STATE_ID_DEACTIVATE_HMI, "DEACTIVATE_HMI")(
        HmiState::StateID::STATE_ID_AUDIO_SOURCE, "AUDIO_SOURCE")(
        HmiState::StateID::STATE_ID_EMBEDDED_NAVI, "EMBEDDED_NAVI");
}  // anonymous namespace

std::ostream& operator<<(std::ostream& os, const HmiState::StateID src) {
  try {
    os << kStateID2StrMap.left.at(src);
  } catch (const std::exception&) {
    // specified element have NOT been found
    os << "UNRECOGNIZED(" << static_cast<int>(src) << ")";
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, const HmiState& src) {
  const ApplicationSharedPtr app =
      src.app_mngr_.application_by_hmi_app(src.hmi_app_id_);
  os << "HMIState(app id:";
  if (app) {
    os << app->app_id();
  } else {
    os << "(none)";
  }
  os << ", hmi app id:" << src.hmi_app_id_ << ", state:" << src.state_id()
     << ", hmi_level:" << src.hmi_level()
     << ", audio:" << src.audio_streaming_state()
     << ", video:" << src.video_streaming_state()
     << ", context:" << src.system_context() << ')';

  return os;
}

}  // namespace application_manager
