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
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/OnAppRegisteredMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/OnAppUnregisteredMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/SendDataMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/SendDataResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Buttons/GetCapabilitiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Buttons/GetCapabilitiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Buttons/OnButtonEventMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Buttons/OnButtonPressMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/GetCapabilitiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/GetCapabilitiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/SpeakMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/SpeakResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AddCommandMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AddCommandResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AddSubMenuMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AddSubMenuResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AlertMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/AlertResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/CreateInteractionChoiceSetMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/CreateInteractionChoiceSetResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteCommandMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteCommandResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteInteractionChoiceSetMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteInteractionChoiceSetResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteSubMenuMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/DeleteSubMenuResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetCapabilitiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetCapabilitiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnAppActivatedMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnAppDeactivatedMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnCommandMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnDriverDistractionMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnReadyMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnSystemContextMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/PerformInteractionMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/PerformInteractionResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ResetGlobalPropertiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ResetGlobalPropertiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetGlobalPropertiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetGlobalPropertiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetMediaClockTimerMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetMediaClockTimerResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ShowMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ShowResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/AddCommandMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/AddCommandResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/DeleteCommandMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/DeleteCommandResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/GetCapabilitiesMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/GetCapabilitiesResponseMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/OnCommandMarshaller.h"

/*
  generated at	Mon Nov 19 07:25:04 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/


class Marshaller
{
public:

  enum Methods
  {
    METHOD_INVALID=-1,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPP,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPPRESPONSE,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPREGISTERED,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPUNREGISTERED,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATA,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATARESPONSE,
    METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIES,
    METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONEVENT,
    METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONPRESS,
    METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIES,
    METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_TTS__SPEAK,
    METHOD_NSRPC2COMMUNICATION_TTS__SPEAKRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMAND,
    METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMANDRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENU,
    METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENURESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__ALERT,
    METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESET,
    METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESETRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMAND,
    METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMANDRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESET,
    METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESETRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENU,
    METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENURESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIES,
    METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__ONAPPACTIVATED,
    METHOD_NSRPC2COMMUNICATION_UI__ONAPPDEACTIVATED,
    METHOD_NSRPC2COMMUNICATION_UI__ONCOMMAND,
    METHOD_NSRPC2COMMUNICATION_UI__ONDRIVERDISTRACTION,
    METHOD_NSRPC2COMMUNICATION_UI__ONREADY,
    METHOD_NSRPC2COMMUNICATION_UI__ONSYSTEMCONTEXT,
    METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTION,
    METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTIONRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIES,
    METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIES,
    METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMER,
    METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMERRESPONSE,
    METHOD_NSRPC2COMMUNICATION_UI__SHOW,
    METHOD_NSRPC2COMMUNICATION_UI__SHOWRESPONSE,
    METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMAND,
    METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMANDRESPONSE,
    METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMAND,
    METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMANDRESPONSE,
    METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIES,
    METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIESRESPONSE,
    METHOD_NSRPC2COMMUNICATION_VR__ONCOMMAND
  };


  Marshaller()			{}
  ~Marshaller()			{}
  
  static ::RPC2Communication::RPC2Command* fromString(const std::string&);
  static std::string toString(const ::RPC2Communication::RPC2Command* msg);
  
  static ::RPC2Communication::RPC2Command* fromJSON(const Json::Value&);
  static Json::Value toJSON(const ::RPC2Communication::RPC2Command* msg);
  
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

// NsRPC2Communication::UI::OnAppActivated
  static bool fromString(const std::string& str, NsRPC2Communication::UI::OnAppActivated& res)
  {
    return mNsRPC2Communication_UI__OnAppActivated.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::OnAppActivated& res)
  {
    return mNsRPC2Communication_UI__OnAppActivated.toString(res);
  }

// NsRPC2Communication::UI::OnAppDeactivated
  static bool fromString(const std::string& str, NsRPC2Communication::UI::OnAppDeactivated& res)
  {
    return mNsRPC2Communication_UI__OnAppDeactivated.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::OnAppDeactivated& res)
  {
    return mNsRPC2Communication_UI__OnAppDeactivated.toString(res);
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

// NsRPC2Communication::UI::OnDriverDistraction
  static bool fromString(const std::string& str, NsRPC2Communication::UI::OnDriverDistraction& res)
  {
    return mNsRPC2Communication_UI__OnDriverDistraction.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::UI::OnDriverDistraction& res)
  {
    return mNsRPC2Communication_UI__OnDriverDistraction.toString(res);
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

// NsRPC2Communication::VR::OnCommand
  static bool fromString(const std::string& str, NsRPC2Communication::VR::OnCommand& res)
  {
    return mNsRPC2Communication_VR__OnCommand.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::VR::OnCommand& res)
  {
    return mNsRPC2Communication_VR__OnCommand.toString(res);
  }


  struct localHash
  {
    const char *name;
    unsigned int idx;
    void* marshaller;					// Bingo!!!  old good plain C with direct casting!!!
  };
  
private:
  
  static Json::Value Request2JSON(const ::RPC2Communication::RPC2Request* msg);
  static Json::Value Response2JSON(const ::RPC2Communication::RPC2Response* msg);
  static Json::Value Notification2JSON(const ::RPC2Communication::RPC2Notification* msg);
  
  static const char* getName(Methods e)
  {
    return (e>=0 && e<53) ? mHashTable[e].name : NULL;
  }
  
  static const Methods getIndex(const char* s);
  
  static const localHash mHashTable[53];
  friend class Marshaller_intHash;
  
  static ::RPC2Communication::RPC2ErrorMarshaller mRPC2ErrorInternal;

  static NsRPC2Communication::AppLinkCore::ActivateAppMarshaller mNsRPC2Communication_AppLinkCore__ActivateApp;
  static NsRPC2Communication::AppLinkCore::ActivateAppResponseMarshaller mNsRPC2Communication_AppLinkCore__ActivateAppResponse;
  static NsRPC2Communication::AppLinkCore::OnAppRegisteredMarshaller mNsRPC2Communication_AppLinkCore__OnAppRegistered;
  static NsRPC2Communication::AppLinkCore::OnAppUnregisteredMarshaller mNsRPC2Communication_AppLinkCore__OnAppUnregistered;
  static NsRPC2Communication::AppLinkCore::SendDataMarshaller mNsRPC2Communication_AppLinkCore__SendData;
  static NsRPC2Communication::AppLinkCore::SendDataResponseMarshaller mNsRPC2Communication_AppLinkCore__SendDataResponse;
  static NsRPC2Communication::Buttons::GetCapabilitiesMarshaller mNsRPC2Communication_Buttons__GetCapabilities;
  static NsRPC2Communication::Buttons::GetCapabilitiesResponseMarshaller mNsRPC2Communication_Buttons__GetCapabilitiesResponse;
  static NsRPC2Communication::Buttons::OnButtonEventMarshaller mNsRPC2Communication_Buttons__OnButtonEvent;
  static NsRPC2Communication::Buttons::OnButtonPressMarshaller mNsRPC2Communication_Buttons__OnButtonPress;
  static NsRPC2Communication::TTS::GetCapabilitiesMarshaller mNsRPC2Communication_TTS__GetCapabilities;
  static NsRPC2Communication::TTS::GetCapabilitiesResponseMarshaller mNsRPC2Communication_TTS__GetCapabilitiesResponse;
  static NsRPC2Communication::TTS::SpeakMarshaller mNsRPC2Communication_TTS__Speak;
  static NsRPC2Communication::TTS::SpeakResponseMarshaller mNsRPC2Communication_TTS__SpeakResponse;
  static NsRPC2Communication::UI::AddCommandMarshaller mNsRPC2Communication_UI__AddCommand;
  static NsRPC2Communication::UI::AddCommandResponseMarshaller mNsRPC2Communication_UI__AddCommandResponse;
  static NsRPC2Communication::UI::AddSubMenuMarshaller mNsRPC2Communication_UI__AddSubMenu;
  static NsRPC2Communication::UI::AddSubMenuResponseMarshaller mNsRPC2Communication_UI__AddSubMenuResponse;
  static NsRPC2Communication::UI::AlertMarshaller mNsRPC2Communication_UI__Alert;
  static NsRPC2Communication::UI::AlertResponseMarshaller mNsRPC2Communication_UI__AlertResponse;
  static NsRPC2Communication::UI::CreateInteractionChoiceSetMarshaller mNsRPC2Communication_UI__CreateInteractionChoiceSet;
  static NsRPC2Communication::UI::CreateInteractionChoiceSetResponseMarshaller mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse;
  static NsRPC2Communication::UI::DeleteCommandMarshaller mNsRPC2Communication_UI__DeleteCommand;
  static NsRPC2Communication::UI::DeleteCommandResponseMarshaller mNsRPC2Communication_UI__DeleteCommandResponse;
  static NsRPC2Communication::UI::DeleteInteractionChoiceSetMarshaller mNsRPC2Communication_UI__DeleteInteractionChoiceSet;
  static NsRPC2Communication::UI::DeleteInteractionChoiceSetResponseMarshaller mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse;
  static NsRPC2Communication::UI::DeleteSubMenuMarshaller mNsRPC2Communication_UI__DeleteSubMenu;
  static NsRPC2Communication::UI::DeleteSubMenuResponseMarshaller mNsRPC2Communication_UI__DeleteSubMenuResponse;
  static NsRPC2Communication::UI::GetCapabilitiesMarshaller mNsRPC2Communication_UI__GetCapabilities;
  static NsRPC2Communication::UI::GetCapabilitiesResponseMarshaller mNsRPC2Communication_UI__GetCapabilitiesResponse;
  static NsRPC2Communication::UI::OnAppActivatedMarshaller mNsRPC2Communication_UI__OnAppActivated;
  static NsRPC2Communication::UI::OnAppDeactivatedMarshaller mNsRPC2Communication_UI__OnAppDeactivated;
  static NsRPC2Communication::UI::OnCommandMarshaller mNsRPC2Communication_UI__OnCommand;
  static NsRPC2Communication::UI::OnDriverDistractionMarshaller mNsRPC2Communication_UI__OnDriverDistraction;
  static NsRPC2Communication::UI::OnReadyMarshaller mNsRPC2Communication_UI__OnReady;
  static NsRPC2Communication::UI::OnSystemContextMarshaller mNsRPC2Communication_UI__OnSystemContext;
  static NsRPC2Communication::UI::PerformInteractionMarshaller mNsRPC2Communication_UI__PerformInteraction;
  static NsRPC2Communication::UI::PerformInteractionResponseMarshaller mNsRPC2Communication_UI__PerformInteractionResponse;
  static NsRPC2Communication::UI::ResetGlobalPropertiesMarshaller mNsRPC2Communication_UI__ResetGlobalProperties;
  static NsRPC2Communication::UI::ResetGlobalPropertiesResponseMarshaller mNsRPC2Communication_UI__ResetGlobalPropertiesResponse;
  static NsRPC2Communication::UI::SetGlobalPropertiesMarshaller mNsRPC2Communication_UI__SetGlobalProperties;
  static NsRPC2Communication::UI::SetGlobalPropertiesResponseMarshaller mNsRPC2Communication_UI__SetGlobalPropertiesResponse;
  static NsRPC2Communication::UI::SetMediaClockTimerMarshaller mNsRPC2Communication_UI__SetMediaClockTimer;
  static NsRPC2Communication::UI::SetMediaClockTimerResponseMarshaller mNsRPC2Communication_UI__SetMediaClockTimerResponse;
  static NsRPC2Communication::UI::ShowMarshaller mNsRPC2Communication_UI__Show;
  static NsRPC2Communication::UI::ShowResponseMarshaller mNsRPC2Communication_UI__ShowResponse;
  static NsRPC2Communication::VR::AddCommandMarshaller mNsRPC2Communication_VR__AddCommand;
  static NsRPC2Communication::VR::AddCommandResponseMarshaller mNsRPC2Communication_VR__AddCommandResponse;
  static NsRPC2Communication::VR::DeleteCommandMarshaller mNsRPC2Communication_VR__DeleteCommand;
  static NsRPC2Communication::VR::DeleteCommandResponseMarshaller mNsRPC2Communication_VR__DeleteCommandResponse;
  static NsRPC2Communication::VR::GetCapabilitiesMarshaller mNsRPC2Communication_VR__GetCapabilities;
  static NsRPC2Communication::VR::GetCapabilitiesResponseMarshaller mNsRPC2Communication_VR__GetCapabilitiesResponse;
  static NsRPC2Communication::VR::OnCommandMarshaller mNsRPC2Communication_VR__OnCommand;

};

#endif
