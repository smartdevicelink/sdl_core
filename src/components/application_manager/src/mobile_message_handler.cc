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
#include <string>

namespace application_manager {
using protocol_handler::Extract;

namespace {
typedef std::map<MessageType, std::string> MessageTypeMap;
#ifdef SDL_CPP11
MessageTypeMap message_types_map = {
    std::make_pair(kRequest, std::string("Request")),
    std::make_pair(kResponse, std::string("Response")),
    std::make_pair(kNotification, std::string("Notification"))};
#else
MessageTypeMap create_map() {
  MessageTypeMap message_types_map;
  message_types_map.insert(std::make_pair(kRequest, std::string("Request")));
  message_types_map.insert(std::make_pair(kResponse, std::string("Response")));
  message_types_map.insert(
      std::make_pair(kNotification, std::string("Notification")));
  return message_types_map;
}
MessageTypeMap message_types_map = create_map();
#endif  // SDL_CPP11

#if defined(ENABLE_LOG)
std::string GetMessageType(const MessageType message_type) {
  MessageTypeMap::const_iterator it = message_types_map.find(message_type);
  if (message_types_map.end() != it) {
    return (*it).second;
  }
  return std::string();
}
#endif

}  // namespace
SDL_CREATE_LOGGER("ApplicationManager")

application_manager::Message*
MobileMessageHandler::HandleIncomingMessageProtocol(
    const protocol_handler::RawMessagePtr message) {
  DCHECK_OR_RETURN(message, NULL);
  application_manager::Message* out_message = NULL;
  switch (message->protocol_version()) {
    case ProtocolVersion::kV1:
      SDL_DEBUG("Protocol version - V1");
      out_message =
          MobileMessageHandler::HandleIncomingMessageProtocolV1(message);
      break;
    case ProtocolVersion::kV2:
      SDL_DEBUG("Protocol version - V2");
      out_message =
          MobileMessageHandler::HandleIncomingMessageProtocolV2(message);
      break;
    case ProtocolVersion::kV3:
      SDL_DEBUG("Protocol version - V3");
      out_message =
          MobileMessageHandler::HandleIncomingMessageProtocolV2(message);
      break;
    case ProtocolVersion::kV4:
      SDL_DEBUG("Protocol version - V4");
      out_message =
          MobileMessageHandler::HandleIncomingMessageProtocolV2(message);
      break;
    default:
      SDL_WARN("Can't recognise protocol version");
      out_message = NULL;
      break;
  }
  if (out_message == NULL) {
    SDL_WARN("Message is NULL");
    return NULL;
  }
  SDL_DEBUG("Incoming RPC_INFO: " << (out_message->connection_key() >> 16)
                                  << ", " << GetMessageType(out_message->type())
                                  << ", " << out_message->function_id() << ", "
                                  << out_message->correlation_id() << ", "
                                  << out_message->json_message());
  return out_message;
}

protocol_handler::RawMessage*
MobileMessageHandler::HandleOutgoingMessageProtocol(
    const MobileMessage& message) {
  SDL_DEBUG("Outgoing RPC_INFO: " << (message->connection_key() >> 16) << ", "
                                  << GetMessageType(message->type()) << ", "
                                  << message->function_id() << ", "
                                  << message->correlation_id() << ", "
                                  << message->json_message());

  if (message->protocol_version() == application_manager::kV1) {
    return MobileMessageHandler::HandleOutgoingMessageProtocolV1(message);
  }
  if ((message->protocol_version() == application_manager::kV2) ||
      (message->protocol_version() == application_manager::kV3) ||
      (message->protocol_version() == application_manager::kV4)) {
    return MobileMessageHandler::HandleOutgoingMessageProtocolV2(message);
  }
  return NULL;
}

application_manager::Message*
MobileMessageHandler::HandleIncomingMessageProtocolV1(
    const ::protocol_handler::RawMessagePtr message) {
  SDL_AUTO_TRACE();
  application_manager::Message* outgoing_message =
      new application_manager::Message(
          protocol_handler::MessagePriority::FromServiceType(
              message->service_type()));
  if (!message) {
    NOTREACHED();
    return NULL;
  }

  outgoing_message->set_connection_key(message->connection_key());
  outgoing_message->set_protocol_version(
      static_cast<application_manager::ProtocolVersion>(
          message->protocol_version()));
  outgoing_message->set_json_message(std::string(
      reinterpret_cast<const char*>(message->data()), message->data_size()));

  if (outgoing_message->json_message().empty()) {
    delete outgoing_message;
    return NULL;
  }

  return outgoing_message;
}

application_manager::Message*
MobileMessageHandler::HandleIncomingMessageProtocolV2(
    const ::protocol_handler::RawMessagePtr message) {
  SDL_AUTO_TRACE();
  utils::BitStream message_bytestream(message->data(), message->data_size());
  protocol_handler::ProtocolPayloadV2 payload;
  protocol_handler::Extract(
      &message_bytestream, &payload, message->data_size());

  // Silently drop message if it wasn't parsed correctly
  if (message_bytestream.IsBad()) {
    SDL_WARN(
        "Drop ill-formed message from mobile, partially parsed: " << payload);
    return NULL;
  }

  std::unique_ptr<application_manager::Message> outgoing_message(
      new application_manager::Message(
          protocol_handler::MessagePriority::FromServiceType(
              message->service_type())));

  outgoing_message->set_json_message(payload.json);
  outgoing_message->set_function_id(payload.header.rpc_function_id);
  outgoing_message->set_message_type(
      MessageTypeFromRpcType(payload.header.rpc_type));
  outgoing_message->set_correlation_id(int32_t(payload.header.correlation_id));
  outgoing_message->set_connection_key(message->connection_key());
  outgoing_message->set_protocol_version(
      static_cast<application_manager::ProtocolVersion>(
          message->protocol_version()));
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
  SDL_AUTO_TRACE();
  std::string messageString = message->json_message();
  if (messageString.length() == 0) {
    SDL_WARN("Drop ill-formed message from mobile");
    return NULL;
  }

  uint8_t* rawMessage = new uint8_t[messageString.length() + 1];
  memcpy(rawMessage, messageString.c_str(), messageString.length() + 1);

  protocol_handler::RawMessage* result = new protocol_handler::RawMessage(
      message->connection_key(), 1, rawMessage, messageString.length() + 1);

  delete[] rawMessage;

  return result;
}

protocol_handler::RawMessage*
MobileMessageHandler::HandleOutgoingMessageProtocolV2(
    const MobileMessage& message) {
  SDL_AUTO_TRACE();
  if (message->json_message().length() == 0) {
    SDL_ERROR("json string is empty.");
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

  // Default the service type to RPC Service
  uint8_t type = 0x07;

  if (message->has_binary_data()) {
    // Change the service type to Hybrid Service
    type = 0x0F;
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
                                       dataForSendingSize,
                                       type);

  delete[] dataForSending;

  return msgToProtocolHandler;
}
}  // namespace application_manager
