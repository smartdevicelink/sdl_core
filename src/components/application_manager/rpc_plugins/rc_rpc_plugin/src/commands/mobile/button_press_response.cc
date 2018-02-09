#include "rc_rpc_plugin/commands/mobile/button_press_response.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

bool ButtonPressResponse::Init() {
    return true;
}
void ButtonPressResponse::Run() {}
void ButtonPressResponse::on_event(const application_manager::event_engine::Event& event) {}

}  // namespace commands
}  // namespace rc_rpc_plugin
