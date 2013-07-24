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

namespace mobile_message_handler {

log4cxx::LoggerPtr MessagesFromMobileAppHandler::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("MessagesFromMobileAppHandler"));

//! ---------------------------------------------------------------------------

MessagesFromMobileAppHandler::MessagesFromMobileAppHandler() {
  LOG4CXX_INFO(logger_, "MessagesFromMobileAppHandler ctor");
}

MessagesFromMobileAppHandler::~MessagesFromMobileAppHandler() {
  LOG4CXX_INFO(logger_, "MessagesFromMobileAppHandler dtor");
}

void MessagesFromMobileAppHandler::threadMain() {
  LOG4CXX_INFO(logger_, "MessagesFromMobileAppHandler threadMain()");
  MobileMessageHandlerImpl* handler = MobileMessageHandlerImpl::instance();

  while (1) {
    while (!handler->messages_from_mobile_app_.empty()) {
      protocol_handler::RawMessageSptr message = handler
          ->messages_from_mobile_app_.pop();

      application_manager::Message* outgoing_message =
        new application_manager::Message;
      // TODO(AK): change this
      protocol_handler::RawMessage* msg = message.get();
      if (message->protocol_version() == 1) {
        outgoing_message = handler->HandleIncomingMessageProtocolV1(msg);
      } else if (message->protocol_version() == 2) {
        outgoing_message = handler->HandleIncomingMessageProtocolV2(msg);
      } else {
        LOG4CXX_WARN(logger_, "Unknown protocol version.");
        continue;
      }

      if (!outgoing_message) {
        LOG4CXX_WARN(logger_, "Incorrect message received");
        continue;
      }

      LOG4CXX_INFO(logger_, "Sending message to listeners.");
      std::vector<MobileMessageObserver*>::const_iterator i = handler
          ->mobile_message_listeners_.begin();
      for (; i != handler->mobile_message_listeners_.end(); ++i) {
        (*i)->OnMobileMessageReceived(outgoing_message);
      }
      delete msg;
    }
    handler->messages_from_mobile_app_.wait();
  }
}
}  // namespace mobile_message_handler
