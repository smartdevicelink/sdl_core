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

#include "AppMgr/DeviceHandler.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
    log4cplus::Logger DeviceHandler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("DeviceHandler"));

    /**
     * \brief Default class constructor
     */
    DeviceHandler::DeviceHandler()
    {
    }

    /**
     * \brief Default class destructor
     */
    DeviceHandler::~DeviceHandler()
    {
        clear();
    }

    /**
     * \brief add a device to a mapping
     * \param sessionKey session/connection key
     * \param device device handler
     */
    void DeviceHandler::addDevice(const int &sessionKey, const NsConnectionHandler::tDeviceHandle &device)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Adding a device " << device << " with a session " << sessionKey );
        mDeviceMapping.insert(DeviceMapItem(sessionKey, device));
    }

    /**
     * \brief remove a device from a mapping
     * \param sessionKey session/connection key
     */
    void DeviceHandler::removeDevice(const int &sessionKey)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Removing a device  with a session " << sessionKey );
        mDeviceMapping.erase(sessionKey);
    }

    /**
     * \brief remove an application from a mapping
     * \param device device to remove all associated sessions from mapping
     */
    void DeviceHandler::removeSession(const NsConnectionHandler::tDeviceHandle &device)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Removing all sessions associated with device " << device );
        for(DeviceMap::iterator it = mDeviceMapping.begin(); it != mDeviceMapping.end(); it++)
        {
            if(it->second == device)
            {
                LOG4CPLUS_INFO_EXT(mLogger, "Removing session " << it->first );
                mDeviceMapping.erase(it->first);
            }
        }
    }

    /**
     * \brief find a device subscribed to sesssion
     * \param sessionKey session/connection key
     * \return device handler
     */
    NsConnectionHandler::tDeviceHandle DeviceHandler::findDeviceAssignedToSession(const int &sessionKey) const
    {
        DeviceMap::const_iterator it = mDeviceMapping.find(sessionKey);
        if(it != mDeviceMapping.end())
        {
            return it->second;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Session " << sessionKey << " not found in subscribed." );
        return 0;
    }

    /**
     * \brief cleans session mapping
     */
    void DeviceHandler::clear()
    {
        mDeviceMapping.clear();
    }

    /**
     * \brief get count of items
     * \return items count
     */
    size_t DeviceHandler::size() const
    {
        return mDeviceMapping.size();
    }

    /**
     * \brief Copy constructor
     */
    DeviceHandler::DeviceHandler(const DeviceHandler &)
    {
    }

}
