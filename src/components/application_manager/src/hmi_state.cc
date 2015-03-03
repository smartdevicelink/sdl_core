#include "application_manager/hmi_state.h"
#include "utils/helpers.h"

namespace application_manager {

// GAL some thing wrong heres
HmiState::HmiState(HmiStatePtr prev):
  parent_(prev),
  hmi_level_(mobile_apis::HMILevel::INVALID_ENUM),
  audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM),
  system_context_(mobile_apis::SystemContext::INVALID_ENUM) {

}

HmiState::HmiState():
  state_id_(STATE_ID_REGULAR),
  parent_(NULL),
  hmi_level_(mobile_apis::HMILevel::INVALID_ENUM),
  audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM),
  system_context_(mobile_apis::SystemContext::INVALID_ENUM) {

}

HmiState::HmiState(const HmiState& copy_from):
  state_id_(STATE_ID_REGULAR),
  parent_(copy_from.parent()), hmi_level_(copy_from.hmi_level()),
  audio_streaming_state_(copy_from.audio_streaming_state()),
  system_context_(copy_from.system_context()) {

}

HmiState::HmiState(mobile_apis::HMILevel::eType hmi_level,
                                 mobile_apis::AudioStreamingState::eType audio_streaming_state,
                                 mobile_apis::SystemContext::eType system_context):
  state_id_(STATE_ID_REGULAR),
  hmi_level_(hmi_level),
  audio_streaming_state_(audio_streaming_state), system_context_(system_context) {
}

void HmiState::setParent(HmiStatePtr parent) {
  DCHECK_OR_RETURN_VOID(parent);
  parent_ = parent;
}

VRHmiState::VRHmiState(HmiStatePtr previous):
  HmiState(previous) {
}

TTSHmiState::TTSHmiState(HmiStatePtr previous):
  HmiState(previous) {
  using namespace mobile_apis;
  state_id_ = STATE_ID_TTS_SESSION;
  if (HMILevel::HMI_NONE != hmi_level() &&
      HMILevel::HMI_BACKGROUND!= hmi_level()) {
    audio_streaming_state_ = AudioStreamingState::ATTENUATED;
  } else {
    audio_streaming_state_ = previous->audio_streaming_state();
  }
}

PhoneCallHmiState::PhoneCallHmiState(HmiStatePtr previous):
  HmiState(previous) {
  state_id_ = STATE_ID_PHONE_CALL;
}

SafetyModeHmiState::SafetyModeHmiState(HmiStatePtr previous):
  HmiState(previous) {
  state_id_ = STATE_ID_SAFETY_MODE;
}

}
