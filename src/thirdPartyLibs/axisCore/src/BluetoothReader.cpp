#include <memory.h>
#include <iostream>

#include "BluetoothReader.hpp"
#include "transport/bt/BluetoothAPI.hpp"

int maxsize = 5000;

BluetoothReader::BluetoothReader()
{
    //TODO maxsize

    mData = new UInt8[maxsize];
}

BluetoothReader::~BluetoothReader()
{
    delete [] mData;
}

ERROR_CODE BluetoothReader::read(ProtocolPacketHeader &header, UInt8 *data, UInt32 dataSize)
{
    std::cout << "BluetoothReader::read raw buffer : " << CMessage::currentBlob.buffer() << "\n";
    UInt32 blobBufferSize = CMessage::currentBlob.size();//Bluetooth::getBuffer().size();
    if (dataSize >= blobBufferSize )
    {
        memcpy(mData, CMessage::currentBlob.buffer() /*Bluetooth::getBuffer().buffer()*/, blobBufferSize);
    }
    else
    {
        std::cout << "BluetoothReader::read buffer is too small for reading\n";
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
    memcpy(&header.dataSize, mData + offset, sizeof(UInt8) );
    offset += sizeof(UInt8);
    memcpy(&header.messageID, mData + offset, sizeof(UInt32) );
    offset += sizeof(UInt32);

    if (data)
        memcpy(data, mData + offset, header.dataSize);

    return ERR_OK;
}
