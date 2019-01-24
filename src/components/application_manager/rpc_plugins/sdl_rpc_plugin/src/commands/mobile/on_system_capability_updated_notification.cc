#include "sdl_rpc_plugin/commands/mobile/on_system_capability_updated_notification.h"
#include "application_manager/application_manager.h"
#include "application_manager/helpers/application_helper.h"
#include "application_manager/message_helper.h"
#include "sdl_rpc_plugin/extensions/get_system_capability_app_extension.h"

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
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "SYSCAP: Send OnSystemCapabilityUpdatedNotification");

  std::vector<ApplicationSharedPtr> app_notification;
  std::vector<ApplicationSharedPtr>::iterator app_notification_it =
      app_notification.begin();
  std::vector<smart_objects::SmartObject> app_so;

  // TODO, figure out how to handle unknown enums for SystemCapabilityType
  mobile_apis::SystemCapabilityType::eType system_capability_type =
      static_cast<mobile_apis::SystemCapabilityType::eType>(
          (*message_)[strings::msg_params][strings::system_capability]
                     [strings::system_capability_type].asInt());

  const char* capability_type_string;
  ns_smart_device_link::ns_smart_objects::EnumConversionHelper<
      mobile_apis::SystemCapabilityType::eType>::
      EnumToCString(system_capability_type, &capability_type_string);

  auto subscribed_to_capability_predicate =
      [&system_capability_type](const ApplicationSharedPtr app) {
        DCHECK_OR_RETURN(app, false);
        auto& ext = SystemCapabilityAppExtension::ExtractExtension(*app);
        return ext.isSubscribedTo(system_capability_type);
      };

  const std::vector<ApplicationSharedPtr>& applications = FindAllApps(
      application_manager_.applications(), subscribed_to_capability_predicate);

  LOG4CXX_DEBUG(logger_,
                "Number of Notifications to be sent: " << applications.size());

  std::vector<ApplicationSharedPtr>::const_iterator app_it =
      applications.begin();

  for (; applications.end() != app_it; ++app_it) {
    const ApplicationSharedPtr app = *app_it;
    LOG4CXX_INFO(logger_,
                 "Sending OnSystemCapabilityUpdated " << capability_type_string
                                                      << " application id "
                                                      << app->app_id());
    (*message_)[strings::params][strings::connection_key] = app->app_id();
    SendNotification();
  }
}

}  // namespace mobile
}  // namespace commands
}  // namespace application_manager
