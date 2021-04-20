/*
 * Copyright (c) 2018, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_RPC_SERVICE_H
#define SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_RPC_SERVICE_H

#include "application_manager/commands/command.h"
#include "application_manager/message.h"
#include "hmi_message_handler/hmi_message_handler.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"
#include "protocol_handler/protocol_handler.h"
#include "smart_objects/object_schema_item.h"

namespace application_manager {
namespace rpc_service {

using ns_smart_device_link::ns_smart_objects::SMember;

class RPCService {
 public:
  virtual ~RPCService() {}

  /**
   * @brief ManageMobileCommand convert message to mobile command and execute it
   * @param message pointer to received message
   * @param source source of command
   * @param warning_info warning message to send on a successful response. Only
   * applies to requests from mobile.
   * @return true if command is executed, otherwise return false
   */
  virtual bool ManageMobileCommand(const commands::MessageSharedPtr message,
                                   commands::Command::CommandSource source) = 0;
  virtual bool ManageMobileCommand(const commands::MessageSharedPtr message,
                                   commands::Command::CommandSource source,
                                   const std::string warning_info) = 0;

  /**
   * @brief ManageHMICommand convert message to HMI command and execute it
   * @param message pointer to received message
   * @param source source of command
   * @param warning_info warning message to send on a successful response. Only
   * applies to requests from HMI.
   * @return true if command is executed, otherwise return false
   */
  virtual bool ManageHMICommand(const commands::MessageSharedPtr message,
                                commands::Command::CommandSource source =
                                    commands::Command::SOURCE_HMI) = 0;
  virtual bool ManageHMICommand(const commands::MessageSharedPtr message,
                                commands::Command::CommandSource source,
                                const std::string warning_info) = 0;

  /**
   * @brief SendMessageToMobile Put message to the queue to be sent to mobile.
   * @param message pointer to message to send
   * @param final_message if true - connection to mobile will be closed
   * after processing this message
   */
  virtual void SendMessageToMobile(const commands::MessageSharedPtr message,
                                   bool final_message = false) = 0;
  /**
   * @brief SendMessageToHMI Put message to the queue to be sent to HMI.
   * @param message pointer to message to send
   */
  virtual void SendMessageToHMI(const commands::MessageSharedPtr message) = 0;

  /**
   * @brief Check if RPC with function_id can be handled by app services(related
   * to app services or handled by app services plugin)
   * @param function_id RPC function id
   * @param source RPC command source
   * @return true if App Services can handle RPC
   */
  virtual bool IsAppServiceRPC(int32_t function_id,
                               commands::Command::CommandSource source) = 0;

  virtual void UpdateMobileRPCParams(
      const mobile_apis::FunctionID::eType& function_id,
      const mobile_apis::messageType::eType& message_type,
      const std::map<std::string, SMember>& members) = 0;

  virtual void UpdateHMIRPCParams(
      const hmi_apis::FunctionID::eType& function_id,
      const hmi_apis::messageType::eType& message_type,
      const std::map<std::string, SMember>& members) = 0;

  /**
   * @brief Stop RPC service by shutting down hmi and mobile message queues
   */
  virtual void Stop() = 0;

  /**
   * @brief set_protocol_handler
   * @param handler
   * set protocol handler
   */
  virtual void set_protocol_handler(
      protocol_handler::ProtocolHandler* handler) = 0;
  virtual void set_hmi_message_handler(
      hmi_message_handler::HMIMessageHandler* handler) = 0;
};
}  // namespace rpc_service
}  // namespace application_manager

#endif  // SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_RPC_SERVICE_H
