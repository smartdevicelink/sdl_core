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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RPC_HANDLER_IMPL_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RPC_HANDLER_IMPL_H

#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"
#include "application_manager/rpc_handler.h"
#include "application_manager/rpc_service.h"
#include "application_manager/mobile_message_handler.h"
#include "application_manager/policies/policy_handler_observer.h"

#include "protocol_handler/protocol_observer.h"
#include "hmi_message_handler/hmi_message_observer.h"
#include "hmi_message_handler/hmi_message_sender.h"

#include "formatters/formatter_json_rpc.h"
#include "formatters/CFormatterJsonSDLRPCv2.h"
#include "formatters/CFormatterJsonSDLRPCv1.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"
#ifdef TELEMETRY_MONITOR
#include "telemetry_observer.h"
#endif  // TELEMETRY_MONITOR

#include "interfaces/v4_protocol_v1_2_no_extra.h"
#include "interfaces/v4_protocol_v1_2_no_extra_schema.h"

#include "utils/threads/message_loop_thread.h"
#include "utils/semantic_version.h"

namespace application_manager {
namespace rpc_handler {

namespace impl {
struct MessageFromMobile : public std::shared_ptr<Message> {
  MessageFromMobile() {}
  explicit MessageFromMobile(const std::shared_ptr<Message>& message)
      : std::shared_ptr<Message>(message) {}
  // PrioritizedQueue requires this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
};
struct MessageFromHmi : public std::shared_ptr<Message> {
  MessageFromHmi() {}
  explicit MessageFromHmi(const std::shared_ptr<Message>& message)
      : std::shared_ptr<Message>(message) {}
  // PrioritizedQueue requires this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
};

// Short type names for prioritized message queues
typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageFromMobile> >
    FromMobileQueue;
typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageFromHmi> >
    FromHmiQueue;
}

class RPCHandlerImpl : public RPCHandler,
                       public impl::FromMobileQueue::Handler,
                       public impl::FromHmiQueue::Handler {
 public:
  RPCHandlerImpl(ApplicationManager& app_manager);
  ~RPCHandlerImpl();

  // CALLED ON messages_from_mobile_ thread!
  void Handle(const impl::MessageFromMobile message) OVERRIDE;
  // CALLED ON messages_from_hmi_ thread!
  void Handle(const impl::MessageFromHmi message) OVERRIDE;

  // Overriden HMIMessageObserver method
  /**
   * @brief OnMessageReceived overriden HMIMessageObserver method,
   * process message from HMI
   * @param message pointer to received message
   */
  void OnMessageReceived(
      hmi_message_handler::MessageSharedPointer message) OVERRIDE;

  /**
   * @brief OnErrorSending overriden HMIMessageObserver method
   * @param message pointer to received message
   */
  void OnErrorSending(
      hmi_message_handler::MessageSharedPointer message) OVERRIDE;

  // Overriden ProtocolObserver method
  /**
   * @brief OnMessageReceived overriden ProtocolObserver method,
   * process message from mobile
   * @param message pointer to received message
   */
  void OnMessageReceived(
      const ::protocol_handler::RawMessagePtr message) OVERRIDE;

  /**
   * @brief OnMobileMessageSent overriden ProtocolObserver method,
   * post message to mobile
   * @param message pointer to received message
   */
  void OnMobileMessageSent(
      const ::protocol_handler::RawMessagePtr message) OVERRIDE;

#ifdef TELEMETRY_MONITOR
  /**
   * @brief Setup observer for time metric.
   *
   * @param observer - pointer to observer
   */
  void SetTelemetryObserver(AMTelemetryObserver* observer) OVERRIDE;
#endif  // TELEMETRY_MONITOR

  /**
   * @brief Extracts and validates the syncMsgVersion included in
   * a RegisterAppInterfaceRequest
   *
   * @param output - SmartObject Message received from mobile
   * @param messageVersion - message version to be updated
   */
  void GetMessageVersion(
      ns_smart_device_link::ns_smart_objects::SmartObject& output,
      utils::SemanticVersion& message_version);

 private:
  void ProcessMessageFromMobile(const std::shared_ptr<Message> message);
  void ProcessMessageFromHMI(const std::shared_ptr<Message> message);
  bool ConvertMessageToSO(const Message& message,
                          smart_objects::SmartObject& output);
  std::shared_ptr<Message> ConvertRawMsgToMessage(
      const ::protocol_handler::RawMessagePtr message);
  hmi_apis::HMI_API& hmi_so_factory();
  mobile_apis::MOBILE_API& mobile_so_factory();

  ApplicationManager& app_manager_;
  // Thread that pumps messages coming from mobile side.
  impl::FromMobileQueue messages_from_mobile_;
  // Thread that pumps messages coming from HMI.
  impl::FromHmiQueue messages_from_hmi_;

  hmi_apis::HMI_API hmi_so_factory_;
  mobile_apis::MOBILE_API mobile_so_factory_;
#ifdef TELEMETRY_MONITOR
  AMTelemetryObserver* metric_observer_;
#endif  // TELEMETRY_MONITOR
};

}  // namespace rpc_handler
}  // namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RPC_HANDLER_IMPL_H
