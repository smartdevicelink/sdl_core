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

#include <algorithm>
#include "utils/logger.h"
#include "config_profile/profile.h"
#include "application_manager/request_controller.h"
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/commands/hmi/request_to_hmi.h"

namespace application_manager {

namespace request_controller {

using namespace sync_primitives;

CREATE_LOGGERPTR_GLOBAL(logger_, "RequestController");

RequestController::RequestController()
  : pool_state_(UNDEFINED),
    pool_size_(profile::Profile::instance()->thread_pool_size()),
    pending_request_set_lock_(true),
    timer_("RequestCtrlTimer", this, &RequestController::onTimer, true)
{
  LOG4CXX_INFO(logger_, "RequestController::RequestController()");
  InitializeThreadpool();
  timer_.start(dafault_sleep_time_);
  LOG4CXX_DEBUG(logger_," Create timer thread ; timer thread = " << timer_.thread_->thread_handle());
}

RequestController::~RequestController() {
  LOG4CXX_TRACE_ENTER(logger_);
  if (pool_state_ != TPoolState::STOPPED) {
    DestroyThreadpool();
  }

  pool_.clear();
  mobile_request_list_.clear();
  pending_request_set_.clear();
}

void RequestController::InitializeThreadpool()
{
  LOG4CXX_TRACE_ENTER(logger_);
  // TODO: Consider lazy loading threads instead of creating all at once
  pool_state_ = TPoolState::STARTED;
  for (uint32_t i = 0; i < pool_size_; i++) {
    char name [50];
    snprintf(name, sizeof(name)/sizeof(name[0]),
             "AM Pool %d", i);
    pool_.push_back(threads::CreateThread(name, new Worker(this)));
    pool_[i]->start();
    LOG4CXX_INFO(logger_, "Request thread initialized: " << name);
  }
}

void RequestController::DestroyThreadpool() {
  LOG4CXX_TRACE_ENTER(logger_);
  {
    sync_primitives::AutoLock auto_lock (mobile_request_list_lock_);
    pool_state_ = TPoolState::STOPPED;
    LOG4CXX_INFO(logger_, "Broadcasting STOP signal to all threads...");
    cond_var_.Broadcast(); // notify all threads we are shutting down
  }
  for (uint32_t i = 0; i < pool_size_; i++) {
    pool_[i]->stop();
    threads::DeleteThread(pool_[i]);
  }
  LOG4CXX_INFO(logger_, "Threads exited from the thread pool " << pool_size_);
}

RequestController::TResult RequestController::addMobileRequest(
    const MobileRequestPtr& request,
    const mobile_apis::HMILevel::eType& hmi_level) {
  LOG4CXX_TRACE_ENTER(logger_);
  if (!request.valid()) {
    LOG4CXX_INFO(logger_, "Null Pointer request");
    LOG4CXX_TRACE_EXIT(logger_);
    cond_var_.NotifyOne();
    return INVALID_DATA;
  }

  TResult result = SUCCESS;
  const commands::CommandRequestImpl* request_impl =
      static_cast<commands::CommandRequestImpl*>(request.get());
  LOG4CXX_DEBUG(logger_, "addMobileRequest " << request_impl->correlation_id());
  const uint32_t& app_hmi_level_none_time_scale =
      profile::Profile::instance()->app_hmi_level_none_time_scale();

  const uint32_t& app_hmi_level_none_max_request_per_time_scale =
      profile::Profile::instance()->app_hmi_level_none_time_scale_max_requests();

  const uint32_t& app_time_scale =
      profile::Profile::instance()->app_time_scale();

  const uint32_t& max_request_per_time_scale =
      profile::Profile::instance()->app_time_scale_max_requests();

  const uint32_t& pending_requests_amount =
      profile::Profile::instance()->pending_requests_amount();

  if (!checkHMILevelTimeScaleMaxRequest(mobile_apis::HMILevel::HMI_NONE,
                                        request_impl->connection_key(),
                                        app_hmi_level_none_time_scale,
                                        app_hmi_level_none_max_request_per_time_scale)) {
    LOG4CXX_ERROR(logger_, "Too many application requests in hmi level NONE");
    result = RequestController::NONE_HMI_LEVEL_MANY_REQUESTS;
  } else if (!checkTimeScaleMaxRequest(
        request_impl->connection_key(),
        app_time_scale, max_request_per_time_scale)) {
    LOG4CXX_ERROR(logger_, "Too many application requests");
    result = RequestController::TOO_MANY_REQUESTS;
  } else if (pending_requests_amount == mobile_request_list_.size()) {
    LOG4CXX_ERROR(logger_, "Too many pending request");
    result = RequestController::TOO_MANY_PENDING_REQUESTS;
  }
  {
    AutoLock auto_lock(mobile_request_list_lock_);

    mobile_request_list_.push_back(request);
    LOG4CXX_INFO(logger_, "mobile_request_list_ size is "
                 << mobile_request_list_.size()
                 << " pending_request_set_ size is "
                 << pending_request_set_.size()
                 );
  }

  // wake up one thread that is waiting for a task to be available
  cond_var_.NotifyOne();
  LOG4CXX_TRACE_EXIT(logger_);
  return result;
}

RequestController::TResult RequestController::addHMIRequest(
    const RequestPtr request) {
  LOG4CXX_TRACE_ENTER(logger_);
  DCHECK(request.valid());
  LOG4CXX_DEBUG(logger_, "addHMIRequest " << request->correlation_id());

  uint32_t timeout_in_seconds = request->default_timeout()/date_time::DateTime::MILLISECONDS_IN_SECOND;
  RequestInfoPtr request_info_ptr =
      new HMIRequestInfo(request,
                         timeout_in_seconds);

  if (0 != timeout_in_seconds) {
    pending_request_set_lock_.Acquire();
    pending_request_set_.insert(request_info_ptr);
    LOG4CXX_INFO(logger_, "pending_request_set_ size is "
                 << pending_request_set_.size());
    UpdateTimer();
    pending_request_set_lock_.Release();
  } else {
    LOG4CXX_INFO(logger_, "Default timeout was set to 0."
                 "RequestController will not track timeout of this request.");
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return RequestController::SUCCESS;
}

void RequestController::addNotification(const RequestPtr ptr) {
  LOG4CXX_TRACE_ENTER(logger_);
  notification_list_.push_back(ptr);
  LOG4CXX_TRACE_EXIT(logger_);
}

void RequestController::removeNotification(const commands::Command* notification) {
  LOG4CXX_TRACE_ENTER(logger_);
  std::list<RequestPtr>::iterator it = notification_list_.begin();
  for (; notification_list_.end() != it; ++it) {
    if (it->get() == notification) {
      notification_list_.erase(it);
      break;
    }
  }
  LOG4CXX_TRACE_EXIT(logger_);
}

void RequestController::terminateMobileRequest(
    const uint32_t& mobile_correlation_id) {
  LOG4CXX_TRACE_ENTER(logger_);

  AutoLock auto_lock(pending_request_set_lock_);
  RequestInfoSet::iterator it = pending_request_set_.begin();
  for (; pending_request_set_.end() != it; ++it) {
    RequestInfo* mobile_request_info = it->get();
    if (NULL == mobile_request_info) {
      continue;
    }
    if (mobile_correlation_id == mobile_request_info->requestId()) {
      mobile_request_info->request()->CleanUp();
      pending_request_set_.erase(it);
      LOG4CXX_INFO(logger_, "Mobile request terminated: " << mobile_correlation_id <<
                   " pending_request_set_ size : " << pending_request_set_.size());
      UpdateTimer();
      LOG4CXX_TRACE_EXIT(logger_);
      return;
    }
  }
  LOG4CXX_INFO(logger_, "Mobile request NOT terminated: " << mobile_correlation_id <<
                        " pending_request_set_ size : " << pending_request_set_.size());
  LOG4CXX_TRACE_EXIT(logger_);
}

void RequestController::terminateHMIRequest(const uint32_t &correlation_id) {
  LOG4CXX_TRACE_ENTER(logger_);
  AutoLock auto_lock(pending_request_set_lock_);
  RequestInfoSet::iterator it = pending_request_set_.begin();
  for (; pending_request_set_.end() != it; ++it) {
    RequestInfo* hmi_request_info = it->get();
    if (NULL == hmi_request_info) {
      continue;
    }
    if (correlation_id == hmi_request_info->requestId()) {
      hmi_request_info->request()->CleanUp();
      pending_request_set_.erase(it);
      LOG4CXX_DEBUG(logger_, "HMI request terminated: " << correlation_id);
      UpdateTimer();
      LOG4CXX_TRACE_EXIT(logger_);
      return;
    }
  }
  LOG4CXX_INFO(logger_, "HMI request NOT terminated: " << correlation_id <<
                        " pending_request_set_ size : " << pending_request_set_.size());
  LOG4CXX_TRACE_EXIT(logger_);
}

void RequestController::terminateAppRequests(
    const uint32_t& app_id) {
  LOG4CXX_TRACE_ENTER(logger_);

  AutoLock auto_lock(pending_request_set_lock_);
  RequestInfoSet::iterator it = pending_request_set_.begin();
  while (pending_request_set_.end() != it) {
    RequestInfo* mobile_request_info = it->get();
    if (NULL == mobile_request_info) {
      ++it;
      continue;
    }

    if (mobile_request_info->app_id() == app_id) {
      mobile_request_info->request()->CleanUp();
      pending_request_set_.erase(it++);
      LOG4CXX_INFO(logger_, "terminated all app requests : " << app_id);
    } else {
      ++it;
    }
  }
  UpdateTimer();
  LOG4CXX_TRACE_EXIT(logger_);
}

void RequestController::terminateAllHMIRequests() {
  LOG4CXX_TRACE_ENTER(logger_);
  AutoLock auto_lock(pending_request_set_lock_);
  RequestInfoSet::iterator it = pending_request_set_.begin();
  while (pending_request_set_.end() != it) {
    RequestInfo* hmi_request_info = it->get();
    if (NULL == hmi_request_info) {
      ++it;
      continue;
    }
    hmi_request_info->request()->CleanUp();
    pending_request_set_.erase(it++);
    LOG4CXX_INFO(logger_, "HMI request terminated: ");
  }
  LOG4CXX_TRACE_EXIT(logger_);
}

void RequestController::updateRequestTimeout(
    const uint32_t& app_id,
    const uint32_t& mobile_correlation_id,
    const uint32_t& new_timeout) {

  LOG4CXX_TRACE_ENTER(logger_);

  AutoLock auto_lock(pending_request_set_lock_);
  RequestInfoSet::iterator it = pending_request_set_.begin();
  RequestInfo* mobile_request_info = NULL;
  RequestInfoPtr request_info;
  for (; pending_request_set_.end() != it; ++it) {
    request_info = *it;
    if (false == request_info.valid()) {
      LOG4CXX_ERROR(logger_, "Invalid request, can't update timeout");
      continue;
    }
    mobile_request_info = request_info.get();
    if (NULL == mobile_request_info) {
      continue;
    }
    if (app_id == mobile_request_info->app_id() &&
        mobile_correlation_id == mobile_request_info->requestId()) {
      break;
    }
  }

  if (it != pending_request_set_.end()) {
    DCHECK(mobile_request_info);
    DCHECK(request_info.valid());

    uint32_t timeout_in_seconds = new_timeout/date_time::DateTime::MILLISECONDS_IN_SECOND;
    mobile_request_info->updateTimeOut(timeout_in_seconds);
    pending_request_set_.erase(it);
    pending_request_set_.insert(request_info);
    // erase and insert need to update ordering of set
    UpdateTimer();
    LOG4CXX_ERROR(logger_, "Timeout updated for "
                  << " app_id " << app_id
                  << " mobile_correlation_id " << mobile_correlation_id
                  << " new_timeout " << new_timeout);
  } else {
    LOG4CXX_ERROR(logger_, "Can't find request with "
                  << " app_id " << app_id
                  << " mobile_correlation_id " << mobile_correlation_id );
  }
}

void RequestController::onTimer() {
  LOG4CXX_TRACE_ENTER(logger_);
  AutoLock auto_lock(pending_request_set_lock_);
  RequestInfoSet::iterator probably_expired = pending_request_set_.begin();
  while (pending_request_set_.end() != probably_expired) {
    RequestInfoPtr request = *probably_expired;
    if (request->timeout_sec() == 0) {
      LOG4CXX_INFO(logger_, "Ignore " << request->requestId());
      ++probably_expired;
      // This request should not be observed for TIME_OUT
      continue;
    }
    if (request->isExpired()) {
      pending_request_set_.erase(probably_expired);
      request->request()->onTimeOut();
      request->request()->CleanUp();
      LOG4CXX_INFO(logger_, "Timeout for request id " << request->requestId() << " expired");
      probably_expired = pending_request_set_.begin();
      break;
    }
  }
  UpdateTimer();
  LOG4CXX_TRACE_EXIT(logger_);
}

RequestController::Worker::Worker(RequestController* requestController)
  : request_controller_(requestController)
  , stop_flag_(false) {
}

RequestController::Worker::~Worker() {
}

void RequestController::Worker::threadMain() {
  LOG4CXX_TRACE_ENTER(logger_);
  sync_primitives::AutoLock auto_lock(thread_lock_);
  while (!stop_flag_) {
    // Try to pick a request
    sync_primitives::AutoLock auto_lock(request_controller_->mobile_request_list_lock_);

    while ((request_controller_->pool_state_ != TPoolState::STOPPED) &&
        (request_controller_->mobile_request_list_.empty())) {
      // Wait until there is a task in the queue
      // Unlock mutex while wait, then lock it back when signaled
      LOG4CXX_INFO(logger_, "Unlocking and waiting");
      request_controller_->cond_var_.Wait(auto_lock);
      LOG4CXX_INFO(logger_, "Signaled and locking");
    }

    // If the thread was shutdown, return from here
    if (request_controller_->pool_state_ == TPoolState::STOPPED) {
      break;
    }

    MobileRequestPtr request(request_controller_->mobile_request_list_.front());

    request_controller_->mobile_request_list_.pop_front();
    bool init_res = request->Init(); // to setup specific default timeout

    uint32_t timeout_in_seconds = request->default_timeout()/date_time::DateTime::MILLISECONDS_IN_SECOND;
    RequestInfoPtr request_info_ptr(new MobileRequestInfo(request,
                                                          timeout_in_seconds));

    request_controller_->pending_request_set_lock_.Acquire();
    request_controller_->pending_request_set_.insert(request_info_ptr);
    if (0 != timeout_in_seconds) {
      LOG4CXX_INFO(logger_, "Add Request " << request_info_ptr->requestId() <<
                            " with timeout: " << timeout_in_seconds);
      request_controller_->UpdateTimer();
    } else {
      LOG4CXX_INFO(logger_, "Default timeout was set to 0."
                   "RequestController will not track timeout of this request.");
    }
    request_controller_->pending_request_set_lock_.Release();
    AutoUnlock unlock(auto_lock);

    // execute
    if (request->CheckPermissions() && init_res) {
      request->Run();
    }
  }

}

bool RequestController::Worker::exitThreadMain() {
  stop_flag_ = true;
  sync_primitives::AutoLock auto_lock(thread_lock_);
  // setup stop flag and whit while threadMain will be finished correctly
  // FIXME (dchmerev@luxoft.com): There is no wating
  return true;
}

bool RequestController::checkTimeScaleMaxRequest(
    const uint32_t& app_id,
    const uint32_t& app_time_scale,
    const uint32_t& max_request_per_time_scale) {
  LOG4CXX_TRACE_ENTER(logger_);
  {
    AutoLock auto_lock(pending_request_set_lock_);
    TimevalStruct end = date_time::DateTime::getCurrentTime();
    TimevalStruct start;
    start.tv_sec = end.tv_sec - app_time_scale;

    TimeScale scale(start, end, app_id);
    uint32_t count = 0;

    count = count_if (pending_request_set_.begin(), pending_request_set_.end(), scale);
    if (count == max_request_per_time_scale ) {
      LOG4CXX_ERROR(logger_, "Requests count " << count <<
                    " exceed application limit " << max_request_per_time_scale);
      return true;
    }
  }
  return true;
}

bool RequestController::checkHMILevelTimeScaleMaxRequest(
    const mobile_apis::HMILevel::eType& hmi_level,
    const uint32_t& app_id,
    const uint32_t& app_time_scale,
    const uint32_t& max_request_per_time_scale) {
  LOG4CXX_TRACE_ENTER(logger_);
    {
      AutoLock auto_lock(pending_request_set_lock_);
      TimevalStruct end = date_time::DateTime::getCurrentTime();
      TimevalStruct start;
      start.tv_sec = end.tv_sec - app_time_scale;

      HMILevelTimeScale scale(start, end, app_id, hmi_level);
      uint32_t count = 0;

      count = count_if (pending_request_set_.begin(), pending_request_set_.end(), scale);
      if (count == max_request_per_time_scale ) {
        LOG4CXX_ERROR(logger_, "Requests count " << count
                      << " exceed application limit " << max_request_per_time_scale
                      << " in hmi level " << hmi_level);
        return false;
      }
    }
  return true;
}

void RequestController::UpdateTimer() {
  LOG4CXX_TRACE_ENTER(logger_);
  uint32_t sleep_time = dafault_sleep_time_;
  RequestInfoSet::iterator it = pending_request_set_.begin();

  while (it != pending_request_set_.end()) {
    RequestInfoPtr request = *it;
    DCHECK(request.valid());
    if (0 == request->timeout_sec()) {
      ++it;
      // This request should not be observed for TIME_OUT
      continue;
    }
    sleep_time = request->end_time().tv_sec -
                           date_time::DateTime::getCurrentTime().tv_sec;
    break;
  }
  timer_.updateTimeOut(sleep_time);
  LOG4CXX_INFO(logger_, "Sleep for: " << sleep_time);
  LOG4CXX_TRACE_EXIT(logger_);
}


}  //  namespace request_controller

}  //  namespace application_manager
