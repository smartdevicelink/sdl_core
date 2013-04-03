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

#include "JSONHandler/incoming_thread_impl.h"
#include "LoggerHelper.hpp"

namespace json_handler {
log4cplus::Logger IncomingThreadImpl::logger_ = log4cplus::Logger::getInstance(
    LOG4CPLUS_TEXT("JSONHandler"));

IncomingThreadImpl::IncomingThreadImpl(JSONHandler* handler)
    : handler_(handler) {
  DCHECK(handler_);
}

IncomingThreadImpl::~IncomingThreadImpl() {
}

void IncomingThreadImpl::threadMain() {
  while (1) {
    while (!handler_->mIncomingMessages.empty()) {
      LOG4CPLUS_INFO(logger_, "Incoming mobile message received.");
      const protocol_handler::RawMessage* message = handler_
          ->mIncomingMessages.pop();

      NsSmartDeviceLinkRPC::SDLRPCMessage* currentMessage = 0;

      LOG4CPLUS_INFO_EXT(
          logger_,
          "Message of protocol version " << message->protocol_version());

      if (message->protocol_version() == 1) {
        currentMessage = handler_->handleIncomingMessageProtocolV1(message);
      } else if (message->protocol_version() == 2) {
        currentMessage = handler_->handleIncomingMessageProtocolV2(message);
      } else {
        LOG4CPLUS_WARN(logger_, "Message of wrong protocol version received.");
        continue;
      }

      if (!currentMessage) {
        LOG4CPLUS_ERROR(logger_, "Invalid mobile message received.");
        continue;
      }

      currentMessage->setProtocolVersion(message->protocol_version());

      if (!handler_->mMessagesObserver) {
        LOG4CPLUS_ERROR(
            logger_,
            "Cannot handle mobile message: MessageObserver doesn't exist.");
        return;
      }

      handler_->mMessagesObserver->onMessageReceivedCallback(
          currentMessage, message->connection_key());

      LOG4CPLUS_INFO(logger_, "Incoming mobile message handled.");
    }
    handler_->mIncomingMessages.wait();
  }
}
}
