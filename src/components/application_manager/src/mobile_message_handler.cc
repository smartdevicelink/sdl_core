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

namespace {
const uint8_t kRequest = 0x0;
const uint8_t kResponse = 0x1;
const uint8_t kNotification = 0x2;
const uint8_t kUnknown = 0xF;
}

namespace application_manager {
using protocol_handler::Extract;

namespace {
typedef std::map<MessageType, std::string> MessageTypeMap;
MessageTypeMap message_types = {std::make_pair(kRequest, "Request"),
                                std::make_pair(kResponse, "Response"),
                                std::make_pair(kNotification, "Notification")};
}
CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

application_manager::Message*
MobileMessageHandler::HandleIncomingMessageProtocol(
    const protocol_handler::RawMessagePtr message) {
  DCHECK_OR_RETURN(message, NULL);
  application_manager::Message* out_message = NULL;
  switch (message->protocol_version()) {
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1:
      LOG4CXX_DEBUG(logger_, "Protocol version - V1");
      out_message =
          MobileMessageHandler::HandleIncomingMessageProtocolV1(message);
      break;
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_2:
      LOG4CXX_DEBUG(logger_, "Protocol version - V2");
      out_message =
          MobileMessageHandler::HandleIncomingMessageProtocolV2(message);
      break;
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_3:
      LOG4CXX_DEBUG(logger_, "Protocol version - V3");
      out_message =
          MobileMessageHandler::HandleIncomingMessageProtocolV2(message);
      break;
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_4:
      LOG4CXX_DEBUG(logger_, "Protocol version - V4");
      out_message =
          MobileMessageHandler::HandleIncomingMessageProtocolV2(message);
      break;
    case protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_5:
      LOG4CXX_DEBUG(logger_, "Protocol version - V5");
      out_message =
          MobileMessageHandler::HandleIncomingMessageProtocolV2(message);
      break;
    default:
      LOG4CXX_WARN(logger_, "Can't recognise protocol version");
      out_message = NULL;
      break;
  }
  if (out_message == NULL) {
    LOG4CXX_WARN(logger_, "Message is NULL");
    return NULL;
  }
  LOG4CXX_DEBUG(logger_,
                "Incoming RPC_INFO: " << (out_message->connection_key() >> 16)
                                      << ", "
                                      << message_types[out_message->type()]
                                      << ", " << out_message->function_id()
                                      << ", " << out_message->correlation_id()
                                      << ", " << out_message->json_message());
  return out_message;
}

protocol_handler::RawMessage*
MobileMessageHandler::HandleOutgoingMessageProtocol(
    const MobileMessage& message) {
  LOG4CXX_DEBUG(logger_,
                "Outgoing RPC_INFO: " << (message->connection_key() >> 16)
                                      << ", " << message_types[message->type()]
                                      << ", " << message->function_id() << ", "
                                      << message->correlation_id() << ", "
                                      << message->json_message());

  if (message->protocol_version() ==
      protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_1) {
    return MobileMessageHandler::HandleOutgoingMessageProtocolV1(message);
  }
  if (Message::is_sufficient_version(
          protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_2,
          message->protocol_version())) {
    return MobileMessageHandler::HandleOutgoingMessageProtocolV2(message);
  }
  return NULL;
}

application_manager::Message*
MobileMessageHandler::HandleIncomingMessageProtocolV1(
    const ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::Message* outgoing_message =
      new application_manager::Message(
          protocol_handler::MessagePriority::FromServiceType(
              message->service_type()));
  if (!message) {
    NOTREACHED();
    delete outgoing_message;
    return NULL;
  }

  outgoing_message->set_connection_key(message->connection_key());
  outgoing_message->set_protocol_version(
      static_cast<protocol_handler::MajorProtocolVersion>(
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
  LOG4CXX_AUTO_TRACE(logger_);
  utils::BitStream message_bytestream(message->data(), message->data_size());
  protocol_handler::ProtocolPayloadV2 payload;
  protocol_handler::Extract(
      &message_bytestream, &payload, message->data_size());

  // Silently drop message if it wasn't parsed correctly
  if (message_bytestream.IsBad()) {
    LOG4CXX_WARN(
        logger_,
        "Drop ill-formed message from mobile, partially parsed: " << payload);
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
  outgoing_message->set_correlation_id(int32_t(payload.header.correlation_id));
  outgoing_message->set_connection_key(message->connection_key());
  outgoing_message->set_protocol_version(
      static_cast<protocol_handler::MajorProtocolVersion>(
          message->protocol_version()));
  outgoing_message->set_data_size(message->data_size());
  outgoing_message->set_payload_size(message->payload_size());

  if (!payload.data.empty()) {
    const BinaryData binary_payload_data(payload.data);
    outgoing_message->set_binary_data(&binary_payload_data);
  }
  return outgoing_message.release();
}

protocol_handler::RawMessage*
MobileMessageHandler::HandleOutgoingMessageProtocolV1(
    const MobileMessage& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string message_string = message->json_message();
  if (message_string.length() == 0) {
    LOG4CXX_WARN(logger_, "Drop ill-formed message from mobile");
    return NULL;
  }

  BinaryData raw_message(message_string.length() + 1);
  memcpy(&raw_message[0], message_string.c_str(), message_string.length() + 1);

  protocol_handler::RawMessage* result =
      new protocol_handler::RawMessage(message->connection_key(),
                                       1,
                                       &raw_message[0],
                                       message_string.length() + 1);

  return result;
}

protocol_handler::RawMessage*
MobileMessageHandler::HandleOutgoingMessageProtocolV2(
    const MobileMessage& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (message->json_message().length() == 0) {
    LOG4CXX_ERROR(logger_, "json string is empty.");
  }
  uint32_t json_size = message->json_message().length();
  uint32_t binary_size = 0;
  if (message->has_binary_data()) {
    binary_size = message->binary_data()->size();
  }

  const size_t data_for_sending_size =
      protocol_handler::PROTOCOL_HEADER_V2_SIZE + json_size + binary_size;
  BinaryData data_for_sending(data_for_sending_size);
  uint8_t offset = 0;

  uint8_t rpc_type_flag = 0;
  switch (message->type()) {
    case application_manager::kRequest:
      rpc_type_flag = kRequest;
      break;
    case application_manager::kResponse:
      rpc_type_flag = kResponse;
      break;
    case application_manager::kNotification:
      rpc_type_flag = kNotification;
      break;
    default:
      NOTREACHED();
      break;
  }

  uint32_t function_id = message->function_id();
  data_for_sending[offset++] =
      ((rpc_type_flag << 4) & 0xF0) | (function_id >> 24);
  data_for_sending[offset++] = function_id >> 16;
  data_for_sending[offset++] = function_id >> 8;
  data_for_sending[offset++] = function_id;

  uint32_t correlation_id = message->correlation_id();
  data_for_sending[offset++] = correlation_id >> 24;
  data_for_sending[offset++] = correlation_id >> 16;
  data_for_sending[offset++] = correlation_id >> 8;
  data_for_sending[offset++] = correlation_id;

  data_for_sending[offset++] = json_size >> 24;
  data_for_sending[offset++] = json_size >> 16;
  data_for_sending[offset++] = json_size >> 8;
  data_for_sending[offset++] = json_size;

  memcpy(&data_for_sending[offset], message->json_message().c_str(), json_size);

  // Default the service type to RPC Service
  uint8_t type = 0x07;

  if (message->has_binary_data()) {
    // Change the service type to Hybrid Service
    type = 0x0F;
    const BinaryData& binary_data = *(message->binary_data());
    BinaryData::value_type* current_pointer =
        &data_for_sending[offset + json_size];
    for (uint32_t i = 0; i < binary_size; ++i) {
      current_pointer[i] = binary_data[i];
    }
  }

  protocol_handler::RawMessage* msg_to_protocol_handler =
      new protocol_handler::RawMessage(message->connection_key(),
                                       message->protocol_version(),
                                       &data_for_sending[0],
                                       data_for_sending_size,
                                       type);

  return msg_to_protocol_handler;
}
}  // namespace application_manager
