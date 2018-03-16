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

#include "application_manager/hmi_state.h"
#include "application_manager/application.h"
#include "application_manager/application_manager.h"
#include "event_engine/event_observer.h"
#include "application_manager/state_controller.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "utils/lock.h"
#include "utils/helpers.h"

namespace application_manager {

class StateControllerImpl : public event_engine::EventObserver,
                            public StateController {
 public:
  explicit StateControllerImpl(ApplicationManager& app_mngr);

  /**
   * @brief SetRegularState setup regular hmi state, that  will appear if
   * no specific events are active
   * @param app appication to setup regular State
   * @param state state of new regular state
   */

  virtual void SetRegularState(ApplicationSharedPtr app,
                               HmiStatePtr state,
                               const bool SendActivateApp);

  /**
   * @brief SetRegularState Change regular hmi level and audio state
   * @param app appication to setup regular State
   * @param hmi_level of new regular state
   * @param audio_state of new regular state
   * @paran video_state of new regular state
   * @param SendActivateApp: if true, ActivateAppRequest will be sent on HMI */

  virtual void SetRegularState(
      ApplicationSharedPtr app,
      const mobile_apis::HMILevel::eType hmi_level,
      const mobile_apis::AudioStreamingState::eType audio_state,
      const mobile_apis::VideoStreamingState::eType video_state,
      const bool SendActivateApp);

  /**
   * @brief SetRegularState Change regular hmi level
   * @param app appication to setup regular State
   * @param hmi_level of new regular state
   * @param SendActivateApp: if true, ActivateAppRequest will be sent on HMI
   */
  virtual void SetRegularState(ApplicationSharedPtr app,
                               const mobile_apis::HMILevel::eType hmi_level,
                               const bool SendActivateApp);

  /**
   * @brief SetRegularState Change regular hmi level, audio state and system
   * context
   * @param app appication to setup regular State
   * @param hmi_level of new regular state
   * @param audio_state of new regular state
   * @param video_state of new regular state
   * @param system_context of new regular state
   * @param SendActivateApp: if true, ActivateAppRequest will be sent on HMI */

  virtual void SetRegularState(
      ApplicationSharedPtr app,
      const mobile_apis::HMILevel::eType hmi_level,
      const mobile_apis::AudioStreamingState::eType audio_state,
      const mobile_apis::VideoStreamingState::eType video_state,
      const mobile_apis::SystemContext::eType system_context,
      const bool SendActivateApp);

  /**
   * @brief SetRegularState Sets regular state with new hmi level
   * to application
   * @param app appication to setup regular state
   * @param hmi_level new hmi level for application
   */
  virtual void SetRegularState(ApplicationSharedPtr app,
                               const mobile_apis::HMILevel::eType hmi_level);

  /**
   * @brief SetRegularState Change regular audio state
   * @param app appication to setup regular State
   * @param audio_state of new regular state
   * @param video_state of new regular state
   */
  virtual void SetRegularState(
      ApplicationSharedPtr app,
      const mobile_apis::AudioStreamingState::eType audio_state,
      const mobile_apis::VideoStreamingState::eType video_state);

  /**
   * @brief SetRegularState Change regular  system context
   * @param app appication to setup regular State
   * @param system_context of new regular state
   */
  virtual void SetRegularState(
      ApplicationSharedPtr app,
      const mobile_apis::SystemContext::eType system_context);

  /**
   * @brief SetRegularState Sets new regular state to application
   * @param app appication to setup regular state
   * @param state new hmi state for application
   */
  virtual void SetRegularState(ApplicationSharedPtr app, HmiStatePtr state);

  // EventObserver interface
  void on_event(const event_engine::Event& event);

  /**
   * @brief Sets default application state and apply currently active HMI states
   * on application registration
   * @param app application to apply states
   * @param default_level default HMI level
   */
  virtual void OnApplicationRegistered(
      ApplicationSharedPtr app,
      const mobile_apis::HMILevel::eType default_level);

  /**
   * @brief OnNaviStreamingStarted process Navi streaming started
   */
  virtual void OnNaviStreamingStarted();

  /**
   * @brief OnNaviStreamingStopped process Navi streaming stopped
   */
  virtual void OnNaviStreamingStopped();

  /**
   * @brief OnStateChanged send HMIStatusNotification if neded
   * @param app application
   * @param old_state state before change
   * @param new_state state after change
   */
  virtual void OnStateChanged(ApplicationSharedPtr app,
                              HmiStatePtr old_state,
                              HmiStatePtr new_state);

  /**
   * @brief Checks activity of Deactivate HMI state.
   * @return Returns TRUE if deactivate HMI state is active, otherwise returns
   * FALSE.
   */
  virtual bool IsStateActive(HmiState::StateID state_id) const;

 private:
  int64_t SendBCActivateApp(ApplicationConstSharedPtr app,
                            hmi_apis::Common_HMILevel::eType level,
                            bool send_policy_priority);
  /**
   * @brief The HmiLevelConflictResolver struct
   * Move other application to HmiStates if applied moved to FULL or LIMITED
   */
  struct HmiLevelConflictResolver {
    ApplicationSharedPtr applied_;
    HmiStatePtr state_;
    StateControllerImpl* state_ctrl_;
    HmiLevelConflictResolver(ApplicationSharedPtr app,
                             HmiStatePtr state,
                             StateControllerImpl* state_ctrl)
        : applied_(app), state_(state), state_ctrl_(state_ctrl) {}
    void operator()(ApplicationSharedPtr to_resolve);
  };

  template <typename UnaryFunction>
  void ForEachApplication(UnaryFunction func) const {
    DataAccessor<ApplicationSet> accessor = app_mngr_.applications();
    ApplicationSet::iterator it = accessor.GetData().begin();
    for (; it != accessor.GetData().end(); ++it) {
      ApplicationConstSharedPtr const_app = *it;
      if (const_app) {
        func(app_mngr_.application(const_app->app_id()));
      }
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
    HmiStatePtr old_hmi_state = app->CurrentHmiState();
    HmiStatePtr new_hmi_state = CreateHmiState(app, ID);
    DCHECK_OR_RETURN_VOID(new_hmi_state);
    DCHECK_OR_RETURN_VOID(new_hmi_state->state_id() !=
                          HmiState::STATE_ID_REGULAR);
    new_hmi_state->set_parent(old_hmi_state);
    app->AddHMIState(new_hmi_state);
    OnStateChanged(app, old_hmi_state, new_hmi_state);
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
  void DeactivateApp(ApplicationSharedPtr app);

  /**
   * Function to remove temporary HmiState for application
   */
  template <HmiState::StateID ID>
  void HMIStateStopped(ApplicationSharedPtr app) {
    DCHECK_OR_RETURN_VOID(app);
    HmiStatePtr cur = app->CurrentHmiState();
    HmiStatePtr old_hmi_state =
        CreateHmiState(app, HmiState::StateID::STATE_ID_REGULAR);
    DCHECK_OR_RETURN_VOID(old_hmi_state);
    old_hmi_state->set_hmi_level(cur->hmi_level());
    old_hmi_state->set_audio_streaming_state(cur->audio_streaming_state());
    old_hmi_state->set_video_streaming_state(cur->video_streaming_state());
    old_hmi_state->set_system_context(cur->system_context());
    app->RemoveHMIState(ID);
    HmiStatePtr new_hmi_state = app->CurrentHmiState();
    OnStateChanged(app, old_hmi_state, new_hmi_state);
  }

  /**
   * @brief ApplyRegularState setup regular hmi state, that will appear if no
   * specific events are active, without sending ActivateApp
   * @param app appication to setup default State
   * @param state state of new defailt state
   */
  void ApplyRegularState(ApplicationSharedPtr app, HmiStatePtr state);

  /**
   * @brief SetupRegularHmiState set regular HMI State without
   * resolving conflicts and ActivateApp request
   * @param app application
   * @param state hmi_state to setup
   */
  void SetupRegularHmiState(ApplicationSharedPtr app, HmiStatePtr state);

  /**
   * @brief IsSameAppType checks if apps has same types
   * @param app1
   * @param app2
   * @return true if aps have same types, otherwise return false
   */
  bool IsSameAppType(ApplicationConstSharedPtr app1,
                     ApplicationConstSharedPtr app2);

  /**
   * @brief SetupRegularHmiState set regular HMI State without
   * resolving conflicts and ActivateApp request
   * @param app application
   * @param hmi_level of new regular state
   * @param audio_state of new regular state
   * @param video_state of new regular state
   * @param system_context of new regular state
   */
  void SetupRegularHmiState(
      ApplicationSharedPtr app,
      const mobile_apis::HMILevel::eType hmi_level,
      const mobile_apis::AudioStreamingState::eType audio_state,
      const mobile_apis::VideoStreamingState::eType video_state,
      const mobile_apis::SystemContext::eType system_context);

  /**
   * @brief SetupRegularHmiState set regular HMI State without
   * resolving conflicts and ActivateApp request
   * @param app application
   * @param hmi_level of new regular state
   * @param audio_state of new regular state
   * @param video_state of new regular state
   */
  void SetupRegularHmiState(
      ApplicationSharedPtr app,
      const mobile_apis::HMILevel::eType hmi_level,
      const mobile_apis::AudioStreamingState::eType audio_state,
      const mobile_apis::VideoStreamingState::eType video_state);

  /**
   * @brief OnActivateAppResponse calback for activate app response
   * @param message Smart Object
   */
  void OnActivateAppResponse(const smart_objects::SmartObject& message);

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
  HmiStatePtr CreateHmiState(utils::SharedPtr<Application> app,
                             HmiState::StateID state_id) const;

  mobile_apis::AudioStreamingState::eType CalcAudioState(
      ApplicationSharedPtr app,
      const mobile_apis::HMILevel::eType hmi_level) const;

  mobile_apis::VideoStreamingState::eType CalcVideoState(
      ApplicationSharedPtr app,
      const mobile_apis::HMILevel::eType hmi_level) const;

  typedef std::list<HmiState::StateID> StateIDList;
  StateIDList active_states_;
  mutable sync_primitives::Lock active_states_lock_;
  std::map<uint32_t, HmiStatePtr> waiting_for_activate;
  ApplicationManager& app_mngr_;
};
}

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTROLLER_IMPL_H_
