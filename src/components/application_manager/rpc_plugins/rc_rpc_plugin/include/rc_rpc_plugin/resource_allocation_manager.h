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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RESOURCE_ALLOCATION_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RESOURCE_ALLOCATION_MANAGER_H_
#include <string>
#include "utils/macro.h"

#include "application_manager/plugin_manager/rpc_plugin.h"
#include "interfaces/HMI_API.h"
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
 * Defines triggers for OnRCStatus notification sending
 */
namespace NotificationTrigger {
/**
 * @brief The eType
 * APP_REGISTRATION RC app registation event
 * RC_STATE_CHANGING enabling/disabling RC on HMI event
 * MODULE_ALLOCATION module allocation/deallocation event
 */
enum eType { APP_REGISTRATION = 0, MODULE_ALLOCATION, RC_STATE_CHANGING };
}  // namespace NotificationTrigger

/**
 * Defines result of releasing specified module type resource.
 */
namespace ResourceReleasedState {
/**
 * NOT_ALLOCATED Module's resource is not allocated
 * IS_ALLOCATED Module's resource is already allocated by
 * different application
 * IS_RELEASED Module's resource is released.
 */
enum eType { NOT_ALLOCATED = 0, IS_ALLOCATED, IS_RELEASED };
}  // namespace ResourceReleasedState

/**
 * @brief ModuleUid uniquely identify a module
 * moduleType + moduleID
 */
typedef std::pair<std::string, std::string> ModuleUid;

/**
 * @brief Resources defines list of resources
 */
typedef std::vector<ModuleUid> Resources;

class ResourceAllocationManager {
 public:
  /**
   * @brief AcquireResource acquires resource by application
   * @param module_type resource to acquire
   * @param module_id uuid of a resource
   * @param app_id application that acquire resource
   * @return ALLOWED if resource acquired \
   *         IN_USE  if resource already acquired
   *         ASK_DRIVER if driver confirmation is required
   */
  virtual AcquireResult::eType AcquireResource(const std::string& module_type,
                                               const std::string& module_id,
                                               const uint32_t app_id) = 0;

  /**
   * @brief SetResourceState changes resource state. Resource must be acquired
   * beforehand.
   * @param module_type Resource to change its state
   * @param module_id uuid of a resource
   * @param app_id Application aquired resource before
   * @param state State to set for resource
   */
  virtual void SetResourceState(const std::string& module_type,
                                const std::string& module_id,
                                const uint32_t app_id,
                                const ResourceState::eType state) = 0;

  /**
   * @brief IsResourceFree check resource state
   * @param module_type Resource name
   * @param module_id uuid of a resource
   * @return True if free, otherwise - false
   */
  virtual bool IsResourceFree(const std::string& module_type,
                              const std::string& module_id) const = 0;

  /**
   * @brief IsResourceAllocated check if module is allocated by certain
   * application
   * @param module_type module to be checked
   * @param module_id uuid of a resource to be checked
   * @param app_id app to be checked
   * @return true if module_type is allocated by application with provided
   * app_id
   */
  virtual bool IsResourceAllocated(const std::string& module_type,
                                   const std::string& module_id,
                                   const uint32_t app_id) = 0;

  /**
   * @brief AcquireResource forces acquiring resource by application
   * @param module_type resource to acquire
   * @param module_id uuid of a resource
   * @param app_id application that acquire resource
   */
  virtual void ForceAcquireResource(const std::string& module_type,
                                    const std::string& module_id,
                                    const uint32_t app_id) = 0;

  /**
   * @brief OnDriverDisallowed callback for rejecting acquiring resource
   * @param module_type resource type
   * @param module_id uuid of a resource
   * @param app_id application id
   */
  virtual void OnDriverDisallowed(const std::string& module_type,
                                  const std::string& module_id,
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

  /**
   * @brief Create and send OnRCStatusNotification to mobile and HMI
   * @param event trigger for notification sending
   * @param application - app that should receive notification
   * in case of registration; in cases of RC enabling/disabling
   * or module allocation - application is just empty shared ptr,
   * because in these cases all registered RC apps should
   * receive a notification
   */
  virtual void SendOnRCStatusNotifications(
      NotificationTrigger::eType event,
      application_manager::ApplicationSharedPtr application) = 0;

  /**
   * @brief Returns current state of RC functionality
   * @return current state of RC functionality
   */
  virtual bool is_rc_enabled() const = 0;

  /**
   * @brief Sets current state of RC functionality to a new one
   * @param value new RC functionality state
   */
  virtual void set_rc_enabled(const bool value) = 0;

  /**
   * @brief ReleaseResource Releases resource acquired by application
   * @param module_type Module name
   * @param module_id uuid of a module
   * @param application_id Application id
   */
  virtual ResourceReleasedState::eType ReleaseResource(
      const std::string& module_type,
      const std::string& module_id,
      const uint32_t application_id) = 0;

  /**
   * @brief SetResourceAquired mark resourse as aquired and process logic of
   * changing state of aquired resources
   * @param module_type resource name
   * @param module_id uuid of a resource
   * @param app applicastion that aquire resource
   */
  virtual void SetResourceAcquired(const std::string& module_type,
                                   const std::string& module_id,
                                   const uint32_t app_id) = 0;

  /**
   * @brief Checks if specific resource is already aquired by specific
   * application
   * @param moduleUid Module resurce (module type + module ID)
   * @param app_id Application ID which try to aquire resource
   * @return true In case when resource is already aquired by specific
   * application
   * @return false In case when isn't aquired by specific
   * application
   */
  virtual bool IsResourceAlreadyAcquiredByApp(
      const rc_rpc_plugin::ModuleUid& moduleUid,
      const uint32_t app_id) const = 0;

  virtual ~ResourceAllocationManager() {}
};

}  // namespace rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RESOURCE_ALLOCATION_MANAGER_H_
