/*
 Copyright (c) 2013, Ford Motor Company
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

#include "can_cooperation/validators/struct_validators/climate_control_data_validator.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace validators {

CREATE_LOGGERPTR_GLOBAL(logger_, "ClimateControlDataValidator")

using namespace message_params;

ClimateControlDataValidator::ClimateControlDataValidator() {
  // name="fanSpeed"
  fan_speed_[ValidationParams::TYPE] = ValueType::INT;
  fan_speed_[ValidationParams::MIN_VALUE] = 0;
  fan_speed_[ValidationParams::MAX_VALUE] = 100;
  fan_speed_[ValidationParams::ARRAY] = 0;
  fan_speed_[ValidationParams::MANDATORY] = 0;

  // name="currentTemp"
  current_temp_[ValidationParams::TYPE] = ValueType::INT;
  current_temp_[ValidationParams::MIN_VALUE] = 0;
  current_temp_[ValidationParams::MAX_VALUE] = 100;
  current_temp_[ValidationParams::ARRAY] = 0;
  current_temp_[ValidationParams::MANDATORY] = 0;

  // name="desiredTemp"
  desired_temp_[ValidationParams::TYPE] = ValueType::INT;
  desired_temp_[ValidationParams::MIN_VALUE] = 0;
  desired_temp_[ValidationParams::MAX_VALUE] = 100;
  desired_temp_[ValidationParams::ARRAY] = 0;
  desired_temp_[ValidationParams::MANDATORY] = 0;

  // name="temperatureUnit"
  temperature_unit_[ValidationParams::TYPE] = ValueType::ENUM;
  temperature_unit_[ValidationParams::ENUM_TYPE] = EnumType::TEMPERATURE_UNIT;
  temperature_unit_[ValidationParams::ARRAY] = 0;
  temperature_unit_[ValidationParams::MANDATORY] = 0;

  // name="acEnable"
  ac_enable_[ValidationParams::TYPE] = ValueType::BOOL;
  ac_enable_[ValidationParams::ARRAY] = 0;
  ac_enable_[ValidationParams::MANDATORY] = 0;

  // name="circulateAirEnable"
  circulate_air_enable_[ValidationParams::TYPE] = ValueType::BOOL;
  circulate_air_enable_[ValidationParams::ARRAY] = 0;
  circulate_air_enable_[ValidationParams::MANDATORY] = 0;

  // name="autoModeEnable"
  auto_mode_enable_[ValidationParams::TYPE] = ValueType::BOOL;
  auto_mode_enable_[ValidationParams::ARRAY] = 0;
  auto_mode_enable_[ValidationParams::MANDATORY] = 0;

  // name="defrostZone"
  defrost_zone_[ValidationParams::TYPE] = ValueType::ENUM;
  defrost_zone_[ValidationParams::ENUM_TYPE] = EnumType::DEFROST_ZONE;
  defrost_zone_[ValidationParams::ARRAY] = 0;
  defrost_zone_[ValidationParams::MANDATORY] = 0;

  // name="dualModeEnable"
  dual_mode_enable_[ValidationParams::TYPE] = ValueType::BOOL;
  dual_mode_enable_[ValidationParams::ARRAY] = 0;
  dual_mode_enable_[ValidationParams::MANDATORY] = 0;

  validation_scope_map_[kFanSpeed] = &fan_speed_;
  validation_scope_map_[kCurrentTemp] = &current_temp_;
  validation_scope_map_[kTemperatureUnit] = &temperature_unit_;
  validation_scope_map_[kACEnable] = &ac_enable_;
  validation_scope_map_[kCirculateAirEnable] = &circulate_air_enable_;
  validation_scope_map_[kDesiredTemp] = &desired_temp_;
  validation_scope_map_[kAutoModeEnable] = &auto_mode_enable_;
  validation_scope_map_[kDefrostZone] = &defrost_zone_;
  validation_scope_map_[kDualModeEnable] = &dual_mode_enable_;
}

ValidationResult ClimateControlDataValidator::Validate(
    const Json::Value& json, Json::Value& outgoing_json) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!json.isObject()) {
    LOG4CXX_ERROR(logger_, "ClimateControlData must be struct");
    return INVALID_DATA;
  }

  ValidationResult result = ValidateSimpleValues(json, outgoing_json);

  if (!outgoing_json.size()) {
    return INVALID_DATA;
  }

  return result;
}

void ClimateControlDataValidator::RemoveReadOnlyParams(Json::Value& json) {
  if (json.isMember(kCurrentTemp)) {
    json.removeMember(kCurrentTemp);
  }
}

}  // namespace valdiators

}  // namespace can_cooperation
