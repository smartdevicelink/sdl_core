#ifndef BLUETOOTHWRITER_HPP
#define BLUETOOTHWRITER_HPP

#include "ProtocolPacketHeader.hpp"

namespace Bluetooth
{
    class IBluetoothAPI;
}

namespace AxisCore
{

/**
 * \class BluetoothWriter
 * \brief Class for writing to CBTAdapter
 * \author amarkosov
 */
class BluetoothWriter
{
public:
    BluetoothWriter();

    ~BluetoothWriter();

    /**
      * Write data to CBTAdapter
      * @param header Message header
      * @param data Data array
      */
    ERROR_CODE write(const ProtocolPacketHeader &header, UInt8 *data);

    /**
      * Set BTAdapter pointer for writing
      * @param adapter Pointer to NsTransportLayer::CBTAdapter
      */
    void setBTAdapter(Bluetooth::IBluetoothAPI *adapter);

private:
    UInt8 *mData;
    Bluetooth::IBluetoothAPI *mBTAdapter;
};

} //namespace AxisCore

#endif // BLUETOOTHWRITER_HPP
