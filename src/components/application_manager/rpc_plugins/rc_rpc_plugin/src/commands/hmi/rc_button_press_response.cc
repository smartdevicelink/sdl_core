#include "rc_rpc_plugin/commands/hmi/rc_button_press_response.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool RCButtonPressResponse::Init() {
    return true;
}
void RCButtonPressResponse::Run() {}
void RCButtonPressResponse::on_event(const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
