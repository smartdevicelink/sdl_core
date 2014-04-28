/**
 * Copyright (c) 2014, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <string>
#include "application_manager/commands/hmi/on_received_policy_update.h"
#include "application_manager/policies/policy_handler.h"
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

OnReceivedPolicyUpdate::OnReceivedPolicyUpdate(const MessageSharedPtr& message)
  : NotificationFromHMI(message) {
}

OnReceivedPolicyUpdate::~OnReceivedPolicyUpdate() {
}

void OnReceivedPolicyUpdate::Run() {
  LOG4CXX_INFO(logger_, "OnReceivedPolicyUpdate::Run");
  const std::string& file_path =
    (*message_)[strings::msg_params][hmi_notification::policyfile].asString();
  policy::BinaryMessage file_content;
  if (!file_system::ReadBinaryFile(file_path, file_content)) {
    LOG4CXX_ERROR(logger_, "Failed to read Update file.");
    return;
  }
  policy::PolicyHandler::instance()->ReceiveMessageFromSDK(file_path, file_content);
}

}  // namespace commands

}  // namespace application_manager
