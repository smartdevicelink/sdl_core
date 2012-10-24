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
#include "CBTAdapter.hpp"

#include "ProtocolHandler.hpp"

#include "JSONHandler/JSONHandler.h"

#include "AppMgr/AppMgr.h"

#include "CMessageBroker.hpp"

#include "mb_tcpserver.hpp"

#include "networking.h"

#include "system.h"

#include "Logger.hpp"

#include "CAppTester.hpp"
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

    NsTransportLayer::CBTAdapter btadapter;

    JSONHandler jsonHandler;

    AxisCore::ProtocolHandler protocolHandler =  AxisCore::ProtocolHandler(&jsonHandler, &btadapter, 1);

    jsonHandler.setProtocolHandler(&protocolHandler);

    NsAppManager::AppMgr::setParams(std::string("127.0.0.1"), 8087, std::string("AppMgr"));

    NsAppManager::AppMgr& appMgr = NsAppManager::AppMgr::getInstance();

    jsonHandler.setRPCMessagesObserver(&appMgr);

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
      printf("Bind failed!\n");
      exit(EXIT_FAILURE);
    } else
    {
      printf("Bind successful!\n");
    }

    if(!pJSONRPC20Server->Listen())
    {
      printf("Listen failed!\n");
      exit(EXIT_FAILURE);
    } else
    {
      LOG4CPLUS_INFO(logger, " Listen successful!");
    }

    NsAppManager::AppLinkInterface appLinkInterface = appMgr.getAppLinkInterface();

    if(!appLinkInterface.Connect())
    {
      printf("Cannot connect to remote peer!\n");
      exit(EXIT_FAILURE);
    } else
    {
      printf("AppMgr JSONRPC 2.0 controller connected to the server! SocketID = %d\n", appLinkInterface.GetSocket());
    }

    printf("Start CMessageBroker thread!\n");
    System::Thread th1(new System::ThreadArgImpl<NsMessageBroker::CMessageBroker>(*pMessageBroker, &NsMessageBroker::CMessageBroker::MethodForThread, NULL));
    th1.Start(false);

    printf("Start MessageBroker TCP server thread!\n");
    System::Thread th2(new System::ThreadArgImpl<NsMessageBroker::TcpServer>(*pJSONRPC20Server, &NsMessageBroker::TcpServer::MethodForThread, NULL));
    th2.Start(false);

    printf("StartAppMgr JSONRPC 2.0 controller receiver thread!\n");
    System::Thread th3(new System::ThreadArgImpl<NsAppManager::AppLinkInterface>(appLinkInterface, &NsAppManager::AppLinkInterface::MethodForReceiverThread, NULL));
    th3.Start(false);

    printf("Start AppMgr!\n");
    appMgr.startAppMgr();
    printf("After Start AppMgr!\n");

    /**********************************/
    /*** Check main function parameters***/
    if (4 > argc)
    {
      printf("too many arguments\n");
      return EXIT_SUCCESS;
    } else if(1 < argc)
    {
      printf("perform test\n");
      int sessioncount = 1;
      if (argc == 3)
      {
        sessioncount = atoi(argv[2]);
        if (0 >= sessioncount)
        {
           sessioncount = 1;
        }
      }
      NsApplicationTester::CAppTester apptester;
      apptester.startSession(sessioncount);
      apptester.sendDataFromFile(argv[1]);
    }
    /**********************************/

    /*** Start BT Devices Discovery***/

    std::vector<NsTransportLayer::CBTDevice> devicesFound;
    btadapter.scanDevices(devicesFound);
    if (0 < devicesFound.size())
    {
        printf("Found %d devices\n", devicesFound.size());
        printf("Please make your choice, 0 for exit:\n");
        printf("\n");
    } else
    {
        printf("No any devices found!\n");
        return EXIT_SUCCESS;
    }

    std::vector<NsTransportLayer::CBTDevice>::iterator it;
    int i = 1;
    for(it = devicesFound.begin(); it != devicesFound.end(); it++)
    {
        NsTransportLayer::CBTDevice device = *it;
        printf("%d: %s %s \n", i++, device.getDeviceAddr().c_str(), device.getDeviceName().c_str());
    }

    std::cin >> i;
    std::string discoveryDeviceAddr = "";
    if ((0 < i) && (i <= devicesFound.size()))
    {
        discoveryDeviceAddr = devicesFound[i-1].getDeviceAddr();
    } else
    {
        printf("Exit!\n");
        return EXIT_SUCCESS;
    }

    /*** Start SDP Discovery on device***/

    std::vector<int> portsRFCOMMFound;
    btadapter.startSDPDiscoveryOnDevice(discoveryDeviceAddr.c_str(), portsRFCOMMFound);
    if (0 < portsRFCOMMFound.size())
    {
        printf("Found %d ports on %s device\n", portsRFCOMMFound.size(), discoveryDeviceAddr.c_str());
        printf("Please make your choice, 0 for exit:\n");
    } else
    {
        printf("No any ports discovered!\n");
        return EXIT_SUCCESS;
    }

    std::vector<int>::iterator itr;
    int j = 1;
    for(itr = portsRFCOMMFound.begin(); itr != portsRFCOMMFound.end(); itr++)
    {
        printf("%d: %d \n", j++, *itr);
    }

    std::cin >> j;
    int portRFCOMM = 0;
    if ((0 < j) && (j <= portsRFCOMMFound.size()))
    {
        portRFCOMM = portsRFCOMMFound[j-1];
    } else
    {
        printf("Exit!\n");
        return EXIT_SUCCESS;
    }

    /*** Start RFCOMM connection***/

    int sockID = btadapter.startRFCOMMConnection(discoveryDeviceAddr.c_str(), portRFCOMM);

    if (0 < sockID)
    {
        btadapter.processRFCOMM(sockID);
    }

    return EXIT_SUCCESS;
} 

