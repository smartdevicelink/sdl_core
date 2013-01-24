#include <cstring>

#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/RegisterAppInterface_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/RegisterAppInterface_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/UnregisterAppInterface_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/UnregisterAppInterface_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SetGlobalProperties_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SetGlobalProperties_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ResetGlobalProperties_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ResetGlobalProperties_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/AddCommand_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/AddCommand_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/DeleteCommand_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/DeleteCommand_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/AddSubMenu_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/AddSubMenu_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/DeleteSubMenu_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/DeleteSubMenu_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/CreateInteractionChoiceSet_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/CreateInteractionChoiceSet_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/PerformInteraction_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/PerformInteraction_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/DeleteInteractionChoiceSet_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/DeleteInteractionChoiceSet_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Alert_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Alert_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Show_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Show_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Speak_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Speak_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SetMediaClockTimer_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SetMediaClockTimer_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/EncodedSyncPData_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/EncodedSyncPData_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/DialNumber_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/DialNumber_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/PerformAudioPassThru_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/PerformAudioPassThru_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/EndAudioPassThru_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/EndAudioPassThru_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SubscribeButton_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SubscribeButton_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/UnsubscribeButton_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/UnsubscribeButton_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SubscribeVehicleData_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SubscribeVehicleData_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/UnsubscribeVehicleData_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/UnsubscribeVehicleData_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/GetVehicleData_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/GetVehicleData_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ReadDID_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ReadDID_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/GetDTCs_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/GetDTCs_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ScrollableMessage_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ScrollableMessage_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Slider_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Slider_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ShowConstantTBT_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ShowConstantTBT_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/AlertManeuver_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/AlertManeuver_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/UpdateTurnList_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/UpdateTurnList_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ChangeRegistration_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ChangeRegistration_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/GenericResponse_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/PutFile_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/PutFile_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/DeleteFile_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/DeleteFile_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ListFiles_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/ListFiles_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SetAppIcon_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SetAppIcon_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SetDisplayLayout_request.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SetDisplayLayout_response.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnHMIStatus.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnAppInterfaceUnregistered.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnButtonEvent.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnButtonPress.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnVehicleData.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnCommand.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnEncodedSyncPData.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnTBTClientState.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnDriverDistraction.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnPermissionsChange.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnAudioPassThru.h"
#include "../include/JSONHandler/ALRPCObjects/V2/OnLanguageChange.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


NsAppLinkRPC::ALRPCMessage* Marshaller::fromString(const std::string& s,FunctionID::FunctionIDInternal fid, messageType::messageTypeInternal mt)
{
  NsAppLinkRPC::ALRPCMessage* rv=0;
  try
  {
    Json::Reader reader;
    Json::Value json;

    if(!reader.parse(s,json,false))  return 0;
    if(!(rv=fromJSON(json,fid,mt)))  return 0;
  }
  catch(...)
  {
    return 0;
  }
  return rv;
}

std::string Marshaller::toString(const NsAppLinkRPC::ALRPCMessage* msg,FunctionID::FunctionIDInternal fid, messageType::messageTypeInternal mt)
{
  if(!msg)  return "";

  Json::Value json=toJSON(msg,fid,mt);

  if(json.isNull()) return "";

  Json::FastWriter writer;
  std::string rv;
  return writer.write(json);
}


NsAppLinkRPC::ALRPCMessage* Marshaller::fromJSON(const Json::Value& json,FunctionID::FunctionIDInternal fid, messageType::messageTypeInternal mt)
{
  if(!json.isObject())  return NULL;
  Json::Value j=Json::Value(Json::nullValue);

  if(!json.isMember("parameters"))  return NULL;
  j=json["parameters"];

  switch(fid)							// let's rock, china style
  {
    case FunctionID::AddCommandID:
      if(mt==messageType::request)
      {
        AddCommand_request* rv=new AddCommand_request;
        if(AddCommand_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        AddCommand_response* rv=new AddCommand_response;
        if(AddCommand_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::AddSubMenuID:
      if(mt==messageType::request)
      {
        AddSubMenu_request* rv=new AddSubMenu_request;
        if(AddSubMenu_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        AddSubMenu_response* rv=new AddSubMenu_response;
        if(AddSubMenu_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::AlertID:
      if(mt==messageType::request)
      {
        Alert_request* rv=new Alert_request;
        if(Alert_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        Alert_response* rv=new Alert_response;
        if(Alert_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::AlertManeuverID:
      if(mt==messageType::request)
      {
        AlertManeuver_request* rv=new AlertManeuver_request;
        if(AlertManeuver_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        AlertManeuver_response* rv=new AlertManeuver_response;
        if(AlertManeuver_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::ChangeRegistrationID:
      if(mt==messageType::request)
      {
        ChangeRegistration_request* rv=new ChangeRegistration_request;
        if(ChangeRegistration_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        ChangeRegistration_response* rv=new ChangeRegistration_response;
        if(ChangeRegistration_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::CreateInteractionChoiceSetID:
      if(mt==messageType::request)
      {
        CreateInteractionChoiceSet_request* rv=new CreateInteractionChoiceSet_request;
        if(CreateInteractionChoiceSet_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        CreateInteractionChoiceSet_response* rv=new CreateInteractionChoiceSet_response;
        if(CreateInteractionChoiceSet_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::DeleteCommandID:
      if(mt==messageType::request)
      {
        DeleteCommand_request* rv=new DeleteCommand_request;
        if(DeleteCommand_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        DeleteCommand_response* rv=new DeleteCommand_response;
        if(DeleteCommand_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::DeleteFileID:
      if(mt==messageType::request)
      {
        DeleteFile_request* rv=new DeleteFile_request;
        if(DeleteFile_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        DeleteFile_response* rv=new DeleteFile_response;
        if(DeleteFile_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::DeleteInteractionChoiceSetID:
      if(mt==messageType::request)
      {
        DeleteInteractionChoiceSet_request* rv=new DeleteInteractionChoiceSet_request;
        if(DeleteInteractionChoiceSet_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        DeleteInteractionChoiceSet_response* rv=new DeleteInteractionChoiceSet_response;
        if(DeleteInteractionChoiceSet_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::DeleteSubMenuID:
      if(mt==messageType::request)
      {
        DeleteSubMenu_request* rv=new DeleteSubMenu_request;
        if(DeleteSubMenu_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        DeleteSubMenu_response* rv=new DeleteSubMenu_response;
        if(DeleteSubMenu_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::DialNumberID:
      if(mt==messageType::request)
      {
        DialNumber_request* rv=new DialNumber_request;
        if(DialNumber_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        DialNumber_response* rv=new DialNumber_response;
        if(DialNumber_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::EncodedSyncPDataID:
      if(mt==messageType::request)
      {
        EncodedSyncPData_request* rv=new EncodedSyncPData_request;
        if(EncodedSyncPData_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        EncodedSyncPData_response* rv=new EncodedSyncPData_response;
        if(EncodedSyncPData_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::EndAudioPassThruID:
      if(mt==messageType::request)
      {
        EndAudioPassThru_request* rv=new EndAudioPassThru_request;
        if(EndAudioPassThru_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        EndAudioPassThru_response* rv=new EndAudioPassThru_response;
        if(EndAudioPassThru_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::GenericResponseID:
      if(mt==messageType::request)
      {
        RegisterAppInterface_request* rv=new RegisterAppInterface_request;
        if(RegisterAppInterface_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        RegisterAppInterface_response* rv=new RegisterAppInterface_response;
        if(RegisterAppInterface_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::GetDTCsID:
      if(mt==messageType::request)
      {
        GetDTCs_request* rv=new GetDTCs_request;
        if(GetDTCs_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        GetDTCs_response* rv=new GetDTCs_response;
        if(GetDTCs_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::GetVehicleDataID:
      if(mt==messageType::request)
      {
        GetVehicleData_request* rv=new GetVehicleData_request;
        if(GetVehicleData_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        GetVehicleData_response* rv=new GetVehicleData_response;
        if(GetVehicleData_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::ListFilesID:
      if(mt==messageType::request)
      {
        ListFiles_request* rv=new ListFiles_request;
        if(ListFiles_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        ListFiles_response* rv=new ListFiles_response;
        if(ListFiles_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::OnAppInterfaceUnregisteredID:
      if(mt==messageType::notification)
      {
        OnAppInterfaceUnregistered* rv=new OnAppInterfaceUnregistered;
        if(OnAppInterfaceUnregisteredMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::OnAudioPassThruID:
      if(mt==messageType::notification)
      {
        OnAudioPassThru* rv=new OnAudioPassThru;
        if(OnAudioPassThruMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::OnButtonEventID:
      if(mt==messageType::notification)
      {
        OnButtonEvent* rv=new OnButtonEvent;
        if(OnButtonEventMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::OnButtonPressID:
      if(mt==messageType::notification)
      {
        OnButtonPress* rv=new OnButtonPress;
        if(OnButtonPressMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::OnCommandID:
      if(mt==messageType::notification)
      {
        OnCommand* rv=new OnCommand;
        if(OnCommandMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::OnDriverDistractionID:
      if(mt==messageType::notification)
      {
        OnDriverDistraction* rv=new OnDriverDistraction;
        if(OnDriverDistractionMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::OnEncodedSyncPDataID:
      if(mt==messageType::notification)
      {
        OnEncodedSyncPData* rv=new OnEncodedSyncPData;
        if(OnEncodedSyncPDataMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::OnHMIStatusID:
      if(mt==messageType::notification)
      {
        OnHMIStatus* rv=new OnHMIStatus;
        if(OnHMIStatusMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::OnLanguageChangeID:
      if(mt==messageType::notification)
      {
        OnLanguageChange* rv=new OnLanguageChange;
        if(OnLanguageChangeMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::OnPermissionsChangeID:
      if(mt==messageType::notification)
      {
        OnPermissionsChange* rv=new OnPermissionsChange;
        if(OnPermissionsChangeMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::OnTBTClientStateID:
      if(mt==messageType::notification)
      {
        OnTBTClientState* rv=new OnTBTClientState;
        if(OnTBTClientStateMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::OnVehicleDataID:
      if(mt==messageType::notification)
      {
        OnVehicleData* rv=new OnVehicleData;
        if(OnVehicleDataMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
      }
    case FunctionID::PerformAudioPassThruID:
      if(mt==messageType::request)
      {
        PerformAudioPassThru_request* rv=new PerformAudioPassThru_request;
        if(PerformAudioPassThru_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        PerformAudioPassThru_response* rv=new PerformAudioPassThru_response;
        if(PerformAudioPassThru_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::PerformInteractionID:
      if(mt==messageType::request)
      {
        PerformInteraction_request* rv=new PerformInteraction_request;
        if(PerformInteraction_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        PerformInteraction_response* rv=new PerformInteraction_response;
        if(PerformInteraction_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::PutFileID:
      if(mt==messageType::request)
      {
        PutFile_request* rv=new PutFile_request;
        if(PutFile_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        PutFile_response* rv=new PutFile_response;
        if(PutFile_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::ReadDIDID:
      if(mt==messageType::request)
      {
        ReadDID_request* rv=new ReadDID_request;
        if(ReadDID_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        ReadDID_response* rv=new ReadDID_response;
        if(ReadDID_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::RegisterAppInterfaceID:
      if(mt==messageType::request)
      {
        RegisterAppInterface_request* rv=new RegisterAppInterface_request;
        if(RegisterAppInterface_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        RegisterAppInterface_response* rv=new RegisterAppInterface_response;
        if(RegisterAppInterface_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::ResetGlobalPropertiesID:
      if(mt==messageType::request)
      {
        ResetGlobalProperties_request* rv=new ResetGlobalProperties_request;
        if(ResetGlobalProperties_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        ResetGlobalProperties_response* rv=new ResetGlobalProperties_response;
        if(ResetGlobalProperties_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::ScrollableMessageID:
      if(mt==messageType::request)
      {
        ScrollableMessage_request* rv=new ScrollableMessage_request;
        if(ScrollableMessage_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        ScrollableMessage_response* rv=new ScrollableMessage_response;
        if(ScrollableMessage_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::SetAppIconID:
      if(mt==messageType::request)
      {
        SetAppIcon_request* rv=new SetAppIcon_request;
        if(SetAppIcon_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        SetAppIcon_response* rv=new SetAppIcon_response;
        if(SetAppIcon_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::SetDisplayLayoutID:
      if(mt==messageType::request)
      {
        SetDisplayLayout_request* rv=new SetDisplayLayout_request;
        if(SetDisplayLayout_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        SetDisplayLayout_response* rv=new SetDisplayLayout_response;
        if(SetDisplayLayout_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::SetGlobalPropertiesID:
      if(mt==messageType::request)
      {
        SetGlobalProperties_request* rv=new SetGlobalProperties_request;
        if(SetGlobalProperties_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        SetGlobalProperties_response* rv=new SetGlobalProperties_response;
        if(SetGlobalProperties_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::SetMediaClockTimerID:
      if(mt==messageType::request)
      {
        SetMediaClockTimer_request* rv=new SetMediaClockTimer_request;
        if(SetMediaClockTimer_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        SetMediaClockTimer_response* rv=new SetMediaClockTimer_response;
        if(SetMediaClockTimer_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::ShowID:
      if(mt==messageType::request)
      {
        Show_request* rv=new Show_request;
        if(Show_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        Show_response* rv=new Show_response;
        if(Show_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::ShowConstantTBTID:
      if(mt==messageType::request)
      {
        ShowConstantTBT_request* rv=new ShowConstantTBT_request;
        if(ShowConstantTBT_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        ShowConstantTBT_response* rv=new ShowConstantTBT_response;
        if(ShowConstantTBT_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::SliderID:
      if(mt==messageType::request)
      {
        Slider_request* rv=new Slider_request;
        if(Slider_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        Slider_response* rv=new Slider_response;
        if(Slider_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::SpeakID:
      if(mt==messageType::request)
      {
        Speak_request* rv=new Speak_request;
        if(Speak_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        Speak_response* rv=new Speak_response;
        if(Speak_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::SubscribeButtonID:
      if(mt==messageType::request)
      {
        SubscribeButton_request* rv=new SubscribeButton_request;
        if(SubscribeButton_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        SubscribeButton_response* rv=new SubscribeButton_response;
        if(SubscribeButton_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::SubscribeVehicleDataID:
      if(mt==messageType::request)
      {
        SubscribeVehicleData_request* rv=new SubscribeVehicleData_request;
        if(SubscribeVehicleData_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        SubscribeVehicleData_response* rv=new SubscribeVehicleData_response;
        if(SubscribeVehicleData_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::UnregisterAppInterfaceID:
      if(mt==messageType::request)
      {
        UnregisterAppInterface_request* rv=new UnregisterAppInterface_request;
        if(UnregisterAppInterface_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        UnregisterAppInterface_response* rv=new UnregisterAppInterface_response;
        if(UnregisterAppInterface_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::UnsubscribeButtonID:
      if(mt==messageType::request)
      {
        UnsubscribeButton_request* rv=new UnsubscribeButton_request;
        if(UnsubscribeButton_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        UnsubscribeButton_response* rv=new UnsubscribeButton_response;
        if(UnsubscribeButton_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::UnsubscribeVehicleDataID:
      if(mt==messageType::request)
      {
        UnsubscribeVehicleData_request* rv=new UnsubscribeVehicleData_request;
        if(UnsubscribeVehicleData_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        UnsubscribeVehicleData_response* rv=new UnsubscribeVehicleData_response;
        if(UnsubscribeVehicleData_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    case FunctionID::UpdateTurnListID:
      if(mt==messageType::request)
      {
        UpdateTurnList_request* rv=new UpdateTurnList_request;
        if(UpdateTurnList_requestMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }

      if(mt==messageType::response)
      {
        UpdateTurnList_response* rv=new UpdateTurnList_response;
        if(UpdateTurnList_responseMarshaller::fromJSON(j,*rv))
          return rv;
        delete rv;
        return NULL;
      }
      return NULL;
    default:;
  }

  return NULL;
}

Json::Value Marshaller::toJSON(const NsAppLinkRPC::ALRPCMessage* msg, FunctionID::FunctionIDInternal fid, messageType::messageTypeInternal mt)
{
  Json::Value j=toJSONparam(msg,fid,mt);
  Json::Value rv=Json::Value(Json::nullValue);
  if(!j.isNull())
  {
    rv=Json::Value(Json::objectValue);
    rv["parameters"]=j;
  }
  return rv;
}


Json::Value Marshaller::toJSONparam(const NsAppLinkRPC::ALRPCMessage* msg,FunctionID::FunctionIDInternal fid, messageType::messageTypeInternal mt)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;

  switch(fid)
  {
    case FunctionID::AddCommandID:
      if(mt==messageType::request)
        return AddCommand_requestMarshaller::toJSON(* static_cast<const AddCommand_request*>(msg));
      if(mt==messageType::response)
         return AddCommand_responseMarshaller::toJSON(* static_cast<const AddCommand_response*>(msg));
      return j;
    case FunctionID::AddSubMenuID:
      if(mt==messageType::request)
        return AddSubMenu_requestMarshaller::toJSON(* static_cast<const AddSubMenu_request*>(msg));
      if(mt==messageType::response)
         return AddSubMenu_responseMarshaller::toJSON(* static_cast<const AddSubMenu_response*>(msg));
      return j;
    case FunctionID::AlertID:
      if(mt==messageType::request)
        return Alert_requestMarshaller::toJSON(* static_cast<const Alert_request*>(msg));
      if(mt==messageType::response)
         return Alert_responseMarshaller::toJSON(* static_cast<const Alert_response*>(msg));
      return j;
    case FunctionID::AlertManeuverID:
      if(mt==messageType::request)
        return AlertManeuver_requestMarshaller::toJSON(* static_cast<const AlertManeuver_request*>(msg));
      if(mt==messageType::response)
         return AlertManeuver_responseMarshaller::toJSON(* static_cast<const AlertManeuver_response*>(msg));
      return j;
    case FunctionID::ChangeRegistrationID:
      if(mt==messageType::request)
        return ChangeRegistration_requestMarshaller::toJSON(* static_cast<const ChangeRegistration_request*>(msg));
      if(mt==messageType::response)
         return ChangeRegistration_responseMarshaller::toJSON(* static_cast<const ChangeRegistration_response*>(msg));
      return j;
    case FunctionID::CreateInteractionChoiceSetID:
      if(mt==messageType::request)
        return CreateInteractionChoiceSet_requestMarshaller::toJSON(* static_cast<const CreateInteractionChoiceSet_request*>(msg));
      if(mt==messageType::response)
         return CreateInteractionChoiceSet_responseMarshaller::toJSON(* static_cast<const CreateInteractionChoiceSet_response*>(msg));
      return j;
    case FunctionID::DeleteCommandID:
      if(mt==messageType::request)
        return DeleteCommand_requestMarshaller::toJSON(* static_cast<const DeleteCommand_request*>(msg));
      if(mt==messageType::response)
         return DeleteCommand_responseMarshaller::toJSON(* static_cast<const DeleteCommand_response*>(msg));
      return j;
    case FunctionID::DeleteFileID:
      if(mt==messageType::request)
        return DeleteFile_requestMarshaller::toJSON(* static_cast<const DeleteFile_request*>(msg));
      if(mt==messageType::response)
         return DeleteFile_responseMarshaller::toJSON(* static_cast<const DeleteFile_response*>(msg));
      return j;
    case FunctionID::DeleteInteractionChoiceSetID:
      if(mt==messageType::request)
        return DeleteInteractionChoiceSet_requestMarshaller::toJSON(* static_cast<const DeleteInteractionChoiceSet_request*>(msg));
      if(mt==messageType::response)
         return DeleteInteractionChoiceSet_responseMarshaller::toJSON(* static_cast<const DeleteInteractionChoiceSet_response*>(msg));
      return j;
    case FunctionID::DeleteSubMenuID:
      if(mt==messageType::request)
        return DeleteSubMenu_requestMarshaller::toJSON(* static_cast<const DeleteSubMenu_request*>(msg));
      if(mt==messageType::response)
         return DeleteSubMenu_responseMarshaller::toJSON(* static_cast<const DeleteSubMenu_response*>(msg));
      return j;
    case FunctionID::DialNumberID:
      if(mt==messageType::request)
        return DialNumber_requestMarshaller::toJSON(* static_cast<const DialNumber_request*>(msg));
      if(mt==messageType::response)
         return DialNumber_responseMarshaller::toJSON(* static_cast<const DialNumber_response*>(msg));
      return j;
    case FunctionID::EncodedSyncPDataID:
      if(mt==messageType::request)
        return EncodedSyncPData_requestMarshaller::toJSON(* static_cast<const EncodedSyncPData_request*>(msg));
      if(mt==messageType::response)
         return EncodedSyncPData_responseMarshaller::toJSON(* static_cast<const EncodedSyncPData_response*>(msg));
      return j;
    case FunctionID::EndAudioPassThruID:
      if(mt==messageType::request)
        return EndAudioPassThru_requestMarshaller::toJSON(* static_cast<const EndAudioPassThru_request*>(msg));
      if(mt==messageType::response)
         return EndAudioPassThru_responseMarshaller::toJSON(* static_cast<const EndAudioPassThru_response*>(msg));
      return j;
    case FunctionID::GenericResponseID:
      if(mt==messageType::request)
        return RegisterAppInterface_requestMarshaller::toJSON(* static_cast<const RegisterAppInterface_request*>(msg));
      if(mt==messageType::response)
         return RegisterAppInterface_responseMarshaller::toJSON(* static_cast<const RegisterAppInterface_response*>(msg));
      return j;
    case FunctionID::GetDTCsID:
      if(mt==messageType::request)
        return GetDTCs_requestMarshaller::toJSON(* static_cast<const GetDTCs_request*>(msg));
      if(mt==messageType::response)
         return GetDTCs_responseMarshaller::toJSON(* static_cast<const GetDTCs_response*>(msg));
      return j;
    case FunctionID::GetVehicleDataID:
      if(mt==messageType::request)
        return GetVehicleData_requestMarshaller::toJSON(* static_cast<const GetVehicleData_request*>(msg));
      if(mt==messageType::response)
         return GetVehicleData_responseMarshaller::toJSON(* static_cast<const GetVehicleData_response*>(msg));
      return j;
    case FunctionID::ListFilesID:
      if(mt==messageType::request)
        return ListFiles_requestMarshaller::toJSON(* static_cast<const ListFiles_request*>(msg));
      if(mt==messageType::response)
         return ListFiles_responseMarshaller::toJSON(* static_cast<const ListFiles_response*>(msg));
      return j;
    case FunctionID::OnAppInterfaceUnregisteredID:
      return mt==messageType::notification ? OnAppInterfaceUnregisteredMarshaller::toJSON(* static_cast<const OnAppInterfaceUnregistered*>(msg)) : j;
    case FunctionID::OnAudioPassThruID:
      return mt==messageType::notification ? OnAudioPassThruMarshaller::toJSON(* static_cast<const OnAudioPassThru*>(msg)) : j;
    case FunctionID::OnButtonEventID:
      return mt==messageType::notification ? OnButtonEventMarshaller::toJSON(* static_cast<const OnButtonEvent*>(msg)) : j;
    case FunctionID::OnButtonPressID:
      return mt==messageType::notification ? OnButtonPressMarshaller::toJSON(* static_cast<const OnButtonPress*>(msg)) : j;
    case FunctionID::OnCommandID:
      return mt==messageType::notification ? OnCommandMarshaller::toJSON(* static_cast<const OnCommand*>(msg)) : j;
    case FunctionID::OnDriverDistractionID:
      return mt==messageType::notification ? OnDriverDistractionMarshaller::toJSON(* static_cast<const OnDriverDistraction*>(msg)) : j;
    case FunctionID::OnEncodedSyncPDataID:
      return mt==messageType::notification ? OnEncodedSyncPDataMarshaller::toJSON(* static_cast<const OnEncodedSyncPData*>(msg)) : j;
    case FunctionID::OnHMIStatusID:
      return mt==messageType::notification ? OnHMIStatusMarshaller::toJSON(* static_cast<const OnHMIStatus*>(msg)) : j;
    case FunctionID::OnLanguageChangeID:
      return mt==messageType::notification ? OnLanguageChangeMarshaller::toJSON(* static_cast<const OnLanguageChange*>(msg)) : j;
    case FunctionID::OnPermissionsChangeID:
      return mt==messageType::notification ? OnPermissionsChangeMarshaller::toJSON(* static_cast<const OnPermissionsChange*>(msg)) : j;
    case FunctionID::OnTBTClientStateID:
      return mt==messageType::notification ? OnTBTClientStateMarshaller::toJSON(* static_cast<const OnTBTClientState*>(msg)) : j;
    case FunctionID::OnVehicleDataID:
      return mt==messageType::notification ? OnVehicleDataMarshaller::toJSON(* static_cast<const OnVehicleData*>(msg)) : j;
    case FunctionID::PerformAudioPassThruID:
      if(mt==messageType::request)
        return PerformAudioPassThru_requestMarshaller::toJSON(* static_cast<const PerformAudioPassThru_request*>(msg));
      if(mt==messageType::response)
         return PerformAudioPassThru_responseMarshaller::toJSON(* static_cast<const PerformAudioPassThru_response*>(msg));
      return j;
    case FunctionID::PerformInteractionID:
      if(mt==messageType::request)
        return PerformInteraction_requestMarshaller::toJSON(* static_cast<const PerformInteraction_request*>(msg));
      if(mt==messageType::response)
         return PerformInteraction_responseMarshaller::toJSON(* static_cast<const PerformInteraction_response*>(msg));
      return j;
    case FunctionID::PutFileID:
      if(mt==messageType::request)
        return PutFile_requestMarshaller::toJSON(* static_cast<const PutFile_request*>(msg));
      if(mt==messageType::response)
         return PutFile_responseMarshaller::toJSON(* static_cast<const PutFile_response*>(msg));
      return j;
    case FunctionID::ReadDIDID:
      if(mt==messageType::request)
        return ReadDID_requestMarshaller::toJSON(* static_cast<const ReadDID_request*>(msg));
      if(mt==messageType::response)
         return ReadDID_responseMarshaller::toJSON(* static_cast<const ReadDID_response*>(msg));
      return j;
    case FunctionID::RegisterAppInterfaceID:
      if(mt==messageType::request)
        return RegisterAppInterface_requestMarshaller::toJSON(* static_cast<const RegisterAppInterface_request*>(msg));
      if(mt==messageType::response)
         return RegisterAppInterface_responseMarshaller::toJSON(* static_cast<const RegisterAppInterface_response*>(msg));
      return j;
    case FunctionID::ResetGlobalPropertiesID:
      if(mt==messageType::request)
        return ResetGlobalProperties_requestMarshaller::toJSON(* static_cast<const ResetGlobalProperties_request*>(msg));
      if(mt==messageType::response)
         return ResetGlobalProperties_responseMarshaller::toJSON(* static_cast<const ResetGlobalProperties_response*>(msg));
      return j;
    case FunctionID::ScrollableMessageID:
      if(mt==messageType::request)
        return ScrollableMessage_requestMarshaller::toJSON(* static_cast<const ScrollableMessage_request*>(msg));
      if(mt==messageType::response)
         return ScrollableMessage_responseMarshaller::toJSON(* static_cast<const ScrollableMessage_response*>(msg));
      return j;
    case FunctionID::SetAppIconID:
      if(mt==messageType::request)
        return SetAppIcon_requestMarshaller::toJSON(* static_cast<const SetAppIcon_request*>(msg));
      if(mt==messageType::response)
         return SetAppIcon_responseMarshaller::toJSON(* static_cast<const SetAppIcon_response*>(msg));
      return j;
    case FunctionID::SetDisplayLayoutID:
      if(mt==messageType::request)
        return SetDisplayLayout_requestMarshaller::toJSON(* static_cast<const SetDisplayLayout_request*>(msg));
      if(mt==messageType::response)
         return SetDisplayLayout_responseMarshaller::toJSON(* static_cast<const SetDisplayLayout_response*>(msg));
      return j;
    case FunctionID::SetGlobalPropertiesID:
      if(mt==messageType::request)
        return SetGlobalProperties_requestMarshaller::toJSON(* static_cast<const SetGlobalProperties_request*>(msg));
      if(mt==messageType::response)
         return SetGlobalProperties_responseMarshaller::toJSON(* static_cast<const SetGlobalProperties_response*>(msg));
      return j;
    case FunctionID::SetMediaClockTimerID:
      if(mt==messageType::request)
        return SetMediaClockTimer_requestMarshaller::toJSON(* static_cast<const SetMediaClockTimer_request*>(msg));
      if(mt==messageType::response)
         return SetMediaClockTimer_responseMarshaller::toJSON(* static_cast<const SetMediaClockTimer_response*>(msg));
      return j;
    case FunctionID::ShowID:
      if(mt==messageType::request)
        return Show_requestMarshaller::toJSON(* static_cast<const Show_request*>(msg));
      if(mt==messageType::response)
         return Show_responseMarshaller::toJSON(* static_cast<const Show_response*>(msg));
      return j;
    case FunctionID::ShowConstantTBTID:
      if(mt==messageType::request)
        return ShowConstantTBT_requestMarshaller::toJSON(* static_cast<const ShowConstantTBT_request*>(msg));
      if(mt==messageType::response)
         return ShowConstantTBT_responseMarshaller::toJSON(* static_cast<const ShowConstantTBT_response*>(msg));
      return j;
    case FunctionID::SliderID:
      if(mt==messageType::request)
        return Slider_requestMarshaller::toJSON(* static_cast<const Slider_request*>(msg));
      if(mt==messageType::response)
         return Slider_responseMarshaller::toJSON(* static_cast<const Slider_response*>(msg));
      return j;
    case FunctionID::SpeakID:
      if(mt==messageType::request)
        return Speak_requestMarshaller::toJSON(* static_cast<const Speak_request*>(msg));
      if(mt==messageType::response)
         return Speak_responseMarshaller::toJSON(* static_cast<const Speak_response*>(msg));
      return j;
    case FunctionID::SubscribeButtonID:
      if(mt==messageType::request)
        return SubscribeButton_requestMarshaller::toJSON(* static_cast<const SubscribeButton_request*>(msg));
      if(mt==messageType::response)
         return SubscribeButton_responseMarshaller::toJSON(* static_cast<const SubscribeButton_response*>(msg));
      return j;
    case FunctionID::SubscribeVehicleDataID:
      if(mt==messageType::request)
        return SubscribeVehicleData_requestMarshaller::toJSON(* static_cast<const SubscribeVehicleData_request*>(msg));
      if(mt==messageType::response)
         return SubscribeVehicleData_responseMarshaller::toJSON(* static_cast<const SubscribeVehicleData_response*>(msg));
      return j;
    case FunctionID::UnregisterAppInterfaceID:
      if(mt==messageType::request)
        return UnregisterAppInterface_requestMarshaller::toJSON(* static_cast<const UnregisterAppInterface_request*>(msg));
      if(mt==messageType::response)
         return UnregisterAppInterface_responseMarshaller::toJSON(* static_cast<const UnregisterAppInterface_response*>(msg));
      return j;
    case FunctionID::UnsubscribeButtonID:
      if(mt==messageType::request)
        return UnsubscribeButton_requestMarshaller::toJSON(* static_cast<const UnsubscribeButton_request*>(msg));
      if(mt==messageType::response)
         return UnsubscribeButton_responseMarshaller::toJSON(* static_cast<const UnsubscribeButton_response*>(msg));
      return j;
    case FunctionID::UnsubscribeVehicleDataID:
      if(mt==messageType::request)
        return UnsubscribeVehicleData_requestMarshaller::toJSON(* static_cast<const UnsubscribeVehicleData_request*>(msg));
      if(mt==messageType::response)
         return UnsubscribeVehicleData_responseMarshaller::toJSON(* static_cast<const UnsubscribeVehicleData_response*>(msg));
      return j;
    case FunctionID::UpdateTurnListID:
      if(mt==messageType::request)
        return UpdateTurnList_requestMarshaller::toJSON(* static_cast<const UpdateTurnList_request*>(msg));
      if(mt==messageType::response)
         return UpdateTurnList_responseMarshaller::toJSON(* static_cast<const UpdateTurnList_response*>(msg));
      return j;
    default:;
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
DialNumber_requestMarshaller Marshaller::mDialNumber_request;
DialNumber_responseMarshaller Marshaller::mDialNumber_response;
PerformAudioPassThru_requestMarshaller Marshaller::mPerformAudioPassThru_request;
PerformAudioPassThru_responseMarshaller Marshaller::mPerformAudioPassThru_response;
EndAudioPassThru_requestMarshaller Marshaller::mEndAudioPassThru_request;
EndAudioPassThru_responseMarshaller Marshaller::mEndAudioPassThru_response;
SubscribeButton_requestMarshaller Marshaller::mSubscribeButton_request;
SubscribeButton_responseMarshaller Marshaller::mSubscribeButton_response;
UnsubscribeButton_requestMarshaller Marshaller::mUnsubscribeButton_request;
UnsubscribeButton_responseMarshaller Marshaller::mUnsubscribeButton_response;
SubscribeVehicleData_requestMarshaller Marshaller::mSubscribeVehicleData_request;
SubscribeVehicleData_responseMarshaller Marshaller::mSubscribeVehicleData_response;
UnsubscribeVehicleData_requestMarshaller Marshaller::mUnsubscribeVehicleData_request;
UnsubscribeVehicleData_responseMarshaller Marshaller::mUnsubscribeVehicleData_response;
GetVehicleData_requestMarshaller Marshaller::mGetVehicleData_request;
GetVehicleData_responseMarshaller Marshaller::mGetVehicleData_response;
ReadDID_requestMarshaller Marshaller::mReadDID_request;
ReadDID_responseMarshaller Marshaller::mReadDID_response;
GetDTCs_requestMarshaller Marshaller::mGetDTCs_request;
GetDTCs_responseMarshaller Marshaller::mGetDTCs_response;
ScrollableMessage_requestMarshaller Marshaller::mScrollableMessage_request;
ScrollableMessage_responseMarshaller Marshaller::mScrollableMessage_response;
Slider_requestMarshaller Marshaller::mSlider_request;
Slider_responseMarshaller Marshaller::mSlider_response;
ShowConstantTBT_requestMarshaller Marshaller::mShowConstantTBT_request;
ShowConstantTBT_responseMarshaller Marshaller::mShowConstantTBT_response;
AlertManeuver_requestMarshaller Marshaller::mAlertManeuver_request;
AlertManeuver_responseMarshaller Marshaller::mAlertManeuver_response;
UpdateTurnList_requestMarshaller Marshaller::mUpdateTurnList_request;
UpdateTurnList_responseMarshaller Marshaller::mUpdateTurnList_response;
ChangeRegistration_requestMarshaller Marshaller::mChangeRegistration_request;
ChangeRegistration_responseMarshaller Marshaller::mChangeRegistration_response;
GenericResponse_responseMarshaller Marshaller::mGenericResponse_response;
PutFile_requestMarshaller Marshaller::mPutFile_request;
PutFile_responseMarshaller Marshaller::mPutFile_response;
DeleteFile_requestMarshaller Marshaller::mDeleteFile_request;
DeleteFile_responseMarshaller Marshaller::mDeleteFile_response;
ListFiles_requestMarshaller Marshaller::mListFiles_request;
ListFiles_responseMarshaller Marshaller::mListFiles_response;
SetAppIcon_requestMarshaller Marshaller::mSetAppIcon_request;
SetAppIcon_responseMarshaller Marshaller::mSetAppIcon_response;
SetDisplayLayout_requestMarshaller Marshaller::mSetDisplayLayout_request;
SetDisplayLayout_responseMarshaller Marshaller::mSetDisplayLayout_response;
OnHMIStatusMarshaller Marshaller::mOnHMIStatus;
OnAppInterfaceUnregisteredMarshaller Marshaller::mOnAppInterfaceUnregistered;
OnButtonEventMarshaller Marshaller::mOnButtonEvent;
OnButtonPressMarshaller Marshaller::mOnButtonPress;
OnVehicleDataMarshaller Marshaller::mOnVehicleData;
OnCommandMarshaller Marshaller::mOnCommand;
OnEncodedSyncPDataMarshaller Marshaller::mOnEncodedSyncPData;
OnTBTClientStateMarshaller Marshaller::mOnTBTClientState;
OnDriverDistractionMarshaller Marshaller::mOnDriverDistraction;
OnPermissionsChangeMarshaller Marshaller::mOnPermissionsChange;
OnAudioPassThruMarshaller Marshaller::mOnAudioPassThru;
OnLanguageChangeMarshaller Marshaller::mOnLanguageChange;
