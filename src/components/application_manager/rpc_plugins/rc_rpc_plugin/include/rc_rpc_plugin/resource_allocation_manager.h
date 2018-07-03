/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_H
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_H
#include <string>
#include "utils/macro.h"
#include "utils/shared_ptr.h"
#include "interfaces/HMI_API.h"
#include "application_manager/plugin_manager/rpc_plugin.h"
#include "rc_rpc_plugin/rc_app_extension.h"

namespace rc_rpc_plugin {

/**
 * Enum for list of results of allocation resources
 */
namespace AcquireResult {
enum eType { ALLOWED = 0, IN_USE, ASK_DRIVER, REJECTED };
}

/**
 * Defines states of acquired resource
 */
namespace ResourceState {
enum eType { FREE = 0, BUSY };
}

/**
 * @brief Resources defines list of resources
 */
typedef std::vector<std::string> Resources;

class ResourceAllocationManager {
 public:
  /**
   * @brief AcquireResource acquires resource by application
   * @param module_type resource to acquire
   * @param app_id application that acquire resource
   * @return ALLOWED if resource acquired \
   *         IN_USE  if resource already acquired
   *         ASK_DRIVER if driver confirmation is required
   */
  virtual AcquireResult::eType AcquireResource(const std::string& module_type,
                                               const uint32_t app_id) = 0;

  /**
   * @brief SetResourceState changes resource state. Resource must be acquired
   * beforehand.
   * @param module_type Resource to change its state
   * @param app_id Application aquired resource before
   * @param state State to set for resource
   */
  virtual void SetResourceState(const std::string& module_type,
                                const uint32_t app_id,
                                const ResourceState::eType state) = 0;

  /**
   * @brief IsResourceFree check resource state
   * @param module_type Resource name
   * @return True if free, otherwise - false
   */
  virtual bool IsResourceFree(const std::string& module_type) const = 0;

  /**
   * @brief AcquireResource forces acquiring resource by application
   * @param module_type resource to acquire
   * @param app_id application that acquire resource
   */
  virtual void ForceAcquireResource(const std::string& module_type,
                                    const uint32_t app_id) = 0;

  /**
   * @brief OnDriverDisallowed callback for rejecting acquiring resource
   * @param module_type resource type
   * @param app_id application id
   */
  virtual void OnDriverDisallowed(const std::string& module_type,
                                  const uint32_t app_id) = 0;

  /**
   * @brief OnApplicationEvent Processes application related events
   * @param event Event
   * @param application Pointer to application struct
   */
  virtual void OnApplicationEvent(
      application_manager::plugin_manager::ApplicationEvent event,
      application_manager::ApplicationSharedPtr application) = 0;

  /**
   * @brief OnPolicyEvent Processes policy related events
   * @param event Policy event
   */
  virtual void OnPolicyEvent(
      application_manager::plugin_manager::PolicyEvent event) = 0;

  /**
   * @brief Set current access mode for acquiring resource
   * @param access_mode
   */
  virtual void SetAccessMode(
      const hmi_apis::Common_RCAccessMode::eType access_mode) = 0;

  /**
   * @brief Get last set access mode for acquiring resource
   * @param access_mode
   */
  virtual hmi_apis::Common_RCAccessMode::eType GetAccessMode() const = 0;

  /**
   * @brief Remove all information about all allocations
   */
  virtual void ResetAllAllocations() = 0;

  virtual ~ResourceAllocationManager() {}
};

}  // namespace rc_rpc_plugin
#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_H
