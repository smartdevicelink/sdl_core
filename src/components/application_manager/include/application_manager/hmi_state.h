#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMISTATE_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMISTATE_H

#include <list>
#include "interfaces/MOBILE_API.h"
#include "utils/shared_ptr.h"

namespace application_manager {

class HmiState;
typedef utils::SharedPtr<HmiState> HmiStatePtr;
typedef std::list<HmiStatePtr > HmiStateList;

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

    HmiState(HmiStatePtr parent);
    HmiState();

    HmiState(const HmiState& copy_from);

    HmiState(mobile_apis::HMILevel::eType hmi_level,
                    mobile_apis::AudioStreamingState::eType audio_streaming_state,
                    mobile_apis::SystemContext::eType system_context);

    virtual ~HmiState() {};

    void setParent(HmiStatePtr parent);

    const HmiStatePtr parent() const {
      return parent_;
    }

    /**
     * @brief hmi_level
     * @return return hmi level member
     */
    virtual mobile_apis::HMILevel::eType hmi_level() const {
      return hmi_level_;
    }

    /**
     * @brief audio_streaming_state
     * @return return audio streaming state member
     */
    virtual mobile_apis::AudioStreamingState::eType audio_streaming_state() const {
      return audio_streaming_state_;
    }

    /**
     * @brief system_context
     * @return return system context member
     */
    virtual mobile_apis::SystemContext::eType system_context() const {
      return system_context_;
    }
    StateID state_id() const {
      return state_id_;
    }
  protected:
    StateID state_id_;
    HmiStatePtr parent_;
    mobile_apis::HMILevel::eType hmi_level_;
    mobile_apis::AudioStreamingState::eType audio_streaming_state_;
    mobile_apis::SystemContext::eType system_context_;

  private:
    void operator=(const HmiState&);
};

class VRHmiState : public HmiState {
  public:
    VRHmiState(HmiStatePtr parent);
};

class TTSHmiState : public HmiState {
  public:
    TTSHmiState(HmiStatePtr parent);
    mobile_apis::AudioStreamingState::eType audio_streaming_state() const {
      return parent()->audio_streaming_state();
    }
};

class PhoneCallHmiState : public HmiState {
  public:
    PhoneCallHmiState(HmiStatePtr parent);
};

class SafetyModeHmiState : public HmiState {
  public:
    SafetyModeHmiState(HmiStatePtr parent);

    mobile_apis::SystemContext::eType system_context() const {
      return parent()->system_context();
    }

    mobile_apis::HMILevel::eType hmi_level() const {
      return parent()->hmi_level();
    }
};



}
#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMISTATE_H
