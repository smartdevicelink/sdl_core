/**
 * @file HMI_API.hpp
 * @brief Generated class HMI_API header file.
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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __CSMARTFACTORY_HMI_API_6f31bf4ecba511e3a275902b34794c53_HPP__
#define __CSMARTFACTORY_HMI_API_6f31bf4ecba511e3a275902b34794c53_HPP__

#include "formatters/CSmartFactory.hpp"
#include "smart_objects/smart_schema.h"
#include "smart_objects/schema_item.h"
#include "utils/shared_ptr.h"
#include "HMI_API.h"

namespace hmi_apis {


/**
 * @brief Class HMI_API.
 *
 * Params:
 *     SDL_date - 2014-03-12
 *     Navigation_version - 1.0
 *     TTS_date - 2013-04-18
 *     Common_date - 2013-10-02
 *     Buttons_date - 2013-04-12
 *     name - SmartDeviceLink HMI API
 *     VehicleInfo_date - 2013-04-21
 *     SDL_version - 1.0
 *     TTS_version - 1.0
 *     VR_date - 2013-04-17
 *     VehicleInfo_version - 1.0
 *     BasicCommunication_date - 2013-04-12
 *     Common_version - 1.1
 *     UI_date - 2013-04-16
 *     Buttons_version - 1.0
 *     UI_version - 1.0
 *     TTS_description - RPCs for communication between TTS and SDL.
 *     BasicCommunication_version - 1.0
 *     Navigation_date - 2013-05-22
 *     VR_version - 1.0
 */
class HMI_API : public NsSmartDeviceLink::NsJSONHandler::CSmartFactory<FunctionID::eType, messageType::eType, StructIdentifiers::eType> {
 public:
  /**
   * @brief Constructor.
   */
  HMI_API();

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
   * @brief Method that generates schema for function Buttons_GetCapabilities.
   *
   * Method is invoked at system start-up. SDL requests the information about all supported hardware buttons and their capabilities
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Buttons_GetCapabilities_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Buttons_GetCapabilities.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Buttons_GetCapabilities_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Buttons_OnButtonEvent.
   *
   * HU system must notify about every UP/DOWN event for buttons
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Buttons_OnButtonEvent_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Buttons_OnButtonPress.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Buttons_OnButtonPress_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnReady.
   *
   * HMI must notify SDL about its readiness to start communication. In fact, this has to be the first message between SDL and HMI.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnReady_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnStartDeviceDiscovery.
   *
   * Initiated by HMI user for getting the list of connected devices.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnStartDeviceDiscovery_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnResumeAudioSource.
   *
   * This method must be invoked by SDL to update audio state.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnResumeAudioSource_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_UpdateDeviceList.
   *
   * Request from SmartDeviceLink to HMI to change device list.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_UpdateDeviceList_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_UpdateDeviceList.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_UpdateDeviceList_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnFileRemoved.
   *
   * Notifies HMI in case some application file has been removed
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnFileRemoved_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_AllowDeviceToConnect.
   *
   * Request from SmartDeviceLink to HMI to get the permissions of new device connection.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_AllowDeviceToConnect_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_AllowDeviceToConnect.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_AllowDeviceToConnect_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnDeviceChosen.
   *
   * Notification must be initiated by HMI on user selecting device in the list of devices.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnDeviceChosen_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnFindApplications.
   *
   * This method must be invoked by HMI to get list of registered apps.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnFindApplications_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_ActivateApp.
   *
   * Request from SDL to HMI to bring specified application to front on UI e.g make it HMI status 'FULL'.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_ActivateApp_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_ActivateApp.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_ActivateApp_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnAppActivated.
   *
   * Must be sent by HU system when the user clicks on app in the list of registered apps or on soft button with 'STEAL_FOCUS' action.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnAppActivated_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnAppDeactivated.
   *
   * Must be sent by HU system when the user switches to any functionality which is not other mobile application.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnAppDeactivated_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnAppRegistered.
   *
   * Issued by SDL to notify HMI about new application registered.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnAppRegistered_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnAppUnregistered.
   *
   * Issued by SDL to notify HMI about application unregistered. Application then to be removed from application list; all data connected with application has to be cleared up.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnAppUnregistered_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnExitApplication.
   *
   * Must be sent by HMI when the User chooses to exit the application..
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnExitApplication_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnExitAllApplications.
   *
   * Sent by HMI to SDL to close all registered applications.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnExitAllApplications_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_MixingAudioSupported.
   *
   * Request from SDL to HMI to find out if the last one supports mixing audio (i.e. recording TTS command and playing audio).
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_MixingAudioSupported_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_MixingAudioSupported.
   *
   * If no response received SDL supposes that mixing audio is not supported
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_MixingAudioSupported_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_PlayTone.
   *
   * Sent by SDL to HMI to notify that the tone should be played.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_PlayTone_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnSystemRequest.
   *
   * 
   *         An asynchronous request from the system for specific data from the device or the cloud or response to a request from the device or cloud
   *         Binary data can be included in hybrid part of message for some requests (such as Authentication request responses)
   *       
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnSystemRequest_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_SystemRequest.
   *
   * An asynchronous request from the device; binary data can be included in hybrid part of message for some requests (such as Authentication requests)
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_SystemRequest_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_SystemRequest.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_SystemRequest_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_PolicyUpdate.
   *
   * 
   *         From: SDL
   *         To: SYNCP Manager
   *         Asks SYNCP Manager to send given Policy Table snapshot to the backend
   *       
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_PolicyUpdate_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_PolicyUpdate.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_PolicyUpdate_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnSDLClose.
   *
   * SDL must notify HMI about finish working application
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnSDLClose_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnPutFile.
   *
   * 
   *       Notification, that comes to HMI when mobile sents file with systemFile flag
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnPutFile_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_GetSystemInfo.
   *
   * Request from SDL to HMI to obtain information about head unit system.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_GetSystemInfo_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_GetSystemInfo.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_GetSystemInfo_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnSystemInfoChanged.
   *
   * Issued by system to SDL to notify that some system param has changed. Currently applied for Sync Language.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnSystemInfoChanged_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function BasicCommunication_OnIgnitionCycleOver.
   *
   * Notification from system to SDL to let it know that ignition cycle is over.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_BasicCommunication_OnIgnitionCycleOver_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_IsReady.
   *
   * Method is invoked at system startup. Response provides information about presence of VR module and its readiness to cooperate with SDL.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_IsReady_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_IsReady.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_IsReady_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_Started.
   *
   * Must be initiated  by VR module to let SDL know that VR session has started.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_Started_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_Stopped.
   *
   * Must be initiated by VR module to let SDL know that VR session has stopped.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_Stopped_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_AddCommand.
   *
   * Request from SDL to add a command(string with associated id) to VR.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_AddCommand_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_AddCommand.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_AddCommand_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_DeleteCommand.
   *
   * Request from SDL to delete a command from VR.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_DeleteCommand_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_DeleteCommand.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_DeleteCommand_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_PerformInteraction.
   *
   * 
   *       Sets some properties for the application initiated request.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_PerformInteraction_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_PerformInteraction.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_PerformInteraction_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_OnCommand.
   *
   * Notifies SDL about command trigerred via VR
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_OnCommand_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_ChangeRegistration.
   *
   * Request from SmartDeviceLink to HMI to change language of VR.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_ChangeRegistration_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_ChangeRegistration.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_ChangeRegistration_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_OnLanguageChange.
   *
   * Notification from HMI to SmartDeviceLink about change of  language.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_OnLanguageChange_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_GetSupportedLanguages.
   *
   * Method is invoked at system start-up. Response must provide the information about VR supported languages.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_GetSupportedLanguages_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_GetSupportedLanguages.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_GetSupportedLanguages_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_GetLanguage.
   *
   * Request from SmartDeviceLink to HMI to get currently active  VR language
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_GetLanguage_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_GetLanguage.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_GetLanguage_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_GetCapabilities.
   *
   * Method is invoked at system startup by SDL to request information about VR capabilities of HMI.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_GetCapabilities_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VR_GetCapabilities.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VR_GetCapabilities_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_GetCapabilities.
   *
   * Method is invoked at system start-up. SDL requests the information about all supported hardware and their capabilities
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_GetCapabilities_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_GetCapabilities.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_GetCapabilities_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_Started.
   *
   * Must be initiated  by TTS module to let SDL know that TTS session has started.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_Started_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_Stopped.
   *
   * Must be initiated by TTS module to let SDL know that TTS session has stopped.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_Stopped_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_IsReady.
   *
   * Method is invoked at system start-up. Response must provide the information about presence of TTS module and its readiness to cooperate with SDL.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_IsReady_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_IsReady.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_IsReady_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_Speak.
   *
   * RPC from SDL to TTS for speaking the text.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_Speak_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_Speak.
   *
   * Provides information about success of operation.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_Speak_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_StopSpeaking.
   *
   * Initiated by SDL to stop speaking the text.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_StopSpeaking_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_StopSpeaking.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_StopSpeaking_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_ChangeRegistration.
   *
   * Request from SmartDeviceLink to HMI to change language of TTS.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_ChangeRegistration_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_ChangeRegistration.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_ChangeRegistration_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_OnLanguageChange.
   *
   * Notification from HMI to SmartDeviceLink about change of  language.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_OnLanguageChange_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_GetSupportedLanguages.
   *
   * Method is invoked at system start-up by SDL. Response must provide the information about TTS supported languages.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_GetSupportedLanguages_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_GetSupportedLanguages.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_GetSupportedLanguages_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_GetLanguage.
   *
   * Request from SmartDeviceLink to HMI to get currently active  TTS language
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_GetLanguage_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_GetLanguage.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_GetLanguage_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_SetGlobalProperties.
   *
   * Sets some properties for the application initiated request.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_SetGlobalProperties_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function TTS_SetGlobalProperties.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_TTS_SetGlobalProperties_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_Alert.
   *
   * Request from SDL to show an alert message on the display.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_Alert_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_Alert.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_Alert_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_Show.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_Show_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_Show.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_Show_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_AddCommand.
   *
   * Request from SDL to add a command to the application menu.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_AddCommand_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_AddCommand.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_AddCommand_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_DeleteCommand.
   *
   * Request from SDL to delete a command from the in-application menu with the specified command id.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_DeleteCommand_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_DeleteCommand.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_DeleteCommand_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_AddSubMenu.
   *
   * Request from SDL to add a sub menu to the in-application menu.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_AddSubMenu_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_AddSubMenu.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_AddSubMenu_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_DeleteSubMenu.
   *
   * Request from SDL to delete a submenu from the in-application menu.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_DeleteSubMenu_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_DeleteSubMenu.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_DeleteSubMenu_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_PerformInteraction.
   *
   * Request from SDL for triggering an interaction (e.g. "Permit GPS?" - Yes, no, Always Allow).
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_PerformInteraction_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_PerformInteraction.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_PerformInteraction_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_SetMediaClockTimer.
   *
   * Sets the initial media clock value and automatic update method.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_SetMediaClockTimer_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_SetMediaClockTimer.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_SetMediaClockTimer_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_SetGlobalProperties.
   *
   * Request from SDL to set some properties for VR help.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_SetGlobalProperties_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_SetGlobalProperties.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_SetGlobalProperties_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_OnCommand.
   *
   * Notification must be initiated by HMI on user choosing menu item.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_OnCommand_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_OnSystemContext.
   *
   * 
   *       Notification must be initiated by HMI when the user changes the context of application: goes to menu (in-application menu or system menu);
   *       switches to VR; pop-up appears on screen etc.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_OnSystemContext_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_GetCapabilities.
   *
   * Method is invoked at system startup by SDL to request information about UI capabilities of HMI.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_GetCapabilities_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_GetCapabilities.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_GetCapabilities_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_ChangeRegistration.
   *
   * Request from SmartDeviceLink to HMI to change language for app.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_ChangeRegistration_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_ChangeRegistration.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_ChangeRegistration_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_OnLanguageChange.
   *
   * Notification from HMI to SmartDeviceLink about change of  language.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_OnLanguageChange_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_GetSupportedLanguages.
   *
   * Method should be invoked at system startup. Response provides information about UI supported languages.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_GetSupportedLanguages_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_GetSupportedLanguages.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_GetSupportedLanguages_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_GetLanguage.
   *
   * Request from SmartDeviceLink to HMI to get currently active  UI language
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_GetLanguage_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_GetLanguage.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_GetLanguage_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_OnDriverDistraction.
   *
   * Notification must be sent from HMI to SDL when driver distraction state is changed. Driver distraction rules are defined by the platform.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_OnDriverDistraction_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_SetAppIcon.
   *
   * Used to set existing local file on SYNC as the app's icon.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_SetAppIcon_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_SetAppIcon.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_SetAppIcon_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_SetDisplayLayout.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_SetDisplayLayout_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_SetDisplayLayout.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_SetDisplayLayout_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_ShowCustomForm.
   *
   * Used to show a custom form; it can be a parent or child screen. If no parent screen is designated, it is set as a parent screen.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_ShowCustomForm_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_ShowCustomForm.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_ShowCustomForm_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_OnKeyboardInput.
   *
   * On-screen keyboard event.
   * Can be full string or individual keypresses depending on keyboard mode.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_OnKeyboardInput_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_OnTouchEvent.
   *
   * Notifies about touch events on the screen's prescribed area
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_OnTouchEvent_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_Slider.
   *
   * Creates a full screen or pop-up overlay (depending on platform) with a single user controlled slider.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_Slider_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_Slider.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_Slider_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_ScrollableMessage.
   *
   * Creates a full screen overlay containing a large block of formatted text that can be scrolled with up to 8 SoftButtons defined
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_ScrollableMessage_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_ScrollableMessage.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_ScrollableMessage_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_PerformAudioPassThru.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_PerformAudioPassThru_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_PerformAudioPassThru.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_PerformAudioPassThru_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_EndAudioPassThru.
   *
   * Request is sent by SDL to stop the audio capturing.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_EndAudioPassThru_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_EndAudioPassThru.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_EndAudioPassThru_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_IsReady.
   *
   * Method is invoked at system startup. Response provides information about presence of UI module and its readiness to cooperate with SDL.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_IsReady_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_IsReady.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_IsReady_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_ClosePopUp.
   *
   * Initiated by SDL to close currently active pop-up on HMI.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_ClosePopUp_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_ClosePopUp.
   *
   * Provides the result of operation.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_ClosePopUp_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_OnResetTimeout.
   *
   * HMI must provide SDL with notifications specific to the current Turn-By-Turn client status on the module
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_OnResetTimeout_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function UI_OnRecordStart.
   *
   * Issued by SDL to notify HMI about capturing mic data should be started
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_UI_OnRecordStart_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_IsReady.
   *
   * Method is invoked at system startup. Response must provide the information about presence of UI Navigation module and its readiness to cooperate with SDL.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_IsReady_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_IsReady.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_IsReady_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_ShowConstantTBT.
   *
   * Request from SmartDeviceLinkCore to HMI to show info about navigation.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_ShowConstantTBT_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_ShowConstantTBT.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_ShowConstantTBT_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_AlertManeuver.
   *
   * Request from SmartDeviceLinkCore to HMI to announce navigation maneuver
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_AlertManeuver_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_AlertManeuver.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_AlertManeuver_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_UpdateTurnList.
   *
   * Request from SmartDeviceLinkCore to HMI to update turn list.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_UpdateTurnList_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_UpdateTurnList.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_UpdateTurnList_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_OnTBTClientState.
   *
   * HMI must provide SDL with notifications specific to the current Turn-By-Turn client status on the module
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_OnTBTClientState_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_StartStream.
   *
   * Notification from SmartDeviceLinkCore to HMI to start playing video streaming.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_StartStream_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_StartStream.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_StartStream_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_StopStream.
   *
   * Notification from SmartDeviceLinkCore to HMI to start playing video streaming.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_StopStream_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_StopStream.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_StopStream_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_StartAudioStream.
   *
   * Notification from SmartDeviceLinkCore to HMI to start playing audio streaming.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_StartAudioStream_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_StartAudioStream.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_StartAudioStream_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_StopAudioStream.
   *
   * Notification from SmartDeviceLinkCore to HMI to start playing audio streaming.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_StopAudioStream_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function Navigation_StopAudioStream.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_Navigation_StopAudioStream_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_IsReady.
   *
   * Method is invoked at system startup. Response should provide information about presence of any of vehicle information modules (ECU, GPS, etc) and their readiness to cooperate with SDL.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_IsReady_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_IsReady.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_IsReady_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_GetVehicleType.
   *
   * Request from SmartDeviceLinkCore to HMI to get info about the vehicle (type, model, etc.).
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_GetVehicleType_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_GetVehicleType.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_GetVehicleType_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_ReadDID.
   *
   * Request from SDL for vehicle data reading.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_ReadDID_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_ReadDID.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_ReadDID_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_GetDTCs.
   *
   * Vehicle module diagnostic trouble code request.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_GetDTCs_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_GetDTCs.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_GetDTCs_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_DiagnosticMessage.
   *
   * Non periodic vehicle diagnostic request
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_DiagnosticMessage_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_DiagnosticMessage.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_DiagnosticMessage_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_SubscribeVehicleData.
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
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_SubscribeVehicleData_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_SubscribeVehicleData.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_SubscribeVehicleData_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_UnsubscribeVehicleData.
   *
   * 
   *       This function is used to unsubscribe the notifications from the subscribeVehicleData function.
   *     
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_UnsubscribeVehicleData_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_UnsubscribeVehicleData.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_UnsubscribeVehicleData_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_GetVehicleData.
   *
   * Non periodic vehicle data read request.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_GetVehicleData_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_GetVehicleData.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_GetVehicleData_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function VehicleInfo_OnVehicleData.
   *
   * Callback for the periodic and non periodic vehicle data read function.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_VehicleInfo_OnVehicleData_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_ActivateApp.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_ActivateApp_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_ActivateApp.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_ActivateApp_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_GetUserFriendlyMessage.
   *
   * Request from HMI to SDL to get user friendly messages for UI/TTS info boxes/texts (i.e. for help/dialogs etc) from Policy Table.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_GetUserFriendlyMessage_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_GetUserFriendlyMessage.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_GetUserFriendlyMessage_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_OnAllowSDLFunctionality.
   *
   * Initiated by HMI. Notifies about user/HMI allowing SDL functionality or disallowing access to all mobile apps. Needed if HMI has additional ways of asking user about this (i.e. Settings etc)
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_OnAllowSDLFunctionality_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_OnReceivedPolicyUpdate.
   *
   * 
   *         From: SYNCP Manager
   *         To: SDL
   *         Notification sent to SDL when SYNCP Manager receives and decrypts updated policy table
   *       
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_OnReceivedPolicyUpdate_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_OnPolicyUpdate.
   *
   * 
   *         From: SYNCP Manager
   *         To: SDL
   *         Notifies SDL to supply a new "PolicyUpdate" request with more recent snapshot data
   *       
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_OnPolicyUpdate_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_GetListOfPermissions.
   *
   * Request from HMI to SDL to get list of permissions for app. If no app specified - list of permissions for all apps.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_GetListOfPermissions_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_GetListOfPermissions.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_GetListOfPermissions_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_OnAppPermissionConsent.
   *
   * Initiated by HMI for specifying the allowance for the application to perform some functionality. Duplicates functionality of the request, needed if HMI has specific ways to allow/disallow functionality (i.e.Setting Menu)
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_OnAppPermissionConsent_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_OnAppPermissionChanged.
   *
   * Notification from SDL to HMI. Occurs when app permissions were reduced. If no permission specified means that app was dissallowed and has to be unregitstered.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_OnAppPermissionChanged_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_OnSDLConsentNeeded.
   *
   * Send from SDL to HMI to notify that data consent is needed for device either because PT update or Retry strategy.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_OnSDLConsentNeeded_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_UpdateSDL.
   *
   * Request from HMI to SDL to start update of Policy Table.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_UpdateSDL_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_UpdateSDL.
   *
   * Specify result: no update needed, update was successful/unsuccessful etc
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_UpdateSDL_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_GetStatusUpdate.
   *
   * Request from HMI to SDL to find out current status of PT exchange process.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_GetStatusUpdate_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_GetStatusUpdate.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_GetStatusUpdate_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_OnStatusUpdate.
   *
   * Notification from SDL to HMI when current status of PT exchange changed (i.e. it Succeded or Failed etc)
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_OnStatusUpdate_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_OnSystemError.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_OnSystemError_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_AddStatisticsInfo.
   *
   * Sent by system to record statiscs and error counts. Increases statistics specified by statisticType by one.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_AddStatisticsInfo_notification(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_GetURLS.
   *
   * Sent by system to SDL to get list of URL for specified service type and optionally policy's application id.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_GetURLS_request(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema for function SDL_GetURLS.
   *
   * @return NsSmartDeviceLink::NsSmartObjects::CSmartSchema
   */
  static NsSmartDeviceLink::NsSmartObjects::CSmartSchema InitFunction_SDL_GetURLS_response(
      const TStructsSchemaItems &struct_schema_items,
      const std::set<FunctionID::eType> &function_id_items,
      const std::set<messageType::eType> &message_type_items);

  /**
   * @brief Method that generates schema item for structure Common_UserFriendlyMessage.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_UserFriendlyMessage(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_PermissionItem.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_PermissionItem(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_ServiceInfo.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_ServiceInfo(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_TextField.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_TextField(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_ButtonCapabilities.
   *
   * Contains the information about capabilities of a button.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_ButtonCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_VehicleDataResult.
   *
   * Individual published data request result
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_VehicleDataResult(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_TouchCoord.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_TouchCoord(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_TouchEvent.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_TouchEvent(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_PresetBankCapabilities.
   *
   * Contains information about on-screen preset capabilities (whether the HW preset buttons could be duplicated with onscreen buttons).
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_PresetBankCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_Image.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_Image(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_SoftButton.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_SoftButton(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_TTSChunk.
   *
   * A TTS chunk, that consists of the text/phonemes to be spoken
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_TTSChunk(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_HMIApplication.
   *
   * Data type containing information about application needed by HMI.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_HMIApplication(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_MenuParams.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_MenuParams(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_Choice.
   *
   * A choice is an option given to the user which can be selected either by menu, or through voice recognition system.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_Choice(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_VrHelpItem.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_VrHelpItem(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_TimeFormat.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_TimeFormat(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_TouchEventCapabilities.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_TouchEventCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_ImageResolution.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_ImageResolution(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_ScreenParams.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_ScreenParams(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_ImageField.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_ImageField(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_DisplayCapabilities.
   *
   * Contains information about the display capabilities.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_DisplayCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_SoftButtonCapabilities.
   *
   * Contains information about a SoftButton's capabilities.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_SoftButtonCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_AudioPassThruCapabilities.
   *
   * 
   *     Describes different audio type configurations for PerformAudioPassThru.
   *     e.g. 8kHz,8-bit,PCM
   *   
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_AudioPassThruCapabilities(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_Coordinate.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_Coordinate(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_TextFieldStruct.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_TextFieldStruct(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_KeyboardProperties.
   *
   * Configuration of on-screen keyboard (if available).
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_KeyboardProperties(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_Turn.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_Turn(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_VehicleType.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_VehicleType(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_DeviceInfo.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_DeviceInfo(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_GPSData.
   *
   * Struct with the GPS data.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_GPSData(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_SingleTireStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_SingleTireStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_DIDResult.
   *
   * Individual requested DID result and data
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_DIDResult(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_HeadLampStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_HeadLampStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_TireStatus.
   *
   * The status and pressure of the tires.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_TireStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_BeltStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_BeltStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_BodyInformation.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_BodyInformation(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_DeviceStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_DeviceStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_ECallInfo.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_ECallInfo(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_AirbagStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_AirbagStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_EmergencyEvent.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_EmergencyEvent(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_ClusterModeStatus.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_ClusterModeStatus(
      const TStructsSchemaItems &struct_schema_items);

  /**
   * @brief Method that generates schema item for structure Common_MyKey.
   */
  static utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> InitStructSchemaItem_Common_MyKey(
      const TStructsSchemaItems &struct_schema_items);
};

} // hmi_apis

#endif //__CSMARTFACTORY_HMI_API_6f31bf4ecba511e3a275902b34794c53_HPP__

