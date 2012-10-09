#ifndef BLUETOOTHREADER_HPP
#define BLUETOOTHREADER_HPP

#include "ProtocolPacketHeader.hpp"

class BluetoothReader
{
public:
    BluetoothReader();

    ~BluetoothReader();

    ERROR_CODE read(ProtocolPacketHeader &header, UInt8 *data, UInt32 dataSize);

private:
    UInt8 *mData;

};

#endif // BLUETOOTHREADER_HPP
