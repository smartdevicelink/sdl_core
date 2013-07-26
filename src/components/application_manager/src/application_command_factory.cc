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

#include "application_manager/application_command_factory.h"
#include "utils/macro.h"
#include "application_manager/application_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/commands/mobile/unregister_app_interface_request.h"
#include "application_manager/commands/mobile/unregister_app_interface_response.h"

#include "interfaces/MOBILE_API.h"

namespace application_manager {

namespace function_ids = mobile_apis::FunctionID;

CommandSharedPtr CreateCommand(const MessageSharedPtr& message) {
  CommandSharedPtr command(NULL);
  DCHECK(message);
  if (!message) {
    return command;
  }

  smart_objects::SmartObject& message_data = *message;
  int function_id = message_data[strings::params][strings::function_id];

  int app_id = message_data[strings::params][strings::connection_key];

  Application* application = ApplicationManagerImpl::instance()->
                             application(app_id);

  if (!application) {
    return command;
  }

  switch (function_id) {
    case function_ids::UnregisterAppInterfaceID: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kRequest) {
        command.reset(new commands::UnregisterAppInterfaceRequest(message));
      } else {
        command.reset(
          new commands::UnregisterAppInterfaceResponse(message));
      }
      break;
    }
  }

  return command;
}

}  // namespace application_manager
