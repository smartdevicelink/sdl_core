#ifndef __TRANSPORTMANAGER_CBLUETOOTHADAPTER_HPP__
#define __TRANSPORTMANAGER_CBLUETOOTHADAPTER_HPP__

#include "Logger.hpp"

#include "IDeviceAdapter.hpp"
#include "IHandleGenerator.hpp"

namespace AppLink
{
    namespace TransportManager
    {
        class IDeviceAdapterListener;

        /**
         * @brief Bluetooth device adapter.
         **/
        class CBluetoothAdapter: public IDeviceAdapter
        {
        public:
            /**
             * @brief Constructor.
             *
             * @param Listener Listener for device adapter notifications.
             * @param HandleGenerator Handle generator implementation.
             **/
            CBluetoothAdapter(IDeviceAdapterListener & Listener, IHandleGenerator & HandleGenerator);

            /**
             * @brief Destructor.
             **/
            virtual ~CBluetoothAdapter(void);

            /**
             * @brief Run device adapter.
             *
             * Called from transport manager to start device adapter.
             **/
            virtual void run(void);

            /**
             * @brief Get list of available devices.
             *
             * @param DeviceList Reference to a variable that will
             *                   receive device list.
             **/
            virtual void getDeviceList(std::vector<SInternalDeviceInfo> & DeviceList) const;

            /**
             * @brief Get type of devices handled by this device adapter.
             *
             * @return Device type.
             **/
            virtual EDeviceType getDeviceType(void) const;

            /**
             * @brief Send frame.
             *
             * @param ConnectionHandle Connection handle.
             * @param Data Frame payload data.
             *
             * @return Frame sequence number. May be used to identify
             *         this frame when send result callback is received.
             **/
            virtual int sendFrame(tConnectionHandle ConnectionHandle, const Blob & Data);

        private:
            /**
             * @brief Listener for device adapter notifications.
             **/
            IDeviceAdapterListener & mListener;

            /**
             * @brief Handle generator implementation.
             **/
            IHandleGenerator & mHandleGenerator;

            /**
             * @brief Logger.
             **/
            const log4cplus::Logger mLogger;
        };
    }
}

#endif
