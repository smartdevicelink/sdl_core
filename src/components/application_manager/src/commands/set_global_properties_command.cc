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

#include "application_manager/commands/set_global_properties_command.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "JSONHandler/SDLRPCObjects/V2/HMILevel.h"
#include "utils/logger.h"

namespace application_manager {

namespace commands {

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Commands"));

SetGlobalPropertiesCommand::SetGlobalPropertiesCommand(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

SetGlobalPropertiesCommand::~SetGlobalPropertiesCommand() {
}

void SetGlobalPropertiesCommand::Run() {
  LOG4CXX_INFO(logger_, "SetGlobalPropertiesCommand::Run ");

  NsSmartDeviceLink::NsSmartObjects::CSmartObject response;
  response[strings::params][strings::message_type] = MessageType::kResponse;
  response[strings::params][strings::correlation_id] =
      (*message_)[strings::params][strings::correlation_id];
  response[strings::params][strings::protocol_version] =
      (*message_)[strings::params][strings::protocol_version];
  response[strings::params][strings::connection_key] =
      (*message_)[strings::params][strings::connection_key];

  response[strings::msg_params][strings::success] = false;
  response[strings::msg_params][strings::result_code] =
      NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED;


  response[strings::params][strings::message_type] = MessageType::kResponse;

  ApplicationImpl* app = static_cast<ApplicationImpl*>(ApplicationManagerImpl::GetInstance()->application((*message_)[strings::params][strings::app_id]));
  if (0 != app) {
    LOG4CXX_ERROR_EXT(logger_, "No application associated with the registry item with session key ");
    response[strings::params]["resultCode"] = "APPLICATION_NOT_REGISTERED";
    response[strings::params]["success"] = "false";

    return;
  }

  //response
  NsSmartDeviceLink::NsSmartObjects::CSmartObject obj = app->hmi_level();
  if(NsSmartDeviceLinkRPCV2::HMILevel::HMI_NONE == obj.asInt())
  {
      LOG4CXX_WARN(logger_, "An application " << app->name() << " has not been activated yet!" );
      response[strings::params]["resultCode"] = "REJECTED";
      response[strings::params]["success"] = "false";
      return;
  }

  const int corellationId = (*message_)[strings::params][strings::correlation_id];
  const int connectionKey = (*message_)[strings::params][strings::connection_key];
  //ApplicationManagerImpl::GetInstance()->AddMessageChain(new MessageChaining(connectionKey, corellationId), connectionKey, corellationId);


  //senMestoHMI




  //ApplicationManagerImpl::GetInstance()->SendMessageToHMI((*message_));
}

}  // namespace commands

}  // namespace application_manager
