/*
 * Copyright (c) 2016, Ford Motor Company
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

#include "utils/logger.h"
#include "application_manager/request_controller.h"
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/commands/hmi/request_to_hmi.h"
#include "utils/make_shared.h"
#include "utils/timer_task_impl.h"

#if defined(_MSC_VER)
#define snprintf _snprintf_s
#endif

namespace application_manager {

namespace request_controller {

#if defined(_MSC_VER)
#define snprintf _snprintf_s
#endif

using namespace sync_primitives;

SDL_CREATE_LOGGER("RequestController")

RequestController::RequestController(const RequestControlerSettings& settings)
    : pool_state_(UNDEFINED)
    , pool_size_(settings.thread_pool_size())
    , timer_("AM RequestCtrlTimer",
             new timer::TimerTaskImpl<RequestController>(
                 this, &RequestController::onTimer))
    , is_low_voltage_(false)
    , settings_(settings) {
  SDL_AUTO_TRACE();
  InitializeThreadpool();
}

RequestController::~RequestController() {
  SDL_AUTO_TRACE();
  if (pool_state_ != TPoolState::STOPPED) {
    DestroyThreadpool();
  }
}

void RequestController::InitializeThreadpool() {
  SDL_AUTO_TRACE();
  // TODO(DK): Consider lazy loading threads instead of creating all at once
  pool_state_ = TPoolState::STARTED;
  char name[50];
  for (uint32_t i = 0; i < pool_size_; i++) {
    snprintf(name, sizeof(name) / sizeof(name[0]), "AM Pool %d", i);
    pool_.push_back(threads::CreateThread(name, new Worker(this)));
    pool_[i]->start();
    SDL_DEBUG("Request thread initialized: " << name);
  }
}

void RequestController::DestroyThreadpool() {
  SDL_AUTO_TRACE();
  {
    AutoLock auto_lock(mobile_request_list_lock_);
    pool_state_ = TPoolState::STOPPED;
    SDL_DEBUG("Broadcasting STOP signal to all threads...");
    cond_var_.Broadcast();  // notify all threads we are shutting down
  }
  for (size_t i = 0; i < pool_.size(); i++) {
    threads::Thread* thread = pool_[i];
    thread->join();
    delete thread->delegate();
    threads::DeleteThread(thread);
  }
  pool_.clear();
}

RequestController::TResult RequestController::CheckPosibilitytoAdd(
    const RequestPtr request) {
  SDL_AUTO_TRACE();
  const uint32_t& app_hmi_level_none_time_scale =
      settings_.app_hmi_level_none_time_scale();

  // app_hmi_level_none_max_request_per_time_scale
  const uint32_t& hmi_level_none_count =
      settings_.app_hmi_level_none_time_scale_max_requests();

  const uint32_t& app_time_scale = settings_.app_time_scale();

  const uint32_t& max_request_per_time_scale =
      settings_.app_time_scale_max_requests();

  const uint32_t& pending_requests_amount = settings_.pending_requests_amount();

  if (!CheckPendingRequestsAmount(pending_requests_amount)) {
    SDL_ERROR("Too many pending request");
    return RequestController::TOO_MANY_PENDING_REQUESTS;
  }

  if (!waiting_for_response_.CheckHMILevelTimeScaleMaxRequest(
          mobile_apis::HMILevel::HMI_NONE,
          request->connection_key(),
          app_hmi_level_none_time_scale,
          hmi_level_none_count)) {
    SDL_ERROR("Too many application requests in hmi level NONE");
    return RequestController::NONE_HMI_LEVEL_MANY_REQUESTS;
  }
  if (!waiting_for_response_.CheckTimeScaleMaxRequest(
          request->connection_key(),
          app_time_scale,
          max_request_per_time_scale)) {
    SDL_ERROR("Too many application requests");
    return RequestController::TOO_MANY_REQUESTS;
  }
  return SUCCESS;
}

bool RequestController::CheckPendingRequestsAmount(
    const uint32_t& pending_requests_amount) {
  SDL_AUTO_TRACE();
  if (pending_requests_amount > 0) {
    const size_t pending_requests_size = mobile_request_list_.size();
    const bool available_to_add =
        pending_requests_amount > pending_requests_size;
    if (!available_to_add) {
      SDL_WARN("Pending requests count " << pending_requests_size
                                         << " exceed application limit "
                                         << pending_requests_amount);
    }
    return available_to_add;
  }
  SDL_DEBUG("CheckPendingRequestsAmount disabled");
  return true;
}

RequestController::TResult RequestController::addMobileRequest(
    const RequestPtr request, const mobile_apis::HMILevel::eType& hmi_level) {
  SDL_AUTO_TRACE();
  if (!request) {
    SDL_ERROR("Null Pointer request");
    cond_var_.NotifyOne();
    return INVALID_DATA;
  }
  SDL_DEBUG("correlation_id : " << request->correlation_id()
                                << "connection_key : "
                                << request->connection_key());
  RequestController::TResult result = CheckPosibilitytoAdd(request);
  if (SUCCESS == result) {
    AutoLock auto_lock_list(mobile_request_list_lock_);
    mobile_request_list_.push_back(request);
    SDL_DEBUG("Waiting for execution: " << mobile_request_list_.size());
    // wake up one thread that is waiting for a task to be available
  }
  cond_var_.NotifyOne();
  return result;
}

RequestController::TResult RequestController::addHMIRequest(
    const RequestPtr request) {
  SDL_AUTO_TRACE();

  if (!request.valid()) {
    SDL_ERROR("HMI request pointer is invalid");
    return RequestController::INVALID_DATA;
  }
  SDL_DEBUG(" correlation_id : " << request->correlation_id());

  const uint64_t timeout_in_mseconds =
      static_cast<uint64_t>(request->default_timeout());
  RequestInfoPtr request_info_ptr(
      new HMIRequestInfo(request, timeout_in_mseconds));

  if (0 == timeout_in_mseconds) {
    SDL_DEBUG(
        "Default timeout was set to 0."
        "RequestController will not track timeout of this request.");
  }
  waiting_for_response_.Add(request_info_ptr);
  SDL_DEBUG("Waiting for response count:" << waiting_for_response_.Size());

  UpdateTimer();
  return RequestController::SUCCESS;
}

void RequestController::addNotification(const RequestPtr ptr) {
  SDL_AUTO_TRACE();
  notification_list_.push_back(ptr);
}

void RequestController::removeNotification(
    const commands::Command* notification) {
  SDL_AUTO_TRACE();
  std::list<RequestPtr>::iterator it = notification_list_.begin();
  for (; notification_list_.end() != it;) {
    if (it->get() == notification) {
      notification_list_.erase(it++);
      SDL_DEBUG("Notification removed");
      break;
    } else {
      ++it;
    }
  }
  SDL_DEBUG("Cant find notification");
}

void RequestController::terminateRequest(const uint32_t& correlation_id,
                                         const uint32_t& connection_key,
                                         bool force_terminate) {
  SDL_AUTO_TRACE();
  SDL_DEBUG("correlation_id = " << correlation_id
                                << " connection_key = " << connection_key
                                << " force_terminate = " << force_terminate);
  RequestInfoPtr request =
      waiting_for_response_.Find(connection_key, correlation_id);
  if (request) {
    if (force_terminate || request->request()->AllowedToTerminate()) {
      waiting_for_response_.RemoveRequest(request);
    } else {
      SDL_WARN("Request was not terminated");
    }
    UpdateTimer();
  } else {
    SDL_WARN("Request not found in waiting_for_response_");
  }
}

void RequestController::OnMobileResponse(const uint32_t& mobile_correlation_id,
                                         const uint32_t& connection_key) {
  SDL_AUTO_TRACE();
  terminateRequest(mobile_correlation_id, connection_key);
}

void RequestController::OnHMIResponse(const uint32_t& correlation_id) {
  SDL_AUTO_TRACE();
  terminateRequest(correlation_id, RequestInfo::HmiConnectoinKey);
}

void RequestController::terminateWaitingForExecutionAppRequests(
    const uint32_t& app_id) {
  SDL_AUTO_TRACE();
  SDL_DEBUG("app_id: " << app_id << "Waiting for execution"
                       << mobile_request_list_.size());
  AutoLock auto_lock(mobile_request_list_lock_);
  std::list<RequestPtr>::iterator request_it = mobile_request_list_.begin();
  while (mobile_request_list_.end() != request_it) {
    RequestPtr request = (*request_it);
    if ((request.valid()) && (request->connection_key() == app_id)) {
      mobile_request_list_.erase(request_it++);
    } else {
      ++request_it;
    }
  }
  SDL_DEBUG("Waiting for execution " << mobile_request_list_.size());
}

void RequestController::terminateWaitingForResponseAppRequests(
    const uint32_t& app_id) {
  SDL_AUTO_TRACE();
  waiting_for_response_.RemoveByConnectionKey(app_id);
  SDL_DEBUG("Waiting for response count : " << waiting_for_response_.Size());
}

void RequestController::terminateAppRequests(const uint32_t& app_id) {
  SDL_AUTO_TRACE();
  SDL_DEBUG("app_id : " << app_id << "Requests waiting for execution count : "
                        << mobile_request_list_.size()
                        << "Requests waiting for response count : "
                        << waiting_for_response_.Size());

  terminateWaitingForExecutionAppRequests(app_id);
  terminateWaitingForResponseAppRequests(app_id);
  UpdateTimer();
}

void RequestController::terminateAllHMIRequests() {
  SDL_AUTO_TRACE();
  terminateWaitingForResponseAppRequests(RequestInfo::HmiConnectoinKey);
}

void RequestController::terminateAllMobileRequests() {
  SDL_AUTO_TRACE();
  waiting_for_response_.RemoveMobileRequests();
  SDL_DEBUG("Mobile Requests waiting for response cleared");
  AutoLock waiting_execution_auto_lock(mobile_request_list_lock_);
  mobile_request_list_.clear();
  SDL_DEBUG("Mobile Requests waiting for execution cleared");
  UpdateTimer();
}

void RequestController::updateRequestTimeout(const uint32_t& app_id,
                                             const uint32_t& correlation_id,
                                             const uint32_t& new_timeout) {
  SDL_AUTO_TRACE();

  SDL_DEBUG("app_id : " << app_id << " mobile_correlation_id : "
                        << correlation_id << " new_timeout : " << new_timeout);
  SDL_DEBUG(
      "New_timeout is NULL. RequestCtrl will "
      "not manage this request any more");

  RequestInfoPtr request_info =
      waiting_for_response_.Find(app_id, correlation_id);
  if (request_info) {
    waiting_for_response_.RemoveRequest(request_info);
    request_info->updateTimeOut(new_timeout);
    waiting_for_response_.Add(request_info);
    UpdateTimer();
    SDL_INFO("Timeout updated for "
             << " app_id: " << app_id << " correlation_id: " << correlation_id
             << " new_timeout (ms): " << new_timeout);
  } else {
    SDL_ERROR("Can't find request with "
              << " app_id: " << app_id
              << " correlation_id: " << correlation_id);
  }
}

void RequestController::OnLowVoltage() {
  SDL_AUTO_TRACE();
  is_low_voltage_ = true;
}

void RequestController::OnWakeUp() {
  SDL_AUTO_TRACE();
  terminateAllHMIRequests();
  terminateAllMobileRequests();
  is_low_voltage_ = false;
  SDL_DEBUG("Terminate old requests done");
}

bool RequestController::IsLowVoltage() {
  SDL_TRACE("result: " << is_low_voltage_);
  return is_low_voltage_;
}

void RequestController::onTimer() {
  SDL_AUTO_TRACE();
  SDL_DEBUG(
      "ENTER Waiting fore response count: " << waiting_for_response_.Size());
  RequestInfoPtr probably_expired =
      waiting_for_response_.FrontWithNotNullTimeout();
  while (probably_expired && probably_expired->isExpired()) {
    SDL_INFO("Timeout for "
             << (RequestInfo::HMIRequest == probably_expired->requst_type()
                     ? "HMI"
                     : "Mobile")
             << " request id: " << probably_expired->requestId()
             << " connection_key: " << probably_expired->app_id()
             << " is expired");
    const uint32_t experied_request_id = probably_expired->requestId();
    const uint32_t experied_app_id = probably_expired->app_id();

    probably_expired->request()->onTimeOut();
    if (RequestInfo::HmiConnectoinKey == probably_expired->app_id()) {
      SDL_DEBUG("Erase HMI request: " << probably_expired->requestId());
      waiting_for_response_.RemoveRequest(probably_expired);
    }
    probably_expired = waiting_for_response_.FrontWithNotNullTimeout();
    if (probably_expired) {
      if (experied_request_id == probably_expired->requestId() &&
          experied_app_id == probably_expired->app_id()) {
        SDL_DEBUG("Expired request wasn't removed");
        break;
      }
    }
  }
  UpdateTimer();
  SDL_DEBUG(
      "EXIT Waiting for response count : " << waiting_for_response_.Size());
}

RequestController::Worker::Worker(RequestController* requestController)
    : request_controller_(requestController), stop_flag_(false) {}

RequestController::Worker::~Worker() {}

void RequestController::Worker::threadMain() {
  SDL_AUTO_TRACE();
  AutoLock auto_lock(thread_lock_);
  while (!stop_flag_) {
    // Try to pick a request
    AutoLock auto_lock(request_controller_->mobile_request_list_lock_);

    while ((request_controller_->pool_state_ != TPoolState::STOPPED) &&
           (request_controller_->mobile_request_list_.empty())) {
      // Wait until there is a task in the queue
      // Unlock mutex while wait, then lock it back when signaled
      SDL_INFO("Unlocking and waiting");
      request_controller_->cond_var_.Wait(auto_lock);
      SDL_INFO("Signaled and locking");
    }

    // If the thread was shutdown, return from here
    if (request_controller_->pool_state_ == TPoolState::STOPPED) {
      break;
    }

    if (request_controller_->mobile_request_list_.empty()) {
      SDL_WARN("Mobile request list is empty");
      break;
    }

    RequestPtr request_ptr(request_controller_->mobile_request_list_.front());
    request_controller_->mobile_request_list_.pop_front();

    bool init_res = request_ptr->Init();  // to setup specific
                                          // default timeout

    const uint32_t timeout_in_mseconds = request_ptr->default_timeout();
    RequestInfoPtr request_info_ptr(
        new MobileRequestInfo(request_ptr, timeout_in_mseconds));

    request_controller_->waiting_for_response_.Add(request_info_ptr);
    SDL_DEBUG("timeout_in_mseconds " << timeout_in_mseconds);

    if (0 != timeout_in_mseconds) {
      request_controller_->UpdateTimer();
    } else {
      SDL_DEBUG(
          "Default timeout was set to 0. "
          "RequestController will not track timeout "
          "of this request.");
    }

    AutoUnlock unlock(auto_lock);

    // execute
    if ((false == request_controller_->IsLowVoltage()) &&
        request_ptr->CheckPermissions() && init_res) {
      SDL_DEBUG("Execute MobileRequest corr_id = "
                << request_info_ptr->requestId()
                << " with timeout: " << timeout_in_mseconds);
      request_ptr->Run();
    }
  }
}

void RequestController::Worker::exitThreadMain() {
  stop_flag_ = true;
  // setup stop flag and whit while threadMain will be finished correctly
  // FIXME (dchmerev@luxoft.com): There is no waiting
}

void RequestController::UpdateTimer() {
  SDL_AUTO_TRACE();
  RequestInfoPtr front = waiting_for_response_.FrontWithNotNullTimeout();
  // Buffer for sending request
  const uint32_t delay_time = 100u;
  if (front) {
    const TimevalStruct current_time = date_time::DateTime::getCurrentTime();
    TimevalStruct end_time = front->end_time();
    date_time::DateTime::AddMilliseconds(end_time, delay_time);
    if (current_time < end_time) {
      const uint32_t msecs = static_cast<uint32_t>(
          date_time::DateTime::getmSecs(end_time - current_time));
      SDL_DEBUG("Sleep for " << msecs << " millisecs");
      // Timeout for bigger than 5 minutes is a mistake
      timer_.Start(msecs, timer::kSingleShot);
    } else {
      SDL_WARN("Request app_id: "
               << front->app_id() << " correlation_id: " << front->requestId()
               << " is expired. "
               << "End time (ms): " << date_time::DateTime::getmSecs(end_time)
               << " Current time (ms): "
               << date_time::DateTime::getmSecs(current_time)
               << " Diff (current - end) (ms): "
               << date_time::DateTime::getmSecs(current_time - end_time)
               << " Request timeout (sec): "
               << front->timeout_msec() / date_time::kMillisecondsInSecond);
    }
  }
}

}  //  namespace request_controller
}  //  namespace application_manager
