/**
 * \file Socket.cpp
 * \brief Implementation of the Socket class
 */

#include "Utils/Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "LoggerHelper.hpp"

log4cplus::Logger Socket::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Utils"));

/**
 * \brief Default class constructor
 */
Socket::Socket()
    :m_sock ( -1 )
{
    memset ( &m_addr,
       0,
       sizeof ( m_addr ) );
}

/**
 * \brief Default class destructor
 */
Socket::~Socket()
{
    if ( is_valid() )
        ::close ( m_sock );
}

/**
 * \brief Server initialization
 */
bool Socket::create()
{
    m_sock = socket ( AF_INET,
            SOCK_STREAM,
            0 );

    if ( ! is_valid() )
        return false;

    // TIME_WAIT - argh
    int on = 1;
    if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
        return false;

    return true;
}

/**
 * \brief Server bind
 * \param port a port to bind to
 * \return success
 */
bool Socket::bind ( const int port )
{
    if ( ! is_valid() )
    {
        return false;
    }

    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons ( port );

    int bind_return = ::bind ( m_sock,
                 ( struct sockaddr * ) &m_addr,
                 sizeof ( m_addr ) );

    if ( bind_return == -1 )
    {
        return false;
    }

    return true;
}

/**
 * \brief Listen to a port
 * \return success
 */
bool Socket::listen() const
{
    if ( ! is_valid() )
    {
        return false;
    }

    int listen_return = ::listen ( m_sock, MAXCONNECTIONS );

    if ( listen_return == -1 )
    {
        return false;
    }

    return true;
}

/**
 * \brief Accept a socket
 * \param new_soclet a socket to accept
 * \return success
 */
bool Socket::accept ( Socket& new_socket ) const
{
    int addr_length = sizeof ( m_addr );
    new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

    if ( new_socket.m_sock <= 0 )
        return false;
    else
        return true;
}

/**
 * \brief Data Transimission
 * \param s data
 * \return success
 */
bool Socket::send ( const std::string s ) const
{
    int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
    if ( status == -1 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * \brief receive data
 * \param s data
 * \return data size received
 */
int Socket::recv ( std::string& s ) const
{
    char buf [ MAXRECV + 1 ];

    s = "";

    memset ( buf, 0, MAXRECV + 1 );

    int status = ::recv ( m_sock, buf, MAXRECV, 0 );

    if ( status == -1 )
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "status == -1   errno == " << errno << "  in Socket::recv\n" );
        return 0;
    }
    else if ( status == 0 )
    {
        return 0;
    }
    else
    {
        s = buf;
        return status;
    }
}

/**
 * \brief Client initialization
 * \param host a host
 * \param port a port
 * \return success
 */
bool Socket::connect ( const std::string host, const int port )
{
    if ( ! is_valid() ) return false;

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons ( port );

    int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

    if ( errno == EAFNOSUPPORT ) return false;

    status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

    if ( status == 0 )
        return true;
    else
        return false;
}

/**
 * \brief set non-blocking state
 * \param b non-blocking state
 */
void Socket::set_non_blocking ( const bool b )
{
    int opts;

    opts = fcntl ( m_sock,
         F_GETFL );

    if ( opts < 0 )
    {
        return;
    }

    if ( b )
        opts = ( opts | O_NONBLOCK );
    else
        opts = ( opts & ~O_NONBLOCK );

    fcntl ( m_sock,
            F_SETFL,opts );
}

/**
 * \brief is a socket valid
 * \return is a socket valid
 */
bool Socket::is_valid() const
{
    return m_sock != -1;
}
