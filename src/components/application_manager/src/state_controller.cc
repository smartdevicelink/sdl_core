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
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnEmergencyEvent);
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnPhoneCall);
  subscribe_on_event(hmi_apis::FunctionID::TTS_Started);
  subscribe_on_event(hmi_apis::FunctionID::TTS_Stopped);
  subscribe_on_event(hmi_apis::FunctionID::VR_Started);
  subscribe_on_event(hmi_apis::FunctionID::VR_Stopped);
}

void StateController::SetRegularState(ApplicationSharedPtr app,
                                      const mobile_apis::AudioStreamingState::eType audio_state) {
  DCHECK_OR_RETURN_VOID(app);
  HmiStatePtr prev_state = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(prev_state);
  HmiStatePtr hmi_state = CreateHmiState(app->app_id(),
                                         HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(hmi_state);
  hmi_state->set_hmi_level(prev_state->hmi_level());
  hmi_state->set_audio_streaming_state(audio_state);
  hmi_state->set_system_context(prev_state->system_context());
  SetRegularState<false>(app, hmi_state);
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
      } else {
        state_ctrl_->SetupRegularHmiState(to_resolve, HMILevel::HMI_LIMITED,
                     AudioStreamingState::AUDIBLE);
      }
    }
  }
}

void StateController::SetupRegularHmiState(ApplicationSharedPtr app,
                                           HmiStatePtr state) {
  HmiStatePtr curr_state = app->CurrentHmiState();
  HmiStatePtr old_state = CreateHmiState(app->app_id(),
                                         HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(old_state);
  old_state->set_hmi_level(curr_state->hmi_level());
  old_state->set_audio_streaming_state(curr_state->audio_streaming_state());
  old_state->set_system_context(curr_state->system_context());
  app->SetRegularState(state);
  if (state->hmi_level() == mobile_apis::HMILevel::HMI_NONE) {
    app->ResetDataInNone();
  }
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
  } else {
    LOG4CXX_ERROR(logger_, "Status not changed");
  }
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
  sync_primitives::AutoLock autolock(active_states_lock_);
  active_states_.remove(ID);
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

HmiStatePtr StateController::CreateHmiState(uint32_t app_id, HmiState::StateID state_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  HmiStatePtr new_state;
  switch (state_id) {
    case HmiState::STATE_ID_PHONE_CALL: {
      new_state.reset(new PhoneCallHmiState(app_id, state_context_));
      break;
    }
    case HmiState::STATE_ID_SAFETY_MODE: {
      new_state.reset(new SafetyModeHmiState(app_id, state_context_));
      break;
    }
    case HmiState::STATE_ID_VR_SESSION: {
      new_state.reset(new VRHmiState(app_id, state_context_));
      break;
    }
    case HmiState::STATE_ID_TTS_SESSION: {
      new_state.reset(new TTSHmiState(app_id, state_context_));
      break;
    }
    case HmiState::STATE_ID_REGULAR: {
      new_state.reset(new HmiState(app_id, state_context_));
      break;
    }
    default:
      LOG4CXX_FATAL(logger_, "Invalid state_id " << state_id);
      NOTREACHED();
      break;
  }
  return new_state;
}

}
