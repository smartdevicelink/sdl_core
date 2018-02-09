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
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RPC_SERVICE_H

#include "application_manager/application_manager.h"
#include "application_manager/rpc_service.h"
#include "application_manager/request_controller.h"
#include "application_manager/message_helper.h"
#include "application_manager/usage_statistics.h"
#include "application_manager/mobile_message_handler.h"
#include "application_manager/command_holder_impl.h"

#include "formatters/formatter_json_rpc.h"
#include "formatters/CFormatterJsonSDLRPCv2.h"
#include "formatters/CFormatterJsonSDLRPCv1.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"

#include "interfaces/v4_protocol_v1_2_no_extra.h"
#include "interfaces/v4_protocol_v1_2_no_extra_schema.h"

#include "utils/threads/message_loop_thread.h"
#include "utils/logger.h"

namespace application_manager {
namespace rpc_service {

namespace impl {

struct MessageToMobile : public utils::SharedPtr<Message> {
  MessageToMobile() : is_final(false) {}
  explicit MessageToMobile(const utils::SharedPtr<Message>& message,
                           bool final_message)
      : utils::SharedPtr<Message>(message), is_final(final_message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
  // Signals if connection to mobile must be closed after sending this message
  bool is_final;
};

struct MessageToHmi : public utils::SharedPtr<Message> {
  MessageToHmi() {}
  explicit MessageToHmi(const utils::SharedPtr<Message>& message)
      : utils::SharedPtr<Message>(message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
};

typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageToMobile> >
    ToMobileQueue;
typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageToHmi> >
    ToHmiQueue;
}

class RPCServiceImpl : public RPCService,
                       public impl::ToMobileQueue::Handler,
                       public impl::ToHmiQueue::Handler {
 public:
  RPCServiceImpl(ApplicationManager& app_manager,
                 request_controller::RequestController& request_ctrl,
                 protocol_handler::ProtocolHandler* protocol_handler,
                 hmi_message_handler::HMIMessageHandler* hmi_handler,
                 CommandHolder& commands_holder);
  ~RPCServiceImpl();

  bool ManageMobileCommand(const commands::MessageSharedPtr message,
                           commands::Command::CommandSource source) OVERRIDE;
  bool ManageHMICommand(const commands::MessageSharedPtr message) OVERRIDE;

  // CALLED ON messages_to_hmi_ thread!
  void Handle(const impl::MessageToHmi message) OVERRIDE;
  // CALLED ON messages_to_mobile_ thread!
  void Handle(const impl::MessageToMobile message) OVERRIDE;

  void SendMessageToMobile(const commands::MessageSharedPtr message,
                           bool final_message = false) OVERRIDE;
  void SendMessageToHMI(const commands::MessageSharedPtr message) OVERRIDE;

  void set_protocol_handler(
      protocol_handler::ProtocolHandler* handler) OVERRIDE;
  void set_hmi_message_handler(
      hmi_message_handler::HMIMessageHandler* handler) OVERRIDE;

#ifdef SDL_REMOTE_CONTROL
  void SendPostMessageToMobile(const MessagePtr& message) OVERRIDE;
  void SendPostMessageToHMI(const MessagePtr& message) OVERRIDE;
#endif  // SDL_REMOTE_CONTROL

 private:
  bool ConvertSOtoMessage(const smart_objects::SmartObject& message,
                          Message& output);
  hmi_apis::HMI_API& hmi_so_factory();
  mobile_apis::MOBILE_API& mobile_so_factory();

  ApplicationManager& app_manager_;
  request_controller::RequestController& request_ctrl_;
  protocol_handler::ProtocolHandler* protocol_handler_;
  hmi_message_handler::HMIMessageHandler* hmi_handler_;
  CommandHolder& commands_holder_;
  // Thread that pumps messages being passed to mobile side.
  impl::ToMobileQueue messages_to_mobile_;
  // Thread that pumps messages being passed to HMI.
  impl::ToHmiQueue messages_to_hmi_;

  hmi_apis::HMI_API hmi_so_factory_;
  mobile_apis::MOBILE_API mobile_so_factory_;
};
}  // namespace rpc_service
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RPC_SERVICE_IMPL_H
