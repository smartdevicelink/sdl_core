#include "AppMgr/AppMgr.h"
#include "LoggerHelper.hpp"
#include "ProtocolHandler.hpp"

#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/JSONRPC2Handler.h"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

int main()
{
    Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("testAppMgr"));
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
    LOG4CPLUS_INFO_EXT(logger, " Application started!");

 //   NsTransportLayer::CBTAdapter btadapter;

//    JSONHandler jsonHandler;

//    AxisCore::ProtocolHandler* pProtocolHandler = new AxisCore::ProtocolHandler(&jsonHandler, &btadapter, 1);

//    jsonHandler.setProtocolHandler(pProtocolHandler);

    NsAppManager::AppMgr& appMgr = NsAppManager::AppMgr::getInstance();

 //   jsonHandler.setRPCMessagesObserver(&appMgr);

 //   appMgr.setJsonHandler(&jsonHandler);


    LOG4CPLUS_INFO(logger, "Start AppMgr threads!");
    appMgr.executeThreads();

    return EXIT_SUCCESS;


	return 0;
}
