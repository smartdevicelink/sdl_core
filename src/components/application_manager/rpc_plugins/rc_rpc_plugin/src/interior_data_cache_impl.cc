/*
 * Copyright (c) 2018, Ford Motor Company
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

#include <iostream>
#include <thread>
#include <chrono>
#include "rc_rpc_plugin/interior_data_cache_impl.h"
#include "utils/date_time.h"
#include "utils/logger.h"

namespace rc_rpc_plugin {

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule");

InteriorDataCacheImpl::InteriorDataCacheImpl(
    const uint32_t time_frame_of_allowed_requests)
    : reset_request_count_timer_(
          "InteriorDataCacheImplResetRequests",
          new timer::TimerTaskImpl<InteriorDataCacheImpl>(
              this, &InteriorDataCacheImpl::ResetRequestCountOnTimer)) {
  StartRequestResetTimer(time_frame_of_allowed_requests);
}

InteriorDataCacheImpl::~InteriorDataCacheImpl() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!reset_request_count_timer_.is_running()) {
    reset_request_count_timer_.Stop();
  }
}

/**
 * @brief MergeModuleData key all keys and values from first parameter and
 * update and append keys and values from the second
 * @param data1 - initial data
 * @param data2 - updated data
 * @return updated data1 with data2 keys and values
 */
smart_objects::SmartObject MergeModuleData(
    const smart_objects::SmartObject& data1,
    const smart_objects::SmartObject& data2) {
  smart_objects::SmartObject result = data1;
  auto it = data2.map_begin();
  for (; it != data2.map_end(); ++it) {
    const std::string& key = it->first;
    const smart_objects::SmartObject& value = it->second;
    result[key] = value;
  }
  return result;
}

void InteriorDataCacheImpl::Add(const std::string& module_type,
                                const smart_objects::SmartObject& module_data) {
  LOG4CXX_TRACE(logger_, "module_type : " << module_type);
  sync_primitives::AutoLock autolock(subscriptions_lock_);
  auto it = subscriptions_.find(module_type);
  if (subscriptions_.end() == it) {
    subscriptions_[module_type] = module_data;
    return;
  }
  subscriptions_[module_type] = MergeModuleData(it->second, module_data);
}

smart_objects::SmartObject InteriorDataCacheImpl::Retrieve(
    const std::string& module_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(subscriptions_lock_);
  auto it = subscriptions_.find(module_type);
  if (it == subscriptions_.end()) {
    LOG4CXX_WARN(logger_,
                 "Module type " << module_type << " was not found in cache");
    return smart_objects::SmartObject(smart_objects::SmartType_Null);
  }
  LOG4CXX_TRACE(logger_, "module_type : " << module_type);
  return it->second;
}

bool InteriorDataCacheImpl::Contains(const std::string& module_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(subscriptions_lock_);
  auto it = subscriptions_.find(module_type);
  const bool contains = it != subscriptions_.end();
  LOG4CXX_TRACE(logger_,
                "module_type : " << module_type << " "
                                 << (contains ? "true" : "false"));
  return contains;
}

void InteriorDataCacheImpl::Remove(const std::string& module_type) {
  LOG4CXX_TRACE(logger_, "module_type : " << module_type);
  sync_primitives::AutoLock autolock(subscriptions_lock_);
  auto it = subscriptions_.find(module_type);
  if (subscriptions_.end() == it) {
    LOG4CXX_TRACE(logger_, "Not existing module_type : " << module_type);
    return;
  }
  subscriptions_.erase(it);
}

void InteriorDataCacheImpl::ClearCache() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(subscriptions_lock_);
  subscriptions_.clear();
}

void InteriorDataCacheImpl::ResetRequestCountOnTimer() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(amount_of_requests_lock_);
  amount_of_request_in_this_time_frame_.clear();
}

void InteriorDataCacheImpl::StartRequestResetTimer(
    const uint32_t time_frame_of_allowed_requests) {
  if (!reset_request_count_timer_.is_running()) {
    reset_request_count_timer_.Stop();
  }

  time_frame_of_allowed_requests_ = time_frame_of_allowed_requests;
  reset_request_count_timer_.Start(
      time_frame_of_allowed_requests_ *
          date_time::DateTime::MILLISECONDS_IN_SECOND,
      timer::kPeriodic);
}

uint32_t InteriorDataCacheImpl::GetCurrentAmountOfRequests(
    const std::string& module_type) const {
  sync_primitives::AutoLock autolock(amount_of_requests_lock_);
  auto it = amount_of_request_in_this_time_frame_.find(module_type);
  auto amount =
      amount_of_request_in_this_time_frame_.end() != it ? it->second : 0;
  return amount;
}

void InteriorDataCacheImpl::IncrementAmountOfRequests(
    const std::string& module_type) {
  sync_primitives::AutoLock autolock(amount_of_requests_lock_);
  (amount_of_request_in_this_time_frame_[module_type])++;
}
}
