//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "AppMgr/DeviceList.h"

namespace NsAppManager
{
    log4cplus::Logger DeviceList::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("DeviceList"));

    /**
     * \brief Default class constructor
     */
    DeviceList::DeviceList()
    {
    }

    /**
     * \brief Default class destructor
     */
    DeviceList::~DeviceList()
    {
    }

    /**
     * \brief set device list
     * \param deviceList device list
     */
    void DeviceList::setDeviceList(const NsConnectionHandler::DeviceList &deviceList)
    {
        mDeviceList = deviceList;
    }

    /**
     * \brief get device list
     * \return device list
     */
    const NsConnectionHandler::DeviceList &DeviceList::getDeviceList() const
    {
        return mDeviceList;
    }

    /**
     * \brief get count of items
     * \return items count
     */
    size_t DeviceList::size() const
    {
        return mDeviceList.size();
    }

    /**
     * \brief find device associated with the given handle
     * \param handle device handle
     * \return device associated with the given handle
     */
    const NsConnectionHandler::CDevice* DeviceList::findDeviceByHandle(const NsConnectionHandler::DeviceHandle &handle) const
    {
        NsConnectionHandler::DeviceList::const_iterator it = mDeviceList.find(handle);
        if(it != mDeviceList.end())
        {
            return &it->second;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Device " << handle << " not found in subscribed." );
        return 0;
    }

    /**
     * \brief find device associated with the given name
     * \param name device name
     * \return device associated with the given name
     */
    const NsConnectionHandler::CDevice *DeviceList::findDeviceByName(const std::string &name) const
    {
        for(NsConnectionHandler::DeviceList::const_iterator it = mDeviceList.begin(); it != mDeviceList.end(); it++)
        {
            const NsConnectionHandler::CDevice& device = it->second;
            if(device.user_friendly_name() == name)
            {
                return &device;
            }
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Device " << name << " not found in subscribed." );
        return 0;
    }

    /**
     * \brief Copy constructor
     */
    DeviceList::DeviceList(const DeviceList &)
    {
    }
}
