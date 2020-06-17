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

#include "rc_rpc_plugin/interior_data_cache_impl.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "application_manager/smart_object_keys.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "utils/date_time.h"
#include "utils/logger.h"

namespace rc_rpc_plugin {

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule");

InteriorDataCacheImpl::InteriorDataCacheImpl() {}

InteriorDataCacheImpl::~InteriorDataCacheImpl() {}

void InteriorDataCacheImpl::Add(const ModuleUid& module,
                                const smart_objects::SmartObject& module_data) {
  LOG4CXX_TRACE(
      logger_,
      "module_type : " << module.first << " module_id : " << module.second);
  sync_primitives::AutoLock autolock(cached_data_lock_);
  auto it = cached_data_.find(module);
  if (cached_data_.end() == it) {
    cached_data_[module] = module_data;
    return;
  }
  cached_data_[module] = RCHelpers::MergeModuleData(it->second, module_data);
}

smart_objects::SmartObject InteriorDataCacheImpl::Retrieve(
    const ModuleUid& module) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(cached_data_lock_);
  auto it = cached_data_.find(module);
  if (it == cached_data_.end()) {
    LOG4CXX_WARN(logger_,
                 "Module with type: " << module.first
                                      << " and id: " << module.second
                                      << " was not found in cache");
    return smart_objects::SmartObject(smart_objects::SmartType_Null);
  }
  LOG4CXX_TRACE(
      logger_,
      "module_type : " << module.first << " module_id : " << module.second);
  return it->second;
}

std::vector<ModuleUid> InteriorDataCacheImpl::GetCachedModulesByType(
    const std::string& module_type) const {
  std::vector<ModuleUid> modules;
  for (auto& item : cached_data_) {
    auto& module = item.first;
    if (module_type == module.first) {
      modules.push_back(module);
    }
  }
  return modules;
}

bool InteriorDataCacheImpl::Contains(const ModuleUid& module) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(cached_data_lock_);
  auto it = cached_data_.find(module);
  const bool contains = it != cached_data_.end();
  LOG4CXX_TRACE(logger_,
                "module_type : " << module.first
                                 << " module_id : " << module.second << " "
                                 << (contains ? "true" : "false"));
  return contains;
}

void InteriorDataCacheImpl::Remove(const ModuleUid& module) {
  LOG4CXX_TRACE(
      logger_,
      "module_type : " << module.first << " module_id : " << module.second);
  sync_primitives::AutoLock autolock(cached_data_lock_);
  auto it = cached_data_.find(module);
  if (cached_data_.end() == it) {
    LOG4CXX_TRACE(
        logger_,
        "Not existing module : " << module.first << " " << module.second);
    return;
  }
  cached_data_.erase(it);
}

void InteriorDataCacheImpl::Clear() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(cached_data_lock_);
  cached_data_.clear();
}
}  // namespace rc_rpc_plugin
