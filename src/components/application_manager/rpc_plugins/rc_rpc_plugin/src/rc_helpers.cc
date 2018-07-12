#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/message.h"
#include "utils/make_shared.h"

namespace rc_rpc_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule");

const std::map<std::string, std::string>&
RCHelpers::GetModuleTypeToDataMapping() {
  static std::map<std::string, std::string> mapping = {
      {enums_value::kRadio, message_params::kRadioControlData},
      {enums_value::kClimate, message_params::kClimateControlData},
      {enums_value::kAudio, message_params::kAudioControlData},
      {enums_value::kLight, message_params::kLightControlData},
      {enums_value::kHmiSettings, message_params::kHmiSettingsControlData}};
  return mapping;
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
}
