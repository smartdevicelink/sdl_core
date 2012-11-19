#include <cstring>
#include "Marshaller.h"

#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/ActivateApp.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/ActivateAppResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/GetAppList.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/GetAppListResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/OnAppRegistered.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/OnAppUnregistered.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/SendData.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/SendDataResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Buttons/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Buttons/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Buttons/OnButtonEvent.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Buttons/OnButtonPress.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/Speak.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/SpeakResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddSubMenu.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddSubMenuResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/Alert.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AlertResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/CreateInteractionChoiceSet.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/CreateInteractionChoiceSetResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteInteractionChoiceSet.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteInteractionChoiceSetResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteSubMenu.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteSubMenuResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnAppActivated.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnAppDeactivated.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnDriverDistraction.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnReady.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnSystemContext.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/PerformInteraction.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/PerformInteractionResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ResetGlobalProperties.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ResetGlobalPropertiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetGlobalProperties.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetGlobalPropertiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetMediaClockTimer.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetMediaClockTimerResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/Show.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ShowResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/AddCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/AddCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/DeleteCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/DeleteCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/OnCommand.h"

#include "../src/Marshaller.inc"

/*
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication;


const Marshaller::Methods Marshaller::getIndex(const char* s)
{
  if(!s)
    return METHOD_INVALID;  const struct localHash* p=Marshaller_intHash::getPointer(s,strlen(s));
  return p ? static_cast<Methods>(p->idx) : METHOD_INVALID;
}

RPC2Command* Marshaller::fromString(const std::string& s)
{
  RPC2Command* rv=0;
  try
  {
    Json::Reader reader;
    Json::Value json;

    if(!reader.parse(s,json,false))  return 0;    if(!(rv=fromJSON(json)))  return 0;
  }
  catch(...)
  {
    return 0;
  }
  return rv;
}

std::string Marshaller::toString(const RPC2Command* msg)
{
  if(!msg)  return "";

  Json::Value json=toJSON(msg);

  if(json.isNull()) return "";

  Json::FastWriter writer;
  std::string rv;
  return writer.write(json);
}

RPC2Command* Marshaller::fromJSON(const Json::Value& json)
{
  if(!json.isObject())  return 0;
  if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return 0;

  if(json.isMember("error"))				// error
  {
    RPC2Error rv;
    if(!RPC2ErrorMarshaller::fromJSON(json,rv))  return 0;

    return new RPC2Error(rv);
  }

  if(!json.isMember("id"))				// notification
  {
    if(!json.isMember("method") || !json["method"].isString())  return 0;

    Methods m=getIndex(json["method"].asString().c_str());

    switch(m)
    {
      case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPREGISTERED:
      {
        NsRPC2Communication::AppLinkCore::OnAppRegistered *rv=new NsRPC2Communication::AppLinkCore::OnAppRegistered;
        return NsRPC2Communication::AppLinkCore::OnAppRegisteredMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPUNREGISTERED:
      {
        NsRPC2Communication::AppLinkCore::OnAppUnregistered *rv=new NsRPC2Communication::AppLinkCore::OnAppUnregistered;
        return NsRPC2Communication::AppLinkCore::OnAppUnregisteredMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONEVENT:
      {
        NsRPC2Communication::Buttons::OnButtonEvent *rv=new NsRPC2Communication::Buttons::OnButtonEvent;
        return NsRPC2Communication::Buttons::OnButtonEventMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONPRESS:
      {
        NsRPC2Communication::Buttons::OnButtonPress *rv=new NsRPC2Communication::Buttons::OnButtonPress;
        return NsRPC2Communication::Buttons::OnButtonPressMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ONAPPACTIVATED:
      {
        NsRPC2Communication::UI::OnAppActivated *rv=new NsRPC2Communication::UI::OnAppActivated;
        return NsRPC2Communication::UI::OnAppActivatedMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ONAPPDEACTIVATED:
      {
        NsRPC2Communication::UI::OnAppDeactivated *rv=new NsRPC2Communication::UI::OnAppDeactivated;
        return NsRPC2Communication::UI::OnAppDeactivatedMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ONCOMMAND:
      {
        NsRPC2Communication::UI::OnCommand *rv=new NsRPC2Communication::UI::OnCommand;
        return NsRPC2Communication::UI::OnCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ONDRIVERDISTRACTION:
      {
        NsRPC2Communication::UI::OnDriverDistraction *rv=new NsRPC2Communication::UI::OnDriverDistraction;
        return NsRPC2Communication::UI::OnDriverDistractionMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ONREADY:
      {
        NsRPC2Communication::UI::OnReady *rv=new NsRPC2Communication::UI::OnReady;
        return NsRPC2Communication::UI::OnReadyMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ONSYSTEMCONTEXT:
      {
        NsRPC2Communication::UI::OnSystemContext *rv=new NsRPC2Communication::UI::OnSystemContext;
        return NsRPC2Communication::UI::OnSystemContextMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VR__ONCOMMAND:
      {
        NsRPC2Communication::VR::OnCommand *rv=new NsRPC2Communication::VR::OnCommand;
        return NsRPC2Communication::VR::OnCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      default:
        return 0;
    }
    return 0;
  }

  if(json.isMember("method"))				// request
  {
    if(!json["id"].isInt())  return 0;

    Methods m=getIndex(json["method"].asString().c_str());
    switch(m)
    {
      case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPP:
      {
        NsRPC2Communication::AppLinkCore::ActivateApp *rv=new NsRPC2Communication::AppLinkCore::ActivateApp;
        return NsRPC2Communication::AppLinkCore::ActivateAppMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLIST:
      {
        NsRPC2Communication::AppLinkCore::GetAppList *rv=new NsRPC2Communication::AppLinkCore::GetAppList;
        return NsRPC2Communication::AppLinkCore::GetAppListMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATA:
      {
        NsRPC2Communication::AppLinkCore::SendData *rv=new NsRPC2Communication::AppLinkCore::SendData;
        return NsRPC2Communication::AppLinkCore::SendDataMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIES:
      {
        NsRPC2Communication::Buttons::GetCapabilities *rv=new NsRPC2Communication::Buttons::GetCapabilities;
        return NsRPC2Communication::Buttons::GetCapabilitiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIES:
      {
        NsRPC2Communication::TTS::GetCapabilities *rv=new NsRPC2Communication::TTS::GetCapabilities;
        return NsRPC2Communication::TTS::GetCapabilitiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_TTS__SPEAK:
      {
        NsRPC2Communication::TTS::Speak *rv=new NsRPC2Communication::TTS::Speak;
        return NsRPC2Communication::TTS::SpeakMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMAND:
      {
        NsRPC2Communication::UI::AddCommand *rv=new NsRPC2Communication::UI::AddCommand;
        return NsRPC2Communication::UI::AddCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENU:
      {
        NsRPC2Communication::UI::AddSubMenu *rv=new NsRPC2Communication::UI::AddSubMenu;
        return NsRPC2Communication::UI::AddSubMenuMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ALERT:
      {
        NsRPC2Communication::UI::Alert *rv=new NsRPC2Communication::UI::Alert;
        return NsRPC2Communication::UI::AlertMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESET:
      {
        NsRPC2Communication::UI::CreateInteractionChoiceSet *rv=new NsRPC2Communication::UI::CreateInteractionChoiceSet;
        return NsRPC2Communication::UI::CreateInteractionChoiceSetMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMAND:
      {
        NsRPC2Communication::UI::DeleteCommand *rv=new NsRPC2Communication::UI::DeleteCommand;
        return NsRPC2Communication::UI::DeleteCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESET:
      {
        NsRPC2Communication::UI::DeleteInteractionChoiceSet *rv=new NsRPC2Communication::UI::DeleteInteractionChoiceSet;
        return NsRPC2Communication::UI::DeleteInteractionChoiceSetMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENU:
      {
        NsRPC2Communication::UI::DeleteSubMenu *rv=new NsRPC2Communication::UI::DeleteSubMenu;
        return NsRPC2Communication::UI::DeleteSubMenuMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIES:
      {
        NsRPC2Communication::UI::GetCapabilities *rv=new NsRPC2Communication::UI::GetCapabilities;
        return NsRPC2Communication::UI::GetCapabilitiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTION:
      {
        NsRPC2Communication::UI::PerformInteraction *rv=new NsRPC2Communication::UI::PerformInteraction;
        return NsRPC2Communication::UI::PerformInteractionMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIES:
      {
        NsRPC2Communication::UI::ResetGlobalProperties *rv=new NsRPC2Communication::UI::ResetGlobalProperties;
        return NsRPC2Communication::UI::ResetGlobalPropertiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIES:
      {
        NsRPC2Communication::UI::SetGlobalProperties *rv=new NsRPC2Communication::UI::SetGlobalProperties;
        return NsRPC2Communication::UI::SetGlobalPropertiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMER:
      {
        NsRPC2Communication::UI::SetMediaClockTimer *rv=new NsRPC2Communication::UI::SetMediaClockTimer;
        return NsRPC2Communication::UI::SetMediaClockTimerMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__SHOW:
      {
        NsRPC2Communication::UI::Show *rv=new NsRPC2Communication::UI::Show;
        return NsRPC2Communication::UI::ShowMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMAND:
      {
        NsRPC2Communication::VR::AddCommand *rv=new NsRPC2Communication::VR::AddCommand;
        return NsRPC2Communication::VR::AddCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMAND:
      {
        NsRPC2Communication::VR::DeleteCommand *rv=new NsRPC2Communication::VR::DeleteCommand;
        return NsRPC2Communication::VR::DeleteCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIES:
      {
        NsRPC2Communication::VR::GetCapabilities *rv=new NsRPC2Communication::VR::GetCapabilities;
        return NsRPC2Communication::VR::GetCapabilitiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      default:
        return 0;
    }
    return 0;
  }
							// response
  if(!json.isMember("result"))  return 0;

  if(!json["id"].isInt()) return 0;

// here is extension of protocol, two fields added: _Result and _Method
  if(!json["result"].isMember("resultCode") || !json["result"]["resultCode"].isString())  return 0;
  if(!json["result"].isMember("method") || !json["result"]["method"].isString())  return 0;

  Methods m=getIndex(json["result"]["method"].asString().c_str());

  switch(m)
  {
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPPRESPONSE:
    {
      NsRPC2Communication::AppLinkCore::ActivateAppResponse *rv=new NsRPC2Communication::AppLinkCore::ActivateAppResponse;
      return NsRPC2Communication::AppLinkCore::ActivateAppResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLISTRESPONSE:
    {
      NsRPC2Communication::AppLinkCore::GetAppListResponse *rv=new NsRPC2Communication::AppLinkCore::GetAppListResponse;
      return NsRPC2Communication::AppLinkCore::GetAppListResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATARESPONSE:
    {
      NsRPC2Communication::AppLinkCore::SendDataResponse *rv=new NsRPC2Communication::AppLinkCore::SendDataResponse;
      return NsRPC2Communication::AppLinkCore::SendDataResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIESRESPONSE:
    {
      NsRPC2Communication::Buttons::GetCapabilitiesResponse *rv=new NsRPC2Communication::Buttons::GetCapabilitiesResponse;
      return NsRPC2Communication::Buttons::GetCapabilitiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIESRESPONSE:
    {
      NsRPC2Communication::TTS::GetCapabilitiesResponse *rv=new NsRPC2Communication::TTS::GetCapabilitiesResponse;
      return NsRPC2Communication::TTS::GetCapabilitiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_TTS__SPEAKRESPONSE:
    {
      NsRPC2Communication::TTS::SpeakResponse *rv=new NsRPC2Communication::TTS::SpeakResponse;
      return NsRPC2Communication::TTS::SpeakResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMANDRESPONSE:
    {
      NsRPC2Communication::UI::AddCommandResponse *rv=new NsRPC2Communication::UI::AddCommandResponse;
      return NsRPC2Communication::UI::AddCommandResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENURESPONSE:
    {
      NsRPC2Communication::UI::AddSubMenuResponse *rv=new NsRPC2Communication::UI::AddSubMenuResponse;
      return NsRPC2Communication::UI::AddSubMenuResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE:
    {
      NsRPC2Communication::UI::AlertResponse *rv=new NsRPC2Communication::UI::AlertResponse;
      return NsRPC2Communication::UI::AlertResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESETRESPONSE:
    {
      NsRPC2Communication::UI::CreateInteractionChoiceSetResponse *rv=new NsRPC2Communication::UI::CreateInteractionChoiceSetResponse;
      return NsRPC2Communication::UI::CreateInteractionChoiceSetResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMANDRESPONSE:
    {
      NsRPC2Communication::UI::DeleteCommandResponse *rv=new NsRPC2Communication::UI::DeleteCommandResponse;
      return NsRPC2Communication::UI::DeleteCommandResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESETRESPONSE:
    {
      NsRPC2Communication::UI::DeleteInteractionChoiceSetResponse *rv=new NsRPC2Communication::UI::DeleteInteractionChoiceSetResponse;
      return NsRPC2Communication::UI::DeleteInteractionChoiceSetResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENURESPONSE:
    {
      NsRPC2Communication::UI::DeleteSubMenuResponse *rv=new NsRPC2Communication::UI::DeleteSubMenuResponse;
      return NsRPC2Communication::UI::DeleteSubMenuResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE:
    {
      NsRPC2Communication::UI::GetCapabilitiesResponse *rv=new NsRPC2Communication::UI::GetCapabilitiesResponse;
      return NsRPC2Communication::UI::GetCapabilitiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTIONRESPONSE:
    {
      NsRPC2Communication::UI::PerformInteractionResponse *rv=new NsRPC2Communication::UI::PerformInteractionResponse;
      return NsRPC2Communication::UI::PerformInteractionResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIESRESPONSE:
    {
      NsRPC2Communication::UI::ResetGlobalPropertiesResponse *rv=new NsRPC2Communication::UI::ResetGlobalPropertiesResponse;
      return NsRPC2Communication::UI::ResetGlobalPropertiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIESRESPONSE:
    {
      NsRPC2Communication::UI::SetGlobalPropertiesResponse *rv=new NsRPC2Communication::UI::SetGlobalPropertiesResponse;
      return NsRPC2Communication::UI::SetGlobalPropertiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMERRESPONSE:
    {
      NsRPC2Communication::UI::SetMediaClockTimerResponse *rv=new NsRPC2Communication::UI::SetMediaClockTimerResponse;
      return NsRPC2Communication::UI::SetMediaClockTimerResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__SHOWRESPONSE:
    {
      NsRPC2Communication::UI::ShowResponse *rv=new NsRPC2Communication::UI::ShowResponse;
      return NsRPC2Communication::UI::ShowResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMANDRESPONSE:
    {
      NsRPC2Communication::VR::AddCommandResponse *rv=new NsRPC2Communication::VR::AddCommandResponse;
      return NsRPC2Communication::VR::AddCommandResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMANDRESPONSE:
    {
      NsRPC2Communication::VR::DeleteCommandResponse *rv=new NsRPC2Communication::VR::DeleteCommandResponse;
      return NsRPC2Communication::VR::DeleteCommandResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIESRESPONSE:
    {
      NsRPC2Communication::VR::GetCapabilitiesResponse *rv=new NsRPC2Communication::VR::GetCapabilitiesResponse;
      return NsRPC2Communication::VR::GetCapabilitiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    default:
      return 0;
  }

  return 0;
}


Json::Value Marshaller::Notification2JSON(const NsRPC2Communication::RPC2Notification* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;

  switch(msg->getMethod())
  {
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPREGISTERED:
      return NsRPC2Communication::AppLinkCore::OnAppRegisteredMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::OnAppRegistered*>(msg));
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPUNREGISTERED:
      return NsRPC2Communication::AppLinkCore::OnAppUnregisteredMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::OnAppUnregistered*>(msg));
    case METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONEVENT:
      return NsRPC2Communication::Buttons::OnButtonEventMarshaller::toJSON(*static_cast<const NsRPC2Communication::Buttons::OnButtonEvent*>(msg));
    case METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONPRESS:
      return NsRPC2Communication::Buttons::OnButtonPressMarshaller::toJSON(*static_cast<const NsRPC2Communication::Buttons::OnButtonPress*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONAPPACTIVATED:
      return NsRPC2Communication::UI::OnAppActivatedMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnAppActivated*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONAPPDEACTIVATED:
      return NsRPC2Communication::UI::OnAppDeactivatedMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnAppDeactivated*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONCOMMAND:
      return NsRPC2Communication::UI::OnCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnCommand*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONDRIVERDISTRACTION:
      return NsRPC2Communication::UI::OnDriverDistractionMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnDriverDistraction*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONREADY:
      return NsRPC2Communication::UI::OnReadyMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnReady*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONSYSTEMCONTEXT:
      return NsRPC2Communication::UI::OnSystemContextMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnSystemContext*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__ONCOMMAND:
      return NsRPC2Communication::VR::OnCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::OnCommand*>(msg));
    default:
      return j;
  }
}


Json::Value Marshaller::Request2JSON(const NsRPC2Communication::RPC2Request* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;
  switch(msg->getMethod())
  {
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPP:
      return NsRPC2Communication::AppLinkCore::ActivateAppMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::ActivateApp*>(msg));
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLIST:
      return NsRPC2Communication::AppLinkCore::GetAppListMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::GetAppList*>(msg));
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATA:
      return NsRPC2Communication::AppLinkCore::SendDataMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::SendData*>(msg));
    case METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIES:
      return NsRPC2Communication::Buttons::GetCapabilitiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::Buttons::GetCapabilities*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIES:
      return NsRPC2Communication::TTS::GetCapabilitiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::GetCapabilities*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__SPEAK:
      return NsRPC2Communication::TTS::SpeakMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::Speak*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMAND:
      return NsRPC2Communication::UI::AddCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::AddCommand*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENU:
      return NsRPC2Communication::UI::AddSubMenuMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::AddSubMenu*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ALERT:
      return NsRPC2Communication::UI::AlertMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::Alert*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESET:
      return NsRPC2Communication::UI::CreateInteractionChoiceSetMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::CreateInteractionChoiceSet*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMAND:
      return NsRPC2Communication::UI::DeleteCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteCommand*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESET:
      return NsRPC2Communication::UI::DeleteInteractionChoiceSetMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteInteractionChoiceSet*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENU:
      return NsRPC2Communication::UI::DeleteSubMenuMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteSubMenu*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIES:
      return NsRPC2Communication::UI::GetCapabilitiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::GetCapabilities*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTION:
      return NsRPC2Communication::UI::PerformInteractionMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::PerformInteraction*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIES:
      return NsRPC2Communication::UI::ResetGlobalPropertiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::ResetGlobalProperties*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIES:
      return NsRPC2Communication::UI::SetGlobalPropertiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetGlobalProperties*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMER:
      return NsRPC2Communication::UI::SetMediaClockTimerMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetMediaClockTimer*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SHOW:
      return NsRPC2Communication::UI::ShowMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::Show*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMAND:
      return NsRPC2Communication::VR::AddCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::AddCommand*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMAND:
      return NsRPC2Communication::VR::DeleteCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::DeleteCommand*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIES:
      return NsRPC2Communication::VR::GetCapabilitiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::GetCapabilities*>(msg));
    default:
      return j;
  }
}


Json::Value Marshaller::Response2JSON(const NsRPC2Communication::RPC2Response* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;
  switch(msg->getMethod())
  {
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPPRESPONSE:
      return NsRPC2Communication::AppLinkCore::ActivateAppResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::ActivateAppResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLISTRESPONSE:
      return NsRPC2Communication::AppLinkCore::GetAppListResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::GetAppListResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATARESPONSE:
      return NsRPC2Communication::AppLinkCore::SendDataResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::SendDataResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIESRESPONSE:
      return NsRPC2Communication::Buttons::GetCapabilitiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::Buttons::GetCapabilitiesResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIESRESPONSE:
      return NsRPC2Communication::TTS::GetCapabilitiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::GetCapabilitiesResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__SPEAKRESPONSE:
      return NsRPC2Communication::TTS::SpeakResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::SpeakResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMANDRESPONSE:
      return NsRPC2Communication::UI::AddCommandResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::AddCommandResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENURESPONSE:
      return NsRPC2Communication::UI::AddSubMenuResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::AddSubMenuResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE:
      return NsRPC2Communication::UI::AlertResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::AlertResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESETRESPONSE:
      return NsRPC2Communication::UI::CreateInteractionChoiceSetResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::CreateInteractionChoiceSetResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMANDRESPONSE:
      return NsRPC2Communication::UI::DeleteCommandResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteCommandResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESETRESPONSE:
      return NsRPC2Communication::UI::DeleteInteractionChoiceSetResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteInteractionChoiceSetResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENURESPONSE:
      return NsRPC2Communication::UI::DeleteSubMenuResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteSubMenuResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE:
      return NsRPC2Communication::UI::GetCapabilitiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::GetCapabilitiesResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTIONRESPONSE:
      return NsRPC2Communication::UI::PerformInteractionResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::PerformInteractionResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIESRESPONSE:
      return NsRPC2Communication::UI::ResetGlobalPropertiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::ResetGlobalPropertiesResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIESRESPONSE:
      return NsRPC2Communication::UI::SetGlobalPropertiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetGlobalPropertiesResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMERRESPONSE:
      return NsRPC2Communication::UI::SetMediaClockTimerResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetMediaClockTimerResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SHOWRESPONSE:
      return NsRPC2Communication::UI::ShowResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::ShowResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMANDRESPONSE:
      return NsRPC2Communication::VR::AddCommandResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::AddCommandResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMANDRESPONSE:
      return NsRPC2Communication::VR::DeleteCommandResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::DeleteCommandResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIESRESPONSE:
      return NsRPC2Communication::VR::GetCapabilitiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::GetCapabilitiesResponse*>(msg));
    default:
      return j;
  }
}


Json::Value Marshaller::toJSON(const RPC2Command* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;

  switch(msg->getCommandType())
  {
    case RPC2Command::REQUEST:
      return Request2JSON(static_cast<const RPC2Request*>(msg));
    case RPC2Command::RESPONSE:
      return Response2JSON(static_cast<const RPC2Response*>(msg));
    case RPC2Command::NOTIFICATION:
      return Notification2JSON(static_cast<const RPC2Notification*>(msg));

    case RPC2Command::ERROR:
      return RPC2ErrorMarshaller::toJSON(*static_cast<const RPC2Error*>(msg));

    case RPC2Command::UNDEFINED:
    default:
      return j;
  }
}


const Marshaller::localHash Marshaller::mHashTable[55]=
{
  {"AppLinkCore.ActivateApp",0,&Marshaller::mNsRPC2Communication_AppLinkCore__ActivateApp},
  {"AppLinkCore.ActivateAppResponse",1,&Marshaller::mNsRPC2Communication_AppLinkCore__ActivateAppResponse},
  {"AppLinkCore.GetAppList",2,&Marshaller::mNsRPC2Communication_AppLinkCore__GetAppList},
  {"AppLinkCore.GetAppListResponse",3,&Marshaller::mNsRPC2Communication_AppLinkCore__GetAppListResponse},
  {"AppLinkCore.OnAppRegistered",4,&Marshaller::mNsRPC2Communication_AppLinkCore__OnAppRegistered},
  {"AppLinkCore.OnAppUnregistered",5,&Marshaller::mNsRPC2Communication_AppLinkCore__OnAppUnregistered},
  {"AppLinkCore.SendData",6,&Marshaller::mNsRPC2Communication_AppLinkCore__SendData},
  {"AppLinkCore.SendDataResponse",7,&Marshaller::mNsRPC2Communication_AppLinkCore__SendDataResponse},
  {"Buttons.GetCapabilities",8,&Marshaller::mNsRPC2Communication_Buttons__GetCapabilities},
  {"Buttons.GetCapabilitiesResponse",9,&Marshaller::mNsRPC2Communication_Buttons__GetCapabilitiesResponse},
  {"Buttons.OnButtonEvent",10,&Marshaller::mNsRPC2Communication_Buttons__OnButtonEvent},
  {"Buttons.OnButtonPress",11,&Marshaller::mNsRPC2Communication_Buttons__OnButtonPress},
  {"TTS.GetCapabilities",12,&Marshaller::mNsRPC2Communication_TTS__GetCapabilities},
  {"TTS.GetCapabilitiesResponse",13,&Marshaller::mNsRPC2Communication_TTS__GetCapabilitiesResponse},
  {"TTS.Speak",14,&Marshaller::mNsRPC2Communication_TTS__Speak},
  {"TTS.SpeakResponse",15,&Marshaller::mNsRPC2Communication_TTS__SpeakResponse},
  {"UI.AddCommand",16,&Marshaller::mNsRPC2Communication_UI__AddCommand},
  {"UI.AddCommandResponse",17,&Marshaller::mNsRPC2Communication_UI__AddCommandResponse},
  {"UI.AddSubMenu",18,&Marshaller::mNsRPC2Communication_UI__AddSubMenu},
  {"UI.AddSubMenuResponse",19,&Marshaller::mNsRPC2Communication_UI__AddSubMenuResponse},
  {"UI.Alert",20,&Marshaller::mNsRPC2Communication_UI__Alert},
  {"UI.AlertResponse",21,&Marshaller::mNsRPC2Communication_UI__AlertResponse},
  {"UI.CreateInteractionChoiceSet",22,&Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSet},
  {"UI.CreateInteractionChoiceSetResponse",23,&Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse},
  {"UI.DeleteCommand",24,&Marshaller::mNsRPC2Communication_UI__DeleteCommand},
  {"UI.DeleteCommandResponse",25,&Marshaller::mNsRPC2Communication_UI__DeleteCommandResponse},
  {"UI.DeleteInteractionChoiceSet",26,&Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSet},
  {"UI.DeleteInteractionChoiceSetResponse",27,&Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse},
  {"UI.DeleteSubMenu",28,&Marshaller::mNsRPC2Communication_UI__DeleteSubMenu},
  {"UI.DeleteSubMenuResponse",29,&Marshaller::mNsRPC2Communication_UI__DeleteSubMenuResponse},
  {"UI.GetCapabilities",30,&Marshaller::mNsRPC2Communication_UI__GetCapabilities},
  {"UI.GetCapabilitiesResponse",31,&Marshaller::mNsRPC2Communication_UI__GetCapabilitiesResponse},
  {"UI.OnAppActivated",32,&Marshaller::mNsRPC2Communication_UI__OnAppActivated},
  {"UI.OnAppDeactivated",33,&Marshaller::mNsRPC2Communication_UI__OnAppDeactivated},
  {"UI.OnCommand",34,&Marshaller::mNsRPC2Communication_UI__OnCommand},
  {"UI.OnDriverDistraction",35,&Marshaller::mNsRPC2Communication_UI__OnDriverDistraction},
  {"UI.OnReady",36,&Marshaller::mNsRPC2Communication_UI__OnReady},
  {"UI.OnSystemContext",37,&Marshaller::mNsRPC2Communication_UI__OnSystemContext},
  {"UI.PerformInteraction",38,&Marshaller::mNsRPC2Communication_UI__PerformInteraction},
  {"UI.PerformInteractionResponse",39,&Marshaller::mNsRPC2Communication_UI__PerformInteractionResponse},
  {"UI.ResetGlobalProperties",40,&Marshaller::mNsRPC2Communication_UI__ResetGlobalProperties},
  {"UI.ResetGlobalPropertiesResponse",41,&Marshaller::mNsRPC2Communication_UI__ResetGlobalPropertiesResponse},
  {"UI.SetGlobalProperties",42,&Marshaller::mNsRPC2Communication_UI__SetGlobalProperties},
  {"UI.SetGlobalPropertiesResponse",43,&Marshaller::mNsRPC2Communication_UI__SetGlobalPropertiesResponse},
  {"UI.SetMediaClockTimer",44,&Marshaller::mNsRPC2Communication_UI__SetMediaClockTimer},
  {"UI.SetMediaClockTimerResponse",45,&Marshaller::mNsRPC2Communication_UI__SetMediaClockTimerResponse},
  {"UI.Show",46,&Marshaller::mNsRPC2Communication_UI__Show},
  {"UI.ShowResponse",47,&Marshaller::mNsRPC2Communication_UI__ShowResponse},
  {"VR.AddCommand",48,&Marshaller::mNsRPC2Communication_VR__AddCommand},
  {"VR.AddCommandResponse",49,&Marshaller::mNsRPC2Communication_VR__AddCommandResponse},
  {"VR.DeleteCommand",50,&Marshaller::mNsRPC2Communication_VR__DeleteCommand},
  {"VR.DeleteCommandResponse",51,&Marshaller::mNsRPC2Communication_VR__DeleteCommandResponse},
  {"VR.GetCapabilities",52,&Marshaller::mNsRPC2Communication_VR__GetCapabilities},
  {"VR.GetCapabilitiesResponse",53,&Marshaller::mNsRPC2Communication_VR__GetCapabilitiesResponse},
  {"VR.OnCommand",54,&Marshaller::mNsRPC2Communication_VR__OnCommand}

};

NsRPC2Communication::RPC2ErrorMarshaller Marshaller::mRPC2ErrorInternal;

NsRPC2Communication::AppLinkCore::ActivateAppMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__ActivateApp;
NsRPC2Communication::AppLinkCore::ActivateAppResponseMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__ActivateAppResponse;
NsRPC2Communication::AppLinkCore::GetAppListMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__GetAppList;
NsRPC2Communication::AppLinkCore::GetAppListResponseMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__GetAppListResponse;
NsRPC2Communication::AppLinkCore::OnAppRegisteredMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__OnAppRegistered;
NsRPC2Communication::AppLinkCore::OnAppUnregisteredMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__OnAppUnregistered;
NsRPC2Communication::AppLinkCore::SendDataMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__SendData;
NsRPC2Communication::AppLinkCore::SendDataResponseMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__SendDataResponse;
NsRPC2Communication::Buttons::GetCapabilitiesMarshaller Marshaller::mNsRPC2Communication_Buttons__GetCapabilities;
NsRPC2Communication::Buttons::GetCapabilitiesResponseMarshaller Marshaller::mNsRPC2Communication_Buttons__GetCapabilitiesResponse;
NsRPC2Communication::Buttons::OnButtonEventMarshaller Marshaller::mNsRPC2Communication_Buttons__OnButtonEvent;
NsRPC2Communication::Buttons::OnButtonPressMarshaller Marshaller::mNsRPC2Communication_Buttons__OnButtonPress;
NsRPC2Communication::TTS::GetCapabilitiesMarshaller Marshaller::mNsRPC2Communication_TTS__GetCapabilities;
NsRPC2Communication::TTS::GetCapabilitiesResponseMarshaller Marshaller::mNsRPC2Communication_TTS__GetCapabilitiesResponse;
NsRPC2Communication::TTS::SpeakMarshaller Marshaller::mNsRPC2Communication_TTS__Speak;
NsRPC2Communication::TTS::SpeakResponseMarshaller Marshaller::mNsRPC2Communication_TTS__SpeakResponse;
NsRPC2Communication::UI::AddCommandMarshaller Marshaller::mNsRPC2Communication_UI__AddCommand;
NsRPC2Communication::UI::AddCommandResponseMarshaller Marshaller::mNsRPC2Communication_UI__AddCommandResponse;
NsRPC2Communication::UI::AddSubMenuMarshaller Marshaller::mNsRPC2Communication_UI__AddSubMenu;
NsRPC2Communication::UI::AddSubMenuResponseMarshaller Marshaller::mNsRPC2Communication_UI__AddSubMenuResponse;
NsRPC2Communication::UI::AlertMarshaller Marshaller::mNsRPC2Communication_UI__Alert;
NsRPC2Communication::UI::AlertResponseMarshaller Marshaller::mNsRPC2Communication_UI__AlertResponse;
NsRPC2Communication::UI::CreateInteractionChoiceSetMarshaller Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSet;
NsRPC2Communication::UI::CreateInteractionChoiceSetResponseMarshaller Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse;
NsRPC2Communication::UI::DeleteCommandMarshaller Marshaller::mNsRPC2Communication_UI__DeleteCommand;
NsRPC2Communication::UI::DeleteCommandResponseMarshaller Marshaller::mNsRPC2Communication_UI__DeleteCommandResponse;
NsRPC2Communication::UI::DeleteInteractionChoiceSetMarshaller Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSet;
NsRPC2Communication::UI::DeleteInteractionChoiceSetResponseMarshaller Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse;
NsRPC2Communication::UI::DeleteSubMenuMarshaller Marshaller::mNsRPC2Communication_UI__DeleteSubMenu;
NsRPC2Communication::UI::DeleteSubMenuResponseMarshaller Marshaller::mNsRPC2Communication_UI__DeleteSubMenuResponse;
NsRPC2Communication::UI::GetCapabilitiesMarshaller Marshaller::mNsRPC2Communication_UI__GetCapabilities;
NsRPC2Communication::UI::GetCapabilitiesResponseMarshaller Marshaller::mNsRPC2Communication_UI__GetCapabilitiesResponse;
NsRPC2Communication::UI::OnAppActivatedMarshaller Marshaller::mNsRPC2Communication_UI__OnAppActivated;
NsRPC2Communication::UI::OnAppDeactivatedMarshaller Marshaller::mNsRPC2Communication_UI__OnAppDeactivated;
NsRPC2Communication::UI::OnCommandMarshaller Marshaller::mNsRPC2Communication_UI__OnCommand;
NsRPC2Communication::UI::OnDriverDistractionMarshaller Marshaller::mNsRPC2Communication_UI__OnDriverDistraction;
NsRPC2Communication::UI::OnReadyMarshaller Marshaller::mNsRPC2Communication_UI__OnReady;
NsRPC2Communication::UI::OnSystemContextMarshaller Marshaller::mNsRPC2Communication_UI__OnSystemContext;
NsRPC2Communication::UI::PerformInteractionMarshaller Marshaller::mNsRPC2Communication_UI__PerformInteraction;
NsRPC2Communication::UI::PerformInteractionResponseMarshaller Marshaller::mNsRPC2Communication_UI__PerformInteractionResponse;
NsRPC2Communication::UI::ResetGlobalPropertiesMarshaller Marshaller::mNsRPC2Communication_UI__ResetGlobalProperties;
NsRPC2Communication::UI::ResetGlobalPropertiesResponseMarshaller Marshaller::mNsRPC2Communication_UI__ResetGlobalPropertiesResponse;
NsRPC2Communication::UI::SetGlobalPropertiesMarshaller Marshaller::mNsRPC2Communication_UI__SetGlobalProperties;
NsRPC2Communication::UI::SetGlobalPropertiesResponseMarshaller Marshaller::mNsRPC2Communication_UI__SetGlobalPropertiesResponse;
NsRPC2Communication::UI::SetMediaClockTimerMarshaller Marshaller::mNsRPC2Communication_UI__SetMediaClockTimer;
NsRPC2Communication::UI::SetMediaClockTimerResponseMarshaller Marshaller::mNsRPC2Communication_UI__SetMediaClockTimerResponse;
NsRPC2Communication::UI::ShowMarshaller Marshaller::mNsRPC2Communication_UI__Show;
NsRPC2Communication::UI::ShowResponseMarshaller Marshaller::mNsRPC2Communication_UI__ShowResponse;
NsRPC2Communication::VR::AddCommandMarshaller Marshaller::mNsRPC2Communication_VR__AddCommand;
NsRPC2Communication::VR::AddCommandResponseMarshaller Marshaller::mNsRPC2Communication_VR__AddCommandResponse;
NsRPC2Communication::VR::DeleteCommandMarshaller Marshaller::mNsRPC2Communication_VR__DeleteCommand;
NsRPC2Communication::VR::DeleteCommandResponseMarshaller Marshaller::mNsRPC2Communication_VR__DeleteCommandResponse;
NsRPC2Communication::VR::GetCapabilitiesMarshaller Marshaller::mNsRPC2Communication_VR__GetCapabilities;
NsRPC2Communication::VR::GetCapabilitiesResponseMarshaller Marshaller::mNsRPC2Communication_VR__GetCapabilitiesResponse;
NsRPC2Communication::VR::OnCommandMarshaller Marshaller::mNsRPC2Communication_VR__OnCommand;
