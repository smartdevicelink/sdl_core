#ifndef BLUETOOTHREADER_HPP
#define BLUETOOTHREADER_HPP

#include "ProtocolPacketHeader.hpp"

namespace NsTransportLayer
{
    class CBTAdapter;
}

namespace AxisCore
{

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

} //namespace AxisCore

#endif // BLUETOOTHREADER_HPP
