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

#include "vehicle_info_plugin/commands/mobile/get_vehicle_data_request.h"
#include "vehicle_info_plugin/custom_vehicle_data_manager.h"

#include <string>

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "policy/policy_table/types.h"

namespace vehicle_info_plugin {
using namespace application_manager;

namespace commands {

namespace str = strings;

GetVehicleDataRequest::GetVehicleDataRequest(
    const application_manager::commands::MessageSharedPtr& message,
    const VehicleInfoCommandParams& params)
    : CommandRequestImpl(message,
                         params.application_manager_,
                         params.rpc_service_,
                         params.hmi_capabilities_,
                         params.policy_handler_)
    , custom_vehicle_data_manager_(params.custom_vehicle_data_manager_) {}

GetVehicleDataRequest::~GetVehicleDataRequest() {}

void GetVehicleDataRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  auto app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "No such application : " << connection_key());
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (!CheckFrequency(*app)) {
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  auto hmi_msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  hmi_msg_params[strings::app_id] = app->app_id();

  int params_count = 0;
  auto& msg_params = (*message_)[strings::msg_params];
  for (const auto& name : msg_params.enumerate()) {
    auto enabled = msg_params[name].asBool();
    if (!enabled) {
      continue;
    }
    hmi_msg_params[name] = msg_params[name];
    params_count++;
  }

  const int minimal_params_count = 1;

  if (params_count >= minimal_params_count) {
    for (const auto& param : msg_params.enumerate()) {
      pending_vehicle_data_.insert(param);
    }
    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VehicleInfo);
    SendHMIRequest(hmi_apis::FunctionID::VehicleInfo_GetVehicleData,
                   &hmi_msg_params,
                   true);
  } else if (HasDisallowedParams()) {
    SendResponse(false, mobile_apis::Result::DISALLOWED);
  } else {
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
  }
}

void GetVehicleDataRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::VehicleInfo_GetVehicleData: {
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VehicleInfo);
      auto result_code = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      auto mobile_result_code = GetMobileResultCode(result_code);
      bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_VehicleInfo);
      std::string response_info;
      GetInfo(message, response_info);

      auto data_not_available_with_params = [this, &result_code, &message]() {
        if (hmi_apis::Common_Result::DATA_NOT_AVAILABLE != result_code) {
          return false;
        }

        const auto& vehicle_data = MessageHelper::vehicle_data();
        const auto& msg_params = message[strings::msg_params];
        for (const auto& item : msg_params.enumerate()) {
          if (vehicle_data.end() != vehicle_data.find(item) ||
              custom_vehicle_data_manager_.IsValidCustomVehicleDataName(item)) {
            return true;
          }
        }
        return false;
      };

      result = result || data_not_available_with_params();

      if (true ==
          message[strings::msg_params].keyExists(hmi_response::method)) {
        message[strings::msg_params].erase(hmi_response::method);
      }
      if (true == message[strings::params].keyExists(strings::error_msg)) {
        response_info = message[strings::params][strings::error_msg].asString();
      }

      custom_vehicle_data_manager_.CreateMobileMessageParams(
          const_cast<smart_objects::SmartObject&>(
              message[strings::msg_params]));

      if (result) {
        for (const auto& item : message[strings::msg_params].enumerate()) {
          const auto& found_item = pending_vehicle_data_.find(item);
          if (pending_vehicle_data_.end() == found_item) {
            message[strings::msg_params].erase(item);
          }
        }

        if (message[strings::msg_params].empty() &&
            hmi_apis::Common_Result::DATA_NOT_AVAILABLE != result_code) {
          response_info = "Failed to retrieve data from vehicle";
          SendResponse(
              false, mobile_apis::Result::GENERIC_ERROR, response_info.c_str());
          return;
        }
      }

      SendResponse(result,
                   mobile_result_code,
                   response_info.empty() ? NULL : response_info.c_str(),
                   &(message[strings::msg_params]));
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }
}

bool GetVehicleDataRequest::CheckFrequency(Application& app) {
  if (app.AreCommandLimitsExceeded(
          static_cast<mobile_apis::FunctionID::eType>(function_id()),
          application_manager::TLimitSource::CONFIG_FILE)) {
    LOG4CXX_ERROR(logger_, "GetVehicleData frequency is too high.");
    return false;
  }
  return true;
}

}  // namespace commands
}  // namespace vehicle_info_plugin
