/**
 * @file MOBILE_API.hpp
 * @brief Generated class MOBILE_API header file.
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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __CSMARTFACTORY_MOBILE_API_8279afa8cba511e3a201902b34794c53_HPP__
#define __CSMARTFACTORY_MOBILE_API_8279afa8cba511e3a201902b34794c53_HPP__

#include "formatters/CSmartFactory.hpp"
#include "smart_objects/smart_schema.h"
#include "smart_objects/schema_item.h"
#include "utils/shared_ptr.h"
#include "MOBILE_API.h"

namespace mobile_apis {


/**
 * @brief Class MOBILE_API.
 *
 * Params:
 *     date - 2013-05-30
 *     version - 2.1B
 */
class MOBILE_API : public NsSmartDeviceLink::NsJSONHandler::CSmartFactory<FunctionID::eType, messageType::eType, StructIdentifiers::eType> {
 public:
  /**
   * @brief Constructor.
   */
  MOBILE_API();

 protected:
  /**
   * @brief Type that maps of struct IDs to schema items.
   */
  typedef std::map<const StructIdentifiers::eType, utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> > TStructsSchemaItems;

  /**
   * @brief Helper that allows to make reference to struct
   *
   * @param struct_schema_items Struct schema items.
   * @param struct_id ID of structure to provide.
   *
   * @return utils::SharedPtr of strucute
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> ProvideObjectSchemaItemForStruct(
        const TStructsSchemaItems &struct_schema_items,
        const StructIdentifiers::eType struct_id);

  /**
   * @brief Initializes all struct schemes.
   */
  void InitStructSchemes(TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Initializes all function schemes.
   *
   * @param struct_schema_items Struct schema items.
   * @param function_id_items Set of all elements of FunctionID enum.
   * @param message_type_items Set of all elements of messageType enum.
   */
  void InitFunctionSchemes(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function RegisterAppInterface.
   *
   * 
   *     	Establishes an interface with a mobile application.
   *     	Before registerAppInterface no other commands will be accepted/executed.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_RegisterAppInterfaceID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function RegisterAppInterface.
   *
   * The response to registerAppInterface
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_RegisterAppInterfaceID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UnregisterAppInterface.
   *
   * 
   *     	Closes an interface from a mobile application.
   *     	After unregisterAppInterface, no commands other than registerAppInterface will be accepted/executed.
   *     	Will fail, if no registerAppInterface was completed successfully before.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UnregisterAppInterfaceID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UnregisterAppInterface.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UnregisterAppInterfaceID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SetGlobalProperties.
   *
   * Allows setting global properties.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SetGlobalPropertiesID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SetGlobalProperties.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SetGlobalPropertiesID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ResetGlobalProperties.
   *
   * Allows resetting global properties.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ResetGlobalPropertiesID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ResetGlobalProperties.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ResetGlobalPropertiesID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function AddCommand.
   *
   * 
   *     	Adds a command to the in application menu.
   *     	Either menuParams or vrCommands must be provided.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_AddCommandID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function AddCommand.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_AddCommandID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function DeleteCommand.
   *
   * Deletes all commands from the in-application menu with the specified command id.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_DeleteCommandID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function DeleteCommand.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_DeleteCommandID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function AddSubMenu.
   *
   * Adds a sub menu to the in-application menu.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_AddSubMenuID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function AddSubMenu.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_AddSubMenuID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function DeleteSubMenu.
   *
   * Deletes a submenu from the in-application menu.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_DeleteSubMenuID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function DeleteSubMenu.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_DeleteSubMenuID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function CreateInteractionChoiceSet.
   *
   * creates interaction choice set to be used later by performInteraction
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_CreateInteractionChoiceSetID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function CreateInteractionChoiceSet.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_CreateInteractionChoiceSetID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function PerformInteraction.
   *
   * Triggers an interaction (e.g. "Permit GPS?" - Yes, no, Always Allow).
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_PerformInteractionID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function PerformInteraction.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_PerformInteractionID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function DeleteInteractionChoiceSet.
   *
   * Deletes interaction choice set that has been created with "CreateInteractionChoiceSet".
   * The interaction may only be deleted when not currently in use by a "performInteraction".
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_DeleteInteractionChoiceSetID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function DeleteInteractionChoiceSet.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_DeleteInteractionChoiceSetID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Alert.
   *
   * Shows an alert which typically consists of text-to-speech message and text on the display. At least either alertText1, alertText2 or TTSChunks need to be provided.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_AlertID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Alert.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_AlertID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Show.
   *
   * Updates the persistent display. Supported fields depend on display capabilities.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ShowID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Show.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ShowID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Speak.
   *
   * Speaks a text.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SpeakID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Speak.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SpeakID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SetMediaClockTimer.
   *
   * Sets the initial media clock value and automatic update method.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SetMediaClockTimerID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SetMediaClockTimer.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SetMediaClockTimerID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function PerformAudioPassThru.
   *
   * Starts audio pass thru session 
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_PerformAudioPassThruID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function PerformAudioPassThru.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_PerformAudioPassThruID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function EndAudioPassThru.
   *
   * When this request is invoked, the audio capture stops.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_EndAudioPassThruID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function EndAudioPassThru.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_EndAudioPassThruID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SubscribeButton.
   *
   * 
   *     	Subscribes to built-in HMI buttons.
   *     	The application will be notified by the OnButtonEvent and OnButtonPress.
   *     	To unsubscribe the notifications, use unsubscribeButton.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SubscribeButtonID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SubscribeButton.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SubscribeButtonID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UnsubscribeButton.
   *
   * Unsubscribes from built-in HMI buttons.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UnsubscribeButtonID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UnsubscribeButton.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UnsubscribeButtonID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SubscribeVehicleData.
   *
   * 
   *     	Subscribes for specific published data items.
   *     	The data will be only sent if it has changed.
   *     	The application will be notified by the onVehicleData notification whenever new data is available.
   *     	To unsubscribe the notifications, use unsubscribe with the same subscriptionType.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SubscribeVehicleDataID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SubscribeVehicleData.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SubscribeVehicleDataID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UnsubscribeVehicleData.
   *
   * This function is used to unsubscribe the notifications from the subscribeVehicleData function.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UnsubscribeVehicleDataID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UnsubscribeVehicleData.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UnsubscribeVehicleDataID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function GetVehicleData.
   *
   * Non periodic vehicle data read request.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_GetVehicleDataID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function GetVehicleData.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_GetVehicleDataID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ReadDID.
   *
   * Non periodic vehicle data read request
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ReadDIDID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ReadDID.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ReadDIDID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function GetDTCs.
   *
   * Vehicle module diagnostic trouble code request.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_GetDTCsID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function GetDTCs.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_GetDTCsID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function DiagnosticMessage.
   *
   * Non periodic vehicle diagnostic request
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_DiagnosticMessageID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function DiagnosticMessage.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_DiagnosticMessageID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ScrollableMessage.
   *
   * Creates a full screen overlay containing a large block of formatted text that can be scrolled with up to 8 SoftButtons defined
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ScrollableMessageID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ScrollableMessage.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ScrollableMessageID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Slider.
   *
   * Creates a full screen or pop-up overlay (depending on platform) with a single user controlled slider.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SliderID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Slider.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SliderID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ShowConstantTBT.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ShowConstantTBTID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ShowConstantTBT.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ShowConstantTBTID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function AlertManeuver.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_AlertManeuverID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function AlertManeuver.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_AlertManeuverID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UpdateTurnList.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UpdateTurnListID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UpdateTurnList.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UpdateTurnListID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ChangeRegistration.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ChangeRegistrationID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ChangeRegistration.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ChangeRegistrationID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function GenericResponse.
   *
   * 
   *     	Generic Response is sent, when the name of a received msg cannot be retrieved. Only used in case of an error.
   * 		Currently, only resultCode INVALID_DATA is used.
   * 	
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_GenericResponseID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function PutFile.
   *
   * 
   *     	Used to push a binary data onto the SYNC module from a mobile device, such as icons and album art
   *     	Not supported on first generation SYNC vehicles.
   *     	Binary data is in binary part of hybrid msg.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_PutFileID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function PutFile.
   *
   * Response is sent, when the file data was copied (success case). Or when an error occured.
   * Not supported on First generation SYNC vehicles. 
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_PutFileID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function DeleteFile.
   *
   * 
   *     	Used to delete a file resident on the SYNC module in the app's local cache.
   *     	Not supported on first generation SYNC vehicles.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_DeleteFileID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function DeleteFile.
   *
   * 
   *     	Response is sent, when the file data was deleted (success case). Or when an error occured.
   *     	Not supported on First generation SYNC vehicles.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_DeleteFileID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ListFiles.
   *
   * 
   *     	Requests the current list of resident filenames for the registered app.
   *     	Not supported on first generation SYNC vehicles.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ListFilesID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function ListFiles.
   *
   * 
   *     	Returns the current list of resident filenames for the registered app along with the current space available
   *     	Not supported on First generation SYNC vehicles.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_ListFilesID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SetAppIcon.
   *
   * 
   *     	Used to set existing local file on SYNC as the app's icon
   *     	Not supported on first generation SYNC vehicles.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SetAppIconID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SetAppIcon.
   *
   * 
   *     	Response is sent, when the file data was copied (success case). Or when an error occured.
   *     	Not supported on First generation SYNC vehicles.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SetAppIconID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SetDisplayLayout.
   *
   * 
   *     	Used to set an alternate display layout.
   *     	If not sent, default screen for given platform will be shown
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SetDisplayLayoutID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnHashChange.
   *
   * 
   *               Notification containing an updated hashID which can be used over connection cycles (i.e. loss of connection, ignition cycles, etc.).
   *               Sent after initial registration and subsequently after any change in the calculated hash of all persisted app data.
   *       
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnHashChangeID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SetDisplayLayout.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SetDisplayLayoutID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SystemRequest.
   *
   * An asynchronous request from the device; binary data can be included in hybrid part of message for some requests (such as Authentication requests)
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SystemRequestID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SystemRequest.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SystemRequestID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnHMIStatus.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnHMIStatusID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnAppInterfaceUnregistered.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnAppInterfaceUnregisteredID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnButtonEvent.
   *
   * Notifies application of UP/DOWN events for buttons to which the application is subscribed.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnButtonEventID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnButtonPress.
   *
   * Notifies application of LONG/SHORT press events for buttons to which the application is subscribed.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnButtonPressID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnVehicleData.
   *
   * Callback for the periodic and non periodic vehicle data read function.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnVehicleDataID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnCommand.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnCommandID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnTBTClientState.
   *
   * Provides applications with notifications specific to the current TBT client status on the module
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnTBTClientStateID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnDriverDistraction.
   *
   * Provides driver distraction state to mobile applications
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnDriverDistractionID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnPermissionsChange.
   *
   * Provides update to app of which policy-table-enabled functions are available
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnPermissionsChangeID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnAudioPassThru.
   *
   * Binary data is in binary part of hybrid msg
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnAudioPassThruID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnLanguageChange.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnLanguageChangeID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnKeyboardInput.
   *
   * On-screen keyboard event.
   * Can be full string or individual keypresses depending on keyboard mode.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnKeyboardInputID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnTouchEvent.
   *
   * Notifies about touch events on the screen's prescribed area
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnTouchEventID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnSystemRequest.
   *
   *  An asynchronous request from the system for specific data from the device or the cloud or response to a request from the device or cloud Binary data can be included in hybrid part of message for some requests (such as Authentication request responses)
   *       
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnSystemRequestID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function EncodedSyncPData.
   *
   * 
   *     	Allows encoded data in the form of SyncP packets to be sent to the SYNC module.
   *     	Legacy / v1 Protocol implementation; use SyncPData instead.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_EncodedSyncPDataID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function EncodedSyncPData.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_EncodedSyncPDataID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SyncPData.
   *
   * 
   *     	Allows binary data in the form of SyncP packets to be sent to the SYNC module.
   *     	Binary data is in binary part of hybrid msg.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SyncPDataID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SyncPData.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SyncPDataID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnEncodedSyncPData.
   *
   * 
   *     	Callback including encoded data of any SyncP packets that SYNC needs to send back to the mobile device.
   *     	Legacy / v1 Protocol implementation; responds to EncodedSyncPData.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnEncodedSyncPDataID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function OnSyncPData.
   *
   * 
   *     	Callback including binary data of any SyncP packets that SYNC needs to send back to the mobile device.
   *     	Responds to SyncPData.
   *     	Binary data is in binary part of hybrid msg.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_OnSyncPDataID_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema item for structure AudioPassThruCapabilities.
   *
   * Describes different audio type configurations for PerformAudioPassThru.
   * e.g. {8kHz,8-bit,PCM}
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_AudioPassThruCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Image.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Image(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure SoftButton.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_SoftButton(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Choice.
   *
   * A choice is an option given to the user, which can be selected either by menu, or through voice recognition system.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Choice(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure VrHelpItem.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_VrHelpItem(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure SyncMsgVersion.
   *
   * Specifies the version number of the SYNC V4 protocol, that is supported by the mobile application
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_SyncMsgVersion(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure SingleTireStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_SingleTireStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure BeltStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_BeltStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure BodyInformation.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_BodyInformation(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure DeviceStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_DeviceStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure HeadLampStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_HeadLampStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure engineInfo.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_engineInfo(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure DeviceInfo.
   *
   * Various information abount connecting device.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_DeviceInfo(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure ECallInfo.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_ECallInfo(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure AirbagStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_AirbagStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure EmergencyEvent.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_EmergencyEvent(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure ClusterModeStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_ClusterModeStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure MyKey.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_MyKey(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure TireStatus.
   *
   * The status and pressure of the tires.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_TireStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure GPSData.
   *
   * Struct with the GPS data.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_GPSData(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure VehicleDataResult.
   *
   * Individual published data request result
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_VehicleDataResult(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure DIDResult.
   *
   * Individual requested DID result and data
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_DIDResult(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure StartTime.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_StartTime(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure TextField.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_TextField(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure ImageResolution.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_ImageResolution(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure ImageField.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_ImageField(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure TouchCoord.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_TouchCoord(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure TouchEvent.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_TouchEvent(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Coordinate.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Coordinate(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure TouchArea.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_TouchArea(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure TouchEventCapabilities.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_TouchEventCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure ScreenParams.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_ScreenParams(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure HMIPermissions.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_HMIPermissions(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure ParameterPermissions.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_ParameterPermissions(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure PermissionItem.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_PermissionItem(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure DisplayCapabilities.
   *
   * Contains information about the display capabilities.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_DisplayCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure ButtonCapabilities.
   *
   * Contains information about a button's capabilities.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_ButtonCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure SoftButtonCapabilities.
   *
   * Contains information about a SoftButton's capabilities.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_SoftButtonCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure PresetBankCapabilities.
   *
   * Contains information about on-screen preset capabilities.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_PresetBankCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure MenuParams.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_MenuParams(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure TTSChunk.
   *
   * A TTS chunk, that consists of the text/phonemes to speak and the type (like text or SAPI)
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_TTSChunk(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Turn.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Turn(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure VehicleType.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_VehicleType(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure KeyboardProperties.
   *
   * Configuration of on-screen keyboard (if available).
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_KeyboardProperties(
      const TStructsSchemaItems &struct_schema_items);
};

} // mobile_apis

#endif //__CSMARTFACTORY_MOBILE_API_8279afa8cba511e3a201902b34794c53_HPP__

