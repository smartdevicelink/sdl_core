#ifndef IBLUETOOTHHANDLER_HPP_
#define IBLUETOOTHHANDLER_HPP_

#include "BluetoothTypes.hpp"

namespace Bluetooth
{
   class IBlutoothHandler
   {
   public:
      virtual void onError(BLUETOOTH_ERROR errCode) =0;
      virtual void dataReceived() =0;

      virtual ~IBluetoothHandler() {}
   };
}

#endif /* IBLUETOOTHHANDLER_HPP_ */
