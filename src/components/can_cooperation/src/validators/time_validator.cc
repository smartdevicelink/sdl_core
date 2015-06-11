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

#include "can_cooperation/validators/time_validator.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace validators {

using namespace message_params;


TimeValidator::TimeValidator() {
/*  // name="hours"
  hours_[kType] = ValueType::INT;
  hours_[kMinValue] = 0;
  hours_[kMaxValue] = 23;
  hours_[kArray] = 0;
  hours_[kMandatory] = 0;

  // name="minutes"
  minutes_[kType] = ValueType::INT;
  minutes_[kMinValue] = 0;
  minutes_[kMaxValue] = 59;
  minutes_[kArray] = 0;
  minutes_[kMandatory] = 0;

  // name="seconds"
  seconds_[kType] = ValueType::INT;
  seconds_[kMinValue] = 0;
  seconds_[kMaxValue] = 59;
  seconds_[kArray] = 0;
  seconds_[kMandatory] = 0;

  // name="year"
  year_[kType] = ValueType::INT;
  year_[kMinValue] = 0;
  year_[kMaxValue] = 3000;
  year_[kArray] = 0;
  year_[kMandatory] = 0;

  // name="month"
  month_[kType] = ValueType::INT;
  month_[kMinValue] = 1;
  month_[kMaxValue] = 12;
  month_[kArray] = 0;
  month_[kMandatory] = 0;

  // name="day"
  day_[kType] = ValueType::INT;
  day_[kMinValue] = 1;
  day_[kMaxValue] = 31;
  day_[kArray] = 0;
  day_[kMandatory] = 0;

  // name="TZD"
  tzd_[kType] = ValueType::STRING; // TODO(VS): Research why it's string
  tzd_[kMinLength] = 0;
  tzd_[kMaxLength] = 100;
  tzd_[kArray] = 0;
  tzd_[kMandatory] = 0;


  validation_scope_map_[kHours] = &hours_;
  validation_scope_map_[kMinutes] =  &minutes_;
  validation_scope_map_[kSeconds] = &seconds_;
  validation_scope_map_[kYear] = &year_;
  validation_scope_map_[kMonth] = &month_;
  validation_scope_map_[kDay] = &day_;
  validation_scope_map_[kTZD] = &tzd_;*/
}

ValidationResult TimeValidator::Validate(const Json::Value& json,
                                                 Json::Value& outgoing_json) {
  return ValidateSimpleValues(json, outgoing_json);
}

}  // namespace valdiators

}  // namespace can_cooperation

