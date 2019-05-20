/*
 Copyright (c) 2019, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the copyright holders nor the names of their contributors
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

#include "rc_rpc_plugin/rc_capabilities_manager_impl.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_module_constants.h"

namespace rc_rpc_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

RCCapabilitiesManagerImpl::RCCapabilitiesManagerImpl(
    application_manager::HMICapabilities& hmi_capabilities)
    : hmi_capabilities_(hmi_capabilities) {}

bool RCCapabilitiesManagerImpl::CheckIfModuleTypeExistInCapabilities(
    const std::string& module_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto rc_capabilities = *(hmi_capabilities_.rc_capability());
  const auto& mapping = RCHelpers::GetModuleTypeToCapabilitiesMapping();
  const auto& module_list = RCHelpers::GetModulesList();
  bool is_module_type_valid = false;
  for (const auto& module : module_list) {
    if (module == module_type) {
      if (rc_capabilities.keyExists(mapping(module))) {
        is_module_type_valid = true;
        break;
      }
    }
  }
  return is_module_type_valid;
}

bool RCCapabilitiesManagerImpl::CheckIfButtonExistInRCCaps(
    const mobile_apis::ButtonName::eType button) {
  auto rc_capabilities = *(hmi_capabilities_.rc_capability());
  if (rc_capabilities.keyExists(strings::kbuttonCapabilities)) {
    const smart_objects::SmartObject& button_caps =
        rc_capabilities[strings::kbuttonCapabilities];
    for (auto& button_cap : *(button_caps.asArray())) {
      int64_t current_id = button_cap[message_params::kName].asInt();
      if (-1 == current_id) {
        // capabilities received from HMI contains enum values
        // capabilities loaded from file contains string values
        // TODO : unificate capabilities storing
        const std::string& bt_name =
            button_cap[message_params::kName].asString();
        static RCHelpers::ButtonsMap btn_map = RCHelpers::buttons_map();
        current_id = btn_map[bt_name];
      }
      const mobile_apis::ButtonName::eType current_button =
          static_cast<mobile_apis::ButtonName::eType>(current_id);
      if (current_button == button) {
        LOG4CXX_TRACE(
            logger_,
            "Button id " << current_button << " exist in capabilities");
        return true;
      }
    }
  }
  LOG4CXX_TRACE(logger_,
                "Button id " << button << " do not exist in capabilities");
  return false;
}

bool RCCapabilitiesManagerImpl::CheckButtonName(
    const std::string& module_type, const std::string& button_name) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto rc_capabilities = hmi_capabilities_.rc_capability();
  if (!rc_capabilities) {
    LOG4CXX_ERROR(logger_, "No remote controll capabilities available");
    return false;
  }

  if (enums_value::kRadio == module_type) {
    if (!helpers::in_range(RCHelpers::buttons_radio(), button_name)) {
      LOG4CXX_WARN(logger_,
                   "Trying to acceess climate button with module type radio");
      return false;
    }
  }

  if (enums_value::kClimate == module_type) {
    if (!helpers::in_range(RCHelpers::buttons_climate(), button_name)) {
      LOG4CXX_WARN(logger_,
                   "Trying to acceess radio button with module type climate");
      return false;
    }
  }
  return true;
}

const std::map<std::string, std::string>
RCCapabilitiesManagerImpl::GetLightCapabilitiesMapping() {
  std::map<std::string, std::string> mapping = {
      {message_params::kId, strings::kName},
      {message_params::kLightStatus, strings::kStatusAvailable},
      {message_params::kLightDensity, strings::kDensityAvailable},
      {message_params::kLightColor, strings::kRGBColorSpaceAvailable}};
  return mapping;
}

const std::map<std::string, std::string>
RCCapabilitiesManagerImpl::GetModuleDataToCapabilitiesMapping() {
  std::map<std::string, std::string> mapping;
  using namespace message_params;
  using namespace rc_rpc_plugin::strings;
  // climate
  mapping[kFanSpeed] = kFanSpeedAvailable;
  mapping[kCurrentTemperature] = kCurrentTemperatureAvailable;
  mapping[kDesiredTemperature] = kDesiredTemperatureAvailable;
  mapping[kACEnable] = kAcEnableAvailable;
  mapping[kCirculateAirEnable] = kCirculateAirEnableAvailable;
  mapping[kAutoModeEnable] = kAutoModeEnableAvailable;
  mapping[kDefrostZone] = kDefrostZoneAvailable;
  mapping[kDualModeEnable] = kDualModeEnableAvailable;
  mapping[kACMaxEnable] = kAcMaxEnableAvailable;
  mapping[kVentilationMode] = kVentilationModeAvailable;
  mapping[kHeatedSteeringWheelEnable] = kHeatedSteeringWheelAvailable;
  mapping[kHeatedWindshieldEnable] = kHeatedWindshieldAvailable;
  mapping[kHeatedMirrorsEnable] = kHeatedMirrorsAvailable;
  mapping[kHeatedRearWindowEnable] = kHeatedRearWindowAvailable;
  mapping[kClimateEnable] = kClimateEnableAvailable;
  mapping[kClimateEnableAvailable] = kClimateEnableAvailable;

  // radio
  mapping[kBand] = kRadioBandAvailable;
  mapping[kFrequencyInteger] = kRadioFrequencyAvailable;
  mapping[kFrequencyFraction] = kRadioFrequencyAvailable;
  mapping[kRdsData] = kRdsDataAvailable;
  mapping[kAvailableHDs] = kAvailableHDsAvailable;
  mapping[kAvailableHdChannels] = kAvailableHdChannelsAvailable;
  mapping[kHdChannel] = kAvailableHdChannelsAvailable;
  mapping[kHdRadioEnable] = kHdRadioEnableAvailable;
  mapping[kSignalStrength] = kSignalStrengthAvailable;
  mapping[kSignalChangeThreshold] = kSignalChangeThresholdAvailable;
  mapping[kRadioEnable] = kRadioEnableAvailable;
  mapping[kState] = kStateAvailable;
  mapping[kSisData] = kSisDataAvailable;

  // seat
  mapping[kHeatingEnabled] = kHeatingEnabledAvailable;
  mapping[kCoolingEnabled] = kCoolingEnabledAvailable;
  mapping[kHeatingLevele] = kHeatingLevelAvailable;
  mapping[kCoolingLevel] = kCoolingLevelAvailable;
  mapping[kHorizontalPosition] = kHorizontalPositionAvailable;
  mapping[kVerticalPosition] = kVerticalPositionAvailable;
  mapping[kFrontVerticalPosition] = kFrontVerticalPositionAvailable;
  mapping[kBackVerticalPosition] = kBackVerticalPositionAvailable;
  mapping[kBackTiltAngle] = kBackTiltAngleAvailable;
  mapping[kHeadSupportHorizontalPosition] =
      kHeadSupportHorizontalPositionAvailable;
  mapping[kHeadSupportVerticalPosition] = kHeadSupportVerticalPositionAvailable;
  mapping[kMassageEnabled] = kMassageEnabledAvailable;
  mapping[kMassageMode] = kMassageModeAvailable;
  mapping[kMassageCushionFirmness] = kMassageCushionFirmnessAvailable;
  mapping[kMemory] = kMemoryAvailable;

  // audio
  mapping[kSource] = kSourceAvailable;
  mapping[kKeepContext] = kKeepContextAvailable;
  mapping[kVolume] = kVolumeAvailable;
  mapping[kEqualizerSettings] = kEqualizerAvailable;

  // hmi settings
  mapping[kDistanceUnit] = kDistanceUnitAvailable;
  mapping[kTemperatureUnit] = kTemperatureUnitAvailable;
  mapping[kDisplayMode] = kDisplayModeUnitAvailable;

  return mapping;
}

ModuleCapability RCCapabilitiesManagerImpl::GetModuleDataCapabilities(
    const smart_objects::SmartObject& module_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto rc_capabilities = *(hmi_capabilities_.rc_capability());

  const auto& all_module_types = RCHelpers::GetModulesList();
  const auto& get_module_data_key = RCHelpers::GetModuleTypeToDataMapping();
  const auto& get_capabilities_key =
      RCHelpers::GetModuleTypeToCapabilitiesMapping();
  ModuleCapability module_data_capabilities =
      std::make_pair("", capabilitiesStatus::missedParam);

  for (const auto& module_type : all_module_types) {
    const auto module_data_key = get_module_data_key(module_type);
    const auto capabilities_key = get_capabilities_key(module_type);
    if (module_data.keyExists(module_data_key)) {
      if (!rc_capabilities.keyExists(capabilities_key)) {
        LOG4CXX_DEBUG(logger_, module_data_key << " capabilities not present");
        return module_data_capabilities;
      }
      const auto& caps = rc_capabilities[capabilities_key];

      if (message_params::kHmiSettingsControlData == module_data_key ||
          message_params::kLightControlData == module_data_key) {
        module_data_capabilities =
            GetControlDataCapabilities(caps, module_data[module_data_key]);
      } else {
        module_data_capabilities =
            GetControlDataCapabilities(caps[0], module_data[module_data_key]);
      }
    }
  }

  return module_data_capabilities;
}

ModuleCapability RCCapabilitiesManagerImpl::GetControlDataCapabilities(
    const smart_objects::SmartObject& capabilities,
    const smart_objects::SmartObject& control_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::map<std::string, std::string> mapping =
      GetModuleDataToCapabilitiesMapping();

  for (auto it = control_data.map_begin(); it != control_data.map_end(); ++it) {
    const std::string& request_parameter = it->first;
    if (message_params::kId == request_parameter) {
      continue;
    }
    if (message_params::kLightState == request_parameter) {
      ModuleCapability light_capability =
          std::make_pair("", capabilitiesStatus::success);

      for (auto& light_data : *(control_data[request_parameter].asArray())) {
        light_capability = GetLightNameCapabilities(
            capabilities[strings::kSupportedLights], light_data);

        if (capabilitiesStatus::success != light_capability.second) {
          return light_capability;
        }
      }

      return light_capability;
    }
    if (message_params::kBand == request_parameter) {
      ModuleCapability radio_capability = GetRadioBandByCapabilities(
          capabilities, control_data[request_parameter]);
      if (capabilitiesStatus::success != radio_capability.second) {
        return radio_capability;
      }
    }

    const capabilitiesStatus status_item_capability =
        GetItemCapability(capabilities,
                          mapping,
                          request_parameter,
                          mobile_apis::Result::UNSUPPORTED_RESOURCE);

    if (capabilitiesStatus::success != status_item_capability) {
      return std::make_pair("", status_item_capability);
    }
  }

  return std::make_pair("", capabilitiesStatus::success);
}

capabilitiesStatus RCCapabilitiesManagerImpl::GetItemCapability(
    const smart_objects::SmartObject& capabilities,
    const std::map<std::string, std::string>& mapping,
    const std::string& request_parameter,
    const mobile_apis::Result::eType& switched_off_result) {
  const auto it = mapping.find(request_parameter);

  if (it == mapping.end()) {
    LOG4CXX_DEBUG(
        logger_,
        "Parameter " << request_parameter << " doesn't exist in capabilities.");
    return capabilitiesStatus::missedParam;
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
    return capabilitiesStatus::missedParam;
  }

  if (!capabilities[caps_key].asBool()) {
    LOG4CXX_DEBUG(logger_,
                  "Capability "
                      << caps_key
                      << " is switched off in RemoteControl capabilities");
    capabilitiesStatus status = capabilitiesStatus::missedParam;
    if (mobile_apis::Result::READ_ONLY == switched_off_result) {
      status = capabilitiesStatus::readOnly;
    }
    return status;
  }

  return capabilitiesStatus::success;
}

ModuleCapability RCCapabilitiesManagerImpl::GetLightDataCapabilities(
    const smart_objects::SmartObject& capabilities,
    const smart_objects::SmartObject& control_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::map<std::string, std::string> mapping = GetLightCapabilitiesMapping();

  for (auto it = control_data.map_begin(); it != control_data.map_end(); ++it) {
    const std::string& request_parameter = it->first;

    if (message_params::kId == request_parameter) {
      continue;
    }

    const capabilitiesStatus status_item_capability =
        GetItemCapability(capabilities,
                          mapping,
                          request_parameter,
                          mobile_apis::Result::READ_ONLY);

    if (capabilitiesStatus::success != status_item_capability) {
      return std::make_pair(message_params::kLightState,
                            status_item_capability);
    }
  }

  return std::make_pair("", capabilitiesStatus::success);
}

ModuleCapability RCCapabilitiesManagerImpl::GetLightNameCapabilities(
    const smart_objects::SmartObject& capabilities_status,
    const smart_objects::SmartObject& light_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  for (auto& so : *(capabilities_status.asArray())) {
    const int64_t current_id = so[message_params::kName].asInt();
    if (current_id == light_data[message_params::kId].asInt()) {
      return GetLightDataCapabilities(so, light_data);
    }
  }
  LOG4CXX_DEBUG(logger_, "There is no such light name in capabilities");
  return std::make_pair(message_params::kLightState,
                        capabilitiesStatus::missedLightName);
}

ModuleCapability RCCapabilitiesManagerImpl::GetRadioBandByCapabilities(
    const smart_objects::SmartObject& capabilities_status,
    const smart_objects::SmartObject& request_parameter) {
  mobile_apis::RadioBand::eType radio_band =
      static_cast<mobile_apis::RadioBand::eType>(request_parameter.asUInt());
  if (mobile_apis::RadioBand::XM == radio_band) {
    if (!capabilities_status.keyExists(strings::kSiriusxmRadioAvailable)) {
      LOG4CXX_DEBUG(logger_,
                    "Capability "
                        << strings::kSiriusxmRadioAvailable
                        << " is missed in RemoteControl capabilities");
      return std::make_pair(strings::kSiriusxmRadioAvailable,
                            capabilitiesStatus::missedParam);
    }
    if (!capabilities_status[strings::kSiriusxmRadioAvailable].asBool()) {
      LOG4CXX_DEBUG(logger_,
                    "Capability "
                        << strings::kSiriusxmRadioAvailable
                        << " is switched off in RemoteControl capabilities");
      return std::make_pair(strings::kSiriusxmRadioAvailable,
                            capabilitiesStatus::missedParam);
    }
  }
  return std::make_pair("", capabilitiesStatus::success);
}

const smart_objects::SmartObject& RCCapabilitiesManagerImpl::ControlData(
    const smart_objects::SmartObject& module_data,
    const std::string& module_type) {
  const auto& all_module_types = RCHelpers::GetModulesList();
  const auto& data_mapping = RCHelpers::GetModuleTypeToDataMapping();
  for (const auto& type : all_module_types) {
    if (type == module_type) {
      return module_data[data_mapping(type)];
    }
  }
  NOTREACHED();
  return module_data[0];
}

bool RCCapabilitiesManagerImpl::CheckReadOnlyParamsForAudio(
    const smart_objects::SmartObject& module_type_params) {
  if (module_type_params.keyExists(message_params::kEqualizerSettings)) {
    const auto& equalizer_settings =
        module_type_params[message_params::kEqualizerSettings];

    for (auto& so : *(equalizer_settings.asArray())) {
      if (so.keyExists(message_params::kChannelName)) {
        LOG4CXX_DEBUG(logger_,
                      "READ ONLY parameter. ChannelName = "
                          << so[message_params::kChannelName].asString());
        return true;
      }
    }
  }

  return false;
}

bool RCCapabilitiesManagerImpl::CheckReadOnlyParamsForLight(
    const smart_objects::SmartObject& module_type_params) {
  if (module_type_params.keyExists(message_params::kLightState)) {
    const auto& light_state = module_type_params[message_params::kLightState];

    for (auto& light_data : *(light_state.asArray())) {
      if (light_data.keyExists(message_params::kLightStatus)) {
        const mobile_apis::LightStatus::eType light_status =
            static_cast<mobile_apis::LightStatus::eType>(
                light_data[message_params::kLightStatus].asUInt());

        if (helpers::Compare<mobile_apis::LightStatus::eType,
                             helpers::EQ,
                             helpers::ONE>(light_status,
                                           mobile_apis::LightStatus::RAMP_UP,
                                           mobile_apis::LightStatus::RAMP_DOWN,
                                           mobile_apis::LightStatus::UNKNOWN,
                                           mobile_apis::LightStatus::INVALID)) {
          LOG4CXX_DEBUG(
              logger_,
              "READ ONLY parameter. Status = "
                  << light_data[message_params::kLightStatus].asInt());
          return true;
        }
      }
    }
  }

  return false;
}

bool RCCapabilitiesManagerImpl::AreReadOnlyParamsPresent(
    const smart_objects::SmartObject& module_data,
    const std::string& module_type,
    ModuleCapability& module_data_capabilities) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& module_type_params =
      ControlData(module_data, module_type);

  if (enums_value::kAudio == module_type) {
    return CheckReadOnlyParamsForAudio(module_type_params);
  }

  if (enums_value::kLight == module_type) {
    const bool result = CheckReadOnlyParamsForLight(module_type_params);

    if (result) {
      module_data_capabilities =
          std::make_pair(module_type, capabilitiesStatus::readOnly);
    }
    return result;
  }

  const std::vector<std::string> ro_params =
      RCHelpers::GetModuleReadOnlyParams(module_type);
  auto it = module_type_params.map_begin();

  for (; it != module_type_params.map_end(); ++it) {
    if (helpers::in_range(ro_params, it->first)) {
      return true;
    }
  }
  return false;
}

bool RCCapabilitiesManagerImpl::AreAllParamsReadOnly(
    const smart_objects::SmartObject& module_data,
    const std::string& module_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& module_type_params =
      ControlData(module_data, module_type);
  auto it = module_type_params.map_begin();
  std::vector<std::string> ro_params =
      RCHelpers::GetModuleReadOnlyParams(module_type);
  for (; it != module_type_params.map_end(); ++it) {
    if (!helpers::in_range(ro_params, it->first)) {
      return false;
    }
  }

  LOG4CXX_DEBUG(logger_, "All params are ReadOnly");
  return true;
}

}  // namespace rc_rpc_plugin
