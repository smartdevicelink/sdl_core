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

#include <string>
#include "application_manager/commands/mobile/update_turn_list_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "utils/custom_string.h"

namespace application_manager {

namespace commands {

namespace custom_str = utils::custom_string;

UpdateTurnListRequest::UpdateTurnListRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message) {}

UpdateTurnListRequest::~UpdateTurnListRequest() {}

void UpdateTurnListRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (!app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    LOG4CXX_ERROR(logger_, "Application is not registered");
    return;
  }

  if (IsWhiteSpaceExist()) {
    LOG4CXX_ERROR(logger_,
                  "Incoming update turn list has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  // ProcessSoftButtons checks strings on the contents incorrect character

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons((*message_)[strings::msg_params], app,
          application_manager::ApplicationManagerImpl::instance()->GetPolicyHandler());

  if (mobile_apis::Result::SUCCESS != processing_result) {
    LOG4CXX_ERROR(logger_, "INVALID_DATA!");
    SendResponse(false, processing_result);
    return;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::turn_list)) {
    smart_objects::SmartObject& turn_list_array =
        ((*message_)[strings::msg_params][strings::turn_list]);
    for (uint32_t i = 0; i < turn_list_array.length(); ++i) {
      if ((turn_list_array[i].keyExists(strings::turn_icon)) &&
          (mobile_apis::Result::SUCCESS !=
           MessageHelper::VerifyImage(turn_list_array[i][strings::turn_icon],
                                      app))) {
        LOG4CXX_ERROR(logger_,
                      "MessageHelper::VerifyImage return INVALID_DATA");
        SendResponse(false, mobile_apis::Result::INVALID_DATA);
        return;
      }
    }
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params = (*message_)[strings::msg_params];

  if ((*message_)[strings::msg_params].keyExists(strings::turn_list)) {
    if (!CheckTurnListArray()) {
      LOG4CXX_ERROR(logger_, "INVALID_DATA!");
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }

    for (uint32_t i = 0; i < msg_params[strings::turn_list].length(); ++i) {
      if (msg_params[strings::turn_list][i].keyExists(hmi_request::navi_text)) {
        const custom_str::CustomString& navigation_text =
            msg_params[strings::turn_list][i][hmi_request::navi_text]
                .asCustomString();
        msg_params[strings::turn_list][i].erase(hmi_request::navi_text);
        msg_params[strings::turn_list][i][hmi_request::navi_text]
                  [hmi_request::field_name] = static_cast<int>(
                      hmi_apis::Common_TextFieldName::turnText);
        msg_params[strings::turn_list][i][hmi_request::navi_text]
                  [hmi_request::field_text] = navigation_text;
      }
    }
  }

  msg_params[strings::app_id] = app->app_id();

  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    MessageHelper::SubscribeApplicationToSoftButton(
        (*message_)[strings::msg_params], app, function_id());
  }

  if ((*message_)[strings::msg_params].keyExists(strings::turn_list) ||
      (*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    SendHMIRequest(
        hmi_apis::FunctionID::Navigation_UpdateTurnList, &msg_params, true);
  } else {
    // conditional mandatory
    LOG4CXX_ERROR(logger_, "INVALID_DATA!");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
  }
}

void UpdateTurnListRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_UpdateTurnList: {
      LOG4CXX_INFO(logger_, "Received Navigation_UpdateTurnList event");

      mobile_apis::Result::eType result_code =
          static_cast<mobile_apis::Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      HMICapabilities& hmi_capabilities =
          ApplicationManagerImpl::instance()->hmi_capabilities();

      bool result =
          (mobile_apis::Result::SUCCESS == result_code) ||
          ((mobile_apis::Result::UNSUPPORTED_RESOURCE == result_code) &&
           (hmi_capabilities.is_ui_cooperating()));

      SendResponse(result, result_code, NULL, &(message[strings::msg_params]));
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      break;
    }
  }
}

bool UpdateTurnListRequest::CheckTurnListArray() {
  int32_t length =
      (*message_)[strings::msg_params][strings::turn_list].length();
  if (0 == length) {
    return false;
  }

  for (int32_t i = 0; i < length; ++i) {
    if (!((*message_)[strings::msg_params][strings::turn_list][i].keyExists(
            hmi_request::navi_text)) &&
        !((*message_)[strings::msg_params][strings::turn_list][i].keyExists(
            strings::turn_icon))) {
      return false;
    }
  }
  return true;
}

bool UpdateTurnListRequest::IsWhiteSpaceExist() {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  if ((*message_)[strings::msg_params].keyExists(strings::turn_list)) {
    const smart_objects::SmartArray* tl_array =
        (*message_)[strings::msg_params][strings::turn_list].asArray();

    smart_objects::SmartArray::const_iterator it_tl = tl_array->begin();
    smart_objects::SmartArray::const_iterator it_tl_end = tl_array->end();

    for (; it_tl != it_tl_end; ++it_tl) {
      if ((*it_tl).keyExists(strings::navigation_text)) {
        str = (*it_tl)[strings::navigation_text].asCharArray();
        if (!CheckSyntax(str)) {
          LOG4CXX_ERROR(
              logger_,
              "Invalid turn_list navigation_text text syntax check failed");
          return true;
        }
      }

      if ((*it_tl).keyExists(strings::turn_icon)) {
        str = (*it_tl)[strings::turn_icon][strings::value].asCharArray();
        if (!CheckSyntax(str)) {
          LOG4CXX_ERROR(
              logger_, "Invalid turn_list turn_icon value syntax check failed");
          return true;
        }
      }
    }
  }
  return false;
}

}  // namespace commands

}  // namespace application_manager
