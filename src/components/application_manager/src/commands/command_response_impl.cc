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

#include "application_manager/commands/command_response_impl.h"
#include "application_manager/application_manager_impl.h"

namespace application_manager {

namespace commands {

CommandResponseImpl::CommandResponseImpl(const MessageSharedPtr& message)
    : CommandImpl(message) {
}

CommandResponseImpl::~CommandResponseImpl() {
}

bool CommandResponseImpl::Init() {
  return true;
}

bool CommandResponseImpl::CleanUp() {
  return true;
}

void CommandResponseImpl::Run() {
}

void CommandResponseImpl::SendResponse(
    bool success, const mobile_apis::Result::eType& result_code, bool final_message) {
  LOG4CXX_INFO(logger_, "Trying to send response");

  (*message_)[strings::params][strings::protocol_type] = mobile_protocol_type_;
  (*message_)[strings::params][strings::protocol_version] = protocol_version_;
  (*message_)[strings::msg_params][strings::success] = success;

  if (!(*message_)[strings::msg_params].keyExists(strings::result_code)) {
    if (mobile_apis::Result::INVALID_ENUM != result_code) {
      (*message_)[strings::msg_params][strings::result_code] = result_code;
    } else if ((*message_)[strings::params].keyExists(hmi_response::code)) {
      (*message_)[strings::msg_params][strings::result_code] =
          (*message_)[strings::params][hmi_response::code];
    } else {
      if (success) {
        (*message_)[strings::msg_params][strings::result_code] =
            mobile_apis::Result::SUCCESS;
      } else {
        (*message_)[strings::msg_params][strings::result_code] =
            mobile_apis::Result::INVALID_ENUM;
      }
    }
  }

  ApplicationManagerImpl::instance()->SendMessageToMobile(message_, final_message);
}

}  // namespace commands

}  // namespace application_manager
