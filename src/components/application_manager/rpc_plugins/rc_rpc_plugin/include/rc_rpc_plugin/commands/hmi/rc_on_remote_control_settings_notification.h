#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_ON_REMOTE_CONTROL_SETTINGS_NOTIFICATION_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_ON_REMOTE_CONTROL_SETTINGS_NOTIFICATION_H

#include "application_manager/commands/notification_from_hmi.h"
#include "rc_rpc_plugin/resource_allocation_manager.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
class RCOnRemoteControlSettingsNotification
    : public application_manager::commands::NotificationFromHMI {
 public:
  RCOnRemoteControlSettingsNotification(
          const app_mngr::commands::MessageSharedPtr& message,
          app_mngr::ApplicationManager& application_manager,
          app_mngr::rpc_service::RPCService& rpc_service,
          app_mngr::HMICapabilities& hmi_capabilities,
          policy::PolicyHandlerInterface& policy_handle,
          rc_rpc_plugin::ResourceAllocationManager& resource_allocation_manager);
  /**
   * @brief Execute command
   **/
  void Run() OVERRIDE;

  virtual ~RCOnRemoteControlSettingsNotification();

private:
 ResourceAllocationManager& resource_allocation_manager_;

 /**
  * @brief Disalows RC functionality for all RC apps
  * All registered apps with appHMIType REMOTE_CONTROL will be put to NONE hmi
  * level
  * OnHMIStatus (NONE) will be send to such apps
  * All registered apps will be unsubsribed from OnInteriorVehicleData
  * notifications
  */
 void DisallowRCFunctionality();
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_ON_REMOTE_CONTROL_SETTINGS_NOTIFICATION_H
