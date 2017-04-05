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

#include "can_cooperation/validators/struct_validators/radio_control_data_validator.h"
#include "can_cooperation/validators/struct_validators/rds_data_validator.h"
#include "can_cooperation/message_helper.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace validators {

CREATE_LOGGERPTR_GLOBAL(logger_, "RadioControlDataValidator")

using namespace message_params;

RadioControlDataValidator::RadioControlDataValidator() {
  // name="frequencyInteger"
  frequency_integer_[ValidationParams::TYPE] = ValueType::INT;
  frequency_integer_[ValidationParams::MIN_VALUE] = 0;
  frequency_integer_[ValidationParams::MAX_VALUE] = 1710;
  frequency_integer_[ValidationParams::ARRAY] = 0;
  frequency_integer_[ValidationParams::MANDATORY] = 0;

  // name="frequencyFraction"
  frequency_fraction_[ValidationParams::TYPE] = ValueType::INT;
  frequency_fraction_[ValidationParams::MIN_VALUE] = 0;
  frequency_fraction_[ValidationParams::MAX_VALUE] = 9;
  frequency_fraction_[ValidationParams::ARRAY] = 0;
  frequency_fraction_[ValidationParams::MANDATORY] = 0;

  // name="band"
  band_[ValidationParams::TYPE] = ValueType::ENUM;
  band_[ValidationParams::ENUM_TYPE] = EnumType::RADIO_BAND;
  band_[ValidationParams::ARRAY] = 0;
  band_[ValidationParams::MANDATORY] = 0;

  // name="availableHDs"
  available_hds_[ValidationParams::TYPE] = ValueType::INT;
  available_hds_[ValidationParams::MIN_VALUE] = 1;
  available_hds_[ValidationParams::MAX_VALUE] = 3;
  available_hds_[ValidationParams::ARRAY] = 0;
  available_hds_[ValidationParams::MANDATORY] = 0;

  // name="hdChannel"
  hd_channel_[ValidationParams::TYPE] = ValueType::INT;
  hd_channel_[ValidationParams::MIN_VALUE] = 1;
  hd_channel_[ValidationParams::MAX_VALUE] = 3;
  hd_channel_[ValidationParams::ARRAY] = 0;
  hd_channel_[ValidationParams::MANDATORY] = 0;

  // name="signalStrength"
  signal_stregth_[ValidationParams::TYPE] = ValueType::INT;
  signal_stregth_[ValidationParams::MIN_VALUE] = 0;
  signal_stregth_[ValidationParams::MAX_VALUE] = 100;
  signal_stregth_[ValidationParams::ARRAY] = 0;
  signal_stregth_[ValidationParams::MANDATORY] = 0;

  // name="signalChangeThreshold"
  signal_change_threshold_[ValidationParams::TYPE] = ValueType::INT;
  signal_change_threshold_[ValidationParams::MIN_VALUE] = 0;
  signal_change_threshold_[ValidationParams::MAX_VALUE] = 100;
  signal_change_threshold_[ValidationParams::ARRAY] = 0;
  signal_change_threshold_[ValidationParams::MANDATORY] = 0;

  // name="radioEnable"
  radio_enable_[ValidationParams::TYPE] = ValueType::BOOL;
  radio_enable_[ValidationParams::ARRAY] = 0;
  radio_enable_[ValidationParams::MANDATORY] = 0;

  // name="state"
  state_[ValidationParams::TYPE] = ValueType::ENUM;
  state_[ValidationParams::ENUM_TYPE] = EnumType::RADIO_STATE;
  state_[ValidationParams::ARRAY] = 0;
  state_[ValidationParams::MANDATORY] = 0;

  validation_scope_map_[kFrequencyInteger] = &frequency_integer_;
  validation_scope_map_[kFrequencyFraction] = &frequency_fraction_;
  validation_scope_map_[kBand] = &band_;
  validation_scope_map_[kAvailableHDs] = &available_hds_;
  validation_scope_map_[kHdChannel] = &hd_channel_;
  validation_scope_map_[kSignalStrength] = &signal_stregth_;
  validation_scope_map_[kSignalChangeThreshold] = &signal_change_threshold_;
  validation_scope_map_[kRadioEnable] = &radio_enable_;
  validation_scope_map_[kState] = &state_;
}

ValidationResult RadioControlDataValidator::Validate(
    const Json::Value& json, Json::Value& outgoing_json) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!json.isObject()) {
    LOG4CXX_ERROR(logger_, "RadioControlData must be struct");
    return INVALID_DATA;
  }

  ValidationResult result = ValidateSimpleValues(json, outgoing_json);

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  if (IsMember(json, kRdsData)) {
    RdsDataValidator validator;
    result = validator.Validate(json[kRdsData], outgoing_json[kRdsData]);
  }

  if (!outgoing_json.size()) {
    return INVALID_DATA;
  }

  return result;
}

void RadioControlDataValidator::RemoveReadOnlyParams(Json::Value& json) {
  RemoveReadOnlyParam(json, kRdsData);
  RemoveReadOnlyParam(json, kAvailableHDs);
  RemoveReadOnlyParam(json, kSignalStrength);
  RemoveReadOnlyParam(json, kSignalChangeThreshold);
  RemoveReadOnlyParam(json, kState);
  RemoveReadOnlyParam(json, kRadioEnable);
}

void RadioControlDataValidator::RemoveReadOnlyParam(Json::Value& json,
                                                    const char* param_name) {
  if (json.isMember(param_name)) {
    json.removeMember(param_name);
  }
}

}  // namespace valdiators

}  // namespace can_cooperation
