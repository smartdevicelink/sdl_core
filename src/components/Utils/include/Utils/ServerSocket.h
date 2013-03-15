/**
 * \file ServerSocket.h
 * \brief Definition of the ServerSocket class
 */

#ifndef ServerSocket_class
#define ServerSocket_class

#include "Socket.h"

namespace log4cplus
{
    class Logger;
}

/**
 * \brief ServerSocket class acts as a server socket wrapper
 */
class ServerSocket : private Socket
{
public:
    /**
     * \brief Class constructor
     * \param port port to bind to
     */
    ServerSocket ( int port );

    /**
     * \brief Default class constructor
     */
    ServerSocket ();

    /**
     * \brief Default class destructor
     */
    virtual ~ServerSocket();

    /**
     * \brief stream input operator
     * \return result
     */
    const ServerSocket& operator << ( const std::string& ) const;

    /**
     * \brief stream output operator
     * \return result
     */
    const ServerSocket& operator >> ( std::string& ) const;

    /**
     * \brief accept a socket
     */
    void accept ( ServerSocket& );

private:

    static log4cplus::Logger mLogger;
};


#endif
