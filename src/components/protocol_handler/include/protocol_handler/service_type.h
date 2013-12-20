#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SERVICE_TYPE_H
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SERVICE_TYPE_H

#include <stdint.h>

#include "protocol_handler/protocol_packet.h"

namespace protocol_handler {

/**
  * \brief Enum describing possible types of sessions: RPC for API messages,
    Navi for video streaming, bulk for PutFile.
  */
enum ServiceType {
	kRpc =       SERVICE_TYPE_RPC,
	kMovileNav = SERVICE_TYPE_NAVI,
	kBulk =      SERVICE_TYPE_BULK,
  kInvalidServiceType
};

/*
 * Service type conversion functions that are used to read and output
 * Service types to binary stream
 */
ServiceType ServiceTypeFromByte(uint8_t type);
uint8_t ServiceTypeToByte(ServiceType type);

} // namespace protocol_handler

#endif // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SERVICE_TYPE_H
