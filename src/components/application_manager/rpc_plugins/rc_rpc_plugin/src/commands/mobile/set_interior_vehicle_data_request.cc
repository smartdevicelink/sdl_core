#include "rc_rpc_plugin/commands/mobile/set_interior_vehicle_data_request.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/macro.h"
#include "json/json.h"
#include "utils/helpers.h"
#include "interfaces/MOBILE_API.h"

namespace rc_rpc_plugin {
namespace commands {

using namespace json_keys;
using namespace message_params;

namespace {
std::vector<std::string> GetModuleReadOnlyParams(
    const std::string& module_type) {
  std::vector<std::string> module_ro_params;
  if (enums_value::kClimate == module_type) {
    module_ro_params.push_back(kCurrentTemperature);
  } else if (enums_value::kRadio == module_type) {
    module_ro_params.push_back(kRdsData);
    module_ro_params.push_back(kAvailableHDs);
    module_ro_params.push_back(kSignalStrength);
    module_ro_params.push_back(kSignalChangeThreshold);
    module_ro_params.push_back(kState);
    module_ro_params.push_back(kSisData);
  }
  return module_ro_params;
}

const std::map<std::string, std::string> GetModuleTypeToDataMapping() {
  std::map<std::string, std::string> mapping = {
      {enums_value::kRadio, message_params::kRadioControlData},
      {enums_value::kClimate, message_params::kClimateControlData},
      {enums_value::kAudio, message_params::kAudioControlData},
      {enums_value::kLight, message_params::kLightControlData},
      {enums_value::kHmiSettings, message_params::kHmiSettingsControlData}};
  return mapping;
}

const std::map<std::string, std::string> GetModuleDataToCapabilitiesMapping() {
  std::map<std::string, std::string> mapping;
  // climate
  mapping["fanSpeed"] = "fanSpeedAvailable";
  mapping["currentTemperature"] = "currentTemperatureAvailable";
  mapping["desiredTemperature"] = "desiredTemperatureAvailable";
  mapping["acEnable"] = "acEnableAvailable";
  mapping["circulateAirEnable"] = "circulateAirEnableAvailable";
  mapping["autoModeEnable"] = "autoModeEnableAvailable";
  mapping["defrostZone"] = "defrostZoneAvailable";
  mapping["dualModeEnable"] = "dualModeEnableAvailable";
  mapping["acMaxEnable"] = "acMaxEnableAvailable";
  mapping["ventilationMode"] = "ventilationModeAvailable";
  mapping["heatedSteeringWheelEnable"] = "heatedSteeringWheelAvailable";
  mapping["heatedWindshieldEnable"] = "heatedWindshieldAvailable";
  mapping["heatedMirrorsEnable"] = "heatedMirrorsAvailable";
  mapping["heatedRearWindowEnable"] = "heatedRearWindowAvailable";

  // radio
  mapping["band"] = "radioBandAvailable";
  mapping["frequencyInteger"] = "radioFrequencyAvailable";
  mapping["frequencyFraction"] = "radioFrequencyAvailable";
  mapping["rdsData"] = "rdsDataAvailable";
  mapping["availableHDs"] = "availableHDsAvailable";
  mapping["hdChannel"] = "availableHDsAvailable";
  mapping["signalStrength"] = "signalStrengthAvailable";
  mapping["signalChangeThreshold"] = "signalChangeThresholdAvailable";
  mapping["radioEnable"] = "radioEnableAvailable";
  mapping["state"] = "stateAvailable";
  mapping["sisData"] = "sisDataAvailable";

  // audio
  mapping["source"] = "sourceAvailable";
  mapping["keepContext"] = "sourceAvailable";
  mapping["volume"] = "volumeAvailable";
  mapping["equalizerSettings"] = "equalizerAvailable";

  // light
  mapping["supportedLights"] = "";

  // hmi settings
  mapping["distanceUnit"] = "distanceUnitAvailable";
  mapping["temperatureUnit"] = "temperatureUnitAvailable";
  mapping["displayMode"] = "displayModeUnitAvailable";

  return mapping;
}
}  // namespace

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

SetInteriorVehicleDataRequest::SetInteriorVehicleDataRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle,
    ResourceAllocationManager& resource_allocation_manager)
    : RCCommandRequest(message,
                       application_manager,
                       rpc_service,
                       hmi_capabilities,
                       policy_handle,
                       resource_allocation_manager) {}

SetInteriorVehicleDataRequest::~SetInteriorVehicleDataRequest() {}

const std::string LightName(const smart_objects::SmartObject& light_name) {
  const char* name;
  const bool ok = NsSmartDeviceLink::NsSmartObjects::
      EnumConversionHelper<mobile_apis::LightName::eType>::EnumToCString(
          static_cast<mobile_apis::LightName::eType>(light_name.asUInt()),
          &name);
  return ok ? name : "unknown";
}

bool CheckLightDataByCapabilities(
    const smart_objects::SmartObject& capabilities,
    const smart_objects::SmartObject& light_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::map<std::string, std::string> lightCapsMapping = {
      {message_params::kId, strings::kName},
      {message_params::kDensity, strings::kDensityAvailable},
      {message_params::kSRGBColor, strings::kSRGBColorSpaceAvailable}};
  auto it = light_data.map_begin();
  for (; it != light_data.map_end(); ++it) {
    if (message_params::kStatus == it->first ||
        message_params::kId == it->first) {
      continue;
    }
    const std::string& caps_key = lightCapsMapping[it->first];
    LOG4CXX_DEBUG(logger_,
                  "Checking request parameter "
                      << it->first << " with capabilities. Appropriate key is "
                      << caps_key);

    if (!capabilities.keyExists(caps_key)) {
      LOG4CXX_DEBUG(logger_,
                    "Capability "
                        << caps_key
                        << " is missed in RemoteControl capabilities");
      return false;
    }
    if (!capabilities[caps_key].asBool()) {
      LOG4CXX_DEBUG(logger_,
                    "Capability "
                        << caps_key
                        << " is switched off in RemoteControl capabilities");
      return false;
    }
  }
  return true;
}

bool CheckLightNameByCapabilities(
    const smart_objects::SmartObject& capabilities_status,
    const smart_objects::SmartObject& light_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto it = capabilities_status.asArray()->begin();
  for (; it != capabilities_status.asArray()->end(); ++it) {
    const smart_objects::SmartObject& so = *it;
    const int64_t current_id = so[message_params::kName].asInt();
    if (current_id == light_data[message_params::kId].asInt()) {
      return CheckLightDataByCapabilities(so, light_data);
    }
  }
  LOG4CXX_DEBUG(logger_, "There is no such light name in capabilities");
  return false;
}

bool CheckControlDataByCapabilities(
    const smart_objects::SmartObject& module_caps,
    const smart_objects::SmartObject& control_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::map<std::string, std::string> mapping =
      GetModuleDataToCapabilitiesMapping();
  const smart_objects::SmartObject& capabilities_status = module_caps[0];
  auto it = control_data.map_begin();
  for (; it != control_data.map_end(); ++it) {
    const std::string& request_parameter = it->first;
    if (message_params::kLightState == request_parameter) {
      return CheckLightNameByCapabilities(
          module_caps[strings::kSupportedLights],
          control_data[request_parameter][0]);
    }
    const std::string& caps_key = mapping[request_parameter];
    LOG4CXX_DEBUG(logger_,
                  "Checking request parameter "
                      << request_parameter
                      << " with capabilities. Appropriate key is " << caps_key);

    if (!capabilities_status.keyExists(caps_key)) {
      LOG4CXX_DEBUG(logger_,
                    "Capability "
                        << caps_key
                        << " is missed in RemoteControl capabilities");
      return false;
    }
    if (!capabilities_status[caps_key].asBool()) {
      LOG4CXX_DEBUG(logger_,
                    "Capability "
                        << caps_key
                        << " is switched off in RemoteControl capabilities");
      return false;
    }
  }
  return true;
}

bool CheckHmiControlDataByCapabilities(
    const smart_objects::SmartObject& module_caps,
    const smart_objects::SmartObject& control_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::map<std::string, std::string> mapping =
      GetModuleDataToCapabilitiesMapping();
  const smart_objects::SmartObject& capabilities_status = module_caps;
  auto it = control_data.map_begin();
  for (; it != control_data.map_end(); ++it) {
    const std::string& request_parameter = it->first;
    const std::string& caps_key = mapping[request_parameter];
    LOG4CXX_DEBUG(logger_,
                  "Checking request parameter "
                      << request_parameter
                      << " with capabilities. Appropriate key is " << caps_key);
    if (!capabilities_status.keyExists(caps_key)) {
      LOG4CXX_DEBUG(logger_,
                    "Capability "
                        << caps_key
                        << " is missed in RemoteControl capabilities");
      return false;
    }
    if (!capabilities_status[caps_key].asBool()) {
      LOG4CXX_DEBUG(logger_,
                    "Capability "
                        << caps_key
                        << " is switched off in RemoteControl capabilities");
      return false;
    }
  }
  return true;
}

bool CheckIfModuleDataExistInCapabilities(
    const smart_objects::SmartObject& rc_capabilities,
    const smart_objects::SmartObject& module_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::map<std::string, std::string> params = {
      {message_params::kRadioControlData, strings::kradioControlCapabilities},
      {message_params::kClimateControlData,
       strings::kclimateControlCapabilities},
      {message_params::kAudioControlData, strings::kaudioControlCapabilities},
      {message_params::kLightControlData, strings::klightControlCapabilities},
      {message_params::kHmiSettingsControlData,
       strings::khmiSettingsControlCapabilities}};
  bool is_module_data_valid = false;
  for (const auto& param : params) {
    if (module_data.keyExists(param.first)) {
      if (!rc_capabilities.keyExists(param.second)) {
        LOG4CXX_DEBUG(logger_, param.first << " capabilities not present");
        return false;
      }
      const smart_objects::SmartObject& caps = rc_capabilities[param.second];
      if (message_params::kHmiSettingsControlData == param.first) {
        is_module_data_valid =
            CheckHmiControlDataByCapabilities(caps, module_data[param.first]);
      } else {
        is_module_data_valid =
            CheckControlDataByCapabilities(caps, module_data[param.first]);
      }
    }
  }
  return is_module_data_valid;
}

bool isModuleTypeAndDataMatch(const std::string& module_type,
                              const smart_objects::SmartObject& module_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::map<std::string, std::string> data_mapping =
      GetModuleTypeToDataMapping();
  bool module_type_and_data_match = false;
  for (const auto& data : data_mapping) {
    if (data.first == module_type) {
      module_type_and_data_match = module_data.keyExists(data.second);
    }
  }
  return module_type_and_data_match;
}

void SetInteriorVehicleDataRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject& module_data =
      (*message_)[app_mngr::strings::msg_params][message_params::kModuleData];
  const std::string module_type = ModuleType();

  if (isModuleTypeAndDataMatch(module_type, module_data)) {
    const smart_objects::SmartObject* rc_capabilities =
        hmi_capabilities_.rc_capability();
    if (rc_capabilities &&
        !CheckIfModuleDataExistInCapabilities(*rc_capabilities, module_data)) {
      LOG4CXX_WARN(logger_, "Accessing not supported module data");
      SetResourceState(ModuleType(), ResourceState::FREE);
      SendResponse(false,
                   mobile_apis::Result::UNSUPPORTED_RESOURCE,
                   "Accessing not supported module data");
      return;
    }
    if (AreAllParamsReadOnly(module_data)) {
      LOG4CXX_WARN(logger_, "All request params in module type are READ ONLY!");
      SetResourceState(ModuleType(), ResourceState::FREE);
      SendResponse(false,
                   mobile_apis::Result::READ_ONLY,
                   "All request params in module type are READ ONLY!");
      return;
    }
    if (AreReadOnlyParamsPresent(module_data)) {
      LOG4CXX_DEBUG(logger_, "Request module type has READ ONLY parameters");
      LOG4CXX_DEBUG(logger_, "Cutting-off READ ONLY parameters... ");
      CutOffReadOnlyParams(module_data);
    }
    application_manager_.RemoveHMIFakeParameters(message_);

    app_mngr::ApplicationSharedPtr app =
        application_manager_.application(connection_key());
    (*message_)[app_mngr::strings::msg_params][app_mngr::strings::app_id] =
        app->app_id();

    const bool app_wants_to_set_audio_src =
        module_data.keyExists(message_params::kAudioControlData) &&
        module_data[message_params::kAudioControlData].keyExists(
            message_params::kSource);

    if (app_wants_to_set_audio_src && !app->IsAllowedToChangeAudioSource()) {
      LOG4CXX_WARN(logger_, "App is not allowed to change audio source");
      SetResourceState(ModuleType(), ResourceState::FREE);
      SendResponse(false,
                   mobile_apis::Result::REJECTED,
                   "App is not allowed to change audio source");
      return;
    }

    SendHMIRequest(hmi_apis::FunctionID::RC_SetInteriorVehicleData,
                   &(*message_)[app_mngr::strings::msg_params],
                   true);

    if (enums_value::kAudio == module_type) {
      CheckAudioSource(module_data[message_params::kAudioControlData]);
    }
  } else {
    LOG4CXX_WARN(logger_, "Request module type & data mismatch!");
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "Request module type & data mismatch!");
  }
}

void SetInteriorVehicleDataRequest::on_event(
    const app_mngr::event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  RCCommandRequest::on_event(event);

  if (hmi_apis::FunctionID::RC_SetInteriorVehicleData != event.id()) {
    return;
  }

  const smart_objects::SmartObject& hmi_response = event.smart_object();
  mobile_apis::Result::eType result_code =
      GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
          hmi_response[app_mngr::strings::params][app_mngr::hmi_response::code]
              .asUInt()));

  bool result =
      helpers::Compare<mobile_apis::Result::eType, helpers::EQ, helpers::ONE>(
          result_code,
          mobile_apis::Result::SUCCESS,
          mobile_apis::Result::WARNINGS);

  smart_objects::SmartObject response_params;
  if (result) {
    response_params = hmi_response[app_mngr::strings::msg_params];
  }
  std::string info;
  GetInfo(hmi_response, info);
  SendResponse(
      result, result_code, info.c_str(), result ? &response_params : nullptr);
}

const smart_objects::SmartObject& SetInteriorVehicleDataRequest::ControlData(
    const smart_objects::SmartObject& module_data) {
  const std::string module_type = ModuleType();
  std::map<std::string, std::string> data_mapping =
      GetModuleTypeToDataMapping();
  for (const auto& data : data_mapping) {
    if (data.first == module_type) {
      return module_data[data.second];
    }
  }
  NOTREACHED();
  return module_data[0];
}

void SetInteriorVehicleDataRequest::CheckAudioSource(
    const smart_objects::SmartObject& audio_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (mobile_apis::PrimaryAudioSource::MOBILE_APP !=
      audio_data[message_params::kSource].asInt()) {
    if (!audio_data.keyExists(message_params::kKeepContext) ||
        !audio_data[message_params::kKeepContext].asBool()) {
      if (mobile_apis::PrimaryAudioSource::MOBILE_APP ==
          application_manager_.get_current_audio_source()) {
        app_mngr::ApplicationSharedPtr app =
            application_manager_.application(connection_key());
        application_manager_.ChangeAppsHMILevel(
            app->app_id(), mobile_apis::HMILevel::eType::HMI_BACKGROUND);
      }
    }
  }
  application_manager_.set_current_audio_source(
      audio_data[message_params::kSource].asUInt());
}

bool SetInteriorVehicleDataRequest::AreAllParamsReadOnly(
    const smart_objects::SmartObject& module_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& module_type_params =
      ControlData(module_data);
  auto it = module_type_params.map_begin();
  std::vector<std::string> ro_params = GetModuleReadOnlyParams(ModuleType());
  for (; it != module_type_params.map_end(); ++it) {
    if (!helpers::in_range(ro_params, it->first)) {
      return false;
    }
  }
  return true;
}

bool SetInteriorVehicleDataRequest::AreReadOnlyParamsPresent(
    const smart_objects::SmartObject& module_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& module_type_params =
      ControlData(module_data);
  auto it = module_type_params.map_begin();
  std::vector<std::string> ro_params = GetModuleReadOnlyParams(ModuleType());
  for (; it != module_type_params.map_end(); ++it) {
    if (helpers::in_range(ro_params, it->first)) {
      return true;
    }
  }
  return false;
}

void SetInteriorVehicleDataRequest::CutOffReadOnlyParams(
    smart_objects::SmartObject& module_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& module_type_params =
      ControlData(module_data);
  const std::string module_type = ModuleType();
  std::vector<std::string> ro_params = GetModuleReadOnlyParams(module_type);

  for (auto& it : ro_params) {
    if (module_type_params.keyExists(it)) {
      if (enums_value::kClimate == module_type) {
        module_data[message_params::kClimateControlData].erase(it);
        LOG4CXX_DEBUG(logger_, "Cutting-off READ ONLY parameter: " << it);
      } else if (enums_value::kRadio == module_type) {
        module_data[message_params::kRadioControlData].erase(it);
        LOG4CXX_DEBUG(logger_, "Cutting-off READ ONLY parameter: " << it);
      }
    }
  }
}

std::string SetInteriorVehicleDataRequest::ModuleType() {
  mobile_apis::ModuleType::eType module_type =
      static_cast<mobile_apis::ModuleType::eType>(
          (*message_)[app_mngr::strings::msg_params]
                     [message_params::kModuleData][message_params::kModuleType]
                         .asUInt());
  const char* str;
  const bool ok = NsSmartDeviceLink::NsSmartObjects::EnumConversionHelper<
      mobile_apis::ModuleType::eType>::EnumToCString(module_type, &str);
  return ok ? str : "unknown";
}

AcquireResult::eType SetInteriorVehicleDataRequest::AcquireResource(
    const app_mngr::commands::MessageSharedPtr& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string module_type = ModuleType();
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());
  return resource_allocation_manager_.AcquireResource(module_type,
                                                      app->app_id());
}

bool SetInteriorVehicleDataRequest::IsResourceFree(
    const std::string& module_type) const {
  return resource_allocation_manager_.IsResourceFree(module_type);
}

void SetInteriorVehicleDataRequest::SetResourceState(
    const std::string& module_type, const ResourceState::eType state) {
  LOG4CXX_AUTO_TRACE(logger_);
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());
  resource_allocation_manager_.SetResourceState(
      module_type, app->app_id(), state);
}

}  // namespace commands
}  // namespace rc_rpc_plugin
