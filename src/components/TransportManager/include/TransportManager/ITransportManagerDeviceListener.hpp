#ifndef __TRANSPORTMANAGER_ITRANSPORTMANAGERDEVICELISTENER_HPP__
#define __TRANSPORTMANAGER_ITRANSPORTMANAGERDEVICELISTENER_HPP__

#include "TransportManager/SDeviceInfo.hpp"

namespace NsAppLink
{
    namespace NsTransportManager
    {
        /**
         * @brief Interface of transport manager listener related to device information.
         *
         * Clients of transport manager must inherit this interface
         * in order to register as listener to receive callbacks about device information
         **/
        class ITransportManagerDeviceListener
        {
            /**
             * @brief Destructor
             *
             **/
            virtual ~ITransportManagerDeviceListener();

            /**
             * @brief Available devices list updated.
             *
             * @param DeviceList New list of available devices.
             * @return void
             **/
            virtual void onDeviceListUpdated(const tDeviceList & DeviceList);

            /**
             * @brief Device connected.
             *
             * @param ConnectedDevice DeviceInfo of connected device.
             * @return void
             **/
            virtual void onDeviceConnected(const SDeviceInfo & ConnectedDevice);

            /**
             * @brief Device disconnected.
             *
             * @param DisconnectedDevice DeviceInfo of disconnected device.
             * @return void
             **/
            virtual void onDeviceDisconnected(const SDeviceInfo & DisconnectedDevice);
        };
    }
}



#endif // __TRANSPORTMANAGER_ITRANSPORTMANAGERDEVICELISTENER_HPP__
