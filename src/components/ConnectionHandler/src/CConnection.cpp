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
    mSessionIDCounter(1)
    {

    }

    CConnection::~CConnection()
    {
        
    }

    int CConnection::addNewSession()
    {
        int result = -1;
        if (255 > mSessionIDCounter)
        {
            mSessionList.push_back(mSessionIDCounter);
            result = mSessionIDCounter++;
        }
        return result;
    }

    tConnectionHandle CConnection::getConnectionHandle()
    {
        return mConnectionHandle;
    }

    tDeviceHandle CConnection::getConnectionDeviceHandle()
    {
        return mConnectionDeviceHandle;
    }
}/* namespace NsConnectionHandler */
