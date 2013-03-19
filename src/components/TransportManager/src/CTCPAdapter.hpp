/**
 * \file CTCPAdapter.hpp
 * \brief Class CTCPAdapter header.
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

#ifndef __TRANSPORTMANAGER_CTCPADAPTER_HPP__
#define __TRANSPORTMANAGER_CTCPADAPTER_HPP__

#include <netinet/in.h>

#include "CDeviceAdapter.hpp"

namespace NsSmartDeviceLink
{
    namespace NsTransportManager
    {
        /**
         * @brief Device adapter for TCP/IP connections.
         *
         * @see @ref components_transportmanager_internal_design_device_adapters_tcp_adapter
         **/
        class CTCPAdapter: public CDeviceAdapter
        {
        public:
            /**
             * @brief Constructor.
             *
             * @param Listener Listener for device adapter notifications.
             * @param HandleGenerator Handle generator implementation.
             **/
            CTCPAdapter(IDeviceAdapterListener & Listener, IHandleGenerator & HandleGenerator);

            /**
             * @brief Destructor.
             **/
            virtual ~CTCPAdapter(void);

            /**
             * @brief Get type of devices handled by this device adapter.
             *
             * @return Device type.
             **/
            virtual EDeviceType getDeviceType(void) const;

        private:
            /**
             * @brief TCP port for listening for incoming connections.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_tcp_adapter_listen
             **/
            static const uint16_t cTCPAdapterPort = 12345u;

            /**
             * @brief Internal structure describing TCP device.
             **/
            struct STCPDevice: public SDevice
            {
                /**
                 * @brief Constructor.
                 *
                 * @param Name User-friendly device name.
                 * @param Address Device IP address.
                 **/
                STCPDevice(const char * Name, const in_addr_t & Address);

                /**
                 * @brief Destructor.
                 **/
                virtual ~STCPDevice(void);

                /**
                 * @brief Compare devices.
                 *
                 * This method checks whether two STCPDevice structures
                 * refer to the same device.
                 *
                 * @param OtherDevice Device to compare with.
                 *
                 * @return true if devices are equal, false otherwise.
                 **/
                virtual bool isSameAs(const SDevice * OtherDevice) const;

                /**
                 * @brief Device IP address.
                 **/
                in_addr_t mAddress;
            };

            /**
             * @brief TCP connection.
             **/
            struct STCPConnection: public SConnection
            {
                /**
                 * @brief Constructor.
                 *
                 * @param DeviceHandle Device handle.
                 * @param ConnectionSocket Connection socket.
                 * @param Port TCP port.
                 **/
                STCPConnection(const tDeviceHandle DeviceHandle, int ConnectionSocket, const in_port_t Port);

                /**
                 * @brief Destructor.
                 **/
                virtual ~STCPConnection(void);

                /**
                 * @brief Compare connections.
                 *
                 * This method compares whether two STCPConnection structures
                 * refer to the same connection.
                 *
                 * @param OtherConnection Connection to compare with.
                 *
                 * @return true if connections are equal, false otherwise.
                 **/
                virtual bool isSameAs(const SConnection * OtherConnection) const;

                /**
                 * @brief TCP port of remote device.
                 **/
                const in_port_t mPort;
            };

            /**
             * @brief Device adapter main thread.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_tcp_adapter_listen
             * @see @ref components_transportmanager_internal_design_device_adapters_tcp_adapter_accept
             * @see @ref components_transportmanager_internal_design_device_adapters_common_main_thread
             **/
            virtual void mainThread(void);

            /**
             * @brief Connection thread.
             *
             * This method is responsible for establishing connection and communicating
             * with remote device via specified connection. It must remove itself from
             * connection map when connection is terminated before terminating connection thread.
             *
             * @param ConnectionHandle Connection handle.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_tcp_adapter_disconnecting
             * @see @ref components_transportmanager_internal_design_device_adapters_common_connection_thread
             **/
            virtual void connectionThread(const tConnectionHandle ConnectionHandle);
        };
    }
}

#endif
