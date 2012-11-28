/**
 * \file ButtonsRPC.cpp
 * \brief MessageBroker Controller for ButtonsRPC.
 * \author AKara
 */

#include "ButtonsRPC.hpp"

#include "MBDebugHelper.h" 

#include "JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/GetCapabilities.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/GetCapabilitiesResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonEvent.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonPress.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/RPC2.h"

namespace NsHMIEmulator
{
  ButtonsRPC::ButtonsRPC(const std::string& address, unsigned short port):
RPC(address, port, std::string("Buttons"))
  {
  }

  ButtonsRPC::~ButtonsRPC()
  {
  }

  void ButtonsRPC::processRequest(Json::Value& root)
  {
      RPC::processRequest(root);
 /*   DBG_MSG(("ButtonsRPC::processRequest()\n"));
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

  void ButtonsRPC::processNotification(Json::Value& root)
  {
      RPC::processNotification(root);
/*    DBG_MSG(("ButtonsRPC::processNotification()\n"));
    root=root;//to prevent compiler warning*/
  }

  void ButtonsRPC::processResponse(std::string method, Json::Value& root)
  {
      RPC::processResponse(method, root);
/*    DBG_MSG(("ButtonsRPC::processResponse()\n"));
    root=root;//to prevent compiler warning
    method=method;//to prevent compiler warning*/
  }

  /**
   * \brief Callback function which is called upon a new message from mobile side arrival
   * \param command RPC2Bus Json message
   */
  void ButtonsRPC::messageReceivedFromDeviceCallback(NsRPC2Communication::RPC2Command *msg)
  {
      if(!msg)
      {
          LOG4CPLUS_ERROR_EXT(mLogger, "null-command!" );
          return;
      }

      ResourceContainer& rc = ResourceContainer::getInstance();
      switch(msg->getMethod())
      {
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIES:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A GetButtonCapabilities request has been income");
              NsRPC2Communication::Buttons::GetCapabilities * btnCaps = (NsRPC2Communication::Buttons::GetCapabilities*)msg;
              NsRPC2Communication::Buttons::GetCapabilitiesResponse * response = new NsRPC2Communication::Buttons::GetCapabilitiesResponse;
              response->set_capabilities(rc.getButtonCapabilities());
              response->setId(btnCaps->getId());
              sendRPC2MessageToMobileSide(response);

              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_INVALID:
          default:
              LOG4CPLUS_ERROR_EXT(mLogger, " Not Buttons RPC message " << msg->getMethod() << " has been received!");
      }

  }

} /* namespace NsHMIEmulator */
