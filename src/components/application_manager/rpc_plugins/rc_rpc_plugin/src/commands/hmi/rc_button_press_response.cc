#include "rc_rpc_plugin/commands/hmi/rc_button_press_response.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

RCButtonPressResponse::RCButtonPressResponse(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager)
    :app_mngr::commands::ResponseToHMI(message, application_manager) {}

bool RCButtonPressResponse::Init() {
  return true;
}
void RCButtonPressResponse::Run() {}
void RCButtonPressResponse::on_event(
    const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
