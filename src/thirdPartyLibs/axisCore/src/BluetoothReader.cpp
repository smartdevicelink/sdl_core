#include <memory.h>
#include <iostream>
#include <stdio.h>

#include "BluetoothReader.hpp"
#include "../../../appMain/CBTAdapter.hpp"

namespace AxisCore
{

Logger BluetoothReader::mLogger = Logger::getInstance(LOG4CPLUS_TEXT("AxisCore.ProtocolHandler") );

BluetoothReader::BluetoothReader()
{    
    mBTAdapter = NULL;
    mData = new UInt8[MAXIMUM_FRAME_SIZE];
}

BluetoothReader::~BluetoothReader()
{
    delete [] mData;
}

void BluetoothReader::setBTAdapter(Bluetooth::IBluetoothAPI *adapter)
{
    mBTAdapter = adapter;
}

ERROR_CODE BluetoothReader::read(ProtocolPacketHeader &header, UInt8 *data, UInt32 dataSize)
{
    UInt32 blobBufferSize = 0;
    if (mBTAdapter)
        blobBufferSize = mBTAdapter->getBuffer().size();

    if ( (dataSize >= blobBufferSize) && (MAXIMUM_FRAME_SIZE >= blobBufferSize) )
    {
        memcpy(mData, mBTAdapter->getBuffer().buffer(), blobBufferSize);
        mBTAdapter->releaseBuffer(mBTAdapter->getBuffer() );
    }
    else
    {
        LOG4CPLUS_WARN(mLogger, "read() - buffer is too small for reading");
        return ERR_FAIL;
    }

    UInt8 offset = 0;
    UInt8 firstByte = 0;
    memcpy(&firstByte, mData, sizeof(UInt8) );
    offset += sizeof(UInt8);

    header.version = firstByte >> 4;
    if (firstByte & 0x08)
        header.compress = true;
    else
        header.compress = false;

    header.frameType = firstByte & 0x07;

    memcpy(&header.serviceType, mData + offset, sizeof(UInt8) );
    offset += sizeof(UInt8);
    memcpy(&header.frameData, mData + offset, sizeof(UInt8) );
    offset += sizeof(UInt8);
    memcpy(&header.sessionID, mData + offset, sizeof(UInt8) );
    offset += sizeof(UInt8);

    header.dataSize  = mData[offset++] << 24;
    header.dataSize |= mData[offset++] << 16;
    header.dataSize |= mData[offset++] << 8;
    header.dataSize |= mData[offset++];

    if (header.version == PROTOCOL_VERSION_2)
    {
        header.messageID  = mData[offset++] << 24;
        header.messageID |= mData[offset++] << 16;
        header.messageID |= mData[offset++] << 8;
        header.messageID |= mData[offset++];
    }
    else
        header.messageID = 0;

    if (data)
        memcpy(data, mData + offset, header.dataSize);

    return ERR_OK;
}

} //namespace AxisCore
