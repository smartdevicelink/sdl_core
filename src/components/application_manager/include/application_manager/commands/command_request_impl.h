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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_

#include "application_manager/commands/command_impl.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class SmartObject;
}
}


namespace application_manager {

class MessageChaining;

namespace commands {

namespace NsSmart = NsSmartDeviceLink::NsSmartObjects;

class CommandRequestImpl : public CommandImpl {
 public:
  explicit CommandRequestImpl(const MessageSharedPtr& message);
  virtual ~CommandRequestImpl();
  virtual bool Init();
  virtual bool CleanUp();
  virtual void Run();
  void SendResponse(const bool success,
                    const mobile_apis::Result::eType& result_code,
                    const char* info = NULL,
                    const NsSmart::SmartObject* response_params = NULL) const;
  void CreateHMIRequest(const hmi_apis::FunctionID::eType& function_id,
                   const NsSmart::SmartObject& msg_params,
                   bool chaining_required = false);
  void CreateHMINotification(const hmi_apis::FunctionID::eType& function_id,
                   const NsSmart::SmartObject& msg_params) const;
 protected:
  MessageChaining*  msg_chaining_;
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_
