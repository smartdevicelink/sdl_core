#include "rc_rpc_plugin/commands/mobile/button_press_response.h"
#include "application_manager/rpc_service.h"

namespace rc_rpc_plugin {
namespace commands {

ButtonPressResponse::ButtonPressResponse(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : application_manager::commands::CommandResponseImpl(message,
                                                         application_manager,
                                                         rpc_service,
                                                         hmi_capabilities,
                                                         policy_handle) {}
ButtonPressResponse::~ButtonPressResponse() {}

void ButtonPressResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager_.GetRPCService().SendMessageToMobile(message_);
}

}  // namespace commands
}  // namespace rc_rpc_plugin
