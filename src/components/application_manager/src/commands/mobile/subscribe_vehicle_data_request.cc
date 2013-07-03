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

#include <map>
#include "application_manager/commands/mobile/subscribe_vehicle_data_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "JSONHandler/SDLRPCObjects/V2/Result.h"

namespace application_manager {

namespace commands {

namespace str = strings;

SubscribeVehicleDataRequest::SubscribeVehicleDataRequest(
  const MessageSharedPtr& message): CommandRequestImpl(message) {
}

SubscribeVehicleDataRequest::~SubscribeVehicleDataRequest() {
}

void SubscribeVehicleDataRequest::Run() {
  LOG4CXX_INFO(logger_, "SubscribeVehicleDataRequest::Run");

  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->
        application((*message_)[str::params][str::connection_key]));

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  std::map<const char*, VehicleDataType> vehicle_data =
  {
      {str::gps,                      VehicleDataType::GPS},
      {str::speed,                    VehicleDataType::SPEED},
      {str::rpm,                      VehicleDataType::RPM},
      {str::fuel_level,               VehicleDataType::FUELLEVEL},
      {str::fuel_level_state,         VehicleDataType::FUELLEVEL_STATE},
      {str::instant_fuel_consumption, VehicleDataType::FUELCONSUMPTION},
      {str::external_temp,            VehicleDataType::EXTERNTEMP},
      /*
      NOT DEFINED in mobile API
      {str::gps,                      VehicleDataType::VIN},
      */
      {str::prndl,                    VehicleDataType::PRNDL},
      {str::tire_pressure,            VehicleDataType::TIREPRESSURE},
      {str::odometer,                 VehicleDataType::ODOMETER},
      {str::belt_status,              VehicleDataType::BELTSTATUS},
      {str::body_information,         VehicleDataType::BODYINFO},
      {str::device_status,            VehicleDataType::DEVICESTATUS},
      {str::e_call_info,              VehicleDataType::ECALLINFO},
      {str::airbag_status,            VehicleDataType::AIRBAGSTATUS},
      {str::emergency_event,          VehicleDataType::EMERGENCYEVENT},
      {str::cluster_mode_status,      VehicleDataType::CLUSTERMODESTATUS},
      {str::my_key,                   VehicleDataType::MYKEY},
      {str::driver_braking,           VehicleDataType::BRAKING},
      {str::wiper_status,             VehicleDataType::WIPERSTATUS},
      {str::head_lamp_status,         VehicleDataType::HEADLAMPSTATUS},
      /*
      NOT DEFINED in mobile API
      {str::gps,                      VehicleDataType::BATTVOLTAGE},
      */
      {str::engine_torque,            VehicleDataType::ENGINETORQUE},
      {str::acc_pedal_pos,            VehicleDataType::ACCPEDAL},
      {str::steering_wheel_angle,     VehicleDataType::STEERINGWHEEL},
  };

  // counter for items to subscribe
  int items_to_subscribe = 0;
  // counter for subscribed items by application
  int subscribed_items = 0;

  std::map<const char*, VehicleDataType>::const_iterator it =
      vehicle_data.begin();

  for (; vehicle_data.end() != it; ++it) {
    if (true == (*message_)[str::msg_params].keyExists(it->first) &&
        true == (*message_)[str::msg_params][it->first].asBool()) {
      ++items_to_subscribe;

      if (app->SubscribeToIVI(static_cast<unsigned int>(it->second))) {
        ++subscribed_items;
      }
    }
  }

  if (subscribed_items == items_to_subscribe) {
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::SUCCESS);
  } else if (0 == subscribed_items) {
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::REJECTED,
                 "Already subscribed on all VehicleData");
  } else if (subscribed_items < items_to_subscribe) {
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::WARNINGS,
                 "Already subscribed on some VehicleData");
  } else {
    LOG4CXX_ERROR(logger_, "Unknown command sequence!");
    return;
  }
}

}  // namespace commands

}  // namespace application_manager
