#ifndef __TRANSPORTMANAGER_SDEVICEINFO_HPP__
#define __TRANSPORTMANAGER_SDEVICEINFO_HPP__

#include <string>

namespace AppLink
{
    namespace TransportManager
    {
        /**
         * @brief Handle to device.
         **/
        typedef int tDeviceHandle;

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
        };
    }
}

#endif
