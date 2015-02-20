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

namespace application_manager {

void StateController::ProcessStateEvent(const StateEventID id) {
  switch (id) {
    case EVENT_ID_PHONE_CALL_STARTED:
      OnPhoneCallStarted(); break;
    case EVENT_ID_PHONE_CALL_ENDED:
      OnPhoneCallEnded(); break;
    case EVENT_ID_SAFETY_MODE_ENABLED:
      OnSafetyModeEnabled(); break;
    case EVENT_ID_SAFETY_MODE_DISABLED:
      OnSafetyModeDisabled(); break;
    case EVENT_ID_TTS_STARTED:
      OnTTSStarted(); break;
    case EVENT_ID_TTS_ENDED:
      OnTTSEnded(); break;
    case EVENT_ID_VR_STARTED:
      OnVRStarted(); break;
    case EVENT_ID_VR_ENDED:
      OnVREnded(); break;
    default:
      break;
  }
}

void StateController::SetDefaultState(ApplicationSharedPtr app,
                                      const mobile_apis::HMILevel::eType hmi_level,
                                      const mobile_apis::AudioStreamingState::eType audio_state) {
  HmiStateList& default_hmi_state = app->GetHmiStateList();
  DCHECK_OR_RETURN_VOID(default_hmi_state.empty() == false);
  utils::SharedPtr<HmiState> hmi_state(new HmiState(hmi_level,
                                                    audio_state,
                                                    system_context_));
  default_hmi_state.erase(default_hmi_state.begin());
  default_hmi_state.push_front(hmi_state);
}

void StateController::SetDefaultState(ApplicationSharedPtr app,
                                      utils::SharedPtr<HmiState> state) {
  DCHECK_OR_RETURN_VOID(state)
  HmiStateList& default_hmi_state = app->GetHmiStateList();
  DCHECK_OR_RETURN_VOID(default_hmi_state.empty() == false);
  default_hmi_state.erase(default_hmi_state.begin());
  default_hmi_state.push_front(state);
}

void StateController::SetSystemContext(const mobile_apis::SystemContext::eType system_context) {
  system_context_ = system_context;
  //TODO (APPLINK-8555) Need to setup system context for app applications
}

void StateController::OnPhoneCallStarted() {

}

void StateController::OnPhoneCallEnded() {

}

void StateController::OnSafetyModeEnabled() {

}

void StateController::OnSafetyModeDisabled() {

}

void StateController::OnVRStarted() {

}

void StateController::OnVREnded() {

}

void StateController::OnTTSStarted() {

}

void StateController::OnTTSEnded() {

}

}
