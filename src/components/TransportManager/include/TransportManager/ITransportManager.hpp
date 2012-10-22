#ifndef __TRANSPORTMANAGER_ITRANSPORTMANAGER_HPP__
#define __TRANSPORTMANAGER_ITRANSPORTMANAGER_HPP__

#include <vector>

#include "TransportManager/SDeviceInfo.hpp"

namespace AppLink
{
    namespace TransportManager
    {
        class ITransportManagerListener;
        class Blob;

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
            virtual void getDeviceList(std::vector<SDeviceInfo> & DeviceList) const = 0;

            /**
             * @brief Add listener.
             *
             * @param Listener Pointer to listener.
             **/
            virtual void addListener(ITransportManagerListener * Listener) = 0;

            /**
             * @brief Remove listener.
             *
             * @param Listener Pointer to listener.
             **/
            virtual void removeListener(ITransportManagerListener * Listener) = 0;

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
