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

/**
  * @brief Enumeration FunctionID.
  *
  * Enumeration linking function names with function IDs in AppLink protocol.
  *Assumes enumeration starts at value 0.
  */
enum FunctionID {
  /**
   * @brief RESERVED.
   */
  RESERVED = 0,

  /**
   * @brief RegisterAppInterfaceID.
   */
  RegisterAppInterfaceID = 1,

  /**
   * @brief UnregisterAppInterfaceID.
   */
  UnregisterAppInterfaceID = 2,

  /**
   * @brief SetGlobalPropertiesID.
   */
  SetGlobalPropertiesID = 3,

  /**
   * @brief ResetGlobalPropertiesID.
   */
  ResetGlobalPropertiesID = 4,

  /**
   * @brief AddCommandID.
   */
  AddCommandID = 5,

  /**
   * @brief DeleteCommandID.
   */
  DeleteCommandID = 6,

  /**
   * @brief AddSubMenuID.
   */
  AddSubMenuID = 7,

  /**
   * @brief DeleteSubMenuID.
   */
  DeleteSubMenuID = 8,

  /**
   * @brief CreateInteractionChoiceSetID.
   */
  CreateInteractionChoiceSetID = 9,

  /**
   * @brief PerformInteractionID.
   */
  PerformInteractionID = 10,

  /**
   * @brief DeleteInteractionChoiceSetID.
   */
  DeleteInteractionChoiceSetID = 11,

  /**
   * @brief AlertID.
   */
  AlertID = 12,

  /**
   * @brief ShowID.
   */
  ShowID = 13,

  /**
   * @brief SpeakID.
   */
  SpeakID = 14,

  /**
   * @brief SetMediaClockTimerID.
   */
  SetMediaClockTimerID = 15,

  /**
   * @brief PerformAudioPassThruID.
   */
  PerformAudioPassThruID = 16,

  /**
   * @brief EndAudioPassThruID.
   */
  EndAudioPassThruID = 17,

  /**
   * @brief SubscribeButtonID.
   */
  SubscribeButtonID = 18,

  /**
   * @brief UnsubscribeButtonID.
   */
  UnsubscribeButtonID = 19,

  /**
   * @brief SubscribeVehicleDataID.
   */
  SubscribeVehicleDataID = 20,

  /**
   * @brief UnsubscribeVehicleDataID.
   */
  UnsubscribeVehicleDataID = 21,

  /**
   * @brief GetVehicleDataID.
   */
  GetVehicleDataID = 22,

  /**
   * @brief ReadDIDID.
   */
  ReadDIDID = 23,

  /**
   * @brief GetDTCsID.
   */
  GetDTCsID = 24,

  /**
   * @brief ScrollableMessageID.
   */
  ScrollableMessageID = 25,

  /**
   * @brief SliderID.
   */
  SliderID = 26,

  /**
   * @brief ShowConstantTBTID.
   */
  ShowConstantTBTID = 27,

  /**
   * @brief AlertManeuverID.
   */
  AlertManeuverID = 28,

  /**
   * @brief UpdateTurnListID.
   */
  UpdateTurnListID = 29,

  /**
   * @brief ChangeRegistrationID.
   */
  ChangeRegistrationID = 30,

  /**
   * @brief GenericResponseID.
   */
  GenericResponseID = 31,

  /**
   * @brief PutFileID.
   */
  PutFileID = 32,

  /**
   * @brief DeleteFileID.
   */
  DeleteFileID = 33,

  /**
   * @brief ListFilesID.
   */
  ListFilesID = 34,

  /**
   * @brief SetAppIconID.
   */
  SetAppIconID = 35,

  /**
   * @brief SetDisplayLayoutID.
   */
  SetDisplayLayoutID = 36,

  /**
   * @brief DiagnosticMessageID.
   */
  DiagnosticMessageID = 37,

  /**
   * @brief SystemRequestID.
   */
  SystemRequestID = 38,

  /**
   * @brief SendLocationID.
   */
  SendLocationID = 39,

  /**
   * @brief DialNumberID.
   */
  DialNumberID = 40,

  /**
   * @brief GetWayPointsID.
   */
  GetWayPointsID = 45,

  /**
   * @brief SubscribeWayPointsID.
   */
  SubscribeWayPointsID = 46,

  /**
   * @brief UnsubscribeWayPointsID.
   */
  UnsubscribeWayPointsID = 47,

  /**
   * @brief OnHMIStatusID.
   */
  OnHMIStatusID = 32768,

  /**
   * @brief OnAppInterfaceUnregisteredID.
   */
  OnAppInterfaceUnregisteredID = 32769,

  /**
   * @brief OnButtonEventID.
   */
  OnButtonEventID = 32770,

  /**
   * @brief OnButtonPressID.
   */
  OnButtonPressID = 32771,

  /**
   * @brief OnVehicleDataID.
   */
  OnVehicleDataID = 32772,

  /**
   * @brief OnCommandID.
   */
  OnCommandID = 32773,

  /**
   * @brief OnTBTClientStateID.
   */
  OnTBTClientStateID = 32774,

  /**
   * @brief OnDriverDistractionID.
   */
  OnDriverDistractionID = 32775,

  /**
   * @brief OnPermissionsChangeID.
   */
  OnPermissionsChangeID = 32776,

  /**
   * @brief OnAudioPassThruID.
   */
  OnAudioPassThruID = 32777,

  /**
   * @brief OnLanguageChangeID.
   */
  OnLanguageChangeID = 32778,

  /**
   * @brief OnKeyboardInputID.
   */
  OnKeyboardInputID = 32779,

  /**
   * @brief OnTouchEventID.
   */
  OnTouchEventID = 32780,

  /**
   * @brief OnSystemRequestID.
   */
  OnSystemRequestID = 32781,

  /**
   * @brief OnHashChangeID.
   */
  OnHashChangeID = 32782,

  /**
   * @brief OnWayPointChangeID.
   */
  OnWayPointChangeID = 32784,

  /**
   * @brief EncodedSyncPDataID.
   */
  EncodedSyncPDataID = 65536,

  /**
   * @brief SyncPDataID.
   */
  SyncPDataID = 65537,

  /**
   * @brief OnEncodedSyncPDataID.
   */
  OnEncodedSyncPDataID = 98304,

  /**
   * @brief OnSyncPDataID.
   */
  OnSyncPDataID = 98305
};
bool IsValidEnum(FunctionID val);
const char* EnumToJsonString(FunctionID val);
bool EnumFromJsonString(const std::string& literal, FunctionID* result);

extern const std::string kDefaultApp;
extern const std::string kPreDataConsentApp;
extern const std::string kDeviceApp;

}  // namespace policy_table_interface_base
}  // namespace rpc

#endif  // POLICY_TABLE_INTERFACE_BASE_POLICY_TABLE_INTERFACE_BASE_ENUMS_H_
