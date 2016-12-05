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

#include "can_cooperation/validators/validator.h"
#include "can_cooperation/can_module_constants.h"
#include "can_cooperation/message_helper.h"
#include "utils/logger.h"

namespace can_cooperation {

namespace validators {

CREATE_LOGGERPTR_GLOBAL(logger_, "Validator")

using namespace message_params;

Validator::~Validator() {}

ValidationResult Validator::ValidateSimpleValues(const Json::Value& json,
                                                 Json::Value& outgoing_json) {
  ValidationScopeMap::iterator it = validation_scope_map_.begin();

  ValidationResult result = ValidationResult::INVALID_DATA;

  for (; it != validation_scope_map_.end(); ++it) {
    result = ValidateValue(it->first, json, outgoing_json, *(it->second));

    if (ValidationResult::SUCCESS != result) {
      return result;
    }
  }

  if (ValidationResult::SUCCESS != result) {
    LOG4CXX_ERROR(logger_, "Validation scope map empty!");
  }

  return result;
}

ValidationResult Validator::ValidateValue(const std::string& value_name,
                                          const Json::Value& json,
                                          Json::Value& outgoing_json,
                                          ValidationScope& validation_scope) {
  // Check if param exist, and its mandatory
  if (!IsMember(json, value_name)) {
    if (validation_scope[ValidationParams::MANDATORY]) {
      LOG4CXX_ERROR(logger_, "Mandatory param " << value_name << " missing!");
      return ValidationResult::INVALID_DATA;
    } else {
      return ValidationResult::SUCCESS;
    }
  }

  ValidationResult result = ValidationResult::INVALID_DATA;

  // Check if param array. If it is array check array borders
  if (validation_scope[ValidationParams::ARRAY]) {
    if (!json[value_name].isArray()) {
      LOG4CXX_ERROR(logger_, value_name << " must be array!");
      return ValidationResult::INVALID_DATA;
    }

    int size = json[value_name].size();

    if ((validation_scope[ValidationParams::MAX_SIZE] < size) ||
        (validation_scope[ValidationParams::MIN_SIZE] > size)) {
      LOG4CXX_ERROR(logger_, value_name << " array wrong size!");
      return ValidationResult::INVALID_DATA;
    }

    for (int i = 0; i < size; ++i) {
      LOG4CXX_INFO(logger_,
                   value_name << " " << i << " array member validation.");
      result = Validate(json[value_name][i], validation_scope);

      if (ValidationResult::SUCCESS != result) {
        return result;
      }

      outgoing_json[value_name][i] = json[value_name][i];
    }
  } else {
    LOG4CXX_INFO(logger_, value_name << " validation.");
    result = Validate(json[value_name], validation_scope);

    if (ValidationResult::SUCCESS != result) {
      return result;
    }

    outgoing_json[value_name] = json[value_name];
  }

  return ValidationResult::SUCCESS;
}

ValidationResult Validator::Validate(const Json::Value& json,
                                     ValidationScope& validation_scope) {
  if (ValueType::STRING == validation_scope[ValidationParams::TYPE]) {
    if (!json.isString()) {
      LOG4CXX_ERROR(logger_, "Must be string!");
      return ValidationResult::INVALID_DATA;
    }

    return ValidateStringValue(json.asCString(), validation_scope);
  } else if (ValueType::INT == validation_scope[ValidationParams::TYPE]) {
    if (!json.isInt()) {
      LOG4CXX_ERROR(logger_, "Must be integer!");
      return ValidationResult::INVALID_DATA;
    }

    return ValidateIntValue(json.asInt(), validation_scope);
  } else if (ValueType::DOUBLE == validation_scope[ValidationParams::TYPE]) {
    if (!json.isDouble()) {
      LOG4CXX_ERROR(logger_, "Must be fractional!");
      return ValidationResult::INVALID_DATA;
    }

    return ValidateDoubleValue(json.asDouble(), validation_scope);
  } else if (ValueType::BOOL == validation_scope[ValidationParams::TYPE]) {
    if (!json.isBool()) {
      LOG4CXX_ERROR(logger_, "Must be boolean!");
      return ValidationResult::INVALID_DATA;
    }

    return ValidationResult::SUCCESS;
  } else if (ValueType::ENUM == validation_scope[ValidationParams::TYPE]) {
    if (!json.isString()) {
      LOG4CXX_ERROR(logger_,
                    "Enum field must be represented as string in json!");
      return ValidationResult::INVALID_DATA;
    }

    return ValidateEnumValue(json.asCString(), validation_scope);
  } else {
    return ValidationResult::INVALID_DATA;
  }
}

ValidationResult Validator::ValidateIntValue(
    int value, ValidationScope& validation_scope) {
  if ((value < validation_scope[ValidationParams::MIN_VALUE]) ||
      (value > validation_scope[ValidationParams::MAX_VALUE])) {
    LOG4CXX_ERROR(logger_, "Out of scope!");
    return ValidationResult::INVALID_DATA;
  }

  return ValidationResult::SUCCESS;
}

ValidationResult Validator::ValidateDoubleValue(
    double value, ValidationScope& validation_scope) {
  if ((value < validation_scope[ValidationParams::MIN_VALUE]) ||
      (value > validation_scope[ValidationParams::MAX_VALUE])) {
    LOG4CXX_ERROR(logger_, "Out of scope!");
    return ValidationResult::INVALID_DATA;
  }

  return ValidationResult::SUCCESS;
}

ValidationResult Validator::ValidateStringValue(
    const std::string& value, ValidationScope& validation_scope) {
  int size = value.size();
  if ((size < validation_scope[ValidationParams::MIN_LENGTH]) ||
      (size > validation_scope[ValidationParams::MAX_LENGTH])) {
    LOG4CXX_ERROR(logger_, "Out of scope!");
    return ValidationResult::INVALID_DATA;
  }

  return ValidationResult::SUCCESS;
}

// TODO(VS): this function needs refactoring(maybe better use vectors with enum
// values and find())
ValidationResult Validator::ValidateEnumValue(
    const std::string& value, ValidationScope& validation_scope) {
  if (validation_scope[ValidationParams::ENUM_TYPE] == EnumType::MODULE_TYPE) {
    if (value != enums_value::kClimate && value != enums_value::kRadio) {
      LOG4CXX_ERROR(logger_, "Wrong ModuleType enum value!");
      return ValidationResult::INVALID_DATA;
    }
    /*  } else if (validation_scope[ValidationParams::ENUM_TYPE] ==
        EnumType::TRIGGER_SOURCE) {
        if (value != enums_value::kMenu &&
            value != enums_value::kVR) {
          LOG4CXX_ERROR(logger_, "Wrong triggerSource enum value!");
          return ValidationResult::INVALID_DATA;
        }*/
  } else if (validation_scope[ValidationParams::ENUM_TYPE] ==
             EnumType::RADIO_BAND) {
    if (value != enums_value::kFM && value != enums_value::kAM &&
        value != enums_value::kXM) {
      LOG4CXX_ERROR(logger_, "Wrong RadioBand enum value!");
      return ValidationResult::INVALID_DATA;
    }
  } else if (validation_scope[ValidationParams::ENUM_TYPE] ==
             EnumType::RADIO_STATE) {
    if (value != enums_value::kAcquiring && value != enums_value::kAcquired &&
        value != enums_value::kMulticast && value != enums_value::kNotFound) {
      LOG4CXX_ERROR(logger_, "Wrong RadioState enum value!");
      return ValidationResult::INVALID_DATA;
    }
  } else if (validation_scope[ValidationParams::ENUM_TYPE] ==
             EnumType::DEFROST_ZONE) {
    if (value != enums_value::kFront && value != enums_value::kRear &&
        value != enums_value::kAll) {
      LOG4CXX_ERROR(logger_, "Wrong DefrostZone enum value!");
      return ValidationResult::INVALID_DATA;
    }
  } else if (validation_scope[ValidationParams::ENUM_TYPE] ==
             EnumType::TEMPERATURE_UNIT) {
    if (value != enums_value::kKelvin && value != enums_value::kFahrenheit &&
        value != enums_value::kCelsius) {
      LOG4CXX_ERROR(logger_, "Wrong TemperatureUnit enum value!");
      return ValidationResult::INVALID_DATA;
    }
  } else if (validation_scope[ValidationParams::ENUM_TYPE] ==
             EnumType::BUTTON_NAME) {
    if (value != enums_value::kACMax && value != enums_value::kAC &&
        value != enums_value::kRecirculate && value != enums_value::kFanUp &&
        value != enums_value::kFanDown && value != enums_value::kTempUp &&
        value != enums_value::kTempDown && value != enums_value::kDefrostMax &&
        value != enums_value::kDefrost && value != enums_value::kDefrostRear &&
        value != enums_value::kUpperVent && value != enums_value::kLowerVent &&
        value != enums_value::kVolumeUp && value != enums_value::kVolumeDown &&
        value != enums_value::kEject && value != enums_value::kSource &&
        value != enums_value::kShuffle && value != enums_value::kRepeat) {
      LOG4CXX_ERROR(logger_, "Wrong ButtonName enum value!");
      return ValidationResult::INVALID_DATA;
    }
  } else if (validation_scope[ValidationParams::ENUM_TYPE] ==
             EnumType::BUTTON_PRESS_MODE) {
    if (value != enums_value::kLong && value != enums_value::kShort) {
      LOG4CXX_ERROR(logger_, "Wrong ButtonPressMode enum value!");
      return ValidationResult::INVALID_DATA;
    }
  } else {
    LOG4CXX_ERROR(logger_, "Unknow enum!");
    return ValidationResult::INVALID_DATA;
  }

  return ValidationResult::SUCCESS;
}

}  // namespace validators

}  // namespace can_cooperation
