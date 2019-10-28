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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RESOURCE_ALLOCATION_MANAGER_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RESOURCE_ALLOCATION_MANAGER_IMPL_H_
#include "application_manager/application_impl.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "rc_rpc_plugin/rc_capabilities_manager.h"
#include "rc_rpc_plugin/resource_allocation_manager.h"
#include "utils/lock.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {

typedef std::vector<application_manager::ApplicationSharedPtr> Apps;

class ResourceAllocationManagerImpl : public ResourceAllocationManager {
 public:
  ResourceAllocationManagerImpl(
      application_manager::ApplicationManager& app_mngr,
      application_manager::rpc_service::RPCService& rpc_service,
      rc_rpc_plugin::RCCapabilitiesManager& rc_capabilities_manager);

  ~ResourceAllocationManagerImpl();

  /**
   * @brief AcquireResource forces acquiring resource by application
   * @param module_type resource to acquire
   * @param module_id uuid of a resource
   * @param app_id application that acquire resourc
   * @return result of acauiring resources
   */
  AcquireResult::eType AcquireResource(const std::string& module_type,
                                       const std::string& module_id,
                                       const uint32_t app_id) OVERRIDE FINAL;
  /**
   * @brief ForceAcquireResource forces acquiring resource by application
   * @param module_type resource to acquire
   * @param module_id uuid of a resource
   * @param app_id application that acquire resource
   */
  void ForceAcquireResource(const std::string& module_type,
                            const std::string& module_id,
                            const uint32_t app_id) FINAL;

  /**
   * @brief SetResourceState changes resource state. Resource must be acquired
   * beforehand.
   * @param module_type Resource to change its state
   * @param module_id uuid of a resource
   * @param app_id Application aquired resource before
   * @param state State to set for resource
   */
  void SetResourceState(const std::string& module_type,
                        const std::string& module_id,
                        const uint32_t app_id,
                        const ResourceState::eType state) FINAL;

  /**
   * @brief IsResourceFree check resource state
   * @param module_type Resource name
   * @param module_id uuid of a resource
   * @return True if free, otherwise - false
   */
  bool IsResourceFree(const std::string& module_type,
                      const std::string& module_id) const FINAL;

  void SetAccessMode(
      const hmi_apis::Common_RCAccessMode::eType access_mode) FINAL;

  hmi_apis::Common_RCAccessMode::eType GetAccessMode() const FINAL;

  void OnDriverDisallowed(const std::string& module_type,
                          const std::string& module_id,
                          const uint32_t app_id) FINAL;

  /**
   * @brief OnApplicationEvent Notifies modules on certain application events
   * @param event Event
   * @param application Pointer to application struct
   */
  void OnApplicationEvent(
      application_manager::plugin_manager::ApplicationEvent event,
      application_manager::ApplicationSharedPtr application) FINAL;

  /**
   * @brief OnPolicyEvent Processes policy related events
   * @param event Policy event
   */
  void OnPolicyEvent(
      application_manager::plugin_manager::PolicyEvent event) FINAL;

  void ResetAllAllocations() FINAL;

  void SendOnRCStatusNotifications(
      NotificationTrigger::eType event,
      application_manager::ApplicationSharedPtr application) FINAL;

  bool is_rc_enabled() const FINAL;

  void set_rc_enabled(const bool value) FINAL;

  ResourceReleasedState::eType ReleaseResource(
      const std::string& module_type,
      const std::string& module_id,
      const uint32_t application_id) FINAL;

  void SetResourceAcquired(const std::string& module_type,
                           const std::string& module_id,
                           const uint32_t app_id) FINAL;

  bool IsResourceAlreadyAcquiredByApp(const rc_rpc_plugin::ModuleUid& moduleUid,
                                      const uint32_t app_id) const FINAL;

 private:
  typedef std::vector<application_manager::ApplicationSharedPtr> Apps;

  /**
   * @brief Create OnRCStatusNotification creates OnRCStatus notification smart
   * object for mobile application
   * @param app_id application to send OnRCStatusNotification
   * @return smart object with mobile OnRCStatusNotification
   */
  smart_objects::SmartObjectSPtr CreateOnRCStatusNotificationToMobile(
      const application_manager::ApplicationSharedPtr app);

  smart_objects::SmartObjectSPtr CreateOnRCStatusNotificationToHmi(
      const application_manager::ApplicationSharedPtr app);

  /**
   * @brief IsModuleTypeRejected check if current resource was rejected by
   * driver for current application
   * @param module_type resource to check
   * @param module_id uuid of a resource
   * @param app_id application id
   * @return true if current resource was rejected by driver for current
   * application, otherwise - false
   */
  bool IsModuleTypeRejected(const std::string& module_type,
                            const std::string& module_id,
                            const uint32_t app_id);

  /**
   * @brief ReleaseModuleType Releases all resources related to
   * the corresponding module type acquired by application
   * @param module_type Module name
   * @param application_id Application id
   */
  void ReleaseModuleType(const std::string& module_type,
                         const uint32_t application_id);

  /**
   * @brief GetAcquiredResources Provides resources acquired by particular
   * application currently
   * @param application_id Application id
   * @return List of acquired resources by specific application
   */
  Resources GetAcquiredResources(const uint32_t application_id) const;

  /**
   * @brief GetAcquiredModuleTypes Provides module types acquired by particular
   * application currently
   * @param application_id Application id
   * @return List of acquired module types by specific application
   */
  std::set<std::string> GetAcquiredModuleTypes(
      const uint32_t application_id) const;

  /**
   * @brief ProcessApplicationPolicyUpdate Checks if allowed modules list is
   * changed for registered RC applications and releases in case some modules
   * now out of the list
   */
  void ProcessApplicationPolicyUpdate();

  /**
   * @brief RemoveAppsSubscriptions Removes subscriptions for interior data for
   * applications in the list
   * @param apps Application list
   */
  void RemoveAppsSubscriptions(const Apps& apps);

  /**
   * @brief SetResourceFree mark resourse as free and process logic of
   * changing state of aquired resources
   * @param module_type resource name
   * @param module_id uuid of a resource
   * @param app applicastion that aquire resource
   */
  ResourceReleasedState::eType SetResourceFree(const std::string& module_type,
                                               const std::string& module_id,
                                               const uint32_t app_id);
  /**
   * @brief CheckLocation checks if the user's grid is equal to or is within the
   * service area of the module, or user location is driver's seat
   * @param module module type + module id
   * @return true if the user's grid equals to or is within module service
   * area or user location is driver's seat, otherwise - false
   */
  bool IsUserLocationValid(ModuleUid& module,
                           application_manager::ApplicationSharedPtr app);

  /**
   * @brief AllocatedResources contains link between resource and application
   * owning that resource
   */

  typedef std::map<ModuleUid, uint32_t> AllocatedResources;
  AllocatedResources allocated_resources_;
  mutable sync_primitives::Lock allocated_resources_lock_;

  /**
   * @brief ResourcesState contains states of ALLOCATED resources
   */
  typedef std::map<ModuleUid, ResourceState::eType> ResourcesState;
  ResourcesState resources_state_;
  mutable sync_primitives::Lock resources_state_lock_;

  /**
   * @brief RejectedResources type for connecting list of resources rejected by
   * driver for application
   * application_id : [vector of rejected resources]
   */
  typedef std::map<uint32_t, std::vector<ModuleUid> > RejectedResources;
  RejectedResources rejected_resources_for_application_;
  mutable sync_primitives::Lock rejected_resources_for_application_lock_;

  hmi_apis::Common_RCAccessMode::eType current_access_mode_;
  application_manager::ApplicationManager& app_mngr_;
  application_manager::rpc_service::RPCService& rpc_service_;
  rc_rpc_plugin::RCCapabilitiesManager& rc_capabilities_manager_;
  bool is_rc_enabled_;
};
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RESOURCE_ALLOCATION_MANAGER_IMPL_H_
