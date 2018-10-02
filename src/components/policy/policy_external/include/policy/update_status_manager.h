/*
 Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_H_
#define SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_H_

#include "policy/policy_types.h"
#include "utils/lock.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/conditional_variable.h"
#include "utils/lock.h"
#include "utils/logger.h"
#include "utils/macro.h"
#include "policy/status.h"

namespace policy {

class PolicyListener;

class UpdateStatusManager {
 public:
  /**
   * @brief Constructor
   */
  UpdateStatusManager();

  ~UpdateStatusManager();

  /**
   * @brief Process event by current status implementations
   * @param event Event
   */
  void ProcessEvent(UpdateEvent event);

  /**
   * @brief Set next status during event processing
   * @param status Status shared pointer
   */
  void SetNextStatus(std::shared_ptr<Status> status);

  /**
   * @brief Set postponed status (will be set after next status) during event
   * processing
   * @param status Status shared pointer
   */
  void SetPostponedStatus(std::shared_ptr<Status> status);

  /**
   * @brief Sets listener pointer
   * @param listener Pointer to policy listener implementation
   */
  void set_listener(PolicyListener* listener);

  /**
   * @brief Update status hanlder for PTS sending out
   * @param update_timeout Timeout for waiting of incoming PTU
   */
  void OnUpdateSentOut(uint32_t update_timeout);

  /**
   * @brief Update status handler for PTU waiting timeout
   */
  void OnUpdateTimeoutOccurs();

  /**
   * @brief Update status handler for valid PTU receiving
   */
  void OnValidUpdateReceived();

  /**
   * @brief Update status handler for wrong PTU receiving
   */
  void OnWrongUpdateReceived();

  /**
   * @brief Update status handler for reset PT to default state
   * @param is_update_required Update necessity flag
   */
  void OnResetDefaultPT(bool is_update_required);

  /**
   * @brief Update status handler for restarting retry sequence
   */
  void OnResetRetrySequence();

  /**
   * @brief Update status handler on existed application registering
   * @param is_update_required Update necessity flag
   */
  void OnExistedApplicationAdded(const bool is_update_required);

  /**
   * @brief Update status handler on new application registering
   */
  void OnNewApplicationAdded(const DeviceConsent consent);

  /**
   * @brief In case application from non-consented device has been registered
   * before and and no updated happened then triggers status change
   */
  void OnDeviceConsented();

  /**
   * @brief IsUpdateRequired allows to distiguish if update is required
   *
   * @return  true if update required.
   */
  bool IsUpdateRequired() const;

  /**
   * @brief IsUpdatePending allows to distinguish if update is in pending mode.
   *
   * @return true if update is in pending mode.
   */
  bool IsUpdatePending() const;

  /**
   * @brief ScheduleUpdate allows to schedule next update.
   * It will change state to Update_Needed, that's is.
   */
  void ScheduleUpdate();

  /**
   * @brief ScheduleUpdate allows to schedule next update.
   * It will change state to Update_Needed, that's is
   * and will not send any notifications about updating to HMI
   */
  void ScheduleManualUpdate();

  /**
   * @brief StringifiedUpdateStatus allows to obtain update status as a string.
   *
   * @return stringified update status.
   */
  std::string StringifiedUpdateStatus() const;

  /**
   * @brief Status handler on applications search started
   */
  void OnAppsSearchStarted();

  /**
   * @brief Status handler on applications search completed
   */
  void OnAppsSearchCompleted();

  /**
   * @brief Returns status is application search in progress
   * @return true, if in progress, otherwise - false
   */
  bool IsAppsSearchInProgress();

#ifdef BUILD_TESTS
  PolicyTableStatus GetLastUpdateStatus() const {
    return current_status_->get_status();
  }
#endif  // BUILD_TESTS

 private:
  /**
   * @brief Does statuses transitions after event handling and notifies the
   * system
   */
  void DoTransition();

 private:
  PolicyListener* listener_;

  /**
   * @brief Current update status
   */
  std::shared_ptr<Status> current_status_;

  /**
   * @brief Next status after current to be set
   */
  std::shared_ptr<Status> next_status_;

  /**
   * @brief Status to be set after 'next' status
   */
  std::shared_ptr<Status> postponed_status_;
  sync_primitives::Lock status_lock_;

  UpdateEvent last_processed_event_;
  bool apps_search_in_progress_;
  bool app_registered_from_non_consented_device_;
  sync_primitives::Lock apps_search_in_progress_lock_;

  class UpdateThreadDelegate : public threads::ThreadDelegate {
   public:
    UpdateThreadDelegate(UpdateStatusManager* update_status_manager);
    ~UpdateThreadDelegate();
    virtual void threadMain();
    virtual void exitThreadMain();
    void updateTimeOut(const uint32_t timeout_ms);

    volatile uint32_t timeout_;
    volatile bool stop_flag_;
    sync_primitives::RecursiveLock state_lock_;
    sync_primitives::ConditionalVariable termination_condition_;
    UpdateStatusManager* update_status_manager_;
  };

  UpdateThreadDelegate* update_status_thread_delegate_;
  threads::Thread* thread_;
};
}

#endif  // SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_H_
