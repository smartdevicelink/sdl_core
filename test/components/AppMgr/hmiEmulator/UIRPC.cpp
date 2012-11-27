/**
 * \file UIRPC.cpp
 * \brief MessageBroker Controller for UIRPC.
 * \author AKara
 */

#include "UIRPC.hpp"

#include "MBDebugHelper.h" 

namespace NsHMIEmulator
{
  UIRPC::UIRPC(const std::string& address, unsigned short port):
CMessageBrokerController(address, port, std::string("UI"))
  {
  }

  UIRPC::~UIRPC()
  {
  }

  void UIRPC::processRequest(Json::Value& root)
  {
    DBG_MSG(("UIRPC::processRequest()\n"));
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

  void UIRPC::processNotification(Json::Value& root)
  {
    DBG_MSG(("UIRPC::processNotification()\n"));
    root=root;//to prevent compiler warning
  }

  void UIRPC::processResponse(std::string method, Json::Value& root)
  {
    DBG_MSG(("UIRPC::processResponse()\n"));
    root=root;//to prevent compiler warning
    method=method;//to prevent compiler warning
  }

} /* namespace NsHMIEmulator */
