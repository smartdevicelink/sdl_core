#include "sdl_rpc_plugin/commands/mobile/set_cloudapp_properties_request.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SetCloudAppPropertiesRequest::SetCloudAppPropertiesRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

SetCloudAppPropertiesRequest::~SetCloudAppPropertiesRequest() {}

void SetCloudAppPropertiesRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if ((*message_)[strings::msg_params].empty()) {
    LOG4CXX_ERROR(logger_, strings::msg_params << " is empty.");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  policy_handler_.OnSetCloudAppProperties(*message_);
  SendResponse(true, mobile_apis::Result::SUCCESS);
}

void SetCloudAppPropertiesRequest::on_event(
    const app_mngr::event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "SetCloudAppPropertiesRequest on_event");
}

}  // namespace commands
}  // namespace sdl_rpc_plugin