/**
 * \file AppLinkCoreClientRPC.cpp
 * \brief MessageBroker Controller for AppLinkCoreClientRPC.
 * \author AKara
 */

#include "AppLinkCoreClientRPC.hpp"

#include "MBDebugHelper.h" 

namespace NsHMIEmulator
{
  AppLinkCoreClientRPC::AppLinkCoreClientRPC(const std::string& address, unsigned short port):
CMessageBrokerController(address, port, std::string("AppLinkCoreClient"))
  {
  }

  AppLinkCoreClientRPC::~AppLinkCoreClientRPC()
  {
  }

  void AppLinkCoreClientRPC::processRequest(Json::Value& root)
  {
    DBG_MSG(("AppLinkCoreClientRPC::processRequest()\n"));
    if (getControllersName() == getDestinationComponentName(root))
    {
      Json::Value response;
      response["jsonrpc"] = root["jsonrpc"];
      response["id"] = root["id"];
      if ("Method1" == getMethodName(root))
      {
        //TODO: Insert implementation for each method.
      } else if ("Method2 " == getMethodName(root))
      {
        //TODO: Insert implementation for each method.
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

  void AppLinkCoreClientRPC::processNotification(Json::Value& root)
  {
    DBG_MSG(("AppLinkCoreClientRPC::processNotification()\n"));
    root=root;//to prevent compiler warning
    Json::FastWriter m_recieverWriter;
    std::string receivedNotification = m_recieverWriter.write(root);
    //receivedNotification cotains pure JSON string
  }

  void AppLinkCoreClientRPC::processResponse(std::string method, Json::Value& root)
  {
    DBG_MSG(("AppLinkCoreClientRPC::processResponse()\n"));
    root=root;//to prevent compiler warning
    method=method;//to prevent compiler warning
  }

} /* namespace NsHMIEmulator */
