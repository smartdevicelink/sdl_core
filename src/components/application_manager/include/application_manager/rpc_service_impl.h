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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RPC_SERVICE_IMPL_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RPC_SERVICE_IMPL_H

#include "application_manager/application_manager.h"
#include "application_manager/command_holder_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/mobile_message_handler.h"
#include "application_manager/request_controller.h"
#include "application_manager/rpc_protection_manager.h"
#include "application_manager/rpc_service.h"
#include "application_manager/usage_statistics.h"

#include "formatters/CFormatterJsonSDLRPCv1.h"
#include "formatters/CFormatterJsonSDLRPCv2.h"
#include "formatters/formatter_json_rpc.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"

#include "interfaces/v4_protocol_v1_2_no_extra.h"
#include "interfaces/v4_protocol_v1_2_no_extra_schema.h"

#include "utils/logger.h"
#include "utils/threads/message_loop_thread.h"

namespace application_manager {
namespace rpc_service {

namespace impl {

struct MessageToMobile : public std::shared_ptr<Message> {
  MessageToMobile() : is_final(false) {}
  explicit MessageToMobile(const std::shared_ptr<Message>& message,
                           bool final_message)
      : std::shared_ptr<Message>(message), is_final(final_message) {}
  // PrioritizedQueue requires this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
  // Signals if connection to mobile must be closed after sending this message
  bool is_final;
};

struct MessageToHmi : public std::shared_ptr<Message> {
  MessageToHmi() {}
  explicit MessageToHmi(const std::shared_ptr<Message>& message)
      : std::shared_ptr<Message>(message) {}
  // PrioritizedQueue requires this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
};

typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageToMobile> >
    ToMobileQueue;
typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageToHmi> >
    ToHmiQueue;
}  // namespace impl

typedef std::shared_ptr<RPCProtectionManager> RPCProtectionManagerSPtr;

enum class EncryptionFlagCheckResult {
  kSuccess_Protected,
  kSuccess_NotProtected,
  kError_EncryptionNeeded
};

class RPCServiceImpl : public RPCService,
                       public impl::ToMobileQueue::Handler,
                       public impl::ToHmiQueue::Handler {
 public:
  /**
   * @brief RPCServiceImpl class constructor
   * @param app_manager ApplicationManager
   * @param request_ctrl RequestController
   * @param protocol_handler ProtocolHandler
   * @param hmi_handler HMIMessageHandler
   * @param commands_holder CommandHolder
   **/
  RPCServiceImpl(ApplicationManager& app_manager,
                 request_controller::RequestController& request_ctrl,
                 protocol_handler::ProtocolHandler* protocol_handler,
                 hmi_message_handler::HMIMessageHandler* hmi_handler,
                 CommandHolder& commands_holder,
                 RPCProtectionManagerSPtr rpc_protection_manager,
                 hmi_apis::HMI_API& hmi_so_factory_,
                 mobile_apis::MOBILE_API& mobile_so_factory_);
  ~RPCServiceImpl();

  void Stop() OVERRIDE;

  bool ManageMobileCommand(const commands::MessageSharedPtr message,
                           commands::Command::CommandSource source) OVERRIDE;
  bool ManageMobileCommand(const commands::MessageSharedPtr message,
                           commands::Command::CommandSource source,
                           const std::string warning_info) OVERRIDE;
  bool ManageHMICommand(const commands::MessageSharedPtr message,
                        commands::Command::CommandSource source =
                            commands::Command::SOURCE_HMI) OVERRIDE;
  bool ManageHMICommand(const commands::MessageSharedPtr message,
                        commands::Command::CommandSource source,
                        const std::string warning_info) OVERRIDE;

  // CALLED ON messages_to_hmi_ thread!
  void Handle(const impl::MessageToHmi message) OVERRIDE;
  // CALLED ON messages_to_mobile_ thread!
  void Handle(const impl::MessageToMobile message) OVERRIDE;

  void SendMessageToMobile(const commands::MessageSharedPtr message,
                           bool final_message = false) OVERRIDE;
  void SendMessageToHMI(const commands::MessageSharedPtr message) OVERRIDE;

  bool IsAppServiceRPC(int32_t function_id,
                       commands::Command::CommandSource source) OVERRIDE;

  void set_protocol_handler(
      protocol_handler::ProtocolHandler* handler) OVERRIDE;
  void set_hmi_message_handler(
      hmi_message_handler::HMIMessageHandler* handler) OVERRIDE;

  void UpdateMobileRPCParams(
      const mobile_apis::FunctionID::eType& function_id,
      const mobile_apis::messageType::eType& message_type,
      const std::map<std::string, SMember>& members) OVERRIDE;
  void UpdateHMIRPCParams(
      const hmi_apis::FunctionID::eType& function_id,
      const hmi_apis::messageType::eType& message_type,
      const std::map<std::string, SMember>& members) OVERRIDE;

 private:
  bool ConvertSOtoMessage(const smart_objects::SmartObject& message,
                          Message& output,
                          const bool allow_unknown_parameters = false);

  EncryptionFlagCheckResult IsEncryptionRequired(
      const smart_objects::SmartObject& message,
      ApplicationSharedPtr app,
      const bool is_rpc_service_secure) const;
  hmi_apis::HMI_API& hmi_so_factory();
  mobile_apis::MOBILE_API& mobile_so_factory();
  void CheckSourceForUnsupportedRequest(
      const commands::MessageSharedPtr message,
      commands::Command::CommandSource source);

  ApplicationManager& app_manager_;
  request_controller::RequestController& request_ctrl_;
  protocol_handler::ProtocolHandler* protocol_handler_;
  hmi_message_handler::HMIMessageHandler* hmi_handler_;
  RPCProtectionManagerSPtr rpc_protection_manager_;
  CommandHolder& commands_holder_;
  // Thread that pumps messages being passed to mobile side.
  impl::ToMobileQueue messages_to_mobile_;
  // Thread that pumps messages being passed to HMI.
  impl::ToHmiQueue messages_to_hmi_;

  hmi_apis::HMI_API& hmi_so_factory_;
  mobile_apis::MOBILE_API& mobile_so_factory_;
};
}  // namespace rpc_service
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RPC_SERVICE_IMPL_H
