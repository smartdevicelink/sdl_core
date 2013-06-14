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

#include "application_manager/commands/mobile/encoded_sync_pdata_request.h"
#include "application_manager/message_chaining.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "utils/file_system.h"
#include "encryption/Base64.h"

namespace application_manager {

namespace commands {

const std::string EncodedSyncPDataRequest::TEMPORARY_HARDCODED_FILENAME
                = "policy_sync_data.dat";
const std::string EncodedSyncPDataRequest::TEMPORARY_HARDCODED_FOLDERNAME
                = "/config/policies";

EncodedSyncPDataRequest::EncodedSyncPDataRequest(
    const MessageSharedPtr& message)
  : CommandRequestImpl(message) {
}

EncodedSyncPDataRequest::~EncodedSyncPDataRequest() {
}

void EncodedSyncPDataRequest::Run() {
  ApplicationImpl* application_impl = static_cast<ApplicationImpl*>
        (application_manager::ApplicationManagerImpl::instance()->
        application((*message_)[strings::msg_params][strings::app_id]));

    if (NULL == application_impl) {
      SendResponse(false, NsSmartDeviceLinkRPC::V2::
                   Result::APPLICATION_NOT_REGISTERED);
      return;
    }

    const int correlationId =
      (*message_)[strings::params][strings::correlation_id];
    const int connectionKey =
      (*message_)[strings::params][strings::connection_key];

    const unsigned int cmd_id = 104;
      ApplicationManagerImpl::instance()->AddMessageChain(
        new MessageChaining(connectionKey, correlationId),
        connectionKey, correlationId, cmd_id);

    ApplicationManagerImpl::instance()->SendMessageToHMI(&(*message_));

    uint64_t free_space = file_system::AvailableSpace();

    const std::string& sync_file_name = TEMPORARY_HARDCODED_FILENAME;

    const std::string string_pdata = base64_decode(((*message_)[strings::params]
                                              [strings::data]).asString());

    const std::vector<unsigned char> char_vector_pdata(string_pdata.begin(),
                                                       string_pdata.end());

    if (free_space > string_pdata.size()) {
      std::string relative_file_path =
          file_system::CreateDirectory(TEMPORARY_HARDCODED_FOLDERNAME);
      relative_file_path += "/";
      relative_file_path += sync_file_name;

      if (file_system::Write(file_system::FullPath(relative_file_path),
                             char_vector_pdata)) {
        SendResponse(true, NsSmartDeviceLinkRPC::V2::Result::SUCCESS);
      } else {
        SendResponse(false, NsSmartDeviceLinkRPC::V2::Result::GENERIC_ERROR);
      }
    } else {
      SendResponse(false, NsSmartDeviceLinkRPC::V2::Result::OUT_OF_MEMORY);
    }
}

}  // namespace commands

}  // namespace application_manager
