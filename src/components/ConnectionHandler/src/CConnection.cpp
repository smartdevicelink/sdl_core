/**
 * \file CConnection.cpp
 * \brief Connection class implementation.
 * \author AKara
 */

#include "Logger.hpp"

#include <algorithm>

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

    int CConnection::removeSession(unsigned char aSession)
    {
        int result = -1;
        tSessionListIterator it = std::find(mSessionList.begin(), mSessionList.end(), aSession);
        if (mSessionList.end() == it)
        {
            LOG4CPLUS_ERROR( mLogger, "Session not found in this connection!");
        } else
        {
            mSessionList.erase(it);
            result = aSession;
        }
        return result;
    }

    int CConnection::getFirstSessionKey()
    {
        int result = -1;
        tSessionListIterator it = mSessionList.begin();
        if (mSessionList.end() != it)
        {
            result = *it;
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
