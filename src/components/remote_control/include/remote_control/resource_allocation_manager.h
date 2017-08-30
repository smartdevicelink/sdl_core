#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_H
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_H
#include <string>
#include "utils/macro.h"
#include "utils/shared_ptr.h"
#include "interfaces/HMI_API.h"
#include "remote_control/event_engine/event.h"
#include "functional_module/generic_module.h"

namespace remote_control {

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
   *         IN_USE  if subscription is not allowed
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
   * @param application_id Application id
   */
  virtual void OnApplicationEvent(functional_modules::ApplicationEvent event,
                                  const uint32_t application_id) = 0;

  /**
   * @brief OnPolicyEvent Processes policy related events
   * @param event Policy event
   */
  virtual void OnPolicyEvent(functional_modules::PolicyEvent event) = 0;

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

}  // namespace remote_control
#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RESOURCE_ALLOCATION_H
