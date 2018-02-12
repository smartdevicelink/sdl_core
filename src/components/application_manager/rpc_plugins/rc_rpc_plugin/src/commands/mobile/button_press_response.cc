#include "rc_rpc_plugin/commands/mobile/button_press_response.h"
#include "application_manager/rpc_service.h"

namespace rc_rpc_plugin {
namespace commands {

ButtonPressResponse::ButtonPressResponse(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager)
    : app_mngr::commands::CommandResponseImpl(message, application_manager) {}

ButtonPressResponse::~ButtonPressResponse(){}

void ButtonPressResponse::Run() {
    LOG4CXX_AUTO_TRACE(logger_);
    application_manager_.GetRPCService().SendMessageToMobile(message_);
}

}  // namespace commands
}  // namespace rc_rpc_plugin
