%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name Marshaller_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsSmartDeviceLinkRPC::Marshaller::localHash
{
  const char *name;
  unsigned int idx;
  void* method;
};

%%
RegisterAppInterface_request,0,&NsSmartDeviceLinkRPC::Marshaller::mRegisterAppInterface_request
RegisterAppInterface_response,1,&NsSmartDeviceLinkRPC::Marshaller::mRegisterAppInterface_response
UnregisterAppInterface_request,2,&NsSmartDeviceLinkRPC::Marshaller::mUnregisterAppInterface_request
UnregisterAppInterface_response,3,&NsSmartDeviceLinkRPC::Marshaller::mUnregisterAppInterface_response
SetGlobalProperties_request,4,&NsSmartDeviceLinkRPC::Marshaller::mSetGlobalProperties_request
SetGlobalProperties_response,5,&NsSmartDeviceLinkRPC::Marshaller::mSetGlobalProperties_response
ResetGlobalProperties_request,6,&NsSmartDeviceLinkRPC::Marshaller::mResetGlobalProperties_request
ResetGlobalProperties_response,7,&NsSmartDeviceLinkRPC::Marshaller::mResetGlobalProperties_response
AddCommand_request,8,&NsSmartDeviceLinkRPC::Marshaller::mAddCommand_request
AddCommand_response,9,&NsSmartDeviceLinkRPC::Marshaller::mAddCommand_response
DeleteCommand_request,10,&NsSmartDeviceLinkRPC::Marshaller::mDeleteCommand_request
DeleteCommand_response,11,&NsSmartDeviceLinkRPC::Marshaller::mDeleteCommand_response
AddSubMenu_request,12,&NsSmartDeviceLinkRPC::Marshaller::mAddSubMenu_request
AddSubMenu_response,13,&NsSmartDeviceLinkRPC::Marshaller::mAddSubMenu_response
DeleteSubMenu_request,14,&NsSmartDeviceLinkRPC::Marshaller::mDeleteSubMenu_request
DeleteSubMenu_response,15,&NsSmartDeviceLinkRPC::Marshaller::mDeleteSubMenu_response
CreateInteractionChoiceSet_request,16,&NsSmartDeviceLinkRPC::Marshaller::mCreateInteractionChoiceSet_request
CreateInteractionChoiceSet_response,17,&NsSmartDeviceLinkRPC::Marshaller::mCreateInteractionChoiceSet_response
PerformInteraction_request,18,&NsSmartDeviceLinkRPC::Marshaller::mPerformInteraction_request
PerformInteraction_response,19,&NsSmartDeviceLinkRPC::Marshaller::mPerformInteraction_response
DeleteInteractionChoiceSet_request,20,&NsSmartDeviceLinkRPC::Marshaller::mDeleteInteractionChoiceSet_request
DeleteInteractionChoiceSet_response,21,&NsSmartDeviceLinkRPC::Marshaller::mDeleteInteractionChoiceSet_response
Alert_request,22,&NsSmartDeviceLinkRPC::Marshaller::mAlert_request
Alert_response,23,&NsSmartDeviceLinkRPC::Marshaller::mAlert_response
Show_request,24,&NsSmartDeviceLinkRPC::Marshaller::mShow_request
Show_response,25,&NsSmartDeviceLinkRPC::Marshaller::mShow_response
Speak_request,26,&NsSmartDeviceLinkRPC::Marshaller::mSpeak_request
Speak_response,27,&NsSmartDeviceLinkRPC::Marshaller::mSpeak_response
SetMediaClockTimer_request,28,&NsSmartDeviceLinkRPC::Marshaller::mSetMediaClockTimer_request
SetMediaClockTimer_response,29,&NsSmartDeviceLinkRPC::Marshaller::mSetMediaClockTimer_response
EncodedSyncPData_request,30,&NsSmartDeviceLinkRPC::Marshaller::mEncodedSyncPData_request
EncodedSyncPData_response,31,&NsSmartDeviceLinkRPC::Marshaller::mEncodedSyncPData_response
SubscribeButton_request,32,&NsSmartDeviceLinkRPC::Marshaller::mSubscribeButton_request
SubscribeButton_response,33,&NsSmartDeviceLinkRPC::Marshaller::mSubscribeButton_response
UnsubscribeButton_request,34,&NsSmartDeviceLinkRPC::Marshaller::mUnsubscribeButton_request
UnsubscribeButton_response,35,&NsSmartDeviceLinkRPC::Marshaller::mUnsubscribeButton_response
GenericResponse_response,36,&NsSmartDeviceLinkRPC::Marshaller::mGenericResponse_response
OnHMIStatus_notification,37,&NsSmartDeviceLinkRPC::Marshaller::mOnHMIStatus
OnAppInterfaceUnregistered_notification,38,&NsSmartDeviceLinkRPC::Marshaller::mOnAppInterfaceUnregistered
OnButtonEvent_notification,39,&NsSmartDeviceLinkRPC::Marshaller::mOnButtonEvent
OnButtonPress_notification,40,&NsSmartDeviceLinkRPC::Marshaller::mOnButtonPress
OnCommand_notification,41,&NsSmartDeviceLinkRPC::Marshaller::mOnCommand
OnEncodedSyncPData_notification,42,&NsSmartDeviceLinkRPC::Marshaller::mOnEncodedSyncPData
OnTBTClientState_notification,43,&NsSmartDeviceLinkRPC::Marshaller::mOnTBTClientState
OnDriverDistraction_notification,44,&NsSmartDeviceLinkRPC::Marshaller::mOnDriverDistraction
