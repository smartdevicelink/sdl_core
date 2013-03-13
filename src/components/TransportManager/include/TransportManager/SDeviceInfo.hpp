/**
 * \file SDeviceInfo.hpp
 * \brief Structure SDeviceInfo header.
 * Copyright (c) 2013 Ford Motor Company
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
