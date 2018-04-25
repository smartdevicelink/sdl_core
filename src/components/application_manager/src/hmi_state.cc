
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

HmiState::HmiState(utils::SharedPtr<Application> app,
                   const ApplicationManager& app_mngr,
                   StateID state_id)
    : app_(app)
    , state_id_(state_id)
    , app_mngr_(app_mngr)
    , hmi_level_(mobile_apis::HMILevel::INVALID_ENUM)
    , audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM)
    , video_streaming_state_(mobile_apis::VideoStreamingState::INVALID_ENUM)
    , system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
  LOG4CXX_DEBUG(logger_, "HMI state: '" << state_id_ << "'");
}

HmiState::HmiState(utils::SharedPtr<Application> app,
                   const ApplicationManager& app_mngr)
    : app_(app)
    , state_id_(STATE_ID_REGULAR)
    , app_mngr_(app_mngr)
    , hmi_level_(mobile_apis::HMILevel::INVALID_ENUM)
    , audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM)
    , video_streaming_state_(mobile_apis::VideoStreamingState::INVALID_ENUM)
    , system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
  LOG4CXX_DEBUG(logger_, "HMI state: '" << state_id_ << "'");
}

DEPRECATED HmiState::HmiState(uint32_t app_id,
                              const ApplicationManager& app_mngr,
                              StateID state_id)
    : state_id_(state_id)
    , app_mngr_(app_mngr)
    , hmi_level_(mobile_apis::HMILevel::INVALID_ENUM)
    , audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM)
    , video_streaming_state_(mobile_apis::VideoStreamingState::INVALID_ENUM)
    , system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
  LOG4CXX_DEBUG(logger_, "HMI state: '" << state_id_ << "'");
  app_ = app_mngr_.application(app_id);
}

DEPRECATED HmiState::HmiState(uint32_t app_id,
                              const ApplicationManager& app_mngr)
    : state_id_(STATE_ID_REGULAR)
    , app_mngr_(app_mngr)
    , hmi_level_(mobile_apis::HMILevel::INVALID_ENUM)
    , audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM)
    , video_streaming_state_(mobile_apis::VideoStreamingState::INVALID_ENUM)
    , system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
  LOG4CXX_DEBUG(logger_, "HMI state: '" << state_id_ << "'");
  app_ = app_mngr_.application(app_id);
}

void HmiState::set_parent(HmiStatePtr parent) {
  DCHECK_OR_RETURN_VOID(parent);
  parent_ = parent;
}

mobile_apis::HMILevel::eType HmiState::hmi_level() const {
  if (parent_) {
    return parent_->hmi_level();
  }
  return hmi_level_;
}

mobile_apis::AudioStreamingState::eType HmiState::audio_streaming_state()
    const {
  if (parent_) {
    return parent_->audio_streaming_state();
  }
  return audio_streaming_state_;
}

mobile_apis::VideoStreamingState::eType HmiState::video_streaming_state()
    const {
  if (parent_) {
    return parent_->video_streaming_state();
  }
  return video_streaming_state_;
}

mobile_apis::SystemContext::eType HmiState::system_context() const {
  if (parent_) {
    return parent_->system_context();
  }
  return system_context_;
}

bool HmiState::is_navi_app() const {
  return app_->is_navi();
}

bool HmiState::is_media_app() const {
  return app_->is_media_application();
}

bool HmiState::is_voice_communication_app() const {
  return app_->is_voice_communication_supported();
}

bool HmiState::is_mobile_projection_app() const {
  return app_->mobile_projection_enabled();
}

mobile_apis::AudioStreamingState::eType VRHmiState::audio_streaming_state()
    const {
  return mobile_apis::AudioStreamingState::NOT_AUDIBLE;
}

VRHmiState::VRHmiState(utils::SharedPtr<Application> app,
                       const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_VR_SESSION) {}

DEPRECATED VRHmiState::VRHmiState(uint32_t app_id,
                                  const ApplicationManager& app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_VR_SESSION) {}

TTSHmiState::TTSHmiState(utils::SharedPtr<Application> app,
                         const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_TTS_SESSION) {}

DEPRECATED TTSHmiState::TTSHmiState(uint32_t app_id,
                                    const ApplicationManager& app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_TTS_SESSION) {}

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

NaviStreamingHmiState::NaviStreamingHmiState(utils::SharedPtr<Application> app,
                                             const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_NAVI_STREAMING) {}

DEPRECATED NaviStreamingHmiState::NaviStreamingHmiState(
    uint32_t app_id, const ApplicationManager& app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_NAVI_STREAMING) {}

mobile_apis::AudioStreamingState::eType
NaviStreamingHmiState::audio_streaming_state() const {
  using namespace helpers;
  using namespace mobile_apis;

  AudioStreamingState::eType expected_state = parent()->audio_streaming_state();
  if (!is_navi_app() && AudioStreamingState::AUDIBLE == expected_state) {
    if (app_mngr_.is_attenuated_supported()) {
      expected_state = AudioStreamingState::ATTENUATED;
    } else {
      expected_state = AudioStreamingState::NOT_AUDIBLE;
    }
  }

  return expected_state;
}

mobile_apis::VideoStreamingState::eType
NaviStreamingHmiState::video_streaming_state() const {
  using namespace helpers;
  using namespace mobile_apis;

  VideoStreamingState::eType expected_state = parent()->video_streaming_state();
  if (!is_navi_app() && VideoStreamingState::STREAMABLE == expected_state) {
    expected_state = VideoStreamingState::NOT_STREAMABLE;
  }

  return expected_state;
}

PhoneCallHmiState::PhoneCallHmiState(utils::SharedPtr<Application> app,
                                     const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_PHONE_CALL) {}

DEPRECATED PhoneCallHmiState::PhoneCallHmiState(
    uint32_t app_id, const ApplicationManager& app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_PHONE_CALL) {}

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

SafetyModeHmiState::SafetyModeHmiState(utils::SharedPtr<Application> app,
                                       const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_SAFETY_MODE) {}

DEPRECATED SafetyModeHmiState::SafetyModeHmiState(
    uint32_t app_id, const ApplicationManager& app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_SAFETY_MODE) {}

DeactivateHMI::DeactivateHMI(utils::SharedPtr<Application> app,
                             const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_DEACTIVATE_HMI) {}

DEPRECATED DeactivateHMI::DeactivateHMI(uint32_t app_id,
                                        const ApplicationManager& app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_DEACTIVATE_HMI) {}

mobile_apis::HMILevel::eType DeactivateHMI::hmi_level() const {
  using namespace helpers;
  using namespace mobile_apis;
  HMILevel::eType hmi_level = HMILevel::HMI_BACKGROUND;
  if (Compare<HMILevel::eType, EQ, ONE>(parent()->hmi_level(),
                                        HMILevel::HMI_BACKGROUND,
                                        HMILevel::HMI_NONE)) {
    hmi_level = parent()->hmi_level();
  }

  return hmi_level;
}

AudioSource::AudioSource(utils::SharedPtr<Application> app,
                         const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_AUDIO_SOURCE) {}

DEPRECATED AudioSource::AudioSource(uint32_t app_id,
                                    const ApplicationManager& app_mngr)
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
  if (is_navi_app() || is_voice_communication_app()) {
    return HMILevel::HMI_LIMITED;
  }
  return HMILevel::HMI_BACKGROUND;
}

EmbeddedNavi::EmbeddedNavi(utils::SharedPtr<Application> app,
                           const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_EMBEDDED_NAVI) {}

DEPRECATED EmbeddedNavi::EmbeddedNavi(uint32_t app_id,
                                      const ApplicationManager& app_mngr)
    : HmiState(app_id, app_mngr, STATE_ID_EMBEDDED_NAVI) {}

mobile_apis::HMILevel::eType EmbeddedNavi::hmi_level() const {
  using namespace mobile_apis;
  using namespace helpers;
  if (Compare<HMILevel::eType, EQ, ONE>(parent()->hmi_level(),
                                        HMILevel::HMI_BACKGROUND,
                                        HMILevel::HMI_NONE)) {
    return parent()->hmi_level();
  }
  if (is_media_app()) {
    return HMILevel::HMI_LIMITED;
  }
  return HMILevel::HMI_BACKGROUND;
}

namespace {
typedef boost::bimap<HmiState::StateID, std::string> StateID2StrMap;
const StateID2StrMap kStateID2StrMap =
    boost::assign::list_of<StateID2StrMap::relation>(
        HmiState::StateID::STATE_ID_CURRENT, "STATE_ID_CURRENT")(
        HmiState::StateID::STATE_ID_REGULAR, "STATE_ID_REGULAR")(
        HmiState::StateID::STATE_ID_POSTPONED, "STATE_ID_POSTPONED")(
        HmiState::StateID::STATE_ID_PHONE_CALL, "STATE_ID_PHONE_CALL")(
        HmiState::StateID::STATE_ID_SAFETY_MODE, "STATE_ID_SAFETY_MODE")(
        HmiState::StateID::STATE_ID_VR_SESSION, "STATE_ID_VR_SESSION")(
        HmiState::StateID::STATE_ID_TTS_SESSION, "STATE_ID_TTS_SESSION")(
        HmiState::StateID::STATE_ID_NAVI_STREAMING, "STATE_ID_NAVI_STREAMING")(
        HmiState::StateID::STATE_ID_DEACTIVATE_HMI, "STATE_ID_DEACTIVATE_HMI")(
        HmiState::StateID::STATE_ID_AUDIO_SOURCE, "STATE_ID_AUDIO_SOURCE")(
        HmiState::StateID::STATE_ID_EMBEDDED_NAVI, "STATE_ID_EMBEDDED_NAVI");
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
  os << "HMIState(state:'" << src.state_id() << "',hmi_level:'"
     << src.hmi_level() << "',audio:'" << src.audio_streaming_state()
     << "',video:'" << src.video_streaming_state() << "',context:'"
     << src.system_context() << "',has parent:" << std::boolalpha
     << (src.parent() ? true : false) << ")";

  return os;
}

}  // namespace application_manager
