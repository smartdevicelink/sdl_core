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

#include "protocol_handler/message_from_mobile_app_handler.h"

namespace protocol_handler {
log4cxx::LoggerPtr MessagesFromMobileAppHandler::logger_ =
      log4cxx::LoggerPtr(log4cxx::Logger::getLogger( "ProtocolHandler"));

MessagesFromMobileAppHandler::MessagesFromMobileAppHandler(
    ProtocolHandlerImpl* handler)
    : handler_(handler) {
  CHECK(handler_);
}

MessagesFromMobileAppHandler::~MessagesFromMobileAppHandler() {
}

void MessagesFromMobileAppHandler::threadMain() {
  while (1) {
    while (!handler_->messages_from_mobile_app_.empty()) {
      const transport_manager::RawMessageSptr& message = handler_
                                      ->messages_from_mobile_app_.pop();
      LOG4CXX_INFO_EXT(
        logger_,
        "Message " << message->data() << " from mobile app received of size "
        << message->data_size());

      if ((0 != message->data()) && (0 != message->data_size())
          && (MAXIMUM_FRAME_DATA_SIZE + PROTOCOL_HEADER_V2_SIZE
              >= message->data_size())) {
        ProtocolPacket* packet = new ProtocolPacket;
        LOG4CXX_INFO_EXT(logger_, "Data: " << packet->data());
        if (packet->deserializePacket(message->data(), message->data_size())
            == RESULT_FAIL) {
          LOG4CXX_ERROR(logger_, "Failed to parse received message.");
          delete packet;
        } else {
          LOG4CXX_INFO_EXT(logger_,
                           "Packet: dataSize " << packet->data_size());
          handler_->HandleMessage(message, packet);
        }
      } else {
        LOG4CXX_WARN(
          logger_, "handleMessagesFromMobileApp() - incorrect or NULL data");
      }

    }
    handler_->messages_from_mobile_app_.wait();
  }
}
}  // namespace protocol_handler
