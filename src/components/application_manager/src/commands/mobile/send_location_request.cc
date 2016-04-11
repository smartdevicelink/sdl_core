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
#include "application_manager/commands/mobile/send_location_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"

namespace application_manager {

namespace commands {

SendLocationRequest::SendLocationRequest(const MessageSharedPtr& message)
 : CommandRequestImpl(message) {
}

SendLocationRequest::~SendLocationRequest() {
}

void SendLocationRequest::Run() {
  using namespace hmi_apis;
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager::ApplicationManagerImpl::instance()
      ->application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_,
                      "An application with connection key " << connection_key()
                      << " is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];
  std::list<Common_TextFieldName::eType> fields_to_check;

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

  if ((*message_)[strings::msg_params].keyExists(strings::location_image)) {
    mobile_apis::Result::eType verification_result =
        mobile_apis::Result::SUCCESS;
    verification_result = MessageHelper::VerifyImage(
        (*message_)[strings::msg_params][strings::location_image], app);
    if (mobile_apis::Result::SUCCESS != verification_result) {
      LOG4CXX_ERROR(logger_, "VerifyImage INVALID_DATA!");
      SendResponse(false, verification_result);
      return;
    }
  }

  smart_objects::SmartObject request_msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  request_msg_params = (*message_)[strings::msg_params];
  request_msg_params[strings::app_id] = app->hmi_app_id();

  SendHMIRequest(hmi_apis::FunctionID::Navigation_SendLocation,
                 &request_msg_params, true);
}

void SendLocationRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_SendLocation: {
      LOG4CXX_INFO(logger_, "Received Navigation_SendLocation event");
      mobile_apis::Result::eType result_code = GetMobileResultCode(
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asUInt()));
      bool result =
          mobile_apis::Result::SUCCESS == result_code ||
          mobile_apis::Result::WARNINGS == result_code ||
          mobile_apis::Result::UNSUPPORTED_RESOURCE == result_code ;
      SendResponse(result, result_code, NULL, &(message[strings::msg_params]));
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      break;
    }
  }
}

bool SendLocationRequest::IsWhiteSpaceExist() {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str;
  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  if (msg_params.keyExists(strings::location_name)) {
    str = msg_params[strings::location_name].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
                    "parameter locationName contains invalid character");
      return true;
    }
  }

  if (msg_params.keyExists(strings::location_description)) {
    str = msg_params[strings::location_description].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
                    "parameter locationDescription contains invalid character");
      return true;
    }
  }

  if (msg_params.keyExists(strings::address_lines)) {
    const smart_objects::SmartArray* al_array =
        msg_params[strings::address_lines].asArray();
    smart_objects::SmartArray::const_iterator it_al = al_array->begin();
    smart_objects::SmartArray::const_iterator it_al_end = al_array->end();
    for (; it_al != it_al_end; ++it_al) {
      str = (*it_al).asCharArray();
      if(!CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_,
                      "parameter address_lines contains invalid character");
        return true;
      }
    }
  }

  if (msg_params.keyExists(strings::phone_number)) {
    str = msg_params[strings::phone_number].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
                    "parameter phoneNumber contains invalid character");
      return true;
    }
  }

  if (msg_params.keyExists(strings::location_image)) {
    str = msg_params[strings::location_image][strings::value].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
                          "parameter value in locationImage contains invalid character");
      return true;
    }
  }

  return false;
}

bool SendLocationRequest::CheckHMICapabilities(std::list<hmi_apis::Common_TextFieldName::eType>& fields_names) {
  using namespace smart_objects;
  using namespace hmi_apis;

  if (fields_names.empty()) {
    return true;
  }

  ApplicationManagerImpl* instance = ApplicationManagerImpl::instance();
  const HMICapabilities& hmi_capabilities = instance->hmi_capabilities();
  if (!hmi_capabilities.is_ui_cooperating()) {
    LOG4CXX_ERROR(logger_, "UI is not supported.");
    return false;
  }

  if (hmi_capabilities.display_capabilities()) {
    const SmartObject disp_cap = (*hmi_capabilities.display_capabilities());
    const SmartObject& text_fields = disp_cap.getElement(hmi_response::text_fields);
    const size_t len = text_fields.length();
    for (size_t i = 0; i < len; ++i) {
      const SmartObject& text_field = text_fields[i];
      const Common_TextFieldName::eType filed_name =
          static_cast<Common_TextFieldName::eType>(text_field.getElement(strings::name).asInt());
      const std::list<Common_TextFieldName::eType>::iterator it =
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
