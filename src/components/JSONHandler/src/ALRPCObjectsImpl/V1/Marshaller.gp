%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name Marshaller_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsAppLinkRPCV1::Marshaller::localHash
{
  const char *name;
  unsigned int idx;
  void* method;
};

%%
RegisterAppInterface_request,0,&NsAppLinkRPCV1::Marshaller::mRegisterAppInterface_request
RegisterAppInterface_response,1,&NsAppLinkRPCV1::Marshaller::mRegisterAppInterface_response
UnregisterAppInterface_request,2,&NsAppLinkRPCV1::Marshaller::mUnregisterAppInterface_request
UnregisterAppInterface_response,3,&NsAppLinkRPCV1::Marshaller::mUnregisterAppInterface_response
SetGlobalProperties_request,4,&NsAppLinkRPCV1::Marshaller::mSetGlobalProperties_request
SetGlobalProperties_response,5,&NsAppLinkRPCV1::Marshaller::mSetGlobalProperties_response
ResetGlobalProperties_request,6,&NsAppLinkRPCV1::Marshaller::mResetGlobalProperties_request
ResetGlobalProperties_response,7,&NsAppLinkRPCV1::Marshaller::mResetGlobalProperties_response
AddCommand_request,8,&NsAppLinkRPCV1::Marshaller::mAddCommand_request
AddCommand_response,9,&NsAppLinkRPCV1::Marshaller::mAddCommand_response
DeleteCommand_request,10,&NsAppLinkRPCV1::Marshaller::mDeleteCommand_request
DeleteCommand_response,11,&NsAppLinkRPCV1::Marshaller::mDeleteCommand_response
AddSubMenu_request,12,&NsAppLinkRPCV1::Marshaller::mAddSubMenu_request
AddSubMenu_response,13,&NsAppLinkRPCV1::Marshaller::mAddSubMenu_response
DeleteSubMenu_request,14,&NsAppLinkRPCV1::Marshaller::mDeleteSubMenu_request
DeleteSubMenu_response,15,&NsAppLinkRPCV1::Marshaller::mDeleteSubMenu_response
CreateInteractionChoiceSet_request,16,&NsAppLinkRPCV1::Marshaller::mCreateInteractionChoiceSet_request
CreateInteractionChoiceSet_response,17,&NsAppLinkRPCV1::Marshaller::mCreateInteractionChoiceSet_response
PerformInteraction_request,18,&NsAppLinkRPCV1::Marshaller::mPerformInteraction_request
PerformInteraction_response,19,&NsAppLinkRPCV1::Marshaller::mPerformInteraction_response
DeleteInteractionChoiceSet_request,20,&NsAppLinkRPCV1::Marshaller::mDeleteInteractionChoiceSet_request
DeleteInteractionChoiceSet_response,21,&NsAppLinkRPCV1::Marshaller::mDeleteInteractionChoiceSet_response
Alert_request,22,&NsAppLinkRPCV1::Marshaller::mAlert_request
Alert_response,23,&NsAppLinkRPCV1::Marshaller::mAlert_response
Show_request,24,&NsAppLinkRPCV1::Marshaller::mShow_request
Show_response,25,&NsAppLinkRPCV1::Marshaller::mShow_response
Speak_request,26,&NsAppLinkRPCV1::Marshaller::mSpeak_request
Speak_response,27,&NsAppLinkRPCV1::Marshaller::mSpeak_response
SetMediaClockTimer_request,28,&NsAppLinkRPCV1::Marshaller::mSetMediaClockTimer_request
SetMediaClockTimer_response,29,&NsAppLinkRPCV1::Marshaller::mSetMediaClockTimer_response
EncodedSyncPData_request,30,&NsAppLinkRPCV1::Marshaller::mEncodedSyncPData_request
EncodedSyncPData_response,31,&NsAppLinkRPCV1::Marshaller::mEncodedSyncPData_response
SubscribeButton_request,32,&NsAppLinkRPCV1::Marshaller::mSubscribeButton_request
SubscribeButton_response,33,&NsAppLinkRPCV1::Marshaller::mSubscribeButton_response
UnsubscribeButton_request,34,&NsAppLinkRPCV1::Marshaller::mUnsubscribeButton_request
UnsubscribeButton_response,35,&NsAppLinkRPCV1::Marshaller::mUnsubscribeButton_response
GenericResponse_response,36,&NsAppLinkRPCV1::Marshaller::mGenericResponse_response
OnHMIStatus_notification,37,&NsAppLinkRPCV1::Marshaller::mOnHMIStatus
OnAppInterfaceUnregistered_notification,38,&NsAppLinkRPCV1::Marshaller::mOnAppInterfaceUnregistered
OnButtonEvent_notification,39,&NsAppLinkRPCV1::Marshaller::mOnButtonEvent
OnButtonPress_notification,40,&NsAppLinkRPCV1::Marshaller::mOnButtonPress
OnCommand_notification,41,&NsAppLinkRPCV1::Marshaller::mOnCommand
OnEncodedSyncPData_notification,42,&NsAppLinkRPCV1::Marshaller::mOnEncodedSyncPData
OnTBTClientState_notification,43,&NsAppLinkRPCV1::Marshaller::mOnTBTClientState
OnDriverDistraction_notification,44,&NsAppLinkRPCV1::Marshaller::mOnDriverDistraction
