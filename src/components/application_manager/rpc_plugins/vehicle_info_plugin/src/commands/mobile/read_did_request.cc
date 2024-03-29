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

#include "vehicle_info_plugin/commands/mobile/read_did_request.h"
#include <string>

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

namespace vehicle_info_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

ReadDIDRequest::ReadDIDRequest(
    const application_manager::commands::MessageSharedPtr& message,
    const VehicleInfoCommandParams& params)
    : RequestFromMobileImpl(message,
                            params.application_manager_,
                            params.rpc_service_,
                            params.hmi_capabilities_,
                            params.policy_handler_) {}

ReadDIDRequest::~ReadDIDRequest() {}

void ReadDIDRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  uint32_t app_id =
      (*message_)[strings::params][strings::connection_key].asUInt();

  ApplicationSharedPtr app = application_manager_.application(app_id);
  SDL_LOG_INFO(
      "Correlation_id :"
      << (*message_)[strings::params][strings::correlation_id].asUInt());

  if (!app) {
    SDL_LOG_ERROR("An application is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (app->AreCommandLimitsExceeded(
          static_cast<mobile_apis::FunctionID::eType>(function_id()),
          application_manager::TLimitSource::CONFIG_FILE)) {
    SDL_LOG_ERROR("ReadDID frequency is too high.");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  if ((*message_)[strings::msg_params][strings::did_location].empty()) {
    SDL_LOG_ERROR("INVALID_DATA");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();
  msg_params[strings::ecu_name] =
      (*message_)[strings::msg_params][strings::ecu_name];
  msg_params[strings::did_location] =
      (*message_)[strings::msg_params][strings::did_location];
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VehicleInfo);

  SendHMIRequest(hmi_apis::FunctionID::VehicleInfo_ReadDID, &msg_params, true);
}

void ReadDIDRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::VehicleInfo_ReadDID: {
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
      SDL_LOG_ERROR("Received unknown event " << event.id());
      return;
    }
  }
}

}  // namespace commands

}  // namespace vehicle_info_plugin
