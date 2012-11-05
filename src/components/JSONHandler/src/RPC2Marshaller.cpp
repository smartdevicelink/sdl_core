#include "JSONHandler/OnButtonEvent.h"
#include <json/reader.h>
#include <json/writer.h>
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

const RPC2Marshaller::Methods RPC2Marshaller::getIndex(const std::string & s)
{
  if ( s.compare("Buttons.OnButtonEvent") == 0 )
    {
        return METHOD_ONBUTTONEVENT;
    }
    if ( s.compare("TTS.Speak") == 0 )
    {
        return METHOD_SPEAK_REQUEST;
    }
    if ( s.compare("UI.Alert") == 0 )
    {
        return METHOD_ALERT_REQUEST;
    }
    if ( s.compare("UI.Show") == 0 )
    {
        return METHOD_SHOW_REQUEST;
    }
    if ( s.compare("Buttons.GetCapabilities") == 0 )
    {
        return METHOD_GET_CAPABILITIES_REQUEST;
    }
    if ( s.compare("Buttons.OnButtonPress") == 0 )
    {
        return METHOD_ONBUTTONPRESS;
    }
    if ( s.compare("UI.SetGlobalProperties") == 0 )
    {
        return METHOD_SET_GLOBAL_PROPERTIES_REQUEST;
    }
    if ( s.compare("UI.ResetGlobalProperties") == 0 )
    {
        return METHOD_RESET_GLOBAL_PROPERTIES_REQUEST;
    }
    if ( s.compare("AppLinkCore.OnAppRegistered") == 0 )
    {
        return METHOD_ONAPPREGISTERED;
    }
    if ( s.compare("AppLinkCore.OnAppUnregistered") == 0 )
    {
        return METHOD_ONAPPUNREDISTERED;
    }
    if ( s.compare("AppLinkCore.activateApp") == 0 )
    {
      return METHOD_ACTIVATEAPP_REQUEST;
    }
    if ( s.compare("UI.AddCommand") == 0 )
    {
        return METHOD_ADDCOMMAND_REQUEST;
    }
    if ( s.compare("UI.DeleteCommand") == 0 )
    {
        return METHOD_DELETECOMMAND_REQUEST;
    }
    if ( s.compare("UI.OnCommand") == 0 )
    {
        return METHOD_UIONCOMMAND_NOTIFICATION;
    }
    if ( s.compare("UI.AddSubMenu") == 0 )
    {
        return METHOD_ADDSUBMENU_REQUEST;
    }
    if ( s.compare("UI.DeleteSubMenu") == 0 )
    {
        return METHOD_DELETESUBMENU_REQUEST;
    }
    if ( s.compare("UI.CreateInteractionChoiceSet") == 0 )
    {
        return METHOD_CREATEINTERACTIONCHOICESET_REQUEST;
    }
    if ( s.compare("UI.DeleteInteractionChoiceSet") == 0 )
    {
        return METHOD_DELETEINTERACTIONCHOICESET_REQUEST;
    }  
    if ( s.compare("UI.PerformInteraction") == 0 )  
    {
        return METHOD_PERFORMINTERACTION_REQUEST;
    }
    
    return METHOD_INVALID;
}

const RPC2Marshaller::Methods RPC2Marshaller::getResponseIndex(const std::string & s)
{
  if ( s.compare("TTS.Speak") == 0 )
  {
      return METHOD_SPEAK_RESPONSE;
  }
  if ( s.compare("UI.Alert") == 0 )
  {
      return METHOD_ALERT_RESPONSE;
  }
  if ( s.compare("UI.Show") == 0 )
  {
      return METHOD_SHOW_RESPONSE;
  }
  if ( s.compare("Buttons.GetCapabilities") == 0 )
  {
      return METHOD_GET_CAPABILITIES_RESPONSE;
  }
  if ( s.compare("UI.SetGlobalProperties") == 0 )
  {
      return METHOD_SET_GLOBAL_PROPERTIES_RESPONSE;
  }
  if ( s.compare("UI.ResetGlobalProperties") == 0 )
  {
      return METHOD_RESET_GLOBAL_PROPERTIES_RESPONSE;
  }
  if ( s.compare("AppLinkCore.activateApp"))
  {
    return METHOD_ACTIVATEAPP_RESPONSE;
  }
  if ( s.compare("UI.AddCommand") == 0 )
  {
      return METHOD_ADDCOMMAND_RESPONSE;
  }
  if ( s.compare("UI.DeleteCommand") == 0 )
  {
      return METHOD_DELETECOMMAND_RESPONSE;
  }
  if ( s.compare("UI.AddSubMenu") == 0 )
  {
      return METHOD_ADDSUBMENU_RESPONSE;
  }
  if ( s.compare("UI.DeleteSubMenu") == 0 )
  {
      return METHOD_DELETESUBMENU_RESPONSE;
  }
  if ( s.compare("UI.CreateInteractionChoiceSet") == 0 )
  {
      return METHOD_CREATEINTERACTIONCHOICESET_RESPONSE;
  }
  if ( s.compare("UI.DeleteInteractionChoiceSet") == 0 )
  {
      return METHOD_DELETEINTERACTIONCHOICESET_RESPONSE;
  }

  return METHOD_INVALID;
}


const RPC2Marshaller::localHash RPC2Marshaller::mHashTable[3]=
{
  {"OnButtonEvent",METHOD_ONBUTTONEVENT,&RPC2Marshaller::mOnButtonEventMarshaller}
};


RPC2Command* RPC2Marshaller::fromJSON(const Json::Value& json, const std::string & methodName)
{
  if(!json.isObject())  return NULL;
  
  Methods m;

  if(!json.isMember("method") || !json["method"].isString())
  {
    if ( methodName.empty() )
    {
        return NULL;
    }
    else 
    {
        m = getResponseIndex( methodName );     //For Responses
    }
  } 
  else
  {
    m = getIndex(json["method"].asString().c_str());
  }  
  
  switch(m)                         
  {
    case METHOD_INVALID:
      return NULL;

    case METHOD_ONBUTTONEVENT:
      {
         OnButtonEvent* rv=new OnButtonEvent;
         if(OnButtonEventMarshaller::fromJSON(json,*rv))
           return rv;
         delete rv;
         return NULL;
      }
    case METHOD_SPEAK_REQUEST:
    {
        Speak * rv = new Speak;
        if ( SpeakMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_SPEAK_RESPONSE:
    {
        SpeakResponse * rv = new SpeakResponse;
        if ( SpeakResponseMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ALERT_REQUEST:
    {
        Alert * rv = new Alert;
        if ( AlertMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ALERT_RESPONSE:
    {
        AlertResponse * rv = new AlertResponse;
        if ( AlertResponseMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_SHOW_REQUEST:
    {
        Show * rv = new Show; 
        if ( ShowMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_SHOW_RESPONSE:
    {
        ShowResponse * rv = new ShowResponse;
        if ( ShowResponseMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_GET_CAPABILITIES_REQUEST:
    {
        GetCapabilities * rv = new GetCapabilities;
        if ( GetCapabilitiesMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_GET_CAPABILITIES_RESPONSE:
    {
        GetCapabilitiesResponse * rv = new GetCapabilitiesResponse;
        if ( GetCapabilitiesResponseMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ONBUTTONPRESS:
    {
        OnButtonPress * rv = new OnButtonPress;
        if (OnButtonPressMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_SET_GLOBAL_PROPERTIES_REQUEST:
    {
        SetGlobalProperties * rv = new SetGlobalProperties;
        if (SetGlobalPropertiesMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_SET_GLOBAL_PROPERTIES_RESPONSE:
    {
        SetGlobalPropertiesResponse * rv = new SetGlobalPropertiesResponse;
        if ( SetGlobalPropertiesResponseMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_RESET_GLOBAL_PROPERTIES_REQUEST:
    {
        ResetGlobalProperties * rv = new ResetGlobalProperties;
        if ( ResetGlobalPropertiesMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_RESET_GLOBAL_PROPERTIES_RESPONSE:
    {
        ResetGlobalPropertiesResponse * rv = new ResetGlobalPropertiesResponse;
        if (ResetGlobalPropertiesResponseMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ONAPPREGISTERED:
    {
        OnAppRegistered * rv = new OnAppRegistered;
        if ( OnAppRegisteredMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ONAPPUNREDISTERED:
    {
        OnAppUnregistered * rv = new OnAppUnregistered;
        if ( OnAppUnregisteredMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ACTIVATEAPP_REQUEST:
    {
        ActivateApp * rv = new ActivateApp;
        if (ActivateAppMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ACTIVATEAPP_RESPONSE:
    {
        ActivateAppResponse * rv = new ActivateAppResponse;
        if (ActivateAppResponseMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ADDCOMMAND_REQUEST:
    {
        AddCommand * rv = new AddCommand;
        if (AddCommandMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ADDCOMMAND_RESPONSE:
    {
        AddCommandResponse * rv = new AddCommandResponse;
        if (AddCommandResponseMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_DELETECOMMAND_REQUEST:
    {
        DeleteCommand * rv = new DeleteCommand;
        if (DeleteCommandMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_DELETECOMMAND_RESPONSE:
    {
        DeleteCommandResponse * rv = new DeleteCommandResponse;
        if (DeleteCommandResponseMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_UIONCOMMAND_NOTIFICATION:
    {
        OnCommand * rv = new OnCommand;
        if (OnCommandMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ADDSUBMENU_REQUEST:
    {
        AddSubMenu * rv = new AddSubMenu;
        if (AddSubMenuMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ADDSUBMENU_RESPONSE:
    {
        AddSubMenuResponse * rv = new AddSubMenuResponse;
        if (AddSubMenuResponseMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_DELETESUBMENU_REQUEST:
    {
        DeleteSubMenu * rv = new DeleteSubMenu;
        if (DeleteSubMenuMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_DELETESUBMENU_RESPONSE:
    {
        DeleteSubMenuResponse * rv = new DeleteSubMenuResponse;
        if (DeleteSubMenuResponseMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_CREATEINTERACTIONCHOICESET_REQUEST:
    {
        CreateInteractionChoiceSet * rv = new CreateInteractionChoiceSet;
        if (CreateInteractionChoiceSetMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_CREATEINTERACTIONCHOICESET_RESPONSE:
    {
        CreateInteractionChoiceSetResponse * rv = new CreateInteractionChoiceSetResponse;
        if (CreateInteractionChoiceSetResponseMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_DELETEINTERACTIONCHOICESET_REQUEST:
    {
        DeleteInteractionChoiceSet * rv = new DeleteInteractionChoiceSet;
        if (DeleteInteractionChoiceSetMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_DELETEINTERACTIONCHOICESET_RESPONSE:
    {
        DeleteInteractionChoiceSetResponse *rv = new DeleteInteractionChoiceSetResponse;
        if (DeleteInteractionChoiceSetResponseMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_PERFORMINTERACTION_REQUEST:
    {
        PerformInteraction * rv = new PerformInteraction;
        if (PerformInteractionMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }

  }

  return NULL;
}

Json::Value RPC2Marshaller::toJSON(const RPC2Command* msg)
{
  Json::Value j=Json::Value(Json::nullValue);

  if(!msg) return j;
  Methods m=static_cast<Methods>(msg->getMethod());

  switch(m)
  {
    case METHOD_INVALID:
      return j;

    case METHOD_ONBUTTONEVENT:
      return OnButtonEventMarshaller::toJSON(* static_cast<const OnButtonEvent*>(msg));
    case METHOD_SPEAK_REQUEST:
      return SpeakMarshaller::toJSON(* static_cast<const Speak*>(msg));
    case METHOD_SPEAK_RESPONSE:
      return SpeakResponseMarshaller::toJSON(* static_cast<const SpeakResponse*>(msg));
    case METHOD_ALERT_REQUEST:
      return AlertMarshaller::toJSON(* static_cast<const Alert*>(msg)) ;
    case METHOD_ALERT_RESPONSE:
      return AlertResponseMarshaller::toJSON(* static_cast<const AlertResponse*>(msg));
    case METHOD_SHOW_REQUEST:
      return ShowMarshaller::toJSON(* static_cast<const Show*> (msg));
    case METHOD_SHOW_RESPONSE:
      return ShowResponseMarshaller::toJSON(* static_cast<const ShowResponse*>(msg));
    case METHOD_GET_CAPABILITIES_REQUEST:
      return GetCapabilitiesMarshaller::toJSON(* static_cast<const GetCapabilities*>(msg));
    case METHOD_GET_CAPABILITIES_RESPONSE:
      return GetCapabilitiesResponseMarshaller::toJSON(* static_cast<const GetCapabilitiesResponse*>(msg));
    case METHOD_ONBUTTONPRESS:
      return OnButtonPressMarshaller::toJSON(* static_cast<const OnButtonPress*>(msg));
    case METHOD_SET_GLOBAL_PROPERTIES_REQUEST:
      return SetGlobalPropertiesMarshaller::toJSON(* static_cast<const SetGlobalProperties*>(msg));
    case METHOD_SET_GLOBAL_PROPERTIES_RESPONSE:
      return SetGlobalPropertiesResponseMarshaller::toJSON(* static_cast<const SetGlobalPropertiesResponse*>(msg));
    case METHOD_RESET_GLOBAL_PROPERTIES_REQUEST:
      return ResetGlobalPropertiesMarshaller::toJSON(* static_cast<const ResetGlobalProperties*>(msg));
    case METHOD_RESET_GLOBAL_PROPERTIES_RESPONSE:
      return ResetGlobalPropertiesResponseMarshaller::toJSON(* static_cast<const ResetGlobalPropertiesResponse*>(msg));
    case METHOD_ONAPPREGISTERED:
      return OnAppRegisteredMarshaller::toJSON(* static_cast<const OnAppRegistered*>(msg));
    case METHOD_ONAPPUNREDISTERED:
      return OnAppUnregisteredMarshaller::toJSON(* static_cast<const OnAppUnregistered*>(msg));
    case METHOD_ACTIVATEAPP_REQUEST:
      return ActivateAppMarshaller::toJSON(* static_cast<const ActivateApp*>(msg));
    case METHOD_ACTIVATEAPP_RESPONSE:
      return ActivateAppResponseMarshaller::toJSON(*static_cast<const ActivateAppResponse*>(msg));
    case METHOD_ADDCOMMAND_REQUEST:
      return AddCommandMarshaller::toJSON(*static_cast<const AddCommand*>(msg));
    case METHOD_ADDCOMMAND_RESPONSE:
      return AddCommandResponseMarshaller::toJSON(*static_cast<const AddCommandResponse*>(msg));
    case METHOD_DELETECOMMAND_REQUEST:
      return DeleteCommandMarshaller::toJSON(*static_cast<const DeleteCommand*>(msg));
    case METHOD_DELETECOMMAND_RESPONSE:
      return DeleteCommandResponseMarshaller::toJSON(*static_cast<const DeleteCommandResponse*>(msg));
    case METHOD_UIONCOMMAND_NOTIFICATION:
      return OnCommandMarshaller::toJSON(*static_cast<const OnCommand*>(msg));
    case METHOD_ADDSUBMENU_REQUEST:
      return AddSubMenuMarshaller::toJSON(*static_cast<const AddSubMenu*>(msg));
    case METHOD_ADDSUBMENU_RESPONSE:
      return AddSubMenuResponseMarshaller::toJSON(*static_cast<const AddSubMenuResponse*>(msg));
    case METHOD_DELETESUBMENU_REQUEST:
      return DeleteSubMenuMarshaller::toJSON(*static_cast<const DeleteSubMenu*>(msg));
    case METHOD_DELETESUBMENU_RESPONSE:
      return DeleteSubMenuResponseMarshaller::toJSON(*static_cast<const DeleteSubMenuResponse*>(msg));
    case METHOD_CREATEINTERACTIONCHOICESET_REQUEST:
      return CreateInteractionChoiceSetMarshaller::toJSON(*static_cast<const CreateInteractionChoiceSet*>(msg));
    case METHOD_CREATEINTERACTIONCHOICESET_RESPONSE:
      return CreateInteractionChoiceSetResponseMarshaller::toJSON(*static_cast<const CreateInteractionChoiceSetResponse*> (msg));
    case METHOD_DELETEINTERACTIONCHOICESET_REQUEST:
      return DeleteInteractionChoiceSetMarshaller::toJSON(*static_cast<const DeleteInteractionChoiceSet*>(msg));
    case METHOD_DELETEINTERACTIONCHOICESET_RESPONSE:
      return DeleteInteractionChoiceSetResponseMarshaller::toJSON(*static_cast<const DeleteInteractionChoiceSetResponse*>(msg));
    case METHOD_PERFORMINTERACTION_REQUEST:
      return PerformInteractionMarshaller::toJSON(*static_cast<const PerformInteraction*>(msg));
  }

  return j;
}

RPC2Command* RPC2Marshaller::fromString(const std::string& s)
{
  RPC2Command* rv=0;
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return 0;
    if(!(rv=fromJSON(json)))  return 0;
  }
  catch(...)
  {
    return 0;
  }
  return rv;
}

std::string RPC2Marshaller::toString(const RPC2Command* msg)
{
  if(!msg)  return "";
  Json::Value json=toJSON(msg);
  if(json.isNull()) return "";

  Json::FastWriter writer;
  std::string rv;
  return writer.write(json);
}

OnButtonEventMarshaller RPC2Marshaller::mOnButtonEventMarshaller;
SpeakMarshaller RPC2Marshaller::mSpeakMarshaller;
SpeakResponseMarshaller RPC2Marshaller::mSpeakResponseMarshaller;
AlertMarshaller RPC2Marshaller::mAlertMarshaller;
AlertResponseMarshaller RPC2Marshaller::mAlertResponseMarshaller;
ShowMarshaller RPC2Marshaller::mShowMarshaller;
ShowResponseMarshaller RPC2Marshaller::mShowResponseMarshaller;
GetCapabilitiesMarshaller RPC2Marshaller::mGetCapabilitiesMarshaller;
GetCapabilitiesResponseMarshaller RPC2Marshaller::mGetCapabilitiesResponseMarshaller;
OnButtonPressMarshaller RPC2Marshaller::mOnButtonPressMarshaller;
SetGlobalPropertiesMarshaller RPC2Marshaller::mSetGlobalPropertiesMarshaller;
SetGlobalPropertiesResponseMarshaller RPC2Marshaller::mSetGlobalPropertiesResponseMarshaller;
ResetGlobalPropertiesMarshaller RPC2Marshaller::mResetGlobalPropertiesMarshaller;
ResetGlobalPropertiesResponseMarshaller RPC2Marshaller::mResetGlobalPropertiesResponseMarshaller;
OnAppRegisteredMarshaller RPC2Marshaller::mOnAppRegisteredMarshaller;
OnAppUnregisteredMarshaller RPC2Marshaller::mOnAppUnregisteredMarshaller;
ActivateAppMarshaller RPC2Marshaller::mActivateAppMarshaller;
AddCommandMarshaller RPC2Marshaller::mAddCommandMarshaller;
AddCommandResponseMarshaller RPC2Marshaller::mAddCommandResponseMarshaller;
DeleteCommandMarshaller RPC2Marshaller::mDeleteCommandMarshaller;
DeleteCommandResponseMarshaller RPC2Marshaller::mDeleteCommandResponseMarshaller;
OnCommandMarshaller RPC2Marshaller::mOnCommandMarshaller;
AddSubMenuMarshaller RPC2Marshaller::mAddSubMenuMarshaller;
AddSubMenuResponseMarshaller RPC2Marshaller::mAddSubMenuResponseMarshaller;
DeleteSubMenuMarshaller RPC2Marshaller::mDeleteSubMenuMarshaller;
DeleteSubMenuResponseMarshaller RPC2Marshaller::mDeleteSubMenuResponseMarshaller;
CreateInteractionChoiceSetMarshaller RPC2Marshaller::mCreateInteractionChoiceSetMarshaller;
CreateInteractionChoiceSetResponseMarshaller RPC2Marshaller::mCreateInteractionChoiceSetResponseMarshaller;
DeleteInteractionChoiceSetMarshaller RPC2Marshaller::mDeleteInteractionChoiceSetMarshaller;
DeleteInteractionChoiceSetResponseMarshaller RPC2Marshaller::mDeleteInteractionChoiceSetResponseMarshaller;
PerformInteractionMarshaller RPC2Marshaller::mPerformInteractionMarshaller;