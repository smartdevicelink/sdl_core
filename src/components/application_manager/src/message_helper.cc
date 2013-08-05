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

#include <set>
#include <string>
#include "application_manager/message_helper.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

const VehicleData MessageHelper::vehicle_data_ = {
  {strings::gps,                      VehicleDataType::GPS},
  {strings::speed,                    VehicleDataType::SPEED},
  {strings::rpm,                      VehicleDataType::RPM},
  {strings::fuel_level,               VehicleDataType::FUELLEVEL},
  {strings::fuel_level_state,         VehicleDataType::FUELLEVEL_STATE},
  {strings::instant_fuel_consumption, VehicleDataType::FUELCONSUMPTION},
  {strings::external_temp,            VehicleDataType::EXTERNTEMP},
  {strings::vin,                      VehicleDataType::VIN},
  {strings::prndl,                    VehicleDataType::PRNDL},
  {strings::tire_pressure,            VehicleDataType::TIREPRESSURE},
  {strings::odometer,                 VehicleDataType::ODOMETER},
  {strings::belt_status,              VehicleDataType::BELTSTATUS},
  {strings::body_information,         VehicleDataType::BODYINFO},
  {strings::device_status,            VehicleDataType::DEVICESTATUS},
  {strings::e_call_info,              VehicleDataType::ECALLINFO},
  {strings::airbag_status,            VehicleDataType::AIRBAGSTATUS},
  {strings::emergency_event,          VehicleDataType::EMERGENCYEVENT},
  {strings::cluster_mode_status,      VehicleDataType::CLUSTERMODESTATUS},
  {strings::my_key,                   VehicleDataType::MYKEY},
  {strings::driver_braking,           VehicleDataType::BRAKING},
  {strings::wiper_status,             VehicleDataType::WIPERSTATUS},
  {strings::head_lamp_status,         VehicleDataType::HEADLAMPSTATUS},
  /*
  NOT DEFINED in mobile API
  {strings::gps,                      VehicleDataType::BATTVOLTAGE},
  */
  {strings::engine_torque,            VehicleDataType::ENGINETORQUE},
  {strings::acc_pedal_pos,            VehicleDataType::ACCPEDAL},
  {strings::steering_wheel_angle,     VehicleDataType::STEERINGWHEEL},
};

void MessageHelper::SendHMIStatusNotification(
  const Application& application_impl) {
  smart_objects::SmartObject* notification = new smart_objects::SmartObject;
  if (!notification) {
    // TODO(VS): please add logger.
    return;
  }
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
    mobile_api::FunctionID::OnHMIStatusID;

  message[strings::params][strings::message_type] = MessageType::kNotification;

  message[strings::params][strings::connection_key] =
    application_impl.app_id();

  message[strings::msg_params][strings::hmi_level] =
    application_impl.hmi_level();

  message[strings::msg_params][strings::audio_streaming_state] =
    application_impl.audio_streaming_state();

  message[strings::msg_params][strings::system_context] =
    application_impl.system_context();

  ApplicationManagerImpl::instance()->ManageMobileCommand(notification);
}

void MessageHelper::SendDeviceListUpdatedNotificationToHMI(
  const std::set<connection_handler::Device>& device_list) {
  smart_objects::SmartObject* notification = new smart_objects::SmartObject;
  if (!notification) {
    // TODO(VS): please add logger.
    return;
  }
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
    hmi_apis::FunctionID::BasicCommunication_OnDeviceListUpdated;

  message[strings::params][strings::message_type] = MessageType::kNotification;

  int index = 0;

  for (std::set<connection_handler::Device>::iterator it =
         device_list.begin();
       device_list.end() != it; ++it) {
    message[strings::msg_params][strings::device_list]
    [index][strings::name] = (*it).user_friendly_name();
    message[strings::msg_params][strings::device_list]
    [index][strings::id] = (*it).device_handle();
    ++index;
  }

  ApplicationManagerImpl::instance()->ManageMobileCommand(notification);
}

void MessageHelper::SendOnAppRegisteredNotificationToHMI(
  const Application& application_impl) {
  smart_objects::SmartObject* notification = new smart_objects::SmartObject;
  if (!notification) {
    // TODO(VS): please add logger.
    return;
  }
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
    hmi_apis::FunctionID::BasicCommunication_OnAppRegistered;

  message[strings::params][strings::message_type] = MessageType::kNotification;

  message[strings::msg_params][strings::application][strings::app_name] =
    application_impl.name();

  message[strings::msg_params]
  [strings::application]
  [strings::ngn_media_screen_app_name] =
    application_impl.ngn_media_screen_name();

  message[strings::msg_params]
  [strings::application]
  [strings::icon] = application_impl.app_icon_path();

  // TODO(VS): get device name from application_impl
  message[strings::msg_params]
  [strings::application]
  [strings::device_name] = "";

  message[strings::msg_params]
  [strings::application][strings::app_id] = application_impl.app_id();

  message[strings::msg_params]
  [strings::application]
  [strings::hmi_display_language_desired] =
    application_impl.ui_language();

  message[strings::msg_params]
  [strings::application]
  [strings::is_media_application] =
    application_impl.is_media_application();

  message[strings::msg_params][strings::application][strings::app_type] =
    application_impl.app_types();

  ApplicationManagerImpl::instance()->ManageHMICommand(notification);
}

void MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
  int connection_key,
  mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  smart_objects::SmartObject* notification = new smart_objects::SmartObject;
  if (!notification) {
    // TODO(VS): please add logger.
    return;
  }
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
    mobile_api::FunctionID::OnAppInterfaceUnregisteredID;

  message[strings::params][strings::message_type] = MessageType::kNotification;

  message[strings::msg_params][strings::connection_key] = connection_key;

  message[strings::msg_params][strings::reason] = reason;

  ApplicationManagerImpl::instance()->ManageMobileCommand(notification);
}

const VehicleData& MessageHelper::vehicle_data() {
  return vehicle_data_;
}

smart_objects::SmartObject* MessageHelper::CreateDeviceListSO(
  const connection_handler::DeviceList& devices) {
  smart_objects::SmartObject* device_list_so  =
    new smart_objects::SmartObject(smart_objects::SmartType_Map);

  if (NULL == device_list_so) {
    return NULL;
  }

  if (!devices.empty())  {
    (*device_list_so)[strings::device_list] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
    smart_objects::SmartObject& list_so =
      (*device_list_so)[strings::device_list];
    int index = 0;
    for (connection_handler::DeviceList::const_iterator it = devices.begin();
         devices.end() != it; ++it) {
      list_so[index][strings::name] = it->second.user_friendly_name();
      list_so[index][strings::id] = it->second.device_handle();
      ++index;
    }
  }
  return device_list_so;
}

smart_objects::SmartObject* MessageHelper::CreateSetAppIcon(
  const std::string& path_to_icon, unsigned int app_id) {
  smart_objects::SmartObject* set_icon = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  if (!set_icon) {
    return NULL;
  }

  smart_objects::SmartObject& object = *set_icon;
  object[strings::sync_file_name][strings::value] = path_to_icon;
  // TODO(PV): need to store actual image type
  object[strings::sync_file_name][strings::image_type] =
    mobile_api::ImageType::DYNAMIC;
  object[strings::app_id] = app_id;

  return set_icon;
}

void MessageHelper::SendAppDataToHMI(const Application* app) {
  unsigned int id = app->app_id();

  utils::SharedPtr<smart_objects::SmartObject> set_app_icon(
    new smart_objects::SmartObject);
  if (set_app_icon) {
    smart_objects::SmartObject& so_to_send = *set_app_icon;
    so_to_send[strings::params][strings::function_id] =
      hmi_apis::FunctionID::UI_SetAppIcon;
    so_to_send[strings::params][strings::message_type] =
      hmi_apis::messageType::request;
    so_to_send[strings::params][strings::protocol_version] = 2;
    so_to_send[strings::params][strings::protocol_type] = 1;
    so_to_send[strings::params][strings::correlation_id] =
      4444;
    so_to_send[strings::msg_params] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
    smart_objects::SmartObject* msg_params = MessageHelper::CreateSetAppIcon(
          app->app_icon_path(), id);
    if (msg_params) {
      so_to_send[strings::msg_params] = *msg_params;
    }
    // TODO(PV): appropriate handling of result
    ApplicationManagerImpl::instance()->ManageHMICommand(set_app_icon);
  }

  // TODO(PV): add help_prompt; timeout_promt; vr_help_title; vr_help;
  // show_command, tbt_show_command; CommandsMap& commands_map();
}

void MessageHelper::RemoveAppDataFromHMI(const Application* app) {
  // TODO(PV): implement
}

smart_objects::SmartObject* MessageHelper::CreateNegativeResponse(
  unsigned int connection_key,
  int function_id,
  unsigned int correlation_id,
  int result_code) {
  smart_objects::SmartObject* response = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);
  smart_objects::SmartObject& response_data = *response;
  response_data[strings::params][strings::function_id] =
    function_id;
  response_data[strings::params][strings::message_type] =
    mobile_apis::messageType::response;
  response_data[strings::params][strings::correlation_id] =
    correlation_id;
  response_data[strings::msg_params][strings::result_code] = result_code;
  response_data[strings::msg_params][strings::success] = false;
  response_data[strings::params][strings::connection_key] = connection_key;

  return response;
}


}  //  namespace application_manager
