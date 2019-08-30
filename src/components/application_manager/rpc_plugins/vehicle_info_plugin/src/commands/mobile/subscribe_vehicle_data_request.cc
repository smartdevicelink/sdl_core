/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.
 Copyright (c) 2018, Livio, Inc.
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

#include "vehicle_info_plugin/commands/mobile/subscribe_vehicle_data_request.h"

#include <boost/algorithm/string.hpp>
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/helpers.h"
#include "vehicle_info_plugin/vehicle_info_app_extension.h"

namespace vehicle_info_plugin {
using namespace application_manager;
namespace commands {

SubscribeVehicleDataRequest::SubscribeVehicleDataRequest(
    const application_manager::commands::MessageSharedPtr& message,
    VehicleInfoCommandParams params)
    : CommandRequestImpl(message,
                         params.application_manager_,
                         params.rpc_service_,
                         params.hmi_capabilities_,
                         params.policy_handler_)
    , custom_vehicle_data_manager_(params.custom_vehicle_data_manager_) {}

SubscribeVehicleDataRequest::~SubscribeVehicleDataRequest() {}

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
    AppendDataTypesToMobileResponse(response_params);
    SendResponse(result,
                 result_code,
                 info.empty() ? NULL : info.c_str(),
                 response_params.empty() ? NULL : &response_params);
    return;
  }

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VehicleInfo);
  SendHMIRequest(hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData,
                 &msg_params,
                 true);
}

void SubscribeVehicleDataRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  const smart_objects::SmartObject& message = event.smart_object();

  if (hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData != event.id()) {
    LOG4CXX_ERROR(logger_, "Received unknown event.");
    return;
  }
  EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VehicleInfo);
  ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer.");
    return;
  }

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

  smart_objects::SmartObject converted_msg_params(message[strings::msg_params]);
  custom_vehicle_data_manager_.CreateMobileMessageParams(converted_msg_params);

  if (is_succeeded) {
    if (!vi_already_subscribed_by_this_app_.empty()) {
      result_code = mobile_apis::Result::IGNORED;
      response_info = "Already subscribed on some provided VehicleData.";
    }

    for (const auto& param : converted_msg_params.enumerate()) {
      const auto& converted_name = ConvertResponseToRequestName(param);
      if (vi_waiting_for_subscribe_.end() ==
          vi_waiting_for_subscribe_.find(converted_name)) {
        LOG4CXX_DEBUG(logger_, "erase " << converted_name);
        converted_msg_params.erase(param);
      }
    }

    if (!SubscribePendingVehicleData(app, converted_msg_params)) {
      result_code = mobile_apis::Result::GENERIC_ERROR;
      response_info = "Subscription failed for some Vehicle data";
      SendResponse(false, result_code, response_info.c_str());
      return;
    }
  }

  if (!vi_already_subscribed_by_another_apps_.empty() ||
      !vi_already_subscribed_by_this_app_.empty()) {
    AddAlreadySubscribedVI(converted_msg_params);
  }
  AppendDataTypesToMobileResponse(converted_msg_params);

  SendResponse(is_succeeded,
               result_code,
               response_info.empty() ? NULL : response_info.c_str(),
               &converted_msg_params);
}

void SubscribeVehicleDataRequest::AppendDataTypesToMobileResponse(
    smart_objects::SmartObject& msg_params) const {
  using namespace smart_objects;

  std::string oem_vehicle_data_type_str;
  EnumConversionHelper<VehicleDataType>::EnumToString(
      VehicleDataType::VEHICLEDATA_OEM_CUSTOM_DATA, &oem_vehicle_data_type_str);

  const auto& rpc_spec_vehicle_data = MessageHelper::vehicle_data();
  for (const auto& item : msg_params.enumerate()) {
    const auto& rpc_spec_vehicle_data_item = rpc_spec_vehicle_data.find(item);
    if (rpc_spec_vehicle_data.end() == rpc_spec_vehicle_data_item) {
      msg_params[item][strings::data_type] = oem_vehicle_data_type_str;
      msg_params[item][strings::oem_custom_data_type] =
          custom_vehicle_data_manager_.GetVehicleDataItemType(item);
    } else {
      msg_params[item][strings::data_type] = rpc_spec_vehicle_data_item->second;
    }
  }
}

bool SubscribeVehicleDataRequest::CheckSubscriptionStatus(
    std::string vi_name, const smart_objects::SmartObject& msg_params) {
  const auto subscribed_items = msg_params.enumerate();
  if (subscribed_items.end() == subscribed_items.find(vi_name)) {
    LOG4CXX_WARN(logger_,
                 vi_name << " is waiting to be subscribed, but missing in "
                            "vehicle response.");
    return false;
  }

  auto res_code = msg_params[vi_name][strings::result_code].asInt();
  if (hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS != res_code &&
      hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED !=
          res_code) {
    LOG4CXX_WARN(logger_,
                 "Subscription to " << vi_name << " for " << connection_key()
                                    << " failed.");
    return false;
  }
  return true;
}

bool SubscribeVehicleDataRequest::SubscribePendingVehicleData(
    ApplicationSharedPtr app, const smart_objects::SmartObject& msg_params) {
  LOG4CXX_DEBUG(logger_, "Subscribing to all pending VehicleData");

  for (const auto& vi_name : vi_waiting_for_subscribe_) {
    const bool is_subscription_successful = CheckSubscriptionStatus(
        ConvertRequestToResponseName(vi_name), msg_params);

    if (is_subscription_successful) {
      auto& ext = VehicleInfoAppExtension::ExtractVIExtension(*app);
      ext.subscribeToVehicleInfo(vi_name);
      vi_waiting_for_subscribe_.erase(vi_name);
    }
  }

  return vi_waiting_for_subscribe_.empty();
}

bool SubscribeVehicleDataRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

void SubscribeVehicleDataRequest::AddAlreadySubscribedVI(
    smart_objects::SmartObject& msg_params) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;

  for (const auto& item : vi_already_subscribed_by_this_app_) {
    msg_params[item][strings::result_code] =
        VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED;
  }

  for (const auto& item : vi_already_subscribed_by_another_apps_) {
    msg_params[item][strings::result_code] =
        VehicleDataResultCode::VDRC_SUCCESS;
  }
}

struct SubscribedToIVIPredicate {
  std::string vehicle_info_;
  SubscribedToIVIPredicate(std::string vehicle_info)
      : vehicle_info_(vehicle_info) {}
  bool operator()(const ApplicationSharedPtr app) const {
    DCHECK_OR_RETURN(app, false);
    auto& ext = VehicleInfoAppExtension::ExtractVIExtension(*app);
    return ext.isSubscribedToVehicleInfo(vehicle_info_);
  }
};

bool SubscribeVehicleDataRequest::IsSomeoneSubscribedFor(
    const std::string& param_name) const {
  LOG4CXX_AUTO_TRACE(logger_);
  SubscribedToIVIPredicate finder(param_name);
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
  const bool is_interface_not_available =
      (HmiInterfaces::STATE_NOT_AVAILABLE ==
       application_manager_.hmi_interfaces().GetInterfaceState(
           HmiInterfaces::HMI_INTERFACE_VehicleInfo));

  VehicleInfoSubscriptions::size_type subscribed_items = 0;

  auto app_already_subscribed =
      [this, &out_response_params](
          const std::string& key_name,
          const mobile_apis::VehicleDataType::eType vd_type) {
        LOG4CXX_DEBUG(
            logger_,
            "App with connection key "
                << connection_key()
                << " is subscribed already for VehicleData: " << key_name);
        vi_already_subscribed_by_this_app_.insert(key_name);
        out_response_params[key_name][strings::data_type] = vd_type;
        out_response_params[key_name][strings::result_code] =
            mobile_apis::VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED;
      };

  auto another_app_subscribed =
      [this, &out_response_params, &subscribed_items](
          const std::string& key_name,
          const mobile_apis::VehicleDataType::eType vd_type,
          vehicle_info_plugin::VehicleInfoAppExtension& ext) {
        LOG4CXX_DEBUG(logger_,
                      "There are apps subscribed already for "
                      "VehicleDataType: "
                          << key_name);

        if (!ext.subscribeToVehicleInfo(key_name)) {
          LOG4CXX_ERROR(
              logger_, "Unable to subscribe for VehicleDataType: " << key_name);
          return;
        }
        LOG4CXX_DEBUG(
            logger_,
            "App with connection key "
                << connection_key()
                << " have been subscribed for VehicleDataType: " << key_name);
        vi_already_subscribed_by_another_apps_.insert(key_name);
        out_response_params[key_name][strings::data_type] = vd_type;
        out_response_params[key_name][strings::result_code] =
            mobile_apis::VehicleDataResultCode::VDRC_SUCCESS;
        ++subscribed_items;
      };

  auto first_subscription = [this, &out_request_params, &subscribed_items](
                                const std::string& vi_name) {
    out_request_params[vi_name] = (*message_)[strings::msg_params][vi_name];

    vi_waiting_for_subscribe_.insert(vi_name);
    LOG4CXX_DEBUG(
        logger_,
        "App with connection key "
            << connection_key()
            << " will be subscribed for VehicleDataType: " << vi_name);
    ++subscribed_items;
    return true;
  };

  auto rpc_spec_vehicle_data = MessageHelper::vehicle_data();
  auto& ext = VehicleInfoAppExtension::ExtractVIExtension(*app);

  VehicleInfoSubscriptions::size_type items_to_subscribe = 0;
  auto item_names = (*message_)[strings::msg_params].enumerate();
  if (!is_interface_not_available) {
    for (const auto& name : item_names) {
      auto enabled = (*message_)[strings::msg_params][name].asBool();
      if (!enabled) {
        continue;
      }

      auto vehicle_data = rpc_spec_vehicle_data.find(name);
      auto vehicle_data_type =
          (vehicle_data == rpc_spec_vehicle_data.end())
              ? VehicleDataType::VEHICLEDATA_OEM_CUSTOM_DATA
              : vehicle_data->second;
      if (ext.isSubscribedToVehicleInfo(name)) {
        app_already_subscribed(name, vehicle_data_type);
      } else if (IsSomeoneSubscribedFor(name)) {
        another_app_subscribed(name, vehicle_data_type, ext);
      } else if (!first_subscription(name)) {
        continue;
      }

      ++items_to_subscribe;
    }
  }

  const bool is_everything_already_subscribed =
      items_to_subscribe == vi_already_subscribed_by_another_apps_.size() +
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
    out_result_code = vi_already_subscribed_by_this_app_.size()
                          ? mobile_apis::Result::IGNORED
                          : mobile_apis::Result::SUCCESS;
    if (!(vi_already_subscribed_by_this_app_.empty())) {
      out_info = "Already subscribed on some provided VehicleData.";
    }
    out_result = true;
    return;
  }
}

const std::string& SubscribeVehicleDataRequest::ConvertResponseToRequestName(
    const std::string& name) {
  static std::map<std::string, std::string> parameter_name_conversion_map = {
      {strings::cluster_modes, strings::cluster_mode_status}};
  const auto converted_it = parameter_name_conversion_map.find(name);
  auto result = parameter_name_conversion_map.end() == converted_it
                    ? name
                    : converted_it->second;
  return parameter_name_conversion_map.end() == converted_it
             ? name
             : converted_it->second;
}

const std::string& SubscribeVehicleDataRequest::ConvertRequestToResponseName(
    const std::string& name) {
  static std::map<std::string, std::string> parameter_name_conversion_map = {
      {strings::cluster_mode_status, strings::cluster_modes}};
  const auto converted_it = parameter_name_conversion_map.find(name);
  auto result = parameter_name_conversion_map.end() == converted_it
                    ? name
                    : converted_it->second;
  return parameter_name_conversion_map.end() == converted_it
             ? name
             : converted_it->second;
}

}  // namespace commands
}  // namespace vehicle_info_plugin
