// This file is generated, do not edit
#ifndef POLICY_TABLE_INTERFACE_BASE_POLICY_TABLE_INTERFACE_BASE_ENUMS_H_
#define POLICY_TABLE_INTERFACE_BASE_POLICY_TABLE_INTERFACE_BASE_ENUMS_H_
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
  P_MYKEY,
  P_AIRBAGSTATUS,
  P_BODYINFORMATION,
  P_CLUSTERMODESTATUS,
  P_DEVICESTATUS,
  P_EMERGENCYEVENT,
  P_ECALLINFO,
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
  RT_FOTA
};

bool IsValidEnum(RequestType val);
const char* EnumToJsonString(RequestType val);
bool EnumFromJsonString(const std::string& literal, RequestType* result);

extern const std::string kDefaultApp;
extern const std::string kPreDataConsentApp;
extern const std::string kDeviceApp;

}  // namespace policy_table_interface_base
}  // namespace rpc

#endif  // POLICY_TABLE_INTERFACE_BASE_POLICY_TABLE_INTERFACE_BASE_ENUMS_H_
