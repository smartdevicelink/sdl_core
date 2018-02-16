#include "rc_rpc_plugin/commands/hmi/rc_on_remote_control_settings_notification.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

namespace {
std::map<std::string, hmi_apis::Common_RCAccessMode::eType> access_modes{
    {enums_value::kAutoAllow, hmi_apis::Common_RCAccessMode::AUTO_ALLOW},
    {enums_value::kAutoDeny, hmi_apis::Common_RCAccessMode::AUTO_DENY},
    {enums_value::kAskDriver, hmi_apis::Common_RCAccessMode::ASK_DRIVER}};
}

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

RCOnRemoteControlSettingsNotification::RCOnRemoteControlSettingsNotification(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle,
    ResourceAllocationManager& resource_allocation_manager)
    : application_manager::commands::NotificationFromHMI(message,
                                                         application_manager,
                                                         rpc_service,
                                                         hmi_capabilities,
                                                         policy_handle)
    , resource_allocation_manager_(resource_allocation_manager) {}

hmi_apis::Common_RCAccessMode::eType AccessModeFromString(
    const std::string& access_mode) {
  std::map<std::string, hmi_apis::Common_RCAccessMode::eType>::const_iterator
      mode = access_modes.find(access_mode);
  return access_modes.end() != mode
             ? mode->second
             : hmi_apis::Common_RCAccessMode::INVALID_ENUM;
}

std::string AccessModeToString(
    const hmi_apis::Common_RCAccessMode::eType access_mode) {
  std::map<std::string, hmi_apis::Common_RCAccessMode::eType>::const_iterator
      it = access_modes.begin();
  for (; access_modes.end() != it; ++it) {
    if (access_mode == it->second) {
      return it->first;
    }
  }
  const std::string error = "UNKNOW_ACCESS_MODE";
  DCHECK_OR_RETURN(false, error);
  return error;
}

void UnsubscribeFromInteriorVehicleDataForAllModules(
    RCAppExtensionPtr extension) {
  LOG4CXX_AUTO_TRACE(logger_);
  extension->UnsubscribeFromInteriorVehicleData(enums_value::kClimate);
  extension->UnsubscribeFromInteriorVehicleData(enums_value::kRadio);
}

void RCOnRemoteControlSettingsNotification::DisallowRCFunctionality() {
  LOG4CXX_AUTO_TRACE(logger_);
  typedef std::vector<application_manager::ApplicationSharedPtr> Apps;
  Apps apps = RCRPCPlugin::GetRCApplications(application_manager_);
  for (Apps::iterator it = apps.begin(); it != apps.end(); ++it) {
    application_manager::ApplicationSharedPtr app = *it;
    DCHECK(app);
    application_manager_.ChangeAppsHMILevel(
        app->app_id(), mobile_apis::HMILevel::eType::HMI_NONE);

    const RCAppExtensionPtr extension =
        application_manager::AppExtensionPtr::static_pointer_cast<
            RCAppExtension>(app->QueryInterface(RCRPCPlugin::kRCPluginID));
    if (extension) {
      UnsubscribeFromInteriorVehicleDataForAllModules(extension);
    }
  }
}

void RCOnRemoteControlSettingsNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!(*message_)[app_mngr::strings::msg_params].keyExists(
          message_params::kAllowed)) {
    LOG4CXX_DEBUG(logger_,
                  "Notification is ignored due to \"allow\" parameter absense");
    LOG4CXX_DEBUG(logger_, "RC Functionality remains unchanged");
    return;
  }

  const bool is_allowed =
      (*message_)[app_mngr::strings::msg_params][message_params::kAllowed]
          .asBool();
  if (is_allowed) {
    hmi_apis::Common_RCAccessMode::eType access_mode =
        hmi_apis::Common_RCAccessMode::INVALID_ENUM;
    LOG4CXX_DEBUG(logger_, "Allowing RC Functionality");
    if ((*message_)[app_mngr::strings::msg_params].keyExists(
            message_params::kAccessMode)) {
      const std::string access_mode_str =
          (*message_)[app_mngr::strings::msg_params]
                     [message_params::kAccessMode].asString();

      access_mode = AccessModeFromString(access_mode_str);
      LOG4CXX_DEBUG(logger_, "Setting up access mode : " << access_mode_str);
    } else {
      access_mode = resource_allocation_manager_.GetAccessMode();
      LOG4CXX_DEBUG(logger_,
                    "No access mode received. Using last known: "
                        << AccessModeToString(access_mode));
    }
    resource_allocation_manager_.SetAccessMode(access_mode);
  } else {
    LOG4CXX_DEBUG(logger_, "Disallowing RC Functionality");
    DisallowRCFunctionality();
    resource_allocation_manager_.ResetAllAllocations();
  }
}

}  // namespace commands
}  // namespace rc_rpc_plugin
