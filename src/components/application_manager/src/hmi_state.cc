
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

#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <ostream>

#include "application_manager/application_manager.h"
#include "utils/helpers.h"

namespace application_manager {

SDL_CREATE_LOG_VARIABLE("HmiState")

HmiState::HmiState(std::shared_ptr<Application> app,
                   const ApplicationManager& app_mngr,
                   StateID state_id)
    : hmi_app_id_(app->hmi_app_id())
    , state_id_(state_id)
    , app_mngr_(app_mngr)
    , window_type_(mobile_apis::WindowType::INVALID_ENUM)
    , hmi_level_(mobile_apis::HMILevel::INVALID_ENUM)
    , audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM)
    , video_streaming_state_(mobile_apis::VideoStreamingState::INVALID_ENUM)
    , system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
  SDL_LOG_DEBUG(*this);
}

HmiState::HmiState(std::shared_ptr<Application> app,
                   const ApplicationManager& app_mngr)
    : hmi_app_id_(app->hmi_app_id())
    , state_id_(STATE_ID_REGULAR)
    , app_mngr_(app_mngr)
    , window_type_(mobile_apis::WindowType::INVALID_ENUM)
    , hmi_level_(mobile_apis::HMILevel::INVALID_ENUM)
    , audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM)
    , video_streaming_state_(mobile_apis::VideoStreamingState::INVALID_ENUM)
    , system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
  SDL_LOG_DEBUG(*this);
}

void HmiState::set_parent(HmiStatePtr parent) {
  parent_ = parent;
}

bool HmiState::is_navi_app() const {
  const ApplicationSharedPtr app =
      app_mngr_.application_by_hmi_app(hmi_app_id_);
  return app ? app->is_navi() : false;
}

bool HmiState::is_projection_app() const {
  const ApplicationSharedPtr app =
      app_mngr_.application_by_hmi_app(hmi_app_id_);
  return app ? app->mobile_projection_enabled() : false;
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

mobile_apis::HMILevel::eType HmiState::parent_hmi_level() const {
  using namespace mobile_apis;
  return parent() ? parent()->hmi_level() : HMILevel::HMI_NONE;
}

mobile_apis::HMILevel::eType HmiState::parent_max_hmi_level() const {
  using namespace mobile_apis;
  return parent() ? parent()->max_hmi_level() : HMILevel::HMI_FULL;
}

mobile_apis::AudioStreamingState::eType HmiState::parent_audio_state() const {
  using namespace mobile_apis;
  return parent() ? parent()->audio_streaming_state()
                  : AudioStreamingState::NOT_AUDIBLE;
}

mobile_apis::AudioStreamingState::eType HmiState::parent_max_audio_state()
    const {
  using namespace mobile_apis;
  return parent() ? parent()->max_audio_streaming_state()
                  : AudioStreamingState::AUDIBLE;
}

mobile_apis::VideoStreamingState::eType HmiState::parent_video_state() const {
  using namespace mobile_apis;
  return parent() ? parent()->video_streaming_state()
                  : VideoStreamingState::NOT_STREAMABLE;
}

mobile_apis::VideoStreamingState::eType HmiState::parent_max_video_state()
    const {
  using namespace mobile_apis;
  return parent() ? parent()->max_video_streaming_state()
                  : VideoStreamingState::STREAMABLE;
}

mobile_apis::WindowType::eType HmiState::window_type() const {
  return window_type_;
}

void HmiState::set_window_type(
    const mobile_apis::WindowType::eType window_type) {
  window_type_ = window_type;
}

mobile_apis::AudioStreamingState::eType VRHmiState::audio_streaming_state()
    const {
  using namespace mobile_apis;
  return AudioStreamingState::NOT_AUDIBLE;
}

mobile_apis::AudioStreamingState::eType VRHmiState::max_audio_streaming_state()
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

  if (Compare<HMILevel::eType, EQ, ONE>(
          hmi_level(), HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)) {
    return std::max(parent_audio_state(), max_audio_streaming_state());
  }
  return AudioStreamingState::NOT_AUDIBLE;
}

mobile_apis::AudioStreamingState::eType TTSHmiState::max_audio_streaming_state()
    const {
  using namespace mobile_apis;

  if (app_mngr_.is_attenuated_supported()) {
    return std::max(parent_max_audio_state(), AudioStreamingState::ATTENUATED);
  }
  return AudioStreamingState::NOT_AUDIBLE;
}

VideoStreamingHmiState::VideoStreamingHmiState(
    std::shared_ptr<Application> app, const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_VIDEO_STREAMING) {}

mobile_apis::VideoStreamingState::eType
VideoStreamingHmiState::video_streaming_state() const {
  return std::max(parent_video_state(), max_video_streaming_state());
}

mobile_apis::VideoStreamingState::eType
VideoStreamingHmiState::max_video_streaming_state() const {
  using namespace mobile_apis;
  const ApplicationSharedPtr app =
      app_mngr_.application_by_hmi_app(hmi_app_id_);
  if (app && app->IsVideoApplication()) {
    return std::max(parent_max_video_state(), VideoStreamingState::STREAMABLE);
  }

  return VideoStreamingState::NOT_STREAMABLE;
}

NaviStreamingHmiState::NaviStreamingHmiState(std::shared_ptr<Application> app,
                                             const ApplicationManager& app_mngr)
    : VideoStreamingHmiState(app, app_mngr) {
  set_state_id(STATE_ID_NAVI_STREAMING);
}

mobile_apis::AudioStreamingState::eType
NaviStreamingHmiState::audio_streaming_state() const {
  return std::max(parent_audio_state(), max_audio_streaming_state());
}

mobile_apis::AudioStreamingState::eType
NaviStreamingHmiState::max_audio_streaming_state() const {
  using namespace mobile_apis;
  auto expected = AudioStreamingState::AUDIBLE;
  if (!is_navi_app()) {
    expected = app_mngr_.is_attenuated_supported()
                   ? AudioStreamingState::ATTENUATED
                   : AudioStreamingState::NOT_AUDIBLE;
  }

  return std::max(expected, parent_max_audio_state());
}

PhoneCallHmiState::PhoneCallHmiState(std::shared_ptr<Application> app,
                                     const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_PHONE_CALL) {}

mobile_apis::HMILevel::eType PhoneCallHmiState::hmi_level() const {
  using namespace mobile_apis;
  if (HMILevel::INVALID_ENUM == parent_hmi_level()) {
    return parent_hmi_level();
  }
  return std::max(parent_hmi_level(), max_hmi_level());
}

mobile_apis::HMILevel::eType PhoneCallHmiState::max_hmi_level() const {
  using namespace helpers;
  using namespace mobile_apis;

  if (WindowType::WIDGET == window_type()) {
    return std::max(HMILevel::HMI_FULL, parent_max_hmi_level());
  }

  auto expected = HMILevel::HMI_FULL;
  if (is_navi_app() || is_mobile_projection_app()) {
    expected = HMILevel::HMI_LIMITED;
  } else if (is_media_app() || is_voice_communication_app()) {
    expected = HMILevel::HMI_BACKGROUND;
  }

  return std::max(expected, parent_max_hmi_level());
}

SafetyModeHmiState::SafetyModeHmiState(std::shared_ptr<Application> app,
                                       const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_SAFETY_MODE) {}

DeactivateHMI::DeactivateHMI(std::shared_ptr<Application> app,
                             const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_DEACTIVATE_HMI) {}

mobile_apis::HMILevel::eType DeactivateHMI::hmi_level() const {
  using namespace mobile_apis;
  if (HMILevel::INVALID_ENUM == parent_hmi_level()) {
    return parent_hmi_level();
  }
  return std::max(parent_hmi_level(), max_hmi_level());
}

mobile_apis::HMILevel::eType DeactivateHMI::max_hmi_level() const {
  using namespace helpers;
  using namespace mobile_apis;

  if (WindowType::WIDGET == window_type()) {
    return std::max(HMILevel::HMI_FULL, parent_max_hmi_level());
  }

  return std::max(HMILevel::HMI_BACKGROUND, parent_max_hmi_level());
}

AudioSource::AudioSource(std::shared_ptr<Application> app,
                         const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_AUDIO_SOURCE)
    , keep_context_(app->keep_context()) {
  app->set_keep_context(false);
}

mobile_apis::HMILevel::eType AudioSource::hmi_level() const {
  using namespace mobile_apis;

  if (HMILevel::INVALID_ENUM == parent_hmi_level()) {
    return parent_hmi_level();
  }

  if (WindowType::WIDGET == window_type() || keep_context_) {
    return std::max(parent_hmi_level(), max_hmi_level());
  }

  auto expected = HMILevel::HMI_BACKGROUND;
  if (is_navi_app() || is_projection_app() || is_voice_communication_app()) {
    expected = HMILevel::HMI_LIMITED;
  }

  return std::max(std::max(expected, max_hmi_level()), parent_hmi_level());
}

mobile_apis::AudioStreamingState::eType AudioSource::audio_streaming_state()
    const {
  return is_media_app() && !is_navi_app()
             ? mobile_apis::AudioStreamingState::NOT_AUDIBLE
             : parent_audio_state();
}

mobile_apis::VideoStreamingState::eType AudioSource::video_streaming_state()
    const {
  return parent_video_state();
}

mobile_apis::HMILevel::eType AudioSource::max_hmi_level() const {
  using namespace mobile_apis;

  if (WindowType::WIDGET == window_type()) {
    return std::max(HMILevel::HMI_FULL, parent_max_hmi_level());
  }

  auto expected = HMILevel::HMI_FULL;
  if (!keep_context_ && is_media_app() && !is_navi_app() &&
      !is_projection_app()) {
    expected = HMILevel::HMI_BACKGROUND;
  }

  return std::max(expected, parent_max_hmi_level());
}

mobile_apis::AudioStreamingState::eType AudioSource::max_audio_streaming_state()
    const {
  return is_media_app() && !is_navi_app()
             ? mobile_apis::AudioStreamingState::NOT_AUDIBLE
             : parent_max_audio_state();
}

mobile_apis::VideoStreamingState::eType AudioSource::max_video_streaming_state()
    const {
  return parent() ? parent()->max_video_streaming_state()
                  : mobile_apis::VideoStreamingState::STREAMABLE;
}

EmbeddedNavi::EmbeddedNavi(std::shared_ptr<Application> app,
                           const ApplicationManager& app_mngr)
    : HmiState(app, app_mngr, STATE_ID_EMBEDDED_NAVI) {}

mobile_apis::AudioStreamingState::eType EmbeddedNavi::audio_streaming_state()
    const {
  return is_navi_app() ? mobile_apis::AudioStreamingState::NOT_AUDIBLE
                       : parent_audio_state();
}

mobile_apis::VideoStreamingState::eType EmbeddedNavi::video_streaming_state()
    const {
  return mobile_apis::VideoStreamingState::NOT_STREAMABLE;
}

mobile_apis::HMILevel::eType EmbeddedNavi::hmi_level() const {
  using namespace mobile_apis;
  using namespace helpers;

  if (HMILevel::INVALID_ENUM == parent_hmi_level()) {
    return parent_hmi_level();
  }

  if (WindowType::WIDGET == window_type()) {
    return std::max(parent_hmi_level(), max_hmi_level());
  }

  auto expected = HMILevel::HMI_BACKGROUND;
  if ((is_media_app() || is_voice_communication_app()) && !is_navi_app()) {
    expected = HMILevel::HMI_LIMITED;
  }

  return std::max(std::max(expected, max_hmi_level()), parent_hmi_level());
}

mobile_apis::AudioStreamingState::eType
EmbeddedNavi::max_audio_streaming_state() const {
  return is_navi_app() ? mobile_apis::AudioStreamingState::NOT_AUDIBLE
                       : parent_max_audio_state();
}

mobile_apis::VideoStreamingState::eType
EmbeddedNavi::max_video_streaming_state() const {
  return is_navi_app() ? mobile_apis::VideoStreamingState::NOT_STREAMABLE
                       : parent_max_video_state();
}

mobile_apis::HMILevel::eType EmbeddedNavi::max_hmi_level() const {
  using namespace mobile_apis;
  using namespace helpers;

  if (WindowType::WIDGET == window_type()) {
    return std::max(HMILevel::HMI_FULL, parent_max_hmi_level());
  }

  auto expected = HMILevel::HMI_FULL;
  if (is_navi_app()) {
    expected = HMILevel::HMI_BACKGROUND;
  }

  return std::max(expected, parent_max_hmi_level());
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

// cppcheck-suppress unusedFunction //Used in the next
// overload of operator<<
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
  os << ", hmi app id:" << src.hmi_app_id_
     << ", window_type:" << src.window_type_ << ", state:" << src.state_id()
     << ", hmi_level:" << src.hmi_level()
     << ", audio:" << src.audio_streaming_state()
     << ", video:" << src.video_streaming_state()
     << ", context:" << src.system_context() << ')';

  return os;
}

}  // namespace application_manager
