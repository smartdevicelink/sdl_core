#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_SET_INTERIOR_VEHICLE_DATA_REQUEST_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_SET_INTERIOR_VEHICLE_DATA_REQUEST_H

#include "rc_rpc_plugin/commands/rc_command_request.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
class SetInteriorVehicleDataRequest : public RCCommandRequest {
 public:
  SetInteriorVehicleDataRequest(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handle,
      rc_rpc_plugin::ResourceAllocationManager& resource_allocation_manager);

  /**
   * @brief Execute command
   */
  void Execute() FINAL;

  /**
   * @brief AcquireResource proxy AcquireResource to Resource allocation manager
   * @param message message of requires contatin module types
   * @return result of acauiring resources
   */
  AcquireResult::eType AcquireResource(
      const app_mngr::commands::MessageSharedPtr& message) FINAL;

  /**
   * @brief IsResourceFree check resource state
   * @param module_type Resource name
   * @return True if free, otherwise - false
   */
  bool IsResourceFree(const std::string& module_type) const FINAL;

  /**
   * @brief SetResourceState changes state of resource
   * @param state State to set for resource
   */
  void SetResourceState(const std::string& module_type,
                        const ResourceState::eType state) FINAL;

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const app_mngr::event_engine::Event& event) FINAL;

  /**
   * @brief Method that check if READ_ONLY parameters present
   * @param request_params params from received message
   * @return true if present , false - otherwise
   */
  bool AreReadOnlyParamsPresent(const smart_objects::SmartObject& module_data);

  /**
   * @brief Method that check if all request parameters are READ_ONLY
   * @param request_params params from received message
   * @return true if all are read only , false - otherwise
   */
  bool AreAllParamsReadOnly(const smart_objects::SmartObject& module_data);

  /**
   * @brief Method that cuts-off READ_ONLY parameters
   * @param request_params params to handle
   */
  void CutOffReadOnlyParams(smart_objects::SmartObject& module_data);

  /**
   * @brief SetInteriorVehicleDataRequest class destructor
   */
  ~SetInteriorVehicleDataRequest();
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_SET_INTERIOR_VEHICLE_DATA_REQUEST_H
