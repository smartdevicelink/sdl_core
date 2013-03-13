/**
 * \file CConnectionHandler.hpp
 * \brief Connection handler class.
 * Observes TransportManager and ProtocolHandler, stores information regarding connections
 * and sessions and provides it to AppManager.
 * Copyright (c) 2013 Ford Motor Company
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
#include "ConnectionHandler/IDevicesDiscoveryStarter.hpp"
#include "ConnectionHandler/IConnectionHandler.hpp"
/**
 * \namespace NsConnectionHandler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace NsConnectionHandler
{
    /**
    * \class CConnectionHandler
    * \brief SmartDeviceLink ConnectionHandler main class
    */
    class CConnectionHandler: public NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener,
                              public NsProtocolHandler::ISessionObserver,
                              public IDevicesDiscoveryStarter,
                              public IConnectionHandler
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
        virtual void onDeviceListUpdated(const NsSmartDeviceLink::NsTransportManager::tDeviceList & DeviceList);

        /**
         * \brief Application connected.
         *
         * \param ConnectedDevice DeviceInfo of connected device.
         * \param Connection Connection handle.
         **/
        virtual void onApplicationConnected(const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & ConnectedDevice, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection);

        /**
         * \brief Application disconnected.
         *
         * \param DisconnectedDevice DeviceInfo of disconnected device.
         * \param Connection Connection handle.
         **/
        virtual void onApplicationDisconnected(const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & DisconnectedDevice, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection);

        virtual int onSessionStartedCallback(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle);

        virtual int onSessionEndedCallback(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
                                               unsigned char sessionId,
                                               unsigned int hashCode);

        virtual int keyFromPair(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
                                               unsigned char sessionId);

        virtual void pairFromKey(int key, NsSmartDeviceLink::NsTransportManager::tConnectionHandle & connectionHandle,
                                               unsigned char & sessionId);

        /**
         * \brief Sets pointer to TransportManager.
         * \param transportManager Pointer to TransportManager object.
         **/
        void setTransportManager( NsSmartDeviceLink::NsTransportManager::ITransportManager * transportManager );

        /**
         * \brief Method which should start devices discoveryng
         */
        virtual void startDevicesDiscovery();

        /**
         * \brief Connects to all services of device
         * \param deviceHandle Handle of device to connect to
         */
        virtual void connectToDevice( NsConnectionHandler::tDeviceHandle deviceHandle );

        void startTransportManager();

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
         * \brief Pointer to TransportManager
         */
        NsSmartDeviceLink::NsTransportManager::ITransportManager * mpTransportManager;

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
