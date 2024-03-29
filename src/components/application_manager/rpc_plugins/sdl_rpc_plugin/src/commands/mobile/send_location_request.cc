/*

 Copyright (c) 2018, Ford Motor Company
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
#include "sdl_rpc_plugin/commands/mobile/send_location_request.h"
#include <algorithm>
#include "application_manager/message_helper.h"
#include "utils/custom_string.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

SendLocationRequest::SendLocationRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromMobileImpl(message,
                            application_manager,
                            rpc_service,
                            hmi_capabilities,
                            policy_handler) {}

SendLocationRequest::~SendLocationRequest() {}

void SendLocationRequest::Run() {
  using namespace hmi_apis;
  using smart_objects::SmartObject;
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("An application with connection key "
                  << connection_key() << " is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];
  if (msg_params.keyExists(strings::delivery_mode)) {
    const RPCParams& allowed_params = parameters_permissions().allowed_params;

    if (helpers::in_range(allowed_params, strings::delivery_mode)) {
      msg_params.erase(strings::delivery_mode);
    }
  }

  std::vector<Common_TextFieldName::eType> fields_to_check;
  if (msg_params.keyExists(strings::location_name)) {
    fields_to_check.push_back(Common_TextFieldName::locationName);
  }
  if (msg_params.keyExists(strings::location_description)) {
    fields_to_check.push_back(Common_TextFieldName::locationDescription);
  }
  if (msg_params.keyExists(strings::address_lines)) {
    fields_to_check.push_back(Common_TextFieldName::addressLines);
  }
  if (msg_params.keyExists(strings::phone_number)) {
    fields_to_check.push_back(Common_TextFieldName::phoneNumber);
  }

  if (!CheckHMICapabilities(fields_to_check)) {
    SendResponse(false, mobile_apis::Result::UNSUPPORTED_RESOURCE);
    return;
  }

  if (IsWhiteSpaceExist()) {
    SDL_LOG_ERROR("Strings contain invalid characters");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (msg_params.keyExists(strings::address)) {
    const utils::custom_string::CustomString& address =
        msg_params[strings::address].asCustomString();
    if (address.empty()) {
      msg_params.erase(strings::address);
    }
  }

  if (!CheckFieldsCompatibility()) {
    SDL_LOG_ERROR("CheckFieldsCompatibility failed");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (msg_params.keyExists(strings::location_image)) {
    mobile_apis::Result::eType verification_result = MessageHelper::VerifyImage(
        (*message_)[strings::msg_params][strings::location_image],
        app,
        application_manager_);
    if (mobile_apis::Result::INVALID_DATA == verification_result) {
      SDL_LOG_ERROR("VerifyImage INVALID_DATA!");
      SendResponse(false, verification_result);
      return;
    }
  }

  SmartObject request_msg_params = msg_params;
  request_msg_params[strings::app_id] = app->hmi_app_id();
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
  SendHMIRequest(
      hmi_apis::FunctionID::Navigation_SendLocation, &request_msg_params, true);
}

void SendLocationRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  using namespace hmi_apis;
  const smart_objects::SmartObject& message = event.smart_object();
  if (hmi_apis::FunctionID::Navigation_SendLocation == event.id()) {
    SDL_LOG_INFO("Received Navigation_SendLocation event");
    EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
    const Common_Result::eType result_code = static_cast<Common_Result::eType>(
        message[strings::params][hmi_response::code].asInt());
    std::string response_info;
    GetInfo(message, response_info);
    const bool result = PrepareResultForMobileResponse(
        result_code, HmiInterfaces::HMI_INTERFACE_Navigation);
    SendResponse(result,
                 MessageHelper::HMIToMobileResult(result_code),
                 response_info.empty() ? NULL : response_info.c_str(),
                 &(message[strings::params]));
    return;
  }
  SDL_LOG_ERROR("Received unknown event " << event.id());
}

bool SendLocationRequest::CheckFieldsCompatibility() {
  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];
  MessageHelper::PrintSmartObject(msg_params);
  const bool longitude_degrees_exist =
      msg_params.keyExists(strings::longitude_degrees);
  const bool latitude_degrees_exist =
      msg_params.keyExists(strings::latitude_degrees);
  const bool address_exist = msg_params.keyExists(strings::address);

  if (latitude_degrees_exist ^ longitude_degrees_exist) {
    SDL_LOG_DEBUG("latitude and longitude should be provided only in pair");
    return false;
  }

  if (!address_exist && !longitude_degrees_exist && !latitude_degrees_exist) {
    SDL_LOG_DEBUG("address or latitude/longtitude should should be provided");
    return false;
  }
  return true;
}
void insert_if_contains(
    const smart_objects::SmartObject& msg_params,
    const std::string& param_key,
    std::vector<utils::custom_string::CustomString>& output_vector) {
  if (msg_params.keyExists(param_key)) {
    output_vector.push_back(msg_params[param_key].asCustomString());
  }
}

bool SendLocationRequest::IsWhiteSpaceExist() {
  SDL_LOG_AUTO_TRACE();
  std::vector<utils::custom_string::CustomString> fields_to_check;
  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];
  insert_if_contains(msg_params, strings::location_name, fields_to_check);
  insert_if_contains(
      msg_params, strings::location_description, fields_to_check);
  insert_if_contains(msg_params, strings::phone_number, fields_to_check);

  if (msg_params.keyExists(strings::address_lines)) {
    const smart_objects::SmartArray* al_array =
        msg_params[strings::address_lines].asArray();
    smart_objects::SmartArray::const_iterator it_al = al_array->begin();
    smart_objects::SmartArray::const_iterator it_al_end = al_array->end();
    for (; it_al != it_al_end; ++it_al) {
      const utils::custom_string::CustomString& val = (*it_al).asCustomString();
      fields_to_check.push_back(val);
    }
  }

  if (msg_params.keyExists(strings::address)) {
    const smart_objects::SmartObject& address_so = msg_params[strings::address];
    insert_if_contains(address_so, strings::country_name, fields_to_check);
    insert_if_contains(address_so, strings::country_code, fields_to_check);
    insert_if_contains(address_so, strings::postal_code, fields_to_check);
    insert_if_contains(
        address_so, strings::administrative_area, fields_to_check);
    insert_if_contains(address_so, strings::locality, fields_to_check);
    insert_if_contains(address_so, strings::sub_locality, fields_to_check);
    insert_if_contains(address_so, strings::thoroughfare, fields_to_check);
    insert_if_contains(address_so, strings::sub_thoroughfare, fields_to_check);
  }

  std::vector<utils::custom_string::CustomString>::iterator it =
      fields_to_check.begin();
  for (; it != fields_to_check.end(); ++it) {
    const std::string& str = it->AsMBString();
    if (!CheckSyntax(str, false)) {
      SDL_LOG_ERROR("string '" << str << "'' contains invalid characters");
      return true;
    }
  }
  return false;
}

bool SendLocationRequest::CheckHMICapabilities(
    std::vector<hmi_apis::Common_TextFieldName::eType>& fields_names) {
  using namespace smart_objects;
  using namespace hmi_apis;
  if (fields_names.empty()) {
    return true;
  }

  const HMICapabilities& hmi_capabilities = hmi_capabilities_;
  if (!hmi_capabilities.is_ui_cooperating()) {
    SDL_LOG_ERROR("UI is not supported.");
    return false;
  }

  auto display_capabilities = hmi_capabilities.display_capabilities();
  if (display_capabilities) {
    const SmartObject& text_fields =
        display_capabilities->getElement(hmi_response::text_fields);
    const size_t len = text_fields.length();
    for (size_t i = 0; i < len; ++i) {
      const SmartObject& text_field = text_fields[i];
      const Common_TextFieldName::eType filed_name =
          static_cast<Common_TextFieldName::eType>(
              text_field.getElement(strings::name).asInt());
      const std::vector<Common_TextFieldName::eType>::iterator it =
          std::find(fields_names.begin(), fields_names.end(), filed_name);
      if (it != fields_names.end()) {
        fields_names.erase(it);
      }
    }
  }

  if (!fields_names.empty()) {
    SDL_LOG_ERROR("Some fields are not supported by capabilities");
    return false;
  }
  return true;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
