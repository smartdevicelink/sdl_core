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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTROLLER_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTROLLER_IMPL_H_

#include <list>
#include <map>
#include "application_manager/application.h"
#include "application_manager/application_manager.h"
#include "application_manager/hmi_state.h"
#include "application_manager/message_helper.h"
#include "application_manager/state_controller.h"
#include "event_engine/event_observer.h"
#include "interfaces/MOBILE_API.h"
#include "utils/helpers.h"
#include "utils/lock.h"

namespace application_manager {

class StateControllerImpl : public event_engine::EventObserver,
                            public StateController {
 public:
  explicit StateControllerImpl(ApplicationManager& app_mngr);

  void SetRegularState(ApplicationSharedPtr app,
                       const WindowID window_id,
                       HmiStatePtr state,
                       const bool SendActivateApp) OVERRIDE;

  void SetRegularState(
      ApplicationSharedPtr app,
      const WindowID window_id,
      const mobile_apis::HMILevel::eType hmi_level,
      const mobile_apis::AudioStreamingState::eType audio_state,
      const mobile_apis::VideoStreamingState::eType video_state,
      const bool SendActivateApp) OVERRIDE;

  void SetRegularState(ApplicationSharedPtr app,
                       const WindowID window_id,
                       const mobile_apis::HMILevel::eType hmi_level,
                       const bool SendActivateApp) OVERRIDE;

  void SetRegularState(
      ApplicationSharedPtr app,
      const WindowID window_id,
      const mobile_apis::HMILevel::eType hmi_level,
      const mobile_apis::AudioStreamingState::eType audio_state,
      const mobile_apis::VideoStreamingState::eType video_state,
      const mobile_apis::SystemContext::eType system_context,
      const bool SendActivateApp) OVERRIDE;

  void SetRegularState(ApplicationSharedPtr app,
                       const WindowID window_id,
                       const mobile_apis::HMILevel::eType hmi_level) OVERRIDE;

  void SetRegularState(
      ApplicationSharedPtr app,
      const WindowID window_id,
      const mobile_apis::AudioStreamingState::eType audio_state,
      const mobile_apis::VideoStreamingState::eType video_state) OVERRIDE;

  void SetRegularState(
      ApplicationSharedPtr app,
      const WindowID window_id,
      const mobile_apis::SystemContext::eType system_context) OVERRIDE;

  void SetRegularState(ApplicationSharedPtr app,
                       const WindowID window_id,
                       HmiStatePtr state) OVERRIDE;

  void OnApplicationRegistered(
      ApplicationSharedPtr app,
      const mobile_apis::HMILevel::eType default_level) OVERRIDE;

  void OnAppWindowAdded(
      ApplicationSharedPtr app,
      const WindowID window_id,
      const mobile_apis::WindowType::eType window_type,
      const mobile_apis::HMILevel::eType default_level) OVERRIDE;

  void OnVideoStreamingStarted(ApplicationConstSharedPtr app) OVERRIDE;

  void OnVideoStreamingStopped(ApplicationConstSharedPtr app) OVERRIDE;

  void OnStateChanged(ApplicationSharedPtr app,
                      const WindowID window_id,
                      HmiStatePtr old_state,
                      HmiStatePtr new_state) OVERRIDE;

  bool IsStateActive(HmiState::StateID state_id) const OVERRIDE;

  // EventObserver interface
  void on_event(const event_engine::Event& event) OVERRIDE;
  void on_event(const event_engine::MobileEvent& event) OVERRIDE;

  void ActivateDefaultWindow(ApplicationSharedPtr app) OVERRIDE;
  void ExitDefaultWindow(ApplicationSharedPtr app) OVERRIDE;

  void ResumePostponedWindows(const uint32_t app_id) OVERRIDE;

  void DropPostponedWindows(const uint32_t app_id) OVERRIDE;

 private:
  int64_t RequestHMIStateChange(ApplicationConstSharedPtr app,
                                hmi_apis::Common_HMILevel::eType level,
                                bool send_policy_priority);
  /**
   * @brief The HmiLevelConflictResolver struct
   * Move other application to HmiStates if applied moved to FULL or LIMITED
   */
  struct HmiLevelConflictResolver {
    const ApplicationSharedPtr applied_;
    const WindowID window_id_;
    const HmiStatePtr state_;
    StateControllerImpl* state_ctrl_;
    HmiLevelConflictResolver(const ApplicationSharedPtr app,
                             const WindowID window_id,
                             const HmiStatePtr state,
                             StateControllerImpl* state_ctrl)
        : applied_(app)
        , window_id_(window_id)
        , state_(state)
        , state_ctrl_(state_ctrl) {}
    void operator()(ApplicationSharedPtr to_resolve);
  };

  template <typename UnaryFunction>
  void ForEachApplication(UnaryFunction func) const {
    ApplicationSet applications;
    {
      DataAccessor<ApplicationSet> accessor = app_mngr_.applications();
      applications = accessor.GetData();
    }

    for (const auto& app : applications) {
      func(app);
    }
  }

  /**
   * @brief ResolveHmiState Checks if requested hmi state is
   * allowed by current states context and correct it if it possible
   *
   * @param app application to apply state
   *
   * @param state state to be checked
   *
   * @return Resolved hmi state or empty pointer in case requested
   * hmi state is not allowed
   */
  HmiStatePtr ResolveHmiState(ApplicationSharedPtr app,
                              HmiStatePtr state) const;

  /**
   * @brief IsResumptionAllowed checks, if app is allowed to be resumed in
   * current state
   * @param app Application
   * @param state State to be checked
   * @return true, if app is allowed to be resumed, otherwise - false
   */
  bool IsResumptionAllowed(ApplicationSharedPtr app, HmiStatePtr state) const;

  /**
   * @brief GetAvailableHmiLevel Returns closest to requested
   * available hmi level for application
   *
   * @param app application to apply state
   *
   * @param hmi_level requested hmi level
   *
   * @return Resolved hmi state or empty pointer in case requested
   * hmi state is not allowed
   */
  mobile_apis::HMILevel::eType GetAvailableHmiLevel(
      ApplicationSharedPtr app, mobile_apis::HMILevel::eType hmi_level) const;

  /**
   * @brief IsStateAvailable Checks if hmi state is available
   * to apply for specified application
   *
   * @param app application to apply state
   *
   * @param state state to be checked
   *
   * @return true if state is available, false otherwise
   */
  bool IsStateAvailable(ApplicationSharedPtr app, HmiStatePtr state) const;

  /**
   * @brief IsStateAvailableForResumption Checks if hmi state is available
   * to apply for specified application during resumption
   *
   * @param app application to apply state
   *
   * @param state state to be checked
   *
   * @return true if state is available, false otherwise
   */
  bool IsStateAvailableForResumption(ApplicationSharedPtr app,
                                     HmiStatePtr state) const;

  /**
   * @brief ApplyPostponedStateForApp tries to apply postponed state
   * to application if it's allowed by current active states
   */
  void ApplyPostponedStateForApp(ApplicationSharedPtr app);

  /**
   * @brief IsTempStateActive Checks if specified temp state
   * is currently active
   *
   * @return true if state is active, false otherwise
   */
  bool IsTempStateActive(HmiState::StateID ID) const;

  /**
   *  Function to add new temporary HmiState for application
   */
  template <HmiState::StateID ID>
  void HMIStateStarted(ApplicationSharedPtr app) {
    DCHECK_OR_RETURN_VOID(app);
    const WindowIds window_ids = app->GetWindowIds();

    for (auto window_id : window_ids) {
      HmiStatePtr old_hmi_state = app->CurrentHmiState(window_id);
      HmiStatePtr new_hmi_state = CreateHmiState(app, ID);
      DCHECK_OR_RETURN_VOID(new_hmi_state);
      DCHECK_OR_RETURN_VOID(new_hmi_state->state_id() !=
                            HmiState::STATE_ID_REGULAR);
      new_hmi_state->set_parent(old_hmi_state);
      new_hmi_state->set_window_type(old_hmi_state->window_type());
      app->AddHMIState(window_id, new_hmi_state);
      OnStateChanged(app, window_id, old_hmi_state, new_hmi_state);
    }
  }

  /**
   * @brief TempStateStarted add HMI State ID in StateControllerImpl collection
   * @param ID state identifier
   */
  void TempStateStarted(HmiState::StateID ID);

  /**
   * @brief TempStateStopped remove HMI State ID from StateControllerImpl
   * collection
   * @param ID state identifier
   */
  void TempStateStopped(HmiState::StateID ID);

  /**
   * @brief Sets BACKGROUND or LIMITED hmi level to application
   * depends on application type
   * @param app Application to deactivate
   */
  void DeactivateApp(ApplicationSharedPtr app, const WindowID window_id);

  /**
   * Function to remove temporary HmiState for application
   */
  template <HmiState::StateID ID>
  void HMIStateStopped(ApplicationSharedPtr app) {
    DCHECK_OR_RETURN_VOID(app);
    const WindowIds window_ids = app->GetWindowIds();

    for (auto window_id : window_ids) {
      HmiStatePtr cur = app->CurrentHmiState(window_id);
      HmiStatePtr old_hmi_state =
          CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);
      DCHECK_OR_RETURN_VOID(old_hmi_state);
      old_hmi_state->set_hmi_level(cur->hmi_level());
      old_hmi_state->set_window_type(cur->window_type());
      old_hmi_state->set_audio_streaming_state(cur->audio_streaming_state());
      old_hmi_state->set_video_streaming_state(cur->video_streaming_state());
      old_hmi_state->set_system_context(cur->system_context());
      app->RemoveHMIState(window_id, ID);
      HmiStatePtr new_hmi_state = app->CurrentHmiState(window_id);
      OnStateChanged(app, window_id, old_hmi_state, new_hmi_state);
    }
  }

  /**
   * @brief ApplyRegularState setup regular hmi state, that will appear if no
   * specific events are active, without sending ActivateApp
   * @param app appication to setup default State
   * @param window_id id of applicaion's window to apply HMI state
   * @param state state of new defailt state
   */
  void ApplyRegularState(ApplicationSharedPtr app,
                         const WindowID window_id,
                         HmiStatePtr state);

  /**
   * @brief UpdateAppWindowsStreamingState updates all application windows
   * audio/video streaming state according to a new HMI state of the main window
   * @param app pointer to affected application
   * @param state pointer to state with the new streaming state of the main
   * window
   */
  void UpdateAppWindowsStreamingState(ApplicationSharedPtr app,
                                      HmiStatePtr state);

  /**
   * @brief SetupRegularHmiState set regular HMI State without
   * resolving conflicts and ActivateApp request
   * @param app application
   * @param window_id id of applicaion's window to apply HMI state
   * @param state hmi_state to setup
   */
  void SetupRegularHmiState(ApplicationSharedPtr app,
                            const WindowID window_id,
                            HmiStatePtr state);

  /**
   * @brief SetupRegularHmiState set regular HMI State without
   * resolving conflicts and ActivateApp request
   * @param app application
   * @param window_id id of applicaion's window to apply HMI state
   * @param hmi_level of new regular state
   * @param audio_state of new regular state
   * @param video_state of new regular state
   */
  void SetupRegularHmiState(
      ApplicationSharedPtr app,
      const WindowID window_id,
      const mobile_apis::HMILevel::eType hmi_level,
      const mobile_apis::AudioStreamingState::eType audio_state,
      const mobile_apis::VideoStreamingState::eType video_state);

  /**
   * @brief OnHMIResponse callback for activate app or close application
   * response
   * @param message Smart Object
   */
  void OnHMIResponse(const smart_objects::SmartObject& message);

  /**
   * @brief OnAppDeactivated callback for OnAppDeactivated notification
   * @param message Smart Object
   */
  void OnAppDeactivated(const smart_objects::SmartObject& message);

  /**
   * @brief OnAppActivated callback for OnAppActivated notification
   * @param message Smart Object
   */
  void OnAppActivated(const smart_objects::SmartObject& message);

  /**
   * @brief Apply temporary state
   */
  template <HmiState::StateID ID>
  void ApplyTempState() {
    ForEachApplication(std::bind1st(
        std::mem_fun(&StateControllerImpl::HMIStateStarted<ID>), this));
    TempStateStarted(ID);
  }

  /**
   * @brief Cancel temporary state
   */
  template <HmiState::StateID ID>
  void CancelTempState() {
    ForEachApplication(std::bind1st(
        std::mem_fun(&StateControllerImpl::HMIStateStopped<ID>), this));
    TempStateStopped(ID);
  }

  /**
   * @brief CreateHmiState creates Hmi state according to state_id
   * @param app_id application ID
   * @param state_id state id
   * @return
   */
  HmiStatePtr CreateHmiState(std::shared_ptr<Application> app,
                             HmiState::StateID state_id) const;

  /**
   * @brief Determines AudioStreamingState value
   * for application with HMI level specified.
   * @param app an application to calculate for
   * @param hmi_level HMI level
   * @return AudioStreamingState value
   */
  mobile_apis::AudioStreamingState::eType CalcAudioState(
      ApplicationSharedPtr app,
      const mobile_apis::HMILevel::eType hmi_level) const;

  /**
   * @brief Determines VideoStreamingState value
   * for application with HMI level specified.
   * @param app an application to calculate for
   * @param hmi_level HMI level
   * @return VideoStreamingState value
   */
  mobile_apis::VideoStreamingState::eType CalcVideoState(
      ApplicationSharedPtr app,
      const mobile_apis::HMILevel::eType hmi_level) const;

  typedef std::list<HmiState::StateID> StateIDList;
  StateIDList active_states_;
  mutable sync_primitives::Lock active_states_lock_;
  std::map<uint32_t, HmiStatePtr> waiting_for_response_;

  typedef std::pair<WindowID, HmiStatePtr> WindowStatePair;
  typedef std::list<WindowStatePair> WindowStatePairs;
  std::map<uint32_t, WindowStatePairs> postponed_app_widgets_;

  ApplicationManager& app_mngr_;
};
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTROLLER_IMPL_H_
