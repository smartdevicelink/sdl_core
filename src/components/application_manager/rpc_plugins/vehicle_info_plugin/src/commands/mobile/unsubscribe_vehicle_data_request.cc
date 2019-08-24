/*

 Copyright (c) 2018, Ford Motor Company
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

#include "vehicle_info_plugin/commands/mobile/unsubscribe_vehicle_data_request.h"
#include "application_manager/commands/command_impl.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/helpers.h"
#include "vehicle_info_plugin/vehicle_info_app_extension.h"

namespace vehicle_info_plugin {
using namespace application_manager;
namespace commands {

UnsubscribeVehicleDataRequest::UnsubscribeVehicleDataRequest(
    const application_manager::commands::MessageSharedPtr& message,
    const VehicleInfoCommandParams& params)
    : CommandRequestImpl(message,
                         params.application_manager_,
                         params.rpc_service_,
                         params.hmi_capabilities_,
                         params.policy_handler_)
    , custom_vehicle_data_manager_(params.custom_vehicle_data_manager_) {}

UnsubscribeVehicleDataRequest::~UnsubscribeVehicleDataRequest() {}

void UnsubscribeVehicleDataRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  int32_t items_to_unsubscribe = 0;
  int32_t unsubscribed_items = 0;

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  response_params_ = smart_objects::SmartObject(smart_objects::SmartType_Map);

  auto rpc_spec_vehicle_data = MessageHelper::vehicle_data();
  std::set<std::string> custom_vehicle_data;

  auto app_not_subscribed_response = [this](const std::string& key_name) {
    LOG4CXX_DEBUG(logger_,
                  "App with connection key "
                      << connection_key()
                      << " is not subscribed for VehicleData: " << key_name);
    vi_already_unsubscribed_by_this_app_.insert(key_name);
    response_params_[key_name][strings::result_code] =
        mobile_apis::VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED;
  };

  auto other_app_subscribed_response = [this](const std::string& key_name) {
    LOG4CXX_DEBUG(logger_,
                  "There are apps still subscribed for "
                  "VehicleDataType: "
                      << key_name);
    vi_still_subscribed_by_another_apps_.insert(key_name);
    response_params_[key_name][strings::result_code] =
        mobile_apis::VehicleDataResultCode::VDRC_SUCCESS;
  };

  auto& ext = VehicleInfoAppExtension::ExtractVIExtension(*app);

  const auto& param_names = (*message_)[strings::msg_params].enumerate();
  for (const auto& name : param_names) {
    const bool enabled = (*message_)[strings::msg_params][name].asBool();
    if (!enabled) {
      continue;
    }

    ++items_to_unsubscribe;

    if (!(ext.isSubscribedToVehicleInfo(name))) {
      app_not_subscribed_response(name);
      continue;
    }

    LOG4CXX_DEBUG(logger_,
                  "Unsubscribed app with connection key "
                      << connection_key() << " from VehicleDataType: " << name);

    ++unsubscribed_items;

    if (IsSomeoneSubscribedFor(name)) {
      other_app_subscribed_response(name);
      ext.unsubscribeFromVehicleInfo(name);
      continue;
    }

    vi_waiting_for_unsubscribe_.insert(name);
    msg_params[name] = true;
  }

  bool is_everything_already_unsubscribed =
      static_cast<uint32_t>(items_to_unsubscribe) ==
      vi_still_subscribed_by_another_apps_.size() +
          vi_already_unsubscribed_by_this_app_.size();

  AppendDataTypesToMobileResponse(response_params_);

  if (0 == items_to_unsubscribe) {
    if (HasDisallowedParams()) {
      SendResponse(false, mobile_apis::Result::DISALLOWED);
    } else {
      SendResponse(
          false, mobile_apis::Result::INVALID_DATA, "No data in the request.");
    }
    return;
  }

  if (0 == unsubscribed_items) {
    SendResponse(false,
                 mobile_apis::Result::IGNORED,
                 "Was not subscribed on any VehicleData.",
                 &response_params_);
    return;
  }

  if (is_everything_already_unsubscribed) {
    if (!vi_already_unsubscribed_by_this_app_.empty()) {
      SendResponse(false,
                   mobile_apis::Result::IGNORED,
                   "Some provided VehicleData was not subscribed.",
                   &response_params_);
    } else {
      SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &response_params_);
    }
    return;
  }

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VehicleInfo);
  SendHMIRequest(hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData,
                 &msg_params,
                 true);
}

void UnsubscribeVehicleDataRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  const smart_objects::SmartObject& message = event.smart_object();

  if (hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData != event.id()) {
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
  bool result = PrepareResultForMobileResponse(
      hmi_result, HmiInterfaces::HMI_INTERFACE_VehicleInfo);

  auto hmi_response_msg_params = message[strings::msg_params];

  mobile_apis::Result::eType result_code =
      MessageHelper::HMIToMobileResult(hmi_result);

  custom_vehicle_data_manager_.CreateMobileMessageParams(
      hmi_response_msg_params);

  if (!vi_still_subscribed_by_another_apps_.empty() ||
      !vi_already_unsubscribed_by_this_app_.empty()) {
    AddAlreadyUnsubscribedVI(hmi_response_msg_params);
  }

  if (result) {
    if (vi_already_unsubscribed_by_this_app_.size()) {
      result_code = mobile_apis::Result::IGNORED;
      response_info = "Some provided VehicleData was not subscribed.";
    }

    for (const auto& param : hmi_response_msg_params.enumerate()) {
      const auto converted_param = ConvertResponseToRequestName(param);
      if (vi_waiting_for_unsubscribe_.end() ==
          vi_waiting_for_unsubscribe_.find(converted_param)) {
        hmi_response_msg_params.erase(param);
      }
    }

    if (!UnsubscribePendingVehicleData(app, hmi_response_msg_params)) {
      result = false;
      result_code = mobile_apis::Result::GENERIC_ERROR;
      response_info = "Unsubscription failed for some Vehicle data";
      hmi_response_msg_params = smart_objects::SmartObject();
    }
  }

  AppendDataTypesToMobileResponse(hmi_response_msg_params);
  for (const auto& key : hmi_response_msg_params.enumerate()) {
    response_params_[key] = hmi_response_msg_params[key];
  }

  SendResponse(
      result,
      result_code,
      response_info.empty() ? NULL : response_info.c_str(),
      smart_objects::SmartType::SmartType_Null == response_params_.getType()
          ? NULL
          : &response_params_);
}

bool UnsubscribeVehicleDataRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

bool UnsubscribeVehicleDataRequest::IsSomeoneSubscribedFor(
    const std::string& param_name) const {
  LOG4CXX_AUTO_TRACE(logger_);
  const auto conn_key = connection_key();
  auto subscribed_to_ivi = [&param_name,
                            &conn_key](const ApplicationSharedPtr app) {
    DCHECK_OR_RETURN(app, false);
    if (conn_key == app->app_id()) {
      return false;
    }
    auto& ext = VehicleInfoAppExtension::ExtractVIExtension(*app);
    return (ext.isSubscribedToVehicleInfo(param_name));
  };

  DataAccessor<ApplicationSet> accessor = application_manager_.applications();
  ApplicationSetConstIt it = std::find_if(
      accessor.GetData().begin(), accessor.GetData().end(), subscribed_to_ivi);
  return it != accessor.GetData().end();
}

void UnsubscribeVehicleDataRequest::AddAlreadyUnsubscribedVI(
    smart_objects::SmartObject& response) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;

  for (const auto& item : vi_already_unsubscribed_by_this_app_) {
    response[item][strings::result_code] =
        VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED;
  }

  for (const auto& item : vi_still_subscribed_by_another_apps_) {
    response[item][strings::result_code] = VehicleDataResultCode::VDRC_SUCCESS;
  }
}

bool UnsubscribeVehicleDataRequest::CheckSubscriptionStatus(
    std::string key, const smart_objects::SmartObject& msg_params) {
  const auto unsubscribed_items = msg_params.enumerate();
  if (unsubscribed_items.end() == unsubscribed_items.find(key)) {
    LOG4CXX_WARN(logger_,
                 key << " is waiting to be unsubscribed, but missing in "
                        "vehicle response.");
    return false;
  }

  auto res_code = msg_params[key][strings::result_code].asInt();
  if (hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS != res_code) {
    LOG4CXX_WARN(logger_,
                 "Unubscribing from " << key << " for " << connection_key()
                                      << " failed.");
    return false;
  }
  return true;
}

bool UnsubscribeVehicleDataRequest::UnsubscribePendingVehicleData(
    ApplicationSharedPtr app, const smart_objects::SmartObject& msg_params) {
  LOG4CXX_DEBUG(logger_, "Unsubscribing from all pending VehicleData");

  for (const auto& vi_name : vi_waiting_for_unsubscribe_) {
    const auto converted_item = ConvertRequestToResponseName(vi_name);
    const bool is_unsubscription_successful =
        CheckSubscriptionStatus(converted_item, msg_params);
    if (is_unsubscription_successful) {
      auto& ext = VehicleInfoAppExtension::ExtractVIExtension(*app);
      ext.unsubscribeFromVehicleInfo(vi_name);
      vi_waiting_for_unsubscribe_.erase(vi_name);
    }
  }
  return vi_waiting_for_unsubscribe_.empty();
}

const std::string& UnsubscribeVehicleDataRequest::ConvertRequestToResponseName(
    const std::string& name) {
  static std::map<std::string, std::string> parameter_name_converion_map = {
      {strings::cluster_mode_status, strings::cluster_modes}};
  const auto converted_it = parameter_name_converion_map.find(name);
  return parameter_name_converion_map.end() == converted_it
             ? name
             : converted_it->second;
}

const std::string& UnsubscribeVehicleDataRequest::ConvertResponseToRequestName(
    const std::string& name) {
  static std::map<std::string, std::string> parameter_name_converion_map = {
      {strings::cluster_modes, strings::cluster_mode_status}};
  const auto converted_it = parameter_name_converion_map.find(name);
  return parameter_name_converion_map.end() == converted_it
             ? name
             : converted_it->second;
}

void UnsubscribeVehicleDataRequest::AppendDataTypesToMobileResponse(
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

}  // namespace commands
}  // namespace vehicle_info_plugin
