#ifndef BLUETOOTHWRITER_HPP
#define BLUETOOTHWRITER_HPP

#include "ProtocolPacketHeader.hpp"

namespace NsTransportLayer
{
    class CBTAdapter;
}

class BluetoothWriter
{
public:
    BluetoothWriter();

    ~BluetoothWriter();

    ERROR_CODE write(const ProtocolPacketHeader &header, UInt8 *data);

    void setBTAdapter(NsTransportLayer::CBTAdapter *adapter);

private:
    UInt8 *mData;
    NsTransportLayer::CBTAdapter *mBTAdapter;

};

#endif // BLUETOOTHWRITER_HPP
