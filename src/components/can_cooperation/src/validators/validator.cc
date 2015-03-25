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

namespace can_cooperation {

namespace validators {

using namespace validation_params;
using namespace enums_value;
using namespace message_params;

ValidationResult Validator::ValidateSimpleValues(const Json::Value& json,
                                       Json::Value& outgoing_json) {
  ValidationScopeMap::iterator it = validation_scope_map_.begin();

  ValidationResult result;

  for (; it != validation_scope_map_.end(); ++it) {
    result = ValidateValue(it->first, json, outgoing_json, *(it->second));

    if (ValidationResult::SUCCESS != result) {
      return result;
    }
  }

  return result;
}


ValidationResult Validator::ValidateEnumValue(const std::string& enum_name,
                                  const Json::Value& json,
                                  Json::Value& outgoing_json) {
  if (enum_name == kTriggerSource) {
    std::string value = json[kTriggerSource].asCString();

    if (value != kMenu &&
        value != kVR) {
      return ValidationResult::INVALID_DATA;
    }

    outgoing_json[kTriggerSource] = json[kTriggerSource].asCString();
  } else {
    return ValidationResult::INVALID_DATA;
  }

  return ValidationResult::SUCCESS;
}

ValidationResult Validator::ValidateValue(const std::string& value_name,
                                 const Json::Value& json,
                                 Json::Value& outgoing_json,
                                 ValidationScope& validation_scope) {
  // Check if param exist, and its mandatory
  if (!json.isMember(value_name)) {
    if (validation_scope[kMandatory]) {
      return ValidationResult::INVALID_DATA;
    } else {
      return ValidationResult::SUCCESS;
    }
  }

  ValidationResult result = ValidationResult::SUCCESS;

  // Check if param array. If it is array check array borders
  if (validation_scope[kArray]) {
    if (!json[value_name].isArray()) {
      return ValidationResult::INVALID_DATA;
    }

    int size = json[value_name].size();

    if ((validation_scope[kMaxSize] < size) ||
        (validation_scope[kMinSize] > size)) {
      return ValidationResult::INVALID_DATA;
    }

    for (int i = 0; i < size; ++i) {
      result = Validate(json[value_name][i], validation_scope);

      if (ValidationResult::SUCCESS != result) {
        return result;
      }

      outgoing_json[value_name][i] = json[value_name][i];
    }
  } else {
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
  if (ValueType::STRING == validation_scope[kType]) {
    if (!json.isString()) {
      return ValidationResult::INVALID_DATA;
    }

    return ValidateStringValue(json.asCString(), validation_scope);
  } else if (ValueType::INT == validation_scope[kType]) {
    if (!json.isInt()) {
      return ValidationResult::INVALID_DATA;
    }

    return ValidateIntValue(json.asInt(), validation_scope);
  } else if (ValueType::DOUBLE == validation_scope[kType]) {
      if (!json.isDouble()) {
        return ValidationResult::INVALID_DATA;
      }

      return ValidateDoubleValue(json.asDouble(), validation_scope);
  } else if (ValueType::BOOL == validation_scope[kType]) {
    if (!json.isBool()) {
      return ValidationResult::INVALID_DATA;
    }

    return ValidationResult::SUCCESS;
  } else {
    return ValidationResult::INVALID_DATA;
  }
}

ValidationResult Validator::ValidateIntValue(int value,
                                            ValidationScope& validation_scope) {
  if ((value < validation_scope[kMinValue]) ||
      (value > validation_scope[kMaxValue])) {
    return ValidationResult::INVALID_DATA;
  }

  return ValidationResult::SUCCESS;
}

ValidationResult Validator::ValidateDoubleValue(double value,
                                            ValidationScope& validation_scope) {
  if ((value < validation_scope[kMinValue]) ||
      (value > validation_scope[kMaxValue])) {
    return ValidationResult::INVALID_DATA;
  }

  return ValidationResult::SUCCESS;
}

ValidationResult Validator::ValidateStringValue(const std::string& value,
                                            ValidationScope& validation_scope) {
  int size = value.size();
  if ((size < validation_scope[kMinLength]) ||
      (size > validation_scope[kMaxLength])) {
    return ValidationResult::INVALID_DATA;
  }

  return ValidationResult::SUCCESS;
}

}  // namespace validators

}  // namespace can_cooperation

