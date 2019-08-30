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
#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_CACHE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_CACHE_H_
#include <string>
#include "smart_objects/smart_object.h"

namespace rc_rpc_plugin {

/**
 * @brief ModuleUid uniquely identify a module
 * moduleType + moduleID
 */
typedef std::pair<std::string, std::string> ModuleUid;

/**
 * @brief The InteriorDataCache interface for caching data class
 * Provide ability to cache module data by module type name and module id
 * and clear cache
 */
class InteriorDataCache {
 public:
  /**
   * @brief Class destructor
   */
  virtual ~InteriorDataCache() {}

  /**
   * @brief Add module data to cache
   * @param module module type + module id
   * @param module_data data to be cached
   */
  virtual void Add(const ModuleUid& module,
                   const smart_objects::SmartObject& module_data) = 0;

  /**
   * @brief Retrieve Get cached data
   * @param module data type to get from cache
   * @return smart object with cached data, or nulll smart object
   */
  virtual smart_objects::SmartObject Retrieve(
      const ModuleUid& module) const = 0;

  virtual std::vector<ModuleUid> GetCachedModulesByType(
      const std::string& module_type) const = 0;

  /**
   * @brief Contains check if data exists in cache
   * @param module module name + module id to check in cache
   * @return true if cached, false otherwize
   */
  virtual bool Contains(const ModuleUid& module) const = 0;

  /**
   * @brief Remove cached data
   * @param module data type to remove from cache
   */
  virtual void Remove(const ModuleUid& module) = 0;

  /**
   * @brief Clear clear all cached data
   */
  virtual void Clear() = 0;
};
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_CACHE_H_
