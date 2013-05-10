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

#include <stddef.h>

#include "utils/macro.h"
#include "mobile_message_handler/mobile_message_handler_impl.h"

namespace {
const unsigned char kRequest = 0x0;
const unsigned char kResponse = 0x1;
const unsigned char kNotification = 0x2;
const unsigned char kUnknown = 0xF;
}

namespace mobile_message_handler {

MobileMessageHandlerImpl* MobileMessageHandlerImpl::self_ = NULL;
//! ----------------------------------------------------------------------------

MobileMessageHandlerImpl::MobileMessageHandlerImpl()
    : handle_messages_from_mobile_app_(NULL),
      protocol_handler_(NULL) {

  handle_messages_from_mobile_app_ = new threads::Thread(
      "MobileMessageHandler::MessagesFromMobileAppHandler",
      new MessagesFromMobileAppHandler());

  handle_messages_from_mobile_app_->startWithOptions(
      threads::ThreadOptions(threads::Thread::kMinStackSize));
}

MobileMessageHandlerImpl::~MobileMessageHandlerImpl() {
  handle_messages_from_mobile_app_->stop();
  delete handle_messages_from_mobile_app_;
  handle_messages_from_mobile_app_ = NULL;
}

MobileMessageHandlerImpl* MobileMessageHandlerImpl::getInstance() {
  if (NULL != MobileMessageHandlerImpl::self_) {
    MobileMessageHandlerImpl::self_ = new MobileMessageHandlerImpl;
  }

  return MobileMessageHandlerImpl::self_;
}

void MobileMessageHandlerImpl::setProtocolHandler(
    protocol_handler::ProtocolHandler* protocolHandler) {
  DCHECK(protocolHandler);
  protocol_handler_ = protocolHandler;
}

void MobileMessageHandlerImpl::onMessageReceived(
    const protocol_handler::RawMessage* message) {
  DCHECK(message);
}

bool MobileMessageHandlerImpl::sendMessageToMobileApp(
    const application_manager::Message* message) {
  DCHECK(message);

  return true;
}

application_manager::Message*
MobileMessageHandlerImpl::handleIncomingMessageProtocolV1(
    const protocol_handler::RawMessage* message) {
  application_manager::Message* outgoing_message =
      new application_manager::Message;
  if (!message) {
    NOTREACHED();

    // TODO(AK): check memory allocation here.
  }

  outgoing_message->set_json_message(
      std::string(reinterpret_cast<const char*>(message->data()),
                  message->data_size()));

  if (outgoing_message->json_message().empty()) {
    return NULL;
  }

  return outgoing_message;
}

application_manager::Message*
MobileMessageHandlerImpl::handleIncomingMessageProtocolV2(
    const protocol_handler::RawMessage* message) {
  application_manager::Message* outgoing_message =
      new application_manager::Message;
  if (!message) {
    NOTREACHED();

    // TODO(AK): check memory allocation here.
  }

  unsigned char* receivedData = message->data();
  unsigned char offset = 0;
  unsigned char firstByte = receivedData[offset++];

  int rpcType = -1;
  unsigned char rpcTypeFlag = firstByte >> 4u;
  switch (rpcTypeFlag) {
    case kRequest:
      rpcType = 0;
      break;
    case kResponse:
      rpcType = 1;
      break;
    case kNotification:
      rpcType = 2;
      break;
  }

  unsigned int functionId = firstByte >> 8u;

  functionId <<= 24u;
  functionId |= receivedData[offset++] << 16u;
  functionId |= receivedData[offset++] << 8u;
  functionId |= receivedData[offset++];

  unsigned int correlationId = receivedData[offset++] << 24u;
  correlationId |= receivedData[offset++] << 16u;
  correlationId |= receivedData[offset++] << 8u;
  correlationId |= receivedData[offset++];

  unsigned int jsonSize = receivedData[offset++] << 24u;
  jsonSize |= receivedData[offset++] << 16u;
  jsonSize |= receivedData[offset++] << 8u;
  jsonSize |= receivedData[offset++];

  if (jsonSize > message->data_size()) {
    delete outgoing_message;
    // Received invalid json packet header.
    return NULL;
  }

  std::string json_string = std::string(
      reinterpret_cast<const char*>(receivedData) + offset, jsonSize);

  if (functionId == 0 || rpcType == application_manager::kUnknownType
      || correlationId == 0 || message->connection_key() == 0
      || outgoing_message->json_message().empty()) {
    delete outgoing_message;
    // Invaled message constructed.
    return NULL;
  }

  outgoing_message->set_json_message(json_string);
  outgoing_message->set_function_id(functionId);
  outgoing_message->set_message_type(
      static_cast<application_manager::MessageType>(rpcType));
  outgoing_message->set_correlation_id(correlationId);
  outgoing_message->set_connection_key(message->connection_key());

  if (message->data_size() > (offset + jsonSize)) {
    application_manager::BinaryData* binaryData =
        new application_manager::BinaryData(
            receivedData + offset + jsonSize,
            receivedData + message->data_size());

    if (!binaryData) {
      delete outgoing_message;
      return NULL;
    }

    outgoing_message->set_binary_data(binaryData);
  }

  return outgoing_message;
}

}  // namespace mobile_message_handler
