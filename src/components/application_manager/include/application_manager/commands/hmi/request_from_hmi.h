/*
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_REQUEST_FROM_HMI_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_REQUEST_FROM_HMI_H_

#include "application_manager/commands/command_impl.h"
#include "interfaces/HMI_API.h"
#include "smart_objects/smart_object.h"

namespace application_manager {

namespace commands {

namespace NsSmart = NsSmartDeviceLink::NsSmartObjects;

class RequestFromHMI : public CommandImpl, public event_engine::EventObserver {
 public:
  RequestFromHMI(const MessageSharedPtr& message,
                 ApplicationManager& application_manager);
  virtual ~RequestFromHMI();
  virtual bool Init();
  virtual bool CleanUp();
  virtual void Run();
  virtual void on_event(const event_engine::Event& event);
  /**
   * @brief SendResponse allows to send response to hmi
   * @param correlation_id the correlation id for the rfesponse.
   * @param function_id the function id for which response will be sent
   * @param result_code the result code.
   */
  void SendResponse(const bool success,
                    const uint32_t correlation_id,
                    const hmi_apis::FunctionID::eType function_id,
                    const hmi_apis::Common_Result::eType result_code);

  void FillCommonParametersOfSO(smart_objects::SmartObject* message,
                                uint32_t correlation_id,
                                hmi_apis::FunctionID::eType function_id);

 private:
  DISALLOW_COPY_AND_ASSIGN(RequestFromHMI);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_REQUEST_FROM_HMI_H_
