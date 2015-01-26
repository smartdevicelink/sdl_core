/*
 * Copyright (c) 2014, Ford Motor Company
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

#include <string.h>

#include "utils/macro.h"
#include "application_manager/mobile_message_handler.h"
#include "protocol_handler/protocol_payload.h"
#include "protocol_handler/protocol_packet.h"
#include "utils/bitstream.h"
#include "utils/logger.h"

#include <stdint.h>
#include <memory>

namespace {
const uint8_t kRequest = 0x0;
const uint8_t kResponse = 0x1;
const uint8_t kNotification = 0x2;
const uint8_t kUnknown = 0xF;
}

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "MobileMessageHandler")


application_manager::Message*
MobileMessageHandler::HandleIncomingMessageProtocolV1(
  const ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandler HandleIncomingMessageProtocolV1()");
  application_manager::Message* outgoing_message =
    new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      message->service_type())
  );
  if (!message) {
    NOTREACHED();
    return NULL;
  }

  outgoing_message->set_connection_key(message->connection_key());
  outgoing_message->set_protocol_version(
    static_cast<application_manager::ProtocolVersion>(message
        ->protocol_version()));
  outgoing_message->set_json_message(
    std::string(reinterpret_cast<const char*>(message->data()),
                message->data_size()));

  if (outgoing_message->json_message().empty()) {
    return NULL;
  }

  return outgoing_message;
}

application_manager::Message*
MobileMessageHandler::HandleIncomingMessageProtocolV2(
  const ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandler HandleIncomingMessageProtocolV2()");

  utils::BitStream message_bytestream(message->data(), message->data_size());
  protocol_handler::ProtocolPayloadV2 payload;
  protocol_handler::Extract(&message_bytestream, &payload,
                            message->data_size());

  // Silently drop message if it wasn't parsed correctly
  if (message_bytestream.IsBad()) {
    LOG4CXX_WARN(logger_,
                 "Drop ill-formed message from mobile, partially parsed: "
                 << payload);
    return NULL;
  }

  std::auto_ptr<application_manager::Message> outgoing_message(
      new application_manager::Message(
          protocol_handler::MessagePriority::FromServiceType(
              message->service_type())));

  outgoing_message->set_json_message(payload.json);
  outgoing_message->set_function_id(payload.header.rpc_function_id);
  outgoing_message->set_message_type(
      MessageTypeFromRpcType(payload.header.rpc_type));
  outgoing_message->set_correlation_id(int32_t(payload.header.corellation_id));
  outgoing_message->set_connection_key(message->connection_key());
  outgoing_message->set_protocol_version(
    static_cast<application_manager::ProtocolVersion>(message
        ->protocol_version()));
  outgoing_message->set_data_size(message->data_size());
  outgoing_message->set_payload_size(message->payload_size());

  if (!payload.data.empty()) {
    outgoing_message->set_binary_data(
        new application_manager::BinaryData(payload.data));
  }
  return outgoing_message.release();
}

protocol_handler::RawMessage*
MobileMessageHandler::HandleOutgoingMessageProtocolV1(
  const MobileMessage& message) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandler HandleOutgoingMessageProtocolV1()");
  std::string messageString = message->json_message();
  if (messageString.length() == 0) {
    LOG4CXX_INFO(logger_,
                 "Drop ill-formed message from mobile");
    return NULL;
  }

  uint8_t* rawMessage = new uint8_t[messageString.length() + 1];
  memcpy(rawMessage, messageString.c_str(), messageString.length() + 1);

  protocol_handler::RawMessage* result = new protocol_handler::RawMessage(
    message->connection_key(), 1, rawMessage, messageString.length() + 1);

  return result;
}

protocol_handler::RawMessage*
MobileMessageHandler::HandleOutgoingMessageProtocolV2(
  const MobileMessage& message) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandler HandleOutgoingMessageProtocolV2()");
  if (message->json_message().length() == 0) {
    LOG4CXX_ERROR(logger_, "json string is empty.");
  }
  uint32_t jsonSize = message->json_message().length();
  uint32_t binarySize = 0;
  if (message->has_binary_data()) {
    binarySize = message->binary_data()->size();
  }

  const size_t dataForSendingSize =
      protocol_handler::PROTOCOL_HEADER_V2_SIZE + jsonSize + binarySize;
  uint8_t* dataForSending = new uint8_t[dataForSendingSize];
  uint8_t offset = 0;

  uint8_t rpcTypeFlag = 0;
  switch (message->type()) {
    case application_manager::kRequest:
      rpcTypeFlag = kRequest;
      break;
    case application_manager::kResponse:
      rpcTypeFlag = kResponse;
      break;
    case application_manager::kNotification:
      rpcTypeFlag = kNotification;
      break;
    default:
      NOTREACHED();
      break;
  }

  uint32_t functionId = message->function_id();
  dataForSending[offset++] = ((rpcTypeFlag << 4) & 0xF0) | (functionId >> 24);
  dataForSending[offset++] = functionId >> 16;
  dataForSending[offset++] = functionId >> 8;
  dataForSending[offset++] = functionId;

  uint32_t correlationId = message->correlation_id();
  dataForSending[offset++] = correlationId >> 24;
  dataForSending[offset++] = correlationId >> 16;
  dataForSending[offset++] = correlationId >> 8;
  dataForSending[offset++] = correlationId;

  dataForSending[offset++] = jsonSize >> 24;
  dataForSending[offset++] = jsonSize >> 16;
  dataForSending[offset++] = jsonSize >> 8;
  dataForSending[offset++] = jsonSize;

  memcpy(dataForSending + offset, message->json_message().c_str(), jsonSize);

  if (message->has_binary_data()) {
    const std::vector<uint8_t>& binaryData = *(message->binary_data());
    uint8_t* currentPointer = dataForSending + offset + jsonSize;
    for (uint32_t i = 0; i < binarySize; ++i) {
      currentPointer[i] = binaryData[i];
    }
  }

  protocol_handler::RawMessage* msgToProtocolHandler =
    new protocol_handler::RawMessage(message->connection_key(),
                                     message->protocol_version(),
                                     dataForSending,
                                     dataForSendingSize);

  return msgToProtocolHandler;
}
}  // namespace application_manager
