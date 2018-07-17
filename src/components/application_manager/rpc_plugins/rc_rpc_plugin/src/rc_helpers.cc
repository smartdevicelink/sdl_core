#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/message.h"
#include "utils/make_shared.h"

namespace rc_rpc_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule");

const std::function<std::string(const std::string& module_type)>
RCHelpers::GetModuleTypeToDataMapping() {
  auto mapping_lambda = [](const std::string& module_type) -> std::string {
    static std::map<std::string, std::string> mapping = {
        {enums_value::kRadio, message_params::kRadioControlData},
        {enums_value::kClimate, message_params::kClimateControlData},
        {enums_value::kSeat, message_params::kSeatControlData}};
    auto it = mapping.find(module_type);
    if (mapping.end() == it) {
      return std::string();
    }
    return it->second;
  };

  return mapping_lambda;
}

RCAppExtensionPtr RCHelpers::GetRCExtension(
    application_manager::Application& app) {
  auto extension_interface = app.QueryInterface(RCRPCPlugin::kRCPluginID);
  RCAppExtensionPtr extension =
      application_manager::AppExtensionPtr::static_pointer_cast<RCAppExtension>(
          extension_interface);
  return extension;
}

smart_objects::SmartObjectSPtr RCHelpers::CreateUnsubscribeRequestToHMI(
    const std::string& module_type, const uint32_t correlation_id) {
  using namespace smart_objects;
  namespace commands = application_manager::commands;
  namespace am_strings = application_manager::strings;

  SmartObjectSPtr message = utils::MakeShared<SmartObject>(SmartType_Map);
  SmartObject& params = (*message)[am_strings::params];
  SmartObject& msg_params = (*message)[am_strings::msg_params];

  params[am_strings::message_type] =
      static_cast<int>(application_manager::kRequest);
  params[am_strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
  params[am_strings::protocol_type] = commands::CommandImpl::hmi_protocol_type_;
  params[am_strings::correlation_id] = correlation_id;
  params[am_strings::function_id] =
      hmi_apis::FunctionID::RC_GetInteriorVehicleData;
  msg_params[message_params::kSubscribe] = false;
  msg_params[message_params::kModuleType] = module_type;
  return message;
}

std::vector<application_manager::ApplicationSharedPtr>
RCHelpers::AppsSubscribedTo(application_manager::ApplicationManager& app_mngr,
                            const std::string& module_type) {
  std::vector<application_manager::ApplicationSharedPtr> result;
  auto rc_apps = RCRPCPlugin::GetRCApplications(app_mngr);
  for (auto& app : rc_apps) {
    auto rc_ext = RCHelpers::GetRCExtension(*app);
    DCHECK_OR_RETURN(rc_ext, result);
    if (rc_ext->IsSubscibedToInteriorVehicleData(module_type)) {
      result.push_back(app);
    }
  }
  return result;
}

RCHelpers::AppsModules RCHelpers::GetApplicaitonsAllowedModules(
    app_mngr::ApplicationManager& app_mngr) {
  auto apps_list = RCRPCPlugin::GetRCApplications(app_mngr);
  RCHelpers::AppsModules result;
  for (auto& app_ptr : apps_list) {
    std::vector<std::string> allowed_modules;
    app_mngr.GetPolicyHandler().GetModuleTypes(app_ptr->policy_app_id(),
                                               &allowed_modules);
    std::sort(allowed_modules.begin(), allowed_modules.end());
    result[app_ptr] = allowed_modules;
  }
  return result;
}

std::vector<application_manager::ApplicationSharedPtr>
RCHelpers::AppsSubscribedToModuleType(const std::string& module_type,
                                      app_mngr::ApplicationManager& app_mngr) {
  std::vector<application_manager::ApplicationSharedPtr> result;
  auto apps_list = RCRPCPlugin::GetRCApplications(app_mngr);
  for (auto& app : apps_list) {
    auto extension = RCHelpers::GetRCExtension(*app);
    if (extension->IsSubscibedToInteriorVehicleData(module_type)) {
      result.push_back(app);
    }
  }
  return result;
}
}
