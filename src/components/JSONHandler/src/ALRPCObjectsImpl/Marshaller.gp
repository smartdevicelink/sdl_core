%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name Marshaller_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct Marshaller::localHash
{
  const char *name;
  unsigned int idx;
  void* method;
};

%%
RegisterAppInterface_request,0,&Marshaller::mRegisterAppInterface_request
RegisterAppInterface_response,1,&Marshaller::mRegisterAppInterface_response
UnregisterAppInterface_request,2,&Marshaller::mUnregisterAppInterface_request
UnregisterAppInterface_response,3,&Marshaller::mUnregisterAppInterface_response
SetGlobalProperties_request,4,&Marshaller::mSetGlobalProperties_request
SetGlobalProperties_response,5,&Marshaller::mSetGlobalProperties_response
ResetGlobalProperties_request,6,&Marshaller::mResetGlobalProperties_request
ResetGlobalProperties_response,7,&Marshaller::mResetGlobalProperties_response
AddCommand_request,8,&Marshaller::mAddCommand_request
AddCommand_response,9,&Marshaller::mAddCommand_response
DeleteCommand_request,10,&Marshaller::mDeleteCommand_request
DeleteCommand_response,11,&Marshaller::mDeleteCommand_response
AddSubMenu_request,12,&Marshaller::mAddSubMenu_request
AddSubMenu_response,13,&Marshaller::mAddSubMenu_response
DeleteSubMenu_request,14,&Marshaller::mDeleteSubMenu_request
DeleteSubMenu_response,15,&Marshaller::mDeleteSubMenu_response
CreateInteractionChoiceSet_request,16,&Marshaller::mCreateInteractionChoiceSet_request
CreateInteractionChoiceSet_response,17,&Marshaller::mCreateInteractionChoiceSet_response
PerformInteraction_request,18,&Marshaller::mPerformInteraction_request
PerformInteraction_response,19,&Marshaller::mPerformInteraction_response
DeleteInteractionChoiceSet_request,20,&Marshaller::mDeleteInteractionChoiceSet_request
DeleteInteractionChoiceSet_response,21,&Marshaller::mDeleteInteractionChoiceSet_response
Alert_request,22,&Marshaller::mAlert_request
Alert_response,23,&Marshaller::mAlert_response
Show_request,24,&Marshaller::mShow_request
Show_response,25,&Marshaller::mShow_response
Speak_request,26,&Marshaller::mSpeak_request
Speak_response,27,&Marshaller::mSpeak_response
SetMediaClockTimer_request,28,&Marshaller::mSetMediaClockTimer_request
SetMediaClockTimer_response,29,&Marshaller::mSetMediaClockTimer_response
EncodedSyncPData_request,30,&Marshaller::mEncodedSyncPData_request
EncodedSyncPData_response,31,&Marshaller::mEncodedSyncPData_response
SubscribeButton_request,32,&Marshaller::mSubscribeButton_request
SubscribeButton_response,33,&Marshaller::mSubscribeButton_response
UnsubscribeButton_request,34,&Marshaller::mUnsubscribeButton_request
UnsubscribeButton_response,35,&Marshaller::mUnsubscribeButton_response
GenericResponse_response,36,&Marshaller::mGenericResponse_response
OnHMIStatus_notification,37,&Marshaller::mOnHMIStatus
OnAppInterfaceUnregistered_notification,38,&Marshaller::mOnAppInterfaceUnregistered
OnButtonEvent_notification,39,&Marshaller::mOnButtonEvent
OnButtonPress_notification,40,&Marshaller::mOnButtonPress
OnCommand_notification,41,&Marshaller::mOnCommand
OnEncodedSyncPData_notification,42,&Marshaller::mOnEncodedSyncPData
OnTBTClientState_notification,43,&Marshaller::mOnTBTClientState
OnDriverDistraction_notification,44,&Marshaller::mOnDriverDistraction
