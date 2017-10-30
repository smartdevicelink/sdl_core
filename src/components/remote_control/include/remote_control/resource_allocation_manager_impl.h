#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_IMPL_H
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_IMPL_H
#include "remote_control/resource_allocation_manager.h"
#include "remote_control/remote_plugin_interface.h"
#include "utils/macro.h"
#include "utils/lock.h"

namespace remote_control {

typedef rc_event_engine::EventDispatcher<application_manager::MessagePtr,
                                         std::string> RCEventDispatcher;

class ResourceAllocationManagerImpl : public ResourceAllocationManager {
 public:
  ResourceAllocationManagerImpl(RemotePluginInterface& rc_plugin);

  ~ResourceAllocationManagerImpl();

  AcquireResult::eType AcquireResource(const std::string& module_type,
                                       const uint32_t app_id) OVERRIDE FINAL;

  void SetResourceState(const std::string& module_type,
                        const uint32_t app_id,
                        const ResourceState::eType state) FINAL;

  bool IsResourceFree(const std::string& module_type) const FINAL;

  void SetAccessMode(
      const hmi_apis::Common_RCAccessMode::eType access_mode) FINAL;

  hmi_apis::Common_RCAccessMode::eType GetAccessMode() const FINAL;

  void ForceAcquireResource(const std::string& module_type,
                            const uint32_t app_id) FINAL;

  void OnDriverDisallowed(const std::string& module_type,
                          const uint32_t app_id) FINAL;

  /**
   * @brief OnApplicationEvent Notifies modules on certain application events
   * @param event Event
   * @param application Pointer to application struct
   */
  void OnApplicationEvent(
      functional_modules::ApplicationEvent event,
      application_manager::ApplicationSharedPtr application) FINAL;

  /**
   * @brief OnPolicyEvent Processes policy related events
   * @param event Policy event
   */
  void OnPolicyEvent(functional_modules::PolicyEvent event) FINAL;

  void ResetAllAllocations() FINAL;

 private:
  typedef std::vector<application_manager::ApplicationSharedPtr> Apps;

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
   * @brief GetApplicationExtention Provides access to application RC extention
   * @param application Application
   * @return Pointer to RC extention of application or NULL if not available
   */
  RCAppExtensionPtr GetApplicationExtention(
      application_manager::ApplicationSharedPtr application);

  /**
   * @brief RemoveAppsSubscriptions Removes subscriptions for interior data for
   * applications in the list
   * @param apps Application list
   */
  void RemoveAppsSubscriptions(const Apps& apps);

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
  RemotePluginInterface& rc_plugin_;
};
}  // remote_control

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_IMPL_H
