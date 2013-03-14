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
  void VRRPC::messageReceivedFromDeviceCallback(NsRPC2Communication::RPC2Command *msg)
  {
      if(!msg)
      {
          LOG4CPLUS_ERROR_EXT(mLogger, "null-command!" );
          return;
      }

      ResourceContainer& rc = ResourceContainer::getInstance();
      switch(msg->getMethod())
      {
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIES:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A GetVRCapabilities request has been income");
              NsRPC2Communication::VR::GetCapabilities * vrCaps = (NsRPC2Communication::VR::GetCapabilities*)msg;
              NsRPC2Communication::VR::GetCapabilitiesResponse * response = new NsRPC2Communication::VR::GetCapabilitiesResponse;
              response->set_capabilities(rc.getVrCapabilities());
              response->setId(vrCaps->getId());
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMAND:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand VR request has been income");
              NsRPC2Communication::VR::AddCommand* object = (NsRPC2Communication::VR::AddCommand*)msg;
              rc.addVrCommand( object->get_cmdId(), object->get_vrCommands() );
              NsRPC2Communication::VR::AddCommandResponse* response = new NsRPC2Communication::VR::AddCommandResponse;
              response->setId(object->getId());
              response->setResult(NsSmartDeviceLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMANDRESPONSE:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand VR request has been income");
              NsRPC2Communication::VR::DeleteCommand* object = (NsRPC2Communication::VR::DeleteCommand*)msg;
              rc.removeVrCommand(object->get_cmdId());
              NsRPC2Communication::VR::DeleteCommandResponse* response = new NsRPC2Communication::VR::DeleteCommandResponse;
              response->setId(object->getId());
              response->setResult(NsSmartDeviceLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_INVALID:
          default:
              LOG4CPLUS_ERROR_EXT(mLogger, " Not VR RPC message " << msg->getMethod() << " has been received!");
      }

  }

} /* namespace NsHMIEmulator */
