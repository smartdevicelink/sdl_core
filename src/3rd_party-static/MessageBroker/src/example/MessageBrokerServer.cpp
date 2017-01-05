/**
 * \file MessageBrokerServer.cpp
 * \brief MessageBrokerServer sources
 * \author AKara
 */

#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <iostream>
#include <ctime>

#include "system.h"

#include "MBDebugHelper.h"

#include "mb_tcpserver.hpp"
#include "mb_tcpclient.hpp"
#include "CMessageBroker.hpp"

#include "MessageBrokerControllerAVA.hpp"
#include "MessageBrokerControllerPhone.hpp"
#include "MessageBrokerControllerBackend.hpp"

/**
 * \brief Signal management.
 * \param code signal code
 */
 static void signal_handler(int code)
 {
   switch(code)
   {
      case SIGINT:
      case SIGTERM:
      break;
      default:
      break;
   }
}

/**
 * \brief stores start time of test operation.
 */
int start;

/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char** argv)
{
   NsMessageBroker::CMessageBroker* mpMessageBroker = NsMessageBroker::CMessageBroker::getInstance();
   if (!mpMessageBroker)
   {
      DBG_MSG_ERROR(("NULL pointer\n"));
      exit(EXIT_FAILURE);
   }

   
   NsMessageBroker::TcpServer server(std::string("127.0.0.1"), 8086, mpMessageBroker);
   
   DBG_MSG(("Start MessageBroker component\n"));
   mpMessageBroker->startMessageBroker(&server);

   NsMessageBroker::CMessageBrokerControllerAVA tcpControllerAVA(std::string("127.0.0.1"), 8086);
   NsMessageBroker::CMessageBrokerControllerPhone tcpControllerPhone(std::string("127.0.0.1"), 8086);
   NsMessageBroker::CMessageBrokerControllerBackend tcpControllerBackend(std::string("127.0.0.1"), 8086);
  
   /* avoid compilation warnings */
   argc = argc;
   argv = argv;

   if(!networking::init())
   {
      DBG_MSG_ERROR(("Networking initialization failed!\n"));
      
   }

   if(signal(SIGTERM, signal_handler) == SIG_ERR)
   {
      DBG_MSG_ERROR(("Error signal SIGTERM will not be handled!\n"));
   }

   if(signal(SIGINT, signal_handler) == SIG_ERR)
   {
      DBG_MSG_ERROR(("Error signal SIGINT will not be handled!\n"));
   }

   if(!server.Bind())
   {
      DBG_MSG_ERROR(("Bind failed!\n"));
      exit(EXIT_FAILURE);
   } else
   {
      DBG_MSG(("Bind successful!\n"));
   }

   if(!server.Listen())
   {
      DBG_MSG_ERROR(("Listen failed!\n"));
      exit(EXIT_FAILURE);
   } else
   {
      DBG_MSG(("Listen successful!\n"));
   }

   if(!tcpControllerAVA.Connect())
   {
      DBG_MSG_ERROR(("Cannot connect to remote peer!\n"));
      exit(EXIT_FAILURE);
   } else
   {
      DBG_MSG(("ClientAVA connected to the server! SocketID = %d\n", tcpControllerAVA.GetSocket()));
   }

   if(!tcpControllerPhone.Connect())
   {
      DBG_MSG_ERROR(("Cannot connect to remote peer!\n"));
      exit(EXIT_FAILURE);
   } else
   {
      DBG_MSG(("ClientPhone connected to the server! SocketID = %d\n",tcpControllerPhone.GetSocket()));
   }

   if(!tcpControllerBackend.Connect())
   {
      DBG_MSG_ERROR(("Cannot connect to remote peer!\n"));
      exit(EXIT_FAILURE);
   } else
   {
      DBG_MSG(("ClientBackend connected to the server! SocketID = %d\n",tcpControllerBackend.GetSocket()));
   }

   DBG_MSG(("Start CMessageBroker thread!\n"));
   System::Thread th1(new System::ThreadArgImpl<NsMessageBroker::CMessageBroker>(*mpMessageBroker, &NsMessageBroker::CMessageBroker::MethodForThread, NULL));
   th1.Start(false);

   DBG_MSG(("Start MessageBroker TCP server thread!\n"));
   System::Thread th2(new System::ThreadArgImpl<NsMessageBroker::TcpServer>(server, &NsMessageBroker::TcpServer::MethodForThread, NULL));
   th2.Start(false);

   DBG_MSG(("Start tcpControllerAVA receiver thread!\n"));
   System::Thread th3(new System::ThreadArgImpl<NsMessageBroker::CMessageBrokerControllerAVA>(tcpControllerAVA, &NsMessageBroker::CMessageBrokerControllerAVA::MethodForReceiverThread, NULL));
   th3.Start(false);

   DBG_MSG(("Start tcpControllerPhone receiver thread!\n"));
   System::Thread th4(new System::ThreadArgImpl<NsMessageBroker::CMessageBrokerControllerPhone>(tcpControllerPhone, &NsMessageBroker::CMessageBrokerControllerPhone::MethodForReceiverThread, NULL));
   th4.Start(false);

   DBG_MSG(("Start tcpControllerBackend receiver thread!\n"));
   System::Thread th5(new System::ThreadArgImpl<NsMessageBroker::CMessageBrokerControllerBackend>(tcpControllerBackend, &NsMessageBroker::CMessageBrokerControllerBackend::MethodForReceiverThread, NULL));
   th5.Start(false);

   bool loop = true;
   while(loop)
   {
      DBG_MSG(("Enter command code:\n"));
      int i;
      std::cin >> i;
      switch(i)
      {
         case 0:
         {
            DBG_MSG(("Exit!\n"));
            mpMessageBroker->stopMessageBroker();
            th1.Stop();
            th2.Stop();
            th3.Stop();
            th4.Stop();
            server.Close();
            loop = false;
            break;
         }
         case 1:// register component
         {
            DBG_MSG(("tcpControllerAVA.register()\n"));
            tcpControllerAVA.registerController(0);
            DBG_MSG(("tcpControllerPhone.register()\n"));
            tcpControllerPhone.registerController(1);
            DBG_MSG(("tcpControllerBackend.register()\n"));
            tcpControllerBackend.registerController(2);
            break;
         }
         case 2:// external message
         {
            DBG_MSG(("tcpControllerAVA.Phone.Call()\n"));
            start = GetTickCount();
            tcpControllerAVA.makeCall("+380677639550");
            break;
         }
         case 3://subscribe
         {
            DBG_MSG(("tcpControllerAVA.subscribeTo()\n"));
            tcpControllerAVA.subscribeTo("Phone.onContactsUpdated");
            break;
         }
         case 4://notify
         {
            DBG_MSG(("tcpControllerPhone.onContactsUpdated()\n"));
            tcpControllerPhone.onContactsUpdated();
            break;
         }
         case 5: //unsubscribe
         {
            DBG_MSG(("tcpControllerAVA.unsubscribeFrom()\n"));
            tcpControllerAVA.unsubscribeFrom("Phone.onContactsUpdated");
            break;
         }
         case 6: //unregister
         {
            DBG_MSG(("tcpControllerPhone.unregister()\n"));
            tcpControllerPhone.unregisterController();
            break;
         }
         case 7: //stress test
         {
            
            DBG_MSG(("tcpControllerAVA.Phone.Call() 500 times\n"));
            start = GetTickCount();
            for (int c =0; c<1000; c++)
            {
               tcpControllerAVA.makeCall("+380677639550");
            }
            int stop = GetTickCount();
            int diff = stop - start;
            printf("Requests execution time is %d ms!\n", diff);
            break;
         }
         case 8: //parser test
         {
            DBG_MSG(("Parser test\n"));
            mpMessageBroker->Test();
            break;
         }
         default:
         {
            DBG_MSG(("Entered: %d\n", i));
            break;
         }
      }
   }
   return EXIT_SUCCESS;
} 