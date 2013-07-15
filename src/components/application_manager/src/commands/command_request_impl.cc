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

#include <string>
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/application_manager_impl.h"
#include "SmartObjects/CSmartObject.hpp"

namespace application_manager {

namespace commands {

CommandRequestImpl::CommandRequestImpl(const MessageSharedPtr& message)
: CommandImpl(message) {
}

CommandRequestImpl::~CommandRequestImpl() {
}

bool CommandRequestImpl::Init() {
  return true;
}

bool CommandRequestImpl::CleanUp() {
  return true;
}

void CommandRequestImpl::Run() {
}

void CommandRequestImpl::SendResponse(const bool success,
    const mobile_apis::Result::eType& result_code,
    const char* info, const NsSmart::CSmartObject* response_params) const {

  NsSmartDeviceLink::NsSmartObjects::CSmartObject* result =
  new NsSmartDeviceLink::NsSmartObjects::CSmartObject;
  if (!result) {
    LOG4CXX_ERROR(logger_, "Memory allocation failed.");
    return;
  }
  NsSmartDeviceLink::NsSmartObjects::CSmartObject& response = *result;

  response[strings::params][strings::message_type] = MessageType::kResponse;
  response[strings::params][strings::correlation_id] =
      (*message_)[strings::params][strings::correlation_id];
  response[strings::params][strings::protocol_type] =
      CommandImpl::mobile_protocol_type_;
  response[strings::params][strings::protocol_version] =
        CommandImpl::protocol_version_;
  response[strings::params][strings::connection_key] =
      (*message_)[strings::params][strings::connection_key];
  response[strings::params][strings::function_id] =
      (*message_)[strings::params][strings::function_id];

  if (response_params) {
    response[strings::msg_params]= response_params;
  }

  if (info) {
    response[strings::msg_params][strings::info] = std::string(info);
  }

  response[strings::msg_params][strings::success] = success;
  response[strings::msg_params][strings::result_code] = result_code;

  ApplicationManagerImpl::instance()->ManageMobileCommand(result);
}

void CommandRequestImpl::CreateHMIRequest(
    const hmi_apis::FunctionID::eType& function_id,
    const NsSmart::CSmartObject& msg_params, bool chaining_required) const {

  const long correlation_id =
    (*message_)[strings::params][strings::correlation_id].asLong();
  const long connection_key =
    (*message_)[strings::params][strings::connection_key].asLong();
  const long hmi_correlation_id = ApplicationManagerImpl::instance()->
  GetHMIcorrelation_id(correlation_id, connection_key);

  NsSmartDeviceLink::NsSmartObjects::CSmartObject* result =
      new NsSmartDeviceLink::NsSmartObjects::CSmartObject;
  if (!result) {
    LOG4CXX_ERROR(logger_, "Memory allocation failed.");
    return;
  }

  NsSmartDeviceLink::NsSmartObjects::CSmartObject& request = *result;
  request[strings::params][strings::message_type] = MessageType::kRequest;
  request[strings::params][strings::function_id] = function_id;
  request[strings::params][strings::correlation_id] = hmi_correlation_id;
  request[strings::params][strings::protocol_version] =
      CommandImpl::protocol_version_;
  request[strings::params][strings::protocol_type] =
      CommandImpl::hmi_protocol_type_;

  request[strings::msg_params] = msg_params;

  if (!ApplicationManagerImpl::instance()->ManageHMICommand(result)) {
    SendResponse(false, mobile_apis::Result::OUT_OF_MEMORY);
  } else if (chaining_required) {
    if (!ApplicationManagerImpl::instance()->AddMessageChain(
        connection_key, correlation_id, hmi_correlation_id, &(*message_))) {
      LOG4CXX_ERROR(logger_, "Unable add request to MessageChain");
    }
  }

}

void CommandRequestImpl::CreateHMINotification(
    const hmi_apis::FunctionID::eType& function_id,
    const NsSmart::CSmartObject& msg_params) const {

  NsSmartDeviceLink::NsSmartObjects::CSmartObject* result =
    new NsSmartDeviceLink::NsSmartObjects::CSmartObject;
  if (!result) {
    LOG4CXX_ERROR(logger_, "Memory allocation failed.");
    return;
  }
  NsSmartDeviceLink::NsSmartObjects::CSmartObject& notify = *result;

  notify[strings::params][strings::message_type] =
      MessageType::kNotification;
  notify[strings::params][strings::function_id] = function_id;
  notify[strings::params][strings::protocol_version] =
      CommandImpl::protocol_version_;
  notify[strings::params][strings::protocol_type] =
      CommandImpl::hmi_protocol_type_;
  notify[strings::msg_params]= msg_params;

  if (!ApplicationManagerImpl::instance()->ManageHMICommand(result)) {
      // SendResponse(false, mobile_apis::Result::OUT_OF_MEMORY);
    }
}

}  // namespace commands

}  // namespace application_manager
