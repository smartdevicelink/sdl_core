#include "protocol_handler/service_type.h"
#include "protocol_handler/protocol_packet.h"

#include "utils/logger.h"
#include "utils/macro.h"


namespace protocol_handler {

namespace {
log4cxx::LoggerPtr g_logger = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("ConnectionHandler"));

// Check if provided service value is one of the specified
bool IsValid(ServiceType service_type) {
  switch (service_type) {
    case kZero:
    case kRpc:
    case kAudio:
    case kMobileNav:
    case kBulk:
      return true;
    default:
      return false;
  }
}
} // namespace

ServiceType ServiceTypeFromByte(uint8_t byte) {
  ServiceType type = ServiceType(byte);
  bool valid_type = IsValid(type);
  if (!valid_type) {
    LOG4CXX_INFO(g_logger, "Invalid service type: "<<int32_t(byte))
  }
  return valid_type ? type : kInvalidServiceType;
}

uint8_t ServiceTypeToByte(ServiceType type) {
  DCHECK(IsValid(type));
  return uint8_t(type);
}

} // namespace protocol_handler
