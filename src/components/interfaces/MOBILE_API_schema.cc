/**
 * @file MOBILE_API.cc
 * @brief Generated class MOBILE_API source file.
 *
 * This class is a part of SmartObjects solution. It provides
 * factory functionallity which allows client to use SmartSchemas
 * in accordance with definitions from MOBILE_API.xml file
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

#include "MOBILE_API_schema.h"
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

mobile_apis::MOBILE_API::MOBILE_API()
 : NsSmartDeviceLink::NsJSONHandler::CSmartFactory<FunctionID::eType, messageType::eType, StructIdentifiers::eType>() {
  TStructsSchemaItems struct_schema_items;
  InitStructSchemes(struct_schema_items);

  std::set<FunctionID::eType> function_id_items;
  function_id_items.insert(FunctionID::RESERVED);
  function_id_items.insert(FunctionID::RegisterAppInterfaceID);
  function_id_items.insert(FunctionID::UnregisterAppInterfaceID);
  function_id_items.insert(FunctionID::SetGlobalPropertiesID);
  function_id_items.insert(FunctionID::ResetGlobalPropertiesID);
  function_id_items.insert(FunctionID::AddCommandID);
  function_id_items.insert(FunctionID::DeleteCommandID);
  function_id_items.insert(FunctionID::AddSubMenuID);
  function_id_items.insert(FunctionID::DeleteSubMenuID);
  function_id_items.insert(FunctionID::CreateInteractionChoiceSetID);
  function_id_items.insert(FunctionID::PerformInteractionID);
  function_id_items.insert(FunctionID::DeleteInteractionChoiceSetID);
  function_id_items.insert(FunctionID::AlertID);
  function_id_items.insert(FunctionID::ShowID);
  function_id_items.insert(FunctionID::SpeakID);
  function_id_items.insert(FunctionID::SetMediaClockTimerID);
  function_id_items.insert(FunctionID::PerformAudioPassThruID);
  function_id_items.insert(FunctionID::EndAudioPassThruID);
  function_id_items.insert(FunctionID::SubscribeButtonID);
  function_id_items.insert(FunctionID::UnsubscribeButtonID);
  function_id_items.insert(FunctionID::SubscribeVehicleDataID);
  function_id_items.insert(FunctionID::UnsubscribeVehicleDataID);
  function_id_items.insert(FunctionID::GetVehicleDataID);
  function_id_items.insert(FunctionID::ReadDIDID);
  function_id_items.insert(FunctionID::GetDTCsID);
  function_id_items.insert(FunctionID::ScrollableMessageID);
  function_id_items.insert(FunctionID::SliderID);
  function_id_items.insert(FunctionID::ShowConstantTBTID);
  function_id_items.insert(FunctionID::AlertManeuverID);
  function_id_items.insert(FunctionID::UpdateTurnListID);
  function_id_items.insert(FunctionID::ChangeRegistrationID);
  function_id_items.insert(FunctionID::GenericResponseID);
  function_id_items.insert(FunctionID::PutFileID);
  function_id_items.insert(FunctionID::DeleteFileID);
  function_id_items.insert(FunctionID::ListFilesID);
  function_id_items.insert(FunctionID::SetAppIconID);
  function_id_items.insert(FunctionID::SetDisplayLayoutID);
  function_id_items.insert(FunctionID::DiagnosticMessageID);
  function_id_items.insert(FunctionID::SystemRequestID);
  function_id_items.insert(FunctionID::OnHMIStatusID);
  function_id_items.insert(FunctionID::OnAppInterfaceUnregisteredID);
  function_id_items.insert(FunctionID::OnButtonEventID);
  function_id_items.insert(FunctionID::OnButtonPressID);
  function_id_items.insert(FunctionID::OnVehicleDataID);
  function_id_items.insert(FunctionID::OnCommandID);
  function_id_items.insert(FunctionID::OnTBTClientStateID);
  function_id_items.insert(FunctionID::OnDriverDistractionID);
  function_id_items.insert(FunctionID::OnPermissionsChangeID);
  function_id_items.insert(FunctionID::OnAudioPassThruID);
  function_id_items.insert(FunctionID::OnLanguageChangeID);
  function_id_items.insert(FunctionID::OnKeyboardInputID);
  function_id_items.insert(FunctionID::OnTouchEventID);
  function_id_items.insert(FunctionID::OnSystemRequestID);
  function_id_items.insert(FunctionID::OnHashChangeID);
  function_id_items.insert(FunctionID::EncodedSyncPDataID);
  function_id_items.insert(FunctionID::SyncPDataID);
  function_id_items.insert(FunctionID::OnEncodedSyncPDataID);
  function_id_items.insert(FunctionID::OnSyncPDataID);

  std::set<messageType::eType> message_type_items;
  message_type_items.insert(messageType::request);
  message_type_items.insert(messageType::response);
  message_type_items.insert(messageType::notification);

  InitFunctionSchemes(struct_schema_items, function_id_items, message_type_items);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::ProvideObjectSchemaItemForStruct(
    const TStructsSchemaItems &struct_schema_items,
    const StructIdentifiers::eType struct_id) {
  const TStructsSchemaItems::const_iterator it = struct_schema_items.find(struct_id);
  if (it != struct_schema_items.end()) {
    return it->second;
  }

  return NsSmartDeviceLink::NsSmartObjects::CAlwaysFalseSchemaItem::create();
}

void mobile_apis::MOBILE_API::InitStructSchemes(
    TStructsSchemaItems &struct_schema_items) {
  utils::SharedPtr<ISchemaItem> struct_schema_item_AudioPassThruCapabilities = InitStructSchemaItem_AudioPassThruCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::AudioPassThruCapabilities, struct_schema_item_AudioPassThruCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::AudioPassThruCapabilities, CSmartSchema(struct_schema_item_AudioPassThruCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Image = InitStructSchemaItem_Image(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Image, struct_schema_item_Image));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Image, CSmartSchema(struct_schema_item_Image)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_SoftButton = InitStructSchemaItem_SoftButton(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::SoftButton, struct_schema_item_SoftButton));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::SoftButton, CSmartSchema(struct_schema_item_SoftButton)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Choice = InitStructSchemaItem_Choice(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Choice, struct_schema_item_Choice));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Choice, CSmartSchema(struct_schema_item_Choice)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_VrHelpItem = InitStructSchemaItem_VrHelpItem(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::VrHelpItem, struct_schema_item_VrHelpItem));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::VrHelpItem, CSmartSchema(struct_schema_item_VrHelpItem)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_SyncMsgVersion = InitStructSchemaItem_SyncMsgVersion(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::SyncMsgVersion, struct_schema_item_SyncMsgVersion));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::SyncMsgVersion, CSmartSchema(struct_schema_item_SyncMsgVersion)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_SingleTireStatus = InitStructSchemaItem_SingleTireStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::SingleTireStatus, struct_schema_item_SingleTireStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::SingleTireStatus, CSmartSchema(struct_schema_item_SingleTireStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_BeltStatus = InitStructSchemaItem_BeltStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::BeltStatus, struct_schema_item_BeltStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::BeltStatus, CSmartSchema(struct_schema_item_BeltStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_BodyInformation = InitStructSchemaItem_BodyInformation(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::BodyInformation, struct_schema_item_BodyInformation));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::BodyInformation, CSmartSchema(struct_schema_item_BodyInformation)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_DeviceStatus = InitStructSchemaItem_DeviceStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::DeviceStatus, struct_schema_item_DeviceStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::DeviceStatus, CSmartSchema(struct_schema_item_DeviceStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_HeadLampStatus = InitStructSchemaItem_HeadLampStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::HeadLampStatus, struct_schema_item_HeadLampStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::HeadLampStatus, CSmartSchema(struct_schema_item_HeadLampStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_engineInfo = InitStructSchemaItem_engineInfo(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::engineInfo, struct_schema_item_engineInfo));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::engineInfo, CSmartSchema(struct_schema_item_engineInfo)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_DeviceInfo = InitStructSchemaItem_DeviceInfo(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::DeviceInfo, struct_schema_item_DeviceInfo));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::DeviceInfo, CSmartSchema(struct_schema_item_DeviceInfo)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_ECallInfo = InitStructSchemaItem_ECallInfo(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::ECallInfo, struct_schema_item_ECallInfo));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::ECallInfo, CSmartSchema(struct_schema_item_ECallInfo)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_AirbagStatus = InitStructSchemaItem_AirbagStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::AirbagStatus, struct_schema_item_AirbagStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::AirbagStatus, CSmartSchema(struct_schema_item_AirbagStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_EmergencyEvent = InitStructSchemaItem_EmergencyEvent(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::EmergencyEvent, struct_schema_item_EmergencyEvent));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::EmergencyEvent, CSmartSchema(struct_schema_item_EmergencyEvent)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_ClusterModeStatus = InitStructSchemaItem_ClusterModeStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::ClusterModeStatus, struct_schema_item_ClusterModeStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::ClusterModeStatus, CSmartSchema(struct_schema_item_ClusterModeStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_MyKey = InitStructSchemaItem_MyKey(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::MyKey, struct_schema_item_MyKey));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::MyKey, CSmartSchema(struct_schema_item_MyKey)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_TireStatus = InitStructSchemaItem_TireStatus(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::TireStatus, struct_schema_item_TireStatus));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::TireStatus, CSmartSchema(struct_schema_item_TireStatus)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_GPSData = InitStructSchemaItem_GPSData(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::GPSData, struct_schema_item_GPSData));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::GPSData, CSmartSchema(struct_schema_item_GPSData)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_VehicleDataResult = InitStructSchemaItem_VehicleDataResult(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::VehicleDataResult, struct_schema_item_VehicleDataResult));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::VehicleDataResult, CSmartSchema(struct_schema_item_VehicleDataResult)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_DIDResult = InitStructSchemaItem_DIDResult(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::DIDResult, struct_schema_item_DIDResult));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::DIDResult, CSmartSchema(struct_schema_item_DIDResult)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_StartTime = InitStructSchemaItem_StartTime(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::StartTime, struct_schema_item_StartTime));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::StartTime, CSmartSchema(struct_schema_item_StartTime)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_TextField = InitStructSchemaItem_TextField(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::TextField, struct_schema_item_TextField));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::TextField, CSmartSchema(struct_schema_item_TextField)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_ImageResolution = InitStructSchemaItem_ImageResolution(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::ImageResolution, struct_schema_item_ImageResolution));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::ImageResolution, CSmartSchema(struct_schema_item_ImageResolution)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_ImageField = InitStructSchemaItem_ImageField(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::ImageField, struct_schema_item_ImageField));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::ImageField, CSmartSchema(struct_schema_item_ImageField)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_TouchCoord = InitStructSchemaItem_TouchCoord(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::TouchCoord, struct_schema_item_TouchCoord));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::TouchCoord, CSmartSchema(struct_schema_item_TouchCoord)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_TouchEvent = InitStructSchemaItem_TouchEvent(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::TouchEvent, struct_schema_item_TouchEvent));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::TouchEvent, CSmartSchema(struct_schema_item_TouchEvent)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Coordinate = InitStructSchemaItem_Coordinate(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Coordinate, struct_schema_item_Coordinate));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Coordinate, CSmartSchema(struct_schema_item_Coordinate)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_TouchArea = InitStructSchemaItem_TouchArea(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::TouchArea, struct_schema_item_TouchArea));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::TouchArea, CSmartSchema(struct_schema_item_TouchArea)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_TouchEventCapabilities = InitStructSchemaItem_TouchEventCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::TouchEventCapabilities, struct_schema_item_TouchEventCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::TouchEventCapabilities, CSmartSchema(struct_schema_item_TouchEventCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_ScreenParams = InitStructSchemaItem_ScreenParams(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::ScreenParams, struct_schema_item_ScreenParams));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::ScreenParams, CSmartSchema(struct_schema_item_ScreenParams)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_HMIPermissions = InitStructSchemaItem_HMIPermissions(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::HMIPermissions, struct_schema_item_HMIPermissions));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::HMIPermissions, CSmartSchema(struct_schema_item_HMIPermissions)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_ParameterPermissions = InitStructSchemaItem_ParameterPermissions(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::ParameterPermissions, struct_schema_item_ParameterPermissions));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::ParameterPermissions, CSmartSchema(struct_schema_item_ParameterPermissions)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_PermissionItem = InitStructSchemaItem_PermissionItem(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::PermissionItem, struct_schema_item_PermissionItem));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::PermissionItem, CSmartSchema(struct_schema_item_PermissionItem)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_DisplayCapabilities = InitStructSchemaItem_DisplayCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::DisplayCapabilities, struct_schema_item_DisplayCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::DisplayCapabilities, CSmartSchema(struct_schema_item_DisplayCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_ButtonCapabilities = InitStructSchemaItem_ButtonCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::ButtonCapabilities, struct_schema_item_ButtonCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::ButtonCapabilities, CSmartSchema(struct_schema_item_ButtonCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_SoftButtonCapabilities = InitStructSchemaItem_SoftButtonCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::SoftButtonCapabilities, struct_schema_item_SoftButtonCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::SoftButtonCapabilities, CSmartSchema(struct_schema_item_SoftButtonCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_PresetBankCapabilities = InitStructSchemaItem_PresetBankCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::PresetBankCapabilities, struct_schema_item_PresetBankCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::PresetBankCapabilities, CSmartSchema(struct_schema_item_PresetBankCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_MenuParams = InitStructSchemaItem_MenuParams(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::MenuParams, struct_schema_item_MenuParams));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::MenuParams, CSmartSchema(struct_schema_item_MenuParams)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_TTSChunk = InitStructSchemaItem_TTSChunk(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::TTSChunk, struct_schema_item_TTSChunk));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::TTSChunk, CSmartSchema(struct_schema_item_TTSChunk)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_Turn = InitStructSchemaItem_Turn(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Turn, struct_schema_item_Turn));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Turn, CSmartSchema(struct_schema_item_Turn)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_VehicleType = InitStructSchemaItem_VehicleType(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::VehicleType, struct_schema_item_VehicleType));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::VehicleType, CSmartSchema(struct_schema_item_VehicleType)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_KeyboardProperties = InitStructSchemaItem_KeyboardProperties(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::KeyboardProperties, struct_schema_item_KeyboardProperties));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::KeyboardProperties, CSmartSchema(struct_schema_item_KeyboardProperties)));
}

void mobile_apis::MOBILE_API::InitFunctionSchemes(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::RegisterAppInterfaceID, messageType::request), InitFunction_RegisterAppInterfaceID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::RegisterAppInterfaceID, messageType::response), InitFunction_RegisterAppInterfaceID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UnregisterAppInterfaceID, messageType::request), InitFunction_UnregisterAppInterfaceID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UnregisterAppInterfaceID, messageType::response), InitFunction_UnregisterAppInterfaceID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetGlobalPropertiesID, messageType::request), InitFunction_SetGlobalPropertiesID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetGlobalPropertiesID, messageType::response), InitFunction_SetGlobalPropertiesID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ResetGlobalPropertiesID, messageType::request), InitFunction_ResetGlobalPropertiesID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ResetGlobalPropertiesID, messageType::response), InitFunction_ResetGlobalPropertiesID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AddCommandID, messageType::request), InitFunction_AddCommandID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AddCommandID, messageType::response), InitFunction_AddCommandID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteCommandID, messageType::request), InitFunction_DeleteCommandID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteCommandID, messageType::response), InitFunction_DeleteCommandID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AddSubMenuID, messageType::request), InitFunction_AddSubMenuID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AddSubMenuID, messageType::response), InitFunction_AddSubMenuID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteSubMenuID, messageType::request), InitFunction_DeleteSubMenuID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteSubMenuID, messageType::response), InitFunction_DeleteSubMenuID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::CreateInteractionChoiceSetID, messageType::request), InitFunction_CreateInteractionChoiceSetID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::CreateInteractionChoiceSetID, messageType::response), InitFunction_CreateInteractionChoiceSetID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::PerformInteractionID, messageType::request), InitFunction_PerformInteractionID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::PerformInteractionID, messageType::response), InitFunction_PerformInteractionID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteInteractionChoiceSetID, messageType::request), InitFunction_DeleteInteractionChoiceSetID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteInteractionChoiceSetID, messageType::response), InitFunction_DeleteInteractionChoiceSetID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AlertID, messageType::request), InitFunction_AlertID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AlertID, messageType::response), InitFunction_AlertID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ShowID, messageType::request), InitFunction_ShowID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ShowID, messageType::response), InitFunction_ShowID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SpeakID, messageType::request), InitFunction_SpeakID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SpeakID, messageType::response), InitFunction_SpeakID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetMediaClockTimerID, messageType::request), InitFunction_SetMediaClockTimerID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetMediaClockTimerID, messageType::response), InitFunction_SetMediaClockTimerID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::PerformAudioPassThruID, messageType::request), InitFunction_PerformAudioPassThruID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::PerformAudioPassThruID, messageType::response), InitFunction_PerformAudioPassThruID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::EndAudioPassThruID, messageType::request), InitFunction_EndAudioPassThruID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::EndAudioPassThruID, messageType::response), InitFunction_EndAudioPassThruID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SubscribeButtonID, messageType::request), InitFunction_SubscribeButtonID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SubscribeButtonID, messageType::response), InitFunction_SubscribeButtonID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UnsubscribeButtonID, messageType::request), InitFunction_UnsubscribeButtonID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UnsubscribeButtonID, messageType::response), InitFunction_UnsubscribeButtonID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SubscribeVehicleDataID, messageType::request), InitFunction_SubscribeVehicleDataID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SubscribeVehicleDataID, messageType::response), InitFunction_SubscribeVehicleDataID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UnsubscribeVehicleDataID, messageType::request), InitFunction_UnsubscribeVehicleDataID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UnsubscribeVehicleDataID, messageType::response), InitFunction_UnsubscribeVehicleDataID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::GetVehicleDataID, messageType::request), InitFunction_GetVehicleDataID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::GetVehicleDataID, messageType::response), InitFunction_GetVehicleDataID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ReadDIDID, messageType::request), InitFunction_ReadDIDID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ReadDIDID, messageType::response), InitFunction_ReadDIDID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::GetDTCsID, messageType::request), InitFunction_GetDTCsID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::GetDTCsID, messageType::response), InitFunction_GetDTCsID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DiagnosticMessageID, messageType::request), InitFunction_DiagnosticMessageID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DiagnosticMessageID, messageType::response), InitFunction_DiagnosticMessageID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ScrollableMessageID, messageType::request), InitFunction_ScrollableMessageID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ScrollableMessageID, messageType::response), InitFunction_ScrollableMessageID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SliderID, messageType::request), InitFunction_SliderID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SliderID, messageType::response), InitFunction_SliderID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ShowConstantTBTID, messageType::request), InitFunction_ShowConstantTBTID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ShowConstantTBTID, messageType::response), InitFunction_ShowConstantTBTID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AlertManeuverID, messageType::request), InitFunction_AlertManeuverID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AlertManeuverID, messageType::response), InitFunction_AlertManeuverID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UpdateTurnListID, messageType::request), InitFunction_UpdateTurnListID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UpdateTurnListID, messageType::response), InitFunction_UpdateTurnListID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ChangeRegistrationID, messageType::request), InitFunction_ChangeRegistrationID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ChangeRegistrationID, messageType::response), InitFunction_ChangeRegistrationID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::GenericResponseID, messageType::response), InitFunction_GenericResponseID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::PutFileID, messageType::request), InitFunction_PutFileID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::PutFileID, messageType::response), InitFunction_PutFileID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteFileID, messageType::request), InitFunction_DeleteFileID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteFileID, messageType::response), InitFunction_DeleteFileID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ListFilesID, messageType::request), InitFunction_ListFilesID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ListFilesID, messageType::response), InitFunction_ListFilesID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetAppIconID, messageType::request), InitFunction_SetAppIconID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetAppIconID, messageType::response), InitFunction_SetAppIconID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetDisplayLayoutID, messageType::request), InitFunction_SetDisplayLayoutID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnHashChangeID, messageType::notification), InitFunction_OnHashChangeID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetDisplayLayoutID, messageType::response), InitFunction_SetDisplayLayoutID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SystemRequestID, messageType::request), InitFunction_SystemRequestID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SystemRequestID, messageType::response), InitFunction_SystemRequestID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnHMIStatusID, messageType::notification), InitFunction_OnHMIStatusID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnAppInterfaceUnregisteredID, messageType::notification), InitFunction_OnAppInterfaceUnregisteredID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnButtonEventID, messageType::notification), InitFunction_OnButtonEventID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnButtonPressID, messageType::notification), InitFunction_OnButtonPressID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnVehicleDataID, messageType::notification), InitFunction_OnVehicleDataID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnCommandID, messageType::notification), InitFunction_OnCommandID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnTBTClientStateID, messageType::notification), InitFunction_OnTBTClientStateID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnDriverDistractionID, messageType::notification), InitFunction_OnDriverDistractionID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnPermissionsChangeID, messageType::notification), InitFunction_OnPermissionsChangeID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnAudioPassThruID, messageType::notification), InitFunction_OnAudioPassThruID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnLanguageChangeID, messageType::notification), InitFunction_OnLanguageChangeID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnKeyboardInputID, messageType::notification), InitFunction_OnKeyboardInputID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnTouchEventID, messageType::notification), InitFunction_OnTouchEventID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnSystemRequestID, messageType::notification), InitFunction_OnSystemRequestID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::EncodedSyncPDataID, messageType::request), InitFunction_EncodedSyncPDataID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::EncodedSyncPDataID, messageType::response), InitFunction_EncodedSyncPDataID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SyncPDataID, messageType::request), InitFunction_SyncPDataID_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SyncPDataID, messageType::response), InitFunction_SyncPDataID_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnEncodedSyncPDataID, messageType::notification), InitFunction_OnEncodedSyncPDataID_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnSyncPDataID, messageType::notification), InitFunction_OnSyncPDataID_notification(struct_schema_items, function_id_items, message_type_items)));
}

//------------- Functions schemes initialization -------------

CSmartSchema mobile_apis::MOBILE_API::InitFunction_RegisterAppInterfaceID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Language::eType> Language_all_enum_values;
  Language_all_enum_values.insert(Language::EN_US);
  Language_all_enum_values.insert(Language::ES_MX);
  Language_all_enum_values.insert(Language::FR_CA);
  Language_all_enum_values.insert(Language::DE_DE);
  Language_all_enum_values.insert(Language::ES_ES);
  Language_all_enum_values.insert(Language::EN_GB);
  Language_all_enum_values.insert(Language::RU_RU);
  Language_all_enum_values.insert(Language::TR_TR);
  Language_all_enum_values.insert(Language::PL_PL);
  Language_all_enum_values.insert(Language::FR_FR);
  Language_all_enum_values.insert(Language::IT_IT);
  Language_all_enum_values.insert(Language::SV_SE);
  Language_all_enum_values.insert(Language::PT_PT);
  Language_all_enum_values.insert(Language::NL_NL);
  Language_all_enum_values.insert(Language::EN_AU);
  Language_all_enum_values.insert(Language::ZH_CN);
  Language_all_enum_values.insert(Language::ZH_TW);
  Language_all_enum_values.insert(Language::JA_JP);
  Language_all_enum_values.insert(Language::AR_SA);
  Language_all_enum_values.insert(Language::KO_KR);
  Language_all_enum_values.insert(Language::PT_BR);
  Language_all_enum_values.insert(Language::CS_CZ);
  Language_all_enum_values.insert(Language::DA_DK);
  Language_all_enum_values.insert(Language::NO_NO);

  std::set<AppHMIType::eType> AppHMIType_all_enum_values;
  AppHMIType_all_enum_values.insert(AppHMIType::DEFAULT);
  AppHMIType_all_enum_values.insert(AppHMIType::COMMUNICATION);
  AppHMIType_all_enum_values.insert(AppHMIType::MEDIA);
  AppHMIType_all_enum_values.insert(AppHMIType::MESSAGING);
  AppHMIType_all_enum_values.insert(AppHMIType::NAVIGATION);
  AppHMIType_all_enum_values.insert(AppHMIType::INFORMATION);
  AppHMIType_all_enum_values.insert(AppHMIType::SOCIAL);
  AppHMIType_all_enum_values.insert(AppHMIType::BACKGROUND_PROCESS);
  AppHMIType_all_enum_values.insert(AppHMIType::TESTING);
  AppHMIType_all_enum_values.insert(AppHMIType::SYSTEM);

  // Function parameter syncMsgVersion.
  //
  // See SyncMsgVersion
  utils::SharedPtr<ISchemaItem> syncMsgVersion_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SyncMsgVersion);

  // Function parameter appName.
  //
  // 
  //       	The mobile application name, e.g. "Ford Drive Green".
  //       	Needs to be unique over all applications.
  //       	May not be empty.
  //       	May not start with a new line character.
  //       	May not interfere with any name or synonym of previously registered applications and any predefined blacklist of words (global commands)
  //       	Needs to be unique over all applications. Applications with the same name will be rejected.
  //       	Only characters from char set [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
  //       
  utils::SharedPtr<ISchemaItem> appName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>());

  // Function parameter ttsName.
  //
  // 
  //       	TTS string for VR recognition of the mobile application name, e.g. "Ford Drive Green".
  //       	Meant to overcome any failing on speech engine in properly pronouncing / understanding app name.
  //       	Needs to be unique over all applications.
  //       	May not be empty.
  //       	May not start with a new line character.
  //       	May not interfere with any name or synonym of previously registered applications and any predefined blacklist of words (global commands)
  //       	Needs to be unique over all applications. Applications with the same name will be rejected.
  //       	Only characters from char set [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
  //       
  utils::SharedPtr<ISchemaItem> ttsName_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter ngnMediaScreenAppName.
  //
  // 
  //       	Provides an abbreviated version of the app name (if needed), that will be displayed on the NGN media screen.
  //       	If not provided, the appName is used instead (and will be truncated if too long)
  //       	Only characters from char set [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
  //       
  utils::SharedPtr<ISchemaItem> ngnMediaScreenAppName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>());

  // Function parameter vrSynonyms.
  //
  // 
  //       	Defines an additional voice recognition command.
  //       	May not interfere with any name or synonym of previously registered applications and any predefined blacklist of words (global commands)
  //       	Only characters from char set [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
  //       
  utils::SharedPtr<ISchemaItem> vrSynonyms_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(40), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter isMediaApplication.
  //
  // 
  //       	Indicates if the application is a media or a non-media application.
  //       	Only media applications will be able to stream audio to Sync that is audible outside of the BT media source.
  //       
  utils::SharedPtr<ISchemaItem> isMediaApplication_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter languageDesired.
  //
  // 
  //       	See Language
  //       	Current app’s expected VR+TTS language
  //       	If there is a mismatch with SYNC, the app will be able to change this registration with changeRegistration prior to app being brought into focus.
  //       
  utils::SharedPtr<ISchemaItem> languageDesired_SchemaItem = TEnumSchemaItem<Language::eType>::create(Language_all_enum_values, TSchemaItemParameter<Language::eType>());

  // Function parameter hmiDisplayLanguageDesired.
  //
  // 
  //       	See Language
  //       	Current app’s expected display language
  //       	If there is a mismatch with SYNC, the app will be able to change this registration with changeRegistration prior to app being brought into focus.
  //       
  utils::SharedPtr<ISchemaItem> hmiDisplayLanguageDesired_SchemaItem = TEnumSchemaItem<Language::eType>::create(Language_all_enum_values, TSchemaItemParameter<Language::eType>());

  // Function parameter appHMIType.
  //
  // 
  //       	See AppHMIType
  //       	List of all applicable app HMI types stating which HMI classifications to be given to the app.
  //       	e.g. for platforms like GEN2, this will determine which "corner(s)" the app can populate.
  //       
  utils::SharedPtr<ISchemaItem> appHMIType_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<AppHMIType::eType>::create(AppHMIType_all_enum_values, TSchemaItemParameter<AppHMIType::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter hashID.
  //
  // 
  //         ID used to uniquely identify current state of all app data that can persist through connection cycles (e.g. ignition cycles).
  //         This registered data (commands, submenus, choice sets, etc.) can be reestablished without needing to explicitly reregister each piece.
  //         If omitted, then the previous state of an app's commands, etc. will not be restored. 
  //       
  utils::SharedPtr<ISchemaItem> hashID_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>());

  // Function parameter deviceInfo.
  //
  // 
  //         See DeviceInfo.
  //       
  utils::SharedPtr<ISchemaItem> deviceInfo_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::DeviceInfo);

  // Function parameter appID.
  //
  // ID used to validate app with policy table entries
  utils::SharedPtr<ISchemaItem> appID_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["syncMsgVersion"] = CObjectSchemaItem::SMember(syncMsgVersion_SchemaItem, true);
  schema_members["appName"] = CObjectSchemaItem::SMember(appName_SchemaItem, true);
  schema_members["ttsName"] = CObjectSchemaItem::SMember(ttsName_SchemaItem, false);
  schema_members["ngnMediaScreenAppName"] = CObjectSchemaItem::SMember(ngnMediaScreenAppName_SchemaItem, false);
  schema_members["vrSynonyms"] = CObjectSchemaItem::SMember(vrSynonyms_SchemaItem, false);
  schema_members["isMediaApplication"] = CObjectSchemaItem::SMember(isMediaApplication_SchemaItem, true);
  schema_members["languageDesired"] = CObjectSchemaItem::SMember(languageDesired_SchemaItem, true);
  schema_members["hmiDisplayLanguageDesired"] = CObjectSchemaItem::SMember(hmiDisplayLanguageDesired_SchemaItem, true);
  schema_members["appHMIType"] = CObjectSchemaItem::SMember(appHMIType_SchemaItem, false);
  schema_members["hashID"] = CObjectSchemaItem::SMember(hashID_SchemaItem, false);
  schema_members["deviceInfo"] = CObjectSchemaItem::SMember(deviceInfo_SchemaItem, false);
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_RegisterAppInterfaceID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::WARNINGS);
  resultCode_allowed_enum_subset_values.insert(Result::WRONG_LANGUAGE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::RESUME_FAILED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_REGISTERED_ALREADY);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::DUPLICATE_NAME);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_VERSION);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_APPLICATIONS);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  std::set<Language::eType> Language_all_enum_values;
  Language_all_enum_values.insert(Language::EN_US);
  Language_all_enum_values.insert(Language::ES_MX);
  Language_all_enum_values.insert(Language::FR_CA);
  Language_all_enum_values.insert(Language::DE_DE);
  Language_all_enum_values.insert(Language::ES_ES);
  Language_all_enum_values.insert(Language::EN_GB);
  Language_all_enum_values.insert(Language::RU_RU);
  Language_all_enum_values.insert(Language::TR_TR);
  Language_all_enum_values.insert(Language::PL_PL);
  Language_all_enum_values.insert(Language::FR_FR);
  Language_all_enum_values.insert(Language::IT_IT);
  Language_all_enum_values.insert(Language::SV_SE);
  Language_all_enum_values.insert(Language::PT_PT);
  Language_all_enum_values.insert(Language::NL_NL);
  Language_all_enum_values.insert(Language::EN_AU);
  Language_all_enum_values.insert(Language::ZH_CN);
  Language_all_enum_values.insert(Language::ZH_TW);
  Language_all_enum_values.insert(Language::JA_JP);
  Language_all_enum_values.insert(Language::AR_SA);
  Language_all_enum_values.insert(Language::KO_KR);
  Language_all_enum_values.insert(Language::PT_BR);
  Language_all_enum_values.insert(Language::CS_CZ);
  Language_all_enum_values.insert(Language::DA_DK);
  Language_all_enum_values.insert(Language::NO_NO);

  std::set<HmiZoneCapabilities::eType> HmiZoneCapabilities_all_enum_values;
  HmiZoneCapabilities_all_enum_values.insert(HmiZoneCapabilities::FRONT);
  HmiZoneCapabilities_all_enum_values.insert(HmiZoneCapabilities::BACK);

  std::set<SpeechCapabilities::eType> SpeechCapabilities_all_enum_values;
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::SC_TEXT);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::SAPI_PHONEMES);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::LHPLUS_PHONEMES);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::PRE_RECORDED);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::SILENCE);

  std::set<VrCapabilities::eType> VrCapabilities_all_enum_values;
  VrCapabilities_all_enum_values.insert(VrCapabilities::VR_TEXT);

  std::set<PrerecordedSpeech::eType> PrerecordedSpeech_all_enum_values;
  PrerecordedSpeech_all_enum_values.insert(PrerecordedSpeech::HELP_JINGLE);
  PrerecordedSpeech_all_enum_values.insert(PrerecordedSpeech::INITIAL_JINGLE);
  PrerecordedSpeech_all_enum_values.insert(PrerecordedSpeech::LISTEN_JINGLE);
  PrerecordedSpeech_all_enum_values.insert(PrerecordedSpeech::POSITIVE_JINGLE);
  PrerecordedSpeech_all_enum_values.insert(PrerecordedSpeech::NEGATIVE_JINGLE);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter syncMsgVersion.
  //
  // See SyncMsgVersion
  utils::SharedPtr<ISchemaItem> syncMsgVersion_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SyncMsgVersion);

  // Function parameter language.
  //
  // The currently active VR+TTS language on Sync. See "Language" for options.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Language::eType>::create(Language_all_enum_values, TSchemaItemParameter<Language::eType>());

  // Function parameter hmiDisplayLanguage.
  //
  // The currently active display language on Sync. See "Language" for       options.
  utils::SharedPtr<ISchemaItem> hmiDisplayLanguage_SchemaItem = TEnumSchemaItem<Language::eType>::create(Language_all_enum_values, TSchemaItemParameter<Language::eType>());

  // Function parameter displayCapabilities.
  //
  // See DisplayCapabilities
  utils::SharedPtr<ISchemaItem> displayCapabilities_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::DisplayCapabilities);

  // Function parameter buttonCapabilities.
  //
  // See ButtonCapabilities
  utils::SharedPtr<ISchemaItem> buttonCapabilities_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ButtonCapabilities), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter softButtonCapabilities.
  //
  // If returned, the platform supports on-screen SoftButtons; see SoftButtonCapabilities.
  utils::SharedPtr<ISchemaItem> softButtonCapabilities_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SoftButtonCapabilities), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter presetBankCapabilities.
  //
  // If returned, the platform supports custom on-screen Presets; see PresetBankCapabilities.
  utils::SharedPtr<ISchemaItem> presetBankCapabilities_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::PresetBankCapabilities);

  // Function parameter hmiZoneCapabilities.
  //
  // See HmiZoneCapabilities
  //
  // Note: If not used yet => remove
  utils::SharedPtr<ISchemaItem> hmiZoneCapabilities_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<HmiZoneCapabilities::eType>::create(HmiZoneCapabilities_all_enum_values, TSchemaItemParameter<HmiZoneCapabilities::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter speechCapabilities.
  //
  // See SpeechCapabilities
  utils::SharedPtr<ISchemaItem> speechCapabilities_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<SpeechCapabilities::eType>::create(SpeechCapabilities_all_enum_values, TSchemaItemParameter<SpeechCapabilities::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter vrCapabilities.
  //
  // See VrCapabilities
  utils::SharedPtr<ISchemaItem> vrCapabilities_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<VrCapabilities::eType>::create(VrCapabilities_all_enum_values, TSchemaItemParameter<VrCapabilities::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter audioPassThruCapabilities.
  //
  // See AudioPassThruCapability
  utils::SharedPtr<ISchemaItem> audioPassThruCapabilities_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::AudioPassThruCapabilities), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter vehicleType.
  //
  // Specifies the vehicle's type. See VehicleType.
  utils::SharedPtr<ISchemaItem> vehicleType_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleType);

  // Function parameter prerecordedSpeech.
  //
  // See PrerecordedSpeech
  utils::SharedPtr<ISchemaItem> prerecordedSpeech_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<PrerecordedSpeech::eType>::create(PrerecordedSpeech_all_enum_values, TSchemaItemParameter<PrerecordedSpeech::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter supportedDiagModes.
  //
  // 
  //       	Specifies the white-list of supported diagnostic modes (0x00-0xFF) capable for DiagnosticMessage requests.
  //       	If a mode outside this list is requested, it will be rejected.
  //       
  utils::SharedPtr<ISchemaItem> supportedDiagModes_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(255), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
  schema_members["syncMsgVersion"] = CObjectSchemaItem::SMember(syncMsgVersion_SchemaItem, false);
  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, false);
  schema_members["hmiDisplayLanguage"] = CObjectSchemaItem::SMember(hmiDisplayLanguage_SchemaItem, false);
  schema_members["displayCapabilities"] = CObjectSchemaItem::SMember(displayCapabilities_SchemaItem, false);
  schema_members["buttonCapabilities"] = CObjectSchemaItem::SMember(buttonCapabilities_SchemaItem, false);
  schema_members["softButtonCapabilities"] = CObjectSchemaItem::SMember(softButtonCapabilities_SchemaItem, false);
  schema_members["presetBankCapabilities"] = CObjectSchemaItem::SMember(presetBankCapabilities_SchemaItem, false);
  schema_members["hmiZoneCapabilities"] = CObjectSchemaItem::SMember(hmiZoneCapabilities_SchemaItem, false);
  schema_members["speechCapabilities"] = CObjectSchemaItem::SMember(speechCapabilities_SchemaItem, false);
  schema_members["vrCapabilities"] = CObjectSchemaItem::SMember(vrCapabilities_SchemaItem, false);
  schema_members["audioPassThruCapabilities"] = CObjectSchemaItem::SMember(audioPassThruCapabilities_SchemaItem, false);
  schema_members["vehicleType"] = CObjectSchemaItem::SMember(vehicleType_SchemaItem, false);
  schema_members["prerecordedSpeech"] = CObjectSchemaItem::SMember(prerecordedSpeech_SchemaItem, false);
  schema_members["supportedDiagModes"] = CObjectSchemaItem::SMember(supportedDiagModes_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_UnregisterAppInterfaceID_request(
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_UnregisterAppInterfaceID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SetGlobalPropertiesID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter helpPrompt.
  //
  // 
  //       	The help prompt.
  //       	An array of text chunks of type TTSChunk. See TTSChunk.
  //       	The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> helpPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter timeoutPrompt.
  //
  // 
  //       	Help text for a wait timeout.
  //       	An array of text chunks of type TTSChunk. See TTSChunk.
  //       	The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> timeoutPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter vrHelpTitle.
  //
  // 
  //       	VR Help Title text.
  //       	If omitted on supported displays, the default SYNC help title shall be used.
  //       	If omitted and one or more vrHelp items are provided, the request will be rejected.
  //       
  utils::SharedPtr<ISchemaItem> vrHelpTitle_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter vrHelp.
  //
  // 
  //       	VR Help Items.
  //       	If omitted on supported displays, the default AppLink VR help / What Can I Say? screen shall be used.
  //       	If the list of VR Help Items contains nonsequential positions (e.g. [1,2,4]), the RPC shall be rejected.
  //       	If omitted and a vrHelpTitle is provided, the request will be rejected.
  //       
  utils::SharedPtr<ISchemaItem> vrHelp_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VrHelpItem), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter menuTitle.
  //
  // Optional text to label an app menu button (for certain touchscreen platforms).
  utils::SharedPtr<ISchemaItem> menuTitle_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter menuIcon.
  //
  // >Optional icon to draw on an app menu button (for certain touchscreen platforms).
  utils::SharedPtr<ISchemaItem> menuIcon_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Image);

  // Function parameter keyboardProperties.
  //
  // On-screen keybaord configuration (if available).
  utils::SharedPtr<ISchemaItem> keyboardProperties_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::KeyboardProperties);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["helpPrompt"] = CObjectSchemaItem::SMember(helpPrompt_SchemaItem, false);
  schema_members["timeoutPrompt"] = CObjectSchemaItem::SMember(timeoutPrompt_SchemaItem, false);
  schema_members["vrHelpTitle"] = CObjectSchemaItem::SMember(vrHelpTitle_SchemaItem, false);
  schema_members["vrHelp"] = CObjectSchemaItem::SMember(vrHelp_SchemaItem, false);
  schema_members["menuTitle"] = CObjectSchemaItem::SMember(menuTitle_SchemaItem, false);
  schema_members["menuIcon"] = CObjectSchemaItem::SMember(menuIcon_SchemaItem, false);
  schema_members["keyboardProperties"] = CObjectSchemaItem::SMember(keyboardProperties_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SetGlobalPropertiesID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ResetGlobalPropertiesID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<GlobalProperty::eType> GlobalProperty_all_enum_values;
  GlobalProperty_all_enum_values.insert(GlobalProperty::HELPPROMPT);
  GlobalProperty_all_enum_values.insert(GlobalProperty::TIMEOUTPROMPT);
  GlobalProperty_all_enum_values.insert(GlobalProperty::VRHELPTITLE);
  GlobalProperty_all_enum_values.insert(GlobalProperty::VRHELPITEMS);
  GlobalProperty_all_enum_values.insert(GlobalProperty::MENUNAME);
  GlobalProperty_all_enum_values.insert(GlobalProperty::MENUICON);
  GlobalProperty_all_enum_values.insert(GlobalProperty::KEYBOARDPROPERTIES);

  // Function parameter properties.
  //
  // Contains the names of all global properties (like timeoutPrompt) that should be unset. Resetting means, that they have the same value as at start up (default)
  utils::SharedPtr<ISchemaItem> properties_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<GlobalProperty::eType>::create(GlobalProperty_all_enum_values, TSchemaItemParameter<GlobalProperty::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["properties"] = CObjectSchemaItem::SMember(properties_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ResetGlobalPropertiesID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_AddCommandID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter cmdID.
  //
  // unique ID of the command to add.
  utils::SharedPtr<ISchemaItem> cmdID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter menuParams.
  //
  // Optional sub value containing menu parameters
  utils::SharedPtr<ISchemaItem> menuParams_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::MenuParams);

  // Function parameter vrCommands.
  //
  // 
  //       	An array of strings to be used as VR synonyms for this command.
  //       	If this array is provided, it may not be empty.
  //       
  utils::SharedPtr<ISchemaItem> vrCommands_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(99), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter cmdIcon.
  //
  // 
  //        	Image struct determining whether static or dynamic icon.
  //        	If omitted on supported displays, no (or the default if applicable) icon shall be displayed.
  //        
  utils::SharedPtr<ISchemaItem> cmdIcon_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Image);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["cmdID"] = CObjectSchemaItem::SMember(cmdID_SchemaItem, true);
  schema_members["menuParams"] = CObjectSchemaItem::SMember(menuParams_SchemaItem, false);
  schema_members["vrCommands"] = CObjectSchemaItem::SMember(vrCommands_SchemaItem, false);
  schema_members["cmdIcon"] = CObjectSchemaItem::SMember(cmdIcon_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_AddCommandID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_ID);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::DUPLICATE_NAME);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_DeleteCommandID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter cmdID.
  //
  // ID of the command(s) to delete.
  utils::SharedPtr<ISchemaItem> cmdID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["cmdID"] = CObjectSchemaItem::SMember(cmdID_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_DeleteCommandID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_ID);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::IN_USE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_AddSubMenuID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter menuID.
  //
  // unique ID of the sub menu to add.
  utils::SharedPtr<ISchemaItem> menuID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter position.
  //
  // 
  //       	Position within the items that are are at top level of the in application menu.
  //       	0 will insert at the front.
  //       	1 will insert at the second position.
  //       	If position is greater or equal than the number of items on top level, the sub menu will be appended to the end.
  //       	Position of any submenu will always be located before the return and exit options
  //       	If this param was omitted the entry will be added at the end.
  //       
  utils::SharedPtr<ISchemaItem> position_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(1000), TSchemaItemParameter<int>(1000));

  // Function parameter menuName.
  //
  // Text to show in the menu for this sub menu.
  utils::SharedPtr<ISchemaItem> menuName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["menuID"] = CObjectSchemaItem::SMember(menuID_SchemaItem, true);
  schema_members["position"] = CObjectSchemaItem::SMember(position_SchemaItem, false);
  schema_members["menuName"] = CObjectSchemaItem::SMember(menuName_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_AddSubMenuID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_ID);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::DUPLICATE_NAME);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_DeleteSubMenuID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter menuID.
  //
  // The "menuID" of the submenu to delete. (See addSubMenu.menuID)
  utils::SharedPtr<ISchemaItem> menuID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["menuID"] = CObjectSchemaItem::SMember(menuID_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_DeleteSubMenuID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_ID);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::IN_USE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_CreateInteractionChoiceSetID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter interactionChoiceSetID.
  //
  // Unique ID used for this interaction choice set.
  utils::SharedPtr<ISchemaItem> interactionChoiceSetID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter choiceSet.
  utils::SharedPtr<ISchemaItem> choiceSet_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Choice), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["interactionChoiceSetID"] = CObjectSchemaItem::SMember(interactionChoiceSetID_SchemaItem, true);
  schema_members["choiceSet"] = CObjectSchemaItem::SMember(choiceSet_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_CreateInteractionChoiceSetID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_ID);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::DUPLICATE_NAME);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_PerformInteractionID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<InteractionMode::eType> InteractionMode_all_enum_values;
  InteractionMode_all_enum_values.insert(InteractionMode::MANUAL_ONLY);
  InteractionMode_all_enum_values.insert(InteractionMode::VR_ONLY);
  InteractionMode_all_enum_values.insert(InteractionMode::BOTH);

  std::set<LayoutMode::eType> LayoutMode_all_enum_values;
  LayoutMode_all_enum_values.insert(LayoutMode::ICON_ONLY);
  LayoutMode_all_enum_values.insert(LayoutMode::ICON_WITH_SEARCH);
  LayoutMode_all_enum_values.insert(LayoutMode::LIST_ONLY);
  LayoutMode_all_enum_values.insert(LayoutMode::LIST_WITH_SEARCH);
  LayoutMode_all_enum_values.insert(LayoutMode::KEYBOARD);

  // Function parameter initialText.
  //
  // Text to be displayed first.
  utils::SharedPtr<ISchemaItem> initialText_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter initialPrompt.
  //
  // 
  //       	This is the intial prompt spoken to the user at the start of an interaction.
  //       	An array of text chunks of type TTSChunk. See TTSChunk.
  //       	The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> initialPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter interactionMode.
  //
  // See InteractionMode.
  utils::SharedPtr<ISchemaItem> interactionMode_SchemaItem = TEnumSchemaItem<InteractionMode::eType>::create(InteractionMode_all_enum_values, TSchemaItemParameter<InteractionMode::eType>());

  // Function parameter interactionChoiceSetIDList.
  //
  // List of interaction choice set IDs to use with an interaction.
  utils::SharedPtr<ISchemaItem> interactionChoiceSetIDList_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(100));

  // Function parameter helpPrompt.
  //
  // 
  //       	Help text. This is the spoken string when a user speaks "help" when the interaction is occuring.
  //       	An array of text chunks of type TTSChunk. See TTSChunk.
  //       	The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> helpPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter timeoutPrompt.
  //
  // 
  //       	Timeout text. This text is spoken when a VR interaction times out.
  //       	An array of text chunks of type TTSChunk. See TTSChunk.
  //       	The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> timeoutPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter timeout.
  //
  // 
  //       	Timeout in milliseconds.
  // 	  	If omitted a standard value of 10000 milliseconds is used.
  // 	  	Applies only to the menu portion of the interaction. The VR timeout will be handled by the platform.
  // 	  
  utils::SharedPtr<ISchemaItem> timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(5000), TSchemaItemParameter<int>(100000), TSchemaItemParameter<int>(10000));

  // Function parameter vrHelp.
  //
  // 
  //       	Ability to send suggested VR Help Items to display on-screen during Perform Interaction.
  //       	If omitted on supported displays, the default SYNC generated list of suggested choices shall be displayed.
  //       
  utils::SharedPtr<ISchemaItem> vrHelp_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VrHelpItem), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter interactionLayout.
  //
  // See LayoutMode.
  utils::SharedPtr<ISchemaItem> interactionLayout_SchemaItem = TEnumSchemaItem<LayoutMode::eType>::create(LayoutMode_all_enum_values, TSchemaItemParameter<LayoutMode::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["initialText"] = CObjectSchemaItem::SMember(initialText_SchemaItem, true);
  schema_members["initialPrompt"] = CObjectSchemaItem::SMember(initialPrompt_SchemaItem, false);
  schema_members["interactionMode"] = CObjectSchemaItem::SMember(interactionMode_SchemaItem, true);
  schema_members["interactionChoiceSetIDList"] = CObjectSchemaItem::SMember(interactionChoiceSetIDList_SchemaItem, true);
  schema_members["helpPrompt"] = CObjectSchemaItem::SMember(helpPrompt_SchemaItem, false);
  schema_members["timeoutPrompt"] = CObjectSchemaItem::SMember(timeoutPrompt_SchemaItem, false);
  schema_members["timeout"] = CObjectSchemaItem::SMember(timeout_SchemaItem, false);
  schema_members["vrHelp"] = CObjectSchemaItem::SMember(vrHelp_SchemaItem, false);
  schema_members["interactionLayout"] = CObjectSchemaItem::SMember(interactionLayout_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_PerformInteractionID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_ID);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::WARNINGS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::TIMED_OUT);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::DUPLICATE_NAME);
  resultCode_allowed_enum_subset_values.insert(Result::ABORTED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  std::set<TriggerSource::eType> TriggerSource_all_enum_values;
  TriggerSource_all_enum_values.insert(TriggerSource::TS_MENU);
  TriggerSource_all_enum_values.insert(TriggerSource::TS_VR);
  TriggerSource_all_enum_values.insert(TriggerSource::TS_KEYBOARD);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter choiceID.
  //
  // 
  //       	ID of the choice that was selected in response to PerformInteraction.
  //       	Only is valid if resultCode is SUCCESS.
  //       
  utils::SharedPtr<ISchemaItem> choiceID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter manualTextEntry.
  //
  // 
  //       	Manually entered text selection, e.g. through keyboard
  //       	Can be returned in lieu of choiceID, depending on trigger source
  //       
  utils::SharedPtr<ISchemaItem> manualTextEntry_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter triggerSource.
  //
  // 
  //       	See TriggerSource
  //       	Only is valid if resultCode is SUCCESS.
  //       
  utils::SharedPtr<ISchemaItem> triggerSource_SchemaItem = TEnumSchemaItem<TriggerSource::eType>::create(TriggerSource_all_enum_values, TSchemaItemParameter<TriggerSource::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
  schema_members["choiceID"] = CObjectSchemaItem::SMember(choiceID_SchemaItem, false);
  schema_members["manualTextEntry"] = CObjectSchemaItem::SMember(manualTextEntry_SchemaItem, false);
  schema_members["triggerSource"] = CObjectSchemaItem::SMember(triggerSource_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_DeleteInteractionChoiceSetID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter interactionChoiceSetID.
  //
  // ID of the interaction choice set to delete.
  utils::SharedPtr<ISchemaItem> interactionChoiceSetID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["interactionChoiceSetID"] = CObjectSchemaItem::SMember(interactionChoiceSetID_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_DeleteInteractionChoiceSetID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_ID);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::IN_USE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_AlertID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter alertText1.
  //
  // The first line of the alert text field
  utils::SharedPtr<ISchemaItem> alertText1_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter alertText2.
  //
  // The second line of the alert text field
  utils::SharedPtr<ISchemaItem> alertText2_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter alertText3.
  //
  // The optional third line of the alert text field
  utils::SharedPtr<ISchemaItem> alertText3_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter ttsChunks.
  //
  // 
  //       	An array of text chunks of type TTSChunk. See TTSChunk.
  //       	The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> ttsChunks_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter duration.
  //
  // 
  //       	Timeout in milliseconds.
  //       	Typical timeouts are 3-5 seconds.
  //       	If omitted, timeout is set to 5s.
  //       
  utils::SharedPtr<ISchemaItem> duration_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(3000), TSchemaItemParameter<int>(10000), TSchemaItemParameter<int>(5000));

  // Function parameter playTone.
  //
  // 
  //       	Defines if tone should be played. Tone is played before TTS.
  //       	If omitted, no tone is played.
  //       
  utils::SharedPtr<ISchemaItem> playTone_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter progressIndicator.
  //
  // 
  //       	If supported on the given platform, the alert GUI will include some sort of animation indicating that loading of a feature is progressing.  e.g. a spinning wheel or hourglass, etc.
  //       
  utils::SharedPtr<ISchemaItem> progressIndicator_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter softButtons.
  //
  // 
  //       	App defined SoftButtons.
  //     	If omitted on supported displays, the displayed alert shall not have any SoftButtons.
  //       
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(4));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["alertText1"] = CObjectSchemaItem::SMember(alertText1_SchemaItem, false);
  schema_members["alertText2"] = CObjectSchemaItem::SMember(alertText2_SchemaItem, false);
  schema_members["alertText3"] = CObjectSchemaItem::SMember(alertText3_SchemaItem, false);
  schema_members["ttsChunks"] = CObjectSchemaItem::SMember(ttsChunks_SchemaItem, false);
  schema_members["duration"] = CObjectSchemaItem::SMember(duration_SchemaItem, false);
  schema_members["playTone"] = CObjectSchemaItem::SMember(playTone_SchemaItem, false);
  schema_members["progressIndicator"] = CObjectSchemaItem::SMember(progressIndicator_SchemaItem, false);
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_AlertID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::WARNINGS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::ABORTED);
  resultCode_allowed_enum_subset_values.insert(Result::USER_DISALLOWED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter tryAgainTime.
  //
  // 
  //       	Amount of time (in seconds) that an app must wait before resending an alert.
  //       	If provided, another system event or overlay currently has a higher priority than this alert.
  //       	An app must not send an alert without waiting at least the amount of time dictated.
  //       
  utils::SharedPtr<ISchemaItem> tryAgainTime_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
  schema_members["tryAgainTime"] = CObjectSchemaItem::SMember(tryAgainTime_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ShowID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<TextAlignment::eType> TextAlignment_all_enum_values;
  TextAlignment_all_enum_values.insert(TextAlignment::LEFT_ALIGNED);
  TextAlignment_all_enum_values.insert(TextAlignment::RIGHT_ALIGNED);
  TextAlignment_all_enum_values.insert(TextAlignment::CENTERED);

  // Function parameter mainField1.
  //
  // 
  //       	The text that should be displayed in a single or upper display line.
  //       	If this text is not set, the text of mainField1 stays unchanged.
  //       	If this text is empty "", the field will be cleared.
  //       
  utils::SharedPtr<ISchemaItem> mainField1_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter mainField2.
  //
  // 
  //       	The text that should be displayed on the second display line.
  //       	If this text is not set, the text of mainField2 stays unchanged.
  //       	If this text is empty "", the field will be cleared.
  //       
  utils::SharedPtr<ISchemaItem> mainField2_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter mainField3.
  //
  // 
  //       	The text that should be displayed on the second "page" first display line.
  //       	If this text is not set, the text of mainField3 stays unchanged.
  //       	If this text is empty "", the field will be cleared.
  //       
  utils::SharedPtr<ISchemaItem> mainField3_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter mainField4.
  //
  // 
  //       	The text that should be displayed on the second "page" second display line.
  //       	If this text is not set, the text of mainField4 stays unchanged.
  //       	If this text is empty "", the field will be cleared.
  //       
  utils::SharedPtr<ISchemaItem> mainField4_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter alignment.
  //
  // 
  //       	Specifies how mainField1 and mainField2 texts should be aligned on display.
  //       	If omitted, texts will be centered.
  //       
  utils::SharedPtr<ISchemaItem> alignment_SchemaItem = TEnumSchemaItem<TextAlignment::eType>::create(TextAlignment_all_enum_values, TSchemaItemParameter<TextAlignment::eType>());

  // Function parameter statusBar.
  //
  // Requires investigation regarding the nav display capabilities. Potentially lower lowerStatusBar, upperStatusBar, titleBar, etc.
  utils::SharedPtr<ISchemaItem> statusBar_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter mediaClock.
  //
  // 
  //       	Text value for MediaClock field. Has to be properly formatted by Mobile App according to Sync capabilities.
  //       	If this text is set, any automatic media clock updates previously set with SetMediaClockTimer will be stopped.
  //       
  utils::SharedPtr<ISchemaItem> mediaClock_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter mediaTrack.
  //
  // 
  //       	The text that should be displayed in the track field.
  //       	This field is only valid for media applications on NGN type ACMs.
  //       	If this text is not set, the text of mediaTrack stays unchanged.
  //       	If this text is empty "", the field will be cleared.
  //       
  utils::SharedPtr<ISchemaItem> mediaTrack_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter graphic.
  //
  // 
  //        	Image struct determining whether static or dynamic image to display in app.
  //        	If omitted on supported displays, the displayed graphic shall not change.
  //        
  utils::SharedPtr<ISchemaItem> graphic_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Image);

  // Function parameter secondaryGraphic.
  //
  // 
  //        	Image struct determining whether static or dynamic secondary image to display in app.
  //        	If omitted on supported displays, the displayed secondary graphic shall not change.
  //        
  utils::SharedPtr<ISchemaItem> secondaryGraphic_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Image);

  // Function parameter softButtons.
  //
  // 
  //       	App defined SoftButtons.
  //       	If omitted on supported displays, the currently displayed SoftButton values will not change.
  //       
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(8));

  // Function parameter customPresets.
  //
  // 
  //       	App labeled on-screen presets (i.e. GEN2).
  //       	If omitted on supported displays, the presets will be shown as not defined.
  //       
  utils::SharedPtr<ISchemaItem> customPresets_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(8));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["mainField1"] = CObjectSchemaItem::SMember(mainField1_SchemaItem, false);
  schema_members["mainField2"] = CObjectSchemaItem::SMember(mainField2_SchemaItem, false);
  schema_members["mainField3"] = CObjectSchemaItem::SMember(mainField3_SchemaItem, false);
  schema_members["mainField4"] = CObjectSchemaItem::SMember(mainField4_SchemaItem, false);
  schema_members["alignment"] = CObjectSchemaItem::SMember(alignment_SchemaItem, false);
  schema_members["statusBar"] = CObjectSchemaItem::SMember(statusBar_SchemaItem, false);
  schema_members["mediaClock"] = CObjectSchemaItem::SMember(mediaClock_SchemaItem, false);
  schema_members["mediaTrack"] = CObjectSchemaItem::SMember(mediaTrack_SchemaItem, false);
  schema_members["graphic"] = CObjectSchemaItem::SMember(graphic_SchemaItem, false);
  schema_members["secondaryGraphic"] = CObjectSchemaItem::SMember(secondaryGraphic_SchemaItem, false);
  schema_members["softButtons"] = CObjectSchemaItem::SMember(softButtons_SchemaItem, false);
  schema_members["customPresets"] = CObjectSchemaItem::SMember(customPresets_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ShowID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::ABORTED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SpeakID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter ttsChunks.
  //
  // 
  //       	An array of text chunks of type TTSChunk. See TTSChunk.
  //       	The array must have at least one item.
  //       
  utils::SharedPtr<ISchemaItem> ttsChunks_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["ttsChunks"] = CObjectSchemaItem::SMember(ttsChunks_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SpeakID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::WARNINGS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::ABORTED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SetMediaClockTimerID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<UpdateMode::eType> UpdateMode_all_enum_values;
  UpdateMode_all_enum_values.insert(UpdateMode::COUNTUP);
  UpdateMode_all_enum_values.insert(UpdateMode::COUNTDOWN);
  UpdateMode_all_enum_values.insert(UpdateMode::PAUSE);
  UpdateMode_all_enum_values.insert(UpdateMode::RESUME);
  UpdateMode_all_enum_values.insert(UpdateMode::CLEAR);

  // Function parameter startTime.
  //
  // 
  //       	See StartTime.
  //       	startTime must be provided for "COUNTUP" and "COUNTDOWN".
  //       	startTime will be ignored for "PAUSE", "RESUME", and "CLEAR"
  //       
  utils::SharedPtr<ISchemaItem> startTime_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::StartTime);

  // Function parameter endTime.
  //
  // 
  //       	See StartTime.
  //       	endTime can be provided for "COUNTUP" and "COUNTDOWN"; to be used to calculate any visual progress bar (if not provided, this feature is ignored)
  //       	If endTime is greater then startTime for COUNTDOWN or less than startTime for COUNTUP, then the request will return an INVALID_DATA.
  //       	endTime will be ignored for "PAUSE", "RESUME", and "CLEAR"
  //       
  utils::SharedPtr<ISchemaItem> endTime_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::StartTime);

  // Function parameter updateMode.
  //
  // 
  //       	Enumeration to control the media clock.
  //       	In case of pause, resume, or clear, the start time value is ignored and shall be left out.  For resume, the time continues with the same value as it was when paused.
  //       
  utils::SharedPtr<ISchemaItem> updateMode_SchemaItem = TEnumSchemaItem<UpdateMode::eType>::create(UpdateMode_all_enum_values, TSchemaItemParameter<UpdateMode::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["startTime"] = CObjectSchemaItem::SMember(startTime_SchemaItem, false);
  schema_members["endTime"] = CObjectSchemaItem::SMember(endTime_SchemaItem, false);
  schema_members["updateMode"] = CObjectSchemaItem::SMember(updateMode_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SetMediaClockTimerID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::IGNORED);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_PerformAudioPassThruID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<SamplingRate::eType> SamplingRate_all_enum_values;
  SamplingRate_all_enum_values.insert(SamplingRate::SamplingRate_8KHZ);
  SamplingRate_all_enum_values.insert(SamplingRate::SamplingRate_16KHZ);
  SamplingRate_all_enum_values.insert(SamplingRate::SamplingRate_22KHZ);
  SamplingRate_all_enum_values.insert(SamplingRate::SamplingRate_44KHZ);

  std::set<BitsPerSample::eType> BitsPerSample_all_enum_values;
  BitsPerSample_all_enum_values.insert(BitsPerSample::BitsPerSample_8_BIT);
  BitsPerSample_all_enum_values.insert(BitsPerSample::BitsPerSample_16_BIT);

  std::set<AudioType::eType> AudioType_all_enum_values;
  AudioType_all_enum_values.insert(AudioType::PCM);

  // Function parameter initialPrompt.
  //
  // 
  //       	SYNC will speak this prompt before opening the audio pass thru session.
  //       	An array of text chunks of type TTSChunk. See TTSChunk.
  //       	The array must have at least one item.
  //       	If omitted, then no initial prompt is spoken.
  //       
  utils::SharedPtr<ISchemaItem> initialPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter audioPassThruDisplayText1.
  //
  // First line of text displayed during audio capture.
  utils::SharedPtr<ISchemaItem> audioPassThruDisplayText1_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter audioPassThruDisplayText2.
  //
  // Second line of text displayed during audio capture.
  utils::SharedPtr<ISchemaItem> audioPassThruDisplayText2_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter samplingRate.
  //
  //  This value shall be allowed at 8 khz or 16 or 22 or 44 khz.
  utils::SharedPtr<ISchemaItem> samplingRate_SchemaItem = TEnumSchemaItem<SamplingRate::eType>::create(SamplingRate_all_enum_values, TSchemaItemParameter<SamplingRate::eType>());

  // Function parameter maxDuration.
  //
  // The maximum duration of audio recording in milliseconds. 
  utils::SharedPtr<ISchemaItem> maxDuration_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(1000000), TSchemaItemParameter<int>());

  // Function parameter bitsPerSample.
  //
  // Specifies the quality the audio is recorded. Currently 8 bit or 16 bit.
  utils::SharedPtr<ISchemaItem> bitsPerSample_SchemaItem = TEnumSchemaItem<BitsPerSample::eType>::create(BitsPerSample_all_enum_values, TSchemaItemParameter<BitsPerSample::eType>());

  // Function parameter audioType.
  //
  // Specifies the type of audio data being requested.
  utils::SharedPtr<ISchemaItem> audioType_SchemaItem = TEnumSchemaItem<AudioType::eType>::create(AudioType_all_enum_values, TSchemaItemParameter<AudioType::eType>());

  // Function parameter muteAudio.
  //
  // 
  // 		Defines if the current audio source should be muted during the APT session.  If not, the audio source will play without interruption.
  //       	If omitted, the value is set to true.
  //       
  utils::SharedPtr<ISchemaItem> muteAudio_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["initialPrompt"] = CObjectSchemaItem::SMember(initialPrompt_SchemaItem, false);
  schema_members["audioPassThruDisplayText1"] = CObjectSchemaItem::SMember(audioPassThruDisplayText1_SchemaItem, false);
  schema_members["audioPassThruDisplayText2"] = CObjectSchemaItem::SMember(audioPassThruDisplayText2_SchemaItem, false);
  schema_members["samplingRate"] = CObjectSchemaItem::SMember(samplingRate_SchemaItem, true);
  schema_members["maxDuration"] = CObjectSchemaItem::SMember(maxDuration_SchemaItem, true);
  schema_members["bitsPerSample"] = CObjectSchemaItem::SMember(bitsPerSample_SchemaItem, true);
  schema_members["audioType"] = CObjectSchemaItem::SMember(audioType_SchemaItem, true);
  schema_members["muteAudio"] = CObjectSchemaItem::SMember(muteAudio_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_PerformAudioPassThruID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::RETRY);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::WARNINGS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::ABORTED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_EndAudioPassThruID_request(
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_EndAudioPassThruID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SubscribeButtonID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<ButtonName::eType> ButtonName_all_enum_values;
  ButtonName_all_enum_values.insert(ButtonName::OK);
  ButtonName_all_enum_values.insert(ButtonName::SEEKLEFT);
  ButtonName_all_enum_values.insert(ButtonName::SEEKRIGHT);
  ButtonName_all_enum_values.insert(ButtonName::TUNEUP);
  ButtonName_all_enum_values.insert(ButtonName::TUNEDOWN);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_0);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_1);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_2);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_3);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_4);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_5);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_6);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_7);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_8);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_9);
  ButtonName_all_enum_values.insert(ButtonName::CUSTOM_BUTTON);
  ButtonName_all_enum_values.insert(ButtonName::SEARCH);

  // Function parameter buttonName.
  //
  // Name of the button to subscribe.
  utils::SharedPtr<ISchemaItem> buttonName_SchemaItem = TEnumSchemaItem<ButtonName::eType>::create(ButtonName_all_enum_values, TSchemaItemParameter<ButtonName::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["buttonName"] = CObjectSchemaItem::SMember(buttonName_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SubscribeButtonID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::IGNORED);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_UnsubscribeButtonID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<ButtonName::eType> ButtonName_all_enum_values;
  ButtonName_all_enum_values.insert(ButtonName::OK);
  ButtonName_all_enum_values.insert(ButtonName::SEEKLEFT);
  ButtonName_all_enum_values.insert(ButtonName::SEEKRIGHT);
  ButtonName_all_enum_values.insert(ButtonName::TUNEUP);
  ButtonName_all_enum_values.insert(ButtonName::TUNEDOWN);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_0);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_1);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_2);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_3);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_4);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_5);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_6);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_7);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_8);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_9);
  ButtonName_all_enum_values.insert(ButtonName::CUSTOM_BUTTON);
  ButtonName_all_enum_values.insert(ButtonName::SEARCH);

  // Function parameter buttonName.
  //
  // Name of the button to unsubscribe.
  utils::SharedPtr<ISchemaItem> buttonName_SchemaItem = TEnumSchemaItem<ButtonName::eType>::create(ButtonName_all_enum_values, TSchemaItemParameter<ButtonName::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["buttonName"] = CObjectSchemaItem::SMember(buttonName_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_UnsubscribeButtonID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::IGNORED);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SubscribeVehicleDataID_request(
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SubscribeVehicleDataID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::IGNORED);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::WARNINGS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::USER_DISALLOWED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter gps.
  //
  // See GPSData
  utils::SharedPtr<ISchemaItem> gps_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter speed.
  //
  // The vehicle speed in kilometers per hour
  utils::SharedPtr<ISchemaItem> speed_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter rpm.
  //
  // The number of revolutions per minute of the engine
  utils::SharedPtr<ISchemaItem> rpm_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter fuelLevel.
  //
  // The fuel level in the tank (percentage)
  utils::SharedPtr<ISchemaItem> fuelLevel_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter fuelLevel_State.
  //
  // The fuel level state
  utils::SharedPtr<ISchemaItem> fuelLevel_State_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter instantFuelConsumption.
  //
  // The instantaneous fuel consumption in microlitres
  utils::SharedPtr<ISchemaItem> instantFuelConsumption_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter externalTemperature.
  //
  // The external temperature in degrees celsius.
  utils::SharedPtr<ISchemaItem> externalTemperature_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter prndl.
  //
  // See PRNDL
  utils::SharedPtr<ISchemaItem> prndl_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter tirePressure.
  //
  // See TireStatus
  utils::SharedPtr<ISchemaItem> tirePressure_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter odometer.
  //
  // Odometer in km
  utils::SharedPtr<ISchemaItem> odometer_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter beltStatus.
  //
  // The status of the seat belts
  utils::SharedPtr<ISchemaItem> beltStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter bodyInformation.
  //
  // The body information including power modes
  utils::SharedPtr<ISchemaItem> bodyInformation_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter deviceStatus.
  //
  // The device status including signal and battery strength
  utils::SharedPtr<ISchemaItem> deviceStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter driverBraking.
  //
  // The status of the brake pedal
  utils::SharedPtr<ISchemaItem> driverBraking_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter wiperStatus.
  //
  // The status of the wipers
  utils::SharedPtr<ISchemaItem> wiperStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter headLampStatus.
  //
  // Status of the head lamps
  utils::SharedPtr<ISchemaItem> headLampStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter engineTorque.
  //
  // Torque value for engine (in Nm) on non-diesel variants
  utils::SharedPtr<ISchemaItem> engineTorque_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter accPedalPosition.
  //
  // Accelerator pedal position (percentage depressed)
  utils::SharedPtr<ISchemaItem> accPedalPosition_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter steeringWheelAngle.
  //
  // Current angle of the steering wheel (in deg)
  utils::SharedPtr<ISchemaItem> steeringWheelAngle_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter eCallInfo.
  //
  // Emergency Call notification and confirmation data
  utils::SharedPtr<ISchemaItem> eCallInfo_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter airbagStatus.
  //
  // The status of the air bags
  utils::SharedPtr<ISchemaItem> airbagStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter emergencyEvent.
  //
  // Information related to an emergency event (and if it occurred)
  utils::SharedPtr<ISchemaItem> emergencyEvent_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter clusterModes.
  //
  // The status modes of the cluster
  utils::SharedPtr<ISchemaItem> clusterModes_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter myKey.
  //
  // Information related to the MyKey feature
  utils::SharedPtr<ISchemaItem> myKey_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
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

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema mobile_apis::MOBILE_API::InitFunction_UnsubscribeVehicleDataID_request(
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_UnsubscribeVehicleDataID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::IGNORED);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::WARNINGS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter gps.
  //
  // See GPSData
  utils::SharedPtr<ISchemaItem> gps_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter speed.
  //
  // The vehicle speed in kilometers per hour
  utils::SharedPtr<ISchemaItem> speed_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter rpm.
  //
  // The number of revolutions per minute of the engine
  utils::SharedPtr<ISchemaItem> rpm_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter fuelLevel.
  //
  // The fuel level in the tank (percentage)
  utils::SharedPtr<ISchemaItem> fuelLevel_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter fuelLevel_State.
  //
  // The fuel level state
  utils::SharedPtr<ISchemaItem> fuelLevel_State_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter instantFuelConsumption.
  //
  // The instantaneous fuel consumption in microlitres
  utils::SharedPtr<ISchemaItem> instantFuelConsumption_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter externalTemperature.
  //
  // The external temperature in degrees celsius
  utils::SharedPtr<ISchemaItem> externalTemperature_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter prndl.
  //
  // See PRNDL
  utils::SharedPtr<ISchemaItem> prndl_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter tirePressure.
  //
  // See TireStatus
  utils::SharedPtr<ISchemaItem> tirePressure_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter odometer.
  //
  // Odometer in km
  utils::SharedPtr<ISchemaItem> odometer_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter beltStatus.
  //
  // The status of the seat belts
  utils::SharedPtr<ISchemaItem> beltStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter bodyInformation.
  //
  // The body information including power modes
  utils::SharedPtr<ISchemaItem> bodyInformation_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter deviceStatus.
  //
  // The device status including signal and battery strength
  utils::SharedPtr<ISchemaItem> deviceStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter driverBraking.
  //
  // The status of the brake pedal
  utils::SharedPtr<ISchemaItem> driverBraking_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter wiperStatus.
  //
  // The status of the wipers
  utils::SharedPtr<ISchemaItem> wiperStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter headLampStatus.
  //
  // Status of the head lamps
  utils::SharedPtr<ISchemaItem> headLampStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter engineTorque.
  //
  // Torque value for engine (in Nm) on non-diesel variants
  utils::SharedPtr<ISchemaItem> engineTorque_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter accPedalPosition.
  //
  // Accelerator pedal position (percentage depressed)
  utils::SharedPtr<ISchemaItem> accPedalPosition_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter steeringWheelAngle.
  //
  // Current angle of the steering wheel (in deg)
  utils::SharedPtr<ISchemaItem> steeringWheelAngle_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter eCallInfo.
  //
  // Emergency Call notification and confirmation data
  utils::SharedPtr<ISchemaItem> eCallInfo_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter airbagStatus.
  //
  // The status of the air bags
  utils::SharedPtr<ISchemaItem> airbagStatus_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter emergencyEvent.
  //
  // Information related to an emergency event (and if it occurred)
  utils::SharedPtr<ISchemaItem> emergencyEvent_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter clusterModes.
  //
  // The status modes of the cluster
  utils::SharedPtr<ISchemaItem> clusterModes_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  // Function parameter myKey.
  //
  // Information related to the MyKey feature
  utils::SharedPtr<ISchemaItem> myKey_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::VehicleDataResult);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
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

  std::map<std::string, CObjectSchemaItem::SMember> root_members_map;
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schema_members), true);
  root_members_map[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(params_members), true);

  return CSmartSchema(CObjectSchemaItem::create(root_members_map));
}

CSmartSchema mobile_apis::MOBILE_API::InitFunction_GetVehicleDataID_request(
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_GetVehicleDataID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::VEHICLE_DATA_NOT_ALLOWED);
  resultCode_allowed_enum_subset_values.insert(Result::USER_DISALLOWED);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::VEHICLE_DATA_NOT_AVAILABLE);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  std::set<ComponentVolumeStatus::eType> ComponentVolumeStatus_all_enum_values;
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_UNKNOWN);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_NORMAL);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_LOW);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_FAULT);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_ALERT);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_NOT_SUPPORTED);

  std::set<PRNDL::eType> PRNDL_all_enum_values;
  PRNDL_all_enum_values.insert(PRNDL::PARK);
  PRNDL_all_enum_values.insert(PRNDL::REVERSE);
  PRNDL_all_enum_values.insert(PRNDL::NEUTRAL);
  PRNDL_all_enum_values.insert(PRNDL::DRIVE);
  PRNDL_all_enum_values.insert(PRNDL::SPORT);
  PRNDL_all_enum_values.insert(PRNDL::LOWGEAR);
  PRNDL_all_enum_values.insert(PRNDL::FIRST);
  PRNDL_all_enum_values.insert(PRNDL::SECOND);
  PRNDL_all_enum_values.insert(PRNDL::THIRD);
  PRNDL_all_enum_values.insert(PRNDL::FOURTH);
  PRNDL_all_enum_values.insert(PRNDL::FIFTH);
  PRNDL_all_enum_values.insert(PRNDL::SIXTH);
  PRNDL_all_enum_values.insert(PRNDL::SEVENTH);
  PRNDL_all_enum_values.insert(PRNDL::EIGHTH);
  PRNDL_all_enum_values.insert(PRNDL::UNKNOWN);
  PRNDL_all_enum_values.insert(PRNDL::FAULT);

  std::set<VehicleDataEventStatus::eType> VehicleDataEventStatus_all_enum_values;
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NO_EVENT);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NO);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_YES);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NOT_SUPPORTED);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_FAULT);

  std::set<WiperStatus::eType> WiperStatus_all_enum_values;
  WiperStatus_all_enum_values.insert(WiperStatus::OFF);
  WiperStatus_all_enum_values.insert(WiperStatus::AUTO_OFF);
  WiperStatus_all_enum_values.insert(WiperStatus::OFF_MOVING);
  WiperStatus_all_enum_values.insert(WiperStatus::MAN_INT_OFF);
  WiperStatus_all_enum_values.insert(WiperStatus::MAN_INT_ON);
  WiperStatus_all_enum_values.insert(WiperStatus::MAN_LOW);
  WiperStatus_all_enum_values.insert(WiperStatus::MAN_HIGH);
  WiperStatus_all_enum_values.insert(WiperStatus::MAN_FLICK);
  WiperStatus_all_enum_values.insert(WiperStatus::WASH);
  WiperStatus_all_enum_values.insert(WiperStatus::AUTO_LOW);
  WiperStatus_all_enum_values.insert(WiperStatus::AUTO_HIGH);
  WiperStatus_all_enum_values.insert(WiperStatus::COURTESYWIPE);
  WiperStatus_all_enum_values.insert(WiperStatus::AUTO_ADJUST);
  WiperStatus_all_enum_values.insert(WiperStatus::STALLED);
  WiperStatus_all_enum_values.insert(WiperStatus::NO_DATA_EXISTS);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter gps.
  //
  // See GPSData
  utils::SharedPtr<ISchemaItem> gps_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::GPSData);

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
  utils::SharedPtr<ISchemaItem> fuelLevel_State_SchemaItem = TEnumSchemaItem<ComponentVolumeStatus::eType>::create(ComponentVolumeStatus_all_enum_values, TSchemaItemParameter<ComponentVolumeStatus::eType>());

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
  utils::SharedPtr<ISchemaItem> prndl_SchemaItem = TEnumSchemaItem<PRNDL::eType>::create(PRNDL_all_enum_values, TSchemaItemParameter<PRNDL::eType>());

  // Function parameter tirePressure.
  //
  // See TireStatus
  utils::SharedPtr<ISchemaItem> tirePressure_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TireStatus);

  // Function parameter odometer.
  //
  // Odometer in km
  utils::SharedPtr<ISchemaItem> odometer_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(17000000), TSchemaItemParameter<int>());

  // Function parameter beltStatus.
  //
  // The status of the seat belts
  utils::SharedPtr<ISchemaItem> beltStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::BeltStatus);

  // Function parameter bodyInformation.
  //
  // The body information including power modes
  utils::SharedPtr<ISchemaItem> bodyInformation_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::BodyInformation);

  // Function parameter deviceStatus.
  //
  // The device status including signal and battery strength
  utils::SharedPtr<ISchemaItem> deviceStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::DeviceStatus);

  // Function parameter driverBraking.
  //
  // The status of the brake pedal
  utils::SharedPtr<ISchemaItem> driverBraking_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Function parameter wiperStatus.
  //
  // The status of the wipers
  utils::SharedPtr<ISchemaItem> wiperStatus_SchemaItem = TEnumSchemaItem<WiperStatus::eType>::create(WiperStatus_all_enum_values, TSchemaItemParameter<WiperStatus::eType>());

  // Function parameter headLampStatus.
  //
  // Status of the head lamps
  utils::SharedPtr<ISchemaItem> headLampStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::HeadLampStatus);

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

  // Function parameter eCallInfo.
  //
  // Emergency Call notification and confirmation data
  utils::SharedPtr<ISchemaItem> eCallInfo_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ECallInfo);

  // Function parameter airbagStatus.
  //
  // The status of the air bags
  utils::SharedPtr<ISchemaItem> airbagStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::AirbagStatus);

  // Function parameter emergencyEvent.
  //
  // Information related to an emergency event (and if it occurred)
  utils::SharedPtr<ISchemaItem> emergencyEvent_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::EmergencyEvent);

  // Function parameter clusterModeStatus.
  //
  // The status modes of the cluster
  utils::SharedPtr<ISchemaItem> clusterModeStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ClusterModeStatus);

  // Function parameter myKey.
  //
  // Information related to the MyKey feature
  utils::SharedPtr<ISchemaItem> myKey_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::MyKey);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ReadDIDID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter ecuName.
  //
  // Name of ECU.
  utils::SharedPtr<ISchemaItem> ecuName_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Function parameter didLocation.
  //
  // Get raw data from vehicle data DID location(s)
  utils::SharedPtr<ISchemaItem> didLocation_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["ecuName"] = CObjectSchemaItem::SMember(ecuName_SchemaItem, true);
  schema_members["didLocation"] = CObjectSchemaItem::SMember(didLocation_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ReadDIDID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::TRUNCATED_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::USER_DISALLOWED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter didResult.
  //
  // Array of requested DID results (with data if available).
  utils::SharedPtr<ISchemaItem> didResult_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::DIDResult), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
  schema_members["didResult"] = CObjectSchemaItem::SMember(didResult_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_GetDTCsID_request(
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

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["ecuName"] = CObjectSchemaItem::SMember(ecuName_SchemaItem, true);
  schema_members["dtcMask"] = CObjectSchemaItem::SMember(dtcMask_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_GetDTCsID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::TRUNCATED_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::USER_DISALLOWED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter ecuHeader.
  //
  // 2 byte ECU Header for DTC response (as defined in VHR_Layout_Specification_DTCs.pdf)
  utils::SharedPtr<ISchemaItem> ecuHeader_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Function parameter dtc.
  //
  // 
  //       	Array of all reported DTCs on module (ecuHeader contains information if list is truncated).
  //       	Each DTC is represented by 4 bytes (3 bytes of data and 1 byte status as defined in VHR_Layout_Specification_DTCs.pdf).
  //       
  utils::SharedPtr<ISchemaItem> dtc_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(10), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(15));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
  schema_members["ecuHeader"] = CObjectSchemaItem::SMember(ecuHeader_SchemaItem, true);
  schema_members["dtc"] = CObjectSchemaItem::SMember(dtc_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_DiagnosticMessageID_request(
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_DiagnosticMessageID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::TRUNCATED_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::USER_DISALLOWED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter messageDataResult.
  //
  // 
  //       	Array of bytes comprising CAN message result.
  //       
  utils::SharedPtr<ISchemaItem> messageDataResult_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(255), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(65535));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
  schema_members["messageDataResult"] = CObjectSchemaItem::SMember(messageDataResult_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ScrollableMessageID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter scrollableMessageBody.
  //
  // Body of text that can include newlines and tabs.
  utils::SharedPtr<ISchemaItem> scrollableMessageBody_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter timeout.
  //
  // App defined timeout.  Indicates how long of a timeout from the last action (i.e. scrolling message resets timeout).
  utils::SharedPtr<ISchemaItem> timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1000), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>(30000));

  // Function parameter softButtons.
  //
  // 
  //       	App defined SoftButtons.
  //       	If omitted on supported displays, only the system defined "Close" SoftButton will be displayed.
  //       
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(8));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["scrollableMessageBody"] = CObjectSchemaItem::SMember(scrollableMessageBody_SchemaItem, true);
  schema_members["timeout"] = CObjectSchemaItem::SMember(timeout_SchemaItem, false);
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ScrollableMessageID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::CHAR_LIMIT_EXCEEDED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::ABORTED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SliderID_request(
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
  // Text header to display
  utils::SharedPtr<ISchemaItem> sliderHeader_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter sliderFooter.
  //
  // 
  //       	Text footer to display (meant to display min/max threshold descriptors).
  //       	For a static text footer, only one footer string shall be provided in the array.
  //       	For a dynamic text footer, the number of footer text string in the array must match the numTicks value.
  //       	For a dynamic text footer, text array string should correlate with potential slider position index.
  //       	If omitted on supported displays, no footer text shall be displayed.
  //       
  utils::SharedPtr<ISchemaItem> sliderFooter_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(26));

  // Function parameter timeout.
  //
  // 
  //       	App defined timeout.  Indicates how long of a timeout from the last action (i.e. sliding control resets timeout).
  //       	If omitted, the value is set to 10000.
  //       
  utils::SharedPtr<ISchemaItem> timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1000), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>(10000));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["numTicks"] = CObjectSchemaItem::SMember(numTicks_SchemaItem, true);
  schema_members["position"] = CObjectSchemaItem::SMember(position_SchemaItem, true);
  schema_members["sliderHeader"] = CObjectSchemaItem::SMember(sliderHeader_SchemaItem, true);
  schema_members["sliderFooter"] = CObjectSchemaItem::SMember(sliderFooter_SchemaItem, false);
  schema_members["timeout"] = CObjectSchemaItem::SMember(timeout_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SliderID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::ABORTED);
  resultCode_allowed_enum_subset_values.insert(Result::SAVED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter sliderPosition.
  //
  // 
  //       	Current slider value returned when saved or canceled (aborted)
  //       	This value is only returned for resultCodes "SAVED" or "ABORTED"
  //       
  utils::SharedPtr<ISchemaItem> sliderPosition_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(26), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
  schema_members["sliderPosition"] = CObjectSchemaItem::SMember(sliderPosition_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ShowConstantTBTID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter navigationText1.
  utils::SharedPtr<ISchemaItem> navigationText1_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter navigationText2.
  utils::SharedPtr<ISchemaItem> navigationText2_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter eta.
  utils::SharedPtr<ISchemaItem> eta_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter timeToDestination.
  utils::SharedPtr<ISchemaItem> timeToDestination_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter totalDistance.
  utils::SharedPtr<ISchemaItem> totalDistance_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter turnIcon.
  utils::SharedPtr<ISchemaItem> turnIcon_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Image);

  // Function parameter nextTurnIcon.
  utils::SharedPtr<ISchemaItem> nextTurnIcon_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Image);

  // Function parameter distanceToManeuver.
  //
  // 
  //       	Fraction of distance till next maneuver (starting from when AlertManeuver is triggered).
  //       	Used to calculate progress bar.
  //       
  utils::SharedPtr<ISchemaItem> distanceToManeuver_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(1000000000.0), TSchemaItemParameter<double>());

  // Function parameter distanceToManeuverScale.
  //
  // 
  //       	Distance till next maneuver (starting from) from previous maneuver.
  //       	Used to calculate progress bar.
  //       
  utils::SharedPtr<ISchemaItem> distanceToManeuverScale_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(1000000000.0), TSchemaItemParameter<double>());

  // Function parameter maneuverComplete.
  //
  // 
  //       	If and when a maneuver has completed while an AlertManeuver is active, the app must send this value set to TRUE in order to clear the AlertManeuver overlay.
  //       	If omitted the value will be assumed as FALSE.
  //       
  utils::SharedPtr<ISchemaItem> maneuverComplete_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter softButtons.
  //
  // 
  //       	Three dynamic SoftButtons available (first SoftButton is fixed to "Turns").
  //       	If omitted on supported displays, the currently displayed SoftButton values will not change.
  //       
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(3));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["navigationText1"] = CObjectSchemaItem::SMember(navigationText1_SchemaItem, false);
  schema_members["navigationText2"] = CObjectSchemaItem::SMember(navigationText2_SchemaItem, false);
  schema_members["eta"] = CObjectSchemaItem::SMember(eta_SchemaItem, false);
  schema_members["timeToDestination"] = CObjectSchemaItem::SMember(timeToDestination_SchemaItem, false);
  schema_members["totalDistance"] = CObjectSchemaItem::SMember(totalDistance_SchemaItem, false);
  schema_members["turnIcon"] = CObjectSchemaItem::SMember(turnIcon_SchemaItem, false);
  schema_members["nextTurnIcon"] = CObjectSchemaItem::SMember(nextTurnIcon_SchemaItem, false);
  schema_members["distanceToManeuver"] = CObjectSchemaItem::SMember(distanceToManeuver_SchemaItem, false);
  schema_members["distanceToManeuverScale"] = CObjectSchemaItem::SMember(distanceToManeuverScale_SchemaItem, false);
  schema_members["maneuverComplete"] = CObjectSchemaItem::SMember(maneuverComplete_SchemaItem, false);
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ShowConstantTBTID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_REQUEST);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_AlertManeuverID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter ttsChunks.
  //
  // An array of text chunks of type TTSChunk. See TTSChunk
  utils::SharedPtr<ISchemaItem> ttsChunks_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter softButtons.
  //
  // If omitted on supported displays, only the system defined "Close" SoftButton shall be displayed.
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(3));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["ttsChunks"] = CObjectSchemaItem::SMember(ttsChunks_SchemaItem, false);
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_AlertManeuverID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::IGNORED);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::WARNINGS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_REQUEST);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::ABORTED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_UpdateTurnListID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter turnList.
  utils::SharedPtr<ISchemaItem> turnList_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Turn), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter softButtons.
  //
  // If omitted on supported displays, app-defined SoftButton will be left blank.
  utils::SharedPtr<ISchemaItem> softButtons_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SoftButton), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["turnList"] = CObjectSchemaItem::SMember(turnList_SchemaItem, false);
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_UpdateTurnListID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_REQUEST);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_RESOURCE);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ChangeRegistrationID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Language::eType> Language_all_enum_values;
  Language_all_enum_values.insert(Language::EN_US);
  Language_all_enum_values.insert(Language::ES_MX);
  Language_all_enum_values.insert(Language::FR_CA);
  Language_all_enum_values.insert(Language::DE_DE);
  Language_all_enum_values.insert(Language::ES_ES);
  Language_all_enum_values.insert(Language::EN_GB);
  Language_all_enum_values.insert(Language::RU_RU);
  Language_all_enum_values.insert(Language::TR_TR);
  Language_all_enum_values.insert(Language::PL_PL);
  Language_all_enum_values.insert(Language::FR_FR);
  Language_all_enum_values.insert(Language::IT_IT);
  Language_all_enum_values.insert(Language::SV_SE);
  Language_all_enum_values.insert(Language::PT_PT);
  Language_all_enum_values.insert(Language::NL_NL);
  Language_all_enum_values.insert(Language::EN_AU);
  Language_all_enum_values.insert(Language::ZH_CN);
  Language_all_enum_values.insert(Language::ZH_TW);
  Language_all_enum_values.insert(Language::JA_JP);
  Language_all_enum_values.insert(Language::AR_SA);
  Language_all_enum_values.insert(Language::KO_KR);
  Language_all_enum_values.insert(Language::PT_BR);
  Language_all_enum_values.insert(Language::CS_CZ);
  Language_all_enum_values.insert(Language::DA_DK);
  Language_all_enum_values.insert(Language::NO_NO);

  // Function parameter language.
  //
  // Requested SYNC voice engine (VR+TTS) language registration
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Language::eType>::create(Language_all_enum_values, TSchemaItemParameter<Language::eType>());

  // Function parameter hmiDisplayLanguage.
  //
  // Request display language registration
  utils::SharedPtr<ISchemaItem> hmiDisplayLanguage_SchemaItem = TEnumSchemaItem<Language::eType>::create(Language_all_enum_values, TSchemaItemParameter<Language::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);
  schema_members["hmiDisplayLanguage"] = CObjectSchemaItem::SMember(hmiDisplayLanguage_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ChangeRegistrationID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);
  resultCode_allowed_enum_subset_values.insert(Result::DISALLOWED);

  // Function parameter success.
  //
  // 
  // 		  	true, if successful
  // 		  	false, if failed
  // 		  
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_GenericResponseID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_PutFileID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<FileType::eType> FileType_all_enum_values;
  FileType_all_enum_values.insert(FileType::GRAPHIC_BMP);
  FileType_all_enum_values.insert(FileType::GRAPHIC_JPEG);
  FileType_all_enum_values.insert(FileType::GRAPHIC_PNG);
  FileType_all_enum_values.insert(FileType::AUDIO_WAVE);
  FileType_all_enum_values.insert(FileType::AUDIO_MP3);
  FileType_all_enum_values.insert(FileType::AUDIO_AAC);
  FileType_all_enum_values.insert(FileType::BINARY);
  FileType_all_enum_values.insert(FileType::JSON);

  // Function parameter syncFileName.
  //
  // File reference name.
  utils::SharedPtr<ISchemaItem> syncFileName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(255), TSchemaItemParameter<std::string>());

  // Function parameter fileType.
  //
  // Selected file type.
  utils::SharedPtr<ISchemaItem> fileType_SchemaItem = TEnumSchemaItem<FileType::eType>::create(FileType_all_enum_values, TSchemaItemParameter<FileType::eType>());

  // Function parameter persistentFile.
  //
  // 
  //       	Indicates if the file is meant to persist between sessions / ignition cycles.
  //       	If set to TRUE, then the system will aim to persist this file through session / cycles.
  //       	While files with this designation will have priority over others, they are subject to deletion by the system at any time.
  //       	In the event of automatic deletion by the system, the app will receive a rejection and have to resend the file.
  //       	If omitted, the value will be set to false.
  //       
  utils::SharedPtr<ISchemaItem> persistentFile_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>(false));

  // Function parameter systemFile.
  //
  // Indicates if the file is meant to be passed thru core to elsewhere on the system.
  //      If set to TRUE, then the system will instead pass the data thru as it arrives to a predetermined area 	 outside of core. If omitted, the value will be set to false.
  utils::SharedPtr<ISchemaItem> systemFile_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>(false));

  // Function parameter offset.
  //
  // Optional offset in bytes for resuming partial data chunks
  utils::SharedPtr<ISchemaItem> offset_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(100000000000), TSchemaItemParameter<int>());

  // Function parameter length.
  //
  // Optional length in bytes for resuming partial data chunks
  utils::SharedPtr<ISchemaItem> length_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(100000000000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["syncFileName"] = CObjectSchemaItem::SMember(syncFileName_SchemaItem, true);
  schema_members["fileType"] = CObjectSchemaItem::SMember(fileType_SchemaItem, true);
  schema_members["persistentFile"] = CObjectSchemaItem::SMember(persistentFile_SchemaItem, false);
  schema_members["systemFile"] = CObjectSchemaItem::SMember(systemFile_SchemaItem, false);
  schema_members["offset"] = CObjectSchemaItem::SMember(offset_SchemaItem, false);
  schema_members["length"] = CObjectSchemaItem::SMember(length_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_PutFileID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_REQUEST);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter spaceAvailable.
  //
  // 
  //             Provides the total local space available on SYNC for the registered app.
  //             If the transfer has systemFile enabled, then the value will be set to 0 automatically.
  //           
  utils::SharedPtr<ISchemaItem> spaceAvailable_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["spaceAvailable"] = CObjectSchemaItem::SMember(spaceAvailable_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_DeleteFileID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter syncFileName.
  //
  // File reference name.
  utils::SharedPtr<ISchemaItem> syncFileName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(255), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["syncFileName"] = CObjectSchemaItem::SMember(syncFileName_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_DeleteFileID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_REQUEST);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter spaceAvailable.
  //
  // Provides the total local space available on SYNC for the registered app.
  utils::SharedPtr<ISchemaItem> spaceAvailable_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["spaceAvailable"] = CObjectSchemaItem::SMember(spaceAvailable_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ListFilesID_request(
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_ListFilesID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_REQUEST);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter filenames.
  //
  // 
  //       	An array of all filenames resident on SYNC for the given registered app.
  //       	If omitted, then no files currently reside on the system.
  //       
  utils::SharedPtr<ISchemaItem> filenames_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000));

  // Function parameter spaceAvailable.
  //
  // Provides the total local space available on SYNC for the registered app.
  utils::SharedPtr<ISchemaItem> spaceAvailable_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["filenames"] = CObjectSchemaItem::SMember(filenames_SchemaItem, false);
  schema_members["spaceAvailable"] = CObjectSchemaItem::SMember(spaceAvailable_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SetAppIconID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter syncFileName.
  //
  // File reference name.
  utils::SharedPtr<ISchemaItem> syncFileName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(255), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["syncFileName"] = CObjectSchemaItem::SMember(syncFileName_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SetAppIconID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_REQUEST);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SetDisplayLayoutID_request(
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

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["displayLayout"] = CObjectSchemaItem::SMember(displayLayout_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnHashChangeID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter hashID.
  //
  // Calculated hash ID to be referenced during RegisterAppInterface.
  utils::SharedPtr<ISchemaItem> hashID_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["hashID"] = CObjectSchemaItem::SMember(hashID_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SetDisplayLayoutID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_REQUEST);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter displayCapabilities.
  //
  // See DisplayCapabilities
  utils::SharedPtr<ISchemaItem> displayCapabilities_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::DisplayCapabilities);

  // Function parameter buttonCapabilities.
  //
  // See ButtonCapabilities
  utils::SharedPtr<ISchemaItem> buttonCapabilities_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ButtonCapabilities), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter softButtonCapabilities.
  //
  // If returned, the platform supports on-screen SoftButtons; see SoftButtonCapabilities.
  utils::SharedPtr<ISchemaItem> softButtonCapabilities_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SoftButtonCapabilities), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter presetBankCapabilities.
  //
  // If returned, the platform supports custom on-screen Presets; see PresetBankCapabilities.
  utils::SharedPtr<ISchemaItem> presetBankCapabilities_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::PresetBankCapabilities);

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["displayCapabilities"] = CObjectSchemaItem::SMember(displayCapabilities_SchemaItem, false);
  schema_members["buttonCapabilities"] = CObjectSchemaItem::SMember(buttonCapabilities_SchemaItem, false);
  schema_members["softButtonCapabilities"] = CObjectSchemaItem::SMember(softButtonCapabilities_SchemaItem, false);
  schema_members["presetBankCapabilities"] = CObjectSchemaItem::SMember(presetBankCapabilities_SchemaItem, false);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SystemRequestID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<RequestType::eType> RequestType_all_enum_values;
  RequestType_all_enum_values.insert(RequestType::HTTP);
  RequestType_all_enum_values.insert(RequestType::FILE_RESUME);
  RequestType_all_enum_values.insert(RequestType::AUTH_REQUEST);
  RequestType_all_enum_values.insert(RequestType::AUTH_CHALLENGE);
  RequestType_all_enum_values.insert(RequestType::AUTH_ACK);
  RequestType_all_enum_values.insert(RequestType::PROPRIETARY);

  // Function parameter requestType.
  //
  // The type of system request.
  utils::SharedPtr<ISchemaItem> requestType_SchemaItem = TEnumSchemaItem<RequestType::eType>::create(RequestType_all_enum_values, TSchemaItemParameter<RequestType::eType>());

  // Function parameter fileName.
  //
  // The name of file.
  utils::SharedPtr<ISchemaItem> fileName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(255), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["requestType"] = CObjectSchemaItem::SMember(requestType_SchemaItem, true);
  schema_members["fileName"] = CObjectSchemaItem::SMember(fileName_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SystemRequestID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::EXPIRED_CERT);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_CERT);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnHMIStatusID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<HMILevel::eType> HMILevel_all_enum_values;
  HMILevel_all_enum_values.insert(HMILevel::HMI_FULL);
  HMILevel_all_enum_values.insert(HMILevel::HMI_LIMITED);
  HMILevel_all_enum_values.insert(HMILevel::HMI_BACKGROUND);
  HMILevel_all_enum_values.insert(HMILevel::HMI_NONE);

  std::set<AudioStreamingState::eType> AudioStreamingState_all_enum_values;
  AudioStreamingState_all_enum_values.insert(AudioStreamingState::AUDIBLE);
  AudioStreamingState_all_enum_values.insert(AudioStreamingState::ATTENUATED);
  AudioStreamingState_all_enum_values.insert(AudioStreamingState::NOT_AUDIBLE);

  std::set<SystemContext::eType> SystemContext_all_enum_values;
  SystemContext_all_enum_values.insert(SystemContext::SYSCTXT_MAIN);
  SystemContext_all_enum_values.insert(SystemContext::SYSCTXT_VRSESSION);
  SystemContext_all_enum_values.insert(SystemContext::SYSCTXT_MENU);
  SystemContext_all_enum_values.insert(SystemContext::SYSCTXT_HMI_OBSCURED);
  SystemContext_all_enum_values.insert(SystemContext::SYSCTXT_ALERT);

  // Function parameter hmiLevel.
  //
  // See HMILevel
  utils::SharedPtr<ISchemaItem> hmiLevel_SchemaItem = TEnumSchemaItem<HMILevel::eType>::create(HMILevel_all_enum_values, TSchemaItemParameter<HMILevel::eType>());

  // Function parameter audioStreamingState.
  //
  // See AudioStreamingState
  utils::SharedPtr<ISchemaItem> audioStreamingState_SchemaItem = TEnumSchemaItem<AudioStreamingState::eType>::create(AudioStreamingState_all_enum_values, TSchemaItemParameter<AudioStreamingState::eType>());

  // Function parameter systemContext.
  //
  // See SystemContext
  utils::SharedPtr<ISchemaItem> systemContext_SchemaItem = TEnumSchemaItem<SystemContext::eType>::create(SystemContext_all_enum_values, TSchemaItemParameter<SystemContext::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["hmiLevel"] = CObjectSchemaItem::SMember(hmiLevel_SchemaItem, true);
  schema_members["audioStreamingState"] = CObjectSchemaItem::SMember(audioStreamingState_SchemaItem, true);
  schema_members["systemContext"] = CObjectSchemaItem::SMember(systemContext_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnAppInterfaceUnregisteredID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<AppInterfaceUnregisteredReason::eType> AppInterfaceUnregisteredReason_all_enum_values;
  AppInterfaceUnregisteredReason_all_enum_values.insert(AppInterfaceUnregisteredReason::USER_EXIT);
  AppInterfaceUnregisteredReason_all_enum_values.insert(AppInterfaceUnregisteredReason::IGNITION_OFF);
  AppInterfaceUnregisteredReason_all_enum_values.insert(AppInterfaceUnregisteredReason::BLUETOOTH_OFF);
  AppInterfaceUnregisteredReason_all_enum_values.insert(AppInterfaceUnregisteredReason::USB_DISCONNECTED);
  AppInterfaceUnregisteredReason_all_enum_values.insert(AppInterfaceUnregisteredReason::REQUEST_WHILE_IN_NONE_HMI_LEVEL);
  AppInterfaceUnregisteredReason_all_enum_values.insert(AppInterfaceUnregisteredReason::TOO_MANY_REQUESTS);
  AppInterfaceUnregisteredReason_all_enum_values.insert(AppInterfaceUnregisteredReason::DRIVER_DISTRACTION_VIOLATION);
  AppInterfaceUnregisteredReason_all_enum_values.insert(AppInterfaceUnregisteredReason::LANGUAGE_CHANGE);
  AppInterfaceUnregisteredReason_all_enum_values.insert(AppInterfaceUnregisteredReason::MASTER_RESET);
  AppInterfaceUnregisteredReason_all_enum_values.insert(AppInterfaceUnregisteredReason::FACTORY_DEFAULTS);
  AppInterfaceUnregisteredReason_all_enum_values.insert(AppInterfaceUnregisteredReason::APP_UNAUTHORIZED);

  // Function parameter reason.
  //
  // See AppInterfaceUnregisteredReason
  utils::SharedPtr<ISchemaItem> reason_SchemaItem = TEnumSchemaItem<AppInterfaceUnregisteredReason::eType>::create(AppInterfaceUnregisteredReason_all_enum_values, TSchemaItemParameter<AppInterfaceUnregisteredReason::eType>());

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnButtonEventID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<ButtonName::eType> ButtonName_all_enum_values;
  ButtonName_all_enum_values.insert(ButtonName::OK);
  ButtonName_all_enum_values.insert(ButtonName::SEEKLEFT);
  ButtonName_all_enum_values.insert(ButtonName::SEEKRIGHT);
  ButtonName_all_enum_values.insert(ButtonName::TUNEUP);
  ButtonName_all_enum_values.insert(ButtonName::TUNEDOWN);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_0);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_1);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_2);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_3);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_4);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_5);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_6);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_7);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_8);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_9);
  ButtonName_all_enum_values.insert(ButtonName::CUSTOM_BUTTON);
  ButtonName_all_enum_values.insert(ButtonName::SEARCH);

  std::set<ButtonEventMode::eType> ButtonEventMode_all_enum_values;
  ButtonEventMode_all_enum_values.insert(ButtonEventMode::BUTTONUP);
  ButtonEventMode_all_enum_values.insert(ButtonEventMode::BUTTONDOWN);

  // Function parameter buttonName.
  utils::SharedPtr<ISchemaItem> buttonName_SchemaItem = TEnumSchemaItem<ButtonName::eType>::create(ButtonName_all_enum_values, TSchemaItemParameter<ButtonName::eType>());

  // Function parameter buttonEventMode.
  //
  // Indicates whether this is an UP or DOWN event.
  utils::SharedPtr<ISchemaItem> buttonEventMode_SchemaItem = TEnumSchemaItem<ButtonEventMode::eType>::create(ButtonEventMode_all_enum_values, TSchemaItemParameter<ButtonEventMode::eType>());

  // Function parameter customButtonID.
  //
  // If ButtonName is "CUSTOM_BUTTON", this references the integer ID passed by a custom button. (e.g. softButton ID)
  utils::SharedPtr<ISchemaItem> customButtonID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65536), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["buttonName"] = CObjectSchemaItem::SMember(buttonName_SchemaItem, true);
  schema_members["buttonEventMode"] = CObjectSchemaItem::SMember(buttonEventMode_SchemaItem, true);
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnButtonPressID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<ButtonName::eType> ButtonName_all_enum_values;
  ButtonName_all_enum_values.insert(ButtonName::OK);
  ButtonName_all_enum_values.insert(ButtonName::SEEKLEFT);
  ButtonName_all_enum_values.insert(ButtonName::SEEKRIGHT);
  ButtonName_all_enum_values.insert(ButtonName::TUNEUP);
  ButtonName_all_enum_values.insert(ButtonName::TUNEDOWN);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_0);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_1);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_2);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_3);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_4);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_5);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_6);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_7);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_8);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_9);
  ButtonName_all_enum_values.insert(ButtonName::CUSTOM_BUTTON);
  ButtonName_all_enum_values.insert(ButtonName::SEARCH);

  std::set<ButtonPressMode::eType> ButtonPressMode_all_enum_values;
  ButtonPressMode_all_enum_values.insert(ButtonPressMode::LONG);
  ButtonPressMode_all_enum_values.insert(ButtonPressMode::SHORT);

  // Function parameter buttonName.
  utils::SharedPtr<ISchemaItem> buttonName_SchemaItem = TEnumSchemaItem<ButtonName::eType>::create(ButtonName_all_enum_values, TSchemaItemParameter<ButtonName::eType>());

  // Function parameter buttonPressMode.
  //
  // Indicates whether this is a LONG or SHORT button press event.
  utils::SharedPtr<ISchemaItem> buttonPressMode_SchemaItem = TEnumSchemaItem<ButtonPressMode::eType>::create(ButtonPressMode_all_enum_values, TSchemaItemParameter<ButtonPressMode::eType>());

  // Function parameter customButtonID.
  //
  // If ButtonName is "CUSTOM_BUTTON", this references the integer ID passed by a custom button. (e.g. softButton ID)
  utils::SharedPtr<ISchemaItem> customButtonID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65536), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["buttonName"] = CObjectSchemaItem::SMember(buttonName_SchemaItem, true);
  schema_members["buttonPressMode"] = CObjectSchemaItem::SMember(buttonPressMode_SchemaItem, true);
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnVehicleDataID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<ComponentVolumeStatus::eType> ComponentVolumeStatus_all_enum_values;
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_UNKNOWN);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_NORMAL);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_LOW);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_FAULT);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_ALERT);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_NOT_SUPPORTED);

  std::set<PRNDL::eType> PRNDL_all_enum_values;
  PRNDL_all_enum_values.insert(PRNDL::PARK);
  PRNDL_all_enum_values.insert(PRNDL::REVERSE);
  PRNDL_all_enum_values.insert(PRNDL::NEUTRAL);
  PRNDL_all_enum_values.insert(PRNDL::DRIVE);
  PRNDL_all_enum_values.insert(PRNDL::SPORT);
  PRNDL_all_enum_values.insert(PRNDL::LOWGEAR);
  PRNDL_all_enum_values.insert(PRNDL::FIRST);
  PRNDL_all_enum_values.insert(PRNDL::SECOND);
  PRNDL_all_enum_values.insert(PRNDL::THIRD);
  PRNDL_all_enum_values.insert(PRNDL::FOURTH);
  PRNDL_all_enum_values.insert(PRNDL::FIFTH);
  PRNDL_all_enum_values.insert(PRNDL::SIXTH);
  PRNDL_all_enum_values.insert(PRNDL::SEVENTH);
  PRNDL_all_enum_values.insert(PRNDL::EIGHTH);
  PRNDL_all_enum_values.insert(PRNDL::UNKNOWN);
  PRNDL_all_enum_values.insert(PRNDL::FAULT);

  std::set<VehicleDataEventStatus::eType> VehicleDataEventStatus_all_enum_values;
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NO_EVENT);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NO);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_YES);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NOT_SUPPORTED);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_FAULT);

  std::set<WiperStatus::eType> WiperStatus_all_enum_values;
  WiperStatus_all_enum_values.insert(WiperStatus::OFF);
  WiperStatus_all_enum_values.insert(WiperStatus::AUTO_OFF);
  WiperStatus_all_enum_values.insert(WiperStatus::OFF_MOVING);
  WiperStatus_all_enum_values.insert(WiperStatus::MAN_INT_OFF);
  WiperStatus_all_enum_values.insert(WiperStatus::MAN_INT_ON);
  WiperStatus_all_enum_values.insert(WiperStatus::MAN_LOW);
  WiperStatus_all_enum_values.insert(WiperStatus::MAN_HIGH);
  WiperStatus_all_enum_values.insert(WiperStatus::MAN_FLICK);
  WiperStatus_all_enum_values.insert(WiperStatus::WASH);
  WiperStatus_all_enum_values.insert(WiperStatus::AUTO_LOW);
  WiperStatus_all_enum_values.insert(WiperStatus::AUTO_HIGH);
  WiperStatus_all_enum_values.insert(WiperStatus::COURTESYWIPE);
  WiperStatus_all_enum_values.insert(WiperStatus::AUTO_ADJUST);
  WiperStatus_all_enum_values.insert(WiperStatus::STALLED);
  WiperStatus_all_enum_values.insert(WiperStatus::NO_DATA_EXISTS);

  // Function parameter gps.
  //
  // See GPSData
  utils::SharedPtr<ISchemaItem> gps_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::GPSData);

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
  utils::SharedPtr<ISchemaItem> fuelLevel_State_SchemaItem = TEnumSchemaItem<ComponentVolumeStatus::eType>::create(ComponentVolumeStatus_all_enum_values, TSchemaItemParameter<ComponentVolumeStatus::eType>());

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
  utils::SharedPtr<ISchemaItem> prndl_SchemaItem = TEnumSchemaItem<PRNDL::eType>::create(PRNDL_all_enum_values, TSchemaItemParameter<PRNDL::eType>());

  // Function parameter tirePressure.
  //
  // See TireStatus
  utils::SharedPtr<ISchemaItem> tirePressure_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TireStatus);

  // Function parameter odometer.
  //
  // Odometer in km
  utils::SharedPtr<ISchemaItem> odometer_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(17000000), TSchemaItemParameter<int>());

  // Function parameter beltStatus.
  //
  // The status of the seat belts
  utils::SharedPtr<ISchemaItem> beltStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::BeltStatus);

  // Function parameter bodyInformation.
  //
  // The body information including power modes
  utils::SharedPtr<ISchemaItem> bodyInformation_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::BodyInformation);

  // Function parameter deviceStatus.
  //
  // The device status including signal and battery strength
  utils::SharedPtr<ISchemaItem> deviceStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::DeviceStatus);

  // Function parameter driverBraking.
  //
  // The status of the brake pedal
  utils::SharedPtr<ISchemaItem> driverBraking_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Function parameter wiperStatus.
  //
  // The status of the wipers
  utils::SharedPtr<ISchemaItem> wiperStatus_SchemaItem = TEnumSchemaItem<WiperStatus::eType>::create(WiperStatus_all_enum_values, TSchemaItemParameter<WiperStatus::eType>());

  // Function parameter headLampStatus.
  //
  // Status of the head lamps
  utils::SharedPtr<ISchemaItem> headLampStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::HeadLampStatus);

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

  // Function parameter eCallInfo.
  //
  // Emergency Call notification and confirmation data
  utils::SharedPtr<ISchemaItem> eCallInfo_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ECallInfo);

  // Function parameter airbagStatus.
  //
  // The status of the air bags
  utils::SharedPtr<ISchemaItem> airbagStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::AirbagStatus);

  // Function parameter emergencyEvent.
  //
  // Information related to an emergency event (and if it occurred)
  utils::SharedPtr<ISchemaItem> emergencyEvent_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::EmergencyEvent);

  // Function parameter clusterModeStatus.
  //
  // The status modes of the cluster
  utils::SharedPtr<ISchemaItem> clusterModeStatus_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ClusterModeStatus);

  // Function parameter myKey.
  //
  // Information related to the MyKey feature
  utils::SharedPtr<ISchemaItem> myKey_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::MyKey);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnCommandID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<TriggerSource::eType> TriggerSource_all_enum_values;
  TriggerSource_all_enum_values.insert(TriggerSource::TS_MENU);
  TriggerSource_all_enum_values.insert(TriggerSource::TS_VR);
  TriggerSource_all_enum_values.insert(TriggerSource::TS_KEYBOARD);

  // Function parameter cmdID.
  //
  // Command ID, which is related to a specific menu entry
  utils::SharedPtr<ISchemaItem> cmdID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter triggerSource.
  //
  // See TriggerSource
  utils::SharedPtr<ISchemaItem> triggerSource_SchemaItem = TEnumSchemaItem<TriggerSource::eType>::create(TriggerSource_all_enum_values, TSchemaItemParameter<TriggerSource::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["cmdID"] = CObjectSchemaItem::SMember(cmdID_SchemaItem, true);
  schema_members["triggerSource"] = CObjectSchemaItem::SMember(triggerSource_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnTBTClientStateID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<TBTState::eType> TBTState_all_enum_values;
  TBTState_all_enum_values.insert(TBTState::ROUTE_UPDATE_REQUEST);
  TBTState_all_enum_values.insert(TBTState::ROUTE_ACCEPTED);
  TBTState_all_enum_values.insert(TBTState::ROUTE_REFUSED);
  TBTState_all_enum_values.insert(TBTState::ROUTE_CANCELLED);
  TBTState_all_enum_values.insert(TBTState::ETA_REQUEST);
  TBTState_all_enum_values.insert(TBTState::NEXT_TURN_REQUEST);
  TBTState_all_enum_values.insert(TBTState::ROUTE_STATUS_REQUEST);
  TBTState_all_enum_values.insert(TBTState::ROUTE_SUMMARY_REQUEST);
  TBTState_all_enum_values.insert(TBTState::TRIP_STATUS_REQUEST);
  TBTState_all_enum_values.insert(TBTState::ROUTE_UPDATE_REQUEST_TIMEOUT);

  // Function parameter state.
  //
  // Current State of TBT client
  utils::SharedPtr<ISchemaItem> state_SchemaItem = TEnumSchemaItem<TBTState::eType>::create(TBTState_all_enum_values, TSchemaItemParameter<TBTState::eType>());

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnDriverDistractionID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<DriverDistractionState::eType> DriverDistractionState_all_enum_values;
  DriverDistractionState_all_enum_values.insert(DriverDistractionState::DD_ON);
  DriverDistractionState_all_enum_values.insert(DriverDistractionState::DD_OFF);

  // Function parameter state.
  //
  // Current State of Driver Distraction
  utils::SharedPtr<ISchemaItem> state_SchemaItem = TEnumSchemaItem<DriverDistractionState::eType>::create(DriverDistractionState_all_enum_values, TSchemaItemParameter<DriverDistractionState::eType>());

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnPermissionsChangeID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter permissionItem.
  //
  // Change in permissions for a given set of RPCs
  utils::SharedPtr<ISchemaItem> permissionItem_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::PermissionItem), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["permissionItem"] = CObjectSchemaItem::SMember(permissionItem_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnAudioPassThruID_notification(
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnLanguageChangeID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Language::eType> Language_all_enum_values;
  Language_all_enum_values.insert(Language::EN_US);
  Language_all_enum_values.insert(Language::ES_MX);
  Language_all_enum_values.insert(Language::FR_CA);
  Language_all_enum_values.insert(Language::DE_DE);
  Language_all_enum_values.insert(Language::ES_ES);
  Language_all_enum_values.insert(Language::EN_GB);
  Language_all_enum_values.insert(Language::RU_RU);
  Language_all_enum_values.insert(Language::TR_TR);
  Language_all_enum_values.insert(Language::PL_PL);
  Language_all_enum_values.insert(Language::FR_FR);
  Language_all_enum_values.insert(Language::IT_IT);
  Language_all_enum_values.insert(Language::SV_SE);
  Language_all_enum_values.insert(Language::PT_PT);
  Language_all_enum_values.insert(Language::NL_NL);
  Language_all_enum_values.insert(Language::EN_AU);
  Language_all_enum_values.insert(Language::ZH_CN);
  Language_all_enum_values.insert(Language::ZH_TW);
  Language_all_enum_values.insert(Language::JA_JP);
  Language_all_enum_values.insert(Language::AR_SA);
  Language_all_enum_values.insert(Language::KO_KR);
  Language_all_enum_values.insert(Language::PT_BR);
  Language_all_enum_values.insert(Language::CS_CZ);
  Language_all_enum_values.insert(Language::DA_DK);
  Language_all_enum_values.insert(Language::NO_NO);

  // Function parameter language.
  //
  // Current SYNC voice engine (VR+TTS) language
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Language::eType>::create(Language_all_enum_values, TSchemaItemParameter<Language::eType>());

  // Function parameter hmiDisplayLanguage.
  //
  // Current display language
  utils::SharedPtr<ISchemaItem> hmiDisplayLanguage_SchemaItem = TEnumSchemaItem<Language::eType>::create(Language_all_enum_values, TSchemaItemParameter<Language::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, true);
  schema_members["hmiDisplayLanguage"] = CObjectSchemaItem::SMember(hmiDisplayLanguage_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnKeyboardInputID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<KeyboardEvent::eType> KeyboardEvent_all_enum_values;
  KeyboardEvent_all_enum_values.insert(KeyboardEvent::KEYPRESS);
  KeyboardEvent_all_enum_values.insert(KeyboardEvent::ENTRY_SUBMITTED);
  KeyboardEvent_all_enum_values.insert(KeyboardEvent::ENTRY_CANCELLED);
  KeyboardEvent_all_enum_values.insert(KeyboardEvent::ENTRY_ABORTED);

  // Function parameter event.
  //
  // On-screen keyboard input data.
  utils::SharedPtr<ISchemaItem> event_SchemaItem = TEnumSchemaItem<KeyboardEvent::eType>::create(KeyboardEvent_all_enum_values, TSchemaItemParameter<KeyboardEvent::eType>());

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnTouchEventID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<TouchType::eType> TouchType_all_enum_values;
  TouchType_all_enum_values.insert(TouchType::BEGIN);
  TouchType_all_enum_values.insert(TouchType::MOVE);
  TouchType_all_enum_values.insert(TouchType::END);

  // Function parameter type.
  //
  // The type of touch event.
  utils::SharedPtr<ISchemaItem> type_SchemaItem = TEnumSchemaItem<TouchType::eType>::create(TouchType_all_enum_values, TSchemaItemParameter<TouchType::eType>());

  // Function parameter event.
  //
  // List of all individual touches involved in this event.
  utils::SharedPtr<ISchemaItem> event_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TouchEvent), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(10));

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnSystemRequestID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<RequestType::eType> RequestType_all_enum_values;
  RequestType_all_enum_values.insert(RequestType::HTTP);
  RequestType_all_enum_values.insert(RequestType::FILE_RESUME);
  RequestType_all_enum_values.insert(RequestType::AUTH_REQUEST);
  RequestType_all_enum_values.insert(RequestType::AUTH_CHALLENGE);
  RequestType_all_enum_values.insert(RequestType::AUTH_ACK);
  RequestType_all_enum_values.insert(RequestType::PROPRIETARY);

  std::set<FileType::eType> FileType_all_enum_values;
  FileType_all_enum_values.insert(FileType::GRAPHIC_BMP);
  FileType_all_enum_values.insert(FileType::GRAPHIC_JPEG);
  FileType_all_enum_values.insert(FileType::GRAPHIC_PNG);
  FileType_all_enum_values.insert(FileType::AUDIO_WAVE);
  FileType_all_enum_values.insert(FileType::AUDIO_MP3);
  FileType_all_enum_values.insert(FileType::AUDIO_AAC);
  FileType_all_enum_values.insert(FileType::BINARY);
  FileType_all_enum_values.insert(FileType::JSON);

  // Function parameter requestType.
  //
  // The type of system request.
  utils::SharedPtr<ISchemaItem> requestType_SchemaItem = TEnumSchemaItem<RequestType::eType>::create(RequestType_all_enum_values, TSchemaItemParameter<RequestType::eType>());

  // Function parameter url.
  //
  // Optional array of URL(s) for HTTP requests.
  // 				If blank, the binary data shall be forwarded to the app.
  // 	      If not blank, the binary data shall be forwarded to the url(s) with a provided timeout in seconds.
  // 			
  utils::SharedPtr<ISchemaItem> url_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter timeout.
  //
  // 
  //       	Optional timeout for HTTP requests
  //       	Required if a URL is provided
  //       
  utils::SharedPtr<ISchemaItem> timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter fileType.
  //
  // Optional file type (meant for HTTP file requests).
  utils::SharedPtr<ISchemaItem> fileType_SchemaItem = TEnumSchemaItem<FileType::eType>::create(FileType_all_enum_values, TSchemaItemParameter<FileType::eType>());

  // Function parameter offset.
  //
  // Optional offset in bytes for resuming partial data chunks
  utils::SharedPtr<ISchemaItem> offset_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(100000000000), TSchemaItemParameter<int>());

  // Function parameter length.
  //
  // Optional length in bytes for resuming partial data chunks
  utils::SharedPtr<ISchemaItem> length_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(100000000000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["requestType"] = CObjectSchemaItem::SMember(requestType_SchemaItem, true);
  schema_members["url"] = CObjectSchemaItem::SMember(url_SchemaItem, false);
  schema_members["timeout"] = CObjectSchemaItem::SMember(timeout_SchemaItem, false);
  schema_members["fileType"] = CObjectSchemaItem::SMember(fileType_SchemaItem, false);
  schema_members["offset"] = CObjectSchemaItem::SMember(offset_SchemaItem, false);
  schema_members["length"] = CObjectSchemaItem::SMember(length_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_EncodedSyncPDataID_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter data.
  //
  // Contains base64 encoded string of SyncP packets.
  //
  // ToDo: What is the maxlength?
  utils::SharedPtr<ISchemaItem> data_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000000), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["data"] = CObjectSchemaItem::SMember(data_SchemaItem, true);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_EncodedSyncPDataID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SyncPDataID_request(
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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_SyncPDataID_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  //  true, if successful; false, if failed 
  utils::SharedPtr<ISchemaItem> success_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter resultCode.
  //
  // See Result
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<Result::eType>::create(resultCode_allowed_enum_subset_values, TSchemaItemParameter<Result::eType>());

  // Function parameter info.
  //
  // Provides additional human readable info regarding the result.
  utils::SharedPtr<ISchemaItem> info_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnEncodedSyncPDataID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter data.
  //
  // Contains base64 encoded string of SyncP packets.
  utils::SharedPtr<ISchemaItem> data_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000000), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter URL.
  //
  // 
  //       	If blank, the SyncP data shall be forwarded to the app.
  //       	If not blank, the SyncP data shall be forwarded to the provided URL.
  //       
  utils::SharedPtr<ISchemaItem> URL_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter Timeout.
  //
  // 
  //       	If blank, the SyncP data shall be forwarded to the app.
  //       	If not blank, the SyncP data shall be forwarded with the provided timeout in seconds.
  //       
  utils::SharedPtr<ISchemaItem> Timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["data"] = CObjectSchemaItem::SMember(data_SchemaItem, true);
  schema_members["URL"] = CObjectSchemaItem::SMember(URL_SchemaItem, false);
  schema_members["Timeout"] = CObjectSchemaItem::SMember(Timeout_SchemaItem, false);

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

CSmartSchema mobile_apis::MOBILE_API::InitFunction_OnSyncPDataID_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter URL.
  //
  // 
  //       	If blank, the SyncP data shall be forwarded to the app.
  //       	If not blank, the SyncP data shall be forwarded to the provided URL.
  //       
  utils::SharedPtr<ISchemaItem> URL_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000), TSchemaItemParameter<std::string>());

  // Function parameter Timeout.
  //
  // 
  //       	If blank, the SyncP data shall be forwarded to the app.
  //       	If not blank, the SyncP data shall be forwarded with the provided timeout in seconds.
  //       
  utils::SharedPtr<ISchemaItem> Timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["URL"] = CObjectSchemaItem::SMember(URL_SchemaItem, false);
  schema_members["Timeout"] = CObjectSchemaItem::SMember(Timeout_SchemaItem, false);

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

//----------- Structs schema items initialization ------------

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_AudioPassThruCapabilities(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<SamplingRate::eType> SamplingRate_all_enum_values;
  SamplingRate_all_enum_values.insert(SamplingRate::SamplingRate_8KHZ);
  SamplingRate_all_enum_values.insert(SamplingRate::SamplingRate_16KHZ);
  SamplingRate_all_enum_values.insert(SamplingRate::SamplingRate_22KHZ);
  SamplingRate_all_enum_values.insert(SamplingRate::SamplingRate_44KHZ);

  std::set<BitsPerSample::eType> BitsPerSample_all_enum_values;
  BitsPerSample_all_enum_values.insert(BitsPerSample::BitsPerSample_8_BIT);
  BitsPerSample_all_enum_values.insert(BitsPerSample::BitsPerSample_16_BIT);

  std::set<AudioType::eType> AudioType_all_enum_values;
  AudioType_all_enum_values.insert(AudioType::PCM);

  // Struct member samplingRate.
  utils::SharedPtr<ISchemaItem> samplingRate_SchemaItem = TEnumSchemaItem<SamplingRate::eType>::create(SamplingRate_all_enum_values, TSchemaItemParameter<SamplingRate::eType>());

  // Struct member bitsPerSample.
  utils::SharedPtr<ISchemaItem> bitsPerSample_SchemaItem = TEnumSchemaItem<BitsPerSample::eType>::create(BitsPerSample_all_enum_values, TSchemaItemParameter<BitsPerSample::eType>());

  // Struct member audioType.
  utils::SharedPtr<ISchemaItem> audioType_SchemaItem = TEnumSchemaItem<AudioType::eType>::create(AudioType_all_enum_values, TSchemaItemParameter<AudioType::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["samplingRate"] = CObjectSchemaItem::SMember(samplingRate_SchemaItem, true);
  schema_members["bitsPerSample"] = CObjectSchemaItem::SMember(bitsPerSample_SchemaItem, true);
  schema_members["audioType"] = CObjectSchemaItem::SMember(audioType_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_Image(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<ImageType::eType> ImageType_all_enum_values;
  ImageType_all_enum_values.insert(ImageType::STATIC);
  ImageType_all_enum_values.insert(ImageType::DYNAMIC);

  // Struct member value.
  //
  // Either the static hex icon value or the binary image file name identifier (sent by PutFile).
  utils::SharedPtr<ISchemaItem> value_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(65535), TSchemaItemParameter<std::string>());

  // Struct member imageType.
  //
  // Describes, whether it is a static or dynamic image.
  utils::SharedPtr<ISchemaItem> imageType_SchemaItem = TEnumSchemaItem<ImageType::eType>::create(ImageType_all_enum_values, TSchemaItemParameter<ImageType::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["value"] = CObjectSchemaItem::SMember(value_SchemaItem, true);
  schema_members["imageType"] = CObjectSchemaItem::SMember(imageType_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_SoftButton(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<SoftButtonType::eType> SoftButtonType_all_enum_values;
  SoftButtonType_all_enum_values.insert(SoftButtonType::SBT_TEXT);
  SoftButtonType_all_enum_values.insert(SoftButtonType::SBT_IMAGE);
  SoftButtonType_all_enum_values.insert(SoftButtonType::SBT_BOTH);

  std::set<SystemAction::eType> SystemAction_all_enum_values;
  SystemAction_all_enum_values.insert(SystemAction::DEFAULT_ACTION);
  SystemAction_all_enum_values.insert(SystemAction::STEAL_FOCUS);
  SystemAction_all_enum_values.insert(SystemAction::KEEP_CONTEXT);

  // Struct member type.
  //
  // Describes, whether it is text, highlighted text, icon, or dynamic image. See softButtonType
  utils::SharedPtr<ISchemaItem> type_SchemaItem = TEnumSchemaItem<SoftButtonType::eType>::create(SoftButtonType_all_enum_values, TSchemaItemParameter<SoftButtonType::eType>());

  // Struct member text.
  //
  // Optional text to display (if defined as TEXT or BOTH)
  utils::SharedPtr<ISchemaItem> text_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member image.
  //
  // Optional image struct for SoftButton (if defined as IMAGE or BOTH)
  utils::SharedPtr<ISchemaItem> image_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Image);

  // Struct member isHighlighted.
  //
  // 
  //     		True, if highlighted
  //     		False, if not highlighted
  //     	
  utils::SharedPtr<ISchemaItem> isHighlighted_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>(false));

  // Struct member softButtonID.
  //
  // Value which is returned via OnButtonPress / OnButtonEvent
  utils::SharedPtr<ISchemaItem> softButtonID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Struct member systemAction.
  //
  // Parameter indicating whether selecting a SoftButton shall call a specific system action.  This is intended to allow Notifications to bring the callee into full / focus; or in the case of persistent overlays, the overlay can persist when a SoftButton is pressed.
  utils::SharedPtr<ISchemaItem> systemAction_SchemaItem = TEnumSchemaItem<SystemAction::eType>::create(SystemAction_all_enum_values, TSchemaItemParameter<SystemAction::eType>(SystemAction::DEFAULT_ACTION));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["type"] = CObjectSchemaItem::SMember(type_SchemaItem, true);
  schema_members["text"] = CObjectSchemaItem::SMember(text_SchemaItem, false);
  schema_members["image"] = CObjectSchemaItem::SMember(image_SchemaItem, false);
  schema_members["isHighlighted"] = CObjectSchemaItem::SMember(isHighlighted_SchemaItem, false);
  schema_members["softButtonID"] = CObjectSchemaItem::SMember(softButtonID_SchemaItem, true);
  schema_members["systemAction"] = CObjectSchemaItem::SMember(systemAction_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_Choice(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member choiceID.
  utils::SharedPtr<ISchemaItem> choiceID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Struct member menuName.
  utils::SharedPtr<ISchemaItem> menuName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member vrCommands.
  utils::SharedPtr<ISchemaItem> vrCommands_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(99), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Struct member image.
  utils::SharedPtr<ISchemaItem> image_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Image);

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
  utils::SharedPtr<ISchemaItem> secondaryImage_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Image);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["choiceID"] = CObjectSchemaItem::SMember(choiceID_SchemaItem, true);
  schema_members["menuName"] = CObjectSchemaItem::SMember(menuName_SchemaItem, true);
  schema_members["vrCommands"] = CObjectSchemaItem::SMember(vrCommands_SchemaItem, true);
  schema_members["image"] = CObjectSchemaItem::SMember(image_SchemaItem, false);
  schema_members["secondaryText"] = CObjectSchemaItem::SMember(secondaryText_SchemaItem, false);
  schema_members["tertiaryText"] = CObjectSchemaItem::SMember(tertiaryText_SchemaItem, false);
  schema_members["secondaryImage"] = CObjectSchemaItem::SMember(secondaryImage_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_VrHelpItem(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member text.
  //
  // Text to display for VR Help item
  utils::SharedPtr<ISchemaItem> text_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member image.
  //
  // Image struct for VR Help item
  utils::SharedPtr<ISchemaItem> image_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Image);

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

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_SyncMsgVersion(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member majorVersion.
  //
  // The major version indicates versions that is not-compatible to previous versions.
  utils::SharedPtr<ISchemaItem> majorVersion_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(10), TSchemaItemParameter<int>());

  // Struct member minorVersion.
  //
  // The minor version indicates a change to a previous version that should still allow to be run on an older version (with limited functionality)
  utils::SharedPtr<ISchemaItem> minorVersion_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(1000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["majorVersion"] = CObjectSchemaItem::SMember(majorVersion_SchemaItem, true);
  schema_members["minorVersion"] = CObjectSchemaItem::SMember(minorVersion_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_SingleTireStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<ComponentVolumeStatus::eType> ComponentVolumeStatus_all_enum_values;
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_UNKNOWN);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_NORMAL);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_LOW);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_FAULT);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_ALERT);
  ComponentVolumeStatus_all_enum_values.insert(ComponentVolumeStatus::CVS_NOT_SUPPORTED);

  // Struct member status.
  //
  // See ComponentVolumeStatus.
  utils::SharedPtr<ISchemaItem> status_SchemaItem = TEnumSchemaItem<ComponentVolumeStatus::eType>::create(ComponentVolumeStatus_all_enum_values, TSchemaItemParameter<ComponentVolumeStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["status"] = CObjectSchemaItem::SMember(status_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_BeltStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<VehicleDataEventStatus::eType> VehicleDataEventStatus_all_enum_values;
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NO_EVENT);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NO);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_YES);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NOT_SUPPORTED);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_FAULT);

  // Struct member driverBeltDeployed.
  //
  // References signal "VedsDrvBelt_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverBeltDeployed_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member passengerBeltDeployed.
  //
  // References signal "VedsPasBelt_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerBeltDeployed_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member passengerBuckleBelted.
  //
  // References signal "VedsRw1PasBckl_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerBuckleBelted_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member driverBuckleBelted.
  //
  // References signal "VedsRw1DrvBckl_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverBuckleBelted_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member leftRow2BuckleBelted.
  //
  // References signal "VedsRw2lBckl_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> leftRow2BuckleBelted_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member passengerChildDetected.
  //
  // References signal "VedsRw1PasChld_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerChildDetected_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member rightRow2BuckleBelted.
  //
  // References signal "VedsRw2rBckl_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> rightRow2BuckleBelted_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member middleRow2BuckleBelted.
  //
  // References signal "VedsRw2mBckl_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> middleRow2BuckleBelted_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member middleRow3BuckleBelted.
  //
  // References signal "VedsRw3mBckl_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> middleRow3BuckleBelted_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member leftRow3BuckleBelted.
  //
  // References signal "VedsRw3lBckl_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> leftRow3BuckleBelted_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member rightRow3BuckleBelted.
  //
  // References signal "VedsRw3rBckl_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> rightRow3BuckleBelted_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member leftRearInflatableBelted.
  //
  // References signal "VedsRw2lRib_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> leftRearInflatableBelted_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member rightRearInflatableBelted.
  //
  // References signal "VedsRw2rRib_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> rightRearInflatableBelted_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member middleRow1BeltDeployed.
  //
  // References signal "VedsRw1mBelt_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> middleRow1BeltDeployed_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member middleRow1BuckleBelted.
  //
  // References signal "VedsRw1mBckl_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> middleRow1BuckleBelted_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["driverBeltDeployed"] = CObjectSchemaItem::SMember(driverBeltDeployed_SchemaItem, true);
  schema_members["passengerBeltDeployed"] = CObjectSchemaItem::SMember(passengerBeltDeployed_SchemaItem, true);
  schema_members["passengerBuckleBelted"] = CObjectSchemaItem::SMember(passengerBuckleBelted_SchemaItem, true);
  schema_members["driverBuckleBelted"] = CObjectSchemaItem::SMember(driverBuckleBelted_SchemaItem, true);
  schema_members["leftRow2BuckleBelted"] = CObjectSchemaItem::SMember(leftRow2BuckleBelted_SchemaItem, true);
  schema_members["passengerChildDetected"] = CObjectSchemaItem::SMember(passengerChildDetected_SchemaItem, true);
  schema_members["rightRow2BuckleBelted"] = CObjectSchemaItem::SMember(rightRow2BuckleBelted_SchemaItem, true);
  schema_members["middleRow2BuckleBelted"] = CObjectSchemaItem::SMember(middleRow2BuckleBelted_SchemaItem, true);
  schema_members["middleRow3BuckleBelted"] = CObjectSchemaItem::SMember(middleRow3BuckleBelted_SchemaItem, true);
  schema_members["leftRow3BuckleBelted"] = CObjectSchemaItem::SMember(leftRow3BuckleBelted_SchemaItem, true);
  schema_members["rightRow3BuckleBelted"] = CObjectSchemaItem::SMember(rightRow3BuckleBelted_SchemaItem, true);
  schema_members["leftRearInflatableBelted"] = CObjectSchemaItem::SMember(leftRearInflatableBelted_SchemaItem, true);
  schema_members["rightRearInflatableBelted"] = CObjectSchemaItem::SMember(rightRearInflatableBelted_SchemaItem, true);
  schema_members["middleRow1BeltDeployed"] = CObjectSchemaItem::SMember(middleRow1BeltDeployed_SchemaItem, true);
  schema_members["middleRow1BuckleBelted"] = CObjectSchemaItem::SMember(middleRow1BuckleBelted_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_BodyInformation(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<IgnitionStableStatus::eType> IgnitionStableStatus_all_enum_values;
  IgnitionStableStatus_all_enum_values.insert(IgnitionStableStatus::IGNITION_SWITCH_NOT_STABLE);
  IgnitionStableStatus_all_enum_values.insert(IgnitionStableStatus::IGNITION_SWITCH_STABLE);
  IgnitionStableStatus_all_enum_values.insert(IgnitionStableStatus::MISSING_FROM_TRANSMITTER);

  std::set<IgnitionStatus::eType> IgnitionStatus_all_enum_values;
  IgnitionStatus_all_enum_values.insert(IgnitionStatus::IS_UNKNOWN);
  IgnitionStatus_all_enum_values.insert(IgnitionStatus::IS_OFF);
  IgnitionStatus_all_enum_values.insert(IgnitionStatus::IS_ACCESSORY);
  IgnitionStatus_all_enum_values.insert(IgnitionStatus::IS_RUN);
  IgnitionStatus_all_enum_values.insert(IgnitionStatus::IS_START);
  IgnitionStatus_all_enum_values.insert(IgnitionStatus::IS_INVALID);

  // Struct member parkBrakeActive.
  //
  // References signal "PrkBrkActv_B_Actl".
  utils::SharedPtr<ISchemaItem> parkBrakeActive_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member ignitionStableStatus.
  //
  // References signal "Ignition_Switch_Stable". See IgnitionStableStatus.
  utils::SharedPtr<ISchemaItem> ignitionStableStatus_SchemaItem = TEnumSchemaItem<IgnitionStableStatus::eType>::create(IgnitionStableStatus_all_enum_values, TSchemaItemParameter<IgnitionStableStatus::eType>());

  // Struct member ignitionStatus.
  //
  // References signal "Ignition_status". See IgnitionStatus.
  utils::SharedPtr<ISchemaItem> ignitionStatus_SchemaItem = TEnumSchemaItem<IgnitionStatus::eType>::create(IgnitionStatus_all_enum_values, TSchemaItemParameter<IgnitionStatus::eType>());

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

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_DeviceStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<DeviceLevelStatus::eType> DeviceLevelStatus_all_enum_values;
  DeviceLevelStatus_all_enum_values.insert(DeviceLevelStatus::ZERO_LEVEL_BARS);
  DeviceLevelStatus_all_enum_values.insert(DeviceLevelStatus::ONE_LEVEL_BARS);
  DeviceLevelStatus_all_enum_values.insert(DeviceLevelStatus::TWO_LEVEL_BARS);
  DeviceLevelStatus_all_enum_values.insert(DeviceLevelStatus::THREE_LEVEL_BARS);
  DeviceLevelStatus_all_enum_values.insert(DeviceLevelStatus::FOUR_LEVEL_BARS);
  DeviceLevelStatus_all_enum_values.insert(DeviceLevelStatus::NOT_PROVIDED);

  std::set<PrimaryAudioSource::eType> PrimaryAudioSource_all_enum_values;
  PrimaryAudioSource_all_enum_values.insert(PrimaryAudioSource::NO_SOURCE_SELECTED);
  PrimaryAudioSource_all_enum_values.insert(PrimaryAudioSource::USB);
  PrimaryAudioSource_all_enum_values.insert(PrimaryAudioSource::USB2);
  PrimaryAudioSource_all_enum_values.insert(PrimaryAudioSource::BLUETOOTH_STEREO_BTST);
  PrimaryAudioSource_all_enum_values.insert(PrimaryAudioSource::LINE_IN);
  PrimaryAudioSource_all_enum_values.insert(PrimaryAudioSource::IPOD);
  PrimaryAudioSource_all_enum_values.insert(PrimaryAudioSource::MOBILE_APP);

  // Struct member voiceRecOn.
  //
  // References signal "CPM_VoiceRec_STAT".
  utils::SharedPtr<ISchemaItem> voiceRecOn_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member btIconOn.
  //
  // References signal "BT_ICON".
  utils::SharedPtr<ISchemaItem> btIconOn_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member callActive.
  //
  // References signal "CPM_Call_Active_STAT".
  utils::SharedPtr<ISchemaItem> callActive_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member phoneRoaming.
  //
  // References signal "CPM_Phone_Roaming_STAT".
  utils::SharedPtr<ISchemaItem> phoneRoaming_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member textMsgAvailable.
  //
  // References signal "CPM_TextMsg_AVAL".
  utils::SharedPtr<ISchemaItem> textMsgAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member battLevelStatus.
  //
  // Device battery level status.  References signal "CPM_Batt_Level_STAT". See DeviceLevelStatus.
  utils::SharedPtr<ISchemaItem> battLevelStatus_SchemaItem = TEnumSchemaItem<DeviceLevelStatus::eType>::create(DeviceLevelStatus_all_enum_values, TSchemaItemParameter<DeviceLevelStatus::eType>());

  // Struct member stereoAudioOutputMuted.
  //
  // References signal "CPM_Stereo_Audio_Output".
  utils::SharedPtr<ISchemaItem> stereoAudioOutputMuted_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member monoAudioOutputMuted.
  //
  // References signal "CPM_Mono_Audio_Output".
  utils::SharedPtr<ISchemaItem> monoAudioOutputMuted_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member signalLevelStatus.
  //
  // Device signal level status.  References signal "CPM_Signal_Strength_STAT". See DeviceLevelStatus.
  utils::SharedPtr<ISchemaItem> signalLevelStatus_SchemaItem = TEnumSchemaItem<DeviceLevelStatus::eType>::create(DeviceLevelStatus_all_enum_values, TSchemaItemParameter<DeviceLevelStatus::eType>());

  // Struct member primaryAudioSource.
  //
  // References signal "CPM_Stereo_PAS_Source". See PrimaryAudioSource.
  utils::SharedPtr<ISchemaItem> primaryAudioSource_SchemaItem = TEnumSchemaItem<PrimaryAudioSource::eType>::create(PrimaryAudioSource_all_enum_values, TSchemaItemParameter<PrimaryAudioSource::eType>());

  // Struct member eCallEventActive.
  //
  // References signal "eCall_Event".
  utils::SharedPtr<ISchemaItem> eCallEventActive_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["voiceRecOn"] = CObjectSchemaItem::SMember(voiceRecOn_SchemaItem, true);
  schema_members["btIconOn"] = CObjectSchemaItem::SMember(btIconOn_SchemaItem, true);
  schema_members["callActive"] = CObjectSchemaItem::SMember(callActive_SchemaItem, true);
  schema_members["phoneRoaming"] = CObjectSchemaItem::SMember(phoneRoaming_SchemaItem, true);
  schema_members["textMsgAvailable"] = CObjectSchemaItem::SMember(textMsgAvailable_SchemaItem, true);
  schema_members["battLevelStatus"] = CObjectSchemaItem::SMember(battLevelStatus_SchemaItem, true);
  schema_members["stereoAudioOutputMuted"] = CObjectSchemaItem::SMember(stereoAudioOutputMuted_SchemaItem, true);
  schema_members["monoAudioOutputMuted"] = CObjectSchemaItem::SMember(monoAudioOutputMuted_SchemaItem, true);
  schema_members["signalLevelStatus"] = CObjectSchemaItem::SMember(signalLevelStatus_SchemaItem, true);
  schema_members["primaryAudioSource"] = CObjectSchemaItem::SMember(primaryAudioSource_SchemaItem, true);
  schema_members["eCallEventActive"] = CObjectSchemaItem::SMember(eCallEventActive_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_HeadLampStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<AmbientLightStatus::eType> AmbientLightStatus_all_enum_values;
  AmbientLightStatus_all_enum_values.insert(AmbientLightStatus::NIGHT);
  AmbientLightStatus_all_enum_values.insert(AmbientLightStatus::TWILIGHT_1);
  AmbientLightStatus_all_enum_values.insert(AmbientLightStatus::TWILIGHT_2);
  AmbientLightStatus_all_enum_values.insert(AmbientLightStatus::TWILIGHT_3);
  AmbientLightStatus_all_enum_values.insert(AmbientLightStatus::TWILIGHT_4);
  AmbientLightStatus_all_enum_values.insert(AmbientLightStatus::DAY);
  AmbientLightStatus_all_enum_values.insert(AmbientLightStatus::UNKNOWN);
  AmbientLightStatus_all_enum_values.insert(AmbientLightStatus::INVALID);

  // Struct member lowBeamsOn.
  //
  // Status of the low beam lamps.  References signal "HeadLampLoActv_B_Stat".
  utils::SharedPtr<ISchemaItem> lowBeamsOn_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member highBeamsOn.
  //
  // Status of the high beam lamps.  References signal "HeadLghtHiOn_B_Stat".
  utils::SharedPtr<ISchemaItem> highBeamsOn_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member ambientLightSensorStatus.
  //
  // Status of the ambient light sensor.
  utils::SharedPtr<ISchemaItem> ambientLightSensorStatus_SchemaItem = TEnumSchemaItem<AmbientLightStatus::eType>::create(AmbientLightStatus_all_enum_values, TSchemaItemParameter<AmbientLightStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["lowBeamsOn"] = CObjectSchemaItem::SMember(lowBeamsOn_SchemaItem, true);
  schema_members["highBeamsOn"] = CObjectSchemaItem::SMember(highBeamsOn_SchemaItem, true);
  schema_members["ambientLightSensorStatus"] = CObjectSchemaItem::SMember(ambientLightSensorStatus_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_engineInfo(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<MaintenanceModeStatus::eType> MaintenanceModeStatus_all_enum_values;
  MaintenanceModeStatus_all_enum_values.insert(MaintenanceModeStatus::MMS_NORMAL);
  MaintenanceModeStatus_all_enum_values.insert(MaintenanceModeStatus::MMS_NEAR);
  MaintenanceModeStatus_all_enum_values.insert(MaintenanceModeStatus::MMS_ACTIVE);
  MaintenanceModeStatus_all_enum_values.insert(MaintenanceModeStatus::MMS_FEATURE_NOT_PRESENT);

  // Struct member electricFuelConsumption.
  //
  // 
  //       	Indicates the electric fuel consumption in terms of gasoline equivalent volume to support fuel economy equivalent calculations.
  //       	Note:Plug-in vehicle use only!
  //       	References signal "ElFuelFlw_Vl_Dsply".
  //       	
  utils::SharedPtr<ISchemaItem> electricFuelConsumption_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(30000.0), TSchemaItemParameter<double>());

  // Struct member stateOfCharge.
  //
  // Percent state of charge for the high voltage battery.  References signal "BattTracSoc_Pc_Dsply".
  utils::SharedPtr<ISchemaItem> stateOfCharge_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(100.0), TSchemaItemParameter<double>());

  // Struct member fuelMaintenanceMode.
  //
  // Current status of fuel maintenance mode (if present on hybrid vehicles).  References signal "FuelMaintMde_D_Dsply".
  utils::SharedPtr<ISchemaItem> fuelMaintenanceMode_SchemaItem = TEnumSchemaItem<MaintenanceModeStatus::eType>::create(MaintenanceModeStatus_all_enum_values, TSchemaItemParameter<MaintenanceModeStatus::eType>());

  // Struct member distanceToEmpty.
  //
  // 
  //       	Electric range (DTE) in km.  References signal "VehElRnge_L_Dsply".
  //       	0xFFE = No Data Exists
  //       	0xFFF = Fault
  //       
  utils::SharedPtr<ISchemaItem> distanceToEmpty_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(1000.0), TSchemaItemParameter<double>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["electricFuelConsumption"] = CObjectSchemaItem::SMember(electricFuelConsumption_SchemaItem, true);
  schema_members["stateOfCharge"] = CObjectSchemaItem::SMember(stateOfCharge_SchemaItem, true);
  schema_members["fuelMaintenanceMode"] = CObjectSchemaItem::SMember(fuelMaintenanceMode_SchemaItem, true);
  schema_members["distanceToEmpty"] = CObjectSchemaItem::SMember(distanceToEmpty_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_DeviceInfo(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member hardware.
  //
  // Device model
  utils::SharedPtr<ISchemaItem> hardware_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member firmwareRev.
  //
  // Device firmware revision
  utils::SharedPtr<ISchemaItem> firmwareRev_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member os.
  //
  // Device OS
  utils::SharedPtr<ISchemaItem> os_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member osVersion.
  //
  // Device OS version
  utils::SharedPtr<ISchemaItem> osVersion_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member carrier.
  //
  // Device mobile carrier (if applicable)
  utils::SharedPtr<ISchemaItem> carrier_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member maxNumberRFCOMMPorts.
  //
  // Omitted if connected not via BT.
  utils::SharedPtr<ISchemaItem> maxNumberRFCOMMPorts_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(100), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["hardware"] = CObjectSchemaItem::SMember(hardware_SchemaItem, false);
  schema_members["firmwareRev"] = CObjectSchemaItem::SMember(firmwareRev_SchemaItem, false);
  schema_members["os"] = CObjectSchemaItem::SMember(os_SchemaItem, false);
  schema_members["osVersion"] = CObjectSchemaItem::SMember(osVersion_SchemaItem, false);
  schema_members["carrier"] = CObjectSchemaItem::SMember(carrier_SchemaItem, false);
  schema_members["maxNumberRFCOMMPorts"] = CObjectSchemaItem::SMember(maxNumberRFCOMMPorts_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_ECallInfo(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<VehicleDataNotificationStatus::eType> VehicleDataNotificationStatus_all_enum_values;
  VehicleDataNotificationStatus_all_enum_values.insert(VehicleDataNotificationStatus::VDNS_NOT_SUPPORTED);
  VehicleDataNotificationStatus_all_enum_values.insert(VehicleDataNotificationStatus::VDNS_NORMAL);
  VehicleDataNotificationStatus_all_enum_values.insert(VehicleDataNotificationStatus::VDNS_ACTIVE);
  VehicleDataNotificationStatus_all_enum_values.insert(VehicleDataNotificationStatus::VDNS_NOT_USED);

  std::set<ECallConfirmationStatus::eType> ECallConfirmationStatus_all_enum_values;
  ECallConfirmationStatus_all_enum_values.insert(ECallConfirmationStatus::ECCS_NORMAL);
  ECallConfirmationStatus_all_enum_values.insert(ECallConfirmationStatus::ECCS_CALL_IN_PROGRESS);
  ECallConfirmationStatus_all_enum_values.insert(ECallConfirmationStatus::ECCS_CALL_CANCELLED);
  ECallConfirmationStatus_all_enum_values.insert(ECallConfirmationStatus::CALL_COMPLETED);
  ECallConfirmationStatus_all_enum_values.insert(ECallConfirmationStatus::ECCS_CALL_UNSUCCESSFUL);
  ECallConfirmationStatus_all_enum_values.insert(ECallConfirmationStatus::ECCS_ECALL_CONFIGURED_OFF);
  ECallConfirmationStatus_all_enum_values.insert(ECallConfirmationStatus::ECCS_CALL_COMPLETE_DTMF_TIMEOUT);

  // Struct member eCallNotificationStatus.
  //
  // References signal "eCallNotification_4A". See VehicleDataNotificationStatus.
  utils::SharedPtr<ISchemaItem> eCallNotificationStatus_SchemaItem = TEnumSchemaItem<VehicleDataNotificationStatus::eType>::create(VehicleDataNotificationStatus_all_enum_values, TSchemaItemParameter<VehicleDataNotificationStatus::eType>());

  // Struct member auxECallNotificationStatus.
  //
  // References signal "eCallNotification". See VehicleDataNotificationStatus.
  utils::SharedPtr<ISchemaItem> auxECallNotificationStatus_SchemaItem = TEnumSchemaItem<VehicleDataNotificationStatus::eType>::create(VehicleDataNotificationStatus_all_enum_values, TSchemaItemParameter<VehicleDataNotificationStatus::eType>());

  // Struct member eCallConfirmationStatus.
  //
  // References signal "eCallConfirmation". See ECallConfirmationStatus.
  utils::SharedPtr<ISchemaItem> eCallConfirmationStatus_SchemaItem = TEnumSchemaItem<ECallConfirmationStatus::eType>::create(ECallConfirmationStatus_all_enum_values, TSchemaItemParameter<ECallConfirmationStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["eCallNotificationStatus"] = CObjectSchemaItem::SMember(eCallNotificationStatus_SchemaItem, true);
  schema_members["auxECallNotificationStatus"] = CObjectSchemaItem::SMember(auxECallNotificationStatus_SchemaItem, true);
  schema_members["eCallConfirmationStatus"] = CObjectSchemaItem::SMember(eCallConfirmationStatus_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_AirbagStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<VehicleDataEventStatus::eType> VehicleDataEventStatus_all_enum_values;
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NO_EVENT);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NO);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_YES);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NOT_SUPPORTED);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_FAULT);

  // Struct member driverAirbagDeployed.
  //
  // References signal "VedsDrvBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverAirbagDeployed_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member driverSideAirbagDeployed.
  //
  // References signal "VedsDrvSideBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverSideAirbagDeployed_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member driverCurtainAirbagDeployed.
  //
  // References signal "VedsDrvCrtnBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverCurtainAirbagDeployed_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member passengerAirbagDeployed.
  //
  // References signal "VedsPasBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerAirbagDeployed_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member passengerCurtainAirbagDeployed.
  //
  // References signal "VedsPasCrtnBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerCurtainAirbagDeployed_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member driverKneeAirbagDeployed.
  //
  // References signal "VedsKneeDrvBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> driverKneeAirbagDeployed_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member passengerSideAirbagDeployed.
  //
  // References signal "VedsPasSideBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerSideAirbagDeployed_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member passengerKneeAirbagDeployed.
  //
  // References signal "VedsKneePasBag_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> passengerKneeAirbagDeployed_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

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

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_EmergencyEvent(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<EmergencyEventType::eType> EmergencyEventType_all_enum_values;
  EmergencyEventType_all_enum_values.insert(EmergencyEventType::EET_NO_EVENT);
  EmergencyEventType_all_enum_values.insert(EmergencyEventType::EET_FRONTAL);
  EmergencyEventType_all_enum_values.insert(EmergencyEventType::EET_SIDE);
  EmergencyEventType_all_enum_values.insert(EmergencyEventType::EET_REAR);
  EmergencyEventType_all_enum_values.insert(EmergencyEventType::EET_ROLLOVER);
  EmergencyEventType_all_enum_values.insert(EmergencyEventType::EET_NOT_SUPPORTED);
  EmergencyEventType_all_enum_values.insert(EmergencyEventType::EET_FAULT);

  std::set<FuelCutoffStatus::eType> FuelCutoffStatus_all_enum_values;
  FuelCutoffStatus_all_enum_values.insert(FuelCutoffStatus::FCS_TERMINATE_FUEL);
  FuelCutoffStatus_all_enum_values.insert(FuelCutoffStatus::FCS_NORMAL_OPERATION);
  FuelCutoffStatus_all_enum_values.insert(FuelCutoffStatus::FCS_FAULT);

  std::set<VehicleDataEventStatus::eType> VehicleDataEventStatus_all_enum_values;
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NO_EVENT);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NO);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_YES);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_NOT_SUPPORTED);
  VehicleDataEventStatus_all_enum_values.insert(VehicleDataEventStatus::VDES_FAULT);

  // Struct member emergencyEventType.
  //
  // References signal "VedsEvntType_D_Ltchd". See EmergencyEventType.
  utils::SharedPtr<ISchemaItem> emergencyEventType_SchemaItem = TEnumSchemaItem<EmergencyEventType::eType>::create(EmergencyEventType_all_enum_values, TSchemaItemParameter<EmergencyEventType::eType>());

  // Struct member fuelCutoffStatus.
  //
  // References signal "RCM_FuelCutoff". See FuelCutoffStatus.
  utils::SharedPtr<ISchemaItem> fuelCutoffStatus_SchemaItem = TEnumSchemaItem<FuelCutoffStatus::eType>::create(FuelCutoffStatus_all_enum_values, TSchemaItemParameter<FuelCutoffStatus::eType>());

  // Struct member rolloverEvent.
  //
  // References signal "VedsEvntRoll_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> rolloverEvent_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  // Struct member maximumChangeVelocity.
  //
  // References signal "VedsMaxDeltaV_D_Ltchd". Change in velocity in KPH.  Additional reserved values:
  //         0x00 No event
  //         0xFE Not supported
  //         0xFF Fault
  //       
  utils::SharedPtr<ISchemaItem> maximumChangeVelocity_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(255), TSchemaItemParameter<int>());

  // Struct member multipleEvents.
  //
  // References signal "VedsMultiEvnt_D_Ltchd". See VehicleDataEventStatus.
  utils::SharedPtr<ISchemaItem> multipleEvents_SchemaItem = TEnumSchemaItem<VehicleDataEventStatus::eType>::create(VehicleDataEventStatus_all_enum_values, TSchemaItemParameter<VehicleDataEventStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["emergencyEventType"] = CObjectSchemaItem::SMember(emergencyEventType_SchemaItem, true);
  schema_members["fuelCutoffStatus"] = CObjectSchemaItem::SMember(fuelCutoffStatus_SchemaItem, true);
  schema_members["rolloverEvent"] = CObjectSchemaItem::SMember(rolloverEvent_SchemaItem, true);
  schema_members["maximumChangeVelocity"] = CObjectSchemaItem::SMember(maximumChangeVelocity_SchemaItem, true);
  schema_members["multipleEvents"] = CObjectSchemaItem::SMember(multipleEvents_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_ClusterModeStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<PowerModeQualificationStatus::eType> PowerModeQualificationStatus_all_enum_values;
  PowerModeQualificationStatus_all_enum_values.insert(PowerModeQualificationStatus::POWER_MODE_UNDEFINED);
  PowerModeQualificationStatus_all_enum_values.insert(PowerModeQualificationStatus::POWER_MODE_EVALUATION_IN_PROGRESS);
  PowerModeQualificationStatus_all_enum_values.insert(PowerModeQualificationStatus::NOT_DEFINED);
  PowerModeQualificationStatus_all_enum_values.insert(PowerModeQualificationStatus::POWER_MODE_OK);

  std::set<CarModeStatus::eType> CarModeStatus_all_enum_values;
  CarModeStatus_all_enum_values.insert(CarModeStatus::CMS_NORMAL);
  CarModeStatus_all_enum_values.insert(CarModeStatus::CMS_FACTORY);
  CarModeStatus_all_enum_values.insert(CarModeStatus::CMS_TRANSPORT);
  CarModeStatus_all_enum_values.insert(CarModeStatus::CMS_CRASH);

  std::set<PowerModeStatus::eType> PowerModeStatus_all_enum_values;
  PowerModeStatus_all_enum_values.insert(PowerModeStatus::KEY_OUT);
  PowerModeStatus_all_enum_values.insert(PowerModeStatus::KEY_RECENTLY_OUT);
  PowerModeStatus_all_enum_values.insert(PowerModeStatus::KEY_APPROVED_0);
  PowerModeStatus_all_enum_values.insert(PowerModeStatus::POST_ACCESORY_0);
  PowerModeStatus_all_enum_values.insert(PowerModeStatus::ACCESORY_1);
  PowerModeStatus_all_enum_values.insert(PowerModeStatus::POST_IGNITION_1);
  PowerModeStatus_all_enum_values.insert(PowerModeStatus::IGNITION_ON_2);
  PowerModeStatus_all_enum_values.insert(PowerModeStatus::RUNNING_2);
  PowerModeStatus_all_enum_values.insert(PowerModeStatus::CRANK_3);

  // Struct member powerModeActive.
  //
  // References signal "PowerMode_UB".
  utils::SharedPtr<ISchemaItem> powerModeActive_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member powerModeQualificationStatus.
  //
  // References signal "PowerModeQF". See PowerModeQualificationStatus.
  utils::SharedPtr<ISchemaItem> powerModeQualificationStatus_SchemaItem = TEnumSchemaItem<PowerModeQualificationStatus::eType>::create(PowerModeQualificationStatus_all_enum_values, TSchemaItemParameter<PowerModeQualificationStatus::eType>());

  // Struct member carModeStatus.
  //
  // References signal "CarMode". See CarMode.
  utils::SharedPtr<ISchemaItem> carModeStatus_SchemaItem = TEnumSchemaItem<CarModeStatus::eType>::create(CarModeStatus_all_enum_values, TSchemaItemParameter<CarModeStatus::eType>());

  // Struct member powerModeStatus.
  //
  // References signal "PowerMode". See PowerMode.
  utils::SharedPtr<ISchemaItem> powerModeStatus_SchemaItem = TEnumSchemaItem<PowerModeStatus::eType>::create(PowerModeStatus_all_enum_values, TSchemaItemParameter<PowerModeStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["powerModeActive"] = CObjectSchemaItem::SMember(powerModeActive_SchemaItem, true);
  schema_members["powerModeQualificationStatus"] = CObjectSchemaItem::SMember(powerModeQualificationStatus_SchemaItem, true);
  schema_members["carModeStatus"] = CObjectSchemaItem::SMember(carModeStatus_SchemaItem, true);
  schema_members["powerModeStatus"] = CObjectSchemaItem::SMember(powerModeStatus_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_MyKey(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<VehicleDataStatus::eType> VehicleDataStatus_all_enum_values;
  VehicleDataStatus_all_enum_values.insert(VehicleDataStatus::VDS_NO_DATA_EXISTS);
  VehicleDataStatus_all_enum_values.insert(VehicleDataStatus::VDS_OFF);
  VehicleDataStatus_all_enum_values.insert(VehicleDataStatus::VDS_ON);

  // Struct member e911Override.
  //
  // Indicates whether e911 override is on.  References signal "MyKey_e911Override_St". See VehicleDataStatus.
  utils::SharedPtr<ISchemaItem> e911Override_SchemaItem = TEnumSchemaItem<VehicleDataStatus::eType>::create(VehicleDataStatus_all_enum_values, TSchemaItemParameter<VehicleDataStatus::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["e911Override"] = CObjectSchemaItem::SMember(e911Override_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_TireStatus(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<WarningLightStatus::eType> WarningLightStatus_all_enum_values;
  WarningLightStatus_all_enum_values.insert(WarningLightStatus::WLS_OFF);
  WarningLightStatus_all_enum_values.insert(WarningLightStatus::WLS_ON);
  WarningLightStatus_all_enum_values.insert(WarningLightStatus::WLS_FLASH);
  WarningLightStatus_all_enum_values.insert(WarningLightStatus::WLS_NOT_USED);

  // Struct member pressureTelltale.
  //
  // Status of the Tire Pressure Telltale. See WarningLightStatus.
  utils::SharedPtr<ISchemaItem> pressureTelltale_SchemaItem = TEnumSchemaItem<WarningLightStatus::eType>::create(WarningLightStatus_all_enum_values, TSchemaItemParameter<WarningLightStatus::eType>());

  // Struct member leftFront.
  //
  // The status of the left front tire.
  utils::SharedPtr<ISchemaItem> leftFront_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SingleTireStatus);

  // Struct member rightFront.
  //
  // The status of the right front tire.
  utils::SharedPtr<ISchemaItem> rightFront_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SingleTireStatus);

  // Struct member leftRear.
  //
  // The status of the left rear tire.
  utils::SharedPtr<ISchemaItem> leftRear_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SingleTireStatus);

  // Struct member rightRear.
  //
  // The status of the right rear tire.
  utils::SharedPtr<ISchemaItem> rightRear_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SingleTireStatus);

  // Struct member innerLeftRear.
  //
  // The status of the inner left rear.
  utils::SharedPtr<ISchemaItem> innerLeftRear_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SingleTireStatus);

  // Struct member innerRightRear.
  //
  // The status of the inner right rear.
  utils::SharedPtr<ISchemaItem> innerRightRear_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SingleTireStatus);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["pressureTelltale"] = CObjectSchemaItem::SMember(pressureTelltale_SchemaItem, true);
  schema_members["leftFront"] = CObjectSchemaItem::SMember(leftFront_SchemaItem, true);
  schema_members["rightFront"] = CObjectSchemaItem::SMember(rightFront_SchemaItem, true);
  schema_members["leftRear"] = CObjectSchemaItem::SMember(leftRear_SchemaItem, true);
  schema_members["rightRear"] = CObjectSchemaItem::SMember(rightRear_SchemaItem, true);
  schema_members["innerLeftRear"] = CObjectSchemaItem::SMember(innerLeftRear_SchemaItem, true);
  schema_members["innerRightRear"] = CObjectSchemaItem::SMember(innerRightRear_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_GPSData(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<CompassDirection::eType> CompassDirection_all_enum_values;
  CompassDirection_all_enum_values.insert(CompassDirection::NORTH);
  CompassDirection_all_enum_values.insert(CompassDirection::NORTHWEST);
  CompassDirection_all_enum_values.insert(CompassDirection::WEST);
  CompassDirection_all_enum_values.insert(CompassDirection::SOUTHWEST);
  CompassDirection_all_enum_values.insert(CompassDirection::SOUTH);
  CompassDirection_all_enum_values.insert(CompassDirection::SOUTHEAST);
  CompassDirection_all_enum_values.insert(CompassDirection::EAST);
  CompassDirection_all_enum_values.insert(CompassDirection::NORTHEAST);

  std::set<Dimension::eType> Dimension_all_enum_values;
  Dimension_all_enum_values.insert(Dimension::Dimension_NO_FIX);
  Dimension_all_enum_values.insert(Dimension::Dimension_2D);
  Dimension_all_enum_values.insert(Dimension::Dimension_3D);

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
  utils::SharedPtr<ISchemaItem> compassDirection_SchemaItem = TEnumSchemaItem<CompassDirection::eType>::create(CompassDirection_all_enum_values, TSchemaItemParameter<CompassDirection::eType>());

  // Struct member pdop.
  //
  // PDOP.  If undefined or unavailable, then value shall be set to 0.
  utils::SharedPtr<ISchemaItem> pdop_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(10.0), TSchemaItemParameter<double>(0.0));

  // Struct member hdop.
  //
  // HDOP.  If value is unknown, value shall be set to 0.
  utils::SharedPtr<ISchemaItem> hdop_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(10.0), TSchemaItemParameter<double>(0.0));

  // Struct member vdop.
  //
  // VDOP.  If value is unknown, value shall be set to 0.
  utils::SharedPtr<ISchemaItem> vdop_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(10.0), TSchemaItemParameter<double>(0.0));

  // Struct member actual.
  //
  // 
  //     		True, if actual.
  //     		False, if infered.
  //     	
  utils::SharedPtr<ISchemaItem> actual_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member satellites.
  //
  // Number of satellites in view
  utils::SharedPtr<ISchemaItem> satellites_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(31), TSchemaItemParameter<int>());

  // Struct member dimension.
  //
  // See Dimension
  utils::SharedPtr<ISchemaItem> dimension_SchemaItem = TEnumSchemaItem<Dimension::eType>::create(Dimension_all_enum_values, TSchemaItemParameter<Dimension::eType>());

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

  schema_members["longitudeDegrees"] = CObjectSchemaItem::SMember(longitudeDegrees_SchemaItem, true);
  schema_members["latitudeDegrees"] = CObjectSchemaItem::SMember(latitudeDegrees_SchemaItem, true);
  schema_members["utcYear"] = CObjectSchemaItem::SMember(utcYear_SchemaItem, true);
  schema_members["utcMonth"] = CObjectSchemaItem::SMember(utcMonth_SchemaItem, true);
  schema_members["utcDay"] = CObjectSchemaItem::SMember(utcDay_SchemaItem, true);
  schema_members["utcHours"] = CObjectSchemaItem::SMember(utcHours_SchemaItem, true);
  schema_members["utcMinutes"] = CObjectSchemaItem::SMember(utcMinutes_SchemaItem, true);
  schema_members["utcSeconds"] = CObjectSchemaItem::SMember(utcSeconds_SchemaItem, true);
  schema_members["compassDirection"] = CObjectSchemaItem::SMember(compassDirection_SchemaItem, true);
  schema_members["pdop"] = CObjectSchemaItem::SMember(pdop_SchemaItem, true);
  schema_members["hdop"] = CObjectSchemaItem::SMember(hdop_SchemaItem, true);
  schema_members["vdop"] = CObjectSchemaItem::SMember(vdop_SchemaItem, true);
  schema_members["actual"] = CObjectSchemaItem::SMember(actual_SchemaItem, true);
  schema_members["satellites"] = CObjectSchemaItem::SMember(satellites_SchemaItem, true);
  schema_members["dimension"] = CObjectSchemaItem::SMember(dimension_SchemaItem, true);
  schema_members["altitude"] = CObjectSchemaItem::SMember(altitude_SchemaItem, true);
  schema_members["heading"] = CObjectSchemaItem::SMember(heading_SchemaItem, true);
  schema_members["speed"] = CObjectSchemaItem::SMember(speed_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_VehicleDataResult(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<VehicleDataType::eType> VehicleDataType_all_enum_values;
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_GPS);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_SPEED);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_RPM);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_FUELLEVEL);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_FUELLEVEL_STATE);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_FUELCONSUMPTION);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_EXTERNTEMP);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_VIN);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_PRNDL);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_TIREPRESSURE);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_ODOMETER);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_BELTSTATUS);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_BODYINFO);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_DEVICESTATUS);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_ECALLINFO);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_AIRBAGSTATUS);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_EMERGENCYEVENT);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_CLUSTERMODESTATUS);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_MYKEY);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_BRAKING);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_WIPERSTATUS);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_HEADLAMPSTATUS);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_BATTVOLTAGE);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_ENGINETORQUE);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_ACCPEDAL);
  VehicleDataType_all_enum_values.insert(VehicleDataType::VEHICLEDATA_STEERINGWHEEL);

  std::set<VehicleDataResultCode::eType> VehicleDataResultCode_all_enum_values;
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_SUCCESS);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_TRUNCATED_DATA);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_DISALLOWED);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_USER_DISALLOWED);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_INVALID_ID);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_DATA_NOT_AVAILABLE);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_IGNORED);

  // Struct member dataType.
  //
  // Defined published data element type.
  utils::SharedPtr<ISchemaItem> dataType_SchemaItem = TEnumSchemaItem<VehicleDataType::eType>::create(VehicleDataType_all_enum_values, TSchemaItemParameter<VehicleDataType::eType>());

  // Struct member resultCode.
  //
  // Published data result code.
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<VehicleDataResultCode::eType>::create(VehicleDataResultCode_all_enum_values, TSchemaItemParameter<VehicleDataResultCode::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["dataType"] = CObjectSchemaItem::SMember(dataType_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_DIDResult(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<VehicleDataResultCode::eType> VehicleDataResultCode_all_enum_values;
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_SUCCESS);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_TRUNCATED_DATA);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_DISALLOWED);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_USER_DISALLOWED);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_INVALID_ID);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_DATA_NOT_AVAILABLE);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED);
  VehicleDataResultCode_all_enum_values.insert(VehicleDataResultCode::VDRC_IGNORED);

  // Struct member resultCode.
  //
  // Individual DID result code.
  utils::SharedPtr<ISchemaItem> resultCode_SchemaItem = TEnumSchemaItem<VehicleDataResultCode::eType>::create(VehicleDataResultCode_all_enum_values, TSchemaItemParameter<VehicleDataResultCode::eType>());

  // Struct member didLocation.
  //
  // Location of raw data from vehicle data DID
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

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_StartTime(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member hours.
  //
  // 
  //     		The hour of the media clock.
  //     		Some radios only support a max of 19 hours. If out of range, it will be rejected.
  //     	
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

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_TextField(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<TextFieldName::eType> TextFieldName_all_enum_values;
  TextFieldName_all_enum_values.insert(TextFieldName::mainField1);
  TextFieldName_all_enum_values.insert(TextFieldName::mainField2);
  TextFieldName_all_enum_values.insert(TextFieldName::mainField3);
  TextFieldName_all_enum_values.insert(TextFieldName::mainField4);
  TextFieldName_all_enum_values.insert(TextFieldName::statusBar);
  TextFieldName_all_enum_values.insert(TextFieldName::mediaClock);
  TextFieldName_all_enum_values.insert(TextFieldName::mediaTrack);
  TextFieldName_all_enum_values.insert(TextFieldName::alertText1);
  TextFieldName_all_enum_values.insert(TextFieldName::alertText2);
  TextFieldName_all_enum_values.insert(TextFieldName::alertText3);
  TextFieldName_all_enum_values.insert(TextFieldName::scrollableMessageBody);
  TextFieldName_all_enum_values.insert(TextFieldName::initialInteractionText);
  TextFieldName_all_enum_values.insert(TextFieldName::navigationText1);
  TextFieldName_all_enum_values.insert(TextFieldName::navigationText2);
  TextFieldName_all_enum_values.insert(TextFieldName::ETA);
  TextFieldName_all_enum_values.insert(TextFieldName::totalDistance);
  TextFieldName_all_enum_values.insert(TextFieldName::audioPassThruDisplayText1);
  TextFieldName_all_enum_values.insert(TextFieldName::audioPassThruDisplayText2);
  TextFieldName_all_enum_values.insert(TextFieldName::sliderHeader);
  TextFieldName_all_enum_values.insert(TextFieldName::sliderFooter);
  TextFieldName_all_enum_values.insert(TextFieldName::notificationText);
  TextFieldName_all_enum_values.insert(TextFieldName::menuName);
  TextFieldName_all_enum_values.insert(TextFieldName::secondaryText);
  TextFieldName_all_enum_values.insert(TextFieldName::tertiaryText);
  TextFieldName_all_enum_values.insert(TextFieldName::timeToDestination);
  TextFieldName_all_enum_values.insert(TextFieldName::turnText);
  TextFieldName_all_enum_values.insert(TextFieldName::menuTitle);

  std::set<CharacterSet::eType> CharacterSet_all_enum_values;
  CharacterSet_all_enum_values.insert(CharacterSet::TYPE2SET);
  CharacterSet_all_enum_values.insert(CharacterSet::TYPE5SET);
  CharacterSet_all_enum_values.insert(CharacterSet::CID1SET);
  CharacterSet_all_enum_values.insert(CharacterSet::CID2SET);

  // Struct member name.
  //
  // The name that identifies the field. See TextFieldName.
  utils::SharedPtr<ISchemaItem> name_SchemaItem = TEnumSchemaItem<TextFieldName::eType>::create(TextFieldName_all_enum_values, TSchemaItemParameter<TextFieldName::eType>());

  // Struct member characterSet.
  //
  // The character set that is supported in this field. See CharacterSet.
  utils::SharedPtr<ISchemaItem> characterSet_SchemaItem = TEnumSchemaItem<CharacterSet::eType>::create(CharacterSet_all_enum_values, TSchemaItemParameter<CharacterSet::eType>());

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

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_ImageResolution(
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

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_ImageField(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<ImageFieldName::eType> ImageFieldName_all_enum_values;
  ImageFieldName_all_enum_values.insert(ImageFieldName::softButtonImage);
  ImageFieldName_all_enum_values.insert(ImageFieldName::choiceImage);
  ImageFieldName_all_enum_values.insert(ImageFieldName::choiceSecondaryImage);
  ImageFieldName_all_enum_values.insert(ImageFieldName::vrHelpItem);
  ImageFieldName_all_enum_values.insert(ImageFieldName::turnIcon);
  ImageFieldName_all_enum_values.insert(ImageFieldName::menuIcon);
  ImageFieldName_all_enum_values.insert(ImageFieldName::cmdIcon);
  ImageFieldName_all_enum_values.insert(ImageFieldName::appIcon);
  ImageFieldName_all_enum_values.insert(ImageFieldName::graphic);
  ImageFieldName_all_enum_values.insert(ImageFieldName::showConstantTBTIcon);
  ImageFieldName_all_enum_values.insert(ImageFieldName::showConstantTBTNextTurnIcon);

  std::set<FileType::eType> FileType_all_enum_values;
  FileType_all_enum_values.insert(FileType::GRAPHIC_BMP);
  FileType_all_enum_values.insert(FileType::GRAPHIC_JPEG);
  FileType_all_enum_values.insert(FileType::GRAPHIC_PNG);
  FileType_all_enum_values.insert(FileType::AUDIO_WAVE);
  FileType_all_enum_values.insert(FileType::AUDIO_MP3);
  FileType_all_enum_values.insert(FileType::AUDIO_AAC);
  FileType_all_enum_values.insert(FileType::BINARY);
  FileType_all_enum_values.insert(FileType::JSON);

  // Struct member name.
  //
  // The name that identifies the field. See ImageFieldName.
  utils::SharedPtr<ISchemaItem> name_SchemaItem = TEnumSchemaItem<ImageFieldName::eType>::create(ImageFieldName_all_enum_values, TSchemaItemParameter<ImageFieldName::eType>());

  // Struct member imageTypeSupported.
  //
  // The image types that are supported in this field. See FileType.
  utils::SharedPtr<ISchemaItem> imageTypeSupported_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<FileType::eType>::create(FileType_all_enum_values, TSchemaItemParameter<FileType::eType>()), TSchemaItemParameter<size_t>(), TSchemaItemParameter<size_t>());

  // Struct member imageResolution.
  //
  // The image resolution of this field.
  utils::SharedPtr<ISchemaItem> imageResolution_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ImageResolution);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["name"] = CObjectSchemaItem::SMember(name_SchemaItem, true);
  schema_members["imageTypeSupported"] = CObjectSchemaItem::SMember(imageTypeSupported_SchemaItem, true);
  schema_members["imageResolution"] = CObjectSchemaItem::SMember(imageResolution_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_TouchCoord(
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

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_TouchEvent(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member id.
  //
  // 
  //          A touch's unique identifier.  The application can track the current touch events by id.
  //          If a touch event has type begin, the id should be added to the set of touches.
  //          If a touch event has type end, the id should be removed from the set of touches.
  //        
  utils::SharedPtr<ISchemaItem> id_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(9), TSchemaItemParameter<int>());

  // Struct member ts.
  //
  // 
  //          The time that the touch was recorded.  This number can the time since the beginning of the session or something else as long as the units are in milliseconds.
  //          The timestamp is used to determined the rate of change of position of a touch.
  //          The application also uses the time to verify whether two touches, with different ids, are part of a single action by the user.
  //          If there is only a single timestamp in this array, it is the same for every coordinate in the coordinates array.
  //        
  utils::SharedPtr<ISchemaItem> ts_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2147483647), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000));

  // Struct member c.
  utils::SharedPtr<ISchemaItem> c_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TouchCoord), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(1000));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["id"] = CObjectSchemaItem::SMember(id_SchemaItem, true);
  schema_members["ts"] = CObjectSchemaItem::SMember(ts_SchemaItem, true);
  schema_members["c"] = CObjectSchemaItem::SMember(c_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_Coordinate(
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

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_TouchArea(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member rotationAngle.
  utils::SharedPtr<ISchemaItem> rotationAngle_SchemaItem = TNumberSchemaItem<double>::create(TSchemaItemParameter<double>(0.0), TSchemaItemParameter<double>(360.0), TSchemaItemParameter<double>());

  // Struct member radiusCoord.
  utils::SharedPtr<ISchemaItem> radiusCoord_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Coordinate);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["rotationAngle"] = CObjectSchemaItem::SMember(rotationAngle_SchemaItem, false);
  schema_members["radiusCoord"] = CObjectSchemaItem::SMember(radiusCoord_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_TouchEventCapabilities(
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

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_ScreenParams(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member resolution.
  //
  // The resolution of the prescribed screen area.
  utils::SharedPtr<ISchemaItem> resolution_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ImageResolution);

  // Struct member touchEventAvailable.
  //
  // Types of screen touch events available in screen area.
  utils::SharedPtr<ISchemaItem> touchEventAvailable_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TouchEventCapabilities);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["resolution"] = CObjectSchemaItem::SMember(resolution_SchemaItem, true);
  schema_members["touchEventAvailable"] = CObjectSchemaItem::SMember(touchEventAvailable_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_HMIPermissions(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<HMILevel::eType> HMILevel_all_enum_values;
  HMILevel_all_enum_values.insert(HMILevel::HMI_FULL);
  HMILevel_all_enum_values.insert(HMILevel::HMI_LIMITED);
  HMILevel_all_enum_values.insert(HMILevel::HMI_BACKGROUND);
  HMILevel_all_enum_values.insert(HMILevel::HMI_NONE);

  // Struct member allowed.
  //
  // A set of all HMI levels that are permitted for this given RPC.
  utils::SharedPtr<ISchemaItem> allowed_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<HMILevel::eType>::create(HMILevel_all_enum_values, TSchemaItemParameter<HMILevel::eType>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(100));

  // Struct member userDisallowed.
  //
  // A set of all HMI levels that are prohibited for this given RPC.
  utils::SharedPtr<ISchemaItem> userDisallowed_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<HMILevel::eType>::create(HMILevel_all_enum_values, TSchemaItemParameter<HMILevel::eType>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["allowed"] = CObjectSchemaItem::SMember(allowed_SchemaItem, true);
  schema_members["userDisallowed"] = CObjectSchemaItem::SMember(userDisallowed_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_ParameterPermissions(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member allowed.
  //
  // A set of all parameters that are permitted for this given RPC.
  utils::SharedPtr<ISchemaItem> allowed_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(100));

  // Struct member userDisallowed.
  //
  // A set of all parameters that are prohibited for this given RPC.
  utils::SharedPtr<ISchemaItem> userDisallowed_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["allowed"] = CObjectSchemaItem::SMember(allowed_SchemaItem, true);
  schema_members["userDisallowed"] = CObjectSchemaItem::SMember(userDisallowed_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_PermissionItem(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member rpcName.
  //
  // Name of the individual RPC in the policy table.
  utils::SharedPtr<ISchemaItem> rpcName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>());

  // Struct member hmiPermissions.
  utils::SharedPtr<ISchemaItem> hmiPermissions_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::HMIPermissions);

  // Struct member parameterPermissions.
  utils::SharedPtr<ISchemaItem> parameterPermissions_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ParameterPermissions);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["rpcName"] = CObjectSchemaItem::SMember(rpcName_SchemaItem, true);
  schema_members["hmiPermissions"] = CObjectSchemaItem::SMember(hmiPermissions_SchemaItem, true);
  schema_members["parameterPermissions"] = CObjectSchemaItem::SMember(parameterPermissions_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_DisplayCapabilities(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<DisplayType::eType> DisplayType_all_enum_values;
  DisplayType_all_enum_values.insert(DisplayType::CID);
  DisplayType_all_enum_values.insert(DisplayType::TYPE2);
  DisplayType_all_enum_values.insert(DisplayType::TYPE5);
  DisplayType_all_enum_values.insert(DisplayType::NGN);
  DisplayType_all_enum_values.insert(DisplayType::GEN2_8_DMA);
  DisplayType_all_enum_values.insert(DisplayType::GEN2_6_DMA);
  DisplayType_all_enum_values.insert(DisplayType::MFD3);
  DisplayType_all_enum_values.insert(DisplayType::MFD4);
  DisplayType_all_enum_values.insert(DisplayType::MFD5);
  DisplayType_all_enum_values.insert(DisplayType::GEN3_8_INCH);

  std::set<MediaClockFormat::eType> MediaClockFormat_all_enum_values;
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCK1);
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCK2);
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCK3);
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCKTEXT1);
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCKTEXT2);
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCKTEXT3);
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCKTEXT4);

  // Struct member displayType.
  //
  // The type of the display. See DisplayType
  utils::SharedPtr<ISchemaItem> displayType_SchemaItem = TEnumSchemaItem<DisplayType::eType>::create(DisplayType_all_enum_values, TSchemaItemParameter<DisplayType::eType>());

  // Struct member textFields.
  //
  // A set of all fields that support text data. See TextField
  utils::SharedPtr<ISchemaItem> textFields_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TextField), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Struct member imageFields.
  //
  // A set of all fields that support images. See ImageField
  utils::SharedPtr<ISchemaItem> imageFields_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ImageField), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Struct member mediaClockFormats.
  //
  // A set of all supported formats of the media clock. See MediaClockFormat
  utils::SharedPtr<ISchemaItem> mediaClockFormats_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<MediaClockFormat::eType>::create(MediaClockFormat_all_enum_values, TSchemaItemParameter<MediaClockFormat::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

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
  utils::SharedPtr<ISchemaItem> screenParams_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ScreenParams);

  // Struct member numCustomPresetsAvailable.
  //
  // The number of on-screen custom presets available (if any); otherwise omitted.
  utils::SharedPtr<ISchemaItem> numCustomPresetsAvailable_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(100), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["displayType"] = CObjectSchemaItem::SMember(displayType_SchemaItem, true);
  schema_members["textFields"] = CObjectSchemaItem::SMember(textFields_SchemaItem, true);
  schema_members["imageFields"] = CObjectSchemaItem::SMember(imageFields_SchemaItem, true);
  schema_members["mediaClockFormats"] = CObjectSchemaItem::SMember(mediaClockFormats_SchemaItem, true);
  schema_members["graphicSupported"] = CObjectSchemaItem::SMember(graphicSupported_SchemaItem, true);
  schema_members["templatesAvailable"] = CObjectSchemaItem::SMember(templatesAvailable_SchemaItem, true);
  schema_members["screenParams"] = CObjectSchemaItem::SMember(screenParams_SchemaItem, false);
  schema_members["numCustomPresetsAvailable"] = CObjectSchemaItem::SMember(numCustomPresetsAvailable_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_ButtonCapabilities(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<ButtonName::eType> ButtonName_all_enum_values;
  ButtonName_all_enum_values.insert(ButtonName::OK);
  ButtonName_all_enum_values.insert(ButtonName::SEEKLEFT);
  ButtonName_all_enum_values.insert(ButtonName::SEEKRIGHT);
  ButtonName_all_enum_values.insert(ButtonName::TUNEUP);
  ButtonName_all_enum_values.insert(ButtonName::TUNEDOWN);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_0);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_1);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_2);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_3);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_4);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_5);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_6);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_7);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_8);
  ButtonName_all_enum_values.insert(ButtonName::PRESET_9);
  ButtonName_all_enum_values.insert(ButtonName::CUSTOM_BUTTON);
  ButtonName_all_enum_values.insert(ButtonName::SEARCH);

  // Struct member name.
  //
  // The name of the button. See ButtonName.
  utils::SharedPtr<ISchemaItem> name_SchemaItem = TEnumSchemaItem<ButtonName::eType>::create(ButtonName_all_enum_values, TSchemaItemParameter<ButtonName::eType>());

  // Struct member shortPressAvailable.
  //
  // 
  //       	The button supports a short press.
  //       	Whenever the button is pressed short, onButtonPressed( SHORT) will be invoked.
  //       
  utils::SharedPtr<ISchemaItem> shortPressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member longPressAvailable.
  //
  // 
  //       	The button supports a LONG press.
  //       	Whenever the button is pressed long, onButtonPressed( LONG) will be invoked.
  //       
  utils::SharedPtr<ISchemaItem> longPressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member upDownAvailable.
  //
  // 
  //       	The button supports "button down" and "button up".
  //       	Whenever the button is pressed, onButtonEvent( DOWN) will be invoked.
  //       	Whenever the button is released, onButtonEvent( UP) will be invoked.
  //       
  utils::SharedPtr<ISchemaItem> upDownAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["name"] = CObjectSchemaItem::SMember(name_SchemaItem, true);
  schema_members["shortPressAvailable"] = CObjectSchemaItem::SMember(shortPressAvailable_SchemaItem, true);
  schema_members["longPressAvailable"] = CObjectSchemaItem::SMember(longPressAvailable_SchemaItem, true);
  schema_members["upDownAvailable"] = CObjectSchemaItem::SMember(upDownAvailable_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_SoftButtonCapabilities(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member shortPressAvailable.
  //
  // 
  //       	The button supports a short press.
  //       	Whenever the button is pressed short, onButtonPressed( SHORT) will be invoked.
  //       
  utils::SharedPtr<ISchemaItem> shortPressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member longPressAvailable.
  //
  // 
  //     	The button supports a LONG press.
  //       	Whenever the button is pressed long, onButtonPressed( LONG) will be invoked.
  //       
  utils::SharedPtr<ISchemaItem> longPressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member upDownAvailable.
  //
  // 
  //       	The button supports "button down" and "button up".
  //       	Whenever the button is pressed, onButtonEvent( DOWN) will be invoked.
  //       	Whenever the button is released, onButtonEvent( UP) will be invoked.
  //       
  utils::SharedPtr<ISchemaItem> upDownAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member imageSupported.
  //
  // The button supports referencing a static or dynamic image.
  utils::SharedPtr<ISchemaItem> imageSupported_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["shortPressAvailable"] = CObjectSchemaItem::SMember(shortPressAvailable_SchemaItem, true);
  schema_members["longPressAvailable"] = CObjectSchemaItem::SMember(longPressAvailable_SchemaItem, true);
  schema_members["upDownAvailable"] = CObjectSchemaItem::SMember(upDownAvailable_SchemaItem, true);
  schema_members["imageSupported"] = CObjectSchemaItem::SMember(imageSupported_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_PresetBankCapabilities(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member onScreenPresetsAvailable.
  //
  // Onscreen custom presets are available.
  utils::SharedPtr<ISchemaItem> onScreenPresetsAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["onScreenPresetsAvailable"] = CObjectSchemaItem::SMember(onScreenPresetsAvailable_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_MenuParams(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member parentID.
  //
  // 
  //       	unique ID of the sub menu, the command will be added to.
  //       	If not provided, it will be provided to the top level of the in application menu.
  //       
  utils::SharedPtr<ISchemaItem> parentID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>(0));

  // Struct member position.
  //
  // 
  //       	Position within the items that are are at top level of the in application menu.
  //       	0 will insert at the front.
  //     	1 will insert at the second position.
  //       	if position is greater or equal than the number of items on top level, the sub menu will be appended to the end.
  //       	If this param was omitted the entry will be added at the end.
  //       
  utils::SharedPtr<ISchemaItem> position_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(1000), TSchemaItemParameter<int>());

  // Struct member menuName.
  //
  // Text to show in the menu for this sub menu.
  utils::SharedPtr<ISchemaItem> menuName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["parentID"] = CObjectSchemaItem::SMember(parentID_SchemaItem, false);
  schema_members["position"] = CObjectSchemaItem::SMember(position_SchemaItem, false);
  schema_members["menuName"] = CObjectSchemaItem::SMember(menuName_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_TTSChunk(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<SpeechCapabilities::eType> SpeechCapabilities_all_enum_values;
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::SC_TEXT);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::SAPI_PHONEMES);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::LHPLUS_PHONEMES);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::PRE_RECORDED);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::SILENCE);

  // Struct member text.
  //
  // 
  //       	The text or phonemes to speak.
  //       	May not be empty.
  //       
  utils::SharedPtr<ISchemaItem> text_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member type.
  //
  // Describes, whether it is text or a specific phoneme set. See SpeechCapabilities
  utils::SharedPtr<ISchemaItem> type_SchemaItem = TEnumSchemaItem<SpeechCapabilities::eType>::create(SpeechCapabilities_all_enum_values, TSchemaItemParameter<SpeechCapabilities::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["text"] = CObjectSchemaItem::SMember(text_SchemaItem, true);
  schema_members["type"] = CObjectSchemaItem::SMember(type_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_Turn(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member navigationText.
  utils::SharedPtr<ISchemaItem> navigationText_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member turnIcon.
  utils::SharedPtr<ISchemaItem> turnIcon_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::Image);

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["navigationText"] = CObjectSchemaItem::SMember(navigationText_SchemaItem, false);
  schema_members["turnIcon"] = CObjectSchemaItem::SMember(turnIcon_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_VehicleType(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member make.
  //
  // Make of the vehicle, e.g. Ford
  utils::SharedPtr<ISchemaItem> make_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member model.
  //
  // Model of the vehicle, e.g. Fiesta
  utils::SharedPtr<ISchemaItem> model_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member modelYear.
  //
  // Model Year of the vehicle, e.g. 2013
  utils::SharedPtr<ISchemaItem> modelYear_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member trim.
  //
  // Trim of the vehicle, e.g. SE
  utils::SharedPtr<ISchemaItem> trim_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["make"] = CObjectSchemaItem::SMember(make_SchemaItem, false);
  schema_members["model"] = CObjectSchemaItem::SMember(model_SchemaItem, false);
  schema_members["modelYear"] = CObjectSchemaItem::SMember(modelYear_SchemaItem, false);
  schema_members["trim"] = CObjectSchemaItem::SMember(trim_SchemaItem, false);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> mobile_apis::MOBILE_API::InitStructSchemaItem_KeyboardProperties(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<Language::eType> Language_all_enum_values;
  Language_all_enum_values.insert(Language::EN_US);
  Language_all_enum_values.insert(Language::ES_MX);
  Language_all_enum_values.insert(Language::FR_CA);
  Language_all_enum_values.insert(Language::DE_DE);
  Language_all_enum_values.insert(Language::ES_ES);
  Language_all_enum_values.insert(Language::EN_GB);
  Language_all_enum_values.insert(Language::RU_RU);
  Language_all_enum_values.insert(Language::TR_TR);
  Language_all_enum_values.insert(Language::PL_PL);
  Language_all_enum_values.insert(Language::FR_FR);
  Language_all_enum_values.insert(Language::IT_IT);
  Language_all_enum_values.insert(Language::SV_SE);
  Language_all_enum_values.insert(Language::PT_PT);
  Language_all_enum_values.insert(Language::NL_NL);
  Language_all_enum_values.insert(Language::EN_AU);
  Language_all_enum_values.insert(Language::ZH_CN);
  Language_all_enum_values.insert(Language::ZH_TW);
  Language_all_enum_values.insert(Language::JA_JP);
  Language_all_enum_values.insert(Language::AR_SA);
  Language_all_enum_values.insert(Language::KO_KR);
  Language_all_enum_values.insert(Language::PT_BR);
  Language_all_enum_values.insert(Language::CS_CZ);
  Language_all_enum_values.insert(Language::DA_DK);
  Language_all_enum_values.insert(Language::NO_NO);

  std::set<KeyboardLayout::eType> KeyboardLayout_all_enum_values;
  KeyboardLayout_all_enum_values.insert(KeyboardLayout::QWERTY);
  KeyboardLayout_all_enum_values.insert(KeyboardLayout::QWERTZ);
  KeyboardLayout_all_enum_values.insert(KeyboardLayout::AZERTY);

  std::set<KeypressMode::eType> KeypressMode_all_enum_values;
  KeypressMode_all_enum_values.insert(KeypressMode::SINGLE_KEYPRESS);
  KeypressMode_all_enum_values.insert(KeypressMode::QUEUE_KEYPRESSES);
  KeypressMode_all_enum_values.insert(KeypressMode::RESEND_CURRENT_ENTRY);

  // Struct member language.
  //
  // The keyboard language.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Language::eType>::create(Language_all_enum_values, TSchemaItemParameter<Language::eType>());

  // Struct member keyboardLayout.
  //
  // Desired keyboard layout.
  utils::SharedPtr<ISchemaItem> keyboardLayout_SchemaItem = TEnumSchemaItem<KeyboardLayout::eType>::create(KeyboardLayout_all_enum_values, TSchemaItemParameter<KeyboardLayout::eType>());

  // Struct member keypressMode.
  //
  // 
  //       	Desired keypress mode.
  //       	If omitted, this value will be set to RESEND_CURRENT_ENTRY.
  //       
  utils::SharedPtr<ISchemaItem> keypressMode_SchemaItem = TEnumSchemaItem<KeypressMode::eType>::create(KeypressMode_all_enum_values, TSchemaItemParameter<KeypressMode::eType>());

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

//-------------- String to value enum mapping ----------------

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

template <>
const std::map<mobile_apis::Result::eType, std::string> &TEnumSchemaItem<mobile_apis::Result::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::Result::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::SUCCESS, "SUCCESS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::UNSUPPORTED_REQUEST, "UNSUPPORTED_REQUEST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::UNSUPPORTED_RESOURCE, "UNSUPPORTED_RESOURCE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::DISALLOWED, "DISALLOWED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::REJECTED, "REJECTED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::ABORTED, "ABORTED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::IGNORED, "IGNORED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::RETRY, "RETRY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::IN_USE, "IN_USE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::VEHICLE_DATA_NOT_AVAILABLE, "VEHICLE_DATA_NOT_AVAILABLE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::TIMED_OUT, "TIMED_OUT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::INVALID_DATA, "INVALID_DATA"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::CHAR_LIMIT_EXCEEDED, "CHAR_LIMIT_EXCEEDED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::INVALID_ID, "INVALID_ID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::DUPLICATE_NAME, "DUPLICATE_NAME"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::APPLICATION_NOT_REGISTERED, "APPLICATION_NOT_REGISTERED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::WRONG_LANGUAGE, "WRONG_LANGUAGE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::OUT_OF_MEMORY, "OUT_OF_MEMORY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::TOO_MANY_PENDING_REQUESTS, "TOO_MANY_PENDING_REQUESTS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::TOO_MANY_APPLICATIONS, "TOO_MANY_APPLICATIONS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::APPLICATION_REGISTERED_ALREADY, "APPLICATION_REGISTERED_ALREADY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::WARNINGS, "WARNINGS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::GENERIC_ERROR, "GENERIC_ERROR"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::USER_DISALLOWED, "USER_DISALLOWED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::UNSUPPORTED_VERSION, "UNSUPPORTED_VERSION"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::VEHICLE_DATA_NOT_ALLOWED, "VEHICLE_DATA_NOT_ALLOWED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::FILE_NOT_FOUND, "FILE_NOT_FOUND"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::CANCEL_ROUTE, "CANCEL_ROUTE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::TRUNCATED_DATA, "TRUNCATED_DATA"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::SAVED, "SAVED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::INVALID_CERT, "INVALID_CERT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::EXPIRED_CERT, "EXPIRED_CERT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Result::RESUME_FAILED, "RESUME_FAILED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::ButtonPressMode::eType, std::string> &TEnumSchemaItem<mobile_apis::ButtonPressMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::ButtonPressMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonPressMode::LONG, "LONG"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonPressMode::SHORT, "SHORT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::ButtonEventMode::eType, std::string> &TEnumSchemaItem<mobile_apis::ButtonEventMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::ButtonEventMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonEventMode::BUTTONUP, "BUTTONUP"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonEventMode::BUTTONDOWN, "BUTTONDOWN"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::Language::eType, std::string> &TEnumSchemaItem<mobile_apis::Language::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::Language::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::EN_US, "EN-US"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::ES_MX, "ES-MX"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::FR_CA, "FR-CA"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::DE_DE, "DE-DE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::ES_ES, "ES-ES"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::EN_GB, "EN-GB"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::RU_RU, "RU-RU"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::TR_TR, "TR-TR"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::PL_PL, "PL-PL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::FR_FR, "FR-FR"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::IT_IT, "IT-IT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::SV_SE, "SV-SE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::PT_PT, "PT-PT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::NL_NL, "NL-NL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::EN_AU, "EN-AU"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::ZH_CN, "ZH-CN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::ZH_TW, "ZH-TW"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::JA_JP, "JA-JP"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::AR_SA, "AR-SA"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::KO_KR, "KO-KR"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::PT_BR, "PT-BR"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::CS_CZ, "CS-CZ"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::DA_DK, "DA-DK"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Language::NO_NO, "NO-NO"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::UpdateMode::eType, std::string> &TEnumSchemaItem<mobile_apis::UpdateMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::UpdateMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::UpdateMode::COUNTUP, "COUNTUP"));
    enum_string_representation.insert(std::make_pair(mobile_apis::UpdateMode::COUNTDOWN, "COUNTDOWN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::UpdateMode::PAUSE, "PAUSE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::UpdateMode::RESUME, "RESUME"));
    enum_string_representation.insert(std::make_pair(mobile_apis::UpdateMode::CLEAR, "CLEAR"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::TimerMode::eType, std::string> &TEnumSchemaItem<mobile_apis::TimerMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::TimerMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::TimerMode::UP, "UP"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TimerMode::DOWN, "DOWN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TimerMode::NONE, "NONE"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::InteractionMode::eType, std::string> &TEnumSchemaItem<mobile_apis::InteractionMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::InteractionMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::InteractionMode::MANUAL_ONLY, "MANUAL_ONLY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::InteractionMode::VR_ONLY, "VR_ONLY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::InteractionMode::BOTH, "BOTH"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::LayoutMode::eType, std::string> &TEnumSchemaItem<mobile_apis::LayoutMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::LayoutMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::LayoutMode::ICON_ONLY, "ICON_ONLY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::LayoutMode::ICON_WITH_SEARCH, "ICON_WITH_SEARCH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::LayoutMode::LIST_ONLY, "LIST_ONLY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::LayoutMode::LIST_WITH_SEARCH, "LIST_WITH_SEARCH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::LayoutMode::KEYBOARD, "KEYBOARD"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::HMILevel::eType, std::string> &TEnumSchemaItem<mobile_apis::HMILevel::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::HMILevel::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::HMILevel::HMI_FULL, "FULL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::HMILevel::HMI_LIMITED, "LIMITED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::HMILevel::HMI_BACKGROUND, "BACKGROUND"));
    enum_string_representation.insert(std::make_pair(mobile_apis::HMILevel::HMI_NONE, "NONE"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::AudioStreamingState::eType, std::string> &TEnumSchemaItem<mobile_apis::AudioStreamingState::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::AudioStreamingState::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::AudioStreamingState::AUDIBLE, "AUDIBLE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AudioStreamingState::ATTENUATED, "ATTENUATED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AudioStreamingState::NOT_AUDIBLE, "NOT_AUDIBLE"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::SystemAction::eType, std::string> &TEnumSchemaItem<mobile_apis::SystemAction::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::SystemAction::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::SystemAction::DEFAULT_ACTION, "DEFAULT_ACTION"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SystemAction::STEAL_FOCUS, "STEAL_FOCUS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SystemAction::KEEP_CONTEXT, "KEEP_CONTEXT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::SystemContext::eType, std::string> &TEnumSchemaItem<mobile_apis::SystemContext::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::SystemContext::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::SystemContext::SYSCTXT_MAIN, "MAIN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SystemContext::SYSCTXT_VRSESSION, "VRSESSION"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SystemContext::SYSCTXT_MENU, "MENU"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SystemContext::SYSCTXT_HMI_OBSCURED, "HMI_OBSCURED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SystemContext::SYSCTXT_ALERT, "ALERT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::SoftButtonType::eType, std::string> &TEnumSchemaItem<mobile_apis::SoftButtonType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::SoftButtonType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::SoftButtonType::SBT_TEXT, "TEXT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SoftButtonType::SBT_IMAGE, "IMAGE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SoftButtonType::SBT_BOTH, "BOTH"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::AppInterfaceUnregisteredReason::eType, std::string> &TEnumSchemaItem<mobile_apis::AppInterfaceUnregisteredReason::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::AppInterfaceUnregisteredReason::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::AppInterfaceUnregisteredReason::USER_EXIT, "USER_EXIT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppInterfaceUnregisteredReason::IGNITION_OFF, "IGNITION_OFF"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppInterfaceUnregisteredReason::BLUETOOTH_OFF, "BLUETOOTH_OFF"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppInterfaceUnregisteredReason::USB_DISCONNECTED, "USB_DISCONNECTED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppInterfaceUnregisteredReason::REQUEST_WHILE_IN_NONE_HMI_LEVEL, "REQUEST_WHILE_IN_NONE_HMI_LEVEL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppInterfaceUnregisteredReason::TOO_MANY_REQUESTS, "TOO_MANY_REQUESTS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppInterfaceUnregisteredReason::DRIVER_DISTRACTION_VIOLATION, "DRIVER_DISTRACTION_VIOLATION"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE, "LANGUAGE_CHANGE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppInterfaceUnregisteredReason::MASTER_RESET, "MASTER_RESET"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppInterfaceUnregisteredReason::FACTORY_DEFAULTS, "FACTORY_DEFAULTS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED, "APP_UNAUTHORIZED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::TriggerSource::eType, std::string> &TEnumSchemaItem<mobile_apis::TriggerSource::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::TriggerSource::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::TriggerSource::TS_MENU, "MENU"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TriggerSource::TS_VR, "VR"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TriggerSource::TS_KEYBOARD, "KEYBOARD"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::HmiZoneCapabilities::eType, std::string> &TEnumSchemaItem<mobile_apis::HmiZoneCapabilities::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::HmiZoneCapabilities::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::HmiZoneCapabilities::FRONT, "FRONT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::HmiZoneCapabilities::BACK, "BACK"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::SpeechCapabilities::eType, std::string> &TEnumSchemaItem<mobile_apis::SpeechCapabilities::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::SpeechCapabilities::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::SpeechCapabilities::SC_TEXT, "TEXT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SpeechCapabilities::SAPI_PHONEMES, "SAPI_PHONEMES"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SpeechCapabilities::LHPLUS_PHONEMES, "LHPLUS_PHONEMES"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SpeechCapabilities::PRE_RECORDED, "PRE_RECORDED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SpeechCapabilities::SILENCE, "SILENCE"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::VrCapabilities::eType, std::string> &TEnumSchemaItem<mobile_apis::VrCapabilities::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::VrCapabilities::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::VrCapabilities::VR_TEXT, "TEXT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::PrerecordedSpeech::eType, std::string> &TEnumSchemaItem<mobile_apis::PrerecordedSpeech::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::PrerecordedSpeech::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::PrerecordedSpeech::HELP_JINGLE, "HELP_JINGLE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PrerecordedSpeech::INITIAL_JINGLE, "INITIAL_JINGLE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PrerecordedSpeech::LISTEN_JINGLE, "LISTEN_JINGLE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PrerecordedSpeech::POSITIVE_JINGLE, "POSITIVE_JINGLE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PrerecordedSpeech::NEGATIVE_JINGLE, "NEGATIVE_JINGLE"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::SamplingRate::eType, std::string> &TEnumSchemaItem<mobile_apis::SamplingRate::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::SamplingRate::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::SamplingRate::SamplingRate_8KHZ, "8KHZ"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SamplingRate::SamplingRate_16KHZ, "16KHZ"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SamplingRate::SamplingRate_22KHZ, "22KHZ"));
    enum_string_representation.insert(std::make_pair(mobile_apis::SamplingRate::SamplingRate_44KHZ, "44KHZ"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::BitsPerSample::eType, std::string> &TEnumSchemaItem<mobile_apis::BitsPerSample::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::BitsPerSample::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::BitsPerSample::BitsPerSample_8_BIT, "8_BIT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::BitsPerSample::BitsPerSample_16_BIT, "16_BIT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::AudioType::eType, std::string> &TEnumSchemaItem<mobile_apis::AudioType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::AudioType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::AudioType::PCM, "PCM"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::VehicleDataType::eType, std::string> &TEnumSchemaItem<mobile_apis::VehicleDataType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::VehicleDataType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_GPS, "VEHICLEDATA_GPS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_SPEED, "VEHICLEDATA_SPEED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_RPM, "VEHICLEDATA_RPM"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_FUELLEVEL, "VEHICLEDATA_FUELLEVEL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_FUELLEVEL_STATE, "VEHICLEDATA_FUELLEVEL_STATE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_FUELCONSUMPTION, "VEHICLEDATA_FUELCONSUMPTION"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_EXTERNTEMP, "VEHICLEDATA_EXTERNTEMP"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_VIN, "VEHICLEDATA_VIN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_PRNDL, "VEHICLEDATA_PRNDL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_TIREPRESSURE, "VEHICLEDATA_TIREPRESSURE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_ODOMETER, "VEHICLEDATA_ODOMETER"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_BELTSTATUS, "VEHICLEDATA_BELTSTATUS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_BODYINFO, "VEHICLEDATA_BODYINFO"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_DEVICESTATUS, "VEHICLEDATA_DEVICESTATUS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_ECALLINFO, "VEHICLEDATA_ECALLINFO"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_AIRBAGSTATUS, "VEHICLEDATA_AIRBAGSTATUS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_EMERGENCYEVENT, "VEHICLEDATA_EMERGENCYEVENT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_CLUSTERMODESTATUS, "VEHICLEDATA_CLUSTERMODESTATUS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_MYKEY, "VEHICLEDATA_MYKEY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_BRAKING, "VEHICLEDATA_BRAKING"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_WIPERSTATUS, "VEHICLEDATA_WIPERSTATUS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_HEADLAMPSTATUS, "VEHICLEDATA_HEADLAMPSTATUS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_BATTVOLTAGE, "VEHICLEDATA_BATTVOLTAGE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_ENGINETORQUE, "VEHICLEDATA_ENGINETORQUE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_ACCPEDAL, "VEHICLEDATA_ACCPEDAL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataType::VEHICLEDATA_STEERINGWHEEL, "VEHICLEDATA_STEERINGWHEEL"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::ButtonName::eType, std::string> &TEnumSchemaItem<mobile_apis::ButtonName::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::ButtonName::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::OK, "OK"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::SEEKLEFT, "SEEKLEFT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::SEEKRIGHT, "SEEKRIGHT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::TUNEUP, "TUNEUP"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::TUNEDOWN, "TUNEDOWN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::PRESET_0, "PRESET_0"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::PRESET_1, "PRESET_1"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::PRESET_2, "PRESET_2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::PRESET_3, "PRESET_3"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::PRESET_4, "PRESET_4"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::PRESET_5, "PRESET_5"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::PRESET_6, "PRESET_6"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::PRESET_7, "PRESET_7"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::PRESET_8, "PRESET_8"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::PRESET_9, "PRESET_9"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::CUSTOM_BUTTON, "CUSTOM_BUTTON"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ButtonName::SEARCH, "SEARCH"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::MediaClockFormat::eType, std::string> &TEnumSchemaItem<mobile_apis::MediaClockFormat::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::MediaClockFormat::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::MediaClockFormat::CLOCK1, "CLOCK1"));
    enum_string_representation.insert(std::make_pair(mobile_apis::MediaClockFormat::CLOCK2, "CLOCK2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::MediaClockFormat::CLOCK3, "CLOCK3"));
    enum_string_representation.insert(std::make_pair(mobile_apis::MediaClockFormat::CLOCKTEXT1, "CLOCKTEXT1"));
    enum_string_representation.insert(std::make_pair(mobile_apis::MediaClockFormat::CLOCKTEXT2, "CLOCKTEXT2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::MediaClockFormat::CLOCKTEXT3, "CLOCKTEXT3"));
    enum_string_representation.insert(std::make_pair(mobile_apis::MediaClockFormat::CLOCKTEXT4, "CLOCKTEXT4"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::DisplayType::eType, std::string> &TEnumSchemaItem<mobile_apis::DisplayType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::DisplayType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::DisplayType::CID, "CID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DisplayType::TYPE2, "TYPE2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DisplayType::TYPE5, "TYPE5"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DisplayType::NGN, "NGN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DisplayType::GEN2_8_DMA, "GEN2_8_DMA"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DisplayType::GEN2_6_DMA, "GEN2_6_DMA"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DisplayType::MFD3, "MFD3"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DisplayType::MFD4, "MFD4"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DisplayType::MFD5, "MFD5"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DisplayType::GEN3_8_INCH, "GEN3_8-INCH"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::TextFieldName::eType, std::string> &TEnumSchemaItem<mobile_apis::TextFieldName::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::TextFieldName::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::mainField1, "mainField1"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::mainField2, "mainField2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::mainField3, "mainField3"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::mainField4, "mainField4"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::statusBar, "statusBar"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::mediaClock, "mediaClock"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::mediaTrack, "mediaTrack"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::alertText1, "alertText1"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::alertText2, "alertText2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::alertText3, "alertText3"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::scrollableMessageBody, "scrollableMessageBody"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::initialInteractionText, "initialInteractionText"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::navigationText1, "navigationText1"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::navigationText2, "navigationText2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::ETA, "ETA"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::totalDistance, "totalDistance"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::audioPassThruDisplayText1, "audioPassThruDisplayText1"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::audioPassThruDisplayText2, "audioPassThruDisplayText2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::sliderHeader, "sliderHeader"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::sliderFooter, "sliderFooter"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::notificationText, "notificationText"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::menuName, "menuName"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::secondaryText, "secondaryText"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::tertiaryText, "tertiaryText"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::timeToDestination, "timeToDestination"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::turnText, "turnText"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextFieldName::menuTitle, "menuTitle"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::ImageFieldName::eType, std::string> &TEnumSchemaItem<mobile_apis::ImageFieldName::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::ImageFieldName::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageFieldName::softButtonImage, "softButtonImage"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageFieldName::choiceImage, "choiceImage"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageFieldName::choiceSecondaryImage, "choiceSecondaryImage"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageFieldName::vrHelpItem, "vrHelpItem"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageFieldName::turnIcon, "turnIcon"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageFieldName::menuIcon, "menuIcon"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageFieldName::cmdIcon, "cmdIcon"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageFieldName::appIcon, "appIcon"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageFieldName::graphic, "graphic"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageFieldName::showConstantTBTIcon, "showConstantTBTIcon"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageFieldName::showConstantTBTNextTurnIcon, "showConstantTBTNextTurnIcon"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::PredefinedLayout::eType, std::string> &TEnumSchemaItem<mobile_apis::PredefinedLayout::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::PredefinedLayout::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::DEFAULT, "DEFAULT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::MEDIA, "MEDIA"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::NON_MEDIA, "NON_MEDIA"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::ONSCREEN_PRESETS, "ONSCREEN_PRESETS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::NAV_FULLSCREEN_MAP, "NAV_FULLSCREEN_MAP"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::NAV_LIST, "NAV_LIST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::NAV_KEYBOARD, "NAV_KEYBOARD"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::GRAPHIC_WITH_TEXT, "GRAPHIC_WITH_TEXT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::TEXT_WITH_GRAPHIC, "TEXT_WITH_GRAPHIC"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::TILES_ONLY, "TILES_ONLY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::TEXTBUTTONS_ONLY, "TEXTBUTTONS_ONLY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::GRAPHIC_WITH_TILES, "GRAPHIC_WITH_TILES"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::TILES_WITH_GRAPHIC, "TILES_WITH_GRAPHIC"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::GRAPHIC_WITH_TEXT_AND_SOFTBUTTONS, "GRAPHIC_WITH_TEXT_AND_SOFTBUTTONS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::TEXT_AND_SOFTBUTTONS_WITH_GRAPHIC, "TEXT_AND_SOFTBUTTONS_WITH_GRAPHIC"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::GRAPHIC_WITH_TEXTBUTTONS, "GRAPHIC_WITH_TEXTBUTTONS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::TEXTBUTTONS_WITH_GRAPHIC, "TEXTBUTTONS_WITH_GRAPHIC"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::LARGE_GRAPHIC_WITH_SOFTBUTTONS, "LARGE_GRAPHIC_WITH_SOFTBUTTONS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::DOUBLE_GRAPHIC_WITH_SOFTBUTTONS, "DOUBLE_GRAPHIC_WITH_SOFTBUTTONS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PredefinedLayout::LARGE_GRAPHIC_ONLY, "LARGE_GRAPHIC_ONLY"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::CharacterSet::eType, std::string> &TEnumSchemaItem<mobile_apis::CharacterSet::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::CharacterSet::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::CharacterSet::TYPE2SET, "TYPE2SET"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CharacterSet::TYPE5SET, "TYPE5SET"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CharacterSet::CID1SET, "CID1SET"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CharacterSet::CID2SET, "CID2SET"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::TextAlignment::eType, std::string> &TEnumSchemaItem<mobile_apis::TextAlignment::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::TextAlignment::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::TextAlignment::LEFT_ALIGNED, "LEFT_ALIGNED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextAlignment::RIGHT_ALIGNED, "RIGHT_ALIGNED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TextAlignment::CENTERED, "CENTERED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::TBTState::eType, std::string> &TEnumSchemaItem<mobile_apis::TBTState::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::TBTState::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::TBTState::ROUTE_UPDATE_REQUEST, "ROUTE_UPDATE_REQUEST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TBTState::ROUTE_ACCEPTED, "ROUTE_ACCEPTED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TBTState::ROUTE_REFUSED, "ROUTE_REFUSED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TBTState::ROUTE_CANCELLED, "ROUTE_CANCELLED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TBTState::ETA_REQUEST, "ETA_REQUEST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TBTState::NEXT_TURN_REQUEST, "NEXT_TURN_REQUEST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TBTState::ROUTE_STATUS_REQUEST, "ROUTE_STATUS_REQUEST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TBTState::ROUTE_SUMMARY_REQUEST, "ROUTE_SUMMARY_REQUEST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TBTState::TRIP_STATUS_REQUEST, "TRIP_STATUS_REQUEST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TBTState::ROUTE_UPDATE_REQUEST_TIMEOUT, "ROUTE_UPDATE_REQUEST_TIMEOUT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::DriverDistractionState::eType, std::string> &TEnumSchemaItem<mobile_apis::DriverDistractionState::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::DriverDistractionState::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::DriverDistractionState::DD_ON, "DD_ON"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DriverDistractionState::DD_OFF, "DD_OFF"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::ImageType::eType, std::string> &TEnumSchemaItem<mobile_apis::ImageType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::ImageType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageType::STATIC, "STATIC"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ImageType::DYNAMIC, "DYNAMIC"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::GlobalProperty::eType, std::string> &TEnumSchemaItem<mobile_apis::GlobalProperty::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::GlobalProperty::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::GlobalProperty::HELPPROMPT, "HELPPROMPT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::GlobalProperty::TIMEOUTPROMPT, "TIMEOUTPROMPT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::GlobalProperty::VRHELPTITLE, "VRHELPTITLE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::GlobalProperty::VRHELPITEMS, "VRHELPITEMS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::GlobalProperty::MENUNAME, "MENUNAME"));
    enum_string_representation.insert(std::make_pair(mobile_apis::GlobalProperty::MENUICON, "MENUICON"));
    enum_string_representation.insert(std::make_pair(mobile_apis::GlobalProperty::KEYBOARDPROPERTIES, "KEYBOARDPROPERTIES"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::CompassDirection::eType, std::string> &TEnumSchemaItem<mobile_apis::CompassDirection::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::CompassDirection::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::CompassDirection::NORTH, "NORTH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CompassDirection::NORTHWEST, "NORTHWEST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CompassDirection::WEST, "WEST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CompassDirection::SOUTHWEST, "SOUTHWEST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CompassDirection::SOUTH, "SOUTH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CompassDirection::SOUTHEAST, "SOUTHEAST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CompassDirection::EAST, "EAST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CompassDirection::NORTHEAST, "NORTHEAST"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::Dimension::eType, std::string> &TEnumSchemaItem<mobile_apis::Dimension::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::Dimension::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::Dimension::Dimension_NO_FIX, "NO_FIX"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Dimension::Dimension_2D, "2D"));
    enum_string_representation.insert(std::make_pair(mobile_apis::Dimension::Dimension_3D, "3D"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::PRNDL::eType, std::string> &TEnumSchemaItem<mobile_apis::PRNDL::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::PRNDL::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::PARK, "PARK"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::REVERSE, "REVERSE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::NEUTRAL, "NEUTRAL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::DRIVE, "DRIVE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::SPORT, "SPORT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::LOWGEAR, "LOWGEAR"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::FIRST, "FIRST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::SECOND, "SECOND"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::THIRD, "THIRD"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::FOURTH, "FOURTH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::FIFTH, "FIFTH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::SIXTH, "SIXTH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::SEVENTH, "SEVENTH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::EIGHTH, "EIGHTH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::UNKNOWN, "UNKNOWN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PRNDL::FAULT, "FAULT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::ComponentVolumeStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::ComponentVolumeStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::ComponentVolumeStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::ComponentVolumeStatus::CVS_UNKNOWN, "UNKNOWN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ComponentVolumeStatus::CVS_NORMAL, "NORMAL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ComponentVolumeStatus::CVS_LOW, "LOW"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ComponentVolumeStatus::CVS_FAULT, "FAULT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ComponentVolumeStatus::CVS_ALERT, "ALERT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ComponentVolumeStatus::CVS_NOT_SUPPORTED, "NOT_SUPPORTED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::WarningLightStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::WarningLightStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::WarningLightStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::WarningLightStatus::WLS_OFF, "OFF"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WarningLightStatus::WLS_ON, "ON"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WarningLightStatus::WLS_FLASH, "FLASH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WarningLightStatus::WLS_NOT_USED, "NOT_USED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::VehicleDataNotificationStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::VehicleDataNotificationStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::VehicleDataNotificationStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataNotificationStatus::VDNS_NOT_SUPPORTED, "NOT_SUPPORTED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataNotificationStatus::VDNS_NORMAL, "NORMAL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataNotificationStatus::VDNS_ACTIVE, "ACTIVE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataNotificationStatus::VDNS_NOT_USED, "NOT_USED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::IgnitionStableStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::IgnitionStableStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::IgnitionStableStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::IgnitionStableStatus::IGNITION_SWITCH_NOT_STABLE, "IGNITION_SWITCH_NOT_STABLE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::IgnitionStableStatus::IGNITION_SWITCH_STABLE, "IGNITION_SWITCH_STABLE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::IgnitionStableStatus::MISSING_FROM_TRANSMITTER, "MISSING_FROM_TRANSMITTER"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::IgnitionStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::IgnitionStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::IgnitionStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::IgnitionStatus::IS_UNKNOWN, "UNKNOWN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::IgnitionStatus::IS_OFF, "OFF"));
    enum_string_representation.insert(std::make_pair(mobile_apis::IgnitionStatus::IS_ACCESSORY, "ACCESSORY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::IgnitionStatus::IS_RUN, "RUN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::IgnitionStatus::IS_START, "START"));
    enum_string_representation.insert(std::make_pair(mobile_apis::IgnitionStatus::IS_INVALID, "INVALID"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::VehicleDataEventStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::VehicleDataEventStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::VehicleDataEventStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataEventStatus::VDES_NO_EVENT, "NO_EVENT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataEventStatus::VDES_NO, "NO"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataEventStatus::VDES_YES, "YES"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataEventStatus::VDES_NOT_SUPPORTED, "NOT_SUPPORTED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataEventStatus::VDES_FAULT, "FAULT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::DeviceLevelStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::DeviceLevelStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::DeviceLevelStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::DeviceLevelStatus::ZERO_LEVEL_BARS, "ZERO_LEVEL_BARS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DeviceLevelStatus::ONE_LEVEL_BARS, "ONE_LEVEL_BARS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DeviceLevelStatus::TWO_LEVEL_BARS, "TWO_LEVEL_BARS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DeviceLevelStatus::THREE_LEVEL_BARS, "THREE_LEVEL_BARS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DeviceLevelStatus::FOUR_LEVEL_BARS, "FOUR_LEVEL_BARS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::DeviceLevelStatus::NOT_PROVIDED, "NOT_PROVIDED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::PrimaryAudioSource::eType, std::string> &TEnumSchemaItem<mobile_apis::PrimaryAudioSource::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::PrimaryAudioSource::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::PrimaryAudioSource::NO_SOURCE_SELECTED, "NO_SOURCE_SELECTED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PrimaryAudioSource::USB, "USB"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PrimaryAudioSource::USB2, "USB2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PrimaryAudioSource::BLUETOOTH_STEREO_BTST, "BLUETOOTH_STEREO_BTST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PrimaryAudioSource::LINE_IN, "LINE_IN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PrimaryAudioSource::IPOD, "IPOD"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PrimaryAudioSource::MOBILE_APP, "MOBILE_APP"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::WiperStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::WiperStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::WiperStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::OFF, "OFF"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::AUTO_OFF, "AUTO_OFF"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::OFF_MOVING, "OFF_MOVING"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::MAN_INT_OFF, "MAN_INT_OFF"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::MAN_INT_ON, "MAN_INT_ON"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::MAN_LOW, "MAN_LOW"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::MAN_HIGH, "MAN_HIGH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::MAN_FLICK, "MAN_FLICK"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::WASH, "WASH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::AUTO_LOW, "AUTO_LOW"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::AUTO_HIGH, "AUTO_HIGH"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::COURTESYWIPE, "COURTESYWIPE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::AUTO_ADJUST, "AUTO_ADJUST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::STALLED, "STALLED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::WiperStatus::NO_DATA_EXISTS, "NO_DATA_EXISTS"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::VehicleDataStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::VehicleDataStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::VehicleDataStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataStatus::VDS_NO_DATA_EXISTS, "NO_DATA_EXISTS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataStatus::VDS_OFF, "OFF"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataStatus::VDS_ON, "ON"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::MaintenanceModeStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::MaintenanceModeStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::MaintenanceModeStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::MaintenanceModeStatus::MMS_NORMAL, "NORMAL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::MaintenanceModeStatus::MMS_NEAR, "NEAR"));
    enum_string_representation.insert(std::make_pair(mobile_apis::MaintenanceModeStatus::MMS_ACTIVE, "ACTIVE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::MaintenanceModeStatus::MMS_FEATURE_NOT_PRESENT, "FEATURE_NOT_PRESENT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::VehicleDataActiveStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::VehicleDataActiveStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::VehicleDataActiveStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataActiveStatus::VDAS_INACTIVE_NOT_CONFIRMED, "INACTIVE_NOT_CONFIRMED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataActiveStatus::VDAS_INACTIVE_CONFIRMED, "INACTIVE_CONFIRMED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataActiveStatus::VDAS_ACTIVE_NOT_CONFIRMED, "ACTIVE_NOT_CONFIRMED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataActiveStatus::VDAS_ACTIVE_CONFIRMED, "ACTIVE_CONFIRMED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataActiveStatus::VDAS_FAULT, "FAULT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::AmbientLightStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::AmbientLightStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::AmbientLightStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::AmbientLightStatus::NIGHT, "NIGHT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AmbientLightStatus::TWILIGHT_1, "TWILIGHT_1"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AmbientLightStatus::TWILIGHT_2, "TWILIGHT_2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AmbientLightStatus::TWILIGHT_3, "TWILIGHT_3"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AmbientLightStatus::TWILIGHT_4, "TWILIGHT_4"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AmbientLightStatus::DAY, "DAY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AmbientLightStatus::UNKNOWN, "UNKNOWN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AmbientLightStatus::INVALID, "INVALID"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::FuelCutoffStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::FuelCutoffStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::FuelCutoffStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::FuelCutoffStatus::FCS_TERMINATE_FUEL, "TERMINATE_FUEL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FuelCutoffStatus::FCS_NORMAL_OPERATION, "NORMAL_OPERATION"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FuelCutoffStatus::FCS_FAULT, "FAULT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::EmergencyEventType::eType, std::string> &TEnumSchemaItem<mobile_apis::EmergencyEventType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::EmergencyEventType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::EmergencyEventType::EET_NO_EVENT, "NO_EVENT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::EmergencyEventType::EET_FRONTAL, "FRONTAL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::EmergencyEventType::EET_SIDE, "SIDE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::EmergencyEventType::EET_REAR, "REAR"));
    enum_string_representation.insert(std::make_pair(mobile_apis::EmergencyEventType::EET_ROLLOVER, "ROLLOVER"));
    enum_string_representation.insert(std::make_pair(mobile_apis::EmergencyEventType::EET_NOT_SUPPORTED, "NOT_SUPPORTED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::EmergencyEventType::EET_FAULT, "FAULT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::ECallConfirmationStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::ECallConfirmationStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::ECallConfirmationStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::ECallConfirmationStatus::ECCS_NORMAL, "NORMAL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ECallConfirmationStatus::ECCS_CALL_IN_PROGRESS, "CALL_IN_PROGRESS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ECallConfirmationStatus::ECCS_CALL_CANCELLED, "CALL_CANCELLED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ECallConfirmationStatus::CALL_COMPLETED, "CALL_COMPLETED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ECallConfirmationStatus::ECCS_CALL_UNSUCCESSFUL, "CALL_UNSUCCESSFUL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ECallConfirmationStatus::ECCS_ECALL_CONFIGURED_OFF, "ECALL_CONFIGURED_OFF"));
    enum_string_representation.insert(std::make_pair(mobile_apis::ECallConfirmationStatus::ECCS_CALL_COMPLETE_DTMF_TIMEOUT, "CALL_COMPLETE_DTMF_TIMEOUT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::FileType::eType, std::string> &TEnumSchemaItem<mobile_apis::FileType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::FileType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::FileType::GRAPHIC_BMP, "GRAPHIC_BMP"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FileType::GRAPHIC_JPEG, "GRAPHIC_JPEG"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FileType::GRAPHIC_PNG, "GRAPHIC_PNG"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FileType::AUDIO_WAVE, "AUDIO_WAVE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FileType::AUDIO_MP3, "AUDIO_MP3"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FileType::AUDIO_AAC, "AUDIO_AAC"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FileType::BINARY, "BINARY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FileType::JSON, "JSON"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::PowerModeQualificationStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::PowerModeQualificationStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::PowerModeQualificationStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeQualificationStatus::POWER_MODE_UNDEFINED, "POWER_MODE_UNDEFINED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeQualificationStatus::POWER_MODE_EVALUATION_IN_PROGRESS, "POWER_MODE_EVALUATION_IN_PROGRESS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeQualificationStatus::NOT_DEFINED, "NOT_DEFINED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeQualificationStatus::POWER_MODE_OK, "POWER_MODE_OK"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::PowerModeStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::PowerModeStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::PowerModeStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeStatus::KEY_OUT, "KEY_OUT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeStatus::KEY_RECENTLY_OUT, "KEY_RECENTLY_OUT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeStatus::KEY_APPROVED_0, "KEY_APPROVED_0"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeStatus::POST_ACCESORY_0, "POST_ACCESORY_0"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeStatus::ACCESORY_1, "ACCESORY_1"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeStatus::POST_IGNITION_1, "POST_IGNITION_1"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeStatus::IGNITION_ON_2, "IGNITION_ON_2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeStatus::RUNNING_2, "RUNNING_2"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PowerModeStatus::CRANK_3, "CRANK_3"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::CarModeStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::CarModeStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::CarModeStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::CarModeStatus::CMS_NORMAL, "NORMAL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CarModeStatus::CMS_FACTORY, "FACTORY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CarModeStatus::CMS_TRANSPORT, "TRANSPORT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::CarModeStatus::CMS_CRASH, "CRASH"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::VehicleDataResultCode::eType, std::string> &TEnumSchemaItem<mobile_apis::VehicleDataResultCode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::VehicleDataResultCode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataResultCode::VDRC_SUCCESS, "SUCCESS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataResultCode::VDRC_TRUNCATED_DATA, "TRUNCATED_DATA"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataResultCode::VDRC_DISALLOWED, "DISALLOWED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataResultCode::VDRC_USER_DISALLOWED, "USER_DISALLOWED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataResultCode::VDRC_INVALID_ID, "INVALID_ID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataResultCode::VDRC_DATA_NOT_AVAILABLE, "VEHICLE_DATA_NOT_AVAILABLE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED, "DATA_ALREADY_SUBSCRIBED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED, "DATA_NOT_SUBSCRIBED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::VehicleDataResultCode::VDRC_IGNORED, "IGNORED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::TouchType::eType, std::string> &TEnumSchemaItem<mobile_apis::TouchType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::TouchType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::TouchType::BEGIN, "BEGIN"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TouchType::MOVE, "MOVE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::TouchType::END, "END"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::PermissionStatus::eType, std::string> &TEnumSchemaItem<mobile_apis::PermissionStatus::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::PermissionStatus::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::PermissionStatus::PS_ALLOWED, "ALLOWED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PermissionStatus::PS_DISALLOWED, "DISALLOWED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PermissionStatus::PS_USER_DISALLOWED, "USER_DISALLOWED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::PermissionStatus::PS_USER_CONSENT_PENDING, "USER_CONSENT_PENDING"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::KeyboardLayout::eType, std::string> &TEnumSchemaItem<mobile_apis::KeyboardLayout::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::KeyboardLayout::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::KeyboardLayout::QWERTY, "QWERTY"));
    enum_string_representation.insert(std::make_pair(mobile_apis::KeyboardLayout::QWERTZ, "QWERTZ"));
    enum_string_representation.insert(std::make_pair(mobile_apis::KeyboardLayout::AZERTY, "AZERTY"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::KeyboardEvent::eType, std::string> &TEnumSchemaItem<mobile_apis::KeyboardEvent::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::KeyboardEvent::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::KeyboardEvent::KEYPRESS, "KEYPRESS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::KeyboardEvent::ENTRY_SUBMITTED, "ENTRY_SUBMITTED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::KeyboardEvent::ENTRY_CANCELLED, "ENTRY_CANCELLED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::KeyboardEvent::ENTRY_ABORTED, "ENTRY_ABORTED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::KeypressMode::eType, std::string> &TEnumSchemaItem<mobile_apis::KeypressMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::KeypressMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::KeypressMode::SINGLE_KEYPRESS, "SINGLE_KEYPRESS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::KeypressMode::QUEUE_KEYPRESSES, "QUEUE_KEYPRESSES"));
    enum_string_representation.insert(std::make_pair(mobile_apis::KeypressMode::RESEND_CURRENT_ENTRY, "RESEND_CURRENT_ENTRY"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::RequestType::eType, std::string> &TEnumSchemaItem<mobile_apis::RequestType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::RequestType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::RequestType::HTTP, "HTTP"));
    enum_string_representation.insert(std::make_pair(mobile_apis::RequestType::FILE_RESUME, "FILE_RESUME"));
    enum_string_representation.insert(std::make_pair(mobile_apis::RequestType::AUTH_REQUEST, "AUTH_REQUEST"));
    enum_string_representation.insert(std::make_pair(mobile_apis::RequestType::AUTH_CHALLENGE, "AUTH_CHALLENGE"));
    enum_string_representation.insert(std::make_pair(mobile_apis::RequestType::AUTH_ACK, "AUTH_ACK"));
    enum_string_representation.insert(std::make_pair(mobile_apis::RequestType::PROPRIETARY, "PROPRIETARY"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::AppHMIType::eType, std::string> &TEnumSchemaItem<mobile_apis::AppHMIType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::AppHMIType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::AppHMIType::DEFAULT, "DEFAULT"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppHMIType::COMMUNICATION, "COMMUNICATION"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppHMIType::MEDIA, "MEDIA"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppHMIType::MESSAGING, "MESSAGING"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppHMIType::NAVIGATION, "NAVIGATION"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppHMIType::INFORMATION, "INFORMATION"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppHMIType::SOCIAL, "SOCIAL"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppHMIType::BACKGROUND_PROCESS, "BACKGROUND_PROCESS"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppHMIType::TESTING, "TESTING"));
    enum_string_representation.insert(std::make_pair(mobile_apis::AppHMIType::SYSTEM, "SYSTEM"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::FunctionID::eType, std::string> &TEnumSchemaItem<mobile_apis::FunctionID::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::FunctionID::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::RESERVED, "RESERVED"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::RegisterAppInterfaceID, "RegisterAppInterfaceID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::UnregisterAppInterfaceID, "UnregisterAppInterfaceID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::SetGlobalPropertiesID, "SetGlobalPropertiesID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::ResetGlobalPropertiesID, "ResetGlobalPropertiesID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::AddCommandID, "AddCommandID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::DeleteCommandID, "DeleteCommandID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::AddSubMenuID, "AddSubMenuID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::DeleteSubMenuID, "DeleteSubMenuID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::CreateInteractionChoiceSetID, "CreateInteractionChoiceSetID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::PerformInteractionID, "PerformInteractionID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::DeleteInteractionChoiceSetID, "DeleteInteractionChoiceSetID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::AlertID, "AlertID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::ShowID, "ShowID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::SpeakID, "SpeakID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::SetMediaClockTimerID, "SetMediaClockTimerID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::PerformAudioPassThruID, "PerformAudioPassThruID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::EndAudioPassThruID, "EndAudioPassThruID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::SubscribeButtonID, "SubscribeButtonID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::UnsubscribeButtonID, "UnsubscribeButtonID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::SubscribeVehicleDataID, "SubscribeVehicleDataID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::UnsubscribeVehicleDataID, "UnsubscribeVehicleDataID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::GetVehicleDataID, "GetVehicleDataID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::ReadDIDID, "ReadDIDID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::GetDTCsID, "GetDTCsID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::ScrollableMessageID, "ScrollableMessageID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::SliderID, "SliderID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::ShowConstantTBTID, "ShowConstantTBTID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::AlertManeuverID, "AlertManeuverID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::UpdateTurnListID, "UpdateTurnListID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::ChangeRegistrationID, "ChangeRegistrationID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::GenericResponseID, "GenericResponseID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::PutFileID, "PutFileID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::DeleteFileID, "DeleteFileID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::ListFilesID, "ListFilesID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::SetAppIconID, "SetAppIconID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::SetDisplayLayoutID, "SetDisplayLayoutID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::DiagnosticMessageID, "DiagnosticMessageID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::SystemRequestID, "SystemRequestID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnHMIStatusID, "OnHMIStatusID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnAppInterfaceUnregisteredID, "OnAppInterfaceUnregisteredID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnButtonEventID, "OnButtonEventID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnButtonPressID, "OnButtonPressID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnVehicleDataID, "OnVehicleDataID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnCommandID, "OnCommandID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnTBTClientStateID, "OnTBTClientStateID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnDriverDistractionID, "OnDriverDistractionID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnPermissionsChangeID, "OnPermissionsChangeID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnAudioPassThruID, "OnAudioPassThruID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnLanguageChangeID, "OnLanguageChangeID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnKeyboardInputID, "OnKeyboardInputID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnTouchEventID, "OnTouchEventID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnSystemRequestID, "OnSystemRequestID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnHashChangeID, "OnHashChangeID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::EncodedSyncPDataID, "EncodedSyncPDataID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::SyncPDataID, "SyncPDataID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnEncodedSyncPDataID, "OnEncodedSyncPDataID"));
    enum_string_representation.insert(std::make_pair(mobile_apis::FunctionID::OnSyncPDataID, "OnSyncPDataID"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<mobile_apis::messageType::eType, std::string> &TEnumSchemaItem<mobile_apis::messageType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<mobile_apis::messageType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(mobile_apis::messageType::request, "request"));
    enum_string_representation.insert(std::make_pair(mobile_apis::messageType::response, "response"));
    enum_string_representation.insert(std::make_pair(mobile_apis::messageType::notification, "notification"));

    is_initialized = true;
  }

  return enum_string_representation;
}

} // NsSmartObjects
} // NsSmartDeviceLink

