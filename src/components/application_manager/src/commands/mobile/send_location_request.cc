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
#include <algorithm>
#include "application_manager/commands/mobile/send_location_request.h"
#include "application_manager/message_helper.h"
#include "utils/custom_string.h"

namespace application_manager {

namespace commands {

SendLocationRequest::SendLocationRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

SendLocationRequest::~SendLocationRequest() {}

void SendLocationRequest::Run() {
  using namespace hmi_apis;
  using smart_objects::SmartObject;
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_,
                  "An application with connection key "
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
    LOG4CXX_ERROR(logger_, "Strings contain invalid characters");
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
    LOG4CXX_ERROR(logger_, "CheckFieldsCompatibility failed");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (msg_params.keyExists(strings::location_image)) {
    mobile_apis::Result::eType verification_result =
        mobile_apis::Result::SUCCESS;
    verification_result = MessageHelper::VerifyImage(
        (*message_)[strings::msg_params][strings::location_image],
        app,
        application_manager_);
    if (mobile_apis::Result::SUCCESS != verification_result) {
      LOG4CXX_ERROR(logger_, "VerifyImage INVALID_DATA!");
      SendResponse(false, verification_result);
      return;
    }
  }

  SmartObject request_msg_params = SmartObject(smart_objects::SmartType_Map);
  request_msg_params = msg_params;
  request_msg_params[strings::app_id] = app->hmi_app_id();
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
  SendHMIRequest(
      hmi_apis::FunctionID::Navigation_SendLocation, &request_msg_params, true);
}

void SendLocationRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace hmi_apis;
  const smart_objects::SmartObject& message = event.smart_object();
  if (hmi_apis::FunctionID::Navigation_SendLocation == event.id()) {
    LOG4CXX_INFO(logger_, "Received Navigation_SendLocation event");
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
  LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
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
    LOG4CXX_DEBUG(logger_,
                  "latitude and longitude should be provided only in pair");
    return false;
  }

  if (!address_exist && !longitude_degrees_exist && !latitude_degrees_exist) {
    LOG4CXX_DEBUG(logger_,
                  "address or latitude/longtitude should should be provided");
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
  LOG4CXX_AUTO_TRACE(logger_);
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
      LOG4CXX_ERROR(logger_,
                    "string '" << str << "'' contains invalid characters");
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

  const DataAccessor<HMICapabilities> hmi_capabilities_accessor = application_manager_.const_hmi_capabilities();
  const HMICapabilities& hmi_capabilities = hmi_capabilities_accessor.GetData();

  if (!hmi_capabilities.is_ui_cooperating()) {
    LOG4CXX_ERROR(logger_, "UI is not supported.");
    return false;
  }

  if (hmi_capabilities.display_capabilities()) {
    const SmartObject disp_cap = (*hmi_capabilities.display_capabilities());
    const SmartObject& text_fields =
        disp_cap.getElement(hmi_response::text_fields);
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
    LOG4CXX_ERROR(logger_, "Some fields are not supported by capabilities");
    return false;
  }
  return true;
}

}  // namespace commands

}  // namespace application_manager
