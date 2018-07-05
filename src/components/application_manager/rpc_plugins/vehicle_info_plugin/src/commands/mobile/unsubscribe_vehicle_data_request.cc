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
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "application_manager/smart_object_keys.h"
#include "utils/helpers.h"
#include "vehicle_info_plugin/vehicle_info_app_extension.h"

namespace vehicle_info_plugin {
using namespace application_manager;
namespace commands {

UnsubscribeVehicleDataRequest::UnsubscribeVehicleDataRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

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

  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  smart_objects::SmartObject response_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  for (; vehicle_data.end() != it; ++it) {
    const std::string& key_name = it->first;
    if ((*message_)[strings::msg_params].keyExists(key_name)) {
      bool is_key_enabled = (*message_)[strings::msg_params][key_name].asBool();
      if (is_key_enabled) {
        ++items_to_unsubscribe;

        mobile_apis::VehicleDataType::eType key_type = it->second;
        const auto is_subscribed =
            VehicleInfoAppExtension::ExtractVIExtension(*app)
                .isSubscribedToVehicleInfo(key_type);
        if (!is_subscribed) {
          ++unsubscribed_items;
          vi_already_unsubscribed_by_this_app_.insert(key_type);
          response_params[key_name][strings::data_type] = key_type;
          response_params[key_name][strings::result_code] =
              mobile_apis::VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED;
          continue;
        }

        if (!VehicleInfoAppExtension::ExtractVIExtension(*app)
                 .unsubscribeFromVehicleInfo(it->second)) {
          LOG4CXX_ERROR(logger_,
                        "Unable to unsubscribe from "
                        "VehicleDataType: "
                            << key_type);
          continue;
        }

        LOG4CXX_DEBUG(logger_,
                      "Unsubscribed app with connection key "
                          << connection_key()
                          << " from VehicleDataType: " << key_type);

        ++unsubscribed_items;

        if (IsSomeoneSubscribedFor(key_type)) {
          LOG4CXX_DEBUG(logger_,
                        "There are another apps still subscribed for "
                        "VehicleDataType: "
                            << key_type);

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
      SendResponse(
          false, mobile_apis::Result::INVALID_DATA, "No data in the request.");
    }
    return;
  }

  if (0 == unsubscribed_items) {
    SendResponse(false,
                 mobile_apis::Result::IGNORED,
                 "Was not subscribed on any VehicleData.",
                 &response_params);
    return;
  }

  if (is_everything_already_unsubscribed) {
    if (!vi_already_unsubscribed_by_this_app_.empty()) {
      SendResponse(false,
                   mobile_apis::Result::IGNORED,
                   "Some provided VehicleData was not subscribed.",
                   &response_params);
    } else {
      SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &response_params);
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
  const bool result = PrepareResultForMobileResponse(
      hmi_result, HmiInterfaces::HMI_INTERFACE_VehicleInfo);

  mobile_apis::Result::eType result_code =
      MessageHelper::HMIToMobileResult(hmi_result);

  if (result) {
    if (vi_already_unsubscribed_by_this_app_.size()) {
      result_code = mobile_apis::Result::IGNORED;
      response_info = "Some provided VehicleData was not subscribed.";
    }
  }

  if (!vi_still_subscribed_by_another_apps_.empty() ||
      !vi_already_unsubscribed_by_this_app_.empty()) {
    AddAlreadyUnsubscribedVI(
        const_cast<smart_objects::SmartObject&>(message[strings::msg_params]));
  }

  if (result) {
    SetAllowedToTerminate(false);
  }
  SendResponse(result,
               result_code,
               response_info.empty() ? NULL : response_info.c_str(),
               &(message[strings::msg_params]));
  if (result) {
    application_manager_.TerminateRequest(
        connection_key(), correlation_id(), function_id());
  }
}

bool UnsubscribeVehicleDataRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

struct SubscribedToIVIPredicate {
  int32_t vehicle_info_;
  SubscribedToIVIPredicate(int32_t vehicle_info)
      : vehicle_info_(vehicle_info) {}
  bool operator()(const ApplicationSharedPtr app) const {
    DCHECK_OR_RETURN(app, false);
    auto& ext = VehicleInfoAppExtension::ExtractVIExtension(*app);
    return ext.isSubscribedToVehicleInfo(
        static_cast<mobile_apis::VehicleDataType::eType>(vehicle_info_));
  }
};

bool UnsubscribeVehicleDataRequest::IsSomeoneSubscribedFor(
    const uint32_t param_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  SubscribedToIVIPredicate finder(param_id);
  DataAccessor<ApplicationSet> accessor = application_manager_.applications();
  ApplicationSetConstIt it = std::find_if(
      accessor.GetData().begin(), accessor.GetData().end(), finder);
  return it != accessor.GetData().end();
}

void UnsubscribeVehicleDataRequest::AddAlreadyUnsubscribedVI(
    smart_objects::SmartObject& response) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;
  VehicleInfoSubscriptions::const_iterator it_same_app =
      vi_already_unsubscribed_by_this_app_.begin();
  for (; vi_already_unsubscribed_by_this_app_.end() != it_same_app;
       ++it_same_app) {
    response[*it_same_app][strings::result_code] =
        VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED;
  }

  VehicleInfoSubscriptions::const_iterator it_another_app =
      vi_still_subscribed_by_another_apps_.begin();
  for (; vi_still_subscribed_by_another_apps_.end() != it_another_app;
       ++it_another_app) {
    response[*it_another_app][strings::result_code] =
        VehicleDataResultCode::VDRC_SUCCESS;
  }
}

}  // namespace commands
}  // namespace application_manager
