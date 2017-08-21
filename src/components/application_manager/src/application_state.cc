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
#include "utils/logger.h"
#include "utils/macro.h"

namespace {

struct StateIDComparator {
  application_manager::HmiState::StateID state_id_;
  StateIDComparator(application_manager::HmiState::StateID state_id)
      : state_id_(state_id) {}
  bool operator()(const application_manager::HmiStatePtr cur) const {
    return cur->state_id() == state_id_;
  }
};
}

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

ApplicationState::ApplicationState() {}

void ApplicationState::InitState(HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(state);
  sync_primitives::AutoLock auto_lock(hmi_states_lock_);
  hmi_states_.clear();
  hmi_states_.push_back(state);
}

void ApplicationState::AddState(HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(state);
  switch (state->state_id()) {
    case HmiState::StateID::STATE_ID_REGULAR:
      SetRegularState(state);
      return;
    case HmiState::StateID::STATE_ID_POSTPONED:
      SetPostponedState(state);
      return;
    case HmiState::StateID::STATE_ID_CURRENT:
      LOG4CXX_ERROR(logger_, "State of type '" << state << "' can't be added.");
      return;
    default:
      AddHMIState(state);
      break;
  }

  return;
}

void ApplicationState::RemoveState(HmiState::StateID state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(state);
  switch (state) {
    case HmiState::StateID::STATE_ID_CURRENT:
    case HmiState::StateID::STATE_ID_REGULAR:
      LOG4CXX_ERROR(logger_,
                    "State of type '" << state << "'can't be removed.");
      break;
    case HmiState::StateID::STATE_ID_POSTPONED:
      RemovePostponedState();
      break;
    default:
      RemoveHMIState(state);
      break;
  }
}

HmiStatePtr ApplicationState::GetState(HmiState::StateID state_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  switch (state_id) {
    case HmiState::StateID::STATE_ID_REGULAR:
      LOG4CXX_DEBUG(logger_, "Getting regular state.");
      return RegularHmiState();
    case HmiState::StateID::STATE_ID_POSTPONED:
      LOG4CXX_DEBUG(logger_, "Getting postponed state.");
      return PostponedHmiState();
    default:
      LOG4CXX_DEBUG(logger_, "Getting current state.");
      return CurrentHmiState();
  }
}

void ApplicationState::AddHMIState(HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(state);
  sync_primitives::AutoLock auto_lock(hmi_states_lock_);
  HmiStates::iterator it = std::find_if(hmi_states_.begin(),
                                        hmi_states_.end(),
                                        StateIDComparator(state->state_id()));
  if (hmi_states_.end() != it) {
    LOG4CXX_WARN(
        logger_,
        "Hmi state with ID "
            << state->state_id()
            << "has been already applied to this application. Ignoring");
    return;
  }

  hmi_states_.push_back(state);
}

void ApplicationState::RemoveHMIState(HmiState::StateID state_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(hmi_states_lock_);
  HmiStates::iterator it = std::find_if(
      hmi_states_.begin(), hmi_states_.end(), StateIDComparator(state_id));
  if (it == hmi_states_.end()) {
    LOG4CXX_ERROR(logger_, "Unsuccesful remove HmiState: " << state_id);
    return;
  }

  // unable to remove regular state
  DCHECK_OR_RETURN_VOID(it != hmi_states_.begin());
  HmiStates::iterator next = it;
  HmiStates::iterator prev = it;
  next++;
  prev--;
  if (next != hmi_states_.end()) {
    HmiStatePtr next_state = *next;
    HmiStatePtr prev_state = *prev;
    next_state->set_parent(prev_state);
  }
  hmi_states_.erase(it);
}

void ApplicationState::RemovePostponedState() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(hmi_states_lock_);
  DCHECK_OR_RETURN_VOID(!hmi_states_.empty());

  StateIDComparator finder(HmiState::StateID::STATE_ID_POSTPONED);

  HmiStates::iterator postponed_state =
      std::find_if(hmi_states_.begin(), hmi_states_.end(), finder);

  if (hmi_states_.end() == postponed_state) {
    LOG4CXX_ERROR(logger_, "No postponed state is set for app.");
    return;
  }
  hmi_states_.erase(postponed_state);
}

void ApplicationState::SetRegularState(HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(state);
  DCHECK_OR_RETURN_VOID(state->state_id() ==
                        HmiState::StateID::STATE_ID_REGULAR);
  sync_primitives::AutoLock auto_lock(hmi_states_lock_);
  DCHECK_OR_RETURN_VOID(!hmi_states_.empty());

  HmiStatePtr front_state = hmi_states_.front();
  HmiState::StateID front_state_id = front_state->state_id();
  if (HmiState::StateID::STATE_ID_POSTPONED == front_state_id) {
    // Drop postponed state
    hmi_states_.erase(hmi_states_.begin());
  }

  // Drop regular state
  hmi_states_.erase(hmi_states_.begin());

  if (!hmi_states_.empty()) {
    HmiStatePtr front_state = hmi_states_.front();
    front_state->set_parent(state);
  }
  hmi_states_.insert(hmi_states_.begin(), state);
  if (HmiState::StateID::STATE_ID_POSTPONED == front_state_id) {
    hmi_states_.insert(hmi_states_.begin(), front_state);
  }
}

void ApplicationState::SetPostponedState(HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(state);
  DCHECK_OR_RETURN_VOID(state->state_id() ==
                        HmiState::StateID::STATE_ID_POSTPONED);
  sync_primitives::AutoLock auto_lock(hmi_states_lock_);
  DCHECK_OR_RETURN_VOID(!hmi_states_.empty());
  HmiStatePtr front_state = hmi_states_.front();
  if (front_state->state_id() == HmiState::StateID::STATE_ID_POSTPONED) {
    hmi_states_.erase(hmi_states_.begin());
  }
  hmi_states_.insert(hmi_states_.begin(), state);
}

HmiStatePtr ApplicationState::CurrentHmiState() const {
  sync_primitives::AutoLock auto_lock(hmi_states_lock_);
  DCHECK_OR_RETURN(!hmi_states_.empty(), HmiStatePtr());
  HmiStatePtr back_state = hmi_states_.back();
  DCHECK_OR_RETURN(
      back_state->state_id() != HmiState::StateID::STATE_ID_POSTPONED,
      HmiStatePtr());
  return back_state;
}

HmiStatePtr ApplicationState::RegularHmiState() const {
  sync_primitives::AutoLock auto_lock(hmi_states_lock_);
  DCHECK_OR_RETURN(!hmi_states_.empty(), HmiStatePtr());
  HmiStates::const_iterator front_itr = hmi_states_.begin();
  if ((*front_itr)->state_id() == HmiState::StateID::STATE_ID_POSTPONED) {
    ++front_itr;
  }
  return *front_itr;
}

HmiStatePtr ApplicationState::PostponedHmiState() const {
  sync_primitives::AutoLock auto_lock(hmi_states_lock_);
  DCHECK_OR_RETURN(!hmi_states_.empty(), HmiStatePtr());
  HmiStatePtr front_state = hmi_states_.front();
  return front_state->state_id() == HmiState::StateID::STATE_ID_POSTPONED
             ? front_state
             : HmiStatePtr();
}

}  // namespace application_manager
