/**
 * \file CBluetoothAdapter.hpp
 * \brief Class CBluetoothAdapter header.
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

#ifndef __TRANSPORTMANAGER_CBLUETOOTHADAPTER_HPP__
#define __TRANSPORTMANAGER_CBLUETOOTHADAPTER_HPP__

#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <time.h>

#include "TransportManager/SDeviceInfo.hpp"

#include "CDeviceAdapter.hpp"

namespace NsSmartDeviceLink
{
    namespace NsTransportManager
    {
        /**
         * @brief Bluetooth device adapter.
         *
         * @see @ref components_transportmanager_internal_design_device_adapters_bluetooth_adapter
         **/
        class CBluetoothAdapter: public CDeviceAdapter
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
             * @brief Get type of devices handled by this device adapter.
             *
             * @return Device type.
             **/
            virtual EDeviceType getDeviceType(void) const;

        private:
            /**
             * @brief Vector of RFCOMM channels.
             **/
            typedef std::vector<uint8_t> tRFCOMMChannelVector;

            /**
             * @brief Internal structure describing bluetooth device.
             **/
            struct SBluetoothDevice: public SDevice
            {
                /**
                 * @brief Constructor.
                 *
                 * @param Address Bluetooth address.
                 * @param Name User-friendly device name.
                 * @param SmartDeviceLinkRFCOMMChannels List of RFCOMM channels where SmartDeviceLink service has been discovered.
                 **/
                SBluetoothDevice(const bdaddr_t & Address, const char * Name, const tRFCOMMChannelVector & SmartDeviceLinkRFCOMMChannels);

                /**
                 * @brief Compare devices.
                 *
                 * This method checks whether two SBluetoothDevice structures
                 * refer to the same device.
                 *
                 * @param OtherDevice Device to compare with.
                 *
                 * @return true if devices are equal, false otherwise.
                 **/
                virtual bool isSameAs(const SDevice * OtherDevice) const;

                /**
                 * @brief Device bluetooth address.
                 **/
                bdaddr_t mAddress;

                /**
                 * @brief List of RFCOMM channels where SmartDeviceLink service has been discovered.
                 **/
                tRFCOMMChannelVector mSmartDeviceLinkRFCOMMChannels;
            };

            /**
             * @brief RFCOMM connection.
             **/
            struct SRFCOMMConnection: public SConnection
            {
                /**
                 * @brief Constructor.
                 *
                 * @param DeviceHandle Device handle.
                 * @param RFCOMMChannel RFCOMM channel of SmartDeviceLink service on remote device.
                 **/
                SRFCOMMConnection(const tDeviceHandle DeviceHandle, const uint8_t RFCOMMChannel);

                /**
                 * @brief Compare connections.
                 *
                 * This method compares whether two SRFCOMMConnection structures
                 * refer to the same connection.
                 *
                 * @param OtherConnection Connection to compare with.
                 *
                 * @return true if connections are equal, false otherwise.
                 **/
                virtual bool isSameAs(const SConnection * OtherConnection) const;

                /**
                 * @brief RFCOMM channel of remote device.
                 **/
                const uint8_t mRFCOMMChannel;
            };

            /**
             * @brief Create list of connections possible for specified device.
             *
             * This method is called from connectDevice(). This implementation will
             * perform service discovery and create connection structures for all
             * RFCOMM channels where SmartDeviceLink service is discovered.
             *
             * @param DeviceHandle Device handle.
             * @param ConnectionsList Reference to connections list that must be filled.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_bluetooth_adapter_connecting_devices
             **/
            virtual void createConnectionsListForDevice(const tDeviceHandle DeviceHandle, std::vector<SConnection*> & ConnectionsList);

            /**
             * @brief Device discovery thread.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_common_main_thread
             * @see @ref components_transportmanager_internal_design_device_adapters_bluetooth_adapter_discovery
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
             * @see @ref components_transportmanager_internal_design_device_adapters_common_connection_thread
             **/
            virtual void connectionThread(const tConnectionHandle ConnectionHandle);

            /**
             * @brief Get unique device ID.
             *
             * Get TransportManager-unique identifier of
             * bluetooth device based on its bluetooth address.
             *
             * @param DeviceAddress Address of device.
             *
             * @return Unique device identifier.
             **/
            static std::string getUniqueDeviceId(const bdaddr_t & DeviceAddress);

            /**
             * @brief Discover SmartDeviceLink RFCOMM channels.
             *
             * Run service discovery for specified remote bluetooth device
             * and discover RFCOMM channels with SmartDeviceLink service.
             *
             * @param DeviceAddress Address of device.
             * @param SmartDeviceLinkRFCOMMChannels Reference to variable that will
             *                              receive list of RFCOMM channels
             *                              with SmartDeviceLink service discovered.
             *
             * @see @ref components_transportmanager_internal_design_device_adapters_bluetooth_adapter_discovery
             * @see @ref components_transportmanager_internal_design_device_adapters_bluetooth_adapter_connecting_devices
             **/
            void discoverSmartDeviceLinkRFCOMMChannels(const bdaddr_t & DeviceAddress, tRFCOMMChannelVector & SmartDeviceLinkRFCOMMChannels);

            /**
             * @brief UUID of SmartDeviceLink service.
             **/
            uuid_t mSmartDeviceLinkServiceUUID;
        };
    }
}

#endif
