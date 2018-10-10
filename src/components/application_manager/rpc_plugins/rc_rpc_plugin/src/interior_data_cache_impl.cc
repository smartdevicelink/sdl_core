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
#include "application_manager/smart_object_keys.h"
#include "utils/date_time.h"
#include "utils/logger.h"

namespace rc_rpc_plugin {

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule");

InteriorDataCacheImpl::InteriorDataCacheImpl() {}

InteriorDataCacheImpl::~InteriorDataCacheImpl() {}

/**
 * @brief MergeModuleData key all keys and values from first parameter and
 * update and append keys and values from the second
 * @param data1 - initial data
 * @param data2 - updated data
 * @return updated data1 with data2 keys and values
 */
smart_objects::SmartObject MergeModuleData(
    const smart_objects::SmartObject& data1,
    const smart_objects::SmartObject& data2);

smart_objects::SmartObject MergeArray(const smart_objects::SmartObject& data1,
                                      const smart_objects::SmartObject& data2);

smart_objects::SmartObject MergeModuleData(
    const smart_objects::SmartObject& data1,
    const smart_objects::SmartObject& data2) {
  smart_objects::SmartObject result = data1;
  auto it = data2.map_begin();
  for (; it != data2.map_end(); ++it) {
    const std::string& key = it->first;
    const smart_objects::SmartObject& value = it->second;
    // Merge maps and arrays with `id` param included, replace other types
    if (result.keyExists(key) && value.getType() == result[key].getType()) {
      if (value.getType() == smart_objects::SmartType::SmartType_Map) {
        result[key] = MergeModuleData(result[key], value);
      } else if (value.getType() == smart_objects::SmartType::SmartType_Array) {
        result[key] = MergeArray(result[key], value);
      } else {
        result[key] = value;
      }
    } else {
      result[key] = value;
    }
  }
  return result;
}

smart_objects::SmartObject MergeArray(const smart_objects::SmartObject& data1,
                                      const smart_objects::SmartObject& data2) {
  // Merge data only in the case where each value in the array is an Object with
  // an ID included, otherwise replace
  if (!data2.empty() &&
      data2.getElement(0).getType() ==
          smart_objects::SmartType::SmartType_Map &&
      data2.getElement(0).keyExists(application_manager::strings::id)) {
    smart_objects::SmartObject result = data1;
    smart_objects::SmartArray* result_array = result.asArray();
    smart_objects::SmartArray* data_array = data2.asArray();
    auto data_it = data_array->begin();
    for (; data_it != data_array->end(); ++data_it) {
      auto result_it =
          std::find_if(result_array->begin(),
                       result_array->end(),
                       [data_it](smart_objects::SmartObject& obj) -> bool {
                         return obj[application_manager::strings::id] ==
                                (*data_it)[application_manager::strings::id];
                       });

      if (result_it != result_array->end()) {
        *result_it = MergeModuleData(*result_it, *data_it);
      } else {
        result_array->push_back(*data_it);
      }
    }
    return result;
  }
  return data2;
}

void InteriorDataCacheImpl::Add(const std::string& module_type,
                                const smart_objects::SmartObject& module_data) {
  LOG4CXX_TRACE(logger_, "module_type : " << module_type);
  sync_primitives::AutoLock autolock(cached_data_lock_);
  auto it = cached_data_.find(module_type);
  if (cached_data_.end() == it) {
    cached_data_[module_type] = module_data;
    return;
  }
  cached_data_[module_type] = MergeModuleData(it->second, module_data);
}

smart_objects::SmartObject InteriorDataCacheImpl::Retrieve(
    const std::string& module_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(cached_data_lock_);
  auto it = cached_data_.find(module_type);
  if (it == cached_data_.end()) {
    LOG4CXX_WARN(logger_,
                 "Module type " << module_type << " was not found in cache");
    return smart_objects::SmartObject(smart_objects::SmartType_Null);
  }
  LOG4CXX_TRACE(logger_, "module_type : " << module_type);
  return it->second;
}

bool InteriorDataCacheImpl::Contains(const std::string& module_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(cached_data_lock_);
  auto it = cached_data_.find(module_type);
  const bool contains = it != cached_data_.end();
  LOG4CXX_TRACE(logger_,
                "module_type : " << module_type << " "
                                 << (contains ? "true" : "false"));
  return contains;
}

void InteriorDataCacheImpl::Remove(const std::string& module_type) {
  LOG4CXX_TRACE(logger_, "module_type : " << module_type);
  sync_primitives::AutoLock autolock(cached_data_lock_);
  auto it = cached_data_.find(module_type);
  if (cached_data_.end() == it) {
    LOG4CXX_TRACE(logger_, "Not existing module_type : " << module_type);
    return;
  }
  cached_data_.erase(it);
}

void InteriorDataCacheImpl::Clear() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(cached_data_lock_);
  cached_data_.clear();
}
}
