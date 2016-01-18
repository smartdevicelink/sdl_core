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

#include "application_manager/commands/mobile/on_system_request_notification.h"
#include "interfaces/MOBILE_API.h"
#include "utils/file_system.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/policies/policy_handler.h"

namespace application_manager {

namespace commands {

namespace mobile {

OnSystemRequestNotification::OnSystemRequestNotification(
    const MessageSharedPtr& message)
    : CommandNotificationImpl(message) {
}

OnSystemRequestNotification::~OnSystemRequestNotification() {
}

void OnSystemRequestNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace application_manager;
  using namespace mobile_apis;

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->
                             application(connection_key());

  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Application with connection key "
                  << connection_key() << " is not registered.");
    return;
  }

  RequestType::eType request_type = static_cast<RequestType::eType>
      ((*message_)[strings::msg_params][strings::request_type].asInt());

  if (!policy::PolicyHandler::instance()->IsRequestTypeAllowed(
           app->mobile_app_id(), request_type)) {
    LOG4CXX_WARN(logger_, "Request type "  << request_type
                 <<" is not allowed by policies");
    return;
  }

  if (RequestType::PROPRIETARY == request_type) {
    /* According to requirements:
       "If the requestType = PROPRIETARY, add to mobile API fileType = JSON
        If the requestType = HTTP, add to mobile API fileType = BINARY"
       Also in Genivi SDL we don't save the PT to file - we put it directly in binary_data */
    (*message_)[strings::msg_params][strings::file_type] = FileType::JSON;
  } else if (RequestType::HTTP == request_type) {
    (*message_)[strings::msg_params][strings::file_type] = FileType::BINARY;
  }

  SendNotification();
}

}  //namespace mobile

}  // namespace commands

}  // namespace application_manager
