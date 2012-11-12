/**
 * \file appMain.cpp
 * \brief AppLink main application sources
 * \author AKara
 */

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

#include "appMain.hpp"

#include "ProtocolHandler.hpp"

#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/JSONRPC2Handler.h"

#include "AppMgr/AppMgr.h"
#include "AppMgr/AppMgrCore.h"

#include "CMessageBroker.hpp"

#include "mb_tcpserver.hpp"

#include "networking.h"

#include "system.h"

#include "Logger.hpp"

#include "CAppTester.hpp"

#include "TransportManager/ITransportManager.hpp"

/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char** argv)
{
    /*** Components instance section***/
    /**********************************/
    Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("appMain"));
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
    LOG4CPLUS_INFO(logger, " Application started!");

    NsAppLink::NsTransportManager::ITransportManager * transportManager = NsAppLink::NsTransportManager::ITransportManager::create();
    transportManager->run();

    JSONHandler jsonHandler;

    AxisCore::ProtocolHandler* pProtocolHandler = new AxisCore::ProtocolHandler(&jsonHandler, transportManager, 1);

    jsonHandler.setProtocolHandler(pProtocolHandler);

    NsAppManager::AppMgr& appMgr = NsAppManager::AppMgr::getInstance();

    jsonHandler.setRPCMessagesObserver(&appMgr);

    appMgr.setJsonHandler(&jsonHandler);

    NsMessageBroker::CMessageBroker *pMessageBroker = NsMessageBroker::CMessageBroker::getInstance();
    if (!pMessageBroker)
    {
        LOG4CPLUS_INFO(logger, " Wrong pMessageBroker pointer!");
        return EXIT_SUCCESS;
    }

    NsMessageBroker::TcpServer *pJSONRPC20Server = new NsMessageBroker::TcpServer(std::string("127.0.0.1"), 8087, pMessageBroker);
    if (!pJSONRPC20Server)
    {
        LOG4CPLUS_INFO(logger, " Wrong pJSONRPC20Server pointer!");
        return EXIT_SUCCESS;
    }
    pMessageBroker->startMessageBroker(pJSONRPC20Server);
    if(!networking::init())
    {
      LOG4CPLUS_INFO(logger, " Networking initialization failed!");
    }

    if(!pJSONRPC20Server->Bind())
    {
      LOG4CPLUS_FATAL(logger, "Bind failed!");
      exit(EXIT_FAILURE);
    } 
    else
    {
      LOG4CPLUS_INFO(logger, "Bind successful!");
    }

    if(!pJSONRPC20Server->Listen())
    {
      LOG4CPLUS_FATAL(logger, "Listen failed!");
      exit(EXIT_FAILURE);
    } 
    else
    {
      LOG4CPLUS_INFO(logger, " Listen successful!");
    }

    JSONRPC2Handler jsonRPC2Handler( std::string("127.0.0.1"), 8087 );
    jsonRPC2Handler.setRPC2CommandsObserver( &appMgr );
    appMgr.setJsonRPC2Handler( &jsonRPC2Handler );
    if (!jsonRPC2Handler.Connect())
    {
        LOG4CPLUS_INFO(logger, "Cannot connect to remote peer!");
    }

    LOG4CPLUS_INFO(logger, "Start CMessageBroker thread!");
    System::Thread th1(new System::ThreadArgImpl<NsMessageBroker::CMessageBroker>(*pMessageBroker, &NsMessageBroker::CMessageBroker::MethodForThread, NULL));
    th1.Start(false);

    LOG4CPLUS_INFO(logger, "Start MessageBroker TCP server thread!");
    System::Thread th2(new System::ThreadArgImpl<NsMessageBroker::TcpServer>(*pJSONRPC20Server, &NsMessageBroker::TcpServer::MethodForThread, NULL));
    th2.Start(false);

    LOG4CPLUS_INFO(logger, "StartAppMgr JSONRPC 2.0 controller receiver thread!");
    //System::Thread th3(new System::ThreadArgImpl<NsAppManager::AppLinkInterface>(appLinkInterface, &NsAppManager::AppLinkInterface::MethodForReceiverThread, NULL));
    System::Thread th3(new System::ThreadArgImpl<JSONRPC2Handler>(jsonRPC2Handler, &JSONRPC2Handler::MethodForReceiverThread, NULL));
    th3.Start(false);

    jsonRPC2Handler.registerController();
    jsonRPC2Handler.subscribeToNotifications();

    LOG4CPLUS_INFO(logger, "Start AppMgr threads!");
    NsAppManager::AppMgrCore& appMgrCore = NsAppManager::AppMgrCore::getInstance();
    appMgrCore.executeThreads();

    while (1)
    {
        sleep(100500);
    }
        
    return EXIT_SUCCESS;
} 

