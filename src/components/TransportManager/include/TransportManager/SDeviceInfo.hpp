#ifndef __TRANSPORTMANAGER_SDEVICEINFO_HPP__
#define __TRANSPORTMANAGER_SDEVICEINFO_HPP__

#include <string>
#include <vector>

namespace NsAppLink
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

        /**
         * @brief Device list.
         **/
        typedef std::vector<SDeviceInfo> tDeviceList;
    }
}

#endif
