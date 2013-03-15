/**
 * \file SocketException.cpp
 * \brief Implementation of the SocketException class
 */

#include "Utils/SocketException.h"

/**
 * \brief Class constructor
 * \param s exception string
 */
SocketException::SocketException(std::string s)
    :m_s ( s )
{
}

/**
 * \brief Default class destructor
 */
SocketException::~SocketException()
{
}

/**
 * \brief description of exception
 * \return description
 */
const std::string &SocketException::description()
{
    return m_s;
}
