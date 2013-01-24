#ifndef MARSHALLER_INCLUDE
#define MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "JSONHandler/RPC2Error.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"

#include "JSONHandler/RPC2ErrorMarshaller.h"

#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/ActivateAppMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/ActivateAppResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/GetAppListMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/GetAppListResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/GetDeviceListMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/GetDeviceListResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/OnAppDeactivatedMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/OnAppDeactivatedResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/OnAppRegisteredMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/OnAppUnregisteredMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/OnDeviceListUpdatedMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/SendDataMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/SendDataResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Buttons/GetCapabilitiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Buttons/GetCapabilitiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Buttons/OnButtonEventMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Buttons/OnButtonPressMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/ChangeRegistrationMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/ChangeRegistrationResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/GetCapabilitiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/GetCapabilitiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/GetLanguageMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/GetLanguageResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/GetSupportedLanguagesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/GetSupportedLanguagesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/OnLanguageChangeMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/SpeakMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/SpeakResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AddCommandMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AddCommandResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AddSubMenuMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AddSubMenuResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AlertMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AlertResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AlertManeuverMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AlertManeuverResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ChangeRegistrationMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ChangeRegistrationResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/CreateInteractionChoiceSetMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/CreateInteractionChoiceSetResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteCommandMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteCommandResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteInteractionChoiceSetMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteInteractionChoiceSetResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteSubMenuMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteSubMenuResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DialNumberMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DialNumberResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/EndAudioPassThruMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/EndAudioPassThruResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetCapabilitiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetCapabilitiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetLanguageMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetLanguageResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetSupportedLanguagesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetSupportedLanguagesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnCommandMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnDeviceChosenMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnDriverDistractionMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnLanguageChangeMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnReadyMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnSystemContextMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnTBTClientStateMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/PerformAudioPassThruMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/PerformAudioPassThruResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/PerformInteractionMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/PerformInteractionResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ResetGlobalPropertiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ResetGlobalPropertiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ScrollableMessageMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ScrollableMessageResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetAppIconMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetAppIconResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetDisplayLayoutMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetDisplayLayoutResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetGlobalPropertiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetGlobalPropertiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetMediaClockTimerMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetMediaClockTimerResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ShowMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ShowResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ShowConstantTBTMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ShowConstantTBTResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SliderMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SliderResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/UpdateTurnListMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/UpdateTurnListResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/AddCommandMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/AddCommandResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/ChangeRegistrationMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/ChangeRegistrationResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/DeleteCommandMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/DeleteCommandResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/GetCapabilitiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/GetCapabilitiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/GetLanguageMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/GetLanguageResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/GetSupportedLanguagesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/GetSupportedLanguagesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/OnCommandMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/OnLanguageChangeMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/GetDTCsMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/GetDTCsResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/GetVehicleDataMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/GetVehicleDataResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/GetVehicleTypeMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/GetVehicleTypeResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/OnVehicleDataMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/ReadDIDMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VehicleInfo/ReadDIDResponseMarshaller.h"

/*
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/
namespace NsRPC2Communication
{

class Marshaller
{
public:

  enum Methods
  {
    METHOD_INVALID=-1,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPP,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPPRESPONSE,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLIST,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLISTRESPONSE,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELIST,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELISTRESPONSE,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPDEACTIVATED,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPDEACTIVATEDRESPONSE,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPREGISTERED,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPUNREGISTERED,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONDEVICELISTUPDATED,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATA,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATARESPONSE,
    METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIES,
    METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONEVENT,
    METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONPRESS,
    METHOD_NSRPC2COMMUNICATION_TTS__CHANGEREGISTRATION,
    METHOD_NSRPC2COMMUNICATION_TTS__CHANGEREGISTRATIONRESPONSE,
    METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIES,
    METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGE,
    METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGERESPONSE,
    METHOD_NSRPC2COMMUNICATION_TTS__GETSUPPORTEDLANGUAGES,
    METHOD_NSRPC2COMMUNICATION_TTS__GETSUPPORTEDLANGUAGESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_TTS__ONLANGUAGECHANGE,
    METHOD_NSRPC2COMMUNICATION_TTS__SPEAK,
    METHOD_NSRPC2COMMUNICATION_TTS__SPEAKRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMAND,
    METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMANDRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENU,
    METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENURESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__ALERT,
    METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__ALERTMANEUVER,
    METHOD_NSRPC2COMMUNICATION_UI__ALERTMANEUVERRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__CHANGEREGISTRATION,
    METHOD_NSRPC2COMMUNICATION_UI__CHANGEREGISTRATIONRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESET,
    METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESETRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMAND,
    METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMANDRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESET,
    METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESETRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENU,
    METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENURESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__DIALNUMBER,
    METHOD_NSRPC2COMMUNICATION_UI__DIALNUMBERRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__ENDAUDIOPASSTHRU,
    METHOD_NSRPC2COMMUNICATION_UI__ENDAUDIOPASSTHRURESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIES,
    METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__GETLANGUAGE,
    METHOD_NSRPC2COMMUNICATION_UI__GETLANGUAGERESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__GETSUPPORTEDLANGUAGES,
    METHOD_NSRPC2COMMUNICATION_UI__GETSUPPORTEDLANGUAGESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__ONCOMMAND,
    METHOD_NSRPC2COMMUNICATION_UI__ONDEVICECHOSEN,
    METHOD_NSRPC2COMMUNICATION_UI__ONDRIVERDISTRACTION,
    METHOD_NSRPC2COMMUNICATION_UI__ONLANGUAGECHANGE,
    METHOD_NSRPC2COMMUNICATION_UI__ONREADY,
    METHOD_NSRPC2COMMUNICATION_UI__ONSYSTEMCONTEXT,
    METHOD_NSRPC2COMMUNICATION_UI__ONTBTCLIENTSTATE,
    METHOD_NSRPC2COMMUNICATION_UI__PERFORMAUDIOPASSTHRU,
    METHOD_NSRPC2COMMUNICATION_UI__PERFORMAUDIOPASSTHRURESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTION,
    METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTIONRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIES,
    METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGE,
    METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGERESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__SETAPPICON,
    METHOD_NSRPC2COMMUNICATION_UI__SETAPPICONRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUT,
    METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUTRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIES,
    METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMER,
    METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMERRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__SHOW,
    METHOD_NSRPC2COMMUNICATION_UI__SHOWRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__SHOWCONSTANTTBT,
    METHOD_NSRPC2COMMUNICATION_UI__SHOWCONSTANTTBTRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__SLIDER,
    METHOD_NSRPC2COMMUNICATION_UI__SLIDERRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__UPDATETURNLIST,
    METHOD_NSRPC2COMMUNICATION_UI__UPDATETURNLISTRESPONSE,
    METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMAND,
    METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMANDRESPONSE,
    METHOD_NSRPC2COMMUNICATION_VR__CHANGEREGISTRATION,
    METHOD_NSRPC2COMMUNICATION_VR__CHANGEREGISTRATIONRESPONSE,
    METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMAND,
    METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMANDRESPONSE,
    METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIES,
    METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_VR__GETLANGUAGE,
    METHOD_NSRPC2COMMUNICATION_VR__GETLANGUAGERESPONSE,
    METHOD_NSRPC2COMMUNICATION_VR__GETSUPPORTEDLANGUAGES,
    METHOD_NSRPC2COMMUNICATION_VR__GETSUPPORTEDLANGUAGESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_VR__ONCOMMAND,
    METHOD_NSRPC2COMMUNICATION_VR__ONLANGUAGECHANGE,
    METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCS,
    METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCSRESPONSE,
    METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLEDATA,
    METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLEDATARESPONSE,
    METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLETYPE,
    METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETVEHICLETYPERESPONSE,
    METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__ONVEHICLEDATA,
    METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDID,
    METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDIDRESPONSE
  };


  Marshaller()			{}
  ~Marshaller()			{}
  
  static ::NsRPC2Communication::RPC2Command* fromString(const std::string&);
  static std::string toString(const ::NsRPC2Communication::RPC2Command* msg);
  
  static ::NsRPC2Communication::RPC2Command* fromJSON(const Json::Value&);
  static Json::Value toJSON(const ::NsRPC2Communication::RPC2Command* msg);
  
protected:

// NsRPC2Communication::AppLinkCore::ActivateApp
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::ActivateApp& res)
  {
    return mNsRPC2Communication_AppLinkCore__ActivateApp.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::ActivateApp& res)
  {
    return mNsRPC2Communication_AppLinkCore__ActivateApp.toString(res);
  }

// NsRPC2Communication::AppLinkCore::ActivateAppResponse
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::ActivateAppResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__ActivateAppResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::ActivateAppResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__ActivateAppResponse.toString(res);
  }

// NsRPC2Communication::AppLinkCore::GetAppList
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::GetAppList& res)
  {
    return mNsRPC2Communication_AppLinkCore__GetAppList.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::GetAppList& res)
  {
    return mNsRPC2Communication_AppLinkCore__GetAppList.toString(res);
  }

// NsRPC2Communication::AppLinkCore::GetAppListResponse
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::GetAppListResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__GetAppListResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::GetAppListResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__GetAppListResponse.toString(res);
  }

// NsRPC2Communication::AppLinkCore::GetDeviceList
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::GetDeviceList& res)
  {
    return mNsRPC2Communication_AppLinkCore__GetDeviceList.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::GetDeviceList& res)
  {
    return mNsRPC2Communication_AppLinkCore__GetDeviceList.toString(res);
  }

// NsRPC2Communication::AppLinkCore::GetDeviceListResponse
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::GetDeviceListResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__GetDeviceListResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::GetDeviceListResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__GetDeviceListResponse.toString(res);
  }

// NsRPC2Communication::AppLinkCore::OnAppDeactivated
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::OnAppDeactivated& res)
  {
    return mNsRPC2Communication_AppLinkCore__OnAppDeactivated.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::OnAppDeactivated& res)
  {
    return mNsRPC2Communication_AppLinkCore__OnAppDeactivated.toString(res);
  }

// NsRPC2Communication::AppLinkCore::OnAppDeactivatedResponse
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::OnAppDeactivatedResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__OnAppDeactivatedResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::OnAppDeactivatedResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__OnAppDeactivatedResponse.toString(res);
  }

// NsRPC2Communication::AppLinkCore::OnAppRegistered
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::OnAppRegistered& res)
  {
    return mNsRPC2Communication_AppLinkCore__OnAppRegistered.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::OnAppRegistered& res)
  {
    return mNsRPC2Communication_AppLinkCore__OnAppRegistered.toString(res);
  }

// NsRPC2Communication::AppLinkCore::OnAppUnregistered
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::OnAppUnregistered& res)
  {
    return mNsRPC2Communication_AppLinkCore__OnAppUnregistered.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::OnAppUnregistered& res)
  {
    return mNsRPC2Communication_AppLinkCore__OnAppUnregistered.toString(res);
  }

// NsRPC2Communication::AppLinkCore::OnDeviceListUpdated
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::OnDeviceListUpdated& res)
  {
    return mNsRPC2Communication_AppLinkCore__OnDeviceListUpdated.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::OnDeviceListUpdated& res)
  {
    return mNsRPC2Communication_AppLinkCore__OnDeviceListUpdated.toString(res);
  }

// NsRPC2Communication::AppLinkCore::SendData
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::SendData& res)
  {
    return mNsRPC2Communication_AppLinkCore__SendData.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::SendData& res)
  {
    return mNsRPC2Communication_AppLinkCore__SendData.toString(res);
  }

// NsRPC2Communication::AppLinkCore::SendDataResponse
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::SendDataResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__SendDataResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::SendDataResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__SendDataResponse.toString(res);
  }

// NsRPC2Communication::Buttons::GetCapabilities
  static bool fromString(const std::string& str, NsRPC2Communication::Buttons::GetCapabilities& res)
  {
    return mNsRPC2Communication_Buttons__GetCapabilities.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::Buttons::GetCapabilities& res)
  {
    return mNsRPC2Communication_Buttons__GetCapabilities.toString(res);
  }

// NsRPC2Communication::Buttons::GetCapabilitiesResponse
  static bool fromString(const std::string& str, NsRPC2Communication::Buttons::GetCapabilitiesResponse& res)
  {
    return mNsRPC2Communication_Buttons__GetCapabilitiesResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::Buttons::GetCapabilitiesResponse& res)
  {
    return mNsRPC2Communication_Buttons__GetCapabilitiesResponse.toString(res);
  }

// NsRPC2Communication::Buttons::OnButtonEvent
  static bool fromString(const std::string& str, NsRPC2Communication::Buttons::OnButtonEvent& res)
  {
    return mNsRPC2Communication_Buttons__OnButtonEvent.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::Buttons::OnButtonEvent& res)
  {
    return mNsRPC2Communication_Buttons__OnButtonEvent.toString(res);
  }

// NsRPC2Communication::Buttons::OnButtonPress
  static bool fromString(const std::string& str, NsRPC2Communication::Buttons::OnButtonPress& res)
  {
    return mNsRPC2Communication_Buttons__OnButtonPress.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::Buttons::OnButtonPress& res)
  {
    return mNsRPC2Communication_Buttons__OnButtonPress.toString(res);
  }

// NsRPC2Communication::TTS::ChangeRegistration
  static bool fromString(const std::string& str, NsRPC2Communication::TTS::ChangeRegistration& res)
  {
    return mNsRPC2Communication_TTS__ChangeRegistration.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::TTS::ChangeRegistration& res)
  {
    return mNsRPC2Communication_TTS__ChangeRegistration.toString(res);
  }

// NsRPC2Communication::TTS::ChangeRegistrationResponse
  static bool fromString(const std::string& str, NsRPC2Communication::TTS::ChangeRegistrationResponse& res)
  {
    return mNsRPC2Communication_TTS__ChangeRegistrationResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::TTS::ChangeRegistrationResponse& res)
  {
    return mNsRPC2Communication_TTS__ChangeRegistrationResponse.toString(res);
  }

// NsRPC2Communication::TTS::GetCapabilities
  static bool fromString(const std::string& str, NsRPC2Communication::TTS::GetCapabilities& res)
  {
    return mNsRPC2Communication_TTS__GetCapabilities.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::TTS::GetCapabilities& res)
  {
    return mNsRPC2Communication_TTS__GetCapabilities.toString(res);
  }

// NsRPC2Communication::TTS::GetCapabilitiesResponse
  static bool fromString(const std::string& str, NsRPC2Communication::TTS::GetCapabilitiesResponse& res)
  {
    return mNsRPC2Communication_TTS__GetCapabilitiesResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::TTS::GetCapabilitiesResponse& res)
  {
    return mNsRPC2Communication_TTS__GetCapabilitiesResponse.toString(res);
  }

// NsRPC2Communication::TTS::GetLanguage
  static bool fromString(const std::string& str, NsRPC2Communication::TTS::GetLanguage& res)
  {
    return mNsRPC2Communication_TTS__GetLanguage.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::TTS::GetLanguage& res)
  {
    return mNsRPC2Communication_TTS__GetLanguage.toString(res);
  }

// NsRPC2Communication::TTS::GetLanguageResponse
  static bool fromString(const std::string& str, NsRPC2Communication::TTS::GetLanguageResponse& res)
  {
    return mNsRPC2Communication_TTS__GetLanguageResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::TTS::GetLanguageResponse& res)
  {
    return mNsRPC2Communication_TTS__GetLanguageResponse.toString(res);
  }

// NsRPC2Communication::TTS::GetSupportedLanguages
  static bool fromString(const std::string& str, NsRPC2Communication::TTS::GetSupportedLanguages& res)
  {
    return mNsRPC2Communication_TTS__GetSupportedLanguages.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::TTS::GetSupportedLanguages& res)
  {
    return mNsRPC2Communication_TTS__GetSupportedLanguages.toString(res);
  }

// NsRPC2Communication::TTS::GetSupportedLanguagesResponse
  static bool fromString(const std::string& str, NsRPC2Communication::TTS::GetSupportedLanguagesResponse& res)
  {
    return mNsRPC2Communication_TTS__GetSupportedLanguagesResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::TTS::GetSupportedLanguagesResponse& res)
  {
    return mNsRPC2Communication_TTS__GetSupportedLanguagesResponse.toString(res);
  }

// NsRPC2Communication::TTS::OnLanguageChange
  static bool fromString(const std::string& str, NsRPC2Communication::TTS::OnLanguageChange& res)
  {
    return mNsRPC2Communication_TTS__OnLanguageChange.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::TTS::OnLanguageChange& res)
  {
    return mNsRPC2Communication_TTS__OnLanguageChange.toString(res);
  }

// NsRPC2Communication::TTS::Speak
  static bool fromString(const std::string& str, NsRPC2Communication::TTS::Speak& res)
  {
    return mNsRPC2Communication_TTS__Speak.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::TTS::Speak& res)
  {
    return mNsRPC2Communication_TTS__Speak.toString(res);
  }

// NsRPC2Communication::TTS::SpeakResponse
  static bool fromString(const std::string& str, NsRPC2Communication::TTS::SpeakResponse& res)
  {
    return mNsRPC2Communication_TTS__SpeakResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::TTS::SpeakResponse& res)
  {
    return mNsRPC2Communication_TTS__SpeakResponse.toString(res);
  }

// NsRPC2Communication::UI::AddCommand
  static bool fromString(const std::string& str, NsRPC2Communication::UI::AddCommand& res)
  {
    return mNsRPC2Communication_UI__AddCommand.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::AddCommand& res)
  {
    return mNsRPC2Communication_UI__AddCommand.toString(res);
  }

// NsRPC2Communication::UI::AddCommandResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::AddCommandResponse& res)
  {
    return mNsRPC2Communication_UI__AddCommandResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::AddCommandResponse& res)
  {
    return mNsRPC2Communication_UI__AddCommandResponse.toString(res);
  }

// NsRPC2Communication::UI::AddSubMenu
  static bool fromString(const std::string& str, NsRPC2Communication::UI::AddSubMenu& res)
  {
    return mNsRPC2Communication_UI__AddSubMenu.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::AddSubMenu& res)
  {
    return mNsRPC2Communication_UI__AddSubMenu.toString(res);
  }

// NsRPC2Communication::UI::AddSubMenuResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::AddSubMenuResponse& res)
  {
    return mNsRPC2Communication_UI__AddSubMenuResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::AddSubMenuResponse& res)
  {
    return mNsRPC2Communication_UI__AddSubMenuResponse.toString(res);
  }

// NsRPC2Communication::UI::Alert
  static bool fromString(const std::string& str, NsRPC2Communication::UI::Alert& res)
  {
    return mNsRPC2Communication_UI__Alert.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::Alert& res)
  {
    return mNsRPC2Communication_UI__Alert.toString(res);
  }

// NsRPC2Communication::UI::AlertResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::AlertResponse& res)
  {
    return mNsRPC2Communication_UI__AlertResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::AlertResponse& res)
  {
    return mNsRPC2Communication_UI__AlertResponse.toString(res);
  }

// NsRPC2Communication::UI::AlertManeuver
  static bool fromString(const std::string& str, NsRPC2Communication::UI::AlertManeuver& res)
  {
    return mNsRPC2Communication_UI__AlertManeuver.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::AlertManeuver& res)
  {
    return mNsRPC2Communication_UI__AlertManeuver.toString(res);
  }

// NsRPC2Communication::UI::AlertManeuverResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::AlertManeuverResponse& res)
  {
    return mNsRPC2Communication_UI__AlertManeuverResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::AlertManeuverResponse& res)
  {
    return mNsRPC2Communication_UI__AlertManeuverResponse.toString(res);
  }

// NsRPC2Communication::UI::ChangeRegistration
  static bool fromString(const std::string& str, NsRPC2Communication::UI::ChangeRegistration& res)
  {
    return mNsRPC2Communication_UI__ChangeRegistration.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::ChangeRegistration& res)
  {
    return mNsRPC2Communication_UI__ChangeRegistration.toString(res);
  }

// NsRPC2Communication::UI::ChangeRegistrationResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::ChangeRegistrationResponse& res)
  {
    return mNsRPC2Communication_UI__ChangeRegistrationResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::ChangeRegistrationResponse& res)
  {
    return mNsRPC2Communication_UI__ChangeRegistrationResponse.toString(res);
  }

// NsRPC2Communication::UI::CreateInteractionChoiceSet
  static bool fromString(const std::string& str, NsRPC2Communication::UI::CreateInteractionChoiceSet& res)
  {
    return mNsRPC2Communication_UI__CreateInteractionChoiceSet.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::CreateInteractionChoiceSet& res)
  {
    return mNsRPC2Communication_UI__CreateInteractionChoiceSet.toString(res);
  }

// NsRPC2Communication::UI::CreateInteractionChoiceSetResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::CreateInteractionChoiceSetResponse& res)
  {
    return mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::CreateInteractionChoiceSetResponse& res)
  {
    return mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse.toString(res);
  }

// NsRPC2Communication::UI::DeleteCommand
  static bool fromString(const std::string& str, NsRPC2Communication::UI::DeleteCommand& res)
  {
    return mNsRPC2Communication_UI__DeleteCommand.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::DeleteCommand& res)
  {
    return mNsRPC2Communication_UI__DeleteCommand.toString(res);
  }

// NsRPC2Communication::UI::DeleteCommandResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::DeleteCommandResponse& res)
  {
    return mNsRPC2Communication_UI__DeleteCommandResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::DeleteCommandResponse& res)
  {
    return mNsRPC2Communication_UI__DeleteCommandResponse.toString(res);
  }

// NsRPC2Communication::UI::DeleteInteractionChoiceSet
  static bool fromString(const std::string& str, NsRPC2Communication::UI::DeleteInteractionChoiceSet& res)
  {
    return mNsRPC2Communication_UI__DeleteInteractionChoiceSet.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::DeleteInteractionChoiceSet& res)
  {
    return mNsRPC2Communication_UI__DeleteInteractionChoiceSet.toString(res);
  }

// NsRPC2Communication::UI::DeleteInteractionChoiceSetResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::DeleteInteractionChoiceSetResponse& res)
  {
    return mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::DeleteInteractionChoiceSetResponse& res)
  {
    return mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse.toString(res);
  }

// NsRPC2Communication::UI::DeleteSubMenu
  static bool fromString(const std::string& str, NsRPC2Communication::UI::DeleteSubMenu& res)
  {
    return mNsRPC2Communication_UI__DeleteSubMenu.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::DeleteSubMenu& res)
  {
    return mNsRPC2Communication_UI__DeleteSubMenu.toString(res);
  }

// NsRPC2Communication::UI::DeleteSubMenuResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::DeleteSubMenuResponse& res)
  {
    return mNsRPC2Communication_UI__DeleteSubMenuResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::DeleteSubMenuResponse& res)
  {
    return mNsRPC2Communication_UI__DeleteSubMenuResponse.toString(res);
  }

// NsRPC2Communication::UI::DialNumber
  static bool fromString(const std::string& str, NsRPC2Communication::UI::DialNumber& res)
  {
    return mNsRPC2Communication_UI__DialNumber.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::DialNumber& res)
  {
    return mNsRPC2Communication_UI__DialNumber.toString(res);
  }

// NsRPC2Communication::UI::DialNumberResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::DialNumberResponse& res)
  {
    return mNsRPC2Communication_UI__DialNumberResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::DialNumberResponse& res)
  {
    return mNsRPC2Communication_UI__DialNumberResponse.toString(res);
  }

// NsRPC2Communication::UI::EndAudioPassThru
  static bool fromString(const std::string& str, NsRPC2Communication::UI::EndAudioPassThru& res)
  {
    return mNsRPC2Communication_UI__EndAudioPassThru.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::EndAudioPassThru& res)
  {
    return mNsRPC2Communication_UI__EndAudioPassThru.toString(res);
  }

// NsRPC2Communication::UI::EndAudioPassThruResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::EndAudioPassThruResponse& res)
  {
    return mNsRPC2Communication_UI__EndAudioPassThruResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::EndAudioPassThruResponse& res)
  {
    return mNsRPC2Communication_UI__EndAudioPassThruResponse.toString(res);
  }

// NsRPC2Communication::UI::GetCapabilities
  static bool fromString(const std::string& str, NsRPC2Communication::UI::GetCapabilities& res)
  {
    return mNsRPC2Communication_UI__GetCapabilities.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::GetCapabilities& res)
  {
    return mNsRPC2Communication_UI__GetCapabilities.toString(res);
  }

// NsRPC2Communication::UI::GetCapabilitiesResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::GetCapabilitiesResponse& res)
  {
    return mNsRPC2Communication_UI__GetCapabilitiesResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::GetCapabilitiesResponse& res)
  {
    return mNsRPC2Communication_UI__GetCapabilitiesResponse.toString(res);
  }

// NsRPC2Communication::UI::GetLanguage
  static bool fromString(const std::string& str, NsRPC2Communication::UI::GetLanguage& res)
  {
    return mNsRPC2Communication_UI__GetLanguage.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::GetLanguage& res)
  {
    return mNsRPC2Communication_UI__GetLanguage.toString(res);
  }

// NsRPC2Communication::UI::GetLanguageResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::GetLanguageResponse& res)
  {
    return mNsRPC2Communication_UI__GetLanguageResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::GetLanguageResponse& res)
  {
    return mNsRPC2Communication_UI__GetLanguageResponse.toString(res);
  }

// NsRPC2Communication::UI::GetSupportedLanguages
  static bool fromString(const std::string& str, NsRPC2Communication::UI::GetSupportedLanguages& res)
  {
    return mNsRPC2Communication_UI__GetSupportedLanguages.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::GetSupportedLanguages& res)
  {
    return mNsRPC2Communication_UI__GetSupportedLanguages.toString(res);
  }

// NsRPC2Communication::UI::GetSupportedLanguagesResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::GetSupportedLanguagesResponse& res)
  {
    return mNsRPC2Communication_UI__GetSupportedLanguagesResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::GetSupportedLanguagesResponse& res)
  {
    return mNsRPC2Communication_UI__GetSupportedLanguagesResponse.toString(res);
  }

// NsRPC2Communication::UI::OnCommand
  static bool fromString(const std::string& str, NsRPC2Communication::UI::OnCommand& res)
  {
    return mNsRPC2Communication_UI__OnCommand.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::OnCommand& res)
  {
    return mNsRPC2Communication_UI__OnCommand.toString(res);
  }

// NsRPC2Communication::UI::OnDeviceChosen
  static bool fromString(const std::string& str, NsRPC2Communication::UI::OnDeviceChosen& res)
  {
    return mNsRPC2Communication_UI__OnDeviceChosen.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::OnDeviceChosen& res)
  {
    return mNsRPC2Communication_UI__OnDeviceChosen.toString(res);
  }

// NsRPC2Communication::UI::OnDriverDistraction
  static bool fromString(const std::string& str, NsRPC2Communication::UI::OnDriverDistraction& res)
  {
    return mNsRPC2Communication_UI__OnDriverDistraction.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::OnDriverDistraction& res)
  {
    return mNsRPC2Communication_UI__OnDriverDistraction.toString(res);
  }

// NsRPC2Communication::UI::OnLanguageChange
  static bool fromString(const std::string& str, NsRPC2Communication::UI::OnLanguageChange& res)
  {
    return mNsRPC2Communication_UI__OnLanguageChange.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::OnLanguageChange& res)
  {
    return mNsRPC2Communication_UI__OnLanguageChange.toString(res);
  }

// NsRPC2Communication::UI::OnReady
  static bool fromString(const std::string& str, NsRPC2Communication::UI::OnReady& res)
  {
    return mNsRPC2Communication_UI__OnReady.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::OnReady& res)
  {
    return mNsRPC2Communication_UI__OnReady.toString(res);
  }

// NsRPC2Communication::UI::OnSystemContext
  static bool fromString(const std::string& str, NsRPC2Communication::UI::OnSystemContext& res)
  {
    return mNsRPC2Communication_UI__OnSystemContext.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::OnSystemContext& res)
  {
    return mNsRPC2Communication_UI__OnSystemContext.toString(res);
  }

// NsRPC2Communication::UI::OnTBTClientState
  static bool fromString(const std::string& str, NsRPC2Communication::UI::OnTBTClientState& res)
  {
    return mNsRPC2Communication_UI__OnTBTClientState.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::OnTBTClientState& res)
  {
    return mNsRPC2Communication_UI__OnTBTClientState.toString(res);
  }

// NsRPC2Communication::UI::PerformAudioPassThru
  static bool fromString(const std::string& str, NsRPC2Communication::UI::PerformAudioPassThru& res)
  {
    return mNsRPC2Communication_UI__PerformAudioPassThru.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::PerformAudioPassThru& res)
  {
    return mNsRPC2Communication_UI__PerformAudioPassThru.toString(res);
  }

// NsRPC2Communication::UI::PerformAudioPassThruResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::PerformAudioPassThruResponse& res)
  {
    return mNsRPC2Communication_UI__PerformAudioPassThruResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::PerformAudioPassThruResponse& res)
  {
    return mNsRPC2Communication_UI__PerformAudioPassThruResponse.toString(res);
  }

// NsRPC2Communication::UI::PerformInteraction
  static bool fromString(const std::string& str, NsRPC2Communication::UI::PerformInteraction& res)
  {
    return mNsRPC2Communication_UI__PerformInteraction.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::PerformInteraction& res)
  {
    return mNsRPC2Communication_UI__PerformInteraction.toString(res);
  }

// NsRPC2Communication::UI::PerformInteractionResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::PerformInteractionResponse& res)
  {
    return mNsRPC2Communication_UI__PerformInteractionResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::PerformInteractionResponse& res)
  {
    return mNsRPC2Communication_UI__PerformInteractionResponse.toString(res);
  }

// NsRPC2Communication::UI::ResetGlobalProperties
  static bool fromString(const std::string& str, NsRPC2Communication::UI::ResetGlobalProperties& res)
  {
    return mNsRPC2Communication_UI__ResetGlobalProperties.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::ResetGlobalProperties& res)
  {
    return mNsRPC2Communication_UI__ResetGlobalProperties.toString(res);
  }

// NsRPC2Communication::UI::ResetGlobalPropertiesResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::ResetGlobalPropertiesResponse& res)
  {
    return mNsRPC2Communication_UI__ResetGlobalPropertiesResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::ResetGlobalPropertiesResponse& res)
  {
    return mNsRPC2Communication_UI__ResetGlobalPropertiesResponse.toString(res);
  }

// NsRPC2Communication::UI::ScrollableMessage
  static bool fromString(const std::string& str, NsRPC2Communication::UI::ScrollableMessage& res)
  {
    return mNsRPC2Communication_UI__ScrollableMessage.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::ScrollableMessage& res)
  {
    return mNsRPC2Communication_UI__ScrollableMessage.toString(res);
  }

// NsRPC2Communication::UI::ScrollableMessageResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::ScrollableMessageResponse& res)
  {
    return mNsRPC2Communication_UI__ScrollableMessageResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::ScrollableMessageResponse& res)
  {
    return mNsRPC2Communication_UI__ScrollableMessageResponse.toString(res);
  }

// NsRPC2Communication::UI::SetAppIcon
  static bool fromString(const std::string& str, NsRPC2Communication::UI::SetAppIcon& res)
  {
    return mNsRPC2Communication_UI__SetAppIcon.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::SetAppIcon& res)
  {
    return mNsRPC2Communication_UI__SetAppIcon.toString(res);
  }

// NsRPC2Communication::UI::SetAppIconResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::SetAppIconResponse& res)
  {
    return mNsRPC2Communication_UI__SetAppIconResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::SetAppIconResponse& res)
  {
    return mNsRPC2Communication_UI__SetAppIconResponse.toString(res);
  }

// NsRPC2Communication::UI::SetDisplayLayout
  static bool fromString(const std::string& str, NsRPC2Communication::UI::SetDisplayLayout& res)
  {
    return mNsRPC2Communication_UI__SetDisplayLayout.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::SetDisplayLayout& res)
  {
    return mNsRPC2Communication_UI__SetDisplayLayout.toString(res);
  }

// NsRPC2Communication::UI::SetDisplayLayoutResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::SetDisplayLayoutResponse& res)
  {
    return mNsRPC2Communication_UI__SetDisplayLayoutResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::SetDisplayLayoutResponse& res)
  {
    return mNsRPC2Communication_UI__SetDisplayLayoutResponse.toString(res);
  }

// NsRPC2Communication::UI::SetGlobalProperties
  static bool fromString(const std::string& str, NsRPC2Communication::UI::SetGlobalProperties& res)
  {
    return mNsRPC2Communication_UI__SetGlobalProperties.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::SetGlobalProperties& res)
  {
    return mNsRPC2Communication_UI__SetGlobalProperties.toString(res);
  }

// NsRPC2Communication::UI::SetGlobalPropertiesResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::SetGlobalPropertiesResponse& res)
  {
    return mNsRPC2Communication_UI__SetGlobalPropertiesResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::SetGlobalPropertiesResponse& res)
  {
    return mNsRPC2Communication_UI__SetGlobalPropertiesResponse.toString(res);
  }

// NsRPC2Communication::UI::SetMediaClockTimer
  static bool fromString(const std::string& str, NsRPC2Communication::UI::SetMediaClockTimer& res)
  {
    return mNsRPC2Communication_UI__SetMediaClockTimer.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::SetMediaClockTimer& res)
  {
    return mNsRPC2Communication_UI__SetMediaClockTimer.toString(res);
  }

// NsRPC2Communication::UI::SetMediaClockTimerResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::SetMediaClockTimerResponse& res)
  {
    return mNsRPC2Communication_UI__SetMediaClockTimerResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::SetMediaClockTimerResponse& res)
  {
    return mNsRPC2Communication_UI__SetMediaClockTimerResponse.toString(res);
  }

// NsRPC2Communication::UI::Show
  static bool fromString(const std::string& str, NsRPC2Communication::UI::Show& res)
  {
    return mNsRPC2Communication_UI__Show.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::Show& res)
  {
    return mNsRPC2Communication_UI__Show.toString(res);
  }

// NsRPC2Communication::UI::ShowResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::ShowResponse& res)
  {
    return mNsRPC2Communication_UI__ShowResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::ShowResponse& res)
  {
    return mNsRPC2Communication_UI__ShowResponse.toString(res);
  }

// NsRPC2Communication::UI::ShowConstantTBT
  static bool fromString(const std::string& str, NsRPC2Communication::UI::ShowConstantTBT& res)
  {
    return mNsRPC2Communication_UI__ShowConstantTBT.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::ShowConstantTBT& res)
  {
    return mNsRPC2Communication_UI__ShowConstantTBT.toString(res);
  }

// NsRPC2Communication::UI::ShowConstantTBTResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::ShowConstantTBTResponse& res)
  {
    return mNsRPC2Communication_UI__ShowConstantTBTResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::ShowConstantTBTResponse& res)
  {
    return mNsRPC2Communication_UI__ShowConstantTBTResponse.toString(res);
  }

// NsRPC2Communication::UI::Slider
  static bool fromString(const std::string& str, NsRPC2Communication::UI::Slider& res)
  {
    return mNsRPC2Communication_UI__Slider.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::Slider& res)
  {
    return mNsRPC2Communication_UI__Slider.toString(res);
  }

// NsRPC2Communication::UI::SliderResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::SliderResponse& res)
  {
    return mNsRPC2Communication_UI__SliderResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::SliderResponse& res)
  {
    return mNsRPC2Communication_UI__SliderResponse.toString(res);
  }

// NsRPC2Communication::UI::UpdateTurnList
  static bool fromString(const std::string& str, NsRPC2Communication::UI::UpdateTurnList& res)
  {
    return mNsRPC2Communication_UI__UpdateTurnList.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::UpdateTurnList& res)
  {
    return mNsRPC2Communication_UI__UpdateTurnList.toString(res);
  }

// NsRPC2Communication::UI::UpdateTurnListResponse
  static bool fromString(const std::string& str, NsRPC2Communication::UI::UpdateTurnListResponse& res)
  {
    return mNsRPC2Communication_UI__UpdateTurnListResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::UpdateTurnListResponse& res)
  {
    return mNsRPC2Communication_UI__UpdateTurnListResponse.toString(res);
  }

// NsRPC2Communication::VR::AddCommand
  static bool fromString(const std::string& str, NsRPC2Communication::VR::AddCommand& res)
  {
    return mNsRPC2Communication_VR__AddCommand.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::AddCommand& res)
  {
    return mNsRPC2Communication_VR__AddCommand.toString(res);
  }

// NsRPC2Communication::VR::AddCommandResponse
  static bool fromString(const std::string& str, NsRPC2Communication::VR::AddCommandResponse& res)
  {
    return mNsRPC2Communication_VR__AddCommandResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::AddCommandResponse& res)
  {
    return mNsRPC2Communication_VR__AddCommandResponse.toString(res);
  }

// NsRPC2Communication::VR::ChangeRegistration
  static bool fromString(const std::string& str, NsRPC2Communication::VR::ChangeRegistration& res)
  {
    return mNsRPC2Communication_VR__ChangeRegistration.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::ChangeRegistration& res)
  {
    return mNsRPC2Communication_VR__ChangeRegistration.toString(res);
  }

// NsRPC2Communication::VR::ChangeRegistrationResponse
  static bool fromString(const std::string& str, NsRPC2Communication::VR::ChangeRegistrationResponse& res)
  {
    return mNsRPC2Communication_VR__ChangeRegistrationResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::ChangeRegistrationResponse& res)
  {
    return mNsRPC2Communication_VR__ChangeRegistrationResponse.toString(res);
  }

// NsRPC2Communication::VR::DeleteCommand
  static bool fromString(const std::string& str, NsRPC2Communication::VR::DeleteCommand& res)
  {
    return mNsRPC2Communication_VR__DeleteCommand.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::DeleteCommand& res)
  {
    return mNsRPC2Communication_VR__DeleteCommand.toString(res);
  }

// NsRPC2Communication::VR::DeleteCommandResponse
  static bool fromString(const std::string& str, NsRPC2Communication::VR::DeleteCommandResponse& res)
  {
    return mNsRPC2Communication_VR__DeleteCommandResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::DeleteCommandResponse& res)
  {
    return mNsRPC2Communication_VR__DeleteCommandResponse.toString(res);
  }

// NsRPC2Communication::VR::GetCapabilities
  static bool fromString(const std::string& str, NsRPC2Communication::VR::GetCapabilities& res)
  {
    return mNsRPC2Communication_VR__GetCapabilities.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::GetCapabilities& res)
  {
    return mNsRPC2Communication_VR__GetCapabilities.toString(res);
  }

// NsRPC2Communication::VR::GetCapabilitiesResponse
  static bool fromString(const std::string& str, NsRPC2Communication::VR::GetCapabilitiesResponse& res)
  {
    return mNsRPC2Communication_VR__GetCapabilitiesResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::GetCapabilitiesResponse& res)
  {
    return mNsRPC2Communication_VR__GetCapabilitiesResponse.toString(res);
  }

// NsRPC2Communication::VR::GetLanguage
  static bool fromString(const std::string& str, NsRPC2Communication::VR::GetLanguage& res)
  {
    return mNsRPC2Communication_VR__GetLanguage.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::GetLanguage& res)
  {
    return mNsRPC2Communication_VR__GetLanguage.toString(res);
  }

// NsRPC2Communication::VR::GetLanguageResponse
  static bool fromString(const std::string& str, NsRPC2Communication::VR::GetLanguageResponse& res)
  {
    return mNsRPC2Communication_VR__GetLanguageResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::GetLanguageResponse& res)
  {
    return mNsRPC2Communication_VR__GetLanguageResponse.toString(res);
  }

// NsRPC2Communication::VR::GetSupportedLanguages
  static bool fromString(const std::string& str, NsRPC2Communication::VR::GetSupportedLanguages& res)
  {
    return mNsRPC2Communication_VR__GetSupportedLanguages.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::GetSupportedLanguages& res)
  {
    return mNsRPC2Communication_VR__GetSupportedLanguages.toString(res);
  }

// NsRPC2Communication::VR::GetSupportedLanguagesResponse
  static bool fromString(const std::string& str, NsRPC2Communication::VR::GetSupportedLanguagesResponse& res)
  {
    return mNsRPC2Communication_VR__GetSupportedLanguagesResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::GetSupportedLanguagesResponse& res)
  {
    return mNsRPC2Communication_VR__GetSupportedLanguagesResponse.toString(res);
  }

// NsRPC2Communication::VR::OnCommand
  static bool fromString(const std::string& str, NsRPC2Communication::VR::OnCommand& res)
  {
    return mNsRPC2Communication_VR__OnCommand.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::OnCommand& res)
  {
    return mNsRPC2Communication_VR__OnCommand.toString(res);
  }

// NsRPC2Communication::VR::OnLanguageChange
  static bool fromString(const std::string& str, NsRPC2Communication::VR::OnLanguageChange& res)
  {
    return mNsRPC2Communication_VR__OnLanguageChange.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::OnLanguageChange& res)
  {
    return mNsRPC2Communication_VR__OnLanguageChange.toString(res);
  }

// NsRPC2Communication::VehicleInfo::GetDTCs
  static bool fromString(const std::string& str, NsRPC2Communication::VehicleInfo::GetDTCs& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetDTCs.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VehicleInfo::GetDTCs& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetDTCs.toString(res);
  }

// NsRPC2Communication::VehicleInfo::GetDTCsResponse
  static bool fromString(const std::string& str, NsRPC2Communication::VehicleInfo::GetDTCsResponse& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetDTCsResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VehicleInfo::GetDTCsResponse& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetDTCsResponse.toString(res);
  }

// NsRPC2Communication::VehicleInfo::GetVehicleData
  static bool fromString(const std::string& str, NsRPC2Communication::VehicleInfo::GetVehicleData& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetVehicleData.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VehicleInfo::GetVehicleData& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetVehicleData.toString(res);
  }

// NsRPC2Communication::VehicleInfo::GetVehicleDataResponse
  static bool fromString(const std::string& str, NsRPC2Communication::VehicleInfo::GetVehicleDataResponse& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetVehicleDataResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VehicleInfo::GetVehicleDataResponse& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetVehicleDataResponse.toString(res);
  }

// NsRPC2Communication::VehicleInfo::GetVehicleType
  static bool fromString(const std::string& str, NsRPC2Communication::VehicleInfo::GetVehicleType& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetVehicleType.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VehicleInfo::GetVehicleType& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetVehicleType.toString(res);
  }

// NsRPC2Communication::VehicleInfo::GetVehicleTypeResponse
  static bool fromString(const std::string& str, NsRPC2Communication::VehicleInfo::GetVehicleTypeResponse& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetVehicleTypeResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VehicleInfo::GetVehicleTypeResponse& res)
  {
    return mNsRPC2Communication_VehicleInfo__GetVehicleTypeResponse.toString(res);
  }

// NsRPC2Communication::VehicleInfo::OnVehicleData
  static bool fromString(const std::string& str, NsRPC2Communication::VehicleInfo::OnVehicleData& res)
  {
    return mNsRPC2Communication_VehicleInfo__OnVehicleData.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VehicleInfo::OnVehicleData& res)
  {
    return mNsRPC2Communication_VehicleInfo__OnVehicleData.toString(res);
  }

// NsRPC2Communication::VehicleInfo::ReadDID
  static bool fromString(const std::string& str, NsRPC2Communication::VehicleInfo::ReadDID& res)
  {
    return mNsRPC2Communication_VehicleInfo__ReadDID.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VehicleInfo::ReadDID& res)
  {
    return mNsRPC2Communication_VehicleInfo__ReadDID.toString(res);
  }

// NsRPC2Communication::VehicleInfo::ReadDIDResponse
  static bool fromString(const std::string& str, NsRPC2Communication::VehicleInfo::ReadDIDResponse& res)
  {
    return mNsRPC2Communication_VehicleInfo__ReadDIDResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VehicleInfo::ReadDIDResponse& res)
  {
    return mNsRPC2Communication_VehicleInfo__ReadDIDResponse.toString(res);
  }


  struct localHash
  {
    const char *name;
    unsigned int idx;
    void* marshaller;					// Bingo!!!  old good plain C with direct casting!!!
  };
  
private:
  
  static Json::Value Request2JSON(const ::NsRPC2Communication::RPC2Request* msg);
  static Json::Value Response2JSON(const ::NsRPC2Communication::RPC2Response* msg);
  static Json::Value Notification2JSON(const ::NsRPC2Communication::RPC2Notification* msg);
  
  static const char* getName(Methods e)
  {
    return (e>=0 && e<110) ? mHashTable[e].name : NULL;
  }
  
  static const Methods getIndex(const char* s);
  
  static const localHash mHashTable[110];
  friend class Marshaller_intHash;
  
  static ::NsRPC2Communication::RPC2ErrorMarshaller mRPC2ErrorInternal;

  static NsRPC2Communication::AppLinkCore::ActivateAppMarshaller mNsRPC2Communication_AppLinkCore__ActivateApp;
  static NsRPC2Communication::AppLinkCore::ActivateAppResponseMarshaller mNsRPC2Communication_AppLinkCore__ActivateAppResponse;
  static NsRPC2Communication::AppLinkCore::GetAppListMarshaller mNsRPC2Communication_AppLinkCore__GetAppList;
  static NsRPC2Communication::AppLinkCore::GetAppListResponseMarshaller mNsRPC2Communication_AppLinkCore__GetAppListResponse;
  static NsRPC2Communication::AppLinkCore::GetDeviceListMarshaller mNsRPC2Communication_AppLinkCore__GetDeviceList;
  static NsRPC2Communication::AppLinkCore::GetDeviceListResponseMarshaller mNsRPC2Communication_AppLinkCore__GetDeviceListResponse;
  static NsRPC2Communication::AppLinkCore::OnAppDeactivatedMarshaller mNsRPC2Communication_AppLinkCore__OnAppDeactivated;
  static NsRPC2Communication::AppLinkCore::OnAppDeactivatedResponseMarshaller mNsRPC2Communication_AppLinkCore__OnAppDeactivatedResponse;
  static NsRPC2Communication::AppLinkCore::OnAppRegisteredMarshaller mNsRPC2Communication_AppLinkCore__OnAppRegistered;
  static NsRPC2Communication::AppLinkCore::OnAppUnregisteredMarshaller mNsRPC2Communication_AppLinkCore__OnAppUnregistered;
  static NsRPC2Communication::AppLinkCore::OnDeviceListUpdatedMarshaller mNsRPC2Communication_AppLinkCore__OnDeviceListUpdated;
  static NsRPC2Communication::AppLinkCore::SendDataMarshaller mNsRPC2Communication_AppLinkCore__SendData;
  static NsRPC2Communication::AppLinkCore::SendDataResponseMarshaller mNsRPC2Communication_AppLinkCore__SendDataResponse;
  static NsRPC2Communication::Buttons::GetCapabilitiesMarshaller mNsRPC2Communication_Buttons__GetCapabilities;
  static NsRPC2Communication::Buttons::GetCapabilitiesResponseMarshaller mNsRPC2Communication_Buttons__GetCapabilitiesResponse;
  static NsRPC2Communication::Buttons::OnButtonEventMarshaller mNsRPC2Communication_Buttons__OnButtonEvent;
  static NsRPC2Communication::Buttons::OnButtonPressMarshaller mNsRPC2Communication_Buttons__OnButtonPress;
  static NsRPC2Communication::TTS::ChangeRegistrationMarshaller mNsRPC2Communication_TTS__ChangeRegistration;
  static NsRPC2Communication::TTS::ChangeRegistrationResponseMarshaller mNsRPC2Communication_TTS__ChangeRegistrationResponse;
  static NsRPC2Communication::TTS::GetCapabilitiesMarshaller mNsRPC2Communication_TTS__GetCapabilities;
  static NsRPC2Communication::TTS::GetCapabilitiesResponseMarshaller mNsRPC2Communication_TTS__GetCapabilitiesResponse;
  static NsRPC2Communication::TTS::GetLanguageMarshaller mNsRPC2Communication_TTS__GetLanguage;
  static NsRPC2Communication::TTS::GetLanguageResponseMarshaller mNsRPC2Communication_TTS__GetLanguageResponse;
  static NsRPC2Communication::TTS::GetSupportedLanguagesMarshaller mNsRPC2Communication_TTS__GetSupportedLanguages;
  static NsRPC2Communication::TTS::GetSupportedLanguagesResponseMarshaller mNsRPC2Communication_TTS__GetSupportedLanguagesResponse;
  static NsRPC2Communication::TTS::OnLanguageChangeMarshaller mNsRPC2Communication_TTS__OnLanguageChange;
  static NsRPC2Communication::TTS::SpeakMarshaller mNsRPC2Communication_TTS__Speak;
  static NsRPC2Communication::TTS::SpeakResponseMarshaller mNsRPC2Communication_TTS__SpeakResponse;
  static NsRPC2Communication::UI::AddCommandMarshaller mNsRPC2Communication_UI__AddCommand;
  static NsRPC2Communication::UI::AddCommandResponseMarshaller mNsRPC2Communication_UI__AddCommandResponse;
  static NsRPC2Communication::UI::AddSubMenuMarshaller mNsRPC2Communication_UI__AddSubMenu;
  static NsRPC2Communication::UI::AddSubMenuResponseMarshaller mNsRPC2Communication_UI__AddSubMenuResponse;
  static NsRPC2Communication::UI::AlertMarshaller mNsRPC2Communication_UI__Alert;
  static NsRPC2Communication::UI::AlertResponseMarshaller mNsRPC2Communication_UI__AlertResponse;
  static NsRPC2Communication::UI::AlertManeuverMarshaller mNsRPC2Communication_UI__AlertManeuver;
  static NsRPC2Communication::UI::AlertManeuverResponseMarshaller mNsRPC2Communication_UI__AlertManeuverResponse;
  static NsRPC2Communication::UI::ChangeRegistrationMarshaller mNsRPC2Communication_UI__ChangeRegistration;
  static NsRPC2Communication::UI::ChangeRegistrationResponseMarshaller mNsRPC2Communication_UI__ChangeRegistrationResponse;
  static NsRPC2Communication::UI::CreateInteractionChoiceSetMarshaller mNsRPC2Communication_UI__CreateInteractionChoiceSet;
  static NsRPC2Communication::UI::CreateInteractionChoiceSetResponseMarshaller mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse;
  static NsRPC2Communication::UI::DeleteCommandMarshaller mNsRPC2Communication_UI__DeleteCommand;
  static NsRPC2Communication::UI::DeleteCommandResponseMarshaller mNsRPC2Communication_UI__DeleteCommandResponse;
  static NsRPC2Communication::UI::DeleteInteractionChoiceSetMarshaller mNsRPC2Communication_UI__DeleteInteractionChoiceSet;
  static NsRPC2Communication::UI::DeleteInteractionChoiceSetResponseMarshaller mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse;
  static NsRPC2Communication::UI::DeleteSubMenuMarshaller mNsRPC2Communication_UI__DeleteSubMenu;
  static NsRPC2Communication::UI::DeleteSubMenuResponseMarshaller mNsRPC2Communication_UI__DeleteSubMenuResponse;
  static NsRPC2Communication::UI::DialNumberMarshaller mNsRPC2Communication_UI__DialNumber;
  static NsRPC2Communication::UI::DialNumberResponseMarshaller mNsRPC2Communication_UI__DialNumberResponse;
  static NsRPC2Communication::UI::EndAudioPassThruMarshaller mNsRPC2Communication_UI__EndAudioPassThru;
  static NsRPC2Communication::UI::EndAudioPassThruResponseMarshaller mNsRPC2Communication_UI__EndAudioPassThruResponse;
  static NsRPC2Communication::UI::GetCapabilitiesMarshaller mNsRPC2Communication_UI__GetCapabilities;
  static NsRPC2Communication::UI::GetCapabilitiesResponseMarshaller mNsRPC2Communication_UI__GetCapabilitiesResponse;
  static NsRPC2Communication::UI::GetLanguageMarshaller mNsRPC2Communication_UI__GetLanguage;
  static NsRPC2Communication::UI::GetLanguageResponseMarshaller mNsRPC2Communication_UI__GetLanguageResponse;
  static NsRPC2Communication::UI::GetSupportedLanguagesMarshaller mNsRPC2Communication_UI__GetSupportedLanguages;
  static NsRPC2Communication::UI::GetSupportedLanguagesResponseMarshaller mNsRPC2Communication_UI__GetSupportedLanguagesResponse;
  static NsRPC2Communication::UI::OnCommandMarshaller mNsRPC2Communication_UI__OnCommand;
  static NsRPC2Communication::UI::OnDeviceChosenMarshaller mNsRPC2Communication_UI__OnDeviceChosen;
  static NsRPC2Communication::UI::OnDriverDistractionMarshaller mNsRPC2Communication_UI__OnDriverDistraction;
  static NsRPC2Communication::UI::OnLanguageChangeMarshaller mNsRPC2Communication_UI__OnLanguageChange;
  static NsRPC2Communication::UI::OnReadyMarshaller mNsRPC2Communication_UI__OnReady;
  static NsRPC2Communication::UI::OnSystemContextMarshaller mNsRPC2Communication_UI__OnSystemContext;
  static NsRPC2Communication::UI::OnTBTClientStateMarshaller mNsRPC2Communication_UI__OnTBTClientState;
  static NsRPC2Communication::UI::PerformAudioPassThruMarshaller mNsRPC2Communication_UI__PerformAudioPassThru;
  static NsRPC2Communication::UI::PerformAudioPassThruResponseMarshaller mNsRPC2Communication_UI__PerformAudioPassThruResponse;
  static NsRPC2Communication::UI::PerformInteractionMarshaller mNsRPC2Communication_UI__PerformInteraction;
  static NsRPC2Communication::UI::PerformInteractionResponseMarshaller mNsRPC2Communication_UI__PerformInteractionResponse;
  static NsRPC2Communication::UI::ResetGlobalPropertiesMarshaller mNsRPC2Communication_UI__ResetGlobalProperties;
  static NsRPC2Communication::UI::ResetGlobalPropertiesResponseMarshaller mNsRPC2Communication_UI__ResetGlobalPropertiesResponse;
  static NsRPC2Communication::UI::ScrollableMessageMarshaller mNsRPC2Communication_UI__ScrollableMessage;
  static NsRPC2Communication::UI::ScrollableMessageResponseMarshaller mNsRPC2Communication_UI__ScrollableMessageResponse;
  static NsRPC2Communication::UI::SetAppIconMarshaller mNsRPC2Communication_UI__SetAppIcon;
  static NsRPC2Communication::UI::SetAppIconResponseMarshaller mNsRPC2Communication_UI__SetAppIconResponse;
  static NsRPC2Communication::UI::SetDisplayLayoutMarshaller mNsRPC2Communication_UI__SetDisplayLayout;
  static NsRPC2Communication::UI::SetDisplayLayoutResponseMarshaller mNsRPC2Communication_UI__SetDisplayLayoutResponse;
  static NsRPC2Communication::UI::SetGlobalPropertiesMarshaller mNsRPC2Communication_UI__SetGlobalProperties;
  static NsRPC2Communication::UI::SetGlobalPropertiesResponseMarshaller mNsRPC2Communication_UI__SetGlobalPropertiesResponse;
  static NsRPC2Communication::UI::SetMediaClockTimerMarshaller mNsRPC2Communication_UI__SetMediaClockTimer;
  static NsRPC2Communication::UI::SetMediaClockTimerResponseMarshaller mNsRPC2Communication_UI__SetMediaClockTimerResponse;
  static NsRPC2Communication::UI::ShowMarshaller mNsRPC2Communication_UI__Show;
  static NsRPC2Communication::UI::ShowResponseMarshaller mNsRPC2Communication_UI__ShowResponse;
  static NsRPC2Communication::UI::ShowConstantTBTMarshaller mNsRPC2Communication_UI__ShowConstantTBT;
  static NsRPC2Communication::UI::ShowConstantTBTResponseMarshaller mNsRPC2Communication_UI__ShowConstantTBTResponse;
  static NsRPC2Communication::UI::SliderMarshaller mNsRPC2Communication_UI__Slider;
  static NsRPC2Communication::UI::SliderResponseMarshaller mNsRPC2Communication_UI__SliderResponse;
  static NsRPC2Communication::UI::UpdateTurnListMarshaller mNsRPC2Communication_UI__UpdateTurnList;
  static NsRPC2Communication::UI::UpdateTurnListResponseMarshaller mNsRPC2Communication_UI__UpdateTurnListResponse;
  static NsRPC2Communication::VR::AddCommandMarshaller mNsRPC2Communication_VR__AddCommand;
  static NsRPC2Communication::VR::AddCommandResponseMarshaller mNsRPC2Communication_VR__AddCommandResponse;
  static NsRPC2Communication::VR::ChangeRegistrationMarshaller mNsRPC2Communication_VR__ChangeRegistration;
  static NsRPC2Communication::VR::ChangeRegistrationResponseMarshaller mNsRPC2Communication_VR__ChangeRegistrationResponse;
  static NsRPC2Communication::VR::DeleteCommandMarshaller mNsRPC2Communication_VR__DeleteCommand;
  static NsRPC2Communication::VR::DeleteCommandResponseMarshaller mNsRPC2Communication_VR__DeleteCommandResponse;
  static NsRPC2Communication::VR::GetCapabilitiesMarshaller mNsRPC2Communication_VR__GetCapabilities;
  static NsRPC2Communication::VR::GetCapabilitiesResponseMarshaller mNsRPC2Communication_VR__GetCapabilitiesResponse;
  static NsRPC2Communication::VR::GetLanguageMarshaller mNsRPC2Communication_VR__GetLanguage;
  static NsRPC2Communication::VR::GetLanguageResponseMarshaller mNsRPC2Communication_VR__GetLanguageResponse;
  static NsRPC2Communication::VR::GetSupportedLanguagesMarshaller mNsRPC2Communication_VR__GetSupportedLanguages;
  static NsRPC2Communication::VR::GetSupportedLanguagesResponseMarshaller mNsRPC2Communication_VR__GetSupportedLanguagesResponse;
  static NsRPC2Communication::VR::OnCommandMarshaller mNsRPC2Communication_VR__OnCommand;
  static NsRPC2Communication::VR::OnLanguageChangeMarshaller mNsRPC2Communication_VR__OnLanguageChange;
  static NsRPC2Communication::VehicleInfo::GetDTCsMarshaller mNsRPC2Communication_VehicleInfo__GetDTCs;
  static NsRPC2Communication::VehicleInfo::GetDTCsResponseMarshaller mNsRPC2Communication_VehicleInfo__GetDTCsResponse;
  static NsRPC2Communication::VehicleInfo::GetVehicleDataMarshaller mNsRPC2Communication_VehicleInfo__GetVehicleData;
  static NsRPC2Communication::VehicleInfo::GetVehicleDataResponseMarshaller mNsRPC2Communication_VehicleInfo__GetVehicleDataResponse;
  static NsRPC2Communication::VehicleInfo::GetVehicleTypeMarshaller mNsRPC2Communication_VehicleInfo__GetVehicleType;
  static NsRPC2Communication::VehicleInfo::GetVehicleTypeResponseMarshaller mNsRPC2Communication_VehicleInfo__GetVehicleTypeResponse;
  static NsRPC2Communication::VehicleInfo::OnVehicleDataMarshaller mNsRPC2Communication_VehicleInfo__OnVehicleData;
  static NsRPC2Communication::VehicleInfo::ReadDIDMarshaller mNsRPC2Communication_VehicleInfo__ReadDID;
  static NsRPC2Communication::VehicleInfo::ReadDIDResponseMarshaller mNsRPC2Communication_VehicleInfo__ReadDIDResponse;

};
}
#endif
