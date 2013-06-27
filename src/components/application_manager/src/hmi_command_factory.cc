/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/hmi_command_factory.h"


#include "application_manager/message.h"
#include "interfaces/HMI_API.h"

#include "application_manager/commands/hmi/get_device_list_request.h"
#include "application_manager/commands/hmi/get_device_list_response.h"
#include "application_manager/commands/hmi/get_app_list_request.h"
#include "application_manager/commands/hmi/get_app_list_response.h"
#include "application_manager/commands/hmi/mixing_audio_supported_request.h"
#include "application_manager/commands/hmi/mixing_audio_supported_response.h"
#include "application_manager/commands/hmi/exit_all_applications_request.h"
#include "application_manager/commands/hmi/exit_all_applications_response.h"
#include "application_manager/commands/hmi/exit_application_request.h"
#include "application_manager/commands/hmi/exit_application_response.h"
#include "application_manager/commands/hmi/ui_get_supported_languages_request.h"
#include "application_manager/commands/hmi/ui_get_supported_languages_response.h"
#include "application_manager/commands/hmi/ui_get_language_request.h"
#include "application_manager/commands/hmi/ui_get_language_response.h"
#include "application_manager/commands/hmi/ui_get_capabilities_request.h"
#include "application_manager/commands/hmi/ui_get_capabilities_response.h"
#include "application_manager/commands/hmi/ui_change_registration_request.h"
#include "application_manager/commands/hmi/ui_change_registration_response.h"
#include "application_manager/commands/hmi/ui_show_request.h"
#include "application_manager/commands/hmi/ui_show_response.h"
#include "application_manager/commands/hmi/vr_is_ready_request.h"
#include "application_manager/commands/hmi/vr_is_ready_response.h"
#include "application_manager/commands/hmi/vr_add_command_request.h"
#include "application_manager/commands/hmi/vr_add_command_response.h"
#include "application_manager/commands/hmi/vr_delete_command_request.h"
#include "application_manager/commands/hmi/vr_delete_command_response.h"
#include "application_manager/commands/hmi/vr_change_registration_request.h"
#include "application_manager/commands/hmi/vr_change_registration_response.h"
#include "application_manager/commands/hmi/vr_get_supported_languages_request.h"
#include "application_manager/commands/hmi/vr_get_supported_languages_response.h"
#include "application_manager/commands/hmi/tts_is_ready_request.h"
#include "application_manager/commands/hmi/tts_is_ready_response.h"
#include "application_manager/commands/hmi/activate_app_request.h"
#include "application_manager/commands/hmi/activate_app_response.h"
#include "application_manager/commands/hmi/on_ready_notification.h"
#include "application_manager/commands/hmi/on_device_chosen_notification.h"
#include "application_manager/commands/hmi/on_system_context_notification.h"
#include "application_manager/commands/hmi/on_device_list_updated_notification.h"
#include "application_manager/commands/hmi/on_app_registered_notification.h"
#include "application_manager/commands/hmi/on_app_unregistered_notification.h"
#include "application_manager/commands/hmi/on_driver_distraction_notification.h"
#include "application_manager/commands/hmi/on_play_tone_notification.h"
#include "application_manager/commands/hmi/on_vr_started_notification.h"
#include "application_manager/commands/hmi/on_vr_stopped_notification.h"
#include "application_manager/commands/hmi/on_vr_command_notification.h"
#include "application_manager/commands/hmi/on_ui_command_notification.h"
#include "application_manager/commands/hmi/on_app_deactivated_notification.h"
#include "application_manager/commands/hmi/on_ui_language_change_notification.h"
#include "application_manager/commands/hmi/on_vr_language_change_notification.h"
#include "application_manager/commands/hmi/on_tts_language_change_notification.h"
#include "application_manager/commands/hmi/close_popup_request.h"
#include "application_manager/commands/hmi/close_popup_response.h"

namespace application_manager {

CommandSharedPtr HMICommandFactory::CreateCommand(
  const MessageSharedPtr& message) {
  CommandSharedPtr command(NULL);

  switch (static_cast<int>((*message)[strings::params][strings::function_id])) {
    case  hmi_apis::FunctionID::BasicCommunication_GetDeviceList: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::GetDeviceListResponse(message));
      } else {
        command.reset(new commands::GetDeviceListRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::BasicCommunication_MixingAudioSupported: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::MixingAudioSupportedResponse(message));
      } else {
        command.reset(new commands::MixingAudioSupportedRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::BasicCommunication_ExitAllApplications: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::ExitAllApplicationsResponse(message));
      } else {
        command.reset(new commands::ExitAllApplicationsRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::UI_GetSupportedLanguages: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::UIGetSupportedLanguagesResponse(message));
      } else {
        command.reset(new commands::UIGetSupportedLanguagesRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::UI_GetLanguage: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::UIGetLanguageResponse(message));
      } else {
        command.reset(new commands::UIGetLanguageRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::UI_GetCapabilities: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::UIGetCapabilitiesResponse(message));
      } else {
        command.reset(new commands::UIGetCapabilitiesRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::UI_ChangeRegistration: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::UIChangeRegistratioResponse(message));
      } else {
        command.reset(new commands::UIChangeRegistrationRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::VR_IsReady: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::VRIsReadyResponse(message));
      } else {
        command.reset(new commands::VRIsReadyRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::VR_AddCommand: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::VRAddCommandResponse(message));
      } else {
        command.reset(new commands::VRAddCommandRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::VR_DeleteCommand: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::VRDeleteCommandResponse(message));
      } else {
        command.reset(new commands::VRDeleteCommandRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::VR_ChangeRegistration: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::VRChangeRegistratioResponse(message));
      } else {
        command.reset(new commands::VRChangeRegistrationRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::VR_GetSupportedLanguages: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::VRGetSupportedLanguagesResponse(message));
      } else {
        command.reset(new commands::VRGetSupportedLanguagesRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::TTS_IsReady: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::TTSIsReadyResponse(message));
      } else {
        command.reset(new commands::TTSIsReadyRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::BasicCommunication_ActivateApp: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::ActivateAppResponse(message));
      } else {
        command.reset(new commands::ActivateAppRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::BasicCommunication_ExitApplication: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::ExitApplicationResponse(message));
      } else {
        command.reset(new commands::ExitApplicationRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::UI_Show: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::UIShowResponse(message));
      } else {
        command.reset(new commands::UIShowRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::UI_ClosePopUp: {
      if ((*message)[strings::params][strings::message_type] ==
          MessageType::kResponse) {
        command.reset(new commands::ClosePopupResponse(message));
      } else {
        command.reset(new commands::ClosePopupRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::BasicCommunication_PlayTone: {
      command.reset(new commands::OnPlayToneNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::BasicCommunication_OnReady: {
      command.reset(new commands::OnReadyNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::UI_OnDeviceChosen: {
      command.reset(new commands::OnDeviceChosenNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::UI_OnSystemContext: {
      command.reset(new commands::OnSystemContextNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::UI_OnDriverDistraction: {
      command.reset(new commands::OnDriverDistractionNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::BasicCommunication_OnDeviceListUpdated: {
      command.reset(new commands::OnDeviceListUpdatedNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::BasicCommunication_OnAppRegistered: {
      command.reset(new commands::OnAppRegisteredNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered: {
      command.reset(new commands::OnAppUnregisteredNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::VR_Started: {
      command.reset(new commands::OnVRStartedNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::VR_Stopped: {
      command.reset(new commands::OnVRStoppedNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::VR_OnCommand: {
      command.reset(new commands::OnVRCommandNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::UI_OnCommand: {
      command.reset(new commands::OnUICommandNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::BasicCommunication_OnAppDeactivated: {
      command.reset(new commands::OnAppDeactivatedNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::UI_OnLanguageChange: {
      command.reset(new commands::OnUILanguageChangeNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::VR_OnLanguageChange: {
      command.reset(new commands::OnVRLanguageChangeNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::TTS_OnLanguageChange: {
      command.reset(new commands::OnTTSLanguageChangeNotification(message));
      break;
    }
  }

  return command;
}

}  // namespace application_manager
