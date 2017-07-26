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

#include "remote_control/commands/base_command_request.h"
#include <cstring>
#include "remote_control/event_engine/event_dispatcher.h"
#include "remote_control/message_helper.h"
#include "remote_control/remote_control_plugin.h"
#include "remote_control/rc_module_constants.h"
#include "application_manager/application_manager_impl.h"

namespace remote_control {

namespace commands {

using rc_event_engine::EventDispatcher;

using namespace json_keys;

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

BaseCommandRequest::BaseCommandRequest(
    const application_manager::MessagePtr& message,
    RemotePluginInterface& rc_module)
    : Command(rc_module), message_(message), auto_allowed_(false) {
  service_ = rc_module_.service();
  app_ = service_->GetApplication(message_->connection_key());
}

BaseCommandRequest::~BaseCommandRequest() {
  rc_module_.event_dispatcher().remove_observer(this);
}

void BaseCommandRequest::OnTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);
  SendResponse(false, result_codes::kTimedOut, "Request timeout expired.");
}

void BaseCommandRequest::SendResponse(bool success,
                                      const char* result_code,
                                      const std::string& info) {
  LOG4CXX_AUTO_TRACE(logger_);
  message_->set_message_type(application_manager::MessageType::kResponse);
  Json::Value msg_params;

  if (!response_params_.isNull()) {
    msg_params = response_params_;
  }

  msg_params[kSuccess] = success;
  const bool is_timeout = (0 == strcmp(result_code, result_codes::kTimedOut));
  msg_params[kResultCode] =
      is_timeout ? result_codes::kGenericError : result_code;
  if (!info.empty()) {
    msg_params[kInfo] = info;
  }

  Json::FastWriter writer;
  std::string params = writer.write(msg_params);
  message_->set_json_message(params);
  if (is_timeout) {
    rc_module_.SendTimeoutResponseToMobile(message_);
  } else {
    rc_module_.SendResponseToMobile(message_);
  }
}

void BaseCommandRequest::SendRequest(const char* function_id,
                                     const Json::Value& message_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::MessagePtr message_to_send =
      CreateHmiRequest(function_id, message_params);
  service_->SendMessageToHMI(message_to_send);
}

application_manager::MessagePtr BaseCommandRequest::CreateHmiRequest(
    const char* function_id, const Json::Value& message_params) {
  Json::Value msg;

  msg[kId] = service_->GetNextCorrelationID();

  rc_module_.event_dispatcher().add_observer(
      function_id, msg[kId].asInt(), this);

  msg[kJsonrpc] = "2.0";
  msg[kMethod] = function_id;
  if (!message_params.isNull()) {
    msg[kParams] = message_params;
  }

  msg[kParams][json_keys::kAppId] = app_->hmi_app_id();

  Json::FastWriter writer;
  application_manager::MessagePtr message_to_send(
      new application_manager::Message(
          protocol_handler::MessagePriority::kDefault));
  message_to_send->set_protocol_version(
      application_manager::ProtocolVersion::kHMI);
  message_to_send->set_correlation_id(msg[kId].asInt());
  std::string json_msg = writer.write(msg);
  message_to_send->set_json_message(json_msg);
  message_to_send->set_message_type(application_manager::MessageType::kRequest);

  LOG4CXX_DEBUG(logger_, "\nRequest to HMI: \n" << json_msg);

  return message_to_send;
}

bool BaseCommandRequest::Validate() {
  return application_manager::MessageValidationResult::SUCCESS ==
         service_->ValidateMessageBySchema(*message_);
}

bool BaseCommandRequest::ParseJsonString(Json::Value* parsed_msg) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(parsed_msg);
  if (!parsed_msg)
    return false;

  (*parsed_msg) = MessageHelper::StringToValue(message_->json_message());
  if (Json::ValueType::nullValue == parsed_msg->type()) {
    LOG4CXX_ERROR(logger_,
                  "Invalid JSON received in " << message_->json_message());
    SendResponse(
        false, result_codes::kInvalidData, "Mobile request validation failed!");
    return false;
  }
  return true;
}

const char* BaseCommandRequest::GetMobileResultCode(
    const hmi_apis::Common_Result::eType& hmi_code) const {
  LOG4CXX_AUTO_TRACE(logger_);
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
    case hmi_apis::Common_Result::READ_ONLY: {
      return result_codes::kReadOnly;
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown HMI result code " << hmi_code);
      return result_codes::kGenericError;
      break;
    }
  }
}

RCAppExtensionPtr BaseCommandRequest::GetAppExtension(
    application_manager::ApplicationSharedPtr app) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!app) {
    return NULL;
  }

  functional_modules::ModuleID id = rc_module_.GetModuleID();

  RCAppExtensionPtr rc_app_extension;
  application_manager::AppExtensionPtr app_extension = app->QueryInterface(id);
  if (!app_extension) {
    LOG4CXX_DEBUG(logger_, "New app extension will be created");
    app_extension = new RCAppExtension(id);
    app->AddExtension(app_extension);
  }

  rc_app_extension =
      application_manager::AppExtensionPtr::static_pointer_cast<RCAppExtension>(
          app_extension);

  return rc_app_extension;
}

bool BaseCommandRequest::ParseResultCode(const Json::Value& value,
                                         std::string& result_code,
                                         std::string& info) {
  LOG4CXX_AUTO_TRACE(logger_);
  result_code = result_codes::kInvalidData;
  info = "";

  if (IsMember(value, kResult) && IsMember(value[kResult], kCode)) {
    result_code =
        GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
            value[kResult][kCode].asInt()));
  } else if (IsMember(value, kError) && IsMember(value[kError], kCode)) {
    result_code =
        GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
            value[kError][kCode].asInt()));

    if (IsMember(value[kError], kMessage)) {
      info = value[kError][kMessage].asCString();
    }
  }

  if ((result_codes::kSuccess == result_code) ||
      (result_codes::kWarnings == result_code)) {
    return true;
  }

  return false;
}

void BaseCommandRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (Validate()) {
    LOG4CXX_INFO(logger_, "Request message validated successfully!");
    if (CheckPolicyPermissions() && CheckDriverConsent()) {
      Execute();  // run child's logic
    }
  } else {
    SendResponse(false, result_codes::kInvalidData, "");
  }
}

bool BaseCommandRequest::CheckPolicyPermissions() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!app_) {
    LOG4CXX_ERROR(logger_, "Application doesn't registered!");
    SendResponse(false, result_codes::kApplicationNotRegistered, "");
    return false;
  }

  mobile_apis::Result::eType ret = service_->CheckPolicyPermissions(message_);
  if (ret != mobile_apis::Result::eType::SUCCESS) {
    SendResponse(false, result_codes::kDisallowed, "");
    LOG4CXX_WARN(logger_,
                 "Function \"" << message_->function_name() << "\" (#"
                               << message_->function_id()
                               << ") not allowed by policy");
    return false;
  }

  return true;
}

application_manager::TypeAccess BaseCommandRequest::CheckAccess(
    const Json::Value& message) {
  const std::string& module = ModuleType(message);
  return service_->CheckAccess(
      app_->app_id(), module, message_->function_name(), ControlData(message));
}

bool BaseCommandRequest::CheckDriverConsent() {
  LOG4CXX_AUTO_TRACE(logger_);
  RCAppExtensionPtr extension = GetAppExtension(app_);
  if (!extension) {
    return false;
  }
  Json::Value value;
  Json::Reader reader;
  LOG4CXX_DEBUG(logger_, "Request: " << message_->json_message());
  reader.parse(message_->json_message(), value);

  application_manager::TypeAccess access = CheckAccess(value);

  if (IsAutoAllowed(access)) {
    set_auto_allowed(true);
    return true;
  }
  if (IsNeededDriverConsent(access)) {
    SendGetUserConsent(value);
  } else {
    SendDisallowed(access);
  }
  return false;
}

bool BaseCommandRequest::IsNeededDriverConsent(
    application_manager::TypeAccess access) const {
  return access == application_manager::kManual;
}

bool BaseCommandRequest::IsAutoAllowed(
    application_manager::TypeAccess access) const {
  return access == application_manager::kAllowed;
}

void BaseCommandRequest::SendDisallowed(
    application_manager::TypeAccess access) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string info;
  switch (access) {
    case application_manager::kAllowed:
    case application_manager::kManual:
      return;
    case application_manager::kDisallowed:
      info = disallowed_info_.empty()
                 ? "The RPC is disallowed by vehicle settings"
                 : disallowed_info_;
      break;
    case application_manager::kNone:
      info = "Internal issue";
      break;
    default:
      info = "Unknown issue";
  }
  LOG4CXX_ERROR(logger_, info);
  SendResponse(false, result_codes::kDisallowed, info);
}

void BaseCommandRequest::SendGetUserConsent(const Json::Value& value) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(app_);
  Json::Value params;
  params[json_keys::kAppId] = app_->hmi_app_id();
  params[message_params::kModuleType] = ModuleType(value);
  SendRequest(functional_modules::hmi_api::get_user_consent, params);
}

std::string BaseCommandRequest::ModuleType(const Json::Value& message) {
  return "";
}

std::vector<std::string> BaseCommandRequest::ControlData(
    const Json::Value& message) {
  return std::vector<std::string>();
}

void BaseCommandRequest::on_event(
    const rc_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (event.id() == functional_modules::hmi_api::get_user_consent) {
    ProcessAccessResponse(event);
  } else {
    if (auto_allowed()) {
      UpdateHMILevel(event);
    }
    OnEvent(event);  // run child's logic
  }
}

void BaseCommandRequest::UpdateHMILevel(
    const rc_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);
  RCAppExtensionPtr extension = GetAppExtension(app_);
  if (!extension) {
    return;
  }
  if (!extension->is_on_driver_device()) {
    Json::Value value =
        MessageHelper::StringToValue(event.event_message()->json_message());
    std::string result_code;
    std::string info;
    bool success = ParseResultCode(value, result_code, info);
    CheckHMILevel(application_manager::kAllowed, success);
  }
}

void BaseCommandRequest::ProcessAccessResponse(
    const rc_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!app_) {
    LOG4CXX_ERROR(logger_, "Application doesn't registered!");
    SendResponse(false, result_codes::kApplicationNotRegistered, "");
    return;
  }
  Json::Value value;
  Json::Reader reader;
  reader.parse(event.event_message()->json_message(), value);

  std::string result_code;
  std::string info;
  bool allowed = ParseResultCode(value, result_code, info);
  // Check if valid successfull message has arrived
  if (allowed) {
    if (IsMember(value[kResult], message_params::kAllowed) &&
        value[kResult][message_params::kAllowed].isBool()) {
      allowed = value[kResult][message_params::kAllowed].asBool();
    } else {
      allowed = false;
    }
  }

  // Check the actual User's answer.
  if (allowed) {
    Json::Value request;
    reader.parse(message_->json_message(), request);
    std::string module = ModuleType(request);
    LOG4CXX_DEBUG(logger_,
                  "Setting allowed access for " << app_->app_id() << " for "
                                                << module);
    service_->SetAccess(app_->app_id(), module, allowed);
    CheckHMILevel(application_manager::kManual, allowed);
    Execute();  // run child's logic
  } else {
    SendResponse(false,
                 result_codes::kUserDisallowed,
                 "The driver disallows this remote-control RPC");
  }
}

void BaseCommandRequest::CheckHMILevel(application_manager::TypeAccess access,
                                       bool user_consented) {
  LOG4CXX_AUTO_TRACE(logger_);
  switch (access) {
    case application_manager::kAllowed:
      if (user_consented) {
        if (app_->hmi_level() == mobile_apis::HMILevel::eType::HMI_NONE) {
          LOG4CXX_DEBUG(logger_,
                        "RSDL functionality for "
                            << app_->name().c_str()
                            << " is auto allowed; setting BACKGROUND level.");
          service_->ChangeNotifyHMILevel(
              app_, mobile_apis::HMILevel::eType::HMI_BACKGROUND);
        }
      }
      break;
    case application_manager::kManual: {
      if (user_consented) {
        if (app_->hmi_level() == mobile_apis::HMILevel::eType::HMI_NONE ||
            app_->hmi_level() == mobile_apis::HMILevel::eType::HMI_BACKGROUND) {
          LOG4CXX_DEBUG(logger_,
                        "User consented RSDL functionality for "
                            << app_->name().c_str()
                            << "; setting LIMITED level.");
          service_->ChangeNotifyHMILevel(
              app_, mobile_apis::HMILevel::eType::HMI_LIMITED);
        }
      }
      break;
    }
    case application_manager::kDisallowed:
    case application_manager::kNone:
    default:
      LOG4CXX_DEBUG(logger_,
                    "No access information or disallowed: "
                        << "do nothing about hmi levels");
      break;
  }
}

}  // namespace commands
}  // namespace remote_control
