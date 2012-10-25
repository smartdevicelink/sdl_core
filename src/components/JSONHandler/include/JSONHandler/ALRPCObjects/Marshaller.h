#ifndef MARSHALLER_INCLUDE
#define MARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

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
#include "../../../src/ALRPCObjectsImpl/SubscribeButton_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/SubscribeButton_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/UnsubscribeButton_requestMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/UnsubscribeButton_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/GenericResponse_responseMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnHMIStatusMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnAppInterfaceUnregisteredMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnButtonEventMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnButtonPressMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnCommandMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnEncodedSyncPDataMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnTBTClientStateMarshaller.h"
#include "../../../src/ALRPCObjectsImpl/OnDriverDistractionMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
    METHOD_SUBSCRIBEBUTTON_REQUEST=32,
    METHOD_SUBSCRIBEBUTTON_RESPONSE=33,
    METHOD_UNSUBSCRIBEBUTTON_REQUEST=34,
    METHOD_UNSUBSCRIBEBUTTON_RESPONSE=35,
    METHOD_GENERICRESPONSE_RESPONSE=36,
    METHOD_ONHMISTATUS=37,
    METHOD_ONAPPINTERFACEUNREGISTERED=38,
    METHOD_ONBUTTONEVENT=39,
    METHOD_ONBUTTONPRESS=40,
    METHOD_ONCOMMAND=41,
    METHOD_ONENCODEDSYNCPDATA=42,
    METHOD_ONTBTCLIENTSTATE=43,
    METHOD_ONDRIVERDISTRACTION=44
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

// GenericResponse_response
  static bool fromString(const std::string& str, GenericResponse_response& res)
  {
    return mGenericResponse_response.fromString(str, res);
  }

  static std::string toString(const GenericResponse_response& res)
  {
    return mGenericResponse_response.toString(res);
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
  static SubscribeButton_requestMarshaller mSubscribeButton_request;
  static SubscribeButton_responseMarshaller mSubscribeButton_response;
  static UnsubscribeButton_requestMarshaller mUnsubscribeButton_request;
  static UnsubscribeButton_responseMarshaller mUnsubscribeButton_response;
  static GenericResponse_responseMarshaller mGenericResponse_response;
  static OnHMIStatusMarshaller mOnHMIStatus;
  static OnAppInterfaceUnregisteredMarshaller mOnAppInterfaceUnregistered;
  static OnButtonEventMarshaller mOnButtonEvent;
  static OnButtonPressMarshaller mOnButtonPress;
  static OnCommandMarshaller mOnCommand;
  static OnEncodedSyncPDataMarshaller mOnEncodedSyncPData;
  static OnTBTClientStateMarshaller mOnTBTClientState;
  static OnDriverDistractionMarshaller mOnDriverDistraction;

  static const char* getName(Methods e)
  {
     return (e>=0 && e<45) ? mHashTable[e].name : NULL;
  }
   
  static const Methods getIndex(const char* s);

  static const localHash mHashTable[45];
  
  friend class Marshaller_intHash;
};

#endif
