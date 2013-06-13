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

#include "application_manager/commands/set_icon_command.h"
#include "application_manager/message_chaining.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

SetIconCommand::SetIconCommand(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

SetIconCommand::~SetIconCommand() {
}

void SetIconCommand::Run() {
  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->
      application((*message_)[strings::params][strings::connection_key]));

  if (NULL == app) {
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const std::string& sync_file_name =
          (*message_)[strings::msg_params][strings::sync_file_name];

  std::string relative_file_path = app->name();
  relative_file_path += "/";
  relative_file_path += sync_file_name;

  std::string full_file_path = file_system::FullPath(relative_file_path);

  if (!file_system::FileExists(full_file_path)) {
    SendResponse(false, NsSmartDeviceLinkRPC::V2::Result::INVALID_DATA);
    return;
  }

  smart_objects::CSmartObject* set_app_icon_hmi_request  = new smart_objects::CSmartObject();

  (*set_app_icon_hmi_request)[strings::params][strings::app_id] =
      (*message_)[strings::msg_params][strings::correlation_id];

  (*set_app_icon_hmi_request)[strings::params][strings::sync_file_name] = full_file_path;

  const int correlation_id =
      (*message_)[strings::params][strings::correlation_id];
  const int connection_key =
      (*message_)[strings::params][strings::connection_key];

  // TODO(VS): HMI Request Id
  const int hmi_request_id = 208;

  ApplicationManagerImpl::instance()->AddMessageChain(NULL,
        connection_key, correlation_id, hmi_request_id, &(*message_));

  ApplicationManagerImpl::instance()->SendMessageToHMI(message_);
}

}  // namespace commands

}  // namespace application_manager
