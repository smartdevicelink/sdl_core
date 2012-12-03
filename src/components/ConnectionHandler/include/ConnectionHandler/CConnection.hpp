/**
 * \file CConnection.hpp
 * \brief Connection class.
 * Stores connetion information
 * \author AKara
 */

#ifndef CONNECTION_H 
#define CONNECTION_H 

#include "Logger.hpp"
#include "ConnectionHandler/CDevice.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{

    /**
     * \brief Type for ConnectionHandle
     */
    typedef int tConnectionHandle;

    /**
     * \brief Type for Connections map
     * Key is ConnectionHandle which is uniq
     */
    typedef std::vector<unsigned char> tSessionList;

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