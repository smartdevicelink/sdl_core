#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_MESSAGE_PRIORITY_H
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_MESSAGE_PRIORITY_H

#include <cstddef>

#include "protocol_handler/service_type.h"

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
  MessagePriority(const MessagePriority& that): value_(that.value_) {}

  // Ordering value which is used by queues to order messages
  // Higher value means higher prioirty
  size_t OrderingValue() { return value_; }
 private:
  // Constructor is private to prevent creation
  // of random priorities in the code please use static constructor if you
  // need priority value
  explicit MessagePriority(int32_t value): value_(value) {}
  size_t value_;
};

} // namespace protocol_handler

#endif // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_MESSAGE_PRIORITY_H
