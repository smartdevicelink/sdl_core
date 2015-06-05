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

#ifndef SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_CONSTANTS_H_
#define SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_CONSTANTS_H_

namespace can_cooperation {

namespace strings {

} //  strings

namespace result_codes {
const char kSuccess[] = "SUCCESS";
const char kUnsupportedRequest[] = "UNSUPPORTED_REQUEST";
const char kUnsupportedResource[] = "UNSUPPORTED_RESOURCE";
const char kDisallowed[] = "DISALLOWED";
const char kRejected[] = "REJECTED";
const char kAborted[] = "ABORTED";
const char kIgnored[] = "IGNORED";
const char kRetry[] = "RETRY";
const char kInUse[] = "IN_USE";
const char kVehicleDataNotAvailable[] = "VEHICLE_DATA_NOT_AVAILABLE";
const char kTimedOut[] = "TIMED_OUT";
const char kInvalidData[] = "INVALID_DATA";
const char kCharLimitExceeded[] = "CHAR_LIMIT_EXCEEDED";
const char kInvalidId[] = "INVALID_ID";
const char kDuplicateName[] = "DUPLICATE_NAME";
const char kApplicationNotRegistered[] = "APPLICATION_NOT_REGISTERED";
const char kOutOfMemory[] = "OUT_OF_MEMORY";
const char kTooManyPendingRequests[] = "TOO_MANY_PENDING_REQUESTS";
const char kWarnings[] = "WARNINGS";
const char kWrongLanguage[] = "WRONG_LANGUAGE";
const char kGenericError[] = "GENERIC_ERROR";
const char kUserDisallowed[] = "USER_DISALLOWED";
} //  result_codes

namespace json_keys {
const char kParams[] = "params";
const char kSuccess[] = "success";
const char kResultCode[] = "resultCode";
const char kResult[] = "result";
const char kInfo[] = "info";
const char kId[] = "id";
const char kJsonrpc[] = "jsonrpc";
const char kMethod[] = "method";
const char kError[] = "error";
const char kMessage[] = "message";
const char kData[] = "data";
const char kAppId[] = "appID";
const char kCode[] = "code";
} //  json_keys

namespace message_params {
const char kCustomPresets[] = "customPresets";
const char kRadioStation[]  = "radioStation";
const char kSongInfo[]      = "songInfo";
const char kEvent[]         = "event";
const char kAdvertisement[] = "advertisement";
const char kActivity[]      = "activity";
const char kTriggerSource[] = "triggerSource";

// RadioStation struct
const char kFrequency[]     = "frequency";
const char kFraction[]      = "fraction";
const char kAvailableHDs[]  = "availableHDs";
const char kCurrentHD[]     = "currentHD";
// RadioStation struct

// Time struct
const char kHours[]   = "hours";
const char kMinutes[] = "minutes";
const char kSeconds[] = "seconds";
const char kYear[]    = "year";
const char kMonth[]   = "month";
const char kDay[]     = "day";
const char kTZD[]     = "TZD";
// Time struct

// Address struct
const char kState[]   = "state";
const char kZipCode[] = "zipcode";
const char kCity[]    = "city";
const char kStreet[]  = "street";
// Address struct

// Location struct
const char kGPSCoordinates[] = "gpsCoordinates";
const char kAddress[]        = "address";
// Location struct

// SongInfo struct
const char kName[]     = "name";
const char kArtist[]   = "artist";
const char kGenre[]    = "genre";
const char kAlbum[]    = "album";
const char kSongYear[] = "year";
const char kDuration[] = "duration";
// SongInfo struct

// Advertisement struct
const char kProductName[] = "productName";
const char kCompanyName[] = "companyName";
const char kPhoneNumber[] = "phoneNumber";
const char kLocation[]    = "location";
// Advertisement struct

// EventDetails struct
const char kEventName[]      = "eventName";
// const char kPhoneNumber[] = "phoneNumber";
const char kPrice[]          = "price";
const char kEventTime[]      = "eventTime";
// EventDetails struct

// WebActivity struct
const char kURL[]        = "url";
const char kActionCode[] = "actionCode";
// WebActivity struct

// OnReverseAppsAllowing notification
const char kAllowed[]    = "allowed";
// OnReverseAppsAllowing notification

// OnPrimaryDevice notification
const char kDevice[]     = "device";
// OnPrimaryDevice notification

const char kHMIAppID[]   = "appID";
const char kHmiLevel[]   = "hmiLevel";
const char kSysContext[] = "systemContext";
const char kAudioState[] = "audioStreamingState";
} //  namespace message_params

namespace enums_value {
  // TriggerSource enum
  const char kMenu[]              = "MENU";
  const char kVR[]                = "VR";
// TriggerSource enum
} //  namespace enums

namespace validation_params {
const char kMinLength[] = "minlength";
const char kMaxLength[] = "maxlength";
const char kMinValue[]  = "minvalue";
const char kMaxValue[]  = "maxvalue";
const char kMinSize[]   = "minsize";
const char kMaxSize[]   = "maxsize";
const char kArray[]     = "array";
const char kMandatory[] = "mandatory";
const char kType[] =       "type";
} //  validation_params

}  //  namespace can_cooperation

#endif  // SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_CONSTANTS_H_
