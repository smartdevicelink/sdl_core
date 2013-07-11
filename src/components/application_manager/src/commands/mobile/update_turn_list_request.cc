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

#include "application_manager/commands/mobile/update_turn_list_request.h"
#include "application_manager/message_chaining.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "interfaces/HMI_API.h"
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

UpdateTurnListRequest::UpdateTurnListRequest(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

UpdateTurnListRequest::~UpdateTurnListRequest() {
}

void UpdateTurnListRequest::Run() {
  LOG4CXX_INFO(logger_, "UpdateTurnListRequest::Run");

  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->
      application((*message_)[strings::params][strings::connection_key]));

  if (NULL == app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    LOG4CXX_ERROR(logger_, "Application is not registered");
    return;
  }

  std::string file_path;

  const size_t turn_list_size = (*message_)[strings::msg_params]
                                           [strings::turn_list].length();

  for (int i = 0; i < turn_list_size; ++i) {
    file_path = app->name() + "/" +
        (*message_)[strings::msg_params][strings::turn_list][i]
                   [strings::turn_icon][strings::value].asString();
    file_path = file_system::FullPath(file_path);

    if (!file_system::FileExists(file_path)) {
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      LOG4CXX_ERROR(logger_, "File is not exists");
      return;
    }

    (*message_)[strings::msg_params][strings::turn_list][i]
               [strings::turn_icon][strings::value] = file_path;
  }

  const int correlation_id =
      (*message_)[strings::params][strings::correlation_id];
  const int connection_key =
      (*message_)[strings::params][strings::connection_key];

  const int hmi_request_id = hmi_apis::FunctionID::Navigation_UpdateTurnList;

  ApplicationManagerImpl::instance()->AddMessageChain(NULL,
        connection_key, correlation_id, hmi_request_id, &(*message_));

  ApplicationManagerImpl::instance()->ManageHMICommand(message_);
}

}  // namespace commands

}  // namespace application_manager
