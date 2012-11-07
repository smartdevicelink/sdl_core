#include <cstring>
#include "../include/JSONHandler/RPC2Objects/UI/Marshaller.h"

#include "../include/JSONHandler/RPC2Objects/UI/AddCommand.h"
#include "../include/JSONHandler/RPC2Objects/UI/AddCommandResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/AddSubMenu.h"
#include "../include/JSONHandler/RPC2Objects/UI/AddSubMenuResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/Alert.h"
#include "../include/JSONHandler/RPC2Objects/UI/AlertResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/CreateInteractionChoiceSet.h"
#include "../include/JSONHandler/RPC2Objects/UI/CreateInteractionChoiceSetResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/DeleteCommand.h"
#include "../include/JSONHandler/RPC2Objects/UI/DeleteCommandResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/DeleteInteractionChoiceSet.h"
#include "../include/JSONHandler/RPC2Objects/UI/DeleteInteractionChoiceSetResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/DeleteSubMenu.h"
#include "../include/JSONHandler/RPC2Objects/UI/DeleteSubMenuResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/GetCapabilities.h"
#include "../include/JSONHandler/RPC2Objects/UI/GetCapabilitiesResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/OnAppActivated.h"
#include "../include/JSONHandler/RPC2Objects/UI/OnAppDeactivated.h"
#include "../include/JSONHandler/RPC2Objects/UI/OnCommand.h"
#include "../include/JSONHandler/RPC2Objects/UI/OnSystemContext.h"
#include "../include/JSONHandler/RPC2Objects/UI/PerformInteraction.h"
#include "../include/JSONHandler/RPC2Objects/UI/PerformInteractionResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/ResetGlobalProperties.h"
#include "../include/JSONHandler/RPC2Objects/UI/ResetGlobalPropertiesResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/SetGlobalProperties.h"
#include "../include/JSONHandler/RPC2Objects/UI/SetGlobalPropertiesResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/SetMediaClockTimer.h"
#include "../include/JSONHandler/RPC2Objects/UI/SetMediaClockTimerResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/Show.h"
#include "../include/JSONHandler/RPC2Objects/UI/ShowResponse.h"
#include <iostream>

namespace RPC2Communication
{
  namespace UI
  {
#include "Marshaller.inc"
  }
}

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;


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

  std::cout<<"UI Marshaller::fromJSON::json.isMember(id) " << json.isMember("id")<<std::endl;
  if(!json.isMember("id"))				// notification
  {
    if(!json.isMember("method") || !json["method"].isString())  return 0;

    Methods m=getIndex(json["method"].asString().c_str());

    switch(m)
    {
      case METHOD_ONAPPACTIVATED:
      {
        OnAppActivated *rv=new OnAppActivated;
        return OnAppActivatedMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_ONAPPDEACTIVATED:
      {
        OnAppDeactivated *rv=new OnAppDeactivated;
        return OnAppDeactivatedMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_ONCOMMAND:
      {
        OnCommand *rv=new OnCommand;
        return OnCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_ONSYSTEMCONTEXT:
      {
        OnSystemContext *rv=new OnSystemContext;
        return OnSystemContextMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      default:
        return 0;
    }
    return 0;
  }

  std::cout<<"UI Marshaller::fromJSON::json.isMember(method) " << json.isMember("method")<<std::endl;
  if(json.isMember("method"))				// request
  {
    if(!json["id"].isInt())  return 0;

    Methods m=getIndex(json["method"].asString().c_str());
    switch(m)
    {
      case METHOD_ADDCOMMAND:
      {
        AddCommand *rv=new AddCommand;
        return AddCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_ADDSUBMENU:
      {
        AddSubMenu *rv=new AddSubMenu;
        return AddSubMenuMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_ALERT:
      {
        Alert *rv=new Alert;
        return AlertMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_CREATEINTERACTIONCHOICESET:
      {
        CreateInteractionChoiceSet *rv=new CreateInteractionChoiceSet;
        return CreateInteractionChoiceSetMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_DELETECOMMAND:
      {
        DeleteCommand *rv=new DeleteCommand;
        return DeleteCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_DELETEINTERACTIONCHOICESET:
      {
        DeleteInteractionChoiceSet *rv=new DeleteInteractionChoiceSet;
        return DeleteInteractionChoiceSetMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_DELETESUBMENU:
      {
        DeleteSubMenu *rv=new DeleteSubMenu;
        return DeleteSubMenuMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_GETCAPABILITIES:
      {
        GetCapabilities *rv=new GetCapabilities;
        return GetCapabilitiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_PERFORMINTERACTION:
      {
        PerformInteraction *rv=new PerformInteraction;
        return PerformInteractionMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_RESETGLOBALPROPERTIES:
      {
        ResetGlobalProperties *rv=new ResetGlobalProperties;
        return ResetGlobalPropertiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_SETGLOBALPROPERTIES:
      {
        SetGlobalProperties *rv=new SetGlobalProperties;
        return SetGlobalPropertiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_SETMEDIACLOCKTIMER:
      {
        SetMediaClockTimer *rv=new SetMediaClockTimer;
        return SetMediaClockTimerMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_SHOW:
      {
        Show *rv=new Show;
        return ShowMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      default:
        return 0;
    }
    return 0;
  }
  std::cout<<"UI Marshaller::fromJSON::json.isMember(result) " << json.isMember("result")<<std::endl;
							// response
  if(!json.isMember("result"))  return 0;

std::cout<<"UI Marshaller::fromJSON::json.isMember(id) " << json.isMember("id")<<std::endl;
  if(!json["id"].isInt()) return 0;

// here is extension of protocol, two fields added: resultCode and method
  if(!json["result"].isMember("resultCode") || !json["result"]["resultCode"].isString())  return 0;
  std::cout<<"UI Marshaller::fromJSON::json.isMember(resultCode) " << json["result"].isMember("resultCode")<<std::endl;
  if(!json["result"].isMember("method") || !json["result"]["method"].isString())  return 0;
  std::cout<<"UI Marshaller::fromJSON::json.isMember(method) " << json["result"].isMember("method")<<std::endl;

  Methods m=getIndex(json["result"]["method"].asString().c_str());

  std::cout<<"UI Marshaller::fromJSON:: method " << json["result"]["method"].asString() << std::endl;

  switch(m)
  {
    case METHOD_ADDCOMMANDRESPONSE:
    {
      AddCommandResponse *rv=new AddCommandResponse;
      return AddCommandResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_ADDSUBMENURESPONSE:
    {
      AddSubMenuResponse *rv=new AddSubMenuResponse;
      return AddSubMenuResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_ALERTRESPONSE:
    {
      AlertResponse *rv=new AlertResponse;
      return AlertResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_CREATEINTERACTIONCHOICESETRESPONSE:
    {
      CreateInteractionChoiceSetResponse *rv=new CreateInteractionChoiceSetResponse;
      return CreateInteractionChoiceSetResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_DELETECOMMANDRESPONSE:
    {
      DeleteCommandResponse *rv=new DeleteCommandResponse;
      return DeleteCommandResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_DELETEINTERACTIONCHOICESETRESPONSE:
    {
      DeleteInteractionChoiceSetResponse *rv=new DeleteInteractionChoiceSetResponse;
      return DeleteInteractionChoiceSetResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_DELETESUBMENURESPONSE:
    {
      DeleteSubMenuResponse *rv=new DeleteSubMenuResponse;
      return DeleteSubMenuResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_GETCAPABILITIESRESPONSE:
    {
      GetCapabilitiesResponse *rv=new GetCapabilitiesResponse;
      return GetCapabilitiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_PERFORMINTERACTIONRESPONSE:
    {
      PerformInteractionResponse *rv=new PerformInteractionResponse;
      return PerformInteractionResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_RESETGLOBALPROPERTIESRESPONSE:
    {
      ResetGlobalPropertiesResponse *rv=new ResetGlobalPropertiesResponse;
      return ResetGlobalPropertiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_SETGLOBALPROPERTIESRESPONSE:
    {
      SetGlobalPropertiesResponse *rv=new SetGlobalPropertiesResponse;
      return SetGlobalPropertiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_SETMEDIACLOCKTIMERRESPONSE:
    {
      SetMediaClockTimerResponse *rv=new SetMediaClockTimerResponse;
      return SetMediaClockTimerResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_SHOWRESPONSE:
    {
      ShowResponse *rv=new ShowResponse;
      return ShowResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    default:
      return 0;
  }

  return 0;
}


Json::Value Marshaller::Notification2JSON(const RPC2Communication::RPC2Notification* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;

  switch(msg->getMethod())
  {
    case METHOD_ONAPPACTIVATED:
      return OnAppActivatedMarshaller::toJSON(*static_cast<const OnAppActivated*>(msg));
    case METHOD_ONAPPDEACTIVATED:
      return OnAppDeactivatedMarshaller::toJSON(*static_cast<const OnAppDeactivated*>(msg));
    case METHOD_ONCOMMAND:
      return OnCommandMarshaller::toJSON(*static_cast<const OnCommand*>(msg));
    case METHOD_ONSYSTEMCONTEXT:
      return OnSystemContextMarshaller::toJSON(*static_cast<const OnSystemContext*>(msg));
    default:
      return j;
  }
}


Json::Value Marshaller::Request2JSON(const RPC2Communication::RPC2Request* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;
  switch(msg->getMethod())
  {
    case METHOD_ADDCOMMAND:
      return AddCommandMarshaller::toJSON(*static_cast<const AddCommand*>(msg));
    case METHOD_ADDSUBMENU:
      return AddSubMenuMarshaller::toJSON(*static_cast<const AddSubMenu*>(msg));
    case METHOD_ALERT:
      return AlertMarshaller::toJSON(*static_cast<const Alert*>(msg));
    case METHOD_CREATEINTERACTIONCHOICESET:
      return CreateInteractionChoiceSetMarshaller::toJSON(*static_cast<const CreateInteractionChoiceSet*>(msg));
    case METHOD_DELETECOMMAND:
      return DeleteCommandMarshaller::toJSON(*static_cast<const DeleteCommand*>(msg));
    case METHOD_DELETEINTERACTIONCHOICESET:
      return DeleteInteractionChoiceSetMarshaller::toJSON(*static_cast<const DeleteInteractionChoiceSet*>(msg));
    case METHOD_DELETESUBMENU:
      return DeleteSubMenuMarshaller::toJSON(*static_cast<const DeleteSubMenu*>(msg));
    case METHOD_GETCAPABILITIES:
      return GetCapabilitiesMarshaller::toJSON(*static_cast<const GetCapabilities*>(msg));
    case METHOD_PERFORMINTERACTION:
      return PerformInteractionMarshaller::toJSON(*static_cast<const PerformInteraction*>(msg));
    case METHOD_RESETGLOBALPROPERTIES:
      return ResetGlobalPropertiesMarshaller::toJSON(*static_cast<const ResetGlobalProperties*>(msg));
    case METHOD_SETGLOBALPROPERTIES:
      return SetGlobalPropertiesMarshaller::toJSON(*static_cast<const SetGlobalProperties*>(msg));
    case METHOD_SETMEDIACLOCKTIMER:
      return SetMediaClockTimerMarshaller::toJSON(*static_cast<const SetMediaClockTimer*>(msg));
    case METHOD_SHOW:
      return ShowMarshaller::toJSON(*static_cast<const Show*>(msg));
    default:
      return j;
  }
}


Json::Value Marshaller::Response2JSON(const RPC2Communication::RPC2Response* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;
  switch(msg->getMethod())
  {
    case METHOD_ADDCOMMANDRESPONSE:
      return AddCommandResponseMarshaller::toJSON(*static_cast<const AddCommandResponse*>(msg));
    case METHOD_ADDSUBMENURESPONSE:
      return AddSubMenuResponseMarshaller::toJSON(*static_cast<const AddSubMenuResponse*>(msg));
    case METHOD_ALERTRESPONSE:
      return AlertResponseMarshaller::toJSON(*static_cast<const AlertResponse*>(msg));
    case METHOD_CREATEINTERACTIONCHOICESETRESPONSE:
      return CreateInteractionChoiceSetResponseMarshaller::toJSON(*static_cast<const CreateInteractionChoiceSetResponse*>(msg));
    case METHOD_DELETECOMMANDRESPONSE:
      return DeleteCommandResponseMarshaller::toJSON(*static_cast<const DeleteCommandResponse*>(msg));
    case METHOD_DELETEINTERACTIONCHOICESETRESPONSE:
      return DeleteInteractionChoiceSetResponseMarshaller::toJSON(*static_cast<const DeleteInteractionChoiceSetResponse*>(msg));
    case METHOD_DELETESUBMENURESPONSE:
      return DeleteSubMenuResponseMarshaller::toJSON(*static_cast<const DeleteSubMenuResponse*>(msg));
    case METHOD_GETCAPABILITIESRESPONSE:
      return GetCapabilitiesResponseMarshaller::toJSON(*static_cast<const GetCapabilitiesResponse*>(msg));
    case METHOD_PERFORMINTERACTIONRESPONSE:
      return PerformInteractionResponseMarshaller::toJSON(*static_cast<const PerformInteractionResponse*>(msg));
    case METHOD_RESETGLOBALPROPERTIESRESPONSE:
      return ResetGlobalPropertiesResponseMarshaller::toJSON(*static_cast<const ResetGlobalPropertiesResponse*>(msg));
    case METHOD_SETGLOBALPROPERTIESRESPONSE:
      return SetGlobalPropertiesResponseMarshaller::toJSON(*static_cast<const SetGlobalPropertiesResponse*>(msg));
    case METHOD_SETMEDIACLOCKTIMERRESPONSE:
      return SetMediaClockTimerResponseMarshaller::toJSON(*static_cast<const SetMediaClockTimerResponse*>(msg));
    case METHOD_SHOWRESPONSE:
      return ShowResponseMarshaller::toJSON(*static_cast<const ShowResponse*>(msg));
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


const Marshaller::localHash Marshaller::mHashTable[30]=
{
  {"UI.AddCommand",0,&Marshaller::mAddCommand},
  {"UI.AddCommandResponse",1,&Marshaller::mAddCommandResponse},
  {"UI.AddSubMenu",2,&Marshaller::mAddSubMenu},
  {"UI.AddSubMenuResponse",3,&Marshaller::mAddSubMenuResponse},
  {"UI.Alert",4,&Marshaller::mAlert},
  {"UI.AlertResponse",5,&Marshaller::mAlertResponse},
  {"UI.CreateInteractionChoiceSet",6,&Marshaller::mCreateInteractionChoiceSet},
  {"UI.CreateInteractionChoiceSetResponse",7,&Marshaller::mCreateInteractionChoiceSetResponse},
  {"UI.DeleteCommand",8,&Marshaller::mDeleteCommand},
  {"UI.DeleteCommandResponse",9,&Marshaller::mDeleteCommandResponse},
  {"UI.DeleteInteractionChoiceSet",10,&Marshaller::mDeleteInteractionChoiceSet},
  {"UI.DeleteInteractionChoiceSetResponse",11,&Marshaller::mDeleteInteractionChoiceSetResponse},
  {"UI.DeleteSubMenu",12,&Marshaller::mDeleteSubMenu},
  {"UI.DeleteSubMenuResponse",13,&Marshaller::mDeleteSubMenuResponse},
  {"UI.GetCapabilities",14,&Marshaller::mGetCapabilities},
  {"UI.GetCapabilitiesResponse",15,&Marshaller::mGetCapabilitiesResponse},
  {"UI.OnAppActivated",16,&Marshaller::mOnAppActivated},
  {"UI.OnAppDeactivated",17,&Marshaller::mOnAppDeactivated},
  {"UI.OnCommand",18,&Marshaller::mOnCommand},
  {"UI.OnSystemContext",19,&Marshaller::mOnSystemContext},
  {"UI.PerformInteraction",20,&Marshaller::mPerformInteraction},
  {"UI.PerformInteractionResponse",21,&Marshaller::mPerformInteractionResponse},
  {"UI.ResetGlobalProperties",22,&Marshaller::mResetGlobalProperties},
  {"UI.ResetGlobalPropertiesResponse",23,&Marshaller::mResetGlobalPropertiesResponse},
  {"UI.SetGlobalProperties",24,&Marshaller::mSetGlobalProperties},
  {"UI.SetGlobalPropertiesResponse",25,&Marshaller::mSetGlobalPropertiesResponse},
  {"UI.SetMediaClockTimer",26,&Marshaller::mSetMediaClockTimer},
  {"UI.SetMediaClockTimerResponse",27,&Marshaller::mSetMediaClockTimerResponse},
  {"UI.Show",28,&Marshaller::mShow},
  {"UI.ShowResponse",29,&Marshaller::mShowResponse}

};

RPC2Communication::RPC2ErrorMarshaller Marshaller::mRPC2ErrorInternal;

AddCommandMarshaller Marshaller::mAddCommand;
AddCommandResponseMarshaller Marshaller::mAddCommandResponse;
AddSubMenuMarshaller Marshaller::mAddSubMenu;
AddSubMenuResponseMarshaller Marshaller::mAddSubMenuResponse;
AlertMarshaller Marshaller::mAlert;
AlertResponseMarshaller Marshaller::mAlertResponse;
CreateInteractionChoiceSetMarshaller Marshaller::mCreateInteractionChoiceSet;
CreateInteractionChoiceSetResponseMarshaller Marshaller::mCreateInteractionChoiceSetResponse;
DeleteCommandMarshaller Marshaller::mDeleteCommand;
DeleteCommandResponseMarshaller Marshaller::mDeleteCommandResponse;
DeleteInteractionChoiceSetMarshaller Marshaller::mDeleteInteractionChoiceSet;
DeleteInteractionChoiceSetResponseMarshaller Marshaller::mDeleteInteractionChoiceSetResponse;
DeleteSubMenuMarshaller Marshaller::mDeleteSubMenu;
DeleteSubMenuResponseMarshaller Marshaller::mDeleteSubMenuResponse;
GetCapabilitiesMarshaller Marshaller::mGetCapabilities;
GetCapabilitiesResponseMarshaller Marshaller::mGetCapabilitiesResponse;
OnAppActivatedMarshaller Marshaller::mOnAppActivated;
OnAppDeactivatedMarshaller Marshaller::mOnAppDeactivated;
OnCommandMarshaller Marshaller::mOnCommand;
OnSystemContextMarshaller Marshaller::mOnSystemContext;
PerformInteractionMarshaller Marshaller::mPerformInteraction;
PerformInteractionResponseMarshaller Marshaller::mPerformInteractionResponse;
ResetGlobalPropertiesMarshaller Marshaller::mResetGlobalProperties;
ResetGlobalPropertiesResponseMarshaller Marshaller::mResetGlobalPropertiesResponse;
SetGlobalPropertiesMarshaller Marshaller::mSetGlobalProperties;
SetGlobalPropertiesResponseMarshaller Marshaller::mSetGlobalPropertiesResponse;
SetMediaClockTimerMarshaller Marshaller::mSetMediaClockTimer;
SetMediaClockTimerResponseMarshaller Marshaller::mSetMediaClockTimerResponse;
ShowMarshaller Marshaller::mShow;
ShowResponseMarshaller Marshaller::mShowResponse;
