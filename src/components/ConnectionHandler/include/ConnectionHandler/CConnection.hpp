/**
 * \file CConnection.hpp
 * \brief Connection class.
 * Stores connetion information
 * Copyright (c) 2013 Ford Motor Company
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
