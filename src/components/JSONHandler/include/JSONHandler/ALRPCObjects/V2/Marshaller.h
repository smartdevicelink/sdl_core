#ifndef NSAPPLINKRPCV2_MARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "FunctionID.h"
#include "messageType.h"

#include "JSONHandler/ALRPCMessage.h"
#include "../src/ALRPCObjectsImpl/V2/RegisterAppInterface_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/RegisterAppInterface_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/UnregisterAppInterface_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/UnregisterAppInterface_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SetGlobalProperties_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SetGlobalProperties_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResetGlobalProperties_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResetGlobalProperties_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/AddCommand_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/AddCommand_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/DeleteCommand_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/DeleteCommand_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/AddSubMenu_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/AddSubMenu_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/DeleteSubMenu_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/DeleteSubMenu_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/CreateInteractionChoiceSet_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/CreateInteractionChoiceSet_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/PerformInteraction_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/PerformInteraction_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/DeleteInteractionChoiceSet_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/DeleteInteractionChoiceSet_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/Alert_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/Alert_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/Show_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/Show_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/Speak_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/Speak_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SetMediaClockTimer_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SetMediaClockTimer_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/EncodedSyncPData_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/EncodedSyncPData_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/DialNumber_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/DialNumber_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/PerformAudioPassThru_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/PerformAudioPassThru_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/EndAudioPassThru_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/EndAudioPassThru_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SubscribeButton_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SubscribeButton_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/UnsubscribeButton_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/UnsubscribeButton_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SubscribeVehicleData_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SubscribeVehicleData_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/UnsubscribeVehicleData_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/UnsubscribeVehicleData_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/GetVehicleData_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/GetVehicleData_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ReadDID_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ReadDID_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/GetDTCs_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/GetDTCs_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ScrollableMessage_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ScrollableMessage_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/Slider_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/Slider_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ShowConstantTBT_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ShowConstantTBT_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/AlertManeuver_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/AlertManeuver_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/UpdateTurnList_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/UpdateTurnList_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ChangeRegistration_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ChangeRegistration_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/GenericResponse_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/PutFile_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/PutFile_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/DeleteFile_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/DeleteFile_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ListFiles_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ListFiles_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SetAppIcon_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SetAppIcon_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SetDisplayLayout_requestMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/SetDisplayLayout_responseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnHMIStatusMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnAppInterfaceUnregisteredMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnButtonEventMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnButtonPressMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnVehicleDataMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnCommandMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnEncodedSyncPDataMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnTBTClientStateMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnDriverDistractionMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnPermissionsChangeMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnAudioPassThruMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/OnLanguageChangeMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{
  class Marshaller
  {
  public:

    Marshaller()					{}
    ~Marshaller()					{}
    
    static NsAppLinkRPC::ALRPCMessage* fromString(const std::string&,FunctionID::FunctionIDInternal fid, messageType::messageTypeInternal mt);
    static std::string toString(const NsAppLinkRPC::ALRPCMessage* msg,FunctionID::FunctionIDInternal fid, messageType::messageTypeInternal mt);

    static NsAppLinkRPC::ALRPCMessage* fromJSON(const Json::Value&,FunctionID::FunctionIDInternal fid, messageType::messageTypeInternal mt);
    static Json::Value toJSON(const NsAppLinkRPC::ALRPCMessage* msg,FunctionID::FunctionIDInternal fid, messageType::messageTypeInternal mt);
  
  protected:
    

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

// ChangeRegistration_request
    static bool fromString(const std::string& str, ChangeRegistration_request& res)
    {
      return mChangeRegistration_request.fromString(str, res);
    }

    static std::string toString(const ChangeRegistration_request& res)
    {
      return mChangeRegistration_request.toString(res);
    }

// ChangeRegistration_response
    static bool fromString(const std::string& str, ChangeRegistration_response& res)
    {
      return mChangeRegistration_response.fromString(str, res);
    }

    static std::string toString(const ChangeRegistration_response& res)
    {
      return mChangeRegistration_response.toString(res);
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

// PutFile_request
    static bool fromString(const std::string& str, PutFile_request& res)
    {
      return mPutFile_request.fromString(str, res);
    }

    static std::string toString(const PutFile_request& res)
    {
      return mPutFile_request.toString(res);
    }

// PutFile_response
    static bool fromString(const std::string& str, PutFile_response& res)
    {
      return mPutFile_response.fromString(str, res);
    }

    static std::string toString(const PutFile_response& res)
    {
      return mPutFile_response.toString(res);
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

// SetDisplayLayout_request
    static bool fromString(const std::string& str, SetDisplayLayout_request& res)
    {
      return mSetDisplayLayout_request.fromString(str, res);
    }

    static std::string toString(const SetDisplayLayout_request& res)
    {
      return mSetDisplayLayout_request.toString(res);
    }

// SetDisplayLayout_response
    static bool fromString(const std::string& str, SetDisplayLayout_response& res)
    {
      return mSetDisplayLayout_response.fromString(str, res);
    }

    static std::string toString(const SetDisplayLayout_response& res)
    {
      return mSetDisplayLayout_response.toString(res);
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
    static ChangeRegistration_requestMarshaller mChangeRegistration_request;
    static ChangeRegistration_responseMarshaller mChangeRegistration_response;
    static GenericResponse_responseMarshaller mGenericResponse_response;
    static PutFile_requestMarshaller mPutFile_request;
    static PutFile_responseMarshaller mPutFile_response;
    static DeleteFile_requestMarshaller mDeleteFile_request;
    static DeleteFile_responseMarshaller mDeleteFile_response;
    static ListFiles_requestMarshaller mListFiles_request;
    static ListFiles_responseMarshaller mListFiles_response;
    static SetAppIcon_requestMarshaller mSetAppIcon_request;
    static SetAppIcon_responseMarshaller mSetAppIcon_response;
    static SetDisplayLayout_requestMarshaller mSetDisplayLayout_request;
    static SetDisplayLayout_responseMarshaller mSetDisplayLayout_response;
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

private:

    static Json::Value toJSONparam(const NsAppLinkRPC::ALRPCMessage* msg,FunctionID::FunctionIDInternal fid, messageType::messageTypeInternal mt);
  };

}

#endif
