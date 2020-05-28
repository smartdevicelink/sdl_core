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

#include <map>
#include <vector>

#include "application_manager/hmi_state.h"
#include "utils/lock.h"
#include "utils/macro.h"

namespace application_manager {

typedef std::vector<WindowID> WindowIds;
typedef std::vector<HmiStatePtr> HmiStates;
typedef std::map<WindowID, HmiStates> HmiStatesMap;
typedef std::vector<std::string> WindowNames;
typedef std::map<WindowID, std::string> WindowNamesMap;

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
   * @param window_id window id for HMI state
   * @param window_name name of inited window
   * @param state Initial state
   */
  void InitState(const WindowID window_id,
                 const std::string& window_name,
                 HmiStatePtr state);

  /**
   * @brief Adds state to states storage
   * @param window_id window id for HMI state
   * @param state State of application
   */
  void AddState(const WindowID window_id, HmiStatePtr state);

  /**
   * @brief Removes state from states storage
   * @param window_id window id for HMI state
   * @param state State of application
   */
  void RemoveState(const WindowID window_id, HmiState::StateID state);

  /**
   * @brief Gets state by state id
   * @param window_id window id for HMI state
   * @param state_id State id
   * @return Pointer to application state
   */
  HmiStatePtr GetState(const WindowID window_id,
                       HmiState::StateID state_id) const;

  /**
   * @brief Gets the list of all states matches provided state id
   * @param state_id state id to get
   * @return list of all states matches provided state id
   */
  HmiStates GetStates(const HmiState::StateID state_id) const;

  /**
   * @brief Getter for a list of available application windows including the
   * main
   * @return list of available window ids
   */
  WindowIds GetWindowIds() const;

  /**
   * @brief Getter f0r a list of all existing window names
   * @return list of available window names
   */
  WindowNames GetWindowNames() const;

 private:
  /**
   * @brief AddHMIState the function that will change application's
   * hmi state.
   *
   * @param window_id window id for HMI state
   *
   * @param state new hmi state for certain application.
   */
  void AddHMIState(const WindowID window_id, HmiStatePtr state);

  /**
   * @brief RemoveHMIState the function that will turn back hmi_level after end
   * of some event
   *
   * @param window_id window id for HMI state.
   *
   * @param state_id that should be removed
   */
  void RemoveHMIState(const WindowID window_id, HmiState::StateID state_id);

  /**
   * @brief RemoveWindowHMIStates removes all HMI states related to specified
   * window
   * @param window_id window ID to remove
   */
  void RemoveWindowHMIStates(const WindowID window_id);

  /**
   * @brief Removes postponed state
   * @param window_id window id for HMI state
   */
  void RemovePostponedState(const WindowID window_id);

  /**
   * @brief Sets regular state of application
   * @param window_id window id for HMI state
   * @param state State of application
   */
  void SetRegularState(const WindowID window_id, HmiStatePtr state);

  /**
   * @brief Sets postponed state of application.
   * This state could be set as regular later on
   * @param window_id window id for HMI state
   * @param state state to setup
   */
  void SetPostponedState(const WindowID window_id, HmiStatePtr state);

  /**
   * @brief HmiState of application within active events PhoneCall, TTS< etc ...
   * @param window_id window id for HMI state
   * @return Active HmiState of application
   */
  HmiStatePtr CurrentHmiState(const WindowID window_id) const;

  /**
   * @brief RegularHmiState of application without active events VR, TTS etc ...
   * @param window_id window id for HMI state
   * @return HmiState of application
   */
  HmiStatePtr RegularHmiState(const WindowID window_id) const;

  /**
   * @brief PostponedHmiState returns postponed hmi state of application
   * if it's present
   * @param window_id window id for HMI state
   * @return Postponed hmi state of application
   */
  HmiStatePtr PostponedHmiState(const WindowID window_id) const;

  /**
   * @brief Active states of application
   */
  HmiStatesMap hmi_states_map_;

  /**
   * @brief hmi_states_map_lock_
   */
  mutable sync_primitives::Lock hmi_states_map_lock_;

  DISALLOW_COPY_AND_ASSIGN(ApplicationState);
};
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_STATE_H_
