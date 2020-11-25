#include "sdl_rpc_plugin/commands/mobile/get_cloud_app_properties_request.h"
#include "application_manager/policies/policy_handler_interface.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

GetCloudAppPropertiesRequest::GetCloudAppPropertiesRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromMobileImpl(message,
                            application_manager,
                            rpc_service,
                            hmi_capabilities,
                            policy_handler) {}

GetCloudAppPropertiesRequest::~GetCloudAppPropertiesRequest() {}

void GetCloudAppPropertiesRequest::Run() {
  SDL_LOG_AUTO_TRACE();
  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  std::string policy_app_id =
      (*message_)[strings::msg_params][strings::app_id].asString();

  policy::AppProperties app_properties;

  const bool result =
      policy_handler_.GetAppProperties(policy_app_id, app_properties);

  if (!result) {
    SendResponse(false,
                 mobile_apis::Result::DATA_NOT_AVAILABLE,
                 "Cloud app does not exist on module");
    return;
  }

  policy::StringArray nicknames;
  policy::StringArray app_hmi_types;

  policy_handler_.GetInitialAppData(policy_app_id, &nicknames, &app_hmi_types);

  smart_objects::SmartObject response_params(smart_objects::SmartType_Map);
  smart_objects::SmartObject properties(smart_objects::SmartType_Map);
  smart_objects::SmartObject nicknames_array(smart_objects::SmartType_Array);
  size_t i = 0;
  for (std::string nickname : nicknames) {
    nicknames_array[i] = nickname;
    ++i;
  }
  properties[strings::nicknames] = nicknames_array;
  properties[strings::app_id] = policy_app_id;
  properties[strings::enabled] = app_properties.enabled;

  if (!app_properties.auth_token.empty()) {
    properties[strings::auth_token] = app_properties.auth_token;
  }
  if (!app_properties.transport_type.empty()) {
    properties[strings::cloud_transport_type] = app_properties.transport_type;
  }
  if (!app_properties.hybrid_app_preference.empty()) {
    properties[strings::hybrid_app_preference] =
        app_properties.hybrid_app_preference;
  }
  if (!app_properties.endpoint.empty()) {
    properties[strings::endpoint] = app_properties.endpoint;
  }

  response_params[strings::properties] = properties;

  SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &response_params);
}

void GetCloudAppPropertiesRequest::on_event(
    const app_mngr::event_engine::Event& event) {
  SDL_LOG_INFO("GetCloudAppPropertiesRequest on_event");
}

}  // namespace commands
}  // namespace sdl_rpc_plugin
