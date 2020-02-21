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

#include <algorithm>
#include <numeric>
#include <string>
#include "utils/macro.h"

#include "application_manager/commands/command_request_impl.h"

#include "application_manager/app_service_manager.h"
#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"
#include "application_manager/rpc_service.h"
#include "smart_objects/smart_object.h"

#include "smart_objects/enum_schema_item.h"

namespace application_manager {

namespace commands {

std::string MergeInfos(const ResponseInfo& first_info,
                       const std::string& first_str,
                       const ResponseInfo& second_info,
                       const std::string& second_str) {
  if ((first_info.interface_state == HmiInterfaces::STATE_NOT_AVAILABLE) &&
      (second_info.interface_state != HmiInterfaces::STATE_NOT_AVAILABLE) &&
      !second_str.empty()) {
    return second_str;
  }

  if ((second_info.interface_state == HmiInterfaces::STATE_NOT_AVAILABLE) &&
      (first_info.interface_state != HmiInterfaces::STATE_NOT_AVAILABLE) &&
      !first_str.empty()) {
    return first_str;
  }

  return MergeInfos(first_str, second_str);
}

std::string MergeInfos(const std::string& first, const std::string& second) {
  return first + ((!first.empty() && !second.empty()) ? ", " : "") + second;
}

std::string MergeInfos(const std::string& first,
                       const std::string& second,
                       const std::string& third) {
  std::string result = MergeInfos(first, second);
  return MergeInfos(result, third);
}

const std::string CreateInfoForUnsupportedResult(
    HmiInterfaces::InterfaceID interface) {
  switch (interface) {
    case (HmiInterfaces::InterfaceID::HMI_INTERFACE_VR): {
      return "VR is not supported by system";
    }
    case (HmiInterfaces::InterfaceID::HMI_INTERFACE_TTS): {
      return "TTS is not supported by system";
    }
    case (HmiInterfaces::InterfaceID::HMI_INTERFACE_UI): {
      return "UI is not supported by system";
    }
    case (HmiInterfaces::InterfaceID::HMI_INTERFACE_Navigation): {
      return "Navi is not supported by system";
    }
    case (HmiInterfaces::InterfaceID::HMI_INTERFACE_VehicleInfo): {
      return "VehicleInfo is not supported by system";
    }
    case (HmiInterfaces::InterfaceID::HMI_INTERFACE_RC): {
      return "Remote control is not supported by system";
    }
    default:
#ifdef ENABLE_LOG
      CREATE_LOGGERPTR_LOCAL(logger, "Commands");
      LOG4CXX_WARN(logger,
                   "Could not create info because"
                   " interface isn't valid. Interface is:"
                       << static_cast<int32_t>(interface));
#endif  // ENABLE_LOG
      return "";
  }
}

bool CommandRequestImpl::CheckResult(const ResponseInfo& first,
                                     const ResponseInfo& second) const {
  if (first.is_ok && second.is_unsupported_resource) {
    return true;
  }
  if (first.is_ok && second.is_not_used) {
    return true;
  }
  if (first.is_ok && second.is_ok) {
    return true;
  }
  return false;
}

bool IsResultCodeWarning(const ResponseInfo& first,
                         const ResponseInfo& second) {
  const bool first_is_ok_second_is_warn =
      (first.is_ok || first.is_not_used) &&
      hmi_apis::Common_Result::WARNINGS == second.result_code;

  const bool both_warnings =
      hmi_apis::Common_Result::WARNINGS == first.result_code &&
      hmi_apis::Common_Result::WARNINGS == second.result_code;

  return first_is_ok_second_is_warn || both_warnings;
}

struct DisallowedParamsInserter {
  DisallowedParamsInserter(smart_objects::SmartObject& response,
                           mobile_apis::VehicleDataResultCode::eType code)
      : response_(response), code_(code) {}

  bool operator()(const std::string& param) {
    smart_objects::SmartObjectSPtr disallowed_param =
        std::make_shared<smart_objects::SmartObject>(
            smart_objects::SmartType_Map);

    auto rpc_spec_vehicle_data = MessageHelper::vehicle_data();
    auto vehicle_data = rpc_spec_vehicle_data.find(param);
    auto vehicle_data_type =
        vehicle_data == rpc_spec_vehicle_data.end()
            ? mobile_apis::VehicleDataType::VEHICLEDATA_OEM_CUSTOM_DATA
            : vehicle_data->second;

    (*disallowed_param)[strings::data_type] = vehicle_data_type;
    (*disallowed_param)[strings::result_code] = code_;
    response_[strings::msg_params][param.c_str()] = *disallowed_param;
    return true;
  }

 private:
  smart_objects::SmartObject& response_;
  mobile_apis::VehicleDataResultCode::eType code_;
};

ResponseInfo::ResponseInfo()
    : result_code(hmi_apis::Common_Result::INVALID_ENUM)
    , interface(HmiInterfaces::HMI_INTERFACE_INVALID_ENUM)
    , interface_state(HmiInterfaces::STATE_NOT_RESPONSE)
    , is_ok(false)
    , is_unsupported_resource(false)
    , is_not_used(false) {}

ResponseInfo::ResponseInfo(const hmi_apis::Common_Result::eType result,
                           const HmiInterfaces::InterfaceID hmi_interface,
                           ApplicationManager& application_manager)
    : result_code(result)
    , interface(hmi_interface)
    , interface_state(HmiInterfaces::STATE_NOT_RESPONSE)
    , is_ok(false)
    , is_unsupported_resource(false)
    , is_not_used(false) {
  using namespace helpers;

  interface_state =
      application_manager.hmi_interfaces().GetInterfaceState(hmi_interface);

  is_ok = CommandRequestImpl::IsHMIResultSuccess(result_code);

  is_not_used = hmi_apis::Common_Result::INVALID_ENUM == result_code;

  is_unsupported_resource =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == result_code;
}

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
    , current_state_(kAwaitingHMIResponse)
    , hash_update_mode_(kSkipHashUpdate)
    , is_success_result_(false) {}

CommandRequestImpl::~CommandRequestImpl() {
  UpdateHash();
}

bool CommandRequestImpl::Init() {
  return true;
}

bool CommandRequestImpl::CheckPermissions() {
  return CheckAllowedParameters(Command::CommandSource::SOURCE_MOBILE);
}

bool CommandRequestImpl::CleanUp() {
  return true;
}

void CommandRequestImpl::Run() {}

void CommandRequestImpl::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);

  unsubscribe_from_all_hmi_events();
  unsubscribe_from_all_mobile_events();
  {
    // FIXME (dchmerev@luxoft.com): atomic_xchg fits better
    sync_primitives::AutoLock auto_lock(state_lock_);
    if (kCompleted == current_state_) {
      LOG4CXX_DEBUG(logger_, "current_state_ = kCompleted");
      // don't send timeout if request completed
      return;
    }

    current_state_ = kTimedOut;
  }

  smart_objects::SmartObjectSPtr response =
      MessageHelper::CreateNegativeResponse(connection_key(),
                                            function_id(),
                                            correlation_id(),
                                            mobile_api::Result::GENERIC_ERROR);
  AddTimeOutComponentInfoToMessage(*response);
  rpc_service_.ManageMobileCommand(response, SOURCE_SDL);
}

void CommandRequestImpl::on_event(const event_engine::Event& event) {}

void CommandRequestImpl::on_event(const event_engine::MobileEvent& event) {}

void CommandRequestImpl::SendResponse(
    const bool success,
    const mobile_apis::Result::eType& result_code,
    const char* info,
    const smart_objects::SmartObject* response_params,
    const std::vector<uint8_t> binary_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock auto_lock(state_lock_);
    if (kTimedOut == current_state_) {
      // don't send response if request timeout expired
      return;
    }

    current_state_ = kCompleted;
  }

  smart_objects::SmartObjectSPtr result =
      std::make_shared<smart_objects::SmartObject>();

  smart_objects::SmartObject& response = *result;

  response[strings::params][strings::message_type] = MessageType::kResponse;
  response[strings::params][strings::correlation_id] = correlation_id();
  response[strings::params][strings::protocol_type] =
      CommandImpl::mobile_protocol_type_;
  response[strings::params][strings::protocol_version] =
      CommandImpl::protocol_version_;
  response[strings::params][strings::connection_key] = connection_key();
  response[strings::params][strings::function_id] = function_id();
  if (!binary_data.empty()) {
    response[strings::params][strings::binary_data] = binary_data;
  }
  if (response_params) {
    response[strings::msg_params] = *response_params;
  }

  if (info) {
    response[strings::msg_params][strings::info] = std::string(info);
  }

  // Add disallowed parameters and info from request back to response with
  // appropriate reasons (VehicleData result codes)
  if (result_code != mobile_apis::Result::APPLICATION_NOT_REGISTERED &&
      result_code != mobile_apis::Result::INVALID_DATA) {
    const mobile_apis::FunctionID::eType& id =
        static_cast<mobile_apis::FunctionID::eType>(function_id());
    if ((id == mobile_apis::FunctionID::SubscribeVehicleDataID) ||
        (id == mobile_apis::FunctionID::UnsubscribeVehicleDataID)) {
      AddDisallowedParameters(response);
      AddDisallowedParametersToInfo(response);
    } else if (id == mobile_apis::FunctionID::GetVehicleDataID) {
      AddDisallowedParametersToInfo(response);
    }
  }

  response[strings::msg_params][strings::success] = success;
  response[strings::msg_params][strings::result_code] = result_code;

  is_success_result_ = success;

  rpc_service_.ManageMobileCommand(result, SOURCE_SDL);
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

smart_objects::SmartObject CreateUnsupportedResourceResponse(
    const hmi_apis::FunctionID::eType function_id,
    const uint32_t hmi_correlation_id,
    HmiInterfaces::InterfaceID interface) {
  smart_objects::SmartObject response(smart_objects::SmartType_Map);
  smart_objects::SmartObject& params = response[strings::params];
  params[strings::message_type] = MessageType::kResponse;
  params[strings::correlation_id] = hmi_correlation_id;
  params[strings::protocol_type] = CommandImpl::hmi_protocol_type_;
  params[strings::protocol_version] = CommandImpl::protocol_version_;
  params[strings::function_id] = function_id;
  params[hmi_response::code] = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  smart_objects::SmartObject& msg_params = response[strings::msg_params];
  msg_params[strings::info] = CreateInfoForUnsupportedResult(interface);
  return response;
}

bool CommandRequestImpl::ProcessHMIInterfacesAvailability(
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
    event_engine::Event event(function_id);
    event.set_smart_object(CreateUnsupportedResourceResponse(
        function_id, hmi_correlation_id, interface));
    event.raise(application_manager_.event_dispatcher());
    return false;
  }
  return true;
}

void CommandRequestImpl::UpdateHash() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (hash_update_mode_ == kSkipHashUpdate) {
    LOG4CXX_DEBUG(logger_, "Hash update is disabled for " << function_id());
    return;
  }

  if (HmiInterfaces::InterfaceState::STATE_NOT_RESPONSE ==
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::InterfaceID::HMI_INTERFACE_UI)) {
    LOG4CXX_ERROR(logger_,
                  "UI interface has not responded. Hash won't be updated.");
    return;
  }

  if (!is_success_result_) {
    LOG4CXX_WARN(logger_, "Command is not succeeded. Hash won't be updated.");
    return;
  }

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());
  if (!application) {
    LOG4CXX_ERROR(logger_,
                  "Application with connection key "
                      << connection_key()
                      << " not found. Not able to update hash.");
    return;
  }

  LOG4CXX_DEBUG(
      logger_,
      "Updating hash for application with connection key "
          << connection_key() << " while processing function id "
          << MessageHelper::StringifiedFunctionID(
                 static_cast<mobile_api::FunctionID::eType>(function_id())));

  application->UpdateHash();
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

uint32_t CommandRequestImpl::SendHMIRequest(
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
  request[strings::params][strings::protocol_version] =
      CommandImpl::protocol_version_;
  request[strings::params][strings::protocol_type] =
      CommandImpl::hmi_protocol_type_;

  if (msg_params) {
    request[strings::msg_params] = *msg_params;
  }

  if (use_events) {
    LOG4CXX_DEBUG(logger_,
                  "SendHMIRequest subscribe_on_event " << function_id << " "
                                                       << hmi_correlation_id);
    subscribe_on_event(function_id, hmi_correlation_id);
  }
  if (ProcessHMIInterfacesAvailability(hmi_correlation_id, function_id)) {
    if (!rpc_service_.ManageHMICommand(result, SOURCE_SDL_TO_HMI)) {
      LOG4CXX_ERROR(logger_, "Unable to send request");
      SendResponse(false, mobile_apis::Result::OUT_OF_MEMORY);
    }
  } else {
    LOG4CXX_DEBUG(logger_, "Interface is not available");
  }
  return hmi_correlation_id;
}

void CommandRequestImpl::CreateHMINotification(
    const hmi_apis::FunctionID::eType& function_id,
    const ns_smart::SmartObject& msg_params) const {
  smart_objects::SmartObjectSPtr result =
      std::make_shared<smart_objects::SmartObject>();
  if (!result) {
    LOG4CXX_ERROR(logger_, "Memory allocation failed.");
    return;
  }
  smart_objects::SmartObject& notify = *result;

  notify[strings::params][strings::message_type] =
      static_cast<int32_t>(application_manager::MessageType::kNotification);
  notify[strings::params][strings::protocol_version] =
      CommandImpl::protocol_version_;
  notify[strings::params][strings::protocol_type] =
      CommandImpl::hmi_protocol_type_;
  notify[strings::params][strings::function_id] = function_id;
  notify[strings::msg_params] = msg_params;

  if (!rpc_service_.ManageHMICommand(result, SOURCE_SDL_TO_HMI)) {
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
    case hmi_apis::Common_Result::SAVED: {
      mobile_result = mobile_apis::Result::SAVED;
      break;
    }
    case hmi_apis::Common_Result::READ_ONLY: {
      mobile_result = mobile_apis::Result::READ_ONLY;
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown HMI result code " << hmi_code);
      break;
    }
  }

  return mobile_result;
}

bool CommandRequestImpl::CheckAllowedParameters(
    const Command::CommandSource source) {
  LOG4CXX_AUTO_TRACE(logger_);

  // RegisterAppInterface should always be allowed
  if (mobile_apis::FunctionID::RegisterAppInterfaceID ==
      static_cast<mobile_apis::FunctionID::eType>(function_id())) {
    return true;
  }

  return CommandImpl::CheckAllowedParameters(source);
}

bool CommandRequestImpl::CheckHMICapabilities(
    const mobile_apis::ButtonName::eType button) const {
  LOG4CXX_AUTO_TRACE(logger_);

  using namespace smart_objects;
  using namespace mobile_apis;

  if (!hmi_capabilities_.is_ui_cooperating()) {
    LOG4CXX_ERROR(logger_, "UI is not supported by HMI");
    return false;
  }

  auto button_capabilities = hmi_capabilities_.button_capabilities();
  if (!button_capabilities) {
    LOG4CXX_ERROR(logger_, "Invalid button capabilities object");
    return false;
  }

  for (size_t i = 0; i < button_capabilities->length(); ++i) {
    const SmartObject& capabilities = (*button_capabilities)[i];
    const ButtonName::eType current_button = static_cast<ButtonName::eType>(
        capabilities.getElement(hmi_response::button_name).asInt());
    if (current_button == button) {
      LOG4CXX_DEBUG(logger_,
                    "Button capabilities for " << button << " was found");
      return true;
    }
  }

  LOG4CXX_DEBUG(logger_,
                "Button capabilities for " << button << " was not found");
  return false;
}

void CommandRequestImpl::AddDissalowedParameterToInfoString(
    std::string& info, const std::string& param) const {
  // prepare disallowed params enumeration for response info string
  if (info.empty()) {
    info = "\'" + param + "\'";
  } else {
    info = info + "," + " " + "\'" + param + "\'";
  }
}

void CommandRequestImpl::AddDisallowedParametersToInfo(
    smart_objects::SmartObject& response) const {
  std::string info;

  RPCParams::const_iterator it =
      removed_parameters_permissions_.disallowed_params.begin();
  for (; it != removed_parameters_permissions_.disallowed_params.end(); ++it) {
    AddDissalowedParameterToInfoString(info, (*it));
  }

  it = removed_parameters_permissions_.undefined_params.begin();
  for (; it != removed_parameters_permissions_.undefined_params.end(); ++it) {
    AddDissalowedParameterToInfoString(info, (*it));
  }

  if (!info.empty()) {
    info += " disallowed by policies.";

    if (!response[strings::msg_params][strings::info].asString().empty()) {
      // If we already have info add info about disallowed params to it
      response[strings::msg_params][strings::info] =
          response[strings::msg_params][strings::info].asString() + " " + info;
    } else {
      response[strings::msg_params][strings::info] = info;
    }
  }
}

void CommandRequestImpl::AddDisallowedParameters(
    smart_objects::SmartObject& response) {
  DisallowedParamsInserter disallowed_inserter(
      response, mobile_apis::VehicleDataResultCode::VDRC_USER_DISALLOWED);
  std::for_each(removed_parameters_permissions_.disallowed_params.begin(),
                removed_parameters_permissions_.disallowed_params.end(),
                disallowed_inserter);

  DisallowedParamsInserter undefined_inserter(
      response, mobile_apis::VehicleDataResultCode::VDRC_DISALLOWED);
  std::for_each(removed_parameters_permissions_.undefined_params.begin(),
                removed_parameters_permissions_.undefined_params.end(),
                undefined_inserter);
}

bool CommandRequestImpl::HasDisallowedParams() const {
  return ((!removed_parameters_permissions_.disallowed_params.empty()) ||
          (!removed_parameters_permissions_.undefined_params.empty()));
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
      mobile_apis::Result::SAVED,
      mobile_apis::Result::TRUNCATED_DATA);
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
      hmi_apis::Common_Result::SAVED,
      hmi_apis::Common_Result::TRUNCATED_DATA);
}

bool CommandRequestImpl::PrepareResultForMobileResponse(
    hmi_apis::Common_Result::eType result_code,
    HmiInterfaces::InterfaceID interface) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (IsHMIResultSuccess(result_code)) {
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

bool CommandRequestImpl::PrepareResultForMobileResponse(
    ResponseInfo& out_first, ResponseInfo& out_second) const {
  LOG4CXX_AUTO_TRACE(logger_);
  bool result =
      CheckResult(out_first, out_second) || CheckResult(out_second, out_first);
  return result;
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

mobile_apis::Result::eType CommandRequestImpl::PrepareResultCodeForResponse(
    const ResponseInfo& first, const ResponseInfo& second) {
  LOG4CXX_AUTO_TRACE(logger_);
  mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;
  if (IsResultCodeUnsupported(first, second) ||
      IsResultCodeUnsupported(second, first)) {
    return mobile_apis::Result::UNSUPPORTED_RESOURCE;
  }
  if (IsResultCodeWarning(first, second) ||
      IsResultCodeWarning(second, first)) {
    return mobile_apis::Result::WARNINGS;
  }
  // If response contains erroneous result code SDL need return erroneus
  // result code.
  hmi_apis::Common_Result::eType first_result =
      hmi_apis::Common_Result::INVALID_ENUM;
  hmi_apis::Common_Result::eType second_result =
      hmi_apis::Common_Result::INVALID_ENUM;
  if (!first.is_unsupported_resource) {
    first_result = first.result_code;
  }
  if (!second.is_unsupported_resource) {
    second_result = second.result_code;
  }
  result_code =
      MessageHelper::HMIToMobileResult(std::max(first_result, second_result));
  return result_code;
}

const CommandParametersPermissions& CommandRequestImpl::parameters_permissions()
    const {
  return parameters_permissions_;
}

void CommandRequestImpl::StartAwaitForInterface(
    const HmiInterfaces::InterfaceID interface_id) {
  sync_primitives::AutoLock lock(awaiting_response_interfaces_lock_);
  awaiting_response_interfaces_.insert(interface_id);
}

bool CommandRequestImpl::IsInterfaceAwaited(
    const HmiInterfaces::InterfaceID& interface_id) const {
  sync_primitives::AutoLock lock(awaiting_response_interfaces_lock_);
  std::set<HmiInterfaces::InterfaceID>::const_iterator it =
      awaiting_response_interfaces_.find(interface_id);
  return (it != awaiting_response_interfaces_.end());
}

void CommandRequestImpl::EndAwaitForInterface(
    const HmiInterfaces::InterfaceID& interface_id) {
  sync_primitives::AutoLock lock(awaiting_response_interfaces_lock_);
  std::set<HmiInterfaces::InterfaceID>::const_iterator it =
      awaiting_response_interfaces_.find(interface_id);
  if (it != awaiting_response_interfaces_.end()) {
    awaiting_response_interfaces_.erase(it);
  } else {
    LOG4CXX_WARN(logger_,
                 "EndAwaitForInterface called on interface \
                    which was not put into await state: "
                     << interface_id);
  }
}

bool CommandRequestImpl::IsResultCodeUnsupported(
    const ResponseInfo& first, const ResponseInfo& second) const {
  const bool first_ok_second_unsupported =
      (first.is_ok || first.is_not_used) && second.is_unsupported_resource;
  const bool both_unsupported =
      first.is_unsupported_resource && second.is_unsupported_resource;
  return first_ok_second_unsupported || both_unsupported;
}

std::string GetComponentNameFromInterface(
    const HmiInterfaces::InterfaceID& interface) {
  switch (interface) {
    case HmiInterfaces::HMI_INTERFACE_Buttons:
      return hmi_interface::buttons;
    case HmiInterfaces::HMI_INTERFACE_BasicCommunication:
      return hmi_interface::basic_communication;
    case HmiInterfaces::HMI_INTERFACE_VR:
      return hmi_interface::vr;
    case HmiInterfaces::HMI_INTERFACE_TTS:
      return hmi_interface::tts;
    case HmiInterfaces::HMI_INTERFACE_UI:
      return hmi_interface::ui;
    case HmiInterfaces::HMI_INTERFACE_Navigation:
      return hmi_interface::navigation;
    case HmiInterfaces::HMI_INTERFACE_VehicleInfo:
      return hmi_interface::vehicle_info;
    case HmiInterfaces::HMI_INTERFACE_SDL:
      return hmi_interface::sdl;
    case HmiInterfaces::HMI_INTERFACE_RC:
      return hmi_interface::rc;
    case HmiInterfaces::HMI_INTERFACE_AppService:
      return hmi_interface::app_service;
    default:
      return "Unknown type";
  }
}

const std::string InfoInterfaceSeparator(
    const std::string& sum, const HmiInterfaces::InterfaceID container_value) {
  return sum.empty()
             ? GetComponentNameFromInterface(container_value)
             : sum + ", " + GetComponentNameFromInterface(container_value);
}

void CommandRequestImpl::AddTimeOutComponentInfoToMessage(
    smart_objects::SmartObject& response) const {
  using ns_smart_device_link::ns_smart_objects::SmartObject;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(awaiting_response_interfaces_lock_);
  if (awaiting_response_interfaces_.empty()) {
    LOG4CXX_ERROR(logger_, "No interfaces awaiting, info param is empty");
    return;
  }

  const std::string not_responding_interfaces_string =
      std::accumulate(awaiting_response_interfaces_.begin(),
                      awaiting_response_interfaces_.end(),
                      std::string(""),
                      InfoInterfaceSeparator);
  LOG4CXX_DEBUG(
      logger_,
      "Not responding interfaces string: " << not_responding_interfaces_string);
  if (!not_responding_interfaces_string.empty()) {
    const std::string component_info =
        not_responding_interfaces_string + " component does not respond";
    response[strings::msg_params][strings::info] = component_info;
  }
}

}  // namespace commands

}  // namespace application_manager
