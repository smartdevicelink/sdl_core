/**
 * \file IConnectionHandler.hpp
 * \brief Connection handler interface class.
 * \author AKara
 */

#ifndef ICONNECTIONHANDLER_H 
#define ICONNECTIONHANDLER_H 

#include "TransportManager/ITransportManagerDeviceListener.hpp"
#include "TransportManager/SDeviceInfo.hpp"
#include "ProtocolHandler/ISessionObserver.h"
#include "ConnectionHandler/IConnectionHandlerObserver.hpp"
#include "ConnectionHandler/CDevice.hpp"
#include "ConnectionHandler/CConnection.hpp"
#include "ConnectionHandler/IDevicesDiscoveryStarter.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{
    /**
    * \class IConnectionHandler
    * \brief AppLink ConnectionHandler interface class
    */
    class IConnectionHandler
    {
    public:
        /**
         * \brief Sets observer pointer for ConnectionHandler.
         * \param observer Pointer to observer object.
         **/
        virtual void setConnectionHandlerObserver(IConnectionHandlerObserver * observer)=0;

        /**
         * \brief Sets pointer to TransportManager.
         * \param transportManager Pointer to TransportManager object.
         **/
        virtual void setTransportManager( NsAppLink::NsTransportManager::ITransportManager * transportManager )=0;

    protected:
        /**
         * \brief Destructor
         */
        virtual ~IConnectionHandler(){};

    };
}/* namespace NsConnectionHandler */

#endif /* ICONNECTIONHANDLER_H */