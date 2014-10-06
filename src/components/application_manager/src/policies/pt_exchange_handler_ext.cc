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

#include "application_manager/policies/pt_exchange_handler_ext.h"

#include <fstream>
#include <string>

#include "application_manager/message_helper.h"
#include "config_profile/profile.h"
#include "utils/file_system.h"

using application_manager::MessageHelper;
using profile::Profile;
using std::string;


namespace policy {

CREATE_LOGGERPTR_GLOBAL(logger_, "PolicyHandler")

PTExchangeHandlerExt::PTExchangeHandlerExt(PolicyHandler* policy_handler)
  : PTExchangeHandler(),
    policy_handler_(policy_handler) {
  DCHECK(policy_handler_);
}

PTExchangeHandlerExt::~PTExchangeHandlerExt() {
}

void PTExchangeHandlerExt::Start() {
  LOG4CXX_TRACE(logger_, "Start exchange PT");

  const string policy_snapshot_file_name =
      Profile::instance()->policies_snapshot_file_name();
  const std::string system_files_path =
      Profile::instance()->system_files_path();
  const std::string policy_snapshot_full_path = system_files_path + '/' +
      policy_snapshot_file_name;
  BinaryMessageSptr pt_snapshot = policy_handler_->RequestPTUpdate();
  if (pt_snapshot.valid()) {
    if (file_system::CreateDirectoryRecursively(system_files_path) &&
        file_system::WriteBinaryFile(policy_snapshot_full_path, *pt_snapshot)) {
      MessageHelper::SendPolicyUpdate(policy_snapshot_full_path,
                                      policy_handler_->TimeoutExchange(),
                                      policy_handler_->RetrySequenceDelaysSeconds());
    } else {
      LOG4CXX_ERROR(logger_, "Failed to write snapshot file to " << policy_snapshot_file_name);
    }
  } else {
    LOG4CXX_ERROR(logger_, "Failed to obtain policy table snapshot");
  }
}

void PTExchangeHandlerExt::Stop() {
  // Nothing doing
}

}  // namespace policy
