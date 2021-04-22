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

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/commands/request_to_hmi.h"
#include "application_manager/request_controller.h"

#include "utils/timer_task_impl.h"

namespace application_manager {

namespace request_controller {

using namespace sync_primitives;

SDL_CREATE_LOG_VARIABLE("RequestController")

RequestController::RequestController(const RequestControlerSettings& settings)
    : pool_state_(UNDEFINED)
    , pool_size_(settings.thread_pool_size())
    , request_tracker_(settings)
    , duplicate_message_count_()
    , timer_("AM RequestCtrlTimer",
             new timer::TimerTaskImpl<RequestController>(
                 this, &RequestController::TimeoutThread))
    , timer_stop_flag_(false)
    , is_low_voltage_(false)
    , settings_(settings) {
  SDL_LOG_AUTO_TRACE();
  InitializeThreadpool();
  timer_.Start(0, timer::kSingleShot);
}

RequestController::~RequestController() {
  SDL_LOG_AUTO_TRACE();
  {
    sync_primitives::AutoLock auto_lock(timer_lock);
    timer_stop_flag_ = true;
    timer_condition_.Broadcast();
  }
  timer_.Stop();
  if (pool_state_ != TPoolState::STOPPED) {
    DestroyThreadpool();
  }
}

void RequestController::InitializeThreadpool() {
  SDL_LOG_AUTO_TRACE();
  // TODO(DK): Consider lazy loading threads instead of creating all at once
  pool_state_ = TPoolState::STARTED;
  char name[50];
  for (uint32_t i = 0; i < pool_size_; ++i) {
    snprintf(name, sizeof(name) / sizeof(name[0]), "AM Pool %u", i);
    pool_.push_back(threads::CreateThread(name, new Worker(this)));
    pool_[i]->Start();
    SDL_LOG_DEBUG("Request thread initialized: " << name);
  }
}

void RequestController::DestroyThreadpool() {
  SDL_LOG_AUTO_TRACE();
  {
    AutoLock auto_lock(mobile_request_list_lock_);
    pool_state_ = TPoolState::STOPPED;
    SDL_LOG_DEBUG("Broadcasting STOP signal to all threads...");
    cond_var_.Broadcast();  // notify all threads we are shutting down
  }
  for (size_t i = 0; i < pool_.size(); ++i) {
    threads::Thread* thread = pool_[i];
    thread->Stop(threads::Thread::kThreadSoftStop);
    delete thread->GetDelegate();
    threads::DeleteThread(thread);
  }
  pool_.clear();
}

RequestController::TResult RequestController::CheckPosibilitytoAdd(
    const RequestPtr request, const mobile_apis::HMILevel::eType level) {
  SDL_LOG_AUTO_TRACE();
  if (!CheckPendingRequestsAmount(settings_.pending_requests_amount())) {
    SDL_LOG_ERROR("Too many pending request");
    return RequestController::TOO_MANY_PENDING_REQUESTS;
  }

  const TrackResult track_result =
      request_tracker_.Track(request->connection_key(), level);

  if (TrackResult::kNoneLevelMaxRequestsExceeded == track_result) {
    SDL_LOG_ERROR("Too many application requests in hmi level NONE");
    return RequestController::NONE_HMI_LEVEL_MANY_REQUESTS;
  }

  if (TrackResult::kMaxRequestsExceeded == track_result) {
    SDL_LOG_ERROR("Too many application requests");
    return RequestController::TOO_MANY_REQUESTS;
  }

  if (IsLowVoltage()) {
    SDL_LOG_ERROR("Impossible to add request due to Low Voltage is active");
    return RequestController::INVALID_DATA;
  }

  return SUCCESS;
}

bool RequestController::CheckPendingRequestsAmount(
    const uint32_t& pending_requests_amount) {
  SDL_LOG_AUTO_TRACE();
  if (pending_requests_amount > 0) {
    const size_t pending_requests_size = mobile_request_list_.size();
    const bool available_to_add =
        pending_requests_amount > pending_requests_size;
    if (!available_to_add) {
      SDL_LOG_WARN("Pending requests count " << pending_requests_size
                                             << " exceed application limit "
                                             << pending_requests_amount);
    }
    return available_to_add;
  }
  SDL_LOG_DEBUG("CheckPendingRequestsAmount disabled");
  return true;
}

RequestController::TResult RequestController::addMobileRequest(
    const RequestPtr request, const mobile_apis::HMILevel::eType& hmi_level) {
  SDL_LOG_AUTO_TRACE();
  if (!request) {
    SDL_LOG_ERROR("Null Pointer request");
    cond_var_.NotifyOne();
    return INVALID_DATA;
  }
  SDL_LOG_DEBUG("correlation_id : " << request->correlation_id()
                                    << "connection_key : "
                                    << request->connection_key());
  RequestController::TResult result = CheckPosibilitytoAdd(request, hmi_level);
  if (SUCCESS == result) {
    AutoLock auto_lock_list(mobile_request_list_lock_);
    mobile_request_list_.push_back(request);
    SDL_LOG_DEBUG("Waiting for execution: " << mobile_request_list_.size());
    // wake up one thread that is waiting for a task to be available
  }
  cond_var_.NotifyOne();
  return result;
}

RequestController::TResult RequestController::addHMIRequest(
    const RequestPtr request) {
  SDL_LOG_AUTO_TRACE();

  if (request.use_count() == 0) {
    SDL_LOG_ERROR("HMI request pointer is invalid");
    return RequestController::INVALID_DATA;
  }
  SDL_LOG_DEBUG(" correlation_id : " << request->correlation_id());

  const uint64_t timeout_in_mseconds =
      static_cast<uint64_t>(request->default_timeout());
  RequestInfoPtr request_info_ptr =
      std::make_shared<HMIRequestInfo>(request, timeout_in_mseconds);

  if (0 == timeout_in_mseconds) {
    SDL_LOG_DEBUG(
        "Default timeout was set to 0."
        "RequestController will not track timeout of this request.");
  }

  if (IsLowVoltage()) {
    SDL_LOG_ERROR("Impossible to add request due to Low Voltage is active");
    return RequestController::INVALID_DATA;
  }

  waiting_for_response_.Add(request_info_ptr);
  SDL_LOG_DEBUG("Waiting for response count:" << waiting_for_response_.Size());

  NotifyTimer();
  return RequestController::SUCCESS;
}

void RequestController::addNotification(const RequestPtr ptr) {
  SDL_LOG_AUTO_TRACE();
  if (IsLowVoltage()) {
    SDL_LOG_ERROR(
        "Impossible to add notification due to Low Voltage is active");
    return;
  }
  notification_list_.push_back(ptr);
}

void RequestController::removeNotification(
    const commands::Command* notification) {
  SDL_LOG_AUTO_TRACE();
  std::list<RequestPtr>::iterator it = notification_list_.begin();
  for (; notification_list_.end() != it;) {
    if (it->get() == notification) {
      notification_list_.erase(it++);
      SDL_LOG_DEBUG("Notification removed");
      return;
    } else {
      ++it;
    }
  }
  SDL_LOG_DEBUG("Cannot find notification");
}

void RequestController::TerminateRequest(const uint32_t correlation_id,
                                         const uint32_t connection_key,
                                         const int32_t function_id,
                                         bool force_terminate) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("correlation_id = "
                << correlation_id << " connection_key = " << connection_key
                << " function_id = " << function_id
                << " force_terminate = " << force_terminate);
  {
    AutoLock auto_lock(duplicate_message_count_lock_);
    auto dup_it = duplicate_message_count_.find(correlation_id);
    if (duplicate_message_count_.end() != dup_it) {
      --duplicate_message_count_[correlation_id];
      if (0 == duplicate_message_count_[correlation_id]) {
        duplicate_message_count_.erase(dup_it);
      }
      SDL_LOG_DEBUG(
          "Ignoring termination request due to duplicate correlation "
          "ID being sent");
      return;
    }
  }

  RequestInfoPtr request =
      waiting_for_response_.Find(connection_key, correlation_id);
  if (!request) {
    SDL_LOG_WARN("Request was not found in waiting_for_response");
    return;
  }
  if (request->request()->function_id() != function_id) {
    SDL_LOG_ERROR("Request and response function_id's don't match");
    return;
  }
  if (force_terminate || request->request()->AllowedToTerminate()) {
    waiting_for_response_.RemoveRequest(request);
  } else {
    SDL_LOG_WARN("Request was not terminated");
  }
  NotifyTimer();
}

void RequestController::OnMobileResponse(const uint32_t mobile_correlation_id,
                                         const uint32_t connection_key,
                                         const int32_t function_id) {
  SDL_LOG_AUTO_TRACE();
  TerminateRequest(mobile_correlation_id, connection_key, function_id);
}

void RequestController::OnHMIResponse(const uint32_t correlation_id,
                                      const int32_t function_id) {
  SDL_LOG_AUTO_TRACE();
  TerminateRequest(correlation_id, RequestInfo::HmiConnectionKey, function_id);
}

void RequestController::terminateWaitingForExecutionAppRequests(
    const uint32_t& app_id) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("app_id: " << app_id << "Waiting for execution"
                           << mobile_request_list_.size());
  AutoLock auto_lock(mobile_request_list_lock_);
  std::list<RequestPtr>::iterator request_it = mobile_request_list_.begin();
  while (mobile_request_list_.end() != request_it) {
    RequestPtr request = (*request_it);
    if ((request.use_count() != 0) && (request->connection_key() == app_id)) {
      mobile_request_list_.erase(request_it++);
    } else {
      ++request_it;
    }
  }
  SDL_LOG_DEBUG("Waiting for execution " << mobile_request_list_.size());
}

void RequestController::terminateWaitingForResponseAppRequests(
    const uint32_t& app_id) {
  SDL_LOG_AUTO_TRACE();
  waiting_for_response_.RemoveByConnectionKey(app_id);
  SDL_LOG_DEBUG(
      "Waiting for response count : " << waiting_for_response_.Size());
}

void RequestController::terminateAppRequests(const uint32_t& app_id) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("app_id : " << app_id
                            << "Requests waiting for execution count : "
                            << mobile_request_list_.size()
                            << "Requests waiting for response count : "
                            << waiting_for_response_.Size());

  terminateWaitingForExecutionAppRequests(app_id);
  terminateWaitingForResponseAppRequests(app_id);
  NotifyTimer();
}

void RequestController::terminateAllHMIRequests() {
  SDL_LOG_AUTO_TRACE();
  terminateWaitingForResponseAppRequests(RequestInfo::HmiConnectionKey);
}

void RequestController::terminateAllMobileRequests() {
  SDL_LOG_AUTO_TRACE();
  waiting_for_response_.RemoveMobileRequests();
  SDL_LOG_DEBUG("Mobile Requests waiting for response cleared");
  AutoLock waiting_execution_auto_lock(mobile_request_list_lock_);
  mobile_request_list_.clear();
  SDL_LOG_DEBUG("Mobile Requests waiting for execution cleared");
  NotifyTimer();
}

void RequestController::updateRequestTimeout(const uint32_t& app_id,
                                             const uint32_t& correlation_id,
                                             const uint32_t& new_timeout) {
  SDL_LOG_AUTO_TRACE();

  SDL_LOG_DEBUG("app_id : " << app_id
                            << " mobile_correlation_id : " << correlation_id
                            << " new_timeout : " << new_timeout);
  SDL_LOG_DEBUG(
      "New_timeout is NULL. RequestCtrl will "
      "not manage this request any more");

  RequestInfoPtr request_info =
      waiting_for_response_.Find(app_id, correlation_id);
  if (request_info) {
    waiting_for_response_.RemoveRequest(request_info);
    request_info->updateTimeOut(new_timeout);
    waiting_for_response_.Add(request_info);
    NotifyTimer();
    SDL_LOG_INFO("Timeout updated for "
                 << " app_id: " << app_id << " correlation_id: "
                 << correlation_id << " new_timeout (ms): " << new_timeout);
  } else {
    SDL_LOG_ERROR("Can't find request with "
                  << " app_id: " << app_id
                  << " correlation_id: " << correlation_id);
  }
}

void RequestController::OnLowVoltage() {
  SDL_LOG_AUTO_TRACE();
  is_low_voltage_ = true;
}

void RequestController::OnWakeUp() {
  SDL_LOG_AUTO_TRACE();
  terminateAllHMIRequests();
  terminateAllMobileRequests();
  is_low_voltage_ = false;
  SDL_LOG_DEBUG("Terminate old requests done");
}

bool RequestController::IsLowVoltage() {
  SDL_LOG_TRACE("result: " << is_low_voltage_);
  return is_low_voltage_;
}

void RequestController::TimeoutThread() {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG(
      "ENTER Waiting fore response count: " << waiting_for_response_.Size());
  sync_primitives::AutoLock auto_lock(timer_lock);
  while (!timer_stop_flag_) {
    RequestInfoPtr probably_expired =
        waiting_for_response_.FrontWithNotNullTimeout();
    if (!probably_expired) {
      timer_condition_.Wait(auto_lock);
      continue;
    }
    if (!probably_expired->isExpired()) {
      SDL_LOG_DEBUG("Timeout for "
                    << (RequestInfo::HMIRequest ==
                                probably_expired->request_type()
                            ? "HMI"
                            : "Mobile")
                    << " request id: " << probably_expired->requestId()
                    << " connection_key: " << probably_expired->app_id()
                    << " NOT expired");
      const date_time::TimeDuration current_time = date_time::getCurrentTime();
      const date_time::TimeDuration end_time = probably_expired->end_time();
      if (current_time < end_time) {
        const uint32_t msecs =
            static_cast<uint32_t>(date_time::getmSecs(end_time - current_time));
        SDL_LOG_DEBUG("Sleep for " << msecs << " millisecs");
        timer_condition_.WaitFor(auto_lock, msecs);
      }
      continue;
    }
    SDL_LOG_INFO("Timeout for "
                 << (RequestInfo::HMIRequest == probably_expired->request_type()
                         ? "HMI"
                         : "Mobile")
                 << " request id: " << probably_expired->requestId()
                 << " connection_key: " << probably_expired->app_id()
                 << " is expired");
    const uint32_t experied_request_id = probably_expired->requestId();
    const uint32_t experied_app_id = probably_expired->app_id();

    probably_expired->request()->onTimeOut();
    if (RequestInfo::HmiConnectionKey == probably_expired->app_id()) {
      SDL_LOG_DEBUG("Erase HMI request: " << probably_expired->requestId());
      waiting_for_response_.RemoveRequest(probably_expired);
    }
    probably_expired = waiting_for_response_.FrontWithNotNullTimeout();
    if (probably_expired) {
      if (experied_request_id == probably_expired->requestId() &&
          experied_app_id == probably_expired->app_id()) {
        SDL_LOG_DEBUG("Expired request wasn't removed");
        break;
      }
    }
  }
  SDL_LOG_DEBUG(
      "EXIT Waiting for response count : " << waiting_for_response_.Size());
}

RequestController::Worker::Worker(RequestController* requestController)
    : request_controller_(requestController), stop_flag_(false) {}

RequestController::Worker::~Worker() {}

void RequestController::Worker::threadMain() {
  SDL_LOG_AUTO_TRACE();
  AutoLock auto_lock(thread_lock_);
  while (!stop_flag_) {
    // Try to pick a request
    AutoLock auto_lock(request_controller_->mobile_request_list_lock_);

    while ((request_controller_->pool_state_ != TPoolState::STOPPED) &&
           (request_controller_->mobile_request_list_.empty())) {
      // Wait until there is a task in the queue
      // Unlock mutex while wait, then lock it back when signaled
      SDL_LOG_INFO("Unlocking and waiting");
      request_controller_->cond_var_.Wait(auto_lock);
      SDL_LOG_INFO("Signaled and locking");
    }

    // If the thread was shutdown, return from here
    if (request_controller_->pool_state_ == TPoolState::STOPPED) {
      break;
    }

    if (request_controller_->mobile_request_list_.empty()) {
      SDL_LOG_WARN("Mobile request list is empty");
      break;
    }

    RequestPtr request_ptr(request_controller_->mobile_request_list_.front());
    request_controller_->mobile_request_list_.pop_front();

    bool init_res = request_ptr->Init();  // to setup specific
                                          // default timeout

    const uint32_t timeout_in_mseconds = request_ptr->default_timeout();
    RequestInfoPtr request_info_ptr =
        std::make_shared<MobileRequestInfo>(request_ptr, timeout_in_mseconds);

    if (!request_controller_->waiting_for_response_.Add(request_info_ptr)) {
      commands::CommandRequestImpl* cmd_request =
          dynamic_cast<commands::CommandRequestImpl*>(request_ptr.get());
      if (cmd_request != NULL) {
        uint32_t corr_id = cmd_request->correlation_id();
        request_controller_->duplicate_message_count_lock_.Acquire();
        auto dup_it =
            request_controller_->duplicate_message_count_.find(corr_id);
        if (request_controller_->duplicate_message_count_.end() == dup_it) {
          request_controller_->duplicate_message_count_[corr_id] = 0;
        }
        request_controller_->duplicate_message_count_[corr_id]++;
        request_controller_->duplicate_message_count_lock_.Release();
        cmd_request->SendResponse(
            false, mobile_apis::Result::INVALID_ID, "Duplicate correlation_id");
      }
      continue;
    }
    SDL_LOG_DEBUG("timeout_in_mseconds " << timeout_in_mseconds);

    if (0 != timeout_in_mseconds) {
      request_controller_->NotifyTimer();
    } else {
      SDL_LOG_DEBUG(
          "Default timeout was set to 0. "
          "RequestController will not track timeout "
          "of this request.");
    }

    AutoUnlock unlock(auto_lock);

    // execute
    if ((false == request_controller_->IsLowVoltage()) &&
        request_ptr->CheckPermissions() && init_res) {
      SDL_LOG_DEBUG("Execute MobileRequest corr_id = "
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

void RequestController::NotifyTimer() {
  SDL_LOG_AUTO_TRACE();
  timer_condition_.NotifyOne();
}

}  //  namespace request_controller
}  //  namespace application_manager
