#include <memory.h>
#include <stdio.h>
#include <iostream>

#include "BluetoothWriter.hpp"
#include "../../../appMain/CBTAdapter.hpp"

namespace AxisCore
{

BluetoothWriter::BluetoothWriter()
{
    mBTAdapter = NULL;
    mData = new UInt8[MAXIMUM_FRAME_SIZE];
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

    mData[offset++] = header.dataSize >> 24;
    mData[offset++] = header.dataSize >> 16;
    mData[offset++] = header.dataSize >> 8;
    mData[offset++] = header.dataSize;

    if (data)
    {
        if ( (offset + header.dataSize) < MAXIMUM_FRAME_SIZE)
            memcpy(mData + offset, data, header.dataSize);
        else
        {
            printf("%s:%d BluetoothWriter::write() buffer is too small for writing\n", __FILE__, __LINE__);
            return ERR_FAIL;
        }
    }

    if (mBTAdapter)
        mBTAdapter->sendBuffer(mData, header.dataSize + PROTOCOL_HEADER_SIZE);
    else
        return ERR_FAIL;

    //Bluetooth::sendBuffer(mData, header.dataSize + PROTOCOL_HEADER_SIZE);

    return ERR_OK;
}

} //namespace AxisCore
