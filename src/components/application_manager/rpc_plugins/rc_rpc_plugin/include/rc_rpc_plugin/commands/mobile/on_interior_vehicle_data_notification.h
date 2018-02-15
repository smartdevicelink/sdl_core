#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_ON_INTERIOR_VEHICLE_DATA_NOTIFICATION_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_ON_INTERIOR_VEHICLE_DATA_NOTIFICATION_H

#include "application_manager/commands/command_notification_impl.h"
#include "rc_rpc_plugin/resource_allocation_manager.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
class OnInteriorVehicleDataNotification
    : public application_manager::commands::CommandNotificationImpl {
 public:
  OnInteriorVehicleDataNotification(
      ResourceAllocationManager& resource_allocation_manager,
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handle);
  bool Init() OVERRIDE;
  void Run() OVERRIDE;
  void on_event(const application_manager::event_engine::Event& event);
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_ON_INTERIOR_VEHICLE_DATA_NOTIFICATION_H
