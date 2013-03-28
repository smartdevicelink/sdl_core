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

#ifndef DEVICELIST_H
#define DEVICELIST_H

#include "ConnectionHandler/device.hpp"

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{
    /**
     * \brief DeviceList acts as a device list
     */
    class DeviceList
    {
    public:

        /**
         * \brief Default class constructor
         */
        DeviceList();

        /**
         * \brief Default class destructor
         */
        ~DeviceList();

        /**
         * \brief set device list
         * \param deviceList device list
         */
        void setDeviceList( const connection_handler::DeviceList& deviceList );

        /**
         * \brief get device list
         * \return device list
         */
        const connection_handler::DeviceList& getDeviceList() const;

        /**
         * \brief get count of items
         * \return items count
         */
        size_t size() const;

        /**
         * \brief find device associated with the given handle
         * \param handle device handle
         * \return device associated with the given handle
         */
        const connection_handler::Device *findDeviceByHandle(const connection_handler::DeviceHandle& handle) const;

        /**
         * \brief find device associated with the given name
         * \param name device name
         * \return device associated with the given name
         */
        const connection_handler::Device *findDeviceByName(const std::string& name) const;

    private:

        /**
         * \brief Copy constructor
         */
        DeviceList(const DeviceList&);

        connection_handler::DeviceList mDeviceList;
        static log4cplus::Logger mLogger;
    };
}

#endif // DEVICELIST_H
