#include "application_manager/hmi_state.h"
#include "utils/helpers.h"

namespace application_manager {

HmiState::HmiState(uint32_t app_id, const StateContext& state_context_,
                   StateID state_id):
  app_id_(app_id),
  state_id_(state_id),
  state_context_(state_context_),
  hmi_level_(mobile_apis::HMILevel::INVALID_ENUM),
  audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM),
  system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
}


HmiState::HmiState(uint32_t app_id, const StateContext& state_context):
  app_id_(app_id),
  state_id_(STATE_ID_REGULAR),
  state_context_(state_context),
  hmi_level_(mobile_apis::HMILevel::INVALID_ENUM),
  audio_streaming_state_(mobile_apis::AudioStreamingState::INVALID_ENUM),
  system_context_(mobile_apis::SystemContext::INVALID_ENUM) {
}

void HmiState::set_parent(HmiStatePtr parent) {
  DCHECK_OR_RETURN_VOID(parent);
  parent_ = parent;
}

mobile_apis::AudioStreamingState::eType
VRHmiState::audio_streaming_state() const {
  using namespace mobile_apis;
  return AudioStreamingState::NOT_AUDIBLE;
}

VRHmiState::VRHmiState(uint32_t app_id, StateContext& state_context):
  HmiState(app_id, state_context, STATE_ID_VR_SESSION) {
}

TTSHmiState::TTSHmiState(uint32_t app_id, StateContext& state_context):
  HmiState(app_id, state_context, STATE_ID_TTS_SESSION) {
}

mobile_apis::AudioStreamingState::eType
TTSHmiState::audio_streaming_state() const {
  using namespace helpers;
  using namespace mobile_apis;

  AudioStreamingState::eType expected_state = AudioStreamingState::NOT_AUDIBLE;
  if (state_context_.is_attenuated_supported() &&
    AudioStreamingState::NOT_AUDIBLE != parent()->audio_streaming_state() &&
    Compare<HMILevel::eType, EQ, ONE> (hmi_level(), HMILevel::HMI_FULL,
                                       HMILevel::HMI_LIMITED)) {
    expected_state = AudioStreamingState::ATTENUATED;
  }
  return expected_state;
}

NaviStreamingHmiState::NaviStreamingHmiState(uint32_t app_id, StateContext& state_context):
  HmiState(app_id, state_context, STATE_ID_NAVI_STREAMING) {
}

mobile_apis::AudioStreamingState::eType
NaviStreamingHmiState::audio_streaming_state() const {
  using namespace helpers;
  using namespace mobile_apis;

  AudioStreamingState::eType expected_state = parent()->audio_streaming_state();
  if (!state_context_.is_navi_app(app_id_) &&
      AudioStreamingState::AUDIBLE == expected_state) {
    if (state_context_.is_attenuated_supported()) {
      expected_state = AudioStreamingState::ATTENUATED;
    } else {
      expected_state = AudioStreamingState::NOT_AUDIBLE;
    }
  }
  return expected_state;
}

PhoneCallHmiState::PhoneCallHmiState(uint32_t app_id, StateContext& state_context):
  HmiState(app_id, state_context, STATE_ID_PHONE_CALL) {
}

mobile_apis::HMILevel::eType PhoneCallHmiState::hmi_level() const {
  using namespace mobile_apis;
   HMILevel::eType expected_level(HMILevel::HMI_BACKGROUND);
   if (parent()->hmi_level() == HMILevel::HMI_FULL
       && state_context_.is_navi_app(app_id_)) {
     expected_level = HMILevel::HMI_LIMITED;
   } else if (parent()->hmi_level() == HMILevel::HMI_NONE) {
     expected_level = HMILevel::HMI_NONE;
   }
   return expected_level;
}

SafetyModeHmiState::SafetyModeHmiState(uint32_t app_id, StateContext& state_context):
  HmiState(app_id, state_context, STATE_ID_SAFETY_MODE) {
}

}
