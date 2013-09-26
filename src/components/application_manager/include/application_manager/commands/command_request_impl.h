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

  /*
   * @brief Function is called by RequestController when request execution time
   * has exceed it's limit
   *
   */
  virtual void onTimeOut() const;

  /*
   * @brief Retrieves request ID
   */
  inline int function_id() const;

  /*
   * @brief Retrieves correlation ID
   */
  inline int correlation_id() const;

  /*
   * @brief Retrieves connection key
   */
  inline int connection_key() const;

  /*
   * @brief Creates Mobile response
   *
   * @param success true if successful; false, if failed
   * @param result_code Result code (SUCCESS, INVALID_DATA, e.t.c)
   * @param info Provides additional human readable info regarding the result
   * @param response_params Additional params in response
   */
  void SendResponse(const bool success,
                    const mobile_apis::Result::eType& result_code,
                    const char* info = NULL,
                    const NsSmart::SmartObject* response_params = NULL) const;

  /*
   * @brief Creates HMI request
   *
   * @param function_id HMI request ID
   * @param msg_params HMI request msg params
   * @param require_chaining Indicate if response from HMI is required
   * before sending response to Mobile
   * @param chaining counter Indicate amount of HMI responses should arrived
   * before sending response to Mobile
   *
   */
  void CreateHMIRequest(const hmi_apis::FunctionID::eType& function_id,
                        const NsSmart::SmartObject& msg_params,
                        bool require_chaining = false,
                        unsigned int chaining_counter = 0);

  /*
   * @brief Creates HMI request
   *
   * @param function_id HMI request ID
   * @param msg_params HMI request msg params
   */
  void CreateHMINotification(const hmi_apis::FunctionID::eType& function_id,
                             const NsSmart::SmartObject& msg_params) const;

 protected:
  MessageChaining* msg_chaining_;

 private:
  DISALLOW_COPY_AND_ASSIGN(CommandRequestImpl);
};

int CommandRequestImpl::function_id() const {
  return (*message_)[strings::params][strings::function_id].asInt();
}

int CommandRequestImpl::correlation_id() const {
  return (*message_)[strings::params][strings::correlation_id].asInt();
}

int CommandRequestImpl::connection_key() const {
  return (*message_)[strings::params][strings::connection_key].asInt();
}

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_
