#include "application_manager/hmi_state.h"

namespace application_manager {


HmiState::HmiState(utils::SharedPtr<HmiState> prev):
  previous_(prev),
  hmi_level_(mobile_apis::HMILevel::INVALID_ENUM),
  audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM),
  system_context_(mobile_apis::SystemContext::INVALID_ENUM) {

}


HmiState::HmiState(mobile_apis::HMILevel::eType hmi_level,
                                 mobile_apis::AudioStreamingState::eType audio_streaming_state,
                                 mobile_apis::SystemContext::eType system_context):
  previous_(NULL), hmi_level_(hmi_level),
  audio_streaming_state_(audio_streaming_state), system_context_(system_context) {
}


VRHmiState::VRHmiState(utils::SharedPtr<HmiState> previous):
  HmiState(previous) {
}

TTSHmiState::TTSHmiState(utils::SharedPtr<HmiState> previous):
  HmiState(previous) {

}

PhoneCallHmiState::PhoneCallHmiState(utils::SharedPtr<HmiState> previous):
  HmiState(previous) {

}

SafetyModeHmiState::SafetyModeHmiState(utils::SharedPtr<HmiState> previous):
  HmiState(previous) {

}


}
