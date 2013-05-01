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

#include "application_manager/message.h"
#include "Utils/macro.h"

namespace application_manager {

Message::Message()
    : function_id_(0),
      type_(kUnknownType),
      correlation_id_(0),
      connection_key_(0),
      binary_data_(NULL),
      version_(kUnknownProtocol) {
}

Message::Message(const Message& message) {
  *this = message;
}

Message& Message::operator=(const Message& message) {
  set_function_id(message.function_id_);
  set_correlation_id(message.correlation_id_);
  set_connection_key(message.connection_key_);
  set_message_type(message.type_);
  set_binary_data(message.binary_data_);
  set_json_message(message.json_message_);
  set_protocol_version(message.protocol_version());

  return *this;
}

Message::~Message() {
}

int Message::function_id() const {
  return function_id_;
}

int Message::correlation_id() const {
  return correlation_id_;
}

int Message::connection_key() const {
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

void Message::set_function_id(int id) {
  function_id_ = id;
}

void Message::set_correlation_id(int id) {
  correlation_id_ = id;
}

void Message::set_connection_key(int key) {
  connection_key_ = key;
}

void Message::set_message_type(MessageType type) {
  type_ = type;
}

void Message::set_binary_data(BinaryData* data) {
  if (NULL == data) {
    // Please, don't add NOTREACHED() here.
    // We can copy object without binary data using copy ctor and operator=
    return;
  }

  binary_data_ = data;
}

void Message::set_json_message(const std::string& json_message) {
  json_message_ = json_message;
}

void Message::set_protocol_version(ProtocolVersion version) {
  version_ = version;
}

}  // namespace application_manager
