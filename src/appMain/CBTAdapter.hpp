/**
 * \file CBTAdapter.hpp
 * \brief Bluetooth adapter for AppLink.
 * \author AKara
 */

#ifndef BTADAPTER_H 
#define BTADAPTER_H 

#include <stdint.h>
#include <string>
#include <vector>

#include "Types.hpp"
#include "Blob.hpp"
#include "IBluetoothHandler.hpp"

/**
 * \namespace NsTransportLayer
 * \brief AppLink transport layer related functions.
 */ 
namespace NsTransportLayer
{
   /**
    * \class CBTDevice
    * \brief Bluetooth adapter for AppLink
    */
   class CBTDevice
   {
   public:
      CBTDevice(std::string DeviceName, std::string DeviceAddr):
      mDeviceName(DeviceName),
      mDeviceAddr(DeviceAddr)
      {
      }
      ~CBTDevice()
      {
      }
      void setDeviceName(std::string DeviceName)
      {
        mDeviceName = DeviceName;
      }
      void setDeviceAddr(std::string DeviceAddr)
      {
        mDeviceAddr = DeviceAddr;
      }
      std::string getDeviceName()
      {        
        return mDeviceName;
      }
      std::string getDeviceAddr()
      {
        return mDeviceAddr;
      }
    private:
      std::string mDeviceName;
      std::string mDeviceAddr;
    };
   /**
    * \class CBTAdapter
    * \brief Bluetooth adapter for AppLink
    */
   class CBTAdapter
   {
      public:
        /**
        * \brief Constructor.
        */
        CBTAdapter();

        /**
        * \brief Destructor.
        */
        ~CBTAdapter();

        int scanDevices(std::vector<CBTDevice>& devicesFound);
        int startSDPDiscoveryOnDevice(const char* targetDevice, std::vector<int>& portsRFCOMMFound);
        int startRFCOMMConnection(const char* targetDevice, int portRFCOMM);
        int processRFCOMM(int sockid);

        void initBluetooth(Bluetooth::IBluetoothHandler * pHandler);
        void deinitBluetooth();

        const Blob getBuffer();
        void releaseBuffer(const Blob& blob);

        void sendBuffer(UInt8 * pBuffer, size_t size);


   };
}/* namespace NsTransportLayer */
#endif /* BTADAPTER_H */