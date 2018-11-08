#include "sdl_rpc_plugin/commands/mobile/set_cloudapp_properties_request.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

    SetCloudAppPropertiesRequest::SetCloudAppPropertiesRequest(const app_mngr::commands::MessageSharedPtr& message,
              app_mngr::ApplicationManager& application_manager,
              app_mngr::rpc_service::RPCService& rpc_service,
              app_mngr::HMICapabilities& hmi_capabilities,
              policy::PolicyHandlerInterface& policy_handler) 
            : CommandRequestImpl(message,
                application_manager,
                rpc_service,
                hmi_capabilities,
                policy_handler){}

    SetCloudAppPropertiesRequest::~SetCloudAppPropertiesRequest(){}
    void SetCloudAppPropertiesRequest::Run(){
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

        //Decide which response params to use
        smart_objects::SmartObject cloudapp_properties(smart_objects::SmartType_Map);

        cloudapp_properties[strings::msg_params][strings::app_name] = (*message_)[strings::msg_params][strings::app_name];
        cloudapp_properties[strings::msg_params][strings::app_id] = (*message_)[strings::msg_params][strings::app_id];

        if ((*message_)[strings::msg_params].keyExists(strings::enabled)) {
            smart_objects::SmartObject enabled = (*message_)[strings::msg_params][strings::enabled];
            cloudapp_properties[strings::msg_params][strings::enabled] = enabled;
        }
        if ((*message_)[strings::msg_params].keyExists(strings::cloud_app_auth_token)) {
            smart_objects::SmartObject auth_token = (*message_)[strings::msg_params][strings::cloud_app_auth_token];
            cloudapp_properties[strings::msg_params][strings::cloud_app_auth_token] = auth_token;
        }
        if ((*message_)[strings::msg_params].keyExists(strings::cloud_transport_type)) {
            smart_objects::SmartObject transport_type = (*message_)[strings::msg_params][strings::cloud_transport_type];
            cloudapp_properties[strings::msg_params][strings::cloud_transport_type] = transport_type;
        }
        if ((*message_)[strings::msg_params].keyExists(strings::hybrid_app_preference)) {
            smart_objects::SmartObject hybrid_app_preference = (*message_)[strings::msg_params][strings::hybrid_app_preference];
            cloudapp_properties[strings::msg_params][strings::hybrid_app_preference] = hybrid_app_preference;
        }
        
        // policy_handler_.OnSetCloudAppProperties(cloudapp_properties);
        SendResponse(true, mobile_apis::Result::SUCCESS);


    }


    void SetCloudAppPropertiesRequest::on_event(const app_mngr::event_engine::Event& event){
        LOG4CXX_INFO(logger_, "SetCloudAppPropertiesRequest on_event");
    }


} // namespace commands
} // namespace sdl_rpc_plugin