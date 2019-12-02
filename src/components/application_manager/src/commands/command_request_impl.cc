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

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/app_service_manager.h"
#include "application_manager/application_impl.h"
#include "application_manager/application_manager.h"

#include "smart_objects/enum_schema_item.h"

namespace application_manager {

namespace {
struct AppExtensionPredicate {
  AppExtensionUID uid;
  bool operator()(const ApplicationSharedPtr app) {
    return app ? (app->QueryInterface(uid).use_count() != 0) : false;
  }
};
}  // namespace

namespace commands {

CommandRequestImpl::CommandRequestImpl(
    const MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandImpl(message,
                  application_manager,
                  rpc_service,
                  hmi_capabilities,
                  policy_handler)
    , EventObserver(application_manager.event_dispatcher())
    , current_state_(RequestState::AwaitingHMIResponse)
    , state_lock_(std::make_shared<sync_primitives::RecursiveLock>()) {}

CommandRequestImpl::~CommandRequestImpl() {
  CleanUp();
}

bool CommandRequestImpl::CheckPermissions() {
  return CommandImpl::CheckPermissions();
}

bool CommandRequestImpl::Init() {
  return CommandImpl::Init();
}

bool CommandRequestImpl::CleanUp() {
  return CommandImpl::CleanUp();
}

void CommandRequestImpl::Run() {}

uint32_t CommandRequestImpl::default_timeout() const {
  return CommandImpl::default_timeout();
}

void CommandRequestImpl::on_event(const event_engine::MobileEvent& event) {}

uint32_t CommandRequestImpl::correlation_id() const {
  return CommandImpl::correlation_id();
}

bool CommandRequestImpl::CheckSyntax(const std::string& str,
                                     bool allow_empty_line) {
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

int32_t CommandRequestImpl::function_id() const {
  return CommandImpl::function_id();
}

uint32_t CommandRequestImpl::connection_key() const {
  return CommandImpl::connection_key();
}

bool CommandRequestImpl::AllowedToTerminate() {
  return CommandImpl::AllowedToTerminate();
}

void CommandRequestImpl::SendProviderRequest(
    const mobile_apis::FunctionID::eType& mobile_function_id,
    const hmi_apis::FunctionID::eType& hmi_function_id,
    const smart_objects::SmartObject* msg,
    bool use_events) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool hmi_destination = false;
  ApplicationSharedPtr app;
  // Default error code and error message
  std::string error_msg = "No app service provider available";
  mobile_apis::Result::eType error_code =
      mobile_apis::Result::DATA_NOT_AVAILABLE;

  if ((*msg)[strings::msg_params].keyExists(strings::service_type)) {
    std::string service_type =
        (*msg)[strings::msg_params][strings::service_type].asString();
    application_manager_.GetAppServiceManager().GetProviderByType(
        service_type, true, app, hmi_destination);
    error_msg = "No app service provider with serviceType: " + service_type +
                " is available";
    error_code = mobile_apis::Result::DATA_NOT_AVAILABLE;
  } else if ((*msg)[strings::msg_params].keyExists(strings::service_id)) {
    std::string service_id =
        (*msg)[strings::msg_params][strings::service_id].asString();
    application_manager_.GetAppServiceManager().GetProviderByID(
        service_id, true, app, hmi_destination);
    error_msg = "No app service provider with serviceId: " + service_id +
                " is available";
    error_code = mobile_apis::Result::INVALID_ID;
  }

  if (hmi_destination) {
    LOG4CXX_DEBUG(logger_, "Sending Request to HMI Provider");
    application_manager_.IncreaseForwardedRequestTimeout(connection_key(),
                                                         correlation_id());
    SendHMIRequest(hmi_function_id, &(*msg)[strings::msg_params], use_events);
    return;
  }

  if (!app) {
    LOG4CXX_DEBUG(logger_, "Invalid App Provider pointer");
    SendResponse(false, error_code, error_msg.c_str());
    return;
  }

  if (connection_key() == app->app_id()) {
    SendResponse(false,
                 mobile_apis::Result::IGNORED,
                 "Consumer app is same as producer app");
    return;
  }

  smart_objects::SmartObjectSPtr new_msg =
      std::make_shared<smart_objects::SmartObject>();
  smart_objects::SmartObject& request = *new_msg;

  request[strings::params] = (*msg)[strings::params];
  request[strings::msg_params] = (*msg)[strings::msg_params];
  request[strings::params][strings::connection_key] = app->app_id();

  application_manager_.IncreaseForwardedRequestTimeout(connection_key(),
                                                       correlation_id());
  SendMobileRequest(mobile_function_id, new_msg, use_events);
}

void CommandRequestImpl::SendMobileRequest(
    const mobile_apis::FunctionID::eType& function_id,
    smart_objects::SmartObjectSPtr msg,
    bool use_events) {
  smart_objects::SmartObject& request = *msg;

  const uint32_t mobile_correlation_id =
      application_manager_.GetNextMobileCorrelationID();

  request[strings::params][strings::correlation_id] = mobile_correlation_id;
  request[strings::params][strings::message_type] = MessageType::kRequest;
  if (use_events) {
    LOG4CXX_DEBUG(logger_,
                  "SendMobileRequest subscribe_on_event "
                      << function_id << " " << mobile_correlation_id);
    subscribe_on_event(function_id, mobile_correlation_id);
  }

  if (!rpc_service_.ManageMobileCommand(msg, SOURCE_SDL)) {
    LOG4CXX_ERROR(logger_, "Unable to send request to mobile");
  }
}

bool CommandRequestImpl::AllowedToTerminate() {
  return CommandImpl::AllowedToTerminate();
}

void CommandRequestImpl::SetAllowedToTerminate(const bool allowed) {
  return CommandImpl::SetAllowedToTerminate(allowed);
}

bool CommandRequestImpl::CheckSyntax(const std::string& str,
                                     bool allow_empty_line) {
  return CommandImpl::CheckSyntax(str, allow_empty_line);
}

bool CommandRequestImpl::ReplaceMobileWithHMIAppId(
    ns_smart_device_link::ns_smart_objects::SmartObject& message) {
  return CommandImpl::ReplaceMobileWithHMIAppId(message);
}

bool CommandRequestImpl::ReplaceHMIWithMobileAppId(
    ns_smart_device_link::ns_smart_objects::SmartObject& message) {
  return CommandImpl::ReplaceHMIWithMobileAppId(message);
}

void CommandRequestImpl::OnTimeOut() {}

void CommandRequestImpl::on_event(const event_engine::Event&) {}

void CommandRequestImpl::HandleTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock auto_lock(*state_lock_);
    if (helpers::Compare<RequestState, helpers::EQ, helpers::ONE>(
            current_state(),
            RequestState::kHandlingResponse,
            RequestState::kResponded)) {
      LOG4CXX_DEBUG(logger_, "Current request state = Responding/Responded");
      return;
    }
    set_current_state(RequestState::kTimedOut);
  }
  OnTimeOut();
}

bool CommandRequestImpl::IsMobileResultSuccess(
    const mobile_apis::Result::eType result_code) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  return Compare<mobile_apis::Result::eType, EQ, ONE>(
      result_code,
      mobile_apis::Result::SUCCESS,
      mobile_apis::Result::WARNINGS,
      mobile_apis::Result::WRONG_LANGUAGE,
      mobile_apis::Result::RETRY,
      mobile_apis::Result::SAVED);
}

bool CommandRequestImpl::IsHMIResultSuccess(
    const hmi_apis::Common_Result::eType result_code) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  return Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
      result_code,
      hmi_apis::Common_Result::SUCCESS,
      hmi_apis::Common_Result::WARNINGS,
      hmi_apis::Common_Result::WRONG_LANGUAGE,
      hmi_apis::Common_Result::RETRY,
      hmi_apis::Common_Result::SAVED);
}

void CommandRequestImpl::HandleOnEvent(const event_engine::Event& event) {
  {
    sync_primitives::AutoLock auto_lock(*state_lock_);
    if (RequestState::kTimedOut == current_state_) {
      LOG4CXX_DEBUG(logger_, "current_state_ = kTimedOut");
      return;
    }
    set_current_state(RequestState::kHandlingResponse);
  }
  on_event(event);

  if (!state_lock_weak.expired()) {
    sync_primitives::AutoLock auto_lock(*state_lock_);
    if (RequestState::kHandlingResponse == current_state()) {
      LOG4CXX_DEBUG(logger_, "Response was not sent, resetting state");
      set_current_state(RequestState::kAwaitingResponse);
    }
  }
}

void CommandRequestImpl::GetInfo(
    const smart_objects::SmartObject& response_from_hmi,
    std::string& out_info) {
  if (response_from_hmi[strings::msg_params].keyExists(strings::info)) {
    if (!response_from_hmi[strings::msg_params][strings::info].empty()) {
      out_info =
          response_from_hmi[strings::msg_params][strings::info].asString();
    }
  }
}

void CommandRequestImpl::OnUpdateTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);
  set_current_state(RequestState::kAwaitingResponse);
}

void CommandRequestImpl::StartAwaitForInterface(
    const HmiInterfaces::InterfaceID& interface_id) {
  sync_primitives::AutoLock lock(awaiting_response_interfaces_lock_);
  awaiting_response_interfaces_.insert(interface_id);
}

bool CommandRequestImpl::IsInterfaceAwaited(
    const HmiInterfaces::InterfaceID& interface_id) const {
  sync_primitives::AutoLock lock(awaiting_response_interfaces_lock_);
  return helpers::in_range(awaiting_response_interfaces_, interface_id);
}

void CommandRequestImpl::EndAwaitForInterface(
    const HmiInterfaces::InterfaceID& interface_id) {
  sync_primitives::AutoLock lock(awaiting_response_interfaces_lock_);
  awaiting_response_interfaces_.erase(interface_id);
}

bool CommandRequestImpl::IsPendingResponseExist() const {
  sync_primitives::AutoLock lock(awaiting_response_interfaces_lock_);
  return !awaiting_response_interfaces_.empty();
}

CommandRequestImpl::RequestState CommandRequestImpl::current_state() const {
  sync_primitives::AutoLock auto_lock(*state_lock_);
  return current_state_;
}

void CommandRequestImpl::set_current_state(
    const CommandRequestImpl::RequestState state) {
  sync_primitives::AutoLock auto_lock(*state_lock_);
  current_state_ = state;
}

#ifdef __QNX__
void CommandRequestImpl::OnHMIMessageSent() {}
#endif

}  // namespace commands

}  // namespace application_manager
