/**
 * \file CConnectionHandler.cpp
 * \brief Connection handler class.
 * \Observes TransportManager and ProtocolHandler, stores information regarding connections
 * \and sessions and provides it to AppManager.
 * \author AKara
 */

#include "Logger.hpp"

#include "ConnectionHandler/CConnectionHandler.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{

    log4cplus::Logger CConnectionHandler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ConnectionHandler"));

    CConnectionHandler* CConnectionHandler::getInstance()
    {
        static CConnectionHandler instance;
        return &instance;
    }

    CConnectionHandler::CConnectionHandler():
    mpConnectionHandlerObserver(NULL)
    {

    }

    CConnectionHandler::~CConnectionHandler()
    {
        
    }

    void CConnectionHandler::setConnectionHandlerObserver(IConnectionHandlerObserver * observer)
    {
        LOG4CPLUS_INFO( mLogger, "CConnectionHandler::setConnectionHandlerObserver()" );
        mpConnectionHandlerObserver = observer;
    }

    void CConnectionHandler::onDeviceListUpdated(const NsAppLink::NsTransportManager::tDeviceList & DeviceList)
    {
        LOG4CPLUS_INFO( mLogger, "CConnectionHandler::onDeviceListUpdated()" );
        NsAppLink::NsTransportManager::tDeviceList::const_iterator it_in;
        for (it_in = DeviceList.begin(); it_in != DeviceList.end(); it_in++)
        {
            tDeviceListIterator it = mDeviceList.find((*it_in).mDeviceHandle);
            if (it == mDeviceList.end())
            {
                LOG4CPLUS_INFO( mLogger, "Add new device" << (*it_in).mUserFriendlyName << " Handler: " << (*it_in).mDeviceHandle);
                mDeviceList.insert(tDeviceList::value_type((*it_in).mDeviceHandle, CDevice((*it_in).mDeviceHandle, (*it_in).mUserFriendlyName)));
            }
        }
        if (0 != mpConnectionHandlerObserver)
        {
            mpConnectionHandlerObserver->onDeviceListUpdated(mDeviceList);
        }
    }

    void CConnectionHandler::onApplicationConnected(const NsAppLink::NsTransportManager::SDeviceInfo & ConnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle Connection)
    {
        LOG4CPLUS_INFO( mLogger, "CConnectionHandler::onApplicationConnected()" );
        tDeviceListIterator it = mDeviceList.find(ConnectedDevice.mDeviceHandle);
        if (it == mDeviceList.end())
        {
            LOG4CPLUS_ERROR( mLogger, "Unknown device!");
        }
        LOG4CPLUS_INFO( mLogger, "Add Connection:" << Connection << "to the list." );
        mConnectionList.insert(tConnectionList::value_type(Connection, CConnection(Connection, ConnectedDevice.mDeviceHandle)));
    }

    void CConnectionHandler::onApplicationDisconnected(const NsAppLink::NsTransportManager::SDeviceInfo & DisconnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle Connection)
    {
        
    }

    int CConnectionHandler::onSessionStartedCallback(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle)
    {
        return 0;
    }
    
    int CConnectionHandler::onSessionEndedCallback(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle, 
                                               unsigned char sessionId,
                                               unsigned int hashCode)
    {
        return 0;
    }
    
    int CConnectionHandler::keyFromPair(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle, 
                                           unsigned char sessionId)
    {
        int key = connectionHandle|(sessionId << 16);
        LOG4CPLUS_INFO( mLogger, "Key for ConnectionHandle:" << connectionHandle << " Session:" << sessionId << "is:" << key );
        return key;
    }
    
    void CConnectionHandler::pairFromKey(int key, NsAppLink::NsTransportManager::tConnectionHandle & connectionHandle, 
                                           unsigned char & sessionId)
    {
        connectionHandle = key & 0xFF00FFFF;
        sessionId = key >> 16;
        LOG4CPLUS_INFO( mLogger, "ConnectionHandle:" << connectionHandle << " Session:" << sessionId << " for key:" << key );
    }

}/* namespace NsConnectionHandler */
