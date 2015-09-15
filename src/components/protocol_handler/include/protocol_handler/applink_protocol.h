#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_APPLINK_PROTOCOL_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_APPLINK_PROTOCOL_H_

#include <stdint.h>

namespace protocol_handler {

enum FrameType {
  kFtControlFrame =     0x0,
  kFtSingleFrame =      0x1,
  kFtFirstFrame =       0x2,
  kFtConsecutiveFrame = 0x3,
  kFtReserved
};

enum ControlFrameType {
  kCftHeartBeat        = 0x00,
  kCftStartService     = 0x01,
  kCftStartServiceAck  = 0x02,
  kCftStartServiceNAck = 0x03,
  kCftEndService       = 0x04,
  kCftEndServiceAck    = 0x05,
  kCftEndServiceNAck   = 0x06,
  kCftReserved,
  kCftServiceDataAck   = 0xFE,
  kCftHeartBeatAck     = 0xFF
};

enum SingleFrameType {
  kSftValue = 0x00,
  kSftReserved
};

enum FirstFrameType {
  kFftValue = 0x00,
  kFftReserved
};

typedef      uint8_t ConsecutiveFrameType;
static const uint8_t kConsecutiveFrameLast = 0x00;

typedef uint8_t SessionIdType;

enum ControlFrameDataSizeType {
  kCfDsValue = 0x00,
  kCfDsReserved
};

enum FirstFrameDataSizeType {
  kFfDsValue = 0x08,
  kFfDsIncorrectValue
};

typedef uint32_t SingleFrameDataSizeType;
typedef uint32_t ConsecutiveFrameDataSizeType;
static const SingleFrameDataSizeType      kSingleFrameDataSizeIncorrectValue      = 0x0;
static const ConsecutiveFrameDataSizeType kConsecutiveFrameDataSizeIncorrectValue = 0x0;

typedef uint32_t MessageIdType;
static const MessageIdType kMessageIdIncorrectValue = 0x0;

typedef uint32_t SessionHashCode;
static const SessionHashCode SessionHashCodeIncorrectValue = 0x0;

struct ControlFrameHeartBeat {
  SessionIdType session_id;
  MessageIdType message_id;
};

struct ControlFrameHeartBeatAck {
  SessionIdType session_id;
  MessageIdType message_id;
};

} // namespace protocol_handler

#endif // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_APPLINK_PROTOCOL_H_
