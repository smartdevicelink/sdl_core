/**
 * \file VRRPC.cpp
 * \brief MessageBroker Controller for VRRPC.
 * \author AKara
 */

#include "VRRPC.hpp"

#include "MBDebugHelper.h" 

#include "JSONHandler/RPC2Objects/NsRPC2Communication/VR/AddCommand.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/VR/AddCommandResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/VR/DeleteCommand.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/VR/DeleteCommandResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/VR/GetCapabilities.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/VR/GetCapabilitiesResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/VR/OnCommand.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/VR/RPC2.h"

namespace NsHMIEmulator
{
  VRRPC::VRRPC(const std::string& address, unsigned short port):
RPC(address, port, std::string("VR"))
  {
  }

  VRRPC::~VRRPC()
  {
  }

  void VRRPC::processRequest(Json::Value& root)
  {
      RPC::processRequest(root);
/*    DBG_MSG(("VRRPC::processRequest()\n"));
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
    }*/
  }

  void VRRPC::processNotification(Json::Value& root)
  {
      RPC::processNotification(root);
/*    DBG_MSG(("VRRPC::processNotification()\n"));
    root=root;//to prevent compiler warning*/
  }

  void VRRPC::processResponse(std::string method, Json::Value& root)
  {
      RPC::processResponse(method, root);
/*    DBG_MSG(("VRRPC::processResponse()\n"));
    root=root;//to prevent compiler warning
    method=method;//to prevent compiler warning*/
  }

  /**
   * \brief Callback function which is called upon a new message from mobile side arrival
   * \param command RPC2Bus Json message
   */
  void VRRPC::messageReceivedFromDeviceCallback(NsRPC2Communication::RPC2Command *command)
  {
      if(!command)
      {
          LOG4CPLUS_ERROR_EXT(mLogger, "null-command!" );
          return;
      }
  }

} /* namespace NsHMIEmulator */
