#include "protocol_handler/message_priority.h"

#include "utils/logger.h"
#include "utils/macro.h"

namespace {
log4cxx::LoggerPtr g_logger = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("ConnectionHandler"));
}

namespace protocol_handler {

// static
const MessagePriority MessagePriority::kDefault = MessagePriority(0);

// static
MessagePriority MessagePriority::FromServiceType(ServiceType service_type) {
  size_t message_priority_value = size_t(service_type);
  DCHECK(message_priority_value <= 0xFF);
  // According to Applink Protocol Specification v5 service with numerically
  // lower service type identifiers have higher priority
  return MessagePriority(0xFF - service_type);
}

} // namespace protocol_handler
