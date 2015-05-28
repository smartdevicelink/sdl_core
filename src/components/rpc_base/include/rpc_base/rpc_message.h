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

#ifndef TYPE_BASE_H_
#define TYPE_BASE_H_

#include "rpc_base.h"

namespace Json {
class Value;
}

namespace rpc {

enum MessageType {
  kRequest = 0,
  kResponse = 1,
  kNotification = 2
};

// Base class for all RPC messages
// Has methods to identify interface this message belongs to
// And message ID
class Message {
public:
  // Type of message
  virtual MessageType message_type() const = 0;
  // Numerical function identifier, unique to the interface
  // Responses share function identifier with corresponding requests
  virtual int32_t function_id() const = 0;
  // Function string identifier as specified in original xml file
  virtual const char* function_string_id() const = 0;
  // Interface name as specified in original xml file
  virtual const char* interface_string_id() const = 0;
  // Serializes message to Json::Value
  virtual Json::Value ToJsonValue() const = 0;
  virtual ~Message() {}
};

// Base class for all interface-specific requests
class RequestBase : public Message, public CompositeType {
 public:
  explicit RequestBase(InitializationState init_state): CompositeType(init_state) {}
  // Message interface
  MessageType message_type() const { return kRequest; }
  virtual ~RequestBase() {}
};

// Base class for all interface-specific responses
class ResponseBase : public Message, public CompositeType {
 public:
  ResponseBase(InitializationState init_state): CompositeType(init_state) {}
  // Message interface
  MessageType message_type() const { return kResponse; }
  virtual ~ResponseBase() {}
};

// Base class for all interface-specific notifications
class NotificationBase : public Message, public CompositeType {
 public:
  explicit NotificationBase(InitializationState init_state): CompositeType(init_state) {}
  MessageType message_type() const { return kNotification; }
  // Message interface
  virtual ~NotificationBase() {}
};

}  // namespace rpc

#endif /* TYPE_BASE_H_ */
