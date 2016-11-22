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

#include "can_cooperation/validators/on_preset_changed_notification_validator.h"
#include "can_cooperation/can_module_constants.h"
#include "can_cooperation/message_helper.h"

namespace can_cooperation {

namespace validators {

using namespace message_params;
using namespace json_keys;

OnPresetChangedNotificationValidator::OnPresetChangedNotificationValidator() {
  // name="customPresets"
  custom_presets_[ValidationParams::TYPE] = ValueType::STRING;
  custom_presets_[ValidationParams::MIN_LENGTH] = 0;
  custom_presets_[ValidationParams::MAX_LENGTH] = 500;
  custom_presets_[ValidationParams::MIN_SIZE] = 6;
  custom_presets_[ValidationParams::MAX_SIZE] = 6;
  custom_presets_[ValidationParams::ARRAY] = 1;
  custom_presets_[ValidationParams::MANDATORY] = 1;

  validation_scope_map_[kCustomPresets] = &custom_presets_;
}

ValidationResult OnPresetChangedNotificationValidator::Validate(
    std::string& json_string) {
  Json::Value json;

  json = MessageHelper::StringToValue(json_string);

  Json::Value outgoing_json;

  ValidationResult result = ValidateSimpleValues(json,
                                                 outgoing_json);

  if  (ValidationResult::SUCCESS == result) {
    json_string = MessageHelper::ValueToString(outgoing_json);
  }

  return result;
}

}  // namespace valdiators

}  // namespace can_cooperation

