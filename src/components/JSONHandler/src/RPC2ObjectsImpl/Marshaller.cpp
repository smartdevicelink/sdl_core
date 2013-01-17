#include <cstring>
#include "JSONHandler/RPC2Objects/Marshaller.h"

#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/ActivateApp.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/ActivateAppResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/DeactivateApp.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/DeactivateAppResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/GetAppList.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/GetAppListResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/GetDeviceList.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/GetDeviceListResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/OnAppRegistered.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/OnAppUnregistered.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/OnDeviceListUpdated.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/SendData.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/SendDataResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Buttons/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Buttons/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Buttons/OnButtonEvent.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Buttons/OnButtonPress.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Navigation/AlertManeuver.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Navigation/AlertManeuverResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Navigation/OnTBTClientState.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Navigation/ShowConstantTBT.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Navigation/ShowConstantTBTResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Navigation/UpdateTurnList.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Navigation/UpdateTurnListResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Phone/DialNumber.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/Phone/DialNumberResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/ChangeRegistration.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/ChangeRegistrationResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/GetLanguage.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/GetLanguageResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/OnAudioPassThru.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/OnLanguageChange.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/Speak.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/TTS/SpeakResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddSubMenu.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddSubMenuResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/Alert.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AlertResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ChangeRegistration.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ChangeRegistrationResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/CreateInteractionChoiceSet.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/CreateInteractionChoiceSetResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteInteractionChoiceSet.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteInteractionChoiceSetResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteSubMenu.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteSubMenuResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/EndAudioPassThru.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/EndAudioPassThruResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetLanguage.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetLanguageResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/PerformAudioPassThru.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/PerformAudioPassThruResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnDeviceChosen.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnDriverDistraction.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnLanguageChange.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnReady.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnSystemContext.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/PerformInteraction.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/PerformInteractionResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ResetGlobalProperties.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ResetGlobalPropertiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ScrollableMessage.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ScrollableMessageResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetAppIcon.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetAppIconResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetDisplayLayout.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetDisplayLayoutResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetGlobalProperties.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetGlobalPropertiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetMediaClockTimer.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetMediaClockTimerResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/Show.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ShowResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/Slider.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SliderResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/AddCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/AddCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/ChangeRegistration.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/ChangeRegistrationResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/DeleteCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/DeleteCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/GetLanguage.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/GetLanguageResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/OnCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VR/OnLanguageChange.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/GetDTCs.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/GetDTCsResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/GetVehicleData.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/GetVehicleDataResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/GetVehicleType.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/GetVehicleTypeResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/OnVehicleData.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/ReadDID.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/ReadDIDResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/SubscribeVehicleData.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/SubscribeVehicleDataResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/UnsubscribeVehicleData.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/VehicleInfo/UnsubscribeVehicleDataResponse.h"

#include "../src/../src/RPC2ObjectsImpl/Marshaller.inc"

/*
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/
#include <iostream>
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
      case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONDEVICELISTUPDATED:
      {
        NsRPC2Communication::AppLinkCore::OnDeviceListUpdated *rv=new NsRPC2Communication::AppLinkCore::OnDeviceListUpdated;
        return NsRPC2Communication::AppLinkCore::OnDeviceListUpdatedMarshaller::fromJSON(json,*rv) ? rv : 0;
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
      case METHOD_NSRPC2COMMUNICATION_NAVIGATION__ONTBTCLIENTSTATE:
      {
        NsRPC2Communication::Navigation::OnTBTClientState *rv=new NsRPC2Communication::Navigation::OnTBTClientState;
        return NsRPC2Communication::Navigation::OnTBTClientStateMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_TTS__ONAUDIOPASSTHRU:
      {
        NsRPC2Communication::TTS::OnAudioPassThru *rv=new NsRPC2Communication::TTS::OnAudioPassThru;
        return NsRPC2Communication::TTS::OnAudioPassThruMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_TTS__ONLANGUAGECHANGE:
      {
        NsRPC2Communication::TTS::OnLanguageChange *rv=new NsRPC2Communication::TTS::OnLanguageChange;
        return NsRPC2Communication::TTS::OnLanguageChangeMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ONCOMMAND:
      {
        NsRPC2Communication::UI::OnCommand *rv=new NsRPC2Communication::UI::OnCommand;
        return NsRPC2Communication::UI::OnCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ONDEVICECHOSEN:
      {
        NsRPC2Communication::UI::OnDeviceChosen *rv=new NsRPC2Communication::UI::OnDeviceChosen;
        return NsRPC2Communication::UI::OnDeviceChosenMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ONDRIVERDISTRACTION:
      {
        NsRPC2Communication::UI::OnDriverDistraction *rv=new NsRPC2Communication::UI::OnDriverDistraction;
        return NsRPC2Communication::UI::OnDriverDistractionMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__ONLANGUAGECHANGE:
      {
        NsRPC2Communication::UI::OnLanguageChange *rv=new NsRPC2Communication::UI::OnLanguageChange;
        return NsRPC2Communication::UI::OnLanguageChangeMarshaller::fromJSON(json,*rv) ? rv : 0;
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
      case METHOD_NSRPC2COMMUNICATION_VR__ONLANGUAGECHANGE:
      {
        NsRPC2Communication::VR::OnLanguageChange *rv=new NsRPC2Communication::VR::OnLanguageChange;
        return NsRPC2Communication::VR::OnLanguageChangeMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__ONVEHICLEDATA:
      {
        NsRPC2Communication::VehicleInfo::OnVehicleData *rv=new NsRPC2Communication::VehicleInfo::OnVehicleData;
        return NsRPC2Communication::VehicleInfo::OnVehicleDataMarshaller::fromJSON(json,*rv) ? rv : 0;
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
      case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__DEACTIVATEAPP:
      {
        NsRPC2Communication::AppLinkCore::DeactivateApp *rv=new NsRPC2Communication::AppLinkCore::DeactivateApp;
        return NsRPC2Communication::AppLinkCore::DeactivateAppMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLIST:
      {
        NsRPC2Communication::AppLinkCore::GetAppList *rv=new NsRPC2Communication::AppLinkCore::GetAppList;
        return NsRPC2Communication::AppLinkCore::GetAppListMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELIST:
      {
        NsRPC2Communication::AppLinkCore::GetDeviceList *rv=new NsRPC2Communication::AppLinkCore::GetDeviceList;
        return NsRPC2Communication::AppLinkCore::GetDeviceListMarshaller::fromJSON(json,*rv) ? rv : 0;
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
      case METHOD_NSRPC2COMMUNICATION_NAVIGATION__ALERTMANEUVER:
      {
        NsRPC2Communication::Navigation::AlertManeuver *rv=new NsRPC2Communication::Navigation::AlertManeuver;
        return NsRPC2Communication::Navigation::AlertManeuverMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_NAVIGATION__SHOWCONSTANTTBT:
      {
        NsRPC2Communication::Navigation::ShowConstantTBT *rv=new NsRPC2Communication::Navigation::ShowConstantTBT;
        return NsRPC2Communication::Navigation::ShowConstantTBTMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_NAVIGATION__UPDATETURNLIST:
      {
        NsRPC2Communication::Navigation::UpdateTurnList *rv=new NsRPC2Communication::Navigation::UpdateTurnList;
        return NsRPC2Communication::Navigation::UpdateTurnListMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_PHONE__DIALNUMBER:
      {
        NsRPC2Communication::Phone::DialNumber *rv=new NsRPC2Communication::Phone::DialNumber;
        return NsRPC2Communication::Phone::DialNumberMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_TTS__CHANGEREGISTRATION:
      {
        NsRPC2Communication::TTS::ChangeRegistration *rv=new NsRPC2Communication::TTS::ChangeRegistration;
        return NsRPC2Communication::TTS::ChangeRegistrationMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIES:
      {
        NsRPC2Communication::TTS::GetCapabilities *rv=new NsRPC2Communication::TTS::GetCapabilities;
        return NsRPC2Communication::TTS::GetCapabilitiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGE:
      {
        NsRPC2Communication::TTS::GetLanguage *rv=new NsRPC2Communication::TTS::GetLanguage;
        return NsRPC2Communication::TTS::GetLanguageMarshaller::fromJSON(json,*rv) ? rv : 0;
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
      case METHOD_NSRPC2COMMUNICATION_UI__CHANGEREGISTRATION:
      {
        NsRPC2Communication::UI::ChangeRegistration *rv=new NsRPC2Communication::UI::ChangeRegistration;
        return NsRPC2Communication::UI::ChangeRegistrationMarshaller::fromJSON(json,*rv) ? rv : 0;
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
      case METHOD_NSRPC2COMMUNICATION_UI__ENDAUDIOPASSTHRU:
      {
        NsRPC2Communication::UI::EndAudioPassThru *rv=new NsRPC2Communication::UI::EndAudioPassThru;
        return NsRPC2Communication::UI::EndAudioPassThruMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIES:
      {
        NsRPC2Communication::UI::GetCapabilities *rv=new NsRPC2Communication::UI::GetCapabilities;
        return NsRPC2Communication::UI::GetCapabilitiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__GETLANGUAGE:
      {
        NsRPC2Communication::UI::GetLanguage *rv=new NsRPC2Communication::UI::GetLanguage;
        return NsRPC2Communication::UI::GetLanguageMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__PERFORMAUDIOPASSTHRU:
      {
        NsRPC2Communication::UI::PerformAudioPassThru *rv=new NsRPC2Communication::UI::PerformAudioPassThru;
        return NsRPC2Communication::UI::PerformAudioPassThruMarshaller::fromJSON(json,*rv) ? rv : 0;
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
      case METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGE:
      {
        NsRPC2Communication::UI::ScrollableMessage *rv=new NsRPC2Communication::UI::ScrollableMessage;
        return NsRPC2Communication::UI::ScrollableMessageMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__SETAPPICON:
      {
        NsRPC2Communication::UI::SetAppIcon *rv=new NsRPC2Communication::UI::SetAppIcon;
        return NsRPC2Communication::UI::SetAppIconMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUT:
      {
        NsRPC2Communication::UI::SetDisplayLayout *rv=new NsRPC2Communication::UI::SetDisplayLayout;
        return NsRPC2Communication::UI::SetDisplayLayoutMarshaller::fromJSON(json,*rv) ? rv : 0;
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
      case METHOD_NSRPC2COMMUNICATION_UI__SLIDER:
      {
        NsRPC2Communication::UI::Slider *rv=new NsRPC2Communication::UI::Slider;
        return NsRPC2Communication::UI::SliderMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMAND:
      {
        NsRPC2Communication::VR::AddCommand *rv=new NsRPC2Communication::VR::AddCommand;
        return NsRPC2Communication::VR::AddCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VR__CHANGEREGISTRATION:
      {
        NsRPC2Communication::VR::ChangeRegistration *rv=new NsRPC2Communication::VR::ChangeRegistration;
        return NsRPC2Communication::VR::ChangeRegistrationMarshaller::fromJSON(json,*rv) ? rv : 0;
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
      case METHOD_NSRPC2COMMUNICATION_VR__GETLANGUAGE:
      {
        NsRPC2Communication::VR::GetLanguage *rv=new NsRPC2Communication::VR::GetLanguage;
        return NsRPC2Communication::VR::GetLanguageMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCS:
      {
        NsRPC2Communication::VehicleInfo::GetDTCs *rv=new NsRPC2Communication::VehicleInfo::GetDTCs;
        return NsRPC2Communication::VehicleInfo::GetDTCsMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLEDATA:
      {
        NsRPC2Communication::VehicleInfo::GetVehicleData *rv=new NsRPC2Communication::VehicleInfo::GetVehicleData;
        return NsRPC2Communication::VehicleInfo::GetVehicleDataMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLETYPE:
      {
        NsRPC2Communication::VehicleInfo::GetVehicleType *rv=new NsRPC2Communication::VehicleInfo::GetVehicleType;
        return NsRPC2Communication::VehicleInfo::GetVehicleTypeMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDID:
      {
        NsRPC2Communication::VehicleInfo::ReadDID *rv=new NsRPC2Communication::VehicleInfo::ReadDID;
        return NsRPC2Communication::VehicleInfo::ReadDIDMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__SUBSCRIBEVEHICLEDATA:
      {
        NsRPC2Communication::VehicleInfo::SubscribeVehicleData *rv=new NsRPC2Communication::VehicleInfo::SubscribeVehicleData;
        return NsRPC2Communication::VehicleInfo::SubscribeVehicleDataMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__UNSUBSCRIBEVEHICLEDATA:
      {
        NsRPC2Communication::VehicleInfo::UnsubscribeVehicleData *rv=new NsRPC2Communication::VehicleInfo::UnsubscribeVehicleData;
        return NsRPC2Communication::VehicleInfo::UnsubscribeVehicleDataMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_NSRPC2COMMUNICATION_UI__GETSUPPORTEDLANGUAGES:
      {
        NsRPC2Communication::UI::GetSupportedLanguages * rv = new NsRPC2Communication::UI::GetSupportedLanguages;
        return NsRPC2Communication::UI::GetSupportedLanguagesMarshaller::fromJSON(json, *rv) ? rv : 0;
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
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__DEACTIVATEAPPRESPONSE:
    {
      NsRPC2Communication::AppLinkCore::DeactivateAppResponse *rv=new NsRPC2Communication::AppLinkCore::DeactivateAppResponse;
      return NsRPC2Communication::AppLinkCore::DeactivateAppResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLISTRESPONSE:
    {
      NsRPC2Communication::AppLinkCore::GetAppListResponse *rv=new NsRPC2Communication::AppLinkCore::GetAppListResponse;
      return NsRPC2Communication::AppLinkCore::GetAppListResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELISTRESPONSE:
    {
      NsRPC2Communication::AppLinkCore::GetDeviceListResponse *rv=new NsRPC2Communication::AppLinkCore::GetDeviceListResponse;
      return NsRPC2Communication::AppLinkCore::GetDeviceListResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
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
    case METHOD_NSRPC2COMMUNICATION_NAVIGATION__ALERTMANEUVERRESPONSE:
    {
      NsRPC2Communication::Navigation::AlertManeuverResponse *rv=new NsRPC2Communication::Navigation::AlertManeuverResponse;
      return NsRPC2Communication::Navigation::AlertManeuverResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_NAVIGATION__SHOWCONSTANTTBTRESPONSE:
    {
      NsRPC2Communication::Navigation::ShowConstantTBTResponse *rv=new NsRPC2Communication::Navigation::ShowConstantTBTResponse;
      return NsRPC2Communication::Navigation::ShowConstantTBTResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_NAVIGATION__UPDATETURNLISTRESPONSE:
    {
      NsRPC2Communication::Navigation::UpdateTurnListResponse *rv=new NsRPC2Communication::Navigation::UpdateTurnListResponse;
      return NsRPC2Communication::Navigation::UpdateTurnListResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_PHONE__DIALNUMBERRESPONSE:
    {
      NsRPC2Communication::Phone::DialNumberResponse *rv=new NsRPC2Communication::Phone::DialNumberResponse;
      return NsRPC2Communication::Phone::DialNumberResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_TTS__CHANGEREGISTRATIONRESPONSE:
    {
      NsRPC2Communication::TTS::ChangeRegistrationResponse *rv=new NsRPC2Communication::TTS::ChangeRegistrationResponse;
      return NsRPC2Communication::TTS::ChangeRegistrationResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIESRESPONSE:
    {
      NsRPC2Communication::TTS::GetCapabilitiesResponse *rv=new NsRPC2Communication::TTS::GetCapabilitiesResponse;
      return NsRPC2Communication::TTS::GetCapabilitiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGERESPONSE:
    {
      NsRPC2Communication::TTS::GetLanguageResponse *rv=new NsRPC2Communication::TTS::GetLanguageResponse;
      return NsRPC2Communication::TTS::GetLanguageResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
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
    case METHOD_NSRPC2COMMUNICATION_UI__CHANGEREGISTRATIONRESPONSE:
    {
      NsRPC2Communication::UI::ChangeRegistrationResponse *rv=new NsRPC2Communication::UI::ChangeRegistrationResponse;
      return NsRPC2Communication::UI::ChangeRegistrationResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
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
    case METHOD_NSRPC2COMMUNICATION_UI__ENDAUDIOPASSTHRURESPONSE:
    {
      NsRPC2Communication::UI::EndAudioPassThruResponse *rv=new NsRPC2Communication::UI::EndAudioPassThruResponse;
      return NsRPC2Communication::UI::EndAudioPassThruResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE:
    {
      NsRPC2Communication::UI::GetCapabilitiesResponse *rv=new NsRPC2Communication::UI::GetCapabilitiesResponse;
      return NsRPC2Communication::UI::GetCapabilitiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__GETLANGUAGERESPONSE:
    {
      NsRPC2Communication::UI::GetLanguageResponse *rv=new NsRPC2Communication::UI::GetLanguageResponse;
      return NsRPC2Communication::UI::GetLanguageResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__PERFORMAUDIOPASSTHRURESPONSE:
    {
      NsRPC2Communication::UI::PerformAudioPassThruResponse *rv=new NsRPC2Communication::UI::PerformAudioPassThruResponse;
      return NsRPC2Communication::UI::PerformAudioPassThruResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
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
    case METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGERESPONSE:
    {
      NsRPC2Communication::UI::ScrollableMessageResponse *rv=new NsRPC2Communication::UI::ScrollableMessageResponse;
      return NsRPC2Communication::UI::ScrollableMessageResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__SETAPPICONRESPONSE:
    {
      NsRPC2Communication::UI::SetAppIconResponse *rv=new NsRPC2Communication::UI::SetAppIconResponse;
      return NsRPC2Communication::UI::SetAppIconResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUTRESPONSE:
    {
      NsRPC2Communication::UI::SetDisplayLayoutResponse *rv=new NsRPC2Communication::UI::SetDisplayLayoutResponse;
      return NsRPC2Communication::UI::SetDisplayLayoutResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
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
    case METHOD_NSRPC2COMMUNICATION_UI__SLIDERRESPONSE:
    {
      NsRPC2Communication::UI::SliderResponse *rv=new NsRPC2Communication::UI::SliderResponse;
      return NsRPC2Communication::UI::SliderResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMANDRESPONSE:
    {
      NsRPC2Communication::VR::AddCommandResponse *rv=new NsRPC2Communication::VR::AddCommandResponse;
      return NsRPC2Communication::VR::AddCommandResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_VR__CHANGEREGISTRATIONRESPONSE:
    {
      NsRPC2Communication::VR::ChangeRegistrationResponse *rv=new NsRPC2Communication::VR::ChangeRegistrationResponse;
      return NsRPC2Communication::VR::ChangeRegistrationResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
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
    case METHOD_NSRPC2COMMUNICATION_VR__GETLANGUAGERESPONSE:
    {
      NsRPC2Communication::VR::GetLanguageResponse *rv=new NsRPC2Communication::VR::GetLanguageResponse;
      return NsRPC2Communication::VR::GetLanguageResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCSRESPONSE:
    {
      NsRPC2Communication::VehicleInfo::GetDTCsResponse *rv=new NsRPC2Communication::VehicleInfo::GetDTCsResponse;
      return NsRPC2Communication::VehicleInfo::GetDTCsResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLEDATARESPONSE:
    {
      NsRPC2Communication::VehicleInfo::GetVehicleDataResponse *rv=new NsRPC2Communication::VehicleInfo::GetVehicleDataResponse;
      return NsRPC2Communication::VehicleInfo::GetVehicleDataResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLETYPERESPONSE:
    {
      NsRPC2Communication::VehicleInfo::GetVehicleTypeResponse *rv=new NsRPC2Communication::VehicleInfo::GetVehicleTypeResponse;
      return NsRPC2Communication::VehicleInfo::GetVehicleTypeResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDIDRESPONSE:
    {
      NsRPC2Communication::VehicleInfo::ReadDIDResponse *rv=new NsRPC2Communication::VehicleInfo::ReadDIDResponse;
      return NsRPC2Communication::VehicleInfo::ReadDIDResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__SUBSCRIBEVEHICLEDATARESPONSE:
    {
      NsRPC2Communication::VehicleInfo::SubscribeVehicleDataResponse *rv=new NsRPC2Communication::VehicleInfo::SubscribeVehicleDataResponse;
      return NsRPC2Communication::VehicleInfo::SubscribeVehicleDataResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__UNSUBSCRIBEVEHICLEDATARESPONSE:
    {
      NsRPC2Communication::VehicleInfo::UnsubscribeVehicleDataResponse *rv=new NsRPC2Communication::VehicleInfo::UnsubscribeVehicleDataResponse;
      return NsRPC2Communication::VehicleInfo::UnsubscribeVehicleDataResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_NSRPC2COMMUNICATION_UI__GETSUPPORTEDLANGUAGESRESPONSE:
    {
      NsRPC2Communication::UI::GetSupportedLanguagesResponse *rv = new NsRPC2Communication::UI::GetSupportedLanguagesResponse;
      return NsRPC2Communication::UI::GetSupportedLanguagesResponseMarshaller::fromJSON(json, *rv) ? rv : 0;
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
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONDEVICELISTUPDATED:
      return NsRPC2Communication::AppLinkCore::OnDeviceListUpdatedMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::OnDeviceListUpdated*>(msg));
    case METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONEVENT:
      return NsRPC2Communication::Buttons::OnButtonEventMarshaller::toJSON(*static_cast<const NsRPC2Communication::Buttons::OnButtonEvent*>(msg));
    case METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONPRESS:
      return NsRPC2Communication::Buttons::OnButtonPressMarshaller::toJSON(*static_cast<const NsRPC2Communication::Buttons::OnButtonPress*>(msg));
    case METHOD_NSRPC2COMMUNICATION_NAVIGATION__ONTBTCLIENTSTATE:
      return NsRPC2Communication::Navigation::OnTBTClientStateMarshaller::toJSON(*static_cast<const NsRPC2Communication::Navigation::OnTBTClientState*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__ONAUDIOPASSTHRU:
      return NsRPC2Communication::TTS::OnAudioPassThruMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::OnAudioPassThru*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__ONLANGUAGECHANGE:
      return NsRPC2Communication::TTS::OnLanguageChangeMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::OnLanguageChange*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONCOMMAND:
      return NsRPC2Communication::UI::OnCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnCommand*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONDEVICECHOSEN:
      return NsRPC2Communication::UI::OnDeviceChosenMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnDeviceChosen*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONDRIVERDISTRACTION:
      return NsRPC2Communication::UI::OnDriverDistractionMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnDriverDistraction*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONLANGUAGECHANGE:
      return NsRPC2Communication::UI::OnLanguageChangeMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnLanguageChange*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONREADY:
      return NsRPC2Communication::UI::OnReadyMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnReady*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ONSYSTEMCONTEXT:
      return NsRPC2Communication::UI::OnSystemContextMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::OnSystemContext*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__ONCOMMAND:
      return NsRPC2Communication::VR::OnCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::OnCommand*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__ONLANGUAGECHANGE:
      return NsRPC2Communication::VR::OnLanguageChangeMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::OnLanguageChange*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__ONVEHICLEDATA:
      return NsRPC2Communication::VehicleInfo::OnVehicleDataMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::OnVehicleData*>(msg));
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
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__DEACTIVATEAPP:
      return NsRPC2Communication::AppLinkCore::DeactivateAppMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::DeactivateApp*>(msg));
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLIST:
      return NsRPC2Communication::AppLinkCore::GetAppListMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::GetAppList*>(msg));
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELIST:
      return NsRPC2Communication::AppLinkCore::GetDeviceListMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::GetDeviceList*>(msg));
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATA:
      return NsRPC2Communication::AppLinkCore::SendDataMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::SendData*>(msg));
    case METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIES:
      return NsRPC2Communication::Buttons::GetCapabilitiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::Buttons::GetCapabilities*>(msg));
    case METHOD_NSRPC2COMMUNICATION_NAVIGATION__ALERTMANEUVER:
      return NsRPC2Communication::Navigation::AlertManeuverMarshaller::toJSON(*static_cast<const NsRPC2Communication::Navigation::AlertManeuver*>(msg));
    case METHOD_NSRPC2COMMUNICATION_NAVIGATION__SHOWCONSTANTTBT:
      return NsRPC2Communication::Navigation::ShowConstantTBTMarshaller::toJSON(*static_cast<const NsRPC2Communication::Navigation::ShowConstantTBT*>(msg));
    case METHOD_NSRPC2COMMUNICATION_NAVIGATION__UPDATETURNLIST:
      return NsRPC2Communication::Navigation::UpdateTurnListMarshaller::toJSON(*static_cast<const NsRPC2Communication::Navigation::UpdateTurnList*>(msg));
    case METHOD_NSRPC2COMMUNICATION_PHONE__DIALNUMBER:
      return NsRPC2Communication::Phone::DialNumberMarshaller::toJSON(*static_cast<const NsRPC2Communication::Phone::DialNumber*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__CHANGEREGISTRATION:
      return NsRPC2Communication::TTS::ChangeRegistrationMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::ChangeRegistration*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIES:
      return NsRPC2Communication::TTS::GetCapabilitiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::GetCapabilities*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGE:
      return NsRPC2Communication::TTS::GetLanguageMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::GetLanguage*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__SPEAK:
      return NsRPC2Communication::TTS::SpeakMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::Speak*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMAND:
      return NsRPC2Communication::UI::AddCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::AddCommand*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENU:
      return NsRPC2Communication::UI::AddSubMenuMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::AddSubMenu*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ALERT:
      return NsRPC2Communication::UI::AlertMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::Alert*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__CHANGEREGISTRATION:
      return NsRPC2Communication::UI::ChangeRegistrationMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::ChangeRegistration*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESET:
      return NsRPC2Communication::UI::CreateInteractionChoiceSetMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::CreateInteractionChoiceSet*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMAND:
      return NsRPC2Communication::UI::DeleteCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteCommand*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESET:
      return NsRPC2Communication::UI::DeleteInteractionChoiceSetMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteInteractionChoiceSet*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENU:
      return NsRPC2Communication::UI::DeleteSubMenuMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteSubMenu*>(msg));
      case METHOD_NSRPC2COMMUNICATION_UI__ENDAUDIOPASSTHRU:
      return NsRPC2Communication::UI::EndAudioPassThruMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::EndAudioPassThru*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIES:
      return NsRPC2Communication::UI::GetCapabilitiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::GetCapabilities*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__GETLANGUAGE:
      return NsRPC2Communication::UI::GetLanguageMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::GetLanguage*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__PERFORMAUDIOPASSTHRU:
      return NsRPC2Communication::UI::PerformAudioPassThruMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::PerformAudioPassThru*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTION:
      return NsRPC2Communication::UI::PerformInteractionMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::PerformInteraction*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIES:
      return NsRPC2Communication::UI::ResetGlobalPropertiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::ResetGlobalProperties*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGE:
      return NsRPC2Communication::UI::ScrollableMessageMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::ScrollableMessage*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETAPPICON:
      return NsRPC2Communication::UI::SetAppIconMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetAppIcon*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUT:
      return NsRPC2Communication::UI::SetDisplayLayoutMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetDisplayLayout*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIES:
      return NsRPC2Communication::UI::SetGlobalPropertiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetGlobalProperties*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMER:
      return NsRPC2Communication::UI::SetMediaClockTimerMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetMediaClockTimer*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SHOW:
      return NsRPC2Communication::UI::ShowMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::Show*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SLIDER:
      return NsRPC2Communication::UI::SliderMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::Slider*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMAND:
      return NsRPC2Communication::VR::AddCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::AddCommand*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__CHANGEREGISTRATION:
      return NsRPC2Communication::VR::ChangeRegistrationMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::ChangeRegistration*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMAND:
      return NsRPC2Communication::VR::DeleteCommandMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::DeleteCommand*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIES:
      return NsRPC2Communication::VR::GetCapabilitiesMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::GetCapabilities*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__GETLANGUAGE:
      return NsRPC2Communication::VR::GetLanguageMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::GetLanguage*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCS:
      return NsRPC2Communication::VehicleInfo::GetDTCsMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::GetDTCs*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLEDATA:
      return NsRPC2Communication::VehicleInfo::GetVehicleDataMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::GetVehicleData*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLETYPE:
      return NsRPC2Communication::VehicleInfo::GetVehicleTypeMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::GetVehicleType*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDID:
      return NsRPC2Communication::VehicleInfo::ReadDIDMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::ReadDID*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__SUBSCRIBEVEHICLEDATA:
      return NsRPC2Communication::VehicleInfo::SubscribeVehicleDataMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::SubscribeVehicleData*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__UNSUBSCRIBEVEHICLEDATA:
      return NsRPC2Communication::VehicleInfo::UnsubscribeVehicleDataMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::UnsubscribeVehicleData*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__GETSUPPORTEDLANGUAGES:
      return NsRPC2Communication::UI::GetSupportedLanguagesMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::GetSupportedLanguages*>(msg));
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
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__DEACTIVATEAPPRESPONSE:
      return NsRPC2Communication::AppLinkCore::DeactivateAppResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::DeactivateAppResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLISTRESPONSE:
      return NsRPC2Communication::AppLinkCore::GetAppListResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::GetAppListResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELISTRESPONSE:
      return NsRPC2Communication::AppLinkCore::GetDeviceListResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::GetDeviceListResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATARESPONSE:
      return NsRPC2Communication::AppLinkCore::SendDataResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::SendDataResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIESRESPONSE:
      return NsRPC2Communication::Buttons::GetCapabilitiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::Buttons::GetCapabilitiesResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_NAVIGATION__ALERTMANEUVERRESPONSE:
      return NsRPC2Communication::Navigation::AlertManeuverResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::Navigation::AlertManeuverResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_NAVIGATION__SHOWCONSTANTTBTRESPONSE:
      return NsRPC2Communication::Navigation::ShowConstantTBTResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::Navigation::ShowConstantTBTResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_NAVIGATION__UPDATETURNLISTRESPONSE:
      return NsRPC2Communication::Navigation::UpdateTurnListResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::Navigation::UpdateTurnListResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_PHONE__DIALNUMBERRESPONSE:
      return NsRPC2Communication::Phone::DialNumberResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::Phone::DialNumberResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__CHANGEREGISTRATIONRESPONSE:
      return NsRPC2Communication::TTS::ChangeRegistrationResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::ChangeRegistrationResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIESRESPONSE:
      return NsRPC2Communication::TTS::GetCapabilitiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::GetCapabilitiesResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGERESPONSE:
      return NsRPC2Communication::TTS::GetLanguageResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::GetLanguageResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_TTS__SPEAKRESPONSE:
      return NsRPC2Communication::TTS::SpeakResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::TTS::SpeakResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMANDRESPONSE:
      return NsRPC2Communication::UI::AddCommandResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::AddCommandResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENURESPONSE:
      return NsRPC2Communication::UI::AddSubMenuResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::AddSubMenuResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE:
      return NsRPC2Communication::UI::AlertResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::AlertResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__CHANGEREGISTRATIONRESPONSE:
      return NsRPC2Communication::UI::ChangeRegistrationResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::ChangeRegistrationResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESETRESPONSE:
      return NsRPC2Communication::UI::CreateInteractionChoiceSetResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::CreateInteractionChoiceSetResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMANDRESPONSE:
      return NsRPC2Communication::UI::DeleteCommandResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteCommandResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESETRESPONSE:
      return NsRPC2Communication::UI::DeleteInteractionChoiceSetResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteInteractionChoiceSetResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENURESPONSE:
      return NsRPC2Communication::UI::DeleteSubMenuResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::DeleteSubMenuResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__ENDAUDIOPASSTHRURESPONSE:
      return NsRPC2Communication::UI::EndAudioPassThruResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::EndAudioPassThruResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE:
      return NsRPC2Communication::UI::GetCapabilitiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::GetCapabilitiesResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__GETLANGUAGERESPONSE:
      return NsRPC2Communication::UI::GetLanguageResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::GetLanguageResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__PERFORMAUDIOPASSTHRURESPONSE:
      return NsRPC2Communication::UI::PerformAudioPassThruResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::PerformAudioPassThruResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTIONRESPONSE:
      return NsRPC2Communication::UI::PerformInteractionResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::PerformInteractionResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIESRESPONSE:
      return NsRPC2Communication::UI::ResetGlobalPropertiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::ResetGlobalPropertiesResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGERESPONSE:
      return NsRPC2Communication::UI::ScrollableMessageResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::ScrollableMessageResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETAPPICONRESPONSE:
      return NsRPC2Communication::UI::SetAppIconResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetAppIconResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUTRESPONSE:
      return NsRPC2Communication::UI::SetDisplayLayoutResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetDisplayLayoutResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIESRESPONSE:
      return NsRPC2Communication::UI::SetGlobalPropertiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetGlobalPropertiesResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMERRESPONSE:
      return NsRPC2Communication::UI::SetMediaClockTimerResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SetMediaClockTimerResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SHOWRESPONSE:
      return NsRPC2Communication::UI::ShowResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::ShowResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__SLIDERRESPONSE:
      return NsRPC2Communication::UI::SliderResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::SliderResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMANDRESPONSE:
      return NsRPC2Communication::VR::AddCommandResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::AddCommandResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__CHANGEREGISTRATIONRESPONSE:
      return NsRPC2Communication::VR::ChangeRegistrationResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::ChangeRegistrationResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMANDRESPONSE:
      return NsRPC2Communication::VR::DeleteCommandResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::DeleteCommandResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIESRESPONSE:
      return NsRPC2Communication::VR::GetCapabilitiesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::GetCapabilitiesResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VR__GETLANGUAGERESPONSE:
      return NsRPC2Communication::VR::GetLanguageResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VR::GetLanguageResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCSRESPONSE:
      return NsRPC2Communication::VehicleInfo::GetDTCsResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::GetDTCsResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLEDATARESPONSE:
      return NsRPC2Communication::VehicleInfo::GetVehicleDataResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::GetVehicleDataResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLETYPERESPONSE:
      return NsRPC2Communication::VehicleInfo::GetVehicleTypeResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::GetVehicleTypeResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDIDRESPONSE:
      return NsRPC2Communication::VehicleInfo::ReadDIDResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::ReadDIDResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__SUBSCRIBEVEHICLEDATARESPONSE:
      return NsRPC2Communication::VehicleInfo::SubscribeVehicleDataResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::SubscribeVehicleDataResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__UNSUBSCRIBEVEHICLEDATARESPONSE:
      return NsRPC2Communication::VehicleInfo::UnsubscribeVehicleDataResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::VehicleInfo::UnsubscribeVehicleDataResponse*>(msg));
    case METHOD_NSRPC2COMMUNICATION_UI__GETSUPPORTEDLANGUAGESRESPONSE:
      return NsRPC2Communication::UI::GetSupportedLanguagesResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::UI::GetSupportedLanguagesResponse*>(msg));
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


const Marshaller::localHash Marshaller::mHashTable[111]=
{
  {"AppLinkCore.ActivateApp",0,&Marshaller::mNsRPC2Communication_AppLinkCore__ActivateApp},
  {"AppLinkCore.ActivateAppResponse",1,&Marshaller::mNsRPC2Communication_AppLinkCore__ActivateAppResponse},
  {"AppLinkCore.DeactivateApp",2,&Marshaller::mNsRPC2Communication_AppLinkCore__DeactivateApp},
  {"AppLinkCore.DeactivateAppResponse",3,&Marshaller::mNsRPC2Communication_AppLinkCore__DeactivateAppResponse},
  {"AppLinkCore.GetAppList",4,&Marshaller::mNsRPC2Communication_AppLinkCore__GetAppList},
  {"AppLinkCore.GetAppListResponse",5,&Marshaller::mNsRPC2Communication_AppLinkCore__GetAppListResponse},
  {"AppLinkCore.GetDeviceList",6,&Marshaller::mNsRPC2Communication_AppLinkCore__GetDeviceList},
  {"AppLinkCore.GetDeviceListResponse",7,&Marshaller::mNsRPC2Communication_AppLinkCore__GetDeviceListResponse},
  {"AppLinkCore.OnAppRegistered",8,&Marshaller::mNsRPC2Communication_AppLinkCore__OnAppRegistered},
  {"AppLinkCore.OnAppUnregistered",9,&Marshaller::mNsRPC2Communication_AppLinkCore__OnAppUnregistered},
  {"AppLinkCore.OnDeviceListUpdated",10,&Marshaller::mNsRPC2Communication_AppLinkCore__OnDeviceListUpdated},
  {"AppLinkCore.SendData",11,&Marshaller::mNsRPC2Communication_AppLinkCore__SendData},
  {"AppLinkCore.SendDataResponse",12,&Marshaller::mNsRPC2Communication_AppLinkCore__SendDataResponse},
  {"Buttons.GetCapabilities",13,&Marshaller::mNsRPC2Communication_Buttons__GetCapabilities},
  {"Buttons.GetCapabilitiesResponse",14,&Marshaller::mNsRPC2Communication_Buttons__GetCapabilitiesResponse},
  {"Buttons.OnButtonEvent",15,&Marshaller::mNsRPC2Communication_Buttons__OnButtonEvent},
  {"Buttons.OnButtonPress",16,&Marshaller::mNsRPC2Communication_Buttons__OnButtonPress},
  {"Navigation.AlertManeuver",17,&Marshaller::mNsRPC2Communication_Navigation__AlertManeuver},
  {"Navigation.AlertManeuverResponse",18,&Marshaller::mNsRPC2Communication_Navigation__AlertManeuverResponse},
  {"Navigation.OnTBTClientState",19,&Marshaller::mNsRPC2Communication_Navigation__OnTBTClientState},
  {"Navigation.ShowConstantTBT",20,&Marshaller::mNsRPC2Communication_Navigation__ShowConstantTBT},
  {"Navigation.ShowConstantTBTResponse",21,&Marshaller::mNsRPC2Communication_Navigation__ShowConstantTBTResponse},
  {"Navigation.UpdateTurnList",22,&Marshaller::mNsRPC2Communication_Navigation__UpdateTurnList},
  {"Navigation.UpdateTurnListResponse",23,&Marshaller::mNsRPC2Communication_Navigation__UpdateTurnListResponse},
  {"Phone.DialNumber",24,&Marshaller::mNsRPC2Communication_Phone__DialNumber},
  {"Phone.DialNumberResponse",25,&Marshaller::mNsRPC2Communication_Phone__DialNumberResponse},
  {"TTS.ChangeRegistration",26,&Marshaller::mNsRPC2Communication_TTS__ChangeRegistration},
  {"TTS.ChangeRegistrationResponse",27,&Marshaller::mNsRPC2Communication_TTS__ChangeRegistrationResponse},
  {"TTS.GetCapabilities",30,&Marshaller::mNsRPC2Communication_TTS__GetCapabilities},
  {"TTS.GetCapabilitiesResponse",31,&Marshaller::mNsRPC2Communication_TTS__GetCapabilitiesResponse},
  {"TTS.GetLanguage",32,&Marshaller::mNsRPC2Communication_TTS__GetLanguage},
  {"TTS.GetLanguageResponse",33,&Marshaller::mNsRPC2Communication_TTS__GetLanguageResponse},
  {"TTS.OnAudioPassThru",34,&Marshaller::mNsRPC2Communication_TTS__OnAudioPassThru},
  {"TTS.OnLanguageChange",35,&Marshaller::mNsRPC2Communication_TTS__OnLanguageChange},
  {"TTS.Speak",38,&Marshaller::mNsRPC2Communication_TTS__Speak},
  {"TTS.SpeakResponse",39,&Marshaller::mNsRPC2Communication_TTS__SpeakResponse},
  {"UI.AddCommand",40,&Marshaller::mNsRPC2Communication_UI__AddCommand},
  {"UI.AddCommandResponse",41,&Marshaller::mNsRPC2Communication_UI__AddCommandResponse},
  {"UI.AddSubMenu",42,&Marshaller::mNsRPC2Communication_UI__AddSubMenu},
  {"UI.AddSubMenuResponse",43,&Marshaller::mNsRPC2Communication_UI__AddSubMenuResponse},
  {"UI.Alert",44,&Marshaller::mNsRPC2Communication_UI__Alert},
  {"UI.AlertResponse",45,&Marshaller::mNsRPC2Communication_UI__AlertResponse},
  {"UI.ChangeRegistration",46,&Marshaller::mNsRPC2Communication_UI__ChangeRegistration},
  {"UI.ChangeRegistrationResponse",47,&Marshaller::mNsRPC2Communication_UI__ChangeRegistrationResponse},
  {"UI.CreateInteractionChoiceSet",48,&Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSet},
  {"UI.CreateInteractionChoiceSetResponse",49,&Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse},
  {"UI.DeleteCommand",50,&Marshaller::mNsRPC2Communication_UI__DeleteCommand},
  {"UI.DeleteCommandResponse",51,&Marshaller::mNsRPC2Communication_UI__DeleteCommandResponse},
  {"UI.DeleteInteractionChoiceSet",52,&Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSet},
  {"UI.DeleteInteractionChoiceSetResponse",53,&Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse},
  {"UI.DeleteSubMenu",54,&Marshaller::mNsRPC2Communication_UI__DeleteSubMenu},
  {"UI.DeleteSubMenuResponse",55,&Marshaller::mNsRPC2Communication_UI__DeleteSubMenuResponse},
  {"UI.EndAudioPassThru",28,&Marshaller::mNsRPC2Communication_UI__EndAudioPassThru},
  {"UI.EndAudioPassThruResponse",29,&Marshaller::mNsRPC2Communication_UI__EndAudioPassThruResponse},
  {"UI.GetCapabilities",56,&Marshaller::mNsRPC2Communication_UI__GetCapabilities},
  {"UI.GetCapabilitiesResponse",57,&Marshaller::mNsRPC2Communication_UI__GetCapabilitiesResponse},
  {"UI.GetLanguage",58,&Marshaller::mNsRPC2Communication_UI__GetLanguage},
  {"UI.GetLanguageResponse",59,&Marshaller::mNsRPC2Communication_UI__GetLanguageResponse},
  {"UI.OnCommand",60,&Marshaller::mNsRPC2Communication_UI__OnCommand},
  {"UI.OnDeviceChosen",61,&Marshaller::mNsRPC2Communication_UI__OnDeviceChosen},
  {"UI.OnDriverDistraction",62,&Marshaller::mNsRPC2Communication_UI__OnDriverDistraction},
  {"UI.OnLanguageChange",63,&Marshaller::mNsRPC2Communication_UI__OnLanguageChange},
  {"UI.OnReady",64,&Marshaller::mNsRPC2Communication_UI__OnReady},
  {"UI.OnSystemContext",65,&Marshaller::mNsRPC2Communication_UI__OnSystemContext},
  {"UI.PerformAudioPassThru",36,&Marshaller::mNsRPC2Communication_UI__PerformAudioPassThru},
  {"UI.PerformAudioPassThruResponse",37,&Marshaller::mNsRPC2Communication_UI__PerformAudioPassThruResponse},
  {"UI.PerformInteraction",66,&Marshaller::mNsRPC2Communication_UI__PerformInteraction},
  {"UI.PerformInteractionResponse",67,&Marshaller::mNsRPC2Communication_UI__PerformInteractionResponse},
  {"UI.ResetGlobalProperties",68,&Marshaller::mNsRPC2Communication_UI__ResetGlobalProperties},
  {"UI.ResetGlobalPropertiesResponse",69,&Marshaller::mNsRPC2Communication_UI__ResetGlobalPropertiesResponse},
  {"UI.ScrollableMessage",70,&Marshaller::mNsRPC2Communication_UI__ScrollableMessage},
  {"UI.ScrollableMessageResponse",71,&Marshaller::mNsRPC2Communication_UI__ScrollableMessageResponse},
  {"UI.SetAppIcon",72,&Marshaller::mNsRPC2Communication_UI__SetAppIcon},
  {"UI.SetAppIconResponse",73,&Marshaller::mNsRPC2Communication_UI__SetAppIconResponse},
  {"UI.SetDisplayLayout",74,&Marshaller::mNsRPC2Communication_UI__SetDisplayLayout},
  {"UI.SetDisplayLayoutResponse",75,&Marshaller::mNsRPC2Communication_UI__SetDisplayLayoutResponse},
  {"UI.SetGlobalProperties",76,&Marshaller::mNsRPC2Communication_UI__SetGlobalProperties},
  {"UI.SetGlobalPropertiesResponse",77,&Marshaller::mNsRPC2Communication_UI__SetGlobalPropertiesResponse},
  {"UI.SetMediaClockTimer",78,&Marshaller::mNsRPC2Communication_UI__SetMediaClockTimer},
  {"UI.SetMediaClockTimerResponse",79,&Marshaller::mNsRPC2Communication_UI__SetMediaClockTimerResponse},
  {"UI.Show",80,&Marshaller::mNsRPC2Communication_UI__Show},
  {"UI.ShowResponse",81,&Marshaller::mNsRPC2Communication_UI__ShowResponse},
  {"UI.Slider",82,&Marshaller::mNsRPC2Communication_UI__Slider},
  {"UI.SliderResponse",83,&Marshaller::mNsRPC2Communication_UI__SliderResponse},
  {"VR.AddCommand",84,&Marshaller::mNsRPC2Communication_VR__AddCommand},
  {"VR.AddCommandResponse",85,&Marshaller::mNsRPC2Communication_VR__AddCommandResponse},
  {"VR.ChangeRegistration",86,&Marshaller::mNsRPC2Communication_VR__ChangeRegistration},
  {"VR.ChangeRegistrationResponse",87,&Marshaller::mNsRPC2Communication_VR__ChangeRegistrationResponse},
  {"VR.DeleteCommand",88,&Marshaller::mNsRPC2Communication_VR__DeleteCommand},
  {"VR.DeleteCommandResponse",89,&Marshaller::mNsRPC2Communication_VR__DeleteCommandResponse},
  {"VR.GetCapabilities",90,&Marshaller::mNsRPC2Communication_VR__GetCapabilities},
  {"VR.GetCapabilitiesResponse",91,&Marshaller::mNsRPC2Communication_VR__GetCapabilitiesResponse},
  {"VR.GetLanguage",92,&Marshaller::mNsRPC2Communication_VR__GetLanguage},
  {"VR.GetLanguageResponse",93,&Marshaller::mNsRPC2Communication_VR__GetLanguageResponse},
  {"VR.OnCommand",94,&Marshaller::mNsRPC2Communication_VR__OnCommand},
  {"VR.OnLanguageChange",95,&Marshaller::mNsRPC2Communication_VR__OnLanguageChange},
  {"VehicleInfo.GetDTCs",96,&Marshaller::mNsRPC2Communication_VehicleInfo__GetDTCs},
  {"VehicleInfo.GetDTCsResponse",97,&Marshaller::mNsRPC2Communication_VehicleInfo__GetDTCsResponse},
  {"VehicleInfo.GetVehicleData",98,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleData},
  {"VehicleInfo.GetVehicleDataResponse",99,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleDataResponse},
  {"VehicleInfo.GetVehicleType",100,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleType},
  {"VehicleInfo.GetVehicleTypeResponse",101,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleTypeResponse},
  {"VehicleInfo.OnVehicleData",102,&Marshaller::mNsRPC2Communication_VehicleInfo__OnVehicleData},
  {"VehicleInfo.ReadDID",103,&Marshaller::mNsRPC2Communication_VehicleInfo__ReadDID},
  {"VehicleInfo.ReadDIDResponse",104,&Marshaller::mNsRPC2Communication_VehicleInfo__ReadDIDResponse},
  {"VehicleInfo.SubscribeVehicleData",105,&Marshaller::mNsRPC2Communication_VehicleInfo__SubscribeVehicleData},
  {"VehicleInfo.SubscribeVehicleDataResponse",106,&Marshaller::mNsRPC2Communication_VehicleInfo__SubscribeVehicleDataResponse},
  {"VehicleInfo.UnsubscribeVehicleData",107,&Marshaller::mNsRPC2Communication_VehicleInfo__UnsubscribeVehicleData},
  {"VehicleInfo.UnsubscribeVehicleDataResponse",108,&Marshaller::mNsRPC2Communication_VehicleInfo__UnsubscribeVehicleDataResponse},
  {"UI.GetSupportedLanguages",109,&Marshaller::mNsRPC2Communication_UI__GetSupportedLanguages},
  {"UI.GetSupportedLanguagesResponse",110,&Marshaller::mNsRPC2Communication_UI__GetSupportedLanguagesResponse}

};

NsRPC2Communication::RPC2ErrorMarshaller Marshaller::mRPC2ErrorInternal;

NsRPC2Communication::AppLinkCore::ActivateAppMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__ActivateApp;
NsRPC2Communication::AppLinkCore::ActivateAppResponseMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__ActivateAppResponse;
NsRPC2Communication::AppLinkCore::DeactivateAppMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__DeactivateApp;
NsRPC2Communication::AppLinkCore::DeactivateAppResponseMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__DeactivateAppResponse;
NsRPC2Communication::AppLinkCore::GetAppListMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__GetAppList;
NsRPC2Communication::AppLinkCore::GetAppListResponseMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__GetAppListResponse;
NsRPC2Communication::AppLinkCore::GetDeviceListMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__GetDeviceList;
NsRPC2Communication::AppLinkCore::GetDeviceListResponseMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__GetDeviceListResponse;
NsRPC2Communication::AppLinkCore::OnAppRegisteredMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__OnAppRegistered;
NsRPC2Communication::AppLinkCore::OnAppUnregisteredMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__OnAppUnregistered;
NsRPC2Communication::AppLinkCore::OnDeviceListUpdatedMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__OnDeviceListUpdated;
NsRPC2Communication::AppLinkCore::SendDataMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__SendData;
NsRPC2Communication::AppLinkCore::SendDataResponseMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__SendDataResponse;
NsRPC2Communication::Buttons::GetCapabilitiesMarshaller Marshaller::mNsRPC2Communication_Buttons__GetCapabilities;
NsRPC2Communication::Buttons::GetCapabilitiesResponseMarshaller Marshaller::mNsRPC2Communication_Buttons__GetCapabilitiesResponse;
NsRPC2Communication::Buttons::OnButtonEventMarshaller Marshaller::mNsRPC2Communication_Buttons__OnButtonEvent;
NsRPC2Communication::Buttons::OnButtonPressMarshaller Marshaller::mNsRPC2Communication_Buttons__OnButtonPress;
NsRPC2Communication::Navigation::AlertManeuverMarshaller Marshaller::mNsRPC2Communication_Navigation__AlertManeuver;
NsRPC2Communication::Navigation::AlertManeuverResponseMarshaller Marshaller::mNsRPC2Communication_Navigation__AlertManeuverResponse;
NsRPC2Communication::Navigation::OnTBTClientStateMarshaller Marshaller::mNsRPC2Communication_Navigation__OnTBTClientState;
NsRPC2Communication::Navigation::ShowConstantTBTMarshaller Marshaller::mNsRPC2Communication_Navigation__ShowConstantTBT;
NsRPC2Communication::Navigation::ShowConstantTBTResponseMarshaller Marshaller::mNsRPC2Communication_Navigation__ShowConstantTBTResponse;
NsRPC2Communication::Navigation::UpdateTurnListMarshaller Marshaller::mNsRPC2Communication_Navigation__UpdateTurnList;
NsRPC2Communication::Navigation::UpdateTurnListResponseMarshaller Marshaller::mNsRPC2Communication_Navigation__UpdateTurnListResponse;
NsRPC2Communication::Phone::DialNumberMarshaller Marshaller::mNsRPC2Communication_Phone__DialNumber;
NsRPC2Communication::Phone::DialNumberResponseMarshaller Marshaller::mNsRPC2Communication_Phone__DialNumberResponse;
NsRPC2Communication::TTS::ChangeRegistrationMarshaller Marshaller::mNsRPC2Communication_TTS__ChangeRegistration;
NsRPC2Communication::TTS::ChangeRegistrationResponseMarshaller Marshaller::mNsRPC2Communication_TTS__ChangeRegistrationResponse;
NsRPC2Communication::TTS::GetCapabilitiesMarshaller Marshaller::mNsRPC2Communication_TTS__GetCapabilities;
NsRPC2Communication::TTS::GetCapabilitiesResponseMarshaller Marshaller::mNsRPC2Communication_TTS__GetCapabilitiesResponse;
NsRPC2Communication::TTS::GetLanguageMarshaller Marshaller::mNsRPC2Communication_TTS__GetLanguage;
NsRPC2Communication::TTS::GetLanguageResponseMarshaller Marshaller::mNsRPC2Communication_TTS__GetLanguageResponse;
NsRPC2Communication::TTS::OnAudioPassThruMarshaller Marshaller::mNsRPC2Communication_TTS__OnAudioPassThru;
NsRPC2Communication::TTS::OnLanguageChangeMarshaller Marshaller::mNsRPC2Communication_TTS__OnLanguageChange;
NsRPC2Communication::TTS::SpeakMarshaller Marshaller::mNsRPC2Communication_TTS__Speak;
NsRPC2Communication::TTS::SpeakResponseMarshaller Marshaller::mNsRPC2Communication_TTS__SpeakResponse;
NsRPC2Communication::UI::AddCommandMarshaller Marshaller::mNsRPC2Communication_UI__AddCommand;
NsRPC2Communication::UI::AddCommandResponseMarshaller Marshaller::mNsRPC2Communication_UI__AddCommandResponse;
NsRPC2Communication::UI::AddSubMenuMarshaller Marshaller::mNsRPC2Communication_UI__AddSubMenu;
NsRPC2Communication::UI::AddSubMenuResponseMarshaller Marshaller::mNsRPC2Communication_UI__AddSubMenuResponse;
NsRPC2Communication::UI::AlertMarshaller Marshaller::mNsRPC2Communication_UI__Alert;
NsRPC2Communication::UI::AlertResponseMarshaller Marshaller::mNsRPC2Communication_UI__AlertResponse;
NsRPC2Communication::UI::ChangeRegistrationMarshaller Marshaller::mNsRPC2Communication_UI__ChangeRegistration;
NsRPC2Communication::UI::ChangeRegistrationResponseMarshaller Marshaller::mNsRPC2Communication_UI__ChangeRegistrationResponse;
NsRPC2Communication::UI::CreateInteractionChoiceSetMarshaller Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSet;
NsRPC2Communication::UI::CreateInteractionChoiceSetResponseMarshaller Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse;
NsRPC2Communication::UI::DeleteCommandMarshaller Marshaller::mNsRPC2Communication_UI__DeleteCommand;
NsRPC2Communication::UI::DeleteCommandResponseMarshaller Marshaller::mNsRPC2Communication_UI__DeleteCommandResponse;
NsRPC2Communication::UI::DeleteInteractionChoiceSetMarshaller Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSet;
NsRPC2Communication::UI::DeleteInteractionChoiceSetResponseMarshaller Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse;
NsRPC2Communication::UI::DeleteSubMenuMarshaller Marshaller::mNsRPC2Communication_UI__DeleteSubMenu;
NsRPC2Communication::UI::DeleteSubMenuResponseMarshaller Marshaller::mNsRPC2Communication_UI__DeleteSubMenuResponse;
NsRPC2Communication::UI::EndAudioPassThruMarshaller Marshaller::mNsRPC2Communication_UI__EndAudioPassThru;
NsRPC2Communication::UI::EndAudioPassThruResponseMarshaller Marshaller::mNsRPC2Communication_UI__EndAudioPassThruResponse;
NsRPC2Communication::UI::GetCapabilitiesMarshaller Marshaller::mNsRPC2Communication_UI__GetCapabilities;
NsRPC2Communication::UI::GetCapabilitiesResponseMarshaller Marshaller::mNsRPC2Communication_UI__GetCapabilitiesResponse;
NsRPC2Communication::UI::GetLanguageMarshaller Marshaller::mNsRPC2Communication_UI__GetLanguage;
NsRPC2Communication::UI::GetLanguageResponseMarshaller Marshaller::mNsRPC2Communication_UI__GetLanguageResponse;
NsRPC2Communication::UI::OnCommandMarshaller Marshaller::mNsRPC2Communication_UI__OnCommand;
NsRPC2Communication::UI::OnDeviceChosenMarshaller Marshaller::mNsRPC2Communication_UI__OnDeviceChosen;
NsRPC2Communication::UI::OnDriverDistractionMarshaller Marshaller::mNsRPC2Communication_UI__OnDriverDistraction;
NsRPC2Communication::UI::OnLanguageChangeMarshaller Marshaller::mNsRPC2Communication_UI__OnLanguageChange;
NsRPC2Communication::UI::OnReadyMarshaller Marshaller::mNsRPC2Communication_UI__OnReady;
NsRPC2Communication::UI::OnSystemContextMarshaller Marshaller::mNsRPC2Communication_UI__OnSystemContext;
NsRPC2Communication::UI::PerformAudioPassThruMarshaller Marshaller::mNsRPC2Communication_UI__PerformAudioPassThru;
NsRPC2Communication::UI::PerformAudioPassThruResponseMarshaller Marshaller::mNsRPC2Communication_UI__PerformAudioPassThruResponse;
NsRPC2Communication::UI::PerformInteractionMarshaller Marshaller::mNsRPC2Communication_UI__PerformInteraction;
NsRPC2Communication::UI::PerformInteractionResponseMarshaller Marshaller::mNsRPC2Communication_UI__PerformInteractionResponse;
NsRPC2Communication::UI::ResetGlobalPropertiesMarshaller Marshaller::mNsRPC2Communication_UI__ResetGlobalProperties;
NsRPC2Communication::UI::ResetGlobalPropertiesResponseMarshaller Marshaller::mNsRPC2Communication_UI__ResetGlobalPropertiesResponse;
NsRPC2Communication::UI::ScrollableMessageMarshaller Marshaller::mNsRPC2Communication_UI__ScrollableMessage;
NsRPC2Communication::UI::ScrollableMessageResponseMarshaller Marshaller::mNsRPC2Communication_UI__ScrollableMessageResponse;
NsRPC2Communication::UI::SetAppIconMarshaller Marshaller::mNsRPC2Communication_UI__SetAppIcon;
NsRPC2Communication::UI::SetAppIconResponseMarshaller Marshaller::mNsRPC2Communication_UI__SetAppIconResponse;
NsRPC2Communication::UI::SetDisplayLayoutMarshaller Marshaller::mNsRPC2Communication_UI__SetDisplayLayout;
NsRPC2Communication::UI::SetDisplayLayoutResponseMarshaller Marshaller::mNsRPC2Communication_UI__SetDisplayLayoutResponse;
NsRPC2Communication::UI::SetGlobalPropertiesMarshaller Marshaller::mNsRPC2Communication_UI__SetGlobalProperties;
NsRPC2Communication::UI::SetGlobalPropertiesResponseMarshaller Marshaller::mNsRPC2Communication_UI__SetGlobalPropertiesResponse;
NsRPC2Communication::UI::SetMediaClockTimerMarshaller Marshaller::mNsRPC2Communication_UI__SetMediaClockTimer;
NsRPC2Communication::UI::SetMediaClockTimerResponseMarshaller Marshaller::mNsRPC2Communication_UI__SetMediaClockTimerResponse;
NsRPC2Communication::UI::ShowMarshaller Marshaller::mNsRPC2Communication_UI__Show;
NsRPC2Communication::UI::ShowResponseMarshaller Marshaller::mNsRPC2Communication_UI__ShowResponse;
NsRPC2Communication::UI::SliderMarshaller Marshaller::mNsRPC2Communication_UI__Slider;
NsRPC2Communication::UI::SliderResponseMarshaller Marshaller::mNsRPC2Communication_UI__SliderResponse;
NsRPC2Communication::VR::AddCommandMarshaller Marshaller::mNsRPC2Communication_VR__AddCommand;
NsRPC2Communication::VR::AddCommandResponseMarshaller Marshaller::mNsRPC2Communication_VR__AddCommandResponse;
NsRPC2Communication::VR::ChangeRegistrationMarshaller Marshaller::mNsRPC2Communication_VR__ChangeRegistration;
NsRPC2Communication::VR::ChangeRegistrationResponseMarshaller Marshaller::mNsRPC2Communication_VR__ChangeRegistrationResponse;
NsRPC2Communication::VR::DeleteCommandMarshaller Marshaller::mNsRPC2Communication_VR__DeleteCommand;
NsRPC2Communication::VR::DeleteCommandResponseMarshaller Marshaller::mNsRPC2Communication_VR__DeleteCommandResponse;
NsRPC2Communication::VR::GetCapabilitiesMarshaller Marshaller::mNsRPC2Communication_VR__GetCapabilities;
NsRPC2Communication::VR::GetCapabilitiesResponseMarshaller Marshaller::mNsRPC2Communication_VR__GetCapabilitiesResponse;
NsRPC2Communication::VR::GetLanguageMarshaller Marshaller::mNsRPC2Communication_VR__GetLanguage;
NsRPC2Communication::VR::GetLanguageResponseMarshaller Marshaller::mNsRPC2Communication_VR__GetLanguageResponse;
NsRPC2Communication::VR::OnCommandMarshaller Marshaller::mNsRPC2Communication_VR__OnCommand;
NsRPC2Communication::VR::OnLanguageChangeMarshaller Marshaller::mNsRPC2Communication_VR__OnLanguageChange;
NsRPC2Communication::VehicleInfo::GetDTCsMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__GetDTCs;
NsRPC2Communication::VehicleInfo::GetDTCsResponseMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__GetDTCsResponse;
NsRPC2Communication::VehicleInfo::GetVehicleDataMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleData;
NsRPC2Communication::VehicleInfo::GetVehicleDataResponseMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleDataResponse;
NsRPC2Communication::VehicleInfo::GetVehicleTypeMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleType;
NsRPC2Communication::VehicleInfo::GetVehicleTypeResponseMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleTypeResponse;
NsRPC2Communication::VehicleInfo::OnVehicleDataMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__OnVehicleData;
NsRPC2Communication::VehicleInfo::ReadDIDMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__ReadDID;
NsRPC2Communication::VehicleInfo::ReadDIDResponseMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__ReadDIDResponse;
NsRPC2Communication::VehicleInfo::SubscribeVehicleDataMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__SubscribeVehicleData;
NsRPC2Communication::VehicleInfo::SubscribeVehicleDataResponseMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__SubscribeVehicleDataResponse;
NsRPC2Communication::VehicleInfo::UnsubscribeVehicleDataMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__UnsubscribeVehicleData;
NsRPC2Communication::VehicleInfo::UnsubscribeVehicleDataResponseMarshaller Marshaller::mNsRPC2Communication_VehicleInfo__UnsubscribeVehicleDataResponse;
NsRPC2Communication::UI::GetSupportedLanguagesMarshaller Marshaller::mNsRPC2Communication_UI__GetSupportedLanguages;
NsRPC2Communication::UI::GetSupportedLanguagesResponseMarshaller Marshaller::mNsRPC2Communication_UI__GetSupportedLanguagesResponse; 
