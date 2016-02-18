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

#ifdef HMI_DBUS_API
#include "application_manager/commands/hmi/vi_get_vehicle_data_request_template.h"
#include "application_manager/commands/hmi/vi_get_vehicle_data_response_template.h"
#include "application_manager/commands/hmi/vi_subscribe_vehicle_data_request_template.h"
#include "application_manager/commands/hmi/vi_subscribe_vehicle_data_response_template.h"
#include "application_manager/commands/hmi/vi_unsubscribe_vehicle_data_request_template.h"
#include "application_manager/commands/hmi/vi_unsubscribe_vehicle_data_response_template.h"
#include "application_manager/commands/hmi/on_vi_gps_data_notification.h"
#include "application_manager/commands/hmi/on_vi_speed_notification.h"
#include "application_manager/commands/hmi/on_vi_rpm_notification.h"
#include "application_manager/commands/hmi/on_vi_fuel_level_notification.h"
#include "application_manager/commands/hmi/on_vi_fuel_level_state_notification.h"
#include "application_manager/commands/hmi/on_vi_instant_fuel_consumption_notification.h"
#include "application_manager/commands/hmi/on_vi_external_temperature_notification.h"
#include "application_manager/commands/hmi/on_vi_vin_notification.h"
#include "application_manager/commands/hmi/on_vi_prndl_notification.h"
#include "application_manager/commands/hmi/on_vi_tire_pressure_notification.h"
#include "application_manager/commands/hmi/on_vi_odometer_notification.h"
#include "application_manager/commands/hmi/on_vi_belt_status_notification.h"
#include "application_manager/commands/hmi/on_vi_body_information_notification.h"
#include "application_manager/commands/hmi/on_vi_device_status_notification.h"
#include "application_manager/commands/hmi/on_vi_driver_braking_notification.h"
#include "application_manager/commands/hmi/on_vi_wiper_status_notification.h"
#include "application_manager/commands/hmi/on_vi_head_lamp_status_notification.h"
#include "application_manager/commands/hmi/on_vi_engine_torque_notification.h"
#include "application_manager/commands/hmi/on_vi_acc_pedal_position_notification.h"
#include "application_manager/commands/hmi/on_vi_steering_wheel_angle_notification.h"
#include "application_manager/commands/hmi/on_vi_my_key_notification.h"
#else
#include "application_manager/commands/hmi/vi_get_vehicle_data_request.h"
#include "application_manager/commands/hmi/vi_get_vehicle_data_response.h"
#include "application_manager/commands/hmi/on_vi_vehicle_data_notification.h"
#include "application_manager/commands/hmi/vi_subscribe_vehicle_data_request.h"
#include "application_manager/commands/hmi/vi_subscribe_vehicle_data_response.h"
#include "application_manager/commands/hmi/vi_unsubscribe_vehicle_data_request.h"
#include "application_manager/commands/hmi/vi_unsubscribe_vehicle_data_response.h"
#endif // #ifdef HMI_DBUS_API

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
#include "application_manager/commands/hmi/on_button_event_notification.h"
#include "application_manager/commands/hmi/on_button_press_notification.h"
#include "application_manager/commands/hmi/on_button_subscription_notification.h"
#include "application_manager/commands/hmi/on_vi_vehicle_data_notification.h"
#include "application_manager/commands/hmi/on_ui_keyboard_input_notification.h"
#include "application_manager/commands/hmi/on_ui_touch_event_notification.h"
#include "application_manager/commands/hmi/on_ui_reset_timeout_notification.h"
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

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

namespace application_manager {

CommandSharedPtr HMICommandFactory::CreateCommand(
    const commands::MessageSharedPtr& message) {
  const int function_id = (*message)[strings::params][strings::function_id]
      .asInt();
  LOG4CXX_DEBUG(logger_,
               "HMICommandFactory::CreateCommand function_id: " << function_id);

  CommandSharedPtr command(
      new application_manager::commands::CommandImpl(message));

  bool is_response = false;
  const int msg_type = (*message)[strings::params][strings::message_type].asInt();
  if (msg_type == static_cast<int>(application_manager::MessageType::kResponse)) {
    is_response = true;
    LOG4CXX_DEBUG(logger_, "HMICommandFactory::CreateCommand response");
  } else if ((*message)[strings::params][strings::message_type]
      == static_cast<int>(application_manager::MessageType::kErrorResponse)) {
    is_response = true;
    LOG4CXX_DEBUG(logger_, "HMICommandFactory::CreateCommand error response");
  } else {
    LOG4CXX_DEBUG(logger_, "HMICommandFactory::CreateCommand request");
  }

  switch (function_id) {
    case hmi_apis::FunctionID::BasicCommunication_OnStartDeviceDiscovery: {
      command.reset(new commands::OnStartDeviceDiscovery(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList: {
      if (is_response) {
        command.reset(new commands::UpdateDeviceListResponse(message));
      } else {
        command.reset(new commands::UpdateDeviceListRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_ActivateApp: {
      if (is_response) {
        command.reset(new commands::ActivateAppResponse(message));
      } else {
        command.reset(new commands::ActivateAppRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_GetSystemInfo: {
      if (is_response) {
        command.reset(new commands::GetSystemInfoResponse(message));
      } else {
        command.reset(new commands::GetSystemInfoRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_ActivateApp: {
      if (is_response) {
        command.reset(new commands::SDLActivateAppResponse(message));
      } else {
        command.reset(new commands::SDLActivateAppRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_PolicyUpdate: {
      if (is_response) {
        command.reset(new commands::SDLPolicyUpdateResponse(message));
      } else {
        command.reset(new commands::SDLPolicyUpdate(message));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_GetURLS: {
      if (is_response) {
        command.reset(new commands::GetUrlsResponse(message));
      } else {
        command.reset(new commands::GetUrls(message));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_OnAppPermissionChanged: {
      command.reset(new commands::OnAppPermissionChangedNotification(message));
      break;
    }
    case hmi_apis::FunctionID::SDL_GetListOfPermissions: {
      if (is_response) {
        command.reset(new commands::SDLGetListOfPermissionsResponse(message));
      } else {
        command.reset(new commands::SDLGetListOfPermissionsRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_GetUserFriendlyMessage: {
      if (is_response) {
        command.reset(new commands::SDLGetUserFriendlyMessageResponse(message));
      } else {
        command.reset(new commands::SDLGetUserFriendlyMessageRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_GetStatusUpdate: {
      if (is_response) {
        command.reset(new commands::SDLGetStatusUpdateResponse(message));
      } else {
        command.reset(new commands::SDLGetStatusUpdateRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_OnStatusUpdate: {
      command.reset(new commands::OnStatusUpdateNotification(message));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnAppPermissionConsent: {
      command.reset(new commands::OnAppPermissionConsentNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_MixingAudioSupported: {
      if (is_response) {
        command.reset(new commands::MixingAudioSupportedResponse(message));
      } else {
        command.reset(new commands::MixingAudioSupportedRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnExitAllApplications: {
      command.reset(new commands::OnExitAllApplicationsNotification(message));
      break;
    }
    case hmi_apis::FunctionID::UI_AddCommand: {
      if (is_response) {
        command.reset(new commands::UIAddCommandResponse(message));
      } else {
        command.reset(new commands::UIAddCommandRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_DeleteCommand: {
      if (is_response) {
        command.reset(new commands::UIDeleteCommandResponse(message));
      } else {
        command.reset(new commands::UIDeleteCommandRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_AddSubMenu: {
      if (is_response) {
        command.reset(new commands::UIAddSubmenuResponse(message));
      } else {
        command.reset(new commands::UIAddSubmenuRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_DeleteSubMenu: {
      if (is_response) {
        command.reset(new commands::UIDeleteSubmenuResponse(message));
      } else {
        command.reset(new commands::UIDeleteSubmenuRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_SetMediaClockTimer: {
      if (is_response) {
        command.reset(new commands::UISetMediaClockTimerResponse(message));
      } else {
        command.reset(new commands::UISetMediaClockTimerRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_PerformInteraction: {
      if (is_response) {
        command.reset(new commands::UIPerformInteractionResponse(message));
      } else {
        command.reset(new commands::UIPerformInteractionRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_SetGlobalProperties: {
      if (is_response) {
        command.reset(new commands::UISetGlobalPropertiesResponse(message));
      } else {
        command.reset(new commands::UISetGlobalPropertiesRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_ScrollableMessage: {
      if (is_response) {
        command.reset(new commands::UIScrollableMessageResponse(message));
      } else {
        command.reset(new commands::UIScrollableMessageRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_SetAppIcon: {
      if (is_response) {
        command.reset(new commands::UISetAppIconResponse(message));
      } else {
        command.reset(new commands::UISetAppIconRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_GetSupportedLanguages: {
      if (is_response) {
        command.reset(new commands::UIGetSupportedLanguagesResponse(message));
      } else {
        command.reset(new commands::UIGetSupportedLanguagesRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_GetLanguage: {
      if (is_response) {
        command.reset(new commands::UIGetLanguageResponse(message));
      } else {
        command.reset(new commands::UIGetLanguageRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_GetCapabilities: {
      if (is_response) {
        command.reset(new commands::UIGetCapabilitiesResponse(message));
      } else {
        command.reset(new commands::UIGetCapabilitiesRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_ChangeRegistration: {
      if (is_response) {
        command.reset(new commands::UIChangeRegistratioResponse(message));
      } else {
        command.reset(new commands::UIChangeRegistrationRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_PerformAudioPassThru: {
      if (is_response) {
        command.reset(new commands::UIPerformAudioPassThruResponse(message));
      } else {
        command.reset(new commands::UIPerformAudioPassThruRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_EndAudioPassThru: {
      if (is_response) {
        command.reset(new commands::UIEndAudioPassThruResponse(message));
      } else {
        command.reset(new commands::UIEndAudioPassThruRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_Alert: {
      if (is_response) {
        command.reset(new commands::UIAlertResponse(message));
      } else {
        command.reset(new commands::UIAlertRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_IsReady: {
      if (is_response) {
        command.reset(new commands::VRIsReadyResponse(message));
      } else {
        command.reset(new commands::VRIsReadyRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_AddCommand: {
      if (is_response) {
        command.reset(new commands::VRAddCommandResponse(message));
      } else {
        command.reset(new commands::VRAddCommandRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_DeleteCommand: {
      if (is_response) {
        command.reset(new commands::VRDeleteCommandResponse(message));
      } else {
        command.reset(new commands::VRDeleteCommandRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_ChangeRegistration: {
      if (is_response) {
        command.reset(new commands::VRChangeRegistrationResponse(message));
      } else {
        command.reset(new commands::VRChangeRegistrationRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_GetSupportedLanguages: {
      if (is_response) {
        command.reset(new commands::VRGetSupportedLanguagesResponse(message));
      } else {
        command.reset(new commands::VRGetSupportedLanguagesRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_GetLanguage: {
      if (is_response) {
        command.reset(new commands::VRGetLanguageResponse(message));
      } else {
        command.reset(new commands::VRGetLanguageRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_GetCapabilities: {
      if (is_response) {
        command.reset(new commands::VRGetCapabilitiesResponse(message));
      } else {
        command.reset(new commands::VRGetCapabilitiesRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_IsReady: {
      if (is_response) {
        command.reset(new commands::TTSIsReadyResponse(message));
      } else {
        command.reset(new commands::TTSIsReadyRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_ChangeRegistration: {
      if (is_response) {
        command.reset(new commands::TTSChangeRegistratioResponse(message));
      } else {
        command.reset(new commands::TTSChangeRegistrationRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_GetSupportedLanguages: {
      if (is_response) {
        command.reset(new commands::TTSGetSupportedLanguagesResponse(message));
      } else {
        command.reset(new commands::TTSGetSupportedLanguagesRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_StopSpeaking: {
      if (is_response) {
        command.reset(new commands::TTSStopSpeakingResponse(message));
      } else {
        command.reset(new commands::TTSStopSpeakingRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_GetLanguage: {
      if (is_response) {
        command.reset(new commands::TTSGetLanguageResponse(message));
      } else {
        command.reset(new commands::TTSGetLanguageRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      if (is_response) {
        command.reset(new commands::TTSSpeakResponse(message));
      } else {
        command.reset(new commands::TTSSpeakRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_SetGlobalProperties: {
      if (is_response) {
        command.reset(new commands::TTSSetGlobalPropertiesResponse(message));
      } else {
        command.reset(new commands::TTSSetGlobalPropertiesRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_GetCapabilities: {
      if (is_response) {
        command.reset(new commands::TTSGetCapabilitiesResponse(message));
      } else {
        command.reset(new commands::TTSGetCapabilitiesRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_Started: {
      command.reset(new commands::OnTTSStartedNotification(message));
      break;
    }
    case hmi_apis::FunctionID::TTS_Stopped: {
      command.reset(new commands::OnTTSStoppedNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppActivated: {
      command.reset(new commands::OnAppActivatedNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnExitApplication: {
      command.reset(new commands::OnExitApplicationNotification(message));
      break;
    }
    case hmi_apis::FunctionID::UI_Show: {
      if (is_response) {
        command.reset(new commands::UIShowResponse(message));
      } else {
        command.reset(new commands::UIShowRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_Slider: {
      if (is_response) {
        command.reset(new commands::UISliderResponse(message));
      } else {
        command.reset(new commands::UISliderRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_ClosePopUp: {
      if (is_response) {
        command.reset(new commands::ClosePopupResponse(message));
      } else {
        command.reset(new commands::ClosePopupRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::UI_IsReady: {
      if (is_response) {
        command.reset(new commands::UIIsReadyResponse(message));
      } else {
        command.reset(new commands::UIIsReadyRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_IsReady: {
      if (is_response) {
        command.reset(new commands::VIIsReadyResponse(message));
      } else {
        command.reset(new commands::VIIsReadyRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_ReadDID: {
      if (is_response) {
        command.reset(new commands::VIReadDIDResponse(message));
      } else {
        command.reset(new commands::VIReadDIDRequest(message));
      }
      break;
    }
#ifdef HMI_DBUS_API
    case hmi_apis::FunctionID::VehicleInfo_GetGpsData: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetGpsData > (message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetGpsData > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetSpeed: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetSpeed > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetSpeed > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetRpm: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetRpm > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetRpm > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetFuelLevel: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetFuelLevel > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetFuelLevel > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetFuelLevelState: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetFuelLevelState > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetFuelLevelState > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetInstantFuelConsumption: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetInstantFuelConsumption > (
                message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetInstantFuelConsumption > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetExternalTemperature: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetExternalTemperature > (
                message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetExternalTemperature > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetPrndl: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetPrndl > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetPrndl > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetVin: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetVin > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetVin > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetTirePressure: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetTirePressure > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetTirePressure > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetOdometer: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetOdometer > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetOdometer > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetBeltStatus: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetBeltStatus > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetBeltStatus > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetBodyInformation: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetBodyInformation > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetBodyInformation > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetDeviceStatus: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetDeviceStatus > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetDeviceStatus > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetDriverBraking: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetDriverBraking > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetDriverBraking > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetWiperStatus: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetWiperStatus > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetWiperStatus > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetHeadLampStatus: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetHeadLampStatus > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetHeadLampStatus > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetEngineTorque: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetEngineTorque > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetEngineTorque > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetAccPedalPosition: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetAccPedalPosition > (
                message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetAccPedalPosition > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetSteeringWheelAngle: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetSteeringWheelAngle > (
                message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetSteeringWheelAngle > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetECallInfo: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetECallInfo > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetECallInfo > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetAirbagStatus: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetAirbagStatus > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetAirbagStatus > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetEmergencyEvent: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetEmergencyEvent > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetEmergencyEvent > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetClusterModeStatus: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetClusterModeStatus > (
                message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetClusterModeStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetMyKey: {
      if (is_response)
        command.reset(
          new commands::VIGetVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetMyKey > (message));
      else
        command.reset(
          new commands::VIGetVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_GetMyKey > (message));
      break;
    }
#else
    case hmi_apis::FunctionID::VehicleInfo_GetVehicleData: {
      if (is_response) {
        command.reset(new commands::VIGetVehicleDataResponse(message));
      } else {
        command.reset(new commands::VIGetVehicleDataRequest(message));
      }
      break;
    }
#endif // #ifdef HMI_DBUS_API
    case hmi_apis::FunctionID::VehicleInfo_GetDTCs: {
      if (is_response) {
        command.reset(new commands::VIGetDTCsResponse(message));
      } else {
        command.reset(new commands::VIGetDTCsRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_DiagnosticMessage: {
      if (is_response) {
        command.reset(new commands::VIDiagnosticMessageResponse(message));
      } else {
        command.reset(new commands::VIDiagnosticMessageRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetVehicleType: {
      if (is_response) {
        command.reset(new commands::VIGetVehicleTypeResponse(message));
      } else {
        command.reset(new commands::VIGetVehicleTypeRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_IsReady: {
      if (is_response) {
        command.reset(new commands::NaviIsReadyResponse(message));
      } else {
        command.reset(new commands::NaviIsReadyRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_AlertManeuver: {
      if (is_response) {
        command.reset(new commands::NaviAlertManeuverResponse(message));
      } else {
        command.reset(new commands::NaviAlertManeuverRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_UpdateTurnList: {
      if (is_response) {
        command.reset(new commands::NaviUpdateTurnListResponse(message));
      } else {
        command.reset(new commands::NaviUpdateTurnListRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_ShowConstantTBT: {
      if (is_response) {
        command.reset(new commands::NaviShowConstantTBTResponse(message));
      } else {
        command.reset(new commands::NaviShowConstantTBTRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::Buttons_GetCapabilities: {
      if (is_response) {
        command.reset(new commands::ButtonGetCapabilitiesResponse(message));
      } else {
        command.reset(new commands::ButtonGetCapabilitiesRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_OnAllowSDLFunctionality: {
      command.reset(new commands::OnAllowSDLFunctionalityNotification(message));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnSDLConsentNeeded: {
      command.reset(new commands::OnSDLConsentNeededNotification(message));
      break;
    }
    case hmi_apis::FunctionID::SDL_UpdateSDL: {
      if (is_response) {
        command.reset(new commands::UpdateSDLResponse(message));
      } else {
        command.reset(new commands::UpdateSDLRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnIgnitionCycleOver: {
      command.reset(new commands::OnIgnitionCycleOverNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSystemInfoChanged: {
      command.reset(new commands::OnSystemInfoChangedNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnReady: {
      command.reset(new commands::OnReadyNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnDeviceChosen: {
      command.reset(new commands::OnDeviceChosenNotification(message));
      break;
    }
    case hmi_apis::FunctionID::UI_OnSystemContext: {
      command.reset(new commands::OnSystemContextNotification(message));
      break;
    }
    case hmi_apis::FunctionID::UI_OnDriverDistraction: {
      command.reset(
          new commands::hmi::OnDriverDistractionNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnUpdateDeviceList: {
      command.reset(new commands::OnUpdateDeviceList(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppRegistered: {
      command.reset(new commands::OnAppRegisteredNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered: {
      command.reset(new commands::OnAppUnregisteredNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnFindApplications: {
      command.reset(new commands::OnFindApplications(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_UpdateAppList: {
      if (is_response) {
        command.reset(new commands::UpdateAppListResponse(message));
      } else {
        command.reset(new commands::UpdateAppListRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VR_Started: {
      command.reset(new commands::OnVRStartedNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VR_Stopped: {
      command.reset(new commands::OnVRStoppedNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VR_OnCommand: {
      command.reset(new commands::OnVRCommandNotification(message));
      break;
    }
    case hmi_apis::FunctionID::UI_OnCommand: {
      command.reset(new commands::OnUICommandNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppDeactivated: {
      command.reset(new commands::OnAppDeactivatedNotification(message));
      break;
    }
    case hmi_apis::FunctionID::UI_OnLanguageChange: {
      command.reset(new commands::OnUILanguageChangeNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VR_OnLanguageChange: {
      command.reset(new commands::OnVRLanguageChangeNotification(message));
      break;
    }
    case hmi_apis::FunctionID::TTS_OnLanguageChange: {
      command.reset(new commands::OnTTSLanguageChangeNotification(message));
      break;
    }
    case hmi_apis::FunctionID::Buttons_OnButtonEvent: {
      command.reset(new commands::hmi::OnButtonEventNotification(message));
      break;
    }
    case hmi_apis::FunctionID::Buttons_OnButtonPress: {
      command.reset(new commands::hmi::OnButtonPressNotification(message));
      break;
    }
    case hmi_apis::FunctionID::Buttons_OnButtonSubscription: {
      command.reset(new commands::hmi::OnButtonSubscriptionNotification(message));
      break;
    }
#ifdef HMI_DBUS_API
    case hmi_apis::FunctionID::VehicleInfo_SubscribeGps: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeGps > (message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeGps > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeSpeed: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeSpeed > (message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeSpeed > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeRpm: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeRpm > (message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeRpm > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeFuelLevel: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeFuelLevel > (message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeFuelLevel > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeFuelLevel_State: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeFuelLevel_State > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeFuelLevel_State > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeInstantFuelConsumption: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeInstantFuelConsumption > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeInstantFuelConsumption > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeExternalTemperature: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeExternalTemperature > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeExternalTemperature > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribePrndl: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribePrndl > (message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribePrndl > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeVin: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeVin > (message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeVin > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeTirePressure: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeTirePressure > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeTirePressure > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeOdometer: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeOdometer > (message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeOdometer > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeBeltStatus: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeBeltStatus > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeBeltStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeBodyInformation: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeBodyInformation > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeBodyInformation > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeDeviceStatus: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeDeviceStatus > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeDeviceStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeDriverBraking: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeDriverBraking > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeDriverBraking > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeWiperStatus: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeWiperStatus > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeWiperStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeHeadLampStatus: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeHeadLampStatus > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeHeadLampStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeEngineTorque: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeEngineTorque > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeEngineTorque > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeAccPedalPosition: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeAccPedalPosition > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeAccPedalPosition > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeSteeringWheelAngle: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeSteeringWheelAngle > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeSteeringWheelAngle > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeECallInfo: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeECallInfo > (message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeECallInfo > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeAirbagStatus: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeAirbagStatus > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeAirbagStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeEmergencyEvent: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeEmergencyEvent > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeEmergencyEvent > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeClusterModeStatus: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeClusterModeStatus > (
                message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeClusterModeStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_SubscribeMyKey: {
      if (is_response)
        command.reset(
          new commands::VISubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeMyKey > (message));
      else
        command.reset(
          new commands::VISubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_SubscribeMyKey > (message));
      break;
    }
#else
    case hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData: {
      if (is_response) {
        command.reset(new commands::VISubscribeVehicleDataResponse(message));
      } else {
        command.reset(new commands::VISubscribeVehicleDataRequest(message));
      }
      break;
    }
#endif // #ifdef HMI_DBUS_API
#ifdef HMI_DBUS_API
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeGps: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeGps > (message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeGps > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeSpeed: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeSpeed > (message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeSpeed > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeRpm: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeRpm > (message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeRpm > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeFuelLevel: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeFuelLevel > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeFuelLevel > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeFuelLevel_State: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeFuelLevel_State > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeFuelLevel_State > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeInstantFuelConsumption: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeInstantFuelConsumption > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeInstantFuelConsumption > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeExternalTemperature: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeExternalTemperature > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeExternalTemperature > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribePrndl: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribePrndl > (message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribePrndl > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeVin: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeVin > (message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeVin > (message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeTirePressure: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeTirePressure > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeTirePressure > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeOdometer: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeOdometer > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeOdometer > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeBeltStatus: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeBeltStatus > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeBeltStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeBodyInformation: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeBodyInformation > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeBodyInformation > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeDeviceStatus: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeDeviceStatus > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeDeviceStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeDriverBraking: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeDriverBraking > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeDriverBraking > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeWiperStatus: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeWiperStatus > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeWiperStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeHeadLampStatus: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeHeadLampStatus > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeHeadLampStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeEngineTorque: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeEngineTorque > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeEngineTorque > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeAccPedalPosition: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeAccPedalPosition > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeAccPedalPosition > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeSteeringWheelAngle: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeSteeringWheelAngle > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeSteeringWheelAngle > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeECallInfo: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeECallInfo > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeECallInfo > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeAirbagStatus: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeAirbagStatus > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeAirbagStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeEmergencyEvent: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeEmergencyEvent > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeEmergencyEvent > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeClusterModeStatus: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeClusterModeStatus > (
                message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeClusterModeStatus > (
                message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeMyKey: {
      if (is_response)
        command.reset(
          new commands::VIUnsubscribeVehicleDataResponseTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeMyKey > (message));
      else
        command.reset(
          new commands::VIUnsubscribeVehicleDataRequestTemplate <
          hmi_apis::FunctionID::VehicleInfo_UnsubscribeMyKey > (message));
      break;
    }
#else
    case hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData: {
      if (is_response) {
        command.reset(new commands::VIUnsubscribeVehicleDataResponse(message));
      } else {
        command.reset(new commands::VIUnsubscribeVehicleDataRequest(message));
      }
      break;
    }
#endif // #ifdef HMI_DBUS_API
#ifdef HMI_DBUS_API
    case hmi_apis::FunctionID::VehicleInfo_OnGpsData: {
      command.reset(new commands::OnVIGpsDataNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnSpeed: {
      command.reset(new commands::OnVISpeedNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnRpm: {
      command.reset(new commands::OnVIRpmNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnFuelLevel: {
      command.reset(new commands::OnVIFuelLevelNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnFuelLevelState: {
      command.reset(new commands::OnVIFuelLevelStateNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnInstantFuelConsumption: {
      command.reset(
          new commands::OnVIInstantFuelConsumptionNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnExternalTemperature: {
      command.reset(new commands::OnVIExternalTemperatureNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnVin: {
      command.reset(new commands::OnVIVinNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnPrndl: {
      command.reset(new commands::OnVIPrndlNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnTirePressure: {
      command.reset(new commands::OnVITirePressureNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnOdometer: {
      command.reset(new commands::OnVIOdometerNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnBeltStatus: {
      command.reset(new commands::OnVIBeltStatusNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnBodyInformation: {
      command.reset(new commands::OnVIBodyInformationNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnDeviceStatus: {
      command.reset(new commands::OnVIDeviceStatusNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnDriverBraking: {
      command.reset(new commands::OnVIDriverBrakingNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnWiperStatus: {
      command.reset(new commands::OnVIWiperStatusNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnHeadLampStatus: {
      command.reset(new commands::OnVIHeadLampStatusNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnEngineTorque: {
      command.reset(new commands::OnVIEngineTorqueNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnAccPedalPosition: {
      command.reset(new commands::OnVIAccPedalPositionNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnSteeringWheelAngle: {
      command.reset(new commands::OnVISteeringWheelAngleNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_OnMyKey: {
      command.reset(new commands::OnVIMyKeyNotification(message));
      break;
    }
#else
    case hmi_apis::FunctionID::VehicleInfo_OnVehicleData: {
      command.reset(new commands::OnVIVehicleDataNotification(message));
      break;
    }
#endif // #ifdef HMI_DBUS_API
    case hmi_apis::FunctionID::Navigation_OnTBTClientState: {
      command.reset(new commands::OnNaviTBTClientStateNotification(message));
      break;
    }
    case hmi_apis::FunctionID::UI_OnKeyboardInput: {
      command.reset(new commands::hmi::OnUIKeyBoardInputNotification(message));
      break;
    }
    case hmi_apis::FunctionID::UI_OnTouchEvent: {
      command.reset(new commands::hmi::OnUITouchEventNotification(message));
      break;
    }
    case hmi_apis::FunctionID::UI_OnResetTimeout: {
      command.reset(new commands::hmi::OnUIResetTimeoutNotification(message));
      break;
    }
    case hmi_apis::FunctionID::Navigation_StartStream: {
      if (is_response) {
        command.reset(new commands::NaviStartStreamResponse(message));
      } else {
        command.reset(new commands::NaviStartStreamRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_StopStream: {
      if (is_response) {
        command.reset(new commands::NaviStopStreamResponse(message));
      } else {
        command.reset(new commands::NaviStopStreamRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_StartAudioStream: {
      if (is_response) {
        command.reset(new commands::AudioStartStreamResponse(message));
      } else {
        command.reset(new commands::AudioStartStreamRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_StopAudioStream: {
      if (is_response) {
        command.reset(new commands::AudioStopStreamResponse(message));
      } else {
        command.reset(new commands::AudioStopStreamRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_OnAudioDataStreaming: {
      command.reset(new commands::OnAudioDataStreamingNotification(message));
      break;
    }
    case hmi_apis::FunctionID::Navigation_OnVideoDataStreaming: {
      command.reset(new commands::OnVideoDataStreamingNotification(message));
      break;
    }
    case hmi_apis::FunctionID::VR_PerformInteraction: {
      if (is_response) {
        command.reset(new commands::VRPerformInteractionResponse(message));
      } else {
        command.reset(new commands::VRPerformInteractionRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSystemRequest: {
      command.reset(new commands::OnSystemRequestNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnPutFile: {
      command.reset(new commands::OnPutFileNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnResumeAudioSource: {
      command.reset(new commands::OnResumeAudioSourceNotification(message));
      break;
    }
    case hmi_apis::FunctionID::UI_SetDisplayLayout: {
      if (is_response) {
        command.reset(new commands::UiSetDisplayLayoutResponse(message));
      } else {
        command.reset(new commands::UiSetDisplayLayoutRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSDLClose: {
      command.reset(new commands::OnSDLCloseNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSDLPersistenceComplete: {
      command.reset(new commands::OnSDLPersistenceCompleteNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnFileRemoved: {
      command.reset(new commands::OnFileRemovedNotification(message));
      break;
    }
    case hmi_apis::FunctionID::UI_OnRecordStart: {
      command.reset(new commands::OnRecordStartdNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_SystemRequest: {
      if (is_response) {
        command.reset(new commands::BasicCommunicationSystemResponse(message));
      } else {
        command.reset(new commands::BasicCommunicationSystemRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::Navigation_SendLocation: {
      if (is_response) {
        command.reset(new commands::NaviSendLocationResponse(message));
      } else {
        command.reset(new commands::NaviSendLocationRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::SDL_AddStatisticsInfo: {
      command.reset(new commands::AddStatisticsInfoNotification(message));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnSystemError: {
      command.reset(new commands::OnSystemErrorNotification(message));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnReceivedPolicyUpdate: {
      command.reset(new commands::OnReceivedPolicyUpdate(message));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnPolicyUpdate: {
      command.reset(new commands::OnPolicyUpdate(message));
      break;
    }
    case hmi_apis::FunctionID::SDL_OnDeviceStateChanged: {
      command.reset(new commands::OnDeviceStateChangedNotification(message));
      break;
    }
    case hmi_apis::FunctionID::TTS_OnResetTimeout: {
      command.reset(new commands::hmi::OnTTSResetTimeoutNotification(message));
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_DialNumber: {
      if (is_response) {
        command.reset(new commands::hmi::DialNumberResponse(message));
      } else {
        command.reset(new commands::hmi::DialNumberRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnEventChanged: {
      command.reset(new commands::OnEventChangedNotification(message));
      break;
    }
  }

  return command;
}

}  // namespace application_manager
