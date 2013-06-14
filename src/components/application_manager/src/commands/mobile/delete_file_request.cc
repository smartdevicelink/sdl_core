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

#include "application_manager/commands/mobile/delete_file_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "utils/file_system.h"


namespace application_manager {

namespace commands {

DeleteFileRequest::DeleteFileRequest(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

DeleteFileRequest::~DeleteFileRequest() {
}

void DeleteFileRequest::Run() {
  ApplicationImpl* application =
      static_cast<ApplicationImpl*>(ApplicationManagerImpl::instance()->
      application((*message_)[strings::params][strings::connection_key]));

  if (!application) {
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const std::string& sync_file_name =
        (*message_)[strings::msg_params][strings::sync_file_name];

  std::string relative_file_path = application->name();
  relative_file_path += "/";
  relative_file_path += sync_file_name;

  std::string full_file_path = file_system::FullPath(relative_file_path);

  if (file_system::FileExists(full_file_path)) {
    if (file_system::DeleteFile(full_file_path)) {
      application->DeleteFile(sync_file_name);
      SendResponse(true, NsSmartDeviceLinkRPC::V2::Result::SUCCESS);
    } else {
      SendResponse(false, NsSmartDeviceLinkRPC::V2::Result::GENERIC_ERROR);
    }
  } else {
    SendResponse(false, NsSmartDeviceLinkRPC::V2::Result::INVALID_DATA);
  }
}

}  // namespace commands

}  // namespace application_manager
