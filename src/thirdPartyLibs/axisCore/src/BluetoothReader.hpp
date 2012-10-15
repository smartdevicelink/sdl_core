#ifndef BLUETOOTHREADER_HPP
#define BLUETOOTHREADER_HPP

#include "ProtocolPacketHeader.hpp"

namespace NsTransportLayer
{
    class CBTAdapter;
}

namespace AxisCore
{

/**
 * \class BluetoothReader
 * \brief Class for reading from CBTAdapter
 * \author amarkosov
 */
class BluetoothReader
{
public:
    BluetoothReader();

    ~BluetoothReader();

    /**
      * Read data from CBTAdapter
      * @param header Message header
      * @param data Data array
      * @param dataSize Size of data array
      */
    ERROR_CODE read(ProtocolPacketHeader &header, UInt8 *data, UInt32 dataSize);

    /**
      * Set BTAdapter pointer for reading
      * @param adapter Pointer to NsTransportLayer::CBTAdapter
      */
    void setBTAdapter(NsTransportLayer::CBTAdapter *adapter);

private:
    UInt8 *mData;
    NsTransportLayer::CBTAdapter *mBTAdapter;
};

} //namespace AxisCore

#endif // BLUETOOTHREADER_HPP
