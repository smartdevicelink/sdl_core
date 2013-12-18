#include "protocol_handler/message_priority.h"

#include "utils/logger.h"

namespace {
log4cxx::LoggerPtr g_logger = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("ConnectionHandler"));
}

namespace protocol_handler {

// static
const MessagePriority MessagePriority::kDefault = MessagePriority(0);

// static
MessagePriority MessagePriority::FromServiceType(ServiceType service_type) {
  switch (service_type) {
    case kRpc:       return MessagePriority(3);
    case kMovileNav: return MessagePriority(2);
    case kBulk:      return MessagePriority(1);
    default:
      LOG4CXX_ERROR(g_logger, "Prioritizing service with invalid type value: "
                    <<int(service_type));
      // If we got here it is logic error (bug) in the program
      // Return lowest priority possible
      return kDefault;
  }
}

} // namespace protocol_handler
