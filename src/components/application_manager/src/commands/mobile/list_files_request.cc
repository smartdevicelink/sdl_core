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

#include "application_manager/commands/mobile/list_files_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "config_profile/profile.h"
#include "application_manager/mobile_command_factory.h"
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

ListFilesRequest::ListFilesRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

ListFilesRequest::~ListFilesRequest() {
}

void ListFilesRequest::Run() {
  LOG4CXX_INFO(logger_, "ListFilesRequest::Run");

  ApplicationSharedPtr application =
      ApplicationManagerImpl::instance()->application(connection_key());

  if (!application) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    LOG4CXX_ERROR(logger_, "Application is not registered");
    return;
  }

  if ((mobile_api::HMILevel::HMI_NONE == application->hmi_level()) &&
      (profile::Profile::instance()->list_files_in_none() <=
       application->list_files_in_none_count())) {
      // If application is in the HMI_NONE level the quantity of allowed
      // DeleteFile request is limited by the configuration profile
      LOG4CXX_ERROR(logger_, "Too many requests from the app with HMILevel HMI_NONE ");
      SendResponse(false, mobile_apis::Result::REJECTED);
      return;
  }

  application->increment_list_files_in_none_count();

  (*message_)[strings::msg_params][strings::space_available] =
        static_cast<int32_t>(ApplicationManagerImpl::instance()->
                             GetAvailableSpaceForApp(application->folder_name()));
  int32_t i = 0;
  const AppFilesMap& app_files = application->getAppFiles();
  for (AppFilesMap::const_iterator it = app_files.begin();
       it != app_files.end(); ++it) {
      //In AppFile to application stored full path to file. In message required
      //to write only name file.
      //Plus one required for move to next letter after '/'.
      (*message_)[strings::msg_params][strings::filenames][i++] =
        it->first.substr(it->first.find_last_of('/') + 1);
  }
  (*message_)[strings::params][strings::message_type] =
      application_manager::MessageType::kResponse;
  SendResponse(true, mobile_apis::Result::SUCCESS, NULL,
               &(*message_)[strings::msg_params]);
}

}  // namespace commands

}  // namespace application_manager
