#include <cstring>

#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "../include/JSONHandler/ALRPCObjects/RegisterAppInterface_request.h"
#include "../include/JSONHandler/ALRPCObjects/RegisterAppInterface_response.h"
#include "../include/JSONHandler/ALRPCObjects/UnregisterAppInterface_request.h"
#include "../include/JSONHandler/ALRPCObjects/UnregisterAppInterface_response.h"
#include "../include/JSONHandler/ALRPCObjects/SetGlobalProperties_request.h"
#include "../include/JSONHandler/ALRPCObjects/SetGlobalProperties_response.h"
#include "../include/JSONHandler/ALRPCObjects/ResetGlobalProperties_request.h"
#include "../include/JSONHandler/ALRPCObjects/ResetGlobalProperties_response.h"
#include "../include/JSONHandler/ALRPCObjects/AddCommand_request.h"
#include "../include/JSONHandler/ALRPCObjects/AddCommand_response.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteCommand_request.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteCommand_response.h"
#include "../include/JSONHandler/ALRPCObjects/AddSubMenu_request.h"
#include "../include/JSONHandler/ALRPCObjects/AddSubMenu_response.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteSubMenu_request.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteSubMenu_response.h"
#include "../include/JSONHandler/ALRPCObjects/CreateInteractionChoiceSet_request.h"
#include "../include/JSONHandler/ALRPCObjects/CreateInteractionChoiceSet_response.h"
#include "../include/JSONHandler/ALRPCObjects/PerformInteraction_request.h"
#include "../include/JSONHandler/ALRPCObjects/PerformInteraction_response.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteInteractionChoiceSet_request.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteInteractionChoiceSet_response.h"
#include "../include/JSONHandler/ALRPCObjects/Alert_request.h"
#include "../include/JSONHandler/ALRPCObjects/Alert_response.h"
#include "../include/JSONHandler/ALRPCObjects/Show_request.h"
#include "../include/JSONHandler/ALRPCObjects/Show_response.h"
#include "../include/JSONHandler/ALRPCObjects/Speak_request.h"
#include "../include/JSONHandler/ALRPCObjects/Speak_response.h"
#include "../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_request.h"
#include "../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_response.h"
#include "../include/JSONHandler/ALRPCObjects/EncodedSyncPData_request.h"
#include "../include/JSONHandler/ALRPCObjects/EncodedSyncPData_response.h"
#include "../include/JSONHandler/ALRPCObjects/SubscribeButton_request.h"
#include "../include/JSONHandler/ALRPCObjects/SubscribeButton_response.h"
#include "../include/JSONHandler/ALRPCObjects/UnsubscribeButton_request.h"
#include "../include/JSONHandler/ALRPCObjects/UnsubscribeButton_response.h"
#include "../include/JSONHandler/ALRPCObjects/GenericResponse_response.h"
#include "../include/JSONHandler/ALRPCObjects/OnHMIStatus.h"
#include "../include/JSONHandler/ALRPCObjects/OnAppInterfaceUnregistered.h"
#include "../include/JSONHandler/ALRPCObjects/OnButtonEvent.h"
#include "../include/JSONHandler/ALRPCObjects/OnButtonPress.h"
#include "../include/JSONHandler/ALRPCObjects/OnCommand.h"
#include "../include/JSONHandler/ALRPCObjects/OnEncodedSyncPData.h"
#include "../include/JSONHandler/ALRPCObjects/OnTBTClientState.h"
#include "../include/JSONHandler/ALRPCObjects/OnDriverDistraction.h"
#include "../include/JSONHandler/ALRPCObjects/AddCommand_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/AddCommand_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/AddSubMenu_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/AddSubMenu_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/Alert_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/Alert_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/AlertManeuver_request.h"
#include "../include/JSONHandler/ALRPCObjects/AlertManeuver_response.h"
#include "../include/JSONHandler/ALRPCObjects/ChangeLanguageRegistration_request.h"
#include "../include/JSONHandler/ALRPCObjects/ChangeLanguageRegistration_response.h"
#include "../include/JSONHandler/ALRPCObjects/CreateInteractionChoiceSet_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/CreateInteractionChoiceSet_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteCommand_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteCommand_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteFile_request.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteFile_response.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteInteractionChoiceSet_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteInteractionChoiceSet_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteSubMenu_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/DeleteSubMenu_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/DialNumber_request.h"
#include "../include/JSONHandler/ALRPCObjects/DialNumber_response.h"
#include "../include/JSONHandler/ALRPCObjects/EncodedSyncPData_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/EncodedSyncPData_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/EndAudioPassThru_request.h"
#include "../include/JSONHandler/ALRPCObjects/EndAudioPassThru_response.h"
#include "../include/JSONHandler/ALRPCObjects/GenericResponse_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/GetDTCs_request.h"
#include "../include/JSONHandler/ALRPCObjects/GetDTCs_response.h"
#include "../include/JSONHandler/ALRPCObjects/GetVehicleData_request.h"
#include "../include/JSONHandler/ALRPCObjects/GetVehicleData_response.h"
#include "../include/JSONHandler/ALRPCObjects/ListFiles_request.h"
#include "../include/JSONHandler/ALRPCObjects/ListFiles_response.h"
#include "../include/JSONHandler/ALRPCObjects/OnAppInterfaceUnregistered_v2.h"
#include "../include/JSONHandler/ALRPCObjects/OnAudioPassThru.h"
#include "../include/JSONHandler/ALRPCObjects/OnButtonEvent_v2.h"
#include "../include/JSONHandler/ALRPCObjects/OnButtonPress_v2.h"
#include "../include/JSONHandler/ALRPCObjects/OnEncodedSyncPData_v2.h"
#include "../include/JSONHandler/ALRPCObjects/OnHMIStatus_v2.h"
#include "../include/JSONHandler/ALRPCObjects/OnLanguageChange.h"
#include "../include/JSONHandler/ALRPCObjects/OnPermissionsChange.h"
#include "../include/JSONHandler/ALRPCObjects/OnTBTClientState_v2.h"
#include "../include/JSONHandler/ALRPCObjects/OnVehicleData.h"
#include "../include/JSONHandler/ALRPCObjects/PerformAudioPassThru_request.h"
#include "../include/JSONHandler/ALRPCObjects/PerformAudioPassThru_response.h"
#include "../include/JSONHandler/ALRPCObjects/PerformInteraction_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/PerformInteraction_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/PutFile_request.h"
#include "../include/JSONHandler/ALRPCObjects/PutFile_response.h"
#include "../include/JSONHandler/ALRPCObjects/ReadDID_request.h"
#include "../include/JSONHandler/ALRPCObjects/ReadDID_response.h"
#include "../include/JSONHandler/ALRPCObjects/RegisterAppInterface_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/RegisterAppInterface_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/ResetGlobalProperties_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/ResetGlobalProperties_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/ScrollableMessage_request.h"
#include "../include/JSONHandler/ALRPCObjects/ScrollableMessage_response.h"
#include "../include/JSONHandler/ALRPCObjects/SetAppIcon_request.h"
#include "../include/JSONHandler/ALRPCObjects/SetAppIcon_response.h"
#include "../include/JSONHandler/ALRPCObjects/SetDisplayLayout_request.h"
#include "../include/JSONHandler/ALRPCObjects/SetDisplayLayout_response.h"
#include "../include/JSONHandler/ALRPCObjects/SetGlobalProperties_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/SetGlobalProperties_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/Show_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/Show_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/ShowConstantTBT_request.h"
#include "../include/JSONHandler/ALRPCObjects/ShowConstantTBT_response.h"
#include "../include/JSONHandler/ALRPCObjects/Slider_request.h"
#include "../include/JSONHandler/ALRPCObjects/Slider_response.h"
#include "../include/JSONHandler/ALRPCObjects/Speak_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/Speak_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/SubscribeButton_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/SubscribeButton_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/SubscribeVehicleData_request.h"
#include "../include/JSONHandler/ALRPCObjects/SubscribeVehicleData_response.h"
#include "../include/JSONHandler/ALRPCObjects/UnregisterAppInterface_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/UnregisterAppInterface_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/UnsubscribeButton_v2_request.h"
#include "../include/JSONHandler/ALRPCObjects/UnsubscribeButton_v2_response.h"
#include "../include/JSONHandler/ALRPCObjects/UnsubscribeVehicleData_request.h"
#include "../include/JSONHandler/ALRPCObjects/UnsubscribeVehicleData_response.h"
#include "../include/JSONHandler/ALRPCObjects/UpdateTurnList_request.h"
#include "../include/JSONHandler/ALRPCObjects/UpdateTurnList_response.h"
namespace NsAppLinkRPC
{
#include "Marshaller.inc"
}


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


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

    case METHOD_ADDCOMMAND_V2_REQUEST:
    {
      AddCommand_v2_request* rv=new AddCommand_v2_request;
      if(AddCommand_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ADDCOMMAND_V2_RESPONSE:
    {
      AddCommand_v2_response* rv=new AddCommand_v2_response;
      if(AddCommand_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ADDSUBMENU_V2_REQUEST:
    {
      AddSubMenu_v2_request* rv=new AddSubMenu_v2_request;
      if(AddSubMenu_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ADDSUBMENU_V2_RESPONSE:
    {
      AddSubMenu_v2_response* rv=new AddSubMenu_v2_response;
      if(AddSubMenu_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ALERT_V2_REQUEST:
    {
      Alert_v2_request* rv=new Alert_v2_request;
      if(Alert_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ALERT_V2_RESPONSE:
    {
      Alert_v2_response* rv=new Alert_v2_response;
      if(Alert_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ALERTMANEUVER_REQUEST:
    {
      AlertManeuver_request* rv=new AlertManeuver_request;
      if(AlertManeuver_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ALERTMANEUVER_RESPONSE:
    {
      AlertManeuver_response* rv=new AlertManeuver_response;
      if(AlertManeuver_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_CHANGELANGUAGEREGISTRATION_REQUEST:
    {
      ChangeLanguageRegistration_request* rv=new ChangeLanguageRegistration_request;
      if(ChangeLanguageRegistration_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_CHANGELANGUAGEREGISTRATION_RESPONSE:
    {
      ChangeLanguageRegistration_response* rv=new ChangeLanguageRegistration_response;
      if(ChangeLanguageRegistration_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_CREATEINTERACTIONCHOICESET_V2_REQUEST:
    {
      CreateInteractionChoiceSet_v2_request* rv=new CreateInteractionChoiceSet_v2_request;
      if(CreateInteractionChoiceSet_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_CREATEINTERACTIONCHOICESET_V2_RESPONSE:
    {
      CreateInteractionChoiceSet_v2_response* rv=new CreateInteractionChoiceSet_v2_response;
      if(CreateInteractionChoiceSet_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETECOMMAND_V2_REQUEST:
    {
      DeleteCommand_v2_request* rv=new DeleteCommand_v2_request;
      if(DeleteCommand_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETECOMMAND_V2_RESPONSE:
    {
      DeleteCommand_v2_response* rv=new DeleteCommand_v2_response;
      if(DeleteCommand_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETEFILE_REQUEST:
    {
      DeleteFile_request* rv=new DeleteFile_request;
      if(DeleteFile_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETEFILE_RESPONSE:
    {
      DeleteFile_response* rv=new DeleteFile_response;
      if(DeleteFile_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETEINTERACTIONCHOICESET_V2_REQUEST:
    {
      DeleteInteractionChoiceSet_v2_request* rv=new DeleteInteractionChoiceSet_v2_request;
      if(DeleteInteractionChoiceSet_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETEINTERACTIONCHOICESET_V2_RESPONSE:
    {
      DeleteInteractionChoiceSet_v2_response* rv=new DeleteInteractionChoiceSet_v2_response;
      if(DeleteInteractionChoiceSet_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETESUBMENU_V2_REQUEST:
    {
      DeleteSubMenu_v2_request* rv=new DeleteSubMenu_v2_request;
      if(DeleteSubMenu_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DELETESUBMENU_V2_RESPONSE:
    {
      DeleteSubMenu_v2_response* rv=new DeleteSubMenu_v2_response;
      if(DeleteSubMenu_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DIALNUMBER_REQUEST:
    {
      DialNumber_request* rv=new DialNumber_request;
      if(DialNumber_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_DIALNUMBER_RESPONSE:
    {
      DialNumber_response* rv=new DialNumber_response;
      if(DialNumber_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ENCODEDSYNCPDATA_V2_REQUEST:
    {
      EncodedSyncPData_v2_request* rv=new EncodedSyncPData_v2_request;
      if(EncodedSyncPData_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ENCODEDSYNCPDATA_V2_RESPONSE:
    {
      EncodedSyncPData_v2_response* rv=new EncodedSyncPData_v2_response;
      if(EncodedSyncPData_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ENDAUDIOPASSTHRU_REQUEST:
    {
      EndAudioPassThru_request* rv=new EndAudioPassThru_request;
      if(EndAudioPassThru_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ENDAUDIOPASSTHRU_RESPONSE:
    {
      EndAudioPassThru_response* rv=new EndAudioPassThru_response;
      if(EndAudioPassThru_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_GENERICRESPONSE_V2_RESPONSE:
    {
      GenericResponse_v2_response* rv=new GenericResponse_v2_response;
      if(GenericResponse_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_GETDTCS_REQUEST:
    {
      GetDTCs_request* rv=new GetDTCs_request;
      if(GetDTCs_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_GETDTCS_RESPONSE:
    {
      GetDTCs_response* rv=new GetDTCs_response;
      if(GetDTCs_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_GETVEHICLEDATA_REQUEST:
    {
      GetVehicleData_request* rv=new GetVehicleData_request;
      if(GetVehicleData_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_GETVEHICLEDATA_RESPONSE:
    {
      GetVehicleData_response* rv=new GetVehicleData_response;
      if(GetVehicleData_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_LISTFILES_REQUEST:
    {
      ListFiles_request* rv=new ListFiles_request;
      if(ListFiles_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_LISTFILES_RESPONSE:
    {
      ListFiles_response* rv=new ListFiles_response;
      if(ListFiles_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONAPPINTERFACEUNREGISTERED_V2:
    {
      OnAppInterfaceUnregistered_v2* rv=new OnAppInterfaceUnregistered_v2;
      if(OnAppInterfaceUnregistered_v2Marshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONAUDIOPASSTHRU:
    {
      OnAudioPassThru* rv=new OnAudioPassThru;
      if(OnAudioPassThruMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONBUTTONEVENT_V2:
    {
      OnButtonEvent_v2* rv=new OnButtonEvent_v2;
      if(OnButtonEvent_v2Marshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONBUTTONPRESS_V2:
    {
      OnButtonPress_v2* rv=new OnButtonPress_v2;
      if(OnButtonPress_v2Marshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONENCODEDSYNCPDATA_V2:
    {
      OnEncodedSyncPData_v2* rv=new OnEncodedSyncPData_v2;
      if(OnEncodedSyncPData_v2Marshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONHMISTATUS_V2:
    {
      OnHMIStatus_v2* rv=new OnHMIStatus_v2;
      if(OnHMIStatus_v2Marshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONLANGUAGECHANGE:
    {
      OnLanguageChange* rv=new OnLanguageChange;
      if(OnLanguageChangeMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONPERMISSIONSCHANGE:
    {
      OnPermissionsChange* rv=new OnPermissionsChange;
      if(OnPermissionsChangeMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONTBTCLIENTSTATE_V2:
    {
      OnTBTClientState_v2* rv=new OnTBTClientState_v2;
      if(OnTBTClientState_v2Marshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_ONVEHICLEDATA:
    {
      OnVehicleData* rv=new OnVehicleData;
      if(OnVehicleDataMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_PERFORMAUDIOPASSTHRU_REQUEST:
    {
      PerformAudioPassThru_request* rv=new PerformAudioPassThru_request;
      if(PerformAudioPassThru_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_PERFORMAUDIOPASSTHRU_RESPONSE:
    {
      PerformAudioPassThru_response* rv=new PerformAudioPassThru_response;
      if(PerformAudioPassThru_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_PERFORMINTERACTION_V2_REQUEST:
    {
      PerformInteraction_v2_request* rv=new PerformInteraction_v2_request;
      if(PerformInteraction_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_PERFORMINTERACTION_V2_RESPONSE:
    {
      PerformInteraction_v2_response* rv=new PerformInteraction_v2_response;
      if(PerformInteraction_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_PUTFILE_REQUEST:
    {
      PutFile_request* rv=new PutFile_request;
      if(PutFile_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_PUTFILE_RESPONSE:
    {
      PutFile_response* rv=new PutFile_response;
      if(PutFile_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_READDID_REQUEST:
    {
      ReadDID_request* rv=new ReadDID_request;
      if(ReadDID_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_READDID_RESPONSE:
    {
      ReadDID_response* rv=new ReadDID_response;
      if(ReadDID_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_REGISTERAPPINTERFACE_V2_REQUEST:
    {
      RegisterAppInterface_v2_request* rv=new RegisterAppInterface_v2_request;
      if(RegisterAppInterface_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_REGISTERAPPINTERFACE_V2_RESPONSE:
    {
      RegisterAppInterface_v2_response* rv=new RegisterAppInterface_v2_response;
      if(RegisterAppInterface_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_RESETGLOBALPROPERTIES_V2_REQUEST:
    {
      ResetGlobalProperties_v2_request* rv=new ResetGlobalProperties_v2_request;
      if(ResetGlobalProperties_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_RESETGLOBALPROPERTIES_V2_RESPONSE:
    {
      ResetGlobalProperties_v2_response* rv=new ResetGlobalProperties_v2_response;
      if(ResetGlobalProperties_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SCROLLABLEMESSAGE_REQUEST:
    {
      ScrollableMessage_request* rv=new ScrollableMessage_request;
      if(ScrollableMessage_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SCROLLABLEMESSAGE_RESPONSE:
    {
      ScrollableMessage_response* rv=new ScrollableMessage_response;
      if(ScrollableMessage_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETAPPICON_REQUEST:
    {
      SetAppIcon_request* rv=new SetAppIcon_request;
      if(SetAppIcon_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETAPPICON_RESPONSE:
    {
      SetAppIcon_response* rv=new SetAppIcon_response;
      if(SetAppIcon_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETDISPLAYLAYOUT_REQUEST:
    {
      SetDisplayLayout_request* rv=new SetDisplayLayout_request;
      if(SetDisplayLayout_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETDISPLAYLAYOUT_RESPONSE:
    {
      SetDisplayLayout_response* rv=new SetDisplayLayout_response;
      if(SetDisplayLayout_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETGLOBALPROPERTIES_V2_REQUEST:
    {
      SetGlobalProperties_v2_request* rv=new SetGlobalProperties_v2_request;
      if(SetGlobalProperties_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETGLOBALPROPERTIES_V2_RESPONSE:
    {
      SetGlobalProperties_v2_response* rv=new SetGlobalProperties_v2_response;
      if(SetGlobalProperties_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETMEDIACLOCKTIMER_V2_REQUEST:
    {
      SetMediaClockTimer_v2_request* rv=new SetMediaClockTimer_v2_request;
      if(SetMediaClockTimer_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SETMEDIACLOCKTIMER_V2_RESPONSE:
    {
      SetMediaClockTimer_v2_response* rv=new SetMediaClockTimer_v2_response;
      if(SetMediaClockTimer_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SHOW_V2_REQUEST:
    {
      Show_v2_request* rv=new Show_v2_request;
      if(Show_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SHOW_V2_RESPONSE:
    {
      Show_v2_response* rv=new Show_v2_response;
      if(Show_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SHOWCONSTANTTBT_REQUEST:
    {
      ShowConstantTBT_request* rv=new ShowConstantTBT_request;
      if(ShowConstantTBT_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SHOWCONSTANTTBT_RESPONSE:
    {
      ShowConstantTBT_response* rv=new ShowConstantTBT_response;
      if(ShowConstantTBT_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SLIDER_REQUEST:
    {
      Slider_request* rv=new Slider_request;
      if(Slider_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SLIDER_RESPONSE:
    {
      Slider_response* rv=new Slider_response;
      if(Slider_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SPEAK_V2_REQUEST:
    {
      Speak_v2_request* rv=new Speak_v2_request;
      if(Speak_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SPEAK_V2_RESPONSE:
    {
      Speak_v2_response* rv=new Speak_v2_response;
      if(Speak_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SUBSCRIBEBUTTON_V2_REQUEST:
    {
      SubscribeButton_v2_request* rv=new SubscribeButton_v2_request;
      if(SubscribeButton_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SUBSCRIBEBUTTON_V2_RESPONSE:
    {
      SubscribeButton_v2_response* rv=new SubscribeButton_v2_response;
      if(SubscribeButton_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SUBSCRIBEVEHICLEDATA_REQUEST:
    {
      SubscribeVehicleData_request* rv=new SubscribeVehicleData_request;
      if(SubscribeVehicleData_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_SUBSCRIBEVEHICLEDATA_RESPONSE:
    {
      SubscribeVehicleData_response* rv=new SubscribeVehicleData_response;
      if(SubscribeVehicleData_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UNREGISTERAPPINTERFACE_V2_REQUEST:
    {
      UnregisterAppInterface_v2_request* rv=new UnregisterAppInterface_v2_request;
      if(UnregisterAppInterface_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UNREGISTERAPPINTERFACE_V2_RESPONSE:
    {
      UnregisterAppInterface_v2_response* rv=new UnregisterAppInterface_v2_response;
      if(UnregisterAppInterface_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UNSUBSCRIBEBUTTON_V2_REQUEST:
    {
      UnsubscribeButton_v2_request* rv=new UnsubscribeButton_v2_request;
      if(UnsubscribeButton_v2_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UNSUBSCRIBEBUTTON_V2_RESPONSE:
    {
      UnsubscribeButton_v2_response* rv=new UnsubscribeButton_v2_response;
      if(UnsubscribeButton_v2_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UNSUBSCRIBEVEHICLEDATA_REQUEST:
    {
      UnsubscribeVehicleData_request* rv=new UnsubscribeVehicleData_request;
      if(UnsubscribeVehicleData_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UNSUBSCRIBEVEHICLEDATA_RESPONSE:
    {
      UnsubscribeVehicleData_response* rv=new UnsubscribeVehicleData_response;
      if(UnsubscribeVehicleData_responseMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UPDATETURNLIST_REQUEST:
    {
      UpdateTurnList_request* rv=new UpdateTurnList_request;
      if(UpdateTurnList_requestMarshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }

    case METHOD_UPDATETURNLIST_RESPONSE:
    {
      UpdateTurnList_response* rv=new UpdateTurnList_response;
      if(UpdateTurnList_responseMarshaller::fromJSON(json,*rv))
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

    case METHOD_ADDCOMMAND_V2_REQUEST:
         return AddCommand_v2_requestMarshaller::toJSON(* static_cast<const AddCommand_v2_request*>(msg));

    case METHOD_ADDCOMMAND_V2_RESPONSE:
         return AddCommand_v2_responseMarshaller::toJSON(* static_cast<const AddCommand_v2_response*>(msg));

    case METHOD_ADDSUBMENU_V2_REQUEST:
         return AddSubMenu_v2_requestMarshaller::toJSON(* static_cast<const AddSubMenu_v2_request*>(msg));

    case METHOD_ADDSUBMENU_V2_RESPONSE:
         return AddSubMenu_v2_responseMarshaller::toJSON(* static_cast<const AddSubMenu_v2_response*>(msg));

    case METHOD_ALERT_V2_REQUEST:
         return Alert_v2_requestMarshaller::toJSON(* static_cast<const Alert_v2_request*>(msg));

    case METHOD_ALERT_V2_RESPONSE:
         return Alert_v2_responseMarshaller::toJSON(* static_cast<const Alert_v2_response*>(msg));

    case METHOD_ALERTMANEUVER_REQUEST:
         return AlertManeuver_requestMarshaller::toJSON(* static_cast<const AlertManeuver_request*>(msg));

    case METHOD_ALERTMANEUVER_RESPONSE:
         return AlertManeuver_responseMarshaller::toJSON(* static_cast<const AlertManeuver_response*>(msg));

    case METHOD_CHANGELANGUAGEREGISTRATION_REQUEST:
         return ChangeLanguageRegistration_requestMarshaller::toJSON(* static_cast<const ChangeLanguageRegistration_request*>(msg));

    case METHOD_CHANGELANGUAGEREGISTRATION_RESPONSE:
         return ChangeLanguageRegistration_responseMarshaller::toJSON(* static_cast<const ChangeLanguageRegistration_response*>(msg));

    case METHOD_CREATEINTERACTIONCHOICESET_V2_REQUEST:
         return CreateInteractionChoiceSet_v2_requestMarshaller::toJSON(* static_cast<const CreateInteractionChoiceSet_v2_request*>(msg));

    case METHOD_CREATEINTERACTIONCHOICESET_V2_RESPONSE:
         return CreateInteractionChoiceSet_v2_responseMarshaller::toJSON(* static_cast<const CreateInteractionChoiceSet_v2_response*>(msg));

    case METHOD_DELETECOMMAND_V2_REQUEST:
         return DeleteCommand_v2_requestMarshaller::toJSON(* static_cast<const DeleteCommand_v2_request*>(msg));

    case METHOD_DELETECOMMAND_V2_RESPONSE:
         return DeleteCommand_v2_responseMarshaller::toJSON(* static_cast<const DeleteCommand_v2_response*>(msg));

    case METHOD_DELETEFILE_REQUEST:
         return DeleteFile_requestMarshaller::toJSON(* static_cast<const DeleteFile_request*>(msg));

    case METHOD_DELETEFILE_RESPONSE:
         return DeleteFile_responseMarshaller::toJSON(* static_cast<const DeleteFile_response*>(msg));

    case METHOD_DELETEINTERACTIONCHOICESET_V2_REQUEST:
         return DeleteInteractionChoiceSet_v2_requestMarshaller::toJSON(* static_cast<const DeleteInteractionChoiceSet_v2_request*>(msg));

    case METHOD_DELETEINTERACTIONCHOICESET_V2_RESPONSE:
         return DeleteInteractionChoiceSet_v2_responseMarshaller::toJSON(* static_cast<const DeleteInteractionChoiceSet_v2_response*>(msg));

    case METHOD_DELETESUBMENU_V2_REQUEST:
         return DeleteSubMenu_v2_requestMarshaller::toJSON(* static_cast<const DeleteSubMenu_v2_request*>(msg));

    case METHOD_DELETESUBMENU_V2_RESPONSE:
         return DeleteSubMenu_v2_responseMarshaller::toJSON(* static_cast<const DeleteSubMenu_v2_response*>(msg));

    case METHOD_DIALNUMBER_REQUEST:
         return DialNumber_requestMarshaller::toJSON(* static_cast<const DialNumber_request*>(msg));

    case METHOD_DIALNUMBER_RESPONSE:
         return DialNumber_responseMarshaller::toJSON(* static_cast<const DialNumber_response*>(msg));

    case METHOD_ENCODEDSYNCPDATA_V2_REQUEST:
         return EncodedSyncPData_v2_requestMarshaller::toJSON(* static_cast<const EncodedSyncPData_v2_request*>(msg));

    case METHOD_ENCODEDSYNCPDATA_V2_RESPONSE:
         return EncodedSyncPData_v2_responseMarshaller::toJSON(* static_cast<const EncodedSyncPData_v2_response*>(msg));

    case METHOD_ENDAUDIOPASSTHRU_REQUEST:
         return EndAudioPassThru_requestMarshaller::toJSON(* static_cast<const EndAudioPassThru_request*>(msg));

    case METHOD_ENDAUDIOPASSTHRU_RESPONSE:
         return EndAudioPassThru_responseMarshaller::toJSON(* static_cast<const EndAudioPassThru_response*>(msg));

    case METHOD_GENERICRESPONSE_V2_RESPONSE:
         return GenericResponse_v2_responseMarshaller::toJSON(* static_cast<const GenericResponse_v2_response*>(msg));

    case METHOD_GETDTCS_REQUEST:
         return GetDTCs_requestMarshaller::toJSON(* static_cast<const GetDTCs_request*>(msg));

    case METHOD_GETDTCS_RESPONSE:
         return GetDTCs_responseMarshaller::toJSON(* static_cast<const GetDTCs_response*>(msg));

    case METHOD_GETVEHICLEDATA_REQUEST:
         return GetVehicleData_requestMarshaller::toJSON(* static_cast<const GetVehicleData_request*>(msg));

    case METHOD_GETVEHICLEDATA_RESPONSE:
         return GetVehicleData_responseMarshaller::toJSON(* static_cast<const GetVehicleData_response*>(msg));

    case METHOD_LISTFILES_REQUEST:
         return ListFiles_requestMarshaller::toJSON(* static_cast<const ListFiles_request*>(msg));

    case METHOD_LISTFILES_RESPONSE:
         return ListFiles_responseMarshaller::toJSON(* static_cast<const ListFiles_response*>(msg));

    case METHOD_ONAPPINTERFACEUNREGISTERED_V2:
         return OnAppInterfaceUnregistered_v2Marshaller::toJSON(* static_cast<const OnAppInterfaceUnregistered_v2*>(msg));

    case METHOD_ONAUDIOPASSTHRU:
         return OnAudioPassThruMarshaller::toJSON(* static_cast<const OnAudioPassThru*>(msg));

    case METHOD_ONBUTTONEVENT_V2:
         return OnButtonEvent_v2Marshaller::toJSON(* static_cast<const OnButtonEvent_v2*>(msg));

    case METHOD_ONBUTTONPRESS_V2:
         return OnButtonPress_v2Marshaller::toJSON(* static_cast<const OnButtonPress_v2*>(msg));

    case METHOD_ONENCODEDSYNCPDATA_V2:
         return OnEncodedSyncPData_v2Marshaller::toJSON(* static_cast<const OnEncodedSyncPData_v2*>(msg));

    case METHOD_ONHMISTATUS_V2:
         return OnHMIStatus_v2Marshaller::toJSON(* static_cast<const OnHMIStatus_v2*>(msg));

    case METHOD_ONLANGUAGECHANGE:
         return OnLanguageChangeMarshaller::toJSON(* static_cast<const OnLanguageChange*>(msg));

    case METHOD_ONPERMISSIONSCHANGE:
         return OnPermissionsChangeMarshaller::toJSON(* static_cast<const OnPermissionsChange*>(msg));

    case METHOD_ONTBTCLIENTSTATE_V2:
         return OnTBTClientState_v2Marshaller::toJSON(* static_cast<const OnTBTClientState_v2*>(msg));

    case METHOD_ONVEHICLEDATA:
         return OnVehicleDataMarshaller::toJSON(* static_cast<const OnVehicleData*>(msg));

    case METHOD_PERFORMAUDIOPASSTHRU_REQUEST:
         return PerformAudioPassThru_requestMarshaller::toJSON(* static_cast<const PerformAudioPassThru_request*>(msg));

    case METHOD_PERFORMAUDIOPASSTHRU_RESPONSE:
         return PerformAudioPassThru_responseMarshaller::toJSON(* static_cast<const PerformAudioPassThru_response*>(msg));

    case METHOD_PERFORMINTERACTION_V2_REQUEST:
         return PerformInteraction_v2_requestMarshaller::toJSON(* static_cast<const PerformInteraction_v2_request*>(msg));

    case METHOD_PERFORMINTERACTION_V2_RESPONSE:
         return PerformInteraction_v2_responseMarshaller::toJSON(* static_cast<const PerformInteraction_v2_response*>(msg));

    case METHOD_PUTFILE_REQUEST:
         return PutFile_requestMarshaller::toJSON(* static_cast<const PutFile_request*>(msg));

    case METHOD_PUTFILE_RESPONSE:
         return PutFile_responseMarshaller::toJSON(* static_cast<const PutFile_response*>(msg));

    case METHOD_READDID_REQUEST:
         return ReadDID_requestMarshaller::toJSON(* static_cast<const ReadDID_request*>(msg));

    case METHOD_READDID_RESPONSE:
         return ReadDID_responseMarshaller::toJSON(* static_cast<const ReadDID_response*>(msg));

    case METHOD_REGISTERAPPINTERFACE_V2_REQUEST:
         return RegisterAppInterface_v2_requestMarshaller::toJSON(* static_cast<const RegisterAppInterface_v2_request*>(msg));

    case METHOD_REGISTERAPPINTERFACE_V2_RESPONSE:
         return RegisterAppInterface_v2_responseMarshaller::toJSON(* static_cast<const RegisterAppInterface_v2_response*>(msg));

    case METHOD_RESETGLOBALPROPERTIES_V2_REQUEST:
         return ResetGlobalProperties_v2_requestMarshaller::toJSON(* static_cast<const ResetGlobalProperties_v2_request*>(msg));

    case METHOD_RESETGLOBALPROPERTIES_V2_RESPONSE:
         return ResetGlobalProperties_v2_responseMarshaller::toJSON(* static_cast<const ResetGlobalProperties_v2_response*>(msg));

    case METHOD_SCROLLABLEMESSAGE_REQUEST:
         return ScrollableMessage_requestMarshaller::toJSON(* static_cast<const ScrollableMessage_request*>(msg));

    case METHOD_SCROLLABLEMESSAGE_RESPONSE:
         return ScrollableMessage_responseMarshaller::toJSON(* static_cast<const ScrollableMessage_response*>(msg));

    case METHOD_SETAPPICON_REQUEST:
         return SetAppIcon_requestMarshaller::toJSON(* static_cast<const SetAppIcon_request*>(msg));

    case METHOD_SETAPPICON_RESPONSE:
         return SetAppIcon_responseMarshaller::toJSON(* static_cast<const SetAppIcon_response*>(msg));

    case METHOD_SETDISPLAYLAYOUT_REQUEST:
         return SetDisplayLayout_requestMarshaller::toJSON(* static_cast<const SetDisplayLayout_request*>(msg));

    case METHOD_SETDISPLAYLAYOUT_RESPONSE:
         return SetDisplayLayout_responseMarshaller::toJSON(* static_cast<const SetDisplayLayout_response*>(msg));

    case METHOD_SETGLOBALPROPERTIES_V2_REQUEST:
         return SetGlobalProperties_v2_requestMarshaller::toJSON(* static_cast<const SetGlobalProperties_v2_request*>(msg));

    case METHOD_SETGLOBALPROPERTIES_V2_RESPONSE:
         return SetGlobalProperties_v2_responseMarshaller::toJSON(* static_cast<const SetGlobalProperties_v2_response*>(msg));

    case METHOD_SETMEDIACLOCKTIMER_V2_REQUEST:
         return SetMediaClockTimer_v2_requestMarshaller::toJSON(* static_cast<const SetMediaClockTimer_v2_request*>(msg));

    case METHOD_SETMEDIACLOCKTIMER_V2_RESPONSE:
         return SetMediaClockTimer_v2_responseMarshaller::toJSON(* static_cast<const SetMediaClockTimer_v2_response*>(msg));

    case METHOD_SHOW_V2_REQUEST:
         return Show_v2_requestMarshaller::toJSON(* static_cast<const Show_v2_request*>(msg));

    case METHOD_SHOW_V2_RESPONSE:
         return Show_v2_responseMarshaller::toJSON(* static_cast<const Show_v2_response*>(msg));

    case METHOD_SHOWCONSTANTTBT_REQUEST:
         return ShowConstantTBT_requestMarshaller::toJSON(* static_cast<const ShowConstantTBT_request*>(msg));

    case METHOD_SHOWCONSTANTTBT_RESPONSE:
         return ShowConstantTBT_responseMarshaller::toJSON(* static_cast<const ShowConstantTBT_response*>(msg));

    case METHOD_SLIDER_REQUEST:
         return Slider_requestMarshaller::toJSON(* static_cast<const Slider_request*>(msg));

    case METHOD_SLIDER_RESPONSE:
         return Slider_responseMarshaller::toJSON(* static_cast<const Slider_response*>(msg));

    case METHOD_SPEAK_V2_REQUEST:
         return Speak_v2_requestMarshaller::toJSON(* static_cast<const Speak_v2_request*>(msg));

    case METHOD_SPEAK_V2_RESPONSE:
         return Speak_v2_responseMarshaller::toJSON(* static_cast<const Speak_v2_response*>(msg));

    case METHOD_SUBSCRIBEBUTTON_V2_REQUEST:
         return SubscribeButton_v2_requestMarshaller::toJSON(* static_cast<const SubscribeButton_v2_request*>(msg));

    case METHOD_SUBSCRIBEBUTTON_V2_RESPONSE:
         return SubscribeButton_v2_responseMarshaller::toJSON(* static_cast<const SubscribeButton_v2_response*>(msg));

    case METHOD_SUBSCRIBEVEHICLEDATA_REQUEST:
         return SubscribeVehicleData_requestMarshaller::toJSON(* static_cast<const SubscribeVehicleData_request*>(msg));

    case METHOD_SUBSCRIBEVEHICLEDATA_RESPONSE:
         return SubscribeVehicleData_responseMarshaller::toJSON(* static_cast<const SubscribeVehicleData_response*>(msg));

    case METHOD_UNREGISTERAPPINTERFACE_V2_REQUEST:
         return UnregisterAppInterface_v2_requestMarshaller::toJSON(* static_cast<const UnregisterAppInterface_v2_request*>(msg));

    case METHOD_UNREGISTERAPPINTERFACE_V2_RESPONSE:
         return UnregisterAppInterface_v2_responseMarshaller::toJSON(* static_cast<const UnregisterAppInterface_v2_response*>(msg));

    case METHOD_UNSUBSCRIBEBUTTON_V2_REQUEST:
         return UnsubscribeButton_v2_requestMarshaller::toJSON(* static_cast<const UnsubscribeButton_v2_request*>(msg));

    case METHOD_UNSUBSCRIBEBUTTON_V2_RESPONSE:
         return UnsubscribeButton_v2_responseMarshaller::toJSON(* static_cast<const UnsubscribeButton_v2_response*>(msg));

    case METHOD_UNSUBSCRIBEVEHICLEDATA_REQUEST:
         return UnsubscribeVehicleData_requestMarshaller::toJSON(* static_cast<const UnsubscribeVehicleData_request*>(msg));

    case METHOD_UNSUBSCRIBEVEHICLEDATA_RESPONSE:
         return UnsubscribeVehicleData_responseMarshaller::toJSON(* static_cast<const UnsubscribeVehicleData_response*>(msg));

    case METHOD_UPDATETURNLIST_REQUEST:
         return UpdateTurnList_requestMarshaller::toJSON(* static_cast<const UpdateTurnList_request*>(msg));

    case METHOD_UPDATETURNLIST_RESPONSE:
         return UpdateTurnList_responseMarshaller::toJSON(* static_cast<const UpdateTurnList_response*>(msg));

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
AddCommand_v2_requestMarshaller Marshaller::mAddCommand_v2_request;
AddCommand_v2_responseMarshaller Marshaller::mAddCommand_v2_response;
AddSubMenu_v2_requestMarshaller Marshaller::mAddSubMenu_v2_request;
AddSubMenu_v2_responseMarshaller Marshaller::mAddSubMenu_v2_response;
Alert_v2_requestMarshaller Marshaller::mAlert_v2_request;
Alert_v2_responseMarshaller Marshaller::mAlert_v2_response;
AlertManeuver_requestMarshaller Marshaller::mAlertManeuver_request;
AlertManeuver_responseMarshaller Marshaller::mAlertManeuver_response;
ChangeLanguageRegistration_requestMarshaller Marshaller::mChangeLanguageRegistration_request;
ChangeLanguageRegistration_responseMarshaller Marshaller::mChangeLanguageRegistration_response;
CreateInteractionChoiceSet_v2_requestMarshaller Marshaller::mCreateInteractionChoiceSet_v2_request;
CreateInteractionChoiceSet_v2_responseMarshaller Marshaller::mCreateInteractionChoiceSet_v2_response;
DeleteCommand_v2_requestMarshaller Marshaller::mDeleteCommand_v2_request;
DeleteCommand_v2_responseMarshaller Marshaller::mDeleteCommand_v2_response;
DeleteFile_requestMarshaller Marshaller::mDeleteFile_request;
DeleteFile_responseMarshaller Marshaller::mDeleteFile_response;
DeleteInteractionChoiceSet_v2_requestMarshaller Marshaller::mDeleteInteractionChoiceSet_v2_request;
DeleteInteractionChoiceSet_v2_responseMarshaller Marshaller::mDeleteInteractionChoiceSet_v2_response;
DeleteSubMenu_v2_requestMarshaller Marshaller::mDeleteSubMenu_v2_request;
DeleteSubMenu_v2_responseMarshaller Marshaller::mDeleteSubMenu_v2_response;
DialNumber_requestMarshaller Marshaller::mDialNumber_request;
DialNumber_responseMarshaller Marshaller::mDialNumber_response;
EncodedSyncPData_v2_requestMarshaller Marshaller::mEncodedSyncPData_v2_request;
EncodedSyncPData_v2_responseMarshaller Marshaller::mEncodedSyncPData_v2_response;
EndAudioPassThru_requestMarshaller Marshaller::mEndAudioPassThru_request;
EndAudioPassThru_responseMarshaller Marshaller::mEndAudioPassThru_response;
GenericResponse_v2_responseMarshaller Marshaller::mGenericResponse_v2_response;
GetDTCs_requestMarshaller Marshaller::mGetDTCs_request;
GetDTCs_responseMarshaller Marshaller::mGetDTCs_response;
GetVehicleData_requestMarshaller Marshaller::mGetVehicleData_request;
GetVehicleData_responseMarshaller Marshaller::mGetVehicleData_response;
ListFiles_requestMarshaller Marshaller::mListFiles_request;
ListFiles_responseMarshaller Marshaller::mListFiles_response;
OnAppInterfaceUnregistered_v2Marshaller Marshaller::mOnAppInterfaceUnregistered_v2;
OnAudioPassThruMarshaller Marshaller::mOnAudioPassThru;
OnButtonEvent_v2Marshaller Marshaller::mOnButtonEvent_v2;
OnButtonPress_v2Marshaller Marshaller::mOnButtonPress_v2;
OnEncodedSyncPData_v2Marshaller Marshaller::mOnEncodedSyncPData_v2;
OnHMIStatus_v2Marshaller Marshaller::mOnHMIStatus_v2;
OnLanguageChangeMarshaller Marshaller::mOnLanguageChange;
OnPermissionsChangeMarshaller Marshaller::mOnPermissionsChange;
OnTBTClientState_v2Marshaller Marshaller::mOnTBTClientState_v2;
OnVehicleDataMarshaller Marshaller::mOnVehicleData;
PerformAudioPassThru_requestMarshaller Marshaller::mPerformAudioPassThru_request;
PerformAudioPassThru_responseMarshaller Marshaller::mPerformAudioPassThru_response;
PerformInteraction_v2_requestMarshaller Marshaller::mPerformInteraction_v2_request;
PerformInteraction_v2_responseMarshaller Marshaller::mPerformInteraction_v2_response;
PutFile_requestMarshaller Marshaller::mPutFile_request;
PutFile_responseMarshaller Marshaller::mPutFile_response;
ReadDID_requestMarshaller Marshaller::mReadDID_request;
ReadDID_responseMarshaller Marshaller::mReadDID_response;
RegisterAppInterface_v2_requestMarshaller Marshaller::mRegisterAppInterface_v2_request;
RegisterAppInterface_v2_responseMarshaller Marshaller::mRegisterAppInterface_v2_response;
ResetGlobalProperties_v2_requestMarshaller Marshaller::mResetGlobalProperties_v2_request;
ResetGlobalProperties_v2_responseMarshaller Marshaller::mResetGlobalProperties_v2_response;
ScrollableMessage_requestMarshaller Marshaller::mScrollableMessage_request;
ScrollableMessage_responseMarshaller Marshaller::mScrollableMessage_response;
SetAppIcon_requestMarshaller Marshaller::mSetAppIcon_request;
SetAppIcon_responseMarshaller Marshaller::mSetAppIcon_response;
SetDisplayLayout_requestMarshaller Marshaller::mSetDisplayLayout_request;
SetDisplayLayout_responseMarshaller Marshaller::mSetDisplayLayout_response;
SetGlobalProperties_v2_requestMarshaller Marshaller::mSetGlobalProperties_v2_request;
SetGlobalProperties_v2_responseMarshaller Marshaller::mSetGlobalProperties_v2_response;
SetMediaClockTimer_v2_requestMarshaller Marshaller::mSetMediaClockTimer_v2_request;
SetMediaClockTimer_v2_responseMarshaller Marshaller::mSetMediaClockTimer_v2_response;
Show_v2_requestMarshaller Marshaller::mShow_v2_request;
Show_v2_responseMarshaller Marshaller::mShow_v2_response;
ShowConstantTBT_requestMarshaller Marshaller::mShowConstantTBT_request;
ShowConstantTBT_responseMarshaller Marshaller::mShowConstantTBT_response;
Slider_requestMarshaller Marshaller::mSlider_request;
Slider_responseMarshaller Marshaller::mSlider_response;
Speak_v2_requestMarshaller Marshaller::mSpeak_v2_request;
Speak_v2_responseMarshaller Marshaller::mSpeak_v2_response;
SubscribeButton_v2_requestMarshaller Marshaller::mSubscribeButton_v2_request;
SubscribeButton_v2_responseMarshaller Marshaller::mSubscribeButton_v2_response;
SubscribeVehicleData_requestMarshaller Marshaller::mSubscribeVehicleData_request;
SubscribeVehicleData_responseMarshaller Marshaller::mSubscribeVehicleData_response;
UnregisterAppInterface_v2_requestMarshaller Marshaller::mUnregisterAppInterface_v2_request;
UnregisterAppInterface_v2_responseMarshaller Marshaller::mUnregisterAppInterface_v2_response;
UnsubscribeButton_v2_requestMarshaller Marshaller::mUnsubscribeButton_v2_request;
UnsubscribeButton_v2_responseMarshaller Marshaller::mUnsubscribeButton_v2_response;
UnsubscribeVehicleData_requestMarshaller Marshaller::mUnsubscribeVehicleData_request;
UnsubscribeVehicleData_responseMarshaller Marshaller::mUnsubscribeVehicleData_response;
UpdateTurnList_requestMarshaller Marshaller::mUpdateTurnList_request;
UpdateTurnList_responseMarshaller Marshaller::mUpdateTurnList_response;

const Marshaller::localHash Marshaller::mHashTable[130]=
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
  {"OnDriverDistraction_notification",METHOD_ONDRIVERDISTRACTION,&Marshaller::mOnDriverDistraction},
  {"AddCommand_v2_request",METHOD_ADDCOMMAND_V2_REQUEST,&Marshaller::mAddCommand_v2_request},
  {"AddCommand_v2_response",METHOD_ADDCOMMAND_V2_RESPONSE,&Marshaller::mAddCommand_v2_response},
  {"AddSubMenu_v2_request",METHOD_ADDSUBMENU_V2_REQUEST,&Marshaller::mAddSubMenu_v2_request},
  {"AddSubMenu_v2_response",METHOD_ADDSUBMENU_V2_RESPONSE,&Marshaller::mAddSubMenu_v2_response},
  {"Alert_v2_request",METHOD_ALERT_V2_REQUEST,&Marshaller::mAlert_v2_request},
  {"Alert_v2_response",METHOD_ALERT_V2_RESPONSE,&Marshaller::mAlert_v2_response},
  {"AlertManeuver_request",METHOD_ALERTMANEUVER_REQUEST,&Marshaller::mAlertManeuver_request},
  {"AlertManeuver_response",METHOD_ALERTMANEUVER_RESPONSE,&Marshaller::mAlertManeuver_response},
  {"ChangeLanguageRegistration_request",METHOD_CHANGELANGUAGEREGISTRATION_REQUEST,&Marshaller::mChangeLanguageRegistration_request},
  {"ChangeLanguageRegistration_response",METHOD_CHANGELANGUAGEREGISTRATION_RESPONSE,&Marshaller::mChangeLanguageRegistration_response},
  {"CreateInteractionChoiceSet_v2_request",METHOD_CREATEINTERACTIONCHOICESET_V2_REQUEST,&Marshaller::mCreateInteractionChoiceSet_v2_request},
  {"CreateInteractionChoiceSet_v2_response",METHOD_CREATEINTERACTIONCHOICESET_V2_RESPONSE,&Marshaller::mCreateInteractionChoiceSet_v2_response},
  {"DeleteCommand_v2_request",METHOD_DELETECOMMAND_V2_REQUEST,&Marshaller::mDeleteCommand_v2_request},
  {"DeleteCommand_v2_response",METHOD_DELETECOMMAND_V2_RESPONSE,&Marshaller::mDeleteCommand_v2_response},
  {"DeleteFile_request",METHOD_DELETEFILE_REQUEST,&Marshaller::mDeleteFile_request},
  {"DeleteFile_response",METHOD_DELETEFILE_RESPONSE,&Marshaller::mDeleteFile_response},
  {"DeleteInteractionChoiceSet_v2_request",METHOD_DELETEINTERACTIONCHOICESET_V2_REQUEST,&Marshaller::mDeleteInteractionChoiceSet_v2_request},
  {"DeleteInteractionChoiceSet_v2_response",METHOD_DELETEINTERACTIONCHOICESET_V2_RESPONSE,&Marshaller::mDeleteInteractionChoiceSet_v2_response},
  {"DeleteSubMenu_v2_request",METHOD_DELETESUBMENU_V2_REQUEST,&Marshaller::mDeleteSubMenu_v2_request},
  {"DeleteSubMenu_v2_response",METHOD_DELETESUBMENU_V2_RESPONSE,&Marshaller::mDeleteSubMenu_v2_response},
  {"DialNumber_request",METHOD_DIALNUMBER_REQUEST,&Marshaller::mDialNumber_request},
  {"DialNumber_response",METHOD_DIALNUMBER_RESPONSE,&Marshaller::mDialNumber_response},
  {"EncodedSyncPData_v2_request",METHOD_ENCODEDSYNCPDATA_V2_REQUEST,&Marshaller::mEncodedSyncPData_v2_request},
  {"EncodedSyncPData_v2_response",METHOD_ENCODEDSYNCPDATA_V2_RESPONSE,&Marshaller::mEncodedSyncPData_v2_response},
  {"EndAudioPassThru_request",METHOD_ENDAUDIOPASSTHRU_REQUEST,&Marshaller::mEndAudioPassThru_request},
  {"EndAudioPassThru_response",METHOD_ENDAUDIOPASSTHRU_RESPONSE,&Marshaller::mEndAudioPassThru_response},
  {"GenericResponse_v2_response",METHOD_GENERICRESPONSE_V2_RESPONSE,&Marshaller::mGenericResponse_v2_response},
  {"GetDTCs_request",METHOD_GETDTCS_REQUEST,&Marshaller::mGetDTCs_request},
  {"GetDTCs_response",METHOD_GETDTCS_RESPONSE,&Marshaller::mGetDTCs_response},
  {"GetVehicleData_request",METHOD_GETVEHICLEDATA_REQUEST,&Marshaller::mGetVehicleData_request},
  {"GetVehicleData_response",METHOD_GETVEHICLEDATA_RESPONSE,&Marshaller::mGetVehicleData_response},
  {"ListFiles_request",METHOD_LISTFILES_REQUEST,&Marshaller::mListFiles_request},
  {"ListFiles_response",METHOD_LISTFILES_RESPONSE,&Marshaller::mListFiles_response},
  {"OnAppInterfaceUnregistered_v2_notification",METHOD_ONAPPINTERFACEUNREGISTERED_V2,&Marshaller::mOnAppInterfaceUnregistered_v2},
  {"OnAudioPassThru_notification",METHOD_ONAUDIOPASSTHRU,&Marshaller::mOnAudioPassThru},
  {"OnButtonEvent_v2_notification",METHOD_ONBUTTONEVENT_V2,&Marshaller::mOnButtonEvent_v2},
  {"OnButtonPress_v2_notification",METHOD_ONBUTTONPRESS_V2,&Marshaller::mOnButtonPress_v2},
  {"OnEncodedSyncPData_v2_notification",METHOD_ONENCODEDSYNCPDATA_V2,&Marshaller::mOnEncodedSyncPData_v2},
  {"OnHMIStatus_v2_notification",METHOD_ONHMISTATUS_V2,&Marshaller::mOnHMIStatus_v2},
  {"OnLanguageChange_notification",METHOD_ONLANGUAGECHANGE,&Marshaller::mOnLanguageChange},
  {"OnPermissionsChange_notification",METHOD_ONPERMISSIONSCHANGE,&Marshaller::mOnPermissionsChange},
  {"OnTBTClientState_v2_notification",METHOD_ONTBTCLIENTSTATE_V2,&Marshaller::mOnTBTClientState_v2},
  {"OnVehicleData_notification",METHOD_ONVEHICLEDATA,&Marshaller::mOnVehicleData},
  {"PerformAudioPassThru_request",METHOD_PERFORMAUDIOPASSTHRU_REQUEST,&Marshaller::mPerformAudioPassThru_request},
  {"PerformAudioPassThru_response",METHOD_PERFORMAUDIOPASSTHRU_RESPONSE,&Marshaller::mPerformAudioPassThru_response},
  {"PerformInteraction_v2_request",METHOD_PERFORMINTERACTION_V2_REQUEST,&Marshaller::mPerformInteraction_v2_request},
  {"PerformInteraction_v2_response",METHOD_PERFORMINTERACTION_V2_RESPONSE,&Marshaller::mPerformInteraction_v2_response},
  {"PutFile_request",METHOD_PUTFILE_REQUEST,&Marshaller::mPutFile_request},
  {"PutFile_response",METHOD_PUTFILE_RESPONSE,&Marshaller::mPutFile_response},
  {"ReadDID_request",METHOD_READDID_REQUEST,&Marshaller::mReadDID_request},
  {"ReadDID_response",METHOD_READDID_RESPONSE,&Marshaller::mReadDID_response},
  {"RegisterAppInterface_v2_request",METHOD_REGISTERAPPINTERFACE_V2_REQUEST,&Marshaller::mRegisterAppInterface_v2_request},
  {"RegisterAppInterface_v2_response",METHOD_REGISTERAPPINTERFACE_V2_RESPONSE,&Marshaller::mRegisterAppInterface_v2_response},
  {"ResetGlobalProperties_v2_request",METHOD_RESETGLOBALPROPERTIES_V2_REQUEST,&Marshaller::mResetGlobalProperties_v2_request},
  {"ResetGlobalProperties_v2_response",METHOD_RESETGLOBALPROPERTIES_V2_RESPONSE,&Marshaller::mResetGlobalProperties_v2_response},
  {"ScrollableMessage_request",METHOD_SCROLLABLEMESSAGE_REQUEST,&Marshaller::mScrollableMessage_request},
  {"ScrollableMessage_response",METHOD_SCROLLABLEMESSAGE_RESPONSE,&Marshaller::mScrollableMessage_response},
  {"SetAppIcon_request",METHOD_SETAPPICON_REQUEST,&Marshaller::mSetAppIcon_request},
  {"SetAppIcon_response",METHOD_SETAPPICON_RESPONSE,&Marshaller::mSetAppIcon_response},
  {"SetDisplayLayout_request",METHOD_SETDISPLAYLAYOUT_REQUEST,&Marshaller::mSetDisplayLayout_request},
  {"SetDisplayLayout_response",METHOD_SETDISPLAYLAYOUT_RESPONSE,&Marshaller::mSetDisplayLayout_response},
  {"SetGlobalProperties_v2_request",METHOD_SETGLOBALPROPERTIES_V2_REQUEST,&Marshaller::mSetGlobalProperties_v2_request},
  {"SetGlobalProperties_v2_response",METHOD_SETGLOBALPROPERTIES_V2_RESPONSE,&Marshaller::mSetGlobalProperties_v2_response},
  {"SetMediaClockTimer_v2_request",METHOD_SETMEDIACLOCKTIMER_V2_REQUEST,&Marshaller::mSetMediaClockTimer_v2_request},
  {"SetMediaClockTimer_v2_response",METHOD_SETMEDIACLOCKTIMER_V2_RESPONSE,&Marshaller::mSetMediaClockTimer_v2_response},
  {"Show_v2_request",METHOD_SHOW_V2_REQUEST,&Marshaller::mShow_v2_request},
  {"Show_v2_response",METHOD_SHOW_V2_RESPONSE,&Marshaller::mShow_v2_response},
  {"ShowConstantTBT_request",METHOD_SHOWCONSTANTTBT_REQUEST,&Marshaller::mShowConstantTBT_request},
  {"ShowConstantTBT_response",METHOD_SHOWCONSTANTTBT_RESPONSE,&Marshaller::mShowConstantTBT_response},
  {"Slider_request",METHOD_SLIDER_REQUEST,&Marshaller::mSlider_request},
  {"Slider_response",METHOD_SLIDER_RESPONSE,&Marshaller::mSlider_response},
  {"Speak_v2_request",METHOD_SPEAK_V2_REQUEST,&Marshaller::mSpeak_v2_request},
  {"Speak_v2_response",METHOD_SPEAK_V2_RESPONSE,&Marshaller::mSpeak_v2_response},
  {"SubscribeButton_v2_request",METHOD_SUBSCRIBEBUTTON_V2_REQUEST,&Marshaller::mSubscribeButton_v2_request},
  {"SubscribeButton_v2_response",METHOD_SUBSCRIBEBUTTON_V2_RESPONSE,&Marshaller::mSubscribeButton_v2_response},
  {"SubscribeVehicleData_request",METHOD_SUBSCRIBEVEHICLEDATA_REQUEST,&Marshaller::mSubscribeVehicleData_request},
  {"SubscribeVehicleData_response",METHOD_SUBSCRIBEVEHICLEDATA_RESPONSE,&Marshaller::mSubscribeVehicleData_response},
  {"UnregisterAppInterface_v2_request",METHOD_UNREGISTERAPPINTERFACE_V2_REQUEST,&Marshaller::mUnregisterAppInterface_v2_request},
  {"UnregisterAppInterface_v2_response",METHOD_UNREGISTERAPPINTERFACE_V2_RESPONSE,&Marshaller::mUnregisterAppInterface_v2_response},
  {"UnsubscribeButton_v2_request",METHOD_UNSUBSCRIBEBUTTON_V2_REQUEST,&Marshaller::mUnsubscribeButton_v2_request},
  {"UnsubscribeButton_v2_response",METHOD_UNSUBSCRIBEBUTTON_V2_RESPONSE,&Marshaller::mUnsubscribeButton_v2_response},
  {"UnsubscribeVehicleData_request",METHOD_UNSUBSCRIBEVEHICLEDATA_REQUEST,&Marshaller::mUnsubscribeVehicleData_request},
  {"UnsubscribeVehicleData_response",METHOD_UNSUBSCRIBEVEHICLEDATA_RESPONSE,&Marshaller::mUnsubscribeVehicleData_response},
  {"UpdateTurnList_request",METHOD_UPDATETURNLIST_REQUEST,&Marshaller::mUpdateTurnList_request},
  {"UpdateTurnList_response",METHOD_UPDATETURNLIST_RESPONSE,&Marshaller::mUpdateTurnList_response}
};
