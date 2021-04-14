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

#include "application_manager/application_state.h"

#include <algorithm>
#include <string>

#include "utils/logger.h"
#include "utils/macro.h"

namespace {

struct StateIDComparator {
  application_manager::HmiState::StateID state_id_;
  explicit StateIDComparator(application_manager::HmiState::StateID state_id)
      : state_id_(state_id) {}
  bool operator()(const application_manager::HmiStatePtr cur) const {
    return cur->state_id() == state_id_;
  }
};
}  // namespace

namespace application_manager {

SDL_CREATE_LOG_VARIABLE("ApplicationManager")

ApplicationState::ApplicationState() {}

void ApplicationState::InitState(const WindowID window_id,
                                 const std::string& window_name,
                                 HmiStatePtr state) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(state);
  SDL_LOG_DEBUG("Initing state " << state << " for window " << window_id
                                 << " with name " << window_name);
  {
    sync_primitives::AutoLock auto_lock(hmi_states_map_lock_);
    HmiStates& states = hmi_states_map_[window_id];
    states.push_back(state);
  }
}

void ApplicationState::AddState(const WindowID window_id, HmiStatePtr state) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(state);
  switch (state->state_id()) {
    case HmiState::StateID::STATE_ID_REGULAR:
      SetRegularState(window_id, state);
      return;
    case HmiState::StateID::STATE_ID_POSTPONED:
      SetPostponedState(window_id, state);
      return;
    case HmiState::StateID::STATE_ID_CURRENT:
      SDL_LOG_ERROR("State of type '" << state << "' can't be added for window "
                                      << window_id);
      return;
    default:
      AddHMIState(window_id, state);
      break;
  }

  return;
}

void ApplicationState::RemoveState(const WindowID window_id,
                                   HmiState::StateID state) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(state);
  switch (state) {
    case HmiState::StateID::STATE_ID_CURRENT:
    case HmiState::StateID::STATE_ID_REGULAR:
      if (mobile_apis::PredefinedWindows::DEFAULT_WINDOW == window_id) {
        SDL_LOG_ERROR("State of type '" << state
                                        << "' can't be removed for window "
                                        << window_id);
        return;
      }

      RemoveWindowHMIStates(window_id);
      break;
    case HmiState::StateID::STATE_ID_POSTPONED:
      RemovePostponedState(window_id);
      break;
    default:
      RemoveHMIState(window_id, state);
      break;
  }
}

HmiStatePtr ApplicationState::GetState(const WindowID window_id,
                                       HmiState::StateID state_id) const {
  switch (state_id) {
    case HmiState::StateID::STATE_ID_REGULAR:
      SDL_LOG_DEBUG("Getting regular state for window " << window_id);
      return RegularHmiState(window_id);
    case HmiState::StateID::STATE_ID_POSTPONED:
      SDL_LOG_DEBUG("Getting postponed state for window " << window_id);
      return PostponedHmiState(window_id);
    default:
      SDL_LOG_TRACE("Getting current state for window " << window_id);
      return CurrentHmiState(window_id);
  }
}

HmiStates ApplicationState::GetStates(const HmiState::StateID state_id) const {
  SDL_LOG_DEBUG("Collecting all states of type " << state_id);

  HmiStates hmi_states;
  sync_primitives::AutoLock auto_lock(hmi_states_map_lock_);
  for (const auto& hmi_state_pair : hmi_states_map_) {
    hmi_states.push_back(GetState(hmi_state_pair.first, state_id));
  }

  return hmi_states;
}

WindowIds ApplicationState::GetWindowIds() const {
  SDL_LOG_DEBUG("Collecting available window ID's");

  WindowIds window_ids;
  std::string stringified_window_ids;

  sync_primitives::AutoLock auto_lock(hmi_states_map_lock_);
  for (const auto& hmi_state_pair : hmi_states_map_) {
    window_ids.push_back(hmi_state_pair.first);
    stringified_window_ids += (stringified_window_ids.empty() ? "" : ", ") +
                              std::to_string(hmi_state_pair.first);
  }

  SDL_LOG_DEBUG("Existing window IDs: [" + stringified_window_ids + "]");
  return window_ids;
}

void ApplicationState::AddHMIState(const WindowID window_id,
                                   HmiStatePtr state) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(state);
  sync_primitives::AutoLock auto_lock(hmi_states_map_lock_);
  HmiStates& hmi_states = hmi_states_map_[window_id];
  HmiStates::iterator it = std::find_if(hmi_states.begin(),
                                        hmi_states.end(),
                                        StateIDComparator(state->state_id()));
  if (hmi_states.end() != it) {
    SDL_LOG_WARN("Hmi state with ID "
                 << state->state_id() << "has been already applied for window "
                 << window_id << " of this application. Overwriting");
    EraseHMIState(hmi_states, it);
  }

  if (!hmi_states.empty()) {
    HmiStatePtr back_state = hmi_states.back();
    state->set_parent(back_state);
  }
  hmi_states.push_back(state);
}

void ApplicationState::RemoveHMIState(const WindowID window_id,
                                      HmiState::StateID state_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(hmi_states_map_lock_);
  HmiStates& hmi_states = hmi_states_map_[window_id];
  // unable to remove regular state
  DCHECK_OR_RETURN_VOID(state_id != HmiState::StateID::STATE_ID_REGULAR);
  HmiStates::iterator it = std::find_if(
      hmi_states.begin(), hmi_states.end(), StateIDComparator(state_id));
  if (hmi_states.end() == it) {
    SDL_LOG_ERROR("Unsuccesful remove HmiState: " << state_id << " for window "
                                                  << window_id);
    return;
  }

  EraseHMIState(hmi_states, it);
}

void ApplicationState::EraseHMIState(HmiStates& hmi_states,
                                     HmiStates::iterator it) {
  if (hmi_states.begin() == it) {
    (*it)->set_parent(nullptr);
  } else {
    HmiStates::iterator next = it;
    HmiStates::iterator prev = it;
    ++next;
    --prev;

    if (hmi_states.end() != next) {
      HmiStatePtr next_state = *next;
      HmiStatePtr prev_state = *prev;
      next_state->set_parent(prev_state);
    }
  }

  hmi_states.erase(it);
}

void ApplicationState::RemoveWindowHMIStates(const WindowID window_id) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(mobile_apis::PredefinedWindows::DEFAULT_WINDOW !=
                        window_id);

  SDL_LOG_DEBUG("Removing HMI states for window with id #" << window_id);
  {
    sync_primitives::AutoLock auto_lock(hmi_states_map_lock_);
    hmi_states_map_.erase(window_id);
  }
}

void ApplicationState::RemovePostponedState(const WindowID window_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(postponed_states_map_lock_);
  size_t deleted_elements = postponed_states_map_.erase(window_id);
  if (0 == deleted_elements) {
    SDL_LOG_ERROR("No postponed state is set for window " << window_id);
  }
}

void ApplicationState::SetRegularState(const WindowID window_id,
                                       HmiStatePtr state) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(state);
  DCHECK_OR_RETURN_VOID(state->state_id() ==
                        HmiState::StateID::STATE_ID_REGULAR);
  sync_primitives::AutoLock auto_lock(hmi_states_map_lock_);
  HmiStates& hmi_states = hmi_states_map_[window_id];
  DCHECK_OR_RETURN_VOID(!hmi_states.empty());

  // Drop regular state
  HmiStates::iterator it =
      std::find_if(hmi_states.begin(),
                   hmi_states.end(),
                   StateIDComparator(HmiState::StateID::STATE_ID_REGULAR));
  DCHECK_OR_RETURN_VOID(hmi_states.end() != it);
  EraseHMIState(hmi_states, it);

  if (!hmi_states.empty()) {
    HmiStatePtr back_state = hmi_states.back();
    state->set_parent(back_state);
  }

  // Insert new regular state
  hmi_states.push_back(state);
}

void ApplicationState::SetPostponedState(const WindowID window_id,
                                         HmiStatePtr state) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(state);
  DCHECK_OR_RETURN_VOID(state->state_id() ==
                        HmiState::StateID::STATE_ID_POSTPONED);

  sync_primitives::AutoLock auto_lock(postponed_states_map_lock_);
  postponed_states_map_[window_id] = state;
}

HmiStatePtr ApplicationState::CurrentHmiState(const WindowID window_id) const {
  sync_primitives::AutoLock auto_lock(hmi_states_map_lock_);
  auto it_states = hmi_states_map_.find(window_id);
  DCHECK_OR_RETURN(it_states != hmi_states_map_.end(), HmiStatePtr());

  const HmiStates& hmi_states = it_states->second;
  DCHECK_OR_RETURN(!hmi_states.empty(), HmiStatePtr());

  HmiStatePtr back_state = hmi_states.back();

  return back_state;
}

HmiStatePtr ApplicationState::RegularHmiState(const WindowID window_id) const {
  sync_primitives::AutoLock auto_lock(hmi_states_map_lock_);
  auto it_states = hmi_states_map_.find(window_id);
  DCHECK_OR_RETURN(it_states != hmi_states_map_.end(), HmiStatePtr());

  const HmiStates& hmi_states = it_states->second;
  DCHECK_OR_RETURN(!hmi_states.empty(), HmiStatePtr());
  auto it =
      std::find_if(hmi_states.begin(),
                   hmi_states.end(),
                   StateIDComparator(HmiState::StateID::STATE_ID_REGULAR));
  DCHECK_OR_RETURN(hmi_states.end() != it, HmiStatePtr());

  return *it;
}

HmiStatePtr ApplicationState::PostponedHmiState(
    const WindowID window_id) const {
  sync_primitives::AutoLock auto_lock(postponed_states_map_lock_);

  auto it = postponed_states_map_.find(window_id);
  return it != postponed_states_map_.end() ? it->second : HmiStatePtr();
}

}  // namespace application_manager
