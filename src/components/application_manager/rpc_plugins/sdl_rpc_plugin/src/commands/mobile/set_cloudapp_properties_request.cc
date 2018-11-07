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
        smart_objects::SmartObject response_params(smart_objects::SmartType_Map);

        std::string app_name = (*message_)[strings::msg_params][strings::app_name].asString();
        std::string app_id = (*message_)[strings::msg_params][strings::app_id].asString();

        if ((*message_)[strings::msg_params].keyExists(strings::enabled)) {
        //    bool enabled = (*message_)[strings::msg_params][strings::enabled].asBool();
        }
        if ((*message_)[strings::msg_params].keyExists(strings::cloud_app_auth_token)) {
        //    std::string auth_token = (*message_)[strings::msg_params][strings::cloud_app_auth_token].asString();
        }
        if ((*message_)[strings::msg_params].keyExists(strings::cloud_transport_type)) {
        //    std::string transport_type = (*message_)[strings::msg_params][strings::cloud_transport_type].asString();
        }
        if ((*message_)[strings::msg_params].keyExists(strings::hybrid_app_preference)) {
        //    mobile_apis::HybridAppPreference::eType hybrid_app_preference = 
        //        static_cast<mobile_apis::HybridAppPreference::eType>((*message_)[strings::msg_params][strings::hybrid_app_preference].asString());
        }
        
        SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &response_params);




    }


    void SetCloudAppPropertiesRequest::on_event(const app_mngr::event_engine::Event& event){
        LOG4CXX_INFO(logger_, "SetCloudAppPropertiesRequest on_event");
    }


} // namespace commands
} // namespace sdl_rpc_plugin