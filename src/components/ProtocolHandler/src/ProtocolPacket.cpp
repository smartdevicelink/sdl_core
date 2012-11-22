#include "ProtocolHandler/ProtocolPacket.h"

using namespace NsProtocolHandler;

ProtocolPacket::ProtocolPacket() :
mHeader( 0 )
,mSize( 0 )
{}

ProtocolPacket::ProtocolPacket(unsigned char version,
                             bool compress,
                             unsigned char frameType,
                             unsigned char serviceType,
                             unsigned char frameData,
                             unsigned char sessionID,
                             unsigned int dataSize,
                             unsigned int messageID
                             const unsigned char * data) :
mHeader( 0 )
,mSize( 0 )
{
    unsigned char offset = 0;
    unsigned char compressF = 0x0;
    mHeader = new unsigned char[MAXIMUM_FRAME_SIZE];
    if (compress)
    {
        compressF = 0x1;
    }
    unsigned char firstByte = ( (version << 4) & 0xF0 )
                      | ( (compressF << 3) & 0x08)
                      | (frameType & 0x07);
         
    mHeader[offset++] = firstByte;
    mHeader[offset++] = serviceType;    
    mHeader[offset++] = frameData;    
    mHeader[offset++] = sessionID;

    mHeader[offset++] = dataSize >> 24;
    mHeader[offset++] = dataSize >> 16;
    mHeader[offset++] = dataSize >> 8;
    mHeader[offset++] = dataSize;

    if (version == PROTOCOL_VERSION_2)
    {
        mHeader[offset++] = messageID >> 24;
        mHeader[offset++] = messageID >> 16;
        mHeader[offset++] = messageID >> 8;
        mHeader[offset++] = messageID;
    }

    if (data)
    {
        if ( (offset + dataSize) <= MAXIMUM_FRAME_SIZE)
        {
            memcpy(mHeader + offset, data, dataSize);
            mSize = offset + dataSize;
        }            
        else
        {
            delete [] mHeader;
            mHeader = 0;
            mSize = 0;
        }
    }
}

ProtocolPacket::~ProtocolPacket()
{
    //TODO: where to clean?
    mHeader = 0;
    mSize = 0;
}

unsigned char * ProtocolPacket::getPacket() const
{
    return mHeader;
}
    
unsigned int ProtocolPacket::getPacketSize() const
{
    return mSize;
}

void ProtocolPacket::appendData( unsigned char * chunkData, 
        unsigned int chunkDataSize )
{
    if ( mDataOffset + chunkDataSize <= totalDataBytes )
    {
        memcpy(data + mDataOffset, chunkData, chunkDataSize);
        mDataOffset += chunkDataSize;
    }
}