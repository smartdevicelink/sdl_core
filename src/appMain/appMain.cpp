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
#include <fstream>
#include <sys/stat.h>
#include <signal.h>

#include "appMain.hpp"
#include "CBTAdapter.hpp"

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
/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char** argv)
{
    pid_t pid_hmi = 0;
    /*** Components instance section***/
    /**********************************/
    Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("appMain"));
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
    LOG4CPLUS_INFO(logger, " Application started!");

    NsTransportLayer::CBTAdapter btadapter;

    JSONHandler jsonHandler;

    AxisCore::ProtocolHandler* pProtocolHandler = new AxisCore::ProtocolHandler(&jsonHandler, &btadapter, 1);

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

    NsMessageBroker::TcpServer *pJSONRPC20Server = new NsMessageBroker::TcpServer(std::string("0.0.0.0"), 8087, pMessageBroker);
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
    } else
    {
      LOG4CPLUS_INFO(logger, "Bind successful!");
    }

    if(!pJSONRPC20Server->Listen())
    {
      LOG4CPLUS_FATAL(logger, "Listen failed!");
      exit(EXIT_FAILURE);
    } else
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
    appMgr.executeThreads();

    /**********************************/
    /*********** Start HMI ************/
    struct stat sb;
    if (stat("hmi_link", &sb) == -1)
    {
        LOG4CPLUS_INFO(logger, "File with HMI link doesn't exist!");
    } else
    {
        std::ifstream file_str;
        file_str.open ("hmi_link");

        if (!file_str.is_open())
        {
            LOG4CPLUS_INFO(logger, "File with HMI link was not opened!");
        } else
        {
            file_str.seekg(0, std::ios::end);
            int length = file_str.tellg();
            file_str.seekg(0, std::ios::beg);
            char * raw_data = new char[length+1];
            memset(raw_data, 0, length+1);
            file_str.getline(raw_data, length+1);
            std::string hmi_link = std::string(raw_data, strlen(raw_data));
            delete[] raw_data;
            LOG4CPLUS_INFO(logger, "Input string:" << hmi_link << " length = " << hmi_link.size());
            file_str.close();
            if (stat(hmi_link.c_str(), &sb) == -1)
            {
                LOG4CPLUS_INFO(logger, "HMI index.html doesn't exist!");
            } else
            {
                pid_hmi = fork(); /* Create a child process */

                switch (pid_hmi)
                {
                    case -1: /* Error */
                    {
                        LOG4CPLUS_INFO(logger, "fork() failed!");
                        break;
                    }
                    case 0: /* Child process */
                    {
                        execlp("/usr/bin/chromium-browser",
                              "chromium-browser",
                              hmi_link.c_str(),
                              (char *) 0); /* Execute the program */
                        LOG4CPLUS_INFO(logger, "execl() failed! Install chromium-browser!");
                        return EXIT_SUCCESS;
                    }
                    default: /* Parent process */
                    {
                        LOG4CPLUS_INFO(logger, "Process created with pid " << pid_hmi);
                    }
                }
            }
        }
    }
    /**********************************/
    /*** Check main function parameters***/
    if (4 < argc)
    {
      LOG4CPLUS_ERROR(logger, "too many arguments");
      if (0 != pid_hmi)
      {
        kill(pid_hmi, SIGQUIT);
      }
      return EXIT_SUCCESS;
    } else if(1 < argc)
    {
      LOG4CPLUS_INFO(logger, "perform test");
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
      delete pProtocolHandler;
      pProtocolHandler = new AxisCore::ProtocolHandler(&jsonHandler, &apptester, 1);
      jsonHandler.setProtocolHandler(pProtocolHandler);
      apptester.startSession(sessioncount);
      apptester.sendDataFromFile(argv[1]);
      while(true)
        {
            sleep(1);
        }
    }
    /**********************************/

    /*** Start BT Devices Discovery***/

    std::vector<NsTransportLayer::CBTDevice> devicesFound;
    btadapter.scanDevices(devicesFound);
    if (0 < devicesFound.size())
    {
        LOG4CPLUS_INFO(logger, "Found " << devicesFound.size() << " devices.");
        printf("Please make your choice, 0 for exit:\n");
        printf("\n");
    } else
    {
        LOG4CPLUS_FATAL(logger, "No any devices found!");
        if (0 != pid_hmi)
        {
          kill(pid_hmi, SIGQUIT);
        }
        return EXIT_SUCCESS;
    }

    std::vector<NsTransportLayer::CBTDevice>::iterator it;
    int i = 1;
    for(it = devicesFound.begin(); it != devicesFound.end(); it++)
    {
        NsTransportLayer::CBTDevice device = *it;
        printf("%d: %s %s\n", i++, device.getDeviceAddr().c_str(), device.getDeviceName().c_str());
    }

    std::cin >> i;
    std::string discoveryDeviceAddr = "";
    if ((0 < i) && (i <= devicesFound.size()))
    {
        discoveryDeviceAddr = devicesFound[i-1].getDeviceAddr();
    } else
    {
        LOG4CPLUS_INFO(logger, "Exit!");
        if (0 != pid_hmi)
        {
          kill(pid_hmi, SIGQUIT);
        }
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
        LOG4CPLUS_FATAL(logger, "No any ports discovered!");
        if (0 != pid_hmi)
        {
          kill(pid_hmi, SIGQUIT);
        }
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
        LOG4CPLUS_INFO(logger, "Exit!");
        if (0 != pid_hmi)
        {
          kill(pid_hmi, SIGQUIT);
        }
        return EXIT_SUCCESS;
    }

    /*** Start RFCOMM connection***/

    int sockID = btadapter.startRFCOMMConnection(discoveryDeviceAddr.c_str(), portRFCOMM);

    if (0 < sockID)
    {
        btadapter.processRFCOMM(sockID);
    }

    if (0 != pid_hmi)
    {
      kill(pid_hmi, SIGQUIT);
    }
    return EXIT_SUCCESS;
} 

