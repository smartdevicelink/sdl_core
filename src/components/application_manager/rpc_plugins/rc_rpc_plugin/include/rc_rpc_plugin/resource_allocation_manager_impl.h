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

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_IMPL_H
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_IMPL_H
#include "rc_rpc_plugin/resource_allocation_manager.h"
#include "application_manager/application_impl.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "utils/macro.h"
#include "utils/lock.h"

namespace rc_rpc_plugin {

typedef std::vector<application_manager::ApplicationSharedPtr> Apps;

class ResourceAllocationManagerImpl : public ResourceAllocationManager {
 public:
  ResourceAllocationManagerImpl(
      application_manager::ApplicationManager& app_mngr,
      application_manager::rpc_service::RPCService& rpc_service);

  ~ResourceAllocationManagerImpl();

  /**
   * @brief AcquireResource forces acquiring resource by application
   * @param module_type resource to acquire
   * @param app_id application that acquire resourc
   * @return result of acauiring resources
   */
  AcquireResult::eType AcquireResource(const std::string& module_type,
                                       const uint32_t app_id) OVERRIDE FINAL;
  /**
   * @brief ForceAcquireResource forces acquiring resource by application
   * @param module_type resource to acquire
   * @param app_id application that acquire resource
   */
  void ForceAcquireResource(const std::string& module_type,
                            const uint32_t app_id) FINAL;

  /**
    * @brief SetResourceState changes resource state. Resource must be acquired
    * beforehand.
    * @param module_type Resource to change its state
    * @param app_id Application aquired resource before
    * @param state State to set for resource
    */
  void SetResourceState(const std::string& module_type,
                        const uint32_t app_id,
                        const ResourceState::eType state) FINAL;

  /**
   * @brief IsResourceFree check resource state
   * @param module_type Resource name
   * @return True if free, otherwise - false
   */
  bool IsResourceFree(const std::string& module_type) const FINAL;

  void SetAccessMode(
      const hmi_apis::Common_RCAccessMode::eType access_mode) FINAL;

  hmi_apis::Common_RCAccessMode::eType GetAccessMode() const FINAL;

  void OnDriverDisallowed(const std::string& module_type,
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

  /**
   * @brief GetApplicationExtention Provides access to application RC extention
   * @param application Application
   * @return Pointer to RC extention of application or NULL if not available
   */
  RCAppExtensionPtr GetApplicationExtention(
      application_manager::ApplicationSharedPtr application) FINAL;

  void SendOnRCStatusNotification() FINAL;

  bool is_rc_enabled() const FINAL;

  void set_rc_enabled(const bool value) FINAL;

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
   * @param app_id application id
   * @return true if current resource was rejected by driver for current
   * application, otherwise - false
   */
  bool IsModuleTypeRejected(const std::string& module_type,
                            const uint32_t app_id);

  /**
   * @brief ReleaseResource Releases resource acquired by application
   * @param module_type Module name
   * @param application_id Application id
   */
  void ReleaseResource(const std::string& module_type,
                       const uint32_t application_id);

  /**
   * @brief GetAcquiredResources Provides resources acquired by particular
   * application currently
   * @param application_id Application id
   * @return List of acquired resources by specific application
   */
  Resources GetAcquiredResources(const uint32_t application_id) const;

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
   * @brief SetResourceAquired mark resourse as aquired and process logic of
   * changing state of aquired resources
   * @param module_type resource name
   * @param app applicastion that aquire resource
   */
  void SetResourceAquired(const std::string& module_type,
                          const uint32_t app_id);
  /**
   * @brief SetResourceFree mark resourse as free and process logic of
   * changing state of aquired resources
   * @param module_type resource name
   * @param app applicastion that aquire resource
   */
  void SetResourceFree(const std::string& module_type, const uint32_t app_id);

  std::vector<std::string> all_supported_modules();

  /**
   * @brief AllocatedResources contains link between resource and application
   * owning that resource
   */
  typedef std::map<std::string, uint32_t> AllocatedResources;
  AllocatedResources allocated_resources_;
  mutable sync_primitives::Lock allocated_resources_lock_;

  /**
   * @brief ResourcesState contains states of ALLOCATED resources
   */
  typedef std::map<std::string, ResourceState::eType> ResourcesState;
  ResourcesState resources_state_;
  mutable sync_primitives::Lock resources_state_lock_;

  /**
   * @brief RejectedResources type for connecting list of resources rejected by
   * driver for application
   * application_id : [vector of rejected resources]
   */
  typedef std::map<uint32_t, std::vector<std::string> > RejectedResources;
  RejectedResources rejected_resources_for_application_;
  mutable sync_primitives::Lock rejected_resources_for_application_lock_;

  hmi_apis::Common_RCAccessMode::eType current_access_mode_;
  application_manager::ApplicationManager& app_mngr_;
  application_manager::rpc_service::RPCService& rpc_service_;
  bool is_rc_enabled_;
};
}  // rc_rpc_plugin

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_IMPL_H
