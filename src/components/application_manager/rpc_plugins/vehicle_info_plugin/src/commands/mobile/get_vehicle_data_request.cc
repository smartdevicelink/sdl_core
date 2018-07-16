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

#include <string>
#include "vehicle_info_plugin/commands/mobile/get_vehicle_data_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace vehicle_info_plugin {
using namespace application_manager;

namespace commands {

namespace str = strings;

GetVehicleDataRequest::GetVehicleDataRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

GetVehicleDataRequest::~GetVehicleDataRequest() {}

void GetVehicleDataRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  int32_t app_id =
      (*message_)[strings::params][strings::connection_key].asUInt();
  ApplicationSharedPtr app = application_manager_.application(app_id);

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (app->AreCommandLimitsExceeded(
          static_cast<mobile_apis::FunctionID::eType>(function_id()),
          application_manager::TLimitSource::CONFIG_FILE)) {
    LOG4CXX_ERROR(logger_, "GetVehicleData frequency is too high.");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }
  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();
  const uint32_t min_length_msg_params = 1;
  for (; vehicle_data.end() != it; ++it) {
    if (true == (*message_)[str::msg_params].keyExists(it->first) &&
        true == (*message_)[str::msg_params][it->first].asBool()) {
      msg_params[it->first] = (*message_)[strings::msg_params][it->first];
    }
  }
  if (msg_params.length() > min_length_msg_params) {
    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VehicleInfo);
    SendHMIRequest(
        hmi_apis::FunctionID::VehicleInfo_GetVehicleData, &msg_params, true);
    return;
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
      hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_VehicleInfo);
      std::string response_info;
      GetInfo(message, response_info);
      result = result ||
               ((hmi_apis::Common_Result::DATA_NOT_AVAILABLE == result_code) &&
                (message[strings::msg_params].length() > 1));

      if (true ==
          message[strings::msg_params].keyExists(hmi_response::method)) {
        message[strings::msg_params].erase(hmi_response::method);
      }
      if (true == message[strings::params].keyExists(strings::error_msg)) {
        response_info = message[strings::params][strings::error_msg].asString();
      }
      SendResponse(result,
                   MessageHelper::HMIToMobileResult(result_code),
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

}  // namespace commands

}  // namespace application_manager
