#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_SET_INTERIOR_VEHICLE_DATA_REQUEST_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_SET_INTERIOR_VEHICLE_DATA_REQUEST_H

#include "application_manager/commands/request_to_hmi.h"
#include "rc_rpc_plugin/resource_allocation_manager.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
class RCSetInteriorVehicleDataRequest
    : public application_manager::commands::RequestToHMI {
 public:
  RCSetInteriorVehicleDataRequest(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handle,
      ResourceAllocationManager& resource_allocation_manager);
  void Run() OVERRIDE;
  ~RCSetInteriorVehicleDataRequest();
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_SET_INTERIOR_VEHICLE_DATA_REQUEST_H
