//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef NSSMARTDEVICELINKKRPCV2_RPC_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_RPC_INCLUDE

#include <string>
#include <json/json.h>

#include "AppInterfaceUnregisteredReason.h"
#include "AppType.h"
#include "AudioCaptureQuality.h"
#include "AudioStreamingState.h"
#include "AudioType.h"
#include "ButtonCapabilities.h"
#include "ButtonEventMode.h"
#include "ButtonName.h"
#include "ButtonPressMode.h"
#include "CharacterSet.h"
#include "Choice.h"
#include "CompassDirection.h"
#include "DTC.h"
#include "DeactivateReason.h"
#include "Dimension.h"
#include "DisplayCapabilities.h"
#include "DisplayType.h"
#include "DriverDistractionState.h"
#include "FileType.h"
#include "FunctionID.h"
#include "GPSData.h"
#include "GlobalProperty.h"
#include "HMIApplication.h"
#include "HMILevel.h"
#include "HMIPermissions.h"
#include "HmiZoneCapabilities.h"
#include "Image.h"
#include "ImageType.h"
#include "InteractionMode.h"
#include "Language.h"
#include "MediaClockFormat.h"
#include "MenuParams.h"
#include "PRNDL.h"
#include "ParameterPermissions.h"
#include "PermissionItem.h"
#include "PermissionStatus.h"
#include "PresetBankCapabilities.h"
#include "Result.h"
#include "SamplingRate.h"
#include "SingleTirePressureStatus.h"
#include "SingleTireStatus.h"
#include "SoftButton.h"
#include "SoftButtonCapabilities.h"
#include "SoftButtonType.h"
#include "SpeechCapabilities.h"
#include "StartTime.h"
#include "SyncMsgVersion.h"
#include "SystemAction.h"
#include "SystemContext.h"
#include "TBTState.h"
#include "TTSChunk.h"
#include "TextAlignment.h"
#include "TextField.h"
#include "TextFieldName.h"
#include "TimerMode.h"
#include "TirePressureTellTale.h"
#include "TireStatus.h"
#include "TriggerSource.h"
#include "Turn.h"
#include "UpdateMode.h"
#include "VehicleDataResult.h"
#include "VehicleDataResultCode.h"
#include "VehicleDataType.h"
#include "VehicleType.h"
#include "VrCapabilities.h"
#include "VrHelpItem.h"
#include "messageType.h"
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
#include "DialNumber_request.h"
#include "DialNumber_response.h"
#include "PerformAudioPassThru_request.h"
#include "PerformAudioPassThru_response.h"
#include "EndAudioPassThru_request.h"
#include "EndAudioPassThru_response.h"
#include "SubscribeButton_request.h"
#include "SubscribeButton_response.h"
#include "UnsubscribeButton_request.h"
#include "UnsubscribeButton_response.h"
#include "SubscribeVehicleData_request.h"
#include "SubscribeVehicleData_response.h"
#include "UnsubscribeVehicleData_request.h"
#include "UnsubscribeVehicleData_response.h"
#include "GetVehicleData_request.h"
#include "GetVehicleData_response.h"
#include "ReadDID_request.h"
#include "ReadDID_response.h"
#include "GetDTCs_request.h"
#include "GetDTCs_response.h"
#include "ScrollableMessage_request.h"
#include "ScrollableMessage_response.h"
#include "Slider_request.h"
#include "Slider_response.h"
#include "ShowConstantTBT_request.h"
#include "ShowConstantTBT_response.h"
#include "AlertManeuver_request.h"
#include "AlertManeuver_response.h"
#include "UpdateTurnList_request.h"
#include "UpdateTurnList_response.h"
#include "ChangeRegistration_request.h"
#include "ChangeRegistration_response.h"
#include "GenericResponse_response.h"
#include "PutFile_request.h"
#include "PutFile_response.h"
#include "DeleteFile_request.h"
#include "DeleteFile_response.h"
#include "ListFiles_request.h"
#include "ListFiles_response.h"
#include "SetAppIcon_request.h"
#include "SetAppIcon_response.h"
#include "SetDisplayLayout_request.h"
#include "SetDisplayLayout_response.h"
#include "OnHMIStatus.h"
#include "OnAppInterfaceUnregistered.h"
#include "OnButtonEvent.h"
#include "OnButtonPress.h"
#include "OnVehicleData.h"
#include "OnCommand.h"
#include "OnEncodedSyncPData.h"
#include "OnTBTClientState.h"
#include "OnDriverDistraction.h"
#include "OnPermissionsChange.h"
#include "OnAudioPassThru.h"
#include "OnLanguageChange.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



#endif
