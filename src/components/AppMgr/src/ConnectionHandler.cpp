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

#include "AppMgr/ConnectionHandler.h"
#include "ConnectionHandler/devices_discovery_starter.h"

namespace NsAppManager
{
    log4cxx::LoggerPtr ConnectionHandler::logger_ =
        log4cxx::LoggerPtr(log4cxx::Logger::getLogger("ConnectionHandler"));

    /**
     * \brief Returning class instance
     * \return class instance
     */
    ConnectionHandler &ConnectionHandler::getInstance()
    {
        static ConnectionHandler instance;
        return instance;
    }

    /**
     * \brief Sets connection handler instance
     * \param handler connection handler
     */
    void ConnectionHandler::setConnectionHandler(connection_handler::DevicesDiscoveryStarter *handler)
    {
        if(!handler)
        {
            LOG4CXX_ERROR_EXT(logger_, "A null pointer is being assigned - is this the intent?");
            return;
        }
        mConnectionHandler = handler;
    }

    /**
     * \brief Gets connection handler instance
     * \return connection handler
     */
    connection_handler::DevicesDiscoveryStarter *ConnectionHandler::getConnectionHandler() const
    {
        if(!mConnectionHandler)
        {
            LOG4CXX_ERROR_EXT(logger_, "Connection handler hasn't yet been assigned, but an attempt to retrieve it has been made! Face a core dump soon...(((");
        }
        return mConnectionHandler;
    }

    /**
     * \brief Start device discovery
     */
    void ConnectionHandler::StartDevicesDiscovery()
    {
        if(!mConnectionHandler)
        {
            LOG4CXX_ERROR_EXT(logger_, "Connection handler hasn't yet been assigned, but an attempt to access it has been made! Giving up...");
            return;
        }
        mConnectionHandler->StartDevicesDiscovery();
    }

    /**
     * \brief Connect to device specified in params
     * \param deviceHandle device handle
     */
    void ConnectionHandler::ConnectToDevice(connection_handler::DeviceHandle deviceHandle)
    {
        if(!mConnectionHandler)
        {
            LOG4CXX_ERROR_EXT(logger_, "Connection handler hasn't yet been assigned, but an attempt to access it has been made! Giving up...");
            return;
        }
        mConnectionHandler->ConnectToDevice(deviceHandle);
    }

    void ConnectionHandler::StartTransportManager()
    {
        if(!mConnectionHandler)
        {
            LOG4CXX_ERROR_EXT(logger_, "Connection handler hasn't yet been assigned, but an attempt to access it has been made! Giving up...");
            return;
        }
        mConnectionHandler->StartTransportManager();
    }

    /**
     * \brief Default class constructor
     */
    ConnectionHandler::ConnectionHandler()
        :mConnectionHandler(0)
    {
    }

    /**
     * \brief Copy constructor
     */
    ConnectionHandler::ConnectionHandler(const ConnectionHandler &)
        :mConnectionHandler(0)
    {
    }

}
