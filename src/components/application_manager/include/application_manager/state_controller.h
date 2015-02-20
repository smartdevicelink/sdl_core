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
#include <list>

#include "application_manager/hmi_state.h"
#include "application_manager/application.h"

namespace application_manager {

class StateController {
  public:

    /**
     * @brief The StateEventID enum describes events to change HMIState
     * of applications
     */
    enum StateEventID {
      EVENT_ID_DEFAULT,
      EVENT_ID_PHONE_CALL_STARTED,
      EVENT_ID_PHONE_CALL_ENDED,
      EVENT_ID_SAFETY_MODE_ENABLED,
      EVENT_ID_SAFETY_MODE_DISABLED,
      EVENT_ID_VR_STARTED,
      EVENT_ID_VR_ENDED,
      EVENT_ID_TTS_STARTED,
      EVENT_ID_TTS_ENDED
    };

    /**
     * @brief ProcessStateEvent process event and change
     * HmiState for all applications
     * @param id - event ID to process
     */
    void ProcessStateEvent(const StateEventID id);

    /**
     * @brief SetDefaultState setup original hmiState, tha will appear if no
     * specific events are active
     * @param app appication to detup default State`
     * @param hmi_level hmi level of defailt state
     * @param audio_state audio_streaming state of default state
     */
    void SetDefaultState(ApplicationSharedPtr app,
                         const mobile_apis::HMILevel::eType hmi_level,
                         const  mobile_apis::AudioStreamingState::eType audio_state);


    /**
     * @brief SetDefaultState setup original hmiState, tha will appear if no
     * specific events are active
     * @param app appication to detup default State`
     * @param hmi_level hmi level of defailt state
     * @param audio_state audio_streaming state of default state
     */
    void SetDefaultState(ApplicationSharedPtr app,
                         utils::SharedPtr<HmiState> state);

    /**
     * @brief setSystemContext setup new system_context for all all applications
     * @param system_context system context to setup
     */
    void SetSystemContext(const mobile_apis::SystemContext::eType system_context);

  private:
    /**
     * @brief OnPhoneCallStarted process Phone Call Started event
     */
    void OnPhoneCallStarted();

    /**
     * @brief OnPhoneCallEnded process Phone Call Ended event
     */
    void OnPhoneCallEnded();


    /**
     * @brief OnSafetyModeEnabled process Safety Mode Enable event
     */
    void OnSafetyModeEnabled();

    /**
     * @brief OnSafetyModeDisabled process Safety Mode Disable event
     */
    void OnSafetyModeDisabled();

    /**
     * @brief OnVRStarted process VR session started
     */
    void OnVRStarted();

    /**
     * @brief OnVREnded process VR session ended
     */
    void OnVREnded();
    /**
     * @brief OnTTSStarted process TTS session started
     */
    void OnTTSStarted();

    /**
     * @brief OnTTSEnded process TTS session ended
     */
    void OnTTSEnded();

  private:

    /**
     * @brief Active states of application
     */
    std::list<HmiState::StateID> current_state_;
    mobile_apis::SystemContext::eType system_context_;
};

}

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTROLLER_H_

