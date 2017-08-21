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
#include <string>
#include "utils/macro.h"
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"
#include "smart_objects/smart_object.h"
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

bool CheckResultCode(const ResponseInfo& first, const ResponseInfo& second) {
  if (first.is_ok && second.is_unsupported_resource &&
      second.interface_state == HmiInterfaces::STATE_NOT_AVAILABLE) {
    return true;
  }
  return false;
}

bool IsResultCodeUnsupported(const ResponseInfo& first,
                             const ResponseInfo& second) {
  return ((first.is_ok || first.is_invalid_enum) &&
          second.is_unsupported_resource) ||
         ((second.is_ok || second.is_invalid_enum) &&
          first.is_unsupported_resource) ||
         (first.is_unsupported_resource && second.is_unsupported_resource);
}

struct DisallowedParamsInserter {
  DisallowedParamsInserter(smart_objects::SmartObject& response,
                           mobile_apis::VehicleDataResultCode::eType code)
      : response_(response), code_(code) {}

  bool operator()(const std::string& param) {
    const VehicleData& vehicle_data =
        application_manager::MessageHelper::vehicle_data();
    VehicleData::const_iterator it = vehicle_data.find(param);
    if (vehicle_data.end() != it) {
      smart_objects::SmartObjectSPtr disallowed_param =
          new smart_objects::SmartObject(smart_objects::SmartType_Map);
      (*disallowed_param)[strings::data_type] = (*it).second;
      (*disallowed_param)[strings::result_code] = code_;
      response_[strings::msg_params][param.c_str()] = *disallowed_param;
      return true;
    }
    return false;
  }

 private:
  smart_objects::SmartObject& response_;
  mobile_apis::VehicleDataResultCode::eType code_;
};

CommandRequestImpl::CommandRequestImpl(const MessageSharedPtr& message,
                                       ApplicationManager& application_manager)
    : CommandImpl(message, application_manager)
    , EventObserver(application_manager.event_dispatcher())
    , current_state_(kAwaitingHMIResponse) {}

CommandRequestImpl::~CommandRequestImpl() {}

bool CommandRequestImpl::Init() {
  return true;
}

bool CommandRequestImpl::CheckPermissions() {
  return CheckAllowedParameters();
}

bool CommandRequestImpl::CleanUp() {
  return true;
}

void CommandRequestImpl::Run() {}

void CommandRequestImpl::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);

  unsubscribe_from_all_events();
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

  application_manager_.ManageMobileCommand(response, ORIGIN_SDL);
}

void CommandRequestImpl::on_event(const event_engine::Event& event) {}

void CommandRequestImpl::SendResponse(
    const bool success,
    const mobile_apis::Result::eType& result_code,
    const char* info,
    const smart_objects::SmartObject* response_params) {
  {
    sync_primitives::AutoLock auto_lock(state_lock_);
    if (kTimedOut == current_state_) {
      // don't send response if request timeout expired
      return;
    }

    current_state_ = kCompleted;
  }

  smart_objects::SmartObjectSPtr result = new smart_objects::SmartObject;
  if (!result) {
    LOG4CXX_ERROR(logger_, "Memory allocation failed.");
    return;
  }
  smart_objects::SmartObject& response = *result;

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

  // Add disallowed parameters and info from request back to response with
  // appropriate
  // reasons (VehicleData result codes)
  if (result_code != mobile_apis::Result::APPLICATION_NOT_REGISTERED) {
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

  application_manager_.ManageMobileCommand(result, ORIGIN_SDL);
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

uint32_t CommandRequestImpl::SendHMIRequest(
    const hmi_apis::FunctionID::eType& function_id,
    const smart_objects::SmartObject* msg_params,
    bool use_events) {
  smart_objects::SmartObjectSPtr result = new smart_objects::SmartObject;

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
                  "subscribe_on_event " << function_id << " "
                                        << hmi_correlation_id);
    subscribe_on_event(function_id, hmi_correlation_id);
  }
  if (ProcessHMIInterfacesAvailability(hmi_correlation_id, function_id)) {
    if (!application_manager_.ManageHMICommand(result)) {
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
    const NsSmart::SmartObject& msg_params) const {
  smart_objects::SmartObjectSPtr result = new smart_objects::SmartObject;
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

  if (!application_manager_.ManageHMICommand(result)) {
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
    default: {
      LOG4CXX_ERROR(logger_, "Unknown HMI result code " << hmi_code);
      break;
    }
  }

  return mobile_result;
}

bool CommandRequestImpl::CheckAllowedParameters() {
  LOG4CXX_AUTO_TRACE(logger_);

  // RegisterAppInterface should always be allowed
  if (mobile_apis::FunctionID::RegisterAppInterfaceID ==
      static_cast<mobile_apis::FunctionID::eType>(function_id())) {
    return true;
  }

  const ApplicationSharedPtr app =
      application_manager_.application(connection_key());
  if (!app) {
    LOG4CXX_ERROR(logger_,
                  "There is no registered application with "
                  "connection key '"
                      << connection_key() << "'");
    return false;
  }

  RPCParams params;

  const smart_objects::SmartObject& s_map = (*message_)[strings::msg_params];
  smart_objects::SmartMap::const_iterator iter = s_map.map_begin();
  smart_objects::SmartMap::const_iterator iter_end = s_map.map_end();

  for (; iter != iter_end; ++iter) {
    if (iter->second.asBool()) {
      LOG4CXX_DEBUG(logger_, "Request's param: " << iter->first);
      params.insert(iter->first);
    }
  }

  mobile_apis::Result::eType check_result =
      application_manager_.CheckPolicyPermissions(
          app,
          MessageHelper::StringifiedFunctionID(
              static_cast<mobile_api::FunctionID::eType>(function_id())),
          params,
          &parameters_permissions_);

  // Check, if RPC is allowed by policy
  if (mobile_apis::Result::SUCCESS != check_result) {
    smart_objects::SmartObjectSPtr response =
        MessageHelper::CreateBlockedByPoliciesResponse(
            static_cast<mobile_api::FunctionID::eType>(function_id()),
            check_result,
            correlation_id(),
            app->app_id());

    application_manager_.SendMessageToMobile(response);
    return false;
  }

  // If no parameters specified in policy table, no restriction will be
  // applied for parameters
  if (parameters_permissions_.allowed_params.empty() &&
      parameters_permissions_.disallowed_params.empty() &&
      parameters_permissions_.undefined_params.empty()) {
    return true;
  }

  RemoveDisallowedParameters();

  return true;
}

void CommandRequestImpl::RemoveDisallowedParameters() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject& params = (*message_)[strings::msg_params];

  // Remove from request all disallowed parameters
  RPCParams::const_iterator it_disallowed =
      parameters_permissions_.disallowed_params.begin();
  RPCParams::const_iterator it_disallowed_end =
      parameters_permissions_.disallowed_params.end();
  for (; it_disallowed != it_disallowed_end; ++it_disallowed) {
    if (params.keyExists(*it_disallowed)) {
      const std::string key = *it_disallowed;
      params.erase(key);
      removed_parameters_permissions_.disallowed_params.insert(key);
      LOG4CXX_INFO(logger_,
                   "Following parameter is disallowed by user: " << key);
    }
  }

  // Remove from request all undefined yet parameters
  RPCParams::const_iterator it_undefined =
      parameters_permissions_.undefined_params.begin();
  RPCParams::const_iterator it_undefined_end =
      parameters_permissions_.undefined_params.end();
  for (; it_undefined != it_undefined_end; ++it_undefined) {
    if (params.keyExists(*it_undefined)) {
      const std::string key = *it_undefined;
      params.erase(key);
      removed_parameters_permissions_.undefined_params.insert(key);
      LOG4CXX_INFO(logger_,
                   "Following parameter is disallowed by policy: " << key);
    }
  }

  // Remove from request all parameters missed in allowed
  const VehicleData& vehicle_data =
      application_manager::MessageHelper::vehicle_data();

  VehicleData::const_iterator it_vehicle_data = vehicle_data.begin();
  VehicleData::const_iterator it_vehicle_data_end = vehicle_data.end();
  for (; it_vehicle_data != it_vehicle_data_end; ++it_vehicle_data) {
    const std::string key = it_vehicle_data->first;
    if (params.keyExists(key) &&
        parameters_permissions_.allowed_params.end() ==
            std::find(parameters_permissions_.allowed_params.begin(),
                      parameters_permissions_.allowed_params.end(),
                      key)) {
      params.erase(key);
      removed_parameters_permissions_.undefined_params.insert(key);
      LOG4CXX_INFO(logger_,
                   "Following parameter is not found among allowed parameters '"
                       << key << "' and will be treated as disallowed.");
    }
  }
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

bool CommandRequestImpl::PrepareResultForMobileResponse(
    hmi_apis::Common_Result::eType result_code,
    HmiInterfaces::InterfaceID interface) const {
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

bool CommandRequestImpl::PrepareResultForMobileResponse(
    ResponseInfo& out_first, ResponseInfo& out_second) const {
  using namespace helpers;

  out_first.is_ok = Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
      out_first.result_code,
      hmi_apis::Common_Result::SUCCESS,
      hmi_apis::Common_Result::WARNINGS,
      hmi_apis::Common_Result::WRONG_LANGUAGE,
      hmi_apis::Common_Result::RETRY,
      hmi_apis::Common_Result::SAVED);

  out_second.is_ok = Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
      out_second.result_code,
      hmi_apis::Common_Result::SUCCESS,
      hmi_apis::Common_Result::WARNINGS,
      hmi_apis::Common_Result::WRONG_LANGUAGE,
      hmi_apis::Common_Result::RETRY,
      hmi_apis::Common_Result::SAVED);

  out_first.is_invalid_enum =
      hmi_apis::Common_Result::INVALID_ENUM == out_first.result_code;

  out_second.is_invalid_enum =
      hmi_apis::Common_Result::INVALID_ENUM == out_second.result_code;

  out_first.is_unsupported_resource =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == out_first.result_code;

  out_second.is_unsupported_resource =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == out_second.result_code;

  out_first.interface_state =
      application_manager_.hmi_interfaces().GetInterfaceState(
          out_first.interface);
  out_second.interface_state =
      application_manager_.hmi_interfaces().GetInterfaceState(
          out_second.interface);

  bool result = (out_first.is_ok && out_second.is_ok) ||
                (out_second.is_invalid_enum && out_first.is_ok) ||
                (out_first.is_invalid_enum && out_second.is_ok);
  result = result || CheckResultCode(out_first, out_second);
  result = result || CheckResultCode(out_second, out_first);
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
  mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;
  if (IsResultCodeUnsupported(first, second)) {
    result_code = mobile_apis::Result::UNSUPPORTED_RESOURCE;
  } else {
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
  }
  return result_code;
}

const CommandParametersPermissions& CommandRequestImpl::parameters_permissions()
    const {
  return parameters_permissions_;
}

}  // namespace commands

}  // namespace application_manager
