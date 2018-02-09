#include "rc_rpc_plugin/commands/hmi/rc_on_remote_control_settings_notification.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool RCOnRemoteControlSettingsNotification::Init() {
  return true;
}
void RCOnRemoteControlSettingsNotification::Run() {}
void RCOnRemoteControlSettingsNotification::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
