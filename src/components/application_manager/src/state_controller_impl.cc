/*
 Copyright (c) 2015, Ford Motor Company
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

#include "application_manager/state_controller_impl.h"
#include <tuple>
#include <sstream>
#include "application_manager/usage_statistics.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"
#include "connection_handler/connection_handler.h"

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "StateControllerImpl")

namespace {
bool IsStateChanged(const HmiState& old_state, const HmiState& new_state) {
  return std::make_tuple(old_state.hmi_level(),
                         old_state.audio_streaming_state(),
                         old_state.video_streaming_state(),
                         old_state.system_context()) !=
         std::make_tuple(new_state.hmi_level(),
                         new_state.audio_streaming_state(),
                         new_state.video_streaming_state(),
                         new_state.system_context());
}
}  // unnamed namespace

StateControllerImpl::StateControllerImpl(ApplicationManager& app_mngr)
    : EventObserver(app_mngr.event_dispatcher()), app_mngr_(app_mngr) {
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnAppActivated);
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnAppDeactivated);
  subscribe_on_event(hmi_apis::FunctionID::TTS_Started);
  subscribe_on_event(hmi_apis::FunctionID::TTS_Stopped);
  subscribe_on_event(hmi_apis::FunctionID::VR_Started);
  subscribe_on_event(hmi_apis::FunctionID::VR_Stopped);
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnEventChanged);
}

void StateControllerImpl::SetRegularState(ApplicationSharedPtr app,
                                          HmiStatePtr state,
                                          const bool send_activate_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  DCHECK_OR_RETURN_VOID(state);
  DCHECK_OR_RETURN_VOID(state->state_id() == HmiState::STATE_ID_REGULAR);

  if (state->hmi_level() == mobile_apis::HMILevel::INVALID_ENUM ||
      state->audio_streaming_state() ==
          mobile_apis::AudioStreamingState::INVALID_ENUM ||
      state->video_streaming_state() ==
          mobile_apis::VideoStreamingState::INVALID_ENUM ||
      state->system_context() == mobile_apis::SystemContext::INVALID_ENUM) {
    LOG4CXX_ERROR(logger_, "Invalid state have been gotten");
    return;
  }

  if (app->is_resuming() && !IsResumptionAllowed(app, state)) {
    return;
  }

  HmiStatePtr resolved_state = ResolveHmiState(app, state);
  if (!resolved_state) {
    state->set_state_id(HmiState::STATE_ID_POSTPONED);
    app->SetPostponedState(state);
    return;
  }
  LOG4CXX_DEBUG(logger_, "Resolved state: " << *resolved_state);
  const hmi_apis::Common_HMILevel::eType hmi_level =
      static_cast<hmi_apis::Common_HMILevel::eType>(
          resolved_state->hmi_level());

  if (send_activate_app) {
    const int64_t corr_id = SendBCActivateApp(app, hmi_level, true);
    if (-1 != corr_id) {
      subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_ActivateApp,
                         corr_id);
      waiting_for_activate_[app->app_id()] = resolved_state;
      return;
    }
    LOG4CXX_ERROR(logger_, "Unable to send BC.ActivateApp");
    return;
  }
  ApplyRegularState(app, resolved_state);
}

void StateControllerImpl::SetRegularState(
    ApplicationSharedPtr app,
    const mobile_apis::HMILevel::eType hmi_level,
    const mobile_apis::AudioStreamingState::eType audio_state,
    const mobile_apis::VideoStreamingState::eType video_state,
    const bool send_activate_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  LOG4CXX_DEBUG(logger_,
                "hmi_level " << hmi_level << ", audio_state " << audio_state
                             << ", video_state " << video_state);
  HmiStatePtr prev_regular = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(prev_regular);
  HmiStatePtr hmi_state =
      CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(hmi_state);
  hmi_state->set_hmi_level(hmi_level);
  hmi_state->set_audio_streaming_state(audio_state);
  hmi_state->set_video_streaming_state(video_state);
  hmi_state->set_system_context(prev_regular->system_context());
  SetRegularState(app, hmi_state, send_activate_app);
}

void StateControllerImpl::SetRegularState(
    ApplicationSharedPtr app,
    const mobile_apis::HMILevel::eType hmi_level,
    const bool send_activate_app) {
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  const HmiStatePtr hmi_state =
      CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);

  DCHECK_OR_RETURN_VOID(hmi_state);
  hmi_state->set_hmi_level(hmi_level);
  hmi_state->set_audio_streaming_state(CalcAudioState(app, hmi_level));
  hmi_state->set_video_streaming_state(CalcVideoState(app, hmi_level));
  hmi_state->set_system_context(mobile_apis::SystemContext::SYSCTXT_MAIN);
  SetRegularState(app, hmi_state, send_activate_app);
}

void StateControllerImpl::SetRegularState(
    ApplicationSharedPtr app,
    const mobile_apis::HMILevel::eType hmi_level,
    const mobile_apis::AudioStreamingState::eType audio_state,
    const mobile_apis::VideoStreamingState::eType video_state,
    const mobile_apis::SystemContext::eType system_context,
    const bool send_activate_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  LOG4CXX_DEBUG(logger_,
                "hmi_level " << hmi_level << ", audio_state " << audio_state
                             << ", video_state " << video_state
                             << ", system_context " << system_context);

  HmiStatePtr hmi_state =
      CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(hmi_state);
  hmi_state->set_hmi_level(hmi_level);
  hmi_state->set_audio_streaming_state(audio_state);
  hmi_state->set_video_streaming_state(video_state);
  hmi_state->set_system_context(system_context);
  SetRegularState(app, hmi_state, send_activate_app);
}

void StateControllerImpl::SetRegularState(
    ApplicationSharedPtr app, const mobile_apis::HMILevel::eType hmi_level) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  HmiStatePtr prev_state = app->RegularHmiState();
  HmiStatePtr hmi_state =
      CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(hmi_state);
  hmi_state->set_hmi_level(hmi_level);
  hmi_state->set_audio_streaming_state(CalcAudioState(app, hmi_level));
  hmi_state->set_video_streaming_state(CalcVideoState(app, hmi_level));
  hmi_state->set_system_context(prev_state
                                    ? prev_state->system_context()
                                    : mobile_apis::SystemContext::SYSCTXT_MAIN);
  SetRegularState(app, hmi_state);
}

void StateControllerImpl::SetRegularState(
    ApplicationSharedPtr app,
    const mobile_apis::SystemContext::eType system_context) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  HmiStatePtr prev_regular = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(prev_regular);
  HmiStatePtr hmi_state =
      CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(hmi_state);
  hmi_state->set_hmi_level(prev_regular->hmi_level());
  hmi_state->set_audio_streaming_state(
      CalcAudioState(app, prev_regular->hmi_level()));
  hmi_state->set_video_streaming_state(
      CalcVideoState(app, prev_regular->hmi_level()));
  hmi_state->set_system_context(system_context);
  SetRegularState(app, hmi_state, false);
}

void StateControllerImpl::SetRegularState(
    ApplicationSharedPtr app,
    const mobile_apis::AudioStreamingState::eType audio_state,
    const mobile_apis::VideoStreamingState::eType video_state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  HmiStatePtr prev_state = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(prev_state);
  HmiStatePtr hmi_state =
      CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(hmi_state);
  hmi_state->set_hmi_level(prev_state->hmi_level());
  hmi_state->set_audio_streaming_state(audio_state);
  hmi_state->set_video_streaming_state(video_state);
  hmi_state->set_system_context(prev_state->system_context());
  SetRegularState(app, hmi_state, false);
}

void StateControllerImpl::SetRegularState(ApplicationSharedPtr app,
                                          HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  DCHECK_OR_RETURN_VOID(state);
  if (mobile_apis::HMILevel::HMI_FULL == state->hmi_level()) {
    SetRegularState(app, state, true);
  } else {
    SetRegularState(app, state, false);
  }
}

namespace {
bool IsStreamableHMIType(mobile_apis::HMILevel::eType val) {
  using namespace helpers;
  return Compare<mobile_apis::HMILevel::eType, EQ, ONE>(
      val, mobile_apis::HMILevel::HMI_FULL, mobile_apis::HMILevel::HMI_LIMITED);
}

bool IsSameAudioAppType(const Application& app1, const Application& app2) {
  // compatibility constants
  const bool both_navi = app1.is_navi() && app2.is_navi();
  const bool both_vc = app1.is_voice_communication_supported() &&
                       app2.is_voice_communication_supported();
  const bool both_media = (app1.is_media_application() && !app1.is_navi() &&
                           !app1.is_voice_communication_supported()) &&
                          (app2.is_media_application() && !app2.is_navi() &&
                           !app2.is_voice_communication_supported());
  const bool both_other =
      !app1.IsAudioApplication() && !app2.IsAudioApplication();

  const bool same_app_audio_type =
      both_navi || both_vc || both_media || both_other;

  return same_app_audio_type;
}
}  // unnamed namespace

void StateControllerImpl::HmiLevelConflictResolver::operator()(
    ApplicationSharedPtr app_to_resolve) {
  DCHECK_OR_RETURN_VOID(app_to_resolve);
  DCHECK_OR_RETURN_VOID(state_ctrl_);
  DCHECK_OR_RETURN_VOID(applied_);
  DCHECK_OR_RETURN_VOID(state_);

  if (app_to_resolve == applied_) {
    return;
  }

  const HmiStatePtr state_to_resolve = app_to_resolve->RegularHmiState();
  DCHECK_OR_RETURN_VOID(state_to_resolve);

  // Just for debug
  // {
  //   const std::string nl_indent = "\n ---> ";
  //   std::stringstream ss;
  //   ss << '\n';
  //   ss <<
  //   "-------------------HMI_CONFLICT_RESOLVER-------------------";
  //   ss << nl_indent << "Applied app id    - " << applied_->app_id();
  //   ss << nl_indent << "Applied reg state - " << *state_;
  //   ss << nl_indent << "Resolve app id    - " << app_to_resolve->app_id();
  //   ss << nl_indent << "Resolve reg state - " << *state_to_resolve;
  //   LOG4CXX_DEBUG(logger_, ss.str());
  // }

  // If applied HMI state is FULL:
  // - all NOT audio/video applications becomes BACKGROUND
  // - all audio/video applications with other app type
  //  (navi, vc, media, projection) in FULL becomes LIMMITED
  // - all applications with same app type becomes BACKGROUND
  //
  // If applied HMI state is LIMITED:
  // - all NOT audio/video applications saves their's HMI states
  // - all applications with other app types saves their's HMI states
  // - all applications with same app type becomes BACKGROUND
  //
  // If applied HMI state is BACKGROUND:
  // - all applications will save their's HMI states
  //
  // Resolve tables to figure out audio/video streaming state
  // for FULL/LIMITED HMI level.
  // For example if applied is 'NAVIGATION' and resolve app is
  // 'PROJECTION' then to resolve becomes 'NOT_STREAMABLE'.
  // According to video resolve table the only one app may
  // be 'STREAMABLE' in the same time.
  //
  // Video app resolve table
  // * S --> STREAMABLE, NS --> NOT_STREAMABLE
  // | resolve / applied -->
  // V
  // _______navi___proj__other__
  // navi  |__NS__|_NS__|__S____|
  // proj  |__NS__|_NS__|__S ___|
  // other |__NS__|_NS__|__NS___|
  //
  // Audio app resolve table
  // * A --> AUDIBLE, NS --> NOT_AUDIBLE
  // | resolve / applied -->
  // V
  // _______navi___vc___media__other__
  // navi  |_NA__|_A___|__A__|____A___|
  // vc    |__A__|_NA__|__A__|____A___|
  // media |__A__|_A___|__NA_|____A___|
  // other |_NA__|_NA__|__NA_|____NA__|
  //
  // NOTE: for instance if applied app is 'PROJECTION' & isMedia=true
  // with applied HMI level 'FULL' (STREAMABLE & AUDIBLE)
  // and app to resolve is 'NAVIGATION' with HMI level FULL or LIMITED
  // (STREAMABLE & AUDIBLE) then to resolve becomes LIMITED
  // (NOT_STREAMABLE & AUDIBLE). So, the only one app may stream video
  // in the same time. But audio app such types as
  // NAVIGATION, COMMUNICATION may stream audio/video in LIMITED level
  // with app of other types.
  // NOTE: if applied app is 'NAVIGATION'
  // (STREAMABLE & AUDIBLE) and app to resolve is same type
  // 'NAVIGATION' (STREAMABLE & AUDIBLE) then to resolve
  // becomes BACKGROUND (NOT_STREAMABLE & NOT_AUDIBLE).

  if (!IsStreamableHMIType(state_->hmi_level())) {
    return;
  }

  if (!IsStreamableHMIType(state_to_resolve->hmi_level())) {
    return;
  }

  // applied app constants
  const mobile_apis::HMILevel::eType applied_hmi_level = state_->hmi_level();

  const bool applied_grabs_video =
      IsStreamableHMIType(applied_hmi_level) && applied_->IsVideoApplication();

  // app to resolve constants
  const mobile_apis::HMILevel::eType to_resolve_hmi_level =
      state_to_resolve->hmi_level();

  const bool to_resolve_grabs_audio =
      IsStreamableHMIType(to_resolve_hmi_level) &&
      app_to_resolve->IsAudioApplication();

  const bool to_resolve_grabs_video =
      IsStreamableHMIType(to_resolve_hmi_level) &&
      app_to_resolve->IsVideoApplication();

  // compatibility constants
  const bool same_app_audio_type =
      IsSameAudioAppType(*applied_, *app_to_resolve);

  // result vars
  mobile_apis::VideoStreamingState::eType result_video_state =
      mobile_apis::VideoStreamingState::NOT_STREAMABLE;
  mobile_apis::AudioStreamingState::eType result_audio_state =
      mobile_apis::AudioStreamingState::NOT_AUDIBLE;

  if (to_resolve_grabs_audio && !same_app_audio_type) {
    result_audio_state = mobile_apis::AudioStreamingState::AUDIBLE;
  }

  if (to_resolve_grabs_video && !applied_grabs_video) {
    result_video_state = mobile_apis::VideoStreamingState::STREAMABLE;
  }

  mobile_apis::HMILevel::eType result_hmi_level = state_to_resolve->hmi_level();

  if (applied_hmi_level == mobile_apis::HMILevel::HMI_FULL) {
    if (result_video_state == mobile_apis::VideoStreamingState::STREAMABLE ||
        result_audio_state == mobile_apis::AudioStreamingState::AUDIBLE) {
      result_hmi_level = mobile_apis::HMILevel::HMI_LIMITED;
    } else {
      result_hmi_level = mobile_apis::HMILevel::HMI_BACKGROUND;
    }
  } else if (applied_hmi_level == mobile_apis::HMILevel::HMI_LIMITED) {
    if (to_resolve_hmi_level == applied_hmi_level) {
      result_hmi_level = mobile_apis::HMILevel::HMI_BACKGROUND;
    } else {
      // other to resolve HMI levels
      // do nothing
    }
  } else {  // BACKGROUND, NONE
    // do nothing
  }

  if (to_resolve_hmi_level != result_hmi_level) {
    LOG4CXX_DEBUG(logger_,
                  "Application "
                      << app_to_resolve->app_id() << " will change HMI level "
                      << to_resolve_hmi_level << " --> " << result_hmi_level
                      << ", audio " << state_to_resolve->audio_streaming_state()
                      << " --> " << result_audio_state << ", video "
                      << state_to_resolve->video_streaming_state() << " --> "
                      << result_video_state);
    state_ctrl_->SetupRegularHmiState(app_to_resolve,
                                      result_hmi_level,
                                      result_audio_state,
                                      result_video_state);
  } else {
    LOG4CXX_DEBUG(logger_,
                  "Application " << app_to_resolve->app_id()
                                 << " will NOT change HMI level");
  }
}

HmiStatePtr StateControllerImpl::ResolveHmiState(ApplicationSharedPtr app,
                                                 HmiStatePtr state) const {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "State to resolve: " << *state);

  HmiStatePtr available_state =
      CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN(available_state, HmiStatePtr());
  available_state->set_hmi_level(state->hmi_level());
  available_state->set_audio_streaming_state(state->audio_streaming_state());
  available_state->set_video_streaming_state(state->video_streaming_state());
  available_state->set_system_context(state->system_context());

  if (app->is_resuming()) {
    const mobile_apis::HMILevel::eType available_level =
        GetAvailableHmiLevel(app, state->hmi_level());
    available_state->set_hmi_level(available_level);
    available_state->set_audio_streaming_state(
        CalcAudioState(app, available_level));
    available_state->set_video_streaming_state(
        CalcVideoState(app, available_level));
  }
  return IsStateAvailable(app, available_state) ? available_state
                                                : HmiStatePtr();
}

bool StateControllerImpl::IsResumptionAllowed(ApplicationSharedPtr app,
                                              HmiStatePtr state) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  using namespace mobile_apis;
  if (!app->is_resuming() ||
      !Compare<HMILevel::eType, EQ, ONE>(
          state->hmi_level(), HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)) {
    LOG4CXX_DEBUG(logger_, "Application is not in resuming mode.");
    return true;
  }

  if (IsTempStateActive(HmiState::StateID::STATE_ID_AUDIO_SOURCE) &&
      app->is_media_application()) {
    LOG4CXX_DEBUG(logger_,
                  "Resumption for media app is not allowed. "
                      << "AUDIO_SOURCE event is active");
    return false;
  }

  if (IsTempStateActive(HmiState::StateID::STATE_ID_EMBEDDED_NAVI) &&
      (app->is_navi() || app->mobile_projection_enabled())) {
    LOG4CXX_DEBUG(logger_,
                  "Resumption for navi or projection app is not allowed. "
                      << "EMBEDDED_NAVI event is active");
    return false;
  }

  return true;
}

mobile_apis::HMILevel::eType StateControllerImpl::GetAvailableHmiLevel(
    ApplicationSharedPtr app, mobile_apis::HMILevel::eType hmi_level) const {
  LOG4CXX_AUTO_TRACE(logger_);

  using namespace mobile_apis;
  using namespace helpers;

  mobile_apis::HMILevel::eType result = hmi_level;
  if (!Compare<HMILevel::eType, EQ, ONE>(
          hmi_level, HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)) {
    return result;
  }

  const bool is_audio_app = app->IsAudioApplication();
  const bool does_audio_app_with_same_type_exist =
      app_mngr_.IsAppTypeExistsInFullOrLimited(app);
  if (HMILevel::HMI_LIMITED == hmi_level) {
    if (!is_audio_app || does_audio_app_with_same_type_exist) {
      result = app_mngr_.GetDefaultHmiLevel(app);
    }
    return result;
  }

  const bool is_active_app_exist = app_mngr_.active_application();
  if (is_audio_app) {
    if (does_audio_app_with_same_type_exist) {
      result = app_mngr_.GetDefaultHmiLevel(app);
    } else if (is_active_app_exist) {
      result = mobile_apis::HMILevel::HMI_LIMITED;
    } else if (app->is_navi() &&
               IsTempStateActive(HmiState::StateID::STATE_ID_AUDIO_SOURCE)) {
      LOG4CXX_DEBUG(logger_,
                    "Navigation app will be resumed to LIMITED, "
                    "because of AUDIO_SOURCE ia active.");
      result = mobile_apis::HMILevel::HMI_LIMITED;
    } else if (app->is_media_application() &&
               IsTempStateActive(HmiState::StateID::STATE_ID_EMBEDDED_NAVI)) {
      LOG4CXX_DEBUG(logger_,
                    "Media app will be resumed to LIMITED, "
                    "because of EMBEDDED_NAVI is active.");
      result = mobile_apis::HMILevel::HMI_LIMITED;
    }
  } else if (is_active_app_exist) {
    result = app_mngr_.GetDefaultHmiLevel(app);
  }

  return result;
}

bool StateControllerImpl::IsStateAvailable(ApplicationSharedPtr app,
                                           HmiStatePtr state) const {
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Checking state: " << *state);

  if (app->is_resuming()) {
    return IsStateAvailableForResumption(app, state);
  }

  if (IsTempStateActive(HmiState::StateID::STATE_ID_AUDIO_SOURCE) ||
      IsTempStateActive(HmiState::StateID::STATE_ID_EMBEDDED_NAVI)) {
    if (HMILevel::HMI_FULL == state->hmi_level()) {
      LOG4CXX_DEBUG(logger_,
                    "AUDIO_SOURCE or EMBEDDED_NAVI is active."
                        << " Requested state is NOT available");
      return false;
    }
  }

  LOG4CXX_DEBUG(logger_, "Requested state is available");
  return true;
}

bool StateControllerImpl::IsStateAvailableForResumption(
    ApplicationSharedPtr app, HmiStatePtr state) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;

  if (!app->is_resuming() || !IsStreamableHMIType(state->hmi_level())) {
    LOG4CXX_DEBUG(logger_,
                  "Application is not in resuming mode."
                      << " Requested state is available");
    return true;
  }

  if (IsTempStateActive(HmiState::StateID::STATE_ID_VR_SESSION) ||
      IsTempStateActive(HmiState::StateID::STATE_ID_SAFETY_MODE)) {
    LOG4CXX_DEBUG(logger_,
                  "Requested state is not available. "
                      << "VR session or emergency event is active");
    return false;
  }
  if (IsTempStateActive(HmiState::StateID::STATE_ID_PHONE_CALL) &&
      app->is_media_application()) {
    LOG4CXX_DEBUG(logger_,
                  "Requested state for media application "
                      << "is not available. Phone call is active");
    return false;
  }
  if (IsTempStateActive(HmiState::StateID::STATE_ID_DEACTIVATE_HMI)) {
    LOG4CXX_DEBUG(logger_,
                  "Requested state is not available. "
                      << "Deactivate HMI event is active");
    return false;
  }

  LOG4CXX_DEBUG(logger_, "Requested state is available");
  return true;
}

void StateControllerImpl::SetupRegularHmiState(ApplicationSharedPtr app,
                                               HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(state);
  LOG4CXX_DEBUG(logger_, "Setup regular state: " << *state);
  HmiStatePtr curr_state = app->CurrentHmiState();
  HmiStatePtr old_state =
      CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(old_state);
  old_state->set_hmi_level(curr_state->hmi_level());
  old_state->set_audio_streaming_state(curr_state->audio_streaming_state());
  old_state->set_video_streaming_state(curr_state->video_streaming_state());
  old_state->set_system_context(curr_state->system_context());
  app->SetRegularState(state);

  if (mobile_apis::HMILevel::HMI_LIMITED == state->hmi_level() &&
      app->is_resuming()) {
    LOG4CXX_DEBUG(logger_,
                  "Resuming to LIMITED level. "
                      << "Send OnResumeAudioSource notification");
    MessageHelper::SendOnResumeAudioSourceToHMI(app->app_id(), app_mngr_);
  }
  app->set_is_resuming(false);

  HmiStatePtr new_state = app->CurrentHmiState();
  OnStateChanged(app, old_state, new_state);
}

void StateControllerImpl::SetupRegularHmiState(
    ApplicationSharedPtr app,
    const mobile_apis::HMILevel::eType hmi_level,
    const mobile_apis::AudioStreamingState::eType audio_state,
    const mobile_apis::VideoStreamingState::eType video_state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  HmiStatePtr prev_state = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(prev_state);
  HmiStatePtr new_state =
      CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(new_state);
  new_state->set_hmi_level(hmi_level);
  new_state->set_audio_streaming_state(audio_state);
  new_state->set_video_streaming_state(video_state);
  new_state->set_system_context(prev_state->system_context());
  SetupRegularHmiState(app, new_state);
}

void StateControllerImpl::ApplyRegularState(ApplicationSharedPtr app,
                                            HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  DCHECK_OR_RETURN_VOID(state);
  DCHECK_OR_RETURN_VOID(state->state_id() == HmiState::STATE_ID_REGULAR);
  SetupRegularHmiState(app, state);
  LOG4CXX_DEBUG(logger_,
                "Applying to app(id=" << app->app_id() << ") state " << *state);
  LOG4CXX_DEBUG(logger_,
                "Resolving HMI level conflicts for app id " << app->app_id());
  ForEachApplication(HmiLevelConflictResolver(app, state, this));
}

void StateControllerImpl::on_event(const event_engine::Event& event) {
  using smart_objects::SmartObject;
  using event_engine::Event;
  using namespace hmi_apis;
  namespace FunctionID = hmi_apis::FunctionID;

  LOG4CXX_AUTO_TRACE(logger_);
  const SmartObject& message = event.smart_object();
  const FunctionID::eType id = static_cast<FunctionID::eType>(event.id());
  switch (id) {
    case FunctionID::BasicCommunication_ActivateApp: {
      OnActivateAppResponse(message);
      break;
    }
    case FunctionID::BasicCommunication_OnAppActivated: {
      OnAppActivated(message);
      break;
    }
    case FunctionID::BasicCommunication_OnAppDeactivated: {
      OnAppDeactivated(message);
      break;
    }
    case FunctionID::VR_Started: {
      ApplyTempState<HmiState::STATE_ID_VR_SESSION>();
      break;
    }
    case FunctionID::VR_Stopped: {
      CancelTempState<HmiState::STATE_ID_VR_SESSION>();
      break;
    }
    case FunctionID::TTS_Started: {
      ApplyTempState<HmiState::STATE_ID_TTS_SESSION>();
      break;
    }
    case FunctionID::TTS_Stopped: {
      CancelTempState<HmiState::STATE_ID_TTS_SESSION>();
      break;
    }
    case FunctionID::BasicCommunication_OnEventChanged: {
      bool is_active =
          message[strings::msg_params][hmi_notification::is_active].asBool();
      const uint32_t id =
          message[strings::msg_params][hmi_notification::event_name].asUInt();
      // TODO(AOleynik): Add verification/conversion check here
      const Common_EventTypes::eType state_id =
          static_cast<Common_EventTypes::eType>(id);
      if (is_active) {
        if (Common_EventTypes::AUDIO_SOURCE == state_id) {
          ApplyTempState<HmiState::STATE_ID_AUDIO_SOURCE>();
          break;
        }
        if (Common_EventTypes::EMBEDDED_NAVI == state_id) {
          ApplyTempState<HmiState::STATE_ID_EMBEDDED_NAVI>();
          break;
        }
        if (Common_EventTypes::PHONE_CALL == state_id) {
          ApplyTempState<HmiState::STATE_ID_PHONE_CALL>();
          break;
        }
        if (Common_EventTypes::EMERGENCY_EVENT == state_id) {
          ApplyTempState<HmiState::STATE_ID_SAFETY_MODE>();
          break;
        }
        if (Common_EventTypes::DEACTIVATE_HMI == state_id) {
          ApplyTempState<HmiState::STATE_ID_DEACTIVATE_HMI>();
          break;
        }
      } else {
        if (Common_EventTypes::AUDIO_SOURCE == state_id) {
          CancelTempState<HmiState::STATE_ID_AUDIO_SOURCE>();
          break;
        }
        if (Common_EventTypes::EMBEDDED_NAVI == state_id) {
          CancelTempState<HmiState::STATE_ID_EMBEDDED_NAVI>();
          break;
        }
        if (Common_EventTypes::PHONE_CALL == state_id) {
          CancelTempState<HmiState::STATE_ID_PHONE_CALL>();
          break;
        }
        if (Common_EventTypes::EMERGENCY_EVENT == state_id) {
          CancelTempState<HmiState::STATE_ID_SAFETY_MODE>();
          break;
        }
        if (Common_EventTypes::DEACTIVATE_HMI == state_id) {
          CancelTempState<HmiState::STATE_ID_DEACTIVATE_HMI>();
          break;
        }
      }

      LOG4CXX_WARN(logger_,
                   "Couldn't recognize state id (val='" << state_id << "')");
      break;
    }
    default:
      break;
  }
}

void StateControllerImpl::OnStateChanged(ApplicationSharedPtr app,
                                         HmiStatePtr old_state,
                                         HmiStatePtr new_state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  DCHECK_OR_RETURN_VOID(old_state);
  DCHECK_OR_RETURN_VOID(new_state);
  LOG4CXX_DEBUG(logger_, "Old state: " << *old_state);
  LOG4CXX_DEBUG(logger_, "New state: " << *new_state);
  if (IsStateChanged(*old_state, *new_state)) {
    app_mngr_.SendHMIStatusNotification(app);
    if (new_state->hmi_level() == mobile_apis::HMILevel::HMI_NONE) {
      app->ResetDataInNone();
    }
    app_mngr_.OnHMILevelChanged(
        app->app_id(), old_state->hmi_level(), new_state->hmi_level());
    app->usage_report().RecordHmiStateChanged(new_state->hmi_level());
  } else {
    LOG4CXX_ERROR(logger_, "State have NOT been changed.");
  }
}

bool StateControllerImpl::IsTempStateActive(HmiState::StateID ID) const {
  sync_primitives::AutoLock autolock(active_states_lock_);
  const StateIDList::const_iterator itr =
      std::find(active_states_.begin(), active_states_.end(), ID);
  return active_states_.end() != itr;
}

void StateControllerImpl::OnApplicationRegistered(
    ApplicationSharedPtr app,
    const mobile_apis::HMILevel::eType default_level) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);

  active_states_lock_.Acquire();
  StateIDList::iterator it = active_states_.begin();
  for (; it != active_states_.end(); ++it) {
    HmiStatePtr new_state = CreateHmiState(app, *it);
    DCHECK_OR_RETURN_VOID(new_state);
    DCHECK_OR_RETURN_VOID(new_state->state_id() != HmiState::STATE_ID_REGULAR);
    HmiStatePtr old_hmi_state = app->CurrentHmiState();
    new_state->set_parent(old_hmi_state);
    app->AddHMIState(new_state);
  }
  active_states_lock_.Release();

  HmiStatePtr default_state =
      CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(default_state);
  default_state->set_hmi_level(default_level);
  default_state->set_audio_streaming_state(CalcAudioState(app, default_level));
  default_state->set_video_streaming_state(CalcVideoState(app, default_level));
  default_state->set_system_context(mobile_apis::SystemContext::SYSCTXT_MAIN);

  HmiStatePtr initial_state = app->RegularHmiState();

  app->SetRegularState(default_state);

  HmiStatePtr new_state = app->CurrentHmiState();

  OnStateChanged(app, initial_state, new_state);
}

int64_t StateControllerImpl::SendBCActivateApp(
    ApplicationConstSharedPtr app,
    hmi_apis::Common_HMILevel::eType level,
    bool send_policy_priority) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr bc_activate_app_request =
      MessageHelper::GetBCActivateAppRequestToHMI(
          app,
          app_mngr_.connection_handler().get_session_observer(),
          app_mngr_.GetPolicyHandler(),
          level,
          send_policy_priority,
          app_mngr_);
  if (!bc_activate_app_request) {
    LOG4CXX_ERROR(logger_, "Unable to create BC.ActivateAppRequest");
    return -1;
  }
  if (!app_mngr_.ManageHMICommand(bc_activate_app_request)) {
    LOG4CXX_ERROR(logger_, "Unable to send BC.ActivateAppRequest");
    return -1;
  }
  const int64_t corr_id =
      (*bc_activate_app_request)[strings::params][strings::correlation_id]
          .asInt();
  return corr_id;
}

void StateControllerImpl::ApplyPostponedStateForApp(ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  HmiStatePtr state = app->PostponedHmiState();
  if (state) {
    app->RemovePostponedState();
    state->set_state_id(HmiState::STATE_ID_REGULAR);
    SetRegularState(app, state);
  }
}

void StateControllerImpl::TempStateStarted(HmiState::StateID id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(active_states_lock_);
  StateIDList::iterator it =
      std::find(active_states_.begin(), active_states_.end(), id);
  if (it == active_states_.end()) {
    active_states_.push_back(id);
  } else {
    LOG4CXX_ERROR(logger_, "StateID '" << id << "' is already active");
  }
}

void StateControllerImpl::TempStateStopped(HmiState::StateID id) {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock autolock(active_states_lock_);
    active_states_.remove(id);
  }
  ForEachApplication(std::bind1st(
      std::mem_fun(&StateControllerImpl::ApplyPostponedStateForApp), this));
}

void StateControllerImpl::DeactivateApp(ApplicationSharedPtr app) {
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);

  DCHECK_OR_RETURN_VOID(app);
  const HmiStatePtr regular = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(regular);
  HmiStatePtr new_regular = utils::MakeShared<HmiState>(*regular);

  LOG4CXX_DEBUG(logger_, "Current HMI level: '" << app->hmi_level() << "'");
  const bool is_audio_app = app->IsAudioApplication();
  const bool is_video_app = app->IsVideoApplication();

  if (is_audio_app || is_video_app) {
    // audio or video app move to HMI level limited
    new_regular->set_hmi_level(HMILevel::HMI_LIMITED);

    if (is_audio_app) {
      new_regular->set_audio_streaming_state(AudioStreamingState::AUDIBLE);
    } else {
      new_regular->set_audio_streaming_state(AudioStreamingState::NOT_AUDIBLE);
    }

    if (is_video_app) {
      new_regular->set_video_streaming_state(VideoStreamingState::STREAMABLE);
    } else {
      new_regular->set_video_streaming_state(
          VideoStreamingState::NOT_STREAMABLE);
    }
  } else {
    new_regular->set_hmi_level(HMILevel::HMI_BACKGROUND);
    new_regular->set_audio_streaming_state(AudioStreamingState::NOT_AUDIBLE);
    new_regular->set_video_streaming_state(VideoStreamingState::NOT_STREAMABLE);
  }

  SetRegularState(app, new_regular, false);
}

void StateControllerImpl::OnActivateAppResponse(
    const smart_objects::SmartObject& message) {
  const hmi_apis::Common_Result::eType code =
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
  const int32_t correlation_id =
      message[strings::params][strings::correlation_id].asInt();
  const uint32_t hmi_app_id = app_mngr_.application_id(correlation_id);
  ApplicationSharedPtr application =
      app_mngr_.application_by_hmi_app(hmi_app_id);
  if (application && hmi_apis::Common_Result::SUCCESS == code) {
    HmiStatePtr pending_state = waiting_for_activate_[application->app_id()];
    DCHECK_OR_RETURN_VOID(pending_state);
    ApplyRegularState(application, pending_state);
  }
}

void StateControllerImpl::OnAppActivated(
    const smart_objects::SmartObject& message) {
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t app_id = message[strings::msg_params][strings::app_id].asUInt();
  ApplicationSharedPtr app = app_mngr_.application(app_id);
  if (!app) {
    LOG4CXX_ERROR(logger_, "Application with id " << app_id << " not found");
    return;
  }

  SetRegularState(app, HMILevel::HMI_FULL, true);
}

void StateControllerImpl::OnAppDeactivated(
    const smart_objects::SmartObject& message) {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t app_id = message[strings::msg_params][strings::app_id].asUInt();
  ApplicationSharedPtr app = app_mngr_.application(app_id);
  if (!app) {
    LOG4CXX_ERROR(logger_, "Application with id " << app_id << " not found");
    return;
  }

  if (mobile_apis::HMILevel::HMI_FULL != app->hmi_level()) {
    return;
  }

  // TODO(AOleynik): Need to delete DeactivateReason and modify OnAppDeactivated
  // when HMI will support that, otherwise won't be testable
  DeactivateApp(app);
}

void StateControllerImpl::OnNaviStreamingStarted() {
  ApplyTempState<HmiState::STATE_ID_NAVI_STREAMING>();
}

void StateControllerImpl::OnNaviStreamingStopped() {
  CancelTempState<HmiState::STATE_ID_NAVI_STREAMING>();
}

bool StateControllerImpl::IsStateActive(HmiState::StateID state_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  switch (state_id) {
    case HmiState::STATE_ID_CURRENT:
    case HmiState::STATE_ID_REGULAR:
      return true;
    default:
      return IsTempStateActive(state_id);
  }
  return false;
}

HmiStatePtr StateControllerImpl::CreateHmiState(
    utils::SharedPtr<Application> app, HmiState::StateID state_id) const {
  using namespace utils;
  LOG4CXX_AUTO_TRACE(logger_);
  HmiStatePtr new_state;
  switch (state_id) {
    case HmiState::STATE_ID_PHONE_CALL: {
      new_state = MakeShared<PhoneCallHmiState>(app, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_SAFETY_MODE: {
      new_state = MakeShared<SafetyModeHmiState>(app, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_VR_SESSION: {
      new_state = MakeShared<VRHmiState>(app, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_TTS_SESSION: {
      new_state = MakeShared<TTSHmiState>(app, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_NAVI_STREAMING: {
      new_state = MakeShared<NaviStreamingHmiState>(app, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_REGULAR: {
      new_state = MakeShared<HmiState>(app, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_POSTPONED: {
      new_state = MakeShared<HmiState>(app, app_mngr_, state_id);
      break;
    }
    case HmiState::STATE_ID_DEACTIVATE_HMI: {
      new_state = MakeShared<DeactivateHMI>(app, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_AUDIO_SOURCE: {
      new_state = MakeShared<AudioSource>(app, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_EMBEDDED_NAVI: {
      new_state = MakeShared<EmbeddedNavi>(app, app_mngr_);
      break;
    }
    default:
      LOG4CXX_FATAL(logger_, "Invalid state_id " << state_id);
      NOTREACHED();
      break;
  }
  return new_state;
}

mobile_apis::AudioStreamingState::eType StateControllerImpl::CalcAudioState(
    ApplicationSharedPtr app,
    const mobile_apis::HMILevel::eType hmi_level) const {
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;

  AudioStreamingState::eType audio_state = AudioStreamingState::NOT_AUDIBLE;
  if (IsStreamableHMIType(hmi_level)) {
    if (app->IsAudioApplication()) {
      audio_state = AudioStreamingState::AUDIBLE;
    }
  }

  LOG4CXX_DEBUG(logger_,
                "Calculated audio state for HMI level '"
                    << hmi_level << "' --> '" << audio_state << "'");
  return audio_state;
}

mobile_apis::VideoStreamingState::eType StateControllerImpl::CalcVideoState(
    ApplicationSharedPtr app,
    const mobile_apis::HMILevel::eType hmi_level) const {
  namespace HMILevel = mobile_apis::HMILevel;
  namespace VideoStreamingState = mobile_apis::VideoStreamingState;

  VideoStreamingState::eType video_state = VideoStreamingState::NOT_STREAMABLE;
  if (IsStreamableHMIType(hmi_level)) {
    if (app->IsVideoApplication()) {
      video_state = VideoStreamingState::STREAMABLE;
    }
  }

  LOG4CXX_DEBUG(logger_,
                "Calculated video state for HMI level '"
                    << hmi_level << "' --> '" << video_state << "'");
  return video_state;
}

}  // namespace application_manager
