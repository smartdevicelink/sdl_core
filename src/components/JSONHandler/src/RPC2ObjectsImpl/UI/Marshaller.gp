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
UI.AddCommand,0,&Marshaller::mAddCommand
UI.AddCommandResponse,1,&Marshaller::mAddCommandResponse
UI.AddSubMenu,2,&Marshaller::mAddSubMenu
UI.AddSubMenuResponse,3,&Marshaller::mAddSubMenuResponse
UI.Alert,4,&Marshaller::mAlert
UI.AlertResponse,5,&Marshaller::mAlertResponse
UI.CreateInteractionChoiceSet,6,&Marshaller::mCreateInteractionChoiceSet
UI.CreateInteractionChoiceSetResponse,7,&Marshaller::mCreateInteractionChoiceSetResponse
UI.DeleteCommand,8,&Marshaller::mDeleteCommand
UI.DeleteCommandResponse,9,&Marshaller::mDeleteCommandResponse
UI.DeleteInteractionChoiceSet,10,&Marshaller::mDeleteInteractionChoiceSet
UI.DeleteInteractionChoiceSetResponse,11,&Marshaller::mDeleteInteractionChoiceSetResponse
UI.DeleteSubMenu,12,&Marshaller::mDeleteSubMenu
UI.DeleteSubMenuResponse,13,&Marshaller::mDeleteSubMenuResponse
UI.GetCapabilities,14,&Marshaller::mGetCapabilities
UI.GetCapabilitiesResponse,15,&Marshaller::mGetCapabilitiesResponse
UI.OnAppActivated,16,&Marshaller::mOnAppActivated
UI.OnAppDeactivated,17,&Marshaller::mOnAppDeactivated
UI.OnCommand,18,&Marshaller::mOnCommand
UI.OnSystemContext,19,&Marshaller::mOnSystemContext
UI.PerformInteraction,20,&Marshaller::mPerformInteraction
UI.PerformInteractionResponse,21,&Marshaller::mPerformInteractionResponse
UI.ResetGlobalProperties,22,&Marshaller::mResetGlobalProperties
UI.ResetGlobalPropertiesResponse,23,&Marshaller::mResetGlobalPropertiesResponse
UI.SetGlobalProperties,24,&Marshaller::mSetGlobalProperties
UI.SetGlobalPropertiesResponse,25,&Marshaller::mSetGlobalPropertiesResponse
UI.SetMediaClockTimer,26,&Marshaller::mSetMediaClockTimer
UI.SetMediaClockTimerResponse,27,&Marshaller::mSetMediaClockTimerResponse
UI.Show,28,&Marshaller::mShow
UI.ShowResponse,29,&Marshaller::mShowResponse
