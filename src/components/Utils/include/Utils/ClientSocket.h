/**
 * \file ClientSocket.h
 * \brief Definition of the ClientSocket class
 */

#ifndef ClientSocket_class
#define ClientSocket_class

#include "Socket.h"

namespace log4cplus
{
    class Logger;
}

/**
 * \brief ClientSocket class acts as a client socket wrapper
 */
class ClientSocket : private Socket
{
public:
    /**
     * \brief Default constructor
     * \param host a host
     * \param port a port
     */
    ClientSocket ( std::string host, int port );

    /**
     * \brief Default class destructor
     */
    virtual ~ClientSocket();

    /**
     * \brief stream input operator
     * \return result
     */
    const ClientSocket& operator << ( const std::string& ) const;

    /**
     * \brief stream output operator
     * \return result
     */
    const ClientSocket& operator >> ( std::string& ) const;

private:

    static log4cplus::Logger mLogger;
};


#endif
