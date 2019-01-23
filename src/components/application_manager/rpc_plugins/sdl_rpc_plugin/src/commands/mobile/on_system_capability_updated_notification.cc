#include "sdl_rpc_plugin/commands/mobile/on_system_capability_updated_notification.h"
#include "application_manager/application_manager.h"
#include "sdl_rpc_plugin/extensions/get_system_capability_app_extension.h"

// #include "interfaces/MOBILE_API.h"
// #include "utils/file_system.h"
// #include "utils/helpers.h"
// #include "policy/policy_table/enums.h"
// #include "application_manager/application_manager.h"
// #include "application_manager/policies/policy_handler_interface.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;
namespace commands {
namespace mobile {

  OnSystemCapabilityUpdatedNotification::OnSystemCapabilityUpdatedNotification(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handler)
      : CommandNotificationImpl(message,
                              application_manager,
                              rpc_service,
                              hmi_capabilities,
                              policy_handler) {
      LOG4CXX_DEBUG(logger_, "SYSCAP: Initialize OnSystemCapabilityUpdatedNotification");
                                  
                              }

  OnSystemCapabilityUpdatedNotification::~OnSystemCapabilityUpdatedNotification(){
      LOG4CXX_DEBUG(logger_, "SYSCAP: Destroy OnSystemCapabilityUpdatedNotification");
  }

  void OnSystemCapabilityUpdatedNotification::Run(){
    //   LOG4CXX_AUTO_TRACE(logger_);
    LOG4CXX_DEBUG(logger_, "SYSCAP: Send OnSystemCapabilityUpdatedNotification");

    app_mngr::ApplicationSharedPtr app = application_manager_.application(connection_key());
    auto& ext = SystemCapabilityAppExtension::ExtractVIExtension(*app);  
    smart_objects::SmartObject notification_params(smart_objects::SmartType_Map);
    const app_mngr::HMICapabilities& hmi_capabilities = hmi_capabilities_;

    if(ext.isSubscribedTo(mobile_apis::SystemCapabilityType::NAVIGATION)){
        notification_params[app_mngr::strings::system_capability][app_mngr::strings::system_capability_type]
        = mobile_apis::SystemCapabilityType::NAVIGATION;
        notification_params[app_mngr::strings::system_capability][app_mngr::strings::navigation_capability]
            = *hmi_capabilities.navigation_capability();
    }
    else if(ext.isSubscribedTo(mobile_apis::SystemCapabilityType::PHONE_CALL)){
        notification_params[app_mngr::strings::system_capability][app_mngr::strings::system_capability_type]
        = mobile_apis::SystemCapabilityType::PHONE_CALL;

        notification_params[app_mngr::strings::system_capability][app_mngr::strings::phone_capability]
            = *hmi_capabilities.phone_capability();
    }
    else if(ext.isSubscribedTo(mobile_apis::SystemCapabilityType::REMOTE_CONTROL)){
        notification_params[app_mngr::strings::system_capability][app_mngr::strings::system_capability_type]
        = mobile_apis::SystemCapabilityType::REMOTE_CONTROL;

        notification_params[app_mngr::strings::system_capability][app_mngr::strings::rc_capability]
            = *hmi_capabilities.rc_capability();
    }
    else if(ext.isSubscribedTo(mobile_apis::SystemCapabilityType::VIDEO_STREAMING)){
        notification_params[app_mngr::strings::system_capability][app_mngr::strings::system_capability_type]
        = mobile_apis::SystemCapabilityType::VIDEO_STREAMING;

        notification_params[app_mngr::strings::system_capability][app_mngr::strings::video_streaming_capability]
            = *hmi_capabilities.video_streaming_capability();
    }
    else{
        LOG4CXX_ERROR(logger_, "Invalid system capability");
        return;
    }

    (*message_)[app_mngr::strings::msg_params] = notification_params;
    SendNotification();

  }

}  // namespace mobile
}  // namespace commands
}  // namespace application_manager
