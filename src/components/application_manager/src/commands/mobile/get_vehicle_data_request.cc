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

#include <string>
#include "application_manager/commands/mobile/get_vehicle_data_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

namespace str = strings;

#ifdef HMI_DBUS_API
GetVehicleDataRequest::GetVehicleDataRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

GetVehicleDataRequest::~GetVehicleDataRequest() {
}

void GetVehicleDataRequest::Run() {
  LOG4CXX_INFO(logger_, "GetVehicleDataRequest::Run");

  int32_t app_id = (*message_)[strings::params][strings::connection_key].asUInt();
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(app_id);

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (mobile_api::HMILevel::HMI_NONE == app->hmi_level()) {
    LOG4CXX_ERROR(logger_, "app in HMI level HMI_NONE");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();

  for (; vehicle_data.end() != it; ++it) {
    if (true == (*message_)[str::msg_params].keyExists(it->first)
        && true == (*message_)[str::msg_params][it->first].asBool()) {
      SendRequestsToHmi(app->app_id());
      return;
    }
  }

  SendResponse(false, mobile_apis::Result::INVALID_DATA);
}

namespace {
  struct Subrequest {
    hmi_apis::FunctionID::eType func_id;
    const char* str;
  };
  Subrequest subrequests[] = {
    { hmi_apis::FunctionID::VehicleInfo_GetGpsData, str::gps},
    { hmi_apis::FunctionID::VehicleInfo_GetSpeed, str::speed},
    { hmi_apis::FunctionID::VehicleInfo_GetRpm, str::rpm},
    { hmi_apis::FunctionID::VehicleInfo_GetFuelLevel, str::fuel_level},
    { hmi_apis::FunctionID::VehicleInfo_GetFuelLevelState, str::fuel_level_state},
    { hmi_apis::FunctionID::VehicleInfo_GetInstantFuelConsumption, str::instant_fuel_consumption},
    { hmi_apis::FunctionID::VehicleInfo_GetExternalTemperature, str::external_temp},
    { hmi_apis::FunctionID::VehicleInfo_GetVin, str::vin},
    { hmi_apis::FunctionID::VehicleInfo_GetPrndl, str::prndl},
    { hmi_apis::FunctionID::VehicleInfo_GetTirePressure, str::tire_pressure},
    { hmi_apis::FunctionID::VehicleInfo_GetOdometer, str::odometer},
    { hmi_apis::FunctionID::VehicleInfo_GetBeltStatus, str::belt_status},
    { hmi_apis::FunctionID::VehicleInfo_GetBodyInformation, str::body_information},
    { hmi_apis::FunctionID::VehicleInfo_GetDeviceStatus, str::device_status},
    { hmi_apis::FunctionID::VehicleInfo_GetDriverBraking, str::driver_braking},
    { hmi_apis::FunctionID::VehicleInfo_GetWiperStatus, str::wiper_status},
    { hmi_apis::FunctionID::VehicleInfo_GetHeadLampStatus, str::head_lamp_status},
    { hmi_apis::FunctionID::VehicleInfo_GetEngineTorque, str::engine_torque},
    { hmi_apis::FunctionID::VehicleInfo_GetAccPedalPosition, str::acc_pedal_pos},
    { hmi_apis::FunctionID::VehicleInfo_GetSteeringWheelAngle, str::steering_wheel_angle},
    { hmi_apis::FunctionID::VehicleInfo_GetECallInfo, str::e_call_info},
    { hmi_apis::FunctionID::VehicleInfo_GetAirbagStatus, str::airbag_status},
    { hmi_apis::FunctionID::VehicleInfo_GetEmergencyEvent, str::emergency_event},
    { hmi_apis::FunctionID::VehicleInfo_GetClusterModeStatus, str::cluster_mode_status},
    { hmi_apis::FunctionID::VehicleInfo_GetMyKey, str::my_key},
  };
}

void GetVehicleDataRequest::SendRequestsToHmi(const int32_t app_id) {
  smart_objects::SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app_id;

  for (size_t i = 0; i < sizeof(subrequests) / sizeof(subrequests[0]); ++i) {
    const Subrequest& sr = subrequests[i];
    if (true == (*message_)[str::msg_params].keyExists(sr.str)
        && true == (*message_)[str::msg_params][sr.str].asBool()) {
      HmiRequest hmi_request;
      hmi_request.str = sr.str;
      hmi_request.func_id = sr.func_id;
      hmi_request.complete = false;
      hmi_requests_.push_back(hmi_request);
    }
  }

  LOG4CXX_INFO(logger_,
               hmi_requests_.size() << " requests are going to be sent to HMI");

  for (HmiRequests::const_iterator it = hmi_requests_.begin();
      it != hmi_requests_.end(); ++it) {
    SendHMIRequest(it->func_id, &msg_params, true);
  }
}

void GetVehicleDataRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "GetVehicleDataRequest::on_event " << event.id());

  const smart_objects::SmartObject& message = event.smart_object();

  for (HmiRequests::iterator it = hmi_requests_.begin();
      it != hmi_requests_.end(); ++it) {
    HmiRequest & hmi_request = *it;
    if (hmi_request.func_id == event.id()) {
      hmi_request.status =
          static_cast<hmi_apis::Common_Result::eType>(message[strings::params][hmi_response::code]
              .asInt());
      if (hmi_apis::Common_Result::SUCCESS == hmi_request.status)
        hmi_request.value = message[str::msg_params][hmi_request.str];
      hmi_request.complete = true;
      break;
    }
  }

  bool all_complete = true;
  bool any_arg_success = false;
  mobile_api::Result::eType status = mobile_api::Result::eType::SUCCESS;
  for (HmiRequests::const_iterator it = hmi_requests_.begin();
      it != hmi_requests_.end(); ++it) {
    if (!it->complete) {
      all_complete = false;
      break;
    }
    if (hmi_apis::Common_Result::SUCCESS != it->status) {
      if (mobile_api::Result::SUCCESS == status) {
        status = static_cast<mobile_apis::Result::eType>(it->status);
      } else if (status
          != static_cast<mobile_apis::Result::eType>(it->status)) {
        status = mobile_api::Result::eType::GENERIC_ERROR;
      }
      LOG4CXX_TRACE(logger_, "Status from HMI: " << it->status << ", so response status become " << status);
    } else {
    	any_arg_success = true;
    }
  }

  if (all_complete) {
    smart_objects::SmartObject response_params(smart_objects::SmartType_Map);
    if (any_arg_success) {
      for (HmiRequests::const_iterator it = hmi_requests_.begin();
          it != hmi_requests_.end(); ++it) {
        response_params[it->str] = it->value;
      }
    }
    LOG4CXX_INFO(
        logger_, "All HMI requests are complete");
    const char *info = NULL;
    std::string error_message;
    if (true == message[strings::params].keyExists(strings::error_msg)) {
      error_message = message[strings::params][strings::error_msg].asString();
      info = error_message.c_str();
    }
    SendResponse( any_arg_success, status, info, &response_params);
  }
}
#else
GetVehicleDataRequest::GetVehicleDataRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

GetVehicleDataRequest::~GetVehicleDataRequest() {
}

void GetVehicleDataRequest::Run() {
  LOG4CXX_INFO(logger_, "GetVehicleDataRequest::Run");

  int32_t app_id = (*message_)[strings::params][strings::connection_key].asUInt();
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(app_id);

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (mobile_api::HMILevel::HMI_NONE == app->hmi_level()) {
    LOG4CXX_ERROR(logger_, "app in HMI level HMI_NONE.");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }
  if (app->IsCommandLimitsExceeded(
        static_cast<mobile_apis::FunctionID::eType>(function_id()),
        application_manager::TLimitSource::CONFIG_FILE)) {
    LOG4CXX_ERROR(logger_, "GetVehicleData frequency is too high.");
    SendResponse(false, mobile_apis::Result::REJECTED);    
    return;
  }
  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();
  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
            smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();
  const uint32_t min_length_msg_params = 1;
  for (; vehicle_data.end() != it; ++it) {
    if (true == (*message_)[str::msg_params].keyExists(it->first)
        && true == (*message_)[str::msg_params][it->first].asBool()) {
      msg_params[it->first] = (*message_)[strings::msg_params][it->first];
    }
  }
  if (msg_params.length() > min_length_msg_params) {
    SendHMIRequest(hmi_apis::FunctionID::VehicleInfo_GetVehicleData,
                   &msg_params, true);
    return;
  } else if (HasDisallowedParams()) {
    SendResponse(false, mobile_apis::Result::DISALLOWED);
  } else {
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
  }
}

void GetVehicleDataRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "GetVehicleDataRequest::on_event");
  smart_objects::SmartObject message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::VehicleInfo_GetVehicleData: {
      mobile_apis::Result::eType result_code =
          static_cast<mobile_apis::Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      bool result = false;
      if (mobile_apis::Result::SUCCESS == result_code ||
          (hmi_apis::Common_Result::DATA_NOT_AVAILABLE ==
              static_cast<hmi_apis::Common_Result::eType>(result_code)
                    && message[strings::msg_params].length() > 1)) {
        result = true;
      }
      const char *info = NULL;
      std::string error_message;
      if (true ==
          message[strings::msg_params].keyExists(hmi_response::method)) {
        message[strings::msg_params].erase(hmi_response::method);
      }
      if (true == message[strings::params].keyExists(strings::error_msg)) {
        error_message = message[strings::params][strings::error_msg].asString();
        info = error_message.c_str();
      }
      SendResponse(result, result_code, info, &(message[strings::msg_params]));
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }
}

#endif // #ifdef HMI_DBUS_API

} // namespace commands

} // namespace application_manager
