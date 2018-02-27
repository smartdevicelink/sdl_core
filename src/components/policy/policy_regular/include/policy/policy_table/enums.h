/*
 * Copyright (c) 2017, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_POLICY_TABLE_ENUMS_H_
#define SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_POLICY_TABLE_ENUMS_H_

#include <string>

namespace rpc {
namespace policy_table_interface_base {

enum Priority {
  P_EMERGENCY,
  P_NAVIGATION,
  P_VOICECOM,
  P_COMMUNICATION,
  P_NORMAL,
  P_NONE,
};

bool IsValidEnum(Priority val);
const char* EnumToJsonString(Priority val);
bool EnumFromJsonString(const std::string& literal, Priority* result);

enum HmiLevel {
  HL_BACKGROUND,
  HL_FULL,
  HL_LIMITED,
  HL_NONE,
};
bool IsValidEnum(HmiLevel val);
const char* EnumToJsonString(HmiLevel val);
bool EnumFromJsonString(const std::string& literal, HmiLevel* result);

enum Parameter {
  P_GPS,
  P_SPEED,
  P_ENGINETORQUE,
  P_EXTERNALTEMPERATURE,
  P_FUELLEVEL,
  P_FUELLEVEL_STATE,
  P_HEADLAMPSTATUS,
  P_INSTANTFUELCONSUMPTION,
  P_FUELRANGE,
  P_ODOMETER,
  P_TIREPRESSURE,
  P_WIPERSTATUS,
  P_VIN,
  P_ACCPEDALPOSITION,
  P_BELTSTATUS,
  P_DRIVERBRAKING,
  P_PRNDL,
  P_RPM,
  P_STEERINGWHEELANGLE,
  P_ENGINEOILLIFE,
  P_MYKEY,
  P_AIRBAGSTATUS,
  P_BODYINFORMATION,
  P_CLUSTERMODESTATUS,
  P_DEVICESTATUS,
  P_EMERGENCYEVENT,
  P_ECALLINFO,
  P_EMPTY  // Added to allow empty parameters handling
};

bool IsValidEnum(Parameter val);
const char* EnumToJsonString(Parameter val);
bool EnumFromJsonString(const std::string& literal, Parameter* result);

enum AppHMIType {
  AHT_DEFAULT,
  AHT_COMMUNICATION,
  AHT_MEDIA,
  AHT_MESSAGING,
  AHT_NAVIGATION,
  AHT_INFORMATION,
  AHT_SOCIAL,
  AHT_BACKGROUND_PROCESS,
  AHT_TESTING,
  AHT_SYSTEM,
  AHT_PROJECTION,
  AHT_REMOTE_CONTROL
};
bool IsValidEnum(AppHMIType val);
const char* EnumToJsonString(AppHMIType val);
bool EnumFromJsonString(const std::string& literal, AppHMIType* result);

enum RequestType {
  RT_HTTP,
  RT_FILE_RESUME,
  RT_AUTH_REQUEST,
  RT_AUTH_CHALLENGE,
  RT_AUTH_ACK,
  RT_PROPRIETARY,
  RT_QUERY_APPS,
  RT_LAUNCH_APP,
  RT_LOCK_SCREEN_ICON_URL,
  RT_TRAFFIC_MESSAGE_CHANNEL,
  RT_DRIVER_PROFILE,
  RT_VOICE_SEARCH,
  RT_NAVIGATION,
  RT_PHONE,
  RT_CLIMATE,
  RT_SETTINGS,
  RT_VEHICLE_DIAGNOSTICS,
  RT_EMERGENCY,
  RT_MEDIA,
  RT_FOTA,
  RT_OEM_SPECIFIC,
  RT_EMPTY  // Added to allow empty Request Types handling
};

bool IsValidEnum(RequestType val);
const char* EnumToJsonString(RequestType val);
bool EnumFromJsonString(const std::string& literal, RequestType* result);

enum Input {
  I_GUI,
  I_VUI,
};
bool IsValidEnum(Input val);
const char* EnumToJsonString(Input val);
bool EnumFromJsonString(const std::string& literal, Input* result);

enum ModuleType { MT_CLIMATE, MT_RADIO, MT_SEAT };
bool IsValidEnum(ModuleType val);
const char* EnumToJsonString(ModuleType val);
bool EnumFromJsonString(const std::string& literal, ModuleType* result);

extern const std::string kDefaultApp;
extern const std::string kPreDataConsentApp;
extern const std::string kDeviceApp;

}  // namespace policy_table_interface_base
}  // namespace rpc

#endif  // SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_POLICY_TABLE_ENUMS_H_
