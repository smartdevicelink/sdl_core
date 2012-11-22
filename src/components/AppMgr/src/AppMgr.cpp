/**
 * \file AppMgr.cpp
 * \brief Application manager main component
 * \author vsalo
 */

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
    * \brief callback to proceed received mobile message
    * \param message the received message
    * \param connectionID id of a connection associated with application that sent the message
    * \param sessionID an id of a session associated with the application which sends message
    */
    void AppMgr::onMessageReceivedCallback(NsAppLinkRPC::ALRPCMessage * message, int connectionID, unsigned char sessionID)
    {
        if(!message)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Calling a function with null command! Session id " << (uint)sessionID << " connection id " << connectionID);
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " Message " << message->getMethodId() << " received from mobile side");
        AppMgrCore::getInstance().pushMobileRPCMessage( message, connectionID, sessionID );
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
    * \brief method to execute threads.
    */
    void AppMgr::executeThreads()
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Threads are being started!");

        AppMgrCore::getInstance().executeThreads();

        LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
    }

}
