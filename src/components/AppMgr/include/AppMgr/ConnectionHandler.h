/**
 * \file ConnectionHandler.h
 * \brief connection handler
 * \author vsalo
 */

#ifndef APPCONNECTIONHANDLER_H
#define APPCONNECTIONHANDLER_H


namespace NsConnectionHandler
{
    class IDevicesDiscoveryStarter;
}

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{
    /**
     * \brief ConnectionHandler acts as wrapper for connection handler
     */
    class ConnectionHandler
    {
    public:
        /**
         * \brief Returning class instance
         * \return class instance
         */
        static ConnectionHandler &getInstance();

        /**
         * \brief Sets connection handler instance
         * \param handler connection handler
         */
        void setConnectionHandler(NsConnectionHandler::IDevicesDiscoveryStarter *handler);

        /**
         * \brief Gets connection handler instance
         * \return connection handler
         */
        NsConnectionHandler::IDevicesDiscoveryStarter *getConnectionHandler( ) const;

        /**
         * \brief Start device discovery
         */
        void startDevicesDiscovery();

    private:

        /**
         * \brief Default class constructor
         */
        ConnectionHandler();

        /**
         * \brief Copy constructor
         */
        ConnectionHandler(const ConnectionHandler&);

        NsConnectionHandler::IDevicesDiscoveryStarter* mConnectionHandler;
        static log4cplus::Logger mLogger;
    };

}

#endif // APPCONNECTIONHANDLER_H
