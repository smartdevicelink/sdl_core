#include "sdl_rpc_plugin/commands/mobile/get_cloud_app_properties_request.h"
	
namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

GetCloudAppPropertiesRequest::GetCloudAppPropertiesRequest(
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

GetCloudAppPropertiesRequest::~GetCloudAppPropertiesRequest() {}

void GetCloudAppPropertiesRequest::Run() {
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

    std::string policy_app_id = (*message_)[strings::msg_params][strings::app_id].asString();
    bool enabled = true;
    std::string endpoint;
    std::string auth_token;
    std::string certificate;
    std::string cloud_transport_type;
    std::string hybrid_app_preference;

    policy_handler_.GetCloudAppParameters(policy_app_id, enabled, endpoint, certificate, auth_token, cloud_transport_type, hybrid_app_preference);
    
    ApplicationSharedPtr cloud_app = application_manager_.application_by_policy_id(policy_app_id);
    std::string cloud_name = "";
    if (cloud_app) {
        cloud_name = cloud_app->name().AsMBString();
    }

    smart_objects::SmartObject response_params(smart_objects::SmartType_Map);
    smart_objects::SmartObject properties(smart_objects::SmartType_Map);

    properties[strings::app_name] = cloud_name;
    properties[strings::app_id] = policy_app_id;
    properties[strings::enabled] = enabled;
    properties[strings::auth_token] = auth_token;
    properties[strings::cloud_transport_type] = cloud_transport_type;
    properties[strings::hybrid_app_preference] = hybrid_app_preference;
    properties[strings::endpoint] = endpoint;

    response_params[strings::properties] = properties;

    SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &response_params);
}

void GetCloudAppPropertiesRequest::on_event(
    const app_mngr::event_engine::Event& event) {
    LOG4CXX_INFO(logger_, "GetCloudAppPropertiesRequest on_event");
}

}  // namespace commands
}  // namespace sdl_rpc_plugin 