#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_MESSAGE_PRIORITY_H
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_MESSAGE_PRIORITY_H

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
  // Priorities are ordered exactly as their values, so something has
  // higher priority if it's priority value is higher.
  // While priority object is tiny, it's faster to pass it by value
  bool operator< (MessagePriority that) const { return value_ < that.value_; }
  bool operator> (MessagePriority that) const { return value_ > that.value_; }
 private:
  // Constructor is private to prevent creation
  // of random priorities in the code please use static constructor if you
  // need priority value
  explicit MessagePriority(int value): value_(value) {}
  int value_;
};

} // namespace protocol_handler

#endif // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_MESSAGE_PRIORITY_H
