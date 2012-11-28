/**
 * \file AppLinkCoreClientRPC.cpp
 * \brief MessageBroker Controller for AppLinkCoreClientRPC.
 * \author AKara
 */

#include "AppLinkCoreClientRPC.hpp"

#include "MBDebugHelper.h" 

#include "JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/ActivateApp.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/ActivateAppResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/GetAppList.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/GetAppListResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppRegistered.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppUnregistered.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/RPC2.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/SendData.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/SendDataResponse.h"

namespace NsHMIEmulator
{
  AppLinkCoreClientRPC::AppLinkCoreClientRPC(const std::string& address, unsigned short port):
RPC(address, port, std::string("AppLinkCoreClient"))
  {
  }

  AppLinkCoreClientRPC::~AppLinkCoreClientRPC()
  {
  }

  void AppLinkCoreClientRPC::processRequest(Json::Value& root)
  {
      RPC::processRequest(root);
 /*   DBG_MSG(("AppLinkCoreClientRPC::processRequest()\n"));
    if (getControllersName() == getDestinationComponentName(root))
    {
        NsRPC2Communication::RPC2Command * currentCommand = NsRPC2Communication::Marshaller::fromJSON( root );
        if ( !currentCommand )
        {
            LOG4CPLUS_ERROR_EXT( mLogger, "Invalid RPCBus message received." );
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "handle command" );



/*      Json::Value response;
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
      sendJsonMessage(response); * /
    } else
    {
      DBG_MSG_ERROR(("Wrong message destination!\n"));
    } */
  }

  void AppLinkCoreClientRPC::processNotification(Json::Value& root)
  {
      RPC::processNotification(root);
/*    DBG_MSG(("AppLinkCoreClientRPC::processNotification()\n"));
    root=root;//to prevent compiler warning
    Json::FastWriter m_recieverWriter;
    std::string receivedNotification = m_recieverWriter.write(root);
    //receivedNotification cotains pure JSON string
    NsRPC2Communication::RPC2Command * currentCommand = NsRPC2Communication::Marshaller::fromJSON( root ); */
  }

  void AppLinkCoreClientRPC::processResponse(std::string method, Json::Value& root)
  {
      RPC::processResponse(method, root);
   /* DBG_MSG(("AppLinkCoreClientRPC::processResponse()\n"));
    ResponseContainer response;
    response.methodName = method;
    response.response = root;
    NsRPC2Communication::RPC2Command * currentCommand = NsRPC2Communication::Marshaller::fromJSON( response.response );
    root=root;//to prevent compiler warning
    method=method;//to prevent compiler warning*/
  }

  /**
   * \brief Callback function which is called by JSONRPC2Handler
   *  when new RPC2Bus Json message is received from HMI.
   * \param command RPC2Bus Json message
   */
  void AppLinkCoreClientRPC::onCommandReceivedCallback(NsRPC2Communication::RPC2Command *command)
  {
  }

} /* namespace NsHMIEmulator */
