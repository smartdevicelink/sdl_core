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
#include <string.h>
#include <algorithm>

#include "utils/macro.h"
#include "mobile_message_handler/mobile_message_handler_impl.h"

#include "mobile_message_handler/messages_from_mobile_app_handler.h"
#include "mobile_message_handler/message_to_mobile_app_handler.h"

namespace {
const unsigned char kRequest = 0x0;
const unsigned char kResponse = 0x1;
const unsigned char kNotification = 0x2;
const unsigned char kUnknown = 0xF;
}

namespace mobile_message_handler {

MobileMessageHandlerImpl* MobileMessageHandlerImpl::self_ = NULL;
log4cxx::LoggerPtr MobileMessageHandlerImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("MobileMessageHandler"));
//! ----------------------------------------------------------------------------

MobileMessageHandlerImpl::MobileMessageHandlerImpl()
  : handle_messages_from_mobile_app_(NULL),
    handle_messages_to_mobile_app_(NULL),
    protocol_handler_(NULL) {
  LOG4CXX_INFO(logger_, "MobileMessageHandlerImpl ctor");
  handle_messages_from_mobile_app_ = new threads::Thread(
    "MobileMessageHandler::MessagesFromMobileAppHandler",
    new MessagesFromMobileAppHandler());

  handle_messages_from_mobile_app_->startWithOptions(
    threads::ThreadOptions(threads::Thread::kMinStackSize));

  handle_messages_to_mobile_app_ = new threads::Thread(
    "MobileMessageHandler::MessagesToMobileAppHandler",
    new MessagesToMobileAppHandler());

  handle_messages_to_mobile_app_->startWithOptions(
    threads::ThreadOptions(threads::Thread::kMinStackSize));
}

MobileMessageHandlerImpl::~MobileMessageHandlerImpl() {
  LOG4CXX_INFO(logger_, "MobileMessageHandlerImpl dtor");
  handle_messages_from_mobile_app_->stop();
  delete handle_messages_from_mobile_app_;
  handle_messages_from_mobile_app_ = NULL;

  handle_messages_to_mobile_app_->stop();
  delete handle_messages_to_mobile_app_;
  handle_messages_to_mobile_app_ = NULL;
}

MobileMessageHandlerImpl* MobileMessageHandlerImpl::instance() {
  LOG4CXX_INFO(logger_, "MobileMessageHandlerImpl instance()");
  if (NULL == MobileMessageHandlerImpl::self_) {
    MobileMessageHandlerImpl::self_ = new MobileMessageHandlerImpl;
  }

  return MobileMessageHandlerImpl::self_;
}

void MobileMessageHandlerImpl::set_protocol_handler(
  protocol_handler::ProtocolHandler* protocolHandler) {
  LOG4CXX_INFO(logger_, "MobileMessageHandlerImpl set_protocol_handler()");
  DCHECK(protocolHandler);
  protocol_handler_ = protocolHandler;
}

void MobileMessageHandlerImpl::OnMessageReceived(
  const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_INFO(logger_, "MobileMessageHandlerImpl onMessageReceived()");
  DCHECK(message);

  if (!message) {
    return;
  }

  messages_from_mobile_app_.push(message);
}

void MobileMessageHandlerImpl::SendMessageToMobileApp(
  const MobileMessage& message) {
  LOG4CXX_INFO(logger_, "MobileMessageHandlerImpl SendMessageToMobileApp()");
  DCHECK(message.valid());

  messages_to_mobile_app_.push(message);
}

application_manager::Message*
MobileMessageHandlerImpl::HandleIncomingMessageProtocolV1(
  const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandlerImpl HandleIncomingMessageProtocolV1()");
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
MobileMessageHandlerImpl::HandleIncomingMessageProtocolV2(
  const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandlerImpl HandleIncomingMessageProtocolV2()");
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
MobileMessageHandlerImpl::HandleOutgoingMessageProtocolV1(
  const MobileMessage& message) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandlerImpl HandleOutgoingMessageProtocolV1()");
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
MobileMessageHandlerImpl::HandleOutgoingMessageProtocolV2(
  const MobileMessage& message) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandlerImpl HandleOutgoingMessageProtocolV2()");
  //  if (json.isNull()) {
  //    if (NsSmartDeviceLinkRPCV2::FunctionID::FunctionIDInternal::OnAudioPassThruID
  //        == message->getMethodId()) {
  //
  //      // Workaround to have no JSON string in OnAudioPassThru notification
  //      // This notification contains audio data only.
  //      const uint MAX_HEADER_SIZE = 12;
  //      unsigned int jsonSize = 0;
  //      unsigned int binarySize = 0;
  //      if (message->has_binary_data()) {
  //        binarySize = message->binary_data()->size();
  //      }
  //      unsigned char* dataForSending = new unsigned char[MAX_HEADER_SIZE
  //          + jsonSize + binarySize];
  //      unsigned char offset = 0;
  //
  //      unsigned char rpcTypeFlag = RPC_NOTIFICATION;
  //
  //      unsigned int functionId = message->function_id();
  //      dataForSending[offset++] = ((rpcTypeFlag << 4) & 0xF0)
  //          | (functionId >> 24);
  //      dataForSending[offset++] = functionId >> 16;
  //      dataForSending[offset++] = functionId >> 8;
  //      dataForSending[offset++] = functionId;
  //
  //      unsigned int correlationId = message->correlation_id()();
  //      dataForSending[offset++] = correlationId >> 24;
  //      dataForSending[offset++] = correlationId >> 16;
  //      dataForSending[offset++] = correlationId >> 8;
  //      dataForSending[offset++] = correlationId;
  //
  //      dataForSending[offset++] = jsonSize >> 24;
  //      dataForSending[offset++] = jsonSize >> 16;
  //      dataForSending[offset++] = jsonSize >> 8;
  //      dataForSending[offset++] = jsonSize;
  //
  //      if (message->has_binary_data()) {
  //        const std::vector<unsigned char>& binaryData =
  //            *(message->binary_data());
  //        unsigned char* currentPointer = dataForSending + offset + jsonSize;
  //        for (unsigned int i = 0; i < binarySize; ++i) {
  //          currentPointer[i] = binaryData[i];
  //        }
  //      }
  //
  //      protocol_handler::RawMessage* msgToProtocolHandler =
  //          new protocol_handler::RawMessage(
  //              message->connection_key(), 2, dataForSending,
  //              MAX_HEADER_SIZE + jsonSize + binarySize);
  //
  //      return msgToProtocolHandler;
  //    } else {
  //      return 0;
  //    }
  //  }

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

void MobileMessageHandlerImpl::AddMobileMessageListener(
  MobileMessageObserver* listener) {
  LOG4CXX_INFO(logger_, "MobileMessageHandlerImpl AddMobileMessageListener()");
  if (NULL == listener) {
    NOTREACHED();
    return;
  }

  std::vector<MobileMessageObserver*>::const_iterator i = std::find(
        mobile_message_listeners_.begin(), mobile_message_listeners_.end(),
        listener);

  if (mobile_message_listeners_.end() == i) {
    mobile_message_listeners_.push_back(listener);
  }
}

void MobileMessageHandlerImpl::RemoveMobileMessageListener(
  MobileMessageObserver* listener) {
  LOG4CXX_INFO(logger_,
               "MobileMessageHandlerImpl RemoveMobileMessageListener()");
  if (NULL == listener) {
    NOTREACHED();
    return;
  }

  std::vector<MobileMessageObserver*>::iterator i = std::find(
        mobile_message_listeners_.begin(), mobile_message_listeners_.end(),
        listener);

  if (mobile_message_listeners_.end() != i) {
    mobile_message_listeners_.erase(i);
  }
}

}  // namespace mobile_message_handler
