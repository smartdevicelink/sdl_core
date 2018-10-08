// This file is generated, do not edit
#include "policy/policy_table/enums.h"

namespace rpc {
namespace policy_table_interface_base {
bool IsValidEnum(Priority val) {
  switch (val) {
    case P_EMERGENCY:
      return true;
    case P_NAVIGATION:
      return true;
    case P_VOICECOM:
      return true;
    case P_COMMUNICATION:
      return true;
    case P_NORMAL:
      return true;
    case P_NONE:
      return true;
    default:
      return false;
  }
}
const char* EnumToJsonString(Priority val) {
  switch (val) {
    case P_EMERGENCY:
      return "EMERGENCY";
    case P_NAVIGATION:
      return "NAVIGATION";
    case P_VOICECOM:
      return "VOICECOM";
    case P_COMMUNICATION:
      return "COMMUNICATION";
    case P_NORMAL:
      return "NORMAL";
    case P_NONE:
      return "NONE";
    default:
      return "";
  }
}
bool EnumFromJsonString(const std::string& literal, Priority* result) {
  if ("EMERGENCY" == literal) {
    *result = P_EMERGENCY;
    return true;
  } else if ("NAVIGATION" == literal) {
    *result = P_NAVIGATION;
    return true;
  } else if ("VOICECOM" == literal) {
    *result = P_VOICECOM;
    return true;
  } else if ("COMMUNICATION" == literal) {
    *result = P_COMMUNICATION;
    return true;
  } else if ("NORMAL" == literal) {
    *result = P_NORMAL;
    return true;
  } else if ("NONE" == literal) {
    *result = P_NONE;
    return true;
  } else {
    return false;
  }
}

bool IsValidEnum(HmiLevel val) {
  switch (val) {
    case HL_BACKGROUND:
      return true;
    case HL_FULL:
      return true;
    case HL_LIMITED:
      return true;
    case HL_NONE:
      return true;
    default:
      return false;
  }
}
const char* EnumToJsonString(HmiLevel val) {
  switch (val) {
    case HL_BACKGROUND:
      return "BACKGROUND";
    case HL_FULL:
      return "FULL";
    case HL_LIMITED:
      return "LIMITED";
    case HL_NONE:
      return "NONE";
    default:
      return "";
  }
}
bool EnumFromJsonString(const std::string& literal, HmiLevel* result) {
  if ("BACKGROUND" == literal) {
    *result = HL_BACKGROUND;
    return true;
  } else if ("FULL" == literal) {
    *result = HL_FULL;
    return true;
  } else if ("LIMITED" == literal) {
    *result = HL_LIMITED;
    return true;
  } else if ("NONE" == literal) {
    *result = HL_NONE;
    return true;
  } else {
    return false;
  }
}

bool IsValidEnum(Parameter val) {
  switch (val) {
    case P_GPS:
      return true;
    case P_SPEED:
      return true;
    case P_ENGINETORQUE:
      return true;
    case P_EXTERNALTEMPERATURE:
      return true;
    case P_TURNSIGNAL:
      return true;
    case P_FUELLEVEL:
      return true;
    case P_FUELLEVEL_STATE:
      return true;
    case P_HEADLAMPSTATUS:
      return true;
    case P_INSTANTFUELCONSUMPTION:
      return true;
    case P_FUELRANGE:
      return true;
    case P_ODOMETER:
      return true;
    case P_TIREPRESSURE:
      return true;
    case P_WIPERSTATUS:
      return true;
    case P_VIN:
      return true;
    case P_ACCPEDALPOSITION:
      return true;
    case P_BELTSTATUS:
      return true;
    case P_ELECTRONICPARKBRAKESTATUS:
      return true;
    case P_DRIVERBRAKING:
      return true;
    case P_PRNDL:
      return true;
    case P_RPM:
      return true;
    case P_STEERINGWHEELANGLE:
      return true;
    case P_ENGINEOILLIFE:
      return true;
    case P_MYKEY:
      return true;
    case P_AIRBAGSTATUS:
      return true;
    case P_BODYINFORMATION:
      return true;
    case P_CLUSTERMODESTATUS:
      return true;
    case P_DEVICESTATUS:
      return true;
    case P_EMERGENCYEVENT:
      return true;
    case P_ECALLINFO:
      return true;
    case P_EMPTY:
      return true;
    default:
      return false;
  }
}
const char* EnumToJsonString(Parameter val) {
  switch (val) {
    case P_GPS:
      return "gps";
    case P_SPEED:
      return "speed";
    case P_ENGINETORQUE:
      return "engineTorque";
    case P_EXTERNALTEMPERATURE:
      return "externalTemperature";
    case P_TURNSIGNAL:
      return "turnSignal";
    case P_FUELLEVEL:
      return "fuelLevel";
    case P_FUELLEVEL_STATE:
      return "fuelLevel_State";
    case P_HEADLAMPSTATUS:
      return "headLampStatus";
    case P_INSTANTFUELCONSUMPTION:
      return "instantFuelConsumption";
    case P_FUELRANGE:
      return "fuelRange";
    case P_ODOMETER:
      return "odometer";
    case P_TIREPRESSURE:
      return "tirePressure";
    case P_WIPERSTATUS:
      return "wiperStatus";
    case P_VIN:
      return "vin";
    case P_ACCPEDALPOSITION:
      return "accPedalPosition";
    case P_BELTSTATUS:
      return "beltStatus";
    case P_ELECTRONICPARKBRAKESTATUS:
      return "electronicParkBrakeStatus";
    case P_DRIVERBRAKING:
      return "driverBraking";
    case P_PRNDL:
      return "prndl";
    case P_RPM:
      return "rpm";
    case P_STEERINGWHEELANGLE:
      return "steeringWheelAngle";
    case P_ENGINEOILLIFE:
      return "engineOilLife";
    case P_MYKEY:
      return "myKey";
    case P_AIRBAGSTATUS:
      return "airbagStatus";
    case P_BODYINFORMATION:
      return "bodyInformation";
    case P_CLUSTERMODESTATUS:
      return "clusterModeStatus";
    case P_DEVICESTATUS:
      return "deviceStatus";
    case P_EMERGENCYEVENT:
      return "emergencyEvent";
    case P_ECALLINFO:
      return "eCallInfo";
    case P_EMPTY:
      return "EMPTY";
    default:
      return "";
  }
}

bool EnumFromJsonString(const std::string& literal, Parameter* result) {
  if ("gps" == literal) {
    *result = P_GPS;
    return true;
  } else if ("speed" == literal) {
    *result = P_SPEED;
    return true;
  } else if ("engineTorque" == literal) {
    *result = P_ENGINETORQUE;
    return true;
  } else if ("externalTemperature" == literal) {
    *result = P_EXTERNALTEMPERATURE;
    return true;
  } else if ("turnSignal" == literal) {
    *result = P_TURNSIGNAL;
    return true;
  } else if ("fuelLevel" == literal) {
    *result = P_FUELLEVEL;
    return true;
  } else if ("fuelLevel_State" == literal) {
    *result = P_FUELLEVEL_STATE;
    return true;
  } else if ("headLampStatus" == literal) {
    *result = P_HEADLAMPSTATUS;
    return true;
  } else if ("instantFuelConsumption" == literal) {
    *result = P_INSTANTFUELCONSUMPTION;
    return true;
  } else if ("fuelRange" == literal) {
    *result = P_FUELRANGE;
    return true;
  } else if ("odometer" == literal) {
    *result = P_ODOMETER;
    return true;
  } else if ("tirePressure" == literal) {
    *result = P_TIREPRESSURE;
    return true;
  } else if ("wiperStatus" == literal) {
    *result = P_WIPERSTATUS;
    return true;
  } else if ("vin" == literal) {
    *result = P_VIN;
    return true;
  } else if ("accPedalPosition" == literal) {
    *result = P_ACCPEDALPOSITION;
    return true;
  } else if ("beltStatus" == literal) {
    *result = P_BELTSTATUS;
    return true;
  } else if ("electronicParkBrakeStatus" == literal) {
    *result = P_ELECTRONICPARKBRAKESTATUS;
    return true;
  } else if ("driverBraking" == literal) {
    *result = P_DRIVERBRAKING;
    return true;
  } else if ("prndl" == literal) {
    *result = P_PRNDL;
    return true;
  } else if ("rpm" == literal) {
    *result = P_RPM;
    return true;
  } else if ("steeringWheelAngle" == literal) {
    *result = P_STEERINGWHEELANGLE;
    return true;
  } else if ("engineOilLife" == literal) {
    *result = P_ENGINEOILLIFE;
    return true;
  } else if ("myKey" == literal) {
    *result = P_MYKEY;
    return true;
  } else if ("airbagStatus" == literal) {
    *result = P_AIRBAGSTATUS;
    return true;
  } else if ("bodyInformation" == literal) {
    *result = P_BODYINFORMATION;
    return true;
  } else if ("clusterModeStatus" == literal) {
    *result = P_CLUSTERMODESTATUS;
    return true;
  } else if ("deviceStatus" == literal) {
    *result = P_DEVICESTATUS;
    return true;
  } else if ("emergencyEvent" == literal) {
    *result = P_EMERGENCYEVENT;
    return true;
  } else if ("eCallInfo" == literal) {
    *result = P_ECALLINFO;
    return true;
  } else if ("EMPTY" == literal) {
    *result = P_EMPTY;
    return true;
  } else {
    return false;
  }
}

bool IsValidEnum(AppHMIType val) {
  switch (val) {
    case AHT_DEFAULT:
      return true;
    case AHT_COMMUNICATION:
      return true;
    case AHT_MEDIA:
      return true;
    case AHT_MESSAGING:
      return true;
    case AHT_NAVIGATION:
      return true;
    case AHT_INFORMATION:
      return true;
    case AHT_SOCIAL:
      return true;
    case AHT_BACKGROUND_PROCESS:
      return true;
    case AHT_TESTING:
      return true;
    case AHT_SYSTEM:
      return true;
    case AHT_PROJECTION:
      return true;
    case AHT_REMOTE_CONTROL:
      return true;
    default:
      return false;
  }
}
const char* EnumToJsonString(AppHMIType val) {
  switch (val) {
    case AHT_DEFAULT:
      return "DEFAULT";
    case AHT_COMMUNICATION:
      return "COMMUNICATION";
    case AHT_MEDIA:
      return "MEDIA";
    case AHT_MESSAGING:
      return "MESSAGING";
    case AHT_NAVIGATION:
      return "NAVIGATION";
    case AHT_INFORMATION:
      return "INFORMATION";
    case AHT_SOCIAL:
      return "SOCIAL";
    case AHT_BACKGROUND_PROCESS:
      return "BACKGROUND_PROCESS";
    case AHT_TESTING:
      return "TESTING";
    case AHT_SYSTEM:
      return "SYSTEM";
    case AHT_PROJECTION:
      return "PROJECTION";
    case AHT_REMOTE_CONTROL:
      return "REMOTE_CONTROL";
    default:
      return "";
  }
}
bool EnumFromJsonString(const std::string& literal, AppHMIType* result) {
  if ("DEFAULT" == literal) {
    *result = AHT_DEFAULT;
    return true;
  } else if ("COMMUNICATION" == literal) {
    *result = AHT_COMMUNICATION;
    return true;
  } else if ("MEDIA" == literal) {
    *result = AHT_MEDIA;
    return true;
  } else if ("MESSAGING" == literal) {
    *result = AHT_MESSAGING;
    return true;
  } else if ("NAVIGATION" == literal) {
    *result = AHT_NAVIGATION;
    return true;
  } else if ("INFORMATION" == literal) {
    *result = AHT_INFORMATION;
    return true;
  } else if ("SOCIAL" == literal) {
    *result = AHT_SOCIAL;
    return true;
  } else if ("BACKGROUND_PROCESS" == literal) {
    *result = AHT_BACKGROUND_PROCESS;
    return true;
  } else if ("TESTING" == literal) {
    *result = AHT_TESTING;
    return true;
  } else if ("SYSTEM" == literal) {
    *result = AHT_SYSTEM;
    return true;
  } else if ("PROJECTION" == literal) {
    *result = AHT_PROJECTION;
    return true;
  } else if ("REMOTE_CONTROL" == literal) {
    *result = AHT_REMOTE_CONTROL;
    return true;
  } else {
    return false;
  }
}

bool IsValidEnum(RequestType val) {
  switch (val) {
    case RT_HTTP:
      return true;
    case RT_FILE_RESUME:
      return true;
    case RT_AUTH_REQUEST:
      return true;
    case RT_AUTH_CHALLENGE:
      return true;
    case RT_AUTH_ACK:
      return true;
    case RT_PROPRIETARY:
      return true;
    case RT_QUERY_APPS:
      return true;
    case RT_LAUNCH_APP:
      return true;
    case RT_LOCK_SCREEN_ICON_URL:
      return true;
    case RT_TRAFFIC_MESSAGE_CHANNEL:
      return true;
    case RT_DRIVER_PROFILE:
      return true;
    case RT_VOICE_SEARCH:
      return true;
    case RT_NAVIGATION:
      return true;
    case RT_PHONE:
      return true;
    case RT_CLIMATE:
      return true;
    case RT_SETTINGS:
      return true;
    case RT_VEHICLE_DIAGNOSTICS:
      return true;
    case RT_EMERGENCY:
      return true;
    case RT_MEDIA:
      return true;
    case RT_FOTA:
      return true;
    case RT_OEM_SPECIFIC:
      return true;
    case RT_EMPTY:
      return true;
    default:
      return false;
  }
}

const char* EnumToJsonString(RequestType val) {
  switch (val) {
    case RT_HTTP:
      return "HTTP";
    case RT_FILE_RESUME:
      return "FILE_RESUME";
    case RT_AUTH_REQUEST:
      return "AUTH_REQUEST";
    case RT_AUTH_CHALLENGE:
      return "AUTH_CHALLENGE";
    case RT_AUTH_ACK:
      return "AUTH_ACK";
    case RT_PROPRIETARY:
      return "PROPRIETARY";
    case RT_QUERY_APPS:
      return "QUERY_APPS";
    case RT_LAUNCH_APP:
      return "LAUNCH_APP";
    case RT_LOCK_SCREEN_ICON_URL:
      return "LOCK_SCREEN_ICON_URL";
    case RT_TRAFFIC_MESSAGE_CHANNEL:
      return "TRAFFIC_MESSAGE_CHANNEL";
    case RT_DRIVER_PROFILE:
      return "DRIVER_PROFILE";
    case RT_VOICE_SEARCH:
      return "VOICE_SEARCH";
    case RT_NAVIGATION:
      return "NAVIGATION";
    case RT_PHONE:
      return "PHONE";
    case RT_CLIMATE:
      return "CLIMATE";
    case RT_SETTINGS:
      return "SETTINGS";
    case RT_VEHICLE_DIAGNOSTICS:
      return "VEHICLE_DIAGNOSTICS";
    case RT_EMERGENCY:
      return "EMERGENCY";
    case RT_MEDIA:
      return "MEDIA";
    case RT_FOTA:
      return "FOTA";
    case RT_OEM_SPECIFIC:
      return "OEM_SPECIFIC";
    case RT_EMPTY:
      return "EMPTY";
    default:
      return "";
  }
}

bool EnumFromJsonString(const std::string& literal, RequestType* result) {
  if ("HTTP" == literal) {
    *result = RT_HTTP;
    return true;
  }
  if ("FILE_RESUME" == literal) {
    *result = RT_FILE_RESUME;
    return true;
  }
  if ("AUTH_REQUEST" == literal) {
    *result = RT_AUTH_REQUEST;
    return true;
  }
  if ("AUTH_CHALLENGE" == literal) {
    *result = RT_AUTH_CHALLENGE;
    return true;
  }
  if ("AUTH_ACK" == literal) {
    *result = RT_AUTH_ACK;
    return true;
  }
  if ("PROPRIETARY" == literal) {
    *result = RT_PROPRIETARY;
    return true;
  }
  if ("QUERY_APPS" == literal) {
    *result = RT_QUERY_APPS;
    return true;
  }
  if ("LAUNCH_APP" == literal) {
    *result = RT_LAUNCH_APP;
    return true;
  }
  if ("LOCK_SCREEN_ICON_URL" == literal) {
    *result = RT_LOCK_SCREEN_ICON_URL;
    return true;
  }
  if ("TRAFFIC_MESSAGE_CHANNEL" == literal) {
    *result = RT_TRAFFIC_MESSAGE_CHANNEL;
    return true;
  }
  if ("DRIVER_PROFILE" == literal) {
    *result = RT_DRIVER_PROFILE;
    return true;
  }
  if ("VOICE_SEARCH" == literal) {
    *result = RT_VOICE_SEARCH;
    return true;
  }
  if ("NAVIGATION" == literal) {
    *result = RT_NAVIGATION;
    return true;
  }
  if ("PHONE" == literal) {
    *result = RT_PHONE;
    return true;
  }
  if ("CLIMATE" == literal) {
    *result = RT_CLIMATE;
    return true;
  }
  if ("SETTINGS" == literal) {
    *result = RT_SETTINGS;
    return true;
  }
  if ("VEHICLE_DIAGNOSTICS" == literal) {
    *result = RT_VEHICLE_DIAGNOSTICS;
    return true;
  }
  if ("EMERGENCY" == literal) {
    *result = RT_EMERGENCY;
    return true;
  }
  if ("MEDIA" == literal) {
    *result = RT_MEDIA;
    return true;
  }
  if ("FOTA" == literal) {
    *result = RT_FOTA;
    return true;
  }
  if ("OEM_SPECIFIC" == literal) {
    *result = RT_OEM_SPECIFIC;
    return true;
  }
  if ("EMPTY" == literal) {
    *result = RT_EMPTY;
    return true;
  } else {
    return false;
  }
}

bool IsValidEnum(ModuleType val) {
  switch (val) {
    case MT_CLIMATE:
      return true;
    case MT_RADIO:
      return true;
    case MT_SEAT:
      return true;
    case MT_AUDIO:
      return true;
    case MT_LIGHT:
      return true;
    case MT_HMI_SETTINGS:
      return true;
    case MT_EMPTY:
      return true;
    default:
      return false;
  }
}
const char* EnumToJsonString(ModuleType val) {
  switch (val) {
    case MT_CLIMATE:
      return "CLIMATE";
    case MT_RADIO:
      return "RADIO";
    case MT_SEAT:
      return "SEAT";
    case MT_AUDIO:
      return "AUDIO";
    case MT_LIGHT:
      return "LIGHT";
    case MT_HMI_SETTINGS:
      return "HMI_SETTINGS";
    case MT_EMPTY:
      return "EMPTY";
    default:
      return "";
  }
}

bool EnumFromJsonString(const std::string& literal, ModuleType* result) {
  if ("CLIMATE" == literal) {
    *result = MT_CLIMATE;
    return true;
  } else if ("RADIO" == literal) {
    *result = MT_RADIO;
    return true;
  } else if ("SEAT" == literal) {
    *result = MT_SEAT;
    return true;
  } else if ("AUDIO" == literal) {
    *result = MT_AUDIO;
    return true;
  } else if ("LIGHT" == literal) {
    *result = MT_LIGHT;
    return true;
  } else if ("HMI_SETTINGS" == literal) {
    *result = MT_HMI_SETTINGS;
    return true;
  } else if ("EMPTY" == literal) {
    *result = MT_EMPTY;
    return true;
  }
  return false;
}

const std::string kDefaultApp = "default";
const std::string kPreDataConsentApp = "pre_DataConsent";
const std::string kDeviceApp = "device";

}  // namespace policy_table_interface_base
}  // namespace rpc
