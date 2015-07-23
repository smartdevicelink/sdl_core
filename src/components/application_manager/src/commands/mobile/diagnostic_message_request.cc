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
#include "application_manager/commands/mobile/diagnostic_message_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "config_profile/profile.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

DiagnosticMessageRequest::DiagnosticMessageRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

DiagnosticMessageRequest::~DiagnosticMessageRequest() {
}

void DiagnosticMessageRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const std::vector<uint32_t>& supported_diag_modes =
      profile::Profile::instance()->supported_diag_modes();

  smart_objects::SmartObject& msg_data =
      (*message_)[strings::msg_params][strings::message_data];

  const uint8_t mode_position = 0;
  const uint32_t msg_diagnostic_mode = msg_data[mode_position].asUInt();
  if (supported_diag_modes.end() == std::find(supported_diag_modes.begin(),
                                              supported_diag_modes.end(),
                                              msg_diagnostic_mode)) {
    LOG4CXX_ERROR(logger_, "Received diagnostic mode " << msg_diagnostic_mode <<
                           " is not supported.");
    SendResponse(false, mobile_apis::Result::REJECTED,
                 "Received diagnostic mode is not supported.");
    return;
  }

  // Add app_id for HMI request
  (*message_)[strings::msg_params][strings::app_id] = app->app_id();

  SendHMIRequest(hmi_apis::FunctionID::VehicleInfo_DiagnosticMessage,
                 &(*message_)[strings::msg_params], true);

}

void DiagnosticMessageRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::VehicleInfo_DiagnosticMessage: {
      mobile_apis::Result::eType result_code =
          static_cast<mobile_apis::Result::eType>(
              message[strings::params][hmi_response::code].asInt());

      bool result = mobile_apis::Result::SUCCESS == result_code;

      SendResponse(result, result_code, NULL, &(message[strings::msg_params]));
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
