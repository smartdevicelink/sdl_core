#ifndef __TRANSPORTMANAGER_ITRANSPORTMANAGERDEVICELISTENER_HPP__
#define __TRANSPORTMANAGER_ITRANSPORTMANAGERDEVICELISTENER_HPP__

#include "TransportManager/SDeviceInfo.hpp"
#include "TransportManager/ITransportManager.hpp"

namespace NsAppLink
{
    namespace NsTransportManager
    {
        /**
         * @brief Interface of transport manager listener related to device information.
         *
         * Clients of transport manager must inherit this interface
         * in order to register as listener to receive callbacks about device information
         *
         * @interface ITransportManagerDeviceListener
         *
         * @see @ref components_transportmanager_client_device_management
         * @see @ref components_transportmanager_client_connection_management
         **/
        class ITransportManagerDeviceListener
        {
        public:
            /**
             * @brief Destructor
             *
             **/
            virtual ~ITransportManagerDeviceListener();

            /**
             * @brief Available devices list updated.
             *
             * Called when device scanning initiated with scanForNewDevices
             * is completed.
             *
             * @param DeviceList New list of available devices.
             *
             * @see @ref components_transportmanager_client_device_management
             **/
            virtual void onDeviceListUpdated(const tDeviceList & DeviceList);

            /**
             * @brief Application connected.
             *
             * @param ConnectedDevice DeviceInfo of connected device.
             * @param Connection Connection handle.
             *
             * @see @ref components_transportmanager_client_connection_management
             **/
            virtual void onApplicationConnected(const SDeviceInfo & ConnectedDevice, const tConnectionHandle Connection);

            /**
             * @brief Application disconnected.
             *
             * @param DisconnectedDevice DeviceInfo of disconnected device.
             * @param Connection Connection handle.
             *
             * @see @ref components_transportmanager_client_connection_management
             **/
            virtual void onApplicationDisconnected(const SDeviceInfo & DisconnectedDevice, const tConnectionHandle Connection);
        };
    }
}



#endif // __TRANSPORTMANAGER_ITRANSPORTMANAGERDEVICELISTENER_HPP__
