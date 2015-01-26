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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTROLLER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTROLLER_H_

#include "interfaces/MOBILE_API.h"

namespace application_manager {

class StateController {
  public:
    enum StateID {
      STATE_ID_DEFAULT,
      STATE_ID_PHONE_CALL_STARTED,
      STATE_ID_PHONE_CALL_ENDED,
      STATE_ID_SAFETY_MODE_ENABLED,
      STATE_ID_SAFETY_MODE_DISABLED,
      STATE_ID_VR_STARTED,
      STATE_ID_VR_ENDED,
      STATE_ID_TTS_STARTED,
      STATE_ID_TTS_ENDED
    };

    void set_state(StateID id);
    void set_state(mobile_apis::HMILevel::eType hmi_level,
                   mobile_apis::AudioStreamingState::eType audio_state);

  private:
    void OnPhoneCallStarted();
    void OnPhoneCallEnded();
    void OnSafetyModeEnabled();
    void OnSafetyModeDisabled();
    void OnVRStarted();
    void OnVREnded();
    void OnTTSStarted();
    void OnTTSEnded();

  private:
    struct State {
      StateID id;
      mobile_apis::HMILevel::eType hmi_level;
      mobile_apis::AudioStreamingState::eType audio_state;
      mobile_apis::SystemContext::eType system_context;
    };

    State current_state_;
};

}

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTROLLER_H_

