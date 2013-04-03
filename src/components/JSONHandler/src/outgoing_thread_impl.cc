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

#include "JSONHandler/outgoing_thread_impl.h"
#include "JSONHandler/SDLRPCMessage.h"

#include "protocol_handler/raw_message.h"
#include "LoggerHelper.hpp"

namespace json_handler {
log4cplus::Logger OutgoingThreadImpl::logger_ = log4cplus::Logger::getInstance(
    LOG4CPLUS_TEXT("JSONHandler"));

OutgoingThreadImpl::OutgoingThreadImpl(JSONHandler* handler)
    : handler_(handler) {
  DCHECK(handler_);
}

OutgoingThreadImpl::~OutgoingThreadImpl() {
}

void OutgoingThreadImpl::threadMain() {
  while (1) {
    while (!handler_->mOutgoingMessages.empty()) {
      std::pair<int, const NsSmartDeviceLinkRPC::SDLRPCMessage*> messagePair =
          handler_->mOutgoingMessages.pop();
      const NsSmartDeviceLinkRPC::SDLRPCMessage* message = messagePair.second;
      LOG4CPLUS_INFO(
          logger_,
          "Outgoing mobile message " << message->getMethodId() << " received.");

      protocol_handler::RawMessage* msgToProtocolHandler = 0;
      if (message->getProtocolVersion() == 1) {
        msgToProtocolHandler = handler_->handleOutgoingMessageProtocolV1(
            messagePair.first, message);
      } else if (message->getProtocolVersion() == 2) {
        LOG4CPLUS_INFO_EXT(
            logger_,
            "method id " << message->getMethodId()
              << "; message type " << message->getMessageType());

        msgToProtocolHandler = handler_->handleOutgoingMessageProtocolV2(
            messagePair.first, message);
      }

      if (!msgToProtocolHandler) {
        LOG4CPLUS_ERROR(logger_, "Failed to create message string.");
        continue;
      }

      if (!handler_->mProtocolHandler) {
        LOG4CPLUS_ERROR(
            logger_,
            "Cannot handle mobile message: ProtocolHandler doesn't exist.");
        return;
      }

      LOG4CPLUS_INFO_EXT(
          logger_,
          "Sending to ProtocolHandler: " << msgToProtocolHandler->data()
            << " of size " << msgToProtocolHandler->data_size());
      handler_->mProtocolHandler->sendData(msgToProtocolHandler);

      delete message;
      LOG4CPLUS_INFO(logger_, "Outgoing mobile message handled.");
    }
    handler_->mOutgoingMessages.wait();
  }
}
}
