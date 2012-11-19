#include "AppMgr/AppMgr.h"
#include "LoggerHelper.hpp"
#include "ProtocolHandler.hpp"

#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/JSONRPC2Handler.h"
#include "JSONHandler/ALRPCMessage.h"
#include "JSONHandler/ALRPCRequest.h"
#include "JSONHandler/ALRPCResponse.h"
#include "JSONHandler/ALRPCNotification.h"
#include "JSONHandler/ALRPCObjects/Marshaller.h"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Notification.h"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("testAppMgr"));

int basicWorkflow()
{
    NsAppManager::AppMgr& appMgr = NsAppManager::AppMgr::getInstance();

    NsAppLinkRPC::RegisterAppInterface_request* registerApp = new NsAppLinkRPC::RegisterAppInterface_request();
    registerApp->set_appName("MyNewFuckingTestApp");
    registerApp->set_isMediaApplication(true);
    registerApp->set_languageDesired(NsAppLinkRPC::Language::EN_US);
    registerApp->set_ngnMediaScreenAppName("NgnMediaScreenAppName");
    registerApp->set_usesVehicleData(false);
    std::vector<std::string> vrSynonyms;
    vrSynonyms.push_back("qwqwwqwqw");
    vrSynonyms.push_back("asdadsadd");
    vrSynonyms.push_back("zxczczcxzc");
    registerApp->set_vrSynonyms(vrSynonyms);

    appMgr.onMessageReceivedCallback(registerApp, 0);

    return EXIT_SUCCESS;
}

int main()
{
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
    LOG4CPLUS_INFO_EXT(logger, " Application started!");

 //   NsTransportLayer::CBTAdapter btadapter;

//    JSONHandler jsonHandler;

//    AxisCore::ProtocolHandler* pProtocolHandler = new AxisCore::ProtocolHandler(&jsonHandler, &btadapter, 1);

//    jsonHandler.setProtocolHandler(pProtocolHandler);

    NsAppManager::AppMgr& appMgr = NsAppManager::AppMgr::getInstance();

 //   jsonHandler.setRPCMessagesObserver(&appMgr);

 //   appMgr.setJsonHandler(&jsonHandler);


    LOG4CPLUS_INFO_EXT(logger, "Start AppMgr threads!");
    appMgr.executeThreads();

    basicWorkflow();

    for(;;)
    {
    }

    LOG4CPLUS_INFO_EXT(logger, "Stopping AppMgr threads!");

    return EXIT_SUCCESS;
}
