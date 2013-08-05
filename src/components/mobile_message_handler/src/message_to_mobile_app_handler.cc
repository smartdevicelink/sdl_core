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

#include "mobile_message_handler/message_to_mobile_app_handler.h"
#include "mobile_message_handler/mobile_message_handler_impl.h"
#include "application_manager/message.h"

namespace mobile_message_handler {

MessagesToMobileAppHandler::MessagesToMobileAppHandler() {
}

MessagesToMobileAppHandler::~MessagesToMobileAppHandler() {
}

void MessagesToMobileAppHandler::threadMain() {
  MobileMessageHandlerImpl* handler = MobileMessageHandlerImpl::instance();

  while (1) {
    while (!handler->messages_to_mobile_app_.empty()) {
      MobileMessage message = handler->messages_to_mobile_app_
          .pop();

      protocol_handler::RawMessage* rawMessage = 0;
      if (message->protocol_version() == application_manager::kV1) {
        rawMessage = handler->HandleOutgoingMessageProtocolV1(message);
      } else if (message->protocol_version() == application_manager::kV2) {
        rawMessage = handler->HandleOutgoingMessageProtocolV2(message);
      } else {
        continue;
      }

      if (!handler->protocol_handler_) {
        return;
      }

      handler->protocol_handler_->sendMessageToMobileApp(rawMessage);
    }
    handler->messages_to_mobile_app_.wait();
  }
}
}  // namespace mobile_message_handler
