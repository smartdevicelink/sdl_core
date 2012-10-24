%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name FunctionID_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
RESERVED,0
RegisterAppInterfaceID,1
UnregisterAppInterfaceID,2
SetGlobalPropertiesID,3
ResetGlobalPropertiesID,4
AddCommandID,5
DeleteCommandID,6
AddSubMenuID,7
DeleteSubMenuID,8
CreateInteractionChoiceSetID,9
PerformInteractionID,10
DeleteInteractionChoiceSetID,11
AlertID,12
ShowID,13
SpeakID,14
SetMediaClockTimerID,15
EncodedSyncPDataID,16
DialNumberID,17
PerformAudioPassThruID,18
EndAudioPassThruID,19
SubscribeButtonID,20
UnsubscribeButtonID,21
SubscribeVehicleDataID,22
UnsubscribeVehicleDataID,23
GetVehicleDataID,24
ReadDIDID,25
GetDTCsID,26
ScrollableMessageID,27
SliderID,28
ShowConstantTBTID,29
AlertManeuverID,30
UpdateTurnListID,31
ChangeLanguageRegistrationID,32
GenericResponseID,33
PutFileID,34
DeleteFileID,35
ListFilesID,36
SetAppIconID,37
SetDisplayLayoutID,38
OnHMIStatusID,39
OnAppInterfaceUnregisteredID,40
OnButtonEventID,41
OnButtonPressID,42
OnVehicleDataID,43
OnCommandID,44
OnEncodedSyncPDataID,45
OnTBTClientStateID,46
OnDriverDistractionID,47
OnPermissionsChangeID,48
OnAudioPassThruID,49
OnLanguageChangeID,50
