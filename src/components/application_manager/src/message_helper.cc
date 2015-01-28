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
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler.h"
#include "config_profile/profile.h"
#include "connection_handler/connection_handler_impl.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/file_system.h"
#include "utils/macro.h"
#include "utils/logger.h"

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

namespace {

hmi_apis::Common_Language::eType ToCommonLanguage(
  mobile_apis::Language::eType mobile_language) {
  // Update this check if mobile_api::Language
  // or hmi_apis::Common_Language changes.
  // Or, better, generate functions like this from XML
  long lang_val = long(mobile_language);
  long max_common_lang_val = long(hmi_apis::Common_Language::NO_NO);
  long max_mobile_lang = long(mobile_apis::Language::NO_NO);
  if (max_common_lang_val != max_mobile_lang) {
    LOG4CXX_ERROR(logger_, "Mapping between Common_Language and Language"
                  " has changed! Please update converter function");
  }
  if (lang_val > max_common_lang_val) {
    LOG4CXX_ERROR(logger_, "Non-convertable language ID");
  }
  return hmi_apis::Common_Language::eType(lang_val);
}

typedef
std::map<std::string, hmi_apis::Common_AppPriority::eType> CommonAppPriorityMap;

CommonAppPriorityMap app_priority_values = {
  {"NORMAL", hmi_apis::Common_AppPriority::NORMAL},
  {"COMMUNICATION", hmi_apis::Common_AppPriority::COMMUNICATION},
  {"EMERGENCY", hmi_apis::Common_AppPriority::EMERGENCY},
  {"NAVIGATION", hmi_apis::Common_AppPriority::NAVIGATION},
  {"NONE", hmi_apis::Common_AppPriority::NONE},
  {"VOICECOM", hmi_apis::Common_AppPriority::VOICE_COMMUNICATION},
  {"INVALID_ENUM", hmi_apis::Common_AppPriority::INVALID_ENUM}
};

const uint32_t GetPriorityCode(const std::string& priority) {
  CommonAppPriorityMap::const_iterator it = app_priority_values.find(priority);
  if (app_priority_values.end() != it) {
    return static_cast<uint32_t>((*it).second);
  }
  return static_cast<uint32_t>(hmi_apis::Common_AppPriority::INVALID_ENUM);
}

bool ValidateSoftButtons(smart_objects::SmartObject& soft_buttons) {
  using namespace smart_objects;
  for (size_t i = 0; i < soft_buttons.length(); ++i) {
    SmartObject& button = soft_buttons[i];

    // Check if image parameter is valid
    if (button.keyExists(strings::image)) {
      SmartObject& buttonImage = button[strings::image];

      // Image name must not be empty and must not contain incorrect
      //character
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
  std::make_pair(strings::gps, VehicleDataType::GPS), std::make_pair(
    strings::speed, VehicleDataType::SPEED), std::make_pair(
      strings::rpm, VehicleDataType::RPM), std::make_pair(
        strings::fuel_level, VehicleDataType::FUELLEVEL), std::make_pair(
          strings::fuel_level_state, VehicleDataType::FUELLEVEL_STATE),
  std::make_pair(strings::instant_fuel_consumption,
  VehicleDataType::FUELCONSUMPTION), std::make_pair(
    strings::external_temp, VehicleDataType::EXTERNTEMP), std::make_pair(
      strings::vin, VehicleDataType::VIN), std::make_pair(
        strings::prndl, VehicleDataType::PRNDL), std::make_pair(
          strings::tire_pressure, VehicleDataType::TIREPRESSURE), std::make_pair(
            strings::odometer, VehicleDataType::ODOMETER), std::make_pair(
              strings::belt_status, VehicleDataType::BELTSTATUS), std::make_pair(
                strings::body_information, VehicleDataType::BODYINFO), std::make_pair(
                  strings::device_status, VehicleDataType::DEVICESTATUS), std::make_pair(
                    strings::driver_braking, VehicleDataType::BRAKING), std::make_pair(
                      strings::wiper_status, VehicleDataType::WIPERSTATUS), std::make_pair(
                        strings::head_lamp_status, VehicleDataType::HEADLAMPSTATUS),
  std::make_pair(strings::e_call_info, VehicleDataType::ECALLINFO),
  std::make_pair(strings::airbag_status, VehicleDataType::AIRBAGSTATUS),
  std::make_pair(strings::emergency_event, VehicleDataType::EMERGENCYEVENT),
  std::make_pair(strings::cluster_mode_status,
  VehicleDataType::CLUSTERMODESTATUS), std::make_pair(
    strings::my_key, VehicleDataType::MYKEY),
  /*
   NOT DEFINED in mobile API
   std::make_pair(strings::gps,                      VehicleDataType::BATTVOLTAGE),
   */
  std::make_pair(strings::engine_torque, VehicleDataType::ENGINETORQUE),
  std::make_pair(strings::acc_pedal_pos, VehicleDataType::ACCPEDAL),
  std::make_pair(strings::steering_wheel_angle,
  VehicleDataType::STEERINGWHEEL),
};

const VehicleData MessageHelper::vehicle_data_(
  kVehicleDataInitializer, kVehicleDataInitializer +
  ARRAYSIZE(kVehicleDataInitializer));

#ifdef HMI_DBUS_API
namespace {
struct VehicleInfo_Requests {
  hmi_apis::FunctionID::eType func_id;
  const char* str;
};
static VehicleInfo_Requests ivi_subrequests[] = {
  { hmi_apis::FunctionID::VehicleInfo_SubscribeGps, strings::gps},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeSpeed, strings::speed},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeRpm, strings::rpm},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeFuelLevel, strings::fuel_level},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeFuelLevel_State, strings::fuel_level_state},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeInstantFuelConsumption, strings::instant_fuel_consumption},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeExternalTemperature, strings::external_temp},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeVin, strings::vin},
  { hmi_apis::FunctionID::VehicleInfo_SubscribePrndl, strings::prndl},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeTirePressure, strings::tire_pressure},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeOdometer, strings::odometer},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeBeltStatus, strings::belt_status},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeBodyInformation, strings::body_information},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeDeviceStatus, strings::device_status},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeDriverBraking, strings::driver_braking},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeWiperStatus, strings::wiper_status},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeHeadLampStatus, strings::head_lamp_status},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeEngineTorque, strings::engine_torque},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeAccPedalPosition, strings::acc_pedal_pos},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeSteeringWheelAngle, strings::steering_wheel_angle},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeECallInfo, strings::e_call_info},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeAirbagStatus, strings::airbag_status},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeEmergencyEvent, strings::emergency_event},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeClusterModeStatus, strings::cluster_mode_status},
  { hmi_apis::FunctionID::VehicleInfo_SubscribeMyKey, strings::my_key},
};
}
#endif // #ifdef HMI_DBUS_API

std::string MessageHelper::CommonLanguageToString(
  hmi_apis::Common_Language::eType language) {
  switch (language) {
    case hmi_apis::Common_Language::EN_US:
      return "en-us";
    case hmi_apis::Common_Language::ES_MX:
      return "es-mx";
    case hmi_apis::Common_Language::FR_CA:
      return "fr-ca";
    case hmi_apis::Common_Language::DE_DE:
      return "de-de";
    case hmi_apis::Common_Language::ES_ES:
      return "es-es";
    case hmi_apis::Common_Language::EN_GB:
      return "en-gb";
    case hmi_apis::Common_Language::RU_RU:
      return "ru-ru";
    case hmi_apis::Common_Language::TR_TR:
      return "tr-tr";
    case hmi_apis::Common_Language::PL_PL:
      return "pl-pl";
    case hmi_apis::Common_Language::FR_FR:
      return "fr-fr";
    case hmi_apis::Common_Language::IT_IT:
      return "it-it";
    case hmi_apis::Common_Language::SV_SE:
      return "sv-se";
    case hmi_apis::Common_Language::PT_PT:
      return "pt-pt";
    case hmi_apis::Common_Language::NL_NL:
      return "nl-nl";
    case hmi_apis::Common_Language::EN_AU:
      return "en-au";
    case hmi_apis::Common_Language::ZH_CN:
      return "zh-cn";
    case hmi_apis::Common_Language::ZH_TW:
      return "zh-tw";
    case hmi_apis::Common_Language::JA_JP:
      return "ja-jp";
    case hmi_apis::Common_Language::AR_SA:
      return "as-sa";
    case hmi_apis::Common_Language::KO_KR:
      return "ko-kr";
    case hmi_apis::Common_Language::PT_BR:
      return "pt-br";
    case hmi_apis::Common_Language::CS_CZ:
      return "cs-cz";
    case hmi_apis::Common_Language::DA_DK:
      return "da-dk";
    case hmi_apis::Common_Language::NO_NO:
      return "no-no";
    default:
      return "";
  }
}

uint32_t MessageHelper::GetAppCommandLimit(const std::string& policy_app_id) {

  std::string priority;
  policy::PolicyHandler::instance()->GetPriority(policy_app_id, &priority);
  return policy::PolicyHandler::instance()->GetNotificationsNumber(priority);
}

void MessageHelper::SendHMIStatusNotification(
  const Application& application_impl) {
  smart_objects::SmartObject* notification = new smart_objects::SmartObject;
  if (!notification) {
    // TODO(VS): please add logger.
    return;
  }
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
    static_cast<int32_t>(mobile_api::FunctionID::OnHMIStatusID);

  message[strings::params][strings::message_type] =
    static_cast<int32_t>(application_manager::MessageType::kNotification);

  message[strings::params][strings::connection_key] =
    static_cast<int32_t>(application_impl.app_id());

  message[strings::msg_params][strings::hmi_level] =
    static_cast<int32_t>(application_impl.hmi_level());

  message[strings::msg_params][strings::audio_streaming_state] =
    static_cast<int32_t>(application_impl.audio_streaming_state());

  message[strings::msg_params][strings::system_context] =
    static_cast<int32_t>(application_impl.system_context());

  ApplicationManagerImpl::instance()->ManageMobileCommand(notification);
}

void MessageHelper::SendOnAppRegisteredNotificationToHMI(
  const Application& application_impl, bool resumption, bool need_restore_vr) {
  smart_objects::SmartObject* notification = new smart_objects::SmartObject;
  if (!notification) {
    // TODO(VS): please add logger.
    return;
  }
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
    static_cast<int32_t>(hmi_apis::FunctionID::BasicCommunication_OnAppRegistered);

  message[strings::params][strings::message_type] =
    static_cast<int32_t>(application_manager::MessageType::kNotification);
  message[strings::msg_params][strings::application][strings::app_name] =
    application_impl.name();

  const smart_objects::SmartObject* ngn_media_screen_name = application_impl
      .ngn_media_screen_name();

  // Due to current requirements in case when we're in resumption mode
  // we have to always send resumeVRGrammar field.
  if (resumption) {
    message[strings::msg_params][strings::resume_vr_grammars] = need_restore_vr;
  }

  if (ngn_media_screen_name) {
    message[strings::msg_params][strings::application][strings::ngn_media_screen_app_name] =
      *ngn_media_screen_name;
  }

  message[strings::msg_params][strings::application][strings::icon] =
    application_impl.app_icon_path();

  std::string dev_name = ApplicationManagerImpl::instance()->GetDeviceName(
                           application_impl.device());
  message[strings::msg_params][strings::application][strings::device_name] =
    dev_name;

  message[strings::msg_params][strings::application][strings::app_id] =
    application_impl.app_id();

  message[strings::msg_params][strings::application][strings::hmi_display_language_desired] =
    static_cast<int32_t>(application_impl.ui_language());

  message[strings::msg_params][strings::application][strings::is_media_application] =
    application_impl.is_media_application();

  const smart_objects::SmartObject* app_type = application_impl.app_types();

  if (app_type) {
    message[strings::msg_params][strings::application][strings::app_type] =
      *app_type;
  }
  if (application_impl.vr_synonyms()) {
    message[strings::msg_params][strings::vr_synonyms] = *(application_impl
        .vr_synonyms());
  }
  if (application_impl.tts_name()) {
    message[strings::msg_params][strings::tts_name] = *(application_impl
        .tts_name());
  }
  std::string priority;

  policy::PolicyHandler::instance()->GetPriority(
        application_impl.mobile_app_id()->asString(), &priority);
  if (!priority.empty()) {
    message[strings::msg_params][strings::priority] = GetPriorityCode(priority);
  }

  DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(notification));
}

smart_objects::SmartObject* MessageHelper::GetHashUpdateNotification(
  const uint32_t app_id) {

  LOG4CXX_INFO(logger_, "GetHashUpdateNotification" << app_id);
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
                               app_id);

  smart_objects::SmartObject* message = NULL;
  if (NULL == app.get()) {
    return message;
  }

  message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  (*message)[strings::params][strings::function_id] =
    mobile_apis::FunctionID::OnHashChangeID;
  (*message)[strings::params][strings::connection_key] = app_id;
  (*message)[strings::params][strings::message_type] =
          static_cast<int32_t>(kNotification);;

  return message;
}

void MessageHelper::SendHashUpdateNotification(const uint32_t app_id) {
  LOG4CXX_INFO(logger_, "SendHashUpdateNotification");

  smart_objects::SmartObject* so = GetHashUpdateNotification(app_id);
  if (so) {
    PrintSmartObject(*so);
    if (!ApplicationManagerImpl::instance()->ManageMobileCommand(so)) {
      LOG4CXX_ERROR_EXT(logger_, "Failed to send HashUpdate notification.");
    }
  }
}

void MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
  int32_t connection_key,
  mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  smart_objects::SmartObject* notification = new smart_objects::SmartObject;
  if (!notification) {
    // TODO(VS): please add logger.
    return;
  }
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
    static_cast<int32_t>(mobile_api::FunctionID::OnAppInterfaceUnregisteredID);

  message[strings::params][strings::message_type] =
    static_cast<int32_t>(kNotification);

  message[strings::params][strings::connection_key] = connection_key;

  message[strings::msg_params][strings::reason] = static_cast<int32_t>(reason);

  DCHECK(ApplicationManagerImpl::instance()->ManageMobileCommand(notification));
}

const VehicleData& MessageHelper::vehicle_data() {
  return vehicle_data_;
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
  using namespace NsSmartDeviceLink::NsSmartObjects;
  const char* str = 0;
  if (EnumConversionHelper<mobile_apis::FunctionID::eType>::EnumToCString(
        function_id, &str)) {
    const std::string enum_name = str;
    // Strip 'ID' suffix from value name
    DCHECK(enum_name.length() > 2
           && enum_name.substr(enum_name.length() - 2) == "ID");
    return enum_name.substr(0, enum_name.length() - 2);
  }
  return std::string();
}

#ifdef HMI_DBUS_API
namespace {
const std::map<std::string, uint16_t> create_get_vehicle_data_args() {
  std::map<std::string, uint16_t> rc;
  rc.insert(std::make_pair(strings::gps, hmi_apis::FunctionID::VehicleInfo_GetGpsData));
  rc.insert(std::make_pair(strings::speed, hmi_apis::FunctionID::VehicleInfo_GetSpeed));
  rc.insert(std::make_pair(strings::rpm, hmi_apis::FunctionID::VehicleInfo_GetRpm));
  rc.insert(std::make_pair(strings::fuel_level, hmi_apis::FunctionID::VehicleInfo_GetFuelLevel));
  rc.insert(std::make_pair(strings::fuel_level_state, hmi_apis::FunctionID::VehicleInfo_GetFuelLevelState));
  rc.insert(std::make_pair(strings::instant_fuel_consumption, hmi_apis::FunctionID::VehicleInfo_GetInstantFuelConsumption));
  rc.insert(std::make_pair(strings::external_temp, hmi_apis::FunctionID::VehicleInfo_GetExternalTemperature));
  rc.insert(std::make_pair(strings::vin, hmi_apis::FunctionID::VehicleInfo_GetVin));
  rc.insert(std::make_pair(strings::prndl, hmi_apis::FunctionID::VehicleInfo_GetPrndl));
  rc.insert(std::make_pair(strings::tire_pressure, hmi_apis::FunctionID::VehicleInfo_GetTirePressure));
  rc.insert(std::make_pair(strings::odometer, hmi_apis::FunctionID::VehicleInfo_GetOdometer));
  rc.insert(std::make_pair(strings::belt_status, hmi_apis::FunctionID::VehicleInfo_GetBeltStatus));
  rc.insert(std::make_pair(strings::body_information, hmi_apis::FunctionID::VehicleInfo_GetBodyInformation));
  rc.insert(std::make_pair(strings::device_status, hmi_apis::FunctionID::VehicleInfo_GetDeviceStatus));
  rc.insert(std::make_pair(strings::driver_braking, hmi_apis::FunctionID::VehicleInfo_GetDriverBraking));
  rc.insert(std::make_pair(strings::wiper_status, hmi_apis::FunctionID::VehicleInfo_GetWiperStatus));
  rc.insert(std::make_pair(strings::head_lamp_status, hmi_apis::FunctionID::VehicleInfo_GetHeadLampStatus));
  rc.insert(std::make_pair(strings::engine_torque, hmi_apis::FunctionID::VehicleInfo_GetEngineTorque));
  rc.insert(std::make_pair(strings::acc_pedal_pos, hmi_apis::FunctionID::VehicleInfo_GetAccPedalPosition));
  rc.insert(std::make_pair(strings::steering_wheel_angle, hmi_apis::FunctionID::VehicleInfo_GetSteeringWheelAngle));
  rc.insert(std::make_pair(strings::e_call_info, hmi_apis::FunctionID::VehicleInfo_GetECallInfo));
  rc.insert(std::make_pair(strings::airbag_status, hmi_apis::FunctionID::VehicleInfo_GetAirbagStatus));
  rc.insert(std::make_pair(strings::emergency_event, hmi_apis::FunctionID::VehicleInfo_GetEmergencyEvent));
  rc.insert(std::make_pair(strings::cluster_mode_status, hmi_apis::FunctionID::VehicleInfo_GetClusterModeStatus));
  rc.insert(std::make_pair(strings::my_key, hmi_apis::FunctionID::VehicleInfo_GetMyKey));
  return rc;
}
static std::map<std::string, uint16_t> vehicle_data_args = create_get_vehicle_data_args();
}
#endif

void MessageHelper::CreateGetVehicleDataRequest(uint32_t correlation_id, const std::vector<std::string>& params) {
#ifdef HMI_DBUS_API
  for (std::vector<std::string>::const_iterator it = params.begin();
       it != params.end(); it++) {
    smart_objects::SmartObject* request = new smart_objects::SmartObject;

    (*request)[strings::params][strings::message_type] = static_cast<int>(kRequest);
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
  smart_objects::SmartObject* request = new smart_objects::SmartObject;

  (*request)[strings::params][strings::message_type] = static_cast<int>(kRequest);
  (*request)[strings::params][strings::function_id] =
    static_cast<int>(hmi_apis::FunctionID::VehicleInfo_GetVehicleData);
  (*request)[strings::params][strings::correlation_id] = correlation_id;
  (*request)[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  (*request)[strings::params][strings::protocol_type] =
    commands::CommandImpl::hmi_protocol_type_;
  (*request)[strings::msg_params] = smart_objects::SmartObject(smart_objects::SmartType_Map);
  for (std::vector<std::string>::const_iterator it = params.begin();
       it != params.end(); it++) {
    (*request)[strings::msg_params][*it] = true;
  }
  ApplicationManagerImpl::instance()->ManageHMICommand(request);
#endif
}

smart_objects::SmartObject* MessageHelper::CreateBlockedByPoliciesResponse(
  mobile_apis::FunctionID::eType function_id,
  mobile_apis::Result::eType result, uint32_t correlation_id,
  uint32_t connection_key) {
  smart_objects::SmartObject* response = new smart_objects::SmartObject;
  if (!response) {
    return NULL;
  }

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

smart_objects::SmartObject* MessageHelper::CreateDeviceListSO(
  const connection_handler::DeviceMap& devices) {
  smart_objects::SmartObject* device_list_so = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  if (NULL == device_list_so) {
    return NULL;
  }

  (*device_list_so)[strings::device_list] = smart_objects::SmartObject(
        smart_objects::SmartType_Array);
  smart_objects::SmartObject& list_so = (*device_list_so)[strings::device_list];

  int32_t index = 0;
  for (connection_handler::DeviceMap::const_iterator it = devices.begin();
       devices.end() != it; ++it) {
    const connection_handler::Device& d =
      static_cast<connection_handler::Device>(it->second);
    list_so[index][strings::name] = d.user_friendly_name();
    list_so[index][strings::id] = it->second.device_handle();

    const policy::DeviceConsent device_consent =
        policy::PolicyHandler::instance()->GetUserConsentForDevice(it->second.mac_address());
    list_so[index][strings::isSDLAllowed] =
        policy::DeviceConsent::kDeviceAllowed == device_consent;
  }
  ++index;
  return device_list_so;
}

smart_objects::SmartObject* MessageHelper::CreateModuleInfoSO(
  uint32_t function_id) {
  smart_objects::SmartObject* module_info = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  smart_objects::SmartObject& object = *module_info;
  object[strings::params][strings::message_type] = static_cast<int>(kRequest);
  object[strings::params][strings::function_id] = static_cast<int>(function_id);
  object[strings::params][strings::correlation_id] =
    ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
  object[strings::msg_params] = smart_objects::SmartObject(
                                  smart_objects::SmartType_Map);
  return module_info;
}

smart_objects::SmartObject* MessageHelper::CreateSetAppIcon(
  const std::string& path_to_icon, uint32_t app_id) {
  smart_objects::SmartObject* set_icon = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

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
  LOG4CXX_INFO(logger_, " MessageHelper::SendIVISubscribtions ");

  bool succes = true;
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
                               app_id);
  DCHECK(app.get());

  SmartObjectList requests = GetIVISubscribtionRequests(app_id);
  for (SmartObjectList::const_iterator it = requests.begin();
       it != requests.end(); ++it) {
    if (!ApplicationManagerImpl::instance()->ManageHMICommand(*it)) {
      succes = false;
    }
  }
  return succes;
}

MessageHelper::SmartObjectList MessageHelper::GetIVISubscribtionRequests(
  const uint32_t app_id) {
  LOG4CXX_INFO(logger_, " MessageHelper::GetIVISubscribtionRequests ");

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
                               app_id);
  DCHECK(app);

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app_id;
  const VehicleData& vehicle_data = MessageHelper::vehicle_data_;
  VehicleData::const_iterator ivi_it = vehicle_data.begin();
  const std::set<uint32_t>& subscribes = app->SubscribesIVI();

  for (; vehicle_data.end() != ivi_it; ++ivi_it) {
    uint32_t type_id = static_cast<int>(ivi_it->second);
    if (subscribes.end() != subscribes.find(type_id)) {
      std::string key_name = ivi_it->first;
      msg_params[key_name] = true;
    }
  }

  SmartObjectList hmi_requests;
#ifdef HMI_JSON_API
  smart_objects::SmartObject* request = MessageHelper::CreateModuleInfoSO(
                                          hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData);
  (*request)[strings::msg_params] = msg_params;
  hmi_requests.push_back(request);
#endif // #ifdef HMI_JSON_API
#ifdef HMI_DBUS_API
  //Generate list of ivi_subrequests
  for (size_t i = 0; i < sizeof(ivi_subrequests) / sizeof(ivi_subrequests[0]); ++i) {
    const VehicleInfo_Requests& sr = ivi_subrequests[i];
    if (true == msg_params.keyExists(sr.str)
        && true == msg_params[sr.str].asBool()) {
      smart_objects::SmartObject* request = MessageHelper::CreateModuleInfoSO(
                                              sr.func_id);
      (*request)[strings::msg_params] = msg_params;
      hmi_requests.push_back(request);
    }
  }
#endif // #ifdef HMI_DBUS_API
  return hmi_requests;
}

void MessageHelper::SendAppDataToHMI(ApplicationConstSharedPtr app) {
  uint32_t id = app->app_id();

  utils::SharedPtr<smart_objects::SmartObject> set_app_icon(
    new smart_objects::SmartObject);
  if (set_app_icon) {
    smart_objects::SmartObject& so_to_send = *set_app_icon;
    so_to_send[strings::params][strings::function_id] =
      static_cast<int>(hmi_apis::FunctionID::UI_SetAppIcon);
    so_to_send[strings::params][strings::message_type] =
      static_cast<int>(hmi_apis::messageType::request);
    so_to_send[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
    so_to_send[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
    so_to_send[strings::params][strings::correlation_id] =
      ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

    so_to_send[strings::msg_params] = smart_objects::SmartObject(
                                        smart_objects::SmartType_Map);
    smart_objects::SmartObject* msg_params = MessageHelper::CreateSetAppIcon(
          app->app_icon_path(), id);

    if (msg_params) {
      so_to_send[strings::msg_params] = *msg_params;
    }
    // TODO(PV): appropriate handling of result
    DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(set_app_icon));
  }

  SendGlobalPropertiesToHMI(app);
  SendShowRequestToHMI(app);
}

void MessageHelper::SendGlobalPropertiesToHMI(ApplicationConstSharedPtr app) {
  DCHECK(app.get());

  SmartObjectList requests = CreateGlobalPropertiesRequestsToHMI(app);
  for (SmartObjectList::const_iterator it = requests.begin();
       it != requests.end(); ++it) {
    DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(*it))
  }
}

MessageHelper::SmartObjectList MessageHelper::CreateGlobalPropertiesRequestsToHMI(
  ApplicationConstSharedPtr app) {

  SmartObjectList requests;
  DCHECK(app.get());

  // UI global properties

  if (app->vr_help_title() || app->vr_help()) {
    smart_objects::SmartObject* ui_global_properties =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);

    if (!ui_global_properties) {
      return requests;
    }

    (*ui_global_properties)[strings::params][strings::function_id] =
      static_cast<int>(hmi_apis::FunctionID::UI_SetGlobalProperties);
    (*ui_global_properties)[strings::params][strings::message_type] =
      static_cast<int>(hmi_apis::messageType::request);
    (*ui_global_properties)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
    (*ui_global_properties)[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
    (*ui_global_properties)[strings::params][strings::correlation_id] =
      ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

    smart_objects::SmartObject ui_msg_params = smart_objects::SmartObject(
          smart_objects::SmartType_Map);
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
    smart_objects::SmartObject* tts_global_properties =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);

    if (!tts_global_properties) {
      return requests;
    }

    (*tts_global_properties)[strings::params][strings::function_id] =
      static_cast<int>(hmi_apis::FunctionID::TTS_SetGlobalProperties);
    (*tts_global_properties)[strings::params][strings::message_type] =
      static_cast<int>(hmi_apis::messageType::request);
    (*tts_global_properties)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
    (*tts_global_properties)[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
    (*tts_global_properties)[strings::params][strings::correlation_id] =
      ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

    smart_objects::SmartObject tts_msg_params = smart_objects::SmartObject(
          smart_objects::SmartType_Map);
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

void MessageHelper::SendTTSGlobalProperties(
    ApplicationSharedPtr app, bool default_help_prompt) {
  if (!app.valid()) {
    return;
  }
  utils::SharedPtr<smart_objects::SmartObject> tts_global_properties(
      new smart_objects::SmartObject);
  if (tts_global_properties) {
    smart_objects::SmartObject& so_to_send = *tts_global_properties;
    so_to_send[strings::params][strings::function_id] =
        static_cast<int>(hmi_apis::FunctionID::TTS_SetGlobalProperties);
    so_to_send[strings::params][strings::message_type] =
        static_cast<int>(hmi_apis::messageType::request);
    so_to_send[strings::params][strings::protocol_version] =
        commands::CommandImpl::protocol_version_;
    so_to_send[strings::params][strings::protocol_type] =
        commands::CommandImpl::hmi_protocol_type_;
    so_to_send[strings::params][strings::correlation_id] =
        ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);
    msg_params[strings::help_prompt] = smart_objects::SmartObject(
        smart_objects::SmartType_Array);
    if (default_help_prompt) {
      const CommandsMap& commands = app->commands_map();
      CommandsMap::const_iterator it = commands.begin();
      uint32_t index = 0;
      for (; commands.end() != it; ++it) {
        smart_objects::SmartObject item(smart_objects::SmartType_Map);
        if ((*it->second).keyExists(strings::menu_params)){
          item[strings::text] = (*it->second)[strings::menu_params][strings::menu_name].asString();
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

smart_objects::SmartObject* MessageHelper::CreateAppVrHelp(
  ApplicationConstSharedPtr app) {
  smart_objects::SmartObject* result = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  if (!result) {
    return NULL;
  }
  smart_objects::SmartObject& vr_help = *result;
  vr_help[strings::vr_help_title] = app->name();

  ApplicationManagerImpl::ApplicationListAccessor accessor;
  const std::set<ApplicationSharedPtr> apps = accessor.applications();

  int32_t index = 0;
  std::set<ApplicationSharedPtr>::const_iterator it_app = apps.begin();
  for (; apps.end() != it_app; ++it_app) {
    if ((*it_app)->vr_synonyms()) {
      smart_objects::SmartObject item(smart_objects::SmartType_Map);
      item[strings::text] = (*((*it_app)->vr_synonyms())).getElement(0);
      item[strings::position] = index + 1;
      vr_help[strings::vr_help][index++] = item;
    }
  }

  // copy all app VR commands
  const CommandsMap& commands = app->commands_map();
  CommandsMap::const_iterator it = commands.begin();

  for (; commands.end() != it; ++it) {
    smart_objects::SmartObject item(smart_objects::SmartType_Map);
    item[strings::text] = (*it->second)[strings::vr_commands][0].asString();
    item[strings::position] = index + 1;
    vr_help[strings::vr_help][index++] = item;
  }
  return result;
}

MessageHelper::SmartObjectList MessageHelper::CreateShowRequestToHMI(
  ApplicationConstSharedPtr app) {
  DCHECK(app.get());

  SmartObjectList requests;
  smart_objects::SmartObject* ui_show = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  if (app->show_command()) {
    (*ui_show)[strings::params][strings::function_id] =
      static_cast<int>(hmi_apis::FunctionID::UI_Show);
    (*ui_show)[strings::params][strings::message_type] =
      static_cast<int>(hmi_apis::messageType::request);
    (*ui_show)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
    (*ui_show)[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
    (*ui_show)[strings::params][strings::correlation_id] =
      ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
    (*ui_show)[strings::msg_params] = (*app->show_command());
    requests.push_back(ui_show);
  }
  return requests;
}

void MessageHelper::SendShowRequestToHMI(ApplicationConstSharedPtr app) {
  if (!app) {
    return;
  }
  SmartObjectList shows = CreateShowRequestToHMI(app);

  for (SmartObjectList::const_iterator it = shows.begin(); it != shows.end();
       ++it) {
    DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(*it));
  }

}

void MessageHelper::SendShowConstantTBTRequestToHMI(
  ApplicationConstSharedPtr app) {
  if (!app) {
    return;
  }

  smart_objects::SmartObject* navi_show_tbt = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  if (!navi_show_tbt) {
    return;
  }

  if (app->tbt_show_command()) {
    (*navi_show_tbt)[strings::params][strings::function_id] =
      static_cast<int>(hmi_apis::FunctionID::Navigation_ShowConstantTBT);
    (*navi_show_tbt)[strings::params][strings::message_type] =
      static_cast<int>(hmi_apis::messageType::request);
    (*navi_show_tbt)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
    (*navi_show_tbt)[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
    (*navi_show_tbt)[strings::params][strings::correlation_id] =
      ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
    (*navi_show_tbt)[strings::msg_params] = (*app->tbt_show_command());
    DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(navi_show_tbt));
  }
}

void MessageHelper::SendAddCommandRequestToHMI(ApplicationConstSharedPtr app) {
  if (!app) {
    return;
  }
  SmartObjectList requests = CreateAddCommandRequestToHMI(app);
  for (SmartObjectList::iterator it = requests.begin(); it != requests.end();
       ++it) {
    DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(*it));
  }
}

MessageHelper::SmartObjectList MessageHelper::CreateAddCommandRequestToHMI(
  ApplicationConstSharedPtr app) {
  DCHECK(app.get());

  SmartObjectList requests;
  const CommandsMap& commands = app->commands_map();
  CommandsMap::const_iterator i = commands.begin();
  for (; commands.end() != i; ++i) {
    // UI Interface
    if ((*i->second).keyExists(strings::menu_params)) {
      smart_objects::SmartObject* ui_command = new smart_objects::SmartObject(
        smart_objects::SmartType_Map);

      if (!ui_command) {
        return requests;
      }

      (*ui_command)[strings::params][strings::function_id] =
        static_cast<int>(hmi_apis::FunctionID::UI_AddCommand);
      (*ui_command)[strings::params][strings::message_type] =
        static_cast<int>(hmi_apis::messageType::request);
      (*ui_command)[strings::params][strings::protocol_version] =
        commands::CommandImpl::protocol_version_;
      (*ui_command)[strings::params][strings::protocol_type] =
        commands::CommandImpl::hmi_protocol_type_;
      (*ui_command)[strings::params][strings::correlation_id] =
        ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

      smart_objects::SmartObject msg_params = smart_objects::SmartObject(
          smart_objects::SmartType_Map);
      msg_params[strings::cmd_id] = i->first;
      msg_params[strings::menu_params] = (*i->second)[strings::menu_params];
      msg_params[strings::app_id] = app->app_id();

      if (((*i->second)[strings::cmd_icon].keyExists(strings::value))
          && (0 < (*i->second)[strings::cmd_icon][strings::value].length())) {
        msg_params[strings::cmd_icon] = (*i->second)[strings::cmd_icon];
        msg_params[strings::cmd_icon][strings::value] =
          (*i->second)[strings::cmd_icon][strings::value].asString();
      }
      (*ui_command)[strings::msg_params] = msg_params;
      requests.push_back(ui_command);
    }

    // VR Interface
    if ((*i->second).keyExists(strings::vr_commands)) {
      SendAddVRCommandToHMI(i->first, (*i->second)[strings::vr_commands],
                            app->app_id());
    }
  }
  return requests;
}

smart_objects::SmartObject* MessageHelper::CreateChangeRegistration(
  int32_t function_id, int32_t language, uint32_t app_id) {
  smart_objects::SmartObject* command = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  if (!command) {
    return NULL;
  }
  smart_objects::SmartObject& params = *command;

  params[strings::params][strings::message_type] =
    static_cast<int>(hmi_apis::messageType::request);
  params[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  params[strings::params][strings::protocol_type] =
    commands::CommandImpl::hmi_protocol_type_;

  params[strings::params][strings::function_id] = function_id;

  params[strings::params][strings::correlation_id] =
    ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  msg_params[strings::language] = language;
  msg_params[strings::app_id] = app_id;

  params[strings::msg_params] = msg_params;
  return command;
}

void MessageHelper::SendChangeRegistrationRequestToHMI(ApplicationConstSharedPtr app) {
  if (!app.valid()) {
    return;
  }
  if (mobile_apis::Language::INVALID_ENUM != app->language()) {
    smart_objects::SmartObject* vr_command = CreateChangeRegistration(
          hmi_apis::FunctionID::VR_ChangeRegistration, app->language(),
          app->app_id());

    if (vr_command) {
      ApplicationManagerImpl::instance()->ManageHMICommand(vr_command);
    }
  }

  if (mobile_apis::Language::INVALID_ENUM != app->language()) {
    smart_objects::SmartObject* tts_command = CreateChangeRegistration(
          hmi_apis::FunctionID::TTS_ChangeRegistration, app->language(),
          app->app_id());

    if (tts_command) {
      ApplicationManagerImpl::instance()->ManageHMICommand(tts_command);
    }
  }

  if (mobile_apis::Language::INVALID_ENUM != app->ui_language()) {
    smart_objects::SmartObject* ui_command = CreateChangeRegistration(
          hmi_apis::FunctionID::UI_ChangeRegistration, app->ui_language(),
          app->app_id());

    if (ui_command) {
      ApplicationManagerImpl::instance()->ManageHMICommand(ui_command);
    }

  }
}

void MessageHelper::SendAddVRCommandToHMI(
  uint32_t cmd_id, const smart_objects::SmartObject& vr_commands,
  uint32_t app_id) {
  smart_objects::SmartObject* request = CreateAddVRCommandToHMI(cmd_id,
                                        vr_commands,
                                        app_id);
  DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(request));
}

smart_objects::SmartObject* MessageHelper::CreateAddVRCommandToHMI(
  uint32_t cmd_id,
  const NsSmartDeviceLink::NsSmartObjects::SmartObject& vr_commands,
  uint32_t app_id) {
  smart_objects::SmartObject* vr_command = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  if (!vr_command) {
    return NULL;
  }

  (*vr_command)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::VR_AddCommand;
  (*vr_command)[strings::params][strings::message_type] =
    hmi_apis::messageType::request;
  (*vr_command)[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  (*vr_command)[strings::params][strings::protocol_type] =
    commands::CommandImpl::hmi_protocol_type_;
  (*vr_command)[strings::params][strings::correlation_id] =
    ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
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

bool MessageHelper::CreateHMIApplicationStruct(ApplicationConstSharedPtr app,
    smart_objects::SmartObject& output) {

  if (false == app.valid()) {
    return false;
  }

  const smart_objects::SmartObject* app_types = app->app_types();
  const smart_objects::SmartObject* ngn_media_screen_name = app->ngn_media_screen_name();
  const connection_handler::DeviceHandle handle = app->device();
  std::string device_name = ApplicationManagerImpl::instance()->GetDeviceName(handle);

  output = smart_objects::SmartObject(smart_objects::SmartType_Map);
  output[strings::app_name] = app->name();
  output[strings::icon] = app->app_icon_path();
  output[strings::device_name] = device_name;
  output[strings::app_id] = app->app_id();
  output[strings::hmi_display_language_desired] = app->ui_language();
  output[strings::is_media_application] = app->is_media_application();

  if (NULL != ngn_media_screen_name) {
    output[strings::ngn_media_screen_app_name] = ngn_media_screen_name->asString();
  }
  if (NULL != app_types) {
    output[strings::app_type] = *app_types;
  }
  return true;
}

void MessageHelper::SendAddSubMenuRequestToHMI(ApplicationConstSharedPtr app) {
  DCHECK(app.get());
  SmartObjectList requests = CreateAddSubMenuRequestToHMI(app);
  for (SmartObjectList::iterator it = requests.begin(); it != requests.end();
       ++it) {
    DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(*it));
  }
}

MessageHelper::SmartObjectList MessageHelper::CreateAddSubMenuRequestToHMI(
  ApplicationConstSharedPtr app) {
  SmartObjectList requsets;
  const SubMenuMap& sub_menu = app->sub_menu_map();
  SubMenuMap::const_iterator i = sub_menu.begin();
  for (; sub_menu.end() != i; ++i) {
    smart_objects::SmartObject* ui_sub_menu = new smart_objects::SmartObject(
      smart_objects::SmartType_Map);

    if (!ui_sub_menu) {
      return requsets;
    }

    (*ui_sub_menu)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::UI_AddSubMenu;
    (*ui_sub_menu)[strings::params][strings::message_type] =
      hmi_apis::messageType::request;
    (*ui_sub_menu)[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
    (*ui_sub_menu)[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
    (*ui_sub_menu)[strings::params][strings::correlation_id] =
      ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);

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
  ApplicationConstSharedPtr app, bool is_unexpected_disconnect) {
  smart_objects::SmartObject* notification = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  if (!notification) {
    return;
  }

  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
    hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered;

  message[strings::params][strings::message_type] = MessageType::kNotification;
  // we put hmi_app_id because applicaton list does not contain application on this momment
  // and ReplaceHMIByMobileAppId function will be unable to replace app_id to hmi_app_id
  message[strings::msg_params][strings::app_id] = app->hmi_app_id();
  message[strings::msg_params][strings::unexpected_disconnect] =
    is_unexpected_disconnect;
  ApplicationManagerImpl::instance()->ManageHMICommand(&message);
}

void MessageHelper::SendActivateAppToHMI(uint32_t const app_id,
    hmi_apis::Common_HMILevel::eType level) {
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  application_manager::ApplicationConstSharedPtr app =
    application_manager::ApplicationManagerImpl::instance()
    ->application(app_id);
  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "Invalid app_id: " << app_id);
    return;
  }

  (*message)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::BasicCommunication_ActivateApp;
  (*message)[strings::params][strings::message_type] = MessageType::kRequest;
  (*message)[strings::params][strings::correlation_id] =
    ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
  (*message)[strings::msg_params][strings::app_id] = app_id;

  std::string priority;
  // TODO(KKolodiy): need remove method policy_manager

  policy::PolicyHandler::instance()->GetPriority(
        app->mobile_app_id()->asString(), &priority);
  // According SDLAQ-CRS-2794
  // SDL have to send ActivateApp without "proirity" parameter to HMI.
  // in case of unconsented device
  std::string mac_adress;
  connection_handler::DeviceHandle device_handle = app->device();
  connection_handler::ConnectionHandlerImpl::instance()->
      GetDataOnDeviceID(device_handle, NULL, NULL, &mac_adress, NULL);

  policy::DeviceConsent consent =
      policy::PolicyHandler::instance()->GetUserConsentForDevice(mac_adress);
  if (!priority.empty() && (policy::DeviceConsent::kDeviceAllowed == consent)) {
    (*message)[strings::msg_params]["priority"] = GetPriorityCode(priority);
  }

  // We haven't send HMI level to HMI in case it FULL.
  if (hmi_apis::Common_HMILevel::INVALID_ENUM != level &&
      hmi_apis::Common_HMILevel::FULL != level) {
    (*message)[strings::msg_params][strings::activate_app_hmi_level] = level;
  }

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendOnResumeAudioSourceToHMI(const uint32_t app_id) {
  LOG4CXX_WARN(logger_, "SendOnResumeAudioSourceToHMI app_id: " << app_id);

  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  application_manager::ApplicationConstSharedPtr app =
    application_manager::ApplicationManagerImpl::instance()
    ->application(app_id);
  if (!app.valid()) {
    LOG4CXX_WARN(logger_, "Invalid app_id: " << app_id);
    return;
  }

  (*message)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::BasicCommunication_OnResumeAudioSource;
  (*message)[strings::params][strings::message_type] = MessageType::kNotification;
  (*message)[strings::params][strings::correlation_id] =
    ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
  (*message)[strings::msg_params][strings::app_id] = app_id;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

std::string MessageHelper::GetDeviceMacAddressForHandle(
  const uint32_t device_handle) {

  std::string device_mac_address = "";
  connection_handler::ConnectionHandlerImpl::instance()->GetDataOnDeviceID(
    device_handle, NULL, NULL, &device_mac_address);

  return device_mac_address;
}

void MessageHelper::GetDeviceInfoForHandle(const uint32_t device_handle,
    policy::DeviceParams* device_info) {
  if (!device_info) {
    return;
  }
  connection_handler::ConnectionHandlerImpl::instance()->GetDataOnDeviceID(
    device_handle, &device_info->device_name, NULL,
    &device_info->device_mac_address, &device_info->device_connection_type);
}

void MessageHelper::GetDeviceInfoForApp(uint32_t connection_key,
                                        policy::DeviceParams* device_info) {
  if (!device_info) {
    return;
  }

  device_info->device_handle = ApplicationManagerImpl::instance()->application(
                                 connection_key)->device();

  GetDeviceInfoForHandle(device_info->device_handle, device_info);
}

void MessageHelper::SendSDLActivateAppResponse(policy::AppPermissions& permissions,
    uint32_t correlation_id) {
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
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
    (*message)[strings::msg_params]["device"]["name"] = permissions.deviceInfo
        .device_name;
    (*message)[strings::msg_params]["device"]["id"] = permissions.deviceInfo
        .device_handle;
  }

  (*message)[strings::msg_params]["isAppRevoked"] = permissions.appRevoked;
  (*message)[strings::msg_params]["isAppPermissionsRevoked"] = permissions
      .isAppPermissionsRevoked;

  if (permissions.isAppPermissionsRevoked) {
    FillAppRevokedPermissions(permissions, *message);
  }

  (*message)[strings::msg_params]["isPermissionsConsentNeeded"] = permissions
      .appPermissionsConsentNeeded;

  if (!permissions.priority.empty()) {
    (*message)[strings::msg_params]["priority"] = GetPriorityCode(
          permissions.priority);
  }

  ApplicationManagerImpl::instance()->ManageHMICommand(message);

  // If application is revoked it should not be activated
  if (permissions.appRevoked || !permissions.isSDLAllowed) {
    return;
  }

  // Send HMI status notification to mobile
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()
                             ->application_by_policy_id(permissions.application_id);
  if (app) {
    ApplicationManagerImpl::instance()->ActivateApplication(app);
  } else {
    LOG4CXX_WARN(logger_, "Unable to find app_id: " << permissions.application_id);
  }
}

void MessageHelper::SendOnSDLConsentNeeded(
  const policy::DeviceParams& device_info) {
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::SDL_OnSDLConsentNeeded;
  (*message)[strings::params][strings::message_type] =
    MessageType::kNotification;

  (*message)[strings::msg_params]["device"]["id"] = device_info.device_handle;
  (*message)[strings::msg_params]["device"]["name"] = device_info.device_name;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendPolicyUpdate(
  const std::string& file_path,
  int timeout,
  const std::vector<int>& retries) {
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  smart_objects::SmartObject& object = *message;
  object[strings::params][strings::function_id] =
    hmi_apis::FunctionID::BasicCommunication_PolicyUpdate;
  object[strings::params][strings::message_type] = MessageType::kRequest;
  object[strings::params][strings::correlation_id] =
    ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
  object[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  object[strings::params][strings::protocol_type] =
    commands::CommandImpl::hmi_protocol_type_;

  object[strings::msg_params][hmi_request::file] = file_path;
  object[strings::msg_params][strings::timeout] = timeout;
  object[strings::msg_params][hmi_request::retry] = smart_objects::SmartObject(
        smart_objects::SmartType_Array);
  for (size_t i = 0; i < retries.size(); ++i) {
    object[strings::msg_params][hmi_request::retry][i] = retries[i];
  }
  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendGetUserFriendlyMessageResponse(
  const std::vector<policy::UserFriendlyMessage>& msg,
  uint32_t correlation_id) {
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::SDL_GetUserFriendlyMessage;
  (*message)[strings::params][strings::message_type] =
    MessageType::kResponse;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::params][hmi_response::code] = 0;

  // If no any messages found - skip sending of "messages" param
  if (msg.empty()) {
    ApplicationManagerImpl::instance()->ManageHMICommand(message);
  }

  const std::string messages = "messages";
  (*message)[strings::msg_params][messages] =
    smart_objects::SmartObject(smart_objects::SmartType_Array);

  smart_objects::SmartObject& user_friendly_messages =
    (*message)[strings::msg_params][messages];


  const std::string tts = "ttsString";
  const std::string label = "label";
  const std::string line1 = "line1";
  const std::string line2 = "line2";
  const std::string textBody = "textBody";
  const std::string message_code = "messageCode";

  std::vector<policy::UserFriendlyMessage>::const_iterator it = msg.begin();
  std::vector<policy::UserFriendlyMessage>::const_iterator it_end = msg.end();
  for (uint32_t index = 0; it != it_end; ++it, ++index) {
    user_friendly_messages[index] = smart_objects::SmartObject(
                                      smart_objects::SmartType_Map);

    smart_objects::SmartObject& obj = user_friendly_messages[index];
    obj[message_code] = it->message_code;

    if (!it->tts.empty()) {
      obj[tts] = it->tts;
    }
    if (!it->label.empty()) {
      obj[label] = it->label;
    }
    if (!it->line1.empty()) {
      obj[line1] = it->line1;
    }
    if (!it->line2.empty()) {
      obj[line2] = it->line2;
    }
    if (!it->text_body.empty()) {
      obj[textBody] = it->text_body;
    }
  }

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendGetListOfPermissionsResponse(
    const std::vector<policy::FunctionalGroupPermission>& permissions,
    uint32_t correlation_id) {
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::SDL_GetListOfPermissions;
  (*message)[strings::params][strings::message_type] =
    MessageType::kResponse;
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
    allowed_functions_array[index] = smart_objects::SmartObject(
                                       smart_objects::SmartType_Map);

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

smart_objects::SmartObject* MessageHelper::CreateNegativeResponse(
  uint32_t connection_key, int32_t function_id, uint32_t correlation_id,
  int32_t result_code) {
  smart_objects::SmartObject* response = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
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

void MessageHelper::SendNaviStartStream(int32_t connection_key) {
  LOG4CXX_INFO(logger_, "MessageHelper::SendNaviStartStream");
  smart_objects::SmartObject* start_stream = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  if (!start_stream) {
    return;
  }

  (*start_stream)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::Navigation_StartStream;
  (*start_stream)[strings::params][strings::message_type] =
    hmi_apis::messageType::request;
  (*start_stream)[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  (*start_stream)[strings::params][strings::protocol_type] =
    commands::CommandImpl::hmi_protocol_type_;
  (*start_stream)[strings::params][strings::correlation_id] =
    ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  uint32_t app_id = 0;
  connection_handler::ConnectionHandlerImpl::instance()->GetDataOnSessionKey(
    connection_key, &app_id);

  char url[100] = {'\0'};
  if ("socket" == profile::Profile::instance()->video_server_type()) {
    snprintf(url, sizeof(url) / sizeof(url[0]), "http://%s:%d",
             profile::Profile::instance()->server_address().c_str(),
             profile::Profile::instance()->video_streaming_port());
  } else if ("pipe" == profile::Profile::instance()->video_server_type()) {
    snprintf(url, sizeof(url) / sizeof(url[0]), "%s",
             profile::Profile::instance()->named_video_pipe_path().c_str());
  } else {
    int snprintf_result;
    snprintf_result = snprintf(url, sizeof(url) / sizeof(url[0]), "%s",
        profile::Profile::instance()->video_stream_file().c_str());
    DCHECK(snprintf_result);
  }
  msg_params[strings::app_id] = app_id;
  msg_params[strings::url] = url;

  (*start_stream)[strings::msg_params] = msg_params;

  ApplicationManagerImpl::instance()->ManageHMICommand(start_stream);
}

void MessageHelper::SendNaviStopStream(int32_t connection_key) {
  smart_objects::SmartObject* stop_stream = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  if (!stop_stream) {
    return;
  }

  (*stop_stream)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::Navigation_StopStream;
  (*stop_stream)[strings::params][strings::message_type] =
    hmi_apis::messageType::request;
  (*stop_stream)[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  (*stop_stream)[strings::params][strings::protocol_type] =
    commands::CommandImpl::hmi_protocol_type_;
  (*stop_stream)[strings::params][strings::correlation_id] =
    ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  uint32_t app_id = 0;
  connection_handler::ConnectionHandlerImpl::instance()->GetDataOnSessionKey(
    connection_key, &app_id);

  msg_params[strings::app_id] = app_id;

  (*stop_stream)[strings::msg_params] = msg_params;

  ApplicationManagerImpl::instance()->ManageHMICommand(stop_stream);
}

void MessageHelper::SendAudioStartStream(int32_t connection_key) {

  smart_objects::SmartObject* start_stream = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  if (!start_stream) {
    return;
  }

  (*start_stream)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::Navigation_StartAudioStream;
  (*start_stream)[strings::params][strings::message_type] =
    hmi_apis::messageType::request;
  (*start_stream)[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  (*start_stream)[strings::params][strings::protocol_type] =
    commands::CommandImpl::hmi_protocol_type_;
  (*start_stream)[strings::params][strings::correlation_id] =
    ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  uint32_t app_id = 0;
  connection_handler::ConnectionHandlerImpl::instance()->GetDataOnSessionKey(
    connection_key, &app_id);

  char url[100] = {'\0'};
  if ("socket" == profile::Profile::instance()->audio_server_type()) {
    snprintf(url, sizeof(url) / sizeof(url[0]), "http://%s:%d",
           profile::Profile::instance()->server_address().c_str(),
           profile::Profile::instance()->audio_streaming_port());
  } else if ("pipe" == profile::Profile::instance()->audio_server_type()) {
    snprintf(url, sizeof(url) / sizeof(url[0]), "%s",
             profile::Profile::instance()->named_audio_pipe_path().c_str());
  } else {
    int snprintf_result;
    snprintf_result = snprintf(url, sizeof(url) / sizeof(url[0]), "%s",
         profile::Profile::instance()->audio_stream_file().c_str());
    DCHECK(snprintf_result);
  }

  msg_params[strings::app_id] = app_id;
  msg_params[strings::url] = url;

  (*start_stream)[strings::msg_params] = msg_params;

  DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(start_stream));
}

void MessageHelper::SendAudioStopStream(int32_t connection_key) {
  smart_objects::SmartObject* stop_stream = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  if (!stop_stream) {
    return;
  }

  (*stop_stream)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::Navigation_StopAudioStream;
  (*stop_stream)[strings::params][strings::message_type] =
    hmi_apis::messageType::request;
  (*stop_stream)[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  (*stop_stream)[strings::params][strings::protocol_type] =
    commands::CommandImpl::hmi_protocol_type_;
  (*stop_stream)[strings::params][strings::correlation_id] =
    ApplicationManagerImpl::instance()->GetNextHMICorrelationID();

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  uint32_t app_id = 0;
  connection_handler::ConnectionHandlerImpl::instance()->GetDataOnSessionKey(
    connection_key, &app_id);

  msg_params[strings::app_id] = app_id;

  (*stop_stream)[strings::msg_params] = msg_params;

  ApplicationManagerImpl::instance()->ManageHMICommand(stop_stream);
}

bool MessageHelper::SendStopAudioPathThru() {
  LOG4CXX_INFO(logger_, "MessageHelper::SendAudioStopAudioPathThru");

  NsSmartDeviceLink::NsSmartObjects::SmartObject* result =
    new NsSmartDeviceLink::NsSmartObjects::SmartObject;
  const uint32_t hmi_correlation_id = ApplicationManagerImpl::instance()
                                      ->GetNextHMICorrelationID();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& request = *result;
  request[strings::params][strings::message_type] = MessageType::kRequest;
  request[strings::params][strings::function_id] =
    hmi_apis::FunctionID::UI_EndAudioPassThru;
  request[strings::params][strings::correlation_id] = hmi_correlation_id;
  request[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  request[strings::params][strings::protocol_type] =
    commands::CommandImpl::hmi_protocol_type_;
  return ApplicationManagerImpl::instance()->ManageHMICommand(result);
}

void MessageHelper::SendPolicySnapshotNotification(
  unsigned int connection_key, const std::vector<uint8_t>& policy_data,
  const std::string& url, int timeout) {
  smart_objects::SmartObject* pt_notification = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  smart_objects::SmartObject& content = *pt_notification;
  ;
  content[strings::params][strings::function_id] =
    mobile_apis::FunctionID::OnSystemRequestID;
  content[strings::params][strings::message_type] =
    mobile_apis::messageType::notification;
  content[strings::params][strings::protocol_type] =
    commands::CommandImpl::mobile_protocol_type_;
  content[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  content[strings::params][strings::connection_key] = connection_key;
  if (!url.empty()) {
    content[strings::msg_params][mobile_notification::syncp_url] = url;
  }
  content[strings::msg_params][strings::file_type] =
    mobile_apis::FileType::BINARY;
  content[strings::msg_params][strings::request_type] =
    mobile_apis::RequestType::HTTP;
  /*if (-1 != timeout) {
   content[strings::msg_params][mobile_notification::syncp_timeout] = timeout;
   }*/
  content[strings::params][strings::binary_data] = smart_objects::SmartObject(
        policy_data);
  ApplicationManagerImpl::instance()->ManageMobileCommand(pt_notification);
}

void MessageHelper::SendOnPermissionsChangeNotification(
  uint32_t connection_key, const policy::Permissions& permissions) {
  smart_objects::SmartObject* notification = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  smart_objects::SmartObject& content = *notification;
  ;
  content[strings::params][strings::function_id] =
    mobile_apis::FunctionID::OnPermissionsChangeID;
  content[strings::params][strings::message_type] =
    mobile_apis::messageType::notification;
  content[strings::params][strings::protocol_type] =
    commands::CommandImpl::mobile_protocol_type_;
  content[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  content[strings::params][strings::connection_key] = connection_key;

  smart_objects::SmartObject* p_msg_params = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  smart_objects::SmartObject& msg_params = *p_msg_params;

  content[strings::msg_params] = msg_params;

  //content[strings::msg_params][strings::app_id] = connection_key;

  content[strings::msg_params]["permissionItem"] = smart_objects::SmartObject(
        smart_objects::SmartType_Array);

  smart_objects::SmartObject& permissions_item_array =
    content[strings::msg_params]["permissionItem"];

  policy::Permissions::const_iterator it_permissions = permissions.begin();
  policy::Permissions::const_iterator it_permissions_end = permissions.end();

  for (size_t index_pi = 0; it_permissions != it_permissions_end;
       ++it_permissions, ++index_pi) {

    permissions_item_array[index_pi] = smart_objects::SmartObject(
                                         smart_objects::SmartType_Map);

    smart_objects::SmartObject& permission_item =
      permissions_item_array[index_pi];

    // Filling the rpcName of PermissionItem
    permission_item["rpcName"] = (*it_permissions).first;
    const policy::RpcPermissions& rpc_permissions = (*it_permissions).second;

    // Creating SO for hmiPermissions
    permission_item["hmiPermissions"] = smart_objects::SmartObject(
                                          smart_objects::SmartType_Map);

    smart_objects::SmartObject& hmi_permissions =
      permission_item["hmiPermissions"];

    policy::HMIPermissions::const_iterator it_hmi_permissions = rpc_permissions
        .hmi_permissions.begin();
    policy::HMIPermissions::const_iterator it_hmi_permissions_end =
      rpc_permissions.hmi_permissions.end();

    // Filling the hmiPermissions of PermissionItem
    for (; it_hmi_permissions != it_hmi_permissions_end; ++it_hmi_permissions) {
      // Possible key names are "allowed", "userDisallowed"
      hmi_permissions[(*it_hmi_permissions).first] = smart_objects::SmartObject(
            smart_objects::SmartType_Array);

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
    permission_item["parameterPermissions"] = smart_objects::SmartObject(
          smart_objects::SmartType_Map);

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
    revoked_permission_items[i] = smart_objects::SmartObject(
                                    smart_objects::SmartType_Map);
    smart_objects::SmartObject& permission_item = revoked_permission_items[i];
    permission_item["name"] = permissions.appRevokedPermissions[i].
                              group_alias;

    permission_item["id"] = permissions.appRevokedPermissions[i].group_id;

    if (policy::kGroupUndefined !=
        permissions.appRevokedPermissions[i].state) {
      permission_item["allowed"] =
          policy::kGroupAllowed == permissions.appRevokedPermissions[i].state
          ? true : false;
    }
  }
}

void MessageHelper::SendOnAppPermissionsChangedNotification(
  uint32_t connection_key, const policy::AppPermissions& permissions) {
  smart_objects::SmartObject* notification = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  if (!notification) {
    return;
  }

  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
    hmi_apis::FunctionID::SDL_OnAppPermissionChanged;

  message[strings::params][strings::message_type] = MessageType::kNotification;
  message[strings::msg_params][strings::app_id] = connection_key;

  // TODO(AOleynik): Add other parameters processing from incoming struct
  if (permissions.appRevoked) {
    message[strings::msg_params]["appRevoked"] = permissions.appRevoked;
  }
  if (permissions.isAppPermissionsRevoked) {
    message[strings::msg_params]["isAppPermissionsRevoked"] = permissions
        .isAppPermissionsRevoked;

    FillAppRevokedPermissions(permissions, message);
  }

  if (permissions.appPermissionsConsentNeeded) {
    message[strings::msg_params]["appPermissionsConsentNeeded"] = permissions
        .appPermissionsConsentNeeded;
  }
  if (permissions.appUnauthorized) {
    message[strings::msg_params]["appUnauthorized"] = permissions
        .appUnauthorized;
  }
  if (!permissions.priority.empty()) {
    message[strings::msg_params]["priority"] = GetPriorityCode(
          permissions.priority);
  }

  ApplicationManagerImpl::instance()->ManageHMICommand(&message);
}

void MessageHelper::SendGetStatusUpdateResponse(const std::string& status,
    uint32_t correlation_id) {
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::SDL_GetStatusUpdate;
  (*message)[strings::params][strings::message_type] =
    MessageType::kResponse;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::params][hmi_response::code] = 0;

  (*message)[strings::msg_params]["status"] = status;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

void MessageHelper::SendUpdateSDLResponse(const std::string& result, uint32_t correlation_id) {
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::SDL_UpdateSDL;
  (*message)[strings::params][strings::message_type] =
    MessageType::kResponse;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::params][hmi_response::code] = 0;

  (*message)[strings::msg_params]["result"] = result;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}



void MessageHelper::SendOnStatusUpdate(const std::string& status) {
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
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
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
    hmi_apis::FunctionID::BasicCommunication_GetSystemInfo;
  (*message)[strings::params][strings::message_type] =
    MessageType::kRequest;
  (*message)[strings::params][strings::correlation_id] =
    ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
  (*message)[strings::params][strings::protocol_version] =
    commands::CommandImpl::protocol_version_;
  (*message)[strings::params][strings::protocol_type] =
    commands::CommandImpl::hmi_protocol_type_;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}

mobile_apis::Result::eType MessageHelper::VerifyImageFiles(
  smart_objects::SmartObject& message, ApplicationConstSharedPtr app) {
  if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == message.getType()) {
    for (uint32_t i = 0; i < message.length(); ++i) {
      mobile_apis::Result::eType res = VerifyImageFiles(message[i], app);
      if (mobile_apis::Result::SUCCESS != res) {
        return res;
      }
    }
  } else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Map
             == message.getType()) {
    if (message.keyExists(strings::image_type)) {
      mobile_apis::Result::eType verification_result = VerifyImage(message,
          app);

      if (mobile_apis::Result::SUCCESS != verification_result) {
        return verification_result;  // exit point
      }
    } else {
      std::set<std::string> keys = message.enumerate();

      for (std::set<std::string>::const_iterator key = keys.begin();
           key != keys.end(); ++key) {
        if (strings::soft_buttons != (*key)) {
          mobile_apis::Result::eType res = VerifyImageFiles(message[*key], app);
          if (mobile_apis::Result::SUCCESS != res) {
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

  std::string full_file_path =
    profile::Profile::instance()->app_storage_folder() + "/";

  if (file_name.size() > 0 && file_name[0] == '/') {
    full_file_path = file_name;
  } else {

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
      verification_result_image =  VerifyImage(message[i][strings::image], app);
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
    LOG4CXX_ERROR(logger_, "MessageHelper::VerifySoftButtonString"
                  "string contains incorrect character");
    return false;
  }
  return true;
}

mobile_apis::Result::eType MessageHelper::ProcessSoftButtons(
  smart_objects::SmartObject& message_params, ApplicationConstSharedPtr app) {
  if (!message_params.keyExists(strings::soft_buttons)) {
    return mobile_apis::Result::SUCCESS;
  }

  const HMICapabilities& hmi_capabilities = ApplicationManagerImpl::instance()
      ->hmi_capabilities();
  const smart_objects::SmartObject* soft_button_capabilities = hmi_capabilities
      .soft_button_capabilities();
  bool image_supported = false;
  if (soft_button_capabilities) {
    image_supported = (*soft_button_capabilities)[hmi_response::image_supported]
                      .asBool();
  }

  smart_objects::SmartObject& request_soft_buttons =
    message_params[strings::soft_buttons];

  // Check whether soft buttons request is well-formed
  if (!ValidateSoftButtons(request_soft_buttons)) {
    return mobile_apis::Result::INVALID_DATA;
  }

  smart_objects::SmartObject soft_buttons = smart_objects::SmartObject(
        smart_objects::SmartType_Array);

  policy::PolicyHandler* policy_handler = policy::PolicyHandler::instance();
  std::string app_mobile_id = app->mobile_app_id()->asString();

  uint32_t j = 0;
  size_t size = request_soft_buttons.length();
  for (uint32_t i = 0; i < size; ++i) {
    int system_action = request_soft_buttons[i][strings::system_action].asInt();
    if (!policy_handler->CheckKeepContext(system_action, app_mobile_id) ||
        !policy_handler->CheckStealFocus(system_action, app_mobile_id)) {
      return mobile_apis::Result::DISALLOWED;
    }

    switch (request_soft_buttons[i][strings::type].asInt()) {
      case mobile_apis::SoftButtonType::SBT_IMAGE: {
        if (!image_supported) {
          continue;
        }
        //Any text value for type "IMAGE" should be ignored.
        if (request_soft_buttons[i].keyExists(strings::text)) {
          request_soft_buttons[i].erase(strings::text);
        }
        if (request_soft_buttons[i].keyExists(strings::image)) {
          mobile_apis::Result::eType verification_result = VerifyImage(
                request_soft_buttons[i][strings::image], app);
          if (mobile_apis::Result::SUCCESS != verification_result) {
            return mobile_apis::Result::INVALID_DATA;
          }
        } else {
          return mobile_apis::Result::INVALID_DATA;
        }
        break;
      }
      case mobile_apis::SoftButtonType::SBT_TEXT: {
        if ((!request_soft_buttons[i].keyExists(strings::text)) ||
            (!VerifySoftButtonString(
                request_soft_buttons[i][strings::text].asString()))) {
          return mobile_apis::Result::INVALID_DATA;
        }
        break;
      }
      case mobile_apis::SoftButtonType::SBT_BOTH: {

        if ((!request_soft_buttons[i].keyExists(strings::text)) ||
            ((request_soft_buttons[i][strings::text].length())
                && (!VerifySoftButtonString(
                request_soft_buttons[i][strings::text].asString())))) {
          return mobile_apis::Result::INVALID_DATA;
        }

        bool image_exist = false;
        if (image_supported) {
          image_exist = request_soft_buttons[i].keyExists(strings::image);
          if (!image_exist) {
            return mobile_apis::Result::INVALID_DATA;
          }
        }
        if (image_exist) {
          mobile_apis::Result::eType verification_result = VerifyImage(
                request_soft_buttons[i][strings::image], app);

          if (mobile_apis::Result::SUCCESS != verification_result) {
            return mobile_apis::Result::INVALID_DATA;

          }
        }
        break;
      }
      default: {
        continue;
        break;
      }
    }

    soft_buttons[j] = request_soft_buttons[i];
    ++j;
  }

  request_soft_buttons = soft_buttons;

  if (0 == request_soft_buttons.length()) {
    message_params.erase(strings::soft_buttons);
  }
  return mobile_apis::Result::SUCCESS;
}

void MessageHelper::SubscribeApplicationToSoftButton(
    smart_objects::SmartObject& message_params, ApplicationSharedPtr app,
    int32_t function_id) {
  SoftButtonID softbuttons_id;
  smart_objects::SmartObject& soft_buttons = message_params[strings::soft_buttons];
  unsigned int length = soft_buttons.length();
  for(unsigned int i = 0; i < length; ++i) {
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
           key != keys.end(); key++) {
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
      printf("%" PRId64 "\n", object.asInt64());
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
