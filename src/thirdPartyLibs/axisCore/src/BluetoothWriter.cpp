#include <memory.h>

#include "BluetoothWriter.hpp"
#include "../../../appMain/CBTAdapter.hpp"
#include <stdio.h>

namespace AxisCore
{

int _maxsize = 5000;

BluetoothWriter::BluetoothWriter()
{
    //TODO maxsize
    mBTAdapter = NULL;
    mData = new UInt8[_maxsize];
}

BluetoothWriter::~BluetoothWriter()
{
    delete [] mData;
}

void BluetoothWriter::setBTAdapter(NsTransportLayer::CBTAdapter *adapter)
{
    mBTAdapter = adapter;
}

ERROR_CODE BluetoothWriter::write(const ProtocolPacketHeader &header, UInt8 *data)
{
    UInt8 offset = 0;
    UInt8 compress = 0x0;
    if (header.compress)
        compress = 0x1;
    UInt8 firstByte = ( (header.version << 4) & 0xF0 )
                      | ( (compress << 3) & 0x08)
                      | (header.frameType & 0x07);
    memcpy(mData, &firstByte, sizeof(UInt8) );
    offset += sizeof(UInt8);
    memcpy(mData + offset, &header.serviceType, sizeof(UInt8) );
    offset += sizeof(UInt8);
    memcpy(mData + offset, &header.frameData, sizeof(UInt8) );
    offset += sizeof(UInt8);
    memcpy(mData + offset, &header.sessionID, sizeof(UInt8) );
    offset += sizeof(UInt8);

    //memcpy(mData + offset, &header.dataSize, sizeof(UInt32) );
    //offset += sizeof(UInt32);

    UInt8 tmp = header.dataSize >> 24;
    memcpy(mData + offset++, &tmp, sizeof(UInt8) );
    tmp = header.dataSize >> 16;
    memcpy(mData + offset++, &tmp, sizeof(UInt8) );
    tmp = header.dataSize >> 8;
    memcpy(mData + offset++, &tmp, sizeof(UInt8) );
    tmp = header.dataSize;
    memcpy(mData + offset++, &tmp, sizeof(UInt8) );

    if (data)
    {
        if ( (offset + header.dataSize) < _maxsize)
            memcpy(mData + offset, data, header.dataSize);
        else
            return ERR_FAIL;
    }

    if (mBTAdapter)
        mBTAdapter->sendBuffer(mData, header.dataSize + PROTOCOL_HEADER_SIZE);

    return ERR_OK;
}

} //namespace AxisCore
