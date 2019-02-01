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

#include "application_manager/commands/command_notification_from_mobile_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/rpc_service.h"
#include "application_manager/message_helper.h"

namespace application_manager {

namespace commands {

CommandNotificationFromMobileImpl::CommandNotificationFromMobileImpl(
    const MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandImpl(message,
                  application_manager,
                  rpc_service,
                  hmi_capabilities,
                  policy_handler) {}

CommandNotificationFromMobileImpl::~CommandNotificationFromMobileImpl() {}

bool CommandNotificationFromMobileImpl::Init() {
  return true;
}

bool CommandNotificationFromMobileImpl::CleanUp() {
  return true;
}

void CommandNotificationFromMobileImpl::Run() {}

void CommandNotificationFromMobileImpl::SendNotification() {
  (*message_)[strings::params][strings::protocol_type] = mobile_protocol_type_;
  (*message_)[strings::params][strings::protocol_version] = protocol_version_;
  (*message_)[strings::params][strings::message_type] =
      static_cast<int32_t>(application_manager::MessageType::kNotification);

  LOG4CXX_INFO(logger_, "SendNotification");
  MessageHelper::PrintSmartObject(*message_);

  rpc_service_.SendMessageToMobile(message_);
}

void CommandNotificationFromMobileImpl::SendNotificationToHMI() {
  (*message_)[strings::params][strings::protocol_type] = hmi_protocol_type_;
  (*message_)[strings::params][strings::protocol_version] = protocol_version_;
  rpc_service_.SendMessageToHMI(message_);
}

void CommandNotificationFromMobileImpl::SendNotificationToConsumers() {
  // This may have to change when handling future / unknown app services
  SendNotification();
  SendNotificationToHMI();
  // application_manager.GetAppServiceManager().NotifyConsumers(message_);
}

}  // namespace commands

}  // namespace application_manager
