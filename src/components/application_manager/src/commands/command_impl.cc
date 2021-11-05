/*
 Copyright (c) 2014, Ford Motor Company
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

#include "application_manager/commands/command_impl.h"

#include "application_manager/application_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"

namespace application_manager {

namespace {

/**
 * @brief Functor for build info string
 */
struct InfoAppender {
  explicit InfoAppender(std::string& info) : info_(info) {}

  void operator()(const RPCParams::value_type& parameter) {
    if (info_.empty()) {
      info_ = "\'" + parameter + "\'";
      return;
    }

    info_ = info_ + ", \'" + parameter + "\'";
  }

 private:
  std::string& info_;
};

}  // namespace

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

bool IsMobileResultSuccess(const mobile_apis::Result::eType result_code) {
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

bool IsHMIResultSuccess(const hmi_apis::Common_Result::eType result_code) {
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

const int32_t CommandImpl::hmi_protocol_type_ = 1;
const int32_t CommandImpl::mobile_protocol_type_ = 0;
const int32_t CommandImpl::protocol_version_ = 3;

CommandImpl::CommandImpl(const MessageSharedPtr& message,
                         ApplicationManager& application_manager,
                         rpc_service::RPCService& rpc_service,
                         HMICapabilities& hmi_capabilities,
                         policy::PolicyHandlerInterface& policy_handler)
    : message_(message)
    , default_timeout_(
          application_manager.get_settings().default_timeout() +
          application_manager.get_settings().default_timeout_compensation())
    , allowed_to_terminate_(true)
    , application_manager_(application_manager)
    , rpc_service_(rpc_service)
    , hmi_capabilities_(hmi_capabilities)
    , policy_handler_(policy_handler) {}

CommandImpl::~CommandImpl() {
  CleanUp();
}

bool CommandImpl::CheckPermissions() {
  return true;
}

bool CommandImpl::Init() {
  return true;
}

bool CommandImpl::CleanUp() {
  return true;
}

void CommandImpl::Run() {}

void CommandImpl::OnUpdateTimeOut() {}

void CommandImpl::Reject() {}

uint32_t CommandImpl::default_timeout() const {
  return default_timeout_;
}

uint32_t CommandImpl::correlation_id() const {
  return (*message_)[strings::params][strings::correlation_id].asUInt();
}

int32_t CommandImpl::function_id() const {
  return (*message_)[strings::params][strings::function_id].asInt();
}

WindowID CommandImpl::window_id() const {
  if ((*message_).keyExists(strings::msg_params)) {
    if ((*message_)[strings::msg_params].keyExists(strings::window_id)) {
      return (*message_)[strings::msg_params][strings::window_id].asInt();
    }
  }
  return mobile_apis::PredefinedWindows::DEFAULT_WINDOW;
}

uint32_t CommandImpl::connection_key() const {
  return (*message_)[strings::params][strings::connection_key].asUInt();
}

void CommandImpl::HandleTimeOut() {}

void CommandImpl::set_warning_info(const std::string info) {
  warning_info_ = info;
}

std::string CommandImpl::warning_info() const {
  return warning_info_;
}

bool CommandImpl::AllowedToTerminate() {
  return allowed_to_terminate_;
}

void CommandImpl::SetAllowedToTerminate(const bool allowed) {
  allowed_to_terminate_ = allowed;
}

bool CommandImpl::CheckAllowedParameters(const Command::CommandSource source) {
  SDL_LOG_AUTO_TRACE();

  const ApplicationSharedPtr app =
      application_manager_.application(connection_key());
  if (!app) {
    SDL_LOG_ERROR(
        "There is no registered application with "
        "connection key '"
        << connection_key() << "'");

    rpc_service_.SendMessageToMobile(MessageHelper::CreateNegativeResponse(
        connection_key(),
        function_id(),
        correlation_id(),
        mobile_apis::Result::APPLICATION_NOT_REGISTERED));
    return false;
  }

  RPCParams params;

  const smart_objects::SmartObject& s_map = (*message_)[strings::msg_params];
  smart_objects::SmartMap::const_iterator iter = s_map.map_begin();
  smart_objects::SmartMap::const_iterator iter_end = s_map.map_end();

  for (; iter != iter_end; ++iter) {
    SDL_LOG_DEBUG("Request's param: " << iter->first);
    params.insert(iter->first);
  }

  mobile_apis::Result::eType check_result =
      mobile_apis::Result::eType::INVALID_ID;
  const auto current_window_id = window_id();
  if (app->WindowIdExists(current_window_id)) {
    check_result = application_manager_.CheckPolicyPermissions(
        app,
        current_window_id,
        MessageHelper::StringifiedFunctionID(
            static_cast<mobile_api::FunctionID::eType>(function_id())),
        params,
        &parameters_permissions_);
  }

  // Check, if RPC is allowed by policy
  if (mobile_apis::Result::SUCCESS != check_result) {
    mobile_apis::messageType::eType message_type =
        static_cast<mobile_apis::messageType::eType>(
            (*message_)[strings::params][strings::message_type].asInt());
    if (message_type == mobile_apis::messageType::request &&
        source == Command::CommandSource::SOURCE_MOBILE) {
      smart_objects::SmartObjectSPtr response =
          MessageHelper::CreateBlockedByPoliciesResponse(
              static_cast<mobile_api::FunctionID::eType>(function_id()),
              check_result,
              correlation_id(),
              app->app_id());

      if (!params.empty()) {
        if (parameters_permissions_.AreDisallowedParamsIncluded(params)) {
          const std::string info = "RPC is disallowed by the user";
          SDL_LOG_DEBUG(info);
          (*response)[strings::msg_params][strings::info] = info;
          AddDisallowedParameters(*response);
        } else if (parameters_permissions_.AreUndefinedParamsIncluded(params)) {
          const std::string info =
              "Requested parameters are disallowed by Policies";

          SDL_LOG_DEBUG(info);
          (*response)[strings::msg_params][strings::info] = info;
          AddDisallowedParameters(*response);
        } else {
          FormatResponse(*response);
        }
      }

      rpc_service_.SendMessageToMobile(response);
    }

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

void CommandImpl::AddDisallowedParameters(
    smart_objects::SmartObject& response) {
  const mobile_apis::FunctionID::eType id =
      static_cast<mobile_apis::FunctionID::eType>(function_id());

  if (!helpers::
          Compare<mobile_apis::FunctionID::eType, helpers::EQ, helpers::ONE>(
              id,
              mobile_apis::FunctionID::SubscribeVehicleDataID,
              mobile_apis::FunctionID::UnsubscribeVehicleDataID)) {
    SDL_LOG_INFO("The function id: " << id << " is not supported.");
    return;
  }

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

void CommandImpl::AddDisallowedParameterToInfoString(
    std::string& info, const std::string& param) const {
  // prepare disallowed params enumeration for response info string
  if (info.empty()) {
    info = "\'" + param + "\'";
  } else {
    info = info + "," + " " + "\'" + param + "\'";
  }
}

void CommandImpl::AddDisallowedParametersToInfo(
    smart_objects::SmartObject& response) const {
  SDL_LOG_AUTO_TRACE();
  const mobile_apis::FunctionID::eType id =
      static_cast<mobile_apis::FunctionID::eType>(function_id());

  if (!helpers::
          Compare<mobile_apis::FunctionID::eType, helpers::EQ, helpers::ONE>(
              id,
              mobile_apis::FunctionID::SubscribeVehicleDataID,
              mobile_apis::FunctionID::UnsubscribeVehicleDataID,
              mobile_apis::FunctionID::GetVehicleDataID,
              mobile_apis::FunctionID::SendLocationID)) {
    SDL_LOG_INFO("The function id: " << id << " is not supported.");
    return;
  }

  std::string disallowed_by_user_info;
  InfoAppender user_info_appender(disallowed_by_user_info);

  std::for_each(removed_parameters_permissions_.disallowed_params.begin(),
                removed_parameters_permissions_.disallowed_params.end(),
                user_info_appender);

  const size_t min_number_of_disallowed_params = 1;
  if (!disallowed_by_user_info.empty()) {
    disallowed_by_user_info +=
        min_number_of_disallowed_params <
                removed_parameters_permissions_.disallowed_params.size()
            ? " are"
            : " is";
    disallowed_by_user_info += " disallowed by user";
  }

  std::string disallowed_by_policy_info;
  InfoAppender policy_info_appender(disallowed_by_policy_info);

  std::for_each(removed_parameters_permissions_.undefined_params.begin(),
                removed_parameters_permissions_.undefined_params.end(),
                policy_info_appender);

  const size_t min_number_of_undefined_params = 1;
  if (!disallowed_by_policy_info.empty()) {
    disallowed_by_policy_info +=
        min_number_of_undefined_params <
                removed_parameters_permissions_.undefined_params.size()
            ? " are"
            : " is";
    disallowed_by_policy_info += " disallowed by policies";
  }

  if (disallowed_by_user_info.empty() && disallowed_by_policy_info.empty()) {
    SDL_LOG_INFO("There are not disallowed by user or by policy parameters.");
    return;
  }

  smart_objects::SmartObject& info =
      response[strings::msg_params][strings::info];

  std::string summary;
  if (!disallowed_by_policy_info.empty()) {
    summary += disallowed_by_policy_info;
  }

  if (!disallowed_by_user_info.empty()) {
    summary = summary.empty() ? disallowed_by_user_info
                              : summary + ", " + disallowed_by_user_info;
  }

  info = info.asString().empty() ? summary : info.asString() + " " + summary;
}

void CommandImpl::RemoveDisallowedParameters() {
  SDL_LOG_AUTO_TRACE();

  smart_objects::SmartObject& params = (*message_)[strings::msg_params];

  for (const auto& key : params.enumerate()) {
    if (parameters_permissions_.disallowed_params.end() !=
        parameters_permissions_.disallowed_params.find(key)) {
      // Remove from request all disallowed parameters
      params.erase(key);
      removed_parameters_permissions_.disallowed_params.insert(key);
      SDL_LOG_INFO("Following parameter is disallowed by user: " << key);
    }

    else if (removed_parameters_permissions_.undefined_params.end() !=
             removed_parameters_permissions_.undefined_params.find(key)) {
      // Remove from request all undefined yet parameters
      params.erase(key);
      removed_parameters_permissions_.undefined_params.insert(key);
      SDL_LOG_INFO("Following parameter is disallowed by policy: " << key);
    }

    else if (parameters_permissions_.allowed_params.end() ==
             parameters_permissions_.allowed_params.find(key)) {
      // Remove from request all parameters missed in allowed
      params.erase(key);
      removed_parameters_permissions_.undefined_params.insert(key);
      SDL_LOG_INFO("Following parameter is not found among allowed parameters '"
                   << key << "' and will be treated as disallowed.");
    }
  }
}

bool CommandImpl::ReplaceMobileWithHMIAppId(
    ns_smart_device_link::ns_smart_objects::SmartObject& message) {
  SDL_LOG_AUTO_TRACE();
  if (message.keyExists(strings::app_id)) {
    ApplicationSharedPtr application =
        application_manager_.application(message[strings::app_id].asUInt());
    if (!application) {
      SDL_LOG_ERROR("Substitution mobile --> HMI id is failed.");
      return false;
    }
    SDL_LOG_DEBUG("ReplaceMobileWithHMIAppId from "
                  << message[strings::app_id].asInt() << " to "
                  << application->hmi_app_id());
    message[strings::app_id] = application->hmi_app_id();
  } else {
    switch (message.getType()) {
      case smart_objects::SmartType::SmartType_Array: {
        smart_objects::SmartArray* message_array = message.asArray();
        smart_objects::SmartArray::iterator it = message_array->begin();
        for (; it != message_array->end(); ++it) {
          if (!ReplaceMobileWithHMIAppId(*it)) {
            break;
          }
        }
        break;
      }
      case smart_objects::SmartType::SmartType_Map: {
        std::set<std::string> keys = message.enumerate();
        std::set<std::string>::const_iterator key = keys.begin();
        for (; key != keys.end(); ++key) {
          std::string k = *key;
          if (!ReplaceMobileWithHMIAppId(message[*key])) {
            break;
          }
        }
        break;
      }
      default: {
        break;
      }
    }
  }

  return true;
}

void CommandImpl::FormatResponse(smart_objects::SmartObject& response) {
  AddDisallowedParametersToInfo(response);
  AddDisallowedParameters(response);
}

bool CommandImpl::ReplaceHMIWithMobileAppId(
    ns_smart_device_link::ns_smart_objects::SmartObject& message) {
  if (message.keyExists(strings::app_id)) {
    ApplicationSharedPtr application =
        application_manager_.application_by_hmi_app(
            message[strings::app_id].asUInt());

    if (!application) {
      SDL_LOG_ERROR("Substitution HMI --> mobile id is failed.");
      return false;
    }
    SDL_LOG_DEBUG("ReplaceHMIWithMobileAppId from "
                  << message[strings::app_id].asInt() << " to "
                  << application->app_id());
    message[strings::app_id] = application->app_id();
  } else {
    switch (message.getType()) {
      case smart_objects::SmartType::SmartType_Array: {
        smart_objects::SmartArray* message_array = message.asArray();
        smart_objects::SmartArray::iterator it = message_array->begin();
        for (; it != message_array->end(); ++it) {
          if (!ReplaceHMIWithMobileAppId(*it)) {
            break;
          }
        }
        break;
      }
      case smart_objects::SmartType::SmartType_Map: {
        std::set<std::string> keys = message.enumerate();
        std::set<std::string>::const_iterator key = keys.begin();
        for (; key != keys.end(); ++key) {
          if (!ReplaceHMIWithMobileAppId(message[*key])) {
            break;
          }
        }
        break;
      }
      default: {
        break;
      }
    }
  }

  return true;
}

uint32_t CommandImpl::CalcCommandInternalConsecutiveNumber(
    ApplicationConstSharedPtr app) {
  SDL_LOG_AUTO_TRACE();
  const DataAccessor<CommandsMap> accessor = app->commands_map();
  const CommandsMap& commands = accessor.GetData();

  uint32_t last_command_number = 0u;
  if (!commands.empty()) {
    last_command_number = commands.rbegin()->first;
  }

  return last_command_number + 1;
}

bool CommandImpl::CheckSyntax(const std::string& str,
                              bool allow_empty_line) const {
  if (std::string::npos != str.find_first_of("\t\n")) {
    SDL_LOG_ERROR("CheckSyntax failed! :" << str);
    return false;
  }
  if (std::string::npos != str.find("\\n") ||
      std::string::npos != str.find("\\t")) {
    SDL_LOG_ERROR("CheckSyntax failed! :" << str);
    return false;
  }
  if (!allow_empty_line) {
    if ((std::string::npos == str.find_first_not_of(' '))) {
      return false;
    }
  }
  return true;
}

bool CommandImpl::IsHMIResultSuccess(
    hmi_apis::Common_Result::eType result_code,
    HmiInterfaces::InterfaceID interface) const {
  SDL_LOG_AUTO_TRACE();
  if (application_manager::commands::IsHMIResultSuccess(result_code)) {
    return true;
  }

  const HmiInterfaces::InterfaceState state =
      application_manager_.hmi_interfaces().GetInterfaceState(interface);
  return hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == result_code &&
         HmiInterfaces::STATE_NOT_AVAILABLE != state;
}

}  // namespace commands
}  // namespace application_manager
