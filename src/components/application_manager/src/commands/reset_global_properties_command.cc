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

#include "application_manager/commands/reset_global_properties_command.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_chaining.h"
#include "application_manager/application_impl.h"
#include "JSONHandler/SDLRPCObjects/V2/HMILevel.h"
#include "utils/logger.h"

namespace application_manager {

namespace commands {

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Commands"));

ResetGlobalPropertiesCommand::ResetGlobalPropertiesCommand(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

ResetGlobalPropertiesCommand::~ResetGlobalPropertiesCommand() {
}

void ResetGlobalPropertiesCommand::Run() {
  LOG4CXX_INFO(logger_, "ResetGlobalPropertiesCommand::Run ");

  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->
      application((*message_)[strings::params][strings::app_id]));

  if (NULL != app) {
    LOG4CXX_ERROR_EXT(logger_, "No application associated with session key ");
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  NsSmartDeviceLink::NsSmartObjects::CSmartObject obj = app->hmi_level();
  if(NsSmartDeviceLinkRPCV2::HMILevel::HMI_NONE == obj.asInt())
  {
    LOG4CXX_WARN(logger_, "An application " << app->name()
                 << " has not been activated yet!" );
    SendResponse(false, NsSmartDeviceLinkRPC::V2::Result::REJECTED);
    return;
  }

  const int corellationId =
      (*message_)[strings::params][strings::correlation_id];
  const int connectionKey =
      (*message_)[strings::params][strings::connection_key];

  ApplicationManagerImpl::instance()->AddMessageChain(
      new MessageChaining(connectionKey, corellationId),
      connectionKey, corellationId);

    /*(*message_)[strings::msg_params][strings::properties] =
        *app->help_promt();*/

  ApplicationManagerImpl::instance()->SendMessageToHMI(&(*message_));
}

}  // namespace commands

}  // namespace application_manager
