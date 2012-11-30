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
    mpConnectionHandlerObserver(NULL),
    mpTransportManager(NULL)
    {

    }

    CConnectionHandler::~CConnectionHandler()
    {
        
    }

    void CConnectionHandler::setConnectionHandlerObserver(IConnectionHandlerObserver * observer)
    {
        LOG4CPLUS_INFO( mLogger, "CConnectionHandler::setConnectionHandlerObserver()" );
        if ( !observer )
        {
            LOG4CPLUS_ERROR(mLogger, "Null pointer to observer.");
            return;
        }
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
                if ( mpTransportManager )
                {
                    mpTransportManager -> connectDevice((*it_in).mDeviceHandle);
                }
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
        LOG4CPLUS_INFO( mLogger, "CConnectionHandler::onSessionStartedCallback()" );
        int newSessionID = -1;
        tConnectionListIterator it = mConnectionList.find(connectionHandle);
        if (it == mConnectionList.end())
        {
            LOG4CPLUS_ERROR( mLogger, "Unknown connection!");
        } else
        {
            newSessionID = (it->second).addNewSession();
            if (0 > newSessionID)
            {
                LOG4CPLUS_ERROR( mLogger, "Not possible to start session!");
            } else
            {
                LOG4CPLUS_INFO( mLogger, "New session ID:" << newSessionID );
                if (0 != mpConnectionHandlerObserver)
                {
                    int sessionKey = keyFromPair(connectionHandle, newSessionID);
                    mpConnectionHandlerObserver->onSessionStartedCallback((it->second).getConnectionDeviceHandle()
                                                                         , sessionKey);
                }

            }
        }
        return newSessionID;
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

    void CConnectionHandler::setTransportManager( NsAppLink::NsTransportManager::ITransportManager * transportManager )
    {
        LOG4CPLUS_INFO( mLogger, "CConnectionHandler::setTransportManager()" );
        if ( !transportManager )
        {
            LOG4CPLUS_ERROR(mLogger, "Null pointer to TransportManager.");
            return;
        }
        mpTransportManager = transportManager;
    }
    void CConnectionHandler::startDevicesDiscovery()
    {
        LOG4CPLUS_INFO( mLogger, "CConnectionHandler::startDevicesDiscovery()" );
        if (NULL == mpTransportManager)
        {
            LOG4CPLUS_ERROR(mLogger, "Null pointer to TransportManager.");
            return;
        }
        mpTransportManager->scanForNewDevices();
    }
}/* namespace NsConnectionHandler */
