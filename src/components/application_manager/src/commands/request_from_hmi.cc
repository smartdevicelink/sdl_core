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
#include "utils/helpers.h"

#include "smart_objects/enum_schema_item.h"

#include "smart_objects/enum_schema_item.h"

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

void RequestFromHMI::on_event(const event_engine::MobileEvent& event) {}

void RequestFromHMI::SendResponse(
    const bool success,
    const uint32_t correlation_id,
    const hmi_apis::FunctionID::eType function_id,
    const hmi_apis::Common_Result::eType result_code,
    const smart_objects::SmartObject* response_params,
    commands::Command::CommandSource source) {
  smart_objects::SmartObjectSPtr message =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  FillCommonParametersOfSO(*message, correlation_id, function_id);
  (*message)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message)[strings::params][hmi_response::code] = 0;
  (*message)[strings::msg_params][strings::success] = success;
  (*message)[strings::msg_params][strings::result_code] = result_code;

  if (response_params) {
    (*message)[strings::msg_params] = *response_params;
  }

  rpc_service_.ManageHMICommand(message, source);
}

void RequestFromHMI::SendErrorResponse(
    const uint32_t correlation_id,
    const hmi_apis::FunctionID::eType function_id,
    const hmi_apis::Common_Result::eType result_code,
    const std::string error_message) {
  smart_objects::SmartObjectSPtr message =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  FillCommonParametersOfSO(*message, correlation_id, function_id);
  (*message)[strings::params][strings::message_type] =
      MessageType::kErrorResponse;
  (*message)[strings::params][hmi_response::code] = result_code;
  (*message)[strings::params][strings::error_msg] = error_message;

  rpc_service_.ManageHMICommand(message);
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

bool RequestFromHMI::IsMobileResultSuccess(
    mobile_apis::Result::eType result_code) const {
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

bool RequestFromHMI::IsHMIResultSuccess(
    hmi_apis::Common_Result::eType result_code,
    HmiInterfaces::InterfaceID interface) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  if (Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          result_code,
          hmi_apis::Common_Result::SUCCESS,
          hmi_apis::Common_Result::WARNINGS,
          hmi_apis::Common_Result::WRONG_LANGUAGE,
          hmi_apis::Common_Result::RETRY,
          hmi_apis::Common_Result::SAVED)) {
    return true;
  }

  const HmiInterfaces::InterfaceState state =
      application_manager_.hmi_interfaces().GetInterfaceState(interface);
  if ((hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == result_code) &&
      (HmiInterfaces::STATE_NOT_AVAILABLE != state)) {
    return true;
  }
  return false;
}

void RequestFromHMI::SendProviderRequest(
    const mobile_apis::FunctionID::eType& mobile_function_id,
    const hmi_apis::FunctionID::eType& hmi_function_id,
    const smart_objects::SmartObject* msg,
    bool use_events) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool hmi_destination = false;
  ApplicationSharedPtr app;
  if ((*msg)[strings::msg_params].keyExists(strings::service_type)) {
    std::string service_type =
        (*msg)[strings::msg_params][strings::service_type].asString();
    application_manager_.GetAppServiceManager().GetProviderByType(
        service_type, app, hmi_destination);
  } else if ((*msg)[strings::msg_params].keyExists(strings::service_id)) {
    std::string service_id =
        (*msg)[strings::msg_params][strings::service_id].asString();
    application_manager_.GetAppServiceManager().GetProviderByID(
        service_id, app, hmi_destination);
  }

  if (hmi_destination) {
    LOG4CXX_DEBUG(logger_, "Sending Request to HMI Provider");
    SendHMIRequest(hmi_function_id, &(*msg)[strings::msg_params], use_events);
    return;
  }

  if (!app) {
    LOG4CXX_DEBUG(logger_, "Invalid App Provider pointer");
    return;
  }

  LOG4CXX_DEBUG(logger_, "Sending Request to Mobile Provider");
  SendMobileRequest(
      mobile_function_id, app, &(*msg)[strings::msg_params], use_events);
}

void RequestFromHMI::SendMobileRequest(
    const mobile_apis::FunctionID::eType& function_id,
    const ApplicationSharedPtr app,
    const smart_objects::SmartObject* msg_params,
    bool use_events) {
  smart_objects::SmartObjectSPtr result =
      std::make_shared<smart_objects::SmartObject>();

  const uint32_t mobile_correlation_id =
      application_manager_.GetNextMobileCorrelationID();

  smart_objects::SmartObject& request = *result;

  request[strings::params][strings::message_type] = MessageType::kRequest;
  request[strings::params][strings::function_id] = function_id;
  request[strings::params][strings::correlation_id] = mobile_correlation_id;
  request[strings::params][strings::protocol_version] = app->protocol_version();
  request[strings::params][strings::protocol_type] =
      CommandImpl::mobile_protocol_type_;

  request[strings::params][strings::connection_key] = app->app_id();

  if (msg_params) {
    request[strings::msg_params] = *msg_params;
  }

  if (use_events) {
    LOG4CXX_DEBUG(logger_,
                  "RequestFromHMI subscribe_on_event "
                      << function_id << " " << mobile_correlation_id);
    subscribe_on_event(function_id, mobile_correlation_id);
  }
  if (!rpc_service_.ManageMobileCommand(
          result, commands::Command::CommandSource::SOURCE_SDL)) {
    LOG4CXX_ERROR(logger_, "Unable to send request to mobile");
  }
}

void RequestFromHMI::SendHMIRequest(
    const hmi_apis::FunctionID::eType& function_id,
    const smart_objects::SmartObject* msg_params,
    bool use_events) {
  smart_objects::SmartObjectSPtr result =
      std::make_shared<smart_objects::SmartObject>();

  const uint32_t hmi_correlation_id =
      application_manager_.GetNextHMICorrelationID();

  smart_objects::SmartObject& request = *result;
  request[strings::params][strings::message_type] = MessageType::kRequest;
  request[strings::params][strings::function_id] = function_id;
  request[strings::params][strings::correlation_id] = hmi_correlation_id;
  request[strings::params][strings::protocol_type] =
      CommandImpl::hmi_protocol_type_;

  if (msg_params) {
    request[strings::msg_params] = *msg_params;
  }

  if (use_events) {
    LOG4CXX_DEBUG(logger_,
                  "RequestFromHMI subscribe_on_event " << function_id << " "
                                                       << hmi_correlation_id);
    subscribe_on_event(function_id, hmi_correlation_id);
  }
  if (ProcessHMIInterfacesAvailability(hmi_correlation_id, function_id)) {
    if (!rpc_service_.ManageHMICommand(
            result, commands::Command::CommandSource::SOURCE_SDL_TO_HMI)) {
      LOG4CXX_ERROR(logger_, "Unable to send request");
    }
  } else {
    LOG4CXX_DEBUG(logger_, "Interface is not available");
  }
}

bool RequestFromHMI::ProcessHMIInterfacesAvailability(
    const uint32_t hmi_correlation_id,
    const hmi_apis::FunctionID::eType& function_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  HmiInterfaces& hmi_interfaces = application_manager_.hmi_interfaces();
  HmiInterfaces::InterfaceID interface =
      hmi_interfaces.GetInterfaceFromFunction(function_id);
  DCHECK(interface != HmiInterfaces::HMI_INTERFACE_INVALID_ENUM);
  const HmiInterfaces::InterfaceState state =
      hmi_interfaces.GetInterfaceState(interface);
  if (HmiInterfaces::STATE_NOT_AVAILABLE == state) {
    return false;
  }
  return true;
}

}  // namespace commands
}  // namespace application_manager
