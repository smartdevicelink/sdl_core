/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Copyright (c) 2017, Livio, Inc.
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
#include <strings.h>
#include <algorithm>
#include <utility>
#include <map>
#include <functional>

#include "application_manager/application.h"
#include "application_manager/application_manager.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/rpc_service.h"
#include "connection_handler/connection_handler_impl.h"
#include "transport_manager/common.h"
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

typedef std::vector<policy::FunctionalGroupPermission> PermissionsList;

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
}

struct GroupsAppender
    : std::unary_function<void, const PermissionsList::value_type&> {
  GroupsAppender(smart_objects::SmartObject& groups)
      : groups_(groups), index_(0) {}

  void operator()(const PermissionsList::value_type& item) {
    using namespace smart_objects;
    using namespace policy;
    groups_[index_] = SmartObject(SmartType_Map);

    SmartObject& group = groups_[index_];
    group[strings::name] = item.group_alias;
    group[strings::id] = item.group_id;
    GroupConsent permission_state = item.state;
    // If state undefined, 'allowed' parameter should be absent
    if (kGroupUndefined != permission_state) {
      group["allowed"] = kGroupAllowed == permission_state;
    }
    ++index_;
  }

 private:
  smart_objects::SmartObject& groups_;
  int32_t index_;
};

#ifdef EXTERNAL_PROPRIETARY_MODE
struct ExternalConsentStatusAppender
    : std::unary_function<void,
                          const policy::ExternalConsentStatus::value_type&> {
  ExternalConsentStatusAppender(smart_objects::SmartObject& status)
      : status_(status), index_(0) {}

  void operator()(const policy::ExternalConsentStatus::value_type& item) {
    using namespace smart_objects;
    using namespace policy;
    using namespace hmi_apis;
    status_[index_] = SmartObject(SmartType_Map);

    SmartObject& external_consent_status = status_[index_];
    external_consent_status[strings::entity_type] = item.entity_type_;
    external_consent_status[strings::entity_id] = item.entity_id_;
    external_consent_status[strings::status] =
        policy::kStatusOn == item.status_
            ? static_cast<int32_t>(Common_EntityStatus::ON)
            : static_cast<int32_t>(Common_EntityStatus::OFF);
    ++index_;
  }

 private:
  smart_objects::SmartObject& status_;
  int32_t index_;
};
#endif  // EXTERNAL_PROPRIETARY_MODE

}  // namespace

std::pair<std::string,
          mobile_apis::VehicleDataType::eType> kVehicleDataInitializer[] = {
    std::make_pair(strings::gps, mobile_apis::VehicleDataType::VEHICLEDATA_GPS),
    std::make_pair(strings::speed,
                   mobile_apis::VehicleDataType::VEHICLEDATA_SPEED),
    std::make_pair(strings::rpm, mobile_apis::VehicleDataType::VEHICLEDATA_RPM),
    std::make_pair(strings::fuel_level,
                   mobile_apis::VehicleDataType::VEHICLEDATA_FUELLEVEL),
    std::make_pair(strings::fuel_level_state,
                   mobile_apis::VehicleDataType::VEHICLEDATA_FUELLEVEL_STATE),
    std::make_pair(strings::instant_fuel_consumption,
                   mobile_apis::VehicleDataType::VEHICLEDATA_FUELCONSUMPTION),
    std::make_pair(strings::fuel_range,
                   mobile_apis::VehicleDataType::VEHICLEDATA_FUELRANGE),
    std::make_pair(strings::external_temp,
                   mobile_apis::VehicleDataType::VEHICLEDATA_EXTERNTEMP),
    std::make_pair(strings::turn_signal,
                   mobile_apis::VehicleDataType::VEHICLEDATA_TURNSIGNAL),
    std::make_pair(strings::vin, mobile_apis::VehicleDataType::VEHICLEDATA_VIN),
    std::make_pair(strings::prndl,
                   mobile_apis::VehicleDataType::VEHICLEDATA_PRNDL),
    std::make_pair(strings::tire_pressure,
                   mobile_apis::VehicleDataType::VEHICLEDATA_TIREPRESSURE),
    std::make_pair(strings::odometer,
                   mobile_apis::VehicleDataType::VEHICLEDATA_ODOMETER),
    std::make_pair(strings::belt_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_BELTSTATUS),
    std::make_pair(strings::body_information,
                   mobile_apis::VehicleDataType::VEHICLEDATA_BODYINFO),
    std::make_pair(strings::device_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_DEVICESTATUS),
    std::make_pair(strings::driver_braking,
                   mobile_apis::VehicleDataType::VEHICLEDATA_BRAKING),
    std::make_pair(strings::wiper_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_WIPERSTATUS),
    std::make_pair(strings::head_lamp_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_HEADLAMPSTATUS),
    std::make_pair(strings::e_call_info,
                   mobile_apis::VehicleDataType::VEHICLEDATA_ECALLINFO),
    std::make_pair(strings::airbag_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_AIRBAGSTATUS),
    std::make_pair(strings::emergency_event,
                   mobile_apis::VehicleDataType::VEHICLEDATA_EMERGENCYEVENT),
    std::make_pair(strings::cluster_mode_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_CLUSTERMODESTATUS),
    std::make_pair(strings::my_key,
                   mobile_apis::VehicleDataType::VEHICLEDATA_MYKEY),
    /*
     NOT DEFINED in mobile API
     std::make_pair(strings::gps,
     BATTVOLTAGE),
     */
    std::make_pair(strings::engine_torque,
                   mobile_apis::VehicleDataType::VEHICLEDATA_ENGINETORQUE),
    std::make_pair(strings::acc_pedal_pos,
                   mobile_apis::VehicleDataType::VEHICLEDATA_ACCPEDAL),
    std::make_pair(strings::steering_wheel_angle,
                   mobile_apis::VehicleDataType::VEHICLEDATA_STEERINGWHEEL),
    std::make_pair(strings::engine_oil_life,
                   mobile_apis::VehicleDataType::VEHICLEDATA_ENGINEOILLIFE)};

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

std::string MessageHelper::GetDeviceMacAddressForHandle(
    const transport_manager::DeviceHandle device_handle,
    const ApplicationManager& app_mngr) {
  std::string device_mac_address = "";
  app_mngr.connection_handler().get_session_observer().GetDataOnDeviceID(
      device_handle, NULL, NULL, &device_mac_address);
  LOG4CXX_DEBUG(logger_, "result : " << device_handle);
  return device_mac_address;
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

smart_objects::SmartObjectSPtr MessageHelper::CreateMessageForHMI(
    hmi_apis::messageType::eType message_type, const uint32_t correlation_id) {
  using namespace smart_objects;

  SmartObjectSPtr message = utils::MakeShared<SmartObject>(SmartType_Map);
  SmartObject& ref = *message;

  ref[strings::params][strings::message_type] = static_cast<int>(message_type);
  ref[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
  ref[strings::params][strings::protocol_type] =
      commands::CommandImpl::hmi_protocol_type_;
  ref[strings::params][strings::correlation_id] = correlation_id;
  return message;
}

smart_objects::SmartObjectSPtr MessageHelper::CreateHashUpdateNotification(
    const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr message =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  (*message)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnHashChangeID;
  (*message)[strings::params][strings::connection_key] = app_id;
  (*message)[strings::params][strings::message_type] =
      static_cast<int32_t>(kNotification);
  return message;
}

void MessageHelper::SendDecryptCertificateToHMI(const std::string& file_name,
                                                ApplicationManager& app_mngr) {
  using namespace smart_objects;
  SmartObjectSPtr message = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
  DCHECK(message);

  SmartObject& object = *message;
  object[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_DecryptCertificate;

  SmartObject msg_params = SmartObject(SmartType_Map);

  msg_params[hmi_request::file_name] = file_name;
  object[strings::msg_params] = msg_params;

  app_mngr.GetRPCService().ManageHMICommand(message);
}

void MessageHelper::SendGetSystemTimeRequest(const uint32_t correlation_id,
                                             ApplicationManager& app_mngr) {
  using namespace smart_objects;
  SmartObjectSPtr message =
      CreateMessageForHMI(hmi_apis::messageType::request, correlation_id);

  DCHECK(message);

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_GetSystemTime;

  app_mngr.GetRPCService().ManageHMICommand(message);
}

void MessageHelper::SendHashUpdateNotification(const uint32_t app_id,
                                               ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr app = app_mngr.application(app_id);
  if (!app) {
    LOG4CXX_ERROR(logger_, "Application not found by appID");
    return;
  }
  smart_objects::SmartObjectSPtr so = CreateHashUpdateNotification(app_id);
  PrintSmartObject(*so);
  if (!app_mngr.GetRPCService().ManageMobileCommand(
          so, commands::Command::SOURCE_SDL)) {
    LOG4CXX_ERROR(logger_, "Failed to send HashUpdate notification.");
    return;
  }
  app_mngr.resume_controller().ApplicationsDataUpdated();
}

smart_objects::SmartObjectSPtr
MessageHelper::GetOnAppInterfaceUnregisteredNotificationToMobile(
    int32_t connection_key,
    mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObjectSPtr notification =
      utils::MakeShared<smart_objects::SmartObject>();
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] = static_cast<int32_t>(
      mobile_api::FunctionID::OnAppInterfaceUnregisteredID);

  message[strings::params][strings::message_type] =
      static_cast<int32_t>(kNotification);

  message[strings::params][strings::connection_key] = connection_key;

  message[strings::msg_params][strings::reason] = static_cast<int32_t>(reason);

  return notification;
}

void MessageHelper::SendDeleteCommandRequest(smart_objects::SmartObject* cmd,
                                             ApplicationSharedPtr application,
                                             ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(cmd);
  using namespace smart_objects;
  SmartObject msg_params = SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::cmd_id] = (*cmd)[strings::cmd_id];
  msg_params[strings::app_id] = application->app_id();

  if ((*cmd).keyExists(strings::menu_params)) {
    SmartObjectSPtr message = CreateMessageForHMI(
        hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
    DCHECK(message);

    SmartObject& object = *message;
    object[strings::params][strings::function_id] =
        hmi_apis::FunctionID::UI_DeleteCommand;

    object[strings::msg_params] = msg_params;

    app_mngr.GetRPCService().ManageHMICommand(message);
  }

  if ((*cmd).keyExists(strings::vr_commands)) {
    msg_params[strings::grammar_id] = application->get_grammar_id();
    msg_params[strings::type] = hmi_apis::Common_VRCommandType::Command;

    SmartObjectSPtr message = CreateMessageForHMI(
        hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
    DCHECK(message);

    SmartObject& object = *message;
    object[strings::params][strings::function_id] =
        hmi_apis::FunctionID::VR_DeleteCommand;

    object[strings::msg_params] = msg_params;

    app_mngr.GetRPCService().ManageHMICommand(message);
  }
}

void MessageHelper::SendDeleteSubmenuRequest(smart_objects::SmartObject* cmd,
                                             ApplicationSharedPtr application,
                                             ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(cmd);
  using namespace smart_objects;

  SmartObject msg_params = SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::menu_id] = (*cmd)[strings::menu_id];
  msg_params[strings::app_id] = application->app_id();

  SmartObjectSPtr message = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
  DCHECK(message);

  SmartObject& object = *message;
  object[strings::params][strings::function_id] =
      hmi_apis::FunctionID::UI_DeleteSubMenu;

  object[strings::msg_params] = msg_params;

  app_mngr.GetRPCService().ManageHMICommand(message);

  const DataAccessor<CommandsMap> accessor = application->commands_map();
  const CommandsMap& commands = accessor.GetData();
  CommandsMap::const_iterator it = commands.begin();

  for (; commands.end() != it; ++it) {
    if (!(*it->second).keyExists(strings::vr_commands)) {
      continue;
    }

    if ((*cmd)[strings::menu_id].asInt() ==
        (*it->second)[strings::menu_params][hmi_request::parent_id].asInt()) {
      SmartObject msg_params = SmartObject(smart_objects::SmartType_Map);
      msg_params[strings::cmd_id] = (*it->second)[strings::cmd_id].asInt();
      msg_params[strings::app_id] = application->app_id();
      msg_params[strings::grammar_id] = application->get_grammar_id();
      msg_params[strings::type] = hmi_apis::Common_VRCommandType::Command;

      SmartObjectSPtr message = CreateMessageForHMI(
          hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
      DCHECK(message);

      SmartObject& object = *message;
      object[strings::params][strings::function_id] =
          hmi_apis::FunctionID::VR_DeleteCommand;

      object[strings::msg_params] = msg_params;

      app_mngr.GetRPCService().ManageHMICommand(message);
    }
  }
}

void MessageHelper::SendDeleteChoiceSetRequest(smart_objects::SmartObject* cmd,
                                               ApplicationSharedPtr application,
                                               ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(cmd);
  using namespace smart_objects;

  // Same is deleted with SendDeleteCommandRequest?

  SmartObject msg_params = SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::app_id] = application->app_id();
  msg_params[strings::type] = hmi_apis::Common_VRCommandType::Choice;
  msg_params[strings::grammar_id] = (*cmd)[strings::grammar_id];
  cmd = &((*cmd)[strings::choice_set]);
  for (uint32_t i = 0; i < (*cmd).length(); ++i) {
    msg_params[strings::cmd_id] = (*cmd)[i][strings::choice_id];

    SmartObjectSPtr message = CreateMessageForHMI(
        hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
    DCHECK(message);

    SmartObject& object = *message;
    object[strings::params][strings::function_id] =
        hmi_apis::FunctionID::VR_DeleteCommand;

    object[strings::msg_params] = msg_params;

    app_mngr.GetRPCService().ManageHMICommand(message);
  }
}

void MessageHelper::SendResetPropertiesRequest(ApplicationSharedPtr application,
                                               ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace smart_objects;

  {
    SmartObject msg_params = SmartObject(smart_objects::SmartType_Map);

    msg_params = *MessageHelper::CreateAppVrHelp(application);
    msg_params[hmi_request::menu_title] = "";

    smart_objects::SmartObject key_board_properties =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    key_board_properties[strings::language] =
        static_cast<int32_t>(hmi_apis::Common_Language::EN_US);
    key_board_properties[hmi_request::keyboard_layout] =
        static_cast<int32_t>(hmi_apis::Common_KeyboardLayout::QWERTY);

    key_board_properties[hmi_request::auto_complete_text] = "";
    msg_params[hmi_request::keyboard_properties] = key_board_properties;

    msg_params[strings::app_id] = application->app_id();

    SmartObjectSPtr message = CreateMessageForHMI(
        hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
    DCHECK(message);

    SmartObject& object = *message;
    object[strings::params][strings::function_id] =
        hmi_apis::FunctionID::UI_SetGlobalProperties;

    object[strings::msg_params] = msg_params;

    app_mngr.GetRPCService().ManageHMICommand(message);
  }

  {
    SmartObject msg_params = SmartObject(smart_objects::SmartType_Map);

    msg_params[strings::help_prompt] = application->help_prompt();
    msg_params[strings::timeout_prompt] = application->timeout_prompt();
    msg_params[strings::app_id] = application->app_id();

    SmartObjectSPtr message = CreateMessageForHMI(
        hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
    DCHECK(message);

    SmartObject& object = *message;
    object[strings::params][strings::function_id] =
        hmi_apis::FunctionID::TTS_SetGlobalProperties;

    object[strings::msg_params] = msg_params;

    app_mngr.GetRPCService().ManageHMICommand(message);
  }
}

void MessageHelper::SendUnsubscribeButtonNotification(
    mobile_apis::ButtonName::eType button,
    ApplicationSharedPtr application,
    ApplicationManager& app_mngr) {
  using namespace smart_objects;
  using namespace hmi_apis;

  SmartObject msg_params = SmartObject(SmartType_Map);
  msg_params[strings::app_id] = application->app_id();
  msg_params[strings::name] = button;
  msg_params[strings::is_suscribed] = false;

  SmartObjectSPtr message = CreateMessageForHMI(
      hmi_apis::messageType::notification, app_mngr.GetNextHMICorrelationID());
  DCHECK(message);

  SmartObject& object = *message;
  object[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Buttons_OnButtonSubscription;

  object[strings::msg_params] = msg_params;

  app_mngr.GetRPCService().ManageHMICommand(message);
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

void MessageHelper::SendHMIStatusNotification(
    const Application& application_impl,
    ApplicationManager& application_manager) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr notification = new smart_objects::SmartObject;
  if (!notification) {
    LOG4CXX_ERROR(logger_, "Failed to create smart object");
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

  application_manager.GetRPCService().ManageMobileCommand(
      notification, commands::Command::SOURCE_SDL);
}

void MessageHelper::SendActivateAppToHMI(
    uint32_t const app_id,
    ApplicationManager& application_manager,
    hmi_apis::Common_HMILevel::eType level,
    bool send_policy_priority) {
  application_manager::ApplicationConstSharedPtr app =
      application_manager.application(app_id);
  if (!app) {
    LOG4CXX_WARN(logger_, "Invalid app_id: " << app_id);
    return;
  }

  utils::SharedPtr<smart_objects::SmartObject> message =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_ActivateApp;
  (*message)[strings::params][strings::message_type] = MessageType::kRequest;
  (*message)[strings::params][strings::correlation_id] =
      application_manager.GetNextHMICorrelationID();
  (*message)[strings::msg_params][strings::app_id] = app_id;

  if (send_policy_priority) {
    std::string priority;
    application_manager.GetPolicyHandler().GetPriority(app->policy_app_id(),
                                                       &priority);
    // According SDLAQ-CRS-2794
    // SDL have to send ActivateApp without "proirity" parameter to HMI.
    // in case of unconsented device
    std::string mac_adress;
    connection_handler::DeviceHandle device_handle = app->device();
    application_manager.connection_handler()
        .get_session_observer()
        .GetDataOnDeviceID(device_handle, NULL, NULL, &mac_adress, NULL);

    policy::DeviceConsent consent =
        application_manager.GetPolicyHandler().GetUserConsentForDevice(
            mac_adress);
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

  application_manager.GetRPCService().ManageHMICommand(message);
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
    const mobile_apis::HMILevel::eType hmi_level) {
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
    const uint32_t correlation_id,
    const std::vector<std::string>& params,
    ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
#ifdef HMI_DBUS_API
  for (std::vector<std::string>::const_iterator it = params.begin();
       it != params.end();
       it++) {
    smart_objects::SmartObjectSPtr request =
        utils::MakeShared<smart_objects::SmartObject>();

    (*request)[strings::params][strings::message_type] =
        static_cast<int>(kRequest);
    (*request)[strings::params][strings::correlation_id] = correlation_id;
    (*request)[strings::params][strings::protocol_version] =
        commands::CommandImpl::protocol_version_;
    (*request)[strings::params][strings::protocol_type] =
        commands::CommandImpl::hmi_protocol_type_;
    (*request)[strings::params][strings::function_id] =
        static_cast<int>(vehicle_data_args[*it]);
    app_mngr.GetRPCService().ManageHMICommand(request);
  }
#else

  smart_objects::SmartObjectSPtr request =
      utils::MakeShared<smart_objects::SmartObject>();

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
  app_mngr.GetRPCService().ManageHMICommand(request);
#endif
}

smart_objects::SmartObjectSPtr MessageHelper::CreateBlockedByPoliciesResponse(
    mobile_apis::FunctionID::eType function_id,
    mobile_apis::Result::eType result,
    const uint32_t correlation_id,
    uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr response =
      utils::MakeShared<smart_objects::SmartObject>();

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
  (*response)[strings::params][strings::protocol_version] = static_cast<int>(
      protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_2);
  return response;
}

smart_objects::SmartObjectSPtr MessageHelper::CreateDeviceListSO(
    const connection_handler::DeviceMap& devices,
    const policy::PolicyHandlerInterface& policy_handler,
    ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr device_list_so =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  (*device_list_so)[strings::device_list] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  smart_objects::SmartObject& list_so = (*device_list_so)[strings::device_list];

  uint32_t index = 0;
  // According to requirements, SDL should send info about 100 devices at
  // maximum, even if SDL has more devices connected.
  const uint32_t max_device_count = 100;
  for (connection_handler::DeviceMap::const_iterator it = devices.begin();
       devices.end() != it && index < max_device_count;
       ++it) {
    const connection_handler::Device& d =
        static_cast<connection_handler::Device>(it->second);
    list_so[index][strings::name] = d.user_friendly_name();
    list_so[index][strings::id] = it->second.mac_address();

    const policy::DeviceConsent device_consent =
        policy_handler.GetUserConsentForDevice(it->second.mac_address());
    list_so[index][strings::isSDLAllowed] =
        policy::DeviceConsent::kDeviceAllowed == device_consent;
    list_so[index][strings::transport_type] =
        app_mngr.GetDeviceTransportType(d.connection_type());
    ++index;
  }
  return device_list_so;
}

smart_objects::SmartObjectSPtr MessageHelper::CreateModuleInfoSO(
    uint32_t function_id, ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr module_info =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  smart_objects::SmartObject& object = *module_info;
  object[strings::params][strings::message_type] = static_cast<int>(kRequest);
  object[strings::params][strings::function_id] = static_cast<int>(function_id);
  object[strings::params][strings::correlation_id] =
      app_mngr.GetNextHMICorrelationID();
  object[strings::msg_params] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  return module_info;
}

smart_objects::SmartObjectSPtr MessageHelper::CreateSetAppIcon(
    const std::string& path_to_icon, uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr set_icon =
      utils::MakeShared<smart_objects::SmartObject>(
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

void MessageHelper::SendOnButtonSubscriptionNotification(
    uint32_t app_id,
    hmi_apis::Common_ButtonName::eType button,
    bool is_subscribed,
    ApplicationManager& app_mngr) {
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

  if (!app_mngr.GetRPCService().ManageHMICommand(notification_ptr)) {
    LOG4CXX_ERROR(logger_, "Unable to send HMI notification");
  }
}

void MessageHelper::SendAllOnButtonSubscriptionNotificationsForApp(
    ApplicationConstSharedPtr app, ApplicationManager& app_mngr) {
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
        app->hmi_app_id(),
        static_cast<Common_ButtonName::eType>(*it),
        true,
        app_mngr);
  }
}

void MessageHelper::SendSetAppIcon(
    const uint32_t app_id,
    const std::string& icon_path,
    application_manager::ApplicationManager& application_manager) {
  using namespace smart_objects;

  SmartObjectSPtr set_app_icon =
      CreateMessageForHMI(hmi_apis::messageType::request,
                          application_manager.GetNextHMICorrelationID());

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
    application_manager.GetRPCService().ManageHMICommand(set_app_icon);
  }
}

void MessageHelper::SendAppDataToHMI(ApplicationConstSharedPtr app,
                                     ApplicationManager& app_man) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (app) {
    SendSetAppIcon(app->app_id(), app->app_icon_path(), app_man);
    SendGlobalPropertiesToHMI(app, app_man);
    SendShowRequestToHMI(app, app_man);
  }
}

void MessageHelper::SendGlobalPropertiesToHMI(ApplicationConstSharedPtr app,
                                              ApplicationManager& app_mngr) {
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Invalid application");
    return;
  }

  smart_objects::SmartObjectList requests = CreateGlobalPropertiesRequestsToHMI(
      app, app_mngr.GetNextHMICorrelationID());
  for (smart_objects::SmartObjectList::const_iterator it = requests.begin();
       it != requests.end();
       ++it) {
    DCHECK(app_mngr.GetRPCService().ManageHMICommand(*it));
  }
}

smart_objects::SmartObjectList
MessageHelper::CreateGlobalPropertiesRequestsToHMI(
    ApplicationConstSharedPtr app, const uint32_t correlation_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObjectList requests;
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Invalid application");
    return requests;
  }

  // UI global properties

  if (app->vr_help_title() || app->vr_help()) {
    smart_objects::SmartObjectSPtr ui_global_properties =
        CreateMessageForHMI(hmi_apis::messageType::request, correlation_id);
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
        CreateMessageForHMI(hmi_apis::messageType::request, correlation_id);
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
                                            bool default_help_prompt,
                                            ApplicationManager& app_man) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!app) {
    return;
  }
  smart_objects::SmartObjectSPtr tts_global_properties = CreateMessageForHMI(
      hmi_apis::messageType::request, app_man.GetNextHMICorrelationID());

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
    app_man.GetRPCService().ManageHMICommand(tts_global_properties);
  }
}

smart_objects::SmartObjectSPtr MessageHelper::CreateAppVrHelp(
    ApplicationConstSharedPtr app) {
  smart_objects::SmartObjectSPtr result =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
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
    ApplicationConstSharedPtr app, const uint32_t correlation_id) {
  smart_objects::SmartObjectList requests;
  if (!app) {
    LOG4CXX_ERROR(logger_, "Invalid application");
    return requests;
  }

  if (app->show_command()) {
    smart_objects::SmartObjectSPtr ui_show =
        CreateMessageForHMI(hmi_apis::messageType::request, correlation_id);
    (*ui_show)[strings::params][strings::function_id] =
        static_cast<int>(hmi_apis::FunctionID::UI_Show);

    (*ui_show)[strings::msg_params] = (*app->show_command());
    requests.push_back(ui_show);
  }
  return requests;
}

void MessageHelper::SendShowRequestToHMI(ApplicationConstSharedPtr app,
                                         ApplicationManager& app_mngr) {
  if (!app) {
    return;
  }
  smart_objects::SmartObjectList shows =
      CreateShowRequestToHMI(app, app_mngr.GetNextHMICorrelationID());

  for (smart_objects::SmartObjectList::const_iterator it = shows.begin();
       it != shows.end();
       ++it) {
    DCHECK(app_mngr.GetRPCService().ManageHMICommand(*it));
  }
}

void MessageHelper::SendShowConstantTBTRequestToHMI(
    ApplicationConstSharedPtr app, ApplicationManager& app_man) {
  if (!app) {
    return;
  }

  if (app->tbt_show_command()) {
    smart_objects::SmartObjectSPtr navi_show_tbt = CreateMessageForHMI(
        hmi_apis::messageType::request, app_man.GetNextHMICorrelationID());
    if (!navi_show_tbt) {
      return;
    }
    (*navi_show_tbt)[strings::params][strings::function_id] =
        static_cast<int>(hmi_apis::FunctionID::Navigation_ShowConstantTBT);

    (*navi_show_tbt)[strings::msg_params] = (*app->tbt_show_command());
    DCHECK(app_man.GetRPCService().ManageHMICommand(navi_show_tbt));
  }
}

void MessageHelper::SendAddCommandRequestToHMI(ApplicationConstSharedPtr app,
                                               ApplicationManager& app_man) {
  if (!app) {
    return;
  }
  smart_objects::SmartObjectList requests =
      CreateAddCommandRequestToHMI(app, app_man);
  for (smart_objects::SmartObjectList::iterator it = requests.begin();
       it != requests.end();
       ++it) {
    DCHECK(app_man.GetRPCService().ManageHMICommand(*it));
  }
}

smart_objects::SmartObjectList MessageHelper::CreateAddCommandRequestToHMI(
    ApplicationConstSharedPtr app, ApplicationManager& app_mngr) {
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
      smart_objects::SmartObjectSPtr ui_command = CreateMessageForHMI(
          hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
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
      SendAddVRCommandToHMI(i->first,
                            (*i->second)[strings::vr_commands],
                            app->app_id(),
                            app_mngr);
    }
  }
  return requests;
}

smart_objects::SmartObjectList
MessageHelper::CreateAddVRCommandRequestFromChoiceToHMI(
    ApplicationConstSharedPtr app, ApplicationManager& app_mngr) {
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
      smart_objects::SmartObjectSPtr vr_command = CreateMessageForHMI(
          hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
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
    const smart_objects::SmartObject* app_types,
    ApplicationManager& app_mngr) {
  smart_objects::SmartObjectSPtr command = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
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
    ApplicationConstSharedPtr app, ApplicationManager& app_mngr) {
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Application is not valid");
    return;
  }

  if (NULL != app->app_types()) {
    smart_objects::SmartObjectSPtr ui_command =
        CreateChangeRegistration(hmi_apis::FunctionID::UI_ChangeRegistration,
                                 app->ui_language(),
                                 app->app_id(),
                                 app->app_types(),
                                 app_mngr);

    if (ui_command) {
      app_mngr.GetRPCService().ManageHMICommand(ui_command);
    }
  }
}

void MessageHelper::SendAddVRCommandToHMI(
    const uint32_t cmd_id,
    const smart_objects::SmartObject& vr_commands,
    const uint32_t app_id,
    ApplicationManager& app_mngr) {
  smart_objects::SmartObjectSPtr request =
      CreateAddVRCommandToHMI(cmd_id, vr_commands, app_id, app_mngr);
  DCHECK(app_mngr.GetRPCService().ManageHMICommand(request));
}

smart_objects::SmartObjectSPtr MessageHelper::CreateAddVRCommandToHMI(
    const uint32_t cmd_id,
    const smart_objects::SmartObject& vr_commands,
    uint32_t app_id,
    ApplicationManager& app_mngr) {
  smart_objects::SmartObjectSPtr vr_command = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
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
      app_mngr.application(app_id)->get_grammar_id();
  msg_params[strings::type] = hmi_apis::Common_VRCommandType::Command;

  (*vr_command)[strings::msg_params] = msg_params;

  return vr_command;
}

bool MessageHelper::CreateHMIApplicationStruct(
    ApplicationConstSharedPtr app,
    const protocol_handler::SessionObserver& session_observer,
    const policy::PolicyHandlerInterface& policy_handler,
    NsSmartDeviceLink::NsSmartObjects::SmartObject* output,
    ApplicationManager& app_mngr) {
  using NsSmartDeviceLink::NsSmartObjects::SmartObject;
  DCHECK_OR_RETURN(output, false);
  SmartObject& message = *output;

  if (!app) {
    LOG4CXX_WARN(logger_, "Application is not valid");
    return false;
  }

  const smart_objects::SmartObject* app_types = app->app_types();
  const smart_objects::SmartObject* ngn_media_screen_name =
      app->ngn_media_screen_name();
  const smart_objects::SmartObject* day_color_scheme = app->day_color_scheme();
  const smart_objects::SmartObject* night_color_scheme =
      app->night_color_scheme();
  std::string device_name;
  std::string mac_address;
  std::string transport_type;
  if (-1 ==
      session_observer.GetDataOnDeviceID(
          app->device(), &device_name, NULL, &mac_address, &transport_type)) {
    LOG4CXX_ERROR(logger_,
                  "Failed to extract information for device " << app->device());
  }

  message = smart_objects::SmartObject(smart_objects::SmartType_Map);
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

  if (day_color_scheme) {
    message[strings::day_color_scheme] = *day_color_scheme;
  }

  if (night_color_scheme) {
    message[strings::night_color_scheme] = *night_color_scheme;
  }

  message[strings::device_info] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  message[strings::device_info][strings::name] = device_name;
  message[strings::device_info][strings::id] = mac_address;
  const policy::DeviceConsent device_consent =
      policy_handler.GetUserConsentForDevice(mac_address);
  message[strings::device_info][strings::isSDLAllowed] =
      policy::DeviceConsent::kDeviceAllowed == device_consent;

  message[strings::device_info][strings::transport_type] =
      app_mngr.GetDeviceTransportType(transport_type);
  return true;
}

void MessageHelper::SendAddSubMenuRequestToHMI(ApplicationConstSharedPtr app,
                                               ApplicationManager& app_mngr) {
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Invalid application");
    return;
  }

  smart_objects::SmartObjectList requests =
      CreateAddSubMenuRequestToHMI(app, app_mngr.GetNextHMICorrelationID());
  for (smart_objects::SmartObjectList::iterator it = requests.begin();
       it != requests.end();
       ++it) {
    DCHECK(app_mngr.GetRPCService().ManageHMICommand(*it));
  }
}

smart_objects::SmartObjectList MessageHelper::CreateAddSubMenuRequestToHMI(
    ApplicationConstSharedPtr app, const uint32_t correlation_id) {
  smart_objects::SmartObjectList requsets;
  const DataAccessor<SubMenuMap> accessor = app->sub_menu_map();
  const SubMenuMap& sub_menu = accessor.GetData();
  SubMenuMap::const_iterator i = sub_menu.begin();
  for (; sub_menu.end() != i; ++i) {
    smart_objects::SmartObjectSPtr ui_sub_menu =
        CreateMessageForHMI(hmi_apis::messageType::request, correlation_id);
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
    if (((*i->second).keyExists(strings::menu_icon)) &&
        (0 < (*i->second)[strings::menu_icon][strings::value].length())) {
      msg_params[strings::menu_icon] = (*i->second)[strings::menu_icon];
      msg_params[strings::menu_icon][strings::value] =
          (*i->second)[strings::menu_icon][strings::value].asString();
    }
    requsets.push_back(ui_sub_menu);
  }
  return requsets;
}

void MessageHelper::SendOnAppUnregNotificationToHMI(
    ApplicationConstSharedPtr app,
    bool is_unexpected_disconnect,
    ApplicationManager& app_mngr) {
  smart_objects::SmartObjectSPtr notification =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered;

  message[strings::params][strings::message_type] = MessageType::kNotification;
  // we put hmi_app_id because applicaton list does not contain application on
  // this momment
  // and ReplaceHMIWithMobileAppId function will be unable to replace app_id to
  // hmi_app_id
  message[strings::msg_params][strings::app_id] = app->hmi_app_id();
  message[strings::msg_params][strings::unexpected_disconnect] =
      is_unexpected_disconnect;
  app_mngr.GetRPCService().ManageHMICommand(notification);
}

smart_objects::SmartObjectSPtr MessageHelper::GetBCActivateAppRequestToHMI(
    ApplicationConstSharedPtr app,
    const protocol_handler::SessionObserver& session_observer,
    const policy::PolicyHandlerInterface& policy_handler,
    hmi_apis::Common_HMILevel::eType level,
    bool send_policy_priority,
    ApplicationManager& app_mngr) {
  DCHECK_OR_RETURN(app, smart_objects::SmartObjectSPtr());

  const uint32_t correlation_id = app_mngr.GetNextHMICorrelationID();
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
    policy_handler.GetPriority(app->policy_app_id(), &priority);
    // According SDLAQ-CRS-2794
    // SDL have to send ActivateApp without "proirity" parameter to HMI.
    // in case of unconsented device
    const std::string& mac_adress = app->mac_address();

    policy::DeviceConsent consent =
        policy_handler.GetUserConsentForDevice(mac_adress);
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

void MessageHelper::SendOnResumeAudioSourceToHMI(const uint32_t app_id,
                                                 ApplicationManager& app_mngr) {
  LOG4CXX_WARN(logger_, "SendOnResumeAudioSourceToHMI app_id: " << app_id);
  application_manager::ApplicationConstSharedPtr app =
      app_mngr.application(app_id);
  if (!app) {
    LOG4CXX_WARN(logger_, "Invalid app_id: " << app_id);
    return;
  }

  utils::SharedPtr<smart_objects::SmartObject> message =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnResumeAudioSource;
  (*message)[strings::params][strings::message_type] =
      MessageType::kNotification;
  (*message)[strings::params][strings::correlation_id] =
      app_mngr.GetNextHMICorrelationID();
  (*message)[strings::msg_params][strings::app_id] = app_id;

  app_mngr.GetRPCService().ManageHMICommand(message);
}

void MessageHelper::SendSDLActivateAppResponse(
    policy::AppPermissions& permissions,
    const uint32_t correlation_id,
    ApplicationManager& app_mngr) {
  smart_objects::SmartObjectSPtr message =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

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

  // If application is revoked it should not be activated
  if (permissions.appRevoked) {
    (*message)[strings::params][hmi_response::code] =
        hmi_apis::Common_Result::REJECTED;
  }

  app_mngr.GetRPCService().ManageHMICommand(message);
}

void MessageHelper::SendOnSDLConsentNeeded(
    const policy::DeviceParams& device_info, ApplicationManager& app_man) {
  smart_objects::SmartObjectSPtr message =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_OnSDLConsentNeeded;
  (*message)[strings::params][strings::message_type] =
      MessageType::kNotification;

  (*message)[strings::msg_params]["device"]["id"] =
      device_info.device_mac_address;
  (*message)[strings::msg_params]["device"]["name"] = device_info.device_name;

  app_man.GetRPCService().ManageHMICommand(message);
}

void MessageHelper::SendPolicyUpdate(const std::string& file_path,
                                     const uint32_t timeout,
                                     const std::vector<int>& retries,
                                     ApplicationManager& app_mngr) {
  smart_objects::SmartObjectSPtr message = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
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
  app_mngr.GetRPCService().ManageHMICommand(message);
}
void MessageHelper::SendGetUserFriendlyMessageResponse(
    const std::vector<policy::UserFriendlyMessage>& msg,
    const uint32_t correlation_id,
    ApplicationManager& app_mngr) {
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
    app_mngr.GetRPCService().ManageHMICommand(message);
    return;
  }

  const std::string messages = "messages";
  (*message)[strings::msg_params][messages] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  smart_objects::SmartObject& user_friendly_messages =
      (*message)[strings::msg_params][messages];
#ifdef EXTERNAL_PROPRIETARY_MODE
  const std::string tts = "ttsString";
  const std::string label = "label";
  const std::string line1 = "line1";
  const std::string line2 = "line2";
  const std::string textBody = "textBody";
#endif  // EXTERNAL_PROPRIETARY_MODE
  const std::string message_code = "messageCode";
  std::vector<policy::UserFriendlyMessage>::const_iterator it = msg.begin();
  std::vector<policy::UserFriendlyMessage>::const_iterator it_end = msg.end();
  for (uint32_t index = 0; it != it_end; ++it, ++index) {
    user_friendly_messages[index] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    smart_objects::SmartObject& obj = user_friendly_messages[index];
    obj[message_code] = it->message_code;
#ifdef EXTERNAL_PROPRIETARY_MODE
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
#endif  // EXTERNAL_PROPRIETARY_MODE
  }

  app_mngr.GetRPCService().ManageHMICommand(message);
}

#ifdef EXTERNAL_PROPRIETARY_MODE
void MessageHelper::SendGetListOfPermissionsResponse(
    const std::vector<policy::FunctionalGroupPermission>& permissions,
    const policy::ExternalConsentStatus& external_consent_status,
    uint32_t correlation_id,
    ApplicationManager& app_mngr) {
  using namespace smart_objects;
  using namespace hmi_apis;

  SmartObjectSPtr message = utils::MakeShared<SmartObject>(SmartType_Map);
  DCHECK_OR_RETURN_VOID(message);

  SmartObject& params = (*message)[strings::params];

  params[strings::function_id] = FunctionID::SDL_GetListOfPermissions;
  params[strings::message_type] = MessageType::kResponse;
  params[strings::correlation_id] = correlation_id;
  params[hmi_response::code] = static_cast<int32_t>(Common_Result::SUCCESS);

  SmartObject& msg_params = (*message)[strings::msg_params];

  const std::string allowed_functions = "allowedFunctions";
  msg_params[allowed_functions] = SmartObject(SmartType_Array);

  SmartObject& allowed_functions_array = msg_params[allowed_functions];

  GroupsAppender groups_appender(allowed_functions_array);
  std::for_each(permissions.begin(), permissions.end(), groups_appender);

  const std::string external_consent_status_key = "externalConsentStatus";
  msg_params[external_consent_status_key] = SmartObject(SmartType_Array);

  SmartObject& external_consent_status_array =
      msg_params[external_consent_status_key];

  ExternalConsentStatusAppender external_consent_status_appender(
      external_consent_status_array);
  std::for_each(external_consent_status.begin(),
                external_consent_status.end(),
                external_consent_status_appender);

  app_mngr.GetRPCService().ManageHMICommand(message);
}
#else
void MessageHelper::SendGetListOfPermissionsResponse(
    const std::vector<policy::FunctionalGroupPermission>& permissions,
    uint32_t correlation_id,
    ApplicationManager& app_mngr) {
  using namespace smart_objects;
  using namespace hmi_apis;

  SmartObjectSPtr message = utils::MakeShared<SmartObject>(SmartType_Map);
  DCHECK_OR_RETURN_VOID(message);

  SmartObject& params = (*message)[strings::params];

  params[strings::function_id] = FunctionID::SDL_GetListOfPermissions;
  params[strings::message_type] = MessageType::kResponse;
  params[strings::correlation_id] = correlation_id;
  params[hmi_response::code] = static_cast<int32_t>(Common_Result::SUCCESS);

  SmartObject& msg_params = (*message)[strings::msg_params];

  const std::string allowed_functions = "allowedFunctions";
  msg_params[allowed_functions] = SmartObject(SmartType_Array);

  SmartObject& allowed_functions_array = msg_params[allowed_functions];

  GroupsAppender groups_appender(allowed_functions_array);
  std::for_each(permissions.begin(), permissions.end(), groups_appender);
  app_mngr.GetRPCService().ManageHMICommand(message);
}
#endif  // EXTERNAL_PROPRIETARY_MODE

smart_objects::SmartObjectSPtr MessageHelper::CreateNegativeResponse(
    uint32_t connection_key,
    int32_t function_id,
    const uint32_t correlation_id,
    int32_t result_code) {
  smart_objects::SmartObject response_data(smart_objects::SmartType_Map);
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

  return utils::MakeShared<smart_objects::SmartObject>(response_data);
}

void MessageHelper::SendNaviSetVideoConfig(
    int32_t app_id,
    ApplicationManager& app_mngr,
    const smart_objects::SmartObject& video_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr request = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
  if (!request) {
    return;
  }

  (*request)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Navigation_SetVideoConfig;

  (*request)[strings::msg_params][strings::app_id] = app_id;
  (*request)[strings::msg_params][strings::config] = video_params;

  app_mngr.GetRPCService().ManageHMICommand(request);
}

void MessageHelper::SendNaviStartStream(const int32_t app_id,
                                        ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr start_stream = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
  if (!start_stream) {
    return;
  }

  (*start_stream)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Navigation_StartStream;

  char url[100] = {'\0'};
  if ("socket" == app_mngr.get_settings().video_server_type()) {
    snprintf(url,
             sizeof(url) / sizeof(url[0]),
             "http://%s:%d",
             app_mngr.get_settings().server_address().c_str(),
             app_mngr.get_settings().video_streaming_port());
  } else if ("pipe" == app_mngr.get_settings().video_server_type()) {
    snprintf(url,
             sizeof(url) / sizeof(url[0]),
             "%s",
             app_mngr.get_settings().named_video_pipe_path().c_str());
  } else {
    int snprintf_result =
        snprintf(url,
                 sizeof(url) / sizeof(url[0]),
                 "%s",
                 app_mngr.get_settings().video_stream_file().c_str());
    DCHECK(snprintf_result);
  }

  (*start_stream)[strings::msg_params][strings::app_id] = app_id;
  (*start_stream)[strings::msg_params][strings::url] = url;

  app_mngr.GetRPCService().ManageHMICommand(start_stream);
}

void MessageHelper::SendNaviStopStream(const int32_t app_id,
                                       ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr stop_stream = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
  if (!stop_stream) {
    return;
  }

  (*stop_stream)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Navigation_StopStream;

  (*stop_stream)[strings::msg_params][strings::app_id] = app_id;

  app_mngr.GetRPCService().ManageHMICommand(stop_stream);
}

void MessageHelper::SendAudioStartStream(const int32_t app_id,
                                         ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr start_stream = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());

  if (!start_stream) {
    return;
  }

  (*start_stream)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Navigation_StartAudioStream;

  char url[100] = {'\0'};
  if ("socket" == app_mngr.get_settings().audio_server_type()) {
    snprintf(url,
             sizeof(url) / sizeof(url[0]),
             "http://%s:%d",
             app_mngr.get_settings().server_address().c_str(),
             app_mngr.get_settings().audio_streaming_port());
  } else if ("pipe" == app_mngr.get_settings().audio_server_type()) {
    snprintf(url,
             sizeof(url) / sizeof(url[0]),
             "%s",
             app_mngr.get_settings().named_audio_pipe_path().c_str());
  } else {
    int snprintf_result =
        snprintf(url,
                 sizeof(url) / sizeof(url[0]),
                 "%s",
                 app_mngr.get_settings().audio_stream_file().c_str());
    DCHECK(snprintf_result);
  }

  (*start_stream)[strings::msg_params][strings::app_id] = app_id;
  (*start_stream)[strings::msg_params][strings::url] = url;

  DCHECK(app_mngr.GetRPCService().ManageHMICommand(start_stream));
}

void MessageHelper::SendAudioStopStream(const int32_t app_id,
                                        ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr stop_stream = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());

  if (!stop_stream) {
    return;
  }

  (*stop_stream)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Navigation_StopAudioStream;

  (*stop_stream)[strings::msg_params][strings::app_id] = app_id;

  app_mngr.GetRPCService().ManageHMICommand(stop_stream);
}

void MessageHelper::SendOnDataStreaming(
    const protocol_handler::ServiceType service,
    const bool available,
    ApplicationManager& app_mngr) {
  using namespace protocol_handler;
  using namespace helpers;

  if (!Compare<ServiceType, EQ, ONE>(
          service, ServiceType::kAudio, ServiceType::kMobileNav)) {
    return;
  }

  smart_objects::SmartObjectSPtr notification =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

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

  app_mngr.GetRPCService().ManageHMICommand(notification);
}

bool MessageHelper::SendStopAudioPathThru(ApplicationManager& app_mngr) {
  LOG4CXX_INFO(logger_, "MessageHelper::SendAudioStopAudioPathThru");

  smart_objects::SmartObjectSPtr result = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());

  (*result)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::UI_EndAudioPassThru;

  return app_mngr.GetRPCService().ManageHMICommand(result);
}

bool MessageHelper::SendUnsubscribedWayPoints(ApplicationManager& app_mngr) {
  LOG4CXX_INFO(logger_, "MessageHelper::SendUnsubscribedWayPoints");

  smart_objects::SmartObjectSPtr result = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());

  (*result)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::Navigation_UnsubscribeWayPoints;

  return app_mngr.GetRPCService().ManageHMICommand(result);
}

void MessageHelper::SendPolicySnapshotNotification(
    uint32_t connection_key,
    const std::vector<uint8_t>& policy_data,
    const std::string& url,
    ApplicationManager& app_mngr) {
  smart_objects::SmartObject content(smart_objects::SmartType_Map);

  if (!url.empty()) {
    content[strings::msg_params][strings::url] =
        url;  // Doesn't work with mobile_notification::syncp_url ("URL")
  } else {
    LOG4CXX_WARN(logger_, "No service URLs");
  }

  content[strings::params][strings::binary_data] =
      smart_objects::SmartObject(policy_data);
#if defined(PROPRIETARY_MODE) || defined(EXTERNAL_PROPRIETARY_MODE)
  content[strings::msg_params][strings::request_type] =
      mobile_apis::RequestType::PROPRIETARY;
#else
  content[strings::msg_params][strings::request_type] =
      mobile_apis::RequestType::HTTP;
#endif  // PROPRIETARY || EXTERNAL_PROPRIETARY_MODE

  SendSystemRequestNotification(connection_key, content, app_mngr);
}

void MessageHelper::SendSystemRequestNotification(
    const uint32_t connection_key,
    smart_objects::SmartObject& content,
    ApplicationManager& app_mngr) {
  content[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnSystemRequestID;
  content[strings::params][strings::message_type] =
      mobile_apis::messageType::notification;
  content[strings::params][strings::protocol_type] =
      commands::CommandImpl::mobile_protocol_type_;
  content[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;

  content[strings::params][strings::connection_key] = connection_key;

#ifdef DEBUG
  PrintSmartObject(content);
#endif

  DCHECK(app_mngr.GetRPCService().ManageMobileCommand(
      utils::MakeShared<smart_objects::SmartObject>(content),
      commands::Command::SOURCE_SDL));
}

void MessageHelper::SendLaunchApp(const uint32_t connection_key,
                                  const std::string& urlSchema,
                                  const std::string& packageName,
                                  ApplicationManager& app_mngr) {
  using namespace mobile_apis;
  using namespace smart_objects;

  SmartObject content(SmartType_Map);
  content[strings::msg_params][strings::request_type] = RequestType::LAUNCH_APP;
  content[strings::msg_params][strings::app_id] = connection_key;
  if (!urlSchema.empty()) {
    content[strings::msg_params][strings::url] = urlSchema;
  } else if (!packageName.empty()) {
    content[strings::msg_params][strings::url] = packageName;
  }

  SendSystemRequestNotification(connection_key, content, app_mngr);
}

void MessageHelper::SendQueryApps(const uint32_t connection_key,
                                  ApplicationManager& app_mngr) {
  using namespace mobile_apis;

  policy::PolicyHandlerInterface& policy_handler = app_mngr.GetPolicyHandler();

  const uint32_t timeout = policy_handler.TimeoutExchangeSec();
  smart_objects::SmartObject content(smart_objects::SmartType_Map);
  content[strings::msg_params][strings::request_type] = RequestType::QUERY_APPS;
  content[strings::msg_params][strings::url] = policy_handler.RemoteAppsUrl();
  content[strings::msg_params][strings::timeout] = timeout;

  Json::Value http_header;

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

  content[strings::params][strings::binary_data] =
      smart_objects::SmartObject(binary_data);
  content[strings::msg_params][strings::file_type] = FileType::BINARY;

  SendSystemRequestNotification(connection_key, content, app_mngr);
}

void MessageHelper::SendOnPermissionsChangeNotification(
    uint32_t connection_key,
    const policy::Permissions& permissions,
    ApplicationManager& app_mngr) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject content(smart_objects::SmartType_Map);

  content[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnPermissionsChangeID;
  content[strings::params][strings::message_type] =
      mobile_apis::messageType::notification;
  content[strings::params][strings::protocol_type] =
      commands::CommandImpl::mobile_protocol_type_;
  content[strings::params][strings::protocol_version] =
      commands::CommandImpl::protocol_version_;
  content[strings::params][strings::connection_key] = connection_key;

  content[strings::msg_params] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

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

  app_mngr.GetRPCService().ManageMobileCommand(
      utils::MakeShared<smart_objects::SmartObject>(content),
      commands::Command::SOURCE_SDL);
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
    const uint32_t connection_key,
    const policy::AppPermissions& permissions,
    ApplicationManager& app_mngr) {
  smart_objects::SmartObject message(smart_objects::SmartType_Map);

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
    smart_objects::SmartObject request_types_array(
        smart_objects::SmartType_Array);

    for (uint16_t index = 0; index < permissions.requestType.size(); ++index) {
      request_types_array[index] = permissions.requestType[index];
    }
    message[strings::msg_params][strings::request_type] = request_types_array;
  }
  if (permissions.requestSubTypeChanged) {
    smart_objects::SmartObject request_subtypes_array(
        smart_objects::SmartType_Array);

    for (uint16_t index = 0; index < permissions.requestSubType.size();
         ++index) {
      request_subtypes_array[index] = permissions.requestSubType[index];
    }
    message[strings::msg_params][strings::request_subtype] =
        request_subtypes_array;
  }

  app_mngr.GetRPCService().ManageHMICommand(
      utils::MakeShared<smart_objects::SmartObject>(message));
}

void MessageHelper::SendGetStatusUpdateResponse(const std::string& status,
                                                const uint32_t correlation_id,
                                                ApplicationManager& app_mngr) {
  smart_objects::SmartObjectSPtr message =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_GetStatusUpdate;
  (*message)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::params][hmi_response::code] = 0;

  (*message)[strings::msg_params]["status"] = status;

  app_mngr.GetRPCService().ManageHMICommand(message);
}

void MessageHelper::SendUpdateSDLResponse(const std::string& result,
                                          const uint32_t correlation_id,
                                          ApplicationManager& app_mngr) {
  smart_objects::SmartObjectSPtr message =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  DCHECK(message);

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_UpdateSDL;
  (*message)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::params][hmi_response::code] = 0;

  (*message)[strings::msg_params]["result"] = result;

  app_mngr.GetRPCService().ManageHMICommand(message);
}

void MessageHelper::SendOnStatusUpdate(const std::string& status,
                                       ApplicationManager& app_mngr) {
  smart_objects::SmartObjectSPtr message =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::SDL_OnStatusUpdate;
  (*message)[strings::params][strings::message_type] =
      MessageType::kNotification;

  (*message)[strings::msg_params]["status"] = status;

  app_mngr.GetRPCService().ManageHMICommand(message);
}

void MessageHelper::SendGetSystemInfoRequest(ApplicationManager& app_mngr) {
  smart_objects::SmartObjectSPtr message = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
  if (!message) {
    return;
  }

  (*message)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_GetSystemInfo;

  app_mngr.GetRPCService().ManageHMICommand(message);
}

mobile_apis::Result::eType MessageHelper::VerifyImageFiles(
    smart_objects::SmartObject& message,
    ApplicationConstSharedPtr app,
    ApplicationManager& app_mngr) {
  if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == message.getType()) {
    for (uint32_t i = 0; i < message.length(); ++i) {
      mobile_apis::Result::eType res =
          VerifyImageFiles(message[i], app, app_mngr);
      if (mobile_apis::Result::SUCCESS != res) {
        LOG4CXX_DEBUG(logger_, "VerifyImageFiles result:" << res);
        return res;
      }
    }
  } else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Map ==
             message.getType()) {
    if (message.keyExists(strings::image_type)) {
      mobile_apis::Result::eType verification_result =
          VerifyImage(message, app, app_mngr);

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
          mobile_apis::Result::eType res =
              VerifyImageFiles(message[*key], app, app_mngr);
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

mobile_apis::Result::eType MessageHelper::VerifyImageApplyPath(
    smart_objects::SmartObject& image,
    ApplicationConstSharedPtr app,
    ApplicationManager& app_mngr) {
  // Checking image type first: if STATIC - skip existence check, since it is
  // HMI related file and it should know it location
  const uint32_t image_type = image[strings::image_type].asUInt();
  mobile_apis::ImageType::eType type =
      static_cast<mobile_apis::ImageType::eType>(image_type);
  if (mobile_apis::ImageType::STATIC == type) {
    return mobile_apis::Result::SUCCESS;
  }

  const std::string& file_name = image[strings::value].asString();
  const std::string& full_file_path = GetAppFilePath(file_name, app, app_mngr);

  image[strings::value] = full_file_path;
  if (file_system::FileExists(full_file_path)) {
    return mobile_apis::Result::SUCCESS;
  }
  return mobile_apis::Result::INVALID_DATA;
}

std::string MessageHelper::GetAppFilePath(std::string file_name,
                                          ApplicationConstSharedPtr app,
                                          ApplicationManager& app_mngr) {
  std::string str = file_name;
  // Verify that file name is not only space characters
  str.erase(remove(str.begin(), str.end(), ' '), str.end());
  if (0 == str.size()) {
    return "";
  }

  std::string full_file_path;
  if (file_name.size() > 0 && file_name[0] == '/') {
    full_file_path = file_name;
  } else {
    const std::string& app_storage_folder =
        app_mngr.get_settings().app_storage_folder();
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

  return full_file_path;
}

mobile_apis::Result::eType MessageHelper::VerifyTtsFiles(
    smart_objects::SmartObject& tts_chunks,
    ApplicationConstSharedPtr app,
    ApplicationManager& app_mngr) {
  mobile_apis::Result::eType result = mobile_apis::Result::SUCCESS;
  for (auto& tts_chunk : *(tts_chunks.asArray())) {
    if (tts_chunk[strings::type] == mobile_apis::SpeechCapabilities::FILE) {
      const std::string full_file_path =
          GetAppFilePath(tts_chunk[strings::text].asString(), app, app_mngr);
      tts_chunk[strings::text] = full_file_path;
      if (!file_system::FileExists(full_file_path)) {
        result = mobile_apis::Result::FILE_NOT_FOUND;
      }
    }
  }
  return result;
}

mobile_apis::Result::eType MessageHelper::VerifyImage(
    smart_objects::SmartObject& image,
    ApplicationConstSharedPtr app,
    ApplicationManager& app_mngr) {
  smart_objects::SmartObject temp_image = image;
  const uint32_t image_type = image[strings::image_type].asUInt();
  const mobile_apis::ImageType::eType type =
      static_cast<mobile_apis::ImageType::eType>(image_type);

  const mobile_apis::Result::eType result =
      VerifyImageApplyPath(temp_image, app, app_mngr);
  if ((mobile_apis::Result::SUCCESS == result) &&
      (mobile_apis::ImageType::DYNAMIC == type)) {
    image[strings::value] = temp_image[strings::value];
  }

  return result;
}

mobile_apis::Result::eType MessageHelper::VerifyImageVrHelpItems(
    smart_objects::SmartObject& message,
    ApplicationConstSharedPtr app,
    ApplicationManager& app_mngr) {
  mobile_apis::Result::eType verification_result_image =
      mobile_apis::Result::SUCCESS;
  for (uint32_t i = 0; i < message.length(); ++i) {
    if (message[i].keyExists(strings::image)) {
      verification_result_image =
          VerifyImage(message[i][strings::image], app, app_mngr);
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

bool CheckWithPolicy(mobile_api::SystemAction::eType system_action,
                     const std::string& app_mobile_id,
                     const policy::PolicyHandlerInterface& policy_handler) {
  using namespace mobile_apis;
  bool result = true;
  if (policy_handler.PolicyEnabled()) {
    result = policy_handler.CheckSystemAction(system_action, app_mobile_id);
  }
  return result;
}

mobile_apis::Result::eType MessageHelper::ProcessSoftButtons(
    smart_objects::SmartObject& message_params,
    ApplicationConstSharedPtr app,
    const policy::PolicyHandlerInterface& policy_handler,
    ApplicationManager& app_mngr) {
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
                         app->policy_app_id(),
                         policy_handler)) {
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
              VerifyImage(
                  request_soft_buttons[i][strings::image], app, app_mngr)))) {
          return Result::INVALID_DATA;
        }
        break;
      }
      case SoftButtonType::SBT_TEXT: {
        if (request_soft_buttons[i].keyExists(strings::image)) {
          request_soft_buttons[i].erase(strings::image);
        }
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
              VerifyImage(
                  request_soft_buttons[i][strings::image], app, app_mngr)))) {
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
      printf("%lld", static_cast<long long int>(object.asInt()));
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
