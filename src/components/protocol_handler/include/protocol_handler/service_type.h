#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SERVICE_TYPE_H
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SERVICE_TYPE_H

#include <stdint.h>

namespace protocol_handler {

  /**
   *\brief Constant: Frame type for HeartBeat
   */
  const uint8_t SERVICE_TYPE_CONTROL = 0x00;

  /**
   *\brief Constant: RPC type of session
   */
  const uint8_t SERVICE_TYPE_RPC = 0x07;

  /**
   *\brief Constant: Raw PCM audio service
   */
  const uint8_t SERVICE_TYPE_AUDIO = 0x0A;

  /**
   * \brief Constant: Mobile Navi type of session for map streaming
   */
  const uint8_t SERVICE_TYPE_NAVI = 0x0B;

  /**
   *\brief Constant: Bulk data type of session (hybrid)
   */
  const uint8_t SERVICE_TYPE_BULK = 0x0F;


/**
  * \brief Enum describing possible types of sessions: RPC for API messages,
    Navi for video streaming, bulk for PutFile.
  */
enum ServiceType {
  kControl =   SERVICE_TYPE_CONTROL,
  kRpc =       SERVICE_TYPE_RPC,
  kAudio =     SERVICE_TYPE_AUDIO,
  kMobileNav = SERVICE_TYPE_NAVI,
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
