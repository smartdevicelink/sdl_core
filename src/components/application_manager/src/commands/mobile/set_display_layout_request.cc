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

#include "application_manager/commands/mobile/set_display_layout_request.h"

#include "application_manager/message_helper.h"
#include "application_manager/application_impl.h"

namespace application_manager {

namespace commands {

SetDisplayLayoutRequest::SetDisplayLayoutRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

SetDisplayLayoutRequest::~SetDisplayLayoutRequest() {}

void SetDisplayLayoutRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationConstSharedPtr app =
      application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  (*message_)[strings::msg_params][strings::app_id] = app->app_id();
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(hmi_apis::FunctionID::UI_SetDisplayLayout,
                 &((*message_)[strings::msg_params]),
                 true);
}

void SetDisplayLayoutRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::UI_SetDisplayLayout: {
      LOG4CXX_INFO(logger_, "Received UI_SetDisplayLayout event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      const bool response_success = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_UI);
      std::string info;
      GetInfo(message, info);
      smart_objects::SmartObject msg_params = message[strings::msg_params];
      if (response_success) {
        // in case templates_available is empty copy from hmi capabilities
        if (msg_params.keyExists(hmi_response::display_capabilities)) {
          if (0 ==
              msg_params[hmi_response::display_capabilities]
                        [hmi_response::templates_available].length()) {
            msg_params[hmi_response::display_capabilities]
                      [hmi_response::templates_available] =
                          application_manager_.hmi_capabilities().GetData().display_capabilities()->getElement(
                              hmi_response::templates_available);
          }
        }
      }
      SendResponse(response_success,
                   MessageHelper::HMIToMobileResult(result_code),
                   info.empty() ? NULL : info.c_str(),
                   &msg_params);
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }
}

}  // namespace commands

}  // namespace application_manager
