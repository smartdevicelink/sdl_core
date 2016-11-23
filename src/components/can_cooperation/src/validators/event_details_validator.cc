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

#include "can_cooperation/validators/event_details_validator.h"
#include "can_cooperation/validators/time_validator.h"
#include "can_cooperation/validators/location_validator.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace validators {

using namespace message_params;

EventDetailsValidator::EventDetailsValidator() {
  /*  // name="eventName"
    event_name_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
    event_name_[kMinLength] = 0;
    event_name_[kMaxLength] = 100;
    event_name_[kArray] = 0;
    event_name_[kMandatory] = 1;

    // name="phoneNumber"
    phone_number_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max
    Length
    phone_number_[kMinLength] = 0;
    phone_number_[kMaxLength] = 100;
    phone_number_[kArray] = 0;
    phone_number_[kMandatory] = 0;

    // name="price"
    price_[kType] = ValueType::DOUBLE; // TODO(VS): Research Min-Max Value
    price_[kMinValue] = 0.01;
    price_[kMaxValue] = 1000000;
    price_[kArray] = 0;
    price_[kMandatory] = 0;

    validation_scope_map_[kEventName] = &event_name_;
    validation_scope_map_[kPhoneNumber] =  &phone_number_;
    validation_scope_map_[kPrice] = &price_;*/
}

ValidationResult EventDetailsValidator::Validate(const Json::Value& json,
                                                 Json::Value& outgoing_json) {
  ValidationResult result = ValidateSimpleValues(json, outgoing_json);

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  if (json.isMember(kEventTime)) {
    result = TimeValidator::instance()->Validate(json[kEventTime],
                                                 outgoing_json[kEventTime]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  if (json.isMember(kLocation)) {
    result = LocationValidator::instance()->Validate(json[kLocation],
                                                     outgoing_json[kLocation]);
  }

  return result;
}

}  // namespace valdiators

}  // namespace can_cooperation
