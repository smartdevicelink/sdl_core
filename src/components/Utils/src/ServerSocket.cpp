/**
 * \file ServerSocket.cpp
 * \brief Implementation of the ServerSocket class
 */

#include "Utils/ServerSocket.h"
#include "Utils/SocketException.h"
#include "LoggerHelper.hpp"

log4cplus::Logger ServerSocket::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Utils"));

/**
 * \brief Class constructor
 * \param port port to bind to
 */
ServerSocket::ServerSocket ( int port )
{
    if ( ! Socket::create() )
    {
        throw SocketException ( "Could not create server socket." );
    }

    if ( ! Socket::bind ( port ) )
    {
        throw SocketException ( "Could not bind to port." );
    }

    if ( ! Socket::listen() )
    {
        throw SocketException ( "Could not listen to socket." );
    }
}

/**
 * \brief Default class constructor
 */
ServerSocket::ServerSocket()
{
}

/**
 * \brief Default class destructor
 */
ServerSocket::~ServerSocket()
{
}

/**
 * \brief stream input operator
 * \return result
 */
const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
    if ( ! Socket::send ( s ) )
    {
        throw SocketException ( "Could not write to socket." );
    }

    return *this;
}

/**
 * \brief stream output operator
 * \return result
 */
const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
    if ( ! Socket::recv ( s ) )
    {
        throw SocketException ( "Could not read from socket." );
    }

    return *this;
}

/**
 * \brief accept a socket
 * \param sock a socket
 */
void ServerSocket::accept ( ServerSocket& sock )
{
    if ( ! Socket::accept ( sock ) )
    {
        throw SocketException ( "Could not accept socket." );
    }
}
