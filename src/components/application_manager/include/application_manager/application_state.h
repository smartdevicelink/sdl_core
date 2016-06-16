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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_STATE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_STATE_H_
#include <vector>
#include "utils/lock.h"
#include "utils/macro.h"
#include "application_manager/hmi_state.h"

namespace application_manager {

typedef std::vector<HmiStatePtr> HmiStates;

/*
 * Class represents application state, i.e. current HMI level, audio streaming
 * state and context
 * Current implementation:
 * - has regular state, which is default or base state
 * - temporary states can be applied on top of regular state
 * - on temporary state end it is being removed from states list
 * - current state is the consolidated state of all the states, since different
 * temporary state can affect one or more parameters (HMI state, audio, context)
 * - can have postponed state (comes from resumption process), which is
 * not applied on top, but is being added before base and can replace base later
 * on
 */
class ApplicationState {
 public:
  /**
   * @brief ApplicationState constructor
   */
  ApplicationState();

  /**
   * @brief Init state
   * @param state Initial state
   */
  void InitState(HmiStatePtr state);

  /**
   * @brief Adds state to states storage
   * @param state State of application
   */
  void AddState(HmiStatePtr state);

  /**
   * @brief Removes state from states storage
   * @param state State of application
   */
  void RemoveState(HmiState::StateID state);

  /**
   * @brief Gets state by state id
   * @param state_id State id
   * @return Pointer to application state
   */
  HmiStatePtr GetState(HmiState::StateID state_id) const;

 private:
  /**
   * @brief AddHMIState the function that will change application's
   * hmi state.
   *
   * @param app_id id of the application whose hmi level should be changed.
   *
   * @param state new hmi state for certain application.
   */
  void AddHMIState(HmiStatePtr state);

  /**
   * @brief RemoveHMIState the function that will turn back hmi_level after end
   * of some event
   *
   * @param app_id id of the application whose hmi level should be changed.
   *
   * @param state_id that should be removed
   */
  void RemoveHMIState(HmiState::StateID state_id);

  /**
   * @brief Removes postponed state
   */
  void RemovePostponedState();

  /**
   * @brief Sets regular state of application
   * @param state State of application
   */
  void SetRegularState(HmiStatePtr state);

  /**
  * @brief Sets postponed state of application.
  * This state could be set as regular later on
  *
  * @param state state to setup
  */
  void SetPostponedState(HmiStatePtr state);

  /**
   * @brief HmiState of application within active events PhoneCall, TTS< etc ...
   * @return Active HmiState of application
   */
  HmiStatePtr CurrentHmiState() const;

  /**
   * @brief RegularHmiState of application without active events VR, TTS etc ...
   * @return HmiState of application
   */
  HmiStatePtr RegularHmiState() const;

  /**
   * @brief PostponedHmiState returns postponed hmi state of application
   * if it's present
   *
   * @return Postponed hmi state of application
   */
  HmiStatePtr PostponedHmiState() const;

  /**
   * @brief Active states of application
   */
  HmiStates hmi_states_;
  mutable sync_primitives::Lock hmi_states_lock_;

  DISALLOW_COPY_AND_ASSIGN(ApplicationState);
};
}

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_STATE_H_
