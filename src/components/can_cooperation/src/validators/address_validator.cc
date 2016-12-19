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

#include "can_cooperation/validators/address_validator.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace validators {

using namespace message_params;


AddressValidator::AddressValidator() {
/*  // name="state"
  state_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  state_[kMinLength] = 0;
  state_[kMaxLength] = 100;
  state_[kArray] = 0;
  state_[kMandatory] = 0;

  // name="zipcode"
  zip_code_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  zip_code_[kMinLength] = 0;
  zip_code_[kMaxLength] = 100;
  zip_code_[kArray] = 0;
  zip_code_[kMandatory] = 0;

  // name="city"
  city_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  city_[kMinLength] = 0;
  city_[kMaxLength] = 100;
  city_[kArray] = 0;
  city_[kMandatory] = 0;

  // name="street"
  street_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  street_[kMinLength] = 0;
  street_[kMaxLength] = 100;
  street_[kArray] = 0;
  street_[kMandatory] = 0;

  validation_scope_map_[kState] = &state_;
  validation_scope_map_[kZipCode] =  &zip_code_;
  validation_scope_map_[kCity] = &city_;
  validation_scope_map_[kStreet] = &street_;*/
}

ValidationResult AddressValidator::Validate(const Json::Value& json,
                                                 Json::Value& outgoing_json) {
  return ValidateSimpleValues(json, outgoing_json);
}

}  // namespace validators

}  // namespace can_cooperation

