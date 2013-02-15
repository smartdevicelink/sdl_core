/**
 * \file ConnectionHandler.cpp
 * \brief connection handler
 * \author vsalo
 */

#include "AppMgr/ConnectionHandler.h"
#include "ConnectionHandler/IDevicesDiscoveryStarter.hpp"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
    log4cplus::Logger ConnectionHandler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ConnectionHandler"));

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
    void ConnectionHandler::setConnectionHandler(NsConnectionHandler::IDevicesDiscoveryStarter *handler)
    {
        if(!handler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "A null pointer is being assigned - is this the intent?");
            return;
        }
        mConnectionHandler = handler;
    }

    /**
     * \brief Gets connection handler instance
     * \return connection handler
     */
    NsConnectionHandler::IDevicesDiscoveryStarter *ConnectionHandler::getConnectionHandler() const
    {
        if(!mConnectionHandler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Connection handler hasn't yet been assigned, but an attempt to retrieve it has been made! Face a core dump soon...(((");
        }
        return mConnectionHandler;
    }

    /**
     * \brief Start device discovery
     */
    void ConnectionHandler::startDevicesDiscovery()
    {
        if(!mConnectionHandler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Connection handler hasn't yet been assigned, but an attempt to access it has been made! Giving up...");
            return;
        }
        mConnectionHandler->startDevicesDiscovery();
    }

    /**
     * \brief Connect to device specified in params
     * \param deviceHandle device handle
     */
    void ConnectionHandler::connectToDevice(NsConnectionHandler::tDeviceHandle deviceHandle)
    {
        if(!mConnectionHandler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Connection handler hasn't yet been assigned, but an attempt to access it has been made! Giving up...");
            return;
        }
        mConnectionHandler->connectToDevice(deviceHandle);
    }

    void ConnectionHandler::startTransportManager()
    {
        if(!mConnectionHandler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Connection handler hasn't yet been assigned, but an attempt to access it has been made! Giving up...");
            return;
        }
        mConnectionHandler->startTransportManager();
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
