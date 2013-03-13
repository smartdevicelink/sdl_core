//
// Copyright (c) 2013 Ford Motor Company
//

#include "AppMgr/AppMgr.h"
#include "AppMgr/AppMgrCore.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
    log4cplus::Logger AppMgr::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgr"));

    /**
    * \brief Returning class instance
    * \return class instance
    */
    AppMgr& AppMgr::getInstance( )
    {
        static AppMgr appMgr;
        return appMgr;
    }

    /**
    * \brief Default class constructor
    */
    AppMgr::AppMgr()
    {
        LOG4CPLUS_INFO_EXT(mLogger, " AppMgr constructed!");
    }

    /**
    * \brief Default class destructor
    */
    AppMgr::~AppMgr()
    {
        LOG4CPLUS_INFO_EXT(mLogger, " AppMgr destructed!");
    }

    /**
    * \brief Copy constructor
    */
    AppMgr::AppMgr(const AppMgr &)
    {
    }

    /**
    * \brief Sets Json mobile handler instance
    * \param handler Json mobile handler
    */
    void AppMgr::setJsonHandler(JSONHandler* handler)
    {
        if(!handler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Setting null handler!");
            return;
        }
        AppMgrCore::getInstance().setJsonHandler( handler );
    }

    /**
    * \brief Sets Json RPC2 handler instance
    * \param handler Json RPC2 handler
    */
    void AppMgr::setJsonRPC2Handler(JSONRPC2Handler *handler)
    {
        if(!handler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Setting null handler!");
            return;
        }
        AppMgrCore::getInstance().setJsonRPC2Handler( handler );
    }

    /**
     * \brief Sets connection handler instance
     * \param handler connection handler
     */
    void AppMgr::setConnectionHandler(NsConnectionHandler::IDevicesDiscoveryStarter *handler)
    {
        if(!handler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Setting null handler!");
            return;
        }
        AppMgrCore::getInstance().setConnectionHandler( handler );
    }

    /**
    * \brief callback to proceed received mobile message
    * \param message the received message
    * \param connectionKey key of a connection associated with application that sent the message
    */
    void AppMgr::onMessageReceivedCallback(NsAppLinkRPC::ALRPCMessage * message, int connectionKey)
    {
        if(!message)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Calling a function with null command! connection key" << connectionKey);
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " Message " << message->getMethodId() << " received from mobile side");
        AppMgrCore::getInstance().pushMobileRPCMessage( message, connectionKey );
    }

    /**
    * \brief callback to proceed received RPC2 command
    * \param command the received command
    */
    void AppMgr::onCommandReceivedCallback(NsRPC2Communication::RPC2Command *command)
    {
        if(!command)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Calling a function with null command!");
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " Message " << command->getMethod() << " received from HMI side");
        AppMgrCore::getInstance().pushRPC2CommunicationMessage(command);
    }

    /**
     * \brief Available devices list updated.
     *
     * Called when device scanning initiated with scanForNewDevices
     * is completed.
     *
     * \param deviceList New list of available devices.
     **/
    void AppMgr::onDeviceListUpdated(const NsConnectionHandler::tDeviceList &deviceList)
    {
        AppMgrCore::getInstance().setDeviceList(deviceList);
    }

    /**
     * \brief callback which is called upon session starting
     * \param deviceHandle
     * \param sessionKey
     */
    void AppMgr::onSessionStartedCallback(NsConnectionHandler::tDeviceHandle deviceHandle, int sessionKey, int firstSessionKey)
    {
        AppMgrCore::getInstance().addDevice(deviceHandle, sessionKey, firstSessionKey);
    }

    /**
     * \brief callback which is called upon session ending
     * \param sessionKey
     */
    void AppMgr::onSessionEndedCallback(int sessionKey, int firstSessionKey)
    {
        AppMgrCore::getInstance().removeDevice(sessionKey, firstSessionKey);
    }

    /**
    * \brief method to execute threads.
    */
    void AppMgr::executeThreads()
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Threads are being started!");

        AppMgrCore::getInstance().executeThreads();

        LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
    }

}
