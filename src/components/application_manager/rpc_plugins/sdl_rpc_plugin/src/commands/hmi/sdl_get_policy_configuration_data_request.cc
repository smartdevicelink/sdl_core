/*
 * Copyright (c) 2019, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sdl_rpc_plugin/commands/hmi/sdl_get_policy_configuration_data_request.h"
#include "application_manager/application_manager.h"
#include "application_manager/policies/policy_handler_interface.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDLGetPolicyConfigurationDataRequest::SDLGetPolicyConfigurationDataRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : RequestFromHMI(message,
                     application_manager,
                     rpc_service,
                     hmi_capabilities,
                     policy_handle) {}

SDLGetPolicyConfigurationDataRequest::~SDLGetPolicyConfigurationDataRequest() {}

void SDLGetPolicyConfigurationDataRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject response_value(
      smart_objects::SmartType::SmartType_Array);

  hmi_apis::Common_Result::eType result_code =
      PrepareResponseParams(response_value);

  if (hmi_apis::Common_Result::SUCCESS != result_code) {
    LOG4CXX_ERROR(logger_, "Unable to PrepareResponseParams");
    SendErrorResponse(
        correlation_id(),
        hmi_apis::FunctionID::SDL_GetPolicyConfigurationData,
        result_code,
        "",
        application_manager::commands::Command::SOURCE_SDL_TO_HMI);
    return;
  }

  smart_objects::SmartObject response_msg_params(
      smart_objects::SmartType::SmartType_Map);
  response_msg_params[strings::value] = response_value;
  SendResponse(true,
               correlation_id(),
               hmi_apis::FunctionID::SDL_GetPolicyConfigurationData,
               result_code,
               &response_msg_params,
               application_manager::commands::Command::SOURCE_SDL_TO_HMI);
}

hmi_apis::Common_Result::eType
SDLGetPolicyConfigurationDataRequest::PrepareResponseParams(
    smart_objects::SmartObject& response_out) const {
  LOG4CXX_AUTO_TRACE(logger_);
  const auto policy_type =
      (*message_)[strings::msg_params][strings::policy_type].asString();

  const auto property =
      (*message_)[strings::msg_params][strings::property].asString();

  auto policy_table_data = policy_handler_.GetPolicyTableData();
  if (!policy_table_data.isMember(policy_type)) {
    LOG4CXX_ERROR(
        logger_,
        "policy_type " << policy_type << " doesn't exist in policy table.");
    return hmi_apis::Common_Result::DATA_NOT_AVAILABLE;
  }

  auto& policy_section_table_data = policy_table_data[policy_type];
  if (!policy_section_table_data.isMember(property)) {
    LOG4CXX_ERROR(
        logger_,
        "property " << property << " doesn't exist in " << policy_type);
    return hmi_apis::Common_Result::DATA_NOT_AVAILABLE;
  }

  auto& property_table_data = policy_section_table_data[property];
  response_out = GetValueParam(property_table_data);

  return hmi_apis::Common_Result::SUCCESS;
}

void clear_new_line_symbol(std::string& str_to_clear) {
  str_to_clear.erase(
      std::remove_if(str_to_clear.begin(),
                     str_to_clear.end(),
                     [](char character) { return '\n' == character; }),
      str_to_clear.end());
}

smart_objects::SmartObject SDLGetPolicyConfigurationDataRequest::GetValueParam(
    const Json::Value& policy_property) const {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject value(smart_objects::SmartType_Array);

  auto put_element_in_value_array = [&value](const Json::Value& element,
                                             const int32_t index) {
    Json::FastWriter writer;
    std::string str;
    if (element.type() == Json::objectValue) {
      str = writer.write(element);
      clear_new_line_symbol(str);
    } else {
      str = element.asString();
    }
    value[index] = str;
  };

  if (policy_property.type() == Json::arrayValue) {
    for (Json::ArrayIndex i = 0; i < policy_property.size(); i++) {
      put_element_in_value_array(policy_property[i], i);
    }
    return value;
  }
  put_element_in_value_array(policy_property, 0);
  return value;
}

}  // namespace commands
}  // namespace sdl_rpc_plugin
