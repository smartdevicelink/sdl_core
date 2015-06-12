/*
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

#include "application_manager/message.h"

namespace {
bool BinaryDataPredicate(uint8_t i, uint8_t j) {
  return (i == j);
}
}

namespace application_manager {

MessageType MessageTypeFromRpcType(protocol_handler::RpcType rpc_type) {
  switch (rpc_type) {
    case protocol_handler::kRpcTypeRequest:
      return kRequest;
    case protocol_handler::kRpcTypeResponse:
      return kResponse;
    case protocol_handler::kRpcTypeNotification:
      return kNotification;
    case protocol_handler::kRpcTypeReserved:
    default:
      DCHECK(false);
      return kUnknownType;
  }
}

Message::Message(protocol_handler::MessagePriority priority)
    : function_id_(0),
      correlation_id_(0),
      type_(kUnknownType),
      priority_(priority),
      connection_key_(0),
      binary_data_(NULL),
      data_size_(0),
      payload_size_(0),
      version_(kUnknownProtocol) {
}

Message::Message(const Message& message)
    : priority_(message.priority_) {
  *this = message;
}

Message& Message::operator=(const Message& message) {
  set_function_id(message.function_id_);
  set_correlation_id(message.correlation_id_);
  set_connection_key(message.connection_key_);
  set_message_type(message.type_);
  set_data_size(message.data_size_);
  set_payload_size(message.payload_size_);
  if (message.binary_data_) {
    set_binary_data(message.binary_data_);
  }
  set_json_message(message.json_message_);
  set_protocol_version(message.protocol_version());
  priority_ = message.priority_;

  return *this;
}

bool Message::operator==(const Message& message) {
  bool function_id = function_id_ == message.function_id_;
  bool correlation_id = correlation_id_ == message.correlation_id_;
  bool connection_key = connection_key_ == message.connection_key_;
  bool type = type_ == message.type_;
  bool json_message = json_message_ == message.json_message_;
  bool version = version_ == message.version_;
  bool data_size = data_size_ == message.data_size_;
  bool payload_size = payload_size_ == message.payload_size_;


  bool binary_data = std::equal(binary_data_->begin(), binary_data_->end(),
                                message.binary_data_->begin(),
                                BinaryDataPredicate);

  return function_id && correlation_id && connection_key && type && binary_data
      && json_message && version && data_size && payload_size;
}

Message::~Message() {
  if (binary_data_) {
    delete binary_data_;
  }
}

int32_t Message::function_id() const {
  return function_id_;
}

int32_t Message::correlation_id() const {
  return correlation_id_;
}

int32_t Message::connection_key() const {
  return connection_key_;
}

MessageType Message::type() const {
  return type_;
}

ProtocolVersion Message::protocol_version() const {
  return version_;
}

const std::string& Message::json_message() const {
  return json_message_;
}

const BinaryData* Message::binary_data() const {
  return binary_data_;
}

bool Message::has_binary_data() const {
  return (binary_data_ != NULL);
}

size_t Message::data_size() const {
  return data_size_;
}

size_t Message::payload_size() const {
  return payload_size_;
}

void Message::set_function_id(int32_t id) {
  function_id_ = id;
}

void Message::set_correlation_id(int32_t id) {
  correlation_id_ = id;
}

void Message::set_connection_key(int32_t key) {
  connection_key_ = key;
}

void Message::set_message_type(MessageType type) {
  type_ = type;
}

void Message::set_binary_data(BinaryData* data) {
  if (NULL == data) {
    NOTREACHED();
    return;
  }

  if (binary_data_) {
    delete binary_data_;
  }

  binary_data_ = data;
}

void Message::set_json_message(const std::string& json_message) {
  json_message_ = json_message;
}

void Message::set_protocol_version(ProtocolVersion version) {
  version_ = version;
}

const smart_objects::SmartObject &Message::smart_object() const {
  return smart_object_;
}

void Message::set_smart_object(const smart_objects::SmartObject& object) {
  smart_object_ = object;
}

void Message::set_data_size(size_t data_size) {
  data_size_ = data_size;
}

void Message::set_payload_size(size_t payload_size) {
  payload_size_ = payload_size;
}
}  // namespace application_manager
