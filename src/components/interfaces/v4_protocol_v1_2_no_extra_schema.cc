/**
 * @file v4_protocol_v1_2_no_extra.cc
 * @brief Generated class v4_protocol_v1_2_no_extra source file.
 *
 * This class is a part of SmartObjects solution. It provides
 * factory functionallity which allows client to use SmartSchemas
 * in accordance with definitions from v4_protocol_v1_2_no_extra.xml file
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

#include "v4_protocol_v1_2_no_extra_schema.h"
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

NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::v4_protocol_v1_2_no_extra()
 : NsSmartDeviceLink::NsJSONHandler::CSmartFactory<FunctionID::eType, messageType::eType, StructIdentifiers::eType>() {
  TStructsSchemaItems struct_schema_items;
  InitStructSchemes(struct_schema_items);

  std::set<FunctionID::eType> function_id_items;
  function_id_items.insert(FunctionID::RegisterAppInterface);
  function_id_items.insert(FunctionID::UnregisterAppInterface);
  function_id_items.insert(FunctionID::SetGlobalProperties);
  function_id_items.insert(FunctionID::ResetGlobalProperties);
  function_id_items.insert(FunctionID::AddCommand);
  function_id_items.insert(FunctionID::DeleteCommand);
  function_id_items.insert(FunctionID::AddSubMenu);
  function_id_items.insert(FunctionID::DeleteSubMenu);
  function_id_items.insert(FunctionID::CreateInteractionChoiceSet);
  function_id_items.insert(FunctionID::PerformInteraction);
  function_id_items.insert(FunctionID::DeleteInteractionChoiceSet);
  function_id_items.insert(FunctionID::Alert);
  function_id_items.insert(FunctionID::Show);
  function_id_items.insert(FunctionID::Speak);
  function_id_items.insert(FunctionID::SetMediaClockTimer);
  function_id_items.insert(FunctionID::EncodedSyncPData);
  function_id_items.insert(FunctionID::SubscribeButton);
  function_id_items.insert(FunctionID::UnsubscribeButton);
  function_id_items.insert(FunctionID::GenericResponse);
  function_id_items.insert(FunctionID::OnHMIStatus);
  function_id_items.insert(FunctionID::OnAppInterfaceUnregistered);
  function_id_items.insert(FunctionID::OnButtonEvent);
  function_id_items.insert(FunctionID::OnButtonPress);
  function_id_items.insert(FunctionID::OnCommand);
  function_id_items.insert(FunctionID::OnEncodedSyncPData);
  function_id_items.insert(FunctionID::OnTBTClientState);
  function_id_items.insert(FunctionID::OnDriverDistraction);

  std::set<messageType::eType> message_type_items;
  message_type_items.insert(messageType::request);
  message_type_items.insert(messageType::response);
  message_type_items.insert(messageType::notification);

  InitFunctionSchemes(struct_schema_items, function_id_items, message_type_items);
}

utils::SharedPtr<ISchemaItem> NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::ProvideObjectSchemaItemForStruct(
    const TStructsSchemaItems &struct_schema_items,
    const StructIdentifiers::eType struct_id) {
  const TStructsSchemaItems::const_iterator it = struct_schema_items.find(struct_id);
  if (it != struct_schema_items.end()) {
    return it->second;
  }

  return NsSmartDeviceLink::NsSmartObjects::CAlwaysFalseSchemaItem::create();
}

void NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitStructSchemes(
    TStructsSchemaItems &struct_schema_items) {
  utils::SharedPtr<ISchemaItem> struct_schema_item_Choice = InitStructSchemaItem_Choice(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::Choice, struct_schema_item_Choice));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::Choice, CSmartSchema(struct_schema_item_Choice)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_SyncMsgVersion = InitStructSchemaItem_SyncMsgVersion(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::SyncMsgVersion, struct_schema_item_SyncMsgVersion));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::SyncMsgVersion, CSmartSchema(struct_schema_item_SyncMsgVersion)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_StartTime = InitStructSchemaItem_StartTime(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::StartTime, struct_schema_item_StartTime));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::StartTime, CSmartSchema(struct_schema_item_StartTime)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_TextField = InitStructSchemaItem_TextField(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::TextField, struct_schema_item_TextField));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::TextField, CSmartSchema(struct_schema_item_TextField)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_DisplayCapabilities = InitStructSchemaItem_DisplayCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::DisplayCapabilities, struct_schema_item_DisplayCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::DisplayCapabilities, CSmartSchema(struct_schema_item_DisplayCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_ButtonCapabilities = InitStructSchemaItem_ButtonCapabilities(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::ButtonCapabilities, struct_schema_item_ButtonCapabilities));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::ButtonCapabilities, CSmartSchema(struct_schema_item_ButtonCapabilities)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_MenuParams = InitStructSchemaItem_MenuParams(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::MenuParams, struct_schema_item_MenuParams));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::MenuParams, CSmartSchema(struct_schema_item_MenuParams)));

  utils::SharedPtr<ISchemaItem> struct_schema_item_TTSChunk = InitStructSchemaItem_TTSChunk(struct_schema_items);
  struct_schema_items.insert(std::make_pair(StructIdentifiers::TTSChunk, struct_schema_item_TTSChunk));
  structs_schemes_.insert(std::make_pair(StructIdentifiers::TTSChunk, CSmartSchema(struct_schema_item_TTSChunk)));
}

void NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunctionSchemes(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::RegisterAppInterface, messageType::request), InitFunction_RegisterAppInterface_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::RegisterAppInterface, messageType::response), InitFunction_RegisterAppInterface_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UnregisterAppInterface, messageType::request), InitFunction_UnregisterAppInterface_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UnregisterAppInterface, messageType::response), InitFunction_UnregisterAppInterface_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetGlobalProperties, messageType::request), InitFunction_SetGlobalProperties_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetGlobalProperties, messageType::response), InitFunction_SetGlobalProperties_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ResetGlobalProperties, messageType::request), InitFunction_ResetGlobalProperties_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::ResetGlobalProperties, messageType::response), InitFunction_ResetGlobalProperties_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AddCommand, messageType::request), InitFunction_AddCommand_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AddCommand, messageType::response), InitFunction_AddCommand_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteCommand, messageType::request), InitFunction_DeleteCommand_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteCommand, messageType::response), InitFunction_DeleteCommand_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AddSubMenu, messageType::request), InitFunction_AddSubMenu_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::AddSubMenu, messageType::response), InitFunction_AddSubMenu_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteSubMenu, messageType::request), InitFunction_DeleteSubMenu_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteSubMenu, messageType::response), InitFunction_DeleteSubMenu_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::CreateInteractionChoiceSet, messageType::request), InitFunction_CreateInteractionChoiceSet_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::CreateInteractionChoiceSet, messageType::response), InitFunction_CreateInteractionChoiceSet_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::PerformInteraction, messageType::request), InitFunction_PerformInteraction_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::PerformInteraction, messageType::response), InitFunction_PerformInteraction_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteInteractionChoiceSet, messageType::request), InitFunction_DeleteInteractionChoiceSet_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::DeleteInteractionChoiceSet, messageType::response), InitFunction_DeleteInteractionChoiceSet_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Alert, messageType::request), InitFunction_Alert_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Alert, messageType::response), InitFunction_Alert_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Show, messageType::request), InitFunction_Show_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Show, messageType::response), InitFunction_Show_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Speak, messageType::request), InitFunction_Speak_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::Speak, messageType::response), InitFunction_Speak_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetMediaClockTimer, messageType::request), InitFunction_SetMediaClockTimer_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SetMediaClockTimer, messageType::response), InitFunction_SetMediaClockTimer_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::EncodedSyncPData, messageType::request), InitFunction_EncodedSyncPData_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::EncodedSyncPData, messageType::response), InitFunction_EncodedSyncPData_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SubscribeButton, messageType::request), InitFunction_SubscribeButton_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::SubscribeButton, messageType::response), InitFunction_SubscribeButton_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UnsubscribeButton, messageType::request), InitFunction_UnsubscribeButton_request(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::UnsubscribeButton, messageType::response), InitFunction_UnsubscribeButton_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::GenericResponse, messageType::response), InitFunction_GenericResponse_response(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnHMIStatus, messageType::notification), InitFunction_OnHMIStatus_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnAppInterfaceUnregistered, messageType::notification), InitFunction_OnAppInterfaceUnregistered_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnButtonEvent, messageType::notification), InitFunction_OnButtonEvent_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnButtonPress, messageType::notification), InitFunction_OnButtonPress_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnCommand, messageType::notification), InitFunction_OnCommand_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnEncodedSyncPData, messageType::notification), InitFunction_OnEncodedSyncPData_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnTBTClientState, messageType::notification), InitFunction_OnTBTClientState_notification(struct_schema_items, function_id_items, message_type_items)));
  functions_schemes_.insert(std::make_pair(NsSmartDeviceLink::NsJSONHandler::SmartSchemaKey<FunctionID::eType, messageType::eType>(FunctionID::OnDriverDistraction, messageType::notification), InitFunction_OnDriverDistraction_notification(struct_schema_items, function_id_items, message_type_items)));
}

//------------- Functions schemes initialization -------------

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_RegisterAppInterface_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Language::eType> Language_all_enum_values;
  Language_all_enum_values.insert(Language::EN_US);
  Language_all_enum_values.insert(Language::ES_MX);
  Language_all_enum_values.insert(Language::FR_CA);

  // Function parameter syncMsgVersion.
  //
  // See SyncMsgVersion
  utils::SharedPtr<ISchemaItem> syncMsgVersion_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::SyncMsgVersion);

  // Function parameter appName.
  //
  // The mobile application name, e.g. "Ford Drive Green".
  // Needs to be unique over all applications.
  // May not be empty.
  // May not start with a new line character.
  // May not interfere with any name or synonym of previously registered applications and the following list of words @TODO: Create list(global commands)
  // Needs to be unique over all applications. Applications with the same name will be rejected.
  // Only characters from char set [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
  utils::SharedPtr<ISchemaItem> appName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>());

  // Function parameter ngnMediaScreenAppName.
  //
  // Provides an abbreviated version of the app name (if needed), that will be displayed on the NGN media screen.
  // If not provided, the appName is used instead (and will be truncated if too long)
  // Only characters from char set [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
  utils::SharedPtr<ISchemaItem> ngnMediaScreenAppName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100), TSchemaItemParameter<std::string>());

  // Function parameter vrSynonyms.
  //
  // Defines an additional voice recognition command.
  // May not interfere with any name or synonym of previously registered applications and the following list of words @TODO: Create list(global commands)
  // Only characters from char set [@TODO: Create char set (character/hex value) for each ACM and refer to] are supported.
  utils::SharedPtr<ISchemaItem> vrSynonyms_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(40), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter usesVehicleData.
  //
  // If not provided, the default is equal to False"
  // Indicates if the mobile application wants to use vehicle data like GPS or speed.
  utils::SharedPtr<ISchemaItem> usesVehicleData_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter isMediaApplication.
  //
  // Indicates if the application is a media or a non-media application.
  // Only media applications will be able to stream audio to Sync that is audible outside of the BT media source.
  utils::SharedPtr<ISchemaItem> isMediaApplication_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Function parameter languageDesired.
  //
  // See Language
  // If the language doesn't match the active language on Sync, it will be rejected.
  // If the language is changed, while an app is registered, it will get disconnected.
  utils::SharedPtr<ISchemaItem> languageDesired_SchemaItem = TEnumSchemaItem<Language::eType>::create(Language_all_enum_values, TSchemaItemParameter<Language::eType>());

  // Function parameter autoActivateID.
  //
  // Used to support auto activation after an initial successful registerAppInterface (for example after an ignition cycle).
  // The app should always provide the id that was provided by the most recent registerAppInterface response.
  // If this is the first call to registerAppInterface, then do not provide this parameter at all.
  // If not provided or not matching with the id of the last registerAppInterface response, the app will not be automatically put into foreground on startup.
  utils::SharedPtr<ISchemaItem> autoActivateID_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(16), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["syncMsgVersion"] = CObjectSchemaItem::SMember(syncMsgVersion_SchemaItem, true);
  schema_members["appName"] = CObjectSchemaItem::SMember(appName_SchemaItem, true);
  schema_members["ngnMediaScreenAppName"] = CObjectSchemaItem::SMember(ngnMediaScreenAppName_SchemaItem, false);
  schema_members["vrSynonyms"] = CObjectSchemaItem::SMember(vrSynonyms_SchemaItem, false);
  schema_members["usesVehicleData"] = CObjectSchemaItem::SMember(usesVehicleData_SchemaItem, false);
  schema_members["isMediaApplication"] = CObjectSchemaItem::SMember(isMediaApplication_SchemaItem, true);
  schema_members["languageDesired"] = CObjectSchemaItem::SMember(languageDesired_SchemaItem, true);
  schema_members["autoActivateID"] = CObjectSchemaItem::SMember(autoActivateID_SchemaItem, false);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_RegisterAppInterface_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::WRONG_LANGUAGE);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_REGISTERED_ALREADY);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::DUPLICATE_NAME);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_VERSION);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_APPLICATIONS);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  std::set<Language::eType> Language_all_enum_values;
  Language_all_enum_values.insert(Language::EN_US);
  Language_all_enum_values.insert(Language::ES_MX);
  Language_all_enum_values.insert(Language::FR_CA);

  std::set<HmiZoneCapabilities::eType> HmiZoneCapabilities_all_enum_values;
  HmiZoneCapabilities_all_enum_values.insert(HmiZoneCapabilities::FRONT);
  HmiZoneCapabilities_all_enum_values.insert(HmiZoneCapabilities::BACK);

  std::set<SpeechCapabilities::eType> SpeechCapabilities_all_enum_values;
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::TEXT);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::SAPI_PHONEMES);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::LHPLUS_PHONEMES);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::PRE_RECORDED);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::SILENCE);

  std::set<VrCapabilities::eType> VrCapabilities_all_enum_values;
  VrCapabilities_all_enum_values.insert(VrCapabilities::Text);

  // Function parameter success.
  //
  // true, if successful
  // false, if failed
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

  // Function parameter autoActivateID.
  //
  // Provides an id that the app may use on a subsequent registerAppInterface to allow for auto activation (for example after an ignition cycle).
  utils::SharedPtr<ISchemaItem> autoActivateID_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(16), TSchemaItemParameter<std::string>());

  // Function parameter language.
  //
  // The currently active language on Sync. See "Language" for options.
  utils::SharedPtr<ISchemaItem> language_SchemaItem = TEnumSchemaItem<Language::eType>::create(Language_all_enum_values, TSchemaItemParameter<Language::eType>());

  // Function parameter displayCapabilities.
  //
  // See DisplayCapabilities
  utils::SharedPtr<ISchemaItem> displayCapabilities_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::DisplayCapabilities);

  // Function parameter buttonCapabilities.
  //
  // See ButtonCapabilities
  utils::SharedPtr<ISchemaItem> buttonCapabilities_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::ButtonCapabilities), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

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

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
  schema_members["syncMsgVersion"] = CObjectSchemaItem::SMember(syncMsgVersion_SchemaItem, false);
  schema_members["autoActivateID"] = CObjectSchemaItem::SMember(autoActivateID_SchemaItem, false);
  schema_members["language"] = CObjectSchemaItem::SMember(language_SchemaItem, false);
  schema_members["displayCapabilities"] = CObjectSchemaItem::SMember(displayCapabilities_SchemaItem, false);
  schema_members["buttonCapabilities"] = CObjectSchemaItem::SMember(buttonCapabilities_SchemaItem, false);
  schema_members["hmiZoneCapabilities"] = CObjectSchemaItem::SMember(hmiZoneCapabilities_SchemaItem, false);
  schema_members["speechCapabilities"] = CObjectSchemaItem::SMember(speechCapabilities_SchemaItem, false);
  schema_members["vrCapabilities"] = CObjectSchemaItem::SMember(vrCapabilities_SchemaItem, false);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_UnregisterAppInterface_request(
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_UnregisterAppInterface_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_SetGlobalProperties_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter helpPrompt.
  //
  // The help prompt.
  // An array of text chunks of type TTSChunk. See TTSChunk
  // The array must have at least one item
  utils::SharedPtr<ISchemaItem> helpPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter timeoutPrompt.
  //
  // Help text for a wait timeout.
  // An array of text chunks of type TTSChunk. See TTSChunk
  // The array must have at least one item
  utils::SharedPtr<ISchemaItem> timeoutPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["helpPrompt"] = CObjectSchemaItem::SMember(helpPrompt_SchemaItem, false);
  schema_members["timeoutPrompt"] = CObjectSchemaItem::SMember(timeoutPrompt_SchemaItem, false);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_SetGlobalProperties_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_ResetGlobalProperties_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<GlobalProperty::eType> GlobalProperty_all_enum_values;
  GlobalProperty_all_enum_values.insert(GlobalProperty::HELPPROMPT);
  GlobalProperty_all_enum_values.insert(GlobalProperty::TIMEOUTPROMPT);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_ResetGlobalProperties_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_AddCommand_request(
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
  // An array of strings to be used as VR sysnonyms for this command.
  // If this array is provided, it may not be empty.
  utils::SharedPtr<ISchemaItem> vrCommands_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(99), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["cmdID"] = CObjectSchemaItem::SMember(cmdID_SchemaItem, true);
  schema_members["menuParams"] = CObjectSchemaItem::SMember(menuParams_SchemaItem, false);
  schema_members["vrCommands"] = CObjectSchemaItem::SMember(vrCommands_SchemaItem, false);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_AddCommand_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_DeleteCommand_request(
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_DeleteCommand_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_AddSubMenu_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter menuID.
  //
  // unique ID of the sub menu to add.
  utils::SharedPtr<ISchemaItem> menuID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter position.
  //
  // Position within the items that are are at top level of the in application menu.
  // 0 will insert at the front.
  // 1 will insert at the second position.
  // If position is greater or equal than the number of items on top level, the sub menu will be appended to the end.
  // Position of any submenu will always be located before the return and exit options
  // If this param was omitted the entry will be added at the end.
  utils::SharedPtr<ISchemaItem> position_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(1000), TSchemaItemParameter<int>());

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_AddSubMenu_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_DeleteSubMenu_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter menuID.
  //
  // The "menuID" of the submenu to delete. (See addSubMenu.menuID)
  utils::SharedPtr<ISchemaItem> menuID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_DeleteSubMenu_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_CreateInteractionChoiceSet_request(
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_CreateInteractionChoiceSet_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_PerformInteraction_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<InteractionMode::eType> InteractionMode_all_enum_values;
  InteractionMode_all_enum_values.insert(InteractionMode::MANUAL_ONLY);
  InteractionMode_all_enum_values.insert(InteractionMode::VR_ONLY);
  InteractionMode_all_enum_values.insert(InteractionMode::BOTH);

  // Function parameter initialText.
  //
  // Text to be displayed first.
  utils::SharedPtr<ISchemaItem> initialText_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter initialPrompt.
  //
  // This is the intial prompt spoken to the user at the start of an interaction
  // An array of text chunks of type TTSChunk. See TTSChunk
  // The array must have at least one item
  utils::SharedPtr<ISchemaItem> initialPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter interactionMode.
  //
  // See InteractionMode.
  utils::SharedPtr<ISchemaItem> interactionMode_SchemaItem = TEnumSchemaItem<InteractionMode::eType>::create(InteractionMode_all_enum_values, TSchemaItemParameter<InteractionMode::eType>());

  // Function parameter interactionChoiceSetIDList.
  //
  // List of interaction choice set IDs to use with an interaction.
  utils::SharedPtr<ISchemaItem> interactionChoiceSetIDList_SchemaItem = CArraySchemaItem::create(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter helpPrompt.
  //
  // Help text. This is the spoken string when a user speaks "help" when the interaction is occuring.
  // An array of text chunks of type TTSChunk. See TTSChunk
  // The array must have at least one item
  utils::SharedPtr<ISchemaItem> helpPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter timeoutPrompt.
  //
  // Timeout text. This text is spoken when a VR interaction times out.
  // An array of text chunks of type TTSChunk. See TTSChunk
  // The array must have at least one item
  utils::SharedPtr<ISchemaItem> timeoutPrompt_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter timeout.
  //
  // Timeout in milliseconds.
  // If omitted a standard value of 10000 milliseconds is used.
  utils::SharedPtr<ISchemaItem> timeout_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(5000), TSchemaItemParameter<int>(100000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["initialText"] = CObjectSchemaItem::SMember(initialText_SchemaItem, true);
  schema_members["initialPrompt"] = CObjectSchemaItem::SMember(initialPrompt_SchemaItem, true);
  schema_members["interactionMode"] = CObjectSchemaItem::SMember(interactionMode_SchemaItem, true);
  schema_members["interactionChoiceSetIDList"] = CObjectSchemaItem::SMember(interactionChoiceSetIDList_SchemaItem, true);
  schema_members["helpPrompt"] = CObjectSchemaItem::SMember(helpPrompt_SchemaItem, false);
  schema_members["timeoutPrompt"] = CObjectSchemaItem::SMember(timeoutPrompt_SchemaItem, false);
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_PerformInteraction_response(
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
  resultCode_allowed_enum_subset_values.insert(Result::ABORTED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  std::set<TriggerSource::eType> TriggerSource_all_enum_values;
  TriggerSource_all_enum_values.insert(TriggerSource::TS_MENU);
  TriggerSource_all_enum_values.insert(TriggerSource::TS_VR);

  // Function parameter success.
  //
  // true, if successful
  // false, if failed
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
  // ID of the choice that was selected in response to PerformInteraction.
  utils::SharedPtr<ISchemaItem> choiceID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Function parameter triggerSource.
  //
  // See TriggerSource
  utils::SharedPtr<ISchemaItem> triggerSource_SchemaItem = TEnumSchemaItem<TriggerSource::eType>::create(TriggerSource_all_enum_values, TSchemaItemParameter<TriggerSource::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
  schema_members["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
  schema_members["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
  schema_members["choiceID"] = CObjectSchemaItem::SMember(choiceID_SchemaItem, false);
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_DeleteInteractionChoiceSet_request(
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_DeleteInteractionChoiceSet_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_Alert_request(
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

  // Function parameter ttsChunks.
  //
  // An array of text chunks of type TTSChunk. See TTSChunk
  // The array must have at least one item
  utils::SharedPtr<ISchemaItem> ttsChunks_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TTSChunk), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Function parameter duration.
  //
  // Timeout in milliseconds.
  // Typical timeouts are 3-5 seconds
  // If omitted, timeout is set to 5s.
  utils::SharedPtr<ISchemaItem> duration_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(3000), TSchemaItemParameter<int>(10000), TSchemaItemParameter<int>());

  // Function parameter playTone.
  //
  // Defines if tone should be played. Tone is played before TTS.
  // If omitted, no tone is played.
  utils::SharedPtr<ISchemaItem> playTone_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["alertText1"] = CObjectSchemaItem::SMember(alertText1_SchemaItem, false);
  schema_members["alertText2"] = CObjectSchemaItem::SMember(alertText2_SchemaItem, false);
  schema_members["ttsChunks"] = CObjectSchemaItem::SMember(ttsChunks_SchemaItem, false);
  schema_members["duration"] = CObjectSchemaItem::SMember(duration_SchemaItem, false);
  schema_members["playTone"] = CObjectSchemaItem::SMember(playTone_SchemaItem, false);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_Alert_response(
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
  resultCode_allowed_enum_subset_values.insert(Result::ABORTED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_Show_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<TextAlignment::eType> TextAlignment_all_enum_values;
  TextAlignment_all_enum_values.insert(TextAlignment::LEFT_ALIGNED);
  TextAlignment_all_enum_values.insert(TextAlignment::RIGHT_ALIGNED);
  TextAlignment_all_enum_values.insert(TextAlignment::CENTERED);

  // Function parameter mainField1.
  //
  // The text that should be displayed in a single or upper display line.
  // If this text is not set, the text of mainField1 stays unchanged.
  // If this text is empty "", the field will be cleared.
  utils::SharedPtr<ISchemaItem> mainField1_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter mainField2.
  //
  // The text that should be displayed on the second dispay line.
  // If this text is not set, the text of mainField2 stays unchanged.
  // If this text is empty "", the field will be cleared.
  utils::SharedPtr<ISchemaItem> mainField2_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter alignment.
  //
  // Specifies how mainField1 and mainField2 texts should be aligned on display.
  // If omitted, texts will be centered
  utils::SharedPtr<ISchemaItem> alignment_SchemaItem = TEnumSchemaItem<TextAlignment::eType>::create(TextAlignment_all_enum_values, TSchemaItemParameter<TextAlignment::eType>());

  // Function parameter statusBar.
  //
  // Requires investigation regarding the nav display capabilities. Potentially lower lowerStatusBar, upperStatusBar, titleBar, etc.
  utils::SharedPtr<ISchemaItem> statusBar_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter mediaClock.
  //
  // Text value for MediaClock field. Has to be properly formatted by Mobile App according to Sync capabilities.
  // If this text is set, any automatic media clock updates previously set with SetMediaClockTimer will be stopped.
  utils::SharedPtr<ISchemaItem> mediaClock_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Function parameter mediaTrack.
  //
  // The text that should be displayed in the track field.
  // This field is only valid for media applications on NGN type ACMs.
  // If this text is not set, the text of mediaTrack stays unchanged.
  // If this text is empty "", the field will be cleared.
  utils::SharedPtr<ISchemaItem> mediaTrack_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["mainField1"] = CObjectSchemaItem::SMember(mainField1_SchemaItem, false);
  schema_members["mainField2"] = CObjectSchemaItem::SMember(mainField2_SchemaItem, false);
  schema_members["alignment"] = CObjectSchemaItem::SMember(alignment_SchemaItem, false);
  schema_members["statusBar"] = CObjectSchemaItem::SMember(statusBar_SchemaItem, false);
  schema_members["mediaClock"] = CObjectSchemaItem::SMember(mediaClock_SchemaItem, false);
  schema_members["mediaTrack"] = CObjectSchemaItem::SMember(mediaTrack_SchemaItem, false);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_Show_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_Speak_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter ttsChunks.
  //
  // An array of text chunks of type TTSChunk. See TTSChunk
  // The array must have at least one item
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_Speak_response(
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
  resultCode_allowed_enum_subset_values.insert(Result::ABORTED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_SetMediaClockTimer_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<UpdateMode::eType> UpdateMode_all_enum_values;
  UpdateMode_all_enum_values.insert(UpdateMode::COUNTUP);
  UpdateMode_all_enum_values.insert(UpdateMode::COUNTDOWN);
  UpdateMode_all_enum_values.insert(UpdateMode::PAUSE);
  UpdateMode_all_enum_values.insert(UpdateMode::RESUME);

  // Function parameter startTime.
  //
  // See StartTime
  // startTime must be provided for "run"
  // startTime will be ignored for "pause" and "resum"
  utils::SharedPtr<ISchemaItem> startTime_SchemaItem = ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::StartTime);

  // Function parameter updateMode.
  //
  // Enumeration to control the media clock.
  // In case of pause or resume, the start time value is ignored and shall be left out.  For resume, the time continues with the same value as it was when paused.
  utils::SharedPtr<ISchemaItem> updateMode_SchemaItem = TEnumSchemaItem<UpdateMode::eType>::create(UpdateMode_all_enum_values, TSchemaItemParameter<UpdateMode::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["startTime"] = CObjectSchemaItem::SMember(startTime_SchemaItem, false);
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_SetMediaClockTimer_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_EncodedSyncPData_request(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter data.
  //
  // Contains base64 encoded string of SyncP packets.
  //
  // ToDo: What is the maxlength
  utils::SharedPtr<ISchemaItem> data_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(10000), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_EncodedSyncPData_response(
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
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_SubscribeButton_request(
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_SubscribeButton_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::IGNORED);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_BUTTON);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_UnsubscribeButton_request(
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_UnsubscribeButton_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::IGNORED);
  resultCode_allowed_enum_subset_values.insert(Result::APPLICATION_NOT_REGISTERED);
  resultCode_allowed_enum_subset_values.insert(Result::SUCCESS);
  resultCode_allowed_enum_subset_values.insert(Result::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowed_enum_subset_values.insert(Result::UNSUPPORTED_BUTTON);
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);
  resultCode_allowed_enum_subset_values.insert(Result::OUT_OF_MEMORY);
  resultCode_allowed_enum_subset_values.insert(Result::REJECTED);
  resultCode_allowed_enum_subset_values.insert(Result::GENERIC_ERROR);

  // Function parameter success.
  //
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_GenericResponse_response(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<Result::eType> resultCode_allowed_enum_subset_values;
  resultCode_allowed_enum_subset_values.insert(Result::INVALID_DATA);

  // Function parameter success.
  //
  // true, if successful
  // false, if failed
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_OnHMIStatus_notification(
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
  AudioStreamingState_all_enum_values.insert(AudioStreamingState::NOT_AUDIBLE);

  std::set<SystemContext::eType> SystemContext_all_enum_values;
  SystemContext_all_enum_values.insert(SystemContext::SYSCTXT_MAIN);
  SystemContext_all_enum_values.insert(SystemContext::SYSCTXT_VRSESSION);
  SystemContext_all_enum_values.insert(SystemContext::SYSCTXT_MENU);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_OnAppInterfaceUnregistered_notification(
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_OnButtonEvent_notification(
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

  std::set<ButtonEventMode::eType> ButtonEventMode_all_enum_values;
  ButtonEventMode_all_enum_values.insert(ButtonEventMode::BUTTONUP);
  ButtonEventMode_all_enum_values.insert(ButtonEventMode::BUTTONDOWN);

  // Function parameter buttonName.
  utils::SharedPtr<ISchemaItem> buttonName_SchemaItem = TEnumSchemaItem<ButtonName::eType>::create(ButtonName_all_enum_values, TSchemaItemParameter<ButtonName::eType>());

  // Function parameter buttonEventMode.
  //
  // Indicates whether this is an UP or DOWN event.
  utils::SharedPtr<ISchemaItem> buttonEventMode_SchemaItem = TEnumSchemaItem<ButtonEventMode::eType>::create(ButtonEventMode_all_enum_values, TSchemaItemParameter<ButtonEventMode::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["buttonName"] = CObjectSchemaItem::SMember(buttonName_SchemaItem, true);
  schema_members["buttonEventMode"] = CObjectSchemaItem::SMember(buttonEventMode_SchemaItem, true);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_OnButtonPress_notification(
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

  std::set<ButtonPressMode::eType> ButtonPressMode_all_enum_values;
  ButtonPressMode_all_enum_values.insert(ButtonPressMode::LONG);
  ButtonPressMode_all_enum_values.insert(ButtonPressMode::SHORT);

  // Function parameter buttonName.
  utils::SharedPtr<ISchemaItem> buttonName_SchemaItem = TEnumSchemaItem<ButtonName::eType>::create(ButtonName_all_enum_values, TSchemaItemParameter<ButtonName::eType>());

  // Function parameter buttonPressMode.
  //
  // Indicates whether this is a LONG or SHORT button press event.
  utils::SharedPtr<ISchemaItem> buttonPressMode_SchemaItem = TEnumSchemaItem<ButtonPressMode::eType>::create(ButtonPressMode_all_enum_values, TSchemaItemParameter<ButtonPressMode::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["buttonName"] = CObjectSchemaItem::SMember(buttonName_SchemaItem, true);
  schema_members["buttonPressMode"] = CObjectSchemaItem::SMember(buttonPressMode_SchemaItem, true);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_OnCommand_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<TriggerSource::eType> TriggerSource_all_enum_values;
  TriggerSource_all_enum_values.insert(TriggerSource::TS_MENU);
  TriggerSource_all_enum_values.insert(TriggerSource::TS_VR);

  // Function parameter cmdID.
  //
  // Command ID, which is related to a specific menu entry.
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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_OnEncodedSyncPData_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  // Function parameter data.
  //
  // Contains base64 encoded string of SyncP packets.
  utils::SharedPtr<ISchemaItem> data_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(10000), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["data"] = CObjectSchemaItem::SMember(data_SchemaItem, true);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_OnTBTClientState_notification(
    const TStructsSchemaItems &struct_schema_items,
    const std::set<FunctionID::eType> &function_id_items,
    const std::set<messageType::eType> &message_type_items) {
  std::set<TBTState::eType> TBTState_all_enum_values;
  TBTState_all_enum_values.insert(TBTState::ROUTE_UPDATE_REQUEST);
  TBTState_all_enum_values.insert(TBTState::ROUTE_ACCEPTED);
  TBTState_all_enum_values.insert(TBTState::ROUTE_REFUSED);
  TBTState_all_enum_values.insert(TBTState::ROUTE_CANCELLED);

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

CSmartSchema NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitFunction_OnDriverDistraction_notification(
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

//----------- Structs schema items initialization ------------

utils::SharedPtr<ISchemaItem> NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitStructSchemaItem_Choice(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member choiceID.
  utils::SharedPtr<ISchemaItem> choiceID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(65535), TSchemaItemParameter<int>());

  // Struct member menuName.
  utils::SharedPtr<ISchemaItem> menuName_SchemaItem = CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(500), TSchemaItemParameter<std::string>());

  // Struct member vrCommands.
  utils::SharedPtr<ISchemaItem> vrCommands_SchemaItem = CArraySchemaItem::create(CStringSchemaItem::create(TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(99), TSchemaItemParameter<std::string>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["choiceID"] = CObjectSchemaItem::SMember(choiceID_SchemaItem, true);
  schema_members["menuName"] = CObjectSchemaItem::SMember(menuName_SchemaItem, true);
  schema_members["vrCommands"] = CObjectSchemaItem::SMember(vrCommands_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitStructSchemaItem_SyncMsgVersion(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member majorVersion.
  //
  // The major version indicates versions that is not-compatible to previous versions.
  utils::SharedPtr<ISchemaItem> majorVersion_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(1), TSchemaItemParameter<int>());

  // Struct member minorVersion.
  //
  // The minor version indicates a change to a previous version that should still allow to be run on an older version (with limited functionality)
  utils::SharedPtr<ISchemaItem> minorVersion_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(1000), TSchemaItemParameter<int>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["majorVersion"] = CObjectSchemaItem::SMember(majorVersion_SchemaItem, true);
  schema_members["minorVersion"] = CObjectSchemaItem::SMember(minorVersion_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitStructSchemaItem_StartTime(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member hours.
  //
  // The hour of the media clock.
  // Some radios only support a max of 19 hours. If out of range, it will be rejected.
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

utils::SharedPtr<ISchemaItem> NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitStructSchemaItem_TextField(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<TextFieldName::eType> TextFieldName_all_enum_values;
  TextFieldName_all_enum_values.insert(TextFieldName::mainField1);
  TextFieldName_all_enum_values.insert(TextFieldName::mainField2);
  TextFieldName_all_enum_values.insert(TextFieldName::statusBar);
  TextFieldName_all_enum_values.insert(TextFieldName::mediaClock);
  TextFieldName_all_enum_values.insert(TextFieldName::mediaTrack);
  TextFieldName_all_enum_values.insert(TextFieldName::alertText1);
  TextFieldName_all_enum_values.insert(TextFieldName::alertText2);

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
  utils::SharedPtr<ISchemaItem> width_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1), TSchemaItemParameter<int>(40), TSchemaItemParameter<int>());

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

utils::SharedPtr<ISchemaItem> NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitStructSchemaItem_DisplayCapabilities(
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

  std::set<MediaClockFormat::eType> MediaClockFormat_all_enum_values;
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCK1);
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCK2);
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCKTEXT1);
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCKTEXT2);
  MediaClockFormat_all_enum_values.insert(MediaClockFormat::CLOCKTEXT3);

  // Struct member displayType.
  //
  // The type of the display. See DisplayType
  utils::SharedPtr<ISchemaItem> displayType_SchemaItem = TEnumSchemaItem<DisplayType::eType>::create(DisplayType_all_enum_values, TSchemaItemParameter<DisplayType::eType>());

  // Struct member textFields.
  //
  // A set of all fields that support text data. See TextField
  utils::SharedPtr<ISchemaItem> textFields_SchemaItem = CArraySchemaItem::create(ProvideObjectSchemaItemForStruct(struct_schema_items, StructIdentifiers::TextField), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  // Struct member mediaClockFormats.
  //
  // A set of all supported formats of the media clock. See MediaClockFormat
  utils::SharedPtr<ISchemaItem> mediaClockFormats_SchemaItem = CArraySchemaItem::create(TEnumSchemaItem<MediaClockFormat::eType>::create(MediaClockFormat_all_enum_values, TSchemaItemParameter<MediaClockFormat::eType>()), TSchemaItemParameter<size_t>(1), TSchemaItemParameter<size_t>(100));

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["displayType"] = CObjectSchemaItem::SMember(displayType_SchemaItem, true);
  schema_members["textFields"] = CObjectSchemaItem::SMember(textFields_SchemaItem, true);
  schema_members["mediaClockFormats"] = CObjectSchemaItem::SMember(mediaClockFormats_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitStructSchemaItem_ButtonCapabilities(
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

  // Struct member name.
  //
  // The name of the button. See ButtonName.
  utils::SharedPtr<ISchemaItem> name_SchemaItem = TEnumSchemaItem<ButtonName::eType>::create(ButtonName_all_enum_values, TSchemaItemParameter<ButtonName::eType>());

  // Struct member shortPressAvailable.
  //
  // The button supports a short press.
  // Whenever the button is pressed short, onButtonPressed( SHORT) will be invoked.
  utils::SharedPtr<ISchemaItem> shortPressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member longPressAvailable.
  //
  // The button supports a LONG press.
  // Whenever the button is pressed long, onButtonPressed( LONG) will be invoked.
  utils::SharedPtr<ISchemaItem> longPressAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  // Struct member upDownAvailable.
  //
  // The button supports "button down" and "button up".
  // Whenever the button is pressed, onButtonEvent( DOWN) will be invoked.
  // Whenever the button is released, onButtonEvent( UP) will be invoked.
  utils::SharedPtr<ISchemaItem> upDownAvailable_SchemaItem = CBoolSchemaItem::create(TSchemaItemParameter<bool>());

  std::map<std::string, CObjectSchemaItem::SMember> schema_members;

  schema_members["name"] = CObjectSchemaItem::SMember(name_SchemaItem, true);
  schema_members["shortPressAvailable"] = CObjectSchemaItem::SMember(shortPressAvailable_SchemaItem, true);
  schema_members["longPressAvailable"] = CObjectSchemaItem::SMember(longPressAvailable_SchemaItem, true);
  schema_members["upDownAvailable"] = CObjectSchemaItem::SMember(upDownAvailable_SchemaItem, true);

  return CObjectSchemaItem::create(schema_members);
}

utils::SharedPtr<ISchemaItem> NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitStructSchemaItem_MenuParams(
    const TStructsSchemaItems &struct_schema_items) {
  // Struct member parentID.
  //
  // unique ID of the sub menu, the command will be added to.
  // If not provided, it will be provided to the top level of the in application menu.
  utils::SharedPtr<ISchemaItem> parentID_SchemaItem = TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000), TSchemaItemParameter<int>());

  // Struct member position.
  //
  // Position within the items that are are at top level of the in application menu.
  // 0 will insert at the front.
  // 1 will insert at the second position.
  // if position is greater or equal than the number of items on top level, the sub menu will be appended to the end.
  // If this param was omitted the entry will be added at the end.
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

utils::SharedPtr<ISchemaItem> NsSmartDeviceLinkRPC::V1::v4_protocol_v1_2_no_extra::InitStructSchemaItem_TTSChunk(
    const TStructsSchemaItems &struct_schema_items) {
  std::set<SpeechCapabilities::eType> SpeechCapabilities_all_enum_values;
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::TEXT);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::SAPI_PHONEMES);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::LHPLUS_PHONEMES);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::PRE_RECORDED);
  SpeechCapabilities_all_enum_values.insert(SpeechCapabilities::SILENCE);

  // Struct member text.
  //
  // The text or phonemes to speak.
  // May not be empty.
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

//-------------- String to value enum mapping ----------------

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

template <>
const std::map<NsSmartDeviceLinkRPC::V1::FunctionID::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::FunctionID::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::FunctionID::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::RegisterAppInterface, "RegisterAppInterface"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::UnregisterAppInterface, "UnregisterAppInterface"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::SetGlobalProperties, "SetGlobalProperties"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::ResetGlobalProperties, "ResetGlobalProperties"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::AddCommand, "AddCommand"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::DeleteCommand, "DeleteCommand"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::AddSubMenu, "AddSubMenu"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::DeleteSubMenu, "DeleteSubMenu"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::CreateInteractionChoiceSet, "CreateInteractionChoiceSet"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::PerformInteraction, "PerformInteraction"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::DeleteInteractionChoiceSet, "DeleteInteractionChoiceSet"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::Alert, "Alert"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::Show, "Show"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::Speak, "Speak"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::SetMediaClockTimer, "SetMediaClockTimer"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::EncodedSyncPData, "EncodedSyncPData"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::SubscribeButton, "SubscribeButton"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::UnsubscribeButton, "UnsubscribeButton"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::GenericResponse, "GenericResponse"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::OnHMIStatus, "OnHMIStatus"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::OnAppInterfaceUnregistered, "OnAppInterfaceUnregistered"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::OnButtonEvent, "OnButtonEvent"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::OnButtonPress, "OnButtonPress"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::OnCommand, "OnCommand"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::OnEncodedSyncPData, "OnEncodedSyncPData"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::OnTBTClientState, "OnTBTClientState"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::FunctionID::OnDriverDistraction, "OnDriverDistraction"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::messageType::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::messageType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::messageType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::messageType::request, "request"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::messageType::response, "response"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::messageType::notification, "notification"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::Result::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::Result::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::Result::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::SUCCESS, "SUCCESS"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA, "INVALID_DATA"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::UNSUPPORTED_REQUEST, "UNSUPPORTED_REQUEST"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::OUT_OF_MEMORY, "OUT_OF_MEMORY"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::TOO_MANY_PENDING_REQUESTS, "TOO_MANY_PENDING_REQUESTS"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::INVALID_ID, "INVALID_ID"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::DUPLICATE_NAME, "DUPLICATE_NAME"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::TOO_MANY_APPLICATIONS, "TOO_MANY_APPLICATIONS"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::APPLICATION_REGISTERED_ALREADY, "APPLICATION_REGISTERED_ALREADY"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::UNSUPPORTED_VERSION, "UNSUPPORTED_VERSION"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::WRONG_LANGUAGE, "WRONG_LANGUAGE"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::APPLICATION_NOT_REGISTERED, "APPLICATION_NOT_REGISTERED"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::IN_USE, "IN_USE"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::SUBSCRIBED_ALREADY, "SUBSCRIBED_ALREADY"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::REJECTED, "REJECTED"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::ABORTED, "ABORTED"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::IGNORED, "IGNORED"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::UNSUPPORTED_BUTTON, "UNSUPPORTED_BUTTON"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::FILE_NOT_FOUND, "FILE_NOT_FOUND"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Result::GENERIC_ERROR, "GENERIC_ERROR"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::ButtonPressMode::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::ButtonPressMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::ButtonPressMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonPressMode::LONG, "LONG"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonPressMode::SHORT, "SHORT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::ButtonEventMode::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::ButtonEventMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::ButtonEventMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonEventMode::BUTTONUP, "BUTTONUP"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonEventMode::BUTTONDOWN, "BUTTONDOWN"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::Language::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::Language::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::Language::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Language::EN_US, "EN-US"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Language::ES_MX, "ES-MX"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::Language::FR_CA, "FR-CA"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::UpdateMode::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::UpdateMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::UpdateMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::UpdateMode::COUNTUP, "COUNTUP"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::UpdateMode::COUNTDOWN, "COUNTDOWN"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::UpdateMode::PAUSE, "PAUSE"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::UpdateMode::RESUME, "RESUME"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::InteractionMode::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::InteractionMode::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::InteractionMode::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::InteractionMode::MANUAL_ONLY, "MANUAL_ONLY"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::InteractionMode::VR_ONLY, "VR_ONLY"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::InteractionMode::BOTH, "BOTH"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::HMILevel::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::HMILevel::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::HMILevel::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::HMILevel::HMI_FULL, "FULL"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::HMILevel::HMI_LIMITED, "LIMITED"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::HMILevel::HMI_BACKGROUND, "BACKGROUND"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::HMILevel::HMI_NONE, "NONE"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::AudioStreamingState::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::AudioStreamingState::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::AudioStreamingState::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AudioStreamingState::AUDIBLE, "AUDIBLE"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AudioStreamingState::NOT_AUDIBLE, "NOT_AUDIBLE"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::SystemContext::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::SystemContext::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::SystemContext::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::SystemContext::SYSCTXT_MAIN, "MAIN"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::SystemContext::SYSCTXT_VRSESSION, "VRSESSION"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::SystemContext::SYSCTXT_MENU, "MENU"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::USER_EXIT, "USER_EXIT"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::IGNITION_OFF, "IGNITION_OFF"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::BLUETOOTH_OFF, "BLUETOOTH_OFF"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::USB_DISCONNECTED, "USB_DISCONNECTED"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::REQUEST_WHILE_IN_NONE_HMI_LEVEL, "REQUEST_WHILE_IN_NONE_HMI_LEVEL"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::TOO_MANY_REQUESTS, "TOO_MANY_REQUESTS"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::DRIVER_DISTRACTION_VIOLATION, "DRIVER_DISTRACTION_VIOLATION"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::LANGUAGE_CHANGE, "LANGUAGE_CHANGE"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::MASTER_RESET, "MASTER_RESET"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AppInterfaceUnregisteredReason::FACTORY_DEFAULTS, "FACTORY_DEFAULTS"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::TriggerSource::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::TriggerSource::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::TriggerSource::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TriggerSource::TS_MENU, "MENU"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TriggerSource::TS_VR, "VR"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::HmiZoneCapabilities::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::HmiZoneCapabilities::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::HmiZoneCapabilities::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::HmiZoneCapabilities::FRONT, "FRONT"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::HmiZoneCapabilities::BACK, "BACK"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::SpeechCapabilities::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::SpeechCapabilities::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::SpeechCapabilities::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::SpeechCapabilities::TEXT, "TEXT"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::SpeechCapabilities::SAPI_PHONEMES, "SAPI_PHONEMES"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::SpeechCapabilities::LHPLUS_PHONEMES, "LHPLUS_PHONEMES"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::SpeechCapabilities::PRE_RECORDED, "PRE_RECORDED"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::SpeechCapabilities::SILENCE, "SILENCE"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::VrCapabilities::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::VrCapabilities::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::VrCapabilities::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::VrCapabilities::Text, "Text"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::AudioCaptureQuality::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::AudioCaptureQuality::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::AudioCaptureQuality::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AudioCaptureQuality::QUALITY_8KHZ, "8KHZ"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::AudioCaptureQuality::QUALITY_16KHZ, "16KHZ"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::ButtonName::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::ButtonName::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::ButtonName::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::OK, "OK"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::SEEKLEFT, "SEEKLEFT"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::SEEKRIGHT, "SEEKRIGHT"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::TUNEUP, "TUNEUP"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::TUNEDOWN, "TUNEDOWN"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::PRESET_0, "PRESET_0"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::PRESET_1, "PRESET_1"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::PRESET_2, "PRESET_2"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::PRESET_3, "PRESET_3"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::PRESET_4, "PRESET_4"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::PRESET_5, "PRESET_5"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::PRESET_6, "PRESET_6"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::PRESET_7, "PRESET_7"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::PRESET_8, "PRESET_8"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::ButtonName::PRESET_9, "PRESET_9"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::MediaClockFormat::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::MediaClockFormat::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::MediaClockFormat::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::MediaClockFormat::CLOCK1, "CLOCK1"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::MediaClockFormat::CLOCK2, "CLOCK2"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::MediaClockFormat::CLOCKTEXT1, "CLOCKTEXT1"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::MediaClockFormat::CLOCKTEXT2, "CLOCKTEXT2"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::MediaClockFormat::CLOCKTEXT3, "CLOCKTEXT3"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::DisplayType::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::DisplayType::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::DisplayType::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::DisplayType::CID, "CID"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::DisplayType::TYPE2, "TYPE2"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::DisplayType::TYPE5, "TYPE5"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::DisplayType::NGN, "NGN"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::DisplayType::GEN2_8_DMA, "GEN2_8_DMA"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::DisplayType::GEN2_6_DMA, "GEN2_6_DMA"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::DisplayType::MFD3, "MFD3"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::DisplayType::MFD4, "MFD4"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::DisplayType::MFD5, "MFD5"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::TextFieldName::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::TextFieldName::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::TextFieldName::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TextFieldName::mainField1, "mainField1"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TextFieldName::mainField2, "mainField2"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TextFieldName::statusBar, "statusBar"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TextFieldName::mediaClock, "mediaClock"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TextFieldName::mediaTrack, "mediaTrack"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TextFieldName::alertText1, "alertText1"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TextFieldName::alertText2, "alertText2"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::CharacterSet::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::CharacterSet::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::CharacterSet::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::CharacterSet::TYPE2SET, "TYPE2SET"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::CharacterSet::TYPE5SET, "TYPE5SET"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::CharacterSet::CID1SET, "CID1SET"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::CharacterSet::CID2SET, "CID2SET"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::TextAlignment::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::TextAlignment::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::TextAlignment::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TextAlignment::LEFT_ALIGNED, "LEFT_ALIGNED"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TextAlignment::RIGHT_ALIGNED, "RIGHT_ALIGNED"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TextAlignment::CENTERED, "CENTERED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::TBTState::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::TBTState::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::TBTState::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TBTState::ROUTE_UPDATE_REQUEST, "ROUTE_UPDATE_REQUEST"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TBTState::ROUTE_ACCEPTED, "ROUTE_ACCEPTED"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TBTState::ROUTE_REFUSED, "ROUTE_REFUSED"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::TBTState::ROUTE_CANCELLED, "ROUTE_CANCELLED"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::DriverDistractionState::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::DriverDistractionState::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::DriverDistractionState::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::DriverDistractionState::DD_ON, "DD_ON"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::DriverDistractionState::DD_OFF, "DD_OFF"));

    is_initialized = true;
  }

  return enum_string_representation;
}

template <>
const std::map<NsSmartDeviceLinkRPC::V1::GlobalProperty::eType, std::string> &TEnumSchemaItem<NsSmartDeviceLinkRPC::V1::GlobalProperty::eType>::getEnumElementsStringRepresentation() {
  static bool is_initialized = false;
  static std::map<NsSmartDeviceLinkRPC::V1::GlobalProperty::eType, std::string> enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::GlobalProperty::HELPPROMPT, "HELPPROMPT"));
    enum_string_representation.insert(std::make_pair(NsSmartDeviceLinkRPC::V1::GlobalProperty::TIMEOUTPROMPT, "TIMEOUTPROMPT"));

    is_initialized = true;
  }

  return enum_string_representation;
}

} // NsSmartObjects
} // NsSmartDeviceLink

