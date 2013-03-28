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

#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include "ConnectionHandler/device.hpp"

#include <map>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{
    /**
     * \brief session-to-device map
     */
    typedef std::map<int, connection_handler::DeviceHandle> DeviceMap;

    /**
     * \brief session-to-device map
     */
    typedef std::pair<int, connection_handler::DeviceHandle> DeviceMapItem;

    class DeviceHandler
    {
    public:

        /**
         * \brief Default class constructor
         */
        DeviceHandler();

        /**
         * \brief Default class destructor
         */
        ~DeviceHandler();

        /**
         * \brief add a device to a mapping
         * \param sessionKey session/connection key
         * \param device device handler
         */
        void addDevice( const int& sessionKey, const connection_handler::DeviceHandle& device );

        /**
         * \brief remove a device from a mapping
         * \param sessionKey session/connection key
         */
        void removeDevice(const int& sessionKey);

        /**
         * \brief remove an application from a mapping
         * \param device device to remove all associated sessions from mapping
         */
        void removeSession( const connection_handler::DeviceHandle& device );

        /**
         * \brief find a device subscribed to sesssion
         * \param sessionKey session/connection key
         * \return device handler
         */
        connection_handler::DeviceHandle findDeviceAssignedToSession(const int& sessionKey) const;

        /**
         * \brief cleans session mapping
         */
        void clear( );

        /**
         * \brief get count of items
         * \return items count
         */
        size_t size() const;

    private:

        /**
         * \brief Copy constructor
         */
        DeviceHandler(const DeviceHandler&);

        DeviceMap    mDeviceMapping;
        static log4cplus::Logger mLogger;
    };

}

#endif // DEVICEHANDLER_H
