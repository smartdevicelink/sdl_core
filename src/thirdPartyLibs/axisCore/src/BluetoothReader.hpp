#ifndef BLUETOOTHREADER_HPP
#define BLUETOOTHREADER_HPP

#include "ProtocolPacketHeader.hpp"

namespace NsTransportLayer
{
    class CBTAdapter;
}

class BluetoothReader
{
public:
    BluetoothReader();

    ~BluetoothReader();

    ERROR_CODE read(ProtocolPacketHeader &header, UInt8 *data, UInt32 dataSize);

    void setBTAdapter(NsTransportLayer::CBTAdapter *adapter);

private:
    UInt8 *mData;
    NsTransportLayer::CBTAdapter *mBTAdapter;
};

#endif // BLUETOOTHREADER_HPP
