/*
 Copyright (c) 2017, Ford Motor Company
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

#include "remote_control/commands/set_interior_vehicle_data_request.h"
#include <algorithm>
#include "remote_control/rc_module_constants.h"
#include "remote_control/message_helper.h"
#include "functional_module/function_ids.h"
#include "json/json.h"
#include "utils/helpers.h"

namespace remote_control {

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
  }
  return module_ro_params;
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

  // radio
  mapping["band"] = "radioBandAvailable";
  mapping["frequencyInteger"] = "radioFrequencyAvailable";
  mapping["frequencyFraction"] = "radioFrequencyAvailable";
  mapping["rdsData"] = "rdsDataAvailable";
  mapping["availableHDs"] = "availableHDsAvailable";
  mapping["hdChannel"] = "availableHDsAvailable";
  mapping["signalStrength"] = "signalStrengthAvailable";
  mapping["radioEnable"] = "radioEnableAvailable";
  mapping["state"] = "stateAvailable";

  return mapping;
}
}  // namespace

CREATE_LOGGERPTR_GLOBAL(logger_, "SetInteriorVehicleDataRequest")

SetInteriorVehicleDataRequest::SetInteriorVehicleDataRequest(
    const application_manager::MessagePtr& message,
    RemotePluginInterface& rc_module)
    : BaseCommandRequest(message, rc_module) {}

SetInteriorVehicleDataRequest::~SetInteriorVehicleDataRequest() {}

bool CheckControlDataByCapabilities(
    const smart_objects::SmartObject& module_caps,
    const Json::Value& control_data) {
  std::map<std::string, std::string> mapping =
      GetModuleDataToCapabilitiesMapping();
  Json::Value::Members control_data_keys = control_data.getMemberNames();

  Json::Value::Members::const_iterator it = control_data_keys.begin();
  for (; it != control_data_keys.end(); ++it) {
    const std::string& caps_key = mapping[*it];
    DCHECK_OR_RETURN(module_caps.keyExists(caps_key), false);
    if (!module_caps[caps_key].asBool()) {
      return false;
    }
  }
  return true;
}

bool CheckIfModuleDataExistInCapabilities(
    const smart_objects::SmartObject& rc_capabilities,
    const Json::Value& module_data) {
  if (rc_capabilities.keyExists(strings::kradioControlCapabilities)) {
    const smart_objects::SmartObject& radio_caps =
        rc_capabilities[strings::kradioControlCapabilities];
    return CheckControlDataByCapabilities(
        radio_caps, module_data[strings::kRadioControlData]);
  }
  if (rc_capabilities.keyExists(strings::kclimateControlCapabilities)) {
    const smart_objects::SmartObject& climate_caps =
        rc_capabilities[strings::kclimateControlCapabilities];
    return CheckControlDataByCapabilities(
        climate_caps, module_data[strings::kClimateControlData]);
  }
  DCHECK(false && "Module Data does not contains control data");
  return true;
}

void SetInteriorVehicleDataRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value request_params =
      MessageHelper::StringToValue(message_->json_message());
  const Json::Value module_data = request_params[kModuleData];
  const std::string module_type = module_data[kModuleType].asString();
  bool module_type_and_data_match = true;

  if (enums_value::kRadio == module_type) {
    module_type_and_data_match = !IsMember(module_data, kClimateControlData);
  }

  if (enums_value::kClimate == module_type) {
    module_type_and_data_match = !IsMember(module_data, kRadioControlData);
  }

  if (module_type_and_data_match) {
    const smart_objects::SmartObject* capabilities =
        service()->GetRCCapabilities();
    if (capabilities &&
        !CheckIfModuleDataExistInCapabilities(*capabilities, module_data)) {
      LOG4CXX_WARN(logger_, "Accessing not supported module data");
      SendResponse(false,
                   result_codes::kUnsupportedResource,
                   "Accessing not supported module data");
      return;
    }
    if (AreAllParamsReadOnly(request_params)) {
      LOG4CXX_WARN(logger_, "All request params in module type are READ ONLY!");
      SendResponse(false,
                   result_codes::kReadOnly,
                   "All request params in module type are READ ONLY!");
      return;
    }
    if (AreReadOnlyParamsPresent(request_params)) {
      LOG4CXX_DEBUG(logger_, "Request module type has READ ONLY parameters");
      LOG4CXX_DEBUG(logger_, "Cutting-off READ ONLY parameters... ");
      CutOffReadOnlyParams(request_params);
    }
    application_manager::MessagePtr hmi_request = CreateHmiRequest(
        functional_modules::hmi_api::set_interior_vehicle_data, request_params);
    service()->RemoveHMIFakeParameters(hmi_request);
    SendMessageToHMI(hmi_request);
  } else {
    LOG4CXX_WARN(logger_, "Request module type & data mismatch!");
    SendResponse(false,
                 result_codes::kInvalidData,
                 "Request module type & data mismatch!");
  }
}

AcquireResult::eType SetInteriorVehicleDataRequest::AcquireResource(
    const Json::Value& message) {
  return rc_module_.resource_allocation_manager().AcquireResource(
      ModuleType(message), app()->app_id());
}

bool SetInteriorVehicleDataRequest::IsResourceFree(
    const std::string& module_type) const {
  return rc_module_.resource_allocation_manager().IsResourceFree(module_type);
}

void SetInteriorVehicleDataRequest::SetResourceState(
    const Json::Value& message, const ResourceState::eType state) {
  const std::string& module_type = ModuleType(message);
  const uint32_t app_id = app()->app_id();

  ResourceAllocationManager& allocation_manager =
      rc_module_.resource_allocation_manager();

  allocation_manager.SetResourceState(module_type, app_id, state);
}

bool SetInteriorVehicleDataRequest::AreReadOnlyParamsPresent(
    const Json::Value& request_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<std::string> module_type_params = ControlData(request_params);
  std::vector<std::string>::iterator it = module_type_params.begin();
  std::vector<std::string> ro_params =
      GetModuleReadOnlyParams(ModuleType(request_params));
  for (; it != module_type_params.end(); ++it) {
    if (helpers::in_range(ro_params, *it)) {
      return true;
    }
  }
  return false;
}

void SetInteriorVehicleDataRequest::CutOffReadOnlyParams(
    Json::Value& request_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<std::string> module_type_params = ControlData(request_params);
  std::vector<std::string>::iterator it = module_type_params.begin();
  const std::string module_type = ModuleType(request_params);
  std::vector<std::string> ro_params = GetModuleReadOnlyParams(module_type);
  for (; it != module_type_params.end(); ++it) {
    if (helpers::in_range(ro_params, *it)) {
      if (enums_value::kClimate == module_type) {
        request_params[message_params::kModuleData]
                      [message_params::kClimateControlData].removeMember(*it);
        LOG4CXX_DEBUG(logger_, "Cutting-off READ ONLY parameter: " << *it);
      } else if (enums_value::kRadio == module_type) {
        request_params[message_params::kModuleData]
                      [message_params::kRadioControlData].removeMember(*it);
        LOG4CXX_DEBUG(logger_, "Cutting-off READ ONLY parameter: " << *it);
      }
    }
  }
}

bool SetInteriorVehicleDataRequest::AreAllParamsReadOnly(
    const Json::Value& request_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<std::string> module_type_params = ControlData(request_params);
  std::vector<std::string>::iterator it = module_type_params.begin();
  std::vector<std::string> ro_params =
      GetModuleReadOnlyParams(ModuleType(request_params));
  for (; it != module_type_params.end(); ++it) {
    if (!helpers::in_range(ro_params, *it)) {
      return false;
    }
  }
  return true;
}

void SetInteriorVehicleDataRequest::OnEvent(
    const rc_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(
      (functional_modules::hmi_api::set_interior_vehicle_data == event.id()));

  application_manager::Message& hmi_response = *(event.event_message());
  if (application_manager::MessageValidationResult::SUCCESS !=
      service()->ValidateMessageBySchema(hmi_response)) {
    SendResponse(false, result_codes::kGenericError, "");
    return;
  }

  LOG4CXX_DEBUG(logger_, "HMI response is valid");
  const Json::Value value =
      MessageHelper::StringToValue(hmi_response.json_message());

  std::string result_code;
  std::string info;

  const bool is_response_successful = ParseResultCode(value, result_code, info);

  if (is_response_successful) {
    response_params_[kModuleData] = value[kResult][kModuleData];
  }

  SendResponse(is_response_successful, result_code.c_str(), info);
}

std::string SetInteriorVehicleDataRequest::ModuleType(
    const Json::Value& message) {
  const Json::Value& module_data =
      message.get(message_params::kModuleData, Json::Value(Json::objectValue));
  return module_data.get(message_params::kModuleType, "").asString();
}

std::vector<std::string> SetInteriorVehicleDataRequest::ControlData(
    const Json::Value& message) {
  Json::Value data =
      message.get(message_params::kModuleData, Json::Value(Json::objectValue));
  const char* name_control_data;
  std::string module = ModuleType(message);
  if (module == enums_value::kRadio) {
    name_control_data = message_params::kRadioControlData;
  }
  if (module == enums_value::kClimate) {
    name_control_data = message_params::kClimateControlData;
  }
  Json::Value params =
      data.get(name_control_data, Json::Value(Json::objectValue));
  return params.getMemberNames();
}

}  // namespace commands

}  // namespace remote_control
