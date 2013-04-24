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

#include "mobile_message_handler/messages_from_mobile_app_handler.h"
#include "mobile_message_handler/mobile_message_handler_impl.h"
#include "application_manager/message.h"
#include "LoggerHelper.hpp"

namespace mobile_message_handler {

MessagesFromMobileAppHandler::MessagesFromMobileAppHandler() {
}

MessagesFromMobileAppHandler::~MessagesFromMobileAppHandler() {
}

void MessagesFromMobileAppHandler::threadMain() {
  MobileMessageHandlerImpl* handler = MobileMessageHandlerImpl::getInstance();

  while (1) {
    while (!handler->messages_from_mobile_app_.empty()) {
      const protocol_handler::RawMessage* message = handler
          ->messages_from_mobile_app_.pop();

      application_manager::Message* outgoing_message =
          new application_manager::Message;
      if (message->protocol_version() == 1) {
        outgoing_message = handler->handleIncomingMessageProtocolV1(message);
      } else if (message->protocol_version() == 2) {
        outgoing_message = handler->handleIncomingMessageProtocolV2(message);
      } else {
        continue;
      }

      if (!outgoing_message) {
        continue;
      }

      // TODO(akandul): send message to application manager here.
      // handler->mMessagesObserver->onMessageReceivedCallback(outgoing_message);
    }
    handler->messages_from_mobile_app_.wait();
  }
}
}  // // namespace mobile_message_handler
