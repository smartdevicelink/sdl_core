/**
 * \file IConnectionHandlerObserver.hpp
 * \brief IConnectionHandlerObserver class.
 * \author AKara
 */

#ifndef CONNECTIONHANDLEROBSERVER_H 
#define CONNECTIONHANDLEROBSERVER_H 

#include "Logger.hpp"

#include "ConnectionHandler/CDevice.hpp"
#include "ConnectionHandler/CConnection.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{
    /**
    * \class IConnectionHandlerObserver
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
        virtual void onDeviceListUpdated(const NsConnectionHandler::tDeviceList & DeviceList) = 0;

        virtual void onSessionStartedCallback(NsConnectionHandler::tDeviceHandle deviceHandle, 
                                               int sessionKey, int firstSessionKey) = 0;

        virtual void onSessionEndedCallback(int sessionKey) = 0;

    protected:
        /**
         * \brief Destructor
         */
        virtual ~IConnectionHandlerObserver() {};
    };
}/* namespace NsConnectionHandler */

#endif /* CONNECTIONHANDLEROBSERVER_H */
