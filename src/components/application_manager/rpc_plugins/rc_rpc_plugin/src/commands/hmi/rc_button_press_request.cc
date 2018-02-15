#include "rc_rpc_plugin/commands/hmi/rc_button_press_request.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

RCButtonPressRequest::RCButtonPressRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : application_manager::commands::RequestToHMI(message,
                                                  application_manager,
                                                  rpc_service,
                                                  hmi_capabilities,
                                                  policy_handle) {}

RCButtonPressRequest::~RCButtonPressRequest() {}

void RCButtonPressRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  SendRequest();
}

}  // namespace commands
}  // namespace rc_rpc_plugin
