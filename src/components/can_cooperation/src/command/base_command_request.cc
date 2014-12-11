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
#include "can_cooperation/event_engine/event_dispatcher.h"
#include "can_cooperation/message_helper.h"
#include "json/json.h"
#include "can_cooperation/can_module.h"

namespace can_cooperation {

namespace commands {

using event_engine::EventDispatcher;

CREATE_LOGGERPTR_GLOBAL(logger_, "BaseCommandRequest");

BaseCommandRequest::BaseCommandRequest(
    const application_manager::MessagePtr& message)
  : message_(message) {
    service_ = CANModule::instance()->GetServiceHandler();
}


BaseCommandRequest::~BaseCommandRequest() {
  EventDispatcher<application_manager::MessagePtr, std::string>::instance()->
      remove_observer(this);
}

// TODO(VS): Add string(in json format) with response params
void BaseCommandRequest::SendResponse(const bool success,
                  const char* result_code,
                  const std::string info) {
 message_->set_message_type(application_manager::MessageType::kResponse);
 Json::Value msg_params;

 msg_params["success"] = success;
 msg_params["resultCode"] = result_code;
 if (!info.empty()) {
   msg_params["info"] = info;
 }

 Json::FastWriter writer;
 std::string params = writer.write(msg_params);
 message_->set_json_message(params);
 CANModule::instance()->SendResponseToMobile(message_);
}

void  BaseCommandRequest::SendRequest(const char* function_id,
                    const std::string& message_params,
                    bool is_hmi_request) {
  Json::Value msg;

  if (is_hmi_request) {
    msg["id"] = service_->GetNextCorrelationID();
  } else {
    msg["id"] = MessageHelper::GetNextCANCorrelationID();
  }

  EventDispatcher<application_manager::MessagePtr, std::string>::instance()->
      add_observer(function_id, msg["id"].asInt(), this);

  msg["jsonrpc"] = "2.0";
  msg["method"] = function_id;
  if (!message_params.empty()) {
    msg["params"] = message_params;
  }

  Json::FastWriter writer;
  std::string json_msg = writer.write(msg);
  if (is_hmi_request) {
    application_manager::MessagePtr message_to_send(
        new application_manager::Message(
            protocol_handler::MessagePriority::kDefault));
    message_to_send->set_protocol_version(
        application_manager::ProtocolVersion::kHMI);
    message_to_send->set_correlation_id(msg["id"].asInt());
    message_to_send->set_json_message(json_msg);
    message_to_send->set_message_type(
        application_manager::MessageType::kRequest);

    service_->SendMessageToHMI(message_to_send);
  } else {
    CANModule::instance()->SendMessageToCan(json_msg);
  }
}

// TODO(VS): Create string constants for result codes and use them everywhere in module
const char* BaseCommandRequest::GetMobileResultCode(
    const hmi_apis::Common_Result::eType& hmi_code) const {
  switch (hmi_code) {
    case hmi_apis::Common_Result::SUCCESS: {
      return "SUCCESS";
      break;
    }
    case hmi_apis::Common_Result::UNSUPPORTED_REQUEST: {
      return "UNSUPPORTED_REQUEST";
      break;
    }
    case hmi_apis::Common_Result::UNSUPPORTED_RESOURCE: {
      return "UNSUPPORTED_RESOURCE";
      break;
    }
    case hmi_apis::Common_Result::DISALLOWED: {
      return "DISALLOWED";
      break;
    }
    case hmi_apis::Common_Result::REJECTED: {
      return "REJECTED";
      break;
    }
    case hmi_apis::Common_Result::ABORTED: {
      return "ABORTED";
      break;
    }
    case hmi_apis::Common_Result::IGNORED: {
      return "IGNORED";
      break;
    }
    case hmi_apis::Common_Result::RETRY: {
      return "RETRY";
      break;
    }
    case hmi_apis::Common_Result::IN_USE: {
      return "IN_USE";
      break;
    }
    case hmi_apis::Common_Result::DATA_NOT_AVAILABLE: {
      return "VEHICLE_DATA_NOT_AVAILABLE";
      break;
    }
    case hmi_apis::Common_Result::TIMED_OUT: {
      return "TIMED_OUT";
      break;
    }
    case hmi_apis::Common_Result::INVALID_DATA: {
      return "INVALID_DATA";
      break;
    }
    case hmi_apis::Common_Result::CHAR_LIMIT_EXCEEDED: {
      return "CHAR_LIMIT_EXCEEDED";
      break;
    }
    case hmi_apis::Common_Result::INVALID_ID: {
      return "INVALID_ID";
      break;
    }
    case hmi_apis::Common_Result::DUPLICATE_NAME: {
      return "DUPLICATE_NAME";
      break;
    }
    case hmi_apis::Common_Result::APPLICATION_NOT_REGISTERED: {
      return "APPLICATION_NOT_REGISTERED";
      break;
    }
    case hmi_apis::Common_Result::WRONG_LANGUAGE: {
      return "WRONG_LANGUAGE";
      break;
    }
    case hmi_apis::Common_Result::OUT_OF_MEMORY: {
      return "OUT_OF_MEMORY";
      break;
    }
    case hmi_apis::Common_Result::TOO_MANY_PENDING_REQUESTS: {
      return "TOO_MANY_PENDING_REQUESTS";
      break;
    }
    case hmi_apis::Common_Result::NO_APPS_REGISTERED: {
      return "APPLICATION_NOT_REGISTERED";
      break;
    }
    case hmi_apis::Common_Result::NO_DEVICES_CONNECTED: {
      return "APPLICATION_NOT_REGISTERED";
      break;
    }
    case hmi_apis::Common_Result::WARNINGS: {
      return "WARNINGS";
      break;
    }
    case hmi_apis::Common_Result::GENERIC_ERROR: {
      return "GENERIC_ERROR";
      break;
    }
    case hmi_apis::Common_Result::USER_DISALLOWED: {
      return "USER_DISALLOWED";
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown HMI result code " << hmi_code);
      return "GENERIC_ERROR";
      break;
    }
  }
}

CANAppExtensionPtr BaseCommandRequest::GetAppExtension(
    application_manager::ApplicationSharedPtr app) const {
  if (!app.valid()) {
    return CANAppExtensionPtr();
  }

  functional_modules::ModuleID id = CANModule::instance()->GetModuleID();

  CANAppExtensionPtr can_app_extension;
  application_manager::AppExtensionPtr app_extension = app->QueryInterface(id);
  if (!app_extension.valid()) {
    can_app_extension = new CANAppExtension(id);
    app->AddExtension(can_app_extension);
  } else {
    can_app_extension = static_cast<CANAppExtension*>(app_extension.get());
  }

  return can_app_extension;
}

}  // namespace commands

}  // namespace can_cooperation

