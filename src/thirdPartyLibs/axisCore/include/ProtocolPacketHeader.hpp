#ifndef PROTOCOL_PACKET_HHEADER_HPP
#define PROTOCOL_PACKET_HHEADER_HPP

#include "utils/misc/Types.hpp"



//TODO TMP
//#include "MessageGenerator/CMessage.hpp"
#include "MessageGenerator/CMessage.hpp"


namespace AxisCore
{

const UInt8 PROTOCOL_HEADER_SIZE = 12;

const UInt8 PROTOCOL_VERSION_1 = 0x01;

const bool COMPRESS_OFF = false;

const UInt8 FRAME_TYPE_CONTROL = 0x00;
const UInt8 FRAME_TYPE_SINGLE = 0x01;
const UInt8 FRAME_TYPE_FIRST = 0x02;
const UInt8 FRAME_TYPE_CONSECUTIVE = 0x03;

const UInt8 SERVICE_TYPE_RPC = 0x07;

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

const UInt8 FIRST_FRAME_DATA_SIZE = 0x08;


struct ProtocolPacketHeader
{
    ProtocolPacketHeader() : version(0),
                             compress(COMPRESS_OFF),
                             frameType(0),
                             serviceType(0),
                             frameData(0),
                             sessionID(0),
                             dataSize(0),
                             messageID(0)
    { }

    ProtocolPacketHeader(UInt8 _version,
                         bool _compress,
                         UInt8 _frameType,
                         UInt8 _serviceType,
                         UInt8 _frameData,
                         UInt8 _sessionID,
                         UInt32 _dataSize,
                         UInt32 _messageID) : 
                             version(_version),
                             compress(_compress),
                             frameType(_frameType),
                             serviceType(_serviceType),
                             frameData(_frameData),
                             sessionID(_sessionID),
                             dataSize(_dataSize),
                             messageID(_messageID)
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

} //namespace AxisCore

#endif //PROTOCOL_PACKET_HHEADER_HPP
