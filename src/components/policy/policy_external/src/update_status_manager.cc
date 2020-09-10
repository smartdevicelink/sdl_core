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
#include "policy/ptu_retry_handler.h"
#include "utils/logger.h"

namespace policy {

SDL_CREATE_LOG_VARIABLE("Policy")

UpdateStatusManager::UpdateStatusManager()
    : listener_(NULL)
    , current_status_(std::make_shared<UpToDateStatus>())
    , last_processed_event_(kNoEvent)
    , apps_search_in_progress_(false)
    , app_registered_from_non_consented_device_(true) {
  update_status_thread_delegate_ = new UpdateThreadDelegate(this);
  thread_ = threads::CreateThread("UpdateStatusThread",
                                  update_status_thread_delegate_);
  thread_->Start();
}

UpdateStatusManager::~UpdateStatusManager() {
  SDL_LOG_AUTO_TRACE();
  DCHECK(update_status_thread_delegate_);
  DCHECK(thread_);
  thread_->Stop(threads::Thread::kThreadSoftStop);
  delete update_status_thread_delegate_;
  threads::DeleteThread(thread_);
}

void UpdateStatusManager::ProcessEvent(UpdateEvent event) {
  SDL_LOG_AUTO_TRACE();
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

void UpdateStatusManager::OnUpdateSentOut(uint32_t update_timeout) {
  SDL_LOG_AUTO_TRACE();
  DCHECK(update_status_thread_delegate_);
  update_status_thread_delegate_->updateTimeOut(update_timeout);
  ProcessEvent(kOnUpdateSentOut);
}

void UpdateStatusManager::OnUpdateTimeoutOccurs() {
  SDL_LOG_AUTO_TRACE();

  auto& ptu_retry_handler = listener_->ptu_retry_handler();

  if (ptu_retry_handler.IsAllowedRetryCountExceeded()) {
    ptu_retry_handler.RetrySequenceFailed();
    return;
  }

  ProcessEvent(kOnUpdateTimeout);
  DCHECK(update_status_thread_delegate_);
  update_status_thread_delegate_->updateTimeOut(0);  // Stop Timer
}

void UpdateStatusManager::OnValidUpdateReceived() {
  SDL_LOG_AUTO_TRACE();
  update_status_thread_delegate_->updateTimeOut(0);  // Stop Timer
  ProcessEvent(kOnValidUpdateReceived);
}

void UpdateStatusManager::ResetTimeout(uint32_t update_timeout) {
  SDL_LOG_AUTO_TRACE();
  update_status_thread_delegate_->updateTimeOut(
      update_timeout);  // Restart Timer
}

void UpdateStatusManager::OnWrongUpdateReceived() {
  SDL_LOG_AUTO_TRACE();
  update_status_thread_delegate_->updateTimeOut(0);  // Stop Timer
  ProcessEvent(kOnWrongUpdateReceived);
}

void UpdateStatusManager::OnResetDefaultPT(bool is_update_required) {
  SDL_LOG_AUTO_TRACE();
  if (is_update_required) {
    ProcessEvent(kOnResetPolicyTableRequireUpdate);
    return;
  }
  ProcessEvent(kOnResetPolicyTableNoUpdate);
}

void UpdateStatusManager::OnResetRetrySequence() {
  SDL_LOG_AUTO_TRACE();
  ProcessEvent(kOnResetRetrySequence);
}

void UpdateStatusManager::OnExistedApplicationAdded(
    const bool is_update_required) {
  SDL_LOG_AUTO_TRACE();
  if (is_update_required) {
    current_status_.reset(new UpToDateStatus());
    ProcessEvent(kScheduleUpdate);
  }
}

void UpdateStatusManager::OnNewApplicationAdded(const DeviceConsent consent) {
  SDL_LOG_AUTO_TRACE();
  if (kDeviceAllowed != consent) {
    SDL_LOG_DEBUG("Application registered from non-consented device");
    app_registered_from_non_consented_device_ = true;
    return;
  }
  SDL_LOG_DEBUG("Application registered from consented device");
  app_registered_from_non_consented_device_ = false;
  if (kOnResetRetrySequence == last_processed_event_) {
    current_status_.reset(new UpToDateStatus());
    ProcessEvent(kScheduleUpdate);
  } else {
    ProcessEvent(kOnNewAppRegistered);
  }
}

void UpdateStatusManager::OnDeviceConsented() {
  SDL_LOG_AUTO_TRACE();
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
  SDL_LOG_AUTO_TRACE();
  ProcessEvent(kScheduleUpdate);
}

void UpdateStatusManager::PendingUpdate() {
  SDL_LOG_AUTO_TRACE();
  ProcessEvent(kPendingUpdate);
}

void UpdateStatusManager::ScheduleManualUpdate() {
  SDL_LOG_AUTO_TRACE();
  ProcessEvent(kScheduleManualUpdate);
}

std::string UpdateStatusManager::StringifiedUpdateStatus() const {
  return current_status_->get_status_string();
}

void policy::UpdateStatusManager::OnAppsSearchStarted() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(apps_search_in_progress_lock_);
  apps_search_in_progress_ = true;
}

void policy::UpdateStatusManager::OnAppsSearchCompleted() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(apps_search_in_progress_lock_);
  apps_search_in_progress_ = false;
}

bool policy::UpdateStatusManager::IsAppsSearchInProgress() {
  SDL_LOG_AUTO_TRACE();
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

  const bool is_update_pending =
      policy::StatusProcessingSnapshot == current_status_->get_status();

  if (last_processed_event_ != kScheduleManualUpdate && !is_update_pending) {
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

UpdateStatusManager::UpdateThreadDelegate::UpdateThreadDelegate(
    UpdateStatusManager* update_status_manager)
    : timeout_(0)
    , stop_flag_(false)
    , update_status_manager_(update_status_manager) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Create UpdateThreadDelegate");
}

UpdateStatusManager::UpdateThreadDelegate::~UpdateThreadDelegate() {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Delete UpdateThreadDelegate");
}

void UpdateStatusManager::UpdateThreadDelegate::threadMain() {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("UpdateStatusManager thread started (started normal)");
  sync_primitives::AutoLock auto_lock(state_lock_);
  while (false == stop_flag_) {
    if (timeout_ > 0) {
      SDL_LOG_DEBUG("Timeout is greater then 0");
      sync_primitives::ConditionalVariable::WaitStatus wait_status =
          termination_condition_.WaitFor(auto_lock, timeout_);
      if (sync_primitives::ConditionalVariable::kTimeout == wait_status) {
        if (update_status_manager_) {
          update_status_manager_->OnUpdateTimeoutOccurs();
        }
      }
    } else {
      // Time is not active, wait until timeout will be set,
      // or UpdateStatusManager will be deleted
      termination_condition_.Wait(auto_lock);
    }
  }
}

void UpdateStatusManager::UpdateThreadDelegate::exitThreadMain() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(state_lock_);
  stop_flag_ = true;
  SDL_LOG_DEBUG("before notify");
  termination_condition_.NotifyOne();
}

void UpdateStatusManager::UpdateThreadDelegate::updateTimeOut(
    const uint32_t timeout_ms) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(state_lock_);
  timeout_ = timeout_ms;
  termination_condition_.NotifyOne();
}

}  // namespace policy
