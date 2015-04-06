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
using namespace validation_params;
using namespace json_keys;

OnRadioDetailsNotificationValidator::OnRadioDetailsNotificationValidator() {
}

ValidationResult OnRadioDetailsNotificationValidator::Validate(
    std::string& json_string) {
  Json::Value json;

  json = MessageHelper::StringToValue(json_string);

  Json::Value outgoing_json;

  ValidationResult result = ValidationResult::INVALID_DATA;

  if (json.isMember(kRadioStation)) {
    result = SongInfoValidator::instance()->Validate(
        json[kParams][kRadioStation], outgoing_json[kParams][kRadioStation]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  if (json.isMember(kSongInfo)) {
    result = SongInfoValidator::instance()->Validate(
        json[kParams][kSongInfo], outgoing_json[kParams][kSongInfo]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  if (json.isMember(kEvent)) {
    result = EventDetailsValidator::instance()->Validate(
        json[kParams][kEvent], outgoing_json[kParams][kEvent]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  if (json.isMember(kAdvertisement)) {
    result = AdvertisementValidator::instance()->Validate(
        json[kParams][kAdvertisement], outgoing_json[kParams][kAdvertisement]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  if (json.isMember(kActivity)) {
    result = WebActivityValidator::instance()->Validate(
        json[kParams][kActivity], outgoing_json[kParams][kActivity]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  if (json.isMember(kLocation)) {
    result = LocationValidator::instance()->Validate(
        json[kParams][kLocation], outgoing_json[kParams][kLocation]);
  }

  if (result != ValidationResult::SUCCESS) {
    return result;
  }

  if (json.isMember(kTriggerSource)) {
    result = ValidateEnumValue(kTriggerSource, json[kParams],
                               outgoing_json[kParams]);
  }

  if  (ValidationResult::SUCCESS == result) {
    json_string = MessageHelper::ValueToString(outgoing_json);
  }

  return result;
}

}  // namespace valdiators

}  // namespace can_cooperation

