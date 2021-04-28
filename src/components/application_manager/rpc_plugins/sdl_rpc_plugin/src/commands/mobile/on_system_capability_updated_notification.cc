#include "sdl_rpc_plugin/commands/mobile/on_system_capability_updated_notification.h"

#include "application_manager/app_service_manager.h"
#include "application_manager/application_manager.h"
#include "application_manager/display_capabilities_builder.h"
#include "application_manager/helpers/application_helper.h"
#include "application_manager/message_helper.h"
#include "sdl_rpc_plugin/extensions/system_capability_app_extension.h"
#include "smart_objects/enum_schema_item.h"

namespace sdl_rpc_plugin {
using namespace application_manager;
namespace commands {
namespace mobile {

SDL_CREATE_LOG_VARIABLE("Commands")

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
                              policy_handler) {}

OnSystemCapabilityUpdatedNotification::
    ~OnSystemCapabilityUpdatedNotification() {}

void OnSystemCapabilityUpdatedNotification::Run() {
  SDL_LOG_AUTO_TRACE();

  smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];
  const auto system_capability_type =
      static_cast<mobile_apis::SystemCapabilityType::eType>(
          msg_params[strings::system_capability]
                    [strings::system_capability_type]
                        .asInt());

  switch (system_capability_type) {
    case mobile_apis::SystemCapabilityType::NAVIGATION: {
      smart_objects::SmartObject nav_capability(smart_objects::SmartType_Map);
      bool has_nav_capability = false;
      if (hmi_capabilities_.navigation_capability()) {
        has_nav_capability = true;
        nav_capability = *hmi_capabilities_.navigation_capability();
      }
      has_nav_capability = application_manager_.GetAppServiceManager()
                               .UpdateNavigationCapabilities(nav_capability) ||
                           has_nav_capability;

      if (has_nav_capability) {
        msg_params[strings::system_capability][strings::navigation_capability] =
            nav_capability;
      } else {
        return;
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::PHONE_CALL: {
      if (hmi_capabilities_.phone_capability()) {
        msg_params[strings::system_capability][strings::phone_capability] =
            *hmi_capabilities_.phone_capability();
      } else {
        return;
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::REMOTE_CONTROL: {
      if (!hmi_capabilities_.is_rc_cooperating()) {
        return;
      }
      if (hmi_capabilities_.rc_capability()) {
        msg_params[strings::system_capability][strings::rc_capability] =
            *hmi_capabilities_.rc_capability();
      } else {
        return;
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::VIDEO_STREAMING: {
      SendNotification();
      return;
    }
    case mobile_apis::SystemCapabilityType::APP_SERVICES: {
      auto all_services =
          application_manager_.GetAppServiceManager().GetAllServiceRecords();
      auto app_service_caps =
          MessageHelper::CreateAppServiceCapabilities(all_services);

      smart_objects::SmartArray* app_services =
          app_service_caps[strings::app_services].asArray();
      smart_objects::SmartObject& updated_capabilities =
          msg_params[strings::system_capability]
                    [strings::app_services_capabilities][strings::app_services];
      for (size_t i = 0; i < updated_capabilities.length(); ++i) {
        std::string service_id =
            updated_capabilities[i][strings::updated_app_service_record]
                                [strings::service_id]
                                    .asString();
        auto matching_service_predicate =
            [&service_id](
                const smart_objects::SmartObject& app_service_capability) {
              return service_id ==
                     app_service_capability[strings::updated_app_service_record]
                                           [strings::service_id]
                                               .asString();
            };

        auto it = std::find_if(app_services->begin(),
                               app_services->end(),
                               matching_service_predicate);
        if (it != app_services->end()) {
          SDL_LOG_DEBUG("Replacing updated record with service_id "
                        << service_id);
          app_services->erase(it);
        }
        app_services->push_back(updated_capabilities[i]);
      }
      msg_params[strings::system_capability]
                [strings::app_services_capabilities] = app_service_caps;
      break;
    }

    case mobile_apis::SystemCapabilityType::DISPLAYS: {
      // Display capabilities content will be populated in the code after the
      // switch so just breaking here
      break;
    }

    default: {
      SDL_LOG_ERROR("Unknown system capability type: "
                    << msg_params[strings::system_capability]
                                 [strings::system_capability_type]
                                     .asInt());
      return;
    }
  }

  const char* capability_type_string;
  ns_smart_device_link::ns_smart_objects::EnumConversionHelper<
      mobile_apis::SystemCapabilityType::eType>::
      EnumToCString(system_capability_type, &capability_type_string);

  const auto initial_connection_key =
      (*message_)[strings::params][strings::connection_key].asUInt();

  auto subscribed_to_capability_predicate =
      [&system_capability_type,
       &initial_connection_key](const ApplicationSharedPtr app) {
        DCHECK_OR_RETURN(app, false);

        if (mobile_apis::SystemCapabilityType::DISPLAYS ==
                system_capability_type &&
            initial_connection_key > 0) {
          SDL_LOG_DEBUG("Display capabilities notification for app "
                        << initial_connection_key << " only");
          return app->app_id() == initial_connection_key;
        }

        auto ext_ptr = app->QueryInterface(
            SystemCapabilityAppExtension::SystemCapabilityAppExtensionUID);

        if (!ext_ptr) {
          SDL_LOG_DEBUG("App "
                        << app->app_id()
                        << " does not have SystemCapabilityAppExtension");
          return false;
        }

        auto ext =
            std::static_pointer_cast<SystemCapabilityAppExtension>(ext_ptr);

        if (!ext->IsSubscribedTo(system_capability_type)) {
          SDL_LOG_DEBUG("App " << app->app_id()
                               << " is not subscribed to this capability type");
          return false;
        }

        SDL_LOG_DEBUG("App " << app->app_id()
                             << " is subscribed to specified capability type");
        return true;
      };

  const std::vector<ApplicationSharedPtr>& applications = FindAllApps(
      application_manager_.applications(), subscribed_to_capability_predicate);

  SDL_LOG_DEBUG("Number of Notifications to be sent: " << applications.size());

  std::vector<ApplicationSharedPtr>::const_iterator app_it =
      applications.begin();

  for (; applications.end() != app_it; ++app_it) {
    const ApplicationSharedPtr app = *app_it;

    switch (system_capability_type) {
      case mobile_apis::SystemCapabilityType::REMOTE_CONTROL: {
        if (!app->is_remote_control_supported()) {
          SDL_LOG_WARN("App with connection key: "
                       << app->app_id()
                       << " was subcribed to REMOTE_CONTROL system "
                          "capabilities, but "
                          "does not have RC permissions. Unsubscribing");
          auto& ext = SystemCapabilityAppExtension::ExtractExtension(*app);
          ext.UnsubscribeFrom(system_capability_type);
        }
      } break;

      case mobile_apis::SystemCapabilityType::DISPLAYS: {
        SDL_LOG_DEBUG("Using common display capabilities");
        auto capabilities = hmi_capabilities_.system_display_capabilities();
        auto& builder = app->display_capabilities_builder();
        if (app->is_resuming() && builder.IsWindowResumptionNeeded()) {
          SDL_LOG_DEBUG("Application "
                        << app->app_id()
                        << " is resuming. Providing cached capabilities");
          auto display_caps = builder.display_capabilities();
          capabilities = display_caps;
        } else if (app->display_capabilities()) {
          SDL_LOG_DEBUG("Application " << app->app_id()
                                       << " has specific display capabilities");
          const WindowID window_id =
              msg_params[strings::system_capability]
                        [strings::display_capabilities][0]
                        [strings::window_capabilities][0][strings::window_id]
                            .asInt();
          capabilities = app->display_capabilities(window_id);
        }

        if (!capabilities) {
          SDL_LOG_WARN(
              "No available display capabilities for sending. Skipping");
          continue;
        }

        msg_params[strings::system_capability][strings::display_capabilities] =
            *capabilities;
      } break;
      default: { SDL_LOG_ERROR("Unknown system capability type"); }
    }

    SDL_LOG_INFO("Sending OnSystemCapabilityUpdated " << capability_type_string
                                                      << " application id "
                                                      << app->app_id());
    (*message_)[strings::params][strings::connection_key] = app->app_id();
    SendNotification();
  }
}

}  // namespace mobile
}  // namespace commands
}  // namespace sdl_rpc_plugin
