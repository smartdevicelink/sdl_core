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
   * \brief Callback function which is called upon a new message from mobile side arrival
   * \param command RPC2Bus Json message
   */
  void TTSRPC::messageReceivedFromDeviceCallback(NsRPC2Communication::RPC2Command *msg)
  {
      if(!msg)
      {
          LOG4CPLUS_ERROR_EXT(mLogger, "null-command!" );
          return;
      }

      ResourceContainer& rc = ResourceContainer::getInstance();
      switch(msg->getMethod())
      {
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIES:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A GetTTSCapabilities request has been income");
              NsRPC2Communication::TTS::GetCapabilities * ttsCaps = (NsRPC2Communication::TTS::GetCapabilities*)msg;
              NsRPC2Communication::TTS::GetCapabilitiesResponse * response = new NsRPC2Communication::TTS::GetCapabilitiesResponse;
              response->set_capabilities(rc.getTtsCapabilities());
              response->setId(ttsCaps->getId());
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__SPEAK:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A Speak request has been income");
              NsRPC2Communication::TTS::Speak* object = (NsRPC2Communication::TTS::Speak*)msg;
              LOG4CPLUS_INFO_EXT(mLogger, " Speaking: ");

              const std::vector<NsAppLinkRPC::TTSChunk>& tts = object->get_ttsChunks();
              for(std::vector<NsAppLinkRPC::TTSChunk>::const_iterator it = tts.begin(); it != tts.end(); it++)
              {
                  const NsAppLinkRPC::TTSChunk& chunk = *it;
                  LOG4CPLUS_INFO_EXT(mLogger, chunk.get_text());
              }
              LOG4CPLUS_INFO_EXT(mLogger, " Speech finished!");

              NsRPC2Communication::TTS::SpeakResponse* response = new NsRPC2Communication::TTS::SpeakResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_INVALID:
          default:
              LOG4CPLUS_ERROR_EXT(mLogger, " Not TTS RPC message " << msg->getMethod() << " has been received!");
      }

  }

} /* namespace NsHMIEmulator */
