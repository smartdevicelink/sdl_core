/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Copyright (c) 2017 Xevo Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the copyright holders nor the names of their contributors
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

#include "application_manager/commands/hmi/update_device_list_request.h"
#include "application_manager/commands/hmi/update_device_list_response.h"
#include "application_manager/commands/hmi/on_update_device_list.h"
#include "application_manager/commands/hmi/on_start_device_discovery.h"
#include "application_manager/commands/hmi/update_app_list_request.h"
#include "application_manager/commands/hmi/update_app_list_response.h"
#include "application_manager/commands/hmi/on_find_applications.h"
#include "application_manager/commands/hmi/allow_all_apps_request.h"
#include "application_manager/commands/hmi/allow_all_apps_response.h"
#include "application_manager/commands/hmi/allow_app_request.h"
#include "application_manager/commands/hmi/allow_app_response.h"
#include "application_manager/commands/hmi/sdl_get_list_of_permissions_request.h"
#include "application_manager/commands/hmi/sdl_get_list_of_permissions_response.h"
#include "application_manager/commands/hmi/sdl_get_user_friendly_message_request.h"
#include "application_manager/commands/hmi/sdl_get_user_friendly_message_response.h"
#include "application_manager/commands/hmi/sdl_get_status_update_request.h"
#include "application_manager/commands/hmi/sdl_get_status_update_response.h"
#include "application_manager/commands/hmi/on_status_update_notification.h"
#include "application_manager/commands/hmi/update_sdl_request.h"
#include "application_manager/commands/hmi/update_sdl_response.h"
#include "application_manager/commands/hmi/activate_app_request.h"
#include "application_manager/commands/hmi/activate_app_response.h"
#include "application_manager/commands/hmi/mixing_audio_supported_request.h"
#include "application_manager/commands/hmi/mixing_audio_supported_response.h"
#include "application_manager/commands/hmi/on_allow_sdl_functionality_notification.h"
#include "application_manager/commands/hmi/on_app_permission_changed_notification.h"
#include "application_manager/commands/hmi/on_app_permission_consent_notification.h"
#include "application_manager/commands/hmi/on_app_activated_notification.h"
#include "application_manager/commands/hmi/on_audio_data_streaming_notification.h"
#include "application_manager/commands/hmi/on_video_data_streaming_notification.h"
#include "application_manager/commands/hmi/on_sdl_consent_needed_notification.h"
#include "application_manager/commands/hmi/on_exit_all_applications_notification.h"
#include "application_manager/commands/hmi/on_exit_application_notification.h"
#include "application_manager/commands/hmi/on_put_file_notification.h"
#include "application_manager/commands/hmi/on_resume_audio_source_notification.h"
#include "application_manager/commands/hmi/on_ignition_cycle_over_notification.h"
#include "application_manager/commands/hmi/on_system_info_changed_notification.h"
#include "application_manager/commands/hmi/get_system_info_request.h"
#include "application_manager/commands/hmi/get_system_info_response.h"
#include "application_manager/commands/hmi/close_popup_request.h"
#include "application_manager/commands/hmi/close_popup_response.h"
#include "application_manager/commands/hmi/button_get_capabilities_request.h"
#include "application_manager/commands/hmi/button_get_capabilities_response.h"
#include "application_manager/commands/hmi/ui_add_command_request.h"
#include "application_manager/commands/hmi/ui_add_command_response.h"
#include "application_manager/commands/hmi/ui_delete_command_request.h"
#include "application_manager/commands/hmi/ui_delete_command_response.h"
#include "application_manager/commands/hmi/ui_add_submenu_request.h"
#include "application_manager/commands/hmi/ui_add_submenu_response.h"
#include "application_manager/commands/hmi/ui_delete_submenu_request.h"
#include "application_manager/commands/hmi/ui_delete_submenu_response.h"
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
#include "application_manager/commands/hmi/ui_alert_request.h"
#include "application_manager/commands/hmi/ui_alert_response.h"
#include "application_manager/commands/hmi/ui_is_ready_request.h"
#include "application_manager/commands/hmi/ui_is_ready_response.h"
#include "application_manager/commands/hmi/ui_slider_request.h"
#include "application_manager/commands/hmi/ui_slider_response.h"
#include "application_manager/commands/hmi/ui_set_media_clock_timer_request.h"
#include "application_manager/commands/hmi/ui_set_media_clock_timer_response.h"
#include "application_manager/commands/hmi/ui_set_global_properties_request.h"
#include "application_manager/commands/hmi/ui_set_global_properties_response.h"
#include "application_manager/commands/hmi/ui_scrollable_message_request.h"
#include "application_manager/commands/hmi/ui_scrollable_message_response.h"
#include "application_manager/commands/hmi/ui_set_app_icon_request.h"
#include "application_manager/commands/hmi/ui_set_app_icon_response.h"
#include "application_manager/commands/hmi/ui_perform_audio_pass_thru_response.h"
#include "application_manager/commands/hmi/ui_perform_audio_pass_thru_request.h"
#include "application_manager/commands/hmi/ui_end_audio_pass_thru_response.h"
#include "application_manager/commands/hmi/ui_end_audio_pass_thru_request.h"
#include "application_manager/commands/hmi/ui_perform_interaction_request.h"
#include "application_manager/commands/hmi/ui_perform_interaction_response.h"
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
#include "application_manager/commands/hmi/vr_get_language_request.h"
#include "application_manager/commands/hmi/vr_get_language_response.h"
#include "application_manager/commands/hmi/vr_get_capabilities_request.h"
#include "application_manager/commands/hmi/vr_get_capabilities_response.h"
#include "application_manager/commands/hmi/tts_is_ready_request.h"
#include "application_manager/commands/hmi/tts_is_ready_response.h"
#include "application_manager/commands/hmi/tts_change_registration_request.h"
#include "application_manager/commands/hmi/tts_change_registration_response.h"
#include "application_manager/commands/hmi/tts_get_supported_languages_request.h"
#include "application_manager/commands/hmi/tts_get_supported_languages_response.h"
#include "application_manager/commands/hmi/tts_get_language_request.h"
#include "application_manager/commands/hmi/tts_get_language_response.h"
#include "application_manager/commands/hmi/tts_stop_speaking_request.h"
#include "application_manager/commands/hmi/tts_stop_speaking_response.h"
#include "application_manager/commands/hmi/tts_speak_request.h"
#include "application_manager/commands/hmi/tts_speak_response.h"
#include "application_manager/commands/hmi/tts_set_global_properties_request.h"
#include "application_manager/commands/hmi/tts_set_global_properties_response.h"
#include "application_manager/commands/hmi/tts_get_capabilities_request.h"
#include "application_manager/commands/hmi/tts_get_capabilities_response.h"
#include "application_manager/commands/hmi/vr_perform_interaction_request.h"
#include "application_manager/commands/hmi/vr_perform_interaction_response.h"
#include "application_manager/commands/hmi/vi_is_ready_request.h"
#include "application_manager/commands/hmi/vi_is_ready_response.h"
#include "application_manager/commands/hmi/vi_read_did_request.h"
#include "application_manager/commands/hmi/vi_read_did_response.h"
#include "application_manager/commands/hmi/sdl_activate_app_request.h"
#include "application_manager/commands/hmi/sdl_activate_app_response.h"
#include "application_manager/commands/hmi/on_app_permission_changed_notification.h"
#include "application_manager/commands/hmi/on_event_changed_notification.h"

#ifdef EXTERNAL_PROPRIETARY_MODE
#include "application_manager/commands/hmi/decrypt_certificate_request.h"
#include "application_manager/commands/hmi/decrypt_certificate_response.h"
#endif  // EXTERNAL_PROPRIETARY_MODE

#include "application_manager/commands/hmi/vi_get_vehicle_data_request.h"
#include "application_manager/commands/hmi/vi_get_vehicle_data_response.h"
#include "application_manager/commands/hmi/on_vi_vehicle_data_notification.h"
#include "application_manager/commands/hmi/vi_subscribe_vehicle_data_request.h"
#include "application_manager/commands/hmi/vi_subscribe_vehicle_data_response.h"
#include "application_manager/commands/hmi/vi_unsubscribe_vehicle_data_request.h"
#include "application_manager/commands/hmi/vi_unsubscribe_vehicle_data_response.h"

#include "application_manager/commands/hmi/vi_get_dtcs_request.h"
#include "application_manager/commands/hmi/vi_get_dtcs_response.h"
#include "application_manager/commands/hmi/vi_diagnostic_message_request.h"
#include "application_manager/commands/hmi/vi_diagnostic_message_response.h"
#include "application_manager/commands/hmi/vi_get_vehicle_type_request.h"
#include "application_manager/commands/hmi/vi_get_vehicle_type_response.h"
#include "application_manager/commands/hmi/navi_is_ready_request.h"
#include "application_manager/commands/hmi/navi_show_constant_tbt_request.h"
#include "application_manager/commands/hmi/navi_show_constant_tbt_response.h"
#include "application_manager/commands/hmi/navi_is_ready_response.h"
#include "application_manager/commands/hmi/navi_alert_maneuver_request.h"
#include "application_manager/commands/hmi/navi_alert_maneuver_response.h"
#include "application_manager/commands/hmi/navi_update_turn_list_request.h"
#include "application_manager/commands/hmi/navi_update_turn_list_response.h"
#include "application_manager/commands/hmi/navi_subscribe_way_points_request.h"
#include "application_manager/commands/hmi/navi_subscribe_way_points_response.h"
#include "application_manager/commands/hmi/navi_unsubscribe_way_points_request.h"
#include "application_manager/commands/hmi/navi_unsubscribe_way_points_response.h"
#include "application_manager/commands/hmi/navi_get_way_points_request.h"
#include "application_manager/commands/hmi/navi_get_way_points_response.h"
#include "application_manager/commands/hmi/on_ready_notification.h"
#include "application_manager/commands/hmi/on_device_chosen_notification.h"
#include "application_manager/commands/hmi/on_file_removed_notification.h"
#include "application_manager/commands/hmi/on_system_context_notification.h"
#include "application_manager/commands/hmi/on_app_registered_notification.h"
#include "application_manager/commands/hmi/on_app_unregistered_notification.h"
#include "application_manager/commands/hmi/on_driver_distraction_notification.h"
#include "application_manager/commands/hmi/on_tts_started_notification.h"
#include "application_manager/commands/hmi/on_tts_stopped_notification.h"
#include "application_manager/commands/hmi/on_vr_started_notification.h"
#include "application_manager/commands/hmi/on_vr_stopped_notification.h"
#include "application_manager/commands/hmi/on_vr_command_notification.h"
#include "application_manager/commands/hmi/on_ui_command_notification.h"
#include "application_manager/commands/hmi/on_app_deactivated_notification.h"
#include "application_manager/commands/hmi/on_ui_language_change_notification.h"
#include "application_manager/commands/hmi/on_vr_language_change_notification.h"
#include "application_manager/commands/hmi/on_tts_language_change_notification.h"
#include "application_manager/commands/hmi/on_navi_tbt_client_state_notification.h"
#include "application_manager/commands/hmi/on_navi_way_point_change_notification.h"
#include "application_manager/commands/hmi/on_button_event_notification.h"
#include "application_manager/commands/hmi/on_button_press_notification.h"
#include "application_manager/commands/hmi/on_button_subscription_notification.h"
#include "application_manager/commands/hmi/on_vi_vehicle_data_notification.h"
#include "application_manager/commands/hmi/on_ui_keyboard_input_notification.h"
#include "application_manager/commands/hmi/on_ui_touch_event_notification.h"
#include "application_manager/commands/hmi/on_ui_reset_timeout_notification.h"
#include "application_manager/commands/hmi/navi_set_video_config_request.h"
#include "application_manager/commands/hmi/navi_set_video_config_response.h"
#include "application_manager/commands/hmi/navi_start_stream_request.h"
#include "application_manager/commands/hmi/navi_start_stream_response.h"
#include "application_manager/commands/hmi/navi_stop_stream_request.h"
#include "application_manager/commands/hmi/navi_stop_stream_response.h"
#include "application_manager/commands/hmi/navi_audio_start_stream_request.h"
#include "application_manager/commands/hmi/navi_audio_start_stream_response.h"
#include "application_manager/commands/hmi/navi_audio_stop_stream_request.h"
#include "application_manager/commands/hmi/navi_audio_stop_stream_response.h"
#include "application_manager/commands/hmi/on_system_request_notification.h"
#include "application_manager/commands/hmi/ui_set_display_layout_request.h"
#include "application_manager/commands/hmi/ui_set_display_layout_response.h"
#include "application_manager/commands/hmi/on_sdl_close_notification.h"
#include "application_manager/commands/hmi/on_sdl_persistence_complete_notification.h"
#include "application_manager/commands/hmi/on_record_start_notification.h"
#include "application_manager/commands/hmi/add_statistics_info_notification.h"
#include "application_manager/commands/hmi/on_system_error_notification.h"
#include "application_manager/commands/hmi/basic_communication_system_request.h"
#include "application_manager/commands/hmi/basic_communication_system_response.h"
#include "application_manager/commands/hmi/basic_communication_on_awake_sdl.h"
#include "application_manager/commands/hmi/sdl_policy_update.h"
#include "application_manager/commands/hmi/sdl_policy_update_response.h"
#include "application_manager/commands/hmi/on_received_policy_update.h"
#include "application_manager/commands/hmi/on_policy_update.h"
#include "application_manager/commands/hmi/get_urls.h"
#include "application_manager/commands/hmi/get_urls_response.h"
#include "application_manager/commands/hmi/on_device_state_changed_notification.h"
#include "application_manager/commands/hmi/navi_send_location_request.h"
#include "application_manager/commands/hmi/navi_send_location_response.h"
#include "application_manager/commands/hmi/on_tts_reset_timeout_notification.h"
#include "application_manager/commands/hmi/dial_number_request.h"
#include "application_manager/commands/hmi/dial_number_response.h"
#include "application_manager/commands/hmi/ui_send_haptic_data_request.h"
#include "application_manager/commands/hmi/ui_send_haptic_data_response.h"

#include "application_manager/commands/hmi/rc_is_ready_request.h"
#include "application_manager/commands/hmi/rc_is_ready_response.h"
#include "application_manager/commands/hmi/rc_get_capabilities_request.h"
#include "application_manager/commands/hmi/rc_get_capabilities_response.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")
namespace application_manager {

CommandSharedPtr HMICommandFactory::CreateCommand(
    const commands::MessageSharedPtr& message,
    ApplicationManager& application_manager) {
  const int function_id =
      (*message)[strings::params][strings::function_id].asInt();
  LOG4CXX_DEBUG(
      logger_, "HMICommandFactory::CreateCommand function_id: " << function_id);

  CommandSharedPtr command(new application_manager::commands::CommandImpl(
      message, application_manager));

  bool is_response = false;
  const int msg_type =
      (*message)[strings::params][strings::message_type].asInt();
  if (msg_type ==
      static_cast<int>(application_manager::MessageType::kResponse)) {
    is_response = true;
    LOG4CXX_DEBUG(logger_, "HMICommandFactory::CreateCommand response");
  } else if ((*message)[strings::params][strings::message_type] ==
             static_cast<int>(
                 application_manager::MessageType::kErrorResponse)) {
    is_response = true;
    LOG4CXX_DEBUG(logger_, "HMICommandFactory::CreateCommand error response");
  } else {
    LOG4CXX_DEBUG(logger_, "HMICommandFactory::CreateCommand request");
  }

  switch (function_id) {
    case hmi_apis::FunctionID::BasicCommunication_OnStartDeviceDiscovery: {
      command.reset(
          new commands::OnStartDeviceDiscovery(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList: {
      if (is_response) {
        command.reset(new commands::UpdateDeviceListResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UpdateDeviceListRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_ActivateApp: {
      if (is_response) {
        command.reset(
            new commands::ActivateAppResponse(message, application_manager));
      } else {
        command.reset(
            new commands::ActivateAppRequest(message, application_manager));
      }
      break;
    }
#ifdef EXTERNAL_PROPRIETARY_MODE
    case hmi_apis::FunctionID::BasicCommunication_DecryptCertificate: {
      if (is_response) {
        command.reset(new commands::DecryptCertificateResponse(
            message, application_manager));
      } else {
        command.reset(new commands::DecryptCertificateRequest(
            message, application_manager));
      }
      break;
    }
#endif  // EXTERNAL_PROPRIETARY_MODE
    case hmi_apis::FunctionID::BasicCommunication_GetSystemInfo: {
      if (is_response) {
        command.reset(
            new commands::GetSystemInfoResponse(message, application_manager));
      } else {
        command.reset(
            new commands::GetSystemInfoRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_ActivateApp: {
      if (is_response) {
        command.reset(
            new commands::SDLActivateAppResponse(message, application_manager));
      } else {
        command.reset(
            new commands::SDLActivateAppRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_PolicyUpdate: {
      if (is_response) {
        command.reset(new commands::SDLPolicyUpdateResponse(
            message, application_manager));
      } else {
        command.reset(
            new commands::SDLPolicyUpdate(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_GetURLS: {
      if (is_response) {
        command.reset(
            new commands::GetUrlsResponse(message, application_manager));
      } else {
        command.reset(new commands::GetUrls(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_OnAppPermissionChanged: {
      command.reset(new commands::OnAppPermissionChangedNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::SDL_GetListOfPermissions: {
      if (is_response) {
        command.reset(new commands::SDLGetListOfPermissionsResponse(
            message, application_manager));
      } else {
        command.reset(new commands::SDLGetListOfPermissionsRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_GetUserFriendlyMessage: {
      if (is_response) {
        command.reset(new commands::SDLGetUserFriendlyMessageResponse(
            message, application_manager));
      } else {
        command.reset(new commands::SDLGetUserFriendlyMessageRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_GetStatusUpdate: {
      if (is_response) {
        command.reset(new commands::SDLGetStatusUpdateResponse(
            message, application_manager));
      } else {
        command.reset(new commands::SDLGetStatusUpdateRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_OnStatusUpdate: {
      command.reset(new commands::OnStatusUpdateNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnAppPermissionConsent: {
      command.reset(new commands::OnAppPermissionConsentNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_MixingAudioSupported: {
      if (is_response) {
        command.reset(new commands::MixingAudioSupportedResponse(
            message, application_manager));
      } else {
        command.reset(new commands::MixingAudioSupportedRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnExitAllApplications: {
      command.reset(new commands::OnExitAllApplicationsNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::UI_AddCommand: {
      if (is_response) {
        command.reset(
            new commands::UIAddCommandResponse(message, application_manager));
      } else {
        command.reset(
            new commands::UIAddCommandRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_DeleteCommand: {
      if (is_response) {
        command.reset(new commands::UIDeleteCommandResponse(
            message, application_manager));
      } else {
        command.reset(
            new commands::UIDeleteCommandRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_AddSubMenu: {
      if (is_response) {
        command.reset(
            new commands::UIAddSubmenuResponse(message, application_manager));
      } else {
        command.reset(
            new commands::UIAddSubmenuRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_DeleteSubMenu: {
      if (is_response) {
        command.reset(new commands::UIDeleteSubmenuResponse(
            message, application_manager));
      } else {
        command.reset(
            new commands::UIDeleteSubmenuRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_SetMediaClockTimer: {
      if (is_response) {
        command.reset(new commands::UISetMediaClockTimerResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UISetMediaClockTimerRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_PerformInteraction: {
      if (is_response) {
        command.reset(new commands::UIPerformInteractionResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UIPerformInteractionRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_SetGlobalProperties: {
      if (is_response) {
        command.reset(new commands::UISetGlobalPropertiesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UISetGlobalPropertiesRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_ScrollableMessage: {
      if (is_response) {
        command.reset(new commands::UIScrollableMessageResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UIScrollableMessageRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_SetAppIcon: {
      if (is_response) {
        command.reset(
            new commands::UISetAppIconResponse(message, application_manager));
      } else {
        command.reset(
            new commands::UISetAppIconRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_GetSupportedLanguages: {
      if (is_response) {
        command.reset(new commands::UIGetSupportedLanguagesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UIGetSupportedLanguagesRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_GetLanguage: {
      if (is_response) {
        command.reset(
            new commands::UIGetLanguageResponse(message, application_manager));
      } else {
        command.reset(
            new commands::UIGetLanguageRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_GetCapabilities: {
      if (is_response) {
        command.reset(new commands::UIGetCapabilitiesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UIGetCapabilitiesRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_ChangeRegistration: {
      if (is_response) {
        command.reset(new commands::UIChangeRegistratioResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UIChangeRegistrationRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_PerformAudioPassThru: {
      if (is_response) {
        command.reset(new commands::UIPerformAudioPassThruResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UIPerformAudioPassThruRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_EndAudioPassThru: {
      if (is_response) {
        command.reset(new commands::UIEndAudioPassThruResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UIEndAudioPassThruRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_Alert: {
      if (is_response) {
        command.reset(
            new commands::UIAlertResponse(message, application_manager));
      } else {
        command.reset(
            new commands::UIAlertRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_IsReady: {
      if (is_response) {
        command.reset(
            new commands::VRIsReadyResponse(message, application_manager));
      } else {
        command.reset(
            new commands::VRIsReadyRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_AddCommand: {
      if (is_response) {
        command.reset(
            new commands::VRAddCommandResponse(message, application_manager));
      } else {
        command.reset(
            new commands::VRAddCommandRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_DeleteCommand: {
      if (is_response) {
        command.reset(new commands::VRDeleteCommandResponse(
            message, application_manager));
      } else {
        command.reset(
            new commands::VRDeleteCommandRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_ChangeRegistration: {
      if (is_response) {
        command.reset(new commands::VRChangeRegistrationResponse(
            message, application_manager));
      } else {
        command.reset(new commands::VRChangeRegistrationRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_GetSupportedLanguages: {
      if (is_response) {
        command.reset(new commands::VRGetSupportedLanguagesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::VRGetSupportedLanguagesRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_GetLanguage: {
      if (is_response) {
        command.reset(
            new commands::VRGetLanguageResponse(message, application_manager));
      } else {
        command.reset(
            new commands::VRGetLanguageRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_GetCapabilities: {
      if (is_response) {
        command.reset(new commands::VRGetCapabilitiesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::VRGetCapabilitiesRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_IsReady: {
      if (is_response) {
        command.reset(
            new commands::TTSIsReadyResponse(message, application_manager));
      } else {
        command.reset(
            new commands::TTSIsReadyRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_ChangeRegistration: {
      if (is_response) {
        command.reset(new commands::TTSChangeRegistratioResponse(
            message, application_manager));
      } else {
        command.reset(new commands::TTSChangeRegistrationRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_GetSupportedLanguages: {
      if (is_response) {
        command.reset(new commands::TTSGetSupportedLanguagesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::TTSGetSupportedLanguagesRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_StopSpeaking: {
      if (is_response) {
        command.reset(new commands::TTSStopSpeakingResponse(
            message, application_manager));
      } else {
        command.reset(
            new commands::TTSStopSpeakingRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_GetLanguage: {
      if (is_response) {
        command.reset(
            new commands::TTSGetLanguageResponse(message, application_manager));
      } else {
        command.reset(
            new commands::TTSGetLanguageRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      if (is_response) {
        command.reset(
            new commands::TTSSpeakResponse(message, application_manager));
      } else {
        command.reset(
            new commands::TTSSpeakRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_SetGlobalProperties: {
      if (is_response) {
        command.reset(new commands::TTSSetGlobalPropertiesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::TTSSetGlobalPropertiesRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_GetCapabilities: {
      if (is_response) {
        command.reset(new commands::TTSGetCapabilitiesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::TTSGetCapabilitiesRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_Started: {
      command.reset(
          new commands::OnTTSStartedNotification(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::TTS_Stopped: {
      command.reset(
          new commands::OnTTSStoppedNotification(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppActivated: {
      command.reset(new commands::OnAppActivatedNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAwakeSDL: {
      command.reset(
          new commands::OnAwakeSDLNotification(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnExitApplication: {
      command.reset(new commands::OnExitApplicationNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::UI_Show: {
      if (is_response) {
        command.reset(
            new commands::UIShowResponse(message, application_manager));
      } else {
        command.reset(
            new commands::UIShowRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_Slider: {
      if (is_response) {
        command.reset(
            new commands::UISliderResponse(message, application_manager));
      } else {
        command.reset(
            new commands::UISliderRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_ClosePopUp: {
      if (is_response) {
        command.reset(
            new commands::ClosePopupResponse(message, application_manager));
      } else {
        command.reset(
            new commands::ClosePopupRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_IsReady: {
      if (is_response) {
        command.reset(
            new commands::UIIsReadyResponse(message, application_manager));
      } else {
        command.reset(
            new commands::UIIsReadyRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_IsReady: {
      if (is_response) {
        command.reset(
            new commands::VIIsReadyResponse(message, application_manager));
      } else {
        command.reset(
            new commands::VIIsReadyRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_ReadDID: {
      if (is_response) {
        command.reset(
            new commands::VIReadDIDResponse(message, application_manager));
      } else {
        command.reset(
            new commands::VIReadDIDRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetVehicleData: {
      if (is_response) {
        command.reset(new commands::VIGetVehicleDataResponse(
            message, application_manager));
      } else {
        command.reset(new commands::VIGetVehicleDataRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetDTCs: {
      if (is_response) {
        command.reset(
            new commands::VIGetDTCsResponse(message, application_manager));
      } else {
        command.reset(
            new commands::VIGetDTCsRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_DiagnosticMessage: {
      if (is_response) {
        command.reset(new commands::VIDiagnosticMessageResponse(
            message, application_manager));
      } else {
        command.reset(new commands::VIDiagnosticMessageRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetVehicleType: {
      if (is_response) {
        command.reset(new commands::VIGetVehicleTypeResponse(
            message, application_manager));
      } else {
        command.reset(new commands::VIGetVehicleTypeRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_IsReady: {
      if (is_response) {
        command.reset(
            new commands::NaviIsReadyResponse(message, application_manager));
      } else {
        command.reset(
            new commands::NaviIsReadyRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_AlertManeuver: {
      if (is_response) {
        command.reset(new commands::NaviAlertManeuverResponse(
            message, application_manager));
      } else {
        command.reset(new commands::NaviAlertManeuverRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_GetWayPoints: {
      if (is_response) {
        command.reset(new commands::NaviGetWayPointsResponse(
            message, application_manager));
      } else {
        command.reset(new commands::NaviGetWayPointsRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_UpdateTurnList: {
      if (is_response) {
        command.reset(new commands::NaviUpdateTurnListResponse(
            message, application_manager));
      } else {
        command.reset(new commands::NaviUpdateTurnListRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_ShowConstantTBT: {
      if (is_response) {
        command.reset(new commands::NaviShowConstantTBTResponse(
            message, application_manager));
      } else {
        command.reset(new commands::NaviShowConstantTBTRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_SubscribeWayPoints: {
      if (is_response) {
        command.reset(new commands::NaviSubscribeWayPointsResponse(
            message, application_manager));
      } else {
        command.reset(new commands::NaviSubscribeWayPointsRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_UnsubscribeWayPoints: {
      if (is_response) {
        command.reset(new commands::NaviUnsubscribeWayPointsResponse(
            message, application_manager));
      } else {
        command.reset(new commands::NaviUnSubscribeWayPointsRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Buttons_GetCapabilities: {
      if (is_response) {
        command.reset(new commands::ButtonGetCapabilitiesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::ButtonGetCapabilitiesRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_OnAllowSDLFunctionality: {
      command.reset(new commands::OnAllowSDLFunctionalityNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnSDLConsentNeeded: {
      command.reset(new commands::OnSDLConsentNeededNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::SDL_UpdateSDL: {
      if (is_response) {
        command.reset(
            new commands::UpdateSDLResponse(message, application_manager));
      } else {
        command.reset(
            new commands::UpdateSDLRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnIgnitionCycleOver: {
      command.reset(new commands::OnIgnitionCycleOverNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSystemInfoChanged: {
      command.reset(new commands::OnSystemInfoChangedNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnReady: {
      command.reset(
          new commands::OnReadyNotification(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnDeviceChosen: {
      command.reset(new commands::OnDeviceChosenNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::UI_OnSystemContext: {
      command.reset(new commands::OnSystemContextNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::UI_OnDriverDistraction: {
      command.reset(new commands::hmi::OnDriverDistractionNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnUpdateDeviceList: {
      command.reset(
          new commands::OnUpdateDeviceList(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppRegistered: {
      command.reset(new commands::OnAppRegisteredNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered: {
      command.reset(new commands::OnAppUnregisteredNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnFindApplications: {
      command.reset(
          new commands::OnFindApplications(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_UpdateAppList: {
      if (is_response) {
        command.reset(
            new commands::UpdateAppListResponse(message, application_manager));
      } else {
        command.reset(
            new commands::UpdateAppListRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_Started: {
      command.reset(
          new commands::OnVRStartedNotification(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::VR_Stopped: {
      command.reset(
          new commands::OnVRStoppedNotification(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::VR_OnCommand: {
      command.reset(
          new commands::OnVRCommandNotification(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::UI_OnCommand: {
      command.reset(
          new commands::OnUICommandNotification(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppDeactivated: {
      command.reset(new commands::OnAppDeactivatedNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::UI_OnLanguageChange: {
      command.reset(new commands::OnUILanguageChangeNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::VR_OnLanguageChange: {
      command.reset(new commands::OnVRLanguageChangeNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::TTS_OnLanguageChange: {
      command.reset(new commands::OnTTSLanguageChangeNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::Buttons_OnButtonEvent: {
      command.reset(new commands::hmi::OnButtonEventNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::Buttons_OnButtonPress: {
      command.reset(new commands::hmi::OnButtonPressNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::Buttons_OnButtonSubscription: {
      command.reset(new commands::hmi::OnButtonSubscriptionNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData: {
      if (is_response) {
        command.reset(new commands::VISubscribeVehicleDataResponse(
            message, application_manager));
      } else {
        command.reset(new commands::VISubscribeVehicleDataRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData: {
      if (is_response) {
        command.reset(new commands::VIUnsubscribeVehicleDataResponse(
            message, application_manager));
      } else {
        command.reset(new commands::VIUnsubscribeVehicleDataRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnVehicleData: {
      command.reset(new commands::OnVIVehicleDataNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::Navigation_OnTBTClientState: {
      command.reset(new commands::OnNaviTBTClientStateNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::UI_OnKeyboardInput: {
      command.reset(new commands::hmi::OnUIKeyBoardInputNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::UI_OnTouchEvent: {
      command.reset(new commands::hmi::OnUITouchEventNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::UI_OnResetTimeout: {
      command.reset(new commands::hmi::OnUIResetTimeoutNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::Navigation_SetVideoConfig: {
      if (is_response) {
        command.reset(new commands::NaviSetVideoConfigResponse(
            message, application_manager));
      } else {
        command.reset(new commands::NaviSetVideoConfigRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_StartStream: {
      if (is_response) {
        command.reset(new commands::NaviStartStreamResponse(
            message, application_manager));
      } else {
        command.reset(
            new commands::NaviStartStreamRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_StopStream: {
      if (is_response) {
        command.reset(
            new commands::NaviStopStreamResponse(message, application_manager));
      } else {
        command.reset(
            new commands::NaviStopStreamRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_StartAudioStream: {
      if (is_response) {
        command.reset(new commands::AudioStartStreamResponse(
            message, application_manager));
      } else {
        command.reset(new commands::AudioStartStreamRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_StopAudioStream: {
      if (is_response) {
        command.reset(new commands::AudioStopStreamResponse(
            message, application_manager));
      } else {
        command.reset(
            new commands::AudioStopStreamRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_OnAudioDataStreaming: {
      command.reset(new commands::OnAudioDataStreamingNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::Navigation_OnVideoDataStreaming: {
      command.reset(new commands::OnVideoDataStreamingNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::VR_PerformInteraction: {
      if (is_response) {
        command.reset(new commands::VRPerformInteractionResponse(
            message, application_manager));
      } else {
        command.reset(new commands::VRPerformInteractionRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSystemRequest: {
      command.reset(new commands::OnSystemRequestNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnPutFile: {
      command.reset(
          new commands::OnPutFileNotification(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnResumeAudioSource: {
      command.reset(new commands::OnResumeAudioSourceNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::UI_SetDisplayLayout: {
      if (is_response) {
        command.reset(new commands::UiSetDisplayLayoutResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UiSetDisplayLayoutRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSDLClose: {
      command.reset(
          new commands::OnSDLCloseNotification(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSDLPersistenceComplete: {
      command.reset(new commands::OnSDLPersistenceCompleteNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnFileRemoved: {
      command.reset(new commands::OnFileRemovedNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::UI_OnRecordStart: {
      command.reset(new commands::OnRecordStartdNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_SystemRequest: {
      if (is_response) {
        command.reset(new commands::BasicCommunicationSystemResponse(
            message, application_manager));
      } else {
        command.reset(new commands::BasicCommunicationSystemRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_SendLocation: {
      if (is_response) {
        command.reset(new commands::NaviSendLocationResponse(
            message, application_manager));
      } else {
        command.reset(new commands::NaviSendLocationRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_AddStatisticsInfo: {
      command.reset(new commands::AddStatisticsInfoNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnSystemError: {
      command.reset(new commands::OnSystemErrorNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnReceivedPolicyUpdate: {
      command.reset(
          new commands::OnReceivedPolicyUpdate(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnPolicyUpdate: {
      command.reset(new commands::OnPolicyUpdate(message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnDeviceStateChanged: {
      command.reset(new commands::OnDeviceStateChangedNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::TTS_OnResetTimeout: {
      command.reset(new commands::hmi::OnTTSResetTimeoutNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnEventChanged: {
      command.reset(new commands::OnEventChangedNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_DialNumber: {
      if (is_response) {
        command.reset(new commands::hmi::DialNumberResponse(
            message, application_manager));
      } else {
        command.reset(
            new commands::hmi::DialNumberRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_OnWayPointChange: {
      command.reset(new commands::OnNaviWayPointChangeNotification(
          message, application_manager));
      break;
    }
    case hmi_apis::FunctionID::RC_IsReady: {
      if (is_response) {
        command.reset(
            new commands::RCIsReadyResponse(message, application_manager));
      } else {
        command.reset(
            new commands::RCIsReadyRequest(message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::RC_GetCapabilities: {
      if (is_response) {
        command.reset(new commands::RCGetCapabilitiesResponse(
            message, application_manager));
      } else {
        command.reset(new commands::RCGetCapabilitiesRequest(
            message, application_manager));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_SendHapticData: {
      if (is_response) {
        command.reset(new commands::UISendHapticDataResponse(
            message, application_manager));
      } else {
        command.reset(new commands::UISendHapticDataRequest(
            message, application_manager));
      }
      break;
    }
  }
  return command;
}

}  // namespace application_manager
