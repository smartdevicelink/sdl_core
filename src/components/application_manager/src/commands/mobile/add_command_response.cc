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

#include "application_manager/commands/mobile/add_command_response.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_chaining.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

AddCommandResponse::AddCommandResponse(
  const MessageSharedPtr& message): CommandResponseImpl(message) {
}

AddCommandResponse::~AddCommandResponse() {
}

void AddCommandResponse::Run() {
  LOG4CXX_INFO(logger_, "AddCommandResponse::Run");

  // check if response false
  if ((*message_)[strings::msg_params][strings::success] == false) {
    LOG4CXX_ERROR(logger_, "Success = false");
    SendResponse(false);
    return;
  }

  const unsigned int correlation_id =
    (*message_)[strings::params][strings::correlation_id].asUInt();

  MessageChaining* msg_chain =
    ApplicationManagerImpl::instance()->GetMessageChain(correlation_id);

  if (NULL == msg_chain) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  const int connection_key =  msg_chain->connection_key();
  smart_objects::SmartObject data = msg_chain->data();

  // we need to retrieve stored response code before message chain decrease
  const hmi_apis::Common_Result::eType result_ui =
      msg_chain->ui_response_result();
  const hmi_apis::Common_Result::eType result_vr =
      msg_chain->vr_response_result();

  // sending response
  if (ApplicationManagerImpl::instance()->DecreaseMessageChain(
        correlation_id)) {

    ApplicationImpl* app = static_cast<ApplicationImpl*>(
             ApplicationManagerImpl::instance()->
             application(connection_key));

    smart_objects::SmartObject* command =
       app->FindCommand(
           data[strings::msg_params][strings::cmd_id].asInt());

    if (!command) {
      if ((data[strings::msg_params].keyExists(strings::menu_params)) ||
          (data[strings::msg_params].keyExists(strings::vr_commands))) {
        app->AddCommand(data[strings::msg_params][strings::cmd_id].asInt(),
                              data[strings::msg_params]);
      }

      if ((hmi_apis::Common_Result::SUCCESS == result_ui) &&
          (hmi_apis::Common_Result::SUCCESS == result_vr)) {
        SendResponse(true);
    } else {
      // TODO: Check Response result code
      SendResponse(false);
    }
  }
}
}

}  // namespace commands

}  // namespace application_manager
