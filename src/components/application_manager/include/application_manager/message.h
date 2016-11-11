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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_MESSAGE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_MESSAGE_H_

#include <string>
#include <vector>

#include "utils/shared_ptr.h"
#include "protocol/message_priority.h"
#include "protocol/rpc_type.h"
#include "smart_objects/smart_object.h"

namespace application_manager {

typedef std::vector<uint8_t> BinaryData;

// Message type is a general type used by both mobile and HMI messages
enum MessageType {
  kUnknownType = -1,
  kRequest = 0,
  kResponse = 1,
  kNotification = 2,
  kErrorResponse = 3  // Error Response HMI ONLY
};

// Map PrcType to corresponding MessageType
MessageType MessageTypeFromRpcType(protocol_handler::RpcType rpc_type);

enum ProtocolVersion {
  kUnknownProtocol = -1,
  kHMI = 0,
  kV1 = 1,
  kV2 = 2,
  kV3 = 3,
  kV4 = 4
};

class Message {
 public:
  Message(protocol_handler::MessagePriority priority);
  Message(const Message& message);
  Message& operator=(const Message& message);
  bool operator==(const Message& message);
  ~Message();

  //! --------------------------------------------------------------------------
  int32_t function_id() const;
  int32_t correlation_id() const;
  int32_t connection_key() const;

  MessageType type() const;
  ProtocolVersion protocol_version() const;

  const std::string& json_message() const;
  const BinaryData* binary_data() const;
  bool has_binary_data() const;
  size_t data_size() const;
  size_t payload_size() const;
  const smart_objects::SmartObject& smart_object() const;

  //! --------------------------------------------------------------------------
  void set_function_id(int32_t id);
  void set_correlation_id(int32_t id);
  void set_connection_key(int32_t key);
  void set_message_type(MessageType type);
  void set_binary_data(BinaryData* data);
  void set_json_message(const std::string& json_message);
  void set_protocol_version(ProtocolVersion version);
  void set_smart_object(const smart_objects::SmartObject& object);
  void set_data_size(size_t data_size);
  void set_payload_size(size_t payload_size);

  protocol_handler::MessagePriority Priority() const {
    return priority_;
  }

 private:
  int32_t function_id_;     // @remark protocol V2.
  int32_t correlation_id_;  // @remark protocol V2.
  MessageType type_;        // @remark protocol V2.

  // Pre-calculated message priority, higher priority messages are
  // Processed first
  protocol_handler::MessagePriority priority_;

  int32_t connection_key_;
  std::string json_message_;
  smart_objects::SmartObject smart_object_;

  // TODO(akandul): replace with shared_ptr
  BinaryData* binary_data_;
  size_t data_size_;
  size_t payload_size_;
  ProtocolVersion version_;
};
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_MESSAGE_H_
