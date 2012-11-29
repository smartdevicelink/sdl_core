/**
 * \file CConnectionHandler.hpp
 * \brief Connection handler class.
 * Observes TransportManager and ProtocolHandler, stores information regarding connections
 * and sessions and provides it to AppManager.
 * \author AKara
 */

#ifndef CONNECTIONHANDLER_H 
#define CONNECTIONHANDLER_H 

#include <map>

#include "Logger.hpp"
#include "TransportManager/ITransportManagerDeviceListener.hpp"
#include "TransportManager/SDeviceInfo.hpp"
#include "ProtocolHandler/ISessionObserver.h"
#include "ConnectionHandler/IConnectionHandlerObserver.hpp"
#include "ConnectionHandler/CDevice.hpp"
#include "ConnectionHandler/CConnection.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{
    /**
    * \class CConnectionHandler
    * \brief AppLink ConnectionHandler main class
    */
    class CConnectionHandler: public NsAppLink::NsTransportManager::ITransportManagerDeviceListener, public NsProtocolHandler::ISessionObserver
    {
    public:
        /**
        * \brief Singletone instantiator.
        * \return pointer to CConnectionHandler instance.
        */
        static CConnectionHandler* getInstance();

        /**
         * \brief Destructor
         */
        ~CConnectionHandler();

        /**
         * \brief Sets observer pointer for ConnectionHandler.
         * \param observer Pointer to observer object.
         **/
        void setConnectionHandlerObserver(IConnectionHandlerObserver * observer);

        /**
         * \brief Available devices list updated.
         *
         * Called when device scanning initiated with scanForNewDevices
         * is completed.
         *
         * \param DeviceList New list of available devices.
         **/
        virtual void onDeviceListUpdated(const NsAppLink::NsTransportManager::tDeviceList & DeviceList);

        /**
         * \brief Application connected.
         *
         * \param ConnectedDevice DeviceInfo of connected device.
         * \param Connection Connection handle.
         **/
        virtual void onApplicationConnected(const NsAppLink::NsTransportManager::SDeviceInfo & ConnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle Connection);

        /**
         * \brief Application disconnected.
         *
         * \param DisconnectedDevice DeviceInfo of disconnected device.
         * \param Connection Connection handle.
         **/
        virtual void onApplicationDisconnected(const NsAppLink::NsTransportManager::SDeviceInfo & DisconnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle Connection);

        virtual int onSessionStartedCallback(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle);

        virtual int onSessionEndedCallback(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle, 
                                               unsigned char sessionId,
                                               unsigned int hashCode);

        virtual int keyFromPair(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle, 
                                               unsigned char sessionId);

        virtual void pairFromKey(int key, NsAppLink::NsTransportManager::tConnectionHandle & connectionHandle, 
                                               unsigned char & sessionId);

    private:
        /**
         * \brief Default class constructor
         */
        CConnectionHandler();

        /**
         * \brief Copy constructor
         */
        CConnectionHandler(const CConnectionHandler&);

        /**
         * \brief Pointer to observer
         */
        IConnectionHandlerObserver* mpConnectionHandlerObserver;

        /**
         * \brief List of devices
         */
        tDeviceList mDeviceList;

        /**
         * \brief List of connections
         */
        tConnectionList mConnectionList;

        /**
          *\brief For logging.
        */
        static log4cplus::Logger mLogger;
    };
}/* namespace NsConnectionHandler */

#endif /* CONNECTIONHANDLER_H */