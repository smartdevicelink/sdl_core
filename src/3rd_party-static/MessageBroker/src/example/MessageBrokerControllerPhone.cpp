/**
 * \file MessageBrokerControllerPhone.cpp
 * \brief MessageBroker Controller for Phone.
 * \author AKara
 */

#include "MessageBrokerControllerPhone.hpp"

#include "CMessageBroker.hpp"

#include "MBDebugHelper.h" 

namespace NsMessageBroker
{
   CMessageBrokerControllerPhone::CMessageBrokerControllerPhone(const std::string& address, uint16_t port):
   CMessageBrokerController(address, port, "Phone")
   {
   }

   CMessageBrokerControllerPhone::~CMessageBrokerControllerPhone()
   {
   }

   void CMessageBrokerControllerPhone::processRequest(Json::Value& root)
   {
      DBG_MSG(("CMessageBrokerControllerPhone::processRequest()\n"));
      if (getControllersName() == getDestinationComponentName(root))
      {
         Json::Value response;
         response["jsonrpc"] = root["jsonrpc"];
         response["id"] = root["id"];
         if ("makeCall" == getMethodName(root))
         {
            if (root.isMember("params"))
            {
               Json::Value params = root["params"];
               if (params.isMember("phoneNumber") && params["phoneNumber"].isString())
               {
                  makeCall(params["phoneNumber"].asString(), response);
               } else
               {
                  DBG_MSG_ERROR(("Wrong params!\n"));
                  prepareErrorMessage(NsMessageBroker::INVALID_PARAMS, "Wrong params!", response);
               }
            } else
            {
               DBG_MSG_ERROR(("Not possible to parse phone number!\n"));
               prepareErrorMessage(NsMessageBroker::INVALID_PARAMS, "Not possible to parse phone number!", response);
            }
         } else if ("endCall" == getMethodName(root))
         {
            endCall(root["params"].asString(), response);
         } else if ("holdCall" == getMethodName(root))
         {
            holdCall(root["params"].asString(), response);
         } else if ("getContacts" == getMethodName(root))
         {
            if (root.isMember("params"))
            {
               Json::Value params = root["params"];
               if (params.isMember("firstLetter") && params["firstLetter"].isString()
                && params.isMember("offset") && params["offset"].isInt()
                && params.isMember("numberOfItems") && params["numberOfItems"].isInt())
               {
                  getContacts(params["firstLetter"].asString(), params["offset"].asInt(), params["numberOfItems"].asInt(), response);
               } else
               {
                  DBG_MSG_ERROR(("Wrong params of getContacts()!\n"));
                  prepareErrorMessage(NsMessageBroker::INVALID_PARAMS, "Wrong params of getContacts()!", response);
               }
            } else
            {
               DBG_MSG_ERROR(("Params is not an object!\n"));
               prepareErrorMessage(NsMessageBroker::INVALID_PARAMS, "Params is not an object!", response);
            }
         } else if ("getHistory" == getMethodName(root))
         {
            if (root.isMember("params"))
            {
               Json::Value params = root["params"];
               if (params.isMember("typeOfContacts") && params["typeOfContacts"].isString()
                && params.isMember("offset") && params["offset"].isInt()
                && params.isMember("numberOfItems") && params["numberOfItems"].isInt())
               {
                  getHistory(params["typeOfContacts"].asString(), params["offset"].asInt(), params["numberOfItems"].asInt(), response);
               } else
               {
                  DBG_MSG_ERROR(("Wrong params of getHistory()!\n"));
                  prepareErrorMessage(NsMessageBroker::INVALID_PARAMS, "Wrong params of getHistory()!", response);
               }
            } else
            {
               DBG_MSG_ERROR(("Params is not an object!\n"));
               prepareErrorMessage(NsMessageBroker::INVALID_PARAMS, "Params is not an object!", response);
            }
         } else
         {
            DBG_MSG_ERROR(("Method has not been found!\n"));
            Json::Value err;
            err["code"] = NsMessageBroker::INVALID_REQUEST;
            err["message"] = "Method has not been found.";
            response["error"] = err;
         }
         sendJsonMessage(response);
      } else
      {
         DBG_MSG_ERROR(("Wrong message destination!\n"));
      }
   }

   void CMessageBrokerControllerPhone::makeCall(std::string phoneNumber, Json::Value& response)
   {
      DBG_MSG(("CMessageBrokerControllerPhone::makeCall()\n"));
      phoneNumber = phoneNumber; // to avoid compiler's warning
      response["result"] = "OK";
   }

   void CMessageBrokerControllerPhone::endCall(std::string phoneNumber, Json::Value& response)
   {
      DBG_MSG(("CMessageBrokerControllerPhone::endCall()\n"));
      phoneNumber = phoneNumber; // to avoid compiler's warning
      response["result"] = "OK";
   }

   void CMessageBrokerControllerPhone::holdCall(std::string phoneNumber, Json::Value& response)
   {
      DBG_MSG(("CMessageBrokerControllerPhone::holdCall()\n"));
      phoneNumber = phoneNumber; // to avoid compiler's warning
      response["result"] = "OK";
   }

   void CMessageBrokerControllerPhone::getContacts(std::string firstLetter, int offset, int numberOfItems, Json::Value& response)
   {
      DBG_MSG(("CMessageBrokerControllerPhone::getContacts()\n"));
      firstLetter = firstLetter; // to avoid compiler's warning
      offset = offset; // to avoid compiler's warning
      numberOfItems = numberOfItems; // to avoid compiler's warning
      response["result"] = "OK";
   }

   void CMessageBrokerControllerPhone::getHistory(std::string firstLetter, int offset, int numberOfItems, Json::Value& response)
   {
      DBG_MSG(("CMessageBrokerControllerPhone::getHistory()\n"));
      firstLetter = firstLetter; // to avoid compiler's warning
      offset = offset; // to avoid compiler's warning
      numberOfItems = numberOfItems; // to avoid compiler's warning
      response["result"] = "OK";
   }

   void CMessageBrokerControllerPhone::onCallStatusChanged(int callStatus)
   {
      DBG_MSG(("CMessageBrokerControllerPhone::onCallStatusChanged()\n"));
      Json::Value request, params;
      request["jsonrpc"] = "2.0";
      request["method"] = "Phone.onCallStatusChanged";
      params["callStatus"] = callStatus;
      request["params"] = params;
      sendJsonMessage(request);
   }

   void CMessageBrokerControllerPhone::onContactsUpdated()
   {
      DBG_MSG(("CMessageBrokerControllerPhone::onContactsUpdated()\n"));
      Json::Value request;
      request["jsonrpc"] = "2.0";
      request["method"] = "Phone.onContactsUpdated";
      sendJsonMessage(request);
   }

   void CMessageBrokerControllerPhone::onHistoryUpdated()
   {
      DBG_MSG(("CMessageBrokerControllerPhone::onHistoryUpdated()\n"));
      Json::Value request;
      request["jsonrpc"] = "2.0";
      request["method"] = "Phone.onHistoryUpdated";
      sendJsonMessage(request);
   }

   void CMessageBrokerControllerPhone::processNotification(Json::Value& root)
   {
      DBG_MSG(("CMessageBrokerControllerPhone::processNotification()\n"));
      root=root;//to prevent compiler warning
   }

   void CMessageBrokerControllerPhone::processResponse(std::string method, Json::Value& root)
   {
      DBG_MSG(("CMessageBrokerControllerPhone::processResponse()\n"));
      root=root;//to prevent compiler warning
      method = method;
   }
} /* namespace NsMessageBroker */
