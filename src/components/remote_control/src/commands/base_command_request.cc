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
#include "utils/make_shared.h"
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
    : Command(rc_module)
    , message_(message)
    , msg_json_(MessageHelper::StringToValue(message->json_message()))
    , auto_allowed_(false) {
  service_ = rc_module_.service();
  app_ = service_->GetApplication(message_->connection_key());
}

BaseCommandRequest::~BaseCommandRequest() {
  rc_module_.event_dispatcher().remove_observer(this);
}

void BaseCommandRequest::OnTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);
  SetResourceState(msg_json_, ResourceState::FREE);

  PrepareResponse(
      false, result_codes::kGenericError, "Request timeout expired.");
  rc_module_.SendTimeoutResponseToMobile(message_);
}

void BaseCommandRequest::PrepareResponse(const bool success,
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
}

void BaseCommandRequest::SendResponse(const bool success,
                                      const char* result_code,
                                      const std::string& info) {
  LOG4CXX_AUTO_TRACE(logger_);
  SetResourceState(msg_json_, ResourceState::FREE);
  PrepareResponse(success, result_code, info);
  rc_module_.SendResponseToMobile(message_);
}

void BaseCommandRequest::SendMessageToHMI(
    const application_manager::MessagePtr& message_to_send) {
  LOG4CXX_AUTO_TRACE(logger_);
  using application_manager::HmiInterfaces;

  const bool is_rc_available =
      service_->IsInterfaceAvailable(HmiInterfaces::HMI_INTERFACE_RC);
  LOG4CXX_DEBUG(logger_, "HMI interface RC is available: " << is_rc_available);
  if (!is_rc_available) {
    const bool success = false;
    const char* result_code = result_codes::kUnsupportedResource;
    const std::string info = "Remote control is not supported by system";

    SendResponse(success, result_code, info);
    return;
  }

  const std::string function_name = message_to_send->function_name();
  const int32_t correlation_id = message_to_send->correlation_id();
  LOG4CXX_DEBUG(logger_,
                "Subsribing to response for function: "
                    << function_name
                    << " and correlation id: " << correlation_id);

  rc_module_.event_dispatcher().add_observer(
      function_name, correlation_id, this);

  LOG4CXX_DEBUG(logger_, "HMI Request:\n " << message_to_send->json_message());

  service_->SendMessageToHMI(message_to_send);
}

void BaseCommandRequest::SendRequest(const char* function_id,
                                     const Json::Value& message_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::MessagePtr message_to_send =
      CreateHmiRequest(function_id, message_params);
  SendMessageToHMI(message_to_send);
}

application_manager::MessagePtr BaseCommandRequest::CreateHmiRequest(
    const char* function_id, const Json::Value& message_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  const uint32_t hmi_app_id = app_->hmi_app_id();
  return MessageHelper::CreateHmiRequest(
      function_id, hmi_app_id, message_params, rc_module_);
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
    }
    case hmi_apis::Common_Result::UNSUPPORTED_REQUEST: {
      return result_codes::kUnsupportedRequest;
    }
    case hmi_apis::Common_Result::UNSUPPORTED_RESOURCE: {
      return result_codes::kUnsupportedResource;
    }
    case hmi_apis::Common_Result::DISALLOWED: {
      return result_codes::kDisallowed;
    }
    case hmi_apis::Common_Result::REJECTED: {
      return result_codes::kRejected;
    }
    case hmi_apis::Common_Result::ABORTED: {
      return result_codes::kAborted;
    }
    case hmi_apis::Common_Result::IGNORED: {
      return result_codes::kIgnored;
    }
    case hmi_apis::Common_Result::RETRY: {
      return result_codes::kRetry;
    }
    case hmi_apis::Common_Result::IN_USE: {
      return result_codes::kInUse;
    }
    case hmi_apis::Common_Result::DATA_NOT_AVAILABLE: {
      return result_codes::kVehicleDataNotAvailable;
    }
    case hmi_apis::Common_Result::TIMED_OUT: {
      return result_codes::kTimedOut;
    }
    case hmi_apis::Common_Result::INVALID_DATA: {
      return result_codes::kInvalidData;
    }
    case hmi_apis::Common_Result::CHAR_LIMIT_EXCEEDED: {
      return result_codes::kCharLimitExceeded;
    }
    case hmi_apis::Common_Result::INVALID_ID: {
      return result_codes::kInvalidId;
    }
    case hmi_apis::Common_Result::DUPLICATE_NAME: {
      return result_codes::kDuplicateName;
    }
    case hmi_apis::Common_Result::APPLICATION_NOT_REGISTERED: {
      return result_codes::kApplicationNotRegistered;
    }
    case hmi_apis::Common_Result::WRONG_LANGUAGE: {
      return result_codes::kWrongLanguage;
    }
    case hmi_apis::Common_Result::OUT_OF_MEMORY: {
      return result_codes::kOutOfMemory;
    }
    case hmi_apis::Common_Result::TOO_MANY_PENDING_REQUESTS: {
      return result_codes::kTooManyPendingRequests;
    }
    case hmi_apis::Common_Result::NO_APPS_REGISTERED: {
      return result_codes::kApplicationNotRegistered;
    }
    case hmi_apis::Common_Result::NO_DEVICES_CONNECTED: {
      return result_codes::kApplicationNotRegistered;
    }
    case hmi_apis::Common_Result::WARNINGS: {
      return result_codes::kWarnings;
    }
    case hmi_apis::Common_Result::GENERIC_ERROR: {
      return result_codes::kGenericError;
    }
    case hmi_apis::Common_Result::USER_DISALLOWED: {
      return result_codes::kUserDisallowed;
    }
    case hmi_apis::Common_Result::READ_ONLY: {
      return result_codes::kReadOnly;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown HMI result code " << hmi_code);
      return result_codes::kGenericError;
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
  if (!Validate()) {
    LOG4CXX_WARN(logger_, "Request message validation failed !");
    SendResponse(
        false, result_codes::kInvalidData, "Validation by schema failed");
    return;
  }
  LOG4CXX_TRACE(logger_, "Request message validated successfully!");
  using application_manager::HmiInterfaces;
  if (!service_->IsInterfaceAvailable(HmiInterfaces::HMI_INTERFACE_RC)) {
    LOG4CXX_WARN(logger_, "HMI interface RC is not available");
    SendResponse(false,
                 result_codes::kUnsupportedResource,
                 "Remote control is not supported by system");
    return;
  }
  LOG4CXX_TRACE(logger_, "RC interface is available!");
  if (CheckPolicyPermissions() && CheckDriverConsent()) {
    if (AqcuireResources()) {
      Execute();  // run child's logic
    }
    // If resource is not aqcuired, AqcuireResources method will either
    // send response to mobile or 
    // send additional request to HMI to ask driver consent
  }
  // TODO : send error in case if policy permissions failed
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
    LOG4CXX_WARN(logger_,
                 "Function \"" << message_->function_name() << "\" (#"
                               << message_->function_id()
                               << ") not allowed by policy");

    SendResponse(false, result_codes::kDisallowed, "");
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

bool BaseCommandRequest::AqcuireResources() {
  LOG4CXX_AUTO_TRACE(logger_);
  const Json::Value message_params =
      MessageHelper::StringToValue(message_->json_message());

  if (!IsResourceFree(ModuleType(message_params))) {
    LOG4CXX_WARN(logger_, "Resource is busy.");
    SendResponse(false, result_codes::kInUse, "");
    return false;
  }

  AcquireResult::eType acquire_result = AcquireResource(message_params);
  switch (acquire_result) {
    case AcquireResult::ALLOWED: {
      SetResourceState(msg_json_, ResourceState::BUSY);
      return true;
    }
    case AcquireResult::IN_USE: {
      SendResponse(false, result_codes::kInUse, "");
      return false;
    }
    case AcquireResult::ASK_DRIVER: {
      SetResourceState(msg_json_, ResourceState::BUSY);

      Json::Value value;
      Json::Reader reader;
      LOG4CXX_DEBUG(logger_, "Request: " << message_->json_message());
      reader.parse(message_->json_message(), value);

      SendGetUserConsent(value);

      return false;
    }
    case AcquireResult::REJECTED: {
      SendResponse(false, result_codes::kRejected, "");
      return false;
    }
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

  SetResourceState(msg_json_, ResourceState::FREE);

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

  application_manager::Message& hmi_response = *(event.event_message());
  const application_manager::MessageValidationResult validate_result =
      service_->ValidateMessageBySchema(hmi_response);
  LOG4CXX_DEBUG(logger_,
                "HMI response validation result is " << validate_result);

  if (validate_result !=
      application_manager::MessageValidationResult::SUCCESS) {
    SendResponse(
        false, result_codes::kGenericError, "HMI has sent invalid parameters");
    return;
  }

  Json::Value value;
  Json::Reader reader;
  reader.parse(event.event_message()->json_message(), value);

  std::string result_code;
  std::string info;
  const bool is_succeeded = ParseResultCode(value, result_code, info);

  bool is_allowed = false;
  if (is_succeeded) {
    if (IsMember(value[kResult], message_params::kAllowed) &&
        value[kResult][message_params::kAllowed].isBool()) {
      is_allowed = value[kResult][message_params::kAllowed].asBool();
    }

    const std::string module = ModuleType(msg_json_);

    // Check the actual User's answer.
    LOG4CXX_DEBUG(logger_,
                  "Setting allowed access for " << app_->app_id() << " for "
                                                << module);
    service_->SetAccess(app_->app_id(), module, is_allowed);
    CheckHMILevel(application_manager::kManual, is_succeeded);

    if (is_allowed) {
      rc_module_.resource_allocation_manager().ForceAcquireResource(
          module, app_->app_id());

      Execute();  // run child's logic
    } else {
      rc_module_.resource_allocation_manager().OnDriverDisallowed(
          module, app_->app_id());

      SendResponse(
          false,
          result_codes::kRejected,
          "The resource is in use and the driver disallows this remote "
          "control RPC");
    }
  } else {
    SendResponse(false, result_code.c_str(), info);
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
