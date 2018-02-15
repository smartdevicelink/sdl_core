#include "rc_rpc_plugin/commands/hmi/rc_on_remote_control_settings_notification.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

RCOnRemoteControlSettingsNotification::RCOnRemoteControlSettingsNotification(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : application_manager::commands::NotificationFromHMI(message,
                                                         application_manager,
                                                         rpc_service,
                                                         hmi_capabilities,
                                                         policy_handle) {}

bool RCOnRemoteControlSettingsNotification::Init() {
  return true;
}
void RCOnRemoteControlSettingsNotification::Run() {}
void RCOnRemoteControlSettingsNotification::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
