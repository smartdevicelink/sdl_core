#ifndef __TRANSPORTMANAGER_IDEVICEADAPTER_HPP__
#define __TRANSPORTMANAGER_IDEVICEADAPTER_HPP__

#include <vector>

#include "TransportManager/ITransportManager.hpp"
#include "SInternalDeviceInfo.hpp"

namespace AppLink
{
    namespace TransportManager
    {
        /**
         * @brief Interface for device adapter.
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
             * @brief Get list of available devices.
             *
             * @param DeviceList Reference to a variable that will
             *                   receive device list.
             **/
            virtual void getDeviceList(std::vector<SInternalDeviceInfo> & DeviceList) const = 0;

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
             * @param Data Frame payload data.
             *
             * @return Frame sequence number. May be used to identify
             *         this frame when send result callback is received.
             **/
            virtual int sendFrame(tConnectionHandle ConnectionHandle, const Blob & Data) = 0;
        };
    }
}

#endif
