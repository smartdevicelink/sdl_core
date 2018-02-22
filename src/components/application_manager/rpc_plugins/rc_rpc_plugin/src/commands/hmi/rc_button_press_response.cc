#include "rc_rpc_plugin/commands/hmi/rc_button_press_response.h"
#include "application_manager/event_engine/event.h"

namespace rc_rpc_plugin {
namespace commands {

RCButtonPressResponse::RCButtonPressResponse(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle,
    ResourceAllocationManager& resource_allocation_manager)
    : application_manager::commands::ResponseFromHMI(message,
                                                     application_manager,
                                                     rpc_service,
                                                     hmi_capabilities,
                                                     policy_handle) {
  UNUSED(resource_allocation_manager);
}

void RCButtonPressResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  app_mngr::event_engine::Event event(
      hmi_apis::FunctionID::Buttons_ButtonPress);
  event.set_smart_object(*message_);
  event.raise(application_manager_.event_dispatcher());
}

RCButtonPressResponse::~RCButtonPressResponse() {}

}  // namespace commands
}  // namespace rc_rpc_plugin
