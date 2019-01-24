#include "sdl_rpc_plugin/commands/mobile/on_system_capability_updated_notification.h"
#include "application_manager/application_manager.h"
#include "sdl_rpc_plugin/extensions/system_capability_app_extension.h"
#include "application_manager/message_helper.h"

// #include "interfaces/MOBILE_API.h"
// #include "utils/file_system.h"
// #include "utils/helpers.h"
// #include "policy/policy_table/enums.h"
// #include "application_manager/application_manager.h"
// #include "application_manager/policies/policy_handler_interface.h"

namespace sdl_rpc_plugin {
using namespace application_manager;
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
  LOG4CXX_DEBUG(logger_,
                "SYSCAP: Initialize OnSystemCapabilityUpdatedNotification");
}

OnSystemCapabilityUpdatedNotification::
    ~OnSystemCapabilityUpdatedNotification() {
  LOG4CXX_DEBUG(logger_,
                "SYSCAP: Destroy OnSystemCapabilityUpdatedNotification");
}

void OnSystemCapabilityUpdatedNotification::Run() {
  //   LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "SYSCAP: Send OnSystemCapabilityUpdatedNotification");

  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(connection_key());
  auto& ext = SystemCapabilityAppExtension::ExtractExtension(*app);
  smart_objects::SmartObject notification_params(smart_objects::SmartType_Map);
  // const app_mngr::HMICapabilities& hmi_capabilities = hmi_capabilities_;

  if (ext.isSubscribedTo(mobile_apis::SystemCapabilityType::APP_SERVICES)) {
    notification_params[app_mngr::strings::system_capability]
                       [app_mngr::strings::system_capability_type] =
                           mobile_apis::SystemCapabilityType::APP_SERVICES;

    smart_objects::SmartObject app_service_capabilities(
        smart_objects::SmartType_Map);
    smart_objects::SmartObject supported_types(smart_objects::SmartType_Array);
    smart_objects::SmartObject app_services(smart_objects::SmartType_Array);

    std::vector<smart_objects::SmartObject> service_records =
        application_manager_.GetAppServiceManager().GetAllServices();
    std::set<mobile_apis::AppServiceType::eType> service_types;

    for (auto& record : service_records) {
      // SUPPORTED TYPES
      mobile_apis::AppServiceType::eType service_type =
          static_cast<mobile_apis::AppServiceType::eType>(
              record[strings::service_manifest][strings::service_type]
                  .asUInt());
      service_types.insert(service_type);

      // APP SERVICES
      smart_objects::SmartObject app_services_capability(
          smart_objects::SmartType_Map);
      app_services_capability[strings::update_reason] =
          mobile_apis::ServiceUpdateReason::PUBLISHED;
      app_services_capability[strings::updated_app_service_record] = record;
      app_services.asArray()->push_back(app_services_capability);
    }

    int i = 0;
    for (auto type_ : service_types) {
      supported_types[i] = type_;
      i++;
    }

    app_service_capabilities[strings::services_supported] = supported_types;
    app_service_capabilities[strings::app_services] = app_services;
    notification_params[strings::system_capability]
                       [strings::app_services_capability] =
                           app_service_capabilities;

    MessageHelper::PrintSmartObject(notification_params);

  } else {
    LOG4CXX_ERROR(logger_, "Invalid system capability");
    return;
  }

  (*message_)[app_mngr::strings::msg_params] = notification_params;
  SendNotification();
}

}  // namespace mobile
}  // namespace commands
}  // namespace application_manager
