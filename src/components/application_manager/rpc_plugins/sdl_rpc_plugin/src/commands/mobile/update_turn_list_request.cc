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

#include "sdl_rpc_plugin/commands/mobile/update_turn_list_request.h"
#include <string>

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "utils/custom_string.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

namespace custom_str = utils::custom_string;

UpdateTurnListRequest::UpdateTurnListRequest(
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

UpdateTurnListRequest::~UpdateTurnListRequest() {}

void UpdateTurnListRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (!app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    SDL_LOG_ERROR("Application is not registered");
    return;
  }

  if (IsWhiteSpaceExist()) {
    SDL_LOG_ERROR("Incoming update turn list has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  // ProcessSoftButtons checks strings on the contents incorrect character

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons((*message_)[strings::msg_params],
                                        app,
                                        policy_handler_,
                                        application_manager_);

  if (mobile_apis::Result::SUCCESS != processing_result) {
    SDL_LOG_ERROR("INVALID_DATA!");
    SendResponse(false, processing_result);
    return;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::turn_list)) {
    smart_objects::SmartObject& turn_list_array =
        ((*message_)[strings::msg_params][strings::turn_list]);
    for (uint32_t i = 0; i < turn_list_array.length(); ++i) {
      if ((turn_list_array[i].keyExists(strings::turn_icon)) &&
          (mobile_apis::Result::INVALID_DATA ==
           MessageHelper::VerifyImage(turn_list_array[i][strings::turn_icon],
                                      app,
                                      application_manager_))) {
        SDL_LOG_ERROR("MessageHelper::VerifyImage return INVALID_DATA");
        SendResponse(false, mobile_apis::Result::INVALID_DATA);
        return;
      }
    }
  }

  smart_objects::SmartObject msg_params = (*message_)[strings::msg_params];

  if ((*message_)[strings::msg_params].keyExists(strings::turn_list)) {
    if (!CheckTurnListArray()) {
      SDL_LOG_ERROR("INVALID_DATA!");
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
    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
    SendHMIRequest(
        hmi_apis::FunctionID::Navigation_UpdateTurnList, &msg_params, true);
  } else {
    // conditional mandatory
    SDL_LOG_ERROR("INVALID_DATA!");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
  }
}

void UpdateTurnListRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_UpdateTurnList: {
      SDL_LOG_INFO("Received Navigation_UpdateTurnList event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
      const hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      std::string response_info;
      GetInfo(message, response_info);
      const bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_Navigation);
      SendResponse(result,
                   MessageHelper::HMIToMobileResult(result_code),
                   response_info.empty() ? NULL : response_info.c_str(),
                   &(message[strings::msg_params]));
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event " << event.id());
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
  SDL_LOG_AUTO_TRACE();
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
          SDL_LOG_ERROR(
              "Invalid turn_list navigation_text text syntax check failed");
          return true;
        }
      }

      if ((*it_tl).keyExists(strings::turn_icon)) {
        str = (*it_tl)[strings::turn_icon][strings::value].asCharArray();
        if (!CheckSyntax(str)) {
          SDL_LOG_ERROR(
              "Invalid turn_list turn_icon value syntax check failed");
          return true;
        }
      }
    }
  }
  return false;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
