/**
 * \file appMain.cpp
 * \brief SmartDeviceLink main application sources
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#include "appMain.hpp"

#include "protocol_handler/protocol_handler_impl.h"

#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/JSONRPC2Handler.h"
#include "connection_handler/connection_handler_impl.h"
#include "mobile_message_handler/mobile_message_handler_impl.h"
#include "utils/macro.h"

#include "AppMgr/AppMgr.h"
#include "AppMgr/AppMgrCore.h"

#include "CMessageBroker.hpp"

#include "mb_tcpserver.hpp"

#include "networking.h"

#include "system.h"

#include "utils/logger.h"
#include "config_profile/profile.h"

#include "TransportManager/ITransportManager.hpp"
#include "TransportManager/ITransportManagerDeviceListener.hpp"

class CTransportManagerListener : public NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener
{
public:

    CTransportManagerListener(NsSmartDeviceLink::NsTransportManager::ITransportManager * transportManager);

private:

    virtual void onDeviceListUpdated(const NsSmartDeviceLink::NsTransportManager::tDeviceList& DeviceList);

    NsSmartDeviceLink::NsTransportManager::ITransportManager * mTransportManager;
};

CTransportManagerListener::CTransportManagerListener(NsSmartDeviceLink::NsTransportManager::ITransportManager* transportManager)
: mTransportManager(transportManager)
{
}

void CTransportManagerListener::onDeviceListUpdated(const NsSmartDeviceLink::NsTransportManager::tDeviceList& DeviceList)
{
    if(DeviceList.empty())
    {
        printf("Device list is updated. No devices with SmartDeviceLink service are available\n");
    }
    else
    {
        printf("Device list is updated. To connect to device enter device number and press Enter\n");
        printf("If You don\'t want to connect to any device enter 0\n\n");

        int i = 1;
        for(NsSmartDeviceLink::NsTransportManager::tDeviceList::const_iterator it = DeviceList.begin(); it != DeviceList.end(); it++)
        {
          NsSmartDeviceLink::NsTransportManager::SDeviceInfo device = *it;
            printf("%d: %s (%s)\n", i++, device.mUniqueDeviceId.c_str(), device.mUserFriendlyName.c_str());
        }

        std::cin >> i;

        if ((0 < i) && (i <= DeviceList.size()))
        {
          NsSmartDeviceLink::NsTransportManager::SDeviceInfo device = DeviceList[i-1];
            printf("Performing connect to: %s (%s)\n", device.mUniqueDeviceId.c_str(), device.mUserFriendlyName.c_str());
            mTransportManager->connectDevice(device.mDeviceHandle);
        }
        else
        {
            printf("If You don\'t want to connect to any device enter 0\n\n");
        }

    }
}

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
    log4cxx::LoggerPtr logger =
        log4cxx::LoggerPtr(log4cxx::Logger::getLogger("appMain"));
    log4cxx::PropertyConfigurator::configure("log4cxx.properties");

    LOG4CXX_INFO(logger, " Application started!");

    NsSmartDeviceLink::NsTransportManager::ITransportManager * transportManager = NsSmartDeviceLink::NsTransportManager::ITransportManager::create();
    CTransportManagerListener tsl(transportManager);

    //JSONHandler jsonHandler;
    mobile_message_handler::MobileMessageHandlerImpl* mmh = mobile_message_handler::MobileMessageHandlerImpl::instance();
    DCHECK(mmh);

    protocol_handler::ProtocolHandlerImpl* pProtocolHandler = new protocol_handler::ProtocolHandlerImpl(transportManager);

    pProtocolHandler -> set_protocol_observer(mmh /*&jsonHandler*/ );

    transportManager -> addDataListener( pProtocolHandler );
    mmh->setProtocolHandler(pProtocolHandler);

    //jsonHandler.setProtocolHandler(pProtocolHandler);

    connection_handler::ConnectionHandlerImpl * connectionHandler = connection_handler::ConnectionHandlerImpl::getInstance();

    pProtocolHandler -> set_session_observer( connectionHandler );

    connectionHandler -> set_transport_manager( transportManager );

    transportManager->addDeviceListener(connectionHandler);

    NsAppManager::AppMgr& appMgr = NsAppManager::AppMgr::getInstance();

    //jsonHandler.setRPCMessagesObserver(&appMgr);

    connectionHandler -> set_connection_handler_observer(&appMgr);

    //appMgr.setJsonHandler(&jsonHandler);

    NsMessageBroker::CMessageBroker *pMessageBroker = NsMessageBroker::CMessageBroker::getInstance();
    if (!pMessageBroker)
    {
        LOG4CXX_INFO(logger, " Wrong pMessageBroker pointer!");
        return EXIT_SUCCESS;
    }

    NsMessageBroker::TcpServer *pJSONRPC20Server = new NsMessageBroker::TcpServer(std::string("0.0.0.0"), 8087, pMessageBroker);
    if (!pJSONRPC20Server)
    {
        LOG4CXX_INFO(logger, " Wrong pJSONRPC20Server pointer!");
        return EXIT_SUCCESS;
    }
    pMessageBroker->startMessageBroker(pJSONRPC20Server);
    if(!networking::init())
    {
      LOG4CXX_INFO(logger, " Networking initialization failed!");
    }

    if(!pJSONRPC20Server->Bind())
    {
      LOG4CXX_FATAL(logger, "Bind failed!");
      exit(EXIT_FAILURE);
    } else
    {
      LOG4CXX_INFO(logger, "Bind successful!");
    }

    if(!pJSONRPC20Server->Listen())
    {
      LOG4CXX_FATAL(logger, "Listen failed!");
      exit(EXIT_FAILURE);
    } else
    {
      LOG4CXX_INFO(logger, " Listen successful!");
    }

    profile::Profile::instance()->config_file_name("smartDeviceLink.ini");

    JSONRPC2Handler jsonRPC2Handler(profile::Profile::instance()->server_address(), profile::Profile::instance()->server_port());
    jsonRPC2Handler.setRPC2CommandsObserver( &appMgr );
    appMgr.setJsonRPC2Handler( &jsonRPC2Handler );
    if (!jsonRPC2Handler.Connect())
    {
        LOG4CXX_INFO(logger, "Cannot connect to remote peer!");
    }

    LOG4CXX_INFO(logger, "Start CMessageBroker thread!");
    System::Thread th1(new System::ThreadArgImpl<NsMessageBroker::CMessageBroker>(*pMessageBroker, &NsMessageBroker::CMessageBroker::MethodForThread, NULL));
    th1.Start(false);

    LOG4CXX_INFO(logger, "Start MessageBroker TCP server thread!");
    System::Thread th2(new System::ThreadArgImpl<NsMessageBroker::TcpServer>(*pJSONRPC20Server, &NsMessageBroker::TcpServer::MethodForThread, NULL));
    th2.Start(false);

    LOG4CXX_INFO(logger, "StartAppMgr JSONRPC 2.0 controller receiver thread!");
    System::Thread th3(new System::ThreadArgImpl<JSONRPC2Handler>(jsonRPC2Handler, &JSONRPC2Handler::MethodForReceiverThread, NULL));
    th3.Start(false);

    jsonRPC2Handler.registerController();
    jsonRPC2Handler.subscribeToNotifications();

    appMgr.setConnectionHandler(connectionHandler);

    LOG4CXX_INFO(logger, "Start AppMgr threads!");
    appMgr.executeThreads();

    /**********************************/
    /*********** Start HMI ************/
    struct stat sb;
    if (stat("hmi_link", &sb) == -1)
    {
        LOG4CXX_INFO(logger, "File with HMI link doesn't exist!");
    } else
    {
        std::ifstream file_str;
        file_str.open ("hmi_link");

        if (!file_str.is_open())
        {
            LOG4CXX_INFO(logger, "File with HMI link was not opened!");
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
            LOG4CXX_INFO(logger, "Input string:" << hmi_link << " length = " << hmi_link.size());
            file_str.close();
            if (stat(hmi_link.c_str(), &sb) == -1)
            {
                LOG4CXX_INFO(logger, "HMI index.html doesn't exist!");
            } else
            {
                pid_hmi = fork(); /* Create a child process */

                switch (pid_hmi)
                {
                    case -1: /* Error */
                    {
                        LOG4CXX_INFO(logger, "fork() failed!");
                        break;
                    }
                    case 0: /* Child process */
                    {
                        int fd_dev0 = open("/dev/null", O_RDWR, S_IWRITE);
                        if (0 > fd_dev0)
                        {
                            LOG4CXX_WARN(logger, "Open dev0 failed!");
                        } else
                        {
                            //close input/output file descriptors.
                            close(STDIN_FILENO);
                            close(STDOUT_FILENO);
                            close(STDERR_FILENO);
                            // move input/output to /dev/null
                            dup2(fd_dev0, STDIN_FILENO);
                            dup2(fd_dev0, STDOUT_FILENO);
                            dup2(fd_dev0, STDERR_FILENO);
                        }
                        execlp("/usr/bin/chromium-browser",
                              "chromium-browser",
                              "--auth-schemes=basic,digest,ntlm",
                              hmi_link.c_str(),
                              (char *) 0); /* Execute the program */
                        LOG4CXX_WARN(logger, "execl() failed! Install chromium-browser!");
                        return EXIT_SUCCESS;
                    }
                    default: /* Parent process */
                    {
                        LOG4CXX_INFO(logger, "Process created with pid " << pid_hmi);
                    }
                }
            }
        }
    }
    /**********************************/

    while(true)
    {
        sleep(100500);
    }
}

