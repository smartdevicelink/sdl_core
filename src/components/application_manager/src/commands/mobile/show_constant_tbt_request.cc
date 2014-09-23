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

#include <string.h>
#include "application_manager/commands/mobile/show_constant_tbt_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

ShowConstantTBTRequest::ShowConstantTBTRequest(const MessageSharedPtr& message)
 : CommandRequestImpl(message) {
}

ShowConstantTBTRequest::~ShowConstantTBTRequest() {
}

void ShowConstantTBTRequest::Run() {
  LOG4CXX_INFO(logger_, "ShowConstantTBTRequest::Run");

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (!app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    LOG4CXX_ERROR(logger_, "Application is not registered");
    return;
  }
  // SDLAQ-CRS-664, VC3.1
  if ((*message_)[strings::msg_params].empty()) {
    LOG4CXX_ERROR(logger_, "INVALID_DATA!");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  msg_params = (*message_)[strings::msg_params];

  if (IsWhiteSpaceExist()) {
    LOG4CXX_ERROR(logger_,
                  "Incoming show constant TBT has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  //ProcessSoftButtons checks strings on the contents incorrect character

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons(msg_params, app);

  if (mobile_apis::Result::SUCCESS != processing_result) {
    LOG4CXX_ERROR(logger_, "INVALID_DATA!");
    SendResponse(false, processing_result);
    return;
  }


  mobile_apis::Result::eType verification_result =
      mobile_apis::Result::SUCCESS;
  if (msg_params.keyExists(strings::turn_icon)) {
    verification_result = MessageHelper::VerifyImage(
        msg_params[strings::turn_icon], app);
    if (mobile_apis::Result::SUCCESS != verification_result) {
      LOG4CXX_ERROR(logger_, "VerifyImage INVALID_DATA!");
      SendResponse(false, verification_result);
      return;
    }
  }

  if (msg_params.keyExists(strings::next_turn_icon)) {
    verification_result = MessageHelper::VerifyImage(
        msg_params[strings::next_turn_icon], app);
    if (mobile_apis::Result::SUCCESS != verification_result) {
      LOG4CXX_ERROR(logger_, "VerifyImage INVALID_DATA!");
      SendResponse(false, verification_result);
      return;
    }
  }

  msg_params[strings::app_id] = app->app_id();

  msg_params[hmi_request::navi_texts] = smart_objects::SmartObject(
      smart_objects::SmartType_Array);

  int32_t index = 0;
  if (msg_params.keyExists(strings::navigation_text_1)) {
    // erase useless parametr
    msg_params.erase(strings::navigation_text_1);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::navigationText1);
    msg_params[hmi_request::navi_texts][index++][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::navigation_text_1];
  }

  if (msg_params.keyExists(strings::navigation_text_2)) {
    // erase useless param
    msg_params.erase(strings::navigation_text_2);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::navigationText2);
    msg_params[hmi_request::navi_texts][index++][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::navigation_text_2];
  }

  if (msg_params.keyExists(strings::eta)) {
    // erase useless param
    msg_params.erase(strings::eta);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::ETA);
    msg_params[hmi_request::navi_texts][index++][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::eta];
  }

  if (msg_params.keyExists(strings::total_distance)) {
    // erase useless param
    msg_params.erase(strings::total_distance);
    msg_params[hmi_request::navi_texts][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::totalDistance);
    msg_params[hmi_request::navi_texts][index++][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::total_distance];
  }

  if (msg_params.keyExists(strings::time_to_destination)) {
      // erase useless param
      msg_params.erase(strings::time_to_destination);
      msg_params[hmi_request::navi_texts][index][hmi_request::field_name] =
          static_cast<int32_t>(hmi_apis::Common_TextFieldName::timeToDestination);
      msg_params[hmi_request::navi_texts][index++][hmi_request::field_text] =
          (*message_)[strings::msg_params][strings::time_to_destination];
  }

  if (msg_params.keyExists(strings::soft_buttons)) {
    MessageHelper::SubscribeApplicationToSoftButton(msg_params, app, function_id());
  }

  app->set_tbt_show_command(msg_params);
  SendHMIRequest(hmi_apis::FunctionID::Navigation_ShowConstantTBT, &msg_params,
                 true);
}


void ShowConstantTBTRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "ShowConstantTBTRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_ShowConstantTBT: {
      LOG4CXX_INFO(logger_, "Received Navigation_ShowConstantTBT event");

      mobile_apis::Result::eType result_code =
          GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt()));
      HMICapabilities& hmi_capabilities =
                ApplicationManagerImpl::instance()->hmi_capabilities();
      bool result = false;
      if (mobile_apis::Result::SUCCESS == result_code) {
        result = true;
      } else if ((mobile_apis::Result::UNSUPPORTED_RESOURCE == result_code) &&
          hmi_capabilities.is_ui_cooperating()) {
        result = true;
      }

      SendResponse(result, result_code, NULL, &(message[strings::msg_params]));
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      break;
    }
  }
}

bool ShowConstantTBTRequest::IsWhiteSpaceExist() {
  LOG4CXX_INFO(logger_, "ShowConstantTBTRequest::IsWhiteSpaceExist");
  const char* str = NULL;

  if ((*message_)[strings::msg_params].keyExists(strings::turn_icon)) {
    str = (*message_)[strings::msg_params]
                      [strings::turn_icon][strings::value].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid turn_icon value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::next_turn_icon)) {
    str = (*message_)[strings::msg_params]
                      [strings::next_turn_icon][strings::value].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
                    "Invalid next_turn_icon value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::navigation_text_1)) {
    str = (*message_)[strings::msg_params]
                      [strings::navigation_text_1].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
                    "Invalid navigation_text_1 value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::navigation_text_2)) {
    str = (*message_)[strings::msg_params]
                      [strings::navigation_text_2].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
                    "Invalid navigation_text_2 value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::eta)) {
    str = (*message_)[strings::msg_params][strings::eta].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid eta value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::total_distance)) {
    str = (*message_)[strings::msg_params]
                      [strings::total_distance].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
                    "Invalid total_distance value syntax check failed");
      return true;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::time_to_destination)) {
    str = (*message_)[strings::msg_params]
                      [strings::time_to_destination].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
                    "Invalid time_to_destination value syntax check failed");
      return true;
    }
  }
  return false;
}

}  // namespace commands

}  // namespace application_manager
