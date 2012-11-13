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
#include <queue>

#include "Types.hpp"
#include "Blob.hpp"
#include "IBluetoothHandler.hpp"
#include "BluetoothAPI.hpp"
#include "Logger.hpp"

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
     /**
     * \brief CBTDevice class constructor.
     * \class CBTDevice - container to store information about the device.
     * \param DeviceName human readable device name
     * \param DeviceAddr bt device physical address
     */
      CBTDevice(std::string DeviceName, std::string DeviceAddr):
      mDeviceName(DeviceName),
      mDeviceAddr(DeviceAddr)
      {
      }

      /**
      * \brief CBTDevice class destructor.
      */
      ~CBTDevice()
      {
      }

     /**
     * \brief sets string DeviceName.
     * \param DeviceName human readable device name
     */
      void setDeviceName(std::string DeviceName)
      {
        mDeviceName = DeviceName;
      }

     /**
     * \brief sets string DeviceAddr.
     * \param DeviceAddr bt device physical address
     */
      void setDeviceAddr(std::string DeviceAddr)
      {
        mDeviceAddr = DeviceAddr;
      }

     /**
     * \brief gets string DeviceName.
     * \return string DeviceName
     */
      std::string getDeviceName()
      {        
        return mDeviceName;
      }

     /**
     * \brief gets string DeviceAddr.
     * \return string bt device physical address
     */
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
   class CBTAdapter: public Bluetooth::IBluetoothAPI
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

        /**
        * \brief starts discovering of BT devices.
        * \param devicesFound reference to the vector to put results
        * \return Error code (-1 in case of any errors or count of devices in case of success)
        */
        int scanDevices(std::vector<CBTDevice>& devicesFound);

        /**
        * \brief starts discovering of service on BT device.
        * \param targetDevice physical address
        * \param portsRFCOMMFound reference to the vector to put results
        * \return Error code (-1 in case of any errors or count of devices in case of success)
        */
        int startSDPDiscoveryOnDevice(const char* targetDevice, std::vector<int>& portsRFCOMMFound);

        /**
        * \brief Opens socket to the port of device.
        * \param targetDevice physical address of device to connect
        * \param portRFCOMM port to connect
        * \return Error code (-1 in case of any errors)
        */
        int startRFCOMMConnection(const char* targetDevice, int portRFCOMM);

        /**
        * \brief Process data from RFCOMM socket.
        * \param sockid handle of opened socket.
        * \return Error code (-1 in case of any errors)
        */
        int processRFCOMM(int sockid);

        /**
        * \brief inits BTAdapter. Gives pointer to ProtocolHandler.
        * \param pHandler pointer to ProtocolHandler.
        */
        void initBluetooth(Bluetooth::IBluetoothHandler * pHandler);

        /**
        * \brief deinits BTAdapter.
        */
        void deinitBluetooth();

        /**
        * \brief gets Blob from BTAdapter que.
        * \return Blob with data from the que.
        */
        const Blob getBuffer();

        /**
        * \brief releases data from Blob and deletes it from que.
        * \param Blob with data from the que reference.
        */
        void releaseBuffer(const Blob& blob);

        /**
        * \brief Sends data to opened socket.
        * \param pBuffer pointer to the buffer with data to send.
        * \param size size of data frame.
        */
        void sendBuffer(UInt8 * pBuffer, size_t size);
      private:
        static log4cplus::Logger mLogger;

        Bluetooth::IBluetoothHandler * mpProtocolHandler;

        std::queue<Blob> blobQueue;

        int sockID;
   };
}/* namespace NsTransportLayer */
#endif /* BTADAPTER_H */