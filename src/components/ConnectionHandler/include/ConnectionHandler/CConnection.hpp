/**
 * \file CConnection.hpp
 * \brief Connection class.
 * Stores connection information
 *
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CONNECTION_H 
#define CONNECTION_H 

#include "Logger.hpp"
#include "ConnectionHandler/CDevice.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{

    /**
     * \brief Type for ConnectionHandle
     */
    typedef int tConnectionHandle;

    /**
     * \brief Type for Sessions vector
     */
    typedef std::vector<unsigned char> tSessionList;

    /**
     * \brief Type for Sessions vector iterator
     */
    typedef std::vector<unsigned char>::iterator tSessionListIterator;

    /**
     * \class CConnection
     * \brief Connection class
     */
    class CConnection
    {
    public:
        /**
         * \brief Class constructor
         */
        CConnection(tConnectionHandle aConnectionHandle, tDeviceHandle aConnectionDeviceHandle);

        /**
         * \brief Destructor
         */
        ~CConnection();

        /**
         * \brief Returns device handle
         * \return DeviceHandle
         */
        tConnectionHandle getConnectionHandle();

        /**
         * \brief Returns connection device handle
         * \return ConnectionDeviceHandle
         */
        tDeviceHandle getConnectionDeviceHandle();

        /**
         * \brief Adds session to connection
         * \return sessionID or -1 in case of issues
         */
        int addNewSession();

        /**
         * \brief Removes session from connection
         * \param aSession session ID
         * \return sessionID or -1 in case of issues
         */
        int removeSession(unsigned char aSession);

        /**
         * \brief Returns ID of first session from connection
         * \return first sessionID or -1 in case of issues
         */
        int getFirstSessionID();
    private:

        /**
         * \brief Current connection handle.
         */
          tConnectionHandle mConnectionHandle;

        /**
         * \brief DeviceHandle of this connection.
         */
          tDeviceHandle mConnectionDeviceHandle;

        /**
         * \brief Counter to generate session id's.
         */
        unsigned char mSessionIDCounter;

        /**
         * \brief Counter to generate session id's.
         */
        tSessionList mSessionList;

        /**
         * \brief For logging.
         */
        static log4cplus::Logger mLogger;
    };

    /**
     * \brief Type for Connections map
     * Key is ConnectionHandle which is uniq
     */
    typedef std::map<int, CConnection> tConnectionList;

    /**
     * \brief Type for Connections map iterator
     */
    typedef std::map<int, CConnection>::iterator tConnectionListIterator;

}/* namespace NsConnectionHandler */

#endif /* CONNECTION_H */
