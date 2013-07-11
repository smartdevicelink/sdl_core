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

#include "application_manager/commands/mobile/show_constant_tbt_request.h"
#include "application_manager/message_chaining.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"

namespace application_manager {

namespace commands {

ShowConstantTBTRequest::ShowConstantTBTRequest(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

ShowConstantTBTRequest::~ShowConstantTBTRequest() {
}

void ShowConstantTBTRequest::Run() {
  LOG4CXX_INFO(logger_, "ShowConstantTBTRequest::Run");

  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->
      application((*message_)[strings::params][strings::connection_key]));

  if (NULL == app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    LOG4CXX_ERROR(logger_, "Application is not registered");
    return;
  }

  smart_objects::CSmartObject* navi_show_constant_tbt  =
      new smart_objects::CSmartObject();

  if (!navi_show_constant_tbt) {
    SendResponse(false, mobile_apis::Result::OUT_OF_MEMORY);
    LOG4CXX_ERROR(logger_, "Null pointer");
    return;
  }

  app->set_tbt_show_command((*message_)[strings::msg_params]);

  const int correlation_id =
      (*message_)[strings::params][strings::correlation_id];

  const int connection_key =
      (*message_)[strings::params][strings::connection_key];

  const long hmi_correlation_id = ApplicationManagerImpl::instance()->
  GetHMIcorrelation_id(correlation_id, connection_key);

  const int hmi_request_id = hmi_apis::FunctionID::Navigation_ShowConstantTBT;

  (*navi_show_constant_tbt)[strings::params][strings::function_id] =
      hmi_request_id;

  // be sure to use HMI correlation id
  (*navi_show_constant_tbt)[strings::params][strings::correlation_id] =
      hmi_correlation_id;

  (*navi_show_constant_tbt)[strings::params][strings::message_type] =
    MessageType::kRequest;

  ApplicationManagerImpl::instance()->AddMessageChain(NULL,
        connection_key, correlation_id, hmi_correlation_id, &(*message_));

  ApplicationManagerImpl::instance()->ManageHMICommand(navi_show_constant_tbt);
}

}  // namespace commands

}  // namespace application_manager
