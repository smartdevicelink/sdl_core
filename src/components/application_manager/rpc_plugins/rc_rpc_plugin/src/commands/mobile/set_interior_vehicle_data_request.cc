/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

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
  } else if (enums_value::kLight == module_type) {
    module_ro_params.push_back(kStatus);
  }

  return module_ro_params;
}

const std::map<std::string, std::string> GetModuleTypeToDataMapping() {
  std::map<std::string, std::string> mapping = {
      {enums_value::kRadio, message_params::kRadioControlData},
      {enums_value::kClimate, message_params::kClimateControlData},
      {enums_value::kAudio, message_params::kAudioControlData},
      {enums_value::kLight, message_params::kLightControlData},
      {enums_value::kHmiSettings, message_params::kHmiSettingsControlData},
      {enums_value::kSeat, message_params::kSeatControlData}};
  return mapping;
}

const std::map<std::string, std::string> GetLightCapabilitiesMapping() {
  std::map<std::string, std::string> mapping = {
      {message_params::kId, strings::kName},
      {message_params::kStatus, strings::kStatusAvailable},
      {message_params::kDensity, strings::kDensityAvailable},
      {message_params::kSRGBColor, strings::kSRGBColorSpaceAvailable}};
  return mapping;
}

const std::map<std::string, std::string> GetModuleDataCapabilitiesMapping() {
  std::map<std::string, std::string> mapping = {
      {message_params::kRadioControlData, strings::kradioControlCapabilities},
      {message_params::kClimateControlData,
       strings::kclimateControlCapabilities},
      {message_params::kAudioControlData, strings::kaudioControlCapabilities},
      {message_params::kLightControlData, strings::klightControlCapabilities},
      {message_params::kSeatControlData, strings::kseatControlCapabilities},
      {message_params::kHmiSettingsControlData,
       strings::khmiSettingsControlCapabilities}};
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

  // seat
  mapping["heatingEnabled"] = "heatingEnabledAvailable";
  mapping["coolingEnabled"] = "coolingEnabledAvailable";
  mapping["heatingLevel"] = "heatingLevelAvailable";
  mapping["coolingLevel"] = "coolingLevelAvailable";
  mapping["horizontalPosition"] = "horizontalPositionAvailable";
  mapping["verticalPosition"] = "verticalPositionAvailable";
  mapping["frontVerticalPosition"] = "frontVerticalPositionAvailable";
  mapping["backVerticalPosition"] = "backVerticalPositionAvailable";
  mapping["backTiltAngle"] = "backTiltAngleAvailable";
  mapping["headSupportHorizontalPosition"] =
      "headSupportHorizontalPositionAvailable";
  mapping["headSupportVerticalPosition"] =
      "headSupportVerticalPositionAvailable";
  mapping["massageEnabled"] = "massageEnabledAvailable";
  mapping["massageMode"] = "massageModeAvailable";
  mapping["massageCushionFirmness"] = "massageCushionFirmnessAvailable";
  mapping["memory"] = "memoryAvailable";
  // audio
  mapping["source"] = "sourceAvailable";
  mapping["keepContext"] = "keepContextAvailable";
  mapping["volume"] = "volumeAvailable";
  mapping["equalizerSettings"] = "equalizerAvailable";

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

mobile_apis::Result::eType GetItemCapability(
    const smart_objects::SmartObject& capabilities,
    const std::map<std::string, std::string>& mapping,
    const std::string& request_parameter,
    const mobile_apis::Result::eType& switched_off_result) {
  const auto it = mapping.find(request_parameter);

  if (it == mapping.end()) {
    LOG4CXX_DEBUG(logger_,
                  "Parameter " << request_parameter
                               << " doesn't exist in capabilities.");
    return mobile_apis::Result::UNSUPPORTED_RESOURCE;
  }

  const std::string& caps_key = it->second;

  LOG4CXX_DEBUG(logger_,
                "Checking request parameter "
                    << request_parameter
                    << " with capabilities. Appropriate key is " << caps_key);

  if (!capabilities.keyExists(caps_key)) {
    LOG4CXX_DEBUG(logger_,
                  "Capability " << caps_key
                                << " is missed in RemoteControl capabilities");
    return mobile_apis::Result::UNSUPPORTED_RESOURCE;
  }

  if (!capabilities[caps_key].asBool()) {
    LOG4CXX_DEBUG(logger_,
                  "Capability "
                      << caps_key
                      << " is switched off in RemoteControl capabilities");
    return switched_off_result;
  }

  return mobile_apis::Result::SUCCESS;
}

mobile_apis::Result::eType GetLightDataCapabilities(
    const smart_objects::SmartObject& capabilities,
    const smart_objects::SmartObject& control_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::map<std::string, std::string> mapping = GetLightCapabilitiesMapping();

  for (auto it = control_data.map_begin(); it != control_data.map_end(); ++it) {
    const std::string& request_parameter = it->first;

    if (message_params::kId == request_parameter) {
      continue;
    }

    const mobile_apis::Result::eType item_capability =
        GetItemCapability(capabilities,
                          mapping,
                          request_parameter,
                          mobile_apis::Result::READ_ONLY);

    if (mobile_apis::Result::SUCCESS != item_capability) {
      return item_capability;
    }
  }

  return mobile_apis::Result::SUCCESS;
}

mobile_apis::Result::eType GetLightNameCapabilities(
    const smart_objects::SmartObject& capabilities_status,
    const smart_objects::SmartObject& light_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto it = capabilities_status.asArray()->begin();

  for (; it != capabilities_status.asArray()->end(); ++it) {
    const smart_objects::SmartObject& so = *it;
    const int64_t current_id = so[message_params::kName].asInt();

    if (current_id == light_data[message_params::kId].asInt()) {
      return GetLightDataCapabilities(so, light_data);
    }
  }

  LOG4CXX_DEBUG(logger_, "There is no such light name in capabilities");
  return mobile_apis::Result::UNSUPPORTED_RESOURCE;
}

ModuleCapability GetControlDataCapabilities(
    const smart_objects::SmartObject& capabilities,
    const smart_objects::SmartObject& control_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::map<std::string, std::string> mapping =
      GetModuleDataToCapabilitiesMapping();

  for (auto it = control_data.map_begin(); it != control_data.map_end(); ++it) {
    const std::string& request_parameter = it->first;

    if (message_params::kLightState == request_parameter) {
      auto light_data = control_data[request_parameter].asArray()->begin();
      mobile_apis::Result::eType light_capability =
          mobile_apis::Result::SUCCESS;

      for (; light_data != control_data[request_parameter].asArray()->end();
           ++light_data) {
        light_capability = GetLightNameCapabilities(
            capabilities[strings::kSupportedLights], *light_data);

        if (mobile_apis::Result::SUCCESS != light_capability) {
          return std::make_pair(request_parameter, light_capability);
          ;
        }
      }

      return std::make_pair(request_parameter, light_capability);
      ;
    }

    const mobile_apis::Result::eType item_capability =
        GetItemCapability(capabilities[0],
                          mapping,
                          request_parameter,
                          mobile_apis::Result::UNSUPPORTED_RESOURCE);

    if (mobile_apis::Result::SUCCESS != item_capability) {
      return std::make_pair(request_parameter, item_capability);
    }
  }

  return std::make_pair("", mobile_apis::Result::SUCCESS);
}

ModuleCapability GetHmiControlDataCapabilities(
    const smart_objects::SmartObject& capabilities,
    const smart_objects::SmartObject& control_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::map<std::string, std::string> mapping =
      GetModuleDataToCapabilitiesMapping();

  for (auto it = control_data.map_begin(); it != control_data.map_end(); ++it) {
    const mobile_apis::Result::eType item_capability =
        GetItemCapability(capabilities,
                          mapping,
                          it->first,
                          mobile_apis::Result::UNSUPPORTED_RESOURCE);

    if (mobile_apis::Result::SUCCESS != item_capability) {
      return std::make_pair(it->first, item_capability);
    }
  }

  return std::make_pair("", mobile_apis::Result::SUCCESS);
}

ModuleCapability GetModuleDataCapabilities(
    const smart_objects::SmartObject& rc_capabilities,
    const smart_objects::SmartObject& module_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::map<std::string, std::string> params =
      GetModuleDataCapabilitiesMapping();
  ModuleCapability module_data_capabilities =
      std::make_pair("", mobile_apis::Result::UNSUPPORTED_RESOURCE);

  for (const auto& param : params) {
    if (module_data.keyExists(param.first)) {
      if (!rc_capabilities.keyExists(param.second)) {
        LOG4CXX_DEBUG(logger_, param.first << " capabilities not present");
        return module_data_capabilities;
      }

      const smart_objects::SmartObject& caps = rc_capabilities[param.second];

      if (message_params::kHmiSettingsControlData == param.first) {
        module_data_capabilities =
            GetHmiControlDataCapabilities(caps, module_data[param.first]);
      } else {
        module_data_capabilities =
            GetControlDataCapabilities(caps[0], module_data[param.first]);
      }
    }
  }

  return module_data_capabilities;
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
      break;
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
    ModuleCapability module_data_capabilities;

    if (rc_capabilities) {
      module_data_capabilities =
          GetModuleDataCapabilities(*rc_capabilities, module_data);

      if (mobile_apis::Result::SUCCESS != module_data_capabilities.second) {
        SetResourceState(ModuleType(), ResourceState::FREE);
        std::string responce_msg;

        if (message_params::kLightState == module_data_capabilities.first) {
          responce_msg =
              "The requested LightName is not supported by the vehicle.";
        } else {
          responce_msg = "Accessing not supported module data";
        }

        LOG4CXX_WARN(logger_, responce_msg);

        SendResponse(
            false, module_data_capabilities.second, responce_msg.c_str());

        return;
      }
    }

    if (AreAllParamsReadOnly(module_data)) {
      LOG4CXX_WARN(logger_, "All request params in module type are READ ONLY!");
      SetResourceState(ModuleType(), ResourceState::FREE);
      SendResponse(false,
                   mobile_apis::Result::READ_ONLY,
                   "All request params in module type are READ ONLY!");
      return;
    }

    module_data_capabilities = std::make_pair("", mobile_apis::Result::SUCCESS);

    if (AreReadOnlyParamsPresent(module_data, module_data_capabilities)) {
      LOG4CXX_DEBUG(logger_, "Request module type has READ ONLY parameters");

      if (enums_value::kLight == module_data_capabilities.first &&
          mobile_apis::Result::SUCCESS != module_data_capabilities.second) {
        SetResourceState(ModuleType(), ResourceState::FREE);
        SendResponse(
            false,
            module_data_capabilities.second,
            "The LightStatus enum passed is READ ONLY and cannot be written.");
        return;
      }

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
    if (enums_value::kAudio == ModuleType()) {
      CheckAudioSource((
          *message_)[app_mngr::strings::msg_params][message_params::kModuleData]
                    [message_params::kAudioControlData]);
    }
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
  const bool should_keep_context =
      audio_data.keyExists(message_params::kKeepContext) &&
      audio_data[message_params::kKeepContext].asBool();
  const bool switch_source_from_app =
      mobile_apis::PrimaryAudioSource::MOBILE_APP ==
          application_manager_.get_current_audio_source() &&
      mobile_apis::PrimaryAudioSource::MOBILE_APP !=
          audio_data[message_params::kSource].asInt();
  if (!should_keep_context && switch_source_from_app) {
    app_mngr::ApplicationSharedPtr app =
        application_manager_.application(connection_key());
    if (app->mobile_projection_enabled()) {
      application_manager_.ChangeAppsHMILevel(
          app->app_id(), mobile_apis::HMILevel::eType::HMI_LIMITED);
    } else {
      application_manager_.ChangeAppsHMILevel(
          app->app_id(), mobile_apis::HMILevel::eType::HMI_BACKGROUND);
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

  LOG4CXX_DEBUG(logger_, "All params are ReadOnly");
  return true;
}

bool CheckReadOnlyParamsForAudio(
    const smart_objects::SmartObject& module_type_params) {
  if (module_type_params.keyExists(message_params::kEqualizerSettings)) {
    const auto& equalizer_settings =
        module_type_params[message_params::kEqualizerSettings];
    auto it = equalizer_settings.asArray()->begin();

    for (; it != equalizer_settings.asArray()->end(); ++it) {
      if (it->keyExists(message_params::kChannelName)) {
        LOG4CXX_DEBUG(logger_,
                      "READ ONLY parameter. ChannelName = "
                          << (*it)[message_params::kChannelName].asString());
        return true;
      }
    }
  }

  return false;
}

bool CheckReadOnlyParamsForLight(
    const smart_objects::SmartObject& module_type_params) {
  if (module_type_params.keyExists(message_params::kLightState)) {
    const auto& light_state = module_type_params[message_params::kLightState];
    auto it = light_state.asArray()->begin();

    for (; it != light_state.asArray()->end(); ++it) {
      if (it->keyExists(message_params::kStatus)) {
        const mobile_apis::LightStatus::eType light_status =
            static_cast<mobile_apis::LightStatus::eType>(
                (*it)[message_params::kStatus].asUInt());

        if (helpers::Compare<mobile_apis::LightStatus::eType,
                             helpers::EQ,
                             helpers::ONE>(light_status,
                                           mobile_apis::LightStatus::RAMP_UP,
                                           mobile_apis::LightStatus::RAMP_DOWN,
                                           mobile_apis::LightStatus::UNKNOWN,
                                           mobile_apis::LightStatus::INVALID)) {
          LOG4CXX_DEBUG(logger_,
                        "READ ONLY parameter. Status = "
                            << (*it)[message_params::kStatus].asInt());
          return true;
        }
      }
    }
  }

  return false;
}

bool SetInteriorVehicleDataRequest::AreReadOnlyParamsPresent(
    const smart_objects::SmartObject& module_data,
    ModuleCapability& module_data_capabilities) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& module_type_params =
      ControlData(module_data);
  const std::string module_type = ModuleType();

  if (enums_value::kAudio == module_type) {
    return CheckReadOnlyParamsForAudio(module_type_params);
  }

  if (enums_value::kLight == module_type) {
    const bool result = CheckReadOnlyParamsForLight(module_type_params);

    if (result) {
      module_data_capabilities =
          std::make_pair(module_type, mobile_apis::Result::READ_ONLY);
    }

    return result;
  }

  const std::vector<std::string> ro_params =
      GetModuleReadOnlyParams(module_type);
  auto it = module_type_params.map_begin();

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
      } else if (enums_value::kRadio == module_type) {
        module_data[message_params::kRadioControlData].erase(it);
      } else {
        continue;
      }

      LOG4CXX_DEBUG(logger_, "Cutting-off READ ONLY parameter: " << it);
    }
  }

  if (enums_value::kAudio == module_type) {
    auto& equalizer_settings = module_data[message_params::kAudioControlData]
                                          [message_params::kEqualizerSettings];
    auto it = equalizer_settings.asArray()->begin();
    for (; it != equalizer_settings.asArray()->end(); ++it) {
      if (it->keyExists(message_params::kChannelName)) {
        it->erase(message_params::kChannelName);
        LOG4CXX_DEBUG(logger_,
                      "Cutting-off READ ONLY parameter: "
                          << message_params::kChannelName);
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
