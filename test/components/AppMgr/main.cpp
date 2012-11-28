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

#include "CMessageBroker.hpp"
#include "mb_tcpserver.hpp"
#include "networking.h"
#include "system.h"

#include "hmiEmulator/AppLinkCoreClientRPC.hpp"
#include "hmiEmulator/ButtonsRPC.hpp"
#include "hmiEmulator/TTSRPC.hpp"
#include "hmiEmulator/UIRPC.hpp"
#include "hmiEmulator/VRRPC.hpp"
#include "TestEnvironment.h"

Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("testAppMgr"));

void OnMobileMessageReceived(const NsAppLinkRPC::ALRPCMessage* msg, int connectionId, unsigned char sessionId)
{
    LOG4CPLUS_INFO_EXT(logger, " Mobile RPC message received: message " << msg->getMethodId() << " connection " << connectionId << " session " << (uint)sessionId);
}

int basicWorkflow(NsTest::TestEnvironment& jsonHandler)
{
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

    jsonHandler.sendRPCMessageToHmi(registerApp, 0, 0);

    return EXIT_SUCCESS;
}

int main()
{
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
    LOG4CPLUS_INFO_EXT(logger, " Application started!");

    NsTest::TestEnvironment jsonHandler;

    NsAppManager::AppMgr& appMgr = NsAppManager::AppMgr::getInstance();

    jsonHandler.registerMobileMessageReceivedCallback(OnMobileMessageReceived);

    jsonHandler.setRPCMessagesObserver(&appMgr);

    appMgr.setJsonHandler(&jsonHandler);

    LOG4CPLUS_INFO_EXT(logger, "Start AppMgr threads!");
    appMgr.executeThreads();

    //============START MESSAGEBROKER===========//
    NsMessageBroker::CMessageBroker *pMessageBroker = NsMessageBroker::CMessageBroker::getInstance();
    if (!pMessageBroker)
    {
        LOG4CPLUS_INFO_EXT(logger, " Wrong pMessageBroker pointer!");
        return EXIT_SUCCESS;
    }

    NsMessageBroker::TcpServer *pJSONRPC20Server = new NsMessageBroker::TcpServer(std::string("0.0.0.0"), 8087, pMessageBroker);
    if (!pJSONRPC20Server)
    {
        LOG4CPLUS_INFO_EXT(logger, " Wrong pJSONRPC20Server pointer!");
        return EXIT_SUCCESS;
    }
    pMessageBroker->startMessageBroker(pJSONRPC20Server);
    if(!networking::init())
    {
      LOG4CPLUS_INFO_EXT(logger, " Networking initialization failed!");
    }

    if(!pJSONRPC20Server->Bind())
    {
      LOG4CPLUS_FATAL_EXT(logger, "Bind failed!");
      exit(EXIT_FAILURE);
    } else
    {
      LOG4CPLUS_INFO_EXT(logger, "Bind successful!");
    }

    if(!pJSONRPC20Server->Listen())
    {
      LOG4CPLUS_FATAL_EXT(logger, "Listen failed!");
      exit(EXIT_FAILURE);
    } else
    {
      LOG4CPLUS_INFO_EXT(logger, " Listen successful!");
    }

    LOG4CPLUS_INFO_EXT(logger, "Start CMessageBroker thread!");
    System::Thread th1(new System::ThreadArgImpl<NsMessageBroker::CMessageBroker>(*pMessageBroker, &NsMessageBroker::CMessageBroker::MethodForThread, NULL));
    th1.Start(false);

    LOG4CPLUS_INFO_EXT(logger, "Start MessageBroker TCP server thread!");
    System::Thread th2(new System::ThreadArgImpl<NsMessageBroker::TcpServer>(*pJSONRPC20Server, &NsMessageBroker::TcpServer::MethodForThread, NULL));
    th2.Start(false);
    //==========================================//

    //===JSONRPC2HANDLER INSTANCE=======//
    JSONRPC2Handler jsonRPC2Handler( std::string("127.0.0.1"), 8087 );
    jsonRPC2Handler.setRPC2CommandsObserver( &appMgr );
    appMgr.setJsonRPC2Handler( &jsonRPC2Handler );
    if (!jsonRPC2Handler.Connect())
    {
        LOG4CPLUS_INFO_EXT(logger, "Cannot connect to remote peer!");
    }
    LOG4CPLUS_INFO_EXT(logger, "Start JSONRPC 2.0 controller receiver thread!");
    System::Thread th3(new System::ThreadArgImpl<JSONRPC2Handler>(jsonRPC2Handler, &JSONRPC2Handler::MethodForReceiverThread, NULL));
    th3.Start(false);

    jsonRPC2Handler.registerController();
    jsonRPC2Handler.subscribeToNotifications();
    //==========================================//

    //===HMI EMULATORCONTROLLERS INSTANCE=======//
    NsHMIEmulator::AppLinkCoreClientRPC appLinkCoreClientRPC( std::string("127.0.0.1"), 8087 );
    if (!appLinkCoreClientRPC.Connect())
    {
        LOG4CPLUS_INFO_EXT(logger, "Cannot connect to remote peer!");
    }
    LOG4CPLUS_INFO_EXT(logger, "Start appLinkCoreClientRPC receiver thread!");
    System::Thread th4(new System::ThreadArgImpl<NsHMIEmulator::AppLinkCoreClientRPC>(appLinkCoreClientRPC, &NsHMIEmulator::AppLinkCoreClientRPC::MethodForReceiverThread, NULL));
    th4.Start(false);
    appLinkCoreClientRPC.registerController(1);

    NsHMIEmulator::ButtonsRPC buttonsRPC( std::string("127.0.0.1"), 8087 );
    if (!buttonsRPC.Connect())
    {
        LOG4CPLUS_INFO_EXT(logger, "Cannot connect to remote peer!");
    }
    LOG4CPLUS_INFO_EXT(logger, "Start buttonsRPC receiver thread!");
    System::Thread th5(new System::ThreadArgImpl<NsHMIEmulator::ButtonsRPC>(buttonsRPC, &NsHMIEmulator::ButtonsRPC::MethodForReceiverThread, NULL));
    th5.Start(false);
    buttonsRPC.registerController(2);

    NsHMIEmulator::TTSRPC tTSRPC( std::string("127.0.0.1"), 8087 );
    if (!tTSRPC.Connect())
    {
        LOG4CPLUS_INFO_EXT(logger, "Cannot connect to remote peer!");
    }
    LOG4CPLUS_INFO_EXT(logger, "Start tTSRPC receiver thread!");
    System::Thread th6(new System::ThreadArgImpl<NsHMIEmulator::TTSRPC>(tTSRPC, &NsHMIEmulator::TTSRPC::MethodForReceiverThread, NULL));
    th6.Start(false);
    tTSRPC.registerController(3);

    NsHMIEmulator::UIRPC uIRPC( std::string("127.0.0.1"), 8087 );
    if (!uIRPC.Connect())
    {
        LOG4CPLUS_INFO_EXT(logger, "Cannot connect to remote peer!");
    }
    LOG4CPLUS_INFO_EXT(logger, "Start uIRPC receiver thread!");
    System::Thread th7(new System::ThreadArgImpl<NsHMIEmulator::UIRPC>(uIRPC, &NsHMIEmulator::UIRPC::MethodForReceiverThread, NULL));
    th7.Start(false);
    uIRPC.registerController(4);

    NsHMIEmulator::VRRPC vRRPC( std::string("127.0.0.1"), 8087 );
    if (!vRRPC.Connect())
    {
        LOG4CPLUS_INFO_EXT(logger, "Cannot connect to remote peer!");
    }
    LOG4CPLUS_INFO_EXT(logger, "Start uIRPC receiver thread!");
    System::Thread th8(new System::ThreadArgImpl<NsHMIEmulator::VRRPC>(vRRPC, &NsHMIEmulator::VRRPC::MethodForReceiverThread, NULL));
    th8.Start(false);
    vRRPC.registerController(5);
    //==========================================//

    basicWorkflow(jsonHandler);

    for(;;)
    {
    }

    LOG4CPLUS_INFO_EXT(logger, "Stopping AppMgr threads!");

    return EXIT_SUCCESS;
}
