/**
 * \file ITransportManagerDeviceListener.hpp
 * \brief Class ITransportManagerDeviceListener header.
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

#ifndef __TRANSPORTMANAGER_ITRANSPORTMANAGERDEVICELISTENER_HPP__
#define __TRANSPORTMANAGER_ITRANSPORTMANAGERDEVICELISTENER_HPP__

#include "TransportManager/SDeviceInfo.hpp"
#include "TransportManager/ITransportManager.hpp"

namespace NsSmartDeviceLink
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
