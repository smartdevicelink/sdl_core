%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name Marshaller_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsAppLinkRPC::Marshaller::localHash
{
  const char *name;
  unsigned int idx;
  void* method;
};

%%
RegisterAppInterface_request,0,&NsAppLinkRPC::Marshaller::mRegisterAppInterface_request
RegisterAppInterface_response,1,&NsAppLinkRPC::Marshaller::mRegisterAppInterface_response
UnregisterAppInterface_request,2,&NsAppLinkRPC::Marshaller::mUnregisterAppInterface_request
UnregisterAppInterface_response,3,&NsAppLinkRPC::Marshaller::mUnregisterAppInterface_response
SetGlobalProperties_request,4,&NsAppLinkRPC::Marshaller::mSetGlobalProperties_request
SetGlobalProperties_response,5,&NsAppLinkRPC::Marshaller::mSetGlobalProperties_response
ResetGlobalProperties_request,6,&NsAppLinkRPC::Marshaller::mResetGlobalProperties_request
ResetGlobalProperties_response,7,&NsAppLinkRPC::Marshaller::mResetGlobalProperties_response
AddCommand_request,8,&NsAppLinkRPC::Marshaller::mAddCommand_request
AddCommand_response,9,&NsAppLinkRPC::Marshaller::mAddCommand_response
DeleteCommand_request,10,&NsAppLinkRPC::Marshaller::mDeleteCommand_request
DeleteCommand_response,11,&NsAppLinkRPC::Marshaller::mDeleteCommand_response
AddSubMenu_request,12,&NsAppLinkRPC::Marshaller::mAddSubMenu_request
AddSubMenu_response,13,&NsAppLinkRPC::Marshaller::mAddSubMenu_response
DeleteSubMenu_request,14,&NsAppLinkRPC::Marshaller::mDeleteSubMenu_request
DeleteSubMenu_response,15,&NsAppLinkRPC::Marshaller::mDeleteSubMenu_response
CreateInteractionChoiceSet_request,16,&NsAppLinkRPC::Marshaller::mCreateInteractionChoiceSet_request
CreateInteractionChoiceSet_response,17,&NsAppLinkRPC::Marshaller::mCreateInteractionChoiceSet_response
PerformInteraction_request,18,&NsAppLinkRPC::Marshaller::mPerformInteraction_request
PerformInteraction_response,19,&NsAppLinkRPC::Marshaller::mPerformInteraction_response
DeleteInteractionChoiceSet_request,20,&NsAppLinkRPC::Marshaller::mDeleteInteractionChoiceSet_request
DeleteInteractionChoiceSet_response,21,&NsAppLinkRPC::Marshaller::mDeleteInteractionChoiceSet_response
Alert_request,22,&NsAppLinkRPC::Marshaller::mAlert_request
Alert_response,23,&NsAppLinkRPC::Marshaller::mAlert_response
Show_request,24,&NsAppLinkRPC::Marshaller::mShow_request
Show_response,25,&NsAppLinkRPC::Marshaller::mShow_response
Speak_request,26,&NsAppLinkRPC::Marshaller::mSpeak_request
Speak_response,27,&NsAppLinkRPC::Marshaller::mSpeak_response
SetMediaClockTimer_request,28,&NsAppLinkRPC::Marshaller::mSetMediaClockTimer_request
SetMediaClockTimer_response,29,&NsAppLinkRPC::Marshaller::mSetMediaClockTimer_response
EncodedSyncPData_request,30,&NsAppLinkRPC::Marshaller::mEncodedSyncPData_request
EncodedSyncPData_response,31,&NsAppLinkRPC::Marshaller::mEncodedSyncPData_response
SubscribeButton_request,32,&NsAppLinkRPC::Marshaller::mSubscribeButton_request
SubscribeButton_response,33,&NsAppLinkRPC::Marshaller::mSubscribeButton_response
UnsubscribeButton_request,34,&NsAppLinkRPC::Marshaller::mUnsubscribeButton_request
UnsubscribeButton_response,35,&NsAppLinkRPC::Marshaller::mUnsubscribeButton_response
GenericResponse_response,36,&NsAppLinkRPC::Marshaller::mGenericResponse_response
OnHMIStatus_notification,37,&NsAppLinkRPC::Marshaller::mOnHMIStatus
OnAppInterfaceUnregistered_notification,38,&NsAppLinkRPC::Marshaller::mOnAppInterfaceUnregistered
OnButtonEvent_notification,39,&NsAppLinkRPC::Marshaller::mOnButtonEvent
OnButtonPress_notification,40,&NsAppLinkRPC::Marshaller::mOnButtonPress
OnCommand_notification,41,&NsAppLinkRPC::Marshaller::mOnCommand
OnEncodedSyncPData_notification,42,&NsAppLinkRPC::Marshaller::mOnEncodedSyncPData
OnTBTClientState_notification,43,&NsAppLinkRPC::Marshaller::mOnTBTClientState
OnDriverDistraction_notification,44,&NsAppLinkRPC::Marshaller::mOnDriverDistraction
