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

#include <string>
#include "application_manager/commands/mobile/on_encoded_sync_pdata_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "utils/file_system.h"
#include "encryption/Base64.h"
#include "utils/logger.h"

namespace application_manager {

namespace commands {

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Commands"));

OnEncodedSyncPDataNotification::OnEncodedSyncPDataNotification(
    const MessageSharedPtr& message): CommandResponseImpl(message) {
}

OnEncodedSyncPDataNotification::~OnEncodedSyncPDataNotification() {
}

void OnEncodedSyncPDataNotification::Run() {
  const std::string fileName =
      (*message_)[strings::params][hmi_notification::file_name];

  if (!file_system::FileExists(fileName)) {
    (*message_)[strings::msg_params][strings::success] = false;
    (*message_)[strings::msg_params][strings::result_code] =
                NsSmartDeviceLinkRPC::V2::Result::FILE_NOT_FOUND;

    SendResponse();

    return;
  }

  std::vector<unsigned char> pData;

  file_system::ReadBinaryFile(fileName, pData);

  const std::string string_pdata = base64_decode(std::string(pData.begin(),
                                                        pData.end()));

  (*message_)[strings::params][strings::data] = string_pdata;

  SendResponse();
}

}  // namespace commands

}  // namespace application_manager

