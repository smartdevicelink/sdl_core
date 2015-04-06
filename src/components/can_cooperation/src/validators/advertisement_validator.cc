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

#include "can_cooperation/validators/advertisement_validator.h"
#include "can_cooperation/validators/location_validator.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace validators {

using namespace message_params;
using namespace validation_params;

AdvertisementValidator::AdvertisementValidator() {
  // name="productName"
  product_name_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  product_name_[kMinLength] = 0;
  product_name_[kMaxLength] = 100;
  product_name_[kArray] = 0;
  product_name_[kMandatory] = 0;

  // name="companyName"
  company_name_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  company_name_[kMinLength] = 0;
  company_name_[kMaxLength] = 100;
  company_name_[kArray] = 0;
  company_name_[kMandatory] = 0;

  // name="phoneNumber"
  phone_number_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  phone_number_[kMinLength] = 0;
  phone_number_[kMaxLength] = 100;
  phone_number_[kArray] = 0;
  phone_number_[kMandatory] = 0;

  validation_scope_map_[kProductName] = &product_name_;
  validation_scope_map_[kCompanyName] =  &company_name_;
  validation_scope_map_[kPhoneNumber] = &phone_number_;
}

ValidationResult AdvertisementValidator::Validate(const Json::Value& json,
                                                 Json::Value& outgoing_json) {
  ValidationResult result = ValidateSimpleValues(json, outgoing_json);

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

