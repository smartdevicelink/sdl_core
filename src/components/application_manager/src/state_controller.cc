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

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "StateController")

bool IsStatusChanged(HmiStatePtr old_state,
                                      HmiStatePtr new_state) {
  if (old_state->hmi_level() != new_state->hmi_level()
      && old_state->audio_streaming_state() != new_state->audio_streaming_state()
      && old_state->system_context() != new_state->system_context() ) {
    return true;
  }
  return false;
}

StateController::StateController():EventObserver() {
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnEmergencyEvent);
  subscribe_on_event(hmi_apis::FunctionID::TTS_Started);
  subscribe_on_event(hmi_apis::FunctionID::TTS_Stopped);

}

void StateController::SetRegularState(ApplicationSharedPtr app,
                                      const mobile_apis::HMILevel::eType hmi_level,
                                      const mobile_apis::AudioStreamingState::eType audio_state) {
  HmiStatePtr hmi_state(new HmiState(hmi_level, audio_state,
                                                    system_context_));
  SetRegularState(app, hmi_state);
}

void StateController::SetRegularState(ApplicationSharedPtr app,
                                      HmiStatePtr state) {
  DCHECK_OR_RETURN_VOID(app);
  DCHECK_OR_RETURN_VOID(state);
  HmiStatePtr old_state(new HmiState(*(app->CurrentHmiState())));
  app->SetRegularState(state);
  HmiStatePtr new_state = app->CurrentHmiState();

  if (IsStatusChanged(old_state, new_state)) {
    OnStateChanged(app, old_state, new_state);
  }
}

void StateController::SetSystemContext(const mobile_apis::SystemContext::eType system_context) {
  system_context_ = system_context;
  //TODO (APPLINK-8555) Need to setup system context for app applications
}

void StateController::on_event(const event_engine::Event& event) {
  using namespace smart_objects;
  using namespace event_engine;
  using namespace hmi_apis;

  LOG4CXX_AUTO_TRACE(logger_);
  const SmartObject& message = event.smart_object();
  const FunctionID::eType id = static_cast<FunctionID::eType> (event.id());
  switch (id) {
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

}

void StateController::OnPhoneCallStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void StateController::OnPhoneCallEnded() {
  LOG4CXX_AUTO_TRACE(logger_);
}

template <typename HMIStatusNAme>
void HMIStateStarted () {
  using namespace utils;
  ApplicationManagerImpl::ApplicationListAccessor accessor;
  for (ApplicationManagerImpl::ApplictionSetConstIt it = accessor.begin();
       it != accessor.end(); ++it) {
    if (it->valid()) {
      ApplicationConstSharedPtr const_app = *it;
      ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(const_app->app_id());
      DCHECK_OR_RETURN_VOID(app);
      HmiStatePtr old_hmi_state = const_app->CurrentHmiState();
      HmiStatePtr new_hmi_state(new HMIStatusNAme(old_hmi_state));
      app->AddHMIState(new_hmi_state);
      if (IsStatusChanged(old_hmi_state, new_hmi_state)) {
        MessageHelper::SendHMIStatusNotification(*app);
      } else {
        LOG4CXX_FATAL(logger_, "Invalid pointer in application collection");
      }
    }
  }
}

void HMIStateStopped(HmiState::StateID state_id) {
  using namespace utils;
  ApplicationManagerImpl::ApplicationListAccessor accessor;
  for (ApplicationManagerImpl::ApplictionSetConstIt it = accessor.begin();
       it != accessor.end(); ++it) {
    if (it->valid()) {
      ApplicationConstSharedPtr const_app = *it;
      ApplicationSharedPtr app =
          ApplicationManagerImpl::instance()->application(const_app->app_id());
      DCHECK_OR_RETURN_VOID(app);
      HmiStatePtr old_hmi_state(new HmiState(*(const_app->CurrentHmiState())));

      app->RemoveHMIState(state_id);
      HmiStatePtr new_hmi_state = const_app->CurrentHmiState();
      if (IsStatusChanged(old_hmi_state, new_hmi_state)) {
        MessageHelper::SendHMIStatusNotification(*app);
      }
    } else {
      LOG4CXX_FATAL(logger_, "Invalid pointer in application collection");
    }
  }
}

void StateController::OnSafetyModeEnabled() {
  LOG4CXX_AUTO_TRACE(logger_);
  HMIStateStarted<SafetyModeHmiState>();
}

void StateController::OnSafetyModeDisabled() {
  LOG4CXX_AUTO_TRACE(logger_);
  HMIStateStopped(HmiState::STATE_ID_SAFETY_MODE);
}

void StateController::OnVRStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void StateController::OnVREnded() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void StateController::OnTTSStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
  HMIStateStarted<TTSHmiState>();
}

void StateController::OnTTSStopped() {
  LOG4CXX_AUTO_TRACE(logger_);
  HMIStateStopped(HmiState::STATE_ID_TTS_SESSION);
}

}
