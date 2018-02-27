/*
 Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RC_MODULE_CONSTANTS_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RC_MODULE_CONSTANTS_H_

namespace rc_rpc_plugin {

namespace strings {
// RemoteControlCapabilities constants
const char kclimateControlCapabilities[] = "climateControlCapabilities";
const char kradioControlCapabilities[] = "radioControlCapabilities";
const char kbuttonCapabilities[] = "buttonCapabilities";
const char kseatControlCapabilities[] = "seatControlCapabilities";
}  //  strings

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
const char kReadOnly[] = "READ_ONLY";
}  //  result_codes

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
}  //  json_keys

namespace message_params {
const char kName[] = "name";

// SetInteriorVehicleData request
const char kModuleData[] = "moduleData";
// SetInteriorVehicleData request

// GetInteriorVehicleData request
const char kSubscribe[] = "subscribe";
// GetInteriorVehicleData request

// GetInteriorVehicleData response
const char kIsSubscribed[] = "isSubscribed";
// GetInteriorVehicleData response

// OnRemoteControlSettings notification
const char kAccessMode[] = "accessMode";
const char kAllowed[] = "allowed";
// OnRemoteControlSettings notification

// ButtonPress request
const char kModuleType[] = "moduleType";
const char kButtonName[] = "buttonName";
const char kButtonPressMode[] = "buttonPressMode";
// ButtonPress request

// RdsData struct
const char kPS[] = "PS";
const char kRT[] = "RT";
const char kCT[] = "CT";
const char kPI[] = "PI";
const char kPTY[] = "PTY";
const char kTA[] = "TA";
const char kTP[] = "TP";
const char kREG[] = "REG";
// RdsData struct

// RadioControlData struct
const char kFrequencyInteger[] = "frequencyInteger";
const char kFrequencyFraction[] = "frequencyFraction";
const char kBand[] = "band";
const char kRdsData[] = "rdsData";
const char kAvailableHDs[] = "availableHDs";
const char kHdChannel[] = "hdChannel";
const char kSignalStrength[] = "signalStrength";
const char kSignalChangeThreshold[] = "signalChangeThreshold";
const char kRadioEnable[] = "radioEnable";
const char kState[] = "state";
// RadioControlData struct

// ClimateControlData struct
const char kFanSpeed[] = "fanSpeed";
const char kCurrentTemperature[] = "currentTemperature";
const char kDesiredTemperature[] = "desiredTemperature";
const char kTemperatureUnit[] = "temperatureUnit";
const char kACEnable[] = "acEnable";
const char kCirculateAirEnable[] = "circulateAirEnable";
const char kAutoModeEnable[] = "autoModeEnable";
const char kDefrostZone[] = "defrostZone";
const char kDualModeEnable[] = "dualModeEnable";
const char kACMaxEnable[] = "acMaxEnable";
const char kVentilationMode[] = "ventilationMode";
// ClimateControlData struct

// ModuleData struct
const char kRadioControlData[] = "radioControlData";
const char kClimateControlData[] = "climateControlData";
const char kSeatControlData[] = "seatControlData";
// ModuleData struct

const char kHMIAppID[] = "appID";
const char kHmiLevel[] = "hmiLevel";
const char kSysContext[] = "systemContext";
const char kAudioState[] = "audioStreamingState";
}  //  namespace message_params

namespace enums_value {

// ModuleType enum
const char kClimate[] = "CLIMATE";
const char kRadio[] = "RADIO";
const char kSeat[] = "SEAT";
// ModuleType enum

// RadioBand enum
const char kAM[] = "AM";
const char kFM[] = "FM";
const char kXM[] = "XM";
// RadioBand enum

// RadioState enum
const char kAcquiring[] = "ACQUIRING";
const char kAcquired[] = "ACQUIRED";
const char kMulticast[] = "MULTICAST";
const char kNotFound[] = "NOT_FOUND";
// RadioState enum

// DefrostZone enum
const char kFront[] = "FRONT";
const char kRear[] = "REAR";
const char kAll[] = "ALL";
// DefrostZone enum

// TemperatureUnit enum
const char kFahrenheit[] = "FAHRENHEIT";
const char kCelsius[] = "CELSIUS";
// TemperatureUnit enum

// ButtonName enum
const char kACMax[] = "AC_MAX";
const char kAC[] = "AC";
const char kRecirculate[] = "RECIRCULATE";
const char kFanUp[] = "FAN_UP";
const char kFanDown[] = "FAN_DOWN";
const char kTempUp[] = "TEMP_UP";
const char kTempDown[] = "TEMP_DOWN";
const char kDefrostMax[] = "DEFROST_MAX";
const char kDefrost[] = "DEFROST";
const char kDefrostRear[] = "DEFROST_REAR";
const char kUpperVent[] = "UPPER_VENT";
const char kLowerVent[] = "LOWER_VENT";
const char kVolumeUp[] = "VOLUME_UP";
const char kVolumeDown[] = "VOLUME_DOWN";
const char kEject[] = "EJECT";
const char kSource[] = "SOURCE";
const char kShuffle[] = "SHUFFLE";
const char kRepeat[] = "REPEAT";
// ButtonName enum

// ButtonPressMode enum
const char kLong[] = "LONG";
const char kShort[] = "SHORT";
// ButtonPressMode enum

// Access mode enum
const char kAutoAllow[] = "AUTO_ALLOW";
const char kAutoDeny[] = "AUTO_DENY";
const char kAskDriver[] = "ASK_DRIVER";
// Access mode enum

}  //  namespace enums_value

}  //  namespace remote_control

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RC_MODULE_CONSTANTS_H_
