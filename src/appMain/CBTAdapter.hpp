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

        int setClass(unsigned int cls, int timeout);
        int registerSDP(uint8_t channel);
        int rfcommListen(uint8_t channel, char *targetDevID);
        int scoListen();
        int handleConnection(int rfcommsock, int scosock);
        int processRFCOMM(int sockid);
        int startSDPDiscoveryOnDevice(const char* targetDevice);
        int scanDevices(std::vector<CBTDevice>& devicesFound);


   };
}/* namespace NsTransportLayer */
#endif /* BTADAPTER_H */