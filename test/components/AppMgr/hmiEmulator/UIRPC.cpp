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
   * \brief Callback function which is called upon a new message from mobile side arrival
   * \param command RPC2Bus Json message
   */
  void UIRPC::messageReceivedFromDeviceCallback(NsRPC2Communication::RPC2Command *msg)
  {
      if(!msg)
      {
          LOG4CPLUS_ERROR_EXT(mLogger, "null-command!" );
          return;
      }

      ResourceContainer& rc = ResourceContainer::getInstance();
      switch(msg->getMethod())
      {
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIES:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A GetUICapabilities request has been income");
              NsRPC2Communication::UI::GetCapabilities * uiCaps = (NsRPC2Communication::UI::GetCapabilities*)msg;
              NsRPC2Communication::UI::GetCapabilitiesResponse * response = new NsRPC2Communication::UI::GetCapabilitiesResponse;
              response->set_displayCapabilities(rc.getDisplayCapabilities());
              response->set_hmiZoneCapabilities(rc.getHmiZoneCapabilities());
              response->setId(uiCaps->getId());
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SHOW:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A Show request has been income");
              NsRPC2Communication::UI::Show* object = (NsRPC2Communication::UI::Show*)msg;
              LOG4CPLUS_INFO_EXT(mLogger, "Info to be shown: ");
              if(object->get_alignment())
              {
                  LOG4CPLUS_INFO_EXT(mLogger, "Alighment: " << object->get_alignment()->get());
              }
              if(object->get_mainField1())
              {
                  LOG4CPLUS_INFO_EXT(mLogger, "Main field 1: " << *object->get_mainField1());
              }
              if(object->get_mainField2())
              {
                  LOG4CPLUS_INFO_EXT(mLogger, "Main field 2: " << *object->get_mainField2());
              }
              if(object->get_mediaClock())
              {
                  LOG4CPLUS_INFO_EXT(mLogger, "Info to be shown: " << *object->get_mediaClock());
              }
              if(object->get_mediaTrack())
              {
                  LOG4CPLUS_INFO_EXT(mLogger, "Info to be shown: " << *object->get_mediaTrack());
              }
              if(object->get_statusBar())
              {
                  LOG4CPLUS_INFO_EXT(mLogger, "Info to be shown: " << *object->get_statusBar());
              }
              LOG4CPLUS_INFO_EXT(mLogger, "End of info");

              NsRPC2Communication::UI::ShowResponse* response = new NsRPC2Communication::UI::ShowResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIES:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A SetGlobalProperties request has been income");
              NsRPC2Communication::UI::SetGlobalProperties* object = (NsRPC2Communication::UI::SetGlobalProperties*)msg;
              if(object->get_helpPrompt())
              {
                  rc.setHelpPrompt( *object->get_helpPrompt() );
              }
              if(object->get_timeoutPrompt())
              {
                  rc.setTimeoutPrompt( *object->get_timeoutPrompt() );
              }
              NsRPC2Communication::UI::SetGlobalPropertiesResponse* response = new NsRPC2Communication::UI::SetGlobalPropertiesResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIES:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A ResetGlobalProperties request has been income");
              NsRPC2Communication::UI::ResetGlobalProperties* object = (NsRPC2Communication::UI::ResetGlobalProperties*)msg;
              rc.setGlobalProperties( object->get_properties() );
              NsRPC2Communication::UI::ResetGlobalPropertiesResponse* response = new NsRPC2Communication::UI::ResetGlobalPropertiesResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERT:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " An Alert request has been income");
              NsRPC2Communication::UI::Alert* object = (NsRPC2Communication::UI::Alert*)msg;
              LOG4CPLUS_INFO_EXT(mLogger, " Alerting: ");
              if(object->get_AlertText1())
              {
                  LOG4CPLUS_INFO_EXT(mLogger, " Text1: " << *object->get_AlertText1());
              }
              if(object->get_AlertText2())
              {
                  LOG4CPLUS_INFO_EXT(mLogger, " Text2: " << *object->get_AlertText2());
              }
              if(object->get_duration())
              {
                  LOG4CPLUS_INFO_EXT(mLogger, " Duration: " << *object->get_duration());
              }
              if(object->get_playTone())
              {
                  LOG4CPLUS_INFO_EXT(mLogger, " Play tone: " << *object->get_playTone());
              }
              LOG4CPLUS_INFO_EXT(mLogger, " End of alert ");
              NsRPC2Communication::UI::AlertResponse* response = new NsRPC2Communication::UI::AlertResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMAND:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand UI request has been income");
              NsRPC2Communication::UI::AddCommand* object = (NsRPC2Communication::UI::AddCommand*)msg;
              rc.addCommand( object->get_cmdId(), object->get_menuParams() );
              NsRPC2Communication::UI::AddCommandResponse* response = new NsRPC2Communication::UI::AddCommandResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMAND:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand UI request has been income");
              NsRPC2Communication::UI::DeleteCommand* object = (NsRPC2Communication::UI::DeleteCommand*)msg;
              rc.removeCommand(object->get_cmdId());
              NsRPC2Communication::UI::DeleteCommandResponse* response = new NsRPC2Communication::UI::DeleteCommandResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENU:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " An AddSubMenu request has been income");
              NsRPC2Communication::UI::AddSubMenu* object = (NsRPC2Communication::UI::AddSubMenu*)msg;
              rc.addMenuItem(object->get_menuId(), object->get_menuName(), object->get_position() ? *object->get_position() : -1 );
              NsRPC2Communication::UI::AddSubMenuResponse* response = new NsRPC2Communication::UI::AddSubMenuResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENU:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A DeleteSubMenu request has been income");
              NsRPC2Communication::UI::DeleteSubMenu* object = (NsRPC2Communication::UI::DeleteSubMenu*)msg;
              rc.removeMenuItem(object->get_menuId());
              NsRPC2Communication::UI::DeleteSubMenuResponse* response = new NsRPC2Communication::UI::DeleteSubMenuResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESET:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A CreateInteractionChoiceSet request has been income");
              NsRPC2Communication::UI::CreateInteractionChoiceSet* object = (NsRPC2Communication::UI::CreateInteractionChoiceSet*)msg;
              rc.addInteractionChoiceSet( object->get_interactionChoiceSetID(), object->get_choiceSet() );
              NsRPC2Communication::UI::CreateInteractionChoiceSetResponse* response = new NsRPC2Communication::UI::CreateInteractionChoiceSetResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESET:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A DeleteInteractionChoiceSet request has been income");
              NsRPC2Communication::UI::DeleteInteractionChoiceSet* object = (NsRPC2Communication::UI::DeleteInteractionChoiceSet*)msg;
              rc.removeInteractionChoiceSet(object->get_interactionChoiceSetID());
              NsRPC2Communication::UI::DeleteInteractionChoiceSetResponse* response = new NsRPC2Communication::UI::DeleteInteractionChoiceSetResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTION:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A PerformInteraction request has been income");
              NsRPC2Communication::UI::PerformInteraction* object = (NsRPC2Communication::UI::PerformInteraction*)msg;
              NsRPC2Communication::UI::PerformInteractionResponse* response = new NsRPC2Communication::UI::PerformInteractionResponse;
              response->setId(object->getId());
              const std::vector<unsigned int> choiceSetIDs = object->get_interactionChoiceSetIDList();
              for(std::vector<unsigned int>::const_iterator it = choiceSetIDs.begin(); it != choiceSetIDs.end(); it++)
              {
                  if(rc.findInteractionChoiceSet(*it).empty())
                  {
                      LOG4CPLUS_ERROR_EXT(mLogger, "An interaction choice set id " << *it << " has not been found in a list of registered interaction choice sets!");
                      response->setResult(NsAppLinkRPC::Result::INVALID_ID);
                      sendRPC2MessageToMobileSide(response);
                      return;
                  }
              }
              LOG4CPLUS_INFO_EXT(mLogger, " Performing an interaction with params: ");
              if(object->get_helpPrompt())
              {
                  for(HelpPrompt::const_iterator it = object->get_helpPrompt()->begin(); it != object->get_helpPrompt()->end(); it++)
                  {
                      const NsAppLinkRPC::TTSChunk& chunk = *it;
                      LOG4CPLUS_INFO_EXT(mLogger, " Help prompt: " << chunk.get_text() << " of type " << chunk.get_type().get());
                  }
              }
              if(object->get_timeoutPrompt())
              {
                  for(TimeoutPrompt::const_iterator it = object->get_timeoutPrompt()->begin(); it != object->get_timeoutPrompt()->end(); it++)
                  {
                      const NsAppLinkRPC::TTSChunk& chunk = *it;
                      LOG4CPLUS_INFO_EXT(mLogger, " Timeout prompt: " << chunk.get_text() << " of type " << chunk.get_type().get());
                  }
              }
              for(InitialPrompt::const_iterator it = object->get_initialPrompt().begin(); it != object->get_initialPrompt().end(); it++)
              {
                  const NsAppLinkRPC::TTSChunk& chunk = *it;
                  LOG4CPLUS_INFO_EXT(mLogger, " Initial prompt: " << chunk.get_text() << " of type " << chunk.get_type().get());
              }
              LOG4CPLUS_INFO_EXT(mLogger, " Initial text: " << object->get_initialText());
              if(object->get_timeout())
              {
                  LOG4CPLUS_INFO_EXT(mLogger, " Timeout: " << *object->get_timeout());
              }
              for(std::vector<unsigned int>::const_iterator it = choiceSetIDs.begin(); it != choiceSetIDs.end(); it++)
              {
                  LOG4CPLUS_INFO_EXT(mLogger, " Choice set id: " << *it);
              }
              LOG4CPLUS_INFO_EXT(mLogger, " Interaction mode: " << object->get_interactionMode().get());
              LOG4CPLUS_INFO_EXT(mLogger, " Interaction performed!");
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMER:
          {
              LOG4CPLUS_INFO_EXT(mLogger, " A SetMediaClockTimer request has been income");
              NsRPC2Communication::UI::SetMediaClockTimer* object = (NsRPC2Communication::UI::SetMediaClockTimer*)msg;
              if(object->get_startTime())
              {
                  rc.setStartTime( *object->get_startTime() );
              }
              rc.setTimerUpdateMode(object->get_updateMode());
              NsRPC2Communication::UI::SetMediaClockTimerResponse* response = new NsRPC2Communication::UI::SetMediaClockTimerResponse;
              response->setId(object->getId());
              response->setResult(NsAppLinkRPC::Result::SUCCESS);
              sendRPC2MessageToMobileSide(response);
              return;
          }
          case NsRPC2Communication::Marshaller::METHOD_INVALID:
          default:
              LOG4CPLUS_ERROR_EXT(mLogger, " Not UI RPC message " << msg->getMethod() << " has been received!");
      }

  }

} /* namespace NsHMIEmulator */
