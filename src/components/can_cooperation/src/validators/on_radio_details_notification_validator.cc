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

#include "can_cooperation/validators/on_radio_details_notification_validator.h"
#include "can_cooperation/validators/radio_station_validator.h"
#include "can_cooperation/validators/song_info_validator.h"
#include "can_cooperation/validators/event_details_validator.h"
#include "can_cooperation/validators/advertisement_validator.h"
#include "can_cooperation/validators/web_activity_validator.h"
#include "can_cooperation/validators/location_validator.h"
#include "can_cooperation/can_module_constants.h"
#include "can_cooperation/message_helper.h"

namespace can_cooperation {

namespace validators {

using namespace message_params;
using namespace json_keys;

OnRadioDetailsNotificationValidator::OnRadioDetailsNotificationValidator() {
  /*  trigger_source_[kType] = ValueType::ENUM;
    trigger_source_[kEnumType] = EnumType::TRIGGER_SOURCE;
    trigger_source_[kArray] = 0;
    trigger_source_[kMandatory] = 1;

    validation_scope_map_[kTriggerSource] = &trigger_source_;*/
}

ValidationResult OnRadioDetailsNotificationValidator::Validate(
    std::string& json_string) {
  Json::Value json;

  json = MessageHelper::StringToValue(json_string);

  Json::Value outgoing_json;

  ValidationResult result = ValidationResult::INVALID_DATA;
  SongInfoValidator song_info_validator;
  if (json.isMember(kRadioStation)) {
    result = song_info_validator.Validate(json[kRadioStation],
                                          outgoing_json[kRadioStation]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  if (json.isMember(kSongInfo)) {
    result =
        song_info_validator.Validate(json[kSongInfo], outgoing_json[kSongInfo]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  EventDetailsValidator event_details_validator;
  if (json.isMember(kEvent)) {
    result =
        event_details_validator.Validate(json[kEvent], outgoing_json[kEvent]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  AdvertisementValidator advertisement_validator;
  if (json.isMember(kAdvertisement)) {
    result = advertisement_validator.Validate(json[kAdvertisement],
                                              outgoing_json[kAdvertisement]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  WebActivityValidator web_activity_validator;
  if (json.isMember(kActivity)) {
    result = web_activity_validator.Validate(json[kActivity],
                                             outgoing_json[kActivity]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  LocationValidator location_validator;
  if (json.isMember(kLocation)) {
    result =
        location_validator.Validate(json[kLocation], outgoing_json[kLocation]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  result = ValidateSimpleValues(json, outgoing_json);

  if (ValidationResult::SUCCESS == result) {
    json_string = MessageHelper::ValueToString(outgoing_json);
  }

  return result;
}

}  // namespace valdiators

}  // namespace can_cooperation
