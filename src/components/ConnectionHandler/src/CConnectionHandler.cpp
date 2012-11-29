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

        }

    }

    void CConnectionHandler::onApplicationConnected(const NsAppLink::NsTransportManager::SDeviceInfo & ConnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle Connection)
    {

    }

    void CConnectionHandler::onApplicationDisconnected(const NsAppLink::NsTransportManager::SDeviceInfo & DisconnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle Connection)
    {
        
    }

    void onSessionStartedCallback(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle)
    {

    }
    
    void onSessionEndedCallback(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle, 
                                           unsigned char sessionId)
    {

    }
    
    int keyFromPair(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle, 
                                           unsigned char sessionId)
    {
        return 0;
    }
    
    void pairFromKey(int key, NsAppLink::NsTransportManager::tConnectionHandle & connectionHandle, 
                                           unsigned char & sessionId)
    {

    }

}/* namespace NsConnectionHandler */
