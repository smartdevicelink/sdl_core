/**
 * \file UIRPC.cpp
 * \brief MessageBroker Controller for UIRPC.
 * \author AKara
 */

#include "UIRPC.hpp"

#include "MBDebugHelper.h" 

#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddCommand.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddCommandResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddSubMenu.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddSubMenuResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/Alert.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/AlertResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/CreateInteractionChoiceSet.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/CreateInteractionChoiceSetResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteCommand.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteCommandResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteInteractionChoiceSet.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteInteractionChoiceSetResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteSubMenu.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteSubMenuResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetCapabilities.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetCapabilitiesResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnAppActivated.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnAppDeactivated.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnCommand.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnDriverDistraction.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnReady.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnSystemContext.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformInteraction.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformInteractionResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/ResetGlobalProperties.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/ResetGlobalPropertiesResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/RPC2.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetGlobalProperties.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetGlobalPropertiesResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetMediaClockTimer.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetMediaClockTimerResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/Show.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/ShowResponse.h"

namespace NsHMIEmulator
{
  UIRPC::UIRPC(const std::string& address, unsigned short port):
RPC(address, port, std::string("UI"))
  {
  }

  UIRPC::~UIRPC()
  {
  }

  void UIRPC::processRequest(Json::Value& root)
  {
      RPC::processRequest(root);
 /*   DBG_MSG(("UIRPC::processRequest()\n"));
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

  void UIRPC::processNotification(Json::Value& root)
  {
      RPC::processNotification(root);
/*    DBG_MSG(("UIRPC::processNotification()\n"));
    root=root;//to prevent compiler warning*/
  }

  void UIRPC::processResponse(std::string method, Json::Value& root)
  {
      RPC::processResponse(method, root);
/*    DBG_MSG(("UIRPC::processResponse()\n"));
    root=root;//to prevent compiler warning
    method=method;//to prevent compiler warning*/
  }

  /**
   * \brief Callback function which is called by JSONRPC2Handler
   *  when new RPC2Bus Json message is received from HMI.
   * \param command RPC2Bus Json message
   */
  void UIRPC::onCommandReceivedCallback(NsRPC2Communication::RPC2Command *command)
  {
  }

} /* namespace NsHMIEmulator */
