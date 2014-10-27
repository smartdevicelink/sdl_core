#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_H
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_H

#include "policy/update_status_manager_interface.h"
#include "policy/policy_types.h"
#include "utils/lock.h"
#include "utils/timer_thread.h"

namespace policy {

class PolicyListener;

class UpdateStatusManager : public UpdateStatusManagerInterface {
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
   * @brief Returns current policy update status
   * @return
   */
  PolicyTableStatus GetUpdateStatus();

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
  PolicyListener* listener_;
  bool exchange_in_progress_;
  bool update_required_;
  bool exchange_pending_;
  sync_primitives::Lock exchange_in_progress_lock_;
  sync_primitives::Lock update_required_lock_;
  sync_primitives::Lock exchange_pending_lock_;
  /**
   * @brief Last status of policy table update
   */
  PolicyTableStatus last_update_status_;


  /**
   * @brief The Policy update response timer class
   */
  class UpdateResponseTimer : public timer::TimerThread<UpdateStatusManager> {
   public:
    UpdateResponseTimer(UpdateStatusManager* callee) :
        timer::TimerThread<UpdateStatusManager>(
          "Policy UpdResponse",
          callee,
          &UpdateStatusManager::OnUpdateTimeoutOccurs) {
    }
    ~UpdateResponseTimer();
  };
  UpdateResponseTimer update_response_timer_;
};

}

#endif // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_UPDATE_STATUS_MANAGER_H
