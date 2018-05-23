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

#include "vehicle_info_plugin/vehicle_info_hmi_command_factory.h"
#include "application_manager/message.h"
#include "interfaces/HMI_API.h"

#include "vehicle_info_plugin/commands/hmi/vi_is_ready_request.h"
#include "vehicle_info_plugin/commands/hmi/vi_is_ready_response.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "Vehicle Info Plugin")

namespace vehicle_info_plugin {
namespace strings = app_mngr::strings;

VehicleInfoHmiCommandFactory::VehicleInfoHmiCommandFactory(
        application_manager::ApplicationManager &application_manager,
        application_manager::rpc_service::RPCService &rpc_service,
        application_manager::HMICapabilities &hmi_capabilities,
        policy::PolicyHandlerInterface &policy_handler)
  : application_manager_(application_manager),
    rpc_service_(rpc_service),
    hmi_capabilities_(hmi_capabilities),
    policy_handler_(policy_handler) {
  LOG4CXX_AUTO_TRACE(logger_);
}

app_mngr::CommandSharedPtr VehicleInfoHmiCommandFactory::CreateCommand(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::commands::Command::CommandSource source) {
  UNUSED(source);

  const hmi_apis::FunctionID::eType function_id =
      static_cast<hmi_apis::FunctionID::eType>(
        (*message)[strings::params][strings::function_id].asInt());

  const hmi_apis::messageType::eType message_type =
      static_cast<hmi_apis::messageType::eType>(
        (*message)[strings::params][strings::message_type].asInt());

  auto message_type_str = "";
  if (hmi_apis::messageType::response == message_type) {
    message_type_str = "response";
  } else if (hmi_apis::messageType::error_response == message_type) {
    message_type_str = "error response";
  } else {
    message_type_str = "request";
  }

  LOG4CXX_DEBUG(
      logger_, "HMICommandFactory::CreateCommand function_id: " << function_id
               << ", message type: " << message_type_str);

  auto factory =
      app_mngr::CommandCreatorFactory(application_manager_, rpc_service_, hmi_capabilities_, policy_handler_);
  auto &creator = factory.GetCreator<app_mngr::InvalidCommand>();

  switch (function_id) {
    case hmi_apis::FunctionID::VehicleInfo_IsReady:
      creator = hmi_apis::messageType::request == message_type
          ? factory.GetCreator<commands::VIIsReadyRequest>()
          : factory.GetCreator<commands::VIIsReadyResponse>();
      break;
    case hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData:
      // SubscribeVehicleData HMI request/response
      break;
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData:
      // UnsubscribeVehicleData HMI request/response
      break;
    case hmi_apis::FunctionID::VehicleInfo_OnVehicleData:
      // OnVehicleData HMI notification
      break;
    case hmi_apis::FunctionID::VehicleInfo_ReadDID:
      // ReadDID HMI request/response
      break;
    case hmi_apis::FunctionID::VehicleInfo_GetDTCs:
      // GetDTCs HMI request/response
      break;
    case hmi_apis::FunctionID::VehicleInfo_DiagnosticMessage:
      // DiagnosticMessage HMI request/response
      break;
    default:
      LOG4CXX_WARN(logger_, "Unsupported function_id: " << function_id);
      break;
  }

  return creator.create(message);
}

bool VehicleInfoHmiCommandFactory::IsAbleToProcess(
    const int32_t function_id,
    const app_mngr::commands::Command::CommandSource source) const {
  // TODO: Implement
  throw -1;
}

}
