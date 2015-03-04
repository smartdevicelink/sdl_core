#include "application_manager/hmi_state.h"
#include "utils/helpers.h"

namespace application_manager {

HmiState::HmiState(HmiStatePtr prev, StateID state_id):
  parent_(prev),
  state_id_(state_id),
  hmi_level_(mobile_apis::HMILevel::INVALID_ENUM),
  audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM),
  system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
}

HmiState::HmiState():
  state_id_(STATE_ID_REGULAR),
  hmi_level_(mobile_apis::HMILevel::INVALID_ENUM),
  audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM),
  system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
}

HmiState::HmiState(const HmiState& copy_from):
  state_id_(STATE_ID_REGULAR),
  hmi_level_(copy_from.hmi_level()),
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
  HmiState(previous, STATE_ID_VR_SESSION) {
}

TTSHmiState::TTSHmiState(HmiStatePtr previous):
  HmiState(previous, STATE_ID_TTS_SESSION) {
}

PhoneCallHmiState::PhoneCallHmiState(HmiStatePtr previous):
  HmiState(previous, STATE_ID_PHONE_CALL) {
}

SafetyModeHmiState::SafetyModeHmiState(HmiStatePtr previous):
  HmiState(previous, STATE_ID_SAFETY_MODE) {
  using namespace mobile_apis;
  audio_streaming_state_ = AudioStreamingState::NOT_AUDIBLE;
}

}
