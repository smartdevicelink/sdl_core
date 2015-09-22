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

#include "application_manager/state_controller.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "StateController");

bool IsStatusChanged(HmiStatePtr old_state,
                                      HmiStatePtr new_state) {
  if (old_state->hmi_level() != new_state->hmi_level()
      || old_state->audio_streaming_state() != new_state->audio_streaming_state()
      || old_state->system_context() != new_state->system_context() ) {
    return true;
  }
  return false;
}

StateController::StateController():EventObserver() {
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnAppActivated);
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnAppDeactivated);
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnEmergencyEvent);
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnPhoneCall);
  subscribe_on_event(hmi_apis::FunctionID::TTS_Started);
  subscribe_on_event(hmi_apis::FunctionID::TTS_Stopped);
  subscribe_on_event(hmi_apis::FunctionID::VR_Started);
  subscribe_on_event(hmi_apis::FunctionID::VR_Stopped);
}

void StateController::HmiLevelConflictResolver::operator ()
    (ApplicationSharedPtr to_resolve) {
  using namespace mobile_apis;
  using namespace helpers;
  DCHECK_OR_RETURN_VOID(state_ctrl_);
  if (to_resolve == applied_)
    return;
  if (Compare<HMILevel::eType, EQ, ONE>(state_->hmi_level(),
                                        HMILevel::HMI_FULL,
                                        HMILevel::HMI_LIMITED)) {
    HmiStatePtr cur_state = to_resolve->RegularHmiState();
    if (Compare<HMILevel::eType, EQ, ONE>(cur_state->hmi_level(),
                                          HMILevel::HMI_FULL,
                                          HMILevel::HMI_LIMITED)) {
      if (applied_->IsAudioApplication() && state_ctrl_->IsSameAppType(applied_, to_resolve)) {
        state_ctrl_->SetupRegularHmiState(to_resolve, HMILevel::HMI_BACKGROUND,
                     AudioStreamingState::NOT_AUDIBLE);
      } else if (HMILevel::HMI_FULL == state_->hmi_level() &&
          HMILevel::HMI_FULL == cur_state->hmi_level()) {
        LOG4CXX_DEBUG(logger_, "HMI level will be changed for application with appID="
                      <<to_resolve->app_id());
        state_ctrl_->SetupRegularHmiState(to_resolve, HMILevel::HMI_LIMITED,
                     AudioStreamingState::AUDIBLE);
      } else {
        LOG4CXX_DEBUG(logger_, "HMI will not be changed level for application with appID="
                      <<to_resolve->app_id());
      }
    }
  }
}

HmiStatePtr StateController::ResolveHmiState(ApplicationSharedPtr app,
                                             HmiStatePtr state) const {
  using namespace mobile_apis;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "State to resolve: hmi_level " << state->hmi_level()
                << ", audio_state " << state->audio_streaming_state()
                << ", system_context " << state->system_context());

  HmiStatePtr available_state = CreateHmiState(app->app_id(),
      HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN(available_state, HmiStatePtr());
  available_state->set_hmi_level(state->hmi_level());
  available_state->set_audio_streaming_state(state->audio_streaming_state());
  available_state->set_system_context(state->system_context());

  if (app->is_resuming()) {
    HMILevel::eType available_level = GetAvailableHmiLevel(
        app, state->hmi_level());
    available_state->set_hmi_level(available_level);
    available_state->set_audio_streaming_state(
        CalcAudioState(app, available_level));
  }
  return IsStateAvailable(app, available_state) ? available_state
                                                : HmiStatePtr();
}

mobile_apis::HMILevel::eType
StateController::GetAvailableHmiLevel(
    ApplicationSharedPtr app, mobile_apis::HMILevel::eType hmi_level) const {
  using namespace mobile_apis;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);

  mobile_apis::HMILevel::eType result = hmi_level;
  if (!Compare<HMILevel::eType, EQ, ONE>(hmi_level,
          HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)) {
    return result;
  }

  const bool is_audio_app = app->IsAudioApplication();
  const bool does_audio_app_with_same_type_exist =
      ApplicationManagerImpl::instance()->IsAppTypeExistsInFullOrLimited(app);
  if (HMILevel::HMI_LIMITED == hmi_level) {
    if (!is_audio_app || does_audio_app_with_same_type_exist) {
      result = ApplicationManagerImpl::instance()->GetDefaultHmiLevel(app);
    }
    return result;
  }

  const bool is_active_app_exist =
      ApplicationManagerImpl::instance()->active_application();
  if (is_audio_app) {
    if (does_audio_app_with_same_type_exist) {
      result = ApplicationManagerImpl::instance()->GetDefaultHmiLevel(app);
    } else if (is_active_app_exist) {
      result = mobile_apis::HMILevel::HMI_LIMITED;
    }
  } else if (is_active_app_exist) {
    result = ApplicationManagerImpl::instance()->GetDefaultHmiLevel(app);
  }

  return result;
}

bool StateController::IsStateAvailable(ApplicationSharedPtr app,
                                       HmiStatePtr state) const {
  using namespace mobile_apis;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Checking state: hmi_level " << state->hmi_level()
                << ", audio_state " << state->audio_streaming_state()
                << ", system_context " << state->system_context());

  if (!app->is_resuming()) {
    LOG4CXX_DEBUG(logger_, "Application is not in resuming mode."
                  << " Requested state is available");
    return true;
  }

  if (!Compare<HMILevel::eType, EQ, ONE>(state->hmi_level(),
          HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)) {
    LOG4CXX_WARN(logger_, "Application is going to resume to background."
                 << " Requested state is available");
    return true;
  }

  if (IsTempStateActive(HmiState::StateID::STATE_ID_VR_SESSION) ||
      IsTempStateActive(HmiState::StateID::STATE_ID_SAFETY_MODE)) {
    LOG4CXX_DEBUG(logger_, "Requested state is not available. "
                  << "VR session or emergency event is active");
    return false;
  }
  if (IsTempStateActive(HmiState::StateID::STATE_ID_PHONE_CALL) &&
      app->is_media_application()) {
    LOG4CXX_DEBUG(logger_, "Requested state for media application "
                  << "is not available. Phone call is active");
    return false;
  }

  LOG4CXX_DEBUG(logger_, "Requested state is available");
  return true;
}

void StateController::SetupRegularHmiState(ApplicationSharedPtr app,
                                           HmiStatePtr state) {
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(state);
  LOG4CXX_DEBUG(logger_, "hmi_level " << state->hmi_level() <<
                ", audio_state " << state->audio_streaming_state() <<
                ", system_context " << state->system_context());
  HmiStatePtr curr_state = app->CurrentHmiState();
  HmiStatePtr old_state = CreateHmiState(app->app_id(),
      HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(old_state);
  old_state->set_hmi_level(curr_state->hmi_level());
  old_state->set_audio_streaming_state(curr_state->audio_streaming_state());
  old_state->set_system_context(curr_state->system_context());
  app->SetRegularState(state);

  if (HMILevel::HMI_LIMITED == state->hmi_level() &&
          app->is_resuming()) {
    LOG4CXX_DEBUG(logger_, "Resuming to LIMITED level. "
                  << "Send OnResumeAudioSource notification");
    MessageHelper::SendOnResumeAudioSourceToHMI(app->app_id());
  }
  app->set_is_resuming(false);

  HmiStatePtr new_state = app->CurrentHmiState();
  OnStateChanged(app, old_state, new_state);
}

void StateController::SetupRegularHmiState(ApplicationSharedPtr app,
                                   const mobile_apis::HMILevel::eType hmi_level,
                                   const mobile_apis::AudioStreamingState::eType audio_state) {
  using namespace mobile_apis;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  HmiStatePtr prev_state = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(prev_state);
  HmiStatePtr new_state = CreateHmiState(app->app_id(),
      HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(new_state);
  new_state->set_hmi_level(hmi_level);
  new_state->set_audio_streaming_state(audio_state);
  new_state->set_system_context(prev_state->system_context());
  SetupRegularHmiState(app, new_state);
}

void StateController::ApplyRegularState(ApplicationSharedPtr app,
                                        HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  DCHECK_OR_RETURN_VOID(state);
  DCHECK_OR_RETURN_VOID(state->state_id() == HmiState::STATE_ID_REGULAR);
  SetupRegularHmiState(app, state);
  ForEachApplication<HmiLevelConflictResolver, ApplicationManagerImpl>
      (HmiLevelConflictResolver(app, state, this));
}

bool StateController::IsSameAppType(ApplicationConstSharedPtr app1,
                                  ApplicationConstSharedPtr app2) {
    return app1->is_media_application() == app2->is_media_application() ||
           app1->is_navi() == app2->is_navi() ||
           app1->is_voice_communication_supported() == app2->is_voice_communication_supported();
}

void StateController::on_event(const event_engine::Event& event) {
  using namespace smart_objects;
  using namespace event_engine;
  using namespace hmi_apis;

  LOG4CXX_AUTO_TRACE(logger_);
  const SmartObject& message = event.smart_object();
  const FunctionID::eType id = static_cast<FunctionID::eType> (event.id());
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
    case FunctionID::BasicCommunication_OnEmergencyEvent: {
      bool is_active =
          message[strings::msg_params][hmi_response::enabled].asBool();
      if (is_active) {
        OnSafetyModeEnabled();
      } else {
        OnSafetyModeDisabled();
      }
      break;
    }
    case FunctionID::BasicCommunication_OnPhoneCall: {
      bool is_active =
          message[strings::msg_params][hmi_notification::is_active].asBool();
      if (is_active) {
        OnPhoneCallStarted();
      } else {
        OnPhoneCallEnded();
      }
      break;
    }
    case FunctionID::VR_Started: {
      OnVRStarted();
      break;
    }
    case FunctionID::VR_Stopped: {
      OnVREnded();
      break;
    }
    case FunctionID::TTS_Started: {
      OnTTSStarted();
      break;
    }
    case FunctionID::TTS_Stopped: {
      OnTTSStopped();
      break;
    }
    default:
      break;
  }
}

void StateController::OnStateChanged(ApplicationSharedPtr app,
                                     HmiStatePtr old_state,
                                     HmiStatePtr new_state) {
  DCHECK_OR_RETURN_VOID(app);
  DCHECK_OR_RETURN_VOID(old_state);
  DCHECK_OR_RETURN_VOID(new_state);
  if (IsStatusChanged(old_state, new_state)) {
    MessageHelper::SendHMIStatusNotification(*app);
    if (new_state->hmi_level() == mobile_apis::HMILevel::HMI_NONE) {
      app->ResetDataInNone();
    }
    ApplicationManagerImpl::instance()->OnHMILevelChanged(app->app_id(),
        old_state->hmi_level(), new_state->hmi_level());
  } else {
    LOG4CXX_ERROR(logger_, "Status not changed");
  }
}

bool StateController::IsTempStateActive(HmiState::StateID ID) const {
  sync_primitives::AutoLock autolock(active_states_lock_);
  StateIDList::const_iterator itr =
      std::find(active_states_.begin(), active_states_.end(), ID);
  return active_states_.end() != itr;
}

void StateController::ApplyStatesForApp(ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(active_states_lock_);
  DCHECK_OR_RETURN_VOID(app);
  StateIDList::iterator it = active_states_.begin();
  for(; it != active_states_.end(); ++it) {
    HmiStatePtr new_state = CreateHmiState(app->app_id(), *it);
    DCHECK_OR_RETURN_VOID(new_state);
    DCHECK_OR_RETURN_VOID(new_state->state_id() != HmiState::STATE_ID_REGULAR);
    HmiStatePtr old_hmi_state = app->CurrentHmiState();
    new_state->set_parent(old_hmi_state);
    app->AddHMIState(new_state);
  }
}

void StateController::ApplyPostponedStateForApp(ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  HmiStatePtr state = app->PostponedHmiState();
  if (state) {
    state->set_state_id(HmiState::STATE_ID_REGULAR);
    SetRegularState(app, state);
  }
}

void StateController::TempStateStarted(HmiState::StateID ID) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(active_states_lock_);
  StateIDList::iterator it =
      std::find(active_states_.begin(), active_states_.end(), ID);
  if (it == active_states_.end()) {
      active_states_.push_back(ID);
  } else {
    LOG4CXX_ERROR(logger_, "StateID " << ID <<" is already active");
  }
}

void StateController::TempStateStopped(HmiState::StateID ID) {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock autolock(active_states_lock_);
    active_states_.remove(ID);
  }
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::ApplyPostponedStateForApp),
                       this)
                     );
}

void StateController::DeactivateAppWithGeneralReason(ApplicationSharedPtr app) {
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);

  DCHECK_OR_RETURN_VOID(app);
  HmiStatePtr regular = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(regular);
  HmiStatePtr new_regular = utils::MakeShared<HmiState>(*regular);

  if (app->IsAudioApplication()) {
    new_regular->set_hmi_level(mobile_api::HMILevel::HMI_LIMITED);
  } else {
    new_regular->set_hmi_level(mobile_api::HMILevel::HMI_BACKGROUND);
  }

  SetRegularState<false>(app, new_regular);
}

void StateController::DeactivateAppWithAudioReason(ApplicationSharedPtr app) {
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);

  DCHECK_OR_RETURN_VOID(app);
  HmiStatePtr regular = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(regular);
  HmiStatePtr new_regular = utils::MakeShared<HmiState>(*regular);

  if (app->is_navi()) {
    new_regular->set_hmi_level(HMILevel::HMI_LIMITED);
  } else {
    new_regular->set_audio_streaming_state(
        AudioStreamingState::NOT_AUDIBLE);
    new_regular->set_hmi_level(HMILevel::HMI_BACKGROUND);
  }

  SetRegularState<false>(app, new_regular);
}

void StateController::OnActivateAppResponse(
    const smart_objects::SmartObject& message) {
  const hmi_apis::Common_Result::eType code =
      static_cast<hmi_apis::Common_Result::eType>(
        message[strings::params][hmi_response::code].asInt());
  const int32_t correlation_id = message[strings::params]
                                 [strings::correlation_id].asInt();
  const uint32_t hmi_app_id = ApplicationManagerImpl::instance()->
                              application_id(correlation_id);
  ApplicationSharedPtr application = ApplicationManagerImpl::instance()->
                                     application_by_hmi_app(hmi_app_id);
  if (application) {
    HmiStatePtr pending_state = waiting_for_activate[application->app_id()];
    DCHECK_OR_RETURN_VOID(pending_state);
    if (code != hmi_apis::Common_Result::SUCCESS) {
      const HmiStatePtr cur = application->RegularHmiState();
      pending_state->set_hmi_level(cur->hmi_level());
    }
    ApplyRegularState(application, pending_state);
  }
}

void StateController::OnAppActivated(
    const smart_objects::SmartObject& message) {
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t app_id = message[strings::msg_params][strings::app_id].asUInt();
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application with id " << app_id << " not found");
    return;
  }

  SetRegularState<true>(app, HMILevel::HMI_FULL);
}

void StateController::OnAppDeactivated(
    const smart_objects::SmartObject& message) {
  using namespace hmi_apis;
  using namespace mobile_apis;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t app_id = message[strings::msg_params][strings::app_id].asUInt();
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application with id " << app_id << " not found");
    return;
  }

  if (Compare<HMILevel::eType, EQ, ONE>(
          app->hmi_level(), HMILevel::HMI_NONE, HMILevel::HMI_BACKGROUND)) {
    return;
  }

  Common_DeactivateReason::eType deactivate_reason =
      static_cast<Common_DeactivateReason::eType>
          (message[strings::msg_params][hmi_request::reason].asInt());

  switch (deactivate_reason) {
    case Common_DeactivateReason::AUDIO: {
      ForEachApplication(std::bind1st(
                           std::mem_fun(
                             &StateController::DeactivateAppWithAudioReason),
                           this)
                         );
      break;
    }
    case Common_DeactivateReason::NAVIGATIONMAP:
    case Common_DeactivateReason::PHONEMENU:
    case Common_DeactivateReason::SYNCSETTINGS:
    case Common_DeactivateReason::GENERAL: {
      DeactivateAppWithGeneralReason(app);
      break;
    }
    default:
      break;
  }
}

void StateController::OnPhoneCallStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStarted<HmiState::STATE_ID_PHONE_CALL>),
                       this)
                     );
  TempStateStarted(HmiState::STATE_ID_PHONE_CALL);
}

void StateController::OnPhoneCallEnded() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStopped<HmiState::STATE_ID_PHONE_CALL>),
                       this)
                    );
  TempStateStopped(HmiState::STATE_ID_PHONE_CALL);
}

void StateController::OnSafetyModeEnabled() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStarted<HmiState::STATE_ID_SAFETY_MODE>),
                       this)
                     );
  TempStateStarted(HmiState::STATE_ID_SAFETY_MODE);
}

void StateController::OnSafetyModeDisabled() {
  LOG4CXX_AUTO_TRACE(logger_);

  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStopped<HmiState::STATE_ID_SAFETY_MODE>),
                       this)
                    );
  TempStateStopped(HmiState::STATE_ID_SAFETY_MODE);
}

void StateController::OnVRStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStarted<HmiState::STATE_ID_VR_SESSION>),
                       this)
                     );
  TempStateStarted(HmiState::STATE_ID_VR_SESSION);
}

void StateController::OnVREnded() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStopped<HmiState::STATE_ID_VR_SESSION>),
                       this)
                    );
  TempStateStopped(HmiState::STATE_ID_VR_SESSION);
}

void StateController::OnTTSStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStarted<HmiState::STATE_ID_TTS_SESSION>),
                       this)
                     );
    TempStateStarted(HmiState::STATE_ID_TTS_SESSION);
}

void StateController::OnTTSStopped() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStopped<HmiState::STATE_ID_TTS_SESSION>),
                       this)
                     );
  TempStateStopped(HmiState::STATE_ID_TTS_SESSION);
}

void StateController::OnNaviStreamingStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStarted<HmiState::STATE_ID_NAVI_STREAMING>),
                       this)
                     );
    TempStateStarted(HmiState::STATE_ID_NAVI_STREAMING);
}

void StateController::OnNaviStreamingStopped() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStopped<HmiState::STATE_ID_NAVI_STREAMING>),
                       this)
                     );
  TempStateStopped(HmiState::STATE_ID_NAVI_STREAMING);
}

HmiStatePtr StateController::CreateHmiState(
    uint32_t app_id, HmiState::StateID state_id) const {
  using namespace utils;
  LOG4CXX_AUTO_TRACE(logger_);
  HmiStatePtr new_state;
  switch (state_id) {
    case HmiState::STATE_ID_PHONE_CALL: {
      new_state = MakeShared<PhoneCallHmiState>(app_id, state_context_);
      break;
    }
    case HmiState::STATE_ID_SAFETY_MODE: {
      new_state = MakeShared<SafetyModeHmiState>(app_id, state_context_);
      break;
    }
    case HmiState::STATE_ID_VR_SESSION: {
      new_state = MakeShared<VRHmiState>(app_id, state_context_);
      break;
    }
    case HmiState::STATE_ID_TTS_SESSION: {
      new_state = MakeShared<TTSHmiState>(app_id, state_context_);
      break;
    }
    case HmiState::STATE_ID_NAVI_STREAMING: {
      new_state = MakeShared<NaviStreamingHmiState>(app_id, state_context_);
      break;
    }
    case HmiState::STATE_ID_REGULAR: {
      new_state = MakeShared<HmiState>(app_id, state_context_);
      break;
    }
    case HmiState::STATE_ID_POSTPONED: {
      new_state = MakeShared<HmiState>(app_id, state_context_, state_id);
      break;
    }
    default:
      LOG4CXX_FATAL(logger_, "Invalid state_id " << state_id);
      NOTREACHED();
      break;
  }
  return new_state;
}

mobile_apis::AudioStreamingState::eType
StateController::CalcAudioState(ApplicationSharedPtr app,
    const mobile_apis::HMILevel::eType hmi_level) const {
  using namespace mobile_apis;
  using namespace helpers;

  AudioStreamingState::eType audio_state = AudioStreamingState::NOT_AUDIBLE;
  if (Compare<HMILevel::eType, EQ, ONE>(hmi_level, HMILevel::HMI_FULL,
                                        HMILevel::HMI_LIMITED)) {
    if (app->IsAudioApplication()) {
      audio_state = AudioStreamingState::AUDIBLE;
    }
  }
  return audio_state;
}

}
