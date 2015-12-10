/*
 Copyright (c) 2015, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/state_controller.h"
#include "application_manager/usage_statistics.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "StateController");

bool IsStatusChanged(HmiStatePtr old_state, HmiStatePtr new_state) {
  if (old_state->hmi_level() != new_state->hmi_level() ||
      old_state->audio_streaming_state() !=
          new_state->audio_streaming_state() ||
      old_state->system_context() != new_state->system_context()) {
    return true;
  }
  return false;
}

StateController::StateController(ApplicationManager* app_mngr)
    : EventObserver(), app_mngr_(app_mngr) {
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnAppActivated);
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnAppDeactivated);
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnEmergencyEvent);
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnPhoneCall);
  subscribe_on_event(hmi_apis::FunctionID::TTS_Started);
  subscribe_on_event(hmi_apis::FunctionID::TTS_Stopped);
  subscribe_on_event(hmi_apis::FunctionID::VR_Started);
  subscribe_on_event(hmi_apis::FunctionID::VR_Stopped);
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnEventChanged);
}

void StateController::HmiLevelConflictResolver::operator()(
    ApplicationSharedPtr to_resolve) {
  using namespace mobile_apis;
  using namespace helpers;
  DCHECK_OR_RETURN_VOID(state_ctrl_);
  if (to_resolve == applied_)
    return;
  HmiStatePtr cur_state = to_resolve->RegularHmiState();

  const bool applied_grabs_audio =
      Compare<HMILevel::eType, EQ, ONE>(
          state_->hmi_level(), HMILevel::HMI_FULL, HMILevel::HMI_LIMITED) &&
      applied_->IsAudioApplication();
  const bool applied_grabs_full = state_->hmi_level() == HMILevel::HMI_FULL;
  const bool to_resolve_handles_full =
      cur_state->hmi_level() == HMILevel::HMI_FULL;
  const bool to_resolve_handles_audio =
      Compare<HMILevel::eType, EQ, ONE>(
          cur_state->hmi_level(), HMILevel::HMI_FULL, HMILevel::HMI_LIMITED) &&
      to_resolve->IsAudioApplication();
  const bool same_app_type = state_ctrl_->IsSameAppType(applied_, to_resolve);

  // If applied Hmi state is FULL:
  //  all not audio applications will get BACKGROUND
  //  all applications with same HMI type will get BACKGROUND
  //  all audio applications with other HMI type(navi, vc, media) in FULL will
  //  get LIMMITED HMI level

  // If applied Hmi state is LIMITED:
  //  all applications with other HMI types will save HMI states
  //  all not audio  applications will save HMI states
  //  all applications with same HMI type will get BACKGROUND

  // If applied Hmi state is BACKGROUND:
  //  all applications will save HMI states

  HMILevel::eType result_hmi_level = cur_state->hmi_level();
  if (applied_grabs_full && to_resolve_handles_audio && !same_app_type)
    result_hmi_level = HMILevel::HMI_LIMITED;

  if ((applied_grabs_full && to_resolve_handles_full &&
       !to_resolve->IsAudioApplication()) ||
      (applied_grabs_audio && to_resolve_handles_audio && same_app_type))
    result_hmi_level = HMILevel::HMI_BACKGROUND;

  if (cur_state->hmi_level() != result_hmi_level) {
    LOG4CXX_DEBUG(logger_,
                  "Application " << to_resolve->app_id()
                                 << " will change HMI level to "
                                 << result_hmi_level);
    state_ctrl_->SetupRegularHmiState(to_resolve,
                                      result_hmi_level,
                                      result_hmi_level == HMILevel::HMI_LIMITED
                                          ? AudioStreamingState::AUDIBLE
                                          : AudioStreamingState::NOT_AUDIBLE);
  } else {
    LOG4CXX_DEBUG(logger_,
                  "Application " << to_resolve->app_id()
                                 << " will not change HMI level");
  }
}

HmiStatePtr StateController::ResolveHmiState(ApplicationSharedPtr app,
                                             HmiStatePtr state) const {
  using namespace mobile_apis;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "State to resolve: hmi_level " << state->hmi_level()
                                               << ", audio_state "
                                               << state->audio_streaming_state()
                                               << ", system_context "
                                               << state->system_context());

  HmiStatePtr available_state =
      CreateHmiState(app->app_id(), HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN(available_state, HmiStatePtr());
  available_state->set_hmi_level(state->hmi_level());
  available_state->set_audio_streaming_state(state->audio_streaming_state());
  available_state->set_system_context(state->system_context());

  if (app->is_resuming()) {
    HMILevel::eType available_level =
        GetAvailableHmiLevel(app, state->hmi_level());
    available_state->set_hmi_level(available_level);
    available_state->set_audio_streaming_state(
        CalcAudioState(app, available_level));
  }
  return IsStateAvailable(app, available_state) ? available_state
                                                : HmiStatePtr();
}

bool StateController::IsResumptionAllowed(ApplicationSharedPtr app,
                                          HmiStatePtr state) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  using namespace mobile_apis;
  if (!app->is_resuming() ||
      !Compare<HMILevel::eType, EQ, ONE>(
          state->hmi_level(), HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)) {
    LOG4CXX_DEBUG(logger_, "Application is not in resuming mode.");
    return true;
  }

  if (IsTempStateActive(HmiState::StateID::STATE_ID_AUDIO_SOURCE) &&
      app->is_media_application()) {
    LOG4CXX_DEBUG(logger_,
                  "Resumption for media app is not allowed. "
                      << "AUDIO_SOURCE event is active");
    return false;
  }

  if (IsTempStateActive(HmiState::StateID::STATE_ID_EMBEDDED_NAVI) &&
      app->is_navi()) {
    LOG4CXX_DEBUG(logger_,
                  "Resumption for navi app is not allowed. "
                      << "EMBEDDED_NAVI event is active");
    return false;
  }

  return true;
}

mobile_apis::HMILevel::eType StateController::GetAvailableHmiLevel(
    ApplicationSharedPtr app, mobile_apis::HMILevel::eType hmi_level) const {
  using namespace mobile_apis;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);

  mobile_apis::HMILevel::eType result = hmi_level;
  if (!Compare<HMILevel::eType, EQ, ONE>(
          hmi_level, HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)) {
    return result;
  }

  const bool is_audio_app = app->IsAudioApplication();
  const bool does_audio_app_with_same_type_exist =
      app_mngr_->IsAppTypeExistsInFullOrLimited(app);
  if (HMILevel::HMI_LIMITED == hmi_level) {
    if (!is_audio_app || does_audio_app_with_same_type_exist) {
      result = app_mngr_->GetDefaultHmiLevel(app);
    }
    return result;
  }

  const bool is_active_app_exist = app_mngr_->active_application();
  if (is_audio_app) {
    if (does_audio_app_with_same_type_exist) {
      result = app_mngr_->GetDefaultHmiLevel(app);
    } else if (is_active_app_exist) {
      result = mobile_apis::HMILevel::HMI_LIMITED;
    } else if (app->is_navi() &&
               IsTempStateActive(HmiState::StateID::STATE_ID_AUDIO_SOURCE)) {
      LOG4CXX_DEBUG(logger_,
                    "Navigation app will be resumed to LIMITED, "
                    "because of AUDIO_SOURCE ia active.");
      result = mobile_apis::HMILevel::HMI_LIMITED;
    } else if (app->is_media_application() &&
               IsTempStateActive(HmiState::StateID::STATE_ID_EMBEDDED_NAVI)) {
      LOG4CXX_DEBUG(logger_,
                    "Media app will be resumed to LIMITED, "
                    "because of EMBEDDED_NAVI is active.");
      result = mobile_apis::HMILevel::HMI_LIMITED;
    }
  } else if (is_active_app_exist) {
    result = app_mngr_->GetDefaultHmiLevel(app);
  }

  return result;
}

bool StateController::IsStateAvailable(ApplicationSharedPtr app,
                                       HmiStatePtr state) const {
  using namespace mobile_apis;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "Checking state: hmi_level " << state->hmi_level()
                                             << ", audio_state "
                                             << state->audio_streaming_state()
                                             << ", system_context "
                                             << state->system_context());

  if (app->is_resuming()) {
    return IsStateAvailableForResumption(app, state);
  }

  if (IsTempStateActive(HmiState::StateID::STATE_ID_AUDIO_SOURCE) ||
      IsTempStateActive(HmiState::StateID::STATE_ID_EMBEDDED_NAVI)) {
    if (HMILevel::HMI_FULL == state->hmi_level()) {
      LOG4CXX_DEBUG(logger_,
                    "AUDIO_SOURCE or EMBEDDED_NAVI is active."
                        << " Requested state is not available");
      return false;
    }
  }

  LOG4CXX_DEBUG(logger_, "Requested state is available");
  return true;
}

bool StateController::IsStateAvailableForResumption(ApplicationSharedPtr app,
                                                    HmiStatePtr state) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;
  using namespace helpers;

  if (app->is_resuming()) {
    return IsStateAvailableForResumption(app, state);
  }

  if (IsTempStateActive(HmiState::StateID::STATE_ID_AUDIO_SOURCE) ||
      IsTempStateActive(HmiState::StateID::STATE_ID_EMBEDDED_NAVI)) {

    if (HMILevel::HMI_FULL == state->hmi_level()) {
      LOG4CXX_DEBUG(logger_, "AUDIO_SOURCE or EMBEDDED_NAVI is active."
                    << " Requested state is not available");
      return false;
    }
  }

  LOG4CXX_DEBUG(logger_, "Requested state is available");
  return true;
}

bool StateController::IsStateAvailableForResumption(ApplicationSharedPtr app,
                                                    HmiStatePtr state) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;
  using namespace helpers;

  if (!app->is_resuming() ||
      !Compare<HMILevel::eType, EQ, ONE>(
          state->hmi_level(), HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)) {
    LOG4CXX_DEBUG(logger_,
                  "Application is not in resuming mode."
                      << " Requested state is available");
    return true;
  }

  if (IsTempStateActive(HmiState::StateID::STATE_ID_VR_SESSION) ||
      IsTempStateActive(HmiState::StateID::STATE_ID_SAFETY_MODE)) {
    LOG4CXX_DEBUG(logger_,
                  "Requested state is not available. "
                      << "VR session or emergency event is active");
    return false;
  }
  if (IsTempStateActive(HmiState::StateID::STATE_ID_PHONE_CALL) &&
      app->is_media_application()) {
    LOG4CXX_DEBUG(logger_,
                  "Requested state for media application "
                      << "is not available. Phone call is active");
    return false;
  }
  if (IsTempStateActive(HmiState::StateID::STATE_ID_DEACTIVATE_HMI)) {
    LOG4CXX_DEBUG(logger_,
                  "Requested state is not available. "
                      << "Deactivate HMI event is active");
    return false;
  }

  LOG4CXX_DEBUG(logger_, "Requested state is available");
  return true;
}

void StateController::SetupRegularHmiState(ApplicationSharedPtr app,
                                           HmiStatePtr state) {
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(state);
  LOG4CXX_DEBUG(logger_,
                "hmi_level " << state->hmi_level() << ", audio_state "
                             << state->audio_streaming_state()
                             << ", system_context "
                             << state->system_context());
  HmiStatePtr curr_state = app->CurrentHmiState();
  HmiStatePtr old_state =
      CreateHmiState(app->app_id(), HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(old_state);
  old_state->set_hmi_level(curr_state->hmi_level());
  old_state->set_audio_streaming_state(curr_state->audio_streaming_state());
  old_state->set_system_context(curr_state->system_context());
  app->SetRegularState(state);

  if (HMILevel::HMI_LIMITED == state->hmi_level() && app->is_resuming()) {
    LOG4CXX_DEBUG(logger_,
                  "Resuming to LIMITED level. "
                      << "Send OnResumeAudioSource notification");
    MessageHelper::SendOnResumeAudioSourceToHMI(app->app_id());
  }
  app->set_is_resuming(false);

  HmiStatePtr new_state = app->CurrentHmiState();
  OnStateChanged(app, old_state, new_state);
}

void StateController::SetupRegularHmiState(
    ApplicationSharedPtr app,
    const mobile_apis::HMILevel::eType hmi_level,
    const mobile_apis::AudioStreamingState::eType audio_state) {
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  using helpers::Compare;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  HmiStatePtr prev_state = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(prev_state);
  HmiStatePtr new_state =
      CreateHmiState(app->app_id(), HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(new_state);
  new_state->set_hmi_level(hmi_level);
  new_state->set_audio_streaming_state(audio_state);
  new_state->set_system_context(prev_state->system_context());
  SetupRegularHmiState(app, new_state);
}

void StateController::ApplyRegularState(ApplicationSharedPtr app,
                                        HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  DCHECK_OR_RETURN_VOID(state);
  DCHECK_OR_RETURN_VOID(state->state_id() == HmiState::STATE_ID_REGULAR);
  SetupRegularHmiState(app, state);
  ForEachApplication<HmiLevelConflictResolver>(
      HmiLevelConflictResolver(app, state, this));
}

bool StateController::IsSameAppType(ApplicationConstSharedPtr app1,
                                    ApplicationConstSharedPtr app2) {
  const bool both_media =
      app1->is_media_application() && app2->is_media_application();
  const bool both_navi = app1->is_navi() && app2->is_navi();
  const bool both_vc = app1->is_voice_communication_supported() &&
                       app2->is_voice_communication_supported();
  const bool both_simple =
      !app1->IsAudioApplication() && !app2->IsAudioApplication();
  return both_simple || both_media || both_navi || both_vc;
}

void StateController::on_event(const event_engine::Event& event) {
  using smart_objects::SmartObject;
  using event_engine::Event;
  using namespace hmi_apis;
  namespace FunctionID = hmi_apis::FunctionID;

  LOG4CXX_AUTO_TRACE(logger_);
  const SmartObject& message = event.smart_object();
  const FunctionID::eType id = static_cast<FunctionID::eType>(event.id());
  switch (id) {
    case FunctionID::BasicCommunication_ActivateApp: {
      OnActivateAppResponse(message);
      break;
    }
    case FunctionID::BasicCommunication_OnAppActivated: {
      OnAppActivated(message);
      break;
    }
    case FunctionID::BasicCommunication_OnAppDeactivated: {
      OnAppDeactivated(message);
      break;
    }
    case FunctionID::BasicCommunication_OnEmergencyEvent: {
      bool is_active =
          message[strings::msg_params][hmi_response::enabled].asBool();
      if (is_active) {
        ApplyTempState<HmiState::STATE_ID_SAFETY_MODE>();

      } else {
        CancelTempState<HmiState::STATE_ID_SAFETY_MODE>();
      }
      break;
    }
    case FunctionID::BasicCommunication_OnPhoneCall: {
      bool is_active =
          message[strings::msg_params][hmi_notification::is_active].asBool();
      if (is_active) {
        ApplyTempState<HmiState::STATE_ID_PHONE_CALL>();

      } else {
        CancelTempState<HmiState::STATE_ID_PHONE_CALL>();
      }
      break;
    }

    case FunctionID::VR_Started: {
      ApplyTempState<HmiState::STATE_ID_VR_SESSION>();
      break;
    }
    case FunctionID::VR_Stopped: {
      CancelTempState<HmiState::STATE_ID_VR_SESSION>();
      break;
    }
    case FunctionID::TTS_Started: {
      ApplyTempState<HmiState::STATE_ID_TTS_SESSION>();
      break;
    }
    case FunctionID::TTS_Stopped: {
      CancelTempState<HmiState::STATE_ID_TTS_SESSION>();
      break;
    }
    case FunctionID::BasicCommunication_OnEventChanged: {
      bool is_active =
          message[strings::msg_params][hmi_response::is_active].asBool();
      const uint32_t id =
          message[strings::msg_params][hmi_response::event_name].asUInt();
      //TODO(AOleynik): Add verification/conversion check here
      Common_EventTypes::eType state_id =
          static_cast<Common_EventTypes::eType>(id);
      if (is_active) {
        if (Common_EventTypes::AUDIO_SOURCE == state_id) {
          ApplyTempState<HmiState::STATE_ID_AUDIO_SOURCE>();
          break;
        }
        if (Common_EventTypes::EMBEDDED_NAVI == state_id) {
          ApplyTempState<HmiState::STATE_ID_EMBEDDED_NAVI>();
          break;
        }
        if (Common_EventTypes::PHONE_CALL == state_id) {
          ApplyTempState<HmiState::STATE_ID_PHONE_CALL>();
          break;
        }
        if (Common_EventTypes::EMERGENCY_EVENT == state_id) {
          ApplyTempState<HmiState::STATE_ID_SAFETY_MODE>();
          break;
        }
        if (Common_EventTypes::DEACTIVATE_HMI == state_id) {
          ApplyTempState<HmiState::STATE_ID_DEACTIVATE_HMI>();
          break;
        }
      } else {
        if (Common_EventTypes::AUDIO_SOURCE == state_id) {
          CancelTempState<HmiState::STATE_ID_AUDIO_SOURCE>();
          break;
        }
        if (Common_EventTypes::EMBEDDED_NAVI == state_id) {
          CancelTempState<HmiState::STATE_ID_EMBEDDED_NAVI>();
          break;
        }
        if (Common_EventTypes::PHONE_CALL == state_id) {
          CancelTempState<HmiState::STATE_ID_PHONE_CALL>();
          break;
        }
        if (Common_EventTypes::EMERGENCY_EVENT == state_id) {
          CancelTempState<HmiState::STATE_ID_SAFETY_MODE>();
          break;
        }
        if (Common_EventTypes::DEACTIVATE_HMI == state_id) {
          CancelTempState<HmiState::STATE_ID_DEACTIVATE_HMI>();
          break;
        }
      }
      break;
    }
    default:
      break;
  }
}

void StateController::OnStateChanged(ApplicationSharedPtr app,
                                     HmiStatePtr old_state,
                                     HmiStatePtr new_state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);
  DCHECK_OR_RETURN_VOID(old_state);
  DCHECK_OR_RETURN_VOID(new_state);
  LOG4CXX_DEBUG(logger_,
                "old: hmi_level " << old_state->hmi_level() << ", audio_state "
                                  << old_state->audio_streaming_state()
                                  << ", system_context "
                                  << old_state->system_context());
  LOG4CXX_DEBUG(logger_,
                "new: hmi_level " << new_state->hmi_level() << ", audio_state "
                                  << new_state->audio_streaming_state()
                                  << ", system_context "
                                  << new_state->system_context());
  if (IsStatusChanged(old_state, new_state)) {
    app_mngr_->SendHMIStatusNotification(app);
    if (new_state->hmi_level() == mobile_apis::HMILevel::HMI_NONE) {
      app->ResetDataInNone();
    }
    app_mngr_->OnHMILevelChanged(
        app->app_id(), old_state->hmi_level(), new_state->hmi_level());
    app->usage_report().RecordHmiStateChanged(new_state->hmi_level());
  } else {
    LOG4CXX_ERROR(logger_, "Status not changed");
  }
}

bool StateController::IsTempStateActive(HmiState::StateID ID) const {
  sync_primitives::AutoLock autolock(active_states_lock_);
  StateIDList::const_iterator itr =
      std::find(active_states_.begin(), active_states_.end(), ID);
  return active_states_.end() != itr;
}

void StateController::OnApplicationRegistered(
    ApplicationSharedPtr app,
    const mobile_apis::HMILevel::eType default_level) {
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);

  active_states_lock_.Acquire();
  StateIDList::iterator it = active_states_.begin();
  for (; it != active_states_.end(); ++it) {
    HmiStatePtr new_state = CreateHmiState(app->app_id(), *it);
    DCHECK_OR_RETURN_VOID(new_state);
    DCHECK_OR_RETURN_VOID(new_state->state_id() != HmiState::STATE_ID_REGULAR);
    HmiStatePtr old_hmi_state = app->CurrentHmiState();
    new_state->set_parent(old_hmi_state);
    app->AddHMIState(new_state);
  }
  active_states_lock_.Release();

  HmiStatePtr default_state =
      CreateHmiState(app->app_id(), HmiState::StateID::STATE_ID_REGULAR);
  DCHECK_OR_RETURN_VOID(default_state);
  default_state->set_hmi_level(default_level);
  default_state->set_audio_streaming_state(CalcAudioState(app, default_level));
  default_state->set_system_context(SystemContext::SYSCTXT_MAIN);

  HmiStatePtr initial_state = app->RegularHmiState();

  app->SetRegularState(default_state);

  HmiStatePtr new_state = app->CurrentHmiState();

  OnStateChanged(app, initial_state, new_state);
}

void StateController::ApplyPostponedStateForApp(ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  HmiStatePtr state = app->PostponedHmiState();
  if (state) {
    app->RemovePostponedState();
    state->set_state_id(HmiState::STATE_ID_REGULAR);
    SetRegularState(app, state);
  }
}

void StateController::TempStateStarted(HmiState::StateID ID) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(active_states_lock_);
  StateIDList::iterator it =
      std::find(active_states_.begin(), active_states_.end(), ID);
  if (it == active_states_.end()) {
    active_states_.push_back(ID);
  } else {
    LOG4CXX_ERROR(logger_, "StateID " << ID << " is already active");
  }
}

void StateController::TempStateStopped(HmiState::StateID ID) {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock autolock(active_states_lock_);
    active_states_.remove(ID);
  }
  ForEachApplication(std::bind1st(
      std::mem_fun(&StateController::ApplyPostponedStateForApp), this));
}

void StateController::DeactivateApp(ApplicationSharedPtr app) {
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);

  DCHECK_OR_RETURN_VOID(app);
  HmiStatePtr regular = app->RegularHmiState();
  DCHECK_OR_RETURN_VOID(regular);
  HmiStatePtr new_regular = utils::MakeShared<HmiState>(*regular);

  if (app->IsAudioApplication()) {
    new_regular->set_hmi_level(HMILevel::HMI_LIMITED);
    new_regular->set_audio_streaming_state(AudioStreamingState::AUDIBLE);
  } else {
    new_regular->set_hmi_level(HMILevel::HMI_BACKGROUND);
    new_regular->set_audio_streaming_state(AudioStreamingState::NOT_AUDIBLE);
  }

  SetRegularState<false>(app, new_regular);
}

void StateController::OnActivateAppResponse(
    const smart_objects::SmartObject& message) {
  const hmi_apis::Common_Result::eType code =
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
  const int32_t correlation_id =
      message[strings::params][strings::correlation_id].asInt();
  const uint32_t hmi_app_id = app_mngr_->application_id(correlation_id);
  ApplicationSharedPtr application =
      app_mngr_->application_by_hmi_app(hmi_app_id);
  if (application && hmi_apis::Common_Result::SUCCESS == code) {
    HmiStatePtr pending_state = waiting_for_activate[application->app_id()];
    DCHECK_OR_RETURN_VOID(pending_state);
    ApplyRegularState(application, pending_state);
  }
}

void StateController::OnAppActivated(
    const smart_objects::SmartObject& message) {
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t app_id = message[strings::msg_params][strings::app_id].asUInt();
  ApplicationSharedPtr app = app_mngr_->application(app_id);
  if (!app) {
    LOG4CXX_ERROR(logger_, "Application with id " << app_id << " not found");
    return;
  }

  SetRegularState<true>(app, HMILevel::HMI_FULL);
}

void StateController::OnAppDeactivated(
    const smart_objects::SmartObject& message) {
  using namespace hmi_apis;
  using namespace mobile_apis;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t app_id = message[strings::msg_params][strings::app_id].asUInt();
  ApplicationSharedPtr app = app_mngr_->application(app_id);

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application with id " << app_id << " not found");
    return;
  }

  if (HMILevel::HMI_FULL != app->hmi_level()) {
    return;
  }

  //TODO(AOleynik): Need to delete DeactivateReason and modify OnAppDeactivated
  // when HMI will support that, otherwise won't be testable
  DeactivateApp(app);
}

void StateController::SetAplicationManager(ApplicationManager* app_mngr) {
  app_mngr_ = app_mngr;
}

void StateController::OnNaviStreamingStarted() {
  ApplyTempState<HmiState::STATE_ID_NAVI_STREAMING>();
}

void StateController::OnNaviStreamingStopped() {
  CancelTempState<HmiState::STATE_ID_NAVI_STREAMING>();
}

bool StateController::IsDeactivateHMIStateActive() const {
  return IsTempStateActive(HmiState::StateID::STATE_ID_DEACTIVATE_HMI);
}

HmiStatePtr StateController::CreateHmiState(uint32_t app_id,
                                            HmiState::StateID state_id) const {
  using namespace utils;
  LOG4CXX_AUTO_TRACE(logger_);
  HmiStatePtr new_state;
  switch (state_id) {
    case HmiState::STATE_ID_PHONE_CALL: {
      new_state = MakeShared<PhoneCallHmiState>(app_id, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_SAFETY_MODE: {
      new_state = MakeShared<SafetyModeHmiState>(app_id, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_VR_SESSION: {
      new_state = MakeShared<VRHmiState>(app_id, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_TTS_SESSION: {
      new_state = MakeShared<TTSHmiState>(app_id, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_NAVI_STREAMING: {
      new_state = MakeShared<NaviStreamingHmiState>(app_id, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_REGULAR: {
      new_state = MakeShared<HmiState>(app_id, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_POSTPONED: {
      new_state = MakeShared<HmiState>(app_id, app_mngr_, state_id);
      break;
    }
    case HmiState::STATE_ID_DEACTIVATE_HMI: {
      new_state = MakeShared<DeactivateHMI>(app_id, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_AUDIO_SOURCE: {
      new_state = MakeShared<AudioSource>(app_id, app_mngr_);
      break;
    }
    case HmiState::STATE_ID_EMBEDDED_NAVI: {
      new_state = MakeShared<EmbeddedNavi>(app_id, app_mngr_);
      break;
    }
    default:
      LOG4CXX_FATAL(logger_, "Invalid state_id " << state_id);
      NOTREACHED();
      break;
  }
  return new_state;
}

mobile_apis::AudioStreamingState::eType StateController::CalcAudioState(
    ApplicationSharedPtr app,
    const mobile_apis::HMILevel::eType hmi_level) const {
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  using helpers::Compare;
  using helpers::EQ;
  using helpers::ONE;

  AudioStreamingState::eType audio_state = AudioStreamingState::NOT_AUDIBLE;
  if (Compare<HMILevel::eType, EQ, ONE>(
          hmi_level, HMILevel::HMI_FULL, HMILevel::HMI_LIMITED)) {
    if (app->IsAudioApplication()) {
      audio_state = AudioStreamingState::AUDIBLE;
    }
  }
  return audio_state;
}

}  // namespace application_manager
