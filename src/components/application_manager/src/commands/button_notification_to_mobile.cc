#include "application_manager/commands/button_notification_to_mobile.h"
namespace application_manager {

namespace commands {
namespace app_mngr = application_manager;

CREATE_LOGGERPTR_GLOBAL(logger_, "ButtonNotificationToMobile")

ButtonNotificationToMobile::ButtonNotificationToMobile(
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

ButtonNotificationToMobile::~ButtonNotificationToMobile() {}

bool ButtonNotificationToMobile::DoesAppIDExist() const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace application_manager;
  return (*message_)[strings::msg_params].keyExists(strings::app_id);
}

void ButtonNotificationToMobile::HandleCustomButton(
    app_mngr::ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace application_manager;
  // app_id is mandatory for CUSTOM_BUTTON notification
  if (!DoesAppIDExist()) {
    LOG4CXX_ERROR(logger_, "CUSTOM_BUTTON mobile notification without app_id.");
    return;
  }

  // custom_button_id is mandatory for CUSTOM_BUTTON notification
  if (!(*message_)[app_mngr::strings::msg_params].keyExists(
          hmi_response::custom_button_id)) {
    LOG4CXX_ERROR(
        logger_, "CUSTOM_BUTTON mobile notification without custom_button_id.");
    return;
  }

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application doesn't exist.");
    return;
  }

  uint32_t custom_btn_id = 0;
  custom_btn_id =
      (*message_)[strings::msg_params][hmi_response::custom_button_id].asUInt();

  if (!app->IsSubscribedToSoftButton(custom_btn_id)) {
    LOG4CXX_ERROR(logger_,
                  "Application doesn't subscribed to this custom_button_id.");
    return;
  }

  if ((mobile_api::HMILevel::HMI_NONE == app->hmi_level())) {
    LOG4CXX_WARN(logger_,
                 "CUSTOM_BUTTON mobile notification is allowed only "
                     << "in FULL, LIMITED or BACKGROUND hmi level");
    return;
  }

  SendButtonNotification(app);
  return;
}

void ButtonNotificationToMobile::HandleOKButton(
    app_mngr::ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace application_manager;
  LOG4CXX_DEBUG(logger_, "OK button received");

  const auto subscribed_apps = SubscribedApps();
  auto app_ptr = subscribed_apps.end();
  if (app) {
    app_ptr = std::find_if(subscribed_apps.begin(),
                           subscribed_apps.end(),
                           [&app](const ApplicationSharedPtr subscribed_app) {
                             return app->app_id() == subscribed_app->app_id();
                           });
    if (app_ptr != subscribed_apps.end()) {
      LOG4CXX_DEBUG(
          logger_,
          "Sending button press for this app  id: " << (*app_ptr)->app_id());
      SendButtonNotification(*app_ptr);
      return;
    }
  }

  app_ptr = std::find_if(subscribed_apps.begin(),
                         subscribed_apps.end(),
                         [](const ApplicationSharedPtr subscribed_app) {
                           return subscribed_app->IsFullscreen();
                         });

  if (app_ptr != subscribed_apps.end()) {
    LOG4CXX_DEBUG(logger_,
                  "Sending button press for app in FULL with app  id: "
                      << (*app_ptr)->app_id());
    SendButtonNotification(*app_ptr);
    return;
  }
}

const std::vector<ApplicationSharedPtr>
ButtonNotificationToMobile::SubscribedApps() const {
  const uint32_t btn_id = static_cast<uint32_t>(
      (*message_)[strings::msg_params][hmi_response::button_name].asInt());
  return application_manager_.applications_by_button(btn_id);
}

void ButtonNotificationToMobile::HandleMediaButton() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace application_manager;
  const auto subscribed_apps = SubscribedApps();

  for (const auto subscribed_app : subscribed_apps) {
    if (!subscribed_app) {
      LOG4CXX_WARN(logger_, "Null pointer to subscribed app.");
      continue;
    }

    // Send ButtonEvent notification only in HMI_FULL or HMI_LIMITED mode
    if ((mobile_api::HMILevel::HMI_FULL != subscribed_app->hmi_level()) &&
        (mobile_api::HMILevel::HMI_LIMITED != subscribed_app->hmi_level())) {
      LOG4CXX_WARN(logger_,
                   "Mobile button notification is allowed only"
                       << "in FULL or LIMITED hmi level");
      continue;
    }
    SendButtonNotification(subscribed_app);
  }
}

void ButtonNotificationToMobile::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace application_manager;

  const uint32_t btn_id = static_cast<uint32_t>(
      (*message_)[strings::msg_params][hmi_response::button_name].asInt());

  LOG4CXX_DEBUG(logger_, "received button id: " << btn_id);

  ApplicationSharedPtr app;

  if (DoesAppIDExist()) {
    app = application_manager_.application(
        (*message_)[strings::msg_params][strings::app_id].asUInt());
  }

  // CUSTOM_BUTTON notification
  if (static_cast<uint32_t>(mobile_apis::ButtonName::CUSTOM_BUTTON) == btn_id) {
    HandleCustomButton(app);
  } else if (static_cast<uint32_t>(mobile_apis::ButtonName::OK) == btn_id) {
    HandleOKButton(app);
  } else {
    HandleMediaButton();
  }
}

}  // namespace commands
}  // namespace application_manager
