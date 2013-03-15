/**
* \file ProtocolPacket.cpp
* \brief ProtocolPacket class source file.
* 
* Copyright (c) 2013 Ford Motor Company
*/

#include <memory.h>
#include "ProtocolHandler/ProtocolPacket.h"

#include <iostream>

using namespace NsProtocolHandler;

ProtocolPacket::ProtocolPacket() :
mPacket( 0 )
,mTotalPacketSize( 0 )
,mDataOffset( 0 )
{}

ProtocolPacket::ProtocolPacket(unsigned char version,
                             bool compress,
                             unsigned char frameType,
                             unsigned char serviceType,
                             unsigned char frameData,
                             unsigned char sessionID,
                             unsigned int dataSize,
                             unsigned int messageID,
                             const unsigned char * data) :
mPacket( 0 )
,mTotalPacketSize( 0 )
,mDataOffset( 0 )
{
    serializePacket(version,
                    compress,
                    frameType,
                    serviceType,
                    frameData,
                    sessionID,
                    dataSize,
                    messageID,
                    data);
}

ProtocolPacket::~ProtocolPacket()
{
    //TODO: where to clean?
    mPacket = 0;
    mTotalPacketSize = 0;
}

/*Serialization*/
RESULT_CODE ProtocolPacket::serializePacket(unsigned char version,
                             bool compress,
                             unsigned char frameType,
                             unsigned char serviceType,
                             unsigned char frameData,
                             unsigned char sessionID,
                             unsigned int dataSize,
                             unsigned int messageID,
                             const unsigned char * data)
{
    if ( mPacket )
    {
        delete [] mPacket;
        mPacket = 0;
        mTotalPacketSize = 0;
    }

    unsigned char offset = 0;
    unsigned char compressF = 0x0;
    mPacket = new unsigned char[MAXIMUM_FRAME_DATA_SIZE];
    if (compress)
    {
        compressF = 0x1;
    }
    unsigned char firstByte = ( (version << 4) & 0xF0 )
                      | ( (compressF << 3) & 0x08)
                      | (frameType & 0x07);

    mPacket[offset++] = firstByte;
    mPacket[offset++] = serviceType;
    mPacket[offset++] = frameData;
    mPacket[offset++] = sessionID;

    mPacket[offset++] = dataSize >> 24;
    mPacket[offset++] = dataSize >> 16;
    mPacket[offset++] = dataSize >> 8;
    mPacket[offset++] = dataSize;

    if (version == PROTOCOL_VERSION_2)
    {
        mPacket[offset++] = messageID >> 24;
        mPacket[offset++] = messageID >> 16;
        mPacket[offset++] = messageID >> 8;
        mPacket[offset++] = messageID;
    }

    mTotalPacketSize = offset;

    if (data)
    {
        if ( (offset + dataSize) <= MAXIMUM_FRAME_DATA_SIZE)
        {
            memcpy(mPacket + offset, data, dataSize);
            mTotalPacketSize += dataSize;
        }
        else
        {
            delete [] mPacket;
            mPacket = 0;
            mTotalPacketSize = 0;
            return RESULT_FAIL;
        }
    }

    return RESULT_OK;
}

unsigned char * ProtocolPacket::getPacket() const
{
    return mPacket;
}

unsigned int ProtocolPacket::getPacketSize() const
{
    return mTotalPacketSize;
}


RESULT_CODE ProtocolPacket::appendData( unsigned char * chunkData,
                    unsigned int chunkDataSize )
{
    if ( mDataOffset + chunkDataSize <= mPacketData.totalDataBytes )
    {
        memcpy(mPacketData.data + mDataOffset, chunkData, chunkDataSize);
        mDataOffset += chunkDataSize;
        return RESULT_OK;
    }
    return RESULT_FAIL;
}
/*End of Serialization*/

/*Deserialization*/
RESULT_CODE ProtocolPacket::deserializePacket(const unsigned char * message, unsigned int messageSize)
{
    unsigned char offset = 0;
    unsigned char firstByte = message[offset];
    offset++;

    mPacketHeader.version = firstByte >> 4u;

    //std::cout << "ProtocolPacket::deserializePacket: version " << mPacketHeader.version << std::endl;
    //std::cout.flush();

    if (firstByte & 0x08u)
    {
        mPacketHeader.compress = true;
    }
    else
    {
        mPacketHeader.compress = false;
    }

    mPacketHeader.frameType = firstByte & 0x07u;

    //std::cout << "ProtocolPacket::deserializePacket: frameType " << mPacketHeader.frameType << std::endl;
    //std::cout.flush();

    mPacketHeader.serviceType = message[offset++];
    mPacketHeader.frameData = message[offset++];
    mPacketHeader.sessionID = message[offset++];

    //std::cout << "ProtocolPacket::deserializePacket: sessionID " << mPacketHeader.sessionID << std::endl;
    //std::cout.flush();

    mPacketHeader.dataSize  = message[offset++] << 24u;
    mPacketHeader.dataSize |= message[offset++] << 16u;
    mPacketHeader.dataSize |= message[offset++] << 8u;
    mPacketHeader.dataSize |= message[offset++];

    //std::cout << "ProtocolPacket::deserializePacket: dataSize " << mPacketHeader.dataSize << std::endl;
    //std::cout.flush();

    if (mPacketHeader.version == PROTOCOL_VERSION_2)
    {
        mPacketHeader.messageId  = message[offset++] << 24u;
        mPacketHeader.messageId |= message[offset++] << 16u;
        mPacketHeader.messageId |= message[offset++] << 8u;
        mPacketHeader.messageId |= message[offset++];
    }
    else
    {
        mPacketHeader.messageId = 0u;
    }

    const unsigned int dataPayloadSize = messageSize - offset;

    if (dataPayloadSize != mPacketHeader.dataSize)
    {
        return RESULT_FAIL;
    }

    unsigned char * data = 0;
    if (dataPayloadSize != 0u)
    {
        data = new unsigned char[messageSize - offset];
        memcpy(data, message + offset, dataPayloadSize);
    }

    mPacketData.data = data;
    mPacketData.totalDataBytes = dataPayloadSize;
    return RESULT_OK;
}

unsigned char ProtocolPacket::getVersion() const
{
    return mPacketHeader.version;
}

bool ProtocolPacket::getIfCompress() const
{
    return mPacketHeader.compress;
}

unsigned char ProtocolPacket::getFrameType() const
{
    return mPacketHeader.frameType;
}

unsigned char ProtocolPacket::getServiceType() const
{
    return mPacketHeader.serviceType;
}

unsigned char ProtocolPacket::getFrameData() const
{
    return mPacketHeader.frameData;
}

unsigned char ProtocolPacket::getSessionId() const
{
    return mPacketHeader.sessionID;
}

unsigned int ProtocolPacket::getDataSize() const
{
    return mPacketHeader.dataSize;
}

unsigned int ProtocolPacket::getMessageId() const
{
    return mPacketHeader.messageId;
}

unsigned char * ProtocolPacket::getData() const
{
    return mPacketData.data;
}

void ProtocolPacket::setTotalDataBytes(unsigned int dataBytes)
{
    if ( dataBytes )
    {
        if ( mPacketData.data )
        {
            delete [] mPacketData.data;
            mPacketData.data = 0;
        }
        mPacketData.data = new unsigned char[dataBytes];
        mPacketData.totalDataBytes = dataBytes;
    }
}

unsigned int ProtocolPacket::getTotalDataBytes() const
{
    return mPacketData.totalDataBytes;
}
/*End of Deserialization*/
