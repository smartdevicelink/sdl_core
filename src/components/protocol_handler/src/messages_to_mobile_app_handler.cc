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

#include "protocol_handler/messages_to_mobile_app_handler.h"
#include "protocol_handler/session_observer.h"

namespace protocol_handler {
log4cxx::LoggerPtr MessagesToMobileAppHandler::logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("ProtocolHandler"));

MessagesToMobileAppHandler::MessagesToMobileAppHandler(
  ProtocolHandlerImpl* handler)
  : handler_(handler) {
  CHECK(handler_);
}

MessagesToMobileAppHandler::~MessagesToMobileAppHandler() {
}

void MessagesToMobileAppHandler::threadMain() {
  while (1) {
    while (!handler_->messages_to_mobile_app_.empty()) {
      const transport_manager::RawMessageSptr& message = handler_->messages_to_mobile_app_
                                      .pop();
      LOG4CXX_INFO_EXT(
        logger_,
        "Message to mobile app: connection " << message->connection_key()
        << "; dataSize: " << message->data_size()
        << " ; protocolVersion " << message->protocol_version());

      unsigned int maxDataSize = 0;
      if (PROTOCOL_VERSION_1 == message->protocol_version()) {
        maxDataSize = MAXIMUM_FRAME_DATA_SIZE - PROTOCOL_HEADER_V1_SIZE;
      } else if (PROTOCOL_VERSION_2 == message->protocol_version()) {
        maxDataSize = MAXIMUM_FRAME_DATA_SIZE - PROTOCOL_HEADER_V2_SIZE;
      }

      if (!handler_->session_observer_) {
        LOG4CXX_ERROR(
          logger_,
          "Cannot handle message to mobile app:"
          << " ISessionObserver doesn't exist.");
        return;
      }
      unsigned int connectionHandle = 0;
      unsigned char sessionID = 0;
      handler_->session_observer_->PairFromKey(message->connection_key(),
          &connectionHandle, &sessionID);

      if (message->data_size() <= maxDataSize) {
        RESULT_CODE result = handler_->SendSingleFrameMessage(
                               message, sessionID, message->protocol_version(),
                               SERVICE_TYPE_RPC,
                               message->data_size(), message->data(),
                               false);
        if (result != RESULT_OK) {
          LOG4CXX_ERROR(
            logger_, "ProtocolHandler failed to send single frame message.");
        }
      } else {
        LOG4CXX_INFO_EXT(
          logger_,
          "Message will be sent in multiple frames; max size is "
          << maxDataSize);

        RESULT_CODE result = handler_->SendMultiFrameMessage(
                               message, sessionID, message->protocol_version(),
                               SERVICE_TYPE_RPC,
                               message->data_size(), message->data(), false,
                               maxDataSize);
        if (result != RESULT_OK) {
          LOG4CXX_ERROR(
            logger_, "ProtocolHandler failed to send multiframe messages.");
        }
      }
    }
    handler_->messages_to_mobile_app_.wait();
  }
}
}  // namespace protocol_handler
