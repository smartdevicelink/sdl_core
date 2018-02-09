#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_SET_INTERIOR_VEHICLE_DATA_REQUEST_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_SET_INTERIOR_VEHICLE_DATA_REQUEST_H

#include "application_manager/commands/request_to_hmi.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {
class RCSetInteriorVehicleDataRequest
    : public application_manager::commands::RequestToHMI {
 public:
  bool Init() OVERRIDE;
  void Run() OVERRIDE;
  void on_event(const application_manager::event_engine::Event& event);
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_SET_INTERIOR_VEHICLE_DATA_REQUEST_H
