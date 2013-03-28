/**
 * \file SDeviceInfo.hpp
 * \brief Structure SDeviceInfo header.
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

#ifndef __TRANSPORTMANAGER_SDEVICEINFO_HPP__
#define __TRANSPORTMANAGER_SDEVICEINFO_HPP__

#include <string>
#include <vector>

namespace NsSmartDeviceLink
{
    namespace NsTransportManager
    {
        /**
         * @brief Handle to device.
         **/
        typedef int tDeviceHandle;

        /**
         * @brief Special value of device handle for indicating invalid device handle.
         **/
        const tDeviceHandle InvalidDeviceHandle = static_cast<tDeviceHandle>(-1);

        /**
         * @brief Enumeration for device type.
         **/
        enum EDeviceType
        {
            /**
             * @brief Bluetooth device.
             **/
            DeviceBluetooth,

            /**
             * @brief USB device.
             **/
            DeviceUSB,

            /**
             * @brief WiFi device.
             **/
            DeviceWiFi
        };

        /**
         * @brief Device information.
         **/
        struct SDeviceInfo
        {
            /**
             * @brief Constructor for creating empty struct
             *
             **/
            SDeviceInfo();

            /**
             * @brief Constructor for creating struct with predefined values
             *
             * @param DeviceHandle DeviceHandle
             * @param DeviceType DeviceType
             * @param UserFriendlyName User-friendly name
             * @param UniqueDeviceId Unique device id
             **/
            SDeviceInfo(tDeviceHandle DeviceHandle, EDeviceType DeviceType, std::string UserFriendlyName, std::string UniqueDeviceId);

            /**
             * @brief Copy constructor
             *
             * @param other Reference to object to be copied
             **/
            SDeviceInfo(const SDeviceInfo& other);

            /**
             * @brief Comparison operator.
             *
             * @param i_other Reference to the object to be compared with
             * @return bool
             **/
            bool operator==( const SDeviceInfo& i_other ) const;

            /**
             * @brief Device handle.
             **/
            tDeviceHandle mDeviceHandle;

            /**
             * @brief Device type.
             **/
            EDeviceType mDeviceType;

            /**
             * @brief User-friendly device name.
             **/
            std::string mUserFriendlyName;

            /**
             * @brief Unique device identifier across all devices
             **/
            std::string mUniqueDeviceId;
        };

        /**
         * @brief Device list.
         **/
        typedef std::vector<SDeviceInfo> tDeviceList;
    }
}

#endif
