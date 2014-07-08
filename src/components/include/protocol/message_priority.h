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
#ifndef SRC_COMPONENTS_INCLUDE_PROTOCOL_MESSAGE_PRIORITY_H_
#define SRC_COMPONENTS_INCLUDE_PROTOCOL_MESSAGE_PRIORITY_H_

#include <cstddef>

#include "protocol/service_type.h"

namespace protocol_handler {

// Class for calculating and tracking message priorities in the system
// It was decided to wrap simple priority number into trivial class to
// Have all priority value calculations in single place and
// prevent random priorities assignment in the code
class MessagePriority {
 public:
  // Default (in other words non-prioritized) priority value
  static const MessagePriority kDefault;
  // Static constructor to get priority value of Service type
  static MessagePriority FromServiceType(ServiceType service_type);

  // Trivial inline copy constructor
  MessagePriority(const MessagePriority &that);

  // Ordering value which is used by queues to order messages
  // Higher value means higher prioirty
  size_t OrderingValue() const;
 private:
  // Constructor is private to prevent creation
  // of random priorities in the code please use static constructor if you
  // need priority value
  explicit MessagePriority(int32_t value): value_(value) {}
  size_t value_;
};
}  // namespace protocol_handler
#endif  // SRC_COMPONENTS_INCLUDE_PROTOCOL_MESSAGE_PRIORITY_H_
