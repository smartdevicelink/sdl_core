#ifndef MARSHALLER_INCLUDE
#define MARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../../src/ALRPCObjectsImpl/RegisterAppInterface_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/RegisterAppInterface_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/UnregisterAppInterface_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/UnregisterAppInterface_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/SetGlobalProperties_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/SetGlobalProperties_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ResetGlobalProperties_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ResetGlobalProperties_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/AddCommand_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/AddCommand_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/DeleteCommand_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/DeleteCommand_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/AddSubMenu_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/AddSubMenu_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/DeleteSubMenu_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/DeleteSubMenu_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/CreateInteractionChoiceSet_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/CreateInteractionChoiceSet_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/PerformInteraction_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/PerformInteraction_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/DeleteInteractionChoiceSet_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/DeleteInteractionChoiceSet_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/Alert_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/Alert_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/Show_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/Show_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/Speak_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/Speak_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/SetMediaClockTimer_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/SetMediaClockTimer_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/EncodedSyncPData_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/EncodedSyncPData_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/DialNumber_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/DialNumber_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/PerformAudioPassThru_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/PerformAudioPassThru_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/EndAudioPassThru_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/EndAudioPassThru_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/SubscribeButton_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/SubscribeButton_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/UnsubscribeButton_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/UnsubscribeButton_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/SubscribeVehicleData_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/SubscribeVehicleData_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/UnsubscribeVehicleData_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/UnsubscribeVehicleData_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/GetVehicleData_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/GetVehicleData_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ReadDID_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ReadDID_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/GetDTCs_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/GetDTCs_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ScrollableMessage_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ScrollableMessage_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/Slider_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/Slider_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ShowConstantTBT_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ShowConstantTBT_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/AlertManeuver_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/AlertManeuver_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/UpdateTurnList_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/UpdateTurnList_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ChangeLanguageRegistration_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ChangeLanguageRegistration_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/GenericResponse_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/DeleteFile_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/DeleteFile_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ListFiles_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/ListFiles_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/SetAppIcon_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/SetAppIcon_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnHMIStatusMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnAppInterfaceUnregisteredMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnButtonEventMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnButtonPressMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnVehicleDataMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnCommandMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnEncodedSyncPDataMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnTBTClientStateMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnDriverDistractionMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnPermissionsChangeMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnAudioPassThruMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnLanguageChangeMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/

class Marshaller
{
public:

  enum Methods
  {
    METHOD_INVALID=-1,
    METHOD_REGISTERAPPINTERFACE_REQUEST=0,
    METHOD_REGISTERAPPINTERFACE_RESPONSE=1,
    METHOD_UNREGISTERAPPINTERFACE_REQUEST=2,
    METHOD_UNREGISTERAPPINTERFACE_RESPONSE=3,
    METHOD_SETGLOBALPROPERTIES_REQUEST=4,
    METHOD_SETGLOBALPROPERTIES_RESPONSE=5,
    METHOD_RESETGLOBALPROPERTIES_REQUEST=6,
    METHOD_RESETGLOBALPROPERTIES_RESPONSE=7,
    METHOD_ADDCOMMAND_REQUEST=8,
    METHOD_ADDCOMMAND_RESPONSE=9,
    METHOD_DELETECOMMAND_REQUEST=10,
    METHOD_DELETECOMMAND_RESPONSE=11,
    METHOD_ADDSUBMENU_REQUEST=12,
    METHOD_ADDSUBMENU_RESPONSE=13,
    METHOD_DELETESUBMENU_REQUEST=14,
    METHOD_DELETESUBMENU_RESPONSE=15,
    METHOD_CREATEINTERACTIONCHOICESET_REQUEST=16,
    METHOD_CREATEINTERACTIONCHOICESET_RESPONSE=17,
    METHOD_PERFORMINTERACTION_REQUEST=18,
    METHOD_PERFORMINTERACTION_RESPONSE=19,
    METHOD_DELETEINTERACTIONCHOICESET_REQUEST=20,
    METHOD_DELETEINTERACTIONCHOICESET_RESPONSE=21,
    METHOD_ALERT_REQUEST=22,
    METHOD_ALERT_RESPONSE=23,
    METHOD_SHOW_REQUEST=24,
    METHOD_SHOW_RESPONSE=25,
    METHOD_SPEAK_REQUEST=26,
    METHOD_SPEAK_RESPONSE=27,
    METHOD_SETMEDIACLOCKTIMER_REQUEST=28,
    METHOD_SETMEDIACLOCKTIMER_RESPONSE=29,
    METHOD_ENCODEDSYNCPDATA_REQUEST=30,
    METHOD_ENCODEDSYNCPDATA_RESPONSE=31,
    METHOD_DIALNUMBER_REQUEST=32,
    METHOD_DIALNUMBER_RESPONSE=33,
    METHOD_PERFORMAUDIOPASSTHRU_REQUEST=34,
    METHOD_PERFORMAUDIOPASSTHRU_RESPONSE=35,
    METHOD_ENDAUDIOPASSTHRU_REQUEST=36,
    METHOD_ENDAUDIOPASSTHRU_RESPONSE=37,
    METHOD_SUBSCRIBEBUTTON_REQUEST=38,
    METHOD_SUBSCRIBEBUTTON_RESPONSE=39,
    METHOD_UNSUBSCRIBEBUTTON_REQUEST=40,
    METHOD_UNSUBSCRIBEBUTTON_RESPONSE=41,
    METHOD_SUBSCRIBEVEHICLEDATA_REQUEST=42,
    METHOD_SUBSCRIBEVEHICLEDATA_RESPONSE=43,
    METHOD_UNSUBSCRIBEVEHICLEDATA_REQUEST=44,
    METHOD_UNSUBSCRIBEVEHICLEDATA_RESPONSE=45,
    METHOD_GETVEHICLEDATA_REQUEST=46,
    METHOD_GETVEHICLEDATA_RESPONSE=47,
    METHOD_READDID_REQUEST=48,
    METHOD_READDID_RESPONSE=49,
    METHOD_GETDTCS_REQUEST=50,
    METHOD_GETDTCS_RESPONSE=51,
    METHOD_SCROLLABLEMESSAGE_REQUEST=52,
    METHOD_SCROLLABLEMESSAGE_RESPONSE=53,
    METHOD_SLIDER_REQUEST=54,
    METHOD_SLIDER_RESPONSE=55,
    METHOD_SHOWCONSTANTTBT_REQUEST=56,
    METHOD_SHOWCONSTANTTBT_RESPONSE=57,
    METHOD_ALERTMANEUVER_REQUEST=58,
    METHOD_ALERTMANEUVER_RESPONSE=59,
    METHOD_UPDATETURNLIST_REQUEST=60,
    METHOD_UPDATETURNLIST_RESPONSE=61,
    METHOD_CHANGELANGUAGEREGISTRATION_REQUEST=62,
    METHOD_CHANGELANGUAGEREGISTRATION_RESPONSE=63,
    METHOD_GENERICRESPONSE_RESPONSE=64,
    METHOD_DELETEFILE_REQUEST=65,
    METHOD_DELETEFILE_RESPONSE=66,
    METHOD_LISTFILES_REQUEST=67,
    METHOD_LISTFILES_RESPONSE=68,
    METHOD_SETAPPICON_REQUEST=69,
    METHOD_SETAPPICON_RESPONSE=70,
    METHOD_ONHMISTATUS=71,
    METHOD_ONAPPINTERFACEUNREGISTERED=72,
    METHOD_ONBUTTONEVENT=73,
    METHOD_ONBUTTONPRESS=74,
    METHOD_ONVEHICLEDATA=75,
    METHOD_ONCOMMAND=76,
    METHOD_ONENCODEDSYNCPDATA=77,
    METHOD_ONTBTCLIENTSTATE=78,
    METHOD_ONDRIVERDISTRACTION=79,
    METHOD_ONPERMISSIONSCHANGE=80,
    METHOD_ONAUDIOPASSTHRU=81,
    METHOD_ONLANGUAGECHANGE=82
  };

  Marshaller()					{}
  ~Marshaller()					{}
  

  static ALRPCMessage* fromString(const std::string&);
  static std::string toString(const ALRPCMessage* msg);

protected:
  static ALRPCMessage* fromJSON(const Json::Value&);
  static Json::Value toJSON(const ALRPCMessage* msg);

// RegisterAppInterface_request
  static bool fromString(const std::string& str, RegisterAppInterface_request& res)
  {
    return mRegisterAppInterface_request.fromString(str, res);
  }

  static std::string toString(const RegisterAppInterface_request& res)
  {
    return mRegisterAppInterface_request.toString(res);
  }

// RegisterAppInterface_response
  static bool fromString(const std::string& str, RegisterAppInterface_response& res)
  {
    return mRegisterAppInterface_response.fromString(str, res);
  }

  static std::string toString(const RegisterAppInterface_response& res)
  {
    return mRegisterAppInterface_response.toString(res);
  }

// UnregisterAppInterface_request
  static bool fromString(const std::string& str, UnregisterAppInterface_request& res)
  {
    return mUnregisterAppInterface_request.fromString(str, res);
  }

  static std::string toString(const UnregisterAppInterface_request& res)
  {
    return mUnregisterAppInterface_request.toString(res);
  }

// UnregisterAppInterface_response
  static bool fromString(const std::string& str, UnregisterAppInterface_response& res)
  {
    return mUnregisterAppInterface_response.fromString(str, res);
  }

  static std::string toString(const UnregisterAppInterface_response& res)
  {
    return mUnregisterAppInterface_response.toString(res);
  }

// SetGlobalProperties_request
  static bool fromString(const std::string& str, SetGlobalProperties_request& res)
  {
    return mSetGlobalProperties_request.fromString(str, res);
  }

  static std::string toString(const SetGlobalProperties_request& res)
  {
    return mSetGlobalProperties_request.toString(res);
  }

// SetGlobalProperties_response
  static bool fromString(const std::string& str, SetGlobalProperties_response& res)
  {
    return mSetGlobalProperties_response.fromString(str, res);
  }

  static std::string toString(const SetGlobalProperties_response& res)
  {
    return mSetGlobalProperties_response.toString(res);
  }

// ResetGlobalProperties_request
  static bool fromString(const std::string& str, ResetGlobalProperties_request& res)
  {
    return mResetGlobalProperties_request.fromString(str, res);
  }

  static std::string toString(const ResetGlobalProperties_request& res)
  {
    return mResetGlobalProperties_request.toString(res);
  }

// ResetGlobalProperties_response
  static bool fromString(const std::string& str, ResetGlobalProperties_response& res)
  {
    return mResetGlobalProperties_response.fromString(str, res);
  }

  static std::string toString(const ResetGlobalProperties_response& res)
  {
    return mResetGlobalProperties_response.toString(res);
  }

// AddCommand_request
  static bool fromString(const std::string& str, AddCommand_request& res)
  {
    return mAddCommand_request.fromString(str, res);
  }

  static std::string toString(const AddCommand_request& res)
  {
    return mAddCommand_request.toString(res);
  }

// AddCommand_response
  static bool fromString(const std::string& str, AddCommand_response& res)
  {
    return mAddCommand_response.fromString(str, res);
  }

  static std::string toString(const AddCommand_response& res)
  {
    return mAddCommand_response.toString(res);
  }

// DeleteCommand_request
  static bool fromString(const std::string& str, DeleteCommand_request& res)
  {
    return mDeleteCommand_request.fromString(str, res);
  }

  static std::string toString(const DeleteCommand_request& res)
  {
    return mDeleteCommand_request.toString(res);
  }

// DeleteCommand_response
  static bool fromString(const std::string& str, DeleteCommand_response& res)
  {
    return mDeleteCommand_response.fromString(str, res);
  }

  static std::string toString(const DeleteCommand_response& res)
  {
    return mDeleteCommand_response.toString(res);
  }

// AddSubMenu_request
  static bool fromString(const std::string& str, AddSubMenu_request& res)
  {
    return mAddSubMenu_request.fromString(str, res);
  }

  static std::string toString(const AddSubMenu_request& res)
  {
    return mAddSubMenu_request.toString(res);
  }

// AddSubMenu_response
  static bool fromString(const std::string& str, AddSubMenu_response& res)
  {
    return mAddSubMenu_response.fromString(str, res);
  }

  static std::string toString(const AddSubMenu_response& res)
  {
    return mAddSubMenu_response.toString(res);
  }

// DeleteSubMenu_request
  static bool fromString(const std::string& str, DeleteSubMenu_request& res)
  {
    return mDeleteSubMenu_request.fromString(str, res);
  }

  static std::string toString(const DeleteSubMenu_request& res)
  {
    return mDeleteSubMenu_request.toString(res);
  }

// DeleteSubMenu_response
  static bool fromString(const std::string& str, DeleteSubMenu_response& res)
  {
    return mDeleteSubMenu_response.fromString(str, res);
  }

  static std::string toString(const DeleteSubMenu_response& res)
  {
    return mDeleteSubMenu_response.toString(res);
  }

// CreateInteractionChoiceSet_request
  static bool fromString(const std::string& str, CreateInteractionChoiceSet_request& res)
  {
    return mCreateInteractionChoiceSet_request.fromString(str, res);
  }

  static std::string toString(const CreateInteractionChoiceSet_request& res)
  {
    return mCreateInteractionChoiceSet_request.toString(res);
  }

// CreateInteractionChoiceSet_response
  static bool fromString(const std::string& str, CreateInteractionChoiceSet_response& res)
  {
    return mCreateInteractionChoiceSet_response.fromString(str, res);
  }

  static std::string toString(const CreateInteractionChoiceSet_response& res)
  {
    return mCreateInteractionChoiceSet_response.toString(res);
  }

// PerformInteraction_request
  static bool fromString(const std::string& str, PerformInteraction_request& res)
  {
    return mPerformInteraction_request.fromString(str, res);
  }

  static std::string toString(const PerformInteraction_request& res)
  {
    return mPerformInteraction_request.toString(res);
  }

// PerformInteraction_response
  static bool fromString(const std::string& str, PerformInteraction_response& res)
  {
    return mPerformInteraction_response.fromString(str, res);
  }

  static std::string toString(const PerformInteraction_response& res)
  {
    return mPerformInteraction_response.toString(res);
  }

// DeleteInteractionChoiceSet_request
  static bool fromString(const std::string& str, DeleteInteractionChoiceSet_request& res)
  {
    return mDeleteInteractionChoiceSet_request.fromString(str, res);
  }

  static std::string toString(const DeleteInteractionChoiceSet_request& res)
  {
    return mDeleteInteractionChoiceSet_request.toString(res);
  }

// DeleteInteractionChoiceSet_response
  static bool fromString(const std::string& str, DeleteInteractionChoiceSet_response& res)
  {
    return mDeleteInteractionChoiceSet_response.fromString(str, res);
  }

  static std::string toString(const DeleteInteractionChoiceSet_response& res)
  {
    return mDeleteInteractionChoiceSet_response.toString(res);
  }

// Alert_request
  static bool fromString(const std::string& str, Alert_request& res)
  {
    return mAlert_request.fromString(str, res);
  }

  static std::string toString(const Alert_request& res)
  {
    return mAlert_request.toString(res);
  }

// Alert_response
  static bool fromString(const std::string& str, Alert_response& res)
  {
    return mAlert_response.fromString(str, res);
  }

  static std::string toString(const Alert_response& res)
  {
    return mAlert_response.toString(res);
  }

// Show_request
  static bool fromString(const std::string& str, Show_request& res)
  {
    return mShow_request.fromString(str, res);
  }

  static std::string toString(const Show_request& res)
  {
    return mShow_request.toString(res);
  }

// Show_response
  static bool fromString(const std::string& str, Show_response& res)
  {
    return mShow_response.fromString(str, res);
  }

  static std::string toString(const Show_response& res)
  {
    return mShow_response.toString(res);
  }

// Speak_request
  static bool fromString(const std::string& str, Speak_request& res)
  {
    return mSpeak_request.fromString(str, res);
  }

  static std::string toString(const Speak_request& res)
  {
    return mSpeak_request.toString(res);
  }

// Speak_response
  static bool fromString(const std::string& str, Speak_response& res)
  {
    return mSpeak_response.fromString(str, res);
  }

  static std::string toString(const Speak_response& res)
  {
    return mSpeak_response.toString(res);
  }

// SetMediaClockTimer_request
  static bool fromString(const std::string& str, SetMediaClockTimer_request& res)
  {
    return mSetMediaClockTimer_request.fromString(str, res);
  }

  static std::string toString(const SetMediaClockTimer_request& res)
  {
    return mSetMediaClockTimer_request.toString(res);
  }

// SetMediaClockTimer_response
  static bool fromString(const std::string& str, SetMediaClockTimer_response& res)
  {
    return mSetMediaClockTimer_response.fromString(str, res);
  }

  static std::string toString(const SetMediaClockTimer_response& res)
  {
    return mSetMediaClockTimer_response.toString(res);
  }

// EncodedSyncPData_request
  static bool fromString(const std::string& str, EncodedSyncPData_request& res)
  {
    return mEncodedSyncPData_request.fromString(str, res);
  }

  static std::string toString(const EncodedSyncPData_request& res)
  {
    return mEncodedSyncPData_request.toString(res);
  }

// EncodedSyncPData_response
  static bool fromString(const std::string& str, EncodedSyncPData_response& res)
  {
    return mEncodedSyncPData_response.fromString(str, res);
  }

  static std::string toString(const EncodedSyncPData_response& res)
  {
    return mEncodedSyncPData_response.toString(res);
  }

// DialNumber_request
  static bool fromString(const std::string& str, DialNumber_request& res)
  {
    return mDialNumber_request.fromString(str, res);
  }

  static std::string toString(const DialNumber_request& res)
  {
    return mDialNumber_request.toString(res);
  }

// DialNumber_response
  static bool fromString(const std::string& str, DialNumber_response& res)
  {
    return mDialNumber_response.fromString(str, res);
  }

  static std::string toString(const DialNumber_response& res)
  {
    return mDialNumber_response.toString(res);
  }

// PerformAudioPassThru_request
  static bool fromString(const std::string& str, PerformAudioPassThru_request& res)
  {
    return mPerformAudioPassThru_request.fromString(str, res);
  }

  static std::string toString(const PerformAudioPassThru_request& res)
  {
    return mPerformAudioPassThru_request.toString(res);
  }

// PerformAudioPassThru_response
  static bool fromString(const std::string& str, PerformAudioPassThru_response& res)
  {
    return mPerformAudioPassThru_response.fromString(str, res);
  }

  static std::string toString(const PerformAudioPassThru_response& res)
  {
    return mPerformAudioPassThru_response.toString(res);
  }

// EndAudioPassThru_request
  static bool fromString(const std::string& str, EndAudioPassThru_request& res)
  {
    return mEndAudioPassThru_request.fromString(str, res);
  }

  static std::string toString(const EndAudioPassThru_request& res)
  {
    return mEndAudioPassThru_request.toString(res);
  }

// EndAudioPassThru_response
  static bool fromString(const std::string& str, EndAudioPassThru_response& res)
  {
    return mEndAudioPassThru_response.fromString(str, res);
  }

  static std::string toString(const EndAudioPassThru_response& res)
  {
    return mEndAudioPassThru_response.toString(res);
  }

// SubscribeButton_request
  static bool fromString(const std::string& str, SubscribeButton_request& res)
  {
    return mSubscribeButton_request.fromString(str, res);
  }

  static std::string toString(const SubscribeButton_request& res)
  {
    return mSubscribeButton_request.toString(res);
  }

// SubscribeButton_response
  static bool fromString(const std::string& str, SubscribeButton_response& res)
  {
    return mSubscribeButton_response.fromString(str, res);
  }

  static std::string toString(const SubscribeButton_response& res)
  {
    return mSubscribeButton_response.toString(res);
  }

// UnsubscribeButton_request
  static bool fromString(const std::string& str, UnsubscribeButton_request& res)
  {
    return mUnsubscribeButton_request.fromString(str, res);
  }

  static std::string toString(const UnsubscribeButton_request& res)
  {
    return mUnsubscribeButton_request.toString(res);
  }

// UnsubscribeButton_response
  static bool fromString(const std::string& str, UnsubscribeButton_response& res)
  {
    return mUnsubscribeButton_response.fromString(str, res);
  }

  static std::string toString(const UnsubscribeButton_response& res)
  {
    return mUnsubscribeButton_response.toString(res);
  }

// SubscribeVehicleData_request
  static bool fromString(const std::string& str, SubscribeVehicleData_request& res)
  {
    return mSubscribeVehicleData_request.fromString(str, res);
  }

  static std::string toString(const SubscribeVehicleData_request& res)
  {
    return mSubscribeVehicleData_request.toString(res);
  }

// SubscribeVehicleData_response
  static bool fromString(const std::string& str, SubscribeVehicleData_response& res)
  {
    return mSubscribeVehicleData_response.fromString(str, res);
  }

  static std::string toString(const SubscribeVehicleData_response& res)
  {
    return mSubscribeVehicleData_response.toString(res);
  }

// UnsubscribeVehicleData_request
  static bool fromString(const std::string& str, UnsubscribeVehicleData_request& res)
  {
    return mUnsubscribeVehicleData_request.fromString(str, res);
  }

  static std::string toString(const UnsubscribeVehicleData_request& res)
  {
    return mUnsubscribeVehicleData_request.toString(res);
  }

// UnsubscribeVehicleData_response
  static bool fromString(const std::string& str, UnsubscribeVehicleData_response& res)
  {
    return mUnsubscribeVehicleData_response.fromString(str, res);
  }

  static std::string toString(const UnsubscribeVehicleData_response& res)
  {
    return mUnsubscribeVehicleData_response.toString(res);
  }

// GetVehicleData_request
  static bool fromString(const std::string& str, GetVehicleData_request& res)
  {
    return mGetVehicleData_request.fromString(str, res);
  }

  static std::string toString(const GetVehicleData_request& res)
  {
    return mGetVehicleData_request.toString(res);
  }

// GetVehicleData_response
  static bool fromString(const std::string& str, GetVehicleData_response& res)
  {
    return mGetVehicleData_response.fromString(str, res);
  }

  static std::string toString(const GetVehicleData_response& res)
  {
    return mGetVehicleData_response.toString(res);
  }

// ReadDID_request
  static bool fromString(const std::string& str, ReadDID_request& res)
  {
    return mReadDID_request.fromString(str, res);
  }

  static std::string toString(const ReadDID_request& res)
  {
    return mReadDID_request.toString(res);
  }

// ReadDID_response
  static bool fromString(const std::string& str, ReadDID_response& res)
  {
    return mReadDID_response.fromString(str, res);
  }

  static std::string toString(const ReadDID_response& res)
  {
    return mReadDID_response.toString(res);
  }

// GetDTCs_request
  static bool fromString(const std::string& str, GetDTCs_request& res)
  {
    return mGetDTCs_request.fromString(str, res);
  }

  static std::string toString(const GetDTCs_request& res)
  {
    return mGetDTCs_request.toString(res);
  }

// GetDTCs_response
  static bool fromString(const std::string& str, GetDTCs_response& res)
  {
    return mGetDTCs_response.fromString(str, res);
  }

  static std::string toString(const GetDTCs_response& res)
  {
    return mGetDTCs_response.toString(res);
  }

// ScrollableMessage_request
  static bool fromString(const std::string& str, ScrollableMessage_request& res)
  {
    return mScrollableMessage_request.fromString(str, res);
  }

  static std::string toString(const ScrollableMessage_request& res)
  {
    return mScrollableMessage_request.toString(res);
  }

// ScrollableMessage_response
  static bool fromString(const std::string& str, ScrollableMessage_response& res)
  {
    return mScrollableMessage_response.fromString(str, res);
  }

  static std::string toString(const ScrollableMessage_response& res)
  {
    return mScrollableMessage_response.toString(res);
  }

// Slider_request
  static bool fromString(const std::string& str, Slider_request& res)
  {
    return mSlider_request.fromString(str, res);
  }

  static std::string toString(const Slider_request& res)
  {
    return mSlider_request.toString(res);
  }

// Slider_response
  static bool fromString(const std::string& str, Slider_response& res)
  {
    return mSlider_response.fromString(str, res);
  }

  static std::string toString(const Slider_response& res)
  {
    return mSlider_response.toString(res);
  }

// ShowConstantTBT_request
  static bool fromString(const std::string& str, ShowConstantTBT_request& res)
  {
    return mShowConstantTBT_request.fromString(str, res);
  }

  static std::string toString(const ShowConstantTBT_request& res)
  {
    return mShowConstantTBT_request.toString(res);
  }

// ShowConstantTBT_response
  static bool fromString(const std::string& str, ShowConstantTBT_response& res)
  {
    return mShowConstantTBT_response.fromString(str, res);
  }

  static std::string toString(const ShowConstantTBT_response& res)
  {
    return mShowConstantTBT_response.toString(res);
  }

// AlertManeuver_request
  static bool fromString(const std::string& str, AlertManeuver_request& res)
  {
    return mAlertManeuver_request.fromString(str, res);
  }

  static std::string toString(const AlertManeuver_request& res)
  {
    return mAlertManeuver_request.toString(res);
  }

// AlertManeuver_response
  static bool fromString(const std::string& str, AlertManeuver_response& res)
  {
    return mAlertManeuver_response.fromString(str, res);
  }

  static std::string toString(const AlertManeuver_response& res)
  {
    return mAlertManeuver_response.toString(res);
  }

// UpdateTurnList_request
  static bool fromString(const std::string& str, UpdateTurnList_request& res)
  {
    return mUpdateTurnList_request.fromString(str, res);
  }

  static std::string toString(const UpdateTurnList_request& res)
  {
    return mUpdateTurnList_request.toString(res);
  }

// UpdateTurnList_response
  static bool fromString(const std::string& str, UpdateTurnList_response& res)
  {
    return mUpdateTurnList_response.fromString(str, res);
  }

  static std::string toString(const UpdateTurnList_response& res)
  {
    return mUpdateTurnList_response.toString(res);
  }

// ChangeLanguageRegistration_request
  static bool fromString(const std::string& str, ChangeLanguageRegistration_request& res)
  {
    return mChangeLanguageRegistration_request.fromString(str, res);
  }

  static std::string toString(const ChangeLanguageRegistration_request& res)
  {
    return mChangeLanguageRegistration_request.toString(res);
  }

// ChangeLanguageRegistration_response
  static bool fromString(const std::string& str, ChangeLanguageRegistration_response& res)
  {
    return mChangeLanguageRegistration_response.fromString(str, res);
  }

  static std::string toString(const ChangeLanguageRegistration_response& res)
  {
    return mChangeLanguageRegistration_response.toString(res);
  }

// GenericResponse_response
  static bool fromString(const std::string& str, GenericResponse_response& res)
  {
    return mGenericResponse_response.fromString(str, res);
  }

  static std::string toString(const GenericResponse_response& res)
  {
    return mGenericResponse_response.toString(res);
  }

// DeleteFile_request
  static bool fromString(const std::string& str, DeleteFile_request& res)
  {
    return mDeleteFile_request.fromString(str, res);
  }

  static std::string toString(const DeleteFile_request& res)
  {
    return mDeleteFile_request.toString(res);
  }

// DeleteFile_response
  static bool fromString(const std::string& str, DeleteFile_response& res)
  {
    return mDeleteFile_response.fromString(str, res);
  }

  static std::string toString(const DeleteFile_response& res)
  {
    return mDeleteFile_response.toString(res);
  }

// ListFiles_request
  static bool fromString(const std::string& str, ListFiles_request& res)
  {
    return mListFiles_request.fromString(str, res);
  }

  static std::string toString(const ListFiles_request& res)
  {
    return mListFiles_request.toString(res);
  }

// ListFiles_response
  static bool fromString(const std::string& str, ListFiles_response& res)
  {
    return mListFiles_response.fromString(str, res);
  }

  static std::string toString(const ListFiles_response& res)
  {
    return mListFiles_response.toString(res);
  }

// SetAppIcon_request
  static bool fromString(const std::string& str, SetAppIcon_request& res)
  {
    return mSetAppIcon_request.fromString(str, res);
  }

  static std::string toString(const SetAppIcon_request& res)
  {
    return mSetAppIcon_request.toString(res);
  }

// SetAppIcon_response
  static bool fromString(const std::string& str, SetAppIcon_response& res)
  {
    return mSetAppIcon_response.fromString(str, res);
  }

  static std::string toString(const SetAppIcon_response& res)
  {
    return mSetAppIcon_response.toString(res);
  }

// OnHMIStatus
  static bool fromString(const std::string& str, OnHMIStatus& res)
  {
    return mOnHMIStatus.fromString(str, res);
  }

  static std::string toString(const OnHMIStatus& res)
  {
    return mOnHMIStatus.toString(res);
  }

// OnAppInterfaceUnregistered
  static bool fromString(const std::string& str, OnAppInterfaceUnregistered& res)
  {
    return mOnAppInterfaceUnregistered.fromString(str, res);
  }

  static std::string toString(const OnAppInterfaceUnregistered& res)
  {
    return mOnAppInterfaceUnregistered.toString(res);
  }

// OnButtonEvent
  static bool fromString(const std::string& str, OnButtonEvent& res)
  {
    return mOnButtonEvent.fromString(str, res);
  }

  static std::string toString(const OnButtonEvent& res)
  {
    return mOnButtonEvent.toString(res);
  }

// OnButtonPress
  static bool fromString(const std::string& str, OnButtonPress& res)
  {
    return mOnButtonPress.fromString(str, res);
  }

  static std::string toString(const OnButtonPress& res)
  {
    return mOnButtonPress.toString(res);
  }

// OnVehicleData
  static bool fromString(const std::string& str, OnVehicleData& res)
  {
    return mOnVehicleData.fromString(str, res);
  }

  static std::string toString(const OnVehicleData& res)
  {
    return mOnVehicleData.toString(res);
  }

// OnCommand
  static bool fromString(const std::string& str, OnCommand& res)
  {
    return mOnCommand.fromString(str, res);
  }

  static std::string toString(const OnCommand& res)
  {
    return mOnCommand.toString(res);
  }

// OnEncodedSyncPData
  static bool fromString(const std::string& str, OnEncodedSyncPData& res)
  {
    return mOnEncodedSyncPData.fromString(str, res);
  }

  static std::string toString(const OnEncodedSyncPData& res)
  {
    return mOnEncodedSyncPData.toString(res);
  }

// OnTBTClientState
  static bool fromString(const std::string& str, OnTBTClientState& res)
  {
    return mOnTBTClientState.fromString(str, res);
  }

  static std::string toString(const OnTBTClientState& res)
  {
    return mOnTBTClientState.toString(res);
  }

// OnDriverDistraction
  static bool fromString(const std::string& str, OnDriverDistraction& res)
  {
    return mOnDriverDistraction.fromString(str, res);
  }

  static std::string toString(const OnDriverDistraction& res)
  {
    return mOnDriverDistraction.toString(res);
  }

// OnPermissionsChange
  static bool fromString(const std::string& str, OnPermissionsChange& res)
  {
    return mOnPermissionsChange.fromString(str, res);
  }

  static std::string toString(const OnPermissionsChange& res)
  {
    return mOnPermissionsChange.toString(res);
  }

// OnAudioPassThru
  static bool fromString(const std::string& str, OnAudioPassThru& res)
  {
    return mOnAudioPassThru.fromString(str, res);
  }

  static std::string toString(const OnAudioPassThru& res)
  {
    return mOnAudioPassThru.toString(res);
  }

// OnLanguageChange
  static bool fromString(const std::string& str, OnLanguageChange& res)
  {
    return mOnLanguageChange.fromString(str, res);
  }

  static std::string toString(const OnLanguageChange& res)
  {
    return mOnLanguageChange.toString(res);
  }

  struct localHash
  {
    const char *name;
    unsigned int idx;
    void* marshaller;					// Bingo!!!  old good plain C with direct casting!!!
  };

private:

  static RegisterAppInterface_requestMarshaller mRegisterAppInterface_request;
  static RegisterAppInterface_responseMarshaller mRegisterAppInterface_response;
  static UnregisterAppInterface_requestMarshaller mUnregisterAppInterface_request;
  static UnregisterAppInterface_responseMarshaller mUnregisterAppInterface_response;
  static SetGlobalProperties_requestMarshaller mSetGlobalProperties_request;
  static SetGlobalProperties_responseMarshaller mSetGlobalProperties_response;
  static ResetGlobalProperties_requestMarshaller mResetGlobalProperties_request;
  static ResetGlobalProperties_responseMarshaller mResetGlobalProperties_response;
  static AddCommand_requestMarshaller mAddCommand_request;
  static AddCommand_responseMarshaller mAddCommand_response;
  static DeleteCommand_requestMarshaller mDeleteCommand_request;
  static DeleteCommand_responseMarshaller mDeleteCommand_response;
  static AddSubMenu_requestMarshaller mAddSubMenu_request;
  static AddSubMenu_responseMarshaller mAddSubMenu_response;
  static DeleteSubMenu_requestMarshaller mDeleteSubMenu_request;
  static DeleteSubMenu_responseMarshaller mDeleteSubMenu_response;
  static CreateInteractionChoiceSet_requestMarshaller mCreateInteractionChoiceSet_request;
  static CreateInteractionChoiceSet_responseMarshaller mCreateInteractionChoiceSet_response;
  static PerformInteraction_requestMarshaller mPerformInteraction_request;
  static PerformInteraction_responseMarshaller mPerformInteraction_response;
  static DeleteInteractionChoiceSet_requestMarshaller mDeleteInteractionChoiceSet_request;
  static DeleteInteractionChoiceSet_responseMarshaller mDeleteInteractionChoiceSet_response;
  static Alert_requestMarshaller mAlert_request;
  static Alert_responseMarshaller mAlert_response;
  static Show_requestMarshaller mShow_request;
  static Show_responseMarshaller mShow_response;
  static Speak_requestMarshaller mSpeak_request;
  static Speak_responseMarshaller mSpeak_response;
  static SetMediaClockTimer_requestMarshaller mSetMediaClockTimer_request;
  static SetMediaClockTimer_responseMarshaller mSetMediaClockTimer_response;
  static EncodedSyncPData_requestMarshaller mEncodedSyncPData_request;
  static EncodedSyncPData_responseMarshaller mEncodedSyncPData_response;
  static DialNumber_requestMarshaller mDialNumber_request;
  static DialNumber_responseMarshaller mDialNumber_response;
  static PerformAudioPassThru_requestMarshaller mPerformAudioPassThru_request;
  static PerformAudioPassThru_responseMarshaller mPerformAudioPassThru_response;
  static EndAudioPassThru_requestMarshaller mEndAudioPassThru_request;
  static EndAudioPassThru_responseMarshaller mEndAudioPassThru_response;
  static SubscribeButton_requestMarshaller mSubscribeButton_request;
  static SubscribeButton_responseMarshaller mSubscribeButton_response;
  static UnsubscribeButton_requestMarshaller mUnsubscribeButton_request;
  static UnsubscribeButton_responseMarshaller mUnsubscribeButton_response;
  static SubscribeVehicleData_requestMarshaller mSubscribeVehicleData_request;
  static SubscribeVehicleData_responseMarshaller mSubscribeVehicleData_response;
  static UnsubscribeVehicleData_requestMarshaller mUnsubscribeVehicleData_request;
  static UnsubscribeVehicleData_responseMarshaller mUnsubscribeVehicleData_response;
  static GetVehicleData_requestMarshaller mGetVehicleData_request;
  static GetVehicleData_responseMarshaller mGetVehicleData_response;
  static ReadDID_requestMarshaller mReadDID_request;
  static ReadDID_responseMarshaller mReadDID_response;
  static GetDTCs_requestMarshaller mGetDTCs_request;
  static GetDTCs_responseMarshaller mGetDTCs_response;
  static ScrollableMessage_requestMarshaller mScrollableMessage_request;
  static ScrollableMessage_responseMarshaller mScrollableMessage_response;
  static Slider_requestMarshaller mSlider_request;
  static Slider_responseMarshaller mSlider_response;
  static ShowConstantTBT_requestMarshaller mShowConstantTBT_request;
  static ShowConstantTBT_responseMarshaller mShowConstantTBT_response;
  static AlertManeuver_requestMarshaller mAlertManeuver_request;
  static AlertManeuver_responseMarshaller mAlertManeuver_response;
  static UpdateTurnList_requestMarshaller mUpdateTurnList_request;
  static UpdateTurnList_responseMarshaller mUpdateTurnList_response;
  static ChangeLanguageRegistration_requestMarshaller mChangeLanguageRegistration_request;
  static ChangeLanguageRegistration_responseMarshaller mChangeLanguageRegistration_response;
  static GenericResponse_responseMarshaller mGenericResponse_response;
  static DeleteFile_requestMarshaller mDeleteFile_request;
  static DeleteFile_responseMarshaller mDeleteFile_response;
  static ListFiles_requestMarshaller mListFiles_request;
  static ListFiles_responseMarshaller mListFiles_response;
  static SetAppIcon_requestMarshaller mSetAppIcon_request;
  static SetAppIcon_responseMarshaller mSetAppIcon_response;
  static OnHMIStatusMarshaller mOnHMIStatus;
  static OnAppInterfaceUnregisteredMarshaller mOnAppInterfaceUnregistered;
  static OnButtonEventMarshaller mOnButtonEvent;
  static OnButtonPressMarshaller mOnButtonPress;
  static OnVehicleDataMarshaller mOnVehicleData;
  static OnCommandMarshaller mOnCommand;
  static OnEncodedSyncPDataMarshaller mOnEncodedSyncPData;
  static OnTBTClientStateMarshaller mOnTBTClientState;
  static OnDriverDistractionMarshaller mOnDriverDistraction;
  static OnPermissionsChangeMarshaller mOnPermissionsChange;
  static OnAudioPassThruMarshaller mOnAudioPassThru;
  static OnLanguageChangeMarshaller mOnLanguageChange;

  static const char* getName(Methods e)
  {
     return (e>=0 && e<83) ? mHashTable[e].name : NULL;
  }
   
  static const Methods getIndex(const char* s);

  static const localHash mHashTable[83];
  
  friend class Marshaller_intHash;
};

#endif
