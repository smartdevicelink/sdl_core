/**
 * \file ITransportManager.hpp
 * \brief Class ITransportManager header.
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

#ifndef __TRANSPORTMANAGER_ITRANSPORTMANAGER_HPP__
#define __TRANSPORTMANAGER_ITRANSPORTMANAGER_HPP__

#include <stdint.h>
#include <sys/types.h>
#include <vector>

#include "TransportManager/SDeviceInfo.hpp"

namespace NsSmartDeviceLink
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
             *
             * @see @ref components_transportmanager_client_connection_management
             **/
            virtual void connectDevice(const tDeviceHandle DeviceHandle) = 0;

            /**
             * @brief Disconnect from all applications connected on device.
             *
             * @param DeviceHandle Handle of device to disconnect from.
             *
             * @see @ref components_transportmanager_client_connection_management
             **/
            virtual void disconnectDevice(const tDeviceHandle DeviceHandle) = 0;

            /**
             * @brief Add listener to the data-related events
             *
             * @param Listener Pointer to listener.
             *
             * @see @ref components_transportmanager_client_data_transfer
             **/
            virtual void addDataListener(ITransportManagerDataListener * Listener) = 0;

            /**
             * @brief Remove listener to the data-related events.
             *
             * @param Listener Pointer to listener.
             *
             * @see @ref components_transportmanager_client_data_transfer
             **/
            virtual void removeDataListener(ITransportManagerDataListener * Listener) = 0;

            /**
             * @brief Add listener to the device-related events.
             *
             * @param Listener Pointer to listener.
             *
             * @see @ref components_transportmanager_client_device_management
             * @see @ref components_transportmanager_client_connection_management
             **/
            virtual void addDeviceListener(ITransportManagerDeviceListener * Listener) = 0;

            /**
             * @brief Remove listenerto the device-related events.
             *
             * @param Listener Pointer to listener.
             *
             * @see @ref components_transportmanager_client_device_management
             * @see @ref components_transportmanager_client_connection_management
             **/
            virtual void removeDeviceListener(ITransportManagerDeviceListener * Listener) = 0;

            /**
             * @brief Send frame.
             *
             * @param ConnectionHandle Connection handle.
             * @param Data Frame payload data.
             * @param DataSize Size of data in bytes.
             * @param UserData Any user data. Will be returned as is in ITransportManagerDataListener::onFrameSendCompleted
             *
             * @see @ref components_transportmanager_client_data_transfer
             **/
            virtual void sendFrame(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize, const int UserData) = 0;
        };
    }
}

#endif
