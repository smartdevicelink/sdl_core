#include "rc_rpc_plugin/commands/hmi/rc_button_press_request.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool RCButtonPressRequest::Init() {
    return true;
}
void RCButtonPressRequest::Run() {}
void RCButtonPressRequest::on_event(const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
