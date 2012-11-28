/**
 * \file IConnectionHandlerObserver.hpp
 * \brief IConnectionHandlerObserver class.
 * \author AKara
 */

#ifndef CONNECTIONHANDLEROBSERVER_H 
#define CONNECTIONHANDLEROBSERVER_H 

#include "Logger.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{
    /**
    * \class IConnectionHandlerObserver
    * \brief IConnectionHandlerObserver class
    */
    class IConnectionHandlerObserver
    {
    public:

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

        virtual void onSessionStartedCallback(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle, 
                                               unsigned char sessionId);

        virtual void onSessionEndedCallback(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle, 

    protected:
        /**
         * \brief Destructor
         */
        ~IConnectionHandlerObserver();
    };
}/* namespace NsConnectionHandler */

#endif /* CONNECTIONHANDLEROBSERVER_H */