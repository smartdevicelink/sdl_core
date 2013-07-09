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
#include "application_manager/application_manager_impl.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

const VehicleData MessageHelper::vehicle_data_ =
{
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
          const ApplicationImpl& application_impl) {
  smart_objects::CSmartObject message;

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

  ApplicationManagerImpl::instance()->ManageMobileCommand(&message);
}

void MessageHelper::SendDeviceListUpdatedNotificationToHMI(
    const std::set<connection_handler::Device>& device_list) {
  smart_objects::CSmartObject message;

  message[strings::params][strings::function_id] =
           hmi_apis::FunctionID::BasicCommunication_OnDeviceListUpdated;

  message[strings::params][strings::message_type] = MessageType::kNotification;

  int index = 0;

  for (std::set<connection_handler::Device>::iterator it = device_list.begin();
      device_list.end() != it; ++it) {
    message[strings::msg_params][strings::device_list]
                           [index][strings::name] = (*it).user_friendly_name();
    message[strings::msg_params][strings::device_list]
                                  [index][strings::id] = (*it).device_handle();
    ++index;
  }

  ApplicationManagerImpl::instance()->ManageMobileCommand(&message);
}

void MessageHelper::SendOnAppRegisteredNotificationToHMI(
                                     const ApplicationImpl& application_impl) {
  smart_objects::CSmartObject message;

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

  ApplicationManagerImpl::instance()->ManageHMICommand(&message);
}

void MessageHelper::SendOnAppInterfaceUnregisteredNotificationToMobile(
    int connection_key, mobile_api::AppInterfaceUnregisteredReason::eType reason) {
  smart_objects::CSmartObject message;

  message[strings::params][strings::function_id] =
      mobile_api::FunctionID::OnAppInterfaceUnregisteredID;

  message[strings::params][strings::message_type] = MessageType::kNotification;

  message[strings::msg_params][strings::connection_key] = connection_key;

  message[strings::msg_params][strings::reason] = reason;

  ApplicationManagerImpl::instance()->ManageMobileCommand(&message);
}

const VehicleData& MessageHelper::vehicle_data() {
  return vehicle_data_;
}

} //  namespace application_manager
