#ifndef FUNCTIONID_INCLUDE
#define FUNCTIONID_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


/**
     Enumeration linking function names with function IDs in WiPro protocol.
     Assumes enumeration starts at value 0.
*/

class FunctionID
{
public:
  enum FunctionIDInternal
  {
    INVALID_ENUM=-1,
    RESERVED=0,
    RegisterAppInterfaceID=1,
    UnregisterAppInterfaceID=2,
    SetGlobalPropertiesID=3,
    ResetGlobalPropertiesID=4,
    AddCommandID=5,
    DeleteCommandID=6,
    AddSubMenuID=7,
    DeleteSubMenuID=8,
    CreateInteractionChoiceSetID=9,
    PerformInteractionID=10,
    DeleteInteractionChoiceSetID=11,
    AlertID=12,
    ShowID=13,
    SpeakID=14,
    SetMediaClockTimerID=15,
    EncodedSyncPDataID=16,
    DialNumberID=17,
    PerformAudioPassThruID=18,
    EndAudioPassThruID=19,
    SubscribeButtonID=20,
    UnsubscribeButtonID=21,
    SubscribeVehicleDataID=22,
    UnsubscribeVehicleDataID=23,
    GetVehicleDataID=24,
    ReadDIDID=25,
    GetDTCsID=26,
    ScrollableMessageID=27,
    SliderID=28,
    ShowConstantTBTID=29,
    AlertManeuverID=30,
    UpdateTurnListID=31,
    ChangeLanguageRegistrationID=32,
    GenericResponseID=33,
    PutFileID=34,
    DeleteFileID=35,
    ListFilesID=36,
    SetAppIconID=37,
    SetDisplayLayoutID=38,
    OnHMIStatusID=39,
    OnAppInterfaceUnregisteredID=40,
    OnButtonEventID=41,
    OnButtonPressID=42,
    OnVehicleDataID=43,
    OnCommandID=44,
    OnEncodedSyncPDataID=45,
    OnTBTClientStateID=46,
    OnDriverDistractionID=47,
    OnPermissionsChangeID=48,
    OnAudioPassThruID=49,
    OnLanguageChangeID=50
  };

  FunctionID() : mInternal(INVALID_ENUM)				{}
  FunctionID(FunctionIDInternal e) : mInternal(e)		{}

  FunctionIDInternal get(void) const	{ return mInternal; }
  void set(FunctionIDInternal e)		{ mInternal=e; }

private:
  FunctionIDInternal mInternal;
  friend class FunctionIDMarshaller;
};

#endif
