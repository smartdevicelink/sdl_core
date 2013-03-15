/**
 * \file ClientSocket.cpp
 * \brief Implementation of the ClientSocket class
 */

#include "Utils/ClientSocket.h"
#include "Utils/SocketException.h"
#include "LoggerHelper.hpp"

log4cplus::Logger ClientSocket::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Utils"));

/**
 * \brief Default constructor
 * \param host a host
 * \param port a port
 */
ClientSocket::ClientSocket ( std::string host, int port )
{
    if ( ! Socket::create() )
    {
        throw SocketException ( "Could not create client socket." );
    }

    if ( ! Socket::connect ( host, port ) )
    {
        throw SocketException ( "Could not bind to port." );
    }
}

/**
 * \brief Default class destructor
 */
ClientSocket::~ClientSocket()
{
}

/**
 * \brief stream input operator
 * \return result
 */
const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
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
const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
    if ( ! Socket::recv ( s ) )
    {
        throw SocketException ( "Could not read from socket." );
    }

    return *this;
}
