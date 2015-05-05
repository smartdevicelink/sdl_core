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

#include "application_manager/commands/hmi/response_from_hmi.h"
#include "application_manager/application_manager_impl.h"
#include "smart_objects/smart_object.h"

namespace application_manager {

namespace commands {

ResponseFromHMI::ResponseFromHMI(const MessageSharedPtr& message)
    : CommandImpl(message) {
  //If it is error response, shift info
  if ((*message)[strings::params].keyExists(hmi_response::message)) {
    (*message)[strings::msg_params][strings::info] =
        (*message)[strings::params][hmi_response::message];
  }

  // Replace HMI app id with Mobile connection id
  ApplicationManagerImpl::instance()->ReplaceHMIByMobileAppId(*(message.get()));
}

ResponseFromHMI::~ResponseFromHMI() {
}

bool ResponseFromHMI::Init() {
  return true;
}

bool ResponseFromHMI::CleanUp() {
  return true;
}

void ResponseFromHMI::Run() {
}

void ResponseFromHMI::SendResponseToMobile(const MessageSharedPtr& message) {
  (*message)[strings::params][strings::message_type] = MessageType::kResponse;

  ApplicationManagerImpl::instance()->ManageMobileCommand(message);
}

void ResponseFromHMI::CreateHMIRequest(
    const hmi_apis::FunctionID::eType& function_id,
    const smart_objects::SmartObject& msg_params) const {

  smart_objects::SmartObjectSPtr result = new smart_objects::SmartObject;

  if (!result) {
    LOG4CXX_ERROR(logger_, "Memory allocation failed.");
    return;
  }

  // get hmi correlation id for chaining further request from this object
  const uint32_t hmi_correlation_id_ = ApplicationManagerImpl::instance()
      ->GetNextHMICorrelationID();

  NsSmartDeviceLink::NsSmartObjects::SmartObject& request = *result;
  request[strings::params][strings::message_type] = MessageType::kRequest;
  request[strings::params][strings::function_id] = function_id;
  request[strings::params][strings::correlation_id] = hmi_correlation_id_;
  request[strings::params][strings::protocol_version] =
      CommandImpl::protocol_version_;
  request[strings::params][strings::protocol_type] =
      CommandImpl::hmi_protocol_type_;

  request[strings::msg_params] = msg_params;

  if (!ApplicationManagerImpl::instance()->ManageHMICommand(result)) {
    LOG4CXX_ERROR(logger_, "Unable to send request");
    return;
  }
}

}  // namespace commands

}  // namespace application_manager
