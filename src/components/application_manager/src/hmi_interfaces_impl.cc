/*
 * Copyright (c) 2016, Ford Motor Company
 * All rights reserved.
 *
 * Copyright (c) 2017 Xevo Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the copyright holders nor the names of their contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/hmi_interfaces_impl.h"
namespace application_manager {

std::map<hmi_apis::FunctionID::eType, HmiInterfaces::InterfaceID>
generate_function_to_interface_convert_map() {
  using namespace hmi_apis::FunctionID;
  std::map<hmi_apis::FunctionID::eType, HmiInterfaces::InterfaceID> convert_map;
  convert_map[Buttons_GetCapabilities] = HmiInterfaces::HMI_INTERFACE_Buttons;
  convert_map[Buttons_OnButtonEvent] = HmiInterfaces::HMI_INTERFACE_Buttons;
  convert_map[Buttons_OnButtonPress] = HmiInterfaces::HMI_INTERFACE_Buttons;
  convert_map[Buttons_OnButtonSubscription] =
      HmiInterfaces::HMI_INTERFACE_Buttons;
  convert_map[BasicCommunication_OnReady] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnStartDeviceDiscovery] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnUpdateDeviceList] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnResumeAudioSource] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnSDLPersistenceComplete] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_UpdateAppList] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_UpdateDeviceList] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnFileRemoved] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnDeviceChosen] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnFindApplications] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_ActivateApp] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnAppActivated] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnAppDeactivated] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnAppRegistered] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnAppUnregistered] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnExitApplication] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnExitAllApplications] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnAwakeSDL] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_MixingAudioSupported] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_DialNumber] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnSystemRequest] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_SystemRequest] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_PolicyUpdate] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnSDLClose] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnPutFile] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_GetSystemInfo] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnSystemInfoChanged] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnIgnitionCycleOver] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_OnEventChanged] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[BasicCommunication_GetFilePath] =
      HmiInterfaces::HMI_INTERFACE_BasicCommunication;
  convert_map[VR_IsReady] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[VR_Started] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[VR_Stopped] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[VR_AddCommand] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[VR_DeleteCommand] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[VR_PerformInteraction] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[VR_OnCommand] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[VR_ChangeRegistration] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[VR_OnLanguageChange] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[VR_GetSupportedLanguages] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[VR_GetLanguage] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[VR_GetCapabilities] = HmiInterfaces::HMI_INTERFACE_VR;
  convert_map[TTS_GetCapabilities] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[TTS_Started] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[TTS_Stopped] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[TTS_IsReady] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[TTS_Speak] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[TTS_StopSpeaking] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[TTS_ChangeRegistration] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[TTS_OnLanguageChange] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[TTS_GetSupportedLanguages] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[TTS_GetLanguage] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[TTS_SetGlobalProperties] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[TTS_OnResetTimeout] = HmiInterfaces::HMI_INTERFACE_TTS;
  convert_map[UI_Alert] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_Show] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_AddCommand] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_DeleteCommand] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_AddSubMenu] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_DeleteSubMenu] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_PerformInteraction] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_SetMediaClockTimer] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_SetGlobalProperties] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_OnCommand] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_OnSystemContext] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_GetCapabilities] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_ChangeRegistration] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_OnLanguageChange] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_GetSupportedLanguages] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_GetLanguage] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_OnDriverDistraction] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_SetAppIcon] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_SetDisplayLayout] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_ShowCustomForm] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_OnKeyboardInput] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_OnTouchEvent] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_Slider] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_ScrollableMessage] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_PerformAudioPassThru] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_EndAudioPassThru] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_IsReady] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_ClosePopUp] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_OnResetTimeout] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[UI_SendHapticData] = HmiInterfaces::HMI_INTERFACE_UI;
  convert_map[Navigation_IsReady] = HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_SendLocation] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_ShowConstantTBT] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_AlertManeuver] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_UpdateTurnList] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_OnTBTClientState] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_SetVideoConfig] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_StartStream] = HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_StopStream] = HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_StartAudioStream] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_StopAudioStream] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_OnAudioDataStreaming] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_OnVideoDataStreaming] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_GetWayPoints] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_OnWayPointChange] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_SubscribeWayPoints] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[Navigation_UnsubscribeWayPoints] =
      HmiInterfaces::HMI_INTERFACE_Navigation;
  convert_map[VehicleInfo_IsReady] = HmiInterfaces::HMI_INTERFACE_VehicleInfo;
  convert_map[VehicleInfo_GetVehicleType] =
      HmiInterfaces::HMI_INTERFACE_VehicleInfo;
  convert_map[VehicleInfo_ReadDID] = HmiInterfaces::HMI_INTERFACE_VehicleInfo;
  convert_map[VehicleInfo_GetDTCs] = HmiInterfaces::HMI_INTERFACE_VehicleInfo;
  convert_map[VehicleInfo_DiagnosticMessage] =
      HmiInterfaces::HMI_INTERFACE_VehicleInfo;
  convert_map[VehicleInfo_SubscribeVehicleData] =
      HmiInterfaces::HMI_INTERFACE_VehicleInfo;
  convert_map[VehicleInfo_UnsubscribeVehicleData] =
      HmiInterfaces::HMI_INTERFACE_VehicleInfo;
  convert_map[VehicleInfo_GetVehicleData] =
      HmiInterfaces::HMI_INTERFACE_VehicleInfo;
  convert_map[VehicleInfo_OnVehicleData] =
      HmiInterfaces::HMI_INTERFACE_VehicleInfo;
  convert_map[SDL_ActivateApp] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_GetUserFriendlyMessage] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_OnAllowSDLFunctionality] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_OnReceivedPolicyUpdate] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_OnPolicyUpdate] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_GetListOfPermissions] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_OnAppPermissionConsent] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_OnAppPermissionChanged] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_OnSDLConsentNeeded] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_UpdateSDL] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_GetStatusUpdate] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_OnStatusUpdate] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_OnSystemError] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_AddStatisticsInfo] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[SDL_GetURLS] = HmiInterfaces::HMI_INTERFACE_SDL;
  convert_map[RC_IsReady] = HmiInterfaces::HMI_INTERFACE_RC;
  convert_map[RC_GetCapabilities] = HmiInterfaces::HMI_INTERFACE_RC;
  convert_map[Buttons_ButtonPress] = HmiInterfaces::HMI_INTERFACE_RC;
  convert_map[RC_SetInteriorVehicleData] = HmiInterfaces::HMI_INTERFACE_RC;
  convert_map[RC_GetInteriorVehicleData] = HmiInterfaces::HMI_INTERFACE_RC;
  convert_map[RC_GetInteriorVehicleDataConsent] =
      HmiInterfaces::HMI_INTERFACE_RC;
  convert_map[RC_OnInteriorVehicleData] = HmiInterfaces::HMI_INTERFACE_RC;
  convert_map[RC_OnRemoteControlSettings] = HmiInterfaces::HMI_INTERFACE_RC;
  convert_map[AppService_PublishAppService] =
      HmiInterfaces::HMI_INTERFACE_AppService;
  convert_map[AppService_GetAppServiceData] =
      HmiInterfaces::HMI_INTERFACE_AppService;
  convert_map[AppService_PerformAppServiceInteraction] =
      HmiInterfaces::HMI_INTERFACE_AppService;
  convert_map[AppService_GetActiveServiceConsent] =
      HmiInterfaces::HMI_INTERFACE_AppService;
  return convert_map;
}

HmiInterfacesImpl::HmiInterfacesImpl() {
  sync_primitives::AutoLock autolock(interfaces_states_lock_);
  interfaces_states_[HmiInterfaces::HMI_INTERFACE_BasicCommunication] =
      HmiInterfaces::STATE_NOT_RESPONSE;
  interfaces_states_[HmiInterfaces::HMI_INTERFACE_Buttons] =
      HmiInterfaces::STATE_NOT_RESPONSE;
  interfaces_states_[HmiInterfaces::HMI_INTERFACE_Navigation] =
      HmiInterfaces::STATE_NOT_RESPONSE;
  interfaces_states_[HmiInterfaces::HMI_INTERFACE_SDL] =
      HmiInterfaces::STATE_NOT_RESPONSE;
  interfaces_states_[HmiInterfaces::HMI_INTERFACE_TTS] =
      HmiInterfaces::STATE_NOT_RESPONSE;
  interfaces_states_[HmiInterfaces::HMI_INTERFACE_UI] =
      HmiInterfaces::STATE_NOT_RESPONSE;
  interfaces_states_[HmiInterfaces::HMI_INTERFACE_VehicleInfo] =
      HmiInterfaces::STATE_NOT_RESPONSE;
  interfaces_states_[HmiInterfaces::HMI_INTERFACE_VR] =
      HmiInterfaces::STATE_NOT_RESPONSE;
  interfaces_states_[HmiInterfaces::HMI_INTERFACE_RC] =
      HmiInterfaces::STATE_NOT_RESPONSE;
  interfaces_states_[HmiInterfaces::HMI_INTERFACE_AppService] =
      HmiInterfaces::STATE_NOT_RESPONSE;
}

HmiInterfaces::InterfaceState HmiInterfacesImpl::GetInterfaceState(
    HmiInterfaces::InterfaceID interface) const {
  sync_primitives::AutoLock autolock(interfaces_states_lock_);
  const InterfaceStatesMap::const_iterator it =
      interfaces_states_.find(interface);
  // all interfaces should be presented in interfaces_states_ map.
  DCHECK_OR_RETURN(it != interfaces_states_.end(), STATE_NOT_RESPONSE);
  return it->second;
}

void HmiInterfacesImpl::SetInterfaceState(HmiInterfaces::InterfaceID interface,
                                          HmiInterfaces::InterfaceState state) {
  sync_primitives::AutoLock autolock(interfaces_states_lock_);
  DCHECK(interfaces_states_.find(interface) != interfaces_states_.end());
  interfaces_states_[interface] = state;
}

HmiInterfaces::InterfaceID HmiInterfacesImpl::GetInterfaceFromFunction(
    hmi_apis::FunctionID::eType function) const {
  // TODO(AKutsan): APPLINK-26874: Generate map of functionid to inteface
  // automaticaly from
  // HMI_API.xml
  static const std::map<hmi_apis::FunctionID::eType, InterfaceID> convert_map =
      generate_function_to_interface_convert_map();
  const std::map<hmi_apis::FunctionID::eType, InterfaceID>::const_iterator it =
      convert_map.find(function);
  return it != convert_map.end() ? it->second : HMI_INTERFACE_INVALID_ENUM;
}
}  // namespace application_manager
