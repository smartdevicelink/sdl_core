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

#include "application_manager/commands/mobile/unsubscribe_vehicle_data_request.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "application_manager/smart_object_keys.h"
#include "utils/helpers.h"

namespace application_manager {
namespace commands {

UnsubscribeVehicleDataRequest::UnsubscribeVehicleDataRequest(
    const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

UnsubscribeVehicleDataRequest::~UnsubscribeVehicleDataRequest() {
}

#ifdef HMI_DBUS_API
namespace {
  struct Subrequest {
    hmi_apis::FunctionID::eType func_id;
    const char* str;
  };
  Subrequest subrequests[] = {
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeGps, strings::gps},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeSpeed, strings::speed},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeRpm, strings::rpm},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeFuelLevel, strings::fuel_level},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeFuelLevel_State, strings::fuel_level_state},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeInstantFuelConsumption, strings::instant_fuel_consumption},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeExternalTemperature, strings::external_temp},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeVin, strings::vin},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribePrndl, strings::prndl},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeTirePressure, strings::tire_pressure},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeOdometer, strings::odometer},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeBeltStatus, strings::belt_status},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeBodyInformation, strings::body_information},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeDeviceStatus, strings::device_status},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeDriverBraking, strings::driver_braking},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeWiperStatus, strings::wiper_status},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeHeadLampStatus, strings::head_lamp_status},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeEngineTorque, strings::engine_torque},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeAccPedalPosition, strings::acc_pedal_pos},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeSteeringWheelAngle, strings::steering_wheel_angle},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeECallInfo, strings::e_call_info},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeAirbagStatus, strings::airbag_status},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeEmergencyEvent, strings::emergency_event},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeClusterModeStatus, strings::cluster_mode_status},
    { hmi_apis::FunctionID::VehicleInfo_UnsubscribeMyKey, strings::my_key},
  };
}
#endif // #ifdef HMI_DBUS_API

void UnsubscribeVehicleDataRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
      connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  int32_t items_to_unsubscribe = 0;
  int32_t unsubscribed_items = 0;

  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  smart_objects::SmartObject response_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  for (; vehicle_data.end() != it; ++it) {
    const std::string& key_name = it->first;
    if ((*message_)[strings::msg_params].keyExists(key_name)) {
      bool is_key_enabled = (*message_)[strings::msg_params][key_name].asBool();
      if (is_key_enabled) {
        ++items_to_unsubscribe;

        VehicleDataType key_type = it->second;
        if (!app->IsSubscribedToIVI(key_type)) {
          ++unsubscribed_items;
          vi_already_unsubscribed_by_this_app_.insert(key_type);
          response_params[key_name][strings::data_type] = key_type;
          response_params[key_name][strings::result_code] =
              mobile_apis::VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED;
          continue;
        }

        if (!app->UnsubscribeFromIVI(static_cast<uint32_t>(key_type))) {
          LOG4CXX_ERROR(logger_, "Unable to unsubscribe from "
                                 "VehicleDataType: " << key_type);
          continue;
        }

        LOG4CXX_DEBUG(logger_, "Unsubscribed app with connection key "
                      << connection_key()
                      << " from VehicleDataType: "
                      << key_type);

        ++unsubscribed_items;

        if (IsSomeoneSubscribedFor(key_type)) {
          LOG4CXX_DEBUG(logger_, "There are another apps still subscribed for "
                                 "VehicleDataType: " << key_type);

          vi_still_subscribed_by_another_apps_.insert(key_type);
          response_params[key_name][strings::data_type] = key_type;
          response_params[key_name][strings::result_code] =
              mobile_apis::VehicleDataResultCode::VDRC_SUCCESS;
          continue;
        }

        msg_params[key_name] = is_key_enabled;
      }
    }
  }

  bool is_everything_already_unsubscribed =
      static_cast<uint32_t>(items_to_unsubscribe) ==
      vi_still_subscribed_by_another_apps_.size() +
      vi_already_unsubscribed_by_this_app_.size();

  if (0 == items_to_unsubscribe) {
    if (HasDisallowedParams()) {
      SendResponse(false, mobile_apis::Result::DISALLOWED);
    } else {
      SendResponse(false, mobile_apis::Result::INVALID_DATA,
                   "No data in the request.");
    }
    return;
  }

  if (0 == unsubscribed_items) {
    SendResponse(false, mobile_apis::Result::IGNORED,
                 "Was not subscribed on any VehicleData.", &response_params);
    return;
  }

  if (is_everything_already_unsubscribed) {
    mobile_apis::Result::eType result_code =
        vi_already_unsubscribed_by_this_app_.size()
        ? mobile_apis::Result::IGNORED
        : mobile_apis::Result::SUCCESS;

    const char* info =
        vi_already_unsubscribed_by_this_app_.size()
        ? "Already subscribed on some provided VehicleData."
        : NULL;

    SendResponse(true,
                 result_code,
                 info,
                 &response_params);
    return;
  }

#ifdef HMI_DBUS_API
  //Generate list of subrequests
  for (size_t i = 0; i < sizeof(subrequests) / sizeof(subrequests[0]); ++i) {
    const Subrequest& sr = subrequests[i];
    if (true == (*message_)[strings::msg_params].keyExists(sr.str)
        && true == (*message_)[strings::msg_params][sr.str].asBool()) {
      HmiRequest hmi_request;
      hmi_request.str = sr.str;
      hmi_request.func_id = sr.func_id;
      hmi_request.complete = false;
      hmi_requests_.push_back(hmi_request);
    }
  }
  LOG4CXX_INFO(logger_,
               hmi_requests_.size() << " requests are going to be sent to HMI");

  //Send subrequests
  for (HmiRequests::const_iterator it = hmi_requests_.begin();
      it != hmi_requests_.end(); ++it)
    SendHMIRequest(it->func_id, &msg_params, true);
#else
  SendHMIRequest(hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData,
      &msg_params, true);
#endif // #ifdef HMI_DBUS_API
}

void UnsubscribeVehicleDataRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  const smart_objects::SmartObject& message = event.smart_object();

  if (hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData != event.id()) {
    LOG4CXX_ERROR(logger_, "Received unknown event.");
    return;
  }

#ifdef HMI_DBUS_API
  for (HmiRequests::iterator it = hmi_requests_.begin();
      it != hmi_requests_.end(); ++it) {
    HmiRequest & hmi_request = *it;
    if (hmi_request.func_id == event.id()) {
      hmi_request.status =
          static_cast<hmi_apis::Common_Result::eType>(
            message[strings::params][hmi_response::code].asInt());
      if (hmi_apis::Common_Result::SUCCESS == hmi_request.status)
        hmi_request.value = message[strings::msg_params][hmi_request.str];
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
      } LOG4CXX_TRACE(logger_, "Status from HMI: " << it->status <<
          ", so response status become " << status);
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

    LOG4CXX_INFO(logger_, "All HMI requests are complete");
    if (true == any_arg_success) {
      SetAllowedToTerminate(false);
    }
    SendResponse(any_arg_success, status, NULL, &response_params);
    if (true == any_arg_success) {
      UpdateHash();
    }
 }
#else
  hmi_apis::Common_Result::eType hmi_result =
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());

  bool is_succeeded =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
        hmi_result,
        hmi_apis::Common_Result::SUCCESS,
        hmi_apis::Common_Result::WARNINGS);

  mobile_apis::Result::eType result_code =
      MessageHelper::HMIToMobileResult(hmi_result);

  const char* return_info = NULL;

  if (is_succeeded) {
    if (vi_already_unsubscribed_by_this_app_.size()) {
      result_code = mobile_apis::Result::IGNORED;
      return_info = "Some provided VehicleData was not subscribed.";
    }
  }

  if (!vi_still_subscribed_by_another_apps_.empty() ||
      !vi_already_unsubscribed_by_this_app_.empty()) {
    AddAlreadyUnsubscribedVI(const_cast<smart_objects::SmartObject&>(
                             message[strings::msg_params]));
  }

  if (is_succeeded) {
    SetAllowedToTerminate(false);
  }
  SendResponse(is_succeeded, result_code, return_info,
               &(message[strings::msg_params]));
  if (is_succeeded) {
    UpdateHash();
  }
#endif // #ifdef HMI_DBUS_API
}

bool UnsubscribeVehicleDataRequest::IsSomeoneSubscribedFor(
    const uint32_t param_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationManagerImpl::SubscribedToIVIPredicate finder(param_id);
  ApplicationManagerImpl::ApplicationListAccessor accessor;
  return !accessor.FindAll(finder).empty();
}

void UnsubscribeVehicleDataRequest::AddAlreadyUnsubscribedVI(
    smart_objects::SmartObject& response) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;
  VehicleInfoSubscriptions::const_iterator it_same_app =
      vi_already_unsubscribed_by_this_app_.begin();
  for (;vi_already_unsubscribed_by_this_app_.end() != it_same_app;
       ++it_same_app) {
    response[*it_same_app][strings::result_code] =
        VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED;
  }

  VehicleInfoSubscriptions::const_iterator it_another_app =
      vi_still_subscribed_by_another_apps_.begin();
  for (;vi_still_subscribed_by_another_apps_.end() != it_another_app;
       ++it_another_app) {
    response[*it_another_app][strings::result_code] =
        VehicleDataResultCode::VDRC_SUCCESS;
  }
}

void UnsubscribeVehicleDataRequest::UpdateHash() const {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr application =
      ApplicationManagerImpl::instance()->application(connection_key());
  if (application) {
    application->UpdateHash();
  } else {
    LOG4CXX_ERROR(logger_, "Application with connection_key = "
                  << connection_key() <<" doesn't exist.");
  }
  ApplicationManagerImpl::instance()->TerminateRequest(connection_key(),
                                                       correlation_id());
}


}  // namespace commands
}  // namespace application_manager
