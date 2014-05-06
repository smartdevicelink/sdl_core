/**
 * @file HMI_API.cc
 * @brief Generated class HMI_API source file.
 *
 * This class is a part of SmartObjects solution. It provides
 * factory functionallity which allows client to use SmartSchemas
 * in accordance with definitions from HMI_API.xml file
 */
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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <map>
#include <set>

#include "HMI_API_schema.h"
#include "smart_objects/always_true_schema_item.h"
#include "smart_objects/always_false_schema_item.h"
#include "smart_objects/array_schema_item.h"
#include "smart_objects/bool_schema_item.h"
#include "smart_objects/object_schema_item.h"
#include "smart_objects/string_schema_item.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/number_schema_item.h"
#include "smart_objects/schema_item_parameter.h"

using namespace NsSmartDeviceLink::NsSmartObjects;

hmi_apis::HMI_API::HMI_API()
 : NsSmartDeviceLink::NsJSONHandler::CSmartFactory<FunctionID::eType, messageType::eType, StructIdentifiers::eType>() {
  TStructsSchemaItems struct_schema_items;
  InitStructSchemes(struct_schema_items);

  std::set<FunctionID::eType> function_id_items;
  function_id_items.insert(FunctionID::Buttons_GetCapabilities);
  function_id_items.insert(FunctionID::Buttons_OnButtonEvent);
  function_id_items.insert(FunctionID::Buttons_OnButtonPress);
  function_id_items.insert(FunctionID::BasicCommunication_OnReady);
  function_id_items.insert(FunctionID::BasicCommunication_OnStartDeviceDiscovery);
  function_id_items.insert(FunctionID::BasicCommunication_OnResumeAudioSource);
  function_id_items.insert(FunctionID::BasicCommunication_UpdateDeviceList);
  function_id_items.insert(FunctionID::BasicCommunication_OnFileRemoved);
  function_id_items.insert(FunctionID::BasicCommunication_AllowDeviceToConnect);
  function_id_items.insert(FunctionID::BasicCommunication_OnDeviceChosen);
  function_id_items.insert(FunctionID::BasicCommunication_OnFindApplications);
  function_id_items.insert(FunctionID::BasicCommunication_ActivateApp);
  function_id_items.insert(FunctionID::BasicCommunication_OnAppActivated);
  function_id_items.insert(FunctionID::BasicCommunication_OnAppDeactivated);
  function_id_items.insert(FunctionID::BasicCommunication_OnAppRegistered);
  function_id_items.insert(FunctionID::BasicCommunication_OnAppUnregistered);
  function_id_items.insert(FunctionID::BasicCommunication_OnExitApplication);
  function_id_items.insert(FunctionID::BasicCommunication_OnExitAllApplications);
  function_id_items.insert(FunctionID::BasicCommunication_MixingAudioSupported);
  function_id_items.insert(FunctionID::BasicCommunication_PlayTone);
  function_id_items.insert(FunctionID::BasicCommunication_OnSystemRequest);
  function_id_items.insert(FunctionID::BasicCommunication_SystemRequest);
  function_id_items.insert(FunctionID::BasicCommunication_PolicyUpdate);
  function_id_items.insert(FunctionID::BasicCommunication_OnSDLClose);
  function_id_items.insert(FunctionID::BasicCommunication_OnPutFile);
  function_id_items.insert(FunctionID::BasicCommunication_GetSystemInfo);
  function_id_items.insert(FunctionID::BasicCommunication_OnSystemInfoChanged);
  function_id_items.insert(FunctionID::BasicCommunication_OnIgnitionCycleOver);
  function_id_items.insert(FunctionID::VR_IsReady);
  function_id_items.insert(FunctionID::VR_Started);
  function_id_items.insert(FunctionID::VR_Stopped);
  function_id_items.insert(FunctionID::VR_AddCommand);
  function_id_items.insert(FunctionID::VR_DeleteCommand);
  function_id_items.insert(FunctionID::VR_PerformInteraction);
  function_id_items.insert(FunctionID::VR_OnCommand);
  function_id_items.insert(FunctionID::VR_ChangeRegistration);
  function_id_items.insert(FunctionID::VR_OnLanguageChange);
  function_id_items.insert(FunctionID::VR_GetSupportedLanguages);
  function_id_items.insert(FunctionID::VR_GetLanguage);
  function_id_items.insert(FunctionID::VR_GetCapabilities);
  function_id_items.insert(FunctionID::TTS_GetCapabilities);
  function_id_items.insert(FunctionID::TTS_Started);
  function_id_items.insert(FunctionID::TTS_Stopped);
  function_id_items.insert(FunctionID::TTS_IsReady);
  function_id_items.insert(FunctionID::TTS_Speak);
  function_id_items.insert(FunctionID::TTS_StopSpeaking);
  function_id_items.insert(FunctionID::TTS_ChangeRegistration);
  function_id_items.insert(FunctionID::TTS_OnLanguageChange);
  function_id_items.insert(FunctionID::TTS_GetSupportedLanguages);
  function_id_items.insert(FunctionID::TTS_GetLanguage);
  function_id_items.insert(FunctionID::TTS_SetGlobalProperties);
  function_id_items.insert(FunctionID::UI_Alert);
  function_id_items.insert(FunctionID::UI_Show);
  function_id_items.insert(FunctionID::UI_AddCommand);
  function_id_items.insert(FunctionID::UI_DeleteCommand);
  function_id_items.insert(FunctionID::UI_AddSubMenu);
  function_id_items.insert(FunctionID::UI_DeleteSubMenu);
  function_id_items.insert(FunctionID::UI_PerformInteraction);
  function_id_items.insert(FunctionID::UI_SetMediaClockTimer);
  function_id_items.insert(FunctionID::UI_SetGlobalProperties);
  function_id_items.insert(FunctionID::UI_OnCommand);
  function_id_items.insert(FunctionID::UI_OnSystemContext);
  function_id_items.insert(FunctionID::UI_GetCapabilities);
  function_id_items.insert(FunctionID::UI_ChangeRegistration);
  function_id_items.insert(FunctionID::UI_OnLanguageChange);
  function_id_items.insert(FunctionID::UI_GetSupportedLanguages);
  function_id_items.insert(FunctionID::UI_GetLanguage);
  function_id_items.insert(FunctionID::UI_OnDriverDistraction);
  function_id_items.insert(FunctionID::UI_SetAppIcon);
  function_id_items.insert(FunctionID::UI_SetDisplayLayout);
  function_id_items.insert(FunctionID::UI_ShowCustomForm);
  function_id_items.insert(FunctionID::UI_OnKeyboardInput);
  function_id_items.insert(FunctionID::UI_OnTouchEvent);
  function_id_items.insert(FunctionID::UI_Slider);
  function_id_items.insert(FunctionID::UI_ScrollableMessage);
  function_id_items.insert(FunctionID::UI_PerformAudioPassThru);
  function_id_items.insert(FunctionID::UI_EndAudioPassThru);
  function_id_items.insert(FunctionID::UI_IsReady);
  function_id_items.insert(FunctionID::UI_ClosePopUp);
  function_id_items.insert(FunctionID::UI_OnResetTimeout);
  function_id_items.insert(FunctionID::UI_OnRecordStart);
  function_id_items.insert(FunctionID::Navigation_IsReady);
  function_id_items.insert(FunctionID::Navigation_ShowConstantTBT);
  function_id_items.insert(FunctionID::Navigation_AlertManeuver);
  function_id_items.insert(FunctionID::Navigation_UpdateTurnList);
  function_id_items.insert(FunctionID::Navigation_OnTBTClientState);
  function_id_items.insert(FunctionID::Navigation_StartStream);
  function_id_items.insert(FunctionID::Navigation_StopStream);
  function_id_items.insert(FunctionID::Navigation_StartAudioStream);
  function_id_items.insert(FunctionID::Navigation_StopAudioStream);
  function_id_items.insert(FunctionID::VehicleInfo_IsReady);
  function_id_items.insert(FunctionID::VehicleInfo_GetVehicleType);
  function_id_items.insert(FunctionID::VehicleInfo_ReadDID);
  function_id_items.insert(FunctionID::VehicleInfo_GetDTCs);
  function_id_items.insert(FunctionID::VehicleInfo_DiagnosticMessage);
  function_id_items.insert(FunctionID::VehicleInfo_SubscribeVehicleData);
  function_id_items.insert(FunctionID::VehicleInfo_UnsubscribeVehicleData);
  function_id_items.insert(FunctionID::VehicleInfo_GetVehicleData);
  function_id_items.insert(FunctionID::VehicleInfo_OnVehicleData);
  function_id_items.insert(FunctionID::SDL_ActivateApp);
  function_id_items.insert(FunctionID::SDL_GetUserFriendlyMessage);
  function_id_items.insert(FunctionID::SDL_OnAllowSDLFunctionality);
  function_id_items.insert(FunctionID::SDL_OnReceivedPolicyUpdate);
  function_id_items.insert(FunctionID::SDL_OnPolicyUpdate);
  function_id_items.insert(FunctionID::SDL_GetListOfPermissions);
  function_id_items.insert(FunctionID::SDL_OnAppPermissionConsent);
  function_id_items.insert(FunctionID::SDL_OnAppPermissionChanged);
  function_id_items.insert(FunctionID::SDL_OnSDLConsentNeeded);
  function_id_items.insert(FunctionID::SDL_UpdateSDL);
  function_id_items.insert(FunctionID::SDL_GetStatusUpdate);
  function_id_items.insert(FunctionID::SDL_OnStatusUpdate);
  function_id_items.insert(FunctionID::SDL_OnSystemError);
  function_id_items.insert(FunctionID::SDL_AddStatisticsInfo);
  function_id_items.insert(FunctionID::SDL_GetURLS);

  std::set<messageType::eType> message_type_items;
  message_type_items.insert(messageType::request);
  message_type_items.insert(messageType::response);
  message_type_items.insert(messageType::notification);
  message_type_items.insert(messageType::error_response);

  InitFunctionSchemes(struct_schema_items, function_id_items, message_type_items);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::ProvideObjectSchemaItemForStruct(
    const TStructsSchemaItems &struct_schema_items,
    const StructIdentifiers::eType struct_id) {
  const TStructsSchemaItems::const_iterator it = struct_schema_items.find(struct_id);
  if (it != struct_schema_items.end()) {
    return it->second;
  }

  return NsSmartDeviceLink::NsSmartObjects::CAlwaysFalseSchemaItem::create();
}

void hmi_apis::HMI_API::InitStructSchemes(
    TStructsSchemaItems &struct_schema_items) {
  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_UserFriendlyMessage = InitStructSchemaItem_Common_UserFriendlyMessage(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_UserFriendlyMessage, struct_schema_item_Common_UserFriendlyMessage));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_UserFriendlyMessage, CSmartSchema(struct_schema_item_Common_UserFriendlyMessage)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_PermissionItem = InitStructSchemaItem_Common_PermissionItem(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_PermissionItem, struct_schema_item_Common_PermissionItem));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_PermissionItem, CSmartSchema(struct_schema_item_Common_PermissionItem)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_ServiceInfo = InitStructSchemaItem_Common_ServiceInfo(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_ServiceInfo, struct_schema_item_Common_ServiceInfo));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_ServiceInfo, CSmartSchema(struct_schema_item_Common_ServiceInfo)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_TextField = InitStructSchemaItem_Common_TextField(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_TextField, struct_schema_item_Common_TextField));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_TextField, CSmartSchema(struct_schema_item_Common_TextField)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_ButtonCapabilities = InitStructSchemaItem_Common_ButtonCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_ButtonCapabilities, struct_schema_item_Common_ButtonCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_ButtonCapabilities, CSmartSchema(struct_schema_item_Common_ButtonCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_VehicleDataResult = InitStructSchemaItem_Common_VehicleDataResult(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_VehicleDataResult, struct_schema_item_Common_VehicleDataResult));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_VehicleDataResult, CSmartSchema(struct_schema_item_Common_VehicleDataResult)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_TouchCoord = InitStructSchemaItem_Common_TouchCoord(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_TouchCoord, struct_schema_item_Common_TouchCoord));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_TouchCoord, CSmartSchema(struct_schema_item_Common_TouchCoord)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_TouchEvent = InitStructSchemaItem_Common_TouchEvent(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_TouchEvent, struct_schema_item_Common_TouchEvent));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_TouchEvent, CSmartSchema(struct_schema_item_Common_TouchEvent)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_PresetBankCapabilities = InitStructSchemaItem_Common_PresetBankCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_PresetBankCapabilities, struct_schema_item_Common_PresetBankCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_PresetBankCapabilities, CSmartSchema(struct_schema_item_Common_PresetBankCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_Image = InitStructSchemaItem_Common_Image(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_Image, struct_schema_item_Common_Image));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_Image, CSmartSchema(struct_schema_item_Common_Image)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_SoftButton = InitStructSchemaItem_Common_SoftButton(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_SoftButton, struct_schema_item_Common_SoftButton));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_SoftButton, CSmartSchema(struct_schema_item_Common_SoftButton)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_TTSChunk = InitStructSchemaItem_Common_TTSChunk(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_TTSChunk, struct_schema_item_Common_TTSChunk));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_TTSChunk, CSmartSchema(struct_schema_item_Common_TTSChunk)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_HMIApplication = InitStructSchemaItem_Common_HMIApplication(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_HMIApplication, struct_schema_item_Common_HMIApplication));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_HMIApplication, CSmartSchema(struct_schema_item_Common_HMIApplication)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_MenuParams = InitStructSchemaItem_Common_MenuParams(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_MenuParams, struct_schema_item_Common_MenuParams));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_MenuParams, CSmartSchema(struct_schema_item_Common_MenuParams)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_Choice = InitStructSchemaItem_Common_Choice(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_Choice, struct_schema_item_Common_Choice));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_Choice, CSmartSchema(struct_schema_item_Common_Choice)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_VrHelpItem = InitStructSchemaItem_Common_VrHelpItem(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_VrHelpItem, struct_schema_item_Common_VrHelpItem));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_VrHelpItem, CSmartSchema(struct_schema_item_Common_VrHelpItem)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_TimeFormat = InitStructSchemaItem_Common_TimeFormat(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_TimeFormat, struct_schema_item_Common_TimeFormat));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_TimeFormat, CSmartSchema(struct_schema_item_Common_TimeFormat)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_TouchEventCapabilities = InitStructSchemaItem_Common_TouchEventCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_TouchEventCapabilities, struct_schema_item_Common_TouchEventCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_TouchEventCapabilities, CSmartSchema(struct_schema_item_Common_TouchEventCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_ImageResolution = InitStructSchemaItem_Common_ImageResolution(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_ImageResolution, struct_schema_item_Common_ImageResolution));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_ImageResolution, CSmartSchema(struct_schema_item_Common_ImageResolution)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_ScreenParams = InitStructSchemaItem_Common_ScreenParams(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_ScreenParams, struct_schema_item_Common_ScreenParams));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_ScreenParams, CSmartSchema(struct_schema_item_Common_ScreenParams)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_ImageField = InitStructSchemaItem_Common_ImageField(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_ImageField, struct_schema_item_Common_ImageField));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_ImageField, CSmartSchema(struct_schema_item_Common_ImageField)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_DisplayCapabilities = InitStructSchemaItem_Common_DisplayCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_DisplayCapabilities, struct_schema_item_Common_DisplayCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_DisplayCapabilities, CSmartSchema(struct_schema_item_Common_DisplayCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_SoftButtonCapabilities = InitStructSchemaItem_Common_SoftButtonCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_SoftButtonCapabilities, struct_schema_item_Common_SoftButtonCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_SoftButtonCapabilities, CSmartSchema(struct_schema_item_Common_SoftButtonCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_AudioPassThruCapabilities = InitStructSchemaItem_Common_AudioPassThruCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_AudioPassThruCapabilities, struct_schema_item_Common_AudioPassThruCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_AudioPassThruCapabilities, CSmartSchema(struct_schema_item_Common_AudioPassThruCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_Coordinate = InitStructSchemaItem_Common_Coordinate(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_Coordinate, struct_schema_item_Common_Coordinate));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_Coordinate, CSmartSchema(struct_schema_item_Common_Coordinate)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_TextFieldStruct = InitStructSchemaItem_Common_TextFieldStruct(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_TextFieldStruct, struct_schema_item_Common_TextFieldStruct));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_TextFieldStruct, CSmartSchema(struct_schema_item_Common_TextFieldStruct)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_KeyboardProperties = InitStructSchemaItem_Common_KeyboardProperties(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_KeyboardProperties, struct_schema_item_Common_KeyboardProperties));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_KeyboardProperties, CSmartSchema(struct_schema_item_Common_KeyboardProperties)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_Turn = InitStructSchemaItem_Common_Turn(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_Turn, struct_schema_item_Common_Turn));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_Turn, CSmartSchema(struct_schema_item_Common_Turn)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_VehicleType = InitStructSchemaItem_Common_VehicleType(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_VehicleType, struct_schema_item_Common_VehicleType));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_VehicleType, CSmartSchema(struct_schema_item_Common_VehicleType)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_DeviceInfo = InitStructSchemaItem_Common_DeviceInfo(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_DeviceInfo, struct_schema_item_Common_DeviceInfo));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_DeviceInfo, CSmartSchema(struct_schema_item_Common_DeviceInfo)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_GPSData = InitStructSchemaItem_Common_GPSData(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_GPSData, struct_schema_item_Common_GPSData));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_GPSData, CSmartSchema(struct_schema_item_Common_GPSData)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_SingleTireStatus = InitStructSchemaItem_Common_SingleTireStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_SingleTireStatus, struct_schema_item_Common_SingleTireStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_SingleTireStatus, CSmartSchema(struct_schema_item_Common_SingleTireStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_DIDResult = InitStructSchemaItem_Common_DIDResult(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_DIDResult, struct_schema_item_Common_DIDResult));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_DIDResult, CSmartSchema(struct_schema_item_Common_DIDResult)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_HeadLampStatus = InitStructSchemaItem_Common_HeadLampStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_HeadLampStatus, struct_schema_item_Common_HeadLampStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_HeadLampStatus, CSmartSchema(struct_schema_item_Common_HeadLampStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_TireStatus = InitStructSchemaItem_Common_TireStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_TireStatus, struct_schema_item_Common_TireStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_TireStatus, CSmartSchema(struct_schema_item_Common_TireStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_BeltStatus = InitStructSchemaItem_Common_BeltStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_BeltStatus, struct_schema_item_Common_BeltStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_BeltStatus, CSmartSchema(struct_schema_item_Common_BeltStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_BodyInformation = InitStructSchemaItem_Common_BodyInformation(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_BodyInformation, struct_schema_item_Common_BodyInformation));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_BodyInformation, CSmartSchema(struct_schema_item_Common_BodyInformation)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_DeviceStatus = InitStructSchemaItem_Common_DeviceStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_DeviceStatus, struct_schema_item_Common_DeviceStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_DeviceStatus, CSmartSchema(struct_schema_item_Common_DeviceStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_ECallInfo = InitStructSchemaItem_Common_ECallInfo(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_ECallInfo, struct_schema_item_Common_ECallInfo));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_ECallInfo, CSmartSchema(struct_schema_item_Common_ECallInfo)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_AirbagStatus = InitStructSchemaItem_Common_AirbagStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_AirbagStatus, struct_schema_item_Common_AirbagStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_AirbagStatus, CSmartSchema(struct_schema_item_Common_AirbagStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_EmergencyEvent = InitStructSchemaItem_Common_EmergencyEvent(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_EmergencyEvent, struct_schema_item_Common_EmergencyEvent));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_EmergencyEvent, CSmartSchema(struct_schema_item_Common_EmergencyEvent)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_ClusterModeStatus = InitStructSchemaItem_Common_ClusterModeStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_ClusterModeStatus, struct_schema_item_Common_ClusterModeStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_ClusterModeStatus, CSmartSchema(struct_schema_item_Common_ClusterModeStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Common_MyKey = InitStructSchemaItem_Common_MyKey(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Common_MyKey, struct_schema_item_Common_MyKey));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Common_MyKey, CSmartSchema(struct_schema_item_Common_MyKey)));
}

void hmi_apis::HMI_API::InitFunctionSchemes(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kMessage] = CObjectSchemaItem::SMember(CStringSchemaItem::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  CSmartSchema error_response_schema(CObjectSchemaItem::create(root_members_map));

  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Buttons_GetCapabilities, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_UpdateDeviceList, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_AllowDeviceToConnect, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_ActivateApp, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_MixingAudioSupported, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_SystemRequest, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_PolicyUpdate, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_GetSystemInfo, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_IsReady, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_AddCommand, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_DeleteCommand, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_PerformInteraction, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_ChangeRegistration, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_GetSupportedLanguages, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_GetLanguage, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_GetCapabilities, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_GetCapabilities, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_IsReady, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_Speak, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_StopSpeaking, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_ChangeRegistration, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_GetSupportedLanguages, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_GetLanguage, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_SetGlobalProperties, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_Alert, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_Show, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_AddCommand, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_DeleteCommand, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_AddSubMenu, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_DeleteSubMenu, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_PerformInteraction, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetMediaClockTimer, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetGlobalProperties, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_GetCapabilities, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ChangeRegistration, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_GetSupportedLanguages, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_GetLanguage, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetAppIcon, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetDisplayLayout, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ShowCustomForm, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_Slider, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ScrollableMessage, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_PerformAudioPassThru, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_EndAudioPassThru, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_IsReady, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ClosePopUp, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_IsReady, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_ShowConstantTBT, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_AlertManeuver, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_UpdateTurnList, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StartStream, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StopStream, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StartAudioStream, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StopAudioStream, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_IsReady, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_GetVehicleType, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_ReadDID, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_GetDTCs, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_DiagnosticMessage, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_SubscribeVehicleData, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_UnsubscribeVehicleData, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_GetVehicleData, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_ActivateApp, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetUserFriendlyMessage, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetListOfPermissions, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_UpdateSDL, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetStatusUpdate, messageType::error_response), error_response_schema));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetURLS, messageType::error_response), error_response_schema));

  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Buttons_GetCapabilities, messageType::request), InitFunction_Buttons_GetCapabilities_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Buttons_GetCapabilities, messageType::response), InitFunction_Buttons_GetCapabilities_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Buttons_OnButtonEvent, messageType::notification), InitFunction_Buttons_OnButtonEvent_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Buttons_OnButtonPress, messageType::notification), InitFunction_Buttons_OnButtonPress_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnReady, messageType::notification), InitFunction_BasicCommunication_OnReady_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnStartDeviceDiscovery, messageType::notification), InitFunction_BasicCommunication_OnStartDeviceDiscovery_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnResumeAudioSource, messageType::notification), InitFunction_BasicCommunication_OnResumeAudioSource_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_UpdateDeviceList, messageType::request), InitFunction_BasicCommunication_UpdateDeviceList_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_UpdateDeviceList, messageType::response), InitFunction_BasicCommunication_UpdateDeviceList_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnFileRemoved, messageType::notification), InitFunction_BasicCommunication_OnFileRemoved_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_AllowDeviceToConnect, messageType::request), InitFunction_BasicCommunication_AllowDeviceToConnect_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_AllowDeviceToConnect, messageType::response), InitFunction_BasicCommunication_AllowDeviceToConnect_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnDeviceChosen, messageType::notification), InitFunction_BasicCommunication_OnDeviceChosen_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnFindApplications, messageType::notification), InitFunction_BasicCommunication_OnFindApplications_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_ActivateApp, messageType::request), InitFunction_BasicCommunication_ActivateApp_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_ActivateApp, messageType::response), InitFunction_BasicCommunication_ActivateApp_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnAppActivated, messageType::notification), InitFunction_BasicCommunication_OnAppActivated_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnAppDeactivated, messageType::notification), InitFunction_BasicCommunication_OnAppDeactivated_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnAppRegistered, messageType::notification), InitFunction_BasicCommunication_OnAppRegistered_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnAppUnregistered, messageType::notification), InitFunction_BasicCommunication_OnAppUnregistered_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnExitApplication, messageType::notification), InitFunction_BasicCommunication_OnExitApplication_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnExitAllApplications, messageType::notification), InitFunction_BasicCommunication_OnExitAllApplications_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_MixingAudioSupported, messageType::request), InitFunction_BasicCommunication_MixingAudioSupported_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_MixingAudioSupported, messageType::response), InitFunction_BasicCommunication_MixingAudioSupported_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_PlayTone, messageType::notification), InitFunction_BasicCommunication_PlayTone_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnSystemRequest, messageType::notification), InitFunction_BasicCommunication_OnSystemRequest_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_SystemRequest, messageType::request), InitFunction_BasicCommunication_SystemRequest_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_SystemRequest, messageType::response), InitFunction_BasicCommunication_SystemRequest_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_PolicyUpdate, messageType::request), InitFunction_BasicCommunication_PolicyUpdate_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_PolicyUpdate, messageType::response), InitFunction_BasicCommunication_PolicyUpdate_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnSDLClose, messageType::notification), InitFunction_BasicCommunication_OnSDLClose_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnPutFile, messageType::notification), InitFunction_BasicCommunication_OnPutFile_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_GetSystemInfo, messageType::request), InitFunction_BasicCommunication_GetSystemInfo_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_GetSystemInfo, messageType::response), InitFunction_BasicCommunication_GetSystemInfo_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnSystemInfoChanged, messageType::notification), InitFunction_BasicCommunication_OnSystemInfoChanged_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::BasicCommunication_OnIgnitionCycleOver, messageType::notification), InitFunction_BasicCommunication_OnIgnitionCycleOver_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_IsReady, messageType::request), InitFunction_VR_IsReady_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_IsReady, messageType::response), InitFunction_VR_IsReady_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_Started, messageType::notification), InitFunction_VR_Started_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_Stopped, messageType::notification), InitFunction_VR_Stopped_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_AddCommand, messageType::request), InitFunction_VR_AddCommand_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_AddCommand, messageType::response), InitFunction_VR_AddCommand_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_DeleteCommand, messageType::request), InitFunction_VR_DeleteCommand_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_DeleteCommand, messageType::response), InitFunction_VR_DeleteCommand_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_PerformInteraction, messageType::request), InitFunction_VR_PerformInteraction_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_PerformInteraction, messageType::response), InitFunction_VR_PerformInteraction_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_OnCommand, messageType::notification), InitFunction_VR_OnCommand_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_ChangeRegistration, messageType::request), InitFunction_VR_ChangeRegistration_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_ChangeRegistration, messageType::response), InitFunction_VR_ChangeRegistration_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_OnLanguageChange, messageType::notification), InitFunction_VR_OnLanguageChange_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_GetSupportedLanguages, messageType::request), InitFunction_VR_GetSupportedLanguages_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_GetSupportedLanguages, messageType::response), InitFunction_VR_GetSupportedLanguages_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_GetLanguage, messageType::request), InitFunction_VR_GetLanguage_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_GetLanguage, messageType::response), InitFunction_VR_GetLanguage_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_GetCapabilities, messageType::request), InitFunction_VR_GetCapabilities_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VR_GetCapabilities, messageType::response), InitFunction_VR_GetCapabilities_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_GetCapabilities, messageType::request), InitFunction_TTS_GetCapabilities_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_GetCapabilities, messageType::response), InitFunction_TTS_GetCapabilities_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_Started, messageType::notification), InitFunction_TTS_Started_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_Stopped, messageType::notification), InitFunction_TTS_Stopped_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_IsReady, messageType::request), InitFunction_TTS_IsReady_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_IsReady, messageType::response), InitFunction_TTS_IsReady_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_Speak, messageType::request), InitFunction_TTS_Speak_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_Speak, messageType::response), InitFunction_TTS_Speak_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_StopSpeaking, messageType::request), InitFunction_TTS_StopSpeaking_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_StopSpeaking, messageType::response), InitFunction_TTS_StopSpeaking_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_ChangeRegistration, messageType::request), InitFunction_TTS_ChangeRegistration_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_ChangeRegistration, messageType::response), InitFunction_TTS_ChangeRegistration_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_OnLanguageChange, messageType::notification), InitFunction_TTS_OnLanguageChange_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_GetSupportedLanguages, messageType::request), InitFunction_TTS_GetSupportedLanguages_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_GetSupportedLanguages, messageType::response), InitFunction_TTS_GetSupportedLanguages_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_GetLanguage, messageType::request), InitFunction_TTS_GetLanguage_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_GetLanguage, messageType::response), InitFunction_TTS_GetLanguage_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_SetGlobalProperties, messageType::request), InitFunction_TTS_SetGlobalProperties_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::TTS_SetGlobalProperties, messageType::response), InitFunction_TTS_SetGlobalProperties_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_Alert, messageType::request), InitFunction_UI_Alert_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_Alert, messageType::response), InitFunction_UI_Alert_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_Show, messageType::request), InitFunction_UI_Show_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_Show, messageType::response), InitFunction_UI_Show_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_AddCommand, messageType::request), InitFunction_UI_AddCommand_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_AddCommand, messageType::response), InitFunction_UI_AddCommand_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_DeleteCommand, messageType::request), InitFunction_UI_DeleteCommand_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_DeleteCommand, messageType::response), InitFunction_UI_DeleteCommand_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_AddSubMenu, messageType::request), InitFunction_UI_AddSubMenu_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_AddSubMenu, messageType::response), InitFunction_UI_AddSubMenu_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_DeleteSubMenu, messageType::request), InitFunction_UI_DeleteSubMenu_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_DeleteSubMenu, messageType::response), InitFunction_UI_DeleteSubMenu_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_PerformInteraction, messageType::request), InitFunction_UI_PerformInteraction_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_PerformInteraction, messageType::response), InitFunction_UI_PerformInteraction_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetMediaClockTimer, messageType::request), InitFunction_UI_SetMediaClockTimer_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetMediaClockTimer, messageType::response), InitFunction_UI_SetMediaClockTimer_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetGlobalProperties, messageType::request), InitFunction_UI_SetGlobalProperties_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetGlobalProperties, messageType::response), InitFunction_UI_SetGlobalProperties_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_OnCommand, messageType::notification), InitFunction_UI_OnCommand_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_OnSystemContext, messageType::notification), InitFunction_UI_OnSystemContext_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_GetCapabilities, messageType::request), InitFunction_UI_GetCapabilities_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_GetCapabilities, messageType::response), InitFunction_UI_GetCapabilities_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ChangeRegistration, messageType::request), InitFunction_UI_ChangeRegistration_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ChangeRegistration, messageType::response), InitFunction_UI_ChangeRegistration_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_OnLanguageChange, messageType::notification), InitFunction_UI_OnLanguageChange_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_GetSupportedLanguages, messageType::request), InitFunction_UI_GetSupportedLanguages_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_GetSupportedLanguages, messageType::response), InitFunction_UI_GetSupportedLanguages_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_GetLanguage, messageType::request), InitFunction_UI_GetLanguage_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_GetLanguage, messageType::response), InitFunction_UI_GetLanguage_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_OnDriverDistraction, messageType::notification), InitFunction_UI_OnDriverDistraction_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetAppIcon, messageType::request), InitFunction_UI_SetAppIcon_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetAppIcon, messageType::response), InitFunction_UI_SetAppIcon_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetDisplayLayout, messageType::request), InitFunction_UI_SetDisplayLayout_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_SetDisplayLayout, messageType::response), InitFunction_UI_SetDisplayLayout_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ShowCustomForm, messageType::request), InitFunction_UI_ShowCustomForm_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ShowCustomForm, messageType::response), InitFunction_UI_ShowCustomForm_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_OnKeyboardInput, messageType::notification), InitFunction_UI_OnKeyboardInput_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_OnTouchEvent, messageType::notification), InitFunction_UI_OnTouchEvent_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_Slider, messageType::request), InitFunction_UI_Slider_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_Slider, messageType::response), InitFunction_UI_Slider_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ScrollableMessage, messageType::request), InitFunction_UI_ScrollableMessage_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ScrollableMessage, messageType::response), InitFunction_UI_ScrollableMessage_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_PerformAudioPassThru, messageType::request), InitFunction_UI_PerformAudioPassThru_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_PerformAudioPassThru, messageType::response), InitFunction_UI_PerformAudioPassThru_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_EndAudioPassThru, messageType::request), InitFunction_UI_EndAudioPassThru_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_EndAudioPassThru, messageType::response), InitFunction_UI_EndAudioPassThru_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_IsReady, messageType::request), InitFunction_UI_IsReady_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_IsReady, messageType::response), InitFunction_UI_IsReady_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ClosePopUp, messageType::request), InitFunction_UI_ClosePopUp_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_ClosePopUp, messageType::response), InitFunction_UI_ClosePopUp_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_OnResetTimeout, messageType::notification), InitFunction_UI_OnResetTimeout_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UI_OnRecordStart, messageType::notification), InitFunction_UI_OnRecordStart_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_IsReady, messageType::request), InitFunction_Navigation_IsReady_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_IsReady, messageType::response), InitFunction_Navigation_IsReady_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_ShowConstantTBT, messageType::request), InitFunction_Navigation_ShowConstantTBT_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_ShowConstantTBT, messageType::response), InitFunction_Navigation_ShowConstantTBT_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_AlertManeuver, messageType::request), InitFunction_Navigation_AlertManeuver_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_AlertManeuver, messageType::response), InitFunction_Navigation_AlertManeuver_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_UpdateTurnList, messageType::request), InitFunction_Navigation_UpdateTurnList_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_UpdateTurnList, messageType::response), InitFunction_Navigation_UpdateTurnList_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_OnTBTClientState, messageType::notification), InitFunction_Navigation_OnTBTClientState_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StartStream, messageType::request), InitFunction_Navigation_StartStream_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StartStream, messageType::response), InitFunction_Navigation_StartStream_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StopStream, messageType::request), InitFunction_Navigation_StopStream_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StopStream, messageType::response), InitFunction_Navigation_StopStream_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StartAudioStream, messageType::request), InitFunction_Navigation_StartAudioStream_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StartAudioStream, messageType::response), InitFunction_Navigation_StartAudioStream_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StopAudioStream, messageType::request), InitFunction_Navigation_StopAudioStream_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Navigation_StopAudioStream, messageType::response), InitFunction_Navigation_StopAudioStream_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_IsReady, messageType::request), InitFunction_VehicleInfo_IsReady_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_IsReady, messageType::response), InitFunction_VehicleInfo_IsReady_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_GetVehicleType, messageType::request), InitFunction_VehicleInfo_GetVehicleType_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_GetVehicleType, messageType::response), InitFunction_VehicleInfo_GetVehicleType_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_ReadDID, messageType::request), InitFunction_VehicleInfo_ReadDID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_ReadDID, messageType::response), InitFunction_VehicleInfo_ReadDID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_GetDTCs, messageType::request), InitFunction_VehicleInfo_GetDTCs_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_GetDTCs, messageType::response), InitFunction_VehicleInfo_GetDTCs_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_DiagnosticMessage, messageType::request), InitFunction_VehicleInfo_DiagnosticMessage_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_DiagnosticMessage, messageType::response), InitFunction_VehicleInfo_DiagnosticMessage_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_SubscribeVehicleData, messageType::request), InitFunction_VehicleInfo_SubscribeVehicleData_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_SubscribeVehicleData, messageType::response), InitFunction_VehicleInfo_SubscribeVehicleData_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_UnsubscribeVehicleData, messageType::request), InitFunction_VehicleInfo_UnsubscribeVehicleData_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_UnsubscribeVehicleData, messageType::response), InitFunction_VehicleInfo_UnsubscribeVehicleData_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_GetVehicleData, messageType::request), InitFunction_VehicleInfo_GetVehicleData_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_GetVehicleData, messageType::response), InitFunction_VehicleInfo_GetVehicleData_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::VehicleInfo_OnVehicleData, messageType::notification), InitFunction_VehicleInfo_OnVehicleData_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_ActivateApp, messageType::request), InitFunction_SDL_ActivateApp_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_ActivateApp, messageType::response), InitFunction_SDL_ActivateApp_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetUserFriendlyMessage, messageType::request), InitFunction_SDL_GetUserFriendlyMessage_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetUserFriendlyMessage, messageType::response), InitFunction_SDL_GetUserFriendlyMessage_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_OnAllowSDLFunctionality, messageType::notification), InitFunction_SDL_OnAllowSDLFunctionality_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_OnReceivedPolicyUpdate, messageType::notification), InitFunction_SDL_OnReceivedPolicyUpdate_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_OnPolicyUpdate, messageType::notification), InitFunction_SDL_OnPolicyUpdate_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetListOfPermissions, messageType::request), InitFunction_SDL_GetListOfPermissions_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetListOfPermissions, messageType::response), InitFunction_SDL_GetListOfPermissions_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_OnAppPermissionConsent, messageType::notification), InitFunction_SDL_OnAppPermissionConsent_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_OnAppPermissionChanged, messageType::notification), InitFunction_SDL_OnAppPermissionChanged_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_OnSDLConsentNeeded, messageType::notification), InitFunction_SDL_OnSDLConsentNeeded_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_UpdateSDL, messageType::request), InitFunction_SDL_UpdateSDL_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_UpdateSDL, messageType::response), InitFunction_SDL_UpdateSDL_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetStatusUpdate, messageType::request), InitFunction_SDL_GetStatusUpdate_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetStatusUpdate, messageType::response), InitFunction_SDL_GetStatusUpdate_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_OnStatusUpdate, messageType::notification), InitFunction_SDL_OnStatusUpdate_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_OnSystemError, messageType::notification), InitFunction_SDL_OnSystemError_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_AddStatisticsInfo, messageType::notification), InitFunction_SDL_AddStatisticsInfo_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetURLS, messageType::request), InitFunction_SDL_GetURLS_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SDL_GetURLS, messageType::response), InitFunction_SDL_GetURLS_response(struct_schema_items, function_id_items, message_type_items)));
}

//------------- Functions schemes initialization -------------

CSmartSchema hmi_apis::HMI_API::InitFunction_Buttons_GetCapabilities_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Buttons_GetCapabilities_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter capabilities.
  //
  // Response must provide the names of available buttons and their capabilities.See ButtonCapabilities
  utils::SharedPtr<ISchemaItem> capabilities_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_ButtonCapabilities), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter presetBankCapabilities.
  //
  // Must be returned if the platform supports custom on-screen Presets
  utils::SharedPtr<ISchemaItem> presetBankCapabilities_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_PresetBankCapabilities);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["capabilities"] = CObjectSchemaItem::SMember(capabilities_SchemaItem, true);
  schema_members["presetBankCapabilities"] = CObjectSchemaItem::SMember(presetBankCapabilities_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Buttons_OnButtonEvent_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_ButtonName::eType> Common_ButtonName_all_enum_values;
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::OK);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::SEEKLEFT);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::SEEKRIGHT);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::TUNEUP);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::TUNEDOWN);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_0);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_1);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_2);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_3);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_4);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_5);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_6);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_7);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_8);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_9);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::CUSTOM_BUTTON);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::SEARCH);

  std::set<Common_ButtonEventMode::eType> Common_ButtonEventMode_all_enum_values;
  Common_ButtonEventMode_all_enum_values.insert(Common_ButtonEventMode::BUTTONUP);
  Common_ButtonEventMode_all_enum_values.insert(Common_ButtonEventMode::BUTTONDOWN);

  // Function parameter name.
  utils::SharedPtr<ISchemaItem> name_SchemaItem = TEnumSchemaItem<Common_ButtonName::eType>::create(Common_ButtonName_all_enum_values, TSchemaItemParameter<Common_ButtonName::eType>());

  // Function parameter mode.
  //
  // Indicates whether this is an UP or DOWN event.
  utils::SharedPtr<ISchemaItem> mode_SchemaItem = TEnumSchemaItem<Common_ButtonEventMode::eType>::create(Common_ButtonEventMode_all_enum_values, TSchemaItemParameter<Common_ButtonEventMode::eType>());

  // Function parameter customButtonID.
  //
  // Must be provided if ButtonName is CUSTOM_BUTTON, this references the integer ID passed by a custom button. (e.g. softButtonName)
  utils::SharedPtr<ISchemaItem> customButtonID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65536), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["name"] = CObjectSchemaItem::SMember(name_SchemaItem, true);
  schema_members["mode"] = CObjectSchemaItem::SMember(mode_SchemaItem, true);
  schema_members["customButtonID"] = CObjectSchemaItem::SMember(customButtonID_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Buttons_OnButtonPress_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_ButtonName::eType> Common_ButtonName_all_enum_values;
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::OK);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::SEEKLEFT);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::SEEKRIGHT);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::TUNEUP);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::TUNEDOWN);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_0);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_1);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_2);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_3);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_4);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_5);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_6);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_7);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_8);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_9);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::CUSTOM_BUTTON);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::SEARCH);

  std::set<Common_ButtonPressMode::eType> Common_ButtonPressMode_all_enum_values;
  Common_ButtonPressMode_all_enum_values.insert(Common_ButtonPressMode::LONG);
  Common_ButtonPressMode_all_enum_values.insert(Common_ButtonPressMode::SHORT);

  // Function parameter name.
  utils::SharedPtr<ISchemaItem> name_SchemaItem = TEnumSchemaItem<Common_ButtonName::eType>::create(Common_ButtonName_all_enum_values, TSchemaItemParameter<Common_ButtonName::eType>());

  // Function parameter mode.
  //
  // Indicates whether this is a LONG or SHORT button press event.
  utils::SharedPtr<ISchemaItem> mode_SchemaItem = TEnumSchemaItem<Common_ButtonPressMode::eType>::create(Common_ButtonPressMode_all_enum_values, TSchemaItemParameter<Common_ButtonPressMode::eType>());

  // Function parameter customButtonID.
  //
  // Must be returned if ButtonName is CUSTOM_BUTTON, this references the string passed by a custom button. (e.g. softButtonName)
  utils::SharedPtr<ISchemaItem> customButtonID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65536), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["name"] = CObjectSchemaItem::SMember(name_SchemaItem, true);
  schema_members["mode"] = CObjectSchemaItem::SMember(mode_SchemaItem, true);
  schema_members["customButtonID"] = CObjectSchemaItem::SMember(customButtonID_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnReady_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnStartDeviceDiscovery_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnResumeAudioSource_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  //
  // The ID of app to be resumed audio source.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DeviceInfo);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_UpdateDeviceList_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter deviceList.
  //
  // The array of names/IDs of connected devices
  utils::SharedPtr<ISchemaItem> deviceList_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DeviceInfo), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["deviceList"] = CObjectSchemaItem::SMember(deviceList_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_UpdateDeviceList_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnFileRemoved_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_FileType::eType> Common_FileType_all_enum_values;
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_BMP);
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_JPEG);
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_PNG);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_WAVE);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_MP3);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_AAC);
  Common_FileType_all_enum_values.insert(Common_FileType::BINARY);
  Common_FileType_all_enum_values.insert(Common_FileType::JSON);

  // Function parameter fileName.
  //
  // The name of the removed file
  utils::SharedPtr<ISchemaItem> fileName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(30), TSchemaItemParameter<std::string>());

  // Function parameter fileType.
  //
  // The file type. See FileType.
  utils::SharedPtr<ISchemaItem> fileType_SchemaItem = TEnumSchemaItem<Common_FileType::eType>::create(Common_FileType_all_enum_values, TSchemaItemParameter<Common_FileType::eType>());

  // Function parameter appID.
  //
  // ID of the application.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["fileName"] = CObjectSchemaItem::SMember(fileName_SchemaItem, true);
  schema_members["fileType"] = CObjectSchemaItem::SMember(fileType_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_AllowDeviceToConnect_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter device.
  utils::SharedPtr<ISchemaItem> device_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DeviceInfo);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["device"] = CObjectSchemaItem::SMember(device_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_AllowDeviceToConnect_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter allow.
  utils::SharedPtr<ISchemaItem> allow_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["allow"] = CObjectSchemaItem::SMember(allow_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnDeviceChosen_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter deviceInfo.
  //
  // The name and ID of the device chosen
  utils::SharedPtr<ISchemaItem> deviceInfo_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DeviceInfo);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["deviceInfo"] = CObjectSchemaItem::SMember(deviceInfo_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnFindApplications_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter deviceInfo.
  //
  // The name and ID of the device the list of registered applications is required for.
  utils::SharedPtr<ISchemaItem> deviceInfo_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DeviceInfo);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["deviceInfo"] = CObjectSchemaItem::SMember(deviceInfo_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_ActivateApp_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_AppPriority::eType> Common_AppPriority_all_enum_values;
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::EMERGENCY);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NAVIGATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::VOICE_COMMUNICATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::COMMUNICATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NORMAL);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NONE);

  // Function parameter appID.
  //
  // ID of deactivated application.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Function parameter priority.
  //
  // Send to HMI so that it can coordinate order of requests/notifications correspondingly.
  utils::SharedPtr<ISchemaItem> priority_SchemaItem = TEnumSchemaItem<Common_AppPriority::eType>::create(Common_AppPriority_all_enum_values, TSchemaItemParameter<Common_AppPriority::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);
  schema_members["priority"] = CObjectSchemaItem::SMember(priority_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_ActivateApp_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnAppActivated_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  //
  // ID of selected application.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnAppDeactivated_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_DeactivateReason::eType> Common_DeactivateReason_all_enum_values;
  Common_DeactivateReason_all_enum_values.insert(Common_DeactivateReason::AUDIO);
  Common_DeactivateReason_all_enum_values.insert(Common_DeactivateReason::PHONECALL);
  Common_DeactivateReason_all_enum_values.insert(Common_DeactivateReason::NAVIGATIONMAP);
  Common_DeactivateReason_all_enum_values.insert(Common_DeactivateReason::PHONEMENU);
  Common_DeactivateReason_all_enum_values.insert(Common_DeactivateReason::SYNCSETTINGS);
  Common_DeactivateReason_all_enum_values.insert(Common_DeactivateReason::GENERAL);

  // Function parameter appID.
  //
  // ID of deactivated application.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Function parameter reason.
  //
  // Specifies the functionality the user has switched to.
  utils::SharedPtr<ISchemaItem> reason_SchemaItem = TEnumSchemaItem<Common_DeactivateReason::eType>::create(Common_DeactivateReason_all_enum_values, TSchemaItemParameter<Common_DeactivateReason::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);
  schema_members["reason"] = CObjectSchemaItem::SMember(reason_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnAppRegistered_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_AppPriority::eType> Common_AppPriority_all_enum_values;
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::EMERGENCY);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NAVIGATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::VOICE_COMMUNICATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::COMMUNICATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NORMAL);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NONE);

  // Function parameter application.
  //
  // The information about application registered. See HMIApplication. 
  utils::SharedPtr<ISchemaItem> application_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_HMIApplication);

  // Function parameter ttsName.
  //
  //  TTS string for VR recognition of the mobile application name, e.g. "Ford Drive Green".Meant to overcome any failing on speech engine in properly pronouncing / understanding app name.
  //           Needs to be unique over all applications.
  //           May not be empty.
  //           May not start with a new line character.
  //           May not interfere with any name or synonym of previously registered applications and any predefined blacklist of words (global commands)
  //         
  utils::SharedPtr<ISchemaItem> ttsName_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter vrSynonyms.
  //
  // 
  //           Defines additional voice recognition commands.
  //           May not interfere with any name or synonym of previously registered applications and any predefined blacklist of words (global commands)
  //         
  utils::SharedPtr<ISchemaItem> vrSynonyms_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(40), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter resumeVrGrammars.
  //
  // The flag if VR grammars resume is required
  utils::SharedPtr<ISchemaItem> resumeVrGrammars_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter priority.
  //
  // Send to HMI so that it can coordinate order of requests/notifications correspondingly.
  utils::SharedPtr<ISchemaItem> priority_SchemaItem = TEnumSchemaItem<Common_AppPriority::eType>::create(Common_AppPriority_all_enum_values, TSchemaItemParameter<Common_AppPriority::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["application"] = CObjectSchemaItem::SMember(application_SchemaItem, true);
  schema_members["ttsName"] = CObjectSchemaItem::SMember(ttsName_SchemaItem, false);
  schema_members["vrSynonyms"] = CObjectSchemaItem::SMember(vrSynonyms_SchemaItem, false);
  schema_members["resumeVrGrammars"] = CObjectSchemaItem::SMember(resumeVrGrammars_SchemaItem, false);
  schema_members["priority"] = CObjectSchemaItem::SMember(priority_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnAppUnregistered_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  //
  // ID of the application unregistered
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnExitApplication_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  //
  // ID of the application to be exited.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnExitAllApplications_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_ApplicationsCloseReason::eType> Common_ApplicationsCloseReason_all_enum_values;
  Common_ApplicationsCloseReason_all_enum_values.insert(Common_ApplicationsCloseReason::IGNITION_OFF);
  Common_ApplicationsCloseReason_all_enum_values.insert(Common_ApplicationsCloseReason::MASTER_RESET);
  Common_ApplicationsCloseReason_all_enum_values.insert(Common_ApplicationsCloseReason::FACTORY_DEFAULTS);

  // Function parameter reason.
  //
  // Specifies reason for exiting all apllications.
  utils::SharedPtr<ISchemaItem> reason_SchemaItem = TEnumSchemaItem<Common_ApplicationsCloseReason::eType>::create(Common_ApplicationsCloseReason_all_enum_values, TSchemaItemParameter<Common_ApplicationsCloseReason::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["reason"] = CObjectSchemaItem::SMember(reason_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_MixingAudioSupported_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_MixingAudioSupported_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter attenuatedSupported.
  //
  // Must be true if supported
  utils::SharedPtr<ISchemaItem> attenuatedSupported_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["attenuatedSupported"] = CObjectSchemaItem::SMember(attenuatedSupported_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_PlayTone_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnSystemRequest_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_RequestType::eType> Common_RequestType_all_enum_values;
  Common_RequestType_all_enum_values.insert(Common_RequestType::HTTP);
  Common_RequestType_all_enum_values.insert(Common_RequestType::FILE_RESUME);
  Common_RequestType_all_enum_values.insert(Common_RequestType::AUTH_REQUEST);
  Common_RequestType_all_enum_values.insert(Common_RequestType::AUTH_CHALLENGE);
  Common_RequestType_all_enum_values.insert(Common_RequestType::AUTH_ACK);
  Common_RequestType_all_enum_values.insert(Common_RequestType::PROPRIETARY);

  std::set<Common_FileType::eType> Common_FileType_all_enum_values;
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_BMP);
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_JPEG);
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_PNG);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_WAVE);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_MP3);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_AAC);
  Common_FileType_all_enum_values.insert(Common_FileType::BINARY);
  Common_FileType_all_enum_values.insert(Common_FileType::JSON);

  // Function parameter requestType.
  //
  // The type of system request.
  utils::SharedPtr<ISchemaItem> requestType_SchemaItem = TEnumSchemaItem<Common_RequestType::eType>::create(Common_RequestType_all_enum_values, TSchemaItemParameter<Common_RequestType::eType>());

  // Function parameter url.
  //
  // Optional array of URL(s) for HTTP requests.
  utils::SharedPtr<ISchemaItem> url_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter fileType.
  //
  // Optional file type (meant for HTTP file requests).
  utils::SharedPtr<ISchemaItem> fileType_SchemaItem = TEnumSchemaItem<Common_FileType::eType>::create(Common_FileType_all_enum_values, TSchemaItemParameter<Common_FileType::eType>());

  // Function parameter offset.
  //
  // Optional offset in bytes for resuming partial data chunks
  utils::SharedPtr<ISchemaItem> offset_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(100000000000), TSchemaItemParameter<int>());

  // Function parameter length.
  //
  // Optional length in bytes for resuming partial data chunks
  utils::SharedPtr<ISchemaItem> length_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(100000000000), TSchemaItemParameter<int>());

  // Function parameter timeout.
  utils::SharedPtr<ISchemaItem> timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter fileName.
  //
  // File reference name.
  utils::SharedPtr<ISchemaItem> fileName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(255), TSchemaItemParameter<std::string>());

  // Function parameter appID.
  //
  // ID of application that requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(50), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["requestType"] = CObjectSchemaItem::SMember(requestType_SchemaItem, true);
  schema_members["url"] = CObjectSchemaItem::SMember(url_SchemaItem, false);
  schema_members["fileType"] = CObjectSchemaItem::SMember(fileType_SchemaItem, false);
  schema_members["offset"] = CObjectSchemaItem::SMember(offset_SchemaItem, false);
  schema_members["length"] = CObjectSchemaItem::SMember(length_SchemaItem, false);
  schema_members["timeout"] = CObjectSchemaItem::SMember(timeout_SchemaItem, false);
  schema_members["fileName"] = CObjectSchemaItem::SMember(fileName_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_SystemRequest_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_RequestType::eType> Common_RequestType_all_enum_values;
  Common_RequestType_all_enum_values.insert(Common_RequestType::HTTP);
  Common_RequestType_all_enum_values.insert(Common_RequestType::FILE_RESUME);
  Common_RequestType_all_enum_values.insert(Common_RequestType::AUTH_REQUEST);
  Common_RequestType_all_enum_values.insert(Common_RequestType::AUTH_CHALLENGE);
  Common_RequestType_all_enum_values.insert(Common_RequestType::AUTH_ACK);
  Common_RequestType_all_enum_values.insert(Common_RequestType::PROPRIETARY);

  // Function parameter requestType.
  //
  // The type of system request.
  utils::SharedPtr<ISchemaItem> requestType_SchemaItem = TEnumSchemaItem<Common_RequestType::eType>::create(Common_RequestType_all_enum_values, TSchemaItemParameter<Common_RequestType::eType>());

  // Function parameter fileName.
  //
  // The path to file.
  utils::SharedPtr<ISchemaItem> fileName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(255), TSchemaItemParameter<std::string>());

  // Function parameter appID.
  //
  // ID of application that requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(50), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["requestType"] = CObjectSchemaItem::SMember(requestType_SchemaItem, true);
  schema_members["fileName"] = CObjectSchemaItem::SMember(fileName_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_SystemRequest_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_PolicyUpdate_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter file.
  //
  // 
  //           Location of policy table snapshot
  //         
  utils::SharedPtr<ISchemaItem> file_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(255), TSchemaItemParameter<std::string>());

  // Function parameter timeout.
  //
  // 
  //           Send attempt timeout in seconds
  //         
  utils::SharedPtr<ISchemaItem> timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Function parameter retry.
  //
  // 
  //           Array of delays to wait after failed atempts
  //         
  utils::SharedPtr<ISchemaItem> retry_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(5));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["file"] = CObjectSchemaItem::SMember(file_SchemaItem, true);
  schema_members["timeout"] = CObjectSchemaItem::SMember(timeout_SchemaItem, true);
  schema_members["retry"] = CObjectSchemaItem::SMember(retry_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_PolicyUpdate_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnSDLClose_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnPutFile_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_FileType::eType> Common_FileType_all_enum_values;
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_BMP);
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_JPEG);
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_PNG);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_WAVE);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_MP3);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_AAC);
  Common_FileType_all_enum_values.insert(Common_FileType::BINARY);
  Common_FileType_all_enum_values.insert(Common_FileType::JSON);

  // Function parameter offset.
  //
  // Optional offset in bytes for resuming partial data chunks
  utils::SharedPtr<ISchemaItem> offset_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(100000000000), TSchemaItemParameter<int>());

  // Function parameter length.
  //
  // Optional length in bytes for resuming partial data chunks
  utils::SharedPtr<ISchemaItem> length_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(100000000000), TSchemaItemParameter<int>());

  // Function parameter fileSize.
  //
  // Full Size of file. sends in first OnPutFile notification if file is splited
  // 		     	   into many PutFiles
  // 	  
  utils::SharedPtr<ISchemaItem> fileSize_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(100000000000), TSchemaItemParameter<int>());

  // Function parameter FileName.
  //
  // File reference name.
  utils::SharedPtr<ISchemaItem> FileName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(255), TSchemaItemParameter<std::string>());

  // Function parameter syncFileName.
  //
  // File reference name.
  utils::SharedPtr<ISchemaItem> syncFileName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(255), TSchemaItemParameter<std::string>());

  // Function parameter fileType.
  //
  // Selected file type.
  utils::SharedPtr<ISchemaItem> fileType_SchemaItem = TEnumSchemaItem<Common_FileType::eType>::create(Common_FileType_all_enum_values, TSchemaItemParameter<Common_FileType::eType>());

  // Function parameter persistentFile.
  //
  // 
  //         Indicates if the file is meant to persist between sessions / ignition cycles.
  //         If set to TRUE, then the system will aim to persist this file through session / cycles.
  //         While files with this designation will have priority over others, they are subject to deletion by the system at any time.
  //         In the event of automatic deletion by the system, the app will receive a rejection and have to resend the file.
  //         If omitted, the value will be set to false.
  //       
  utils::SharedPtr<ISchemaItem> persistentFile_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>(false));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["offset"] = CObjectSchemaItem::SMember(offset_SchemaItem, false);
  schema_members["length"] = CObjectSchemaItem::SMember(length_SchemaItem, false);
  schema_members["fileSize"] = CObjectSchemaItem::SMember(fileSize_SchemaItem, false);
  schema_members["FileName"] = CObjectSchemaItem::SMember(FileName_SchemaItem, true);
  schema_members["syncFileName"] = CObjectSchemaItem::SMember(syncFileName_SchemaItem, true);
  schema_members["fileType"] = CObjectSchemaItem::SMember(fileType_SchemaItem, true);
  schema_members["persistentFile"] = CObjectSchemaItem::SMember(persistentFile_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_GetSystemInfo_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_GetSystemInfo_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter ccpu_version.
  //
  // Software version of the module
  utils::SharedPtr<ISchemaItem> ccpu_version_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter language.
  //
  // ISO 639-1 combined with ISO 3166 alpha-2 country code (i.e. en-us)
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  // Function parameter wersCountryCode.
  //
  // Country code from the Ford system WERS (i.e.WAEGB).
  utils::SharedPtr<ISchemaItem> wersCountryCode_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["ccpu_version"] = CObjectSchemaItem::SMember(ccpu_version_SchemaItem, true);
  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);
  schema_members["wersCountryCode"] = CObjectSchemaItem::SMember(wersCountryCode_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnSystemInfoChanged_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter language.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_BasicCommunication_OnIgnitionCycleOver_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_IsReady_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_IsReady_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter available.
  //
  // Must be true if VR is present and ready to communicate with SDL.
  utils::SharedPtr<ISchemaItem> available_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["available"] = CObjectSchemaItem::SMember(available_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_Started_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_Stopped_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_AddCommand_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_VRCommandType::eType> Common_VRCommandType_all_enum_values;
  Common_VRCommandType_all_enum_values.insert(Common_VRCommandType::Choice);
  Common_VRCommandType_all_enum_values.insert(Common_VRCommandType::Command);

  // Function parameter cmdID.
  //
  // ID of a command (further to be used in OnCommand notification).
  utils::SharedPtr<ISchemaItem> cmdID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter vrCommands.
  //
  // List of strings to be used as VR commands.
  utils::SharedPtr<ISchemaItem> vrCommands_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(99), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter type.
  //
  // Type of added command. See VRCommandType.
  utils::SharedPtr<ISchemaItem> type_SchemaItem = TEnumSchemaItem<Common_VRCommandType::eType>::create(Common_VRCommandType_all_enum_values, TSchemaItemParameter<Common_VRCommandType::eType>());

  // Function parameter grammarID.
  //
  // 
  //         ID of the specific grammar, whether top-level or choice set.
  //       
  utils::SharedPtr<ISchemaItem> grammarID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter appID.
  //
  // ID of application that requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["cmdID"] = CObjectSchemaItem::SMember(cmdID_SchemaItem, true);
  schema_members["vrCommands"] = CObjectSchemaItem::SMember(vrCommands_SchemaItem, true);
  schema_members["type"] = CObjectSchemaItem::SMember(type_SchemaItem, true);
  schema_members["grammarID"] = CObjectSchemaItem::SMember(grammarID_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_AddCommand_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_DeleteCommand_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_VRCommandType::eType> Common_VRCommandType_all_enum_values;
  Common_VRCommandType_all_enum_values.insert(Common_VRCommandType::Choice);
  Common_VRCommandType_all_enum_values.insert(Common_VRCommandType::Command);

  // Function parameter cmdID.
  //
  // Id of a command (list of strings), previously sent by AddCommand.
  utils::SharedPtr<ISchemaItem> cmdID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter type.
  //
  // Type of added command. See VRCommandType.
  utils::SharedPtr<ISchemaItem> type_SchemaItem = TEnumSchemaItem<Common_VRCommandType::eType>::create(Common_VRCommandType_all_enum_values, TSchemaItemParameter<Common_VRCommandType::eType>());

  // Function parameter grammarID.
  //
  // ID of the specific grammar.
  utils::SharedPtr<ISchemaItem> grammarID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter appID.
  //
  // ID of application that requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["cmdID"] = CObjectSchemaItem::SMember(cmdID_SchemaItem, true);
  schema_members["type"] = CObjectSchemaItem::SMember(type_SchemaItem, true);
  schema_members["grammarID"] = CObjectSchemaItem::SMember(grammarID_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_DeleteCommand_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_PerformInteraction_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter helpPrompt.
  //
  // 
  //         The help prompt. An array of text chunks of type TTSChunk. See TTSChunk. The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> helpPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter initialPrompt.
  //
  // 
  //         This is the intial prompt spoken to the user at the start of an interaction. An array of text chunks of type TTSChunk. See TTSChunk. The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> initialPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter timeoutPrompt.
  //
  // 
  //         Help text for a wait timeout. An array of text chunks of type TTSChunk. See TTSChunk. The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> timeoutPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter timeout.
  //
  // Timeout initiate timeoutPrompt
  utils::SharedPtr<ISchemaItem> timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Function parameter grammarID.
  //
  // 
  //         IDs of the specific compiled grammars for this voice interaction.
  //       
  utils::SharedPtr<ISchemaItem> grammarID_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["helpPrompt"] = CObjectSchemaItem::SMember(helpPrompt_SchemaItem, false);
  schema_members["initialPrompt"] = CObjectSchemaItem::SMember(initialPrompt_SchemaItem, false);
  schema_members["timeoutPrompt"] = CObjectSchemaItem::SMember(timeoutPrompt_SchemaItem, false);
  schema_members["timeout"] = CObjectSchemaItem::SMember(timeout_SchemaItem, true);
  schema_members["grammarID"] = CObjectSchemaItem::SMember(grammarID_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_PerformInteraction_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter choiceID.
  //
  // 
  //         ID of the choice that was selected in response to PerformInteraction.
  //       
  utils::SharedPtr<ISchemaItem> choiceID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["choiceID"] = CObjectSchemaItem::SMember(choiceID_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_OnCommand_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter cmdID.
  //
  // ID of of the command (list of strings)
  utils::SharedPtr<ISchemaItem> cmdID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["cmdID"] = CObjectSchemaItem::SMember(cmdID_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_ChangeRegistration_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter language.
  //
  // The language application wants to switch to.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  // Function parameter appID.
  //
  // ID of application that concerns this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_ChangeRegistration_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_OnLanguageChange_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter language.
  //
  // Language VR has switched to.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_GetSupportedLanguages_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_GetSupportedLanguages_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter languages.
  //
  // List of languages supported in VR.
  utils::SharedPtr<ISchemaItem> languages_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["languages"] = CObjectSchemaItem::SMember(languages_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_GetLanguage_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_GetLanguage_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter language.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_GetCapabilities_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VR_GetCapabilities_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_VrCapabilities::eType> Common_VrCapabilities_all_enum_values;
  Common_VrCapabilities_all_enum_values.insert(Common_VrCapabilities::VR_TEXT);

  // Function parameter vrCapabilities.
  //
  // Types of input recognized by VR module.
  utils::SharedPtr<ISchemaItem> vrCapabilities_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Common_VrCapabilities::eType>::create(Common_VrCapabilities_all_enum_values, TSchemaItemParameter<Common_VrCapabilities::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["vrCapabilities"] = CObjectSchemaItem::SMember(vrCapabilities_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_GetCapabilities_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_GetCapabilities_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_SpeechCapabilities::eType> Common_SpeechCapabilities_all_enum_values;
  Common_SpeechCapabilities_all_enum_values.insert(Common_SpeechCapabilities::SC_TEXT);
  Common_SpeechCapabilities_all_enum_values.insert(Common_SpeechCapabilities::SAPI_PHONEMES);
  Common_SpeechCapabilities_all_enum_values.insert(Common_SpeechCapabilities::LHPLUS_PHONEMES);
  Common_SpeechCapabilities_all_enum_values.insert(Common_SpeechCapabilities::PRE_RECORDED);
  Common_SpeechCapabilities_all_enum_values.insert(Common_SpeechCapabilities::SILENCE);

  std::set<Common_PrerecordedSpeech::eType> Common_PrerecordedSpeech_all_enum_values;
  Common_PrerecordedSpeech_all_enum_values.insert(Common_PrerecordedSpeech::HELP_JINGLE);
  Common_PrerecordedSpeech_all_enum_values.insert(Common_PrerecordedSpeech::INITIAL_JINGLE);
  Common_PrerecordedSpeech_all_enum_values.insert(Common_PrerecordedSpeech::LISTEN_JINGLE);
  Common_PrerecordedSpeech_all_enum_values.insert(Common_PrerecordedSpeech::POSITIVE_JINGLE);
  Common_PrerecordedSpeech_all_enum_values.insert(Common_PrerecordedSpeech::NEGATIVE_JINGLE);

  // Function parameter speechCapabilities.
  //
  // See SpeechCapabilities
  utils::SharedPtr<ISchemaItem> speechCapabilities_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Common_SpeechCapabilities::eType>::create(Common_SpeechCapabilities_all_enum_values, TSchemaItemParameter<Common_SpeechCapabilities::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(5));

  // Function parameter prerecordedSpeechCapabilities.
  //
  // See PrerecordedSpeech
  utils::SharedPtr<ISchemaItem> prerecordedSpeechCapabilities_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Common_PrerecordedSpeech::eType>::create(Common_PrerecordedSpeech_all_enum_values, TSchemaItemParameter<Common_PrerecordedSpeech::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(5));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["speechCapabilities"] = CObjectSchemaItem::SMember(speechCapabilities_SchemaItem, true);
  schema_members["prerecordedSpeechCapabilities"] = CObjectSchemaItem::SMember(prerecordedSpeechCapabilities_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_Started_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_Stopped_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_IsReady_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_IsReady_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter available.
  //
  // Must be true if TTS is present and ready to communicate with SDL.
  utils::SharedPtr<ISchemaItem> available_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["available"] = CObjectSchemaItem::SMember(available_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_Speak_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter ttsChunks.
  //
  // List of strings to be spoken.
  utils::SharedPtr<ISchemaItem> ttsChunks_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter appID.
  //
  // ID of application that requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["ttsChunks"] = CObjectSchemaItem::SMember(ttsChunks_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_Speak_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_StopSpeaking_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_StopSpeaking_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_ChangeRegistration_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter language.
  //
  // The language application wants to switch to.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_ChangeRegistration_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_OnLanguageChange_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter language.
  //
  // Language TTS has switched to.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_GetSupportedLanguages_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_GetSupportedLanguages_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter languages.
  //
  // List of languages supported in TTS.
  utils::SharedPtr<ISchemaItem> languages_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["languages"] = CObjectSchemaItem::SMember(languages_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_GetLanguage_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_GetLanguage_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter language.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_SetGlobalProperties_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter helpPrompt.
  //
  // 
  //           The help prompt.
  //           An array of text chunks of type TTSChunk. See TTSChunk.
  //           The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> helpPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter timeoutPrompt.
  //
  // 
  //           Help text for a wait timeout.
  //           An array of text chunks of type TTSChunk. See TTSChunk.
  //           The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> timeoutPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["helpPrompt"] = CObjectSchemaItem::SMember(helpPrompt_SchemaItem, false);
  schema_members["timeoutPrompt"] = CObjectSchemaItem::SMember(timeoutPrompt_SchemaItem, false);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_TTS_SetGlobalProperties_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_Alert_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter alertStrings.
  //
  // Array of lines of alert text fields. See TextFieldStruct. Uses alertText1, alertText2, alertText3.
  utils::SharedPtr<ISchemaItem> alertStrings_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TextFieldStruct), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(3));

  // Function parameter duration.
  //
  // Timeout in milliseconds.
  utils::SharedPtr<ISchemaItem> duration_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(3000), TSchemaItemParameter<int>(10000), TSchemaItemParameter<int>());

  // Function parameter softButtons.
  //
  // App defined SoftButtons
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(4));

  // Function parameter progressIndicator.
  //
  // If supported on the given platform, the alert GUI will include some sort of animation indicating that loading of a feature is progressing.  e.g. a spinning wheel or hourglass, etc.
  utils::SharedPtr<ISchemaItem> progressIndicator_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter appID.
  //
  // ID of application requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["alertStrings"] = CObjectSchemaItem::SMember(alertStrings_SchemaItem, true);
  schema_members["duration"] = CObjectSchemaItem::SMember(duration_SchemaItem, true);
  schema_members["softButtons"] = CObjectSchemaItem::SMember(softButtons_SchemaItem, false);
  schema_members["progressIndicator"] = CObjectSchemaItem::SMember(progressIndicator_SchemaItem, false);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_Alert_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter tryAgainTime.
  //
  // Amount of time (in milliseconds) that SDL must wait before resending an alert. Must be provided if another system event or overlay currently has a higher priority than this alert.
  utils::SharedPtr<ISchemaItem> tryAgainTime_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["tryAgainTime"] = CObjectSchemaItem::SMember(tryAgainTime_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_Show_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_TextAlignment::eType> Common_TextAlignment_all_enum_values;
  Common_TextAlignment_all_enum_values.insert(Common_TextAlignment::LEFT_ALIGNED);
  Common_TextAlignment_all_enum_values.insert(Common_TextAlignment::RIGHT_ALIGNED);
  Common_TextAlignment_all_enum_values.insert(Common_TextAlignment::CENTERED);

  // Function parameter showStrings.
  //
  // Array of lines of show text fields. See TextFieldStruct. Uses mainField1, mainField2, mainField3, mainField4. If some field is not set, the corresponding text should stay unchanged. If field's text is empty "", the field must be cleared.
  //           mainField1: The text that should be displayed in a single or upper display line.
  //           mainField2: The text that should be displayed on the second display line.
  //           mainField3: The text that should be displayed on the second "page" first display line.
  //           mainField4: The text that should be displayed on the second "page" second display line.
  //           statusBar: statusBar.
  //           mediaClock: Text value for MediaClock field.
  //           mediaTrack: The text that should be displayed in the track field. This field is only valid for media applications on NGN type ACMs.
  //       
  utils::SharedPtr<ISchemaItem> showStrings_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TextFieldStruct), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(7));

  // Function parameter alignment.
  //
  // Specifies how mainField1 and mainField2 texts should be aligned on the display.
  // If omitted, texts must be centered
  utils::SharedPtr<ISchemaItem> alignment_SchemaItem = TEnumSchemaItem<Common_TextAlignment::eType>::create(Common_TextAlignment_all_enum_values, TSchemaItemParameter<Common_TextAlignment::eType>());

  // Function parameter graphic.
  //
  // Path to optional dynamic image or the static binary image itself.  See Image. If omitted, the displayed graphic should not change.
  utils::SharedPtr<ISchemaItem> graphic_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  // Function parameter secondaryGraphic.
  //
  // 
  //         Image struct determining whether static or dynamic secondary image to display in app.
  //         If omitted on supported displays, the displayed secondary graphic shall not change.
  //       
  utils::SharedPtr<ISchemaItem> secondaryGraphic_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  // Function parameter softButtons.
  //
  // App defined SoftButtons.
  // If omitted, the currently displayed SoftButton values should not change.
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(8));

  // Function parameter customPresets.
  //
  // App labeled on-screen presets (i.e. GEN3 media presets or dynamic search suggestions).
  // If omitted on supported displays, the presets will be shown as not defined.
  utils::SharedPtr<ISchemaItem> customPresets_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(8));

  // Function parameter appID.
  //
  // Id of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["showStrings"] = CObjectSchemaItem::SMember(showStrings_SchemaItem, true);
  schema_members["alignment"] = CObjectSchemaItem::SMember(alignment_SchemaItem, false);
  schema_members["graphic"] = CObjectSchemaItem::SMember(graphic_SchemaItem, false);
  schema_members["secondaryGraphic"] = CObjectSchemaItem::SMember(secondaryGraphic_SchemaItem, false);
  schema_members["softButtons"] = CObjectSchemaItem::SMember(softButtons_SchemaItem, false);
  schema_members["customPresets"] = CObjectSchemaItem::SMember(customPresets_SchemaItem, false);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_Show_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_AddCommand_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter cmdID.
  //
  // ID of the command to be added.
  utils::SharedPtr<ISchemaItem> cmdID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter menuParams.
  //
  // Optional sub value containing parameters of the command (position, name, etc.). See MenuParams. If omitted the command should be added to the end of the list of commands.
  utils::SharedPtr<ISchemaItem> menuParams_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_MenuParams);

  // Function parameter cmdIcon.
  //
  // Image to be displayed for representing the command. See Image.
  // If omitted, no (or the default if applicable) icon should be displayed.
  utils::SharedPtr<ISchemaItem> cmdIcon_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  // Function parameter appID.
  //
  // ID of application that concerns this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["cmdID"] = CObjectSchemaItem::SMember(cmdID_SchemaItem, true);
  schema_members["menuParams"] = CObjectSchemaItem::SMember(menuParams_SchemaItem, false);
  schema_members["cmdIcon"] = CObjectSchemaItem::SMember(cmdIcon_SchemaItem, false);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_AddCommand_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_DeleteCommand_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter cmdID.
  //
  // cmdId previously sent via AddCommand request - id of the command to be deleted.
  utils::SharedPtr<ISchemaItem> cmdID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter appID.
  //
  // ID of application that concerns this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["cmdID"] = CObjectSchemaItem::SMember(cmdID_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_DeleteCommand_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_AddSubMenu_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter menuID.
  //
  // ID of the sub menu to be added. Unique for the application.
  utils::SharedPtr<ISchemaItem> menuID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter menuParams.
  //
  // Position and name of menu to be added. 'parent' field is omitted for this RPC.  
  utils::SharedPtr<ISchemaItem> menuParams_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_MenuParams);

  // Function parameter appID.
  //
  // ID of application that requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["menuID"] = CObjectSchemaItem::SMember(menuID_SchemaItem, true);
  schema_members["menuParams"] = CObjectSchemaItem::SMember(menuParams_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_AddSubMenu_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_DeleteSubMenu_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter menuID.
  //
  // The "menuID" of the sub-menu to be deleted. (See addSubMenu.menuID)
  utils::SharedPtr<ISchemaItem> menuID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter appID.
  //
  // ID of application that concerns this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["menuID"] = CObjectSchemaItem::SMember(menuID_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_DeleteSubMenu_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_PerformInteraction_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_LayoutMode::eType> Common_LayoutMode_all_enum_values;
  Common_LayoutMode_all_enum_values.insert(Common_LayoutMode::ICON_ONLY);
  Common_LayoutMode_all_enum_values.insert(Common_LayoutMode::ICON_WITH_SEARCH);
  Common_LayoutMode_all_enum_values.insert(Common_LayoutMode::LIST_ONLY);
  Common_LayoutMode_all_enum_values.insert(Common_LayoutMode::LIST_WITH_SEARCH);
  Common_LayoutMode_all_enum_values.insert(Common_LayoutMode::KEYBOARD);

  // Function parameter initialText.
  //
  // Uses initialInteractionText. See TextFieldStruct.
  utils::SharedPtr<ISchemaItem> initialText_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TextFieldStruct);

  // Function parameter choiceSet.
  //
  // The list of choices to be used for the interaction with the user
  utils::SharedPtr<ISchemaItem> choiceSet_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Choice), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter vrHelpTitle.
  //
  // VR Help Title text.
  // If omitted on supported displays, the default HU system help title should be used.
  utils::SharedPtr<ISchemaItem> vrHelpTitle_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter vrHelp.
  //
  // VR Help Items. If omitted on supported displays, the default HU system generated help items should be used.
  utils::SharedPtr<ISchemaItem> vrHelp_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VrHelpItem), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter timeout.
  //
  // Timeout in milliseconds.
  utils::SharedPtr<ISchemaItem> timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(5000), TSchemaItemParameter<int>(100000), TSchemaItemParameter<int>(10000));

  // Function parameter interactionLayout.
  //
  // See LayoutMode.
  utils::SharedPtr<ISchemaItem> interactionLayout_SchemaItem = TEnumSchemaItem<Common_LayoutMode::eType>::create(Common_LayoutMode_all_enum_values, TSchemaItemParameter<Common_LayoutMode::eType>());

  // Function parameter appID.
  //
  // ID of application that concerns this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["initialText"] = CObjectSchemaItem::SMember(initialText_SchemaItem, false);
  schema_members["choiceSet"] = CObjectSchemaItem::SMember(choiceSet_SchemaItem, false);
  schema_members["vrHelpTitle"] = CObjectSchemaItem::SMember(vrHelpTitle_SchemaItem, false);
  schema_members["vrHelp"] = CObjectSchemaItem::SMember(vrHelp_SchemaItem, false);
  schema_members["timeout"] = CObjectSchemaItem::SMember(timeout_SchemaItem, true);
  schema_members["interactionLayout"] = CObjectSchemaItem::SMember(interactionLayout_SchemaItem, false);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_PerformInteraction_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter choiceID.
  //
  // ID of the choice that was selected in response to PerformInteraction.
  utils::SharedPtr<ISchemaItem> choiceID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter manualTextEntry.
  //
  // 
  // 	       	Manually entered text selection, e.g. through keyboard
  //         	Can be returned in lieu of choiceID, depending on trigger source
  //       
  utils::SharedPtr<ISchemaItem> manualTextEntry_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["choiceID"] = CObjectSchemaItem::SMember(choiceID_SchemaItem, false);
  schema_members["manualTextEntry"] = CObjectSchemaItem::SMember(manualTextEntry_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_SetMediaClockTimer_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_ClockUpdateMode::eType> Common_ClockUpdateMode_all_enum_values;
  Common_ClockUpdateMode_all_enum_values.insert(Common_ClockUpdateMode::COUNTUP);
  Common_ClockUpdateMode_all_enum_values.insert(Common_ClockUpdateMode::COUNTDOWN);
  Common_ClockUpdateMode_all_enum_values.insert(Common_ClockUpdateMode::PAUSE);
  Common_ClockUpdateMode_all_enum_values.insert(Common_ClockUpdateMode::RESUME);
  Common_ClockUpdateMode_all_enum_values.insert(Common_ClockUpdateMode::CLEAR);

  // Function parameter startTime.
  //
  // startTime should be ignored for "PAUSE", "RESUME", and "CLEAR"
  utils::SharedPtr<ISchemaItem> startTime_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TimeFormat);

  // Function parameter endTime.
  //
  // 
  //           See TimeFormat.
  //       	  endTime can be provided for "COUNTUP" and "COUNTDOWN"; to be used to calculate any visual progress bar (if not provided, this feature is ignored)
  //       	  If endTime is greater then startTime for COUNTDOWN or less than startTime for COUNTUP, then the request will return an INVALID_DATA.
  //       	  endTime will be ignored for "PAUSE", "RESUME", and "CLEAR"
  //       
  utils::SharedPtr<ISchemaItem> endTime_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TimeFormat);

  // Function parameter updateMode.
  //
  // The update method of the media clock.
  // In case of pause, resume, or clear, the start time value is ignored and shall be left out.  For resume, the time continues with the same value as it was when paused.
  utils::SharedPtr<ISchemaItem> updateMode_SchemaItem = TEnumSchemaItem<Common_ClockUpdateMode::eType>::create(Common_ClockUpdateMode_all_enum_values, TSchemaItemParameter<Common_ClockUpdateMode::eType>());

  // Function parameter appID.
  //
  // ID of application that requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["startTime"] = CObjectSchemaItem::SMember(startTime_SchemaItem, false);
  schema_members["endTime"] = CObjectSchemaItem::SMember(endTime_SchemaItem, false);
  schema_members["updateMode"] = CObjectSchemaItem::SMember(updateMode_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_SetMediaClockTimer_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_SetGlobalProperties_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter vrHelpTitle.
  //
  // VR Help Title text.
  // If omitted on supported displays, the default HU system help title should be used.
  utils::SharedPtr<ISchemaItem> vrHelpTitle_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter vrHelp.
  //
  // VR Help Items. If omitted on supported displays, the default HU system generated help items should be used.
  utils::SharedPtr<ISchemaItem> vrHelp_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VrHelpItem), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter menuTitle.
  //
  // Optional text to label an app menu button (for certain touchscreen platforms).
  utils::SharedPtr<ISchemaItem> menuTitle_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter menuIcon.
  //
  // >Optional icon to draw on an app menu button (for certain touchscreen platforms).
  utils::SharedPtr<ISchemaItem> menuIcon_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  // Function parameter keyboardProperties.
  //
  // On-screen keybaord configuration (if available).
  utils::SharedPtr<ISchemaItem> keyboardProperties_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_KeyboardProperties);

  // Function parameter appID.
  //
  // ID of application that concerns this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["vrHelpTitle"] = CObjectSchemaItem::SMember(vrHelpTitle_SchemaItem, false);
  schema_members["vrHelp"] = CObjectSchemaItem::SMember(vrHelp_SchemaItem, false);
  schema_members["menuTitle"] = CObjectSchemaItem::SMember(menuTitle_SchemaItem, false);
  schema_members["menuIcon"] = CObjectSchemaItem::SMember(menuIcon_SchemaItem, false);
  schema_members["keyboardProperties"] = CObjectSchemaItem::SMember(keyboardProperties_SchemaItem, false);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_SetGlobalProperties_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_OnCommand_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter cmdID.
  //
  // Command ID, which is related to a specific menu entry (previously sent with AddCommand).
  utils::SharedPtr<ISchemaItem> cmdID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter appID.
  //
  // ID of application that is related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["cmdID"] = CObjectSchemaItem::SMember(cmdID_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_OnSystemContext_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_SystemContext::eType> Common_SystemContext_all_enum_values;
  Common_SystemContext_all_enum_values.insert(Common_SystemContext::SYSCTXT_MAIN);
  Common_SystemContext_all_enum_values.insert(Common_SystemContext::SYSCTXT_VRSESSION);
  Common_SystemContext_all_enum_values.insert(Common_SystemContext::SYSCTXT_MENU);
  Common_SystemContext_all_enum_values.insert(Common_SystemContext::SYSCTXT_HMI_OBSCURED);
  Common_SystemContext_all_enum_values.insert(Common_SystemContext::SYSCTXT_ALERT);

  // Function parameter systemContext.
  //
  // The context the application is brought into.
  utils::SharedPtr<ISchemaItem> systemContext_SchemaItem = TEnumSchemaItem<Common_SystemContext::eType>::create(Common_SystemContext_all_enum_values, TSchemaItemParameter<Common_SystemContext::eType>());

  // Function parameter appID.
  //
  // ID of application that is related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["systemContext"] = CObjectSchemaItem::SMember(systemContext_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_GetCapabilities_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_GetCapabilities_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_HmiZoneCapabilities::eType> Common_HmiZoneCapabilities_all_enum_values;
  Common_HmiZoneCapabilities_all_enum_values.insert(Common_HmiZoneCapabilities::FRONT);
  Common_HmiZoneCapabilities_all_enum_values.insert(Common_HmiZoneCapabilities::BACK);

  // Function parameter displayCapabilities.
  //
  // Information about the capabilities of the display: its type, text field supported, etc. See DisplayCapabilities. 
  utils::SharedPtr<ISchemaItem> displayCapabilities_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DisplayCapabilities);

  // Function parameter audioPassThruCapabilities.
  utils::SharedPtr<ISchemaItem> audioPassThruCapabilities_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_AudioPassThruCapabilities);

  // Function parameter hmiZoneCapabilities.
  utils::SharedPtr<ISchemaItem> hmiZoneCapabilities_SchemaItem = TEnumSchemaItem<Common_HmiZoneCapabilities::eType>::create(Common_HmiZoneCapabilities_all_enum_values, TSchemaItemParameter<Common_HmiZoneCapabilities::eType>());

  // Function parameter softButtonCapabilities.
  //
  // Must be returned if the platform supports on-screen SoftButtons.
  utils::SharedPtr<ISchemaItem> softButtonCapabilities_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SoftButtonCapabilities);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["displayCapabilities"] = CObjectSchemaItem::SMember(displayCapabilities_SchemaItem, true);
  schema_members["audioPassThruCapabilities"] = CObjectSchemaItem::SMember(audioPassThruCapabilities_SchemaItem, true);
  schema_members["hmiZoneCapabilities"] = CObjectSchemaItem::SMember(hmiZoneCapabilities_SchemaItem, true);
  schema_members["softButtonCapabilities"] = CObjectSchemaItem::SMember(softButtonCapabilities_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_ChangeRegistration_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter language.
  //
  // The language application wants to switch to.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  // Function parameter appID.
  //
  // ID of application that concerns this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_ChangeRegistration_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_OnLanguageChange_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter language.
  //
  // Language UI has switched to.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_GetSupportedLanguages_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_GetSupportedLanguages_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter languages.
  //
  // List of languages supported in UI.
  utils::SharedPtr<ISchemaItem> languages_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["languages"] = CObjectSchemaItem::SMember(languages_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_GetLanguage_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_GetLanguage_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter language.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_OnDriverDistraction_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_DriverDistractionState::eType> Common_DriverDistractionState_all_enum_values;
  Common_DriverDistractionState_all_enum_values.insert(Common_DriverDistractionState::DD_ON);
  Common_DriverDistractionState_all_enum_values.insert(Common_DriverDistractionState::DD_OFF);

  // Function parameter state.
  //
  // See DriverDistractionState. 
  utils::SharedPtr<ISchemaItem> state_SchemaItem = TEnumSchemaItem<Common_DriverDistractionState::eType>::create(Common_DriverDistractionState_all_enum_values, TSchemaItemParameter<Common_DriverDistractionState::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["state"] = CObjectSchemaItem::SMember(state_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_SetAppIcon_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter syncFileName.
  //
  // Either the path to the dynamic image stored on HY or the static binary image itself. See Image
  utils::SharedPtr<ISchemaItem> syncFileName_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["syncFileName"] = CObjectSchemaItem::SMember(syncFileName_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_SetAppIcon_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_SetDisplayLayout_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter displayLayout.
  //
  // 
  //       	Predefined or dynamically created screen layout.
  //       	Currently only predefined screen layouts are defined.
  //       
  utils::SharedPtr<ISchemaItem> displayLayout_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["displayLayout"] = CObjectSchemaItem::SMember(displayLayout_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_SetDisplayLayout_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter displayCapabilities.
  //
  // See DisplayCapabilities
  utils::SharedPtr<ISchemaItem> displayCapabilities_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DisplayCapabilities);

  // Function parameter buttonCapabilities.
  //
  // See ButtonCapabilities
  utils::SharedPtr<ISchemaItem> buttonCapabilities_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_ButtonCapabilities), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter softButtonCapabilities.
  //
  // If returned, the platform supports on-screen SoftButtons; see SoftButtonCapabilities.
  utils::SharedPtr<ISchemaItem> softButtonCapabilities_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SoftButtonCapabilities), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter presetBankCapabilities.
  //
  // If returned, the platform supports custom on-screen Presets; see PresetBankCapabilities.
  utils::SharedPtr<ISchemaItem> presetBankCapabilities_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_PresetBankCapabilities);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["displayCapabilities"] = CObjectSchemaItem::SMember(displayCapabilities_SchemaItem, false);
  schema_members["buttonCapabilities"] = CObjectSchemaItem::SMember(buttonCapabilities_SchemaItem, false);
  schema_members["softButtonCapabilities"] = CObjectSchemaItem::SMember(softButtonCapabilities_SchemaItem, false);
  schema_members["presetBankCapabilities"] = CObjectSchemaItem::SMember(presetBankCapabilities_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_ShowCustomForm_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter customFormID.
  //
  // 
  // 			Predefined or dynamically created screen layout.
  // 			Currently only predefined forms are defined.
  // 			Predefined layouts include:
  // 				"DEFAULT" - Default media / non-media screen
  // 				"ONSCREEN_PRESETS" - Custom root media screen containing app-defined onscreen presets.
  // 				"KEYBOARD_SEARCH" - Custom template containing app-configured on-screen keyboard with active search updating; user also can trigger voice search.
  // 				"NAV_FULLSCREEN_MAP" - Custom root template screen containing full screen map with navigation controls.
  // 				"NAV_POI_MENU" - Custom template containing app-defined POI options.
  // 				"NAV_SEARCH_RESULTS" - Custom template containing a list of app-defined search results
  // 				"NAV_POI_INFO" - Custom template containing app-defined POI information (and potentially map data).
  //       
  utils::SharedPtr<ISchemaItem> customFormID_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter parentFormID.
  //
  // 
  // 			Parent screen of predefined form to display.
  // 			Currently only predefined forms are defined.
  // 			If not provided, then set to "DEFAULT".
  // 			Predefined layouts include:
  // 				"DEFAULT" - Default media / non-media screen
  // 				"ONSCREEN_PRESETS" - Custom root media screen containing app-defined onscreen presets.
  // 				"NAV_FULLSCREEN_MAP" - Custom template containing full screen map with navigation controls.
  //       
  utils::SharedPtr<ISchemaItem> parentFormID_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>("DEFAULT"));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["customFormID"] = CObjectSchemaItem::SMember(customFormID_SchemaItem, true);
  schema_members["parentFormID"] = CObjectSchemaItem::SMember(parentFormID_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_ShowCustomForm_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_OnKeyboardInput_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_KeyboardEvent::eType> Common_KeyboardEvent_all_enum_values;
  Common_KeyboardEvent_all_enum_values.insert(Common_KeyboardEvent::KEYPRESS);
  Common_KeyboardEvent_all_enum_values.insert(Common_KeyboardEvent::ENTRY_SUBMITTED);
  Common_KeyboardEvent_all_enum_values.insert(Common_KeyboardEvent::ENTRY_CANCELLED);
  Common_KeyboardEvent_all_enum_values.insert(Common_KeyboardEvent::ENTRY_ABORTED);

  // Function parameter event.
  //
  // On-screen keyboard input data.
  utils::SharedPtr<ISchemaItem> event_SchemaItem = TEnumSchemaItem<Common_KeyboardEvent::eType>::create(Common_KeyboardEvent_all_enum_values, TSchemaItemParameter<Common_KeyboardEvent::eType>());

  // Function parameter data.
  //
  // On-screen keyboard input data.
  // For dynamic keypress events, this will be the current compounded string of entry text.
  // For entry submission events, this will be the full text entry (this will always return regardless of the mode).
  // For entry cancelled and entry aborted events, this data param will be omitted.
  utils::SharedPtr<ISchemaItem> data_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["event"] = CObjectSchemaItem::SMember(event_SchemaItem, true);
  schema_members["data"] = CObjectSchemaItem::SMember(data_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_OnTouchEvent_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_TouchType::eType> Common_TouchType_all_enum_values;
  Common_TouchType_all_enum_values.insert(Common_TouchType::BEGIN);
  Common_TouchType_all_enum_values.insert(Common_TouchType::MOVE);
  Common_TouchType_all_enum_values.insert(Common_TouchType::END);

  // Function parameter type.
  //
  // The type of touch event.
  utils::SharedPtr<ISchemaItem> type_SchemaItem = TEnumSchemaItem<Common_TouchType::eType>::create(Common_TouchType_all_enum_values, TSchemaItemParameter<Common_TouchType::eType>());

  // Function parameter event.
  //
  // List of all individual touches involved in this event.
  utils::SharedPtr<ISchemaItem> event_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TouchEvent), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(10));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["type"] = CObjectSchemaItem::SMember(type_SchemaItem, true);
  schema_members["event"] = CObjectSchemaItem::SMember(event_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_Slider_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter numTicks.
  //
  // Number of selectable items on a horizontal axis
  utils::SharedPtr<ISchemaItem> numTicks_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(2), TSchemaItemParameter<int>(26), TSchemaItemParameter<int>());

  // Function parameter position.
  //
  // Initial position of slider control (cannot exceed numTicks)
  utils::SharedPtr<ISchemaItem> position_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(26), TSchemaItemParameter<int>());

  // Function parameter sliderHeader.
  //
  // Text header to be displayed.
  utils::SharedPtr<ISchemaItem> sliderHeader_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter sliderFooter.
  //
  // Text footer to be displayed (meant to display min/max threshold descriptors).
  // For a static text footer, only one footer string shall be provided in the array.
  // For a dynamic text footer, the number of footer text string in the array must match the numTicks value.
  // For a dynamic text footer, text array string should correlate with potential slider position index.
  // If omitted on supported displays, no footer text shall be displayed.
  utils::SharedPtr<ISchemaItem> sliderFooter_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(26));

  // Function parameter timeout.
  //
  // Timeout. The slider should be displayed until the defined amount of time has elapsed. 
  utils::SharedPtr<ISchemaItem> timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1000), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Function parameter appID.
  //
  // ID of application that concerns this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["numTicks"] = CObjectSchemaItem::SMember(numTicks_SchemaItem, true);
  schema_members["position"] = CObjectSchemaItem::SMember(position_SchemaItem, true);
  schema_members["sliderHeader"] = CObjectSchemaItem::SMember(sliderHeader_SchemaItem, true);
  schema_members["sliderFooter"] = CObjectSchemaItem::SMember(sliderFooter_SchemaItem, false);
  schema_members["timeout"] = CObjectSchemaItem::SMember(timeout_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_Slider_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter sliderPosition.
  //
  // Current slider position. Must be returned when the user has clicked the ‘Save’ or ‘Canceled’ button or by the timeout 
  utils::SharedPtr<ISchemaItem> sliderPosition_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(26), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["sliderPosition"] = CObjectSchemaItem::SMember(sliderPosition_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_ScrollableMessage_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter messageText.
  //
  // Body of text that can include newlines and tabs. Uses scrollableMessageBody.
  utils::SharedPtr<ISchemaItem> messageText_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TextFieldStruct);

  // Function parameter timeout.
  //
  // Timeout in milliseconds. The message should be displayed until the time defined is up.
  utils::SharedPtr<ISchemaItem> timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Function parameter softButtons.
  //
  // App defined SoftButtons.
  // If omitted on supported displays, only the system defined "Close" SoftButton should be displayed.
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(8));

  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["messageText"] = CObjectSchemaItem::SMember(messageText_SchemaItem, true);
  schema_members["timeout"] = CObjectSchemaItem::SMember(timeout_SchemaItem, true);
  schema_members["softButtons"] = CObjectSchemaItem::SMember(softButtons_SchemaItem, false);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_ScrollableMessage_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_PerformAudioPassThru_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Function parameter audioPassThruDisplayTexts.
  //
  // Uses
  //             audioPassThruDisplayText1: First line of text displayed during audio capture.
  //             audioPassThruDisplayText2: Second line of text displayed during audio capture.
  utils::SharedPtr<ISchemaItem> audioPassThruDisplayTexts_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TextFieldStruct), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(2));

  // Function parameter maxDuration.
  //
  // The maximum duration of audio recording in milliseconds. If not provided, the recording should be performed until EndAudioPassThru arrives.
  utils::SharedPtr<ISchemaItem> maxDuration_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(1000000), TSchemaItemParameter<int>());

  // Function parameter muteAudio.
  //
  // 
  //         Defines if the current audio source should be muted during the APT session. If not, the audio source will play without interruption.
  //         If omitted, the value is set to true.
  //       
  utils::SharedPtr<ISchemaItem> muteAudio_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);
  schema_members["audioPassThruDisplayTexts"] = CObjectSchemaItem::SMember(audioPassThruDisplayTexts_SchemaItem, true);
  schema_members["maxDuration"] = CObjectSchemaItem::SMember(maxDuration_SchemaItem, true);
  schema_members["muteAudio"] = CObjectSchemaItem::SMember(muteAudio_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_PerformAudioPassThru_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_EndAudioPassThru_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_EndAudioPassThru_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_IsReady_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_IsReady_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter available.
  //
  // Must be true if UI is present and ready to communicate with SDL.
  utils::SharedPtr<ISchemaItem> available_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["available"] = CObjectSchemaItem::SMember(available_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_ClosePopUp_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter methodName.
  //
  // Method to be closed
  utils::SharedPtr<ISchemaItem> methodName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["methodName"] = CObjectSchemaItem::SMember(methodName_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_ClosePopUp_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_OnResetTimeout_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  //
  // Id of application that invoked notifcation.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Function parameter methodName.
  //
  // Currently used method name on which was triggered action
  utils::SharedPtr<ISchemaItem> methodName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);
  schema_members["methodName"] = CObjectSchemaItem::SMember(methodName_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_UI_OnRecordStart_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_IsReady_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_IsReady_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter available.
  //
  // Must be true if Navigation is present and ready to communicate with SDL.
  utils::SharedPtr<ISchemaItem> available_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["available"] = CObjectSchemaItem::SMember(available_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_ShowConstantTBT_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter navigationTexts.
  //
  // See TextFieldStruct. Uses:
  //         navigationText1
  //         navigationText2
  //         ETA
  //         totalDistance
  //         timeToDestination.
  //       
  utils::SharedPtr<ISchemaItem> navigationTexts_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TextFieldStruct), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(5));

  // Function parameter turnIcon.
  utils::SharedPtr<ISchemaItem> turnIcon_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  // Function parameter nextTurnIcon.
  utils::SharedPtr<ISchemaItem> nextTurnIcon_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  // Function parameter distanceToManeuver.
  //
  // Fraction of distance till next maneuver (from previous maneuver).
  // May be used to calculate progress bar.
  utils::SharedPtr<ISchemaItem> distanceToManeuver_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(1000000000.0), TSchemaItemParameter<double>());

  // Function parameter distanceToManeuverScale.
  //
  // Fraction of distance till next maneuver (starting from when AlertManeuver is triggered).
  // May be used to calculate progress bar.
  utils::SharedPtr<ISchemaItem> distanceToManeuverScale_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(1000000000.0), TSchemaItemParameter<double>());

  // Function parameter maneuverComplete.
  //
  // If and when a maneuver has completed while an AlertManeuver is active, SDL will send this value set to TRUE in order to clear the AlertManeuver overlay.
  // If omitted the value should be assumed as FALSE.
  utils::SharedPtr<ISchemaItem> maneuverComplete_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter softButtons.
  //
  // Three dynamic SoftButtons available
  // If omitted on supported displays, the currently displayed SoftButton values will not change.
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(3));

  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["navigationTexts"] = CObjectSchemaItem::SMember(navigationTexts_SchemaItem, true);
  schema_members["turnIcon"] = CObjectSchemaItem::SMember(turnIcon_SchemaItem, false);
  schema_members["nextTurnIcon"] = CObjectSchemaItem::SMember(nextTurnIcon_SchemaItem, false);
  schema_members["distanceToManeuver"] = CObjectSchemaItem::SMember(distanceToManeuver_SchemaItem, true);
  schema_members["distanceToManeuverScale"] = CObjectSchemaItem::SMember(distanceToManeuverScale_SchemaItem, true);
  schema_members["maneuverComplete"] = CObjectSchemaItem::SMember(maneuverComplete_SchemaItem, false);
  schema_members["softButtons"] = CObjectSchemaItem::SMember(softButtons_SchemaItem, false);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_ShowConstantTBT_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_AlertManeuver_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter softButtons.
  //
  // If omitted, only the system defined "Close" SoftButton should be displayed.
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(3));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["softButtons"] = CObjectSchemaItem::SMember(softButtons_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_AlertManeuver_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_UpdateTurnList_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter turnList.
  utils::SharedPtr<ISchemaItem> turnList_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Turn), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter softButtons.
  //
  // If omitted, app-defined SoftButton should be left blank.
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1));

  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["turnList"] = CObjectSchemaItem::SMember(turnList_SchemaItem, false);
  schema_members["softButtons"] = CObjectSchemaItem::SMember(softButtons_SchemaItem, false);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_UpdateTurnList_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_OnTBTClientState_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_TBTState::eType> Common_TBTState_all_enum_values;
  Common_TBTState_all_enum_values.insert(Common_TBTState::ROUTE_UPDATE_REQUEST);
  Common_TBTState_all_enum_values.insert(Common_TBTState::ROUTE_ACCEPTED);
  Common_TBTState_all_enum_values.insert(Common_TBTState::ROUTE_REFUSED);
  Common_TBTState_all_enum_values.insert(Common_TBTState::ROUTE_CANCELLED);
  Common_TBTState_all_enum_values.insert(Common_TBTState::ETA_REQUEST);
  Common_TBTState_all_enum_values.insert(Common_TBTState::NEXT_TURN_REQUEST);
  Common_TBTState_all_enum_values.insert(Common_TBTState::ROUTE_STATUS_REQUEST);
  Common_TBTState_all_enum_values.insert(Common_TBTState::ROUTE_SUMMARY_REQUEST);
  Common_TBTState_all_enum_values.insert(Common_TBTState::TRIP_STATUS_REQUEST);
  Common_TBTState_all_enum_values.insert(Common_TBTState::ROUTE_UPDATE_REQUEST_TIMEOUT);

  // Function parameter state.
  //
  // Current State of TBT client
  utils::SharedPtr<ISchemaItem> state_SchemaItem = TEnumSchemaItem<Common_TBTState::eType>::create(Common_TBTState_all_enum_values, TSchemaItemParameter<Common_TBTState::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["state"] = CObjectSchemaItem::SMember(state_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_StartStream_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter url.
  //
  // URL that HMI start playing.
  utils::SharedPtr<ISchemaItem> url_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(21), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["url"] = CObjectSchemaItem::SMember(url_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_StartStream_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_StopStream_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_StopStream_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_StartAudioStream_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter url.
  //
  // URL that HMI start playing.
  utils::SharedPtr<ISchemaItem> url_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(21), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["url"] = CObjectSchemaItem::SMember(url_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_StartAudioStream_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_StopAudioStream_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_Navigation_StopAudioStream_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_IsReady_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_IsReady_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter available.
  //
  // Must be true if vehicle data modules are present and ready to communicate with SDL.
  utils::SharedPtr<ISchemaItem> available_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["available"] = CObjectSchemaItem::SMember(available_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_GetVehicleType_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_GetVehicleType_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter vehicleType.
  utils::SharedPtr<ISchemaItem> vehicleType_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleType);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["vehicleType"] = CObjectSchemaItem::SMember(vehicleType_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_ReadDID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter ecuName.
  //
  // Name of ECU.
  utils::SharedPtr<ISchemaItem> ecuName_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Function parameter didLocation.
  //
  // Get raw data from vehicle data DID location(s).
  utils::SharedPtr<ISchemaItem> didLocation_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000));

  // Function parameter appID.
  //
  // ID of application related to this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["ecuName"] = CObjectSchemaItem::SMember(ecuName_SchemaItem, true);
  schema_members["didLocation"] = CObjectSchemaItem::SMember(didLocation_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_ReadDID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter didResult.
  //
  // Array of requested DID results (with data if available).
  utils::SharedPtr<ISchemaItem> didResult_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DIDResult), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["didResult"] = CObjectSchemaItem::SMember(didResult_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_GetDTCs_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter ecuName.
  //
  // Name of ECU.
  utils::SharedPtr<ISchemaItem> ecuName_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Function parameter dtcMask.
  //
  // DTC Mask Byte to be sent in diagnostic request to module .
  utils::SharedPtr<ISchemaItem> dtcMask_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(255), TSchemaItemParameter<int>());

  // Function parameter appID.
  //
  // ID of application that requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["ecuName"] = CObjectSchemaItem::SMember(ecuName_SchemaItem, true);
  schema_members["dtcMask"] = CObjectSchemaItem::SMember(dtcMask_SchemaItem, false);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_GetDTCs_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter ecuHeader.
  //
  // 2 byte ECU Header for DTC response (as defined in VHR_Layout_Specification_DTCs.pdf)
  utils::SharedPtr<ISchemaItem> ecuHeader_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Function parameter dtc.
  //
  // 
  //         Array of all reported DTCs on module. Each DTC is represented with 4 bytes:
  //         3 bytes for data
  //         1 byte for status
  //       
  utils::SharedPtr<ISchemaItem> dtc_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(10), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(15));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["ecuHeader"] = CObjectSchemaItem::SMember(ecuHeader_SchemaItem, true);
  schema_members["dtc"] = CObjectSchemaItem::SMember(dtc_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_DiagnosticMessage_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter targetID.
  //
  // Name of target ECU.
  utils::SharedPtr<ISchemaItem> targetID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Function parameter messageLength.
  //
  // Length of message (in bytes).
  utils::SharedPtr<ISchemaItem> messageLength_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Function parameter messageData.
  //
  // 
  //       	Array of bytes comprising CAN message.
  //       
  utils::SharedPtr<ISchemaItem> messageData_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(255), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(65535));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["targetID"] = CObjectSchemaItem::SMember(targetID_SchemaItem, true);
  schema_members["messageLength"] = CObjectSchemaItem::SMember(messageLength_SchemaItem, true);
  schema_members["messageData"] = CObjectSchemaItem::SMember(messageData_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_DiagnosticMessage_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter messageDataResult.
  //
  // 
  //       	Array of bytes comprising CAN message result.
  //       
  utils::SharedPtr<ISchemaItem> messageDataResult_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(255), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(65535));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["messageDataResult"] = CObjectSchemaItem::SMember(messageDataResult_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_SubscribeVehicleData_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  //
  // ID of application that requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Function parameter gps.
  //
  // See GPSData
  utils::SharedPtr<ISchemaItem> gps_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter speed.
  //
  // The vehicle speed in kilometers per hour
  utils::SharedPtr<ISchemaItem> speed_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter rpm.
  //
  // The number of revolutions per minute of the engine
  utils::SharedPtr<ISchemaItem> rpm_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter fuelLevel.
  //
  // The fuel level in the tank (percentage)
  utils::SharedPtr<ISchemaItem> fuelLevel_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter fuelLevel_State.
  //
  // The fuel level state
  utils::SharedPtr<ISchemaItem> fuelLevel_State_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter instantFuelConsumption.
  //
  // The instantaneous fuel consumption in microlitres
  utils::SharedPtr<ISchemaItem> instantFuelConsumption_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter externalTemperature.
  //
  // The external temperature in degrees celsius
  utils::SharedPtr<ISchemaItem> externalTemperature_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter prndl.
  //
  // See PRNDL
  utils::SharedPtr<ISchemaItem> prndl_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter tirePressure.
  //
  // See TireStatus
  utils::SharedPtr<ISchemaItem> tirePressure_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter odometer.
  //
  // Odometer in km
  utils::SharedPtr<ISchemaItem> odometer_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter beltStatus.
  //
  // The status of the seat belts
  utils::SharedPtr<ISchemaItem> beltStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter bodyInformation.
  //
  // The body information including power modes
  utils::SharedPtr<ISchemaItem> bodyInformation_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter deviceStatus.
  //
  // The device status including signal and battery strength
  utils::SharedPtr<ISchemaItem> deviceStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter driverBraking.
  //
  // The status of the brake pedal
  utils::SharedPtr<ISchemaItem> driverBraking_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter wiperStatus.
  //
  // The status of the wipers
  utils::SharedPtr<ISchemaItem> wiperStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter headLampStatus.
  //
  // Status of the head lamps
  utils::SharedPtr<ISchemaItem> headLampStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter engineTorque.
  //
  // Torque value for engine (in Nm) on non-diesel variants
  utils::SharedPtr<ISchemaItem> engineTorque_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter accPedalPosition.
  //
  // Accelerator pedal position (percentage depressed)
  utils::SharedPtr<ISchemaItem> accPedalPosition_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter steeringWheelAngle.
  //
  // Current angle of the steering wheel (in deg)
  utils::SharedPtr<ISchemaItem> steeringWheelAngle_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter eCallInfo.
  //
  // Emergency Call notification and confirmation data
  utils::SharedPtr<ISchemaItem> eCallInfo_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter airbagStatus.
  //
  // The status of the air bags
  utils::SharedPtr<ISchemaItem> airbagStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter emergencyEvent.
  //
  // Information related to an emergency event (and if it occurred)
  utils::SharedPtr<ISchemaItem> emergencyEvent_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter clusterModeStatus.
  //
  // The status modes of the cluster
  utils::SharedPtr<ISchemaItem> clusterModeStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter myKey.
  //
  // Information related to the MyKey feature
  utils::SharedPtr<ISchemaItem> myKey_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);
  schema_members["gps"] = CObjectSchemaItem::SMember(gps_SchemaItem, false);
  schema_members["speed"] = CObjectSchemaItem::SMember(speed_SchemaItem, false);
  schema_members["rpm"] = CObjectSchemaItem::SMember(rpm_SchemaItem, false);
  schema_members["fuelLevel"] = CObjectSchemaItem::SMember(fuelLevel_SchemaItem, false);
  schema_members["fuelLevel_State"] = CObjectSchemaItem::SMember(fuelLevel_State_SchemaItem, false);
  schema_members["instantFuelConsumption"] = CObjectSchemaItem::SMember(instantFuelConsumption_SchemaItem, false);
  schema_members["externalTemperature"] = CObjectSchemaItem::SMember(externalTemperature_SchemaItem, false);
  schema_members["prndl"] = CObjectSchemaItem::SMember(prndl_SchemaItem, false);
  schema_members["tirePressure"] = CObjectSchemaItem::SMember(tirePressure_SchemaItem, false);
  schema_members["odometer"] = CObjectSchemaItem::SMember(odometer_SchemaItem, false);
  schema_members["beltStatus"] = CObjectSchemaItem::SMember(beltStatus_SchemaItem, false);
  schema_members["bodyInformation"] = CObjectSchemaItem::SMember(bodyInformation_SchemaItem, false);
  schema_members["deviceStatus"] = CObjectSchemaItem::SMember(deviceStatus_SchemaItem, false);
  schema_members["driverBraking"] = CObjectSchemaItem::SMember(driverBraking_SchemaItem, false);
  schema_members["wiperStatus"] = CObjectSchemaItem::SMember(wiperStatus_SchemaItem, false);
  schema_members["headLampStatus"] = CObjectSchemaItem::SMember(headLampStatus_SchemaItem, false);
  schema_members["engineTorque"] = CObjectSchemaItem::SMember(engineTorque_SchemaItem, false);
  schema_members["accPedalPosition"] = CObjectSchemaItem::SMember(accPedalPosition_SchemaItem, false);
  schema_members["steeringWheelAngle"] = CObjectSchemaItem::SMember(steeringWheelAngle_SchemaItem, false);
  schema_members["eCallInfo"] = CObjectSchemaItem::SMember(eCallInfo_SchemaItem, false);
  schema_members["airbagStatus"] = CObjectSchemaItem::SMember(airbagStatus_SchemaItem, false);
  schema_members["emergencyEvent"] = CObjectSchemaItem::SMember(emergencyEvent_SchemaItem, false);
  schema_members["clusterModeStatus"] = CObjectSchemaItem::SMember(clusterModeStatus_SchemaItem, false);
  schema_members["myKey"] = CObjectSchemaItem::SMember(myKey_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_SubscribeVehicleData_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter gps.
  //
  // See GPSData
  utils::SharedPtr<ISchemaItem> gps_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter speed.
  //
  // The vehicle speed in kilometers per hour
  utils::SharedPtr<ISchemaItem> speed_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter rpm.
  //
  // The number of revolutions per minute of the engine
  utils::SharedPtr<ISchemaItem> rpm_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter fuelLevel.
  //
  // The fuel level in the tank (percentage)
  utils::SharedPtr<ISchemaItem> fuelLevel_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter fuelLevel_State.
  //
  // The fuel level state
  utils::SharedPtr<ISchemaItem> fuelLevel_State_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter instantFuelConsumption.
  //
  // The instantaneous fuel consumption in microlitres
  utils::SharedPtr<ISchemaItem> instantFuelConsumption_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter externalTemperature.
  //
  // The external temperature in degrees celsius.
  utils::SharedPtr<ISchemaItem> externalTemperature_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter prndl.
  //
  // See PRNDL
  utils::SharedPtr<ISchemaItem> prndl_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter tirePressure.
  //
  // See TireStatus
  utils::SharedPtr<ISchemaItem> tirePressure_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter odometer.
  //
  // Odometer in km
  utils::SharedPtr<ISchemaItem> odometer_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter beltStatus.
  //
  // The status of the seat belts
  utils::SharedPtr<ISchemaItem> beltStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter bodyInformation.
  //
  // The body information including power modes
  utils::SharedPtr<ISchemaItem> bodyInformation_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter deviceStatus.
  //
  // The device status including signal and battery strength
  utils::SharedPtr<ISchemaItem> deviceStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter driverBraking.
  //
  // The status of the brake pedal
  utils::SharedPtr<ISchemaItem> driverBraking_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter wiperStatus.
  //
  // The status of the wipers
  utils::SharedPtr<ISchemaItem> wiperStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter headLampStatus.
  //
  // Status of the head lamps
  utils::SharedPtr<ISchemaItem> headLampStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter engineTorque.
  //
  // Torque value for engine (in Nm) on non-diesel variants
  utils::SharedPtr<ISchemaItem> engineTorque_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter accPedalPosition.
  //
  // Accelerator pedal position (percentage depressed)
  utils::SharedPtr<ISchemaItem> accPedalPosition_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter steeringWheelAngle.
  //
  // Current angle of the steering wheel (in deg)
  utils::SharedPtr<ISchemaItem> steeringWheelAngle_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter eCallInfo.
  //
  // Emergency Call notification and confirmation data
  utils::SharedPtr<ISchemaItem> eCallInfo_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter airbagStatus.
  //
  // The status of the air bags
  utils::SharedPtr<ISchemaItem> airbagStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter emergencyEvent.
  //
  // Information related to an emergency event (and if it occurred)
  utils::SharedPtr<ISchemaItem> emergencyEvent_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter clusterModes.
  //
  // The status modes of the cluster
  utils::SharedPtr<ISchemaItem> clusterModes_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter myKey.
  //
  // Information related to the MyKey feature
  utils::SharedPtr<ISchemaItem> myKey_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["gps"] = CObjectSchemaItem::SMember(gps_SchemaItem, false);
  schema_members["speed"] = CObjectSchemaItem::SMember(speed_SchemaItem, false);
  schema_members["rpm"] = CObjectSchemaItem::SMember(rpm_SchemaItem, false);
  schema_members["fuelLevel"] = CObjectSchemaItem::SMember(fuelLevel_SchemaItem, false);
  schema_members["fuelLevel_State"] = CObjectSchemaItem::SMember(fuelLevel_State_SchemaItem, false);
  schema_members["instantFuelConsumption"] = CObjectSchemaItem::SMember(instantFuelConsumption_SchemaItem, false);
  schema_members["externalTemperature"] = CObjectSchemaItem::SMember(externalTemperature_SchemaItem, false);
  schema_members["prndl"] = CObjectSchemaItem::SMember(prndl_SchemaItem, false);
  schema_members["tirePressure"] = CObjectSchemaItem::SMember(tirePressure_SchemaItem, false);
  schema_members["odometer"] = CObjectSchemaItem::SMember(odometer_SchemaItem, false);
  schema_members["beltStatus"] = CObjectSchemaItem::SMember(beltStatus_SchemaItem, false);
  schema_members["bodyInformation"] = CObjectSchemaItem::SMember(bodyInformation_SchemaItem, false);
  schema_members["deviceStatus"] = CObjectSchemaItem::SMember(deviceStatus_SchemaItem, false);
  schema_members["driverBraking"] = CObjectSchemaItem::SMember(driverBraking_SchemaItem, false);
  schema_members["wiperStatus"] = CObjectSchemaItem::SMember(wiperStatus_SchemaItem, false);
  schema_members["headLampStatus"] = CObjectSchemaItem::SMember(headLampStatus_SchemaItem, false);
  schema_members["engineTorque"] = CObjectSchemaItem::SMember(engineTorque_SchemaItem, false);
  schema_members["accPedalPosition"] = CObjectSchemaItem::SMember(accPedalPosition_SchemaItem, false);
  schema_members["steeringWheelAngle"] = CObjectSchemaItem::SMember(steeringWheelAngle_SchemaItem, false);
  schema_members["eCallInfo"] = CObjectSchemaItem::SMember(eCallInfo_SchemaItem, false);
  schema_members["airbagStatus"] = CObjectSchemaItem::SMember(airbagStatus_SchemaItem, false);
  schema_members["emergencyEvent"] = CObjectSchemaItem::SMember(emergencyEvent_SchemaItem, false);
  schema_members["clusterModes"] = CObjectSchemaItem::SMember(clusterModes_SchemaItem, false);
  schema_members["myKey"] = CObjectSchemaItem::SMember(myKey_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_UnsubscribeVehicleData_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  //
  // ID of application that requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Function parameter gps.
  //
  // See GPSData
  utils::SharedPtr<ISchemaItem> gps_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter speed.
  //
  // The vehicle speed in kilometers per hour
  utils::SharedPtr<ISchemaItem> speed_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter rpm.
  //
  // The number of revolutions per minute of the engine
  utils::SharedPtr<ISchemaItem> rpm_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter fuelLevel.
  //
  // The fuel level in the tank (percentage)
  utils::SharedPtr<ISchemaItem> fuelLevel_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter fuelLevel_State.
  //
  // The fuel level state
  utils::SharedPtr<ISchemaItem> fuelLevel_State_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter instantFuelConsumption.
  //
  // The instantaneous fuel consumption in microlitres
  utils::SharedPtr<ISchemaItem> instantFuelConsumption_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter externalTemperature.
  //
  // The external temperature in degrees celsius.
  utils::SharedPtr<ISchemaItem> externalTemperature_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter prndl.
  //
  // See PRNDL
  utils::SharedPtr<ISchemaItem> prndl_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter tirePressure.
  //
  // See TireStatus
  utils::SharedPtr<ISchemaItem> tirePressure_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter odometer.
  //
  // Odometer in km
  utils::SharedPtr<ISchemaItem> odometer_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter beltStatus.
  //
  // The status of the seat belts
  utils::SharedPtr<ISchemaItem> beltStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter bodyInformation.
  //
  // The body information including power modes
  utils::SharedPtr<ISchemaItem> bodyInformation_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter deviceStatus.
  //
  // The device status including signal and battery strength
  utils::SharedPtr<ISchemaItem> deviceStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter driverBraking.
  //
  // The status of the brake pedal
  utils::SharedPtr<ISchemaItem> driverBraking_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter wiperStatus.
  //
  // The status of the wipers
  utils::SharedPtr<ISchemaItem> wiperStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter headLampStatus.
  //
  // Status of the head lamps
  utils::SharedPtr<ISchemaItem> headLampStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter engineTorque.
  //
  // Torque value for engine (in Nm) on non-diesel variants
  utils::SharedPtr<ISchemaItem> engineTorque_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter accPedalPosition.
  //
  // Accelerator pedal position (percentage depressed)
  utils::SharedPtr<ISchemaItem> accPedalPosition_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter steeringWheelAngle.
  //
  // Current angle of the steering wheel (in deg)
  utils::SharedPtr<ISchemaItem> steeringWheelAngle_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter eCallInfo.
  //
  // Emergency Call notification and confirmation data
  utils::SharedPtr<ISchemaItem> eCallInfo_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter airbagStatus.
  //
  // The status of the air bags
  utils::SharedPtr<ISchemaItem> airbagStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter emergencyEvent.
  //
  // Information related to an emergency event (and if it occurred)
  utils::SharedPtr<ISchemaItem> emergencyEvent_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter clusterModeStatus.
  //
  // The status modes of the cluster
  utils::SharedPtr<ISchemaItem> clusterModeStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter myKey.
  //
  // Information related to the MyKey feature
  utils::SharedPtr<ISchemaItem> myKey_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);
  schema_members["gps"] = CObjectSchemaItem::SMember(gps_SchemaItem, false);
  schema_members["speed"] = CObjectSchemaItem::SMember(speed_SchemaItem, false);
  schema_members["rpm"] = CObjectSchemaItem::SMember(rpm_SchemaItem, false);
  schema_members["fuelLevel"] = CObjectSchemaItem::SMember(fuelLevel_SchemaItem, false);
  schema_members["fuelLevel_State"] = CObjectSchemaItem::SMember(fuelLevel_State_SchemaItem, false);
  schema_members["instantFuelConsumption"] = CObjectSchemaItem::SMember(instantFuelConsumption_SchemaItem, false);
  schema_members["externalTemperature"] = CObjectSchemaItem::SMember(externalTemperature_SchemaItem, false);
  schema_members["prndl"] = CObjectSchemaItem::SMember(prndl_SchemaItem, false);
  schema_members["tirePressure"] = CObjectSchemaItem::SMember(tirePressure_SchemaItem, false);
  schema_members["odometer"] = CObjectSchemaItem::SMember(odometer_SchemaItem, false);
  schema_members["beltStatus"] = CObjectSchemaItem::SMember(beltStatus_SchemaItem, false);
  schema_members["bodyInformation"] = CObjectSchemaItem::SMember(bodyInformation_SchemaItem, false);
  schema_members["deviceStatus"] = CObjectSchemaItem::SMember(deviceStatus_SchemaItem, false);
  schema_members["driverBraking"] = CObjectSchemaItem::SMember(driverBraking_SchemaItem, false);
  schema_members["wiperStatus"] = CObjectSchemaItem::SMember(wiperStatus_SchemaItem, false);
  schema_members["headLampStatus"] = CObjectSchemaItem::SMember(headLampStatus_SchemaItem, false);
  schema_members["engineTorque"] = CObjectSchemaItem::SMember(engineTorque_SchemaItem, false);
  schema_members["accPedalPosition"] = CObjectSchemaItem::SMember(accPedalPosition_SchemaItem, false);
  schema_members["steeringWheelAngle"] = CObjectSchemaItem::SMember(steeringWheelAngle_SchemaItem, false);
  schema_members["eCallInfo"] = CObjectSchemaItem::SMember(eCallInfo_SchemaItem, false);
  schema_members["airbagStatus"] = CObjectSchemaItem::SMember(airbagStatus_SchemaItem, false);
  schema_members["emergencyEvent"] = CObjectSchemaItem::SMember(emergencyEvent_SchemaItem, false);
  schema_members["clusterModeStatus"] = CObjectSchemaItem::SMember(clusterModeStatus_SchemaItem, false);
  schema_members["myKey"] = CObjectSchemaItem::SMember(myKey_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_UnsubscribeVehicleData_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter gps.
  //
  // See GPSData
  utils::SharedPtr<ISchemaItem> gps_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter speed.
  //
  // The vehicle speed in kilometers per hour
  utils::SharedPtr<ISchemaItem> speed_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter rpm.
  //
  // The number of revolutions per minute of the engine
  utils::SharedPtr<ISchemaItem> rpm_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter fuelLevel.
  //
  // The fuel level in the tank (percentage)
  utils::SharedPtr<ISchemaItem> fuelLevel_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter fuelLevel_State.
  //
  // The fuel level state
  utils::SharedPtr<ISchemaItem> fuelLevel_State_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter instantFuelConsumption.
  //
  // The instantaneous fuel consumption in microlitres
  utils::SharedPtr<ISchemaItem> instantFuelConsumption_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter externalTemperature.
  //
  // The external temperature in degrees celsius
  utils::SharedPtr<ISchemaItem> externalTemperature_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter prndl.
  //
  // See PRNDL
  utils::SharedPtr<ISchemaItem> prndl_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter tirePressure.
  //
  // See TireStatus
  utils::SharedPtr<ISchemaItem> tirePressure_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter odometer.
  //
  // Odometer in km
  utils::SharedPtr<ISchemaItem> odometer_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter beltStatus.
  //
  // The status of the seat belts
  utils::SharedPtr<ISchemaItem> beltStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter bodyInformation.
  //
  // The body information including power modes
  utils::SharedPtr<ISchemaItem> bodyInformation_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter deviceStatus.
  //
  // The device status including signal and battery strength
  utils::SharedPtr<ISchemaItem> deviceStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter driverBraking.
  //
  // The status of the brake pedal
  utils::SharedPtr<ISchemaItem> driverBraking_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter wiperStatus.
  //
  // The status of the wipers
  utils::SharedPtr<ISchemaItem> wiperStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter headLampStatus.
  //
  // Status of the head lamps
  utils::SharedPtr<ISchemaItem> headLampStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter engineTorque.
  //
  // Torque value for engine (in Nm) on non-diesel variants
  utils::SharedPtr<ISchemaItem> engineTorque_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter accPedalPosition.
  //
  // Accelerator pedal position (percentage depressed)
  utils::SharedPtr<ISchemaItem> accPedalPosition_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter steeringWheelAngle.
  //
  // Current angle of the steering wheel (in deg)
  utils::SharedPtr<ISchemaItem> steeringWheelAngle_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter eCallInfo.
  //
  // Emergency Call notification and confirmation data
  utils::SharedPtr<ISchemaItem> eCallInfo_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter airbagStatus.
  //
  // The status of the air bags
  utils::SharedPtr<ISchemaItem> airbagStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter emergencyEvent.
  //
  // Information related to an emergency event (and if it occurred)
  utils::SharedPtr<ISchemaItem> emergencyEvent_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter clusterModes.
  //
  // The status modes of the cluster
  utils::SharedPtr<ISchemaItem> clusterModes_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  // Function parameter myKey.
  //
  // Information related to the MyKey feature
  utils::SharedPtr<ISchemaItem> myKey_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_VehicleDataResult);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["gps"] = CObjectSchemaItem::SMember(gps_SchemaItem, false);
  schema_members["speed"] = CObjectSchemaItem::SMember(speed_SchemaItem, false);
  schema_members["rpm"] = CObjectSchemaItem::SMember(rpm_SchemaItem, false);
  schema_members["fuelLevel"] = CObjectSchemaItem::SMember(fuelLevel_SchemaItem, false);
  schema_members["fuelLevel_State"] = CObjectSchemaItem::SMember(fuelLevel_State_SchemaItem, false);
  schema_members["instantFuelConsumption"] = CObjectSchemaItem::SMember(instantFuelConsumption_SchemaItem, false);
  schema_members["externalTemperature"] = CObjectSchemaItem::SMember(externalTemperature_SchemaItem, false);
  schema_members["prndl"] = CObjectSchemaItem::SMember(prndl_SchemaItem, false);
  schema_members["tirePressure"] = CObjectSchemaItem::SMember(tirePressure_SchemaItem, false);
  schema_members["odometer"] = CObjectSchemaItem::SMember(odometer_SchemaItem, false);
  schema_members["beltStatus"] = CObjectSchemaItem::SMember(beltStatus_SchemaItem, false);
  schema_members["bodyInformation"] = CObjectSchemaItem::SMember(bodyInformation_SchemaItem, false);
  schema_members["deviceStatus"] = CObjectSchemaItem::SMember(deviceStatus_SchemaItem, false);
  schema_members["driverBraking"] = CObjectSchemaItem::SMember(driverBraking_SchemaItem, false);
  schema_members["wiperStatus"] = CObjectSchemaItem::SMember(wiperStatus_SchemaItem, false);
  schema_members["headLampStatus"] = CObjectSchemaItem::SMember(headLampStatus_SchemaItem, false);
  schema_members["engineTorque"] = CObjectSchemaItem::SMember(engineTorque_SchemaItem, false);
  schema_members["accPedalPosition"] = CObjectSchemaItem::SMember(accPedalPosition_SchemaItem, false);
  schema_members["steeringWheelAngle"] = CObjectSchemaItem::SMember(steeringWheelAngle_SchemaItem, false);
  schema_members["eCallInfo"] = CObjectSchemaItem::SMember(eCallInfo_SchemaItem, false);
  schema_members["airbagStatus"] = CObjectSchemaItem::SMember(airbagStatus_SchemaItem, false);
  schema_members["emergencyEvent"] = CObjectSchemaItem::SMember(emergencyEvent_SchemaItem, false);
  schema_members["clusterModes"] = CObjectSchemaItem::SMember(clusterModes_SchemaItem, false);
  schema_members["myKey"] = CObjectSchemaItem::SMember(myKey_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_GetVehicleData_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter gps.
  //
  // See GPSData
  utils::SharedPtr<ISchemaItem> gps_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter speed.
  //
  // The vehicle speed in kilometers per hour
  utils::SharedPtr<ISchemaItem> speed_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter rpm.
  //
  // The number of revolutions per minute of the engine
  utils::SharedPtr<ISchemaItem> rpm_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter fuelLevel.
  //
  // The fuel level in the tank (percentage)
  utils::SharedPtr<ISchemaItem> fuelLevel_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter fuelLevel_State.
  //
  // The fuel level state
  utils::SharedPtr<ISchemaItem> fuelLevel_State_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter instantFuelConsumption.
  //
  // The instantaneous fuel consumption in microlitres
  utils::SharedPtr<ISchemaItem> instantFuelConsumption_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter externalTemperature.
  //
  // The external temperature in degrees celsius
  utils::SharedPtr<ISchemaItem> externalTemperature_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter vin.
  //
  // Vehicle identification number
  utils::SharedPtr<ISchemaItem> vin_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter prndl.
  //
  // See PRNDL
  utils::SharedPtr<ISchemaItem> prndl_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter tirePressure.
  //
  // See TireStatus
  utils::SharedPtr<ISchemaItem> tirePressure_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter odometer.
  //
  // Odometer in km
  utils::SharedPtr<ISchemaItem> odometer_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter beltStatus.
  //
  // The status of the seat belts
  utils::SharedPtr<ISchemaItem> beltStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter bodyInformation.
  //
  // The body information including ignition status and internal temp
  utils::SharedPtr<ISchemaItem> bodyInformation_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter deviceStatus.
  //
  // The device status including signal and battery strength
  utils::SharedPtr<ISchemaItem> deviceStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter driverBraking.
  //
  // The status of the brake pedal
  utils::SharedPtr<ISchemaItem> driverBraking_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter wiperStatus.
  //
  // The status of the wipers
  utils::SharedPtr<ISchemaItem> wiperStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter headLampStatus.
  //
  // Status of the head lamps
  utils::SharedPtr<ISchemaItem> headLampStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter engineTorque.
  //
  // Torque value for engine (in Nm) on non-diesel variants
  utils::SharedPtr<ISchemaItem> engineTorque_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter accPedalPosition.
  //
  // Accelerator pedal position (percentage depressed)
  utils::SharedPtr<ISchemaItem> accPedalPosition_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter steeringWheelAngle.
  //
  // Current angle of the steering wheel (in deg)
  utils::SharedPtr<ISchemaItem> steeringWheelAngle_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter eCallInfo.
  //
  // Emergency Call notification and confirmation data
  utils::SharedPtr<ISchemaItem> eCallInfo_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_ECallInfo);

  // Function parameter airbagStatus.
  //
  // The status of the air bags
  utils::SharedPtr<ISchemaItem> airbagStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_AirbagStatus);

  // Function parameter emergencyEvent.
  //
  // Information related to an emergency event (and if it occurred)
  utils::SharedPtr<ISchemaItem> emergencyEvent_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_EmergencyEvent);

  // Function parameter clusterModeStatus.
  //
  // The status modes of the cluster
  utils::SharedPtr<ISchemaItem> clusterModeStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_ClusterModeStatus);

  // Function parameter myKey.
  //
  // Information related to the MyKey feature
  utils::SharedPtr<ISchemaItem> myKey_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter appID.
  //
  // ID of application requested this RPC.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["gps"] = CObjectSchemaItem::SMember(gps_SchemaItem, false);
  schema_members["speed"] = CObjectSchemaItem::SMember(speed_SchemaItem, false);
  schema_members["rpm"] = CObjectSchemaItem::SMember(rpm_SchemaItem, false);
  schema_members["fuelLevel"] = CObjectSchemaItem::SMember(fuelLevel_SchemaItem, false);
  schema_members["fuelLevel_State"] = CObjectSchemaItem::SMember(fuelLevel_State_SchemaItem, false);
  schema_members["instantFuelConsumption"] = CObjectSchemaItem::SMember(instantFuelConsumption_SchemaItem, false);
  schema_members["externalTemperature"] = CObjectSchemaItem::SMember(externalTemperature_SchemaItem, false);
  schema_members["vin"] = CObjectSchemaItem::SMember(vin_SchemaItem, false);
  schema_members["prndl"] = CObjectSchemaItem::SMember(prndl_SchemaItem, false);
  schema_members["tirePressure"] = CObjectSchemaItem::SMember(tirePressure_SchemaItem, false);
  schema_members["odometer"] = CObjectSchemaItem::SMember(odometer_SchemaItem, false);
  schema_members["beltStatus"] = CObjectSchemaItem::SMember(beltStatus_SchemaItem, false);
  schema_members["bodyInformation"] = CObjectSchemaItem::SMember(bodyInformation_SchemaItem, false);
  schema_members["deviceStatus"] = CObjectSchemaItem::SMember(deviceStatus_SchemaItem, false);
  schema_members["driverBraking"] = CObjectSchemaItem::SMember(driverBraking_SchemaItem, false);
  schema_members["wiperStatus"] = CObjectSchemaItem::SMember(wiperStatus_SchemaItem, false);
  schema_members["headLampStatus"] = CObjectSchemaItem::SMember(headLampStatus_SchemaItem, false);
  schema_members["engineTorque"] = CObjectSchemaItem::SMember(engineTorque_SchemaItem, false);
  schema_members["accPedalPosition"] = CObjectSchemaItem::SMember(accPedalPosition_SchemaItem, false);
  schema_members["steeringWheelAngle"] = CObjectSchemaItem::SMember(steeringWheelAngle_SchemaItem, false);
  schema_members["eCallInfo"] = CObjectSchemaItem::SMember(eCallInfo_SchemaItem, false);
  schema_members["airbagStatus"] = CObjectSchemaItem::SMember(airbagStatus_SchemaItem, false);
  schema_members["emergencyEvent"] = CObjectSchemaItem::SMember(emergencyEvent_SchemaItem, false);
  schema_members["clusterModeStatus"] = CObjectSchemaItem::SMember(clusterModeStatus_SchemaItem, false);
  schema_members["myKey"] = CObjectSchemaItem::SMember(myKey_SchemaItem, false);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_GetVehicleData_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_ComponentVolumeStatus::eType> Common_ComponentVolumeStatus_all_enum_values;
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_UNKNOWN);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_NORMAL);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_LOW);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_FAULT);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_ALERT);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_NOT_SUPPORTED);

  std::set<Common_PRNDL::eType> Common_PRNDL_all_enum_values;
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::PARK);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::REVERSE);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::NEUTRAL);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::DRIVE);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::SPORT);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::LOWGEAR);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::FIRST);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::SECOND);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::THIRD);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::FOURTH);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::FIFTH);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::SIXTH);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::SEVENTH);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::EIGHTH);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::FAULT);

  std::set<Common_VehicleDataEventStatus::eType> Common_VehicleDataEventStatus_all_enum_values;
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NO_EVENT);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NO);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_YES);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NOT_SUPPORTED);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_FAULT);

  std::set<Common_WiperStatus::eType> Common_WiperStatus_all_enum_values;
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::OFF);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::AUTO_OFF);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::OFF_MOVING);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::MAN_INT_OFF);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::MAN_INT_ON);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::MAN_LOW);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::MAN_HIGH);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::MAN_FLICK);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::WASH);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::AUTO_LOW);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::AUTO_HIGH);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::COURTESYWIPE);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::AUTO_ADJUST);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::STALLED);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::NO_DATA_EXISTS);

  // Function parameter gps.
  //
  // See GPSData
  utils::SharedPtr<ISchemaItem> gps_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_GPSData);

  // Function parameter speed.
  //
  // The vehicle speed in kilometers per hour
  utils::SharedPtr<ISchemaItem> speed_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(700.0), TSchemaItemParameter<double>());

  // Function parameter rpm.
  //
  // The number of revolutions per minute of the engine
  utils::SharedPtr<ISchemaItem> rpm_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(20000), TSchemaItemParameter<int>());

  // Function parameter fuelLevel.
  //
  // The fuel level in the tank (percentage)
  utils::SharedPtr<ISchemaItem> fuelLevel_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(-6.0), TSchemaItemParameter<double>(106.0), TSchemaItemParameter<double>());

  // Function parameter fuelLevel_State.
  //
  // The fuel level state
  utils::SharedPtr<ISchemaItem> fuelLevel_State_SchemaItem = TEnumSchemaItem<Common_ComponentVolumeStatus::eType>::create(Common_ComponentVolumeStatus_all_enum_values, TSchemaItemParameter<Common_ComponentVolumeStatus::eType>());

  // Function parameter instantFuelConsumption.
  //
  // The instantaneous fuel consumption in microlitres
  utils::SharedPtr<ISchemaItem> instantFuelConsumption_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(25575.0), TSchemaItemParameter<double>());

  // Function parameter externalTemperature.
  //
  // The external temperature in degrees celsius
  utils::SharedPtr<ISchemaItem> externalTemperature_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(-40.0), TSchemaItemParameter<double>(100.0), TSchemaItemParameter<double>());

  // Function parameter vin.
  //
  // Vehicle identification number
  utils::SharedPtr<ISchemaItem> vin_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(17), TSchemaItemParameter<std::string>());

  // Function parameter prndl.
  //
  // See PRNDL
  utils::SharedPtr<ISchemaItem> prndl_SchemaItem = TEnumSchemaItem<Common_PRNDL::eType>::create(Common_PRNDL_all_enum_values, TSchemaItemParameter<Common_PRNDL::eType>());

  // Function parameter tirePressure.
  //
  // See TireStatus
  utils::SharedPtr<ISchemaItem> tirePressure_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TireStatus);

  // Function parameter odometer.
  //
  // Odometer in km
  utils::SharedPtr<ISchemaItem> odometer_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(17000000), TSchemaItemParameter<int>());

  // Function parameter beltStatus.
  //
  // The status of the seat belts
  utils::SharedPtr<ISchemaItem> beltStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_BeltStatus);

  // Function parameter bodyInformation.
  //
  // The body information including power modes
  utils::SharedPtr<ISchemaItem> bodyInformation_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_BodyInformation);

  // Function parameter deviceStatus.
  //
  // The device status including signal and battery strength
  utils::SharedPtr<ISchemaItem> deviceStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DeviceStatus);

  // Function parameter driverBraking.
  //
  // The status of the brake pedal
  utils::SharedPtr<ISchemaItem> driverBraking_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Function parameter wiperStatus.
  //
  // The status of the wipers
  utils::SharedPtr<ISchemaItem> wiperStatus_SchemaItem = TEnumSchemaItem<Common_WiperStatus::eType>::create(Common_WiperStatus_all_enum_values, TSchemaItemParameter<Common_WiperStatus::eType>());

  // Function parameter headLampStatus.
  //
  // Status of the head lamps
  utils::SharedPtr<ISchemaItem> headLampStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_HeadLampStatus);

  // Function parameter engineTorque.
  //
  // Torque value for engine (in Nm) on non-diesel variants
  utils::SharedPtr<ISchemaItem> engineTorque_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(-1000.0), TSchemaItemParameter<double>(2000.0), TSchemaItemParameter<double>());

  // Function parameter accPedalPosition.
  //
  // Accelerator pedal position (percentage depressed)
  utils::SharedPtr<ISchemaItem> accPedalPosition_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(100.0), TSchemaItemParameter<double>());

  // Function parameter steeringWheelAngle.
  //
  // Current angle of the steering wheel (in deg)
  utils::SharedPtr<ISchemaItem> steeringWheelAngle_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(-2000.0), TSchemaItemParameter<double>(2000.0), TSchemaItemParameter<double>());

  // Function parameter myKey.
  //
  // Information related to the MyKey feature
  utils::SharedPtr<ISchemaItem> myKey_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_MyKey);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["gps"] = CObjectSchemaItem::SMember(gps_SchemaItem, false);
  schema_members["speed"] = CObjectSchemaItem::SMember(speed_SchemaItem, false);
  schema_members["rpm"] = CObjectSchemaItem::SMember(rpm_SchemaItem, false);
  schema_members["fuelLevel"] = CObjectSchemaItem::SMember(fuelLevel_SchemaItem, false);
  schema_members["fuelLevel_State"] = CObjectSchemaItem::SMember(fuelLevel_State_SchemaItem, false);
  schema_members["instantFuelConsumption"] = CObjectSchemaItem::SMember(instantFuelConsumption_SchemaItem, false);
  schema_members["externalTemperature"] = CObjectSchemaItem::SMember(externalTemperature_SchemaItem, false);
  schema_members["vin"] = CObjectSchemaItem::SMember(vin_SchemaItem, false);
  schema_members["prndl"] = CObjectSchemaItem::SMember(prndl_SchemaItem, false);
  schema_members["tirePressure"] = CObjectSchemaItem::SMember(tirePressure_SchemaItem, false);
  schema_members["odometer"] = CObjectSchemaItem::SMember(odometer_SchemaItem, false);
  schema_members["beltStatus"] = CObjectSchemaItem::SMember(beltStatus_SchemaItem, false);
  schema_members["bodyInformation"] = CObjectSchemaItem::SMember(bodyInformation_SchemaItem, false);
  schema_members["deviceStatus"] = CObjectSchemaItem::SMember(deviceStatus_SchemaItem, false);
  schema_members["driverBraking"] = CObjectSchemaItem::SMember(driverBraking_SchemaItem, false);
  schema_members["wiperStatus"] = CObjectSchemaItem::SMember(wiperStatus_SchemaItem, false);
  schema_members["headLampStatus"] = CObjectSchemaItem::SMember(headLampStatus_SchemaItem, false);
  schema_members["engineTorque"] = CObjectSchemaItem::SMember(engineTorque_SchemaItem, false);
  schema_members["accPedalPosition"] = CObjectSchemaItem::SMember(accPedalPosition_SchemaItem, false);
  schema_members["steeringWheelAngle"] = CObjectSchemaItem::SMember(steeringWheelAngle_SchemaItem, false);
  schema_members["myKey"] = CObjectSchemaItem::SMember(myKey_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_VehicleInfo_OnVehicleData_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_ComponentVolumeStatus::eType> Common_ComponentVolumeStatus_all_enum_values;
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_UNKNOWN);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_NORMAL);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_LOW);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_FAULT);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_ALERT);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_NOT_SUPPORTED);

  std::set<Common_PRNDL::eType> Common_PRNDL_all_enum_values;
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::PARK);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::REVERSE);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::NEUTRAL);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::DRIVE);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::SPORT);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::LOWGEAR);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::FIRST);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::SECOND);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::THIRD);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::FOURTH);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::FIFTH);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::SIXTH);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::SEVENTH);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::EIGHTH);
  Common_PRNDL_all_enum_values.insert(Common_PRNDL::FAULT);

  std::set<Common_VehicleDataEventStatus::eType> Common_VehicleDataEventStatus_all_enum_values;
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NO_EVENT);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NO);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_YES);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NOT_SUPPORTED);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_FAULT);

  std::set<Common_WiperStatus::eType> Common_WiperStatus_all_enum_values;
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::OFF);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::AUTO_OFF);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::OFF_MOVING);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::MAN_INT_OFF);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::MAN_INT_ON);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::MAN_LOW);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::MAN_HIGH);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::MAN_FLICK);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::WASH);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::AUTO_LOW);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::AUTO_HIGH);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::COURTESYWIPE);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::AUTO_ADJUST);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::STALLED);
  Common_WiperStatus_all_enum_values.insert(Common_WiperStatus::NO_DATA_EXISTS);

  // Function parameter gps.
  //
  // See GPSData
  utils::SharedPtr<ISchemaItem> gps_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_GPSData);

  // Function parameter speed.
  //
  // The vehicle speed in kilometers per hour
  utils::SharedPtr<ISchemaItem> speed_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(700.0), TSchemaItemParameter<double>());

  // Function parameter rpm.
  //
  // The number of revolutions per minute of the engine
  utils::SharedPtr<ISchemaItem> rpm_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(20000), TSchemaItemParameter<int>());

  // Function parameter fuelLevel.
  //
  // The fuel level in the tank (percentage)
  utils::SharedPtr<ISchemaItem> fuelLevel_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(-6.0), TSchemaItemParameter<double>(106.0), TSchemaItemParameter<double>());

  // Function parameter fuelLevel_State.
  //
  // The fuel level state
  utils::SharedPtr<ISchemaItem> fuelLevel_State_SchemaItem = TEnumSchemaItem<Common_ComponentVolumeStatus::eType>::create(Common_ComponentVolumeStatus_all_enum_values, TSchemaItemParameter<Common_ComponentVolumeStatus::eType>());

  // Function parameter instantFuelConsumption.
  //
  // The instantaneous fuel consumption in microlitres
  utils::SharedPtr<ISchemaItem> instantFuelConsumption_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(25575.0), TSchemaItemParameter<double>());

  // Function parameter externalTemperature.
  //
  // The external temperature in degrees celsius
  utils::SharedPtr<ISchemaItem> externalTemperature_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(-40.0), TSchemaItemParameter<double>(100.0), TSchemaItemParameter<double>());

  // Function parameter vin.
  //
  // Vehicle identification number.
  utils::SharedPtr<ISchemaItem> vin_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(17), TSchemaItemParameter<std::string>());

  // Function parameter prndl.
  //
  // See PRNDL
  utils::SharedPtr<ISchemaItem> prndl_SchemaItem = TEnumSchemaItem<Common_PRNDL::eType>::create(Common_PRNDL_all_enum_values, TSchemaItemParameter<Common_PRNDL::eType>());

  // Function parameter tirePressure.
  //
  // See TireStatus
  utils::SharedPtr<ISchemaItem> tirePressure_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TireStatus);

  // Function parameter odometer.
  //
  // Odometer in km
  utils::SharedPtr<ISchemaItem> odometer_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(17000000), TSchemaItemParameter<int>());

  // Function parameter beltStatus.
  //
  // The status of the seat belts
  utils::SharedPtr<ISchemaItem> beltStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_BeltStatus);

  // Function parameter bodyInformation.
  //
  // The body information including power modes
  utils::SharedPtr<ISchemaItem> bodyInformation_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_BodyInformation);

  // Function parameter deviceStatus.
  //
  // The device status including signal and battery strength
  utils::SharedPtr<ISchemaItem> deviceStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DeviceStatus);

  // Function parameter driverBraking.
  //
  // The status of the brake pedal
  utils::SharedPtr<ISchemaItem> driverBraking_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Function parameter wiperStatus.
  //
  // The status of the wipers
  utils::SharedPtr<ISchemaItem> wiperStatus_SchemaItem = TEnumSchemaItem<Common_WiperStatus::eType>::create(Common_WiperStatus_all_enum_values, TSchemaItemParameter<Common_WiperStatus::eType>());

  // Function parameter headLampStatus.
  //
  // Status of the head lamps
  utils::SharedPtr<ISchemaItem> headLampStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_HeadLampStatus);

  // Function parameter engineTorque.
  //
  // Torque value for engine (in Nm) on non-diesel variants
  utils::SharedPtr<ISchemaItem> engineTorque_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(-1000.0), TSchemaItemParameter<double>(2000.0), TSchemaItemParameter<double>());

  // Function parameter accPedalPosition.
  //
  // Accelerator pedal position (percentage depressed)
  utils::SharedPtr<ISchemaItem> accPedalPosition_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(100.0), TSchemaItemParameter<double>());

  // Function parameter steeringWheelAngle.
  //
  // Current angle of the steering wheel (in deg)
  utils::SharedPtr<ISchemaItem> steeringWheelAngle_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(-2000.0), TSchemaItemParameter<double>(2000.0), TSchemaItemParameter<double>());

  // Function parameter myKey.
  //
  // Information related to the MyKey feature
  utils::SharedPtr<ISchemaItem> myKey_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_MyKey);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["gps"] = CObjectSchemaItem::SMember(gps_SchemaItem, false);
  schema_members["speed"] = CObjectSchemaItem::SMember(speed_SchemaItem, false);
  schema_members["rpm"] = CObjectSchemaItem::SMember(rpm_SchemaItem, false);
  schema_members["fuelLevel"] = CObjectSchemaItem::SMember(fuelLevel_SchemaItem, false);
  schema_members["fuelLevel_State"] = CObjectSchemaItem::SMember(fuelLevel_State_SchemaItem, false);
  schema_members["instantFuelConsumption"] = CObjectSchemaItem::SMember(instantFuelConsumption_SchemaItem, false);
  schema_members["externalTemperature"] = CObjectSchemaItem::SMember(externalTemperature_SchemaItem, false);
  schema_members["vin"] = CObjectSchemaItem::SMember(vin_SchemaItem, false);
  schema_members["prndl"] = CObjectSchemaItem::SMember(prndl_SchemaItem, false);
  schema_members["tirePressure"] = CObjectSchemaItem::SMember(tirePressure_SchemaItem, false);
  schema_members["odometer"] = CObjectSchemaItem::SMember(odometer_SchemaItem, false);
  schema_members["beltStatus"] = CObjectSchemaItem::SMember(beltStatus_SchemaItem, false);
  schema_members["bodyInformation"] = CObjectSchemaItem::SMember(bodyInformation_SchemaItem, false);
  schema_members["deviceStatus"] = CObjectSchemaItem::SMember(deviceStatus_SchemaItem, false);
  schema_members["driverBraking"] = CObjectSchemaItem::SMember(driverBraking_SchemaItem, false);
  schema_members["wiperStatus"] = CObjectSchemaItem::SMember(wiperStatus_SchemaItem, false);
  schema_members["headLampStatus"] = CObjectSchemaItem::SMember(headLampStatus_SchemaItem, false);
  schema_members["engineTorque"] = CObjectSchemaItem::SMember(engineTorque_SchemaItem, false);
  schema_members["accPedalPosition"] = CObjectSchemaItem::SMember(accPedalPosition_SchemaItem, false);
  schema_members["steeringWheelAngle"] = CObjectSchemaItem::SMember(steeringWheelAngle_SchemaItem, false);
  schema_members["myKey"] = CObjectSchemaItem::SMember(myKey_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_ActivateApp_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_ActivateApp_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_AppPriority::eType> Common_AppPriority_all_enum_values;
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::EMERGENCY);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NAVIGATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::VOICE_COMMUNICATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::COMMUNICATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NORMAL);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NONE);

  // Function parameter isSDLAllowed.
  utils::SharedPtr<ISchemaItem> isSDLAllowed_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter device.
  //
  // If isSDLAllowed is false, consent for sending PT through specified device is required.
  utils::SharedPtr<ISchemaItem> device_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DeviceInfo);

  // Function parameter isPermissionsConsentNeeded.
  utils::SharedPtr<ISchemaItem> isPermissionsConsentNeeded_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter isAppPermissionsRevoked.
  utils::SharedPtr<ISchemaItem> isAppPermissionsRevoked_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter appRevokedPermissions.
  //
  // If app permissions were reduced (isAppPermissionsRevoked == true), then this array specifies list of removed permissions. 
  utils::SharedPtr<ISchemaItem> appRevokedPermissions_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_PermissionItem), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter isAppRevoked.
  utils::SharedPtr<ISchemaItem> isAppRevoked_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter priority.
  //
  // Send to HMI so that it can coordinate order of requests/notifications correspondingly.
  utils::SharedPtr<ISchemaItem> priority_SchemaItem = TEnumSchemaItem<Common_AppPriority::eType>::create(Common_AppPriority_all_enum_values, TSchemaItemParameter<Common_AppPriority::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["isSDLAllowed"] = CObjectSchemaItem::SMember(isSDLAllowed_SchemaItem, true);
  schema_members["device"] = CObjectSchemaItem::SMember(device_SchemaItem, false);
  schema_members["isPermissionsConsentNeeded"] = CObjectSchemaItem::SMember(isPermissionsConsentNeeded_SchemaItem, true);
  schema_members["isAppPermissionsRevoked"] = CObjectSchemaItem::SMember(isAppPermissionsRevoked_SchemaItem, true);
  schema_members["appRevokedPermissions"] = CObjectSchemaItem::SMember(appRevokedPermissions_SchemaItem, false);
  schema_members["isAppRevoked"] = CObjectSchemaItem::SMember(isAppRevoked_SchemaItem, true);
  schema_members["priority"] = CObjectSchemaItem::SMember(priority_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_GetUserFriendlyMessage_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  // Function parameter messageCodes.
  //
  // Id of message to be received according to Policy Table i.e. StatusNeeded, Notifications, DrivingCharacteristics etc.
  utils::SharedPtr<ISchemaItem> messageCodes_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter language.
  //
  // Optional parameter if HMI wants message in some other language then its current one already known to SDL.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["messageCodes"] = CObjectSchemaItem::SMember(messageCodes_SchemaItem, true);
  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_GetUserFriendlyMessage_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter messages.
  //
  // If no message was found in PT for specified message code and for HMI current or specified language, this parameter will be omitted.
  utils::SharedPtr<ISchemaItem> messages_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_UserFriendlyMessage), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["messages"] = CObjectSchemaItem::SMember(messages_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_OnAllowSDLFunctionality_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_ConsentSource::eType> Common_ConsentSource_all_enum_values;
  Common_ConsentSource_all_enum_values.insert(Common_ConsentSource::GUI);
  Common_ConsentSource_all_enum_values.insert(Common_ConsentSource::VUI);

  // Function parameter device.
  //
  // If no device is specified permission counts for SDL functionality in general.
  utils::SharedPtr<ISchemaItem> device_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DeviceInfo);

  // Function parameter allowed.
  //
  // Must be true if allowed
  utils::SharedPtr<ISchemaItem> allowed_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter source.
  utils::SharedPtr<ISchemaItem> source_SchemaItem = TEnumSchemaItem<Common_ConsentSource::eType>::create(Common_ConsentSource_all_enum_values, TSchemaItemParameter<Common_ConsentSource::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["device"] = CObjectSchemaItem::SMember(device_SchemaItem, false);
  schema_members["allowed"] = CObjectSchemaItem::SMember(allowed_SchemaItem, true);
  schema_members["source"] = CObjectSchemaItem::SMember(source_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_OnReceivedPolicyUpdate_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter policyfile.
  //
  // Location of decrypted policy table Json file on target
  utils::SharedPtr<ISchemaItem> policyfile_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(255), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["policyfile"] = CObjectSchemaItem::SMember(policyfile_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_OnPolicyUpdate_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_GetListOfPermissions_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter appID.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_GetListOfPermissions_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter allowedFunctions.
  //
  // If no permissions were specified for application the array will come empty.
  utils::SharedPtr<ISchemaItem> allowedFunctions_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_PermissionItem), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["allowedFunctions"] = CObjectSchemaItem::SMember(allowedFunctions_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_OnAppPermissionConsent_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_ConsentSource::eType> Common_ConsentSource_all_enum_values;
  Common_ConsentSource_all_enum_values.insert(Common_ConsentSource::GUI);
  Common_ConsentSource_all_enum_values.insert(Common_ConsentSource::VUI);

  // Function parameter appID.
  //
  // Information about the application. See HMIApplication. If omitted - allow/disallow all applications 
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Function parameter consentedFunctions.
  utils::SharedPtr<ISchemaItem> consentedFunctions_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_PermissionItem), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter source.
  utils::SharedPtr<ISchemaItem> source_SchemaItem = TEnumSchemaItem<Common_ConsentSource::eType>::create(Common_ConsentSource_all_enum_values, TSchemaItemParameter<Common_ConsentSource::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, false);
  schema_members["consentedFunctions"] = CObjectSchemaItem::SMember(consentedFunctions_SchemaItem, true);
  schema_members["source"] = CObjectSchemaItem::SMember(source_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_OnAppPermissionChanged_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_AppPriority::eType> Common_AppPriority_all_enum_values;
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::EMERGENCY);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NAVIGATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::VOICE_COMMUNICATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::COMMUNICATION);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NORMAL);
  Common_AppPriority_all_enum_values.insert(Common_AppPriority::NONE);

  // Function parameter appID.
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Function parameter isAppPermissionsRevoked.
  utils::SharedPtr<ISchemaItem> isAppPermissionsRevoked_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter appRevokedPermissions.
  //
  // If app permissions were reduced (isAppPermissionsRevoked == true), then this array specifies list of removed permissions. 
  utils::SharedPtr<ISchemaItem> appRevokedPermissions_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_PermissionItem), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter appRevoked.
  //
  // If present then specified application was prohibited to used with Sync.
  utils::SharedPtr<ISchemaItem> appRevoked_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter appPermissionsConsentNeeded.
  //
  // If present specifies that permissions were added to application that require User Consent, then HMI can send GetListOfPermissions request to obtain list of permissions.
  utils::SharedPtr<ISchemaItem> appPermissionsConsentNeeded_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter appUnauthorized.
  //
  // When present and set to true (should be if present) then this means that application was not authorized (nickname check failed.)
  utils::SharedPtr<ISchemaItem> appUnauthorized_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter priority.
  //
  // Send to HMI so that it can coordinate order of requests/notifications correspondingly.
  utils::SharedPtr<ISchemaItem> priority_SchemaItem = TEnumSchemaItem<Common_AppPriority::eType>::create(Common_AppPriority_all_enum_values, TSchemaItemParameter<Common_AppPriority::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);
  schema_members["isAppPermissionsRevoked"] = CObjectSchemaItem::SMember(isAppPermissionsRevoked_SchemaItem, false);
  schema_members["appRevokedPermissions"] = CObjectSchemaItem::SMember(appRevokedPermissions_SchemaItem, false);
  schema_members["appRevoked"] = CObjectSchemaItem::SMember(appRevoked_SchemaItem, false);
  schema_members["appPermissionsConsentNeeded"] = CObjectSchemaItem::SMember(appPermissionsConsentNeeded_SchemaItem, false);
  schema_members["appUnauthorized"] = CObjectSchemaItem::SMember(appUnauthorized_SchemaItem, false);
  schema_members["priority"] = CObjectSchemaItem::SMember(priority_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_OnSDLConsentNeeded_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter device.
  utils::SharedPtr<ISchemaItem> device_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_DeviceInfo);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["device"] = CObjectSchemaItem::SMember(device_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_UpdateSDL_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_UpdateSDL_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_UpdateResult::eType> Common_UpdateResult_all_enum_values;
  Common_UpdateResult_all_enum_values.insert(Common_UpdateResult::UP_TO_DATE);
  Common_UpdateResult_all_enum_values.insert(Common_UpdateResult::UPDATING);
  Common_UpdateResult_all_enum_values.insert(Common_UpdateResult::UPDATE_NEEDED);

  // Function parameter result.
  utils::SharedPtr<ISchemaItem> result_SchemaItem = TEnumSchemaItem<Common_UpdateResult::eType>::create(Common_UpdateResult_all_enum_values, TSchemaItemParameter<Common_UpdateResult::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["result"] = CObjectSchemaItem::SMember(result_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_GetStatusUpdate_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_GetStatusUpdate_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_UpdateResult::eType> Common_UpdateResult_all_enum_values;
  Common_UpdateResult_all_enum_values.insert(Common_UpdateResult::UP_TO_DATE);
  Common_UpdateResult_all_enum_values.insert(Common_UpdateResult::UPDATING);
  Common_UpdateResult_all_enum_values.insert(Common_UpdateResult::UPDATE_NEEDED);

  // Function parameter status.
  utils::SharedPtr<ISchemaItem> status_SchemaItem = TEnumSchemaItem<Common_UpdateResult::eType>::create(Common_UpdateResult_all_enum_values, TSchemaItemParameter<Common_UpdateResult::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["status"] = CObjectSchemaItem::SMember(status_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_OnStatusUpdate_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_UpdateResult::eType> Common_UpdateResult_all_enum_values;
  Common_UpdateResult_all_enum_values.insert(Common_UpdateResult::UP_TO_DATE);
  Common_UpdateResult_all_enum_values.insert(Common_UpdateResult::UPDATING);
  Common_UpdateResult_all_enum_values.insert(Common_UpdateResult::UPDATE_NEEDED);

  // Function parameter status.
  utils::SharedPtr<ISchemaItem> status_SchemaItem = TEnumSchemaItem<Common_UpdateResult::eType>::create(Common_UpdateResult_all_enum_values, TSchemaItemParameter<Common_UpdateResult::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["status"] = CObjectSchemaItem::SMember(status_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_OnSystemError_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_SystemError::eType> Common_SystemError_all_enum_values;
  Common_SystemError_all_enum_values.insert(Common_SystemError::SYNC_REBOOTED);
  Common_SystemError_all_enum_values.insert(Common_SystemError::SYNC_OUT_OF_MEMMORY);

  // Function parameter error.
  utils::SharedPtr<ISchemaItem> error_SchemaItem = TEnumSchemaItem<Common_SystemError::eType>::create(Common_SystemError_all_enum_values, TSchemaItemParameter<Common_SystemError::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["error"] = CObjectSchemaItem::SMember(error_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_AddStatisticsInfo_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Common_StatisticsType::eType> Common_StatisticsType_all_enum_values;
  Common_StatisticsType_all_enum_values.insert(Common_StatisticsType::iAPP_BUFFER_FULL);

  // Function parameter statisticType.
  utils::SharedPtr<ISchemaItem> statisticType_SchemaItem = TEnumSchemaItem<Common_StatisticsType::eType>::create(Common_StatisticsType_all_enum_values, TSchemaItemParameter<Common_StatisticsType::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["statisticType"] = CObjectSchemaItem::SMember(statisticType_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_GetURLS_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter service.
  utils::SharedPtr<ISchemaItem> service_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(100), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["service"] = CObjectSchemaItem::SMember(service_SchemaItem, true);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema hmi_apis::HMI_API::InitFunction_SDL_GetURLS_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter urls.
  utils::SharedPtr<ISchemaItem> urls_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_ServiceInfo), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["urls"] = CObjectSchemaItem::SMember(urls_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> params_members;
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<FunctionID::eType>::create(function_id_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<messageType::eType>::create(message_type_items), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
  params_members[NsSmartDeviceLink::NsJSONHandler::strings::kCode] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

//----------- Structs schema items initialization ------------

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_UserFriendlyMessage(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member messageCode.
  utils::SharedPtr<ISchemaItem> messageCode_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  // Struct member ttsString.
  utils::SharedPtr<ISchemaItem> ttsString_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  // Struct member label.
  utils::SharedPtr<ISchemaItem> label_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  // Struct member line1.
  utils::SharedPtr<ISchemaItem> line1_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  // Struct member line2.
  utils::SharedPtr<ISchemaItem> line2_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  // Struct member textBody.
  utils::SharedPtr<ISchemaItem> textBody_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["messageCode"] = CObjectSchemaItem::SMember(messageCode_SchemaItem, true);
  schema_members["ttsString"] = CObjectSchemaItem::SMember(ttsString_SchemaItem, false);
  schema_members["label"] = CObjectSchemaItem::SMember(label_SchemaItem, false);
  schema_members["line1"] = CObjectSchemaItem::SMember(line1_SchemaItem, false);
  schema_members["line2"] = CObjectSchemaItem::SMember(line2_SchemaItem, false);
  schema_members["textBody"] = CObjectSchemaItem::SMember(textBody_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_PermissionItem(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member name.
  //
  // Code of message of user-friendly text about functional group to be allowed/disallowed
  utils::SharedPtr<ISchemaItem> name_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  // Struct member id.
  //
  // Id of corresponding functional group, needed by SDL.
  utils::SharedPtr<ISchemaItem> id_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Struct member allowed.
  //
  // Specifies whether functionality was allowed/disallowed. If ommited - no information about User Consent is yet found for app.
  utils::SharedPtr<ISchemaItem> allowed_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["name"] = CObjectSchemaItem::SMember(name_SchemaItem, true);
  schema_members["id"] = CObjectSchemaItem::SMember(id_SchemaItem, true);
  schema_members["allowed"] = CObjectSchemaItem::SMember(allowed_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_ServiceInfo(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member url.
  //
  // Get URL based on service type.
  utils::SharedPtr<ISchemaItem> url_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  // Struct member policyAppId.
  //
  // Used if URL needed are specific for application.
  utils::SharedPtr<ISchemaItem> policyAppId_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["url"] = CObjectSchemaItem::SMember(url_SchemaItem, true);
  schema_members["policyAppId"] = CObjectSchemaItem::SMember(policyAppId_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_TextField(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_TextFieldName::eType> Common_TextFieldName_all_enum_values;
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mainField1);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mainField2);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mainField3);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mainField4);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::statusBar);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mediaClock);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mediaTrack);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::alertText1);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::alertText2);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::alertText3);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::scrollableMessageBody);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::initialInteractionText);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::navigationText1);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::navigationText2);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::ETA);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::totalDistance);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::navigationText);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::audioPassThruDisplayText1);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::audioPassThruDisplayText2);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::sliderHeader);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::sliderFooter);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::notificationText);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::menuName);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::secondaryText);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::tertiaryText);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::menuTitle);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::timeToDestination);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::turnText);

  std::set<Common_CharacterSet::eType> Common_CharacterSet_all_enum_values;
  Common_CharacterSet_all_enum_values.insert(Common_CharacterSet::TYPE2SET);
  Common_CharacterSet_all_enum_values.insert(Common_CharacterSet::TYPE5SET);
  Common_CharacterSet_all_enum_values.insert(Common_CharacterSet::CID1SET);
  Common_CharacterSet_all_enum_values.insert(Common_CharacterSet::CID2SET);

  // Struct member name.
  //
  // The name that identifies the field. See TextFieldName.
  utils::SharedPtr<ISchemaItem> name_SchemaItem = TEnumSchemaItem<Common_TextFieldName::eType>::create(Common_TextFieldName_all_enum_values, TSchemaItemParameter<Common_TextFieldName::eType>());

  // Struct member characterSet.
  //
  // The character set that is supported in this field. See CharacterSet.
  utils::SharedPtr<ISchemaItem> characterSet_SchemaItem = TEnumSchemaItem<Common_CharacterSet::eType>::create(Common_CharacterSet_all_enum_values, TSchemaItemParameter<Common_CharacterSet::eType>());

  // Struct member width.
  //
  // The number of characters in one row of this field.
  utils::SharedPtr<ISchemaItem> width_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(500), TSchemaItemParameter<int>());

  // Struct member rows.
  //
  // The number of rows of this field.
  utils::SharedPtr<ISchemaItem> rows_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(3), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["name"] = CObjectSchemaItem::SMember(name_SchemaItem, true);
  schema_members["characterSet"] = CObjectSchemaItem::SMember(characterSet_SchemaItem, true);
  schema_members["width"] = CObjectSchemaItem::SMember(width_SchemaItem, true);
  schema_members["rows"] = CObjectSchemaItem::SMember(rows_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_ButtonCapabilities(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_ButtonName::eType> Common_ButtonName_all_enum_values;
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::OK);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::SEEKLEFT);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::SEEKRIGHT);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::TUNEUP);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::TUNEDOWN);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_0);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_1);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_2);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_3);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_4);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_5);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_6);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_7);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_8);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::PRESET_9);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::CUSTOM_BUTTON);
  Common_ButtonName_all_enum_values.insert(Common_ButtonName::SEARCH);

  // Struct member name.
  //
  // The name of the Button from the ButtonName enum
  utils::SharedPtr<ISchemaItem> name_SchemaItem = TEnumSchemaItem<Common_ButtonName::eType>::create(Common_ButtonName_all_enum_values, TSchemaItemParameter<Common_ButtonName::eType>());

  // Struct member shortPressAvailable.
  //
  // The button supports a short press. Whenever the button is pressed short, onButtonPressed(SHORT) should be invoked.
  utils::SharedPtr<ISchemaItem> shortPressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member longPressAvailable.
  //
  // The button supports a LONG press. Whenever the button is pressed long, onButtonPressed(LONG) should be invoked.
  utils::SharedPtr<ISchemaItem> longPressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member upDownAvailable.
  //
  // The button supports "button down" and "button up". Whenever the button is pressed, onButtonEvent(DOWN) should be invoked. Whenever the button is released, onButtonEvent(UP) should be invoked.
  utils::SharedPtr<ISchemaItem> upDownAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["name"] = CObjectSchemaItem::SMember(name_SchemaItem, true);
  schema_members["shortPressAvailable"] = CObjectSchemaItem::SMember(shortPressAvailable_SchemaItem, true);
  schema_members["longPressAvailable"] = CObjectSchemaItem::SMember(longPressAvailable_SchemaItem, true);
  schema_members["upDownAvailable"] = CObjectSchemaItem::SMember(upDownAvailable_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_VehicleDataResult(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_VehicleDataType::eType> Common_VehicleDataType_all_enum_values;
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_GPS);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_SPEED);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_RPM);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_FUELLEVEL);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_FUELLEVEL_STATE);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_FUELCONSUMPTION);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_EXTERNTEMP);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_VIN);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_PRNDL);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_TIREPRESSURE);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_ODOMETER);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_BELTSTATUS);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_BODYINFO);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_DEVICESTATUS);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_ECALLINFO);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_AIRBAGSTATUS);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_EMERGENCYEVENT);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_CLUSTERMODESTATUS);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_MYKEY);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_BRAKING);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_WIPERSTATUS);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_HEADLAMPSTATUS);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_BATTVOLTAGE);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_ENGINETORQUE);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_ACCPEDAL);
  Common_VehicleDataType_all_enum_values.insert(Common_VehicleDataType::VEHICLEDATA_STEERINGWHEEL);

  std::set<Common_VehicleDataResultCode::eType> Common_VehicleDataResultCode_all_enum_values;
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_SUCCESS);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_TRUNCATED_DATA);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_DISALLOWED);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_USER_DISALLOWED);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_INVALID_ID);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_DATA_NOT_AVAILABLE);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_IGNORED);

  // Struct member dataType.
  //
  // Defined published data element type.
  utils::SharedPtr<ISchemaItem> dataType_SchemaItem = TEnumSchemaItem<Common_VehicleDataType::eType>::create(Common_VehicleDataType_all_enum_values, TSchemaItemParameter<Common_VehicleDataType::eType>());

  // Struct member resultCode.
  //
  // Published data result code.
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Common_VehicleDataResultCode::eType>::create(Common_VehicleDataResultCode_all_enum_values, TSchemaItemParameter<Common_VehicleDataResultCode::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["dataType"] = CObjectSchemaItem::SMember(dataType_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_TouchCoord(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member x.
  //
  // The x coordinate of the touch.
  utils::SharedPtr<ISchemaItem> x_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(10000), TSchemaItemParameter<int>());

  // Struct member y.
  //
  // The y coordinate of the touch.
  utils::SharedPtr<ISchemaItem> y_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(10000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["x"] = CObjectSchemaItem::SMember(x_SchemaItem, true);
  schema_members["y"] = CObjectSchemaItem::SMember(y_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_TouchEvent(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member id.
  //
  // 
  //       A touch's unique identifier.  The application can track the current touch events by id.
  //       If a touch event has type begin, the id should be added to the set of touches.
  //       If a touch event has type end, the id should be removed from the set of touches.
  //     
  utils::SharedPtr<ISchemaItem> id_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(9), TSchemaItemParameter<int>());

  // Struct member ts.
  //
  // 
  //       The time that the touch was recorded.  This number can the time since the beginning of the session or something else as long as the units are in milliseconds.
  //       The timestamp is used to determined the rate of change of position of a touch.
  //       The application also uses the time to verify whether two touches, with different ids, are part of a single action by the user.
  //       If there is only a single timestamp in this array, it is the same for every coordinate in the coordinates array.
  //     
  utils::SharedPtr<ISchemaItem> ts_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2147483647), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000));

  // Struct member c.
  utils::SharedPtr<ISchemaItem> c_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TouchCoord), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["id"] = CObjectSchemaItem::SMember(id_SchemaItem, true);
  schema_members["ts"] = CObjectSchemaItem::SMember(ts_SchemaItem, true);
  schema_members["c"] = CObjectSchemaItem::SMember(c_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_PresetBankCapabilities(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member onScreenPresetsAvailable.
  //
  // Must be true if onscreen custom presets are available.
  utils::SharedPtr<ISchemaItem> onScreenPresetsAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["onScreenPresetsAvailable"] = CObjectSchemaItem::SMember(onScreenPresetsAvailable_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_Image(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_ImageType::eType> Common_ImageType_all_enum_values;
  Common_ImageType_all_enum_values.insert(Common_ImageType::STATIC);
  Common_ImageType_all_enum_values.insert(Common_ImageType::DYNAMIC);

  // Struct member value.
  //
  // The path to the dynamic image stored on HU or the static binary image itself.
  utils::SharedPtr<ISchemaItem> value_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(65535), TSchemaItemParameter<std::string>());

  // Struct member imageType.
  //
  // Describes, whether it is a static or dynamic image.
  utils::SharedPtr<ISchemaItem> imageType_SchemaItem = TEnumSchemaItem<Common_ImageType::eType>::create(Common_ImageType_all_enum_values, TSchemaItemParameter<Common_ImageType::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["value"] = CObjectSchemaItem::SMember(value_SchemaItem, true);
  schema_members["imageType"] = CObjectSchemaItem::SMember(imageType_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_SoftButton(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_SoftButtonType::eType> Common_SoftButtonType_all_enum_values;
  Common_SoftButtonType_all_enum_values.insert(Common_SoftButtonType::SBT_TEXT);
  Common_SoftButtonType_all_enum_values.insert(Common_SoftButtonType::SBT_IMAGE);
  Common_SoftButtonType_all_enum_values.insert(Common_SoftButtonType::SBT_BOTH);

  std::set<Common_SystemAction::eType> Common_SystemAction_all_enum_values;
  Common_SystemAction_all_enum_values.insert(Common_SystemAction::DEFAULT_ACTION);
  Common_SystemAction_all_enum_values.insert(Common_SystemAction::STEAL_FOCUS);
  Common_SystemAction_all_enum_values.insert(Common_SystemAction::KEEP_CONTEXT);

  // Struct member type.
  //
  // Describes, whether text, icon or both text and image should be displayed on the soft button. See softButtonType
  utils::SharedPtr<ISchemaItem> type_SchemaItem = TEnumSchemaItem<Common_SoftButtonType::eType>::create(Common_SoftButtonType_all_enum_values, TSchemaItemParameter<Common_SoftButtonType::eType>());

  // Struct member text.
  //
  // Optional text to be displayed (if defined as TEXT or BOTH)
  utils::SharedPtr<ISchemaItem> text_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member image.
  //
  // Optional image struct for SoftButton (if defined as IMAGE or BOTH)
  utils::SharedPtr<ISchemaItem> image_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  // Struct member isHighlighted.
  //
  // If true, must be highlighted
  // If false, must be not
  utils::SharedPtr<ISchemaItem> isHighlighted_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member softButtonID.
  //
  // Value which must be returned via OnButtonPress / OnButtonEvent
  utils::SharedPtr<ISchemaItem> softButtonID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Struct member systemAction.
  //
  // Parameter indicates whether clicking a SoftButton must call a specific system action.  See SystemAction
  utils::SharedPtr<ISchemaItem> systemAction_SchemaItem = TEnumSchemaItem<Common_SystemAction::eType>::create(Common_SystemAction_all_enum_values, TSchemaItemParameter<Common_SystemAction::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["type"] = CObjectSchemaItem::SMember(type_SchemaItem, true);
  schema_members["text"] = CObjectSchemaItem::SMember(text_SchemaItem, false);
  schema_members["image"] = CObjectSchemaItem::SMember(image_SchemaItem, false);
  schema_members["isHighlighted"] = CObjectSchemaItem::SMember(isHighlighted_SchemaItem, false);
  schema_members["softButtonID"] = CObjectSchemaItem::SMember(softButtonID_SchemaItem, true);
  schema_members["systemAction"] = CObjectSchemaItem::SMember(systemAction_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_TTSChunk(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_SpeechCapabilities::eType> Common_SpeechCapabilities_all_enum_values;
  Common_SpeechCapabilities_all_enum_values.insert(Common_SpeechCapabilities::SC_TEXT);
  Common_SpeechCapabilities_all_enum_values.insert(Common_SpeechCapabilities::SAPI_PHONEMES);
  Common_SpeechCapabilities_all_enum_values.insert(Common_SpeechCapabilities::LHPLUS_PHONEMES);
  Common_SpeechCapabilities_all_enum_values.insert(Common_SpeechCapabilities::PRE_RECORDED);
  Common_SpeechCapabilities_all_enum_values.insert(Common_SpeechCapabilities::SILENCE);

  // Struct member text.
  //
  // The text or phonemes to be spoken.
  utils::SharedPtr<ISchemaItem> text_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member type.
  //
  // Describes, whether it is text or a specific phoneme set. See SpeechCapabilities.
  utils::SharedPtr<ISchemaItem> type_SchemaItem = TEnumSchemaItem<Common_SpeechCapabilities::eType>::create(Common_SpeechCapabilities_all_enum_values, TSchemaItemParameter<Common_SpeechCapabilities::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["text"] = CObjectSchemaItem::SMember(text_SchemaItem, true);
  schema_members["type"] = CObjectSchemaItem::SMember(type_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_HMIApplication(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  std::set<Common_AppHMIType::eType> Common_AppHMIType_all_enum_values;
  Common_AppHMIType_all_enum_values.insert(Common_AppHMIType::DEFAULT);
  Common_AppHMIType_all_enum_values.insert(Common_AppHMIType::COMMUNICATION);
  Common_AppHMIType_all_enum_values.insert(Common_AppHMIType::MEDIA);
  Common_AppHMIType_all_enum_values.insert(Common_AppHMIType::MESSAGING);
  Common_AppHMIType_all_enum_values.insert(Common_AppHMIType::NAVIGATION);
  Common_AppHMIType_all_enum_values.insert(Common_AppHMIType::INFORMATION);
  Common_AppHMIType_all_enum_values.insert(Common_AppHMIType::SOCIAL);
  Common_AppHMIType_all_enum_values.insert(Common_AppHMIType::BACKGROUND_PROCESS);
  Common_AppHMIType_all_enum_values.insert(Common_AppHMIType::TESTING);
  Common_AppHMIType_all_enum_values.insert(Common_AppHMIType::SYSTEM);

  // Struct member appName.
  //
  // The mobile application name, e.g. "Ford Drive Green".
  utils::SharedPtr<ISchemaItem> appName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>());

  // Struct member ngnMediaScreenAppName.
  //
  // Provides an abbreviated version of the app name (if needed), that may be displayed on the NGN media screen.
  // If not provided, the appName should be used instead (and may be truncated if too long)
  utils::SharedPtr<ISchemaItem> ngnMediaScreenAppName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>());

  // Struct member icon.
  //
  // Path to application icon stored on HU.
  utils::SharedPtr<ISchemaItem> icon_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  // Struct member deviceName.
  //
  // The name of device which the provided application is running on.
  utils::SharedPtr<ISchemaItem> deviceName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  // Struct member appID.
  //
  // Unique (during ignition cycle) id of the application. To be used in all RPCs sent by both HU system and SDL
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Struct member hmiDisplayLanguageDesired.
  //
  // The language the application intends to use on HU  
  utils::SharedPtr<ISchemaItem> hmiDisplayLanguageDesired_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  // Struct member isMediaApplication.
  //
  // Indicates whether it is a media or a non-media application.
  utils::SharedPtr<ISchemaItem> isMediaApplication_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member appType.
  //
  // List of all applicable app types stating which classifications to be given to the app.
  // e.g. for platforms like GEN2, this determines which "corner(s)" the app can populate.
  utils::SharedPtr<ISchemaItem> appType_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Common_AppHMIType::eType>::create(Common_AppHMIType_all_enum_values, TSchemaItemParameter<Common_AppHMIType::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["appName"] = CObjectSchemaItem::SMember(appName_SchemaItem, true);
  schema_members["ngnMediaScreenAppName"] = CObjectSchemaItem::SMember(ngnMediaScreenAppName_SchemaItem, false);
  schema_members["icon"] = CObjectSchemaItem::SMember(icon_SchemaItem, false);
  schema_members["deviceName"] = CObjectSchemaItem::SMember(deviceName_SchemaItem, true);
  schema_members["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem, true);
  schema_members["hmiDisplayLanguageDesired"] = CObjectSchemaItem::SMember(hmiDisplayLanguageDesired_SchemaItem, true);
  schema_members["isMediaApplication"] = CObjectSchemaItem::SMember(isMediaApplication_SchemaItem, true);
  schema_members["appType"] = CObjectSchemaItem::SMember(appType_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_MenuParams(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member parentID.
  //
  // unique ID of the sub menu, the command must be added to.
  // If not provided, the command must be added to the top level of the in application menu.
  utils::SharedPtr<ISchemaItem> parentID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Struct member position.
  //
  // Position within the items that are at the top level of the in application menu.
  // 0 should insert at the front.
  // 1 should insert at the second position.
  // if position is greater than or equal to the number of items on the top level, the the sub menu/command should be appended to the end.
  // If this param is omitted the entry should be added at the end.
  utils::SharedPtr<ISchemaItem> position_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(1000), TSchemaItemParameter<int>());

  // Struct member menuName.
  //
  // The name of the sub menu/command.
  utils::SharedPtr<ISchemaItem> menuName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["parentID"] = CObjectSchemaItem::SMember(parentID_SchemaItem, false);
  schema_members["position"] = CObjectSchemaItem::SMember(position_SchemaItem, false);
  schema_members["menuName"] = CObjectSchemaItem::SMember(menuName_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_Choice(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member choiceID.
  //
  //  The unique within the concerned application identifier for this choice 
  utils::SharedPtr<ISchemaItem> choiceID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Struct member menuName.
  //
  //  The name of the choice 
  utils::SharedPtr<ISchemaItem> menuName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member image.
  //
  //  The image for representing the choice 
  utils::SharedPtr<ISchemaItem> image_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  // Struct member secondaryText.
  //
  // Optional secondary text to display; e.g. address of POI in a search result entry
  utils::SharedPtr<ISchemaItem> secondaryText_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member tertiaryText.
  //
  // Optional tertiary text to display; e.g. distance to POI for a search result entry
  utils::SharedPtr<ISchemaItem> tertiaryText_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member secondaryImage.
  //
  // Optional secondary image struct for choice
  utils::SharedPtr<ISchemaItem> secondaryImage_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["choiceID"] = CObjectSchemaItem::SMember(choiceID_SchemaItem, true);
  schema_members["menuName"] = CObjectSchemaItem::SMember(menuName_SchemaItem, false);
  schema_members["image"] = CObjectSchemaItem::SMember(image_SchemaItem, false);
  schema_members["secondaryText"] = CObjectSchemaItem::SMember(secondaryText_SchemaItem, false);
  schema_members["tertiaryText"] = CObjectSchemaItem::SMember(tertiaryText_SchemaItem, false);
  schema_members["secondaryImage"] = CObjectSchemaItem::SMember(secondaryImage_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_VrHelpItem(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member text.
  //
  // Text to display for VR Help item
  utils::SharedPtr<ISchemaItem> text_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member image.
  //
  // Image struct for VR Help item
  utils::SharedPtr<ISchemaItem> image_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  // Struct member position.
  //
  // Position to display item in VR Help list
  utils::SharedPtr<ISchemaItem> position_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(100), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["text"] = CObjectSchemaItem::SMember(text_SchemaItem, true);
  schema_members["image"] = CObjectSchemaItem::SMember(image_SchemaItem, false);
  schema_members["position"] = CObjectSchemaItem::SMember(position_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_TimeFormat(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member hours.
  //
  // The hour of the media clock.
  // Some units only support a max of 19 hours. If out of range, it should be rejected.
  utils::SharedPtr<ISchemaItem> hours_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(59), TSchemaItemParameter<int>());

  // Struct member minutes.
  utils::SharedPtr<ISchemaItem> minutes_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(59), TSchemaItemParameter<int>());

  // Struct member seconds.
  utils::SharedPtr<ISchemaItem> seconds_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(59), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["hours"] = CObjectSchemaItem::SMember(hours_SchemaItem, true);
  schema_members["minutes"] = CObjectSchemaItem::SMember(minutes_SchemaItem, true);
  schema_members["seconds"] = CObjectSchemaItem::SMember(seconds_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_TouchEventCapabilities(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member pressAvailable.
  utils::SharedPtr<ISchemaItem> pressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member multiTouchAvailable.
  utils::SharedPtr<ISchemaItem> multiTouchAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member doublePressAvailable.
  utils::SharedPtr<ISchemaItem> doublePressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["pressAvailable"] = CObjectSchemaItem::SMember(pressAvailable_SchemaItem, true);
  schema_members["multiTouchAvailable"] = CObjectSchemaItem::SMember(multiTouchAvailable_SchemaItem, true);
  schema_members["doublePressAvailable"] = CObjectSchemaItem::SMember(doublePressAvailable_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_ImageResolution(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member resolutionWidth.
  //
  // The image resolution width.
  utils::SharedPtr<ISchemaItem> resolutionWidth_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(10000), TSchemaItemParameter<int>());

  // Struct member resolutionHeight.
  //
  // The image resolution height.
  utils::SharedPtr<ISchemaItem> resolutionHeight_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(10000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["resolutionWidth"] = CObjectSchemaItem::SMember(resolutionWidth_SchemaItem, true);
  schema_members["resolutionHeight"] = CObjectSchemaItem::SMember(resolutionHeight_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_ScreenParams(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member resolution.
  //
  // The resolution of the prescribed screen area.
  utils::SharedPtr<ISchemaItem> resolution_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_ImageResolution);

  // Struct member touchEventAvailable.
  //
  // Types of screen touch events available in screen area.
  utils::SharedPtr<ISchemaItem> touchEventAvailable_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TouchEventCapabilities);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["resolution"] = CObjectSchemaItem::SMember(resolution_SchemaItem, true);
  schema_members["touchEventAvailable"] = CObjectSchemaItem::SMember(touchEventAvailable_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_ImageField(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_ImageFieldName::eType> Common_ImageFieldName_all_enum_values;
  Common_ImageFieldName_all_enum_values.insert(Common_ImageFieldName::softButtonImage);
  Common_ImageFieldName_all_enum_values.insert(Common_ImageFieldName::choiceImage);
  Common_ImageFieldName_all_enum_values.insert(Common_ImageFieldName::choiceSecondaryImage);
  Common_ImageFieldName_all_enum_values.insert(Common_ImageFieldName::vrHelpItem);
  Common_ImageFieldName_all_enum_values.insert(Common_ImageFieldName::turnIcon);
  Common_ImageFieldName_all_enum_values.insert(Common_ImageFieldName::menuIcon);
  Common_ImageFieldName_all_enum_values.insert(Common_ImageFieldName::cmdIcon);
  Common_ImageFieldName_all_enum_values.insert(Common_ImageFieldName::appIcon);
  Common_ImageFieldName_all_enum_values.insert(Common_ImageFieldName::graphic);
  Common_ImageFieldName_all_enum_values.insert(Common_ImageFieldName::showConstantTBTIcon);
  Common_ImageFieldName_all_enum_values.insert(Common_ImageFieldName::showConstantTBTNextTurnIcon);

  std::set<Common_FileType::eType> Common_FileType_all_enum_values;
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_BMP);
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_JPEG);
  Common_FileType_all_enum_values.insert(Common_FileType::GRAPHIC_PNG);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_WAVE);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_MP3);
  Common_FileType_all_enum_values.insert(Common_FileType::AUDIO_AAC);
  Common_FileType_all_enum_values.insert(Common_FileType::BINARY);
  Common_FileType_all_enum_values.insert(Common_FileType::JSON);

  // Struct member name.
  //
  // The name that identifies the field. See ImageFieldName.
  utils::SharedPtr<ISchemaItem> name_SchemaItem = TEnumSchemaItem<Common_ImageFieldName::eType>::create(Common_ImageFieldName_all_enum_values, TSchemaItemParameter<Common_ImageFieldName::eType>());

  // Struct member imageTypeSupported.
  //
  // The image types that are supported in this field. See FileType.
  utils::SharedPtr<ISchemaItem> imageTypeSupported_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Common_FileType::eType>::create(Common_FileType_all_enum_values, TSchemaItemParameter<Common_FileType::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Struct member imageResolution.
  //
  // The image resolution of this field.
  utils::SharedPtr<ISchemaItem> imageResolution_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_ImageResolution);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["name"] = CObjectSchemaItem::SMember(name_SchemaItem, true);
  schema_members["imageTypeSupported"] = CObjectSchemaItem::SMember(imageTypeSupported_SchemaItem, false);
  schema_members["imageResolution"] = CObjectSchemaItem::SMember(imageResolution_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_DisplayCapabilities(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_DisplayType::eType> Common_DisplayType_all_enum_values;
  Common_DisplayType_all_enum_values.insert(Common_DisplayType::CID);
  Common_DisplayType_all_enum_values.insert(Common_DisplayType::TYPE2);
  Common_DisplayType_all_enum_values.insert(Common_DisplayType::TYPE5);
  Common_DisplayType_all_enum_values.insert(Common_DisplayType::NGN);
  Common_DisplayType_all_enum_values.insert(Common_DisplayType::GEN2_8_DMA);
  Common_DisplayType_all_enum_values.insert(Common_DisplayType::GEN2_6_DMA);
  Common_DisplayType_all_enum_values.insert(Common_DisplayType::MFD3);
  Common_DisplayType_all_enum_values.insert(Common_DisplayType::MFD4);
  Common_DisplayType_all_enum_values.insert(Common_DisplayType::MFD5);
  Common_DisplayType_all_enum_values.insert(Common_DisplayType::GEN3_8_INCH);

  std::set<Common_MediaClockFormat::eType> Common_MediaClockFormat_all_enum_values;
  Common_MediaClockFormat_all_enum_values.insert(Common_MediaClockFormat::CLOCK1);
  Common_MediaClockFormat_all_enum_values.insert(Common_MediaClockFormat::CLOCK2);
  Common_MediaClockFormat_all_enum_values.insert(Common_MediaClockFormat::CLOCK3);
  Common_MediaClockFormat_all_enum_values.insert(Common_MediaClockFormat::CLOCKTEXT1);
  Common_MediaClockFormat_all_enum_values.insert(Common_MediaClockFormat::CLOCKTEXT2);
  Common_MediaClockFormat_all_enum_values.insert(Common_MediaClockFormat::CLOCKTEXT3);
  Common_MediaClockFormat_all_enum_values.insert(Common_MediaClockFormat::CLOCKTEXT4);

  std::set<Common_ImageType::eType> Common_ImageType_all_enum_values;
  Common_ImageType_all_enum_values.insert(Common_ImageType::STATIC);
  Common_ImageType_all_enum_values.insert(Common_ImageType::DYNAMIC);

  // Struct member displayType.
  //
  // The type of the display. See DisplayType
  utils::SharedPtr<ISchemaItem> displayType_SchemaItem = TEnumSchemaItem<Common_DisplayType::eType>::create(Common_DisplayType_all_enum_values, TSchemaItemParameter<Common_DisplayType::eType>());

  // Struct member textFields.
  //
  // A set of all fields for text displaying supported by HU. See TextFieldName.
  // If there are no textfields supported, the empty array must be returned
  utils::SharedPtr<ISchemaItem> textFields_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TextField), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(100));

  // Struct member imageFields.
  //
  // A set of all fields that support images. See ImageField
  utils::SharedPtr<ISchemaItem> imageFields_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_ImageField), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Struct member mediaClockFormats.
  //
  // A set of all supported formats of the media clock. See MediaClockFormat
  utils::SharedPtr<ISchemaItem> mediaClockFormats_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Common_MediaClockFormat::eType>::create(Common_MediaClockFormat_all_enum_values, TSchemaItemParameter<Common_MediaClockFormat::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Struct member imageCapabilities.
  utils::SharedPtr<ISchemaItem> imageCapabilities_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<Common_ImageType::eType>::create(Common_ImageType_all_enum_values, TSchemaItemParameter<Common_ImageType::eType>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(2));

  // Struct member graphicSupported.
  //
  // The display's persistent screen supports referencing a static or dynamic image.
  utils::SharedPtr<ISchemaItem> graphicSupported_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member templatesAvailable.
  //
  // A set of all predefined persistent display templates available on headunit.  To be referenced in SetDisplayLayout.
  utils::SharedPtr<ISchemaItem> templatesAvailable_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(100));

  // Struct member screenParams.
  //
  // A set of all parameters related to a prescribed screen area (e.g. for video / touch input).
  utils::SharedPtr<ISchemaItem> screenParams_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_ScreenParams);

  // Struct member numCustomPresetsAvailable.
  //
  // The number of on-screen custom presets available (if any); otherwise omitted.
  utils::SharedPtr<ISchemaItem> numCustomPresetsAvailable_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(100), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["displayType"] = CObjectSchemaItem::SMember(displayType_SchemaItem, true);
  schema_members["textFields"] = CObjectSchemaItem::SMember(textFields_SchemaItem, true);
  schema_members["imageFields"] = CObjectSchemaItem::SMember(imageFields_SchemaItem, false);
  schema_members["mediaClockFormats"] = CObjectSchemaItem::SMember(mediaClockFormats_SchemaItem, true);
  schema_members["imageCapabilities"] = CObjectSchemaItem::SMember(imageCapabilities_SchemaItem, false);
  schema_members["graphicSupported"] = CObjectSchemaItem::SMember(graphicSupported_SchemaItem, true);
  schema_members["templatesAvailable"] = CObjectSchemaItem::SMember(templatesAvailable_SchemaItem, true);
  schema_members["screenParams"] = CObjectSchemaItem::SMember(screenParams_SchemaItem, false);
  schema_members["numCustomPresetsAvailable"] = CObjectSchemaItem::SMember(numCustomPresetsAvailable_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_SoftButtonCapabilities(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member shortPressAvailable.
  //
  // The button supports a short press.
  // Whenever the button is pressed short, onButtonPressed( SHORT) must be invoked.
  utils::SharedPtr<ISchemaItem> shortPressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member longPressAvailable.
  //
  // The button supports a LONG press.
  // Whenever the button is pressed long, onButtonPressed( LONG) must be invoked.
  utils::SharedPtr<ISchemaItem> longPressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member upDownAvailable.
  //
  // The button supports "button down" and "button up".
  // Whenever the button is pressed, onButtonEvent( DOWN) must be invoked.
  // Whenever the button is released, onButtonEvent( UP) must be invoked.
  utils::SharedPtr<ISchemaItem> upDownAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member imageSupported.
  //
  // Must be true if the button supports referencing a static or dynamic image.
  utils::SharedPtr<ISchemaItem> imageSupported_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["shortPressAvailable"] = CObjectSchemaItem::SMember(shortPressAvailable_SchemaItem, true);
  schema_members["longPressAvailable"] = CObjectSchemaItem::SMember(longPressAvailable_SchemaItem, true);
  schema_members["upDownAvailable"] = CObjectSchemaItem::SMember(upDownAvailable_SchemaItem, true);
  schema_members["imageSupported"] = CObjectSchemaItem::SMember(imageSupported_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_AudioPassThruCapabilities(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_SamplingRate::eType> Common_SamplingRate_all_enum_values;
  Common_SamplingRate_all_enum_values.insert(Common_SamplingRate::RATE_8KHZ);
  Common_SamplingRate_all_enum_values.insert(Common_SamplingRate::RATE_16KHZ);
  Common_SamplingRate_all_enum_values.insert(Common_SamplingRate::RATE_22KHZ);
  Common_SamplingRate_all_enum_values.insert(Common_SamplingRate::RATE_44KHZ);

  std::set<Common_BitsPerSample::eType> Common_BitsPerSample_all_enum_values;
  Common_BitsPerSample_all_enum_values.insert(Common_BitsPerSample::RATE_8_BIT);
  Common_BitsPerSample_all_enum_values.insert(Common_BitsPerSample::RATE_16_BIT);

  std::set<Common_AudioType::eType> Common_AudioType_all_enum_values;
  Common_AudioType_all_enum_values.insert(Common_AudioType::PCM);

  // Struct member samplingRate.
  utils::SharedPtr<ISchemaItem> samplingRate_SchemaItem = TEnumSchemaItem<Common_SamplingRate::eType>::create(Common_SamplingRate_all_enum_values, TSchemaItemParameter<Common_SamplingRate::eType>());

  // Struct member bitsPerSample.
  utils::SharedPtr<ISchemaItem> bitsPerSample_SchemaItem = TEnumSchemaItem<Common_BitsPerSample::eType>::create(Common_BitsPerSample_all_enum_values, TSchemaItemParameter<Common_BitsPerSample::eType>());

  // Struct member audioType.
  utils::SharedPtr<ISchemaItem> audioType_SchemaItem = TEnumSchemaItem<Common_AudioType::eType>::create(Common_AudioType_all_enum_values, TSchemaItemParameter<Common_AudioType::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["samplingRate"] = CObjectSchemaItem::SMember(samplingRate_SchemaItem, true);
  schema_members["bitsPerSample"] = CObjectSchemaItem::SMember(bitsPerSample_SchemaItem, true);
  schema_members["audioType"] = CObjectSchemaItem::SMember(audioType_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_Coordinate(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member xCoord.
  utils::SharedPtr<ISchemaItem> xCoord_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  // Struct member yCoord.
  utils::SharedPtr<ISchemaItem> yCoord_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["xCoord"] = CObjectSchemaItem::SMember(xCoord_SchemaItem, true);
  schema_members["yCoord"] = CObjectSchemaItem::SMember(yCoord_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_TextFieldStruct(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_TextFieldName::eType> Common_TextFieldName_all_enum_values;
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mainField1);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mainField2);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mainField3);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mainField4);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::statusBar);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mediaClock);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::mediaTrack);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::alertText1);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::alertText2);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::alertText3);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::scrollableMessageBody);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::initialInteractionText);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::navigationText1);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::navigationText2);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::ETA);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::totalDistance);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::navigationText);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::audioPassThruDisplayText1);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::audioPassThruDisplayText2);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::sliderHeader);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::sliderFooter);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::notificationText);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::menuName);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::secondaryText);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::tertiaryText);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::menuTitle);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::timeToDestination);
  Common_TextFieldName_all_enum_values.insert(Common_TextFieldName::turnText);

  // Struct member fieldName.
  //
  // The name of the field for displaying the text.
  utils::SharedPtr<ISchemaItem> fieldName_SchemaItem = TEnumSchemaItem<Common_TextFieldName::eType>::create(Common_TextFieldName_all_enum_values, TSchemaItemParameter<Common_TextFieldName::eType>());

  // Struct member fieldText.
  //
  // The  text itself.
  utils::SharedPtr<ISchemaItem> fieldText_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["fieldName"] = CObjectSchemaItem::SMember(fieldName_SchemaItem, true);
  schema_members["fieldText"] = CObjectSchemaItem::SMember(fieldText_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_KeyboardProperties(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_Language::eType> Common_Language_all_enum_values;
  Common_Language_all_enum_values.insert(Common_Language::EN_US);
  Common_Language_all_enum_values.insert(Common_Language::ES_MX);
  Common_Language_all_enum_values.insert(Common_Language::FR_CA);
  Common_Language_all_enum_values.insert(Common_Language::DE_DE);
  Common_Language_all_enum_values.insert(Common_Language::ES_ES);
  Common_Language_all_enum_values.insert(Common_Language::EN_GB);
  Common_Language_all_enum_values.insert(Common_Language::RU_RU);
  Common_Language_all_enum_values.insert(Common_Language::TR_TR);
  Common_Language_all_enum_values.insert(Common_Language::PL_PL);
  Common_Language_all_enum_values.insert(Common_Language::FR_FR);
  Common_Language_all_enum_values.insert(Common_Language::IT_IT);
  Common_Language_all_enum_values.insert(Common_Language::SV_SE);
  Common_Language_all_enum_values.insert(Common_Language::PT_PT);
  Common_Language_all_enum_values.insert(Common_Language::NL_NL);
  Common_Language_all_enum_values.insert(Common_Language::EN_AU);
  Common_Language_all_enum_values.insert(Common_Language::ZH_CN);
  Common_Language_all_enum_values.insert(Common_Language::ZH_TW);
  Common_Language_all_enum_values.insert(Common_Language::JA_JP);
  Common_Language_all_enum_values.insert(Common_Language::AR_SA);
  Common_Language_all_enum_values.insert(Common_Language::KO_KR);
  Common_Language_all_enum_values.insert(Common_Language::PT_BR);
  Common_Language_all_enum_values.insert(Common_Language::CS_CZ);
  Common_Language_all_enum_values.insert(Common_Language::DA_DK);
  Common_Language_all_enum_values.insert(Common_Language::NO_NO);

  std::set<Common_KeyboardLayout::eType> Common_KeyboardLayout_all_enum_values;
  Common_KeyboardLayout_all_enum_values.insert(Common_KeyboardLayout::QWERTY);
  Common_KeyboardLayout_all_enum_values.insert(Common_KeyboardLayout::QWERTZ);
  Common_KeyboardLayout_all_enum_values.insert(Common_KeyboardLayout::AZERTY);

  std::set<Common_KeypressMode::eType> Common_KeypressMode_all_enum_values;
  Common_KeypressMode_all_enum_values.insert(Common_KeypressMode::SINGLE_KEYPRESS);
  Common_KeypressMode_all_enum_values.insert(Common_KeypressMode::QUEUE_KEYPRESSES);
  Common_KeypressMode_all_enum_values.insert(Common_KeypressMode::RESEND_CURRENT_ENTRY);

  // Struct member language.
  //
  // The keyboard language.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Common_Language::eType>::create(Common_Language_all_enum_values, TSchemaItemParameter<Common_Language::eType>());

  // Struct member keyboardLayout.
  //
  // Desired keyboard layout.
  utils::SharedPtr<ISchemaItem> keyboardLayout_SchemaItem = TEnumSchemaItem<Common_KeyboardLayout::eType>::create(Common_KeyboardLayout_all_enum_values, TSchemaItemParameter<Common_KeyboardLayout::eType>());

  // Struct member keypressMode.
  //
  // 
  //     	Desired keypress mode.
  //     	If omitted, this value will be set to RESEND_CURRENT_ENTRY.
  //     
  utils::SharedPtr<ISchemaItem> keypressMode_SchemaItem = TEnumSchemaItem<Common_KeypressMode::eType>::create(Common_KeypressMode_all_enum_values, TSchemaItemParameter<Common_KeypressMode::eType>());

  // Struct member limitedCharacterList.
  //
  // Array of keyboard characters to enable.
  // All omitted characters will be greyed out (disabled) on the keyboard.
  // If omitted, the entire keyboard will be enabled.
  utils::SharedPtr<ISchemaItem> limitedCharacterList_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Struct member autoCompleteText.
  //
  // Allows an app to prepopulate the text field with a suggested or completed entry as the user types
  utils::SharedPtr<ISchemaItem> autoCompleteText_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, false);
  schema_members["keyboardLayout"] = CObjectSchemaItem::SMember(keyboardLayout_SchemaItem, false);
  schema_members["keypressMode"] = CObjectSchemaItem::SMember(keypressMode_SchemaItem, false);
  schema_members["limitedCharacterList"] = CObjectSchemaItem::SMember(limitedCharacterList_SchemaItem, false);
  schema_members["autoCompleteText"] = CObjectSchemaItem::SMember(autoCompleteText_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_Turn(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member navigationText.
  //
  // Uses navigationText from TextFieldStruct.
  utils::SharedPtr<ISchemaItem> navigationText_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_TextFieldStruct);

  // Struct member turnIcon.
  utils::SharedPtr<ISchemaItem> turnIcon_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_Image);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["navigationText"] = CObjectSchemaItem::SMember(navigationText_SchemaItem, false);
  schema_members["turnIcon"] = CObjectSchemaItem::SMember(turnIcon_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_VehicleType(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member make.
  //
  // Make of the vehicle
  // e.g. Ford
  utils::SharedPtr<ISchemaItem> make_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member model.
  //
  // Model of the vehicle
  // e.g. Fiesta
  utils::SharedPtr<ISchemaItem> model_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member modelYear.
  //
  // Model Year of the vehicle
  // e.g. 2013
  utils::SharedPtr<ISchemaItem> modelYear_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member trim.
  //
  // Trim of the vehicle
  // e.g. SE
  utils::SharedPtr<ISchemaItem> trim_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["make"] = CObjectSchemaItem::SMember(make_SchemaItem, false);
  schema_members["model"] = CObjectSchemaItem::SMember(model_SchemaItem, false);
  schema_members["modelYear"] = CObjectSchemaItem::SMember(modelYear_SchemaItem, false);
  schema_members["trim"] = CObjectSchemaItem::SMember(trim_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_DeviceInfo(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member name.
  //
  // The name of the device connected.
  utils::SharedPtr<ISchemaItem> name_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(), TSchemaItemParameter<std::string>());

  // Struct member id.
  //
  // The ID of the device connected
  utils::SharedPtr<ISchemaItem> id_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(), TSchemaItemParameter<int>(), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["name"] = CObjectSchemaItem::SMember(name_SchemaItem, true);
  schema_members["id"] = CObjectSchemaItem::SMember(id_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_GPSData(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_CompassDirection::eType> Common_CompassDirection_all_enum_values;
  Common_CompassDirection_all_enum_values.insert(Common_CompassDirection::NORTH);
  Common_CompassDirection_all_enum_values.insert(Common_CompassDirection::NORTHWEST);
  Common_CompassDirection_all_enum_values.insert(Common_CompassDirection::WEST);
  Common_CompassDirection_all_enum_values.insert(Common_CompassDirection::SOUTHWEST);
  Common_CompassDirection_all_enum_values.insert(Common_CompassDirection::SOUTH);
  Common_CompassDirection_all_enum_values.insert(Common_CompassDirection::SOUTHEAST);
  Common_CompassDirection_all_enum_values.insert(Common_CompassDirection::EAST);
  Common_CompassDirection_all_enum_values.insert(Common_CompassDirection::NORTHEAST);

  std::set<Common_Dimension::eType> Common_Dimension_all_enum_values;
  Common_Dimension_all_enum_values.insert(Common_Dimension::Dimension_NO_FIX);
  Common_Dimension_all_enum_values.insert(Common_Dimension::Dimension_2D);
  Common_Dimension_all_enum_values.insert(Common_Dimension::Dimension_3D);

  // Struct member longitudeDegrees.
  utils::SharedPtr<ISchemaItem> longitudeDegrees_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(-180.0), TSchemaItemParameter<double>(180.0), TSchemaItemParameter<double>());

  // Struct member latitudeDegrees.
  utils::SharedPtr<ISchemaItem> latitudeDegrees_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(-90.0), TSchemaItemParameter<double>(90.0), TSchemaItemParameter<double>());

  // Struct member utcYear.
  //
  // The current UTC year.
  utils::SharedPtr<ISchemaItem> utcYear_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(2010), TSchemaItemParameter<int>(2100), TSchemaItemParameter<int>());

  // Struct member utcMonth.
  //
  // The current UTC month.
  utils::SharedPtr<ISchemaItem> utcMonth_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(12), TSchemaItemParameter<int>());

  // Struct member utcDay.
  //
  // The current UTC day.
  utils::SharedPtr<ISchemaItem> utcDay_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(31), TSchemaItemParameter<int>());

  // Struct member utcHours.
  //
  // The current UTC hour.
  utils::SharedPtr<ISchemaItem> utcHours_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(23), TSchemaItemParameter<int>());

  // Struct member utcMinutes.
  //
  // The current UTC minute.
  utils::SharedPtr<ISchemaItem> utcMinutes_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(59), TSchemaItemParameter<int>());

  // Struct member utcSeconds.
  //
  // The current UTC second.
  utils::SharedPtr<ISchemaItem> utcSeconds_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(59), TSchemaItemParameter<int>());

  // Struct member compassDirection.
  //
  // See CompassDirection.
  utils::SharedPtr<ISchemaItem> compassDirection_SchemaItem = TEnumSchemaItem<Common_CompassDirection::eType>::create(Common_CompassDirection_all_enum_values, TSchemaItemParameter<Common_CompassDirection::eType>());

  // Struct member pdop.
  //
  // PDOP.
  utils::SharedPtr<ISchemaItem> pdop_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(10.0), TSchemaItemParameter<double>());

  // Struct member hdop.
  //
  // HDOP.
  utils::SharedPtr<ISchemaItem> hdop_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(10.0), TSchemaItemParameter<double>());

  // Struct member vdop.
  //
  // VDOP.
  utils::SharedPtr<ISchemaItem> vdop_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(10.0), TSchemaItemParameter<double>());

  // Struct member actual.
  //
  // 
  //       True, if actual.
  //       False, if infered.
  //     
  utils::SharedPtr<ISchemaItem> actual_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member satellites.
  //
  // Number of satellites in view
  utils::SharedPtr<ISchemaItem> satellites_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(31), TSchemaItemParameter<int>());

  // Struct member dimension.
  //
  // See Dimension
  utils::SharedPtr<ISchemaItem> dimension_SchemaItem = TEnumSchemaItem<Common_Dimension::eType>::create(Common_Dimension_all_enum_values, TSchemaItemParameter<Common_Dimension::eType>());

  // Struct member altitude.
  //
  // Altitude in meters
  utils::SharedPtr<ISchemaItem> altitude_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(-10000.0), TSchemaItemParameter<double>(10000.0), TSchemaItemParameter<double>());

  // Struct member heading.
  //
  // The heading. North is 0. Resolution is 0.01
  utils::SharedPtr<ISchemaItem> heading_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(359.99), TSchemaItemParameter<double>());

  // Struct member speed.
  //
  // The speed in KPH
  utils::SharedPtr<ISchemaItem> speed_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(500.0), TSchemaItemParameter<double>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["longitudeDegrees"] = CObjectSchemaItem::SMember(longitudeDegrees_SchemaItem, false);
  schema_members["latitudeDegrees"] = CObjectSchemaItem::SMember(latitudeDegrees_SchemaItem, false);
  schema_members["utcYear"] = CObjectSchemaItem::SMember(utcYear_SchemaItem, false);
  schema_members["utcMonth"] = CObjectSchemaItem::SMember(utcMonth_SchemaItem, false);
  schema_members["utcDay"] = CObjectSchemaItem::SMember(utcDay_SchemaItem, false);
  schema_members["utcHours"] = CObjectSchemaItem::SMember(utcHours_SchemaItem, false);
  schema_members["utcMinutes"] = CObjectSchemaItem::SMember(utcMinutes_SchemaItem, false);
  schema_members["utcSeconds"] = CObjectSchemaItem::SMember(utcSeconds_SchemaItem, false);
  schema_members["compassDirection"] = CObjectSchemaItem::SMember(compassDirection_SchemaItem, false);
  schema_members["pdop"] = CObjectSchemaItem::SMember(pdop_SchemaItem, false);
  schema_members["hdop"] = CObjectSchemaItem::SMember(hdop_SchemaItem, false);
  schema_members["vdop"] = CObjectSchemaItem::SMember(vdop_SchemaItem, false);
  schema_members["actual"] = CObjectSchemaItem::SMember(actual_SchemaItem, false);
  schema_members["satellites"] = CObjectSchemaItem::SMember(satellites_SchemaItem, false);
  schema_members["dimension"] = CObjectSchemaItem::SMember(dimension_SchemaItem, false);
  schema_members["altitude"] = CObjectSchemaItem::SMember(altitude_SchemaItem, false);
  schema_members["heading"] = CObjectSchemaItem::SMember(heading_SchemaItem, false);
  schema_members["speed"] = CObjectSchemaItem::SMember(speed_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_SingleTireStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_ComponentVolumeStatus::eType> Common_ComponentVolumeStatus_all_enum_values;
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_UNKNOWN);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_NORMAL);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_LOW);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_FAULT);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_ALERT);
  Common_ComponentVolumeStatus_all_enum_values.insert(Common_ComponentVolumeStatus::CVS_NOT_SUPPORTED);

  // Struct member status.
  //
  // The status of component volume. See ComponentVolumeStatus.
  utils::SharedPtr<ISchemaItem> status_SchemaItem = TEnumSchemaItem<Common_ComponentVolumeStatus::eType>::create(Common_ComponentVolumeStatus_all_enum_values, TSchemaItemParameter<Common_ComponentVolumeStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["status"] = CObjectSchemaItem::SMember(status_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_DIDResult(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_VehicleDataResultCode::eType> Common_VehicleDataResultCode_all_enum_values;
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_SUCCESS);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_TRUNCATED_DATA);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_DISALLOWED);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_USER_DISALLOWED);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_INVALID_ID);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_DATA_NOT_AVAILABLE);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED);
  Common_VehicleDataResultCode_all_enum_values.insert(Common_VehicleDataResultCode::VDRC_IGNORED);

  // Struct member resultCode.
  //
  // Individual DID result code.
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Common_VehicleDataResultCode::eType>::create(Common_VehicleDataResultCode_all_enum_values, TSchemaItemParameter<Common_VehicleDataResultCode::eType>());

  // Struct member didLocation.
  //
  // Location of raw data (the address from ReadDID request)
  utils::SharedPtr<ISchemaItem> didLocation_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Struct member data.
  //
  // Raw DID-based data returned for requested element.
  utils::SharedPtr<ISchemaItem> data_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(5000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["didLocation"] = CObjectSchemaItem::SMember(didLocation_SchemaItem, true);
  schema_members["data"] = CObjectSchemaItem::SMember(data_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_HeadLampStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_AmbientLightStatus::eType> Common_AmbientLightStatus_all_enum_values;
  Common_AmbientLightStatus_all_enum_values.insert(Common_AmbientLightStatus::NIGHT);
  Common_AmbientLightStatus_all_enum_values.insert(Common_AmbientLightStatus::TWILIGHT_1);
  Common_AmbientLightStatus_all_enum_values.insert(Common_AmbientLightStatus::TWILIGHT_2);
  Common_AmbientLightStatus_all_enum_values.insert(Common_AmbientLightStatus::TWILIGHT_3);
  Common_AmbientLightStatus_all_enum_values.insert(Common_AmbientLightStatus::TWILIGHT_4);
  Common_AmbientLightStatus_all_enum_values.insert(Common_AmbientLightStatus::DAY);
  Common_AmbientLightStatus_all_enum_values.insert(Common_AmbientLightStatus::UNKNOWN);
  Common_AmbientLightStatus_all_enum_values.insert(Common_AmbientLightStatus::INVALID);

  // Struct member lowBeamsOn.
  //
  // Status of the low beam lamps.
  utils::SharedPtr<ISchemaItem> lowBeamsOn_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member highBeamsOn.
  //
  // Status of the high beam lamps.
  utils::SharedPtr<ISchemaItem> highBeamsOn_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member ambientLightSensorStatus.
  //
  // Status of the ambient light sensor.
  utils::SharedPtr<ISchemaItem> ambientLightSensorStatus_SchemaItem = TEnumSchemaItem<Common_AmbientLightStatus::eType>::create(Common_AmbientLightStatus_all_enum_values, TSchemaItemParameter<Common_AmbientLightStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["lowBeamsOn"] = CObjectSchemaItem::SMember(lowBeamsOn_SchemaItem, true);
  schema_members["highBeamsOn"] = CObjectSchemaItem::SMember(highBeamsOn_SchemaItem, true);
  schema_members["ambientLightSensorStatus"] = CObjectSchemaItem::SMember(ambientLightSensorStatus_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_TireStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_WarningLightStatus::eType> Common_WarningLightStatus_all_enum_values;
  Common_WarningLightStatus_all_enum_values.insert(Common_WarningLightStatus::WLS_OFF);
  Common_WarningLightStatus_all_enum_values.insert(Common_WarningLightStatus::WLS_ON);
  Common_WarningLightStatus_all_enum_values.insert(Common_WarningLightStatus::WLS_FLASH);
  Common_WarningLightStatus_all_enum_values.insert(Common_WarningLightStatus::WLS_NOT_USED);

  // Struct member pressureTelltale.
  //
  // Status of the Tire Pressure Telltale. See WarningLightStatus.
  utils::SharedPtr<ISchemaItem> pressureTelltale_SchemaItem = TEnumSchemaItem<Common_WarningLightStatus::eType>::create(Common_WarningLightStatus_all_enum_values, TSchemaItemParameter<Common_WarningLightStatus::eType>());

  // Struct member leftFront.
  //
  // The status of the left front tire.
  utils::SharedPtr<ISchemaItem> leftFront_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SingleTireStatus);

  // Struct member rightFront.
  //
  // The status of the right front tire.
  utils::SharedPtr<ISchemaItem> rightFront_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SingleTireStatus);

  // Struct member leftRear.
  //
  // The status of the left rear tire.
  utils::SharedPtr<ISchemaItem> leftRear_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SingleTireStatus);

  // Struct member rightRear.
  //
  // The status of the right rear tire.
  utils::SharedPtr<ISchemaItem> rightRear_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SingleTireStatus);

  // Struct member innerLeftRear.
  //
  // The status of the inner left rear.
  utils::SharedPtr<ISchemaItem> innerLeftRear_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SingleTireStatus);

  // Struct member innerRightRear.
  //
  // The status of the inner right rear.
  utils::SharedPtr<ISchemaItem> innerRightRear_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Common_SingleTireStatus);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["pressureTelltale"] = CObjectSchemaItem::SMember(pressureTelltale_SchemaItem, false);
  schema_members["leftFront"] = CObjectSchemaItem::SMember(leftFront_SchemaItem, false);
  schema_members["rightFront"] = CObjectSchemaItem::SMember(rightFront_SchemaItem, false);
  schema_members["leftRear"] = CObjectSchemaItem::SMember(leftRear_SchemaItem, false);
  schema_members["rightRear"] = CObjectSchemaItem::SMember(rightRear_SchemaItem, false);
  schema_members["innerLeftRear"] = CObjectSchemaItem::SMember(innerLeftRear_SchemaItem, false);
  schema_members["innerRightRear"] = CObjectSchemaItem::SMember(innerRightRear_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_BeltStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_VehicleDataEventStatus::eType> Common_VehicleDataEventStatus_all_enum_values;
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NO_EVENT);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NO);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_YES);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NOT_SUPPORTED);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_FAULT);

  // Struct member driverBeltDeployed.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverBeltDeployed_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member passengerBeltDeployed.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerBeltDeployed_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member passengerBuckleBelted.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerBuckleBelted_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member driverBuckleBelted.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverBuckleBelted_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member leftRow2BuckleBelted.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> leftRow2BuckleBelted_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member passengerChildDetected.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerChildDetected_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member rightRow2BuckleBelted.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> rightRow2BuckleBelted_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member middleRow2BuckleBelted.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> middleRow2BuckleBelted_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member middleRow3BuckleBelted.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> middleRow3BuckleBelted_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member leftRow3BuckleBelted.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> leftRow3BuckleBelted_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member rightRow3BuckleBelted.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> rightRow3BuckleBelted_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member leftRearInflatableBelted.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> leftRearInflatableBelted_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member rightRearInflatableBelted.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> rightRearInflatableBelted_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member middleRow1BeltDeployed.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> middleRow1BeltDeployed_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member middleRow1BuckleBelted.
  //
  // See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> middleRow1BuckleBelted_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["driverBeltDeployed"] = CObjectSchemaItem::SMember(driverBeltDeployed_SchemaItem, false);
  schema_members["passengerBeltDeployed"] = CObjectSchemaItem::SMember(passengerBeltDeployed_SchemaItem, false);
  schema_members["passengerBuckleBelted"] = CObjectSchemaItem::SMember(passengerBuckleBelted_SchemaItem, false);
  schema_members["driverBuckleBelted"] = CObjectSchemaItem::SMember(driverBuckleBelted_SchemaItem, false);
  schema_members["leftRow2BuckleBelted"] = CObjectSchemaItem::SMember(leftRow2BuckleBelted_SchemaItem, false);
  schema_members["passengerChildDetected"] = CObjectSchemaItem::SMember(passengerChildDetected_SchemaItem, false);
  schema_members["rightRow2BuckleBelted"] = CObjectSchemaItem::SMember(rightRow2BuckleBelted_SchemaItem, false);
  schema_members["middleRow2BuckleBelted"] = CObjectSchemaItem::SMember(middleRow2BuckleBelted_SchemaItem, false);
  schema_members["middleRow3BuckleBelted"] = CObjectSchemaItem::SMember(middleRow3BuckleBelted_SchemaItem, false);
  schema_members["leftRow3BuckleBelted"] = CObjectSchemaItem::SMember(leftRow3BuckleBelted_SchemaItem, false);
  schema_members["rightRow3BuckleBelted"] = CObjectSchemaItem::SMember(rightRow3BuckleBelted_SchemaItem, false);
  schema_members["leftRearInflatableBelted"] = CObjectSchemaItem::SMember(leftRearInflatableBelted_SchemaItem, false);
  schema_members["rightRearInflatableBelted"] = CObjectSchemaItem::SMember(rightRearInflatableBelted_SchemaItem, false);
  schema_members["middleRow1BeltDeployed"] = CObjectSchemaItem::SMember(middleRow1BeltDeployed_SchemaItem, false);
  schema_members["middleRow1BuckleBelted"] = CObjectSchemaItem::SMember(middleRow1BuckleBelted_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_BodyInformation(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_IgnitionStableStatus::eType> Common_IgnitionStableStatus_all_enum_values;
  Common_IgnitionStableStatus_all_enum_values.insert(Common_IgnitionStableStatus::IGNITION_SWITCH_NOT_STABLE);
  Common_IgnitionStableStatus_all_enum_values.insert(Common_IgnitionStableStatus::IGNITION_SWITCH_STABLE);
  Common_IgnitionStableStatus_all_enum_values.insert(Common_IgnitionStableStatus::MISSING_FROM_TRANSMITTER);

  std::set<Common_IgnitionStatus::eType> Common_IgnitionStatus_all_enum_values;
  Common_IgnitionStatus_all_enum_values.insert(Common_IgnitionStatus::IS_UNKNOWN);
  Common_IgnitionStatus_all_enum_values.insert(Common_IgnitionStatus::IS_OFF);
  Common_IgnitionStatus_all_enum_values.insert(Common_IgnitionStatus::IS_ACCESSORY);
  Common_IgnitionStatus_all_enum_values.insert(Common_IgnitionStatus::IS_RUN);
  Common_IgnitionStatus_all_enum_values.insert(Common_IgnitionStatus::IS_START);
  Common_IgnitionStatus_all_enum_values.insert(Common_IgnitionStatus::IS_INVALID);

  // Struct member parkBrakeActive.
  //
  // Must be true if the park brake is active
  utils::SharedPtr<ISchemaItem> parkBrakeActive_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member ignitionStableStatus.
  //
  // Information about the ignition switch. See IgnitionStableStatus.
  utils::SharedPtr<ISchemaItem> ignitionStableStatus_SchemaItem = TEnumSchemaItem<Common_IgnitionStableStatus::eType>::create(Common_IgnitionStableStatus_all_enum_values, TSchemaItemParameter<Common_IgnitionStableStatus::eType>());

  // Struct member ignitionStatus.
  //
  // The status of the ignition. See IgnitionStatus.
  utils::SharedPtr<ISchemaItem> ignitionStatus_SchemaItem = TEnumSchemaItem<Common_IgnitionStatus::eType>::create(Common_IgnitionStatus_all_enum_values, TSchemaItemParameter<Common_IgnitionStatus::eType>());

  // Struct member driverDoorAjar.
  //
  // References signal "DrStatDrv_B_Actl".
  utils::SharedPtr<ISchemaItem> driverDoorAjar_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member passengerDoorAjar.
  //
  // References signal "DrStatPsngr_B_Actl".
  utils::SharedPtr<ISchemaItem> passengerDoorAjar_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member rearLeftDoorAjar.
  //
  // References signal "DrStatRl_B_Actl".
  utils::SharedPtr<ISchemaItem> rearLeftDoorAjar_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member rearRightDoorAjar.
  //
  // References signal "DrStatRr_B_Actl".
  utils::SharedPtr<ISchemaItem> rearRightDoorAjar_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["parkBrakeActive"] = CObjectSchemaItem::SMember(parkBrakeActive_SchemaItem, true);
  schema_members["ignitionStableStatus"] = CObjectSchemaItem::SMember(ignitionStableStatus_SchemaItem, true);
  schema_members["ignitionStatus"] = CObjectSchemaItem::SMember(ignitionStatus_SchemaItem, true);
  schema_members["driverDoorAjar"] = CObjectSchemaItem::SMember(driverDoorAjar_SchemaItem, false);
  schema_members["passengerDoorAjar"] = CObjectSchemaItem::SMember(passengerDoorAjar_SchemaItem, false);
  schema_members["rearLeftDoorAjar"] = CObjectSchemaItem::SMember(rearLeftDoorAjar_SchemaItem, false);
  schema_members["rearRightDoorAjar"] = CObjectSchemaItem::SMember(rearRightDoorAjar_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_DeviceStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_DeviceLevelStatus::eType> Common_DeviceLevelStatus_all_enum_values;
  Common_DeviceLevelStatus_all_enum_values.insert(Common_DeviceLevelStatus::ZERO_LEVEL_BARS);
  Common_DeviceLevelStatus_all_enum_values.insert(Common_DeviceLevelStatus::ONE_LEVEL_BARS);
  Common_DeviceLevelStatus_all_enum_values.insert(Common_DeviceLevelStatus::TWO_LEVEL_BARS);
  Common_DeviceLevelStatus_all_enum_values.insert(Common_DeviceLevelStatus::THREE_LEVEL_BARS);
  Common_DeviceLevelStatus_all_enum_values.insert(Common_DeviceLevelStatus::FOUR_LEVEL_BARS);
  Common_DeviceLevelStatus_all_enum_values.insert(Common_DeviceLevelStatus::NOT_PROVIDED);

  std::set<Common_PrimaryAudioSource::eType> Common_PrimaryAudioSource_all_enum_values;
  Common_PrimaryAudioSource_all_enum_values.insert(Common_PrimaryAudioSource::NO_SOURCE_SELECTED);
  Common_PrimaryAudioSource_all_enum_values.insert(Common_PrimaryAudioSource::USB);
  Common_PrimaryAudioSource_all_enum_values.insert(Common_PrimaryAudioSource::USB2);
  Common_PrimaryAudioSource_all_enum_values.insert(Common_PrimaryAudioSource::BLUETOOTH_STEREO_BTST);
  Common_PrimaryAudioSource_all_enum_values.insert(Common_PrimaryAudioSource::LINE_IN);
  Common_PrimaryAudioSource_all_enum_values.insert(Common_PrimaryAudioSource::IPOD);
  Common_PrimaryAudioSource_all_enum_values.insert(Common_PrimaryAudioSource::MOBILE_APP);

  // Struct member voiceRecOn.
  //
  // Must be true if the voice recording is on.
  utils::SharedPtr<ISchemaItem> voiceRecOn_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member btIconOn.
  //
  // Must be true if Bluetooth icon is displayed.
  utils::SharedPtr<ISchemaItem> btIconOn_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member callActive.
  //
  // Must be true if there is an active call..
  utils::SharedPtr<ISchemaItem> callActive_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member phoneRoaming.
  //
  // Must be true if ther is a phone roaming.
  utils::SharedPtr<ISchemaItem> phoneRoaming_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member textMsgAvailable.
  //
  // Must be true if the text message is available.
  utils::SharedPtr<ISchemaItem> textMsgAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member battLevelStatus.
  //
  // Device battery level status. See DeviceLevelStatus.
  utils::SharedPtr<ISchemaItem> battLevelStatus_SchemaItem = TEnumSchemaItem<Common_DeviceLevelStatus::eType>::create(Common_DeviceLevelStatus_all_enum_values, TSchemaItemParameter<Common_DeviceLevelStatus::eType>());

  // Struct member stereoAudioOutputMuted.
  //
  // Must be true if stereo audio output is muted.
  utils::SharedPtr<ISchemaItem> stereoAudioOutputMuted_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member monoAudioOutputMuted.
  //
  // Must be true if mono audio output is muted.
  utils::SharedPtr<ISchemaItem> monoAudioOutputMuted_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member signalLevelStatus.
  //
  // Device signal level status. See DeviceLevelStatus.
  utils::SharedPtr<ISchemaItem> signalLevelStatus_SchemaItem = TEnumSchemaItem<Common_DeviceLevelStatus::eType>::create(Common_DeviceLevelStatus_all_enum_values, TSchemaItemParameter<Common_DeviceLevelStatus::eType>());

  // Struct member primaryAudioSource.
  //
  // See PrimaryAudioSource.
  utils::SharedPtr<ISchemaItem> primaryAudioSource_SchemaItem = TEnumSchemaItem<Common_PrimaryAudioSource::eType>::create(Common_PrimaryAudioSource_all_enum_values, TSchemaItemParameter<Common_PrimaryAudioSource::eType>());

  // Struct member eCallEventActive.
  //
  // Must be true if emergency call event is active.
  utils::SharedPtr<ISchemaItem> eCallEventActive_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["voiceRecOn"] = CObjectSchemaItem::SMember(voiceRecOn_SchemaItem, false);
  schema_members["btIconOn"] = CObjectSchemaItem::SMember(btIconOn_SchemaItem, false);
  schema_members["callActive"] = CObjectSchemaItem::SMember(callActive_SchemaItem, false);
  schema_members["phoneRoaming"] = CObjectSchemaItem::SMember(phoneRoaming_SchemaItem, false);
  schema_members["textMsgAvailable"] = CObjectSchemaItem::SMember(textMsgAvailable_SchemaItem, false);
  schema_members["battLevelStatus"] = CObjectSchemaItem::SMember(battLevelStatus_SchemaItem, false);
  schema_members["stereoAudioOutputMuted"] = CObjectSchemaItem::SMember(stereoAudioOutputMuted_SchemaItem, false);
  schema_members["monoAudioOutputMuted"] = CObjectSchemaItem::SMember(monoAudioOutputMuted_SchemaItem, false);
  schema_members["signalLevelStatus"] = CObjectSchemaItem::SMember(signalLevelStatus_SchemaItem, false);
  schema_members["primaryAudioSource"] = CObjectSchemaItem::SMember(primaryAudioSource_SchemaItem, false);
  schema_members["eCallEventActive"] = CObjectSchemaItem::SMember(eCallEventActive_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_ECallInfo(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_VehicleDataNotificationStatus::eType> Common_VehicleDataNotificationStatus_all_enum_values;
  Common_VehicleDataNotificationStatus_all_enum_values.insert(Common_VehicleDataNotificationStatus::VDNS_NOT_SUPPORTED);
  Common_VehicleDataNotificationStatus_all_enum_values.insert(Common_VehicleDataNotificationStatus::VDNS_NORMAL);
  Common_VehicleDataNotificationStatus_all_enum_values.insert(Common_VehicleDataNotificationStatus::VDNS_ACTIVE);
  Common_VehicleDataNotificationStatus_all_enum_values.insert(Common_VehicleDataNotificationStatus::VDNS_NOT_USED);

  std::set<Common_ECallConfirmationStatus::eType> Common_ECallConfirmationStatus_all_enum_values;
  Common_ECallConfirmationStatus_all_enum_values.insert(Common_ECallConfirmationStatus::ECCS_NORMAL);
  Common_ECallConfirmationStatus_all_enum_values.insert(Common_ECallConfirmationStatus::ECCS_CALL_IN_PROGRESS);
  Common_ECallConfirmationStatus_all_enum_values.insert(Common_ECallConfirmationStatus::ECCS_CALL_CANCELLED);
  Common_ECallConfirmationStatus_all_enum_values.insert(Common_ECallConfirmationStatus::CALL_COMPLETED);
  Common_ECallConfirmationStatus_all_enum_values.insert(Common_ECallConfirmationStatus::ECCS_CALL_UNSUCCESSFUL);
  Common_ECallConfirmationStatus_all_enum_values.insert(Common_ECallConfirmationStatus::ECCS_ECALL_CONFIGURED_OFF);
  Common_ECallConfirmationStatus_all_enum_values.insert(Common_ECallConfirmationStatus::ECCS_CALL_COMPLETE_DTMF_TIMEOUT);

  // Struct member eCallNotificationStatus.
  //
  // References signal "eCallNotification_4A". See VehicleDataNotificationStatus.
  utils::SharedPtr<ISchemaItem> eCallNotificationStatus_SchemaItem = TEnumSchemaItem<Common_VehicleDataNotificationStatus::eType>::create(Common_VehicleDataNotificationStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataNotificationStatus::eType>());

  // Struct member auxECallNotificationStatus.
  //
  // References signal "eCallNotification". See VehicleDataNotificationStatus.
  utils::SharedPtr<ISchemaItem> auxECallNotificationStatus_SchemaItem = TEnumSchemaItem<Common_VehicleDataNotificationStatus::eType>::create(Common_VehicleDataNotificationStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataNotificationStatus::eType>());

  // Struct member eCallConfirmationStatus.
  //
  // References signal "eCallConfirmation". See ECallConfirmationStatus.
  utils::SharedPtr<ISchemaItem> eCallConfirmationStatus_SchemaItem = TEnumSchemaItem<Common_ECallConfirmationStatus::eType>::create(Common_ECallConfirmationStatus_all_enum_values, TSchemaItemParameter<Common_ECallConfirmationStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["eCallNotificationStatus"] = CObjectSchemaItem::SMember(eCallNotificationStatus_SchemaItem, true);
  schema_members["auxECallNotificationStatus"] = CObjectSchemaItem::SMember(auxECallNotificationStatus_SchemaItem, true);
  schema_members["eCallConfirmationStatus"] = CObjectSchemaItem::SMember(eCallConfirmationStatus_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_AirbagStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_VehicleDataEventStatus::eType> Common_VehicleDataEventStatus_all_enum_values;
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NO_EVENT);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NO);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_YES);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NOT_SUPPORTED);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_FAULT);

  // Struct member driverAirbagDeployed.
  //
  // References signal "VedsDrvBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverAirbagDeployed_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member driverSideAirbagDeployed.
  //
  // References signal "VedsDrvSideBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverSideAirbagDeployed_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member driverCurtainAirbagDeployed.
  //
  // References signal "VedsDrvCrtnBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverCurtainAirbagDeployed_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member passengerAirbagDeployed.
  //
  // References signal "VedsPasBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerAirbagDeployed_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member passengerCurtainAirbagDeployed.
  //
  // References signal "VedsPasCrtnBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerCurtainAirbagDeployed_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member driverKneeAirbagDeployed.
  //
  // References signal "VedsKneeDrvBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverKneeAirbagDeployed_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member passengerSideAirbagDeployed.
  //
  // References signal "VedsPasSideBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerSideAirbagDeployed_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member passengerKneeAirbagDeployed.
  //
  // References signal "VedsKneePasBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerKneeAirbagDeployed_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["driverAirbagDeployed"] = CObjectSchemaItem::SMember(driverAirbagDeployed_SchemaItem, true);
  schema_members["driverSideAirbagDeployed"] = CObjectSchemaItem::SMember(driverSideAirbagDeployed_SchemaItem, true);
  schema_members["driverCurtainAirbagDeployed"] = CObjectSchemaItem::SMember(driverCurtainAirbagDeployed_SchemaItem, true);
  schema_members["passengerAirbagDeployed"] = CObjectSchemaItem::SMember(passengerAirbagDeployed_SchemaItem, true);
  schema_members["passengerCurtainAirbagDeployed"] = CObjectSchemaItem::SMember(passengerCurtainAirbagDeployed_SchemaItem, true);
  schema_members["driverKneeAirbagDeployed"] = CObjectSchemaItem::SMember(driverKneeAirbagDeployed_SchemaItem, true);
  schema_members["passengerSideAirbagDeployed"] = CObjectSchemaItem::SMember(passengerSideAirbagDeployed_SchemaItem, true);
  schema_members["passengerKneeAirbagDeployed"] = CObjectSchemaItem::SMember(passengerKneeAirbagDeployed_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_EmergencyEvent(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_EmergencyEventType::eType> Common_EmergencyEventType_all_enum_values;
  Common_EmergencyEventType_all_enum_values.insert(Common_EmergencyEventType::EET_NO_EVENT);
  Common_EmergencyEventType_all_enum_values.insert(Common_EmergencyEventType::EET_FRONTAL);
  Common_EmergencyEventType_all_enum_values.insert(Common_EmergencyEventType::EET_SIDE);
  Common_EmergencyEventType_all_enum_values.insert(Common_EmergencyEventType::EET_REAR);
  Common_EmergencyEventType_all_enum_values.insert(Common_EmergencyEventType::EET_ROLLOVER);
  Common_EmergencyEventType_all_enum_values.insert(Common_EmergencyEventType::EET_NOT_SUPPORTED);
  Common_EmergencyEventType_all_enum_values.insert(Common_EmergencyEventType::EET_FAULT);

  std::set<Common_FuelCutoffStatus::eType> Common_FuelCutoffStatus_all_enum_values;
  Common_FuelCutoffStatus_all_enum_values.insert(Common_FuelCutoffStatus::FCS_TERMINATE_FUEL);
  Common_FuelCutoffStatus_all_enum_values.insert(Common_FuelCutoffStatus::FCS_NORMAL_OPERATION);
  Common_FuelCutoffStatus_all_enum_values.insert(Common_FuelCutoffStatus::FCS_FAULT);

  std::set<Common_VehicleDataEventStatus::eType> Common_VehicleDataEventStatus_all_enum_values;
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NO_EVENT);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NO);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_YES);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_NOT_SUPPORTED);
  Common_VehicleDataEventStatus_all_enum_values.insert(Common_VehicleDataEventStatus::VDES_FAULT);

  // Struct member emergencyEventType.
  //
  // References signal "VedsEvntType_D_Ltchd". See EmergencyEventType.
  utils::SharedPtr<ISchemaItem> emergencyEventType_SchemaItem = TEnumSchemaItem<Common_EmergencyEventType::eType>::create(Common_EmergencyEventType_all_enum_values, TSchemaItemParameter<Common_EmergencyEventType::eType>());

  // Struct member fuelCutoffStatus.
  //
  // References signal "RCM_FuelCutoff". See FuelCutoffStatus.
  utils::SharedPtr<ISchemaItem> fuelCutoffStatus_SchemaItem = TEnumSchemaItem<Common_FuelCutoffStatus::eType>::create(Common_FuelCutoffStatus_all_enum_values, TSchemaItemParameter<Common_FuelCutoffStatus::eType>());

  // Struct member rolloverEvent.
  //
  // References signal "VedsEvntRoll_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> rolloverEvent_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member maximumChangeVelocity.
  //
  // References signal "VedsMaxDeltaV_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> maximumChangeVelocity_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  // Struct member multipleEvents.
  //
  // References signal "VedsMultiEvnt_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> multipleEvents_SchemaItem = TEnumSchemaItem<Common_VehicleDataEventStatus::eType>::create(Common_VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataEventStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["emergencyEventType"] = CObjectSchemaItem::SMember(emergencyEventType_SchemaItem, true);
  schema_members["fuelCutoffStatus"] = CObjectSchemaItem::SMember(fuelCutoffStatus_SchemaItem, true);
  schema_members["rolloverEvent"] = CObjectSchemaItem::SMember(rolloverEvent_SchemaItem, true);
  schema_members["maximumChangeVelocity"] = CObjectSchemaItem::SMember(maximumChangeVelocity_SchemaItem, true);
  schema_members["multipleEvents"] = CObjectSchemaItem::SMember(multipleEvents_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_ClusterModeStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_PowerModeQualificationStatus::eType> Common_PowerModeQualificationStatus_all_enum_values;
  Common_PowerModeQualificationStatus_all_enum_values.insert(Common_PowerModeQualificationStatus::POWER_MODE_UNDEFINED);
  Common_PowerModeQualificationStatus_all_enum_values.insert(Common_PowerModeQualificationStatus::POWER_MODE_EVALUATION_IN_PROGRESS);
  Common_PowerModeQualificationStatus_all_enum_values.insert(Common_PowerModeQualificationStatus::NOT_DEFINED);
  Common_PowerModeQualificationStatus_all_enum_values.insert(Common_PowerModeQualificationStatus::POWER_MODE_OK);

  std::set<Common_CarModeStatus::eType> Common_CarModeStatus_all_enum_values;
  Common_CarModeStatus_all_enum_values.insert(Common_CarModeStatus::CMS_NORMAL);
  Common_CarModeStatus_all_enum_values.insert(Common_CarModeStatus::CMS_FACTORY);
  Common_CarModeStatus_all_enum_values.insert(Common_CarModeStatus::CMS_TRANSPORT);
  Common_CarModeStatus_all_enum_values.insert(Common_CarModeStatus::CMS_CRASH);

  std::set<Common_PowerModeStatus::eType> Common_PowerModeStatus_all_enum_values;
  Common_PowerModeStatus_all_enum_values.insert(Common_PowerModeStatus::KEY_OUT);
  Common_PowerModeStatus_all_enum_values.insert(Common_PowerModeStatus::KEY_RECENTLY_OUT);
  Common_PowerModeStatus_all_enum_values.insert(Common_PowerModeStatus::KEY_APPROVED_0);
  Common_PowerModeStatus_all_enum_values.insert(Common_PowerModeStatus::POST_ACCESORY_0);
  Common_PowerModeStatus_all_enum_values.insert(Common_PowerModeStatus::ACCESORY_1);
  Common_PowerModeStatus_all_enum_values.insert(Common_PowerModeStatus::POST_IGNITION_1);
  Common_PowerModeStatus_all_enum_values.insert(Common_PowerModeStatus::IGNITION_ON_2);
  Common_PowerModeStatus_all_enum_values.insert(Common_PowerModeStatus::RUNNING_2);
  Common_PowerModeStatus_all_enum_values.insert(Common_PowerModeStatus::CRANK_3);

  // Struct member powerModeActive.
  //
  // References signal "PowerMode_UB".
  utils::SharedPtr<ISchemaItem> powerModeActive_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member powerModeQualificationStatus.
  //
  // References signal "PowerModeQF". See PowerModeQualificationStatus.
  utils::SharedPtr<ISchemaItem> powerModeQualificationStatus_SchemaItem = TEnumSchemaItem<Common_PowerModeQualificationStatus::eType>::create(Common_PowerModeQualificationStatus_all_enum_values, TSchemaItemParameter<Common_PowerModeQualificationStatus::eType>());

  // Struct member carModeStatus.
  //
  // References signal "CarMode". See CarMode.
  utils::SharedPtr<ISchemaItem> carModeStatus_SchemaItem = TEnumSchemaItem<Common_CarModeStatus::eType>::create(Common_CarModeStatus_all_enum_values, TSchemaItemParameter<Common_CarModeStatus::eType>());

  // Struct member powerModeStatus.
  //
  // References signal "PowerMode". See PowerMode.
  utils::SharedPtr<ISchemaItem> powerModeStatus_SchemaItem = TEnumSchemaItem<Common_PowerModeStatus::eType>::create(Common_PowerModeStatus_all_enum_values, TSchemaItemParameter<Common_PowerModeStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["powerModeActive"] = CObjectSchemaItem::SMember(powerModeActive_SchemaItem, true);
  schema_members["powerModeQualificationStatus"] = CObjectSchemaItem::SMember(powerModeQualificationStatus_SchemaItem, true);
  schema_members["carModeStatus"] = CObjectSchemaItem::SMember(carModeStatus_SchemaItem, true);
  schema_members["powerModeStatus"] = CObjectSchemaItem::SMember(powerModeStatus_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> hmi_apis::HMI_API::InitStructSchemaItem_Common_MyKey(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Common_VehicleDataStatus::eType> Common_VehicleDataStatus_all_enum_values;
  Common_VehicleDataStatus_all_enum_values.insert(Common_VehicleDataStatus::VDS_NO_DATA_EXISTS);
  Common_VehicleDataStatus_all_enum_values.insert(Common_VehicleDataStatus::VDS_OFF);
  Common_VehicleDataStatus_all_enum_values.insert(Common_VehicleDataStatus::VDS_ON);

  // Struct member e911Override.
  //
  // Indicates whether e911 override is on. See VehicleDataStatus.
  utils::SharedPtr<ISchemaItem> e911Override_SchemaItem = TEnumSchemaItem<Common_VehicleDataStatus::eType>::create(Common_VehicleDataStatus_all_enum_values, TSchemaItemParameter<Common_VehicleDataStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["e911Override"] = CObjectSchemaItem::SMember(e911Override_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

//-------------- String to value enum mapping ----------------

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

template <>
const std::map<hmi_apis::FunctionID::eType, std::string> &TEnumSchemaItem<hmi_apis::FunctionID::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::FunctionID::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Buttons_GetCapabilities, "Buttons.GetCapabilities"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Buttons_OnButtonEvent, "Buttons.OnButtonEvent"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Buttons_OnButtonPress, "Buttons.OnButtonPress"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnReady, "BasicCommunication.OnReady"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnStartDeviceDiscovery, "BasicCommunication.OnStartDeviceDiscovery"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnResumeAudioSource, "BasicCommunication.OnResumeAudioSource"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList, "BasicCommunication.UpdateDeviceList"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnFileRemoved, "BasicCommunication.OnFileRemoved"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_AllowDeviceToConnect, "BasicCommunication.AllowDeviceToConnect"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnDeviceChosen, "BasicCommunication.OnDeviceChosen"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnFindApplications, "BasicCommunication.OnFindApplications"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_ActivateApp, "BasicCommunication.ActivateApp"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnAppActivated, "BasicCommunication.OnAppActivated"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnAppDeactivated, "BasicCommunication.OnAppDeactivated"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnAppRegistered, "BasicCommunication.OnAppRegistered"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered, "BasicCommunication.OnAppUnregistered"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnExitApplication, "BasicCommunication.OnExitApplication"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnExitAllApplications, "BasicCommunication.OnExitAllApplications"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_MixingAudioSupported, "BasicCommunication.MixingAudioSupported"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_PlayTone, "BasicCommunication.PlayTone"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnSystemRequest, "BasicCommunication.OnSystemRequest"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_SystemRequest, "BasicCommunication.SystemRequest"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_PolicyUpdate, "BasicCommunication.PolicyUpdate"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnSDLClose, "BasicCommunication.OnSDLClose"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnPutFile, "BasicCommunication.OnPutFile"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_GetSystemInfo, "BasicCommunication.GetSystemInfo"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnSystemInfoChanged, "BasicCommunication.OnSystemInfoChanged"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::BasicCommunication_OnIgnitionCycleOver, "BasicCommunication.OnIgnitionCycleOver"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_IsReady, "VR.IsReady"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_Started, "VR.Started"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_Stopped, "VR.Stopped"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_AddCommand, "VR.AddCommand"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_DeleteCommand, "VR.DeleteCommand"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_PerformInteraction, "VR.PerformInteraction"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_OnCommand, "VR.OnCommand"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_ChangeRegistration, "VR.ChangeRegistration"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_OnLanguageChange, "VR.OnLanguageChange"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_GetSupportedLanguages, "VR.GetSupportedLanguages"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_GetLanguage, "VR.GetLanguage"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VR_GetCapabilities, "VR.GetCapabilities"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::TTS_GetCapabilities, "TTS.GetCapabilities"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::TTS_Started, "TTS.Started"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::TTS_Stopped, "TTS.Stopped"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::TTS_IsReady, "TTS.IsReady"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::TTS_Speak, "TTS.Speak"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::TTS_StopSpeaking, "TTS.StopSpeaking"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::TTS_ChangeRegistration, "TTS.ChangeRegistration"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::TTS_OnLanguageChange, "TTS.OnLanguageChange"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::TTS_GetSupportedLanguages, "TTS.GetSupportedLanguages"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::TTS_GetLanguage, "TTS.GetLanguage"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::TTS_SetGlobalProperties, "TTS.SetGlobalProperties"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_Alert, "UI.Alert"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_Show, "UI.Show"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_AddCommand, "UI.AddCommand"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_DeleteCommand, "UI.DeleteCommand"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_AddSubMenu, "UI.AddSubMenu"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_DeleteSubMenu, "UI.DeleteSubMenu"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_PerformInteraction, "UI.PerformInteraction"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_SetMediaClockTimer, "UI.SetMediaClockTimer"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_SetGlobalProperties, "UI.SetGlobalProperties"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_OnCommand, "UI.OnCommand"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_OnSystemContext, "UI.OnSystemContext"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_GetCapabilities, "UI.GetCapabilities"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_ChangeRegistration, "UI.ChangeRegistration"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_OnLanguageChange, "UI.OnLanguageChange"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_GetSupportedLanguages, "UI.GetSupportedLanguages"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_GetLanguage, "UI.GetLanguage"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_OnDriverDistraction, "UI.OnDriverDistraction"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_SetAppIcon, "UI.SetAppIcon"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_SetDisplayLayout, "UI.SetDisplayLayout"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_ShowCustomForm, "UI.ShowCustomForm"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_OnKeyboardInput, "UI.OnKeyboardInput"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_OnTouchEvent, "UI.OnTouchEvent"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_Slider, "UI.Slider"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_ScrollableMessage, "UI.ScrollableMessage"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_PerformAudioPassThru, "UI.PerformAudioPassThru"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_EndAudioPassThru, "UI.EndAudioPassThru"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_IsReady, "UI.IsReady"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_ClosePopUp, "UI.ClosePopUp"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_OnResetTimeout, "UI.OnResetTimeout"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::UI_OnRecordStart, "UI.OnRecordStart"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Navigation_IsReady, "Navigation.IsReady"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Navigation_ShowConstantTBT, "Navigation.ShowConstantTBT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Navigation_AlertManeuver, "Navigation.AlertManeuver"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Navigation_UpdateTurnList, "Navigation.UpdateTurnList"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Navigation_OnTBTClientState, "Navigation.OnTBTClientState"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Navigation_StartStream, "Navigation.StartStream"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Navigation_StopStream, "Navigation.StopStream"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Navigation_StartAudioStream, "Navigation.StartAudioStream"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::Navigation_StopAudioStream, "Navigation.StopAudioStream"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VehicleInfo_IsReady, "VehicleInfo.IsReady"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VehicleInfo_GetVehicleType, "VehicleInfo.GetVehicleType"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VehicleInfo_ReadDID, "VehicleInfo.ReadDID"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VehicleInfo_GetDTCs, "VehicleInfo.GetDTCs"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VehicleInfo_DiagnosticMessage, "VehicleInfo.DiagnosticMessage"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData, "VehicleInfo.SubscribeVehicleData"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData, "VehicleInfo.UnsubscribeVehicleData"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VehicleInfo_GetVehicleData, "VehicleInfo.GetVehicleData"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::VehicleInfo_OnVehicleData, "VehicleInfo.OnVehicleData"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_ActivateApp, "SDL.ActivateApp"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_GetUserFriendlyMessage, "SDL.GetUserFriendlyMessage"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_OnAllowSDLFunctionality, "SDL.OnAllowSDLFunctionality"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_OnReceivedPolicyUpdate, "SDL.OnReceivedPolicyUpdate"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_OnPolicyUpdate, "SDL.OnPolicyUpdate"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_GetListOfPermissions, "SDL.GetListOfPermissions"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_OnAppPermissionConsent, "SDL.OnAppPermissionConsent"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_OnAppPermissionChanged, "SDL.OnAppPermissionChanged"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_OnSDLConsentNeeded, "SDL.OnSDLConsentNeeded"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_UpdateSDL, "SDL.UpdateSDL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_GetStatusUpdate, "SDL.GetStatusUpdate"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_OnStatusUpdate, "SDL.OnStatusUpdate"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_OnSystemError, "SDL.OnSystemError"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_AddStatisticsInfo, "SDL.AddStatisticsInfo"));
    enum_string_representation.insert(std::make_pair(hmi_apis::FunctionID::SDL_GetURLS, "SDL.GetURLS"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::messageType::eType, std::string> &TEnumSchemaItem<hmi_apis::messageType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::messageType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::messageType::request, "request"));
    enum_string_representation.insert(std::make_pair(hmi_apis::messageType::response, "response"));
    enum_string_representation.insert(std::make_pair(hmi_apis::messageType::notification, "notification"));
    enum_string_representation.insert(std::make_pair(hmi_apis::messageType::error_response, "error_response"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_Result::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_Result::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_Result::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::SUCCESS, "SUCCESS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::UNSUPPORTED_REQUEST, "UNSUPPORTED_REQUEST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE, "UNSUPPORTED_RESOURCE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::DISALLOWED, "DISALLOWED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::REJECTED, "REJECTED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::ABORTED, "ABORTED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::IGNORED, "IGNORED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::RETRY, "RETRY"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::IN_USE, "IN_USE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::DATA_NOT_AVAILABLE, "DATA_NOT_AVAILABLE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::TIMED_OUT, "TIMED_OUT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::INVALID_DATA, "INVALID_DATA"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::CHAR_LIMIT_EXCEEDED, "CHAR_LIMIT_EXCEEDED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::INVALID_ID, "INVALID_ID"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::DUPLICATE_NAME, "DUPLICATE_NAME"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::APPLICATION_NOT_REGISTERED, "APPLICATION_NOT_REGISTERED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::WRONG_LANGUAGE, "WRONG_LANGUAGE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::OUT_OF_MEMORY, "OUT_OF_MEMORY"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::TOO_MANY_PENDING_REQUESTS, "TOO_MANY_PENDING_REQUESTS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::NO_APPS_REGISTERED, "NO_APPS_REGISTERED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::NO_DEVICES_CONNECTED, "NO_DEVICES_CONNECTED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::WARNINGS, "WARNINGS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::GENERIC_ERROR, "GENERIC_ERROR"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Result::USER_DISALLOWED, "USER_DISALLOWED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_ButtonName::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_ButtonName::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_ButtonName::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::OK, "OK"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::SEEKLEFT, "SEEKLEFT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::SEEKRIGHT, "SEEKRIGHT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::TUNEUP, "TUNEUP"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::TUNEDOWN, "TUNEDOWN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::PRESET_0, "PRESET_0"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::PRESET_1, "PRESET_1"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::PRESET_2, "PRESET_2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::PRESET_3, "PRESET_3"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::PRESET_4, "PRESET_4"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::PRESET_5, "PRESET_5"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::PRESET_6, "PRESET_6"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::PRESET_7, "PRESET_7"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::PRESET_8, "PRESET_8"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::PRESET_9, "PRESET_9"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::CUSTOM_BUTTON, "CUSTOM_BUTTON"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonName::SEARCH, "SEARCH"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_ButtonEventMode::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_ButtonEventMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_ButtonEventMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonEventMode::BUTTONUP, "BUTTONUP"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonEventMode::BUTTONDOWN, "BUTTONDOWN"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_ButtonPressMode::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_ButtonPressMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_ButtonPressMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonPressMode::LONG, "LONG"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ButtonPressMode::SHORT, "SHORT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_Language::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_Language::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_Language::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::EN_US, "EN-US"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::ES_MX, "ES-MX"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::FR_CA, "FR-CA"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::DE_DE, "DE-DE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::ES_ES, "ES-ES"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::EN_GB, "EN-GB"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::RU_RU, "RU-RU"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::TR_TR, "TR-TR"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::PL_PL, "PL-PL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::FR_FR, "FR-FR"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::IT_IT, "IT-IT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::SV_SE, "SV-SE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::PT_PT, "PT-PT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::NL_NL, "NL-NL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::EN_AU, "EN-AU"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::ZH_CN, "ZH-CN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::ZH_TW, "ZH-TW"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::JA_JP, "JA-JP"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::AR_SA, "AR-SA"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::KO_KR, "KO-KR"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::PT_BR, "PT-BR"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::CS_CZ, "CS-CZ"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::DA_DK, "DA-DK"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Language::NO_NO, "NO-NO"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_SoftButtonType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_SoftButtonType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_SoftButtonType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SoftButtonType::SBT_TEXT, "TEXT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SoftButtonType::SBT_IMAGE, "IMAGE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SoftButtonType::SBT_BOTH, "BOTH"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_SystemAction::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_SystemAction::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_SystemAction::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SystemAction::DEFAULT_ACTION, "DEFAULT_ACTION"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SystemAction::STEAL_FOCUS, "STEAL_FOCUS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SystemAction::KEEP_CONTEXT, "KEEP_CONTEXT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_AppHMIType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_AppHMIType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_AppHMIType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppHMIType::DEFAULT, "DEFAULT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppHMIType::COMMUNICATION, "COMMUNICATION"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppHMIType::MEDIA, "MEDIA"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppHMIType::MESSAGING, "MESSAGING"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppHMIType::NAVIGATION, "NAVIGATION"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppHMIType::INFORMATION, "INFORMATION"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppHMIType::SOCIAL, "SOCIAL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppHMIType::BACKGROUND_PROCESS, "BACKGROUND_PROCESS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppHMIType::TESTING, "TESTING"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppHMIType::SYSTEM, "SYSTEM"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_LayoutMode::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_LayoutMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_LayoutMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_LayoutMode::ICON_ONLY, "ICON_ONLY"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_LayoutMode::ICON_WITH_SEARCH, "ICON_WITH_SEARCH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_LayoutMode::LIST_ONLY, "LIST_ONLY"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_LayoutMode::LIST_WITH_SEARCH, "LIST_WITH_SEARCH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_LayoutMode::KEYBOARD, "KEYBOARD"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_DeactivateReason::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_DeactivateReason::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_DeactivateReason::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeactivateReason::AUDIO, "AUDIO"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeactivateReason::PHONECALL, "PHONECALL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeactivateReason::NAVIGATIONMAP, "NAVIGATIONMAP"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeactivateReason::PHONEMENU, "PHONEMENU"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeactivateReason::SYNCSETTINGS, "SYNCSETTINGS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeactivateReason::GENERAL, "GENERAL"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_ClockUpdateMode::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_ClockUpdateMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_ClockUpdateMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ClockUpdateMode::COUNTUP, "COUNTUP"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ClockUpdateMode::COUNTDOWN, "COUNTDOWN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ClockUpdateMode::PAUSE, "PAUSE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ClockUpdateMode::RESUME, "RESUME"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ClockUpdateMode::CLEAR, "CLEAR"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_SystemContext::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_SystemContext::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_SystemContext::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SystemContext::SYSCTXT_MAIN, "MAIN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SystemContext::SYSCTXT_VRSESSION, "VRSESSION"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SystemContext::SYSCTXT_MENU, "MENU"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SystemContext::SYSCTXT_HMI_OBSCURED, "HMI_OBSCURED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SystemContext::SYSCTXT_ALERT, "ALERT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_HmiZoneCapabilities::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_HmiZoneCapabilities::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_HmiZoneCapabilities::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_HmiZoneCapabilities::FRONT, "FRONT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_HmiZoneCapabilities::BACK, "BACK"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_SpeechCapabilities::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_SpeechCapabilities::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_SpeechCapabilities::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SpeechCapabilities::SC_TEXT, "TEXT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SpeechCapabilities::SAPI_PHONEMES, "SAPI_PHONEMES"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SpeechCapabilities::LHPLUS_PHONEMES, "LHPLUS_PHONEMES"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SpeechCapabilities::PRE_RECORDED, "PRE_RECORDED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SpeechCapabilities::SILENCE, "SILENCE"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_VrCapabilities::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_VrCapabilities::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_VrCapabilities::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VrCapabilities::VR_TEXT, "TEXT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_PrerecordedSpeech::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_PrerecordedSpeech::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_PrerecordedSpeech::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrerecordedSpeech::HELP_JINGLE, "HELP_JINGLE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrerecordedSpeech::INITIAL_JINGLE, "INITIAL_JINGLE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrerecordedSpeech::LISTEN_JINGLE, "LISTEN_JINGLE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrerecordedSpeech::POSITIVE_JINGLE, "POSITIVE_JINGLE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrerecordedSpeech::NEGATIVE_JINGLE, "NEGATIVE_JINGLE"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_TBTState::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_TBTState::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_TBTState::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TBTState::ROUTE_UPDATE_REQUEST, "ROUTE_UPDATE_REQUEST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TBTState::ROUTE_ACCEPTED, "ROUTE_ACCEPTED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TBTState::ROUTE_REFUSED, "ROUTE_REFUSED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TBTState::ROUTE_CANCELLED, "ROUTE_CANCELLED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TBTState::ETA_REQUEST, "ETA_REQUEST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TBTState::NEXT_TURN_REQUEST, "NEXT_TURN_REQUEST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TBTState::ROUTE_STATUS_REQUEST, "ROUTE_STATUS_REQUEST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TBTState::ROUTE_SUMMARY_REQUEST, "ROUTE_SUMMARY_REQUEST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TBTState::TRIP_STATUS_REQUEST, "TRIP_STATUS_REQUEST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TBTState::ROUTE_UPDATE_REQUEST_TIMEOUT, "ROUTE_UPDATE_REQUEST_TIMEOUT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_ApplicationsCloseReason::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_ApplicationsCloseReason::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_ApplicationsCloseReason::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ApplicationsCloseReason::IGNITION_OFF, "IGNITION_OFF"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ApplicationsCloseReason::MASTER_RESET, "MASTER_RESET"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ApplicationsCloseReason::FACTORY_DEFAULTS, "FACTORY_DEFAULTS"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_DisplayType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_DisplayType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_DisplayType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DisplayType::CID, "CID"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DisplayType::TYPE2, "TYPE2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DisplayType::TYPE5, "TYPE5"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DisplayType::NGN, "NGN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DisplayType::GEN2_8_DMA, "GEN2_8_DMA"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DisplayType::GEN2_6_DMA, "GEN2_6_DMA"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DisplayType::MFD3, "MFD3"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DisplayType::MFD4, "MFD4"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DisplayType::MFD5, "MFD5"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DisplayType::GEN3_8_INCH, "GEN3_8-INCH"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_ImageType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_ImageType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_ImageType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageType::STATIC, "STATIC"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageType::DYNAMIC, "DYNAMIC"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_TextFieldName::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_TextFieldName::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_TextFieldName::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::mainField1, "mainField1"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::mainField2, "mainField2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::mainField3, "mainField3"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::mainField4, "mainField4"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::statusBar, "statusBar"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::mediaClock, "mediaClock"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::mediaTrack, "mediaTrack"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::alertText1, "alertText1"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::alertText2, "alertText2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::alertText3, "alertText3"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::scrollableMessageBody, "scrollableMessageBody"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::initialInteractionText, "initialInteractionText"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::navigationText1, "navigationText1"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::navigationText2, "navigationText2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::ETA, "ETA"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::totalDistance, "totalDistance"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::navigationText, "navigationText"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::audioPassThruDisplayText1, "audioPassThruDisplayText1"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::audioPassThruDisplayText2, "audioPassThruDisplayText2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::sliderHeader, "sliderHeader"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::sliderFooter, "sliderFooter"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::notificationText, "notificationText"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::menuName, "menuName"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::secondaryText, "secondaryText"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::tertiaryText, "tertiaryText"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::menuTitle, "menuTitle"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::timeToDestination, "timeToDestination"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextFieldName::turnText, "turnText"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_ImageFieldName::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_ImageFieldName::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_ImageFieldName::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageFieldName::softButtonImage, "softButtonImage"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageFieldName::choiceImage, "choiceImage"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageFieldName::choiceSecondaryImage, "choiceSecondaryImage"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageFieldName::vrHelpItem, "vrHelpItem"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageFieldName::turnIcon, "turnIcon"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageFieldName::menuIcon, "menuIcon"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageFieldName::cmdIcon, "cmdIcon"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageFieldName::appIcon, "appIcon"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageFieldName::graphic, "graphic"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageFieldName::showConstantTBTIcon, "showConstantTBTIcon"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ImageFieldName::showConstantTBTNextTurnIcon, "showConstantTBTNextTurnIcon"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_TextAlignment::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_TextAlignment::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_TextAlignment::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextAlignment::LEFT_ALIGNED, "LEFT_ALIGNED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextAlignment::RIGHT_ALIGNED, "RIGHT_ALIGNED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TextAlignment::CENTERED, "CENTERED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_DriverDistractionState::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_DriverDistractionState::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_DriverDistractionState::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DriverDistractionState::DD_ON, "DD_ON"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DriverDistractionState::DD_OFF, "DD_OFF"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_MediaClockFormat::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_MediaClockFormat::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_MediaClockFormat::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_MediaClockFormat::CLOCK1, "CLOCK1"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_MediaClockFormat::CLOCK2, "CLOCK2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_MediaClockFormat::CLOCK3, "CLOCK3"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_MediaClockFormat::CLOCKTEXT1, "CLOCKTEXT1"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_MediaClockFormat::CLOCKTEXT2, "CLOCKTEXT2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_MediaClockFormat::CLOCKTEXT3, "CLOCKTEXT3"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_MediaClockFormat::CLOCKTEXT4, "CLOCKTEXT4"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_VRCommandType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_VRCommandType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_VRCommandType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VRCommandType::Choice, "Choice"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VRCommandType::Command, "Command"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_ComponentVolumeStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_ComponentVolumeStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_ComponentVolumeStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ComponentVolumeStatus::CVS_UNKNOWN, "UNKNOWN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ComponentVolumeStatus::CVS_NORMAL, "NORMAL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ComponentVolumeStatus::CVS_LOW, "LOW"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ComponentVolumeStatus::CVS_FAULT, "FAULT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ComponentVolumeStatus::CVS_ALERT, "ALERT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ComponentVolumeStatus::CVS_NOT_SUPPORTED, "NOT_SUPPORTED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_PRNDL::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_PRNDL::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_PRNDL::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::PARK, "PARK"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::REVERSE, "REVERSE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::NEUTRAL, "NEUTRAL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::DRIVE, "DRIVE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::SPORT, "SPORT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::LOWGEAR, "LOWGEAR"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::FIRST, "FIRST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::SECOND, "SECOND"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::THIRD, "THIRD"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::FOURTH, "FOURTH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::FIFTH, "FIFTH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::SIXTH, "SIXTH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::SEVENTH, "SEVENTH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::EIGHTH, "EIGHTH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PRNDL::FAULT, "FAULT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_VehicleDataEventStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_VehicleDataEventStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_VehicleDataEventStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataEventStatus::VDES_NO_EVENT, "NO_EVENT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataEventStatus::VDES_NO, "NO"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataEventStatus::VDES_YES, "YES"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataEventStatus::VDES_NOT_SUPPORTED, "NOT_SUPPORTED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataEventStatus::VDES_FAULT, "FAULT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_VehicleDataStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_VehicleDataStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_VehicleDataStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataStatus::VDS_NO_DATA_EXISTS, "NO_DATA_EXISTS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataStatus::VDS_OFF, "OFF"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataStatus::VDS_ON, "ON"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_IgnitionStableStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_IgnitionStableStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_IgnitionStableStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_IgnitionStableStatus::IGNITION_SWITCH_NOT_STABLE, "IGNITION_SWITCH_NOT_STABLE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_IgnitionStableStatus::IGNITION_SWITCH_STABLE, "IGNITION_SWITCH_STABLE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_IgnitionStableStatus::MISSING_FROM_TRANSMITTER, "MISSING_FROM_TRANSMITTER"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_IgnitionStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_IgnitionStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_IgnitionStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_IgnitionStatus::IS_UNKNOWN, "UNKNOWN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_IgnitionStatus::IS_OFF, "OFF"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_IgnitionStatus::IS_ACCESSORY, "ACCESSORY"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_IgnitionStatus::IS_RUN, "RUN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_IgnitionStatus::IS_START, "START"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_IgnitionStatus::IS_INVALID, "INVALID"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_DeviceLevelStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_DeviceLevelStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_DeviceLevelStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeviceLevelStatus::ZERO_LEVEL_BARS, "ZERO_LEVEL_BARS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeviceLevelStatus::ONE_LEVEL_BARS, "ONE_LEVEL_BARS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeviceLevelStatus::TWO_LEVEL_BARS, "TWO_LEVEL_BARS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeviceLevelStatus::THREE_LEVEL_BARS, "THREE_LEVEL_BARS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeviceLevelStatus::FOUR_LEVEL_BARS, "FOUR_LEVEL_BARS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_DeviceLevelStatus::NOT_PROVIDED, "NOT_PROVIDED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_PrimaryAudioSource::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_PrimaryAudioSource::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_PrimaryAudioSource::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrimaryAudioSource::NO_SOURCE_SELECTED, "NO_SOURCE_SELECTED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrimaryAudioSource::USB, "USB"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrimaryAudioSource::USB2, "USB2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrimaryAudioSource::BLUETOOTH_STEREO_BTST, "BLUETOOTH_STEREO_BTST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrimaryAudioSource::LINE_IN, "LINE_IN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrimaryAudioSource::IPOD, "IPOD"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PrimaryAudioSource::MOBILE_APP, "MOBILE_APP"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_CompassDirection::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_CompassDirection::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_CompassDirection::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CompassDirection::NORTH, "NORTH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CompassDirection::NORTHWEST, "NORTHWEST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CompassDirection::WEST, "WEST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CompassDirection::SOUTHWEST, "SOUTHWEST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CompassDirection::SOUTH, "SOUTH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CompassDirection::SOUTHEAST, "SOUTHEAST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CompassDirection::EAST, "EAST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CompassDirection::NORTHEAST, "NORTHEAST"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_Dimension::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_Dimension::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_Dimension::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Dimension::Dimension_NO_FIX, "NO_FIX"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Dimension::Dimension_2D, "2D"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_Dimension::Dimension_3D, "3D"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_TouchType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_TouchType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_TouchType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TouchType::BEGIN, "BEGIN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TouchType::MOVE, "MOVE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_TouchType::END, "END"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_WarningLightStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_WarningLightStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_WarningLightStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WarningLightStatus::WLS_OFF, "OFF"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WarningLightStatus::WLS_ON, "ON"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WarningLightStatus::WLS_FLASH, "FLASH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WarningLightStatus::WLS_NOT_USED, "NOT_USED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_VehicleDataResultCode::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_VehicleDataResultCode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_VehicleDataResultCode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS, "SUCCESS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataResultCode::VDRC_TRUNCATED_DATA, "TRUNCATED_DATA"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataResultCode::VDRC_DISALLOWED, "DISALLOWED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataResultCode::VDRC_USER_DISALLOWED, "USER_DISALLOWED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataResultCode::VDRC_INVALID_ID, "INVALID_ID"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_NOT_AVAILABLE, "VEHICLE_DATA_NOT_AVAILABLE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED, "DATA_ALREADY_SUBSCRIBED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED, "DATA_NOT_SUBSCRIBED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataResultCode::VDRC_IGNORED, "IGNORED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_VehicleDataType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_VehicleDataType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_VehicleDataType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_GPS, "VEHICLEDATA_GPS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_SPEED, "VEHICLEDATA_SPEED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_RPM, "VEHICLEDATA_RPM"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_FUELLEVEL, "VEHICLEDATA_FUELLEVEL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_FUELLEVEL_STATE, "VEHICLEDATA_FUELLEVEL_STATE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_FUELCONSUMPTION, "VEHICLEDATA_FUELCONSUMPTION"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_EXTERNTEMP, "VEHICLEDATA_EXTERNTEMP"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_VIN, "VEHICLEDATA_VIN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_PRNDL, "VEHICLEDATA_PRNDL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_TIREPRESSURE, "VEHICLEDATA_TIREPRESSURE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_ODOMETER, "VEHICLEDATA_ODOMETER"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_BELTSTATUS, "VEHICLEDATA_BELTSTATUS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_BODYINFO, "VEHICLEDATA_BODYINFO"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_DEVICESTATUS, "VEHICLEDATA_DEVICESTATUS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_ECALLINFO, "VEHICLEDATA_ECALLINFO"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_AIRBAGSTATUS, "VEHICLEDATA_AIRBAGSTATUS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_EMERGENCYEVENT, "VEHICLEDATA_EMERGENCYEVENT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_CLUSTERMODESTATUS, "VEHICLEDATA_CLUSTERMODESTATUS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_MYKEY, "VEHICLEDATA_MYKEY"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_BRAKING, "VEHICLEDATA_BRAKING"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_WIPERSTATUS, "VEHICLEDATA_WIPERSTATUS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_HEADLAMPSTATUS, "VEHICLEDATA_HEADLAMPSTATUS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_BATTVOLTAGE, "VEHICLEDATA_BATTVOLTAGE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_ENGINETORQUE, "VEHICLEDATA_ENGINETORQUE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_ACCPEDAL, "VEHICLEDATA_ACCPEDAL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataType::VEHICLEDATA_STEERINGWHEEL, "VEHICLEDATA_STEERINGWHEEL"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_WiperStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_WiperStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_WiperStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::OFF, "OFF"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::AUTO_OFF, "AUTO_OFF"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::OFF_MOVING, "OFF_MOVING"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::MAN_INT_OFF, "MAN_INT_OFF"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::MAN_INT_ON, "MAN_INT_ON"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::MAN_LOW, "MAN_LOW"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::MAN_HIGH, "MAN_HIGH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::MAN_FLICK, "MAN_FLICK"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::WASH, "WASH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::AUTO_LOW, "AUTO_LOW"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::AUTO_HIGH, "AUTO_HIGH"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::COURTESYWIPE, "COURTESYWIPE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::AUTO_ADJUST, "AUTO_ADJUST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::STALLED, "STALLED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_WiperStatus::NO_DATA_EXISTS, "NO_DATA_EXISTS"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_SamplingRate::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_SamplingRate::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_SamplingRate::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SamplingRate::RATE_8KHZ, "8KHZ"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SamplingRate::RATE_16KHZ, "16KHZ"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SamplingRate::RATE_22KHZ, "22KHZ"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SamplingRate::RATE_44KHZ, "44KHZ"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_BitsPerSample::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_BitsPerSample::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_BitsPerSample::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_BitsPerSample::RATE_8_BIT, "8_BIT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_BitsPerSample::RATE_16_BIT, "16_BIT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_AudioType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_AudioType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_AudioType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AudioType::PCM, "PCM"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_KeyboardLayout::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_KeyboardLayout::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_KeyboardLayout::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_KeyboardLayout::QWERTY, "QWERTY"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_KeyboardLayout::QWERTZ, "QWERTZ"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_KeyboardLayout::AZERTY, "AZERTY"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_KeyboardEvent::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_KeyboardEvent::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_KeyboardEvent::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_KeyboardEvent::KEYPRESS, "KEYPRESS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_KeyboardEvent::ENTRY_SUBMITTED, "ENTRY_SUBMITTED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_KeyboardEvent::ENTRY_CANCELLED, "ENTRY_CANCELLED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_KeyboardEvent::ENTRY_ABORTED, "ENTRY_ABORTED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_KeypressMode::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_KeypressMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_KeypressMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_KeypressMode::SINGLE_KEYPRESS, "SINGLE_KEYPRESS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_KeypressMode::QUEUE_KEYPRESSES, "QUEUE_KEYPRESSES"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_KeypressMode::RESEND_CURRENT_ENTRY, "RESEND_CURRENT_ENTRY"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_AmbientLightStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_AmbientLightStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_AmbientLightStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AmbientLightStatus::NIGHT, "NIGHT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AmbientLightStatus::TWILIGHT_1, "TWILIGHT_1"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AmbientLightStatus::TWILIGHT_2, "TWILIGHT_2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AmbientLightStatus::TWILIGHT_3, "TWILIGHT_3"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AmbientLightStatus::TWILIGHT_4, "TWILIGHT_4"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AmbientLightStatus::DAY, "DAY"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AmbientLightStatus::UNKNOWN, "UNKNOWN"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AmbientLightStatus::INVALID, "INVALID"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_FileType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_FileType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_FileType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_FileType::GRAPHIC_BMP, "GRAPHIC_BMP"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_FileType::GRAPHIC_JPEG, "GRAPHIC_JPEG"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_FileType::GRAPHIC_PNG, "GRAPHIC_PNG"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_FileType::AUDIO_WAVE, "AUDIO_WAVE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_FileType::AUDIO_MP3, "AUDIO_MP3"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_FileType::AUDIO_AAC, "AUDIO_AAC"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_FileType::BINARY, "BINARY"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_FileType::JSON, "JSON"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_RequestType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_RequestType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_RequestType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_RequestType::HTTP, "HTTP"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_RequestType::FILE_RESUME, "FILE_RESUME"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_RequestType::AUTH_REQUEST, "AUTH_REQUEST"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_RequestType::AUTH_CHALLENGE, "AUTH_CHALLENGE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_RequestType::AUTH_ACK, "AUTH_ACK"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_RequestType::PROPRIETARY, "PROPRIETARY"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_ECallConfirmationStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_ECallConfirmationStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_ECallConfirmationStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ECallConfirmationStatus::ECCS_NORMAL, "NORMAL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ECallConfirmationStatus::ECCS_CALL_IN_PROGRESS, "CALL_IN_PROGRESS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ECallConfirmationStatus::ECCS_CALL_CANCELLED, "CALL_CANCELLED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ECallConfirmationStatus::CALL_COMPLETED, "CALL_COMPLETED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ECallConfirmationStatus::ECCS_CALL_UNSUCCESSFUL, "CALL_UNSUCCESSFUL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ECallConfirmationStatus::ECCS_ECALL_CONFIGURED_OFF, "ECALL_CONFIGURED_OFF"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ECallConfirmationStatus::ECCS_CALL_COMPLETE_DTMF_TIMEOUT, "CALL_COMPLETE_DTMF_TIMEOUT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_VehicleDataNotificationStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_VehicleDataNotificationStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_VehicleDataNotificationStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataNotificationStatus::VDNS_NOT_SUPPORTED, "NOT_SUPPORTED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataNotificationStatus::VDNS_NORMAL, "NORMAL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataNotificationStatus::VDNS_ACTIVE, "ACTIVE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_VehicleDataNotificationStatus::VDNS_NOT_USED, "NOT_USED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_EmergencyEventType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_EmergencyEventType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_EmergencyEventType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_EmergencyEventType::EET_NO_EVENT, "NO_EVENT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_EmergencyEventType::EET_FRONTAL, "FRONTAL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_EmergencyEventType::EET_SIDE, "SIDE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_EmergencyEventType::EET_REAR, "REAR"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_EmergencyEventType::EET_ROLLOVER, "ROLLOVER"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_EmergencyEventType::EET_NOT_SUPPORTED, "NOT_SUPPORTED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_EmergencyEventType::EET_FAULT, "FAULT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_FuelCutoffStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_FuelCutoffStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_FuelCutoffStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_FuelCutoffStatus::FCS_TERMINATE_FUEL, "TERMINATE_FUEL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_FuelCutoffStatus::FCS_NORMAL_OPERATION, "NORMAL_OPERATION"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_FuelCutoffStatus::FCS_FAULT, "FAULT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_PowerModeQualificationStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_PowerModeQualificationStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_PowerModeQualificationStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeQualificationStatus::POWER_MODE_UNDEFINED, "POWER_MODE_UNDEFINED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeQualificationStatus::POWER_MODE_EVALUATION_IN_PROGRESS, "POWER_MODE_EVALUATION_IN_PROGRESS"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeQualificationStatus::NOT_DEFINED, "NOT_DEFINED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeQualificationStatus::POWER_MODE_OK, "POWER_MODE_OK"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_CarModeStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_CarModeStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_CarModeStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CarModeStatus::CMS_NORMAL, "NORMAL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CarModeStatus::CMS_FACTORY, "FACTORY"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CarModeStatus::CMS_TRANSPORT, "TRANSPORT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CarModeStatus::CMS_CRASH, "CRASH"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_PowerModeStatus::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_PowerModeStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_PowerModeStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeStatus::KEY_OUT, "KEY_OUT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeStatus::KEY_RECENTLY_OUT, "KEY_RECENTLY_OUT"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeStatus::KEY_APPROVED_0, "KEY_APPROVED_0"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeStatus::POST_ACCESORY_0, "POST_ACCESORY_0"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeStatus::ACCESORY_1, "ACCESORY_1"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeStatus::POST_IGNITION_1, "POST_IGNITION_1"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeStatus::IGNITION_ON_2, "IGNITION_ON_2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeStatus::RUNNING_2, "RUNNING_2"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_PowerModeStatus::CRANK_3, "CRANK_3"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_CharacterSet::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_CharacterSet::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_CharacterSet::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CharacterSet::TYPE2SET, "TYPE2SET"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CharacterSet::TYPE5SET, "TYPE5SET"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CharacterSet::CID1SET, "CID1SET"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_CharacterSet::CID2SET, "CID2SET"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_AppPriority::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_AppPriority::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_AppPriority::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppPriority::EMERGENCY, "EMERGENCY"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppPriority::NAVIGATION, "NAVIGATION"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppPriority::VOICE_COMMUNICATION, "VOICE_COMMUNICATION"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppPriority::COMMUNICATION, "COMMUNICATION"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppPriority::NORMAL, "NORMAL"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_AppPriority::NONE, "NONE"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_UpdateResult::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_UpdateResult::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_UpdateResult::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_UpdateResult::UP_TO_DATE, "UP_TO_DATE"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_UpdateResult::UPDATING, "UPDATING"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_UpdateResult::UPDATE_NEEDED, "UPDATE_NEEDED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_SystemError::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_SystemError::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_SystemError::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SystemError::SYNC_REBOOTED, "SYNC_REBOOTED"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_SystemError::SYNC_OUT_OF_MEMMORY, "SYNC_OUT_OF_MEMMORY"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_StatisticsType::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_StatisticsType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_StatisticsType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_StatisticsType::iAPP_BUFFER_FULL, "iAPP_BUFFER_FULL"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<hmi_apis::Common_ConsentSource::eType, std::string> &TEnumSchemaItem<hmi_apis::Common_ConsentSource::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<hmi_apis::Common_ConsentSource::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ConsentSource::GUI, "GUI"));
    enum_string_representation.insert(std::make_pair(hmi_apis::Common_ConsentSource::VUI, "VUI"));

    is_initialized = true;
  }

  return enum_string_representation;
}

} // NsSmartObjects
} // NsSmartDeviceLink

