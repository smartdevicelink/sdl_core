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
    timer_("RequestCtrlTimer", this, &RequestController::onTimer, true),
    is_low_voltage_(false) {
  LOG4CXX_AUTO_TRACE(logger_);
  InitializeThreadpool();
  timer_.start(dafault_sleep_time_);
}

RequestController::~RequestController() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (pool_state_ != TPoolState::STOPPED) {
    DestroyThreadpool();
  }
}

void RequestController::InitializeThreadpool() {
  LOG4CXX_AUTO_TRACE(logger_);
  // TODO: Consider lazy loading threads instead of creating all at once
  pool_state_ = TPoolState::STARTED;
  char name[50];
  for (uint32_t i = 0; i < pool_size_; i++) {
    snprintf(name, sizeof(name)/sizeof(name[0]), "AM Pool %d", i);
    pool_.push_back(threads::CreateThread(name, new Worker(this)));
    pool_[i]->start();
    LOG4CXX_DEBUG(logger_, "Request thread initialized: " << name);
  }
}

void RequestController::DestroyThreadpool() {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock auto_lock(mobile_request_list_lock_);
    pool_state_ = TPoolState::STOPPED;
    LOG4CXX_DEBUG(logger_, "Broadcasting STOP signal to all threads...");
    cond_var_.Broadcast();  // notify all threads we are shutting down
  }
  for (size_t i = 0; i < pool_.size(); i++) {
    Thread* thread = pool_[i];
    thread->join();
    delete thread->delegate();
    threads::DeleteThread(thread);
  }
  pool_.clear();
}

RequestController::TResult RequestController::addMobileRequest(
    const RequestPtr request,
    const mobile_apis::HMILevel::eType& hmi_level) {
  if (!request.valid()) {
    LOG4CXX_ERROR(logger_, "Null Pointer request");
    cond_var_.NotifyOne();
    return INVALID_DATA;
  }

  LOG4CXX_DEBUG(logger_, "ENTER correlation_id : " << request->correlation_id()
                << "connection_key : " << request->connection_key());
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

  if (!CheckHMILevelTimeScaleMaxRequest(mobile_apis::HMILevel::HMI_NONE,
                                        request->connection_key(),
                                        app_hmi_level_none_time_scale,
                                        app_hmi_level_none_max_request_per_time_scale)) {
    LOG4CXX_ERROR(logger_, "Too many application requests in hmi level NONE");
    return RequestController::NONE_HMI_LEVEL_MANY_REQUESTS;
  }
  if (!CheckTimeScaleMaxRequest(request->connection_key(),
                                app_time_scale,
                                max_request_per_time_scale)) {
    LOG4CXX_ERROR(logger_, "Too many application requests");
    return RequestController::TOO_MANY_REQUESTS;
  }
  if (!CheckPendingRequestsAmount(pending_requests_amount)) {
    LOG4CXX_ERROR(logger_, "Too many pending request");
    return RequestController::TOO_MANY_PENDING_REQUESTS;
  }
  {
    AutoLock auto_lock_list(mobile_request_list_lock_);
    AutoLock auto_lock_set(pending_request_set_lock_);
    mobile_request_list_.push_back(request);
    LOG4CXX_DEBUG(logger_,
                  "new mobile_request_list_ size is " << mobile_request_list_.size()
                  << ", pending_request_set_ size is " << pending_request_set_.size());
  // wake up one thread that is waiting for a task to be available
  }
  cond_var_.NotifyOne();
  return SUCCESS;
}

RequestController::TResult RequestController::addHMIRequest(
    const RequestPtr request) {
  DCHECK(request.valid());
  if (!request.valid()) {
    LOG4CXX_ERROR(logger_, "HMI request pointer is invalid");
    return RequestController::INVALID_DATA;
  }
  LOG4CXX_DEBUG(logger_, "ENTER addHMIRequest : " << request->correlation_id());

  const uint32_t timeout_in_seconds =
      request->default_timeout() / date_time::DateTime::MILLISECONDS_IN_SECOND;
  RequestInfoPtr request_info_ptr =
      new HMIRequestInfo(request,
                         timeout_in_seconds);

  if (0 != timeout_in_seconds) {
    AutoLock auto_lock(pending_request_set_lock_);
    pending_request_set_.insert(request_info_ptr);
    LOG4CXX_INFO(logger_, "pending_request_set_ size is "
                 << pending_request_set_.size());
    UpdateTimer();
  } else {
    LOG4CXX_INFO(logger_, "Default timeout was set to 0."
                 "RequestController will not track timeout of this request.");
  }
  return RequestController::SUCCESS;
}

void RequestController::addNotification(const RequestPtr ptr) {
  LOG4CXX_AUTO_TRACE(logger_);
  notification_list_.push_back(ptr);
}

void RequestController::removeNotification(const commands::Command* notification) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::list<RequestPtr>::iterator it = notification_list_.begin();
  for (; notification_list_.end() != it; ) {
    if (it->get() == notification) {
      notification_list_.erase(it++);
      break;
    } else {
      ++it;
    }
  }
}

void RequestController::terminateMobileRequest(
    const uint32_t& mobile_correlation_id,
    const uint32_t& connection_key) {
  AutoLock auto_lock(pending_request_set_lock_);
  LOG4CXX_TRACE(logger_, "ENTER corr_id = " << mobile_correlation_id
                << " connection_key = " << connection_key);
  RequestInfoSet::iterator it = pending_request_set_.begin();
  for (; pending_request_set_.end() != it; ++it) {
    RequestInfoPtr request_info = (*it);
    if ((false == request_info.valid()) ||
        RequestInfo::MobileRequest != request_info->requst_type()||
        connection_key != request_info->app_id()) {
      continue;
    }
    if (mobile_correlation_id == request_info->requestId()) {
      request_info->request()->CleanUp();
      pending_request_set_.erase(it);
      LOG4CXX_INFO(logger_, "Mobile request terminated: " << mobile_correlation_id <<
                   " pending_request_set_ size : " << pending_request_set_.size());
      UpdateTimer();
      return;
    }
  }
  LOG4CXX_INFO(logger_, "Mobile request NOT terminated: " << mobile_correlation_id <<
                        " pending_request_set_ size : " << pending_request_set_.size());
}

void RequestController::terminateHMIRequest(const uint32_t &correlation_id) {
  AutoLock auto_lock(pending_request_set_lock_);
  LOG4CXX_TRACE(logger_, "ENTER correlation_id = " << correlation_id);
  RequestInfoSet::iterator it = pending_request_set_.begin();
  for (; pending_request_set_.end() != it; ++it) {
    RequestInfoPtr request_info = (*it);
    if ((false == request_info.valid()) ||
        RequestInfo::HMIRequest != request_info->requst_type() ||
        (0 != request_info->app_id())) {
      continue;
    }
    if (correlation_id == request_info->requestId()) {
      request_info->request()->CleanUp();
      pending_request_set_.erase(it);
      LOG4CXX_DEBUG(logger_, "HMI request terminated: " << correlation_id);
      UpdateTimer();
      return;
    }
  }
  LOG4CXX_INFO(logger_, "HMI request NOT terminated: " << correlation_id <<
                        " pending_request_set_ size : " << pending_request_set_.size());
}

void RequestController::terminateAppRequests(
    const uint32_t& app_id) {
  LOG4CXX_INFO(logger_, "ENTER terminate all app requests : " << app_id
                        << " mobile_request_list_ size is : " << mobile_request_list_.size()
                        << " pending_request_set_ size is : " << pending_request_set_.size());

  AutoLock mobile_requests_auto_lock(mobile_request_list_lock_);
  std::list<RequestPtr>::iterator request_it = mobile_request_list_.begin();
  while (mobile_request_list_.end() != request_it) {
    RequestPtr request = (*request_it);
    if ((request.valid()) && (request->connection_key() == app_id)) {
      mobile_request_list_.erase(request_it++);
    } else {
      ++request_it;
    }
  }

  AutoLock auto_lock(pending_request_set_lock_);
  RequestInfoSet::iterator it = pending_request_set_.begin();
  while (pending_request_set_.end() != it) {
    RequestInfoPtr request_info = (*it);
    if ((false == request_info.valid()) ||
        RequestInfo::MobileRequest != request_info->requst_type()) {
      ++it;
      continue;
    }

    if (request_info->app_id() == app_id) {
      request_info->request()->CleanUp();
      pending_request_set_.erase(it++);
    } else {
      ++it;
    }
  }

  LOG4CXX_INFO(logger_, "EXIT terminate all app requests : " << app_id
                        << " mobile_request_list_ size is : " << mobile_request_list_.size()
                        << " pending_request_set_ size is : " << pending_request_set_.size());

  UpdateTimer();
}

void RequestController::terminateAllHMIRequests() {
  LOG4CXX_AUTO_TRACE(logger_);
  AutoLock auto_lock(pending_request_set_lock_);
  RequestInfoSet::iterator it = pending_request_set_.begin();
  while (pending_request_set_.end() != it) {
    RequestInfoPtr request_info = (*it);
    if ((false == request_info.valid()) ||
        RequestInfo::HMIRequest != request_info->requst_type()) {
      ++it;
      continue;
    }
    request_info->request()->CleanUp();
    pending_request_set_.erase(it++);
    LOG4CXX_INFO(logger_, "HMI request terminated: ");
  }
}
void RequestController::terminateAllMobileRequests() {
  LOG4CXX_AUTO_TRACE(logger_);
  AutoLock auto_lock(pending_request_set_lock_);
  RequestInfoSet::iterator it = pending_request_set_.begin();
  while (pending_request_set_.end() != it) {
    RequestInfoPtr request_info = (*it);
    if ((false == request_info.valid()) ||
        RequestInfo::MobileRequest != request_info->requst_type()) {
      ++it;
      continue;
    }
    request_info->request()->CleanUp();
    LOG4CXX_INFO(logger_, "Mobile request terminated: "
                 <<  request_info->requestId());
    pending_request_set_.erase(it++);
  }
  UpdateTimer();
}

void RequestController::updateRequestTimeout(
    const uint32_t& app_id,
    const uint32_t& mobile_correlation_id,
    const uint32_t& new_timeout) {
  LOG4CXX_TRACE(logger_, " ENTER app_id : " << app_id
                << " mobile_correlation_id : " << mobile_correlation_id
                << " new_timeout : " << new_timeout);
  AutoLock auto_lock(pending_request_set_lock_);
  RequestInfoSet::iterator it = pending_request_set_.begin();
  RequestInfoPtr request_info;
  for (; pending_request_set_.end() != it; ++it) {
    request_info = *it;
    if (false == request_info.valid()) {
      LOG4CXX_ERROR(logger_, "Invalid request pointer in pending_request_set_");
      continue;
    }
    if (RequestInfo::MobileRequest != request_info->requst_type()) {
      continue;
    }
    if (app_id == request_info->app_id() &&
        mobile_correlation_id == request_info->requestId()) {
      break;
    }
  }

  if (it != pending_request_set_.end()) {
    DCHECK(request_info.valid());

    uint32_t timeout_in_seconds = new_timeout/date_time::DateTime::MILLISECONDS_IN_SECOND;
    request_info->updateTimeOut(timeout_in_seconds);
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
                  << " mobile_correlation_id " << mobile_correlation_id);
  }
}

void RequestController::OnLowVoltage() {
  LOG4CXX_AUTO_TRACE(logger_);
  is_low_voltage_ = true;
}

void RequestController::OnWakeUp() {
  LOG4CXX_AUTO_TRACE(logger_);
  terminateAllHMIRequests();
  terminateAllMobileRequests();
  is_low_voltage_ = false;
  LOG4CXX_DEBUG(logger_, "Terminate old requests done");
}

bool RequestController::IsLowVoltage() {
  LOG4CXX_TRACE(logger_, "result: " << is_low_voltage_);
  return is_low_voltage_;
}

void RequestController::onTimer() {
  AutoLock auto_lock(pending_request_set_lock_);
  LOG4CXX_TRACE(logger_, "ENTER pending_request_set_ size :"
                << pending_request_set_.size());
  RequestInfoSet::iterator probably_expired = pending_request_set_.begin();
  while (probably_expired != pending_request_set_.end()) {
    RequestInfoPtr request = *probably_expired;
    if (false == request.valid()) {
      LOG4CXX_ERROR(logger_, "Invalid pointer in pending_request_set_");
      pending_request_set_.erase(probably_expired);
      probably_expired =  pending_request_set_.begin();
      continue;
    }
    if (request->timeout_sec() == 0) {
      // FIXME(EZamakhov): inf loop on true
      LOG4CXX_DEBUG(logger_, "Ignore " << request->requestId());
      ++probably_expired;
      // This request should not be observed for TIME_OUT
      continue;
    }
    if (request->isExpired()) {
      LOG4CXX_INFO(logger_, "Timeout for "
                   << (RequestInfo::HMIRequest == request->requst_type() ? "HMI": "Mobile")
                   << " request. id: " << request->requestId()
                   << " connection_key: " << request->app_id() << " is expired");

      // Mobile Requests will  be erased by TIME_OUT response;
      request->request()->onTimeOut();
      if (RequestInfo::HMIRequest == request->requst_type()) {
        pending_request_set_.erase(probably_expired);
      }
      // If request is ersed by response probably_expired iterator is invalid
      // If request timeout updated, set probably_expired iterator is invalid too.
      probably_expired =  pending_request_set_.begin();
    } else {
      ++probably_expired;
    }
  }
  UpdateTimer();
}

RequestController::Worker::Worker(RequestController* requestController)
  : request_controller_(requestController)
  , stop_flag_(false) {
}

RequestController::Worker::~Worker() {
}

void RequestController::Worker::threadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
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

    if (request_controller_->mobile_request_list_.empty()) {
      LOG4CXX_WARN(logger_, "Mobile request list is empty");
      break;
    }

    RequestPtr request(request_controller_->mobile_request_list_.front());
    request_controller_->mobile_request_list_.pop_front();
    bool init_res = request->Init();  // to setup specific default timeout

    const uint32_t timeout_in_seconds =
        request->default_timeout() / date_time::DateTime::MILLISECONDS_IN_SECOND;
    RequestInfoPtr request_info_ptr(new MobileRequestInfo(request,
                                                          timeout_in_seconds));

    request_controller_->pending_request_set_lock_.Acquire();
    request_controller_->pending_request_set_.insert(request_info_ptr);
    if (0 != timeout_in_seconds) {
      LOG4CXX_INFO(logger_, "Execute MobileRequest corr_id = " << request_info_ptr->requestId() <<
                            " with timeout: " << timeout_in_seconds);
      request_controller_->UpdateTimer();
    } else {
      LOG4CXX_INFO(logger_, "Default timeout was set to 0."
                   "RequestController will not track timeout of this request.");
    }
    request_controller_->pending_request_set_lock_.Release();
    AutoUnlock unlock(auto_lock);

    // execute
    if ((false == request_controller_->IsLowVoltage()) &&
        request->CheckPermissions() && init_res) {
      request->Run();
    }
  }
}

void RequestController::Worker::exitThreadMain() {
  stop_flag_ = true;
  // setup stop flag and whit while threadMain will be finished correctly
  // FIXME (dchmerev@luxoft.com): There is no waiting
}

bool RequestController::CheckTimeScaleMaxRequest(
    const uint32_t& app_id,
    const uint32_t& app_time_scale,
    const uint32_t& max_request_per_time_scale) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (max_request_per_time_scale > 0
      && app_time_scale > 0) {
    AutoLock auto_lock(pending_request_set_lock_);
    TimevalStruct end = date_time::DateTime::getCurrentTime();
    TimevalStruct start = {0, 0};
    start.tv_sec = end.tv_sec - app_time_scale;

    TimeScale scale(start, end, app_id);
    const uint32_t count = std::count_if(pending_request_set_.begin(),
                                         pending_request_set_.end(), scale);
    if (count >= max_request_per_time_scale) {
      LOG4CXX_WARN(logger_, "Processing requests count " << count <<
                   " exceed application limit " << max_request_per_time_scale);
      return false;
    }
    LOG4CXX_DEBUG(logger_, "Requests count " << count);
  } else {
    LOG4CXX_DEBUG(logger_, "CheckTimeScaleMaxRequest disabled");
  }
  return true;
}

bool RequestController::CheckHMILevelTimeScaleMaxRequest(
    const mobile_apis::HMILevel::eType& hmi_level,
    const uint32_t& app_id,
    const uint32_t& app_time_scale,
    const uint32_t& max_request_per_time_scale) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (max_request_per_time_scale > 0 &&
      app_time_scale > 0) {
    AutoLock auto_lock(pending_request_set_lock_);
    TimevalStruct end = date_time::DateTime::getCurrentTime();
    TimevalStruct start = {0, 0};
    start.tv_sec = end.tv_sec - app_time_scale;

    HMILevelTimeScale scale(start, end, app_id, hmi_level);
    const uint32_t count = std::count_if(pending_request_set_.begin(),
                                         pending_request_set_.end(), scale);
    if (count >= max_request_per_time_scale) {
      LOG4CXX_WARN(logger_, "Processing requests count " << count
                   << " exceed application limit " << max_request_per_time_scale
                   << " in hmi level " << hmi_level);
      return false;
    }
    LOG4CXX_DEBUG(logger_, "Requests count " << count);
  } else {
    LOG4CXX_DEBUG(logger_, "CheckHMILevelTimeScaleMaxRequest disabled");
  }
  return true;
}

bool RequestController::CheckPendingRequestsAmount(
    const uint32_t& pending_requests_amount) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (pending_requests_amount > 0) {
    AutoLock auto_lock(mobile_request_list_lock_);
    const size_t pending_requests_size = mobile_request_list_.size();
    const bool available_to_add =
        pending_requests_amount > pending_requests_size;
    if(!available_to_add) {
      LOG4CXX_WARN(logger_, "Pending requests count " << pending_requests_size
                   << " exceed application limit " << pending_requests_amount);
    }
    return available_to_add;
  }
  LOG4CXX_DEBUG(logger_, "CheckPendingRequestsAmount disabled");
  return true;
}

void RequestController::UpdateTimer() {
  LOG4CXX_AUTO_TRACE(logger_);
  if(pending_request_set_.empty()) {
    LOG4CXX_DEBUG(logger_, "Sleep for default sleep time "
                 << dafault_sleep_time_ << " secs");
    timer_.updateTimeOut(dafault_sleep_time_);
    return;
  }

  RequestInfoSet::iterator it = pending_request_set_.begin();
  while (it != pending_request_set_.end()) {
    RequestInfoPtr request = *it;
    DCHECK(request.valid());
    // This request should not be observed for TIME_OUT
    if (0 != request->timeout_sec()) {
      const TimevalStruct current_time = date_time::DateTime::getCurrentTime();
      const TimevalStruct end_time = request->end_time();
      if (current_time < end_time) {
        const uint64_t secs = end_time.tv_sec - current_time.tv_sec;
        LOG4CXX_DEBUG(logger_, "Sleep for " << secs << " secs");
        // Timeout for bigger than 5 minutes is a mistake
        DCHECK(secs < 300);
        timer_.updateTimeOut(secs);
      } else {
        LOG4CXX_DEBUG(logger_, "Request is expired: "
                      << end_time.tv_sec << " - "
                      << current_time.tv_sec << " >= "
                      << request->timeout_sec());
        timer_.updateTimeOut(0);
      }
      return;
    }
    ++it;
  }
}

}  //  namespace request_controller
}  //  namespace application_manager
