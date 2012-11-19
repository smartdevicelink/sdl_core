#ifndef __TRANSPORTMANAGER_IDEVICEADAPTER_HPP__
#define __TRANSPORTMANAGER_IDEVICEADAPTER_HPP__

#include <stdint.h>
#include <sys/types.h>
#include <vector>

#include "TransportManager/ITransportManager.hpp"
#include "SInternalDeviceInfo.hpp"

namespace NsAppLink
{
    namespace NsTransportManager
    {
        /**
         * @brief Interface for device adapter.
         * @interface IDeviceAdapter
         **/
        class IDeviceAdapter
        {
        public:
            /**
             * @brief Destructor.
             **/
            virtual ~IDeviceAdapter(void);

            /**
             * @brief Run device adapter.
             *
             * Called from transport manager to start device adapter.
             **/
            virtual void run(void) = 0;

            /**
             * @brief Start scanning for new devices.
             *
             * List of new devices will be supplied in onDeviceListUpdated callback.
             **/
            virtual void scanForNewDevices(void) = 0;

            /**
             * @brief Connect to all applications discovered on device.
             *
             * @param DeviceHandle Handle of device to connect to.
             **/
            virtual void connectDevice(const tDeviceHandle DeviceHandle) = 0;

            /**
             * @brief Disconnect from all applications connected on device.
             *
             * @param DeviceHandle Handle of device to disconnect from.
             **/
            virtual void disconnectDevice(const tDeviceHandle DeviceHandle) = 0;

            /**
             * @brief Get type of devices handled by this device adapter.
             *
             * @return Device type.
             **/
            virtual EDeviceType getDeviceType(void) const = 0;

            /**
             * @brief Send frame.
             *
             * @param ConnectionHandle Connection handle.
             * @param Data Frame data.
             * @param DataSize Size of data in bytes.
             *
             * @return Frame sequence number. May be used to identify
             *         this frame when send result callback is received.
             **/
            virtual int sendFrame(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize) = 0;
        };
    }
}

#endif
