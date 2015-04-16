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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTROLLER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTROLLER_H_
#include <list>

#include "application_manager/hmi_state.h"
#include "application_manager/application.h"
#include "event_engine/event_observer.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/state_context.h"
#include "utils/lock.h"

namespace application_manager {
class ApplicationManagerImpl;
class StateController : public event_engine::EventObserver {
  public:

    StateController();

    /**
     * @brief SetRegularState setup regular hmi state, tha will appear if no
     * specific events are active
     * @param app appication to setup regular State
     * @param state state of new regular state
     */
    template <bool SendActivateApp>
    void SetRegularState(ApplicationSharedPtr app,
                         HmiStatePtr state) {
      DCHECK_OR_RETURN_VOID(app);
      DCHECK_OR_RETURN_VOID(state);
      DCHECK_OR_RETURN_VOID(state->state_id() == HmiState::STATE_ID_REGULAR);

      if (SendActivateApp) {
        uint32_t corr_id = MessageHelper::SendActivateAppToHMI(app->app_id());
        subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_ActivateApp,
                           corr_id);
        waiting_for_activate[app->app_id()] = state;
      } else {
        ApplyRegularState(app, state);
      }
    }

    /**
     * @brief SetRegularState Change regular audio state
     * @param app appication to setup regular State
     * @param audio_state of new regular state
     */
    void SetRegularState(ApplicationSharedPtr app,
                         const mobile_apis::AudioStreamingState::eType audio_state);

    /**
     * @brief SetRegularState Change regular hmi level and audio state
     * @param app appication to setup regular State
     * @param hmi_level of new regular state
     * @param audio_state of new regular state
     * @param SendActivateApp: if true, ActivateAppRequest will be sent on HMI    */
    template <bool SendActivateApp>
    void SetRegularState(ApplicationSharedPtr app,
                         const mobile_apis::HMILevel::eType hmi_level,
                         const mobile_apis::AudioStreamingState::eType audio_state) {
      DCHECK_OR_RETURN_VOID(app);
      HmiStatePtr prev_regular = app->RegularHmiState();
      DCHECK_OR_RETURN_VOID(prev_regular);
      HmiStatePtr hmi_state = CreateHmiState(app->app_id(),
                                             HmiState::StateID::STATE_ID_REGULAR);
      DCHECK_OR_RETURN_VOID(hmi_state);
      hmi_state->set_hmi_level(hmi_level);
      hmi_state->set_audio_streaming_state(audio_state);
      hmi_state->set_system_context(prev_regular->system_context());
      SetRegularState<SendActivateApp>(app, hmi_state);
    }


    /**
     * @brief SetRegularState Change regular hmi level
     * @param app appication to setup regular State
     * @param hmi_level of new regular state
     * @param SendActivateApp: if true, ActivateAppRequest will be sent on HMI    */
    template <bool SendActivateApp>
    void SetRegularState(ApplicationSharedPtr app,
                         const mobile_apis::HMILevel::eType hmi_level) {
      DCHECK_OR_RETURN_VOID(app);
      HmiStatePtr prev_regular = app->RegularHmiState();
      DCHECK_OR_RETURN_VOID(prev_regular);
      HmiStatePtr hmi_state = CreateHmiState(app->app_id(),
                                             HmiState::StateID::STATE_ID_REGULAR);
      DCHECK_OR_RETURN_VOID(hmi_state);
      hmi_state->set_hmi_level(hmi_level);
      hmi_state->set_audio_streaming_state(prev_regular->audio_streaming_state());
      hmi_state->set_system_context(prev_regular->system_context());
      SetRegularState<SendActivateApp>(app, hmi_state);
    }

    /**
     * @brief SetRegularState Change regular hmi level, audio state and system context
     * @param app appication to setup regular State
     * @param hmi_level of new regular state
     * @param audio_state of new regular state
     * @param system_context of new regular state
     * @param SendActivateApp: if true, ActivateAppRequest will be sent on HMI    */
    template <bool SendActivateApp>
    void SetRegularState(ApplicationSharedPtr app,
                         const mobile_apis::HMILevel::eType hmi_level,
                         const mobile_apis::AudioStreamingState::eType audio_state,
                         const mobile_apis::SystemContext::eType system_context) {
      DCHECK_OR_RETURN_VOID(app);
      HmiStatePtr hmi_state = CreateHmiState(app->app_id(),
                                             HmiState::StateID::STATE_ID_REGULAR);
      DCHECK_OR_RETURN_VOID(hmi_state);
      hmi_state->set_hmi_level(hmi_level);
      hmi_state->set_audio_streaming_state(audio_state);
      hmi_state->set_system_context(system_context);
      SetRegularState<SendActivateApp>(app, hmi_state);
    }

    /**
     * @brief SetRegularState Change regular  system context
     * @param app appication to setup regular State
     * @param system_context of new regular state
     */
    void SetRegularState(ApplicationSharedPtr app,
                         const mobile_apis::SystemContext::eType system_context) {
      DCHECK_OR_RETURN_VOID(app);
      HmiStatePtr prev_regular = app->RegularHmiState();
      DCHECK_OR_RETURN_VOID(prev_regular);
      HmiStatePtr hmi_state = CreateHmiState(app->app_id(),
                                             HmiState::StateID::STATE_ID_REGULAR);
      DCHECK_OR_RETURN_VOID(hmi_state);
      hmi_state->set_hmi_level(prev_regular->hmi_level());
      hmi_state->set_audio_streaming_state(prev_regular->audio_streaming_state());
      hmi_state->set_system_context(system_context);
      SetRegularState<false>(app, hmi_state);
    }

    // EventObserver interface
    void on_event(const event_engine::Event& event);

    /**
     * @brief OnStateChanged send HMIStatusNotification if neded
     * @param app application
     * @param old_state state before change
     * @param new_state state after change
     */
    void OnStateChanged(ApplicationSharedPtr app, HmiStatePtr old_state,
                        HmiStatePtr new_state);
    /**
     * @brief state_context getter for state_context
     * @return
     */
    const StateContext& state_context() const {
      return state_context_;
    }

    /**
     * @brief ApplyStatesForApp apply active HMI states for new App without s
     * ending any OnHMIStatus
     * @param app application to apply states
     */
    void ApplyStatesForApp(ApplicationSharedPtr app);
  private:
    /**
     * Execute Unary punction for each application
     */
    template < typename UnaryFunction,
             typename ContextAcessor = ApplicationManagerImpl >
    void ForEachApplication(UnaryFunction func) {
      using namespace utils;
      typename ContextAcessor::ApplicationListAccessor accessor;
      typedef typename ContextAcessor::ApplictionSetConstIt Iter;
      for (Iter it = accessor.begin(); it != accessor.end(); ++it) {
        if (it->valid()) {
          ApplicationConstSharedPtr const_app = *it;
          func(ContextAcessor::instance()->application(const_app->app_id()));
        }
      }
    }

    /**
     * @brief The HmiLevelConflictResolver struct
     * Move other application to HmiStates if applied moved to FULL or LIMITED
     */
    struct HmiLevelConflictResolver {
      ApplicationSharedPtr applied_;
      HmiStatePtr state_;
      StateController* state_ctrl_;
      HmiLevelConflictResolver(ApplicationSharedPtr app,
                               HmiStatePtr state,
                               StateController* state_ctrl):
        applied_(app), state_(state) {}
      void operator()(ApplicationSharedPtr to_resolve);
    };

    /**
     *  Function to add new temporary HmiState for application
     */
    template <HmiState::StateID ID>
    void HMIStateStarted(ApplicationSharedPtr app) {
      DCHECK_OR_RETURN_VOID(app);
      HmiStatePtr old_hmi_state = app->CurrentHmiState();
      HmiStatePtr new_hmi_state = CreateHmiState(app->app_id(), ID);
      DCHECK_OR_RETURN_VOID(new_hmi_state);
      DCHECK_OR_RETURN_VOID(new_hmi_state->state_id() != HmiState::STATE_ID_REGULAR);
      new_hmi_state->set_parent(old_hmi_state);
      app->AddHMIState(new_hmi_state);
      OnStateChanged(app, old_hmi_state, new_hmi_state);
    }

    /**
     * @brief TempStateStarted add HMI State ID in StateController collection
     * @param ID state identifier
     */
    void TempStateStarted(HmiState::StateID ID);

    /**
     * @brief TempStateStopped remove HMI State ID from StateController collection
     * @param ID state identifier
     */
    void TempStateStopped(HmiState::StateID ID);


    /**
     * Function to remove temporary HmiState for application
     */
    template <HmiState::StateID ID>
    void HMIStateStopped(ApplicationSharedPtr app) {
      DCHECK_OR_RETURN_VOID(app);
      HmiStatePtr cur = app->CurrentHmiState();
      HmiStatePtr old_hmi_state = CreateHmiState(app->app_id(),
                                  HmiState::StateID::STATE_ID_REGULAR);
      DCHECK_OR_RETURN_VOID(old_hmi_state);
      old_hmi_state->set_hmi_level(cur->hmi_level());
      old_hmi_state->set_audio_streaming_state(cur->audio_streaming_state());
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
    void ApplyRegularState(ApplicationSharedPtr app,
                           HmiStatePtr state);

    /**
     * @brief SetupRegularHmiState set regular HMI State without
     * resolwing conflicts and ActivateApp request
     * @param app application
     * @param state hmi_state to setup
     */
    void SetupRegularHmiState(ApplicationSharedPtr app,
                              HmiStatePtr state);

    /**
     * @brief IsSameAppType checkis if apps has same types
     * @param app1
     * @param app2
     * @return true if aps have same types, otherwise return false
     */
    bool IsSameAppType(ApplicationConstSharedPtr app1,
                       ApplicationConstSharedPtr app2);

    /**
     * @brief SetupRegularHmiState set regular HMI State without
     * resolwing conflicts and ActivateApp request
     * @param app application
     * @param hmi_level of new regular state
     * @param audio_state of new regular state
     * @param system_context of new regular state
     */
    void SetupRegularHmiState(ApplicationSharedPtr app,
                              const mobile_apis::HMILevel::eType hmi_level,
                              const mobile_apis::AudioStreamingState::eType audio_state,
                              const mobile_apis::SystemContext::eType system_context);

    /**
     * @brief SetupRegularHmiState set regular HMI State without
     * resolwing conflicts and ActivateApp request
     * @param app application
     * @param hmi_level of new regular state
     * @param audio_state of new regular state
     */
    void SetupRegularHmiState(ApplicationSharedPtr app,
                              const mobile_apis::HMILevel::eType hmi_level,
                              const mobile_apis::AudioStreamingState::eType audio_state);

    /**
     * @brief OnActivateAppResponse calback for activate app response
     * @param message Smart Object
     */
    void OnActivateAppResponse(const smart_objects::SmartObject& message);

    /**
     * @brief OnPhoneCallStarted process Phone Call Started event
     */
    void OnPhoneCallStarted();

    /**
     * @brief OnPhoneCallEnded process Phone Call Ended event
     */
    void OnPhoneCallEnded();


    /**
     * @brief OnSafetyModeEnabled process Safety Mode Enable event
     */
    void OnSafetyModeEnabled();

    /**
     * @brief OnSafetyModeDisabled process Safety Mode Disable event
     */
    void OnSafetyModeDisabled();

    /**
     * @brief OnVRStarted process VR session started
     */
    void OnVRStarted();

    /**
     * @brief OnVREnded process VR session ended
     */
    void OnVREnded();
    /**
     * @brief OnTTSStarted process TTS session started
     */
    void OnTTSStarted();

    /**
     * @brief OnTTSEnded process TTS session ended
     */
    void OnTTSStopped();

    /**
     * @brief CreateHmiState creates Hmi state according to state_id
     * @param app_id application ID
     * @param state_id state id
     * @return
     */
    HmiStatePtr CreateHmiState(uint32_t app_id, HmiState::StateID state_id);

    typedef std::list<HmiState::StateID> StateIDList;
    StateIDList active_states_;
    sync_primitives::Lock active_states_lock_;
    std::map<uint32_t, HmiStatePtr> waiting_for_activate;
    StateContext state_context_;
};

}

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_STATE_CONTROLLER_H_

