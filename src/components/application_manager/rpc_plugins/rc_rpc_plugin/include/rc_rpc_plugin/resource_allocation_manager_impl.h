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

  AcquireResult::eType AcquireResource(const std::string& module_type,
                                       const uint32_t app_id) OVERRIDE FINAL;
  void ForceAcquireResource(const std::string& module_type,
                            const uint32_t app_id) FINAL;

  void SetResourceState(const std::string& module_type,
                        const uint32_t app_id,
                        const ResourceState::eType state) FINAL;

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

 private:
  typedef std::vector<application_manager::ApplicationSharedPtr> Apps;

  /**
   * @brief Create OnRCStatusNotification creates OnRCStatus notification smart
   * object for mobile application
   * @param app_id application to send OnRCStatusNotification
   * @return smart object with mobile OnRCStatusNotification
   */
  smart_objects::SmartObjectSPtr CreateOnRCStatusNotificationToMobile(
      const uint32_t app_id);

  smart_objects::SmartObjectSPtr CreateOnRCStatusNotificationToHmi(
      const uint32_t app_id);


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
};
}  // rc_rpc_plugin

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_IMPL_H
