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

#include "can_cooperation/validators/radio_station_validator.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace validators {

using namespace message_params;
using namespace validation_params;

RadioStationValidator::RadioStationValidator() {
  // name="frequency"
  frequency_[kType] = ValueType::INT;
  frequency_[kMinValue] = 87;
  frequency_[kMaxValue] = 100;
  frequency_[kArray] = 0;
  frequency_[kMandatory] = 1;

  // name="fraction"
  fraction_[kType] = ValueType::INT;
  fraction_[kMinValue] = 0;
  fraction_[kMaxValue] = 9;
  fraction_[kArray] = 0;
  fraction_[kMandatory] = 0;

  // name="availableHDs"
  available_hds_[kType] = ValueType::INT;
  available_hds_[kMinValue] = 1;
  available_hds_[kMaxValue] = 3;
  available_hds_[kArray] = 0;
  available_hds_[kMandatory] = 0;

  // name="currentHD"
  current_hd_[kType] = ValueType::INT;
  current_hd_[kMinValue] = 1;
  current_hd_[kMaxValue] = 3;
  current_hd_[kArray] = 0;
  current_hd_[kMandatory] = 0;

  validation_scope_map_[kFrequency] = &frequency_;
  validation_scope_map_[kFraction] =  &fraction_;
  validation_scope_map_[kAvailableHDs] = &frequency_;
  validation_scope_map_[kCurrentHD] = &current_hd_;
}

ValidationResult RadioStationValidator::Validate(const Json::Value& json,
                                                 Json::Value& outgoing_json) {
  return ValidateSimpleValues(json, outgoing_json);
}

}  // namespace valdiators

}  // namespace can_cooperation

