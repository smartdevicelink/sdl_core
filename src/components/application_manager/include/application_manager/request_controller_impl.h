/*
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_CONTROLLER_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_CONTROLLER_IMPL_H_

#include <climits>
#include <list>
#include <vector>

#include "utils/lock.h"

#include "utils/conditional_variable.h"
#include "utils/threads/async_runner.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/timer.h"

#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

#include "application_manager/event_engine/event_dispatcher_impl.h"
#include "application_manager/request_controller.h"
#include "application_manager/request_controller_settings.h"
#include "application_manager/request_info.h"
#include "application_manager/request_tracker.h"

namespace application_manager {

namespace request_controller {

class RequestControllerImpl : public RequestController, threads::AsyncRunner {
 public:
  /**
   * @brief Class constructor
   *
   */
  RequestControllerImpl(const RequestControlerSettings& settings,
                        RequestTimeoutHandler& request_timeout_handler,
                        event_engine::EventDispatcher& event_disptacher);

  ~RequestControllerImpl();

  void Stop() OVERRIDE;

  void InitializeThreadpool() OVERRIDE;

  void DestroyThreadpool() OVERRIDE;

  TResult AddMobileRequest(
      const RequestPtr request,
      const mobile_apis::HMILevel::eType& hmi_level) OVERRIDE;

  TResult AddHMIRequest(const RequestPtr request) OVERRIDE;

  void AddNotification(const RequestPtr ptr) OVERRIDE;

  bool RetainRequestInstance(const uint32_t connection_key,
                             const uint32_t correlation_id) OVERRIDE;

  bool RemoveRetainedRequest(const uint32_t connection_key,
                             const uint32_t correlation_id) OVERRIDE;

  bool IsStillWaitingForResponse(const uint32_t connection_key,
                                 const uint32_t correlation_id) const OVERRIDE;

  /**
   * @brief Removes request from queue
   *
   * @param correlation_id Active request correlation ID,
   * @param connection_key Active request connection key (0 for HMI requersts)
   * @param function_id Active request  function id
   * @param force_terminate if true, request controller will terminate
   * even if not allowed by request
   */

  void TerminateRequest(const uint32_t correlation_id,
                        const uint32_t connection_key,
                        const int32_t function_id,
                        const bool force_terminate = false) OVERRIDE;

  void OnMobileResponse(const uint32_t mobile_correlation_id,
                        const uint32_t connection_key,
                        const int32_t function_id) OVERRIDE;

  void OnHMIResponse(const uint32_t correlation_id,
                     const int32_t function_id) OVERRIDE;

  void RemoveNotification(const commands::Command* notification) OVERRIDE;

  void TerminateAppRequests(const uint32_t app_id) OVERRIDE;

  void TerminateAllHMIRequests() OVERRIDE;

  void TerminateAllMobileRequests() OVERRIDE;

  void UpdateRequestTimeout(const uint32_t app_id,
                            const uint32_t mobile_correlation_id,
                            const uint32_t new_timeout) OVERRIDE;

  void OnLowVoltage() OVERRIDE;

  void OnWakeUp() OVERRIDE;

  bool IsLowVoltage() OVERRIDE;

 protected:
  /**
   * @brief Timer callback which handles all request timeouts
   */
  void TimeoutThread();

  /**
   * @brief Signal timer condition variable
   */
  void NotifyTimer();

  void TerminateWaitingForExecutionAppRequests(const uint32_t app_id);
  void TerminateWaitingForResponseAppRequests(const uint32_t app_id);

  /**
   * @brief Starts a new async task for cleaning up the provided requests
   * references
   * and perform the rest of cleanup actions for each request
   * @param requests list of requests to cleanup
   */
  typedef std::list<RequestInfoPtr> RequestInfoPtrs;
  void scheduleRequestsCleanup(const RequestInfoPtrs& requests);

  /**
   * @brief Checks whether all constraints are met before adding of request into
   * processing queue. Verifies amount of pending requests, amount of requests
   * per time scale for different HMI levels
   * @param request - request to check constraints for
   * @param level - HMI level in which request has been issued
   * @return Appropriate result code of verification
   */
  TResult CheckPosibilitytoAdd(const RequestPtr request,
                               const mobile_api::HMILevel::eType level);

  /**
   * @brief Check Posibility to add new requests, or limits was exceeded
   * @param pending_requests_amount - maximum count of request that should be
   * allowed for all applications
   * @return True if new request could be added, false otherwise
   */
  bool CheckPendingRequestsAmount(const uint32_t pending_requests_amount);

 private:
  class Worker : public threads::ThreadDelegate {
   public:
    explicit Worker(RequestControllerImpl* request_controller);
    virtual ~Worker();
    virtual void threadMain();
    virtual void exitThreadMain();

   protected:
   private:
    RequestControllerImpl* request_controller_;
    sync_primitives::Lock thread_lock_;
    volatile bool stop_flag_;
  };

  class RequestCleanerDelegate : public threads::ThreadDelegate {
   public:
    explicit RequestCleanerDelegate(const RequestInfoPtrs& requests);
    ~RequestCleanerDelegate();
    void threadMain() OVERRIDE;
    void exitThreadMain() OVERRIDE;

   private:
    sync_primitives::Lock state_lock_;
    sync_primitives::ConditionalVariable state_cond_;
    RequestInfoPtrs requests_;
  };

  std::vector<threads::Thread*> pool_;
  volatile TPoolState pool_state_;
  uint32_t pool_size_;
  sync_primitives::ConditionalVariable cond_var_;

  std::list<RequestPtr> mobile_request_list_;
  sync_primitives::Lock mobile_request_list_lock_;

  /**
   * @brief Requests, that are waiting for responses
   * RequestInfoSet provides correct processing of requests with thre same
   * app_id and corr_id
   */
  RequestInfoSet waiting_for_response_;

  /**
   * @brief Requests, that are retained to be not destroyed right after
   * sending response to mobile request
   */
  HashSortedRequestInfoSet retained_mobile_requests_;

  /**
   * @brief Tracker verifying time scale and maximum requests amount in
   * different HMI levels
   */
  RequestTracker request_tracker_;

  /**
   * @brief Set of HMI notifications with timeout.
   */
  std::list<RequestPtr> notification_list_;
  sync_primitives::Lock notification_list_lock_;

  /**
   * @brief Map keeping track of how many duplicate messages were sent for a
   * given correlation id, to prevent early termination of a request
   */
  std::map<uint32_t, uint32_t> duplicate_message_count_;
  sync_primitives::Lock duplicate_message_count_lock_;

  /*
   * timer for checking requests timeout
   */
  timer::Timer timer_;

  /*
   * Timer for lock
   */
  bool timer_stop_flag_;
  sync_primitives::Lock timer_lock;
  sync_primitives::ConditionalVariable timer_condition_;

  bool is_low_voltage_;
  const RequestControlerSettings& settings_;

  RequestTimeoutHandler& request_timeout_handler_;
  event_engine::EventDispatcher& event_dispatcher_;
  DISALLOW_COPY_AND_ASSIGN(RequestControllerImpl);
};

}  // namespace request_controller

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_CONTROLLER_IMPL_H_
