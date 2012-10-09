#ifndef BLUETOOTHWRITER_HPP
#define BLUETOOTHWRITER_HPP

#include "ProtocolPacketHeader.hpp"

class BluetoothWriter
{
public:
    BluetoothWriter();

    ~BluetoothWriter();

    ERROR_CODE write(const ProtocolPacketHeader &header, UInt8 *data);

private:
    UInt8 *mData;

};

#endif // BLUETOOTHWRITER_HPP
