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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_STATE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_STATE_H_

#include <list>
#include "interfaces/MOBILE_API.h"
#include "utils/shared_ptr.h"
#include "utils/macro.h"

namespace application_manager {

class HmiState;
class ApplicationManager;
class Application;

typedef utils::SharedPtr<HmiState> HmiStatePtr;

/**
* @brief The HmiState class
*  Handle Hmi state of application (hmi level,
*  audio streaming state, system context)
*
*/
class HmiState {
 public:
  /**
   * @brief The StateID enum describes state of application
   * If no events occured STATE_ID_DEFAULT shuld be presented
   */
  enum StateID {
    STATE_ID_CURRENT,
    STATE_ID_REGULAR,
    STATE_ID_POSTPONED,
    STATE_ID_PHONE_CALL,
    STATE_ID_SAFETY_MODE,
    STATE_ID_VR_SESSION,
    STATE_ID_TTS_SESSION,
    STATE_ID_NAVI_STREAMING,
    STATE_ID_DEACTIVATE_HMI,
    STATE_ID_AUDIO_SOURCE,
    STATE_ID_EMBEDDED_NAVI
  };

  /**
   * @brief HmiState constructor
   * @param app Application pointer
   * @param app_mngr Application manager
   */
  HmiState(utils::SharedPtr<Application> app,
           const ApplicationManager& app_mngr);

  /**
   * @brief HmiState constructor
   * @param app Application pointer
   * @param app_mngr Application manager
   * @param state_id HMI state to assign
   */
  HmiState(utils::SharedPtr<Application> app,
           const ApplicationManager& app_mngr,
           StateID state_id);

  /**
   * DEPRECATED
   * @brief HmiState constructor
   * @param app_id Application id
   * @param app_mngr Application manager
   */
  HmiState(uint32_t app_id, const ApplicationManager& app_mngr);

  /**
   * DEPRECATED
   * @brief HmiState constructor
   * @param app_id Application id
   * @param app_mngr Application manager
   * @param state_id HMI state to assign
   */
  HmiState(uint32_t app_id,
           const ApplicationManager& app_mngr,
           StateID state_id);

  virtual ~HmiState() {}

  /**
   * @brief setParent setup parent state
   * @param parent state to setup
   */
  void set_parent(HmiStatePtr parent);

  /**
   * @brief parent get parent state
   * @return parent state
   */
  const HmiStatePtr parent() const {
    return parent_;
  }

  /**
   * @brief hmi_level
   * @return return hmi level member
   */
  virtual mobile_apis::HMILevel::eType hmi_level() const {
    if (parent_) {
      return parent_->hmi_level();
    }
    return hmi_level_;
  }
  /**
   * @brief set_hmi_level set hmi_level member
   * @param hmi_level hmi level to setup
   */
  void set_hmi_level(mobile_apis::HMILevel::eType hmi_level) {
    hmi_level_ = hmi_level;
  }

  /**
   * @brief audio_streaming_state
   * @return return audio streaming state member
   */
  virtual mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const {
    if (parent_) {
      return parent_->audio_streaming_state();
    }
    return audio_streaming_state_;
  }

  /**
   * @brief video_streaming_state
   * @return return video streaming state member
   */
  virtual mobile_apis::VideoStreamingState::eType video_streaming_state()
      const {
    if (parent_) {
      return parent_->video_streaming_state();
    }
    return video_streaming_state_;
  }

  /**
   * @brief set_video_streaming_state set set_video_streaming_state member
   * @param video_state video_state to setup
   */
  virtual void set_video_streaming_state(
      mobile_apis::VideoStreamingState::eType video_state) {
    video_streaming_state_ = video_state;
  }

  /**
   * @brief set_audio_streaming_state set audio_streaming_state member
   * @param audio_state audio_state to setup
   */
  virtual void set_audio_streaming_state(
      mobile_apis::AudioStreamingState::eType audio_state) {
    audio_streaming_state_ = audio_state;
  }

  /**
   * @brief system_context
   * @return return system context member
   */
  virtual mobile_apis::SystemContext::eType system_context() const {
    if (parent_) {
      return parent_->system_context();
    }
    return system_context_;
  }

  /**
   * @brief set_system_context set system_context member
   * @param system_context system_context to setup
   */
  virtual void set_system_context(
      mobile_apis::SystemContext::eType system_context) {
    system_context_ = system_context;
  }

  /**
   * @brief state_id state type
   * @return return state type
   */
  StateID state_id() const {
    return state_id_;
  }

  /**
   * @brief set_state_id sets state id
   * @param state_id state id to setup
   */
  virtual void set_state_id(StateID state_id) {
    state_id_ = state_id;
  }

 protected:
  utils::SharedPtr<Application> app_;
  StateID state_id_;
  const ApplicationManager& app_mngr_;
  HmiStatePtr parent_;
  mobile_apis::HMILevel::eType hmi_level_;
  mobile_apis::AudioStreamingState::eType audio_streaming_state_;
  mobile_apis::VideoStreamingState::eType video_streaming_state_;
  mobile_apis::SystemContext::eType system_context_;

 protected:
  /**
   * @brief is_navi_app check if app is navi
   * @return true if app is navi, otherwise return false
   */
  bool is_navi_app() const;

  /**
   * @brief is_media_app check if app is media
   * @return true if media_app, otherwise return false
   */
  bool is_media_app() const;

  /**
   * @brief is_voice_communicationn_app check if app is voice comunication
   * @return true if voice_communicationn_app, otherwise return false
   */
  bool is_voice_communication_app() const;

  /**
   * @brief is_mobile_projection_app checks if application of 'projection'
   * HMI type
   * @return True if application is of 'projection' HMI type, otherwise - false
   */
  bool is_mobile_projection_app() const;

 private:
  void operator=(const HmiState&);
};

/**
 * @brief The VRHmiState class implements logic of VR temporary state
 */
class VRHmiState : public HmiState {
 public:
  virtual mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const OVERRIDE;
  VRHmiState(utils::SharedPtr<Application> app,
             const ApplicationManager& app_mngr);

  DEPRECATED VRHmiState(uint32_t app_id, const ApplicationManager& app_mngr);
};

/**
 * @brief The TTSHmiState class implements logic of TTS temporary state
 */
class TTSHmiState : public HmiState {
 public:
  TTSHmiState(utils::SharedPtr<Application> app,
              const ApplicationManager& app_mngr);

  DEPRECATED TTSHmiState(uint32_t app_id, const ApplicationManager& app_mngr);

  virtual mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const OVERRIDE;
};

/**
 * @brief The NaviStreamingState class implements logic of NaviStreaming
 * temporary state
 */
class NaviStreamingHmiState : public HmiState {
 public:
  NaviStreamingHmiState(utils::SharedPtr<Application> app,
                        const ApplicationManager& app_mngr);

  DEPRECATED NaviStreamingHmiState(uint32_t app_id,
                                   const ApplicationManager& app_mngr);

  mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const OVERRIDE;
  mobile_apis::VideoStreamingState::eType video_streaming_state()
      const OVERRIDE;
};

/**
 * @brief The PhoneCallHmiState class implements logic of PhoneCall temporary
 * state
 */
class PhoneCallHmiState : public HmiState {
 public:
  PhoneCallHmiState(utils::SharedPtr<Application> app,
                    const ApplicationManager& app_mngr);

  DEPRECATED PhoneCallHmiState(uint32_t app_id,
                               const ApplicationManager& app_mngr);

  mobile_apis::HMILevel::eType hmi_level() const OVERRIDE;
  mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const OVERRIDE {
    return mobile_apis::AudioStreamingState::NOT_AUDIBLE;
  }
};

/**
 * @brief The SafetyModeHmiState class implements logic of SafetyMode temporary
 * state
 */
class SafetyModeHmiState : public HmiState {
 public:
  SafetyModeHmiState(utils::SharedPtr<Application> app,
                     const ApplicationManager& app_mngr);

  DEPRECATED SafetyModeHmiState(uint32_t app_id,
                                const ApplicationManager& app_mngr);

  mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const OVERRIDE {
    return mobile_apis::AudioStreamingState::NOT_AUDIBLE;
  }
};

/**
 * @brief The DeactivateHMI class implements logic of DeactivateHMI temporary
 * state
 */
class DeactivateHMI : public HmiState {
 public:
  DeactivateHMI(utils::SharedPtr<Application> app,
                const ApplicationManager& app_mngr);

  DEPRECATED DeactivateHMI(uint32_t app_id, const ApplicationManager& app_mngr);

  mobile_apis::HMILevel::eType hmi_level() const OVERRIDE;
  mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const OVERRIDE {
    return mobile_apis::AudioStreamingState::NOT_AUDIBLE;
  }
};

/**
 * @brief The AudioSource class implements logic of OnEventChanged(AUDIO_SOURCE)
 * temporary state
 */
class AudioSource : public HmiState {
 public:
  AudioSource(utils::SharedPtr<Application> app,
              const ApplicationManager& app_mngr);

  DEPRECATED AudioSource(uint32_t app_id, const ApplicationManager& app_mngr);

  mobile_apis::HMILevel::eType hmi_level() const OVERRIDE;
  mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const OVERRIDE {
    return mobile_apis::AudioStreamingState::NOT_AUDIBLE;
  }
};

/**
 * @brief The EmbeddedNavi class implements logic of
 * OnEventChanged(EMBEDDED_NAVI)
 * temporary state
 */
class EmbeddedNavi : public HmiState {
 public:
  EmbeddedNavi(utils::SharedPtr<Application> app,
               const ApplicationManager& app_mngr);

  DEPRECATED EmbeddedNavi(uint32_t app_id, const ApplicationManager& app_mngr);

  mobile_apis::HMILevel::eType hmi_level() const OVERRIDE;
  mobile_apis::AudioStreamingState::eType audio_streaming_state()
      const OVERRIDE {
    return mobile_apis::AudioStreamingState::NOT_AUDIBLE;
  }
};
}
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_STATE_H_
