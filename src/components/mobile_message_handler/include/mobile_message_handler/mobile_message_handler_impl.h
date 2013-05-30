/**
 * Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_MOBILE_MESSAGE_HANDLER_INCLUDE_MOBILE_MESSAGE_HANDLER_MOBILE_MESSAGE_HANDLER_IMPL_H_
#define SRC_COMPONENTS_MOBILE_MESSAGE_HANDLER_INCLUDE_MOBILE_MESSAGE_HANDLER_MOBILE_MESSAGE_HANDLER_IMPL_H_

#include <vector>

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/message_queue.h"
#include "utils/threads/thread.h"
#include "utils/shared_ptr.h"

#include "protocol_handler/raw_message.h"
#include "protocol_handler/protocol_handler.h"
#include "protocol_handler/protocol_observer.h"

#include "mobile_message_handler/mobile_message_handler.h"
#include "mobile_message_handler/messages_from_mobile_app_handler.h"

namespace mobile_message_handler {
class MobileMessageHandlerImpl : public MobileMessageHandler,
    public protocol_handler::ProtocolObserver {
 public:
  static MobileMessageHandlerImpl* instance();

  void setProtocolHandler(protocol_handler::ProtocolHandler* protocolHandler);
  void onMessageReceived(const protocol_handler::RawMessage* message);
  void SendMessageToMobileApp(const MobileMessage& message);

  void AddMobileMessageListener(MobileMessageObserver* listener);
  void RemoveMobileMessageListener(MobileMessageObserver* listener);

 private:
  // TODO(AK): add message listener here.
  MobileMessageHandlerImpl();
  ~MobileMessageHandlerImpl();

  //! -------------------------------------------------------------------------

  application_manager::Message* HandleIncomingMessageProtocolV1(
      const protocol_handler::RawMessage* message);

  application_manager::Message* HandleIncomingMessageProtocolV2(
      const protocol_handler::RawMessage* message);

  //! -------------------------------------------------------------------------

  protocol_handler::RawMessage* HandleOutgoingMessageProtocolV1(
      const application_manager::Message* message);

  protocol_handler::RawMessage* HandleOutgoingMessageProtocolV2(
      const application_manager::Message* message);

  //! -------------------------------------------------------------------------

  protocol_handler::ProtocolHandler* protocol_handler_;

  MessageQueue<const protocol_handler::RawMessage*> messages_from_mobile_app_;
  MessageQueue<application_manager::Message*> messages_to_mobile_app_;

  std::vector<MobileMessageObserver*> mobile_message_listeners_;

  // Thread for handling messages from Mobile.
  threads::Thread* handle_messages_from_mobile_app_;
  friend class MessagesFromMobileAppHandler;

  // Thread for sending messages to Mobile.
  threads::Thread* handle_messages_to_mobile_app_;
  friend class MessagesToMobileAppHandler;


  static MobileMessageHandlerImpl* self_;
  static log4cxx::LoggerPtr logger_;

  DISALLOW_COPY_AND_ASSIGN(MobileMessageHandlerImpl);
};
}  // namespace mobile_message_handler

#endif  // SRC_COMPONENTS_MOBILE_MESSAGE_HANDLER_INCLUDE_MOBILE_MESSAGE_HANDLER_MOBILE_MESSAGE_HANDLER_IMPL_H_
