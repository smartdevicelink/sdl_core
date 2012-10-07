#ifndef PROTOCOL_PACKET_HHEADER_HPP
#define PROTOCOL_PACKET_HHEADER_HPP

#include "utils/misc/Types.hpp"

const UInt8 PROTOCOL_VERSION_1 = 0x01;

const bool COMPRESS_OFF = false;

const UInt8 FRAME_TYPE_CONTROL = 0x00;
const UInt8 FRAME_TYPE_SINGLE = 0x01;
const UInt8 FRAME_TYPE_FIRST = 0x02;
const UInt8 FRAME_TYPE_CONSECUTIVE = 0x03;

enum SERVICE_TYPE
{
    SERVICE_TYPE_RPC = 0x07
};
//const UInt8 SERVICE_TYPE_RPC = 0x07;

/**
 * If FRAME_TYPE_CONTROL :
 */
const UInt8 FRAME_DATA_HEART_BEAT = 0x00;   //Unused
const UInt8 FRAME_DATA_START_SESSION = 0x01;
const UInt8 FRAME_DATA_START_SESSION_ACK = 0x02;
const UInt8 FRAME_DATA_START_SESSION_NACK = 0x03;
const UInt8 FRAME_DATA_END_SESSION = 0x04;

/**
 * If FRAME_TYPE_CONSECUTIVE :
 */
const UInt8 FRAME_DATA_LAST_FRAME = 0x00;


struct ProtocolPacketHeader
{
    ProtocolPacketHeader() : version(0),
                             compress(false),
                             frameType(0),
                             serviceType(0),
                             frameData(0),
                             sessionID(0),
                             dataSize(0),
                             messageID(0)
    { }

    UInt8 version;
    bool compress;
    UInt8 frameType;
    UInt8 serviceType;
    UInt8 frameData;
    UInt8 sessionID;
    UInt32 dataSize;
    UInt32 messageID;
};

#endif //PROTOCOL_PACKET_HHEADER_HPP
