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

#include "application_manager/commands/mobile/delete_command_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

DeleteCommandRequest::DeleteCommandRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message),
      is_ui_send_(false),
      is_vr_send_(false),
      is_ui_received_(false),
      is_vr_received_(false),
      ui_result_(hmi_apis::Common_Result::INVALID_ENUM),
      vr_result_(hmi_apis::Common_Result::INVALID_ENUM)  {
}

DeleteCommandRequest::~DeleteCommandRequest() {
}

void DeleteCommandRequest::Run() {
  LOG4CXX_INFO(logger_, "DeleteCommandRequest::Run");

  ApplicationSharedPtr application = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (!application) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    LOG4CXX_ERROR(logger_, "Application is not registered");
    return;
  }

  smart_objects::SmartObject* command = application->FindCommand(
      (*message_)[strings::msg_params][strings::cmd_id].asInt());

  if (!command) {
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    LOG4CXX_ERROR(logger_, "Invalid ID");
    return;
  }

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  msg_params[strings::cmd_id] =
      (*message_)[strings::msg_params][strings::cmd_id];
  msg_params[strings::app_id] = application->app_id();

  // we should specify amount of required responses in the 1st request
  uint32_t chaining_counter = 0;
  if ((*command).keyExists(strings::menu_params)) {
    ++chaining_counter;
  }

  if ((*command).keyExists(strings::vr_commands)) {
    ++chaining_counter;
  }

  if ((*command).keyExists(strings::menu_params)) {
    is_ui_send_ = true;

    SendHMIRequest(hmi_apis::FunctionID::UI_DeleteCommand, &msg_params, true);
  }
  // check vr params
  if ((*command).keyExists(strings::vr_commands)) {
    is_vr_send_ = true;

    // VR params
    msg_params[strings::grammar_id] = application->get_grammar_id();
    msg_params[strings::type] = hmi_apis::Common_VRCommandType::Command;
    SendHMIRequest(hmi_apis::FunctionID::VR_DeleteCommand, &msg_params, true);
  }
}

void DeleteCommandRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "DeleteCommandRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_DeleteCommand: {
      LOG4CXX_INFO(logger_, "Received UI_DeleteCommand event");
      is_ui_received_ = true;
      ui_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());

      break;
    }
    case hmi_apis::FunctionID::VR_DeleteCommand: {
      LOG4CXX_INFO(logger_, "Received VR_DeleteCommand event");
      is_vr_received_ = true;
      vr_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());

      break;
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      return;
    }
  }

  if (!IsPendingResponseExist()) {
    ApplicationSharedPtr application =
        ApplicationManagerImpl::instance()->application(connection_key());

    if (!application) {
      LOG4CXX_ERROR(logger_, "NULL pointer");
      return;
    }

    smart_objects::SmartObject* command = application->FindCommand(
        (*message_)[strings::msg_params][strings::cmd_id].asInt());

    if (command) {
      mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;

      bool result = ((hmi_apis::Common_Result::SUCCESS == ui_result_) &&
                     (hmi_apis::Common_Result::SUCCESS == vr_result_)) ||
                     ((hmi_apis::Common_Result::SUCCESS == ui_result_) &&
                     (hmi_apis::Common_Result::INVALID_ENUM == vr_result_)) ||
                     ((hmi_apis::Common_Result::INVALID_ENUM == ui_result_) &&
                     (hmi_apis::Common_Result::SUCCESS == vr_result_));

      if (result) {
        application->RemoveCommand(
          (*message_)[strings::msg_params][strings::cmd_id].asInt());
      }

      if (!result && (hmi_apis::Common_Result::REJECTED == ui_result_)) {
        result_code = static_cast<mobile_apis::Result::eType>(vr_result_);
      } else {
        result_code = static_cast<mobile_apis::Result::eType>(
            std::max(ui_result_, vr_result_));
      }

      SendResponse(result, result_code, NULL, &(message[strings::msg_params]));
    }
  }
}

bool DeleteCommandRequest::IsPendingResponseExist() {
  return is_ui_send_ != is_ui_received_ || is_vr_send_ != is_vr_received_;
}

}  // namespace commands

}  // namespace application_manager
