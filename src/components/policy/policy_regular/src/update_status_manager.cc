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

namespace policy {

CREATE_LOGGERPTR_GLOBAL(logger_, "Policy")

UpdateStatusManager::UpdateStatusManager()
    : listener_(NULL)
    , current_status_(std::make_shared<UpToDateStatus>())
    , last_processed_event_(kNoEvent)
    , apps_search_in_progress_(false)
    , app_registered_from_non_consented_device_(true) {}

UpdateStatusManager::~UpdateStatusManager() {}

void UpdateStatusManager::ProcessEvent(UpdateEvent event) {
  sync_primitives::AutoLock lock(status_lock_);
  current_status_->ProcessEvent(this, event);
  last_processed_event_ = event;
  DoTransition();
}

void UpdateStatusManager::SetNextStatus(std::shared_ptr<Status> status) {
  next_status_ = status;
}

void UpdateStatusManager::SetPostponedStatus(std::shared_ptr<Status> status) {
  postponed_status_ = status;
}

void UpdateStatusManager::set_listener(PolicyListener* listener) {
  listener_ = listener;
}

void UpdateStatusManager::OnUpdateSentOut() {
  LOG4CXX_AUTO_TRACE(logger_);
  ProcessEvent(kOnUpdateSentOut);
}

void UpdateStatusManager::OnUpdateTimeoutOccurs() {
  LOG4CXX_AUTO_TRACE(logger_);
  ProcessEvent(kOnUpdateTimeout);
}

void UpdateStatusManager::OnValidUpdateReceived() {
  LOG4CXX_AUTO_TRACE(logger_);
  ProcessEvent(kOnValidUpdateReceived);
}

void UpdateStatusManager::OnWrongUpdateReceived() {
  LOG4CXX_AUTO_TRACE(logger_);
  ProcessEvent(kOnWrongUpdateReceived);
}

void UpdateStatusManager::OnResetDefaultPT(bool is_update_required) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (is_update_required) {
    ProcessEvent(kOnResetPolicyTableRequireUpdate);
    return;
  }
  ProcessEvent(kOnResetPolicyTableNoUpdate);
}

void UpdateStatusManager::OnResetRetrySequence() {
  LOG4CXX_AUTO_TRACE(logger_);
  ProcessEvent(kOnResetRetrySequence);
}

void UpdateStatusManager::OnExistedApplicationAdded(
    const bool is_update_required) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (is_update_required) {
    current_status_.reset(new UpToDateStatus());
    ProcessEvent(kScheduleUpdate);
  }
}

void UpdateStatusManager::OnNewApplicationAdded(const DeviceConsent consent) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (kDeviceAllowed != consent) {
    app_registered_from_non_consented_device_ = true;
    return;
  }
  app_registered_from_non_consented_device_ = false;
  ProcessEvent(kOnNewAppRegistered);
}

void UpdateStatusManager::OnDeviceConsented() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (app_registered_from_non_consented_device_) {
    ProcessEvent(kOnNewAppRegistered);
  }
}

bool UpdateStatusManager::IsUpdateRequired() const {
  return current_status_->IsUpdateRequired();
}

bool UpdateStatusManager::IsUpdatePending() const {
  return current_status_->IsUpdatePending();
}

void UpdateStatusManager::ScheduleUpdate() {
  ProcessEvent(kScheduleUpdate);
}

void UpdateStatusManager::ScheduleManualUpdate() {
  ProcessEvent(kScheduleManualUpdate);
}

std::string UpdateStatusManager::StringifiedUpdateStatus() const {
  return current_status_->get_status_string();
}

void policy::UpdateStatusManager::OnAppsSearchStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(apps_search_in_progress_lock_);
  apps_search_in_progress_ = true;
}

void policy::UpdateStatusManager::OnAppsSearchCompleted() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(apps_search_in_progress_lock_);
  apps_search_in_progress_ = false;
}

bool policy::UpdateStatusManager::IsAppsSearchInProgress() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(apps_search_in_progress_lock_);
  return apps_search_in_progress_;
}

void UpdateStatusManager::DoTransition() {
  DCHECK_OR_RETURN_VOID(listener_);
  if (!next_status_) {
    return;
  }

  current_status_ = next_status_;
  next_status_.reset();
  LOG4CXX_DEBUG(logger_, "last_processed_event_ = " << last_processed_event_);
  if (last_processed_event_ != kScheduleManualUpdate) {
    listener_->OnUpdateStatusChanged(current_status_->get_status_string());
  }
  if (!postponed_status_) {
    return;
  }

  current_status_ = postponed_status_;
  if (last_processed_event_ != kScheduleManualUpdate) {
    listener_->OnUpdateStatusChanged(current_status_->get_status_string());
  }
  postponed_status_.reset();
}

}  // namespace policy
