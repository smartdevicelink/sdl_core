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
#include "application_manager/message_helper.h"
#include "smart_objects/smart_object.h"
#include "config_profile/profile.h"

namespace application_manager {

namespace commands {

CommandRequestImpl::CommandRequestImpl(const MessageSharedPtr& message)
 : CommandImpl(message),
   default_timeout_(profile::Profile::instance()->default_timeout()),
   current_state_(kAwaitingHMIResponse) {
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

void CommandRequestImpl::onTimeOut() {
  LOG4CXX_INFO(logger_, "CommandRequestImpl::onTimeOut");

  unsubscribe_from_all_events();
  {
    sync_primitives::AutoLock auto_lock(state_lock_);
    if (kCompleted == current_state_) {
      // don't send timeout if request completed
      return;
    }

    current_state_ = kTimedOut;
  }

  smart_objects::SmartObject* response =
    MessageHelper::CreateNegativeResponse(connection_key(), function_id(),
    correlation_id(), mobile_api::Result::TIMED_OUT);

  ApplicationManagerImpl::instance()->ManageMobileCommand(response);
}

void CommandRequestImpl::on_event(const event_engine::Event& event) {
}

void CommandRequestImpl::SendResponse(
    const bool success, const mobile_apis::Result::eType& result_code,
    const char* info, const NsSmart::SmartObject* response_params) {

  {
    sync_primitives::AutoLock auto_lock(state_lock_);
    if (kTimedOut == current_state_) {
      // don't send response if request timeout expired
      return;
    }

    current_state_ = kCompleted;
  }

  NsSmartDeviceLink::NsSmartObjects::SmartObject* result =
      new NsSmartDeviceLink::NsSmartObjects::SmartObject;
  if (!result) {
    LOG4CXX_ERROR(logger_, "Memory allocation failed.");
    return;
  }
  NsSmartDeviceLink::NsSmartObjects::SmartObject& response = *result;

  response[strings::params][strings::message_type] = MessageType::kResponse;
  response[strings::params][strings::correlation_id] = correlation_id();
  response[strings::params][strings::protocol_type] =
      CommandImpl::mobile_protocol_type_;
  response[strings::params][strings::protocol_version] =
      CommandImpl::protocol_version_;
  response[strings::params][strings::connection_key] = connection_key();
  response[strings::params][strings::function_id] = function_id();

  if (response_params) {
    response[strings::msg_params] = *response_params;
  }

  if (info) {
    response[strings::msg_params][strings::info] = std::string(info);
  }

  response[strings::msg_params][strings::success] = success;
  response[strings::msg_params][strings::result_code] = result_code;

  ApplicationManagerImpl::instance()->ManageMobileCommand(result);
}

bool CommandRequestImpl::CheckSyntax(std::string str, bool allow_empty_line) {
  if (std::string::npos != str.find_first_of("\t\n")) {
    LOG4CXX_ERROR(logger_, "CheckSyntax failed! :" << str);
    return false;
  }
  if (std::string::npos != str.find("\\n") ||
      std::string::npos != str.find("\\t")) {
    LOG4CXX_ERROR(logger_, "CheckSyntax failed! :" << str);
    return false;
  }
  if (!allow_empty_line) {
    if ((std::string::npos == str.find_first_not_of(' '))) {
      return false;
    }
  }
  return true;
}

void CommandRequestImpl::SendHMIRequest(
    const hmi_apis::FunctionID::eType& function_id,
    const NsSmart::SmartObject* msg_params, bool use_events) {

  NsSmartDeviceLink::NsSmartObjects::SmartObject* result =
      new NsSmartDeviceLink::NsSmartObjects::SmartObject;
  if (!result) {
    LOG4CXX_ERROR(logger_, "Memory allocation failed.");
    return;
  }

  const uint32_t hmi_correlation_id =
       ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
  if (use_events) {
    subscribe_on_event(function_id, hmi_correlation_id);
  }

  NsSmartDeviceLink::NsSmartObjects::SmartObject& request = *result;
  request[strings::params][strings::message_type] = MessageType::kRequest;
  request[strings::params][strings::function_id] = function_id;
  request[strings::params][strings::correlation_id] = hmi_correlation_id;
  request[strings::params][strings::protocol_version] =
      CommandImpl::protocol_version_;
  request[strings::params][strings::protocol_type] =
      CommandImpl::hmi_protocol_type_;

  if (msg_params) {
    request[strings::msg_params] = *msg_params;
  }

  if (!ApplicationManagerImpl::instance()->ManageHMICommand(result)) {
    LOG4CXX_ERROR(logger_, "Unable to send request");
    SendResponse(false, mobile_apis::Result::OUT_OF_MEMORY);
  }
}

void CommandRequestImpl::CreateHMINotification(
    const hmi_apis::FunctionID::eType& function_id,
    const NsSmart::SmartObject& msg_params) const {

  NsSmartDeviceLink::NsSmartObjects::SmartObject* result =
      new NsSmartDeviceLink::NsSmartObjects::SmartObject;
  if (!result) {
    LOG4CXX_ERROR(logger_, "Memory allocation failed.");
    return;
  }
  NsSmartDeviceLink::NsSmartObjects::SmartObject& notify = *result;

  notify[strings::params][strings::message_type] =
      static_cast<int32_t>(application_manager::MessageType::kNotification);
  notify[strings::params][strings::function_id] = function_id;
  notify[strings::msg_params] = msg_params;

  if (!ApplicationManagerImpl::instance()->ManageHMICommand(result)) {
    LOG4CXX_ERROR(logger_, "Unable to send HMI notification");
  }
}

mobile_apis::Result::eType CommandRequestImpl::GetMobileResultCode(
    const hmi_apis::Common_Result::eType& hmi_code) const {

  mobile_apis::Result::eType mobile_result = mobile_apis::Result::GENERIC_ERROR;
  switch (hmi_code) {
    case hmi_apis::Common_Result::SUCCESS: {
      mobile_result = mobile_apis::Result::SUCCESS;
      break;
    }
    case hmi_apis::Common_Result::UNSUPPORTED_REQUEST: {
      mobile_result = mobile_apis::Result::UNSUPPORTED_REQUEST;
      break;
    }
    case hmi_apis::Common_Result::UNSUPPORTED_RESOURCE: {
      mobile_result = mobile_apis::Result::UNSUPPORTED_RESOURCE;
      break;
    }
    case hmi_apis::Common_Result::DISALLOWED: {
      mobile_result = mobile_apis::Result::DISALLOWED;
      break;
    }
    case hmi_apis::Common_Result::REJECTED: {
      mobile_result = mobile_apis::Result::REJECTED;
      break;
    }
    case hmi_apis::Common_Result::ABORTED: {
      mobile_result = mobile_apis::Result::ABORTED;
      break;
    }
    case hmi_apis::Common_Result::IGNORED: {
      mobile_result = mobile_apis::Result::IGNORED;
      break;
    }
    case hmi_apis::Common_Result::RETRY: {
      mobile_result = mobile_apis::Result::RETRY;
      break;
    }
    case hmi_apis::Common_Result::IN_USE: {
      mobile_result = mobile_apis::Result::IN_USE;
      break;
    }
    case hmi_apis::Common_Result::DATA_NOT_AVAILABLE: {
      mobile_result = mobile_apis::Result::VEHICLE_DATA_NOT_AVAILABLE;
      break;
    }
    case hmi_apis::Common_Result::TIMED_OUT: {
      mobile_result = mobile_apis::Result::TIMED_OUT;
      break;
    }
    case hmi_apis::Common_Result::INVALID_DATA: {
      mobile_result = mobile_apis::Result::INVALID_DATA;
      break;
    }
    case hmi_apis::Common_Result::CHAR_LIMIT_EXCEEDED: {
      mobile_result = mobile_apis::Result::CHAR_LIMIT_EXCEEDED;
      break;
    }
    case hmi_apis::Common_Result::INVALID_ID: {
      mobile_result = mobile_apis::Result::INVALID_ID;
      break;
    }
    case hmi_apis::Common_Result::DUPLICATE_NAME: {
      mobile_result = mobile_apis::Result::DUPLICATE_NAME;
      break;
    }
    case hmi_apis::Common_Result::APPLICATION_NOT_REGISTERED: {
      mobile_result = mobile_apis::Result::APPLICATION_NOT_REGISTERED;
      break;
    }
    case hmi_apis::Common_Result::WRONG_LANGUAGE: {
      mobile_result = mobile_apis::Result::WRONG_LANGUAGE;
      break;
    }
    case hmi_apis::Common_Result::OUT_OF_MEMORY: {
      mobile_result = mobile_apis::Result::OUT_OF_MEMORY;
      break;
    }
    case hmi_apis::Common_Result::TOO_MANY_PENDING_REQUESTS: {
      mobile_result = mobile_apis::Result::TOO_MANY_PENDING_REQUESTS;
      break;
    }
    case hmi_apis::Common_Result::NO_APPS_REGISTERED: {
      mobile_result = mobile_apis::Result::APPLICATION_NOT_REGISTERED;
      break;
    }
    case hmi_apis::Common_Result::NO_DEVICES_CONNECTED: {
      mobile_result = mobile_apis::Result::APPLICATION_NOT_REGISTERED;
      break;
    }
    case hmi_apis::Common_Result::WARNINGS: {
      mobile_result = mobile_apis::Result::WARNINGS;
      break;
    }
    case hmi_apis::Common_Result::GENERIC_ERROR: {
      mobile_result = mobile_apis::Result::GENERIC_ERROR;
      break;
    }
    case hmi_apis::Common_Result::USER_DISALLOWED: {
      mobile_result = mobile_apis::Result::USER_DISALLOWED;
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown HMI result code " << hmi_code);
      break;
    }
  }

  return mobile_result;
}

}  // namespace commands

}  // namespace application_manager
