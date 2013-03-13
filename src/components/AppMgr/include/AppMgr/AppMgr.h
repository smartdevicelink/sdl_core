//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef APPMGR_H
#define APPMGR_H

#include "JSONHandler/IRPCMessagesObserver.h"
#include "JSONHandler/IRPC2CommandsObserver.h"
#include "ConnectionHandler/IConnectionHandlerObserver.hpp"

class JSONHandler;
class ALRPCMessage;
class JSONRPC2Handler;

namespace log4cplus
{
    class Logger;
}

namespace NsRPC2Communication
{
    class RPC2Command;
}

namespace NsConnectionHandler
{
    class IDevicesDiscoveryStarter;
}

namespace NsAppManager
{
    class AppMgrRegistry;
    class AppMgrCore;
    class AppFactory;

    /**
    * \brief a main app manager class which acts like container for other classes
    */
    class AppMgr: public IRPCMessagesObserver, public IRPC2CommandsObserver, public NsConnectionHandler::IConnectionHandlerObserver
    {
    public:

        /**
         * \brief Returning class instance
         * \return class instance
         */
        static AppMgr& getInstance();

        /**
         * \brief callback to proceed received mobile message
         * \param message the received message
         * \param connectionKey key of a connection associated with application that sent the message
         */
        virtual void onMessageReceivedCallback( NsAppLinkRPC::ALRPCMessage * message, int connectionKey );

        /**
         * \brief callback to proceed received RPC2 command
         * \param command the received command
         */
        virtual void onCommandReceivedCallback( NsRPC2Communication::RPC2Command * command );

        /**
         * \brief Available devices list updated.
         *
         * Called when device scanning initiated with scanForNewDevices
         * is completed.
         *
         * \param deviceList New list of available devices.
         **/
        virtual void onDeviceListUpdated(const NsConnectionHandler::tDeviceList & deviceList);

        /**
         * \brief callback which is called upon session starting
         * \param deviceHandle
         * \param sessionKey
         */
        virtual void onSessionStartedCallback(NsConnectionHandler::tDeviceHandle deviceHandle, int sessionKey, int firstSessionKey);

        /**
         * \brief callback which is called upon session ending
         * \param sessionKey
         */
        virtual void onSessionEndedCallback(int sessionKey, int firstSessionKey);

        /**
         * \brief Sets Json mobile handler instance
         * \param handler Json mobile handler
         */
        void setJsonHandler(JSONHandler* handler);

        /**
         * \brief Sets Json RPC2 handler instance
         * \param handler Json RPC2 handler
         */
        void setJsonRPC2Handler(JSONRPC2Handler* handler);

        /**
         * \brief Sets connection handler instance
         * \param handler connection handler
         */
        void setConnectionHandler(NsConnectionHandler::IDevicesDiscoveryStarter *handler);

        /**
         * \brief method to execute threads.
         */
        void executeThreads();

    private:

        /**
         * \brief Default class destructor
         */
        virtual ~AppMgr();

        /**
         * \brief Copy constructor
         */
        AppMgr(const AppMgr&);

        /**
         * \brief Default class constructor
         */
        AppMgr();

        static log4cplus::Logger mLogger;
    };

} // namespace NsAppManager

#endif // APPMGR_H
