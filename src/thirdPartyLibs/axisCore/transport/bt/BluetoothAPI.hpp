#ifndef BLUETOOTHAPI_HPP_
#define BLUETOOTHAPI_HPP_

#include "Blob.hpp"
#include "IBluetoothHandler.hpp"

namespace Bluetooth
{
	void initBluetooth(IBluetoothHandler * pHandler);
	void deinitBluetooth();

    const Blob getBuffer();
	void releaseBuffer(Blob&);

	void sendBuffer(UInt8 * pBuffer, size_t size);
}

#endif /* BLUETOOTHAPI_HPP_ */
