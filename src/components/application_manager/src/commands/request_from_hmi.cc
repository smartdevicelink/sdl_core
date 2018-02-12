/*
 Copyright (c) 2016, Ford Motor Company
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

#include "application_manager/commands/request_from_hmi.h"
#include "application_manager/application_manager.h"
#include "application_manager/rpc_service.h"
#include "utils/make_shared.h"

namespace application_manager {

namespace commands {

RequestFromHMI::RequestFromHMI(const MessageSharedPtr& message,
                               ApplicationManager& application_manager,
                               rpc_service::RPCService& rpc_service,
                               HMICapabilities& hmi_capabilities,
                               policy::PolicyHandlerInterface& policy_handler)
    : CommandImpl(message,
                  application_manager,
                  rpc_service,
                  hmi_capabilities,
                  policy_handler)
    , EventObserver(application_manager.event_dispatcher()) {
  // Replace HMI app id with Mobile connection id
  ReplaceHMIWithMobileAppId(*message);
}

RequestFromHMI::~RequestFromHMI() {}

bool RequestFromHMI::Init() {
  return true;
}

bool RequestFromHMI::CleanUp() {
  return true;
}

void RequestFromHMI::Run() {}

void RequestFromHMI::on_event(const event_engine::Event& event) {}

void RequestFromHMI::SendResponse(
    const bool success,
    const uint32_t correlation_id,
    const hmi_apis::FunctionID::eType function_id,
    const hmi_apis::Common_Result::eType result_code) {
  smart_objects::SmartObjectSPtr message =
      ::utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  FillCommonParametersOfSO(*message, correlation_id, function_id);
  (*message)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message)[strings::params][hmi_response::code] = 0;
  (*message)[strings::msg_params][strings::success] = success;
  (*message)[strings::msg_params][strings::result_code] = result_code;

  application_manager_.GetRPCService().ManageHMICommand(message);
}

void RequestFromHMI::SendErrorResponse(
    const uint32_t correlation_id,
    const hmi_apis::FunctionID::eType function_id,
    const hmi_apis::Common_Result::eType result_code,
    const std::string error_message) {
  smart_objects::SmartObjectSPtr message =
      ::utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  FillCommonParametersOfSO(*message, correlation_id, function_id);
  (*message)[strings::params][strings::message_type] =
      MessageType::kErrorResponse;
  (*message)[strings::params][hmi_response::code] = result_code;
  (*message)[strings::params][strings::error_msg] = error_message;

  application_manager_.GetRPCService().ManageHMICommand(message);
}

void RequestFromHMI::FillCommonParametersOfSO(
    smart_objects::SmartObject& message,
    const uint32_t correlation_id,
    const hmi_apis::FunctionID::eType function_id) {
  (message)[strings::params][strings::function_id] = function_id;
  (message)[strings::params][strings::protocol_type] = hmi_protocol_type_;
  (message)[strings::params][strings::protocol_version] = protocol_version_;
  (message)[strings::params][strings::correlation_id] = correlation_id;
}

}  // namespace commands
}  // namespace application_manager
