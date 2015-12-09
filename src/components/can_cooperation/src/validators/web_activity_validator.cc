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

#include "can_cooperation/validators/web_activity_validator.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace validators {

WebActivityValidator::WebActivityValidator() {
/*  // name="url"
  url_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  url_[kMinLength] = 0;
  url_[kMaxLength] = 100;
  url_[kArray] = 0;
  url_[kMandatory] = 1;

  // name="actionCode"
  action_code_[kType] = ValueType::INT; // TODO(VS): Research Min-Max Value
  action_code_[kMinValue] = 0;
  action_code_[kMaxValue] = 100;
  action_code_[kArray] = 0;
  action_code_[kMandatory] = 0;

  validation_scope_map_[kURL] = &url_;
  validation_scope_map_[kActionCode] =  &action_code_;*/
}

ValidationResult WebActivityValidator::Validate(const Json::Value& json,
                                                 Json::Value& outgoing_json) {
  return ValidateSimpleValues(json, outgoing_json);
}

}  // namespace validators

}  // namespace can_cooperation

