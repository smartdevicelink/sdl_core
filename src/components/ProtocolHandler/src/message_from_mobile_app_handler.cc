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

#include "ProtocolHandler/message_from_mobile_app_handler.h"
#include "LoggerHelper.hpp"

namespace NsProtocolHandler {
log4cplus::Logger MessagesFromMobileAppHandler::logger_ =
    log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ProtocolHandler"));

MessagesFromMobileAppHandler::MessagesFromMobileAppHandler(
    ProtocolHandler* handler)
    : handler_(handler) {
  DCHECK(handler_);
}

MessagesFromMobileAppHandler::~MessagesFromMobileAppHandler() {
}

void MessagesFromMobileAppHandler::threadMain() {
  while (1) {
    while (!handler_->mMessagesFromMobileApp.empty()) {
      ProtocolHandler::IncomingMessage* message = handler_
          ->mMessagesFromMobileApp.pop();
      LOG4CPLUS_INFO_EXT(
          logger_,
          "Message " << message->mData << " from mobile app received of size "
              << message->mDataSize);

      // TODO(PV): check for ConnectionHandle.
      // TODO(PV): check for data size - crash is possible.
      if ((0 != message->mData) && (0 != message->mDataSize)
          && (MAXIMUM_FRAME_DATA_SIZE + PROTOCOL_HEADER_V2_SIZE
              >= message->mDataSize)) {
        ProtocolPacket * packet = new ProtocolPacket;
        LOG4CPLUS_INFO_EXT(logger_, "Data: " << packet->getData());
        if (packet->deserializePacket(message->mData, message->mDataSize)
            == RESULT_FAIL) {
          LOG4CPLUS_ERROR(logger_, "Failed to parse received message.");
          delete packet;
        } else {
          LOG4CPLUS_INFO_EXT(logger_,
                             "Packet: dataSize " << packet->getDataSize());
          handler_->handleMessage(message->mConnectionHandle, packet);
        }
      } else {
        LOG4CPLUS_WARN(
            logger_, "handleMessagesFromMobileApp() - incorrect or NULL data");
      }

      delete message;
    }
    handler_->mMessagesFromMobileApp.wait();
  }
}
}  // namespace NsProtocolHandler
