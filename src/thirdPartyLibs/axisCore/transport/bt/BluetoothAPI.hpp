#ifndef BLUETOOTHAPI_HPP_
#define BLUETOOTHAPI_HPP_

#include "Blob.hpp"
#include "IBluetoothHandler.hpp"

namespace Bluetooth
{
    class IBluetoothAPI
    {
    public:
        virtual void initBluetooth(IBluetoothHandler * pHandler) = 0;
        virtual void deinitBluetooth() = 0;

        virtual const Blob getBuffer() = 0;
        virtual void releaseBuffer(const Blob&) = 0;

        virtual void sendBuffer(UInt8 * pBuffer, size_t size) = 0;

        virtual ~IBluetoothAPI() {}
    };
}

#endif /* BLUETOOTHAPI_HPP_ */
