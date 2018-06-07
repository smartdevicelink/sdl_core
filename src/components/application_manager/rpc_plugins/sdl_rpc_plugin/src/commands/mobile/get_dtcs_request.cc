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

#include "application_manager/commands/mobile/get_dtcs_request.h"

#include "application_manager/application_impl.h"
#include "interfaces/HMI_API.h"
#include "application_manager/message_helper.h"

namespace application_manager {

namespace commands {

GetDTCsRequest::GetDTCsRequest(const MessageSharedPtr& message,
                               ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

GetDTCsRequest::~GetDTCsRequest() {}

void GetDTCsRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::ecu_name] =
      (*message_)[strings::msg_params][strings::ecu_name];

  if ((*message_)[strings::msg_params].keyExists(strings::dtc_mask)) {
    msg_params[strings::dtc_mask] =
        (*message_)[strings::msg_params][strings::dtc_mask];
  }

  msg_params[strings::app_id] = app->app_id();

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VehicleInfo);
  SendHMIRequest(hmi_apis::FunctionID::VehicleInfo_GetDTCs, &msg_params, true);
}

void GetDTCsRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::VehicleInfo_GetDTCs: {
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VehicleInfo);
      hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());

      const bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_VehicleInfo);
      std::string response_info;
      GetInfo(message, response_info);

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
