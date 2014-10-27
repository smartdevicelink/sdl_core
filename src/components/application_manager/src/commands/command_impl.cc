/**
 Copyright (c) 2014, Ford Motor Company
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

#include "application_manager/commands/command_impl.h"
#include "config_profile/profile.h"

namespace application_manager {
namespace commands {

CREATE_LOGGERPTR_LOCAL(CommandImpl::logger_, "Commands")

const int32_t CommandImpl::hmi_protocol_type_ = 1;
const int32_t CommandImpl::mobile_protocol_type_ = 0;
const int32_t CommandImpl::protocol_version_ = 3;

CommandImpl::CommandImpl(const MessageSharedPtr& message)
    : message_(message),
      default_timeout_(profile::Profile::instance()->default_timeout()) {
}

CommandImpl::~CommandImpl() {
  CleanUp();
}

bool CommandImpl::CheckPermissions(){
  return true;
}

bool CommandImpl::Init() {
  return true;
}

bool CommandImpl::CleanUp() {
  return true;
}

void CommandImpl::Run() {
}

uint32_t CommandImpl::default_timeout() const {
  return default_timeout_;
}

uint32_t CommandImpl::correlation_id() const {
  return (*message_)[strings::params][strings::correlation_id].asUInt();
}

int32_t CommandImpl::function_id() const {
  return (*message_)[strings::params][strings::function_id].asInt();
}

uint32_t CommandImpl::connection_key() const {
  return (*message_)[strings::params][strings::connection_key].asUInt();
}

void CommandImpl::onTimeOut() {

}

}  // namespace commands
}  // namespace application_manager
