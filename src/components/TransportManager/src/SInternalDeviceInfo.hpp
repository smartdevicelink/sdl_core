#ifndef __TRANSPORTMANAGER_SINTERNALDEVICEINFO_HPP__
#define __TRANSPORTMANAGER_SINTERNALDEVICEINFO_HPP__

#include <vector>

#include "TransportManager/SDeviceInfo.hpp"

namespace NsAppLink
{
    namespace NsTransportManager
    {
        /**
         * @brief Internal device information.
         **/
        struct SInternalDeviceInfo
        {
            /**
             * @brief Default constructor.
             **/
            SInternalDeviceInfo(void);

            /**
             * @brief Constructor.
             *
             * @param DeviceHandle Device handle.
             * @param UserFriendlyName User-friendly device name.
             * @param UniqueDeviceId Unique device identifier across all devices.
             **/
            SInternalDeviceInfo(tDeviceHandle DeviceHandle, const std::string & UserFriendlyName, const std::string & UniqueDeviceId);

            /**
             * @brief Device handle.
             **/
            tDeviceHandle mDeviceHandle;

            /**
             * @brief User-friendly device name.
             **/
            std::string mUserFriendlyName;

            /**
             * @brief Unique device identifier across all devices.
             **/
            std::string mUniqueDeviceId;
        };

        /**
         * @brief List of internal devices.
         **/
        typedef std::vector<SInternalDeviceInfo> tInternalDeviceList;
    }
}

#endif
