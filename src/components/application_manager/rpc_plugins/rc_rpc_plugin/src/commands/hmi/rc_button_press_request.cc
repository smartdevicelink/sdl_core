#include "rc_rpc_plugin/commands/hmi/rc_button_press_request.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

RCButtonPressRequest::RCButtonPressRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager)
    :app_mngr::commands::RequestToHMI(message, application_manager) {}

RCButtonPressRequest::~RCButtonPressRequest(){}

void RCButtonPressRequest::Run() {
    LOG4CXX_AUTO_TRACE(logger_);
    SendRequest();
}

}  // namespace commands
}  // namespace rc_rpc_plugin
