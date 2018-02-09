#include "rc_rpc_plugin/commands/mobile/button_press_request.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool ButtonPressRequest::Init() {
    return true;
}
void ButtonPressRequest::Run() {}
void ButtonPressRequest::on_event(const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
