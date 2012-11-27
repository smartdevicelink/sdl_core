/**
 * \file TTSRPC.cpp
 * \brief MessageBroker Controller for TTSRPC.
 * \author AKara
 */

#include "TTSRPC.hpp"

#include "MBDebugHelper.h" 

#include "JSONHandler/RPC2Objects/NsRPC2Communication/TTS/GetCapabilities.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/TTS/GetCapabilitiesResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/TTS/RPC2.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/TTS/Speak.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/TTS/SpeakResponse.h"

namespace NsHMIEmulator
{
  TTSRPC::TTSRPC(const std::string& address, unsigned short port):
RPC(address, port, std::string("TTS"))
  {
  }

  TTSRPC::~TTSRPC()
  {
  }

  void TTSRPC::processRequest(Json::Value& root)
  {
      RPC::processRequest(root);
 /*   DBG_MSG(("TTSRPC::processRequest()\n"));
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

  void TTSRPC::processNotification(Json::Value& root)
  {
      RPC::processNotification(root);
/*    DBG_MSG(("TTSRPC::processNotification()\n"));
    root=root;//to prevent compiler warning*/
  }

  void TTSRPC::processResponse(std::string method, Json::Value& root)
  {
      RPC::processResponse(method, root);
 /*   DBG_MSG(("TTSRPC::processResponse()\n"));
    root=root;//to prevent compiler warning
    method=method;//to prevent compiler warning*/
  }

  /**
   * \brief Callback function which is called by JSONRPC2Handler
   *  when new RPC2Bus Json message is received from HMI.
   * \param command RPC2Bus Json message
   */
  void TTSRPC::onCommandReceivedCallback(NsRPC2Communication::RPC2Command *command)
  {
  }

} /* namespace NsHMIEmulator */
