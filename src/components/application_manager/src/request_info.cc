/*
* \file request_info.h
* \brief request information structure source file.
*
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
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include "application_manager/request_info.h"

#include <algorithm>
namespace application_manager {

namespace request_controller {

SDL_CREATE_LOG_VARIABLE("RequestController")

constexpr uint32_t RequestInfo::kHmiConnectionKey;

HMIRequestInfo::HMIRequestInfo(RequestPtr request, const uint64_t timeout_msec)
    : RequestInfo(request, HMIRequest, timeout_msec) {
  correlation_id_ = request_->correlation_id();
  app_id_ = RequestInfo::kHmiConnectionKey;
}

HMIRequestInfo::HMIRequestInfo(RequestPtr request,
                               const date_time::TimeDuration& start_time,
                               const uint64_t timeout_msec)
    : RequestInfo(request, HMIRequest, start_time, timeout_msec) {
  correlation_id_ = request_->correlation_id();
  app_id_ = RequestInfo::kHmiConnectionKey;
}

MobileRequestInfo::MobileRequestInfo(RequestPtr request,
                                     const uint64_t timeout_msec)
    : RequestInfo(request, MobileRequest, timeout_msec) {
  correlation_id_ = request_.get()->correlation_id();
  app_id_ = request_.get()->connection_key();
}

MobileRequestInfo::MobileRequestInfo(RequestPtr request,
                                     const date_time::TimeDuration& start_time,
                                     const uint64_t timeout_msec)
    : RequestInfo(request, MobileRequest, start_time, timeout_msec) {
  correlation_id_ = request_.get()->correlation_id();
  app_id_ = request_.get()->connection_key();
}

RequestInfo::RequestInfo(RequestPtr request,
                         const RequestInfo::RequestType request_type,
                         const date_time::TimeDuration& start_time,
                         const uint64_t timeout_msec)
    : request_(request), start_time_(start_time), timeout_msec_(timeout_msec) {
  updateEndTime();
  request_type_ = request_type;
  correlation_id_ = request_->correlation_id();
  app_id_ = request_->connection_key();
}

void application_manager::request_controller::RequestInfo::updateEndTime() {
  end_time_ = date_time::getCurrentTime();
  date_time::AddMilliseconds(end_time_, timeout_msec_);
}

void RequestInfo::updateTimeOut(const uint64_t& timeout_msec) {
  timeout_msec_ = timeout_msec;
  updateEndTime();
  request_->OnUpdateTimeOut();
}

bool RequestInfo::isExpired() {
  date_time::TimeDuration curr_time = date_time::getCurrentTime();
  return date_time::getmSecs(end_time_) <= date_time::getmSecs(curr_time);
}

uint64_t RequestInfo::hash() {
  return GenerateHash(app_id(), requestId());
}

uint64_t RequestInfo::GenerateHash(uint32_t var1, uint32_t var2) {
  uint64_t hash_result = 0;
  hash_result = var1;
  hash_result = hash_result << 32;
  hash_result = hash_result | var2;
  return hash_result;
}

FakeRequestInfo::FakeRequestInfo(uint32_t app_id, uint32_t correaltion_id) {
  app_id_ = app_id;
  correlation_id_ = correaltion_id;
}

RequestInfoSet::~RequestInfoSet() {
  sync_primitives::AutoLock lock(pending_requests_lock_);
  auto it = time_sorted_pending_requests_.begin();
  while (!time_sorted_pending_requests_.empty()) {
    Erase(*it);
    it = time_sorted_pending_requests_.begin();
  }
}

bool RequestInfoSet::Add(RequestInfoPtr request_info) {
  DCHECK_OR_RETURN(request_info, false);
  SDL_LOG_DEBUG("Add request app_id = " << request_info->app_id()
                                        << "; corr_id = "
                                        << request_info->requestId());
  sync_primitives::AutoLock lock(pending_requests_lock_);
  CheckSetSizes();
  const std::pair<HashSortedRequestInfoSet::iterator, bool>& insert_result =
      hash_sorted_pending_requests_.insert(request_info);
  if (insert_result.second == true) {
    const std::pair<TimeSortedRequestInfoSet::iterator, bool>& insert_result =
        time_sorted_pending_requests_.insert(request_info);
    DCHECK(insert_result.second);
    if (!insert_result.second) {
      return false;
    }
    CheckSetSizes();
    return true;
  } else {
    SDL_LOG_ERROR("Request with app_id = "
                  << request_info->app_id() << "; corr_id "
                  << request_info->requestId() << " Already exist ");
  }
  CheckSetSizes();
  return false;
}

RequestInfoPtr RequestInfoSet::Find(const uint32_t connection_key,
                                    const uint32_t correlation_id) const {
  RequestInfoPtr result;

  // Request info for searching in request info set by log_n time
  std::shared_ptr<FakeRequestInfo> request_info_for_search(
      new FakeRequestInfo(connection_key, correlation_id));

  sync_primitives::AutoLock lock(pending_requests_lock_);
  HashSortedRequestInfoSet::iterator it =
      hash_sorted_pending_requests_.find(request_info_for_search);
  if (it != hash_sorted_pending_requests_.end()) {
    result = *it;
  }
  return result;
}

RequestInfoPtr RequestInfoSet::Front() {
  RequestInfoPtr result;

  sync_primitives::AutoLock lock(pending_requests_lock_);
  TimeSortedRequestInfoSet::iterator it = time_sorted_pending_requests_.begin();
  if (it != time_sorted_pending_requests_.end()) {
    result = *it;
  }
  return result;
}

RequestInfoPtr RequestInfoSet::FrontWithNotNullTimeout() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(pending_requests_lock_);
  RequestInfoPtr result;
  TimeSortedRequestInfoSet::iterator it = time_sorted_pending_requests_.begin();
  while (it != time_sorted_pending_requests_.end()) {
    RequestInfoPtr tmp = *it;
    if (0 == tmp->timeout_msec()) {
      ++it;
    } else {
      result = tmp;
      it = time_sorted_pending_requests_.end();
    }
  }
  return result;
}

std::list<RequestInfoPtr> RequestInfoSet::GetRequestsByConnectionKey(
    const uint32_t connection_key) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(pending_requests_lock_);

  std::list<RequestInfoPtr> output_list;
  AppIdCompararator comparator(AppIdCompararator::Equal, connection_key);

  std::copy_if(hash_sorted_pending_requests_.begin(),
               hash_sorted_pending_requests_.end(),
               std::back_inserter(output_list),
               comparator);

  return output_list;
}

bool RequestInfoSet::Erase(const RequestInfoPtr request_info) {
  DCHECK(request_info);
  if (!request_info) {
    SDL_LOG_ERROR("NULL ponter request_info");
    return false;
  }
  CheckSetSizes();

  size_t erased_count = hash_sorted_pending_requests_.erase(request_info);
  DCHECK((erased_count <= 1));
  if (1 == erased_count) {
    TimeSortedRequestInfoSet::iterator it =
        time_sorted_pending_requests_.find(request_info);
    DCHECK(it != time_sorted_pending_requests_.end());
    if (it == time_sorted_pending_requests_.end()) {
      SDL_LOG_ERROR("Can't find request in time_sorted_requests_");
      return false;
    }
    const RequestInfoPtr found = *it;
    DCHECK(request_info == found);
    time_sorted_pending_requests_.erase(it);
    CheckSetSizes();
    return true;
  }
  CheckSetSizes();
  return false;
}

bool RequestInfoSet::RemoveRequest(const RequestInfoPtr request_info) {
  sync_primitives::AutoLock lock(pending_requests_lock_);
  return Erase(request_info);
}

uint32_t RequestInfoSet::RemoveRequests(
    const RequestInfoSet::AppIdCompararator& filter) {
  SDL_LOG_AUTO_TRACE();
  uint32_t erased = 0;

  sync_primitives::AutoLock lock(pending_requests_lock_);
  HashSortedRequestInfoSet::iterator it =
      std::find_if(hash_sorted_pending_requests_.begin(),
                   hash_sorted_pending_requests_.end(),
                   filter);
  while (it != hash_sorted_pending_requests_.end()) {
    HashSortedRequestInfoSet::iterator to_erase = it++;
    Erase(*to_erase);
    it = std::find_if(it, hash_sorted_pending_requests_.end(), filter);
    ++erased;
  }
  CheckSetSizes();
  return erased;
}

uint32_t RequestInfoSet::RemoveByConnectionKey(uint32_t connection_key) {
  SDL_LOG_AUTO_TRACE();
  return RemoveRequests(
      AppIdCompararator(AppIdCompararator::Equal, connection_key));
}

uint32_t RequestInfoSet::RemoveMobileRequests() {
  SDL_LOG_AUTO_TRACE();
  return RemoveRequests(AppIdCompararator(AppIdCompararator::NotEqual,
                                          RequestInfo::kHmiConnectionKey));
}

const size_t RequestInfoSet::Size() {
  sync_primitives::AutoLock lock(pending_requests_lock_);
  CheckSetSizes();
  return time_sorted_pending_requests_.size();
}

void RequestInfoSet::CheckSetSizes() {
  const ssize_t time_set_size = time_sorted_pending_requests_.size();
  const ssize_t hash_set_size = hash_sorted_pending_requests_.size();
  const bool set_sizes_equal = (time_set_size == hash_set_size);
  DCHECK(set_sizes_equal);
}

bool RequestInfoSet::AppIdCompararator::operator()(
    const RequestInfoPtr value_compare) const {
  switch (compare_type_) {
    case Equal:
      return value_compare->app_id() == app_id_;
    case NotEqual:
      return value_compare->app_id() != app_id_;
    default:
      return false;
  }
}

bool RequestInfoTimeComparator::operator()(const RequestInfoPtr lhs,
                                           const RequestInfoPtr rhs) const {
  date_time::TimeCompare compare_result =
      date_time::compareTime(lhs->end_time(), rhs->end_time());
  if (compare_result == date_time::LESS) {
    return true;
  } else if (compare_result == date_time::GREATER) {
    return false;
  }
  // compare_result == date_time::EQUAL
  // If time is equal, sort by hash
  return lhs->hash() < rhs->hash();
}

bool RequestInfoHashComparator::operator()(const RequestInfoPtr lhs,
                                           const RequestInfoPtr rhs) const {
  return lhs->hash() < rhs->hash();
}

}  // namespace request_controller

}  // namespace application_manager
