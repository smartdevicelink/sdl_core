#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_GET_INTERIOR_VEHICLE_DATA_REQUEST_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_GET_INTERIOR_VEHICLE_DATA_REQUEST_H

#include "rc_rpc_plugin/commands/rc_command_request.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
class GetInteriorVehicleDataRequest : public RCCommandRequest {
 public:
  GetInteriorVehicleDataRequest(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handle,
      ResourceAllocationManager& resource_allocation_manager);
  /**
   * @brief Execute command
   */
  void Execute() FINAL;

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const app_mngr::event_engine::Event& event) FINAL;

  /**
   * @brief GetInteriorVehicleDataRequest class destructor
   */
  virtual ~GetInteriorVehicleDataRequest();

 private:
  /**
   * @brief Check if app wants to proceed with already setup subscription
   * @param request_params request parameters to check
   * @return true if app already subscribed(unsubsribed) for module type but
   * wants to subscribe(unsubscribe) for the same module again
   * otherwise - false
   */
  bool HasRequestExcessiveSubscription();

  /**
    * @brief Handle subscription to vehicle data
    * @param hmi_response json message with response from HMI
    */
  void ProccessSubscription(
      const NsSmartDeviceLink::NsSmartObjects::SmartObject& hmi_response);

  /**
   * @brief Cuts off subscribe parameter
   * @param request_params request parameters to handle
   */
  void RemoveExcessiveSubscription();
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_GET_INTERIOR_VEHICLE_DATA_REQUEST_H
