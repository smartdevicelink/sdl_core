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

#include "application_manager/message_helper.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS

#include <set>
#include <string>
#include <algorithm>
#include <utility>
#include <map>

#include "application_manager/application.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/policies/policy_handler.h"
#include "config_profile/profile.h"
#include "connection_handler/connection_handler_impl.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/file_system.h"
#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/make_shared.h"

#include "formatters/formatter_json_rpc.h"
#include "formatters/CFormatterJsonSDLRPCv2.h"
#include "formatters/CFormatterJsonSDLRPCv1.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

namespace application_manager {
namespace {

typedef std::map<std::string, hmi_apis::Common_AppPriority::eType>
    CommonAppPriorityMap;

CommonAppPriorityMap app_priority_values = {
    {"NORMAL", hmi_apis::Common_AppPriority::NORMAL},
    {"COMMUNICATION", hmi_apis::Common_AppPriority::COMMUNICATION},
    {"EMERGENCY", hmi_apis::Common_AppPriority::EMERGENCY},
    {"NAVIGATION", hmi_apis::Common_AppPriority::NAVIGATION},
    {"NONE", hmi_apis::Common_AppPriority::NONE},
    {"VOICECOM", hmi_apis::Common_AppPriority::VOICE_COMMUNICATION},
    {"INVALID_ENUM", hmi_apis::Common_AppPriority::INVALID_ENUM}};

bool ValidateSoftButtons(smart_objects::SmartObject& soft_buttons) {
  using namespace smart_objects;
  for (size_t i = 0; i < soft_buttons.length(); ++i) {
    SmartObject& button = soft_buttons[i];

    // Check if image parameter is valid
    if (button.keyExists(strings::image)) {
      SmartObject& buttonImage = button[strings::image];

      // Image name must not be empty and must not contain incorrect
      // character
      if (false == MessageHelper::VerifySoftButtonString(
                       buttonImage[strings::value].asString())) {
        return false;
      }
    }
  }
  return true;
}  // namespace
}
std::pair<std::string, VehicleDataType> kVehicleDataInitializer[] = {
    std::make_pair(strings::gps, VehicleDataType::GPS),
    std::make_pair(strings::speed, VehicleDataType::SPEED),
    std::make_pair(strings::rpm, VehicleDataType::RPM),
    std::make_pair(strings::fuel_level, VehicleDataType::FUELLEVEL),
    std::make_pair(strings::fuel_level_state, VehicleDataType::FUELLEVEL_STATE),
    std::make_pair(strings::instant_fuel_consumption,
                   VehicleDataType::FUELCONSUMPTION),
    std::make_pair(strings::external_temp, VehicleDataType::EXTERNTEMP),
    std::make_pair(strings::vin, VehicleDataType::VIN),
    std::make_pair(strings::prndl, VehicleDataType::PRNDL),
    std::make_pair(strings::tire_pressure, VehicleDataType::TIREPRESSURE),
    std::make_pair(strings::odometer, VehicleDataType::ODOMETER),
    std::make_pair(strings::belt_status, VehicleDataType::BELTSTATUS),
    std::make_pair(strings::body_information, VehicleDataType::BODYINFO),
    std::make_pair(strings::device_status, VehicleDataType::DEVICESTATUS),
    std::make_pair(strings::driver_braking, VehicleDataType::BRAKING),
    std::make_pair(strings::wiper_status, VehicleDataType::WIPERSTATUS),
    std::make_pair(strings::head_lamp_status, VehicleDataType::HEADLAMPSTATUS),
    std::make_pair(strings::e_call_info, VehicleDataType::ECALLINFO),
    std::make_pair(strings::airbag_status, VehicleDataType::AIRBAGSTATUS),
    std::make_pair(strings::emergency_event, VehicleDataType::EMERGENCYEVENT),
    std::make_pair(strings::cluster_mode_status,
                   VehicleDataType::CLUSTERMODESTATUS),
    std::make_pair(strings::my_key, VehicleDataType::MYKEY),
    /*
     NOT DEFINED in mobile API
     std::make_pair(strings::gps, VehicleDataType::BATTVOLTAGE),
     */
    std::make_pair(strings::engine_torque, VehicleDataType::ENGINETORQUE),
    std::make_pair(strings::acc_pedal_pos, VehicleDataType::ACCPEDAL),
    std::make_pair(strings::steering_wheel_angle,
                   VehicleDataType::STEERINGWHEEL),
};

const VehicleData MessageHelper::vehicle_data_(
    kVehicleDataInitializer,
    kVehicleDataInitializer + ARRAYSIZE(kVehicleDataInitializer));

#ifdef HMI_DBUS_API
namespace {
struct VehicleInfo_Requests {
  hmi_apis::FunctionID::eType func_id;
  const char* str;
};
static VehicleInfo_Requests ivi_subrequests[] = {
    {hmi_apis::FunctionID::VehicleInfo_SubscribeGps, strings::gps},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeSpeed, strings::speed},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeRpm, strings::rpm},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeFuelLevel, strings::fuel_level},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeFuelLevel_State,
     strings::fuel_level_state},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeInstantFuelConsumption,
     strings::instant_fuel_consumption},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeExternalTemperature,
     strings::external_temp},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeVin, strings::vin},
    {hmi_apis::FunctionID::VehicleInfo_SubscribePrndl, strings::prndl},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeTirePressure,
     strings::tire_pressure},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeOdometer, strings::odometer},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeBeltStatus,
     strings::belt_status},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeBodyInformation,
     strings::body_information},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeDeviceStatus,
     strings::device_status},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeDriverBraking,
     strings::driver_braking},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeWiperStatus,
     strings::wiper_status},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeHeadLampStatus,
     strings::head_lamp_status},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeEngineTorque,
     strings::engine_torque},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeAccPedalPosition,
     strings::acc_pedal_pos},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeSteeringWheelAngle,
     strings::steering_wheel_angle},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeECallInfo,
     strings::e_call_info},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeAirbagStatus,
     strings::airbag_status},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeEmergencyEvent,
     strings::emergency_event},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeClusterModeStatus,
     strings::cluster_mode_status},
    {hmi_apis::FunctionID::VehicleInfo_SubscribeMyKey, strings::my_key},
};
}
#endif  // #ifdef HMI_DBUS_API

const uint32_t MessageHelper::GetPriorityCode(const std::string& priority) {
  CommonAppPriorityMap::const_iterator it = app_priority_values.find(priority);
  if (app_priority_values.end() != it) {
    return static_cast<uint32_t>((*it).second);
  }
  return static_cast<uint32_t>(hmi_apis::Common_AppPriority::INVALID_ENUM);
}

std::string MessageHelper::CommonLanguageToString(
    hmi_apis::Common_Language::eType language) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  const char* str = 0;
  if (EnumConversionHelper<hmi_apis::Common_Language::eType>::EnumToCString(
          language, &str)) {
    return str ? str : "";
  }
  return std::string();
}

hmi_apis::Common_Language::eType MessageHelper::CommonLanguageFromString(
    const std::string& language) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  hmi_apis::Common_Language::eType value;
  if (EnumConversionHelper<hmi_apis::Common_Language::eType>::StringToEnum(
          language, &value)) {
    return value;
  }
  return hmi_apis::Common_Language::INVALID_ENUM;
}

uint32_t MessageHelper::GetAppCommandLimit(const std::string& policy_app_id) {
  std::string priority;
  policy::PolicyHandler::instance()->GetPriority(policy_app_id, &priority);
  return policy::PolicyHandler::instance()->GetNotificationsNumber(priority);
}

smart_objects::SmartObjectSPtr MessageHelper::CreateRequestObject() {
  using namespace smart_objects;

  SmartObjectSPtr request(new SmartObject(SmartType_Map));
  if (request) {
    SmartObject& ref = *request;

    ref[strings::params][strings::message_type] =
        static_cast<int>(hmi_apis::messageType::request);
    ref[strings::params][strings::protocol_version] =
        commands::CommandImpl::protocol_version_;
    ref[strings::params][strings::protocol_type] =
        commands::CommandImpl::hmi_protocol_type_;
    ref[strings::params][strings::correlation_id] =
        ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
  }
  return request;
}

smart_objects::SmartObjectSPtr MessageHelper::GetHashUpdateNotification(
    const uint32_t app_id) {
  LOG4CXX_INFO(logger_, "GetHashUpdateNotification" << app_id);
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);

  if (!app) {
    return NULL;
  }
  smart_objects::SmartObjectSPtr message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  (*message)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnHashChangeID;
  (*message)[strings::params][strings::connection_key] = app_id;
  (*message)[strings::params][strings::message_type] =
      static_cast<int32_t>(kNotification);
  return message;
}

smart_objects::SmartObject* MessageHelper::GetLockScreenIconUrlNotification(
    const uint32_t connection_key) {
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key);
  DCHECK(app.get());

  smart_objects::SmartObject* message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  (*message)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnSystemRequestID;
  (*message)[strings::params][strings::connection_key] = connection_key;
  (*message)[strings::params][strings::message_type] =
      mobile_apis::messageType::notification;
  (*message)[strings::params][strings::protocol_type] =
      commands::CommandImpl::mobile_protocol_type_;
  (*message)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;

  (*message)[strings::msg_params][strings::request_type] =
      mobile_apis::RequestType::LOCK_SCREEN_ICON_URL;

  (*message)[strings::msg_params][strings::url] =
      policy::PolicyHandler::instance()->GetLockScreenIconUrl();

  return message;
}
void MessageHelper::SendLockScreenIconUrlNotification(
    const uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject* so =
      GetLockScreenIconUrlNotification(connection_key);
  PrintSmartObject(*so);
  DCHECK(ApplicationManagerImpl::instance()->ManageMobileCommand(so));
}

void MessageHelper::SendHashUpdateNotification(const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObjectSPtr so = GetHashUpdateNotification(app_id);
  if (so) {
    PrintSmartObject(*so);
    if (!ApplicationManagerImpl::instance()->ManageMobileCommand(so)) {
      LOG4CXX_ERROR(logger_, "Failed to send HashUpdate notification.");
    } else {
      ApplicationManagerImpl::instance()
          ->resume_controller()
          .ApplicationsDataUpdated();
    }
  }
}

void MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
    int32_t connection_key,
    mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObjectSPtr notification = new smart_objects::SmartObject;
  DCHECK(notification);
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] = static_cast<int32_t>(
      mobile_api::FunctionID::OnAppInterfaceUnregisteredID);

  message[strings::params][strings::message_type] =
      static_cast<int32_t>(kNotification);

  message[strings::params][strings::connection_key] = connection_key;

  message[strings::msg_params][strings::reason] = static_cast<int32_t>(reason);

  if (ApplicationManagerImpl::instance()->ManageMobileCommand(notification)) {
    LOG4CXX_DEBUG(logger_, "Mobile command sent");
  } else {
    LOG4CXX_WARN(logger_, "Cannot send mobile command");
  }
}

const VehicleData& MessageHelper::vehicle_data() {
  return vehicle_data_;
}

std::string MessageHelper::HMIResultToString(
    hmi_apis::Common_Result::eType hmi_result) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  const char* str = 0;
  if (EnumConversionHelper<hmi_apis::Common_Result::eType>::EnumToCString(
          hmi_result, &str)) {
    return str;
  }
  return std::string();
}

hmi_apis::Common_Result::eType MessageHelper::HMIResultFromString(
    const std::string& hmi_result) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  hmi_apis::Common_Result::eType value;
  if (EnumConversionHelper<hmi_apis::Common_Result::eType>::StringToEnum(
          hmi_result, &value)) {
    return value;
  }
  return hmi_apis::Common_Result::INVALID_ENUM;
}

std::string MessageHelper::MobileResultToString(
    mobile_apis::Result::eType mobile_result) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  const char* str = 0;
  if (EnumConversionHelper<mobile_apis::Result::eType>::EnumToCString(
          mobile_result, &str)) {
    return str;
  }
  return std::string();
}

mobile_apis::Result::eType MessageHelper::MobileResultFromString(
    const std::string& mobile_result) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  mobile_apis::Result::eType value;
  if (EnumConversionHelper<mobile_apis::Result::eType>::StringToEnum(
          mobile_result, &value)) {
    return value;
  }
  return mobile_apis::Result::INVALID_ENUM;
}

mobile_apis::Result::eType MessageHelper::HMIToMobileResult(
    const hmi_apis::Common_Result::eType hmi_result) {
  const std::string result = HMIResultToString(hmi_result);
  if (result.empty()) {
    return mobile_api::Result::INVALID_ENUM;
  }
  return MobileResultFromString(result);
}

hmi_apis::Common_Result::eType MessageHelper::MobileToHMIResult(
    const mobile_apis::Result::eType mobile_result) {
  const std::string result = MobileResultToString(mobile_result);
  if (result.empty()) {
    return hmi_apis::Common_Result::INVALID_ENUM;
  }
  return HMIResultFromString(result);
}

mobile_apis::HMILevel::eType MessageHelper::StringToHMILevel(
    const std::string& hmi_level) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  mobile_apis::HMILevel::eType value;
  if (EnumConversionHelper<mobile_apis::HMILevel::eType>::StringToEnum(
          hmi_level, &value)) {
    return value;
  }
  return mobile_apis::HMILevel::INVALID_ENUM;
}

std::string MessageHelper::StringifiedHMILevel(
    mobile_apis::HMILevel::eType hmi_level) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  const char* str = 0;
  if (EnumConversionHelper<mobile_apis::HMILevel::eType>::EnumToCString(
          hmi_level, &str)) {
    return str;
  }
  return std::string();
}

std::string MessageHelper::StringifiedFunctionID(
    mobile_apis::FunctionID::eType function_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace NsSmartDeviceLink::NsSmartObjects;
  const char* str = 0;
  if (EnumConversionHelper<mobile_apis::FunctionID::eType>::EnumToCString(
          function_id, &str)) {
    const std::string enum_name = str;
    // Strip 'ID' suffix from value name
    DCHECK(enum_name.length() > 2 &&
           enum_name.substr(enum_name.length() - 2) == "ID");
    return enum_name.substr(0, enum_name.length() - 2);
  }
  return std::string();
}

#ifdef HMI_DBUS_API
namespace {
const std::map<std::string, uint16_t> create_get_vehicle_data_args() {
  std::map<std::string, uint16_t> rc;
  rc.insert(std::make_pair(strings::gps,
                           hmi_apis::FunctionID::VehicleInfo_GetGpsData));
  rc.insert(std::make_pair(strings::speed,
                           hmi_apis::FunctionID::VehicleInfo_GetSpeed));
  rc.insert(
      std::make_pair(strings::rpm, hmi_apis::FunctionID::VehicleInfo_GetRpm));
  rc.insert(std::make_pair(strings::fuel_level,
                           hmi_apis::FunctionID::VehicleInfo_GetFuelLevel));
  rc.insert(
      std::make_pair(strings::fuel_level_state,
                     hmi_apis::FunctionID::VehicleInfo_GetFuelLevelState));
  rc.insert(std::make_pair(
      strings::instant_fuel_consumption,
      hmi_apis::FunctionID::VehicleInfo_GetInstantFuelConsumption));
  rc.insert(
      std::make_pair(strings::external_temp,
                     hmi_apis::FunctionID::VehicleInfo_GetExternalTemperature));
  rc.insert(
      std::make_pair(strings::vin, hmi_apis::FunctionID::VehicleInfo_GetVin));
  rc.insert(std::make_pair(strings::prndl,
                           hmi_apis::FunctionID::VehicleInfo_GetPrndl));
  rc.insert(std::make_pair(strings::tire_pressure,
                           hmi_apis::FunctionID::VehicleInfo_GetTirePressure));
  rc.insert(std::make_pair(strings::odometer,
                           hmi_apis::FunctionID::VehicleInfo_GetOdometer));
  rc.insert(std::make_pair(strings::belt_status,
                           hmi_apis::FunctionID::VehicleInfo_GetBeltStatus));
  rc.insert(
      std::make_pair(strings::body_information,
                     hmi_apis::FunctionID::VehicleInfo_GetBodyInformation));
  rc.insert(std::make_pair(strings::device_status,
                           hmi_apis::FunctionID::VehicleInfo_GetDeviceStatus));
  rc.insert(std::make_pair(strings::driver_braking,
                           hmi_apis::FunctionID::VehicleInfo_GetDriverBraking));
  rc.insert(std::make_pair(strings::wiper_status,
                           hmi_apis::FunctionID::VehicleInfo_GetWiperStatus));
  rc.insert(
      std::make_pair(strings::head_lamp_status,
                     hmi_apis::FunctionID::VehicleInfo_GetHeadLampStatus));
  rc.insert(std::make_pair(strings::engine_torque,
                           hmi_apis::FunctionID::VehicleInfo_GetEngineTorque));
  rc.insert(
      std::make_pair(strings::acc_pedal_pos,
                     hmi_apis::FunctionID::VehicleInfo_GetAccPedalPosition));
  rc.insert(
      std::make_pair(strings::steering_wheel_angle,
                     hmi_apis::FunctionID::VehicleInfo_GetSteeringWheelAngle));
  rc.insert(std::make_pair(strings::e_call_info,
                           hmi_apis::FunctionID::VehicleInfo_GetECallInfo));
  rc.insert(std::make_pair(strings::airbag_status,
                           hmi_apis::FunctionID::VehicleInfo_GetAirbagStatus));
  rc.insert(
      std::make_pair(strings::emergency_event,
                     hmi_apis::FunctionID::VehicleInfo_GetEmergencyEvent));
  rc.insert(
      std::make_pair(strings::cluster_mode_status,
                     hmi_apis::FunctionID::VehicleInfo_GetClusterModeStatus));
  rc.insert(std::make_pair(strings::my_key,
                           hmi_apis::FunctionID::VehicleInfo_GetMyKey));
  return rc;
}
static std::map<std::string, uint16_t> vehicle_data_args =
    create_get_vehicle_data_args();
}
#endif

void MessageHelper::CreateGetVehicleDataRequest(
    uint32_t correlation_id, const std::vector<std::string>& params) {
  LOG4CXX_AUTO_TRACE(logger_);
#ifdef HMI_DBUS_API
  for (std::vector<std::string>::const_iterator it = params.begin();
       it != params.end();
       it++) {
    smart_objects::SmartObjectSPtr request = new smart_objects::SmartObject;

    (*request)[strings::params][strings::message_type] =
        static_cast<int>(kRequest);
    (*request)[strings::params][strings::correlation_id] = correlation_id;
    (*request)[strings::params][strings::protocol_version] =
        commands::CommandImpl::protocol_version_;
    (*request)[strings::params][strings::protocol_type] =
        commands::CommandImpl::hmi_protocol_type_;
    (*request)[strings::params][strings::function_id] =
        static_cast<int>(vehicle_data_args[*it]);
    ApplicationManagerImpl::instance()->ManageHMICommand(request);
  }
#else
  smart_objects::SmartObjectSPtr request = new smart_objects::SmartObject;

  (*request)[strings::params][strings::message_type] =
      static_cast<int>(kRequest);
  (*request)[strings::params][strings::function_id] =
      static_cast<int>(hmi_apis::FunctionID::VehicleInfo_GetVehicleData);
  (*request)[strings::params][strings::correlation_id] = correlation_id;
  (*request)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
  (*request)[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
  (*request)[strings::msg_params] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  for (std::vector<std::string>::const_iterator it = params.begin();
       it != params.end();
       it++) {
    (*request)[strings::msg_params][*it] = true;
  }
  ApplicationManagerImpl::instance()->ManageHMICommand(request);
#endif
}

smart_objects::SmartObjectSPtr MessageHelper::CreateBlockedByPoliciesResponse(
    mobile_apis::FunctionID::eType function_id,
    mobile_apis::Result::eType result,
    uint32_t correlation_id,
    uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr response = new smart_objects::SmartObject;

  (*response)[strings::params][strings::function_id] =
      static_cast<int>(function_id);
  (*response)[strings::params][strings::message_type] =
      static_cast<int>(kResponse);
  (*response)[strings::msg_params][strings::success] = false;
  (*response)[strings::msg_params][strings::result_code] =
      static_cast<int>(result);
  (*response)[strings::params][strings::correlation_id] = correlation_id;
  (*response)[strings::params][strings::connection_key] = connection_key;
  (*response)[strings::params][strings::protocol_type] =
      commands::CommandImpl::mobile_protocol_type_;
  (*response)[strings::params][strings::protocol_version] =
      static_cast<int>(kV2);
  return response;
}

smart_objects::SmartObjectSPtr MessageHelper::CreateDeviceListSO(
    const connection_handler::DeviceMap& devices) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr device_list_so =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);

  (*device_list_so)[strings::device_list] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  smart_objects::SmartObject& list_so = (*device_list_so)[strings::device_list];

  int32_t index = 0;
  for (connection_handler::DeviceMap::const_iterator it = devices.begin();
       devices.end() != it;
       ++it) {
    const connection_handler::Device& d =
        static_cast<connection_handler::Device>(it->second);
    list_so[index][strings::name] = d.user_friendly_name();
    list_so[index][strings::id] = it->second.mac_address();

    const policy::DeviceConsent device_consent =
        policy::PolicyHandler::instance()->GetUserConsentForDevice(
            it->second.mac_address());
    list_so[index][strings::isSDLAllowed] =
        policy::DeviceConsent::kDeviceAllowed == device_consent;
    list_so[index][strings::transport_type] =
        ApplicationManagerImpl::instance()->GetDeviceTransportType(
            d.connection_type());
    ++index;
  }
  return device_list_so;
}

smart_objects::SmartObjectSPtr MessageHelper::CreateModuleInfoSO(
    uint32_t function_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr module_info =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject& object = *module_info;
  object[strings::params][strings::message_type] = static_cast<int>(kRequest);
  object[strings::params][strings::function_id] = static_cast<int>(function_id);
  object[strings::params][strings::correlation_id] =
      ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
  object[strings::msg_params] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  return module_info;
}

smart_objects::SmartObjectSPtr MessageHelper::CreateSetAppIcon(
    const std::string& path_to_icon, uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr set_icon =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);

  if (!set_icon) {
    return NULL;
  }

  smart_objects::SmartObject& object = *set_icon;
  object[strings::sync_file_name][strings::value] = path_to_icon;
  // TODO(PV): need to store actual image type
  object[strings::sync_file_name][strings::image_type] =
      static_cast<int>(mobile_api::ImageType::DYNAMIC);
  object[strings::app_id] = app_id;

  return set_icon;
}

bool MessageHelper::SendIVISubscribtions(const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  bool result = true;
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);

  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Invalid application " << app_id);
    return result;
  }

  smart_objects::SmartObjectList requests = GetIVISubscriptionRequests(app);
  for (smart_objects::SmartObjectList::const_iterator it = requests.begin();
       it != requests.end();
       ++it) {
    if (!ApplicationManagerImpl::instance()->ManageHMICommand(*it)) {
      result = false;
    }
  }
  return result;
}

smart_objects::SmartObjectList MessageHelper::GetIVISubscriptionRequests(
    ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObjectList hmi_requests;
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Invalid application pointer ");
    return hmi_requests;
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();
  const VehicleData& vehicle_data = MessageHelper::vehicle_data_;
  VehicleData::const_iterator ivi_it = vehicle_data.begin();
  DataAccessor<VehicleInfoSubscriptions> vi_accessor = app->SubscribedIVI();
  const VehicleInfoSubscriptions& subscriptions = vi_accessor.GetData();

  for (; vehicle_data.end() != ivi_it; ++ivi_it) {
    uint32_t type_id = static_cast<int>(ivi_it->second);
    if (subscriptions.end() != subscriptions.find(type_id)) {
      std::string key_name = ivi_it->first;
      msg_params[key_name] = true;
    }
  }

#ifdef HMI_JSON_API
  smart_objects::SmartObjectSPtr request = MessageHelper::CreateModuleInfoSO(
      hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData);
  (*request)[strings::msg_params] = msg_params;
  hmi_requests.push_back(request);
#endif  // #ifdef HMI_JSON_API
#ifdef HMI_DBUS_API
  // Generate list of ivi_subrequests
  for (size_t i = 0; i < sizeof(ivi_subrequests) / sizeof(ivi_subrequests[0]);
       ++i) {
    const VehicleInfo_Requests& sr = ivi_subrequests[i];
    if (true == msg_params.keyExists(sr.str) &&
        true == msg_params[sr.str].asBool()) {
      smart_objects::SmartObjectSPtr request =
          MessageHelper::CreateModuleInfoSO(sr.func_id);
      (*request)[strings::msg_params] = msg_params;
      hmi_requests.push_back(request);
    }
  }
#endif  // #ifdef HMI_DBUS_API
  return hmi_requests;
}

void MessageHelper::SendOnButtonSubscriptionNotification(
    const uint32_t app_id,
    const hmi_apis::Common_ButtonName::eType button,
    const bool is_subscribed) {
  using namespace smart_objects;
  using namespace hmi_apis;
  LOG4CXX_AUTO_TRACE(logger_);

  SmartObjectSPtr notification_ptr =
      utils::MakeShared<SmartObject>(SmartType_Map);
  if (!notification_ptr) {
    LOG4CXX_ERROR(logger_, "Memory allocation failed.");
    return;
  }
  SmartObject& notification = *notification_ptr;

  SmartObject msg_params = SmartObject(SmartType_Map);
  msg_params[strings::app_id] = app_id;
  msg_params[strings::name] = button;
  msg_params[strings::is_suscribed] = is_subscribed;

  notification[strings::params][strings::message_type] =
      static_cast<int32_t>(application_manager::MessageType::kNotification);
  notification[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
  notification[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
  notification[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Buttons_OnButtonSubscription;
  notification[strings::msg_params] = msg_params;

  if (!ApplicationManagerImpl::instance()->ManageHMICommand(notification_ptr)) {
    LOG4CXX_ERROR(logger_, "Unable to send HMI notification");
  }
}

void MessageHelper::SendAllOnButtonSubscriptionNotificationsForApp(
    ApplicationConstSharedPtr app) {
  using namespace smart_objects;
  using namespace hmi_apis;
  using namespace mobile_apis;
  LOG4CXX_AUTO_TRACE(logger_);

  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Invalid application pointer ");
    return;
  }

  DataAccessor<ButtonSubscriptions> button_accessor = app->SubscribedButtons();
  ButtonSubscriptions subscriptions = button_accessor.GetData();
  ButtonSubscriptions::iterator it = subscriptions.begin();
  for (; subscriptions.end() != it; ++it) {
    SendOnButtonSubscriptionNotification(
        app->hmi_app_id(), static_cast<Common_ButtonName::eType>(*it), true);
  }
}

void MessageHelper::SendSetAppIcon(const uint32_t app_id,
                                   const std::string& icon_path) {
  using namespace smart_objects;
  SmartObjectSPtr set_app_icon = CreateRequestObject();
  if (set_app_icon) {
    SmartObject& so_to_send = *set_app_icon;
    so_to_send[strings::params][strings::function_id] =
        static_cast<int>(hmi_apis::FunctionID::UI_SetAppIcon);

    so_to_send[strings::msg_params] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    SmartObjectSPtr msg_params(
        MessageHelper::CreateSetAppIcon(icon_path, app_id));

    if (msg_params) {
      so_to_send[strings::msg_params] = *msg_params;
    }
    ApplicationManagerImpl::instance()->ManageHMICommand(set_app_icon);
  }
}

void MessageHelper::SendAppDataToHMI(ApplicationConstSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (app) {
    SendSetAppIcon(app, app->app_icon_path());
    SendGlobalPropertiesToHMI(app);
    SendShowRequestToHMI(app);
  }
}

void MessageHelper::SendGlobalPropertiesToHMI(ApplicationConstSharedPtr app) {
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Invalid application");
    return;
  }

  smart_objects::SmartObjectList requests =
      CreateGlobalPropertiesRequestsToHMI(app);
  for (smart_objects::SmartObjectList::const_iterator it = requests.begin();
       it != requests.end();
       ++it) {
    DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(*it));
  }
}

smart_objects::SmartObjectList
MessageHelper::CreateGlobalPropertiesRequestsToHMI(
    ApplicationConstSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObjectList requests;
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Invalid application");
    return requests;
  }

  // UI global properties

  if (app->vr_help_title() || app->vr_help()) {
    smart_objects::SmartObjectSPtr ui_global_properties = CreateRequestObject();
    if (!ui_global_properties) {
      return requests;
    }

    (*ui_global_properties)[strings::params][strings::function_id] =
        static_cast<int>(hmi_apis::FunctionID::UI_SetGlobalProperties);

    smart_objects::SmartObject ui_msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    if (app->vr_help_title()) {
      ui_msg_params[strings::vr_help_title] = (*app->vr_help_title());
    }
    if (app->vr_help()) {
      ui_msg_params[strings::vr_help] = (*app->vr_help());
    }
    if (app->keyboard_props()) {
      ui_msg_params[strings::keyboard_properties] = (*app->keyboard_props());
    }
    if (app->menu_title()) {
      ui_msg_params[strings::menu_title] = (*app->menu_title());
    }
    if (app->menu_icon()) {
      ui_msg_params[strings::menu_icon] = (*app->menu_icon());
    }
    ui_msg_params[strings::app_id] = app->app_id();

    (*ui_global_properties)[strings::msg_params] = ui_msg_params;

    requests.push_back(ui_global_properties);
  }

  // TTS global properties
  if (app->help_prompt() || app->timeout_prompt()) {
    smart_objects::SmartObjectSPtr tts_global_properties =
        CreateRequestObject();
    if (!tts_global_properties) {
      return requests;
    }

    (*tts_global_properties)[strings::params][strings::function_id] =
        static_cast<int>(hmi_apis::FunctionID::TTS_SetGlobalProperties);

    smart_objects::SmartObject tts_msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    if (app->help_prompt()) {
      tts_msg_params[strings::help_prompt] = (*app->help_prompt());
    }
    if (app->timeout_prompt()) {
      tts_msg_params[strings::timeout_prompt] = (*app->timeout_prompt());
    }
    tts_msg_params[strings::app_id] = app->app_id();

    (*tts_global_properties)[strings::msg_params] = tts_msg_params;

    requests.push_back(tts_global_properties);
  }
  return requests;
}

void MessageHelper::SendTTSGlobalProperties(ApplicationSharedPtr app,
                                            const bool default_help_prompt) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!app) {
    return;
  }
  smart_objects::SmartObjectSPtr tts_global_properties = CreateRequestObject();
  if (tts_global_properties) {
    smart_objects::SmartObject& so_to_send = *tts_global_properties;
    so_to_send[strings::params][strings::function_id] =
        static_cast<int>(hmi_apis::FunctionID::TTS_SetGlobalProperties);

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[strings::help_prompt] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    if (default_help_prompt) {
      const DataAccessor<CommandsMap> accessor = app->commands_map();
      const CommandsMap& commands = accessor.GetData();
      CommandsMap::const_iterator it = commands.begin();
      uint32_t index = 0;
      for (; commands.end() != it; ++it) {
        smart_objects::SmartObject item(smart_objects::SmartType_Map);
        if ((*it->second).keyExists(strings::menu_params)) {
          item[strings::text] =
              (*it->second)[strings::menu_params][strings::menu_name]
                  .asString();
          item[strings::type] = mobile_apis::SpeechCapabilities::SC_TEXT;
        } else {
          continue;
        }
        msg_params[strings::help_prompt][index++] = item;
      }
    }
    app->set_help_prompt(msg_params[strings::help_prompt]);
    msg_params[strings::app_id] = app->app_id();
    so_to_send[strings::msg_params] = msg_params;
    ApplicationManagerImpl::instance()->ManageHMICommand(tts_global_properties);
  }
}

smart_objects::SmartObjectSPtr MessageHelper::CreateAppVrHelp(
    ApplicationConstSharedPtr app) {
  smart_objects::SmartObjectSPtr result =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (!result) {
    return NULL;
  }
  smart_objects::SmartObject& vr_help = *result;
  vr_help[strings::vr_help_title] = app->name();

  int32_t index = 0;
  if (app->vr_synonyms()) {
    smart_objects::SmartObject item(smart_objects::SmartType_Map);
    item[strings::text] = (*(app->vr_synonyms())).getElement(0);
    item[strings::position] = index + 1;
    vr_help[strings::vr_help][index++] = item;
  }

  // copy all app VR commands
  const DataAccessor<CommandsMap> cmd_accessor = app->commands_map();
  const CommandsMap& commands = cmd_accessor.GetData();
  CommandsMap::const_iterator it = commands.begin();

  for (; commands.end() != it; ++it) {
    smart_objects::SmartObject item(smart_objects::SmartType_Map);
    item[strings::text] = (*it->second)[strings::vr_commands][0].asString();
    item[strings::position] = index + 1;
    vr_help[strings::vr_help][index++] = item;
  }
  return result;
}

smart_objects::SmartObjectList MessageHelper::CreateShowRequestToHMI(
    ApplicationConstSharedPtr app) {
  smart_objects::SmartObjectList requests;
  if (!app) {
    LOG4CXX_ERROR(logger_, "Invalid application");
    return requests;
  }

  if (app->show_command()) {
    smart_objects::SmartObjectSPtr ui_show = CreateRequestObject();
    (*ui_show)[strings::params][strings::function_id] =
        static_cast<int>(hmi_apis::FunctionID::UI_Show);

    (*ui_show)[strings::msg_params] = (*app->show_command());
    requests.push_back(ui_show);
  }
  return requests;
}

void MessageHelper::SendShowRequestToHMI(ApplicationConstSharedPtr app) {
  if (!app) {
    return;
  }
  smart_objects::SmartObjectList shows = CreateShowRequestToHMI(app);

  for (smart_objects::SmartObjectList::const_iterator it = shows.begin();
       it != shows.end();
       ++it) {
    DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(*it));
  }
}

void MessageHelper::SendShowConstantTBTRequestToHMI(
    ApplicationConstSharedPtr app) {
  if (!app) {
    return;
  }

  if (app->tbt_show_command()) {
    smart_objects::SmartObjectSPtr navi_show_tbt = CreateRequestObject();
    if (!navi_show_tbt) {
      return;
    }
    (*navi_show_tbt)[strings::params][strings::function_id] =
        static_cast<int>(hmi_apis::FunctionID::Navigation_ShowConstantTBT);

    (*navi_show_tbt)[strings::msg_params] = (*app->tbt_show_command());
    DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(navi_show_tbt));
  }
}

void MessageHelper::SendAddCommandRequestToHMI(ApplicationConstSharedPtr app) {
  if (!app) {
    return;
  }
  smart_objects::SmartObjectList requests = CreateAddCommandRequestToHMI(app);
  for (smart_objects::SmartObjectList::iterator it = requests.begin();
       it != requests.end();
       ++it) {
    DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(*it));
  }
}

smart_objects::SmartObjectList MessageHelper::CreateAddCommandRequestToHMI(
    ApplicationConstSharedPtr app) {
  smart_objects::SmartObjectList requests;
  if (!app) {
    LOG4CXX_ERROR(logger_, "Invalid application");
    return requests;
  }

  const DataAccessor<CommandsMap> accessor = app->commands_map();
  const CommandsMap& commands = accessor.GetData();
  CommandsMap::const_iterator i = commands.begin();
  for (; commands.end() != i; ++i) {
    // UI Interface
    if ((*i->second).keyExists(strings::menu_params)) {
      smart_objects::SmartObjectSPtr ui_command = CreateRequestObject();
      if (!ui_command) {
        return requests;
      }

      (*ui_command)[strings::params][strings::function_id] =
          static_cast<int>(hmi_apis::FunctionID::UI_AddCommand);

      smart_objects::SmartObject msg_params =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
      msg_params[strings::cmd_id] = i->first;
      msg_params[strings::menu_params] = (*i->second)[strings::menu_params];
      msg_params[strings::app_id] = app->app_id();

      if (((*i->second).keyExists(strings::cmd_icon)) &&
          (0 < (*i->second)[strings::cmd_icon][strings::value].length())) {
        msg_params[strings::cmd_icon] = (*i->second)[strings::cmd_icon];
        msg_params[strings::cmd_icon][strings::value] =
            (*i->second)[strings::cmd_icon][strings::value].asString();
      }
      (*ui_command)[strings::msg_params] = msg_params;
      requests.push_back(ui_command);
    }

    // VR Interface
    if ((*i->second).keyExists(strings::vr_commands)) {
      SendAddVRCommandToHMI(
          i->first, (*i->second)[strings::vr_commands], app->app_id());
    }
  }
  return requests;
}

smart_objects::SmartObjectList
MessageHelper::CreateAddVRCommandRequestFromChoiceToHMI(
    ApplicationConstSharedPtr app) {
  smart_objects::SmartObjectList requests;
  if (!app) {
    LOG4CXX_ERROR(logger_, "Invalid application");
    return requests;
  }

  const DataAccessor<ChoiceSetMap> accessor = app->choice_set_map();
  const ChoiceSetMap& choices = accessor.GetData();
  ChoiceSetMap::const_iterator it = choices.begin();
  for (; choices.end() != it; ++it) {
    const uint32_t choice_grammar_id =
        (*(it->second))[strings::grammar_id].asUInt();
    const size_t size = (*(it->second))[strings::choice_set].length();
    for (size_t j = 0; j < size; ++j) {
      smart_objects::SmartObjectSPtr vr_command = CreateRequestObject();
      if (!vr_command) {
        return requests;
      }

      (*vr_command)[strings::params][strings::function_id] =
          static_cast<int>(hmi_apis::FunctionID::VR_AddCommand);

      smart_objects::SmartObject msg_params =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
      msg_params[strings::app_id] = app->app_id();
      msg_params[strings::cmd_id] =
          (*(it->second))[strings::choice_set][j][strings::choice_id];
      msg_params[strings::vr_commands] =
          smart_objects::SmartObject(smart_objects::SmartType_Array);
      msg_params[strings::vr_commands] =
          (*(it->second))[strings::choice_set][j][strings::vr_commands];
      msg_params[strings::type] = hmi_apis::Common_VRCommandType::Choice;
      msg_params[strings::grammar_id] = choice_grammar_id;

      (*vr_command)[strings::msg_params] = msg_params;
      requests.push_back(vr_command);
    }
  }
  return requests;
}

smart_objects::SmartObjectSPtr MessageHelper::CreateChangeRegistration(
    const int32_t function_id,
    const int32_t language,
    const uint32_t app_id,
    const smart_objects::SmartObject* app_types) {
  smart_objects::SmartObjectSPtr command = CreateRequestObject();
  if (!command) {
    return NULL;
  }
  smart_objects::SmartObject& params = *command;
  params[strings::params][strings::function_id] = function_id;

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::language] = language;
  msg_params[strings::app_id] = app_id;

  if (app_types != NULL) {
    msg_params[strings::app_hmi_type] = *app_types;
  }

  params[strings::msg_params] = msg_params;
  return command;
}

void MessageHelper::SendUIChangeRegistrationRequestToHMI(
    ApplicationConstSharedPtr app) {
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Application is not valid");
    return;
  }

  if (NULL != app->app_types()) {
    smart_objects::SmartObjectSPtr ui_command =
        CreateChangeRegistration(hmi_apis::FunctionID::UI_ChangeRegistration,
                                 app->ui_language(),
                                 app->app_id(),
                                 app->app_types());

    if (ui_command) {
      ApplicationManagerImpl::instance()->ManageHMICommand(ui_command);
    }
  }
}

void MessageHelper::SendChangeRegistrationRequestToHMI(
    ApplicationConstSharedPtr app) {
  if (!app.valid()) {
    return;
  }
  if (mobile_apis::Language::INVALID_ENUM != app->language()) {
    smart_objects::SmartObjectSPtr vr_command =
        CreateChangeRegistration(hmi_apis::FunctionID::VR_ChangeRegistration,
                                 app->language(),
                                 app->app_id());

    if (vr_command) {
      ApplicationManagerImpl::instance()->ManageHMICommand(vr_command);
    }
  }

  if (mobile_apis::Language::INVALID_ENUM != app->language()) {
    smart_objects::SmartObjectSPtr tts_command =
        CreateChangeRegistration(hmi_apis::FunctionID::TTS_ChangeRegistration,
                                 app->language(),
                                 app->app_id());

    if (tts_command) {
      ApplicationManagerImpl::instance()->ManageHMICommand(tts_command);
    }
  }

  if (mobile_apis::Language::INVALID_ENUM != app->ui_language()) {
    smart_objects::SmartObjectSPtr ui_command =
        CreateChangeRegistration(hmi_apis::FunctionID::UI_ChangeRegistration,
                                 app->ui_language(),
                                 app->app_id());

    if (ui_command) {
      ApplicationManagerImpl::instance()->ManageHMICommand(ui_command);
    }
  }
}

void MessageHelper::SendAddVRCommandToHMI(
    const uint32_t cmd_id,
    const smart_objects::SmartObject& vr_commands,
    const uint32_t app_id) {
  smart_objects::SmartObjectSPtr request =
      CreateAddVRCommandToHMI(cmd_id, vr_commands, app_id);
  DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(request));
}

smart_objects::SmartObjectSPtr MessageHelper::CreateAddVRCommandToHMI(
    const uint32_t cmd_id,
    const smart_objects::SmartObject& vr_commands,
    const uint32_t app_id) {
  smart_objects::SmartObjectSPtr vr_command = CreateRequestObject();
  if (!vr_command) {
    return NULL;
  }

  (*vr_command)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::VR_AddCommand;

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (0 != cmd_id) {
    msg_params[strings::cmd_id] = cmd_id;
  }
  msg_params[strings::vr_commands] = vr_commands;
  if (0 < app_id) {
    msg_params[strings::app_id] = app_id;
  }
  msg_params[strings::grammar_id] =
      ApplicationManagerImpl::instance()->application(app_id)->get_grammar_id();
  msg_params[strings::type] = hmi_apis::Common_VRCommandType::Command;

  (*vr_command)[strings::msg_params] = msg_params;

  return vr_command;
}

bool MessageHelper::CreateHMIApplicationStruct(
      ApplicationConstSharedPtr app,
      const protocol_handler::SessionObserver& session_observer,
      NsSmartDeviceLink::NsSmartObjects::SmartObject* output) {
  LOG4CXX_AUTO_TRACE(logger_);
  using smart_objects::SmartObject;

  DCHECK(output);
  SmartObject& message = *output;
  if (!app) {
    LOG4CXX_WARN(logger_, "Application is not valid");
    return false;
  }

  const SmartObject* app_types = app->app_types();
  const SmartObject* ngn_media_screen_name = app->ngn_media_screen_name();
  const connection_handler::DeviceHandle handle = app->device();
  std::string device_name =
      ApplicationManagerImpl::instance()->GetDeviceName(handle);
  std::string mac_address;
  std::string transport_type;
  if (-1 ==
      session_observer.GetDataOnDeviceID(
          app->device(), &device_name, NULL, &mac_address, &transport_type)) {
    LOG4CXX_ERROR(logger_,
                  "Failed to extract information for device " << app->device());
  }

  message = SmartObject(smart_objects::SmartType_Map);
  message[strings::app_name] = app->name();
  message[strings::app_id] = app->hmi_app_id();
  const std::string icon_path = app->app_icon_path();
  if (!icon_path.empty()) {
    message[strings::icon] = icon_path;
  }
  if (app->IsRegistered()) {
    message[strings::hmi_display_language_desired] = app->ui_language();
    message[strings::is_media_application] = app->is_media_application();
  } else {
    message[strings::greyOut] = app->is_greyed_out();
  }
  if (app->tts_name() && !app->tts_name()->empty()) {
    message[json::ttsName] = *(app->tts_name());
  }
  if (app->vr_synonyms() && !app->vr_synonyms()->empty()) {
    message[json::vrSynonyms] = *(app->vr_synonyms());
  }
  if (ngn_media_screen_name) {
    message[strings::ngn_media_screen_app_name] =
        ngn_media_screen_name->asString();
  }
  if (app_types) {
    message[strings::app_type] = *app_types;
  }

  message[strings::device_info] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  message[strings::device_info][strings::name] = device_name;
  message[strings::device_info][strings::id] = mac_address;
  const policy::DeviceConsent device_consent =
      policy::PolicyHandler::instance()->GetUserConsentForDevice(mac_address);
  message[strings::device_info][strings::isSDLAllowed] =
      policy::DeviceConsent::kDeviceAllowed == device_consent;

  message[strings::device_info][strings::transport_type] =
      ApplicationManagerImpl::instance()->GetDeviceTransportType(
          transport_type);
  return true;
}

void MessageHelper::SendAddSubMenuRequestToHMI(ApplicationConstSharedPtr app) {
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Invalid application");
    return;
  }

  smart_objects::SmartObjectList requests = CreateAddSubMenuRequestToHMI(app);
  for (smart_objects::SmartObjectList::iterator it = requests.begin();
       it != requests.end();
       ++it) {
    DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(*it));
  }
}

smart_objects::SmartObjectList MessageHelper::CreateAddSubMenuRequestToHMI(
    ApplicationConstSharedPtr app) {
  smart_objects::SmartObjectList requsets;
  const DataAccessor<SubMenuMap> accessor = app->sub_menu_map();
  const SubMenuMap& sub_menu = accessor.GetData();
  SubMenuMap::const_iterator i = sub_menu.begin();
  for (; sub_menu.end() != i; ++i) {
    smart_objects::SmartObjectSPtr ui_sub_menu = CreateRequestObject();
    if (!ui_sub_menu) {
      return requsets;
    }

    (*ui_sub_menu)[strings::params][strings::function_id] =
        hmi_apis::FunctionID::UI_AddSubMenu;

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    msg_params[strings::menu_id] = i->first;
    msg_params[strings::menu_params][strings::position] =
        (*i->second)[strings::position];
    msg_params[strings::menu_params][strings::menu_name] =
        (*i->second)[strings::menu_name];
    msg_params[strings::app_id] = app->app_id();
    (*ui_sub_menu)[strings::msg_params] = msg_params;
    requsets.push_back(ui_sub_menu);
  }
  return requsets;
}

void MessageHelper::SendOnAppUnregNotificationToHMI(
    ApplicationConstSharedPtr app, const bool is_unexpected_disconnect) {
  smart_objects::SmartObjectSPtr notification =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (!notification) {
    return;
  }
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered;

  message[strings::params][strings::message_type] = MessageType::kNotification;
  // we put hmi_app_id because applicaton list does not contain application on
  // this momment
  // and ReplaceHMIByMobileAppId function will be unable to replace app_id to
  // hmi_app_id
  message[strings::msg_params][strings::app_id] = app->hmi_app_id();
  message[strings::msg_params][strings::unexpected_disconnect] =
      is_unexpected_disconnect;
  ApplicationManagerImpl::instance()->ManageHMICommand(notification);
}

smart_objects::SmartObjectSPtr
MessageHelper::GetBCActivateAppRequestToHMI(
      ApplicationConstSharedPtr app,
     const hmi_apis::Common_HMILevel::eType level,
     const bool send_policy_priority) {
  DCHECK_OR_RETURN(app, smart_objects::SmartObjectSPtr());

  const uint32_t correlation_id =
      ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
  smart_objects::SmartObjectSPtr message =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_ActivateApp;
  (*message)[strings::params][strings::message_type] = MessageType::kRequest;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::msg_params][strings::app_id] = app->app_id();

  if (send_policy_priority) {
    std::string priority;
    // TODO(KKolodiy): need remove method policy_manager

    policy::PolicyHandler::instance()->GetPriority(app->mobile_app_id(),
                                                   &priority);
    // According SDLAQ-CRS-2794
    // SDL have to send ActivateApp without "proirity" parameter to HMI.
    // in case of unconsented device
    const std::string& mac_adress = app->mac_address();

    policy::DeviceConsent consent =
        policy::PolicyHandler::instance()->GetUserConsentForDevice(mac_adress);
    if (!priority.empty() &&
        (policy::DeviceConsent::kDeviceAllowed == consent)) {
      (*message)[strings::msg_params][strings::priority] =
          GetPriorityCode(priority);
    }
  }

  // We haven't send HMI level to HMI in case it FULL.
  if (hmi_apis::Common_HMILevel::INVALID_ENUM != level &&
      hmi_apis::Common_HMILevel::FULL != level) {
    (*message)[strings::msg_params][strings::activate_app_hmi_level] = level;
  }
  return message;
}

void MessageHelper::SendOnResumeAudioSourceToHMI(const uint32_t app_id) {
  LOG4CXX_WARN(logger_, "SendOnResumeAudioSourceToHMI app_id: " << app_id);
  application_manager::ApplicationConstSharedPtr app =
      application_manager::ApplicationManagerImpl::instance()->application(
          app_id);
  if (!app) {
    LOG4CXX_WARN(logger_, "Invalid app_id: " << app_id);
    return;
  }

  utils::SharedPtr<smart_objects::SmartObject> message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnResumeAudioSource;
  (*message)[strings::params][strings::message_type] =
      MessageType::kNotification;
  (*message)[strings::params][strings::correlation_id] =
      ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
  (*message)[strings::msg_params][strings::app_id] = app_id;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendSDLActivateAppResponse(
    policy::AppPermissions& permissions, const uint32_t correlation_id) {
  smart_objects::SmartObjectSPtr message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_ActivateApp;
  (*message)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
  (*message)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
  (*message)[strings::params][hmi_response::code] = 0;

  (*message)[strings::msg_params]["isSDLAllowed"] = permissions.isSDLAllowed;
  if (!permissions.isSDLAllowed) {
    (*message)[strings::msg_params]["device"]["name"] =
        permissions.deviceInfo.device_name;
    (*message)[strings::msg_params]["device"]["id"] =
        permissions.deviceInfo.device_mac_address;
  }

  (*message)[strings::msg_params]["isAppRevoked"] = permissions.appRevoked;
  (*message)[strings::msg_params]["isAppPermissionsRevoked"] =
      permissions.isAppPermissionsRevoked;

  if (permissions.isAppPermissionsRevoked) {
    FillAppRevokedPermissions(permissions, *message);
  }

  (*message)[strings::msg_params]["isPermissionsConsentNeeded"] =
      permissions.appPermissionsConsentNeeded;

  if (!permissions.priority.empty()) {
    (*message)[strings::msg_params]["priority"] =
        GetPriorityCode(permissions.priority);
  }

  ApplicationManagerImpl::instance()->ManageHMICommand(message);

  // If application is revoked it should not be activated
  if (permissions.appRevoked || !permissions.isSDLAllowed) {
    return;
  }
}

void MessageHelper::SendOnSDLConsentNeeded(
    const policy::DeviceParams& device_info) {
  smart_objects::SmartObjectSPtr message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_OnSDLConsentNeeded;
  (*message)[strings::params][strings::message_type] =
      MessageType::kNotification;

  (*message)[strings::msg_params]["device"]["id"] =
      device_info.device_mac_address;
  (*message)[strings::msg_params]["device"]["name"] = device_info.device_name;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendPolicyUpdate(const std::string& file_path,
                                     const int timeout,
                                     const std::vector<int>& retries) {
  smart_objects::SmartObjectSPtr message = CreateRequestObject();
  smart_objects::SmartObject& object = *message;
  object[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_PolicyUpdate;

  object[strings::msg_params][hmi_request::file] = file_path;
  object[strings::msg_params][strings::timeout] = timeout;
  object[strings::msg_params][hmi_request::retry] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  for (size_t i = 0; i < retries.size(); ++i) {
    object[strings::msg_params][hmi_request::retry][i] = retries[i];
  }
  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendGetUserFriendlyMessageResponse(
    const std::vector<policy::UserFriendlyMessage>& msg,
    const uint32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_GetUserFriendlyMessage;
  (*message)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::params][hmi_response::code] = 0;

  // If no any messages found - skip sending of "messages" param
  if (msg.empty()) {
    ApplicationManagerImpl::instance()->ManageHMICommand(message);
    return;
  }

  const std::string messages = "messages";
  (*message)[strings::msg_params][messages] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  smart_objects::SmartObject& user_friendly_messages =
      (*message)[strings::msg_params][messages];

  const std::string message_code = "messageCode";

  std::vector<policy::UserFriendlyMessage>::const_iterator it = msg.begin();
  std::vector<policy::UserFriendlyMessage>::const_iterator it_end = msg.end();
  for (uint32_t index = 0; it != it_end; ++it, ++index) {
    user_friendly_messages[index] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    smart_objects::SmartObject& obj = user_friendly_messages[index];
    obj[message_code] = it->message_code;
  }

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendGetListOfPermissionsResponse(
    const std::vector<policy::FunctionalGroupPermission>& permissions,
    const uint32_t correlation_id) {
  smart_objects::SmartObjectSPtr message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_GetListOfPermissions;
  (*message)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::params][hmi_response::code] = 0;

  const std::string allowed_functions = "allowedFunctions";
  (*message)[strings::msg_params][allowed_functions] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  smart_objects::SmartObject& allowed_functions_array =
      (*message)[strings::msg_params][allowed_functions];

  std::vector<policy::FunctionalGroupPermission>::const_iterator it =
      permissions.begin();
  std::vector<policy::FunctionalGroupPermission>::const_iterator it_end =
      permissions.end();
  for (uint32_t index = 0; it != it_end; ++it, ++index) {
    allowed_functions_array[index] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    smart_objects::SmartObject& item = allowed_functions_array[index];
    item[strings::name] = (*it).group_alias;
    item[strings::id] = (*it).group_id;
    policy::GroupConsent permission_state = (*it).state;
    // If state undefined, 'allowed' parameter should be absent
    if (policy::kGroupUndefined != permission_state) {
      item["allowed"] = policy::kGroupAllowed == permission_state;
    }
  }

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

smart_objects::SmartObjectSPtr MessageHelper::CreateNegativeResponse(
    uint32_t connection_key,
    int32_t function_id,
    uint32_t correlation_id,
    int32_t result_code) {
  smart_objects::SmartObjectSPtr response =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject& response_data = *response;
  response_data[strings::params][strings::function_id] = function_id;
  response_data[strings::params][strings::message_type] =
      mobile_apis::messageType::response;
  response_data[strings::params][strings::correlation_id] = correlation_id;
  response_data[strings::params][strings::protocol_type] =
      commands::CommandImpl::mobile_protocol_type_;
  response_data[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
  response_data[strings::msg_params][strings::result_code] = result_code;
  response_data[strings::msg_params][strings::success] = false;
  response_data[strings::params][strings::connection_key] = connection_key;

  return response;
}

void MessageHelper::SendNaviStartStream(const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr start_stream = CreateRequestObject();
  if (!start_stream) {
    return;
  }

  (*start_stream)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Navigation_StartStream;

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  char url[100] = {'\0'};
  if ("socket" == profile::Profile::instance()->video_server_type()) {
    snprintf(url,
             sizeof(url) / sizeof(url[0]),
             "http://%s:%d",
             profile::Profile::instance()->server_address().c_str(),
             profile::Profile::instance()->video_streaming_port());
  } else if ("pipe" == profile::Profile::instance()->video_server_type()) {
    snprintf(url,
             sizeof(url) / sizeof(url[0]),
             "%s",
             profile::Profile::instance()->named_video_pipe_path().c_str());
  } else {
    int snprintf_result;
    snprintf_result =
        snprintf(url,
                 sizeof(url) / sizeof(url[0]),
                 "%s",
                 profile::Profile::instance()->video_stream_file().c_str());
    DCHECK(snprintf_result);
  }
  msg_params[strings::app_id] = app_id;
  msg_params[strings::url] = url;

  (*start_stream)[strings::msg_params] = msg_params;

  ApplicationManagerImpl::instance()->ManageHMICommand(start_stream);
}

void MessageHelper::SendNaviStopStream(const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr stop_stream = CreateRequestObject();
  if (!stop_stream) {
    return;
  }

  (*stop_stream)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Navigation_StopStream;

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::app_id] = app_id;

  (*stop_stream)[strings::msg_params] = msg_params;

  ApplicationManagerImpl::instance()->ManageHMICommand(stop_stream);
}

void MessageHelper::SendAudioStartStream(const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr start_stream = CreateRequestObject();
  if (!start_stream) {
    return;
  }

  (*start_stream)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Navigation_StartAudioStream;

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  char url[100] = {'\0'};
  if ("socket" == profile::Profile::instance()->audio_server_type()) {
    snprintf(url,
             sizeof(url) / sizeof(url[0]),
             "http://%s:%d",
             profile::Profile::instance()->server_address().c_str(),
             profile::Profile::instance()->audio_streaming_port());
  } else if ("pipe" == profile::Profile::instance()->audio_server_type()) {
    snprintf(url,
             sizeof(url) / sizeof(url[0]),
             "%s",
             profile::Profile::instance()->named_audio_pipe_path().c_str());
  } else {
    int snprintf_result;
    snprintf_result =
        snprintf(url,
                 sizeof(url) / sizeof(url[0]),
                 "%s",
                 profile::Profile::instance()->audio_stream_file().c_str());
    DCHECK(snprintf_result);
  }

  msg_params[strings::app_id] = app_id;
  msg_params[strings::url] = url;

  (*start_stream)[strings::msg_params] = msg_params;

  DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(start_stream));
}

void MessageHelper::SendAudioStopStream(const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr stop_stream = CreateRequestObject();
  if (!stop_stream) {
    return;
  }

  (*stop_stream)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Navigation_StopAudioStream;

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::app_id] = app_id;

  (*stop_stream)[strings::msg_params] = msg_params;

  ApplicationManagerImpl::instance()->ManageHMICommand(stop_stream);
}

void MessageHelper::SendOnDataStreaming(const protocol_handler::ServiceType service,
                                        const bool available) {
  using namespace protocol_handler;
  smart_objects::SmartObjectSPtr notification =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);

  if (!notification) {
    return;
  }

  if (ServiceType::kAudio != service && ServiceType::kMobileNav != service) {
    return;
  }

  (*notification)[strings::params][strings::function_id] =
      ServiceType::kAudio == service
          ? hmi_apis::FunctionID::Navigation_OnAudioDataStreaming
          : hmi_apis::FunctionID::Navigation_OnVideoDataStreaming;
  (*notification)[strings::params][strings::message_type] =
      hmi_apis::messageType::notification;
  (*notification)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
  (*notification)[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;

  (*notification)[strings::msg_params]["available"] = available;

  ApplicationManagerImpl::instance()->ManageHMICommand(notification);
}

bool MessageHelper::SendStopAudioPathThru() {
  LOG4CXX_INFO(logger_, "MessageHelper::SendAudioStopAudioPathThru");

  smart_objects::SmartObjectSPtr result = CreateRequestObject();

  smart_objects::SmartObject& request = *result;
  request[strings::params][strings::function_id] =
      hmi_apis::FunctionID::UI_EndAudioPassThru;

  return ApplicationManagerImpl::instance()->ManageHMICommand(result);
}

void MessageHelper::SendPolicySnapshotNotification(
    const unsigned int connection_key,
    const std::vector<uint8_t>& policy_data,
    const std::string& url) {
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key);
  DCHECK(app.get());

  smart_objects::SmartObject* content =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);  // AKirov: possible memory leak here

  if (!url.empty()) {
    (*content)[strings::msg_params][strings::url] = url;  // Doesn't work with mobile_notification::syncp_url ("URL")
  } else {
    LOG4CXX_WARN(logger_, "No service URLs");
  }

  (*content)[strings::msg_params][strings::request_type] =
      mobile_apis::RequestType::PROPRIETARY;
  (*content)[strings::params][strings::binary_data] =
      smart_objects::SmartObject(policy_data);
  (*content)[strings::msg_params][strings::file_type] =
      mobile_apis::FileType::BINARY;

  SendSystemRequestNotification(connection_key, content);
}

void MessageHelper::SendSystemRequestNotification(
    const uint32_t connection_key, smart_objects::SmartObject*& content) {
  using namespace mobile_apis;
  using namespace commands;
  (*content)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnSystemRequestID;
  (*content)[strings::params][strings::message_type] =
      mobile_apis::messageType::notification;
  (*content)[strings::params][strings::protocol_type] =
      commands::CommandImpl::mobile_protocol_type_;
  (*content)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;

  (*content)[strings::params][strings::connection_key] = connection_key;

  smart_objects::SmartObject* so = new smart_objects::SmartObject(*content);
#ifdef DEBUG
  PrintSmartObject(*so);
#endif

  DCHECK(ApplicationManagerImpl::instance()->ManageMobileCommand(so));
}

void MessageHelper::SendLaunchApp(const uint32_t connection_key,
                                  const std::string& urlSchema,
                                  const std::string& packageName) {
  using namespace mobile_apis;
  using namespace smart_objects;

  SmartObject* content = new SmartObject(SmartType_Map);
  (*content)[strings::msg_params][strings::request_type] =
      RequestType::LAUNCH_APP;
  (*content)[strings::msg_params][strings::app_id] = connection_key;
  if (!urlSchema.empty()) {
    (*content)[strings::msg_params][strings::url] = urlSchema;
  } else if (!packageName.empty()) {
    (*content)[strings::msg_params][strings::url] = packageName;
  }

  SendSystemRequestNotification(connection_key, content);
}

void application_manager::MessageHelper::SendQueryApps(
    const uint32_t connection_key) {
  using namespace mobile_apis;
  using namespace smart_objects;

  policy::PolicyHandler* policy_handler = policy::PolicyHandler::instance();

  SmartObject* content = new SmartObject(SmartType_Map);
  (*content)[strings::msg_params][strings::request_type] =
      RequestType::QUERY_APPS;
  (*content)[strings::msg_params][strings::url] =
      policy_handler->RemoteAppsUrl();
  (*content)[strings::msg_params][strings::timeout] =
      policy_handler->TimeoutExchange();

  Json::Value http;
  Json::Value& http_header =
      http[http_request::httpRequest][http_request::headers];

  const int timeout = policy_handler->TimeoutExchange();

  http_header[http_request::content_type] = "application/json";
  http_header[http_request::connect_timeout] = timeout;
  http_header[http_request::do_output] = true;
  http_header[http_request::do_input] = true;
  http_header[http_request::use_caches] = false;
  http_header[http_request::request_method] = http_request::GET;
  http_header[http_request::read_timeout] = timeout;
  http_header[http_request::instance_follow_redirect] = false;
  http_header[http_request::charset] = "utf-8";
  http_header[http_request::content_lenght] = 0;

  std::string data = http_header.toStyledString();
  std::vector<uint8_t> binary_data(data.begin(), data.end());

  (*content)[strings::params][strings::binary_data] = SmartObject(binary_data);
  (*content)[strings::msg_params][strings::file_type] = FileType::BINARY;

  SendSystemRequestNotification(connection_key, content);
}

void MessageHelper::SendOnPermissionsChangeNotification(
    uint32_t connection_key, const policy::Permissions& permissions) {
  utils::SharedPtr<smart_objects::SmartObject> notification =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject& content = *notification;

  content[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnPermissionsChangeID;
  content[strings::params][strings::message_type] =
      mobile_apis::messageType::notification;
  content[strings::params][strings::protocol_type] =
      commands::CommandImpl::mobile_protocol_type_;
  content[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
  content[strings::params][strings::connection_key] = connection_key;

  utils::SharedPtr<smart_objects::SmartObject> p_msg_params =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);

  smart_objects::SmartObject& msg_params = *p_msg_params;

  content[strings::msg_params] = msg_params;

  // content[strings::msg_params][strings::app_id] = connection_key;

  content[strings::msg_params]["permissionItem"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  smart_objects::SmartObject& permissions_item_array =
      content[strings::msg_params]["permissionItem"];

  policy::Permissions::const_iterator it_permissions = permissions.begin();
  policy::Permissions::const_iterator it_permissions_end = permissions.end();

  for (size_t index_pi = 0; it_permissions != it_permissions_end;
       ++it_permissions, ++index_pi) {
    permissions_item_array[index_pi] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    smart_objects::SmartObject& permission_item =
        permissions_item_array[index_pi];

    // Filling the rpcName of PermissionItem
    permission_item["rpcName"] = (*it_permissions).first;
    const policy::RpcPermissions& rpc_permissions = (*it_permissions).second;

    // Creating SO for hmiPermissions
    permission_item["hmiPermissions"] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    smart_objects::SmartObject& hmi_permissions =
        permission_item["hmiPermissions"];

    policy::HMIPermissions::const_iterator it_hmi_permissions =
        rpc_permissions.hmi_permissions.begin();
    policy::HMIPermissions::const_iterator it_hmi_permissions_end =
        rpc_permissions.hmi_permissions.end();

    // Filling the hmiPermissions of PermissionItem
    for (; it_hmi_permissions != it_hmi_permissions_end; ++it_hmi_permissions) {
      // Possible key names are "allowed", "userDisallowed"
      hmi_permissions[(*it_hmi_permissions).first] =
          smart_objects::SmartObject(smart_objects::SmartType_Array);

      smart_objects::SmartObject& hmi_levels =
          hmi_permissions[(*it_hmi_permissions).first];

      std::set<policy::HMILevel>::const_iterator it_hmi_levels =
          (*it_hmi_permissions).second.begin();
      std::set<policy::HMILevel>::const_iterator it_hmi_levels_end =
          (*it_hmi_permissions).second.end();

      for (size_t index_hmi_levels = 0; it_hmi_levels != it_hmi_levels_end;
           ++it_hmi_levels, ++index_hmi_levels) {
        hmi_levels[index_hmi_levels] = *it_hmi_levels;
      }
    }

    // Creating SO for parameterPermissions
    permission_item["parameterPermissions"] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    smart_objects::SmartObject& parameter_permissions =
        permission_item["parameterPermissions"];

    policy::ParameterPermissions::const_iterator it_parameter_permissions =
        rpc_permissions.parameter_permissions.begin();
    policy::ParameterPermissions::const_iterator it_parameter_permissions_end =
        rpc_permissions.parameter_permissions.end();

    // Filling the parameterPermissions of PermissionItem
    for (; it_parameter_permissions != it_parameter_permissions_end;
         ++it_parameter_permissions) {
      // Possible key names are "allowed", "userDisallowed"
      parameter_permissions[(*it_parameter_permissions).first] =
          smart_objects::SmartObject(smart_objects::SmartType_Array);

      smart_objects::SmartObject& parameters =
          parameter_permissions[(*it_parameter_permissions).first];

      std::set<policy::Parameter>::const_iterator it_parameters =
          (*it_parameter_permissions).second.begin();
      std::set<policy::Parameter>::const_iterator it_parameters_end =
          (*it_parameter_permissions).second.end();

      for (size_t index_parameters = 0; it_parameters != it_parameters_end;
           ++it_parameters, ++index_parameters) {
        parameters[index_parameters] = *it_parameters;
      }
    }
  }

  ApplicationManagerImpl::instance()->ManageMobileCommand(notification);
}

void MessageHelper::FillAppRevokedPermissions(
    const policy::AppPermissions& permissions,
    smart_objects::SmartObject& message) {
  message[strings::msg_params]["appRevokedPermissions"] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  smart_objects::SmartObject& revoked_permission_items =
      message[strings::msg_params]["appRevokedPermissions"];
  for (size_t i = 0; i < permissions.appRevokedPermissions.size(); ++i) {
    revoked_permission_items[i] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    smart_objects::SmartObject& permission_item = revoked_permission_items[i];
    permission_item["name"] = permissions.appRevokedPermissions[i].group_alias;

    permission_item["id"] = permissions.appRevokedPermissions[i].group_id;

    if (policy::kGroupUndefined != permissions.appRevokedPermissions[i].state) {
      permission_item["allowed"] =
          policy::kGroupAllowed == permissions.appRevokedPermissions[i].state
              ? true
              : false;
    }
  }
}

void MessageHelper::SendOnAppPermissionsChangedNotification(
    const uint32_t connection_key, const policy::AppPermissions& permissions) {
  using namespace smart_objects;
  SmartObjectSPtr notification = new SmartObject(SmartType_Map);
  if (!notification) {
    return;
  }

  SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_OnAppPermissionChanged;

  message[strings::params][strings::message_type] = MessageType::kNotification;
  message[strings::msg_params][strings::app_id] = connection_key;

  // TODO(AOleynik): Add other parameters processing from incoming struct
  if (permissions.appRevoked) {
    message[strings::msg_params]["appRevoked"] = permissions.appRevoked;
  }
  if (permissions.isAppPermissionsRevoked) {
    message[strings::msg_params]["isAppPermissionsRevoked"] =
        permissions.isAppPermissionsRevoked;

    FillAppRevokedPermissions(permissions, message);
  }

  if (permissions.appPermissionsConsentNeeded) {
    message[strings::msg_params]["appPermissionsConsentNeeded"] =
        permissions.appPermissionsConsentNeeded;
  }
  if (permissions.appUnauthorized) {
    message[strings::msg_params]["appUnauthorized"] =
        permissions.appUnauthorized;
  }
  if (!permissions.priority.empty()) {
    message[strings::msg_params]["priority"] =
        GetPriorityCode(permissions.priority);
  }
  if (permissions.requestTypeChanged) {
    SmartObject request_types_array = SmartObject(SmartType_Array);
    for (uint16_t index = 0; index < permissions.requestType.size(); ++index) {
      request_types_array[index] = permissions.requestType[index];
    }
    message[strings::msg_params][strings::request_type] = request_types_array;
  }

  ApplicationManagerImpl::instance()->ManageHMICommand(notification);
}

void MessageHelper::SendGetStatusUpdateResponse(const std::string& status,
                                                const uint32_t correlation_id) {
  smart_objects::SmartObjectSPtr message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_GetStatusUpdate;
  (*message)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::params][hmi_response::code] = 0;

  (*message)[strings::msg_params]["status"] = status;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendUpdateSDLResponse(const std::string& result,
                                          const uint32_t correlation_id) {
  smart_objects::SmartObjectSPtr message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  DCHECK(message);

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_UpdateSDL;
  (*message)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::params][hmi_response::code] = 0;

  (*message)[strings::msg_params]["result"] = result;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendOnStatusUpdate(const std::string& status) {
  smart_objects::SmartObjectSPtr message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_OnStatusUpdate;
  (*message)[strings::params][strings::message_type] =
      MessageType::kNotification;

  (*message)[strings::msg_params]["status"] = status;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendGetSystemInfoRequest() {
  smart_objects::SmartObjectSPtr message = CreateRequestObject();
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_GetSystemInfo;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

mobile_apis::Result::eType MessageHelper::VerifyImageFiles(
    smart_objects::SmartObject& message, ApplicationConstSharedPtr app) {
  if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == message.getType()) {
    for (uint32_t i = 0; i < message.length(); ++i) {
      mobile_apis::Result::eType res = VerifyImageFiles(message[i], app);
      if (mobile_apis::Result::SUCCESS != res) {
        LOG4CXX_DEBUG(logger_, "VerifyImageFiles result:" << res);
        return res;
      }
    }
  } else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Map ==
             message.getType()) {
    if (message.keyExists(strings::image_type)) {
      mobile_apis::Result::eType verification_result =
          VerifyImage(message, app);

      if (mobile_apis::Result::SUCCESS != verification_result) {
        LOG4CXX_DEBUG(logger_,
                      "VerifyImageFiles result:" << verification_result);
        return verification_result;  // exit point
      }
    } else {
      std::set<std::string> keys = message.enumerate();

      for (std::set<std::string>::const_iterator key = keys.begin();
           key != keys.end();
           ++key) {
        if (strings::soft_buttons != (*key)) {
          mobile_apis::Result::eType res = VerifyImageFiles(message[*key], app);
          if (mobile_apis::Result::SUCCESS != res) {
            LOG4CXX_DEBUG(logger_, "VerifyImageFiles result:" << res);
            return res;
          }
        }
      }
    }
  }  // all other types shoudn't be processed

  return mobile_apis::Result::SUCCESS;
}

mobile_apis::Result::eType MessageHelper::VerifyImage(
    smart_objects::SmartObject& image, ApplicationConstSharedPtr app) {
  // Checking image type first: if STATIC - skip existence check, since it is
  // HMI related file and it should know it location
  const uint32_t image_type = image[strings::image_type].asUInt();
  mobile_apis::ImageType::eType type =
      static_cast<mobile_apis::ImageType::eType>(image_type);
  if (mobile_apis::ImageType::STATIC == type) {
    return mobile_apis::Result::SUCCESS;
  }

  const std::string& file_name = image[strings::value].asString();

  std::string str = file_name;
  str.erase(remove(str.begin(), str.end(), ' '), str.end());
  if (0 == str.size()) {
    return mobile_apis::Result::INVALID_DATA;
  }

  std::string full_file_path;
  if (file_name.size() > 0 && file_name[0] == '/') {
    full_file_path = file_name;
  } else {
    const std::string& app_storage_folder =
        profile::Profile::instance()->app_storage_folder();
    if (!app_storage_folder.empty()) {
      // TODO(nvaganov@luxoft.com): APPLINK-11293
      if (app_storage_folder[0] == '/') {  // absolute path
        full_file_path = app_storage_folder + "/";
      } else {  // relative path
        full_file_path = file_system::CurrentWorkingDirectory() + "/" +
                         app_storage_folder + "/";
      }
    } else {  // empty app storage folder
      full_file_path = file_system::CurrentWorkingDirectory() + "/";
    }

    full_file_path += app->folder_name();
    full_file_path += "/";
    full_file_path += file_name;
  }

  if (!file_system::FileExists(full_file_path)) {
    return mobile_apis::Result::INVALID_DATA;
  }

  image[strings::value] = full_file_path;

  return mobile_apis::Result::SUCCESS;
}

mobile_apis::Result::eType MessageHelper::VerifyImageVrHelpItems(
    smart_objects::SmartObject& message, ApplicationConstSharedPtr app) {
  mobile_apis::Result::eType verification_result_image =
      mobile_apis::Result::SUCCESS;
  for (uint32_t i = 0; i < message.length(); ++i) {
    if (message[i].keyExists(strings::image)) {
      verification_result_image = VerifyImage(message[i][strings::image], app);
      if (mobile_apis::Result::SUCCESS != verification_result_image) {
        return verification_result_image;
      }
    }
  }
  return mobile_apis::Result::SUCCESS;
}

bool MessageHelper::VerifySoftButtonString(const std::string& str) {
  if ((std::string::npos != str.find_first_of("\t\n")) ||
      (std::string::npos != str.find("\\n")) ||
      (std::string::npos != str.find("\\t")) ||
      (std::string::npos == str.find_first_not_of(' '))) {
    LOG4CXX_ERROR(logger_,
                  "MessageHelper::VerifySoftButtonString"
                  "string contains incorrect character");
    return false;
  }
  return true;
}

bool MessageHelper::CheckWithPolicy(
    mobile_api::SystemAction::eType system_action,
    const std::string& app_mobile_id) {
  using namespace mobile_apis;
  bool result = true;
  policy::PolicyHandler* policy_handler = policy::PolicyHandler::instance();
  if (NULL != policy_handler && policy_handler->PolicyEnabled()) {
    result = policy_handler->CheckSystemAction(system_action, app_mobile_id);
  }

  return result;
}

mobile_apis::Result::eType MessageHelper::ProcessSoftButtons(
    smart_objects::SmartObject& message_params, ApplicationConstSharedPtr app) {
  using namespace mobile_apis;
  using namespace smart_objects;

  if (!message_params.keyExists(strings::soft_buttons)) {
    return mobile_apis::Result::SUCCESS;
  }

  SmartObject& request_soft_buttons = message_params[strings::soft_buttons];

  // Check whether soft buttons request is well-formed
  if (!ValidateSoftButtons(request_soft_buttons)) {
    return Result::INVALID_DATA;
  }

  SmartObject soft_buttons(SmartType_Array);

  uint32_t j = 0;
  size_t size = request_soft_buttons.length();
  for (uint32_t i = 0; i < size; ++i) {
    const int system_action =
        request_soft_buttons[i][strings::system_action].asInt();

    if (!CheckWithPolicy(static_cast<SystemAction::eType>(system_action),
                         app->mobile_app_id())) {
      return Result::DISALLOWED;
    }

    switch (request_soft_buttons[i][strings::type].asInt()) {
      case SoftButtonType::SBT_IMAGE: {
        // Any text value for type "IMAGE" should be ignored.
        if (request_soft_buttons[i].keyExists(strings::text)) {
          request_soft_buttons[i].erase(strings::text);
        }

        if ((!request_soft_buttons[i].keyExists(strings::image) ||
             (Result::SUCCESS !=
              VerifyImage(request_soft_buttons[i][strings::image], app)))) {
          return Result::INVALID_DATA;
        }
        break;
      }
      case SoftButtonType::SBT_TEXT: {
        if ((!request_soft_buttons[i].keyExists(strings::text)) ||
            (!VerifySoftButtonString(
                request_soft_buttons[i][strings::text].asString()))) {
          return Result::INVALID_DATA;
        }
        break;
      }
      case SoftButtonType::SBT_BOTH: {
        if ((!request_soft_buttons[i].keyExists(strings::text)) ||
            ((request_soft_buttons[i][strings::text].length()) &&
             (!VerifySoftButtonString(
                 request_soft_buttons[i][strings::text].asString())))) {
          return Result::INVALID_DATA;
        }

        if ((!request_soft_buttons[i].keyExists(strings::image) ||
             (Result::SUCCESS !=
              VerifyImage(request_soft_buttons[i][strings::image], app)))) {
          return Result::INVALID_DATA;
        }
        break;
      }
      default: {
        continue;
        break;
      }
    }

    soft_buttons[j++] = request_soft_buttons[i];
  }

  request_soft_buttons = soft_buttons;

  if (0 == request_soft_buttons.length()) {
    message_params.erase(strings::soft_buttons);
  }
  return Result::SUCCESS;
}

void MessageHelper::SubscribeApplicationToSoftButton(
    smart_objects::SmartObject& message_params,
    ApplicationSharedPtr app,
    int32_t function_id) {
  SoftButtonID softbuttons_id;
  smart_objects::SmartObject& soft_buttons =
      message_params[strings::soft_buttons];
  unsigned int length = soft_buttons.length();
  for (unsigned int i = 0; i < length; ++i) {
    softbuttons_id.insert(soft_buttons[i][strings::soft_button_id].asUInt());
  }
  app->SubscribeToSoftButtons(function_id, softbuttons_id);
}

// TODO(AK): change printf to logger
bool MessageHelper::PrintSmartObject(const smart_objects::SmartObject& object) {
  return true;
#ifdef ENABLE_LOG
  static uint32_t tab = 0;
  std::string tab_buffer;

  if (tab == 0) {
    printf("\n-------------------------------------------------------------");
  }

  for (uint32_t i = 0; i < tab; ++i) {
    tab_buffer += "\t";
  }

  switch (object.getType()) {
    case NsSmartDeviceLink::NsSmartObjects::SmartType_Array: {
      for (size_t i = 0; i < object.length(); i++) {
        ++tab;

        printf("\n%s%zu: ", tab_buffer.c_str(), i);
        if (!PrintSmartObject(object.getElement(i))) {
          printf("\n");
          return false;
        }
      }
      break;
    }
    case NsSmartDeviceLink::NsSmartObjects::SmartType_Map: {
      std::set<std::string> keys = object.enumerate();

      for (std::set<std::string>::const_iterator key = keys.begin();
           key != keys.end();
           key++) {
        ++tab;

        printf("\n%s%s: ", tab_buffer.c_str(), (*key).c_str());
        if (!PrintSmartObject(object[(*key).c_str()])) {
          printf("\n");
          return false;
        }
      }
      break;
    }
    case NsSmartDeviceLink::NsSmartObjects::SmartType_Boolean:
      object.asBool() ? printf("true\n") : printf("false\n");
      break;
    case NsSmartDeviceLink::NsSmartObjects::SmartType_Double: {
      printf("%f", object.asDouble());
      break;
    }
    case NsSmartDeviceLink::NsSmartObjects::SmartType_Integer:
      printf("%" PRId64 "\n", object.asInt());
      break;
    case NsSmartDeviceLink::NsSmartObjects::SmartType_String:
      printf("%s", object.asString().c_str());
      break;
    case NsSmartDeviceLink::NsSmartObjects::SmartType_Character:
      printf("%c", object.asChar());
      break;
    default:
      printf("PrintSmartObject - default case\n");
      break;
  }

  if (0 != tab) {
    --tab;
  } else {
    printf("\n-------------------------------------------------------------\n");
  }
#endif
  return true;
}

}  //  namespace application_manager
