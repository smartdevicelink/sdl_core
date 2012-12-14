#ifndef __TRANSPORTMANAGER_ITRANSPORTMANAGER_HPP__
#define __TRANSPORTMANAGER_ITRANSPORTMANAGER_HPP__

#include <stdint.h>
#include <sys/types.h>
#include <vector>

#include "TransportManager/SDeviceInfo.hpp"

namespace NsAppLink
{
    namespace NsTransportManager
    {
        class ITransportManagerDeviceListener;
        class ITransportManagerDataListener;

        /**
         * @brief Connection handle.
         **/
        typedef int tConnectionHandle;

        /**
         * @brief Special value of connection handle for indicating invalid connection handle.
         **/
        const tConnectionHandle InvalidConnectionHandle = static_cast<tConnectionHandle>(-1);

        /**
         * @brief Interface of transport manager.
         * @interface ITransportManager
         **/
        class ITransportManager
        {
        public:
            /**
             * @brief Destructor.
             **/
            virtual ~ITransportManager(void);

            /**
             * @brief Create trasport manager.
             *
             * @return Pointer to created transport manager instance.
             **/
            static ITransportManager * create(void);

            /**
             * @brief Run transport manager.
             *
             * Must be called from startup after all references to
             * external components are initialized to start trasport manager.
             **/
            virtual void run(void) = 0;

            /**
             * @brief Start scanning for new devices.
             *
             * List of new devices will be supplied in onDeviceListUpdated callback.
             *
             * @see @ref components_transportmanager_client_device_management
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
             * @brief Add listener to the data-related events
             *
             * @param Listener Pointer to listener.
             **/
            virtual void addDataListener(ITransportManagerDataListener * Listener) = 0;

            /**
             * @brief Remove listener to the data-related events.
             *
             * @param Listener Pointer to listener.
             **/
            virtual void removeDataListener(ITransportManagerDataListener * Listener) = 0;

            /**
             * @brief Add listener to the device-related events.
             *
             * @param Listener Pointer to listener.
             **/
            virtual void addDeviceListener(ITransportManagerDeviceListener * Listener) = 0;

            /**
             * @brief Remove listenerto the device-related events.
             *
             * @param Listener Pointer to listener.
             **/
            virtual void removeDeviceListener(ITransportManagerDeviceListener * Listener) = 0;

            /**
             * @brief Send frame.
             *
             * @param ConnectionHandle Connection handle.
             * @param Data Frame payload data.
             * @param DataSize Size of data in bytes.
             * @param UserData Any user data. Will be returned as is in ITransportManagerDataListener::onFrameSendCompleted
             **/
            virtual void sendFrame(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize, const int UserData) = 0;
        };
    }
}

#endif
