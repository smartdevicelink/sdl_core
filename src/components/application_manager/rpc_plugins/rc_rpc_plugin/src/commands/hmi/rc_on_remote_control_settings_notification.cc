#include "rc_rpc_plugin/commands/hmi/rc_on_remote_control_settings_notification.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

RCOnRemoteControlSettingsNotification::RCOnRemoteControlSettingsNotification(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager)
    :app_mngr::commands::NotificationFromHMI(message, application_manager) {}

bool RCOnRemoteControlSettingsNotification::Init() {
  return true;
}
void RCOnRemoteControlSettingsNotification::Run() {}
void RCOnRemoteControlSettingsNotification::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
