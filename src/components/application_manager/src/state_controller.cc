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
  HmiStatePtr prev_regular = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(prev_regular);
  HmiStatePtr hmi_state(new HmiState(prev_regular->hmi_level(),
                                     audio_state,
                                     prev_regular->system_context()));
  SetRegularState<false>(app, hmi_state);
}



void StateController::HmiLevelConflictResolver::operator ()
    (ApplicationSharedPtr to_resolve) {
  using namespace mobile_apis;
  using namespace helpers;
  DCHECK_OR_RETURN_VOID(state_ctrl_);
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
  HmiStatePtr old_state(new HmiState(*(app->CurrentHmiState())));
  app->SetRegularState(state);
  HmiStatePtr new_state = app->RegularHmiState();
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
  HmiStatePtr new_state(new HmiState(hmi_level,
                                     audio_state,
                                     prev_state->system_context()));
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
          message[strings::msg_params][hmi_notification::is_active].asBool();
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
  } else {
    LOG4CXX_ERROR(logger_, "Status not changed");
  }
}

template<>
void StateController::HMIStateStarted<PhoneCallHmiState>(ApplicationSharedPtr app) {
  using namespace mobile_apis;
  HmiStatePtr old_hmi_state = app->CurrentHmiState();
  HmiStatePtr new_hmi_state(new PhoneCallHmiState(old_hmi_state));

  new_hmi_state->set_hmi_level(app->is_navi() ? HMILevel::HMI_LIMITED :
                                                HMILevel::HMI_BACKGROUND);
  new_hmi_state->set_audio_streaming_state(AudioStreamingState::NOT_AUDIBLE);

  app->AddHMIState(new_hmi_state);
  OnStateChanged(app,old_hmi_state, new_hmi_state);
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
                         &StateController::HMIStateStarted<PhoneCallHmiState>),
                       this)
                     );
}

void StateController::OnPhoneCallEnded() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStopped<HmiState::STATE_ID_PHONE_CALL>),
                       this)
                    );
}

void StateController::OnSafetyModeEnabled() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStarted<SafetyModeHmiState>),
                       this)
                     );
}

void StateController::OnSafetyModeDisabled() {
  LOG4CXX_AUTO_TRACE(logger_);

  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStopped<HmiState::STATE_ID_SAFETY_MODE>),
                       this)
                    );
}

void StateController::OnVRStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void StateController::OnVREnded() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void StateController::OnTTSStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStarted<TTSHmiState>),
                       this)
                     );
}

void StateController::OnTTSStopped() {
  LOG4CXX_AUTO_TRACE(logger_);
  ForEachApplication(std::bind1st(
                       std::mem_fun(
                         &StateController::HMIStateStopped<HmiState::STATE_ID_TTS_SESSION>),
                       this)
                    );
}

}
