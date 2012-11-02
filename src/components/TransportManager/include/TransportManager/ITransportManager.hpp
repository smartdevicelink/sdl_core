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
             * @brief Get list of available devices.
             *
             * @param DeviceList Reference to a variable that will
             *                   receive device list.
             **/
            virtual void getDeviceList(tDeviceList & DeviceList) const = 0;

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
             *
             * @return Frame sequence number. May be used to identify
             *         this frame when send result callback is received.
             **/
            virtual int sendFrame(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize) = 0;
        };
    }
}

#endif
