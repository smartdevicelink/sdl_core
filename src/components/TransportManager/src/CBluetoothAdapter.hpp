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
             * @brief Start scanning for new devices.
             *
             * List of new devices will be supplied in onDeviceListUpdated callback.
             **/
            virtual void scanForNewDevices(void);

            /**
             * @brief Connect to all applications discovered on device.
             *
             * @param DeviceHandle Handle of device to connect to.
             **/
            virtual void connectDevice(const tDeviceHandle DeviceHandle);

            /**
             * @brief Disconnect from all applications connected on device.
             *
             * @param DeviceHandle Handle of device to disconnect from.
             **/
            virtual void disconnectDevice(const tDeviceHandle DeviceHandle);

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
             * @param Data Frame data.
             * @param DataSize Size of data in bytes.
             *
             * @return Frame sequence number. May be used to identify
             *         this frame when send result callback is received.
             **/
            virtual int sendFrame(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize);

        private:
            /**
             * @brief Frame.
             *
             * Used to store data frames that must be sent to remote device.
             **/
            struct SFrame
            {
                /**
                 * @brief Constructor.
                 *
                 * @param Data Frame data. SFrame stores a copy of this data,
                 *             i.e. data may be freed after SFrame object is constructed.
                 * @param DataSize Size of frame data in bytes.
                 **/
                SFrame(const uint8_t * Data, const size_t DataSize);

                /**
                 * @brief Destructor.
                 *
                 * Frees stored frame data.
                 **/
                ~SFrame(void);

                /**
                 * @brief Frame data.
                 **/
                uint8_t * mData;

                /**
                 * @brief Frame data size in bytes.
                 **/
                size_t mDataSize;
            };

            /**
             * @brief Map of frame sequence number to frame data.
             **/
            typedef std::map<int, SFrame*> tFrameMap;

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
                 * @param AppLinkRFCOMMChannels List of RFCOMM channels where AppLink service has been discovered.
                 **/
                SBluetoothDevice(const bdaddr_t & Address, const char * Name, const std::vector<uint8_t> & AppLinkRFCOMMChannels);

                /**
                 * @brief Device bluetooth address.
                 **/
                bdaddr_t mAddress;

                /**
                 * @brief Device user-friendly name.
                 **/
                std::string mName;

                /**
                * @brief Unique device identifier across all devices.
                **/
                std::string mUniqueDeviceId;

                /**
                 * @brief List of RFCOMM channels where AppLink service has been discovered.
                 **/
                std::vector<uint8_t> mAppLinkRFCOMMChannels;
            };

            /**
             * @brief RFCOMM connection.
             **/
            struct SRFCOMMConnection
            {
                /**
                 * @brief Constructor.
                 *
                 * @param DeviceHandle Device handle.
                 * @param RFCOMMChannel RFCOMM channel of AppLink service on remote device.
                 **/
                SRFCOMMConnection(const tDeviceHandle DeviceHandle, const uint8_t RFCOMMChannel);

                /**
                 * @brief Destructor.
                 *
                 * Clears map of frames to send.
                 **/
                ~SRFCOMMConnection(void);

                /**
                 * @brief Device handle.
                 **/
                const tDeviceHandle mDeviceHandle;

                /**
                 * @brief RFCOMM channel of remote device.
                 **/
                const uint8_t mRFCOMMChannel;

                /**
                 * @brief Thread that handles connection.
                 **/
                pthread_t mConnectionThread;

                /**
                 * @brief File descriptors of notification pipe.
                 *
                 * Notification pipe is used to wake up connection thread
                 * on external event (e.g. new data is available to send or
                 * connection is requested to be terminated).
                 *
                 * mNotificationPipeFds[0] is a descriptor of the read end of the pipe
                 * (the one that is used in poll() by connection thread) and
                 * mNotificationPipeFds[1] is a descriptor of the write end of the pipe
                 * (the one that is used in methods exposed to transport manager to
                 * wake up connection thread when necessary).
                 *
                 * @note eventfd cannot be used instead of a pipe because it does not
                 *       conform to POSIX (eventfd is Linux-specific).
                 **/
                int mNotificationPipeFds[2];

                /**
                 * @brief Sequence number for next frame.
                 **/
                int mNextFrameSequenceNumber;

                /**
                 * @brief Map of frames that must be sent to remote device.
                 **/
                tFrameMap mFramesToSend;

                /**
                 * @brief Terminate flag.
                 *
                 * This flag is set to notify connection thread that connection
                 * must be closed and connection thread must be terminated.
                 **/
                bool mTerminateFlag;
            };

            /**
             * @brief Parameters for establishing RFCOMM connection.
             *
             * Used to pass connection parameters to connection thread.
             **/
            struct SRFCOMMConnectionParameters
            {
                /**
                 * @brief Constructor.
                 *
                 * @param BluetoothAdapter Reference to bluetooth adapter.
                 * @param ConnectionHandle Connection handle.
                 **/
                SRFCOMMConnectionParameters(CBluetoothAdapter & BluetoothAdapter, tConnectionHandle ConnectionHandle);

                /**
                 * @brief Reference to bluetooth adapter.
                 **/
                CBluetoothAdapter & mBluetoothAdapter;

                /**
                 * @brief Connection handle.
                 **/
                tDeviceHandle mConnectionHandle;
            };

            /**
             * @brief Bluetooth devices map.
             **/
            typedef std::map<tDeviceHandle, SBluetoothDevice> tBluetoothDevicesMap;

            /**
             * @brief RFCOMM connection map.
             **/
            typedef std::map<tConnectionHandle, SRFCOMMConnection *> tRFCOMMConnectionMap;

            /**
             * @brief Start RFCOMM connection.
             *
             * Initialize connection structure in RFCOMM connections map
             * and start connection thread for handling communication.
             *
             * @param DeviceHandle Handle of device to connect to.
             * @param RFCOMMChannel RFCOMM channel of AppLink service on remote device.
             **/
            void startRFCOMMConnection(const tDeviceHandle DeviceHandle, const uint8_t RFCOMMChannel);

            /**
             * @brief Stop RFCOMM connection.
             *
             * This method only initiates connection termination. It returns immediately
             * without waiting for actual termination of the connection.
             *
             * @param ConnectionHandle Handle of connection to stop.
             **/
            void stopRFCOMMConnection(const tConnectionHandle ConnectionHandle);

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
             * @brief Connection thread start routine.
             *
             * @param Data Must be pointer to SRFCOMMConnectionParameters. Ownership
             *             of connection parameters is passed to connection thread, i.e.
             *             connection thread is responsible for freeing this object.
             *
             * @return Thread return value.
             **/
            static void * connectionThreadStartRoutine(void * Data);

            /**
             * @brief Connection thread.
             *
             * This method is responsible for establishing connection and communicating
             * with remote device via specified connection. It must remove itself from
             * connection map when connection is terminated before terminating connection thread.
             *
             * @param ConnectionHandle Connection handle.
             **/
            void connectionThread(const tConnectionHandle ConnectionHandle);

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

            /**
             * @brief Map of RFCOMM connections.
             **/
            tRFCOMMConnectionMap mRFCOMMConnections;

            /**
             * @brief Mutex restricting access to RFCOMM connections map.
             **/
            mutable pthread_mutex_t mRFCOMMConnectionsMutex;
        };
    }
}

#endif
