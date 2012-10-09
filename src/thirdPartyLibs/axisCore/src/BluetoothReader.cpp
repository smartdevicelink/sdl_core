#include <memory.h>

#include "BluetoothReader.hpp"

int maxsize = 5000;

BluetoothReader::BluetoothReader()
{
    //TODO maxsize

    mData = new UInt8(maxsize);
}

BluetoothReader::~BluetoothReader()
{
    delete [] mData;
}

ERROR_CODE BluetoothReader::read(ProtocolPacketHeader &header, UInt8 *data)
{
    //TODO read

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
    memcpy(&header.dataSize, mData + offset, sizeof(UInt8) );
    offset += sizeof(UInt8);
    memcpy(&header.messageID, mData + offset, sizeof(UInt32) );
    offset += sizeof(UInt32);

    if (data)
        memcpy(data, mData + offset, header.dataSize);

    return ERR_OK;
}
