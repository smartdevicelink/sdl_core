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
   * \brief Callback function which is called upon a new message from mobile side arrival
   * \param command RPC2Bus Json message
   */
  void AppLinkCoreClientRPC::messageReceivedFromDeviceCallback(NsRPC2Communication::RPC2Command *msg)
  {
      if(!msg)
      {
          LOG4CPLUS_ERROR_EXT(mLogger, "null-command!" );
          return;
      }

      ResourceContainer& rc = ResourceContainer::getInstance();
      switch(msg->getMethod())
      {
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPREGISTERED:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " An OnAppRegistered notification has been income");
              NsRPC2Communication::AppLinkCore::OnAppRegistered * object = (NsRPC2Communication::AppLinkCore::OnAppRegistered*)msg;
              Application* app = new Application(object->get_appName());
              app->setDeviceName(object->get_deviceName());
              app->setIcon(object->get_appIcon());
              app->setIsMedia(object->get_isMediaApplication());
              app->setLanguageDesired(object->get_languageDesired());
              if(object->get_vrSynonym())
              {
                  app->setVrSynonyms(*object->get_vrSynonym());
              }
              rc.addApplication(app->getName(), app);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPUNREGISTERED:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " An OnAppUnregistered notification has been income");
              NsRPC2Communication::AppLinkCore::OnAppUnregistered * object = (NsRPC2Communication::AppLinkCore::OnAppUnregistered*)msg;
              rc.removeApplication(object->get_appName());
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPPRESPONSE:
          {
              LOG4CPLUS_INFO_EXT(mLogger, "ActivateApp response has been received!");
              NsRPC2Communication::AppLinkCore::ActivateAppResponse* object = static_cast<NsRPC2Communication::AppLinkCore::ActivateAppResponse*>(msg);
              if ( !object )
              {
                  LOG4CPLUS_ERROR_EXT(mLogger, "Couldn't cast object to ActivateApp type");
                  return;
              }
              LOG4CPLUS_INFO_EXT(mLogger, "Result " << object->getResult());
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATARESPONSE:
          {
              LOG4CPLUS_INFO_EXT(mLogger, "SendData response has been received!");
              NsRPC2Communication::AppLinkCore::SendDataResponse* object = static_cast<NsRPC2Communication::AppLinkCore::SendDataResponse*>(msg);
              LOG4CPLUS_INFO_EXT(mLogger, "Result " << object->getResult());
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLISTRESPONSE:
          {
              LOG4CPLUS_INFO_EXT(mLogger, "GetAppList response has been received!");
              NsRPC2Communication::AppLinkCore::GetAppListResponse* object = static_cast<NsRPC2Communication::AppLinkCore::GetAppListResponse*>(msg);
              LOG4CPLUS_INFO_EXT(mLogger, "Found " << object->get_appList().size() << " applications!");
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_INVALID:
          default:
              LOG4CPLUS_ERROR_EXT(mLogger, " Unknown RPC message " << msg->getMethod() << " has been received!");
      }

  }

} /* namespace NsHMIEmulator */
