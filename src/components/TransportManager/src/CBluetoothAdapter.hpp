#ifndef __TRANSPORTMANAGER_CBLUETOOTHADAPTER_HPP__
#define __TRANSPORTMANAGER_CBLUETOOTHADAPTER_HPP__

#include <bluetooth/bluetooth.h>
#include <pthread.h>

#include "Logger.hpp"

#include "TransportManager/SDeviceInfo.hpp"

#include "IDeviceAdapter.hpp"
#include "IHandleGenerator.hpp"

namespace NsAppLink
{
    namespace NsTransportManager
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
            virtual void getDeviceList(tInternalDeviceList & DeviceList) const;

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
             * @brief Internal structure describing bluetooth device.
             **/
            struct SBluetoothDevice
            {
                /**
                 * @brief Default constructor.
                 **/
                SBluetoothDevice(void);

                /**
                 * @brief Copy constructor.
                 *
                 * @param Other Bluetooth device that must be copied.
                 **/
                SBluetoothDevice(const SBluetoothDevice & Other);

                /**
                 * @brief Constructor.
                 *
                 * @param Address Bluetooth address.
                 * @param Name User-friendly device name.
                 * @param AppLinkRFCOMMPorts List of RFCOMM ports where AppLink service has been discovered.
                 **/
                SBluetoothDevice(const bdaddr_t & Address, const char * Name, const std::vector<uint8_t> & AppLinkRFCOMMPorts);

                /**
                 * @brief Device bluetooth address.
                 **/
                bdaddr_t mAddress;

                /**
                 * @brief Device user-friendly name.
                 **/
                std::string mName;

                /**
                 * @brief List of RFCOMM ports where AppLink service has been discovered.
                 **/
                std::vector<uint8_t> mAppLinkRFCOMMPorts;
            };

            /**
             * @brief Bluetooth devices map.
             **/
            typedef std::map<tDeviceHandle, SBluetoothDevice> tBluetoothDevicesMap;

            /**
             * @brief Start routine for device discovery thread.
             *
             * @param Data Must be pointer to CBluetoothAdapter instance.
             *
             * @return Thread return value.
             **/
            static void * deviceDiscoveryThreadStartRoutine(void * Data);

            /**
             * @brief Device discovery thread.
             **/
            void deviceDiscoveryThread(void);

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

            /**
             * @brief Shutdown flag.
             *
             * This flag is set to true on shutdown to inform all bluetooth adapter
             * threads that bluetooth adapter shutdown is in progress. After setting
             * this flag bluetooth adapter waits until all its threads are terminated.
             **/
            bool mShutdownFlag;

            /**
             * @brief ID of device discovery thread.
             **/
            pthread_t mDeviceDiscoveryThread;

            /**
             * @brief Flag indicating whether the device discovery thread has been started successfully.
             **/
            bool mDeviceDiscoveryThreadStarted;

            /**
             * @brief Map of device handle to bluetooth device.
             *
             * This map contains all currently available bluetooth devices.
             **/
            tBluetoothDevicesMap mDevices;

            /**
             * @brief Mutex restricting access to device map.
             **/
            mutable pthread_mutex_t mDevicesMutex;
        };
    }
}

#endif
