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

#include "can_cooperation/validators/song_info_validator.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace validators {

using namespace message_params;
using namespace validation_params;

SongInfoValidator::SongInfoValidator() {
  // name="name"
  name_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  name_[kMinLength] = 0;
  name_[kMaxLength] = 100;
  name_[kArray] = 0;
  name_[kMandatory] = 0;

  // name="artist"
  artist_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  artist_[kMinLength] = 0;
  artist_[kMaxLength] = 100;
  artist_[kArray] = 0;
  artist_[kMandatory] = 0;

  // name="genre"
  genre_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  genre_[kMinLength] = 0;
  genre_[kMaxLength] = 100;
  genre_[kArray] = 0;
  genre_[kMandatory] = 0;

  // name="album"
  album_[kType] = ValueType::STRING; // TODO(VS): Research Min-Max Length
  album_[kMinLength] = 0;
  album_[kMaxLength] = 100;
  album_[kArray] = 0;
  album_[kMandatory] = 0;

  // name="year"
  year_[kType] = ValueType::INT;
  year_[kMinValue] = 1000;
  year_[kMaxLength] = 3000;
  year_[kArray] = 0;
  year_[kMandatory] = 0;

  // name="duration"
  duration_[kType] = ValueType::INT; // TODO(VS): Research Min-Max Value(Maybe it must be not integer at all)
  duration_[kMinValue] = 1;
  duration_[kMaxLength] = 100;
  duration_[kArray] = 0;
  duration_[kMandatory] = 0;

  validation_scope_map_[kName] = &name_;
  validation_scope_map_[kArtist] =  &artist_;
  validation_scope_map_[kGenre] = &genre_;
  validation_scope_map_[kAlbum] = &album_;
  validation_scope_map_[kSongYear] = &year_;
  validation_scope_map_[kDuration] = &duration_;
}

ValidationResult SongInfoValidator::Validate(const Json::Value& json,
                                             Json::Value& outgoing_json) {
  return ValidateSimpleValues(json, outgoing_json);
}

}  // namespace valdiators

}  // namespace can_cooperation

