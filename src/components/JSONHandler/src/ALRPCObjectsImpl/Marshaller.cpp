#include <cstring>

#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/RegisterAppInterface_request.h"
#include "../../include/JSONHandler/ALRPCObjects/RegisterAppInterface_response.h"
#include "../../include/JSONHandler/ALRPCObjects/UnregisterAppInterface_request.h"
#include "../../include/JSONHandler/ALRPCObjects/UnregisterAppInterface_response.h"
#include "../../include/JSONHandler/ALRPCObjects/SetGlobalProperties_request.h"
#include "../../include/JSONHandler/ALRPCObjects/SetGlobalProperties_response.h"
#include "../../include/JSONHandler/ALRPCObjects/ResetGlobalProperties_request.h"
#include "../../include/JSONHandler/ALRPCObjects/ResetGlobalProperties_response.h"
#include "../../include/JSONHandler/ALRPCObjects/AddCommand_request.h"
#include "../../include/JSONHandler/ALRPCObjects/AddCommand_response.h"
#include "../../include/JSONHandler/ALRPCObjects/DeleteCommand_request.h"
#include "../../include/JSONHandler/ALRPCObjects/DeleteCommand_response.h"
#include "../../include/JSONHandler/ALRPCObjects/AddSubMenu_request.h"
#include "../../include/JSONHandler/ALRPCObjects/AddSubMenu_response.h"
#include "../../include/JSONHandler/ALRPCObjects/DeleteSubMenu_request.h"
#include "../../include/JSONHandler/ALRPCObjects/DeleteSubMenu_response.h"
#include "../../include/JSONHandler/ALRPCObjects/CreateInteractionChoiceSet_request.h"
#include "../../include/JSONHandler/ALRPCObjects/CreateInteractionChoiceSet_response.h"
#include "../../include/JSONHandler/ALRPCObjects/PerformInteraction_request.h"
#include "../../include/JSONHandler/ALRPCObjects/PerformInteraction_response.h"
#include "../../include/JSONHandler/ALRPCObjects/DeleteInteractionChoiceSet_request.h"
#include "../../include/JSONHandler/ALRPCObjects/DeleteInteractionChoiceSet_response.h"
#include "../../include/JSONHandler/ALRPCObjects/Alert_request.h"
#include "../../include/JSONHandler/ALRPCObjects/Alert_response.h"
#include "../../include/JSONHandler/ALRPCObjects/Show_request.h"
#include "../../include/JSONHandler/ALRPCObjects/Show_response.h"
#include "../../include/JSONHandler/ALRPCObjects/Speak_request.h"
#include "../../include/JSONHandler/ALRPCObjects/Speak_response.h"
#include "../../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_request.h"
#include "../../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_response.h"
#include "../../include/JSONHandler/ALRPCObjects/EncodedSyncPData_request.h"
#include "../../include/JSONHandler/ALRPCObjects/EncodedSyncPData_response.h"
#include "../../include/JSONHandler/ALRPCObjects/SubscribeButton_request.h"
#include "../../include/JSONHandler/ALRPCObjects/SubscribeButton_response.h"
#include "../../include/JSONHandler/ALRPCObjects/UnsubscribeButton_request.h"
#include "../../include/JSONHandler/ALRPCObjects/UnsubscribeButton_response.h"
#include "../../include/JSONHandler/ALRPCObjects/GenericResponse_response.h"
#include "../../include/JSONHandler/ALRPCObjects/OnHMIStatus.h"
#include "../../include/JSONHandler/ALRPCObjects/OnAppInterfaceUnregistered.h"
#include "../../include/JSONHandler/ALRPCObjects/OnButtonEvent.h"
#include "../../include/JSONHandler/ALRPCObjects/OnButtonPress.h"
#include "../../include/JSONHandler/ALRPCObjects/OnCommand.h"
#include "../../include/JSONHandler/ALRPCObjects/OnEncodedSyncPData.h"
#include "../../include/JSONHandler/ALRPCObjects/OnTBTClientState.h"
#include "../../include/JSONHandler/ALRPCObjects/OnDriverDistraction.h"

#include "Marshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


const Marshaller::Methods Marshaller::getIndex(const char* s)
{
  if(!s)
    return METHOD_INVALID;
  const struct localHash* p=Marshaller_intHash::getPointer(s,strlen(s));
  return p ? static_cast<Methods>(p->idx) : METHOD_INVALID;
}


ALRPCMessage* Marshaller::fromString(const std::string& s)
{
  ALRPCMessage* rv=0;
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

std::string Marshaller::toString(const ALRPCMessage* msg)
{
  if(!msg)  return "";

  Json::Value json=toJSON(msg);

  if(json.isNull()) return "";

  Json::FastWriter writer;
  std::string rv;
  return writer.write(json);
}


ALRPCMessage* Marshaller::fromJSON(const Json::Value& json)
{
  if(!json.isObject())  return NULL;
  Json::Value j=Json::Value(Json::nullValue);

  std::string tp;
  if(json.isMember("notification")) j=json[tp="notification"];

  if(json.isMember("request"))  j=json[tp="request"];
  if(json.isMember("response"))  j=json[tp="response"];

  if(!j.isObject()) return NULL;
  if(!j.isMember("name") || !j["name"].isString()) return NULL;

  std::string ts=j["name"].asString()+"_"+tp;

  Methods m=getIndex(ts.c_str());
  switch(m)							// let's rock, china style
  {
    case METHOD_INVALID:
      return NULL;

    case METHOD_REGISTERAPPINTERFACE_REQUEST:
    {
      RegisterAppInterface_request* rv=new RegisterAppInterface_request;
      if(RegisterAppInterface_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_REGISTERAPPINTERFACE_RESPONSE:
    {
      RegisterAppInterface_response* rv=new RegisterAppInterface_response;
      if(RegisterAppInterface_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UNREGISTERAPPINTERFACE_REQUEST:
    {
      UnregisterAppInterface_request* rv=new UnregisterAppInterface_request;
      if(UnregisterAppInterface_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UNREGISTERAPPINTERFACE_RESPONSE:
    {
      UnregisterAppInterface_response* rv=new UnregisterAppInterface_response;
      if(UnregisterAppInterface_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETGLOBALPROPERTIES_REQUEST:
    {
      SetGlobalProperties_request* rv=new SetGlobalProperties_request;
      if(SetGlobalProperties_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETGLOBALPROPERTIES_RESPONSE:
    {
      SetGlobalProperties_response* rv=new SetGlobalProperties_response;
      if(SetGlobalProperties_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_RESETGLOBALPROPERTIES_REQUEST:
    {
      ResetGlobalProperties_request* rv=new ResetGlobalProperties_request;
      if(ResetGlobalProperties_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_RESETGLOBALPROPERTIES_RESPONSE:
    {
      ResetGlobalProperties_response* rv=new ResetGlobalProperties_response;
      if(ResetGlobalProperties_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ADDCOMMAND_REQUEST:
    {
      AddCommand_request* rv=new AddCommand_request;
      if(AddCommand_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ADDCOMMAND_RESPONSE:
    {
      AddCommand_response* rv=new AddCommand_response;
      if(AddCommand_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETECOMMAND_REQUEST:
    {
      DeleteCommand_request* rv=new DeleteCommand_request;
      if(DeleteCommand_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETECOMMAND_RESPONSE:
    {
      DeleteCommand_response* rv=new DeleteCommand_response;
      if(DeleteCommand_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ADDSUBMENU_REQUEST:
    {
      AddSubMenu_request* rv=new AddSubMenu_request;
      if(AddSubMenu_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ADDSUBMENU_RESPONSE:
    {
      AddSubMenu_response* rv=new AddSubMenu_response;
      if(AddSubMenu_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETESUBMENU_REQUEST:
    {
      DeleteSubMenu_request* rv=new DeleteSubMenu_request;
      if(DeleteSubMenu_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETESUBMENU_RESPONSE:
    {
      DeleteSubMenu_response* rv=new DeleteSubMenu_response;
      if(DeleteSubMenu_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_CREATEINTERACTIONCHOICESET_REQUEST:
    {
      CreateInteractionChoiceSet_request* rv=new CreateInteractionChoiceSet_request;
      if(CreateInteractionChoiceSet_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_CREATEINTERACTIONCHOICESET_RESPONSE:
    {
      CreateInteractionChoiceSet_response* rv=new CreateInteractionChoiceSet_response;
      if(CreateInteractionChoiceSet_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_PERFORMINTERACTION_REQUEST:
    {
      PerformInteraction_request* rv=new PerformInteraction_request;
      if(PerformInteraction_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_PERFORMINTERACTION_RESPONSE:
    {
      PerformInteraction_response* rv=new PerformInteraction_response;
      if(PerformInteraction_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETEINTERACTIONCHOICESET_REQUEST:
    {
      DeleteInteractionChoiceSet_request* rv=new DeleteInteractionChoiceSet_request;
      if(DeleteInteractionChoiceSet_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETEINTERACTIONCHOICESET_RESPONSE:
    {
      DeleteInteractionChoiceSet_response* rv=new DeleteInteractionChoiceSet_response;
      if(DeleteInteractionChoiceSet_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ALERT_REQUEST:
    {
      Alert_request* rv=new Alert_request;
      if(Alert_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ALERT_RESPONSE:
    {
      Alert_response* rv=new Alert_response;
      if(Alert_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SHOW_REQUEST:
    {
      Show_request* rv=new Show_request;
      if(Show_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SHOW_RESPONSE:
    {
      Show_response* rv=new Show_response;
      if(Show_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SPEAK_REQUEST:
    {
      Speak_request* rv=new Speak_request;
      if(Speak_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SPEAK_RESPONSE:
    {
      Speak_response* rv=new Speak_response;
      if(Speak_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETMEDIACLOCKTIMER_REQUEST:
    {
      SetMediaClockTimer_request* rv=new SetMediaClockTimer_request;
      if(SetMediaClockTimer_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETMEDIACLOCKTIMER_RESPONSE:
    {
      SetMediaClockTimer_response* rv=new SetMediaClockTimer_response;
      if(SetMediaClockTimer_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ENCODEDSYNCPDATA_REQUEST:
    {
      EncodedSyncPData_request* rv=new EncodedSyncPData_request;
      if(EncodedSyncPData_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ENCODEDSYNCPDATA_RESPONSE:
    {
      EncodedSyncPData_response* rv=new EncodedSyncPData_response;
      if(EncodedSyncPData_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SUBSCRIBEBUTTON_REQUEST:
    {
      SubscribeButton_request* rv=new SubscribeButton_request;
      if(SubscribeButton_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SUBSCRIBEBUTTON_RESPONSE:
    {
      SubscribeButton_response* rv=new SubscribeButton_response;
      if(SubscribeButton_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UNSUBSCRIBEBUTTON_REQUEST:
    {
      UnsubscribeButton_request* rv=new UnsubscribeButton_request;
      if(UnsubscribeButton_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UNSUBSCRIBEBUTTON_RESPONSE:
    {
      UnsubscribeButton_response* rv=new UnsubscribeButton_response;
      if(UnsubscribeButton_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_GENERICRESPONSE_RESPONSE:
    {
      GenericResponse_response* rv=new GenericResponse_response;
      if(GenericResponse_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONHMISTATUS:
    {
      OnHMIStatus* rv=new OnHMIStatus;
      if(OnHMIStatusMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONAPPINTERFACEUNREGISTERED:
    {
      OnAppInterfaceUnregistered* rv=new OnAppInterfaceUnregistered;
      if(OnAppInterfaceUnregisteredMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONBUTTONEVENT:
    {
      OnButtonEvent* rv=new OnButtonEvent;
      if(OnButtonEventMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONBUTTONPRESS:
    {
      OnButtonPress* rv=new OnButtonPress;
      if(OnButtonPressMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONCOMMAND:
    {
      OnCommand* rv=new OnCommand;
      if(OnCommandMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONENCODEDSYNCPDATA:
    {
      OnEncodedSyncPData* rv=new OnEncodedSyncPData;
      if(OnEncodedSyncPDataMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONTBTCLIENTSTATE:
    {
      OnTBTClientState* rv=new OnTBTClientState;
      if(OnTBTClientStateMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONDRIVERDISTRACTION:
    {
      OnDriverDistraction* rv=new OnDriverDistraction;
      if(OnDriverDistractionMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }
  }

  return NULL;
}

Json::Value Marshaller::toJSON(const ALRPCMessage* msg)
{
  Json::Value j=Json::Value(Json::nullValue);

  if(!msg) return j;
  Methods m=static_cast<Methods>(msg->getMethodId());

  switch(m)
  {
    case METHOD_REGISTERAPPINTERFACE_REQUEST:
         return RegisterAppInterface_requestMarshaller::toJSON(* static_cast<const RegisterAppInterface_request*>(msg));

    case METHOD_REGISTERAPPINTERFACE_RESPONSE:
         return RegisterAppInterface_responseMarshaller::toJSON(* static_cast<const RegisterAppInterface_response*>(msg));

    case METHOD_UNREGISTERAPPINTERFACE_REQUEST:
         return UnregisterAppInterface_requestMarshaller::toJSON(* static_cast<const UnregisterAppInterface_request*>(msg));

    case METHOD_UNREGISTERAPPINTERFACE_RESPONSE:
         return UnregisterAppInterface_responseMarshaller::toJSON(* static_cast<const UnregisterAppInterface_response*>(msg));

    case METHOD_SETGLOBALPROPERTIES_REQUEST:
         return SetGlobalProperties_requestMarshaller::toJSON(* static_cast<const SetGlobalProperties_request*>(msg));

    case METHOD_SETGLOBALPROPERTIES_RESPONSE:
         return SetGlobalProperties_responseMarshaller::toJSON(* static_cast<const SetGlobalProperties_response*>(msg));

    case METHOD_RESETGLOBALPROPERTIES_REQUEST:
         return ResetGlobalProperties_requestMarshaller::toJSON(* static_cast<const ResetGlobalProperties_request*>(msg));

    case METHOD_RESETGLOBALPROPERTIES_RESPONSE:
         return ResetGlobalProperties_responseMarshaller::toJSON(* static_cast<const ResetGlobalProperties_response*>(msg));

    case METHOD_ADDCOMMAND_REQUEST:
         return AddCommand_requestMarshaller::toJSON(* static_cast<const AddCommand_request*>(msg));

    case METHOD_ADDCOMMAND_RESPONSE:
         return AddCommand_responseMarshaller::toJSON(* static_cast<const AddCommand_response*>(msg));

    case METHOD_DELETECOMMAND_REQUEST:
         return DeleteCommand_requestMarshaller::toJSON(* static_cast<const DeleteCommand_request*>(msg));

    case METHOD_DELETECOMMAND_RESPONSE:
         return DeleteCommand_responseMarshaller::toJSON(* static_cast<const DeleteCommand_response*>(msg));

    case METHOD_ADDSUBMENU_REQUEST:
         return AddSubMenu_requestMarshaller::toJSON(* static_cast<const AddSubMenu_request*>(msg));

    case METHOD_ADDSUBMENU_RESPONSE:
         return AddSubMenu_responseMarshaller::toJSON(* static_cast<const AddSubMenu_response*>(msg));

    case METHOD_DELETESUBMENU_REQUEST:
         return DeleteSubMenu_requestMarshaller::toJSON(* static_cast<const DeleteSubMenu_request*>(msg));

    case METHOD_DELETESUBMENU_RESPONSE:
         return DeleteSubMenu_responseMarshaller::toJSON(* static_cast<const DeleteSubMenu_response*>(msg));

    case METHOD_CREATEINTERACTIONCHOICESET_REQUEST:
         return CreateInteractionChoiceSet_requestMarshaller::toJSON(* static_cast<const CreateInteractionChoiceSet_request*>(msg));

    case METHOD_CREATEINTERACTIONCHOICESET_RESPONSE:
         return CreateInteractionChoiceSet_responseMarshaller::toJSON(* static_cast<const CreateInteractionChoiceSet_response*>(msg));

    case METHOD_PERFORMINTERACTION_REQUEST:
         return PerformInteraction_requestMarshaller::toJSON(* static_cast<const PerformInteraction_request*>(msg));

    case METHOD_PERFORMINTERACTION_RESPONSE:
         return PerformInteraction_responseMarshaller::toJSON(* static_cast<const PerformInteraction_response*>(msg));

    case METHOD_DELETEINTERACTIONCHOICESET_REQUEST:
         return DeleteInteractionChoiceSet_requestMarshaller::toJSON(* static_cast<const DeleteInteractionChoiceSet_request*>(msg));

    case METHOD_DELETEINTERACTIONCHOICESET_RESPONSE:
         return DeleteInteractionChoiceSet_responseMarshaller::toJSON(* static_cast<const DeleteInteractionChoiceSet_response*>(msg));

    case METHOD_ALERT_REQUEST:
         return Alert_requestMarshaller::toJSON(* static_cast<const Alert_request*>(msg));

    case METHOD_ALERT_RESPONSE:
         return Alert_responseMarshaller::toJSON(* static_cast<const Alert_response*>(msg));

    case METHOD_SHOW_REQUEST:
         return Show_requestMarshaller::toJSON(* static_cast<const Show_request*>(msg));

    case METHOD_SHOW_RESPONSE:
         return Show_responseMarshaller::toJSON(* static_cast<const Show_response*>(msg));

    case METHOD_SPEAK_REQUEST:
         return Speak_requestMarshaller::toJSON(* static_cast<const Speak_request*>(msg));

    case METHOD_SPEAK_RESPONSE:
         return Speak_responseMarshaller::toJSON(* static_cast<const Speak_response*>(msg));

    case METHOD_SETMEDIACLOCKTIMER_REQUEST:
         return SetMediaClockTimer_requestMarshaller::toJSON(* static_cast<const SetMediaClockTimer_request*>(msg));

    case METHOD_SETMEDIACLOCKTIMER_RESPONSE:
         return SetMediaClockTimer_responseMarshaller::toJSON(* static_cast<const SetMediaClockTimer_response*>(msg));

    case METHOD_ENCODEDSYNCPDATA_REQUEST:
         return EncodedSyncPData_requestMarshaller::toJSON(* static_cast<const EncodedSyncPData_request*>(msg));

    case METHOD_ENCODEDSYNCPDATA_RESPONSE:
         return EncodedSyncPData_responseMarshaller::toJSON(* static_cast<const EncodedSyncPData_response*>(msg));

    case METHOD_SUBSCRIBEBUTTON_REQUEST:
         return SubscribeButton_requestMarshaller::toJSON(* static_cast<const SubscribeButton_request*>(msg));

    case METHOD_SUBSCRIBEBUTTON_RESPONSE:
         return SubscribeButton_responseMarshaller::toJSON(* static_cast<const SubscribeButton_response*>(msg));

    case METHOD_UNSUBSCRIBEBUTTON_REQUEST:
         return UnsubscribeButton_requestMarshaller::toJSON(* static_cast<const UnsubscribeButton_request*>(msg));

    case METHOD_UNSUBSCRIBEBUTTON_RESPONSE:
         return UnsubscribeButton_responseMarshaller::toJSON(* static_cast<const UnsubscribeButton_response*>(msg));

    case METHOD_GENERICRESPONSE_RESPONSE:
         return GenericResponse_responseMarshaller::toJSON(* static_cast<const GenericResponse_response*>(msg));

    case METHOD_ONHMISTATUS:
         return OnHMIStatusMarshaller::toJSON(* static_cast<const OnHMIStatus*>(msg));

    case METHOD_ONAPPINTERFACEUNREGISTERED:
         return OnAppInterfaceUnregisteredMarshaller::toJSON(* static_cast<const OnAppInterfaceUnregistered*>(msg));

    case METHOD_ONBUTTONEVENT:
         return OnButtonEventMarshaller::toJSON(* static_cast<const OnButtonEvent*>(msg));

    case METHOD_ONBUTTONPRESS:
         return OnButtonPressMarshaller::toJSON(* static_cast<const OnButtonPress*>(msg));

    case METHOD_ONCOMMAND:
         return OnCommandMarshaller::toJSON(* static_cast<const OnCommand*>(msg));

    case METHOD_ONENCODEDSYNCPDATA:
         return OnEncodedSyncPDataMarshaller::toJSON(* static_cast<const OnEncodedSyncPData*>(msg));

    case METHOD_ONTBTCLIENTSTATE:
         return OnTBTClientStateMarshaller::toJSON(* static_cast<const OnTBTClientState*>(msg));

    case METHOD_ONDRIVERDISTRACTION:
         return OnDriverDistractionMarshaller::toJSON(* static_cast<const OnDriverDistraction*>(msg));

    case METHOD_INVALID:
    default:
      return j;
  }

  return j;
}

RegisterAppInterface_requestMarshaller Marshaller::mRegisterAppInterface_request;
RegisterAppInterface_responseMarshaller Marshaller::mRegisterAppInterface_response;
UnregisterAppInterface_requestMarshaller Marshaller::mUnregisterAppInterface_request;
UnregisterAppInterface_responseMarshaller Marshaller::mUnregisterAppInterface_response;
SetGlobalProperties_requestMarshaller Marshaller::mSetGlobalProperties_request;
SetGlobalProperties_responseMarshaller Marshaller::mSetGlobalProperties_response;
ResetGlobalProperties_requestMarshaller Marshaller::mResetGlobalProperties_request;
ResetGlobalProperties_responseMarshaller Marshaller::mResetGlobalProperties_response;
AddCommand_requestMarshaller Marshaller::mAddCommand_request;
AddCommand_responseMarshaller Marshaller::mAddCommand_response;
DeleteCommand_requestMarshaller Marshaller::mDeleteCommand_request;
DeleteCommand_responseMarshaller Marshaller::mDeleteCommand_response;
AddSubMenu_requestMarshaller Marshaller::mAddSubMenu_request;
AddSubMenu_responseMarshaller Marshaller::mAddSubMenu_response;
DeleteSubMenu_requestMarshaller Marshaller::mDeleteSubMenu_request;
DeleteSubMenu_responseMarshaller Marshaller::mDeleteSubMenu_response;
CreateInteractionChoiceSet_requestMarshaller Marshaller::mCreateInteractionChoiceSet_request;
CreateInteractionChoiceSet_responseMarshaller Marshaller::mCreateInteractionChoiceSet_response;
PerformInteraction_requestMarshaller Marshaller::mPerformInteraction_request;
PerformInteraction_responseMarshaller Marshaller::mPerformInteraction_response;
DeleteInteractionChoiceSet_requestMarshaller Marshaller::mDeleteInteractionChoiceSet_request;
DeleteInteractionChoiceSet_responseMarshaller Marshaller::mDeleteInteractionChoiceSet_response;
Alert_requestMarshaller Marshaller::mAlert_request;
Alert_responseMarshaller Marshaller::mAlert_response;
Show_requestMarshaller Marshaller::mShow_request;
Show_responseMarshaller Marshaller::mShow_response;
Speak_requestMarshaller Marshaller::mSpeak_request;
Speak_responseMarshaller Marshaller::mSpeak_response;
SetMediaClockTimer_requestMarshaller Marshaller::mSetMediaClockTimer_request;
SetMediaClockTimer_responseMarshaller Marshaller::mSetMediaClockTimer_response;
EncodedSyncPData_requestMarshaller Marshaller::mEncodedSyncPData_request;
EncodedSyncPData_responseMarshaller Marshaller::mEncodedSyncPData_response;
SubscribeButton_requestMarshaller Marshaller::mSubscribeButton_request;
SubscribeButton_responseMarshaller Marshaller::mSubscribeButton_response;
UnsubscribeButton_requestMarshaller Marshaller::mUnsubscribeButton_request;
UnsubscribeButton_responseMarshaller Marshaller::mUnsubscribeButton_response;
GenericResponse_responseMarshaller Marshaller::mGenericResponse_response;
OnHMIStatusMarshaller Marshaller::mOnHMIStatus;
OnAppInterfaceUnregisteredMarshaller Marshaller::mOnAppInterfaceUnregistered;
OnButtonEventMarshaller Marshaller::mOnButtonEvent;
OnButtonPressMarshaller Marshaller::mOnButtonPress;
OnCommandMarshaller Marshaller::mOnCommand;
OnEncodedSyncPDataMarshaller Marshaller::mOnEncodedSyncPData;
OnTBTClientStateMarshaller Marshaller::mOnTBTClientState;
OnDriverDistractionMarshaller Marshaller::mOnDriverDistraction;

const Marshaller::localHash Marshaller::mHashTable[45]=
{
  {"RegisterAppInterface_request",METHOD_REGISTERAPPINTERFACE_REQUEST,&Marshaller::mRegisterAppInterface_request},
  {"RegisterAppInterface_response",METHOD_REGISTERAPPINTERFACE_RESPONSE,&Marshaller::mRegisterAppInterface_response},
  {"UnregisterAppInterface_request",METHOD_UNREGISTERAPPINTERFACE_REQUEST,&Marshaller::mUnregisterAppInterface_request},
  {"UnregisterAppInterface_response",METHOD_UNREGISTERAPPINTERFACE_RESPONSE,&Marshaller::mUnregisterAppInterface_response},
  {"SetGlobalProperties_request",METHOD_SETGLOBALPROPERTIES_REQUEST,&Marshaller::mSetGlobalProperties_request},
  {"SetGlobalProperties_response",METHOD_SETGLOBALPROPERTIES_RESPONSE,&Marshaller::mSetGlobalProperties_response},
  {"ResetGlobalProperties_request",METHOD_RESETGLOBALPROPERTIES_REQUEST,&Marshaller::mResetGlobalProperties_request},
  {"ResetGlobalProperties_response",METHOD_RESETGLOBALPROPERTIES_RESPONSE,&Marshaller::mResetGlobalProperties_response},
  {"AddCommand_request",METHOD_ADDCOMMAND_REQUEST,&Marshaller::mAddCommand_request},
  {"AddCommand_response",METHOD_ADDCOMMAND_RESPONSE,&Marshaller::mAddCommand_response},
  {"DeleteCommand_request",METHOD_DELETECOMMAND_REQUEST,&Marshaller::mDeleteCommand_request},
  {"DeleteCommand_response",METHOD_DELETECOMMAND_RESPONSE,&Marshaller::mDeleteCommand_response},
  {"AddSubMenu_request",METHOD_ADDSUBMENU_REQUEST,&Marshaller::mAddSubMenu_request},
  {"AddSubMenu_response",METHOD_ADDSUBMENU_RESPONSE,&Marshaller::mAddSubMenu_response},
  {"DeleteSubMenu_request",METHOD_DELETESUBMENU_REQUEST,&Marshaller::mDeleteSubMenu_request},
  {"DeleteSubMenu_response",METHOD_DELETESUBMENU_RESPONSE,&Marshaller::mDeleteSubMenu_response},
  {"CreateInteractionChoiceSet_request",METHOD_CREATEINTERACTIONCHOICESET_REQUEST,&Marshaller::mCreateInteractionChoiceSet_request},
  {"CreateInteractionChoiceSet_response",METHOD_CREATEINTERACTIONCHOICESET_RESPONSE,&Marshaller::mCreateInteractionChoiceSet_response},
  {"PerformInteraction_request",METHOD_PERFORMINTERACTION_REQUEST,&Marshaller::mPerformInteraction_request},
  {"PerformInteraction_response",METHOD_PERFORMINTERACTION_RESPONSE,&Marshaller::mPerformInteraction_response},
  {"DeleteInteractionChoiceSet_request",METHOD_DELETEINTERACTIONCHOICESET_REQUEST,&Marshaller::mDeleteInteractionChoiceSet_request},
  {"DeleteInteractionChoiceSet_response",METHOD_DELETEINTERACTIONCHOICESET_RESPONSE,&Marshaller::mDeleteInteractionChoiceSet_response},
  {"Alert_request",METHOD_ALERT_REQUEST,&Marshaller::mAlert_request},
  {"Alert_response",METHOD_ALERT_RESPONSE,&Marshaller::mAlert_response},
  {"Show_request",METHOD_SHOW_REQUEST,&Marshaller::mShow_request},
  {"Show_response",METHOD_SHOW_RESPONSE,&Marshaller::mShow_response},
  {"Speak_request",METHOD_SPEAK_REQUEST,&Marshaller::mSpeak_request},
  {"Speak_response",METHOD_SPEAK_RESPONSE,&Marshaller::mSpeak_response},
  {"SetMediaClockTimer_request",METHOD_SETMEDIACLOCKTIMER_REQUEST,&Marshaller::mSetMediaClockTimer_request},
  {"SetMediaClockTimer_response",METHOD_SETMEDIACLOCKTIMER_RESPONSE,&Marshaller::mSetMediaClockTimer_response},
  {"EncodedSyncPData_request",METHOD_ENCODEDSYNCPDATA_REQUEST,&Marshaller::mEncodedSyncPData_request},
  {"EncodedSyncPData_response",METHOD_ENCODEDSYNCPDATA_RESPONSE,&Marshaller::mEncodedSyncPData_response},
  {"SubscribeButton_request",METHOD_SUBSCRIBEBUTTON_REQUEST,&Marshaller::mSubscribeButton_request},
  {"SubscribeButton_response",METHOD_SUBSCRIBEBUTTON_RESPONSE,&Marshaller::mSubscribeButton_response},
  {"UnsubscribeButton_request",METHOD_UNSUBSCRIBEBUTTON_REQUEST,&Marshaller::mUnsubscribeButton_request},
  {"UnsubscribeButton_response",METHOD_UNSUBSCRIBEBUTTON_RESPONSE,&Marshaller::mUnsubscribeButton_response},
  {"GenericResponse_response",METHOD_GENERICRESPONSE_RESPONSE,&Marshaller::mGenericResponse_response},
  {"OnHMIStatus_notification",METHOD_ONHMISTATUS,&Marshaller::mOnHMIStatus},
  {"OnAppInterfaceUnregistered_notification",METHOD_ONAPPINTERFACEUNREGISTERED,&Marshaller::mOnAppInterfaceUnregistered},
  {"OnButtonEvent_notification",METHOD_ONBUTTONEVENT,&Marshaller::mOnButtonEvent},
  {"OnButtonPress_notification",METHOD_ONBUTTONPRESS,&Marshaller::mOnButtonPress},
  {"OnCommand_notification",METHOD_ONCOMMAND,&Marshaller::mOnCommand},
  {"OnEncodedSyncPData_notification",METHOD_ONENCODEDSYNCPDATA,&Marshaller::mOnEncodedSyncPData},
  {"OnTBTClientState_notification",METHOD_ONTBTCLIENTSTATE,&Marshaller::mOnTBTClientState},
  {"OnDriverDistraction_notification",METHOD_ONDRIVERDISTRACTION,&Marshaller::mOnDriverDistraction}
};
