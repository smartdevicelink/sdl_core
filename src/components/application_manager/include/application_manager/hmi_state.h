#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMISTATE_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMISTATE_H

#include "interfaces/MOBILE_API.h"
#include "utils/shared_ptr.h"
#include <list>

namespace application_manager {

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
      STATE_ID_DEFAULT,
      EVENT_ID_PHONE_CALL,
      EVENT_ID_SAFETY_MODE,
      EVENT_ID_VR,
      EVENT_ID_TTS,
    };

    HmiState(utils::SharedPtr<HmiState> previous);
    HmiState(mobile_apis::HMILevel::eType hmi_level,
                    mobile_apis::AudioStreamingState::eType audio_streaming_state,
                    mobile_apis::SystemContext::eType system_context);

    const utils::SharedPtr<HmiState> previous() {
      return previous_;
    }

    /**
     * @brief hmi_level
     * @return return hmi level member
     */
    mobile_apis::HMILevel::eType hmi_level() const {
      return hmi_level_;
    }

    /**
     * @brief audio_streaming_state
     * @return return audio streaming state member
     */
    mobile_apis::AudioStreamingState::eType audio_streaming_state() const {
      return audio_streaming_state_;
    }

    /**
     * @brief system_context
     * @return return system context member
     */
    mobile_apis::SystemContext::eType system_context() const {
      return system_context_;
    }

  private:
    utils::SharedPtr<HmiState>  previous_;
    mobile_apis::HMILevel::eType hmi_level_;
    mobile_apis::AudioStreamingState::eType audio_streaming_state_;
    mobile_apis::SystemContext::eType system_context_;\

    DISALLOW_COPY_AND_ASSIGN(HmiState);
};

typedef std::list<utils::SharedPtr<HmiState> > HmiStateList;

class VRHmiState : public HmiState {
  public:
    VRHmiState(utils::SharedPtr<HmiState> previous);
};

class TTSHmiState : public HmiState {
  public:
    TTSHmiState(utils::SharedPtr<HmiState> previous);
};

class PhoneCallHmiState : public HmiState {
  public:
    PhoneCallHmiState(utils::SharedPtr<HmiState> previous);
};

class SafetyModeHmiState : public HmiState {
  public:
    SafetyModeHmiState(utils::SharedPtr<HmiState> previous);
};



}
#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMISTATE_H
