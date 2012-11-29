/**
 * \file CConnection.cpp
 * \brief Connection class implementation.
 * \author AKara
 */

#include "Logger.hpp"

#include "ConnectionHandler/CConnection.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{

    log4cplus::Logger CConnection::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ConnectionHandler"));

    CConnection::CConnection(tConnectionHandle aConnectionHandle, tDeviceHandle aConnectionDeviceHandle):
    mConnectionHandle(aConnectionHandle),
    mConnectionDeviceHandle(aConnectionDeviceHandle),
    mSessionIDCounter(0)
    {

    }

    CConnection::~CConnection()
    {
        
    }
}/* namespace NsConnectionHandler */
