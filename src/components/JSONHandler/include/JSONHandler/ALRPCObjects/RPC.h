#ifndef APPLINKRPC_RPC_INCLUDE
#define APPLINKRPC_RPC_INCLUDE

#include <string>
#include <json/json.h>

#include "AppInterfaceUnregisteredReason.h"
#include "AudioCaptureQuality.h"
#include "AudioStreamingState.h"
#include "ButtonCapabilities.h"
#include "ButtonEventMode.h"
#include "ButtonName.h"
#include "ButtonPressMode.h"
#include "CharacterSet.h"
#include "Choice.h"
#include "DisplayCapabilities.h"
#include "DisplayType.h"
#include "DriverDistractionState.h"
#include "GlobalProperty.h"
#include "HMILevel.h"
#include "HmiZoneCapabilities.h"
#include "InteractionMode.h"
#include "Language.h"
#include "MediaClockFormat.h"
#include "MenuParams.h"
#include "Result.h"
#include "SpeechCapabilities.h"
#include "StartTime.h"
#include "SyncMsgVersion.h"
#include "SystemContext.h"
#include "TBTState.h"
#include "TTSChunk.h"
#include "TextAlignment.h"
#include "TextField.h"
#include "TextFieldName.h"
#include "TriggerSource.h"
#include "UpdateMode.h"
#include "VrCapabilities.h"
#include "RegisterAppInterface_request.h"
#include "RegisterAppInterface_response.h"
#include "UnregisterAppInterface_request.h"
#include "UnregisterAppInterface_response.h"
#include "SetGlobalProperties_request.h"
#include "SetGlobalProperties_response.h"
#include "ResetGlobalProperties_request.h"
#include "ResetGlobalProperties_response.h"
#include "AddCommand_request.h"
#include "AddCommand_response.h"
#include "DeleteCommand_request.h"
#include "DeleteCommand_response.h"
#include "AddSubMenu_request.h"
#include "AddSubMenu_response.h"
#include "DeleteSubMenu_request.h"
#include "DeleteSubMenu_response.h"
#include "CreateInteractionChoiceSet_request.h"
#include "CreateInteractionChoiceSet_response.h"
#include "PerformInteraction_request.h"
#include "PerformInteraction_response.h"
#include "DeleteInteractionChoiceSet_request.h"
#include "DeleteInteractionChoiceSet_response.h"
#include "Alert_request.h"
#include "Alert_response.h"
#include "Show_request.h"
#include "Show_response.h"
#include "Speak_request.h"
#include "Speak_response.h"
#include "SetMediaClockTimer_request.h"
#include "SetMediaClockTimer_response.h"
#include "EncodedSyncPData_request.h"
#include "EncodedSyncPData_response.h"
#include "SubscribeButton_request.h"
#include "SubscribeButton_response.h"
#include "UnsubscribeButton_request.h"
#include "UnsubscribeButton_response.h"
#include "GenericResponse_response.h"
#include "OnHMIStatus.h"
#include "OnAppInterfaceUnregistered.h"
#include "OnButtonEvent.h"
#include "OnButtonPress.h"
#include "OnCommand.h"
#include "OnEncodedSyncPData.h"
#include "OnTBTClientState.h"
#include "OnDriverDistraction.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/



#endif
