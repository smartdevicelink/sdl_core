#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"

namespace rc_rpc_plugin {
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
}
