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

#include <string.h>

#include "utils/macro.h"
#include "application_manager/mobile_message_handler.h"

namespace {
const unsigned char kRequest = 0x0;
const unsigned char kResponse = 0x1;
const unsigned char kNotification = 0x2;
const unsigned char kUnknown = 0xF;
}

namespace application_manager {

log4cxx::LoggerPtr MobileMessageHandler::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("MobileMessageHandler"));

application_manager::Message*
MobileMessageHandler::HandleIncomingMessageProtocolV1(
  const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandler HandleIncomingMessageProtocolV1()");
  application_manager::Message* outgoing_message =
    new application_manager::Message;
  if (!message) {
    NOTREACHED();
    return NULL;
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
MobileMessageHandler::HandleIncomingMessageProtocolV2(
  const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandler HandleIncomingMessageProtocolV2()");
  application_manager::Message* outgoing_message =
    new application_manager::Message;
  if (!message) {
    NOTREACHED();
    LOG4CXX_ERROR(logger_, "Allocation failed: outgoing message");
    return NULL;
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
    default:
      NOTREACHED();
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
    LOG4CXX_ERROR(logger_, "Received invalid json packet header.");
    return NULL;
  }

  std::string json_string = std::string(
                              reinterpret_cast<const char*>(receivedData) + offset, jsonSize);

  if (functionId == 0 || correlationId == 0 || message->connection_key() == 0) {
    delete outgoing_message;
    LOG4CXX_ERROR(logger_, "Invalid message constructed.");
    return NULL;
  }

  outgoing_message->set_json_message(json_string);
  outgoing_message->set_function_id(functionId);
  outgoing_message->set_message_type(
    static_cast<application_manager::MessageType>(rpcType));
  outgoing_message->set_correlation_id(correlationId);
  outgoing_message->set_connection_key(message->connection_key());
  outgoing_message->set_protocol_version(
    static_cast<application_manager::ProtocolVersion>(message
        ->protocol_version()));

  if (message->data_size() > (offset + jsonSize)) {
    application_manager::BinaryData* binaryData =
      new application_manager::BinaryData(
      receivedData + offset + jsonSize,
      receivedData + message->data_size());

    if (!binaryData) {
      delete outgoing_message;
      LOG4CXX_ERROR(logger_, "Allocation failed: binary data");
      NOTREACHED();
      return NULL;
    }

    outgoing_message->set_binary_data(binaryData);
  }

  return outgoing_message;
}

protocol_handler::RawMessage*
MobileMessageHandler::HandleOutgoingMessageProtocolV1(
  const MobileMessage& message) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandler HandleOutgoingMessageProtocolV1()");
  std::string messageString = message->json_message();
  if (messageString.length() == 0) {
    return NULL;
  }

  unsigned char* rawMessage = new unsigned char[messageString.length() + 1];
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
    LOG4CXX_ERROR(logger_, "json string is empty.")
    // return NULL;
  }

  const uint MAX_HEADER_SIZE = 12;
  unsigned int jsonSize = message->json_message().length();
  unsigned int binarySize = 0;
  if (message->has_binary_data()) {
    binarySize = message->binary_data()->size();
  }

  unsigned char* dataForSending = new unsigned char[MAX_HEADER_SIZE + jsonSize
      + binarySize];
  unsigned char offset = 0;

  unsigned char rpcTypeFlag = 0;
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

  unsigned int functionId = message->function_id();
  dataForSending[offset++] = ((rpcTypeFlag << 4) & 0xF0) | (functionId >> 24);
  dataForSending[offset++] = functionId >> 16;
  dataForSending[offset++] = functionId >> 8;
  dataForSending[offset++] = functionId;

  unsigned int correlationId = message->correlation_id();
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
    const std::vector<unsigned char>& binaryData = *(message->binary_data());
    unsigned char* currentPointer = dataForSending + offset + jsonSize;
    for (unsigned int i = 0; i < binarySize; ++i) {
      currentPointer[i] = binaryData[i];
    }
  }

  protocol_handler::RawMessage* msgToProtocolHandler =
    new protocol_handler::RawMessage(message->connection_key(), 2,
                                     dataForSending,
                                     MAX_HEADER_SIZE + jsonSize + binarySize);

  return msgToProtocolHandler;
}
}  // namespace application_manager
