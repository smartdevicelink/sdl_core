#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMISTATE_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMISTATE_H

#include <list>
#include "interfaces/MOBILE_API.h"
#include "utils/shared_ptr.h"
#include "application_manager/state_context.h"

namespace application_manager {

class HmiState;
typedef utils::SharedPtr<HmiState> HmiStatePtr;
typedef std::list<HmiStatePtr> HmiStateList;

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
      STATE_ID_REGULAR,
      STATE_ID_PHONE_CALL,
      STATE_ID_SAFETY_MODE,
      STATE_ID_VR_SESSION,
      STATE_ID_TTS_SESSION,
    };

    HmiState(uint32_t app_id, const StateContext& state_context_);
    HmiState(uint32_t app_id, const StateContext& state_context_,
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
    virtual mobile_apis::AudioStreamingState::eType
    audio_streaming_state() const {
      if (parent_) {
        return parent_->audio_streaming_state();
      }
      return audio_streaming_state_;
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
        mobile_apis::SystemContext::eType system_context){
      system_context_ = system_context;
    }

    /**
     * @brief state_id state type
     * @return reutrn state type
     */
    StateID state_id() const {
      return state_id_;
    }
  protected:
    uint32_t app_id_;
    StateID state_id_;
    const StateContext& state_context_;
    HmiStatePtr parent_;
    mobile_apis::HMILevel::eType hmi_level_;
    mobile_apis::AudioStreamingState::eType audio_streaming_state_;
    mobile_apis::SystemContext::eType system_context_;
  private:
    void operator=(const HmiState&);
};

/**
 * @brief The VRHmiState class impement logic of VR temporary state
 */
class VRHmiState : public HmiState {
  public:
    virtual mobile_apis::AudioStreamingState::eType audio_streaming_state() const;
    VRHmiState(uint32_t app_id, StateContext& state_context);
};

/**
 * @brief The TTSHmiState class impement logic of TTS temporary state
 */
class TTSHmiState : public HmiState {
  public:
    TTSHmiState(uint32_t app_id, StateContext& state_context);
    virtual  mobile_apis::AudioStreamingState::eType audio_streaming_state() const;
};

/**
 * @brief The PhoneCallHmiState class impement logic of PhoneCall temporary state
 */
class PhoneCallHmiState : public HmiState {
  public:
    PhoneCallHmiState(uint32_t app_id, StateContext& state_context);
    virtual mobile_apis::HMILevel::eType hmi_level() const;
    virtual  mobile_apis::AudioStreamingState::eType audio_streaming_state() const {
      return mobile_apis::AudioStreamingState::NOT_AUDIBLE;
    }
};

/**
 * @brief The SafetyModeHmiState class impement logic of SafetyMode temporary state
 */
class SafetyModeHmiState : public HmiState {
  public:
    SafetyModeHmiState(uint32_t app_id, StateContext& state_context);
    virtual  mobile_apis::AudioStreamingState::eType audio_streaming_state() const {
      return mobile_apis::AudioStreamingState::NOT_AUDIBLE;
    }
};

}
#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMISTATE_H
