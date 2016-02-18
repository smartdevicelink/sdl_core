#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_H
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_H

#include "policy/policy_types.h"
#include "utils/lock.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/conditional_variable.h"
#include "utils/lock.h"
#include "utils/logger.h"
#include "utils/macro.h"

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
   * @brief Update status handler on new application registering
   */
  void OnNewApplicationAdded();

  /**
   * @brief Update status handler for policy initialization
   * @param is_update_required Update necessity flag
   */
  void OnPolicyInit(bool is_update_required);

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
   * @brief ResetUpdateSchedule allows to reset all scheduled updates.
   */
  void ResetUpdateSchedule();

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
      return GetUpdateStatus();
  }
#endif // BUILD_TESTS

private:
  /*
   * @brief Sets flag for update progress
   *
   * @param value
   */
  void set_exchange_in_progress(bool value);

  /*
   * @brief Sets flag for pending update
   *
   * @param value
   */
  void set_exchange_pending(bool value);

  /*
   * @brief Sets flag for update necessity
   *
   * @param value
   */
  void set_update_required(bool value);

  /**
   * @brief Check update status and notify HMI on changes
   */
  void CheckUpdateStatus();

private:

  /**
   * @brief Returns current policy update status
   * @return
   */
  PolicyTableStatus GetUpdateStatus() const;

  PolicyListener* listener_;
  bool exchange_in_progress_;
  bool update_required_;
  bool update_scheduled_;
  bool exchange_pending_;
  bool apps_search_in_progress_;
  sync_primitives::Lock exchange_in_progress_lock_;
  sync_primitives::Lock update_required_lock_;
  sync_primitives::Lock exchange_pending_lock_;
  sync_primitives::Lock apps_search_in_progress_lock_;
  /**
   * @brief Last status of policy table update
   */
  PolicyTableStatus last_update_status_;

  class UpdateThreadDelegate: public threads::ThreadDelegate {

  public:
    UpdateThreadDelegate(UpdateStatusManager* update_status_manager);
    ~UpdateThreadDelegate();
    virtual void threadMain();
    virtual void exitThreadMain();
    void updateTimeOut(const uint32_t timeout_ms);

    volatile uint32_t                                timeout_;
    volatile bool                                    stop_flag_;
    sync_primitives::Lock                            state_lock_;
    sync_primitives::ConditionalVariable             termination_condition_;
    UpdateStatusManager*                             update_status_manager_;
  };

  UpdateThreadDelegate*                              update_status_thread_delegate_;
  threads::Thread*                                   thread_;
};

}

#endif // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_H
