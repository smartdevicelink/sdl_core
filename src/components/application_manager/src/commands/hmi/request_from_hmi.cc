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

#include "application_manager/commands/hmi/request_from_hmi.h"
#include "application_manager/application_manager_impl.h"

namespace application_manager {

namespace commands {

RequestFromHMI::RequestFromHMI(const MessageSharedPtr& message)
    : CommandImpl(message) {

  // Replace HMI app id with Mobile connection id
  ApplicationManagerImpl::instance()->ReplaceHMIByMobileAppId(*(message.get()));
}

RequestFromHMI::~RequestFromHMI() {
}

bool RequestFromHMI::Init() {
  return true;
}

bool RequestFromHMI::CleanUp() {
  return true;
}

void RequestFromHMI::Run() {
}

void RequestFromHMI::on_event(const event_engine::Event& event) {
}

void RequestFromHMI::SendResponse(uint32_t correlation_id,
                                  hmi_apis::FunctionID::eType function_id,
                                  hmi_apis::Common_Result::eType result_code) {
  smart_objects::SmartObject* message = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  (*message)[strings::params][strings::function_id] = function_id;
  (*message)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message)[strings::params][strings::correlation_id] = correlation_id;
  (*message)[strings::params][hmi_response::code] = result_code;

  ApplicationManagerImpl::instance()->ManageHMICommand(message);
}


}  // namespace commands
}  // namespace application_manager

