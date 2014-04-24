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
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "config_profile/profile.h"
#include "interfaces/MOBILE_API.h"
#include "utils/file_system.h"
#include "encryption/Base64.h"

namespace application_manager {

namespace commands {

const std::string EncodedSyncPDataRequest::TEMPORARY_HARDCODED_FILENAME =
    "policy_sync_data.dat";
const std::string EncodedSyncPDataRequest::TEMPORARY_HARDCODED_FOLDERNAME =
    "policies";

EncodedSyncPDataRequest::EncodedSyncPDataRequest(
    const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

EncodedSyncPDataRequest::~EncodedSyncPDataRequest() {
}

void EncodedSyncPDataRequest::Run() {
  LOG4CXX_INFO(logger_, "EncodedSyncPDataRequest::Run");

  ApplicationSharedPtr application =
      ApplicationManagerImpl::instance()->application(connection_key());

  if (!application) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const std::string& sync_file_name = TEMPORARY_HARDCODED_FILENAME;

  size_t data_array_size = (*message_)[strings::msg_params][strings::data].
      length();

  std::string encoded_string_pdata;
  for (size_t i = 0; i < data_array_size; ++i) {
    encoded_string_pdata += (*message_)[strings::msg_params][strings::data][i]
        .asString();
  }

  LOG4CXX_INFO(logger_, "encoded_string_pdata = " << encoded_string_pdata);

  const std::string string_pdata = base64_decode(encoded_string_pdata);

  LOG4CXX_INFO(logger_, "string_pdata = " << string_pdata);

  const std::vector<uint8_t> file_data(string_pdata.begin(),
                                             string_pdata.end());

  std::string path =
      profile::Profile::instance()->app_storage_folder() + "/";
  path += TEMPORARY_HARDCODED_FOLDERNAME;

  std::string file_path =
      file_system::CreateDirectory(TEMPORARY_HARDCODED_FOLDERNAME);

  mobile_apis::Result::eType save_result =
      ApplicationManagerImpl::instance()->SaveBinary(file_data,
                                                     file_path,
                                                     sync_file_name,
                                                     0);

  switch(save_result) {
    case mobile_apis::Result::SUCCESS:
      SendResponse(true, save_result);
      break;
    default:
      SendResponse(false, save_result);
      break;
  }
}

}  // namespace commands

}  // namespace application_manager
