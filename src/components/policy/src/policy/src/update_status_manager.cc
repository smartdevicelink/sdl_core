/*
 Copyright (c) 2014, Ford Motor Company
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

#include "policy/update_status_manager.h"
#include "policy/policy_listener.h"
#include "utils/logger.h"

namespace  policy {

CREATE_LOGGERPTR_GLOBAL(logger_, "UpdateStatusManager")

UpdateStatusManager::UpdateStatusManager() :
  listener_(NULL),
  exchange_in_progress_(false),
  update_required_(false),
  exchange_pending_(false),
  last_update_status_(policy::StatusUnknown),
  update_response_timer_(this) {
}

UpdateStatusManager::~UpdateStatusManager() {
  LOG4CXX_DEBUG(logger_, "Destroy update Status manager");
}

void UpdateStatusManager::set_listener(PolicyListener* listener) {
  listener_ = listener;
}

void UpdateStatusManager::OnUpdateSentOut(uint32_t update_timeout) {
  LOG4CXX_INFO(logger_, "OnUpdateSentOut");
  update_response_timer_.start(update_timeout);
  set_exchange_in_progress(true);
}

void UpdateStatusManager::OnUpdateTimeoutOccurs() {
  LOG4CXX_INFO(logger_, "OnUpdateTimeoutOccurs");
  set_update_required(true);
  set_exchange_in_progress(false);
}

void UpdateStatusManager::OnValidUpdateReceived() {
  LOG4CXX_INFO(logger_, "OnValidUpdateReceived");
  update_response_timer_.stop();
  set_update_required(false);
  set_exchange_in_progress(false);
}

void UpdateStatusManager::OnWrongUpdateReceived() {
  LOG4CXX_INFO(logger_, "OnWrongUpdateReceived");
  update_response_timer_.stop();
  set_update_required(true);
  set_exchange_in_progress(false);
}

void UpdateStatusManager::OnResetDefaultPT(bool is_update_required) {
  LOG4CXX_INFO(logger_, "OnResetDefaultPT");
  exchange_in_progress_ = false;
  update_required_ = is_update_required;
  exchange_pending_ = false;
  last_update_status_ = policy::StatusUnknown;
}

void UpdateStatusManager::OnResetRetrySequence() {
  LOG4CXX_INFO(logger_, "OnResetRetrySequence");
  if (exchange_in_progress_) {
    set_exchange_pending(true);
  }
  set_update_required(true);
}

void UpdateStatusManager::OnNewApplicationAdded() {
  LOG4CXX_INFO(logger_, "OnNewApplicationAdded");
  set_update_required(true);
}

void UpdateStatusManager::OnPolicyInit(bool is_update_required) {
  LOG4CXX_INFO(logger_, "OnPolicyInit");
  update_required_ = is_update_required;
}

PolicyTableStatus UpdateStatusManager::GetUpdateStatus() {
  LOG4CXX_INFO(logger_, "GetUpdateStatus");
  if (!exchange_in_progress_ && !exchange_pending_ && !update_required_) {
    return PolicyTableStatus::StatusUpToDate;
  }

  if (update_required_ && !exchange_in_progress_ && !exchange_pending_) {
    return PolicyTableStatus::StatusUpdateRequired;
  }

  return PolicyTableStatus::StatusUpdatePending;
}

void UpdateStatusManager::CheckUpdateStatus() {
  LOG4CXX_INFO(logger_, "CheckUpdateStatus");
  policy::PolicyTableStatus status = GetUpdateStatus();
  if (listener_ && last_update_status_ != status) {
    listener_->OnUpdateStatusChanged(status);
  }
  last_update_status_ = status;
}

void UpdateStatusManager::set_exchange_in_progress(bool value) {
  sync_primitives::AutoLock lock(exchange_in_progress_lock_);
  LOG4CXX_INFO(logger_,
               "Exchange in progress value is:" << std::boolalpha << value);
  exchange_in_progress_ = value;
  CheckUpdateStatus();
}

void UpdateStatusManager::set_exchange_pending(bool value) {
  sync_primitives::AutoLock lock(exchange_pending_lock_);
  LOG4CXX_INFO(logger_,
               "Exchange pending value is:" << std::boolalpha << value);
  exchange_pending_ = value;
  CheckUpdateStatus();
}

void UpdateStatusManager::set_update_required(bool value) {
  sync_primitives::AutoLock lock(update_required_lock_);
  LOG4CXX_INFO(logger_, "Update required value is:" << std::boolalpha << value);
  update_required_ = value;
  CheckUpdateStatus();
}

UpdateStatusManager::UpdateResponseTimer::~UpdateResponseTimer() {
  LOG4CXX_DEBUG(logger_, "Destroy update Status manager timer");
}

} // namespace policy
