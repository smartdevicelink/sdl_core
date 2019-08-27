#include "rc_rpc_plugin/rc_helpers.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/message.h"
#include "application_manager/smart_object_keys.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"

namespace rc_rpc_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule");

const std::vector<std::string> RCHelpers::buttons_climate() {
  std::vector<std::string> data;
  data.push_back(enums_value::kACMax);
  data.push_back(enums_value::kAC);
  data.push_back(enums_value::kRecirculate);
  data.push_back(enums_value::kFanUp);
  data.push_back(enums_value::kFanDown);
  data.push_back(enums_value::kTempUp);
  data.push_back(enums_value::kTempDown);
  data.push_back(enums_value::kDefrostMax);
  data.push_back(enums_value::kDefrost);
  data.push_back(enums_value::kDefrostRear);
  data.push_back(enums_value::kUpperVent);
  data.push_back(enums_value::kLowerVent);
  return data;
}

const std::vector<std::string> RCHelpers::buttons_radio() {
  std::vector<std::string> data;
  data.push_back(enums_value::kVolumeUp);
  data.push_back(enums_value::kVolumeDown);
  data.push_back(enums_value::kEject);
  data.push_back(enums_value::kSource);
  data.push_back(enums_value::kShuffle);
  data.push_back(enums_value::kRepeat);
  return data;
}

const RCHelpers::ButtonsMap RCHelpers::buttons_map() {
  using namespace mobile_apis;

  ButtonsMap buttons_map;
  buttons_map[enums_value::kACMax] = ButtonName::AC_MAX;
  buttons_map[enums_value::kAC] = ButtonName::AC;
  buttons_map[enums_value::kRecirculate] = ButtonName::RECIRCULATE;
  buttons_map[enums_value::kFanUp] = ButtonName::FAN_UP;
  buttons_map[enums_value::kFanDown] = ButtonName::FAN_DOWN;
  buttons_map[enums_value::kTempUp] = ButtonName::TEMP_UP;
  buttons_map[enums_value::kTempDown] = ButtonName::TEMP_DOWN;
  buttons_map[enums_value::kDefrostMax] = ButtonName::DEFROST_MAX;
  buttons_map[enums_value::kDefrost] = ButtonName::DEFROST;
  buttons_map[enums_value::kDefrostRear] = ButtonName::DEFROST_REAR;
  buttons_map[enums_value::kUpperVent] = ButtonName::UPPER_VENT;
  buttons_map[enums_value::kLowerVent] = ButtonName::LOWER_VENT;
  buttons_map[enums_value::kVolumeUp] = ButtonName::VOLUME_UP;
  buttons_map[enums_value::kVolumeDown] = ButtonName::VOLUME_DOWN;
  buttons_map[enums_value::kEject] = ButtonName::EJECT;
  buttons_map[enums_value::kSource] = ButtonName::SOURCE;
  buttons_map[enums_value::kShuffle] = ButtonName::SHUFFLE;
  buttons_map[enums_value::kRepeat] = ButtonName::REPEAT;

  return buttons_map;
}

std::vector<std::string> RCHelpers::GetModuleReadOnlyParams(
    const std::string& module_type) {
  using namespace message_params;
  std::vector<std::string> module_ro_params;
  if (enums_value::kClimate == module_type) {
    module_ro_params.push_back(kCurrentTemperature);
  } else if (enums_value::kRadio == module_type) {
    module_ro_params.push_back(kRdsData);
    module_ro_params.push_back(kAvailableHDs);
    module_ro_params.push_back(kAvailableHdChannels);
    module_ro_params.push_back(kSignalStrength);
    module_ro_params.push_back(kSignalChangeThreshold);
    module_ro_params.push_back(kState);
    module_ro_params.push_back(kSisData);
  } else if (enums_value::kLight == module_type) {
    module_ro_params.push_back(kLightStatus);
  }

  return module_ro_params;
}

rc_rpc_types::ModuleIdConsentVector RCHelpers::FillModuleConsents(
    const std::string& module_type,
    const std::vector<std::string>& module_ids,
    const std::vector<bool> allowed) {
  using namespace rc_rpc_types;
  if (module_ids.size() != allowed.size()) {
    return rc_rpc_types::ModuleIdConsentVector();
  }

  rc_rpc_types::ModuleIdConsentVector module_consents;
  std::time_t current_date = std::time(0);
  size_t array_size = module_ids.size();

  for (size_t i = 0; i < array_size; ++i) {
    rc_rpc_types::ModuleIdConsent module_consent;
    module_consent.module_id = {module_type, module_ids[i]};
    module_consent.consent =
        allowed[i] ? ModuleConsent::CONSENTED : ModuleConsent::NOT_CONSENTED;
    module_consent.date_of_consent = current_date;

    module_consents.push_back(module_consent);
  }
  return module_consents;
}

std::vector<std::string> RCHelpers::RetrieveModuleIds(
    const ns_smart_device_link::ns_smart_objects::SmartObject& moduleIds) {
  std::vector<std::string> module_ids;
  for (const auto& module_id : (*moduleIds.asArray())) {
    module_ids.push_back(module_id.asString());
  }
  return module_ids;
}

std::vector<bool> RCHelpers::RetrieveModuleConsents(
    const ns_smart_device_link::ns_smart_objects::SmartObject& consents) {
  std::vector<bool> module_consents;
  for (const auto& allowed_item : (*consents.asArray())) {
    module_consents.push_back(allowed_item.asBool());
  }
  return module_consents;
}

const std::function<std::string(const std::string& module_type)>
RCHelpers::GetModuleTypeToDataMapping() {
  auto mapping_lambda = [](const std::string& module_type) -> std::string {
    static std::map<std::string, std::string> mapping = {
        {enums_value::kClimate, message_params::kClimateControlData},
        {enums_value::kRadio, message_params::kRadioControlData},
        {enums_value::kSeat, message_params::kSeatControlData},
        {enums_value::kAudio, message_params::kAudioControlData},
        {enums_value::kLight, message_params::kLightControlData},
        {enums_value::kHmiSettings, message_params::kHmiSettingsControlData}};
    auto it = mapping.find(module_type);
    if (mapping.end() == it) {
      LOG4CXX_ERROR(logger_, "Unknown module type" << module_type);
      return std::string();
    }
    return it->second;
  };

  return mapping_lambda;
}

const std::function<std::string(const std::string& module_type)>
RCHelpers::GetModuleTypeToCapabilitiesMapping() {
  auto mapping_lambda = [](const std::string& module_type) -> std::string {
    static std::map<std::string, std::string> mapping = {
        {enums_value::kClimate, strings::kclimateControlCapabilities},
        {enums_value::kRadio, strings::kradioControlCapabilities},
        {enums_value::kSeat, strings::kseatControlCapabilities},
        {enums_value::kAudio, strings::kaudioControlCapabilities},
        {enums_value::kLight, strings::klightControlCapabilities},
        {enums_value::kHmiSettings, strings::khmiSettingsControlCapabilities}};
    auto it = mapping.find(module_type);
    if (mapping.end() == it) {
      LOG4CXX_ERROR(logger_, "Unknown module type" << module_type);
      return std::string();
    }
    return it->second;
  };

  return mapping_lambda;
}

const std::vector<std::string> RCHelpers::GetModuleTypesList() {
  using namespace enums_value;
  return {kClimate, kRadio, kSeat, kAudio, kLight, kHmiSettings};
}

RCAppExtensionPtr RCHelpers::GetRCExtension(
    application_manager::Application& app) {
  auto extension_interface = app.QueryInterface(RCRPCPlugin::kRCPluginID);
  auto extension =
      std::static_pointer_cast<RCAppExtension>(extension_interface);
  return extension;
}

smart_objects::SmartObjectSPtr RCHelpers::CreateUnsubscribeRequestToHMI(
    const ModuleUid& module, const uint32_t correlation_id) {
  using namespace smart_objects;
  namespace commands = application_manager::commands;
  namespace am_strings = application_manager::strings;

  SmartObjectSPtr message = std::make_shared<SmartObject>(SmartType_Map);
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
  msg_params[message_params::kModuleType] = module.first;
  msg_params[message_params::kModuleId] = module.second;
  return message;
}

std::vector<application_manager::ApplicationSharedPtr>
RCHelpers::AppsSubscribedToModule(
    application_manager::ApplicationManager& app_mngr,
    const ModuleUid& module) {
  std::vector<application_manager::ApplicationSharedPtr> result;
  auto rc_apps = RCRPCPlugin::GetRCApplications(app_mngr);
  for (auto& app : rc_apps) {
    auto rc_ext = RCHelpers::GetRCExtension(*app);
    DCHECK_OR_RETURN(rc_ext, result);
    if (rc_ext->IsSubscribedToInteriorVehicleData(module)) {
      result.push_back(app);
    }
  }
  return result;
}

std::vector<application_manager::ApplicationSharedPtr>
RCHelpers::AppsSubscribedToModuleType(
    application_manager::ApplicationManager& app_mngr,
    const std::string& module_type) {
  std::vector<application_manager::ApplicationSharedPtr> result;
  auto rc_apps = RCRPCPlugin::GetRCApplications(app_mngr);
  for (auto& app : rc_apps) {
    auto rc_ext = RCHelpers::GetRCExtension(*app);
    DCHECK_OR_RETURN(rc_ext, result);
    if (rc_ext->IsSubscribedToInteriorVehicleDataOfType(module_type)) {
      result.push_back(app);
    }
  }
  return result;
}

RCHelpers::AppsModuleTypes RCHelpers::GetApplicationsAllowedModuleTypes(
    app_mngr::ApplicationManager& app_mngr) {
  auto apps_list = RCRPCPlugin::GetRCApplications(app_mngr);
  RCHelpers::AppsModuleTypes result;
  for (auto& app_ptr : apps_list) {
    std::vector<std::string> allowed_modules;
    app_mngr.GetPolicyHandler().GetModuleTypes(app_ptr->policy_app_id(),
                                               &allowed_modules);
    std::sort(allowed_modules.begin(), allowed_modules.end());
    result[app_ptr] = allowed_modules;
  }
  return result;
}

void RCHelpers::RemoveRedundantGPSDataFromIVDataMsg(
    smart_objects::SmartObject& msg_params) {
  using namespace message_params;
  using namespace application_manager::strings;

  LOG4CXX_AUTO_TRACE(logger_);
  auto& module_data = msg_params[kModuleData];
  if (!module_data.keyExists(kRadioControlData) ||
      !module_data[kRadioControlData].keyExists(kSisData) ||
      !module_data[kRadioControlData][kSisData].keyExists(station_location)) {
    return;
  }

  auto& location_data =
      module_data[kRadioControlData][kSisData][station_location];
  auto new_location_data =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  new_location_data[latitude_degrees] = location_data[latitude_degrees];
  new_location_data[longitude_degrees] = location_data[longitude_degrees];
  if (location_data.keyExists(altitude)) {
    new_location_data[altitude] = location_data[altitude];

    location_data = new_location_data;
  }
}
}  // namespace rc_rpc_plugin
