/**
 * \file Socket.h
 * \brief Definition of the Socket class
 * \author vsalo
 */

#ifndef Socket_class
#define Socket_class

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

namespace log4cplus
{
    class Logger;
}

/**
 * \brief Socket class acts as a posix sockets wrapper
 */
class Socket
{
public:
    /**
     * \brief Default class constructor
     */
    Socket();

    /**
     * \brief Default class destructor
     */
    virtual ~Socket();

    /**
     * \brief Server initialization
     */
    bool create();

    /**
     * \brief Server bind
     * \param port a port to bind to
     * \return success
     */
    bool bind ( const int port );

    /**
     * \brief Listen to a port
     * \return success
     */
    bool listen() const;

    /**
     * \brief Accept a socket
     * \return success
     */
    bool accept ( Socket& ) const;

    /**
     * \brief Client initialization
     * \param host a host
     * \param port a port
     * \return success
     */
    bool connect ( const std::string host, const int port );

    /**
     * \brief Data Transimission
     * \param s data
     * \return success
     */
    bool send ( const std::string s ) const;

    /**
     * \brief receive data
     * \return data size received
     */
    int recv ( std::string& ) const;

    /**
     * \brief set non-blocking state
     */
    void set_non_blocking ( const bool );

    /**
     * \brief is a socket valid
     * \return is a socket valid
     */
    bool is_valid() const;

private:

    int m_sock;
    sockaddr_in m_addr;

    static log4cplus::Logger mLogger;
};


#endif
