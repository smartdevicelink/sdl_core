/**
 * \file CDeviceAdapter.hpp
 * \brief Class CDeviceAdapter header.
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __TRANSPORTMANAGER_CDEVICEADAPTER_HPP__
#define __TRANSPORTMANAGER_CDEVICEADAPTER_HPP__

#include <pthread.h>
#include <queue>
#include <time.h>

#include "Logger.hpp"

#include "IDeviceAdapter.hpp"

#define LOG4CPLUS_ERROR_WITH_ERRNO(logger, message) LOG4CPLUS_ERROR(logger, message << ", error code " << errno << " (" << strerror(errno) << ")")

namespace NsSmartDeviceLink
{
    namespace NsTransportManager
    {
        class IDeviceAdapterListener;
        class IHandleGenerator;

        /**
         * @brief Base class for @link components_transportmanager_internal_design_device_adapters device adapters @endlink.
         **/
        class CDeviceAdapter: public IDeviceAdapter
        {
        public:
            /**
             * @brief Constructor.
             *
             * @param Listener Listener for device adapter notifications.
             * @param HandleGenerator Handle generator implementation.
             **/
            CDeviceAdapter(IDeviceAdapterListener & Listener, IHandleGenerator & HandleGenerator);

            /**
             * @brief Destructor.
             **/
            virtual ~CDeviceAdapter(void);

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
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_device_scan
             **/
            virtual void scanForNewDevices(void);

            /**
             * @brief Connect to all applications discovered on device.
             *
             * @param DeviceHandle Handle of device to connect to.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_connecting_devices
             **/
            virtual void connectDevice(const tDeviceHandle DeviceHandle);

            /**
             * @brief Disconnect from all applications connected on device.
             *
             * @param DeviceHandle Handle of device to disconnect from.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_disconnecting_devices
             **/
            virtual void disconnectDevice(const tDeviceHandle DeviceHandle);

            /**
             * @brief Send frame.
             *
             * @param ConnectionHandle Connection handle.
             * @param Data Frame payload data.
             * @param DataSize Size of data in bytes.
             * @param UserData Any user data.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_handling_communication
             **/
            virtual void sendFrame(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize, int UserData);

        protected:
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
                 * @param UserData User data
                 * @param Data Frame data. SFrame stores a copy of this data,
                 *             i.e. data may be freed after SFrame object is constructed.
                 * @param DataSize Size of frame data in bytes.
                 **/
                SFrame(int UserData, const uint8_t * Data, const size_t DataSize);

                /**
                 * @brief Destructor.
                 *
                 * Frees stored frame data.
                 **/
                ~SFrame(void);

                /**
                 * @brief User data
                 **/
                int mUserData;

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
             * @brief Frame queue.
             **/
            typedef std::queue<SFrame*> tFrameQueue;

            /**
             * @brief Internal structure describing device.
             **/
            struct SDevice
            {
                /**
                 * @brief Constructor.
                 *
                 * @param Name User-friendly device name.
                 **/
                SDevice(const char * Name);

                /**
                 * @brief Destructor.
                 **/
                virtual ~SDevice(void);

                /**
                 * @brief Compare devices.
                 *
                 * This method checks whether two SDevice structures
                 * refer to the same device.
                 *
                 * @param OtherDevice Device to compare with.
                 *
                 * @return true if devices are equal, false otherwise.
                 **/
                virtual bool isSameAs(const SDevice * OtherDevice) const;

                /**
                 * @brief Device user-friendly name.
                 **/
                std::string mName;

                /**
                 * @brief Unique device identifier across all devices.
                 **/
                std::string mUniqueDeviceId;
            };

            /**
             * @brief Vector of devices.
             **/
            typedef std::vector<SDevice *> tDeviceVector;

            /**
             * @brief Devices map.
             **/
            typedef std::map<tDeviceHandle, SDevice *> tDeviceMap;

            /**
             * @brief Application connection.
             **/
            struct SConnection
            {
                /**
                 * @brief Constructor.
                 *
                 * @param DeviceHandle Device handle.
                 **/
                SConnection(const tDeviceHandle DeviceHandle);

                /**
                 * @brief Destructor.
                 *
                 * Clears map of frames to send.
                 **/
                virtual ~SConnection(void);

                /**
                 * @brief Compare connections.
                 *
                 * This method checks whether two SConnection structures
                 * refer to the same connection.
                 *
                 * @param OtherConnection Connection to compare with.
                 *
                 * @return true if connections are equal, false otherwise.
                 **/
                virtual bool isSameAs(const SConnection * OtherConnection) const;

                /**
                 * @brief Device handle.
                 **/
                const tDeviceHandle mDeviceHandle;

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
                 * @brief Descriptor of connection socket.
                 **/
                int mConnectionSocket;

                /**
                 * @brief Frames that must be sent to remote device.
                 **/
                tFrameQueue mFramesToSend;

                /**
                 * @brief Terminate flag.
                 *
                 * This flag is set to notify connection thread that connection
                 * must be closed and connection thread must be terminated.
                 **/
                bool mTerminateFlag;
            };

            /**
             * @brief Parameters for starting connection thread.
             **/
            struct SConnectionThreadParameters
            {
                /**
                 * @brief Constructor.
                 *
                 * @param DeviceAdapter Reference to device adapter.
                 * @param ConnectionHandle Connection handle.
                 **/
                SConnectionThreadParameters(CDeviceAdapter & DeviceAdapter, tConnectionHandle ConnectionHandle);

                /**
                 * @brief Reference to device adapter.
                 **/
                CDeviceAdapter & mDeviceAdapter;

                /**
                 * @brief Connection handle.
                 **/
                tConnectionHandle mConnectionHandle;
            };

            /**
             * @brief Wait until all device adapter threads are terminated.
             *
             * Every device adapter must call this method in destructor to wait
             * for other threads to terminate before destroying device adapter.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_threads_termination
             **/
            void waitForThreadsTermination(void);

            /**
             * @brief Start connection.
             *
             * Check if connection is not in connection map
             * and if it's not, add it to map and start connection thread.
             *
             * @param Connection Connection to start.
             *
             * @return true if connection thread has been started, false otherwise.
             **/
            bool startConnection(SConnection * Connection);

            /**
             * @brief Stop connection.
             *
             * This method only initiates connection termination. It returns immediately
             * without waiting for actual termination of the connection.
             *
             * @param ConnectionHandle Handle of connection to stop.
            **/
            void stopConnection(tConnectionHandle ConnectionHandle);

            /**
             * @brief Wait for device scan request.
             *
             * Wait until scanForNewDevices() is called or timeout
             * expires.
             *
             * @param Timeout Timeout value in seconds. 0 means no
             *                timeout.
             *
             * @return true if scanForNewDevices() has been called,
             *         false if timeout expired.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_device_scan
             **/
            bool waitForDeviceScanRequest(const time_t Timeout);

            /**
             * @brief Handle communication.
             *
             * Handle communication for specified connection (send/receive data
             * to/from connection socket).
             *
             * This method must be called from connection thread implementation
             * when connection is established and connection socket descriptor
             * is set for the connection.
             *
             * This methods returns when connection is terminated.
             *
             * @param ConnectionHandle Connection handle.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_handling_communication
             **/
            void handleCommunication(const tConnectionHandle ConnectionHandle);

            /**
             * @brief Update client device list.
             *
             * This method is called when list of devices is changed to
             * notify device adapter listener about new list of devices.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_update_client_device_list
             **/
            void updateClientDeviceList(void);

            /**
             * @brief Create list of connections possible for specified device.
             *
             * This method is called from connectDevice(). Device adapter may implement
             * this method to provide list of connections that must be running on
             * connected device.
             *
             * @param DeviceHandle Device handle.
             * @param ConnectionsList Reference to connections list that must be filled.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_connecting_devices
             **/
            virtual void createConnectionsListForDevice(const tDeviceHandle DeviceHandle, std::vector<SConnection*> & ConnectionsList);

            /**
             * @brief Device adapter main thread.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_main_thread
             **/
            virtual void mainThread(void) = 0;

            /**
             * @brief Connection thread.
             *
             * This method is responsible for establishing connection and communicating
             * with remote device via specified connection. It must remove itself from
             * connection map when connection is terminated before terminating connection thread.
             *
             * @param ConnectionHandle Connection handle.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_connection_thread
             **/
            virtual void connectionThread(const tConnectionHandle ConnectionHandle) = 0;

            /**
             * @brief Connections map.
             **/
            typedef std::map<tConnectionHandle, SConnection *> tConnectionMap;

            /**
             * @brief Logger.
             **/
            const log4cplus::Logger mLogger;

            /**
             * @brief Listener for device adapter notifications.
             **/
            IDeviceAdapterListener & mListener;

            /**
             * @brief Handle generator implementation.
             **/
            IHandleGenerator & mHandleGenerator;

            /**
             * @brief Flag indicating that device scan was requested.
             *
             * This flag is set in scanForNewDevices and reset after requested
             * device scan is completed.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_device_scan
             **/
            bool mDeviceScanRequested;

            /**
             * @brief Mutex restricting access to DeviceScanRequested flag.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_device_scan
             **/
            pthread_mutex_t mDeviceScanRequestedMutex;

            /**
             * @brief Conditional variable for signaling discovery thread about requested device scan.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_device_scan
             **/
            pthread_cond_t mDeviceScanRequestedCond;

            /**
             * @brief Map of device handle to device.
             *
             * This map contains all currently available bluetooth devices.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_devices_map
             **/
            tDeviceMap mDevices;

            /**
             * @brief Mutex restricting access to device map.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_devices_map
             **/
            mutable pthread_mutex_t mDevicesMutex;

            /**
             * @brief Map of connections.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_connections_map
             **/
            tConnectionMap mConnections;

            /**
             * @brief Mutex restricting access to connections map.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_connections_map
             **/
            mutable pthread_mutex_t mConnectionsMutex;

            /**
             * @brief Shutdown flag.
             *
             * This flag is set to true on shutdown to inform all device adapter
             * threads that device adapter shutdown is in progress. After setting
             * this flag device adapter waits until all its threads are terminated.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_threads_termination
             **/
            bool mShutdownFlag;

        private:
            /**
             * @brief Start routine for device adapter main thread.
             *
             * @param Data Must be pointer to CDeviceAdapter instance.
             *
             * @return Thread return value.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_main_thread
             **/
            static void * mainThreadStartRoutine(void * Data);

            /**
             * @brief Connection thread start routine.
             *
             * @param Data Must be pointer to SConnectionThreadParameters. Ownership
             *             of connection thread parameters is passed to connection thread, i.e.
             *             connection thread is responsible for freeing this object.
             *
             * @return Thread return value.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_connection_thread
             **/
            static void * connectionThreadStartRoutine(void * Data);

            /**
             * @brief ID of device adapter main thread.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_main_thread
             **/
            pthread_t mMainThread;

            /**
             * @brief Flag indicating whether the device adapter main thread has been started successfully.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_main_thread
             **/
            bool mMainThreadStarted;
        };
    }
}

#endif
