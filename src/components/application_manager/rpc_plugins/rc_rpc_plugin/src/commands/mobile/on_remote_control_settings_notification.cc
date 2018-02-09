#include "rc_rpc_plugin/commands/mobile/on_remote_control_settings_notification.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool OnRemoveControlSettingsNotification::Init() {
  return true;
}
void OnRemoveControlSettingsNotification::Run() {}
void OnRemoveControlSettingsNotification::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
