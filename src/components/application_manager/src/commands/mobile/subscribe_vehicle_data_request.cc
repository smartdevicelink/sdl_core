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

#include "application_manager/commands/mobile/subscribe_vehicle_data_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "utils/helpers.h"

namespace application_manager {
namespace commands {

SubscribeVehicleDataRequest::SubscribeVehicleDataRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

SubscribeVehicleDataRequest::~SubscribeVehicleDataRequest() {}

#ifdef HMI_DBUS_API
namespace {
struct Subrequest {
  hmi_apis::FunctionID::eType func_id;
  const char* str;
};
Subrequest subrequests[] = {
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

void SubscribeVehicleDataRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }
  std::string info;
  mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject response_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  bool result = false;
  CheckVISubscriptions(
      app, info, result_code, response_params, msg_params, result);

  if (mobile_apis::Result::INVALID_ENUM != result_code) {
    SendResponse(result,
                 result_code,
                 info.empty() ? NULL : info.c_str(),
                 response_params.empty() ? NULL : &response_params);
    return;
  }

#ifdef HMI_DBUS_API
  // Generate list of subrequests
  for (size_t i = 0; i < sizeof(subrequests) / sizeof(subrequests[0]); ++i) {
    const Subrequest& sr = subrequests[i];
    if (true == (*message_)[strings::msg_params].keyExists(sr.str) &&
        true == (*message_)[strings::msg_params][sr.str].asBool()) {
      HmiRequest hmi_request;
      hmi_request.str = sr.str;
      hmi_request.func_id = sr.func_id;
      hmi_request.complete = false;
      hmi_requests_.push_back(hmi_request);
    }
  }
  LOG4CXX_DEBUG(
      logger_, hmi_requests_.size() << " requests are going to be sent to HMI");

  // Send subrequests
  for (HmiRequests::const_iterator it = hmi_requests_.begin();
       it != hmi_requests_.end();
       ++it)
    SendHMIRequest(it->func_id, &msg_params, true);
#else
  SendHMIRequest(hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData,
                 &msg_params,
                 true);
#endif  // #ifdef HMI_DBUS_API
}

void SubscribeVehicleDataRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  const smart_objects::SmartObject& message = event.smart_object();

  if (hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData != event.id()) {
    LOG4CXX_ERROR(logger_, "Received unknown event.");
    return;
  }

  ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer.");
    return;
  }

#ifdef HMI_DBUS_API
  for (HmiRequests::iterator it = hmi_requests_.begin();
       it != hmi_requests_.end();
       ++it) {
    HmiRequest& hmi_request = *it;
    if (hmi_request.func_id == event.id()) {
      hmi_request.status = static_cast<hmi_apis::Common_Result::eType>(
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
       it != hmi_requests_.end();
       ++it) {
    if (!it->complete) {
      all_complete = false;
      break;
    }
    if (hmi_apis::Common_Result::SUCCESS != it->status) {
      if (mobile_api::Result::SUCCESS == status) {
        status = static_cast<mobile_apis::Result::eType>(it->status);
      } else if (status !=
                 static_cast<mobile_apis::Result::eType>(it->status)) {
        status = mobile_api::Result::eType::GENERIC_ERROR;
      }
      LOG4CXX_TRACE(logger_,
                    "Status from HMI: " << it->status
                                        << ", so response status become "
                                        << status);
    } else {
      any_arg_success = true;
    }
  }

  if (all_complete) {
    smart_objects::SmartObject response_params(smart_objects::SmartType_Map);
    if (any_arg_success) {
      for (HmiRequests::const_iterator it = hmi_requests_.begin();
           it != hmi_requests_.end();
           ++it) {
        response_params[it->str] = it->value;
      }
    }
    LOG4CXX_DEBUG(logger_, "All HMI requests are complete");
    const bool result = any_arg_success;
    SendResponse(any_arg_success, status, NULL, &response_params);
    if (result) {
      app->UpdateHash();
    }
  }
#else

  hmi_apis::Common_Result::eType hmi_result =
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
  std::string response_info;
  GetInfo(message, response_info);
  const bool result = PrepareResultForMobileResponse(
      hmi_result, HmiInterfaces::HMI_INTERFACE_VehicleInfo);

  bool is_succeeded = result || !vi_already_subscribed_by_another_apps_.empty();

  mobile_apis::Result::eType result_code =
      MessageHelper::HMIToMobileResult(hmi_result);

  if (is_succeeded) {
    if (!vi_already_subscribed_by_this_app_.empty()) {
      result_code = mobile_apis::Result::IGNORED;
      response_info = "Already subscribed on some provided VehicleData.";
    }

    if (!vi_waiting_for_subscribe_.empty()) {
      LOG4CXX_DEBUG(logger_, "Subscribing to all pending VehicleData");
      VehicleInfoSubscriptions::const_iterator key =
          vi_waiting_for_subscribe_.begin();
      for (; key != vi_waiting_for_subscribe_.end(); ++key) {
        app->SubscribeToIVI(*key);
      }
    }
  }

  UnsubscribeFailedSubscriptions(app, message[strings::msg_params]);

  if (!vi_already_subscribed_by_another_apps_.empty() ||
      !vi_already_subscribed_by_this_app_.empty()) {
    AddAlreadySubscribedVI(
        const_cast<smart_objects::SmartObject&>(message[strings::msg_params]));
  }

  SendResponse(is_succeeded,
               result_code,
               response_info.empty() ? NULL : response_info.c_str(),
               &(message[strings::msg_params]));
#endif  // #ifdef HMI_DBUS_API
}

bool SubscribeVehicleDataRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

void SubscribeVehicleDataRequest::AddAlreadySubscribedVI(
    smart_objects::SmartObject& msg_params) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;
  VehicleInfoSubscriptions::const_iterator it_same_app =
      vi_already_subscribed_by_this_app_.begin();
  for (; vi_already_subscribed_by_this_app_.end() != it_same_app;
       ++it_same_app) {
    msg_params[*it_same_app][strings::result_code] =
        VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED;
  }

  VehicleInfoSubscriptions::const_iterator it_another_app =
      vi_already_subscribed_by_another_apps_.begin();
  for (; vi_already_subscribed_by_another_apps_.end() != it_another_app;
       ++it_another_app) {
    msg_params[*it_another_app][strings::result_code] =
        VehicleDataResultCode::VDRC_SUCCESS;
  }
}

void SubscribeVehicleDataRequest::UnsubscribeFailedSubscriptions(
    ApplicationSharedPtr app,
    const smart_objects::SmartObject& msg_params) const {
  LOG4CXX_AUTO_TRACE(logger_);
  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();

  for (; vehicle_data.end() != it; ++it) {
    if (msg_params.keyExists(it->first)) {
      if (msg_params[it->first][strings::result_code].asInt() !=
          hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS) {
        LOG4CXX_DEBUG(logger_,
                      "Subscription for VehicleDataType "
                          << it->first
                          << " is unsuccessfull. "
                             "Unsubscribing app with connection key "
                          << connection_key() << " from it.");
        app->UnsubscribeFromIVI(it->second);
      }
    }
  }
}

struct SubscribedToIVIPredicate {
  int32_t vehicle_info_;
  SubscribedToIVIPredicate(int32_t vehicle_info)
      : vehicle_info_(vehicle_info) {}
  bool operator()(const ApplicationSharedPtr app) const {
    return app ? app->IsSubscribedToIVI(vehicle_info_) : false;
  }
};

bool SubscribeVehicleDataRequest::IsSomeoneSubscribedFor(
    const uint32_t param_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  SubscribedToIVIPredicate finder(param_id);
  DataAccessor<ApplicationSet> accessor = application_manager_.applications();
  ApplicationSetConstIt it = std::find_if(
      accessor.GetData().begin(), accessor.GetData().end(), finder);
  return it != accessor.GetData().end();
}

void SubscribeVehicleDataRequest::CheckVISubscriptions(
    ApplicationSharedPtr app,
    std::string& out_info,
    mobile_apis::Result::eType& out_result_code,
    smart_objects::SmartObject& out_response_params,
    smart_objects::SmartObject& out_request_params,
    bool& out_result) {
  LOG4CXX_AUTO_TRACE(logger_);
  // Counter for items to subscribe
  VehicleInfoSubscriptions::size_type items_to_subscribe = 0;
  // Counter for subscribed items by application
  uint32_t subscribed_items = 0;

  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();

  HmiInterfaces::InterfaceState interface_state =
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_VehicleInfo);

  const bool is_interface_not_available =
      interface_state == HmiInterfaces::STATE_NOT_AVAILABLE;

  for (; vehicle_data.end() != it; ++it) {
    const std::string& key_name = it->first;
    if ((*message_)[strings::msg_params].keyExists(key_name)) {
      const bool is_key_enabled =
          (*message_)[strings::msg_params][key_name].asBool();
      if (is_key_enabled) {
        ++items_to_subscribe;
      }
      if (!is_interface_not_available && is_key_enabled) {
        VehicleDataType key_type = it->second;
        if (app->IsSubscribedToIVI(key_type)) {
          LOG4CXX_DEBUG(logger_,
                        "App with connection key "
                            << connection_key()
                            << " is subscribed already for VehicleDataType: "
                            << key_type);
          ++subscribed_items;
          vi_already_subscribed_by_this_app_.insert(key_type);
          out_response_params[key_name][strings::data_type] = key_type;
          out_response_params[key_name][strings::result_code] =
              mobile_apis::VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED;
          continue;
        }

        if (IsSomeoneSubscribedFor(key_type)) {
          LOG4CXX_DEBUG(logger_,
                        "There are apps subscribed already for "
                        "VehicleDataType: "
                            << key_type);
          if (!app->SubscribeToIVI(static_cast<uint32_t>(key_type))) {
            LOG4CXX_ERROR(
                logger_,
                "Unable to subscribe for VehicleDataType: " << key_type);
            continue;
          }
          LOG4CXX_DEBUG(
              logger_,
              "App with connection key "
                  << connection_key()
                  << " have been subscribed for VehicleDataType: " << key_type);
          ++subscribed_items;
          vi_already_subscribed_by_another_apps_.insert(key_type);
          out_response_params[key_name][strings::data_type] = key_type;
          out_response_params[key_name][strings::result_code] =
              mobile_apis::VehicleDataResultCode::VDRC_SUCCESS;
          continue;
        }

        out_request_params[key_name] = is_key_enabled;

        if (is_key_enabled) {
          vi_waiting_for_subscribe_.insert(key_type);
          LOG4CXX_DEBUG(
              logger_,
              "App with connection key "
                  << connection_key()
                  << " will be subscribed for VehicleDataType: " << key_type);
          ++subscribed_items;
        }
      }
    }
  }

  const bool is_everything_already_subscribed =
      items_to_subscribe ==
      vi_already_subscribed_by_another_apps_.size() +
          vi_already_subscribed_by_this_app_.size();

  if (0 == items_to_subscribe) {
    if (HasDisallowedParams()) {
      out_result_code = mobile_apis::Result::DISALLOWED;
    } else {
      out_result_code = mobile_apis::Result::INVALID_DATA;
      out_info = "No data in the request";
    }
    out_result = false;
    return;
  }

  if (0 == subscribed_items && !is_interface_not_available) {
    out_result_code = mobile_apis::Result::IGNORED;
    out_info = "Already subscribed on provided VehicleData.";
    out_result = false;
    return;
  }

  if (is_everything_already_subscribed) {
    if (vi_already_subscribed_by_this_app_.empty()) {
      out_result_code = mobile_apis::Result::SUCCESS;
      out_result = true;
    } else {
      out_result_code = mobile_apis::Result::IGNORED;
      out_info = "Already subscribed on some provided VehicleData.";
      out_result = false;
    }
    return;
  }
}

}  // namespace commands
}  // namespace application_manager
