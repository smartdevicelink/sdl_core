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

const std::string
RCCapabilitiesManagerImpl::GetDefaultModuleIdFromCapabilitiesStructure(
    const smart_objects::SmartObject& control_capabilities,
    const std::string& module_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (control_capabilities.keyExists(message_params::kModuleInfo)) {
    // moduleId - mandatory param for ModuleInfo structure
    const auto module_id = control_capabilities[message_params::kModuleInfo]
                                               [message_params::kModuleId]
                                                   .asString();
    LOG4CXX_WARN(logger_,
                 "Use default moduleId from hmi capabilities: "
                     << module_id
                     << " for requested moduleType: " << module_type);
    return module_id;
  }
  LOG4CXX_WARN(logger_,
               "There are no moduleInfo in hmi capabilities for requested "
               "moduleType "
                   << module_type);
  return "";
}

const std::string
RCCapabilitiesManagerImpl::GetDefaultModuleIdFromCapabilitiesArray(
    const smart_objects::SmartObject& control_capabilities,
    const std::string& module_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  for (auto& cap_item : *(control_capabilities.asArray())) {
    if (cap_item.keyExists(message_params::kModuleInfo)) {
      // moduleId - mandatory param for ModuleInfo structure
      const auto module_id =
          cap_item[message_params::kModuleInfo][message_params::kModuleId]
              .asString();
      LOG4CXX_WARN(logger_,
                   "Use default moduleId from hmi capabilities: "
                       << module_id
                       << " for requested moduleType: " << module_type);
      return module_id;
    }
  }
  LOG4CXX_WARN(logger_,
               "There are no moduleInfo in hmi capabilities for requested "
               "moduleType "
                   << module_type);
  return "";
}

const std::string RCCapabilitiesManagerImpl::GetDefaultModuleIdFromCapabilities(
    const std::string& module_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  auto rc_capabilities_ptr = hmi_capabilities_.rc_capability();
  if (!rc_capabilities_ptr) {
    LOG4CXX_WARN(logger_, "RC capability is not initialized");
    return std::string();
  }

  auto rc_capabilities = *rc_capabilities_ptr;
  const auto& mapping = RCHelpers::GetModuleTypeToCapabilitiesMapping();
  if (!rc_capabilities.keyExists(mapping(module_type))) {
    LOG4CXX_WARN(
        logger_,
        "There is no RC capability for requested module_type " << module_type);
    return "";
  }
  if (enums_value::kHmiSettings == module_type ||
      enums_value::kLight == module_type) {
    return GetDefaultModuleIdFromCapabilitiesStructure(
        rc_capabilities[mapping(module_type)], module_type);
  }
  return GetDefaultModuleIdFromCapabilitiesArray(
      rc_capabilities[mapping(module_type)], module_type);
}

const bool RCCapabilitiesManagerImpl::CheckModuleIdWithCapabilitiesStructure(
    const smart_objects::SmartObject& control_capabilities,
    const std::string& module_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (control_capabilities.keyExists(message_params::kModuleInfo) &&
      (module_id == control_capabilities[message_params::kModuleInfo]
                                        [message_params::kModuleId]
                                            .asString())) {
    return true;
  }
  LOG4CXX_WARN(logger_,
               "There are no moduleInfo in hmi capabilities for requested "
               "moduleId "
                   << module_id);
  return false;
}

const bool RCCapabilitiesManagerImpl::CheckModuleIdWithCapabilitiesArrays(
    const smart_objects::SmartObject& control_capabilities,
    const std::string& module_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  for (auto& cap_item : *(control_capabilities.asArray())) {
    if (cap_item.keyExists(message_params::kModuleInfo) &&
        (module_id ==
         cap_item[message_params::kModuleInfo][message_params::kModuleId]
             .asString())) {
      return true;
    }
  }
  LOG4CXX_WARN(logger_,
               "There are no moduleInfo in hmi capabilities for requested "
               "moduleId "
                   << module_id);
  return false;
}

const bool RCCapabilitiesManagerImpl::CheckModuleIdWithCapabilities(
    const smart_objects::SmartObject& rc_capabilities,
    const ModuleUid& module) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (module.second.empty()) {
    return true;
  }
  const auto& mapping = RCHelpers::GetModuleTypeToCapabilitiesMapping();
  if (enums_value::kHmiSettings == module.first ||
      enums_value::kLight == module.first) {
    return CheckModuleIdWithCapabilitiesStructure(
        rc_capabilities[mapping(module.first)], module.second);
  }
  return CheckModuleIdWithCapabilitiesArrays(
      rc_capabilities[mapping(module.first)], module.second);
}

bool RCCapabilitiesManagerImpl::CheckIfModuleExistsInCapabilities(
    const ModuleUid& module) const {
  LOG4CXX_AUTO_TRACE(logger_);

  auto rc_capabilities_ptr = hmi_capabilities_.rc_capability();
  if (!rc_capabilities_ptr) {
    LOG4CXX_WARN(logger_, "RC capability is not initialized");
    return false;
  }

  auto rc_capabilities = *rc_capabilities_ptr;
  const auto& mapping = RCHelpers::GetModuleTypeToCapabilitiesMapping();
  const auto& resource_list = GetResources();
  bool is_module_type_valid = false;
  for (const auto& resource : resource_list) {
    if (resource.first == module.first) {
      if (rc_capabilities.keyExists(mapping(module.first)) &&
          CheckModuleIdWithCapabilities(rc_capabilities, module)) {
        is_module_type_valid = true;
        break;
      }
    }
  }
  return is_module_type_valid;
}

const std::vector<std::string> RCCapabilitiesManagerImpl::GetCapabilitiesList()
    const {
  using namespace enums_value;
  return {strings::kclimateControlCapabilities,
          strings::kradioControlCapabilities,
          strings::kseatControlCapabilities,
          strings::kaudioControlCapabilities,
          strings::klightControlCapabilities,
          strings::khmiSettingsControlCapabilities};
}

const std::function<std::string(const std::string& control_cap)>
RCCapabilitiesManagerImpl::GetCapabilitiesToModuleTypeMapping() const {
  auto mapping_lambda = [](const std::string& control_cap) -> std::string {
    static std::map<std::string, std::string> mapping = {
        {strings::kclimateControlCapabilities, enums_value::kClimate},
        {strings::kradioControlCapabilities, enums_value::kRadio},
        {strings::kseatControlCapabilities, enums_value::kSeat},
        {strings::kaudioControlCapabilities, enums_value::kAudio},
        {strings::klightControlCapabilities, enums_value::kLight},
        {strings::khmiSettingsControlCapabilities, enums_value::kHmiSettings}};
    auto it = mapping.find(control_cap);
    if (mapping.end() == it) {
      LOG4CXX_ERROR(logger_, "Unknown control capability " << control_cap);
      return std::string();
    }
    return it->second;
  };

  return mapping_lambda;
}

void RCCapabilitiesManagerImpl::GetResourcesFromCapabilitiesStructure(
    const smart_objects::SmartObject& control_capabilities,
    const std::string& capability_key,
    std::vector<ModuleUid>& out_resources) const {
  const auto& mapping = GetCapabilitiesToModuleTypeMapping();
  if (control_capabilities.keyExists(message_params::kModuleInfo)) {
    std::string module_id = control_capabilities[message_params::kModuleInfo]
                                                [message_params::kModuleId]
                                                    .asString();
    out_resources.push_back(std::make_pair(mapping(capability_key), module_id));
  } else {
    LOG4CXX_WARN(logger_, "There are no moduleId in " << capability_key);
    out_resources.push_back(std::make_pair(mapping(capability_key), ""));
  }
}

void RCCapabilitiesManagerImpl::GetResourcesFromCapabilitiesArray(
    const smart_objects::SmartObject& control_capabilities,
    const std::string& capability_key,
    std::vector<ModuleUid>& out_resources) const {
  const auto& mapping = GetCapabilitiesToModuleTypeMapping();
  for (auto cap_item : *(control_capabilities.asArray())) {
    if (cap_item.keyExists(message_params::kModuleInfo)) {
      std::string module_id =
          cap_item[message_params::kModuleInfo][message_params::kModuleId]
              .asString();
      out_resources.push_back(
          std::make_pair(mapping(capability_key), module_id));
    } else {
      LOG4CXX_WARN(logger_,
                   "There are no moduleId for item from " << capability_key);
      out_resources.push_back(std::make_pair(mapping(capability_key), ""));
    }
  }
}

const std::vector<ModuleUid> RCCapabilitiesManagerImpl::GetResources() const {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<ModuleUid> resources;
  auto rc_capabilities_ptr = hmi_capabilities_.rc_capability();
  if (!rc_capabilities_ptr) {
    LOG4CXX_WARN(logger_, "RC capability is not initialized");
    return resources;
  }

  auto rc_capabilities = *rc_capabilities_ptr;
  const auto& control_caps_list = GetCapabilitiesList();
  for (const auto& capability_key : control_caps_list) {
    if (rc_capabilities.keyExists(capability_key)) {
      if (strings::khmiSettingsControlCapabilities == capability_key ||
          strings::klightControlCapabilities == capability_key) {
        GetResourcesFromCapabilitiesStructure(
            rc_capabilities[capability_key], capability_key, resources);
      } else {
        GetResourcesFromCapabilitiesArray(
            rc_capabilities[capability_key], capability_key, resources);
      }
    }
  }
  return resources;
}

const std::string RCCapabilitiesManagerImpl::GetModuleIdForSeatLocation(
    const mobile_apis::SupportedSeat::eType id) const {
  LOG4CXX_AUTO_TRACE(logger_);

  auto rc_capabilities_ptr = hmi_capabilities_.rc_capability();
  if (!rc_capabilities_ptr) {
    LOG4CXX_WARN(logger_, "RC capability is not initialized.");
    return std::string();
  }

  auto rc_capabilities = *rc_capabilities_ptr;
  const auto seat_capabilities =
      rc_capabilities[strings::kseatControlCapabilities];
  if (seat_capabilities.length() > 0) {
    if (mobile_apis::SupportedSeat::DRIVER == id) {
      return seat_capabilities[0][message_params::kModuleInfo]
                              [message_params::kModuleId]
                                  .asString();
    }
    if ((seat_capabilities.length() > 1) &&
        mobile_apis::SupportedSeat::FRONT_PASSENGER == id) {
      return seat_capabilities[1][message_params::kModuleInfo]
                              [message_params::kModuleId]
                                  .asString();
    }
  }
  LOG4CXX_DEBUG(logger_, "There are no capabitities for requested id: " << id);
  return "";
}

bool RCCapabilitiesManagerImpl::CheckIfButtonExistInRCCaps(
    const mobile_apis::ButtonName::eType button) const {
  auto rc_capabilities_ptr = hmi_capabilities_.rc_capability();
  if (!rc_capabilities_ptr) {
    LOG4CXX_WARN(logger_, "RC capability is not initialized");
    return false;
  }

  auto rc_capabilities = *rc_capabilities_ptr;
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

smart_objects::SmartObject
RCCapabilitiesManagerImpl::GetCapabilitiesByModuleIdFromArray(
    const smart_objects::SmartObject& module_data_capabilities,
    const std::string& module_id) const {
  for (auto& cap_item : *(module_data_capabilities.asArray())) {
    std::string current_id =
        cap_item[message_params::kModuleInfo][message_params::kModuleId]
            .asString();
    if (module_id == current_id) {
      return cap_item;
    }
  }
  LOG4CXX_WARN(logger_,
               "Capabilities for moduleId " << module_id
                                            << " do not exist in capabilities");
  return smart_objects::SmartObject(smart_objects::SmartType_Null);
}

bool RCCapabilitiesManagerImpl::CheckButtonName(
    const std::string& module_type, const std::string& button_name) const {
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
RCCapabilitiesManagerImpl::GetLightCapabilitiesMapping() const {
  std::map<std::string, std::string> mapping = {
      {message_params::kId, strings::kName},
      {message_params::kLightStatus, strings::kStatusAvailable},
      {message_params::kLightDensity, strings::kDensityAvailable},
      {message_params::kLightColor, strings::kRGBColorSpaceAvailable}};
  return mapping;
}

const std::map<std::string, std::string>
RCCapabilitiesManagerImpl::GetModuleDataToCapabilitiesMapping() const {
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

ModuleTypeCapability RCCapabilitiesManagerImpl::GetModuleDataCapabilities(
    const smart_objects::SmartObject& module_data,
    const std::string& module_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  auto rc_capabilities_ptr = hmi_capabilities_.rc_capability();
  if (!rc_capabilities_ptr) {
    LOG4CXX_WARN(logger_, "RC capability is not initialized");
    return {std::string(), capabilitiesStatus::kInvalidStatus};
  }

  auto rc_capabilities = *rc_capabilities_ptr;

  const auto& all_module_types = RCHelpers::GetModuleTypesList();
  const auto& get_module_data_key = RCHelpers::GetModuleTypeToDataMapping();
  const auto& get_capabilities_key =
      RCHelpers::GetModuleTypeToCapabilitiesMapping();
  ModuleTypeCapability module_data_capabilities =
      std::make_pair("", capabilitiesStatus::kMissedParam);

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
        module_data_capabilities = GetControlDataCapabilities(
            GetCapabilitiesByModuleIdFromArray(caps, module_id),
            module_data[module_data_key]);
      }
    }
  }

  return module_data_capabilities;
}

ModuleTypeCapability RCCapabilitiesManagerImpl::GetControlDataCapabilities(
    const smart_objects::SmartObject& capabilities,
    const smart_objects::SmartObject& control_data) const {
  LOG4CXX_AUTO_TRACE(logger_);
  std::map<std::string, std::string> mapping =
      GetModuleDataToCapabilitiesMapping();

  for (auto it = control_data.map_begin(); it != control_data.map_end(); ++it) {
    const std::string& request_parameter = it->first;
    if (message_params::kId == request_parameter) {
      continue;
    }
    if (message_params::kLightState == request_parameter) {
      ModuleTypeCapability light_capability =
          std::make_pair("", capabilitiesStatus::kSuccess);

      for (auto& light_data : *(control_data[request_parameter].asArray())) {
        light_capability = GetLightNameCapabilities(
            capabilities[strings::kSupportedLights], light_data);

        if (capabilitiesStatus::kSuccess != light_capability.second) {
          return light_capability;
        }
      }

      return light_capability;
    }
    if (message_params::kBand == request_parameter) {
      ModuleTypeCapability radio_capability = GetRadioBandByCapabilities(
          capabilities, control_data[request_parameter]);
      if (capabilitiesStatus::kSuccess != radio_capability.second) {
        return radio_capability;
      }
    }

    const capabilitiesStatus status_item_capability =
        GetItemCapability(capabilities,
                          mapping,
                          request_parameter,
                          mobile_apis::Result::UNSUPPORTED_RESOURCE);

    if (capabilitiesStatus::kSuccess != status_item_capability) {
      return std::make_pair("", status_item_capability);
    }
  }

  return std::make_pair("", capabilitiesStatus::kSuccess);
}

capabilitiesStatus RCCapabilitiesManagerImpl::GetItemCapability(
    const smart_objects::SmartObject& capabilities,
    const std::map<std::string, std::string>& mapping,
    const std::string& request_parameter,
    const mobile_apis::Result::eType& switched_off_result) const {
  const auto it = mapping.find(request_parameter);

  if (it == mapping.end()) {
    LOG4CXX_DEBUG(
        logger_,
        "Parameter " << request_parameter << " doesn't exist in capabilities.");
    return capabilitiesStatus::kMissedParam;
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
    return capabilitiesStatus::kMissedParam;
  }

  if (!capabilities[caps_key].asBool()) {
    LOG4CXX_DEBUG(logger_,
                  "Capability "
                      << caps_key
                      << " is switched off in RemoteControl capabilities");
    capabilitiesStatus status = capabilitiesStatus::kMissedParam;
    if (mobile_apis::Result::READ_ONLY == switched_off_result) {
      status = capabilitiesStatus::kReadOnly;
    }
    return status;
  }

  return capabilitiesStatus::kSuccess;
}

ModuleTypeCapability RCCapabilitiesManagerImpl::GetLightDataCapabilities(
    const smart_objects::SmartObject& capabilities,
    const smart_objects::SmartObject& control_data) const {
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

    if (capabilitiesStatus::kSuccess != status_item_capability) {
      return std::make_pair(message_params::kLightState,
                            status_item_capability);
    }
  }

  return std::make_pair("", capabilitiesStatus::kSuccess);
}

ModuleTypeCapability RCCapabilitiesManagerImpl::GetLightNameCapabilities(
    const smart_objects::SmartObject& capabilities_status,
    const smart_objects::SmartObject& light_data) const {
  LOG4CXX_AUTO_TRACE(logger_);
  for (auto& so : *(capabilities_status.asArray())) {
    const int64_t current_id = so[message_params::kName].asInt();
    if (current_id == light_data[message_params::kId].asInt()) {
      return GetLightDataCapabilities(so, light_data);
    }
  }
  LOG4CXX_DEBUG(logger_, "There is no such light name in capabilities");
  return std::make_pair(message_params::kLightState,
                        capabilitiesStatus::kMissedLightName);
}

ModuleTypeCapability RCCapabilitiesManagerImpl::GetRadioBandByCapabilities(
    const smart_objects::SmartObject& capabilities_status,
    const smart_objects::SmartObject& request_parameter) const {
  mobile_apis::RadioBand::eType radio_band =
      static_cast<mobile_apis::RadioBand::eType>(request_parameter.asUInt());
  if (mobile_apis::RadioBand::XM == radio_band) {
    if (!capabilities_status.keyExists(strings::kSiriusxmRadioAvailable)) {
      LOG4CXX_DEBUG(logger_,
                    "Capability "
                        << strings::kSiriusxmRadioAvailable
                        << " is missed in RemoteControl capabilities");
      return std::make_pair(strings::kSiriusxmRadioAvailable,
                            capabilitiesStatus::kMissedParam);
    }
    if (!capabilities_status[strings::kSiriusxmRadioAvailable].asBool()) {
      LOG4CXX_DEBUG(logger_,
                    "Capability "
                        << strings::kSiriusxmRadioAvailable
                        << " is switched off in RemoteControl capabilities");
      return std::make_pair(strings::kSiriusxmRadioAvailable,
                            capabilitiesStatus::kMissedParam);
    }
  }
  return std::make_pair("", capabilitiesStatus::kSuccess);
}

const smart_objects::SmartObject& RCCapabilitiesManagerImpl::ControlDataForType(
    const smart_objects::SmartObject& module_data,
    const std::string& module_type) const {
  const auto& all_module_types = RCHelpers::GetModuleTypesList();
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
    const smart_objects::SmartObject& module_type_params) const {
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
    const smart_objects::SmartObject& module_type_params) const {
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
    ModuleTypeCapability& module_data_capabilities) const {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& module_type_params =
      ControlDataForType(module_data, module_type);

  if (enums_value::kAudio == module_type) {
    return CheckReadOnlyParamsForAudio(module_type_params);
  }

  if (enums_value::kLight == module_type) {
    const bool result = CheckReadOnlyParamsForLight(module_type_params);

    if (result) {
      module_data_capabilities =
          std::make_pair(module_type, capabilitiesStatus::kReadOnly);
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
    const std::string& module_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& module_type_params =
      ControlDataForType(module_data, module_type);
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

bool RCCapabilitiesManagerImpl::IsSeatLocationCapabilityProvided() const {
  LOG4CXX_AUTO_TRACE(logger_);
  auto seat_location_capability = hmi_capabilities_.seat_location_capability();
  if (!seat_location_capability || seat_location_capability->empty()) {
    LOG4CXX_DEBUG(logger_, "Seat Location capability is not provided by HMI");
    return false;
  }

  if (seat_location_capability->keyExists(strings::kRows) &&
      seat_location_capability->keyExists(strings::kCols) &&
      seat_location_capability->keyExists(strings::kLevels) &&
      seat_location_capability->keyExists(strings::kSeats)) {
    const auto* seats = (*seat_location_capability)[strings::kSeats].asArray();
    if (!seats->empty()) {
      return true;
    }
  }

  LOG4CXX_DEBUG(
      logger_,
      "Seat Location capability doesn't contain all necessary parameters");
  return false;
}

const Grid
RCCapabilitiesManagerImpl::GetDriverLocationFromSeatLocationCapability() const {
  LOG4CXX_AUTO_TRACE(logger_);
  Grid grid;
  if (IsSeatLocationCapabilityProvided()) {
    auto seat_location_capability =
        hmi_capabilities_.seat_location_capability();
    const auto* seats = (*seat_location_capability)[strings::kSeats].asArray();
    const auto& driver_seat = (*seats)[0];
    if (driver_seat.keyExists(strings::kGrid)) {
      const auto& driver_location = driver_seat[strings::kGrid];
      grid = Grid(driver_location[strings::kCol].asInt(),
                  driver_location[strings::kRow].asInt(),
                  driver_location[strings::kLevel].asInt(),
                  driver_location[strings::kColspan].asInt(),
                  driver_location[strings::kRowspan].asInt(),
                  driver_location[strings::kLevelspan].asInt());
    } else {
      LOG4CXX_DEBUG(logger_, "Driver's location doesn't provided");
    }
  }
  return grid;
}

Grid RCCapabilitiesManagerImpl::GetWholeVehicleArea() const {
  auto seat_location_capability =
      *(hmi_capabilities_.seat_location_capability());
  int32_t colspan = seat_location_capability.keyExists(strings::kCols)
                        ? seat_location_capability[strings::kCols].asInt()
                        : 0;

  int32_t rowspan = seat_location_capability.keyExists(strings::kRows)
                        ? seat_location_capability[strings::kRows].asInt()
                        : 0;

  int32_t levelspan = seat_location_capability.keyExists(strings::kLevels)
                          ? seat_location_capability[strings::kLevels].asInt()
                          : 0;
  return Grid(0, 0, 0, colspan, rowspan, levelspan);
}

Grid RCCapabilitiesManagerImpl::GetModuleLocationFromControlCapability(
    const smart_objects::SmartObject& control_capabilities) const {
  if (control_capabilities[message_params::kModuleInfo].keyExists(
          strings::kLocation)) {
    const auto& location =
        control_capabilities[message_params::kModuleInfo][strings::kLocation];

    return Grid(location[strings::kCol].asInt(),
                location[strings::kRow].asInt(),
                location[strings::kLevel].asInt(),
                location[strings::kColspan].asInt(),
                location[strings::kRowspan].asInt(),
                location[strings::kLevelspan].asInt());
  }
  return GetWholeVehicleArea();
}

Grid RCCapabilitiesManagerImpl::GetModuleServiceAreaFromControlCapability(
    const smart_objects::SmartObject& control_capabilities) const {
  if (control_capabilities.keyExists(message_params::kModuleInfo)) {
    if (control_capabilities[message_params::kModuleInfo].keyExists(
            strings::kServiceArea)) {
      const auto& serviceArea =
          control_capabilities[message_params::kModuleInfo]
                              [strings::kServiceArea];

      return Grid(serviceArea[strings::kCol].asInt(),
                  serviceArea[strings::kRow].asInt(),
                  serviceArea[strings::kLevel].asInt(),
                  serviceArea[strings::kColspan].asInt(),
                  serviceArea[strings::kRowspan].asInt(),
                  serviceArea[strings::kLevelspan].asInt());
    }
  }
  return GetModuleLocationFromControlCapability(control_capabilities);
}

Grid RCCapabilitiesManagerImpl::GetModuleServiceArea(
    const ModuleUid& module) const {
  auto rc_capabilities_ptr = hmi_capabilities_.rc_capability();
  if (!rc_capabilities_ptr) {
    LOG4CXX_WARN(logger_, "RC capability is not initialized");
    return Grid();
  }

  auto rc_capabilities = *rc_capabilities_ptr;
  const auto& mapping = RCHelpers::GetModuleTypeToCapabilitiesMapping();
  const auto& module_type = module.first;
  const auto& capabilities_key = mapping(module_type);
  if (!rc_capabilities.keyExists(capabilities_key)) {
    LOG4CXX_DEBUG(logger_, module_type << "control capabilities not present");
    return Grid();
  }
  const auto& caps = rc_capabilities[capabilities_key];

  if (strings::khmiSettingsControlCapabilities == capabilities_key ||
      strings::klightControlCapabilities == capabilities_key) {
    return GetModuleServiceAreaFromControlCapability(caps);
  } else {
    const auto& capability_item =
        GetCapabilitiesByModuleIdFromArray(caps, module.second);
    return GetModuleServiceAreaFromControlCapability(capability_item);
  }
}

bool RCCapabilitiesManagerImpl::IsMultipleAccessAllowedInControlCaps(
    const smart_objects::SmartObject& control_capabilities) const {
  if (control_capabilities.keyExists(message_params::kModuleInfo) &&
      control_capabilities[message_params::kModuleInfo].keyExists(
          strings::kAllowMultipleAccess)) {
    return control_capabilities[message_params::kModuleInfo]
                               [strings::kAllowMultipleAccess]
                                   .asBool();
  }
  return true;
}

bool RCCapabilitiesManagerImpl::IsMultipleAccessAllowed(
    const ModuleUid& module) const {
  auto rc_capabilities_ptr = hmi_capabilities_.rc_capability();
  if (!rc_capabilities_ptr) {
    LOG4CXX_ERROR(logger_, "RC capability is not initialized");
    return false;
  }

  auto rc_capabilities = *rc_capabilities_ptr;
  const auto& mapping = RCHelpers::GetModuleTypeToCapabilitiesMapping();
  const auto& module_type = module.first;
  const auto& capabilities_key = mapping(module_type);
  if (!rc_capabilities.keyExists(capabilities_key)) {
    LOG4CXX_DEBUG(logger_, module_type << "control capabilities not present");
    return false;
  }
  const auto& caps = rc_capabilities[capabilities_key];

  if (strings::khmiSettingsControlCapabilities == capabilities_key ||
      strings::klightControlCapabilities == capabilities_key) {
    return IsMultipleAccessAllowedInControlCaps(caps);
  } else {
    return IsMultipleAccessAllowedInControlCaps(
        GetCapabilitiesByModuleIdFromArray(caps, module.second));
  }
}

}  // namespace rc_rpc_plugin
