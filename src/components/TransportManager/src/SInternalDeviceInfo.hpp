#ifndef __TRANSPORTMANAGER_SINTERNALDEVICEINFO_HPP__
#define __TRANSPORTMANAGER_SINTERNALDEVICEINFO_HPP__

#include "TransportManager/SDeviceInfo.hpp"

namespace AppLink
{
    namespace TransportManager
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
    }
}

#endif
