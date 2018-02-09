#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_ON_REMOTE_CONTROL_SETTINGS_NOTIFICATION_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_ON_REMOTE_CONTROL_SETTINGS_NOTIFICATION_H

#include "application_manager/commands/notification_from_hmi.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {
class RCOnRemoteControlSettingsNotification
    : public application_manager::commands::NotificationFromHMI {
 public:
  bool Init() OVERRIDE;
  void Run() OVERRIDE;
  void on_event(const application_manager::event_engine::Event& event);
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_HMI_RC_ON_REMOTE_CONTROL_SETTINGS_NOTIFICATION_H
