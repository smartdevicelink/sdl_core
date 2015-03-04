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

#include "can_cooperation/commands/base_command_request.h"
#include <cstring>
#include "can_cooperation/event_engine/event_dispatcher.h"
#include "can_cooperation/message_helper.h"
#include "can_cooperation/can_module.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace commands {

using event_engine::EventDispatcher;

using namespace json_keys;

CREATE_LOGGERPTR_GLOBAL(logger_, "BaseCommandRequest")

BaseCommandRequest::BaseCommandRequest(
  const application_manager::MessagePtr& message)
  : message_(message) {
  service_ = CANModule::instance()->service();
}


BaseCommandRequest::~BaseCommandRequest() {
  EventDispatcher<application_manager::MessagePtr, std::string>::instance()->
  remove_observer(this);
}


void BaseCommandRequest::OnTimeout() {
  SendResponse(false, result_codes::kTimedOut, "Request timeout expired.");
}

void BaseCommandRequest::SendResponse(bool success,
                                      const char* result_code,
                                      const std::string& info) {
  message_->set_message_type(application_manager::MessageType::kResponse);
  Json::Value msg_params;

  if (!response_params_.isNull()) {
    msg_params = response_params_;
  }

  msg_params[kSuccess] = success;
  msg_params[kResultCode] = result_code;
  if (!info.empty()) {
    msg_params[kInfo] = info;
  }

  Json::FastWriter writer;
  std::string params = writer.write(msg_params);
  message_->set_json_message(params);
  if (0 == strcmp(result_code, result_codes::kTimedOut)) {
    CANModule::instance()->SendTimeoutResponseToMobile(message_);
  } else {
    CANModule::instance()->SendResponseToMobile(message_);
  }
}

void  BaseCommandRequest::SendRequest(const char* function_id,
                                      const Json::Value& message_params,
                                      bool is_hmi_request) {
  Json::Value msg;

  if (is_hmi_request) {
    msg[kId] = service_->GetNextCorrelationID();
  } else {
    msg[kId] = MessageHelper::GetNextCANCorrelationID();
  }

  EventDispatcher<application_manager::MessagePtr, std::string>::instance()->
  add_observer(function_id, msg[kId].asInt(), this);

  msg[kJsonrpc] = "2.0";
  msg[kMethod] = function_id;
  if (!message_params.isNull()) {
    msg[kParams] = message_params;
  }

  Json::FastWriter writer;
  std::string json_msg = writer.write(msg);
  if (is_hmi_request) {
    application_manager::MessagePtr message_to_send(
      new application_manager::Message(
        protocol_handler::MessagePriority::kDefault));
    message_to_send->set_protocol_version(
      application_manager::ProtocolVersion::kHMI);
    message_to_send->set_correlation_id(msg[kId].asInt());
    message_to_send->set_json_message(json_msg);
    message_to_send->set_message_type(
      application_manager::MessageType::kRequest);

    service_->SendMessageToHMI(message_to_send);
  } else {
    CANModule::instance()->SendMessageToCan(json_msg);
  }
}

const char* BaseCommandRequest::GetMobileResultCode(
  const hmi_apis::Common_Result::eType& hmi_code) const {
  switch (hmi_code) {
    case hmi_apis::Common_Result::SUCCESS: {
      return result_codes::kSuccess;
      break;
    }
    case hmi_apis::Common_Result::UNSUPPORTED_REQUEST: {
      return result_codes::kUnsupportedRequest;
      break;
    }
    case hmi_apis::Common_Result::UNSUPPORTED_RESOURCE: {
      return result_codes::kUnsupportedResource;
      break;
    }
    case hmi_apis::Common_Result::DISALLOWED: {
      return result_codes::kDisallowed;
      break;
    }
    case hmi_apis::Common_Result::REJECTED: {
      return result_codes::kRejected;
      break;
    }
    case hmi_apis::Common_Result::ABORTED: {
      return result_codes::kAborted;
      break;
    }
    case hmi_apis::Common_Result::IGNORED: {
      return result_codes::kIgnored;
      break;
    }
    case hmi_apis::Common_Result::RETRY: {
      return result_codes::kRetry;
      break;
    }
    case hmi_apis::Common_Result::IN_USE: {
      return result_codes::kInUse;
      break;
    }
    case hmi_apis::Common_Result::DATA_NOT_AVAILABLE: {
      return result_codes::kVehicleDataNotAvailable;
      break;
    }
    case hmi_apis::Common_Result::TIMED_OUT: {
      return result_codes::kTimedOut;
      break;
    }
    case hmi_apis::Common_Result::INVALID_DATA: {
      return result_codes::kInvalidData;
      break;
    }
    case hmi_apis::Common_Result::CHAR_LIMIT_EXCEEDED: {
      return result_codes::kCharLimitExceeded;
      break;
    }
    case hmi_apis::Common_Result::INVALID_ID: {
      return result_codes::kInvalidId;
      break;
    }
    case hmi_apis::Common_Result::DUPLICATE_NAME: {
      return result_codes::kDuplicateName;
      break;
    }
    case hmi_apis::Common_Result::APPLICATION_NOT_REGISTERED: {
      return result_codes::kApplicationNotRegistered;
      break;
    }
    case hmi_apis::Common_Result::WRONG_LANGUAGE: {
      return result_codes::kWrongLanguage;
      break;
    }
    case hmi_apis::Common_Result::OUT_OF_MEMORY: {
      return result_codes::kOutOfMemory;
      break;
    }
    case hmi_apis::Common_Result::TOO_MANY_PENDING_REQUESTS: {
      return result_codes::kTooManyPendingRequests;
      break;
    }
    case hmi_apis::Common_Result::NO_APPS_REGISTERED: {
      return result_codes::kApplicationNotRegistered;
      break;
    }
    case hmi_apis::Common_Result::NO_DEVICES_CONNECTED: {
      return result_codes::kApplicationNotRegistered;
      break;
    }
    case hmi_apis::Common_Result::WARNINGS: {
      return result_codes::kWarnings;
      break;
    }
    case hmi_apis::Common_Result::GENERIC_ERROR: {
      return result_codes::kGenericError;
      break;
    }
    case hmi_apis::Common_Result::USER_DISALLOWED: {
      return result_codes::kUserDisallowed;
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown HMI result code " << hmi_code);
      return result_codes::kGenericError;
      break;
    }
  }
}

CANAppExtensionPtr BaseCommandRequest::GetAppExtension(
  application_manager::ApplicationSharedPtr app) const {
  if (!app.valid()) {
    return NULL;
  }

  functional_modules::ModuleID id = CANModule::instance()->GetModuleID();

  CANAppExtensionPtr can_app_extension;
  application_manager::AppExtensionPtr app_extension = app->QueryInterface(id);
  if (!app_extension.valid()) {
    app_extension = new CANAppExtension(id);
    app->AddExtension(app_extension);
  }

  can_app_extension =
    application_manager::AppExtensionPtr::static_pointer_cast<CANAppExtension>(
      app_extension);

  return can_app_extension;
}

bool BaseCommandRequest::ParseResultCode(const Json::Value& value,
    std::string& result_code,
    std::string& info) {
  result_code = result_codes::kInvalidData;
  info = "";

  if (value.isMember(kResult) && value[kResult].isMember(kCode)) {
    result_code = GetMobileResultCode(
                    static_cast<hmi_apis::Common_Result::eType>(
                      value[kResult][kCode].asInt()));
  } else if (value.isMember(kError) && value[kError].isMember(kCode)) {
    result_code = GetMobileResultCode(
                    static_cast<hmi_apis::Common_Result::eType>(
                      value[kError][kCode].asInt()));

    if (value[kError].isMember(kMessage)) {
      info = value[kError][kMessage].asCString();
    }
  }

  if ((result_codes::kSuccess == result_code) ||
      (result_codes::kWarnings == result_code)) {
    return true;
  }

  return false;
}

}  // namespace commands

}  // namespace can_cooperation

