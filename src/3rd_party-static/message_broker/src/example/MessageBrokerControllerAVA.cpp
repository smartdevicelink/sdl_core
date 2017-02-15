/**
 * \file MessageBrokerControllerAVA.cpp
 * \brief MessageBroker Controller for Avatar.
 * \author AKara
 */

#include <ctime>
 
#include "MessageBrokerControllerAVA.hpp"

#include "MBDebugHelper.h" 

extern int start;

namespace NsMessageBroker
{
   CMessageBrokerControllerAVA::CMessageBrokerControllerAVA(const std::string& address, uint16_t port):
   CMessageBrokerController(address, port, "AVA")
   {
   }

   CMessageBrokerControllerAVA::~CMessageBrokerControllerAVA()
   {
   }

   void CMessageBrokerControllerAVA::processRequest(Json::Value& root)
   {
      DBG_MSG(("CMessageBrokerControllerAVA::processRequest()\n"));
      root=root;//to prevent compiler warning
   }

   void CMessageBrokerControllerAVA::processNotification(Json::Value& root)
   {
      DBG_MSG(("CMessageBrokerControllerAVA::processNotification()\n"));
      root=root;//to prevent compiler warning
   }

   void CMessageBrokerControllerAVA::processResponse(std::string method, Json::Value& root)
   {
      DBG_MSG(("CMessageBrokerControllerAVA::processResponse()\n"));
      DWORD stop = GetTickCount();
      int diff = stop - start;
      std::string id = root["id"].asString();
      printf("Execution time for id %s is %d ms!\n", id.c_str(), diff);
      root=root;//to prevent compiler warning
      method=method;//to prevent compiler warning
   }

   void CMessageBrokerControllerAVA::makeCall(std::string phoneNumber)
   {
      DBG_MSG(("CMessageBrokerControllerAVA::makeCall()\n"));
      Json::Value request, params;
      prepareMessage(request);
      request["method"] = "Phone.makeCall";
      params["phoneNumber"] = phoneNumber;
      request["params"] = params;
      sendJsonMessage(request);
   }
} /* namespace NsMessageBroker */
