#include "sdl_rpc_plugin/commands/mobile/set_cloud_app_properties_request.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler_interface.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

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
  SDL_LOG_AUTO_TRACE();
  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const auto& properties =
      (*message_)[strings::msg_params][strings::properties];

  const auto app_id(properties[strings::app_id].asString());

  const auto properties_change_status =
      policy_handler_.GetAppPropertiesStatus(properties, app_id);

  using AppPropertiesState = policy::PolicyHandlerInterface::AppPropertiesState;
  const bool is_properties_changed =
      AppPropertiesState::NO_CHANGES != properties_change_status;

  const bool is_new_app = policy_handler_.IsNewApplication(app_id);

  policy_handler_.OnSetCloudAppProperties(*message_);
  SendResponse(true, mobile_apis::Result::SUCCESS);

  if (is_properties_changed || is_new_app) {
    const auto notification =
        MessageHelper::CreateOnAppPropertiesChangeNotification(
            app_id, application_manager_);
    application_manager_.GetRPCService().ManageHMICommand(notification);
  }
}

void SetCloudAppPropertiesRequest::on_event(
    const app_mngr::event_engine::Event& event) {
  SDL_LOG_INFO("SetCloudAppPropertiesRequest on_event");
}

}  // namespace commands
}  // namespace sdl_rpc_plugin
