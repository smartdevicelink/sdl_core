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

#include "application_manager/commands/delete_command_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"


namespace application_manager {

namespace commands {

DeleteCommandRequest::DeleteCommandRequest(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

DeleteCommandRequest::~DeleteCommandRequest() {
}

void DeleteCommandRequest::Run() {
  ApplicationImpl* application =
        static_cast<ApplicationImpl*>(ApplicationManagerImpl::instance()->
            application((*message_)[strings::params][strings::connection_key]));

  if (!application) {
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED);
    return;
  }
  smart_objects::CSmartObject* command = application->
        FindCommand((*message_)[strings::msg_params][strings::cmd_id].asInt());

  if (!command) {
      SendResponse(false, NsSmartDeviceLinkRPC::V2::Result::INVALID_ID);
      return;
  }

  const int corellation_id =
        (*message_)[strings::params][strings::correlation_id];
  const int connection_key =
        (*message_)[strings::params][strings::connection_key];


  MessageChaining * chain = NULL;

  if ((*command)[strings::msg_params].keyExists(strings::menu_params)) {
      smart_objects::CSmartObject* p_smrt_ui  =
          new smart_objects::CSmartObject();

      // TODO(DK) HMI Request Id
      const int ui_cmd_id = 202;
      (*p_smrt_ui)[strings::params][strings::function_id] = ui_cmd_id;

      (*p_smrt_ui)[strings::params][strings::message_type] =
          MessageType::kRequest;

      (*p_smrt_ui)[strings::msg_params][strings::cmd_id] =
          (*message_)[strings::msg_params][strings::cmd_id];

      (*p_smrt_ui)[strings::msg_params][strings::app_id] =
          application->app_id();

     chain = ApplicationManagerImpl::instance()->AddMessageChain(chain,
          connection_key, corellation_id, ui_cmd_id, p_smrt_ui);

      ApplicationManagerImpl::instance()->SendMessageToHMI(p_smrt_ui);
    }

    // check vr params
    if ((*command)[strings::msg_params].keyExists(strings::vr_commands)) {
      smart_objects::CSmartObject* p_smrt_vr  =
          new smart_objects::CSmartObject();

      // TODO(DK) HMI Request Id
      const int vr_cmd_id = 203;
      (*p_smrt_vr)[strings::params][strings::function_id] = vr_cmd_id;

      (*p_smrt_vr)[strings::params][strings::message_type] =
          MessageType::kRequest;

      (*p_smrt_vr)[strings::msg_params][strings::cmd_id] =
          (*message_)[strings::msg_params][strings::cmd_id];

      (*p_smrt_vr)[strings::msg_params][strings::vr_commands] =
          (*message_)[strings::msg_params][strings::vr_commands];

      (*p_smrt_vr)[strings::msg_params][strings::app_id] =
               application->app_id();

      ApplicationManagerImpl::instance()->AddMessageChain(chain,
          connection_key, corellation_id, vr_cmd_id, p_smrt_vr);

      ApplicationManagerImpl::instance()->SendMessageToHMI(p_smrt_vr);
    }
}

}  // namespace commands

}  // namespace application_manager
