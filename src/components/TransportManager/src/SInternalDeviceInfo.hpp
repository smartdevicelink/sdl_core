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
             * @brief Device handle.
             **/
            tDeviceHandle mDeviceHandle;

            /**
             * @brief User-friendly device name.
             **/
            std::string mUserFriendlyName;
        };

        /**
         * @brief List of internal devices.
         **/
        typedef std::vector<SInternalDeviceInfo> tInternalDeviceList;
    }
}

#endif
