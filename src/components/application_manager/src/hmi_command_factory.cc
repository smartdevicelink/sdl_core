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
#include "application_manager/commands/hmi/activate_app_request.h"
#include "application_manager/commands/hmi/activate_app_response.h"
#include "application_manager/commands/hmi/mixing_audio_supported_request.h"
#include "application_manager/commands/hmi/mixing_audio_supported_response.h"
#include "application_manager/commands/hmi/on_app_activated_notification.h"
#include "application_manager/commands/hmi/on_exit_all_applications_notification.h"
#include "application_manager/commands/hmi/on_exit_application_notification.h"
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
#include "application_manager/commands/hmi/ui_set_icon_request.h"
#include "application_manager/commands/hmi/ui_set_icon_response.h"
#include "application_manager/commands/hmi/ui_perform_audio_pass_thru_response.h"
#include "application_manager/commands/hmi/ui_perform_audio_pass_thru_request.h"
#include "application_manager/commands/hmi/ui_end_audio_pass_thru_response.h"
#include "application_manager/commands/hmi/ui_end_audio_pass_thru_request.h"
#include "application_manager/commands/hmi/ui_perform_interaction_request.h"
#include "application_manager/commands/hmi/ui_perform_interaction_response.h"
#include "application_manager/commands/hmi/ui_show_vr_help_request.h"
#include "application_manager/commands/hmi/ui_show_vr_help_response.h"
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
#include "application_manager/commands/hmi/vi_is_ready_request.h"
#include "application_manager/commands/hmi/vi_is_ready_response.h"
#include "application_manager/commands/hmi/vi_read_did_request.h"
#include "application_manager/commands/hmi/vi_read_did_response.h"

#ifdef WEB_HMI
#include "application_manager/commands/hmi/vi_get_vehicle_data_request.h"
#include "application_manager/commands/hmi/vi_get_vehicle_data_response.h"
#include "application_manager/commands/hmi/on_vi_vehicle_data_notification.h"
#endif // #ifdef WEB_HMI

#ifdef QT_HMI
#include "application_manager/commands/hmi/vi_get_gps_data_request.h"
#include "application_manager/commands/hmi/vi_get_gps_data_response.h"
#include "application_manager/commands/hmi/vi_get_speed_request.h"
#include "application_manager/commands/hmi/vi_get_speed_response.h"
#include "application_manager/commands/hmi/vi_get_rpm_request.h"
#include "application_manager/commands/hmi/vi_get_rpm_response.h"
#include "application_manager/commands/hmi/vi_get_fuel_level_request.h"
#include "application_manager/commands/hmi/vi_get_fuel_level_response.h"
#include "application_manager/commands/hmi/vi_get_fuel_level_state_request.h"
#include "application_manager/commands/hmi/vi_get_fuel_level_state_response.h"
#include "application_manager/commands/hmi/vi_get_instant_fuel_consumption_request.h"
#include "application_manager/commands/hmi/vi_get_instant_fuel_consumption_response.h"
#include "application_manager/commands/hmi/vi_get_external_temperature_request.h"
#include "application_manager/commands/hmi/vi_get_external_temperature_response.h"
#include "application_manager/commands/hmi/vi_get_vin_request.h"
#include "application_manager/commands/hmi/vi_get_vin_response.h"
#include "application_manager/commands/hmi/vi_get_prndl_request.h"
#include "application_manager/commands/hmi/vi_get_prndl_response.h"
#include "application_manager/commands/hmi/vi_get_tire_pressure_request.h"
#include "application_manager/commands/hmi/vi_get_tire_pressure_response.h"
#include "application_manager/commands/hmi/vi_get_odometer_request.h"
#include "application_manager/commands/hmi/vi_get_odometer_response.h"
#include "application_manager/commands/hmi/vi_get_belt_status_request.h"
#include "application_manager/commands/hmi/vi_get_belt_status_response.h"
#include "application_manager/commands/hmi/vi_get_body_information_request.h"
#include "application_manager/commands/hmi/vi_get_body_information_response.h"
#include "application_manager/commands/hmi/vi_get_device_status_request.h"
#include "application_manager/commands/hmi/vi_get_device_status_response.h"
#include "application_manager/commands/hmi/vi_get_driver_braking_request.h"
#include "application_manager/commands/hmi/vi_get_driver_braking_response.h"
#include "application_manager/commands/hmi/vi_get_wiper_status_request.h"
#include "application_manager/commands/hmi/vi_get_wiper_status_response.h"
#include "application_manager/commands/hmi/vi_get_head_lamp_status_request.h"
#include "application_manager/commands/hmi/vi_get_head_lamp_status_response.h"
#include "application_manager/commands/hmi/vi_get_engine_torque_request.h"
#include "application_manager/commands/hmi/vi_get_engine_torque_response.h"
#include "application_manager/commands/hmi/vi_get_acc_pedal_position_request.h"
#include "application_manager/commands/hmi/vi_get_acc_pedal_position_response.h"
#include "application_manager/commands/hmi/vi_get_steering_wheel_angle_request.h"
#include "application_manager/commands/hmi/vi_get_steering_wheel_angle_response.h"
#include "application_manager/commands/hmi/vi_get_my_key_request.h"
#include "application_manager/commands/hmi/vi_get_my_key_response.h"
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
#endif // #ifdef QT_HMI

#include "application_manager/commands/hmi/vi_get_dtcs_request.h"
#include "application_manager/commands/hmi/vi_get_dtcs_response.h"
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
#include "application_manager/commands/hmi/on_system_context_notification.h"
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
#include "application_manager/commands/hmi/on_navi_tbt_client_state_notification.h"
#include "application_manager/commands/hmi/on_button_event_notification.h"
#include "application_manager/commands/hmi/on_button_press_notification.h"
#include "application_manager/commands/hmi/on_show_notification.h"
#include "application_manager/commands/hmi/on_ui_keyboard_input_notification.h"
#include "application_manager/commands/hmi/on_ui_touch_event_notification.h"
#include "application_manager/commands/hmi/on_ui_reset_timeout_notification.h"
#include "application_manager/commands/hmi/navi_start_stream_request.h"
#include "application_manager/commands/hmi/navi_start_stream_response.h"
#include "application_manager/commands/hmi/navi_stop_stream_request.h"
#include "application_manager/commands/hmi/navi_stop_stream_response.h"

namespace application_manager {

log4cxx::LoggerPtr HMICommandFactory::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("ApplicationManager"));

CommandSharedPtr HMICommandFactory::CreateCommand(
  const MessageSharedPtr& message) {
  LOG4CXX_INFO(
    logger_,
    "HMICommandFactory::CreateCommand function_id: "
    << (*message)[strings::params][strings::function_id].asInt());

  CommandSharedPtr command(
    new application_manager::commands::CommandImpl(message));

  bool is_response = false;
  if ((*message)[strings::params][strings::message_type]
      == MessageType::kResponse) {
    is_response = true;
    LOG4CXX_INFO(logger_, "HMICommandFactory::CreateCommand response");
  } else if ((*message)[strings::params][strings::message_type]
             == MessageType::kErrorResponse) {
    is_response = true;
    LOG4CXX_INFO(logger_, "HMICommandFactory::CreateCommand error response");
  } else {
    LOG4CXX_INFO(logger_, "HMICommandFactory::CreateCommand request");
  }

  switch ((*message)[strings::params][strings::function_id].asInt()) {
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
    case hmi_apis::FunctionID::BasicCommunication_AllowAllApps: {
      if (is_response) {
        command.reset(new commands::AllowAllAppsResponse(message));
      } else {
        command.reset(new commands::AllowAllAppsRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_AllowApp: {
      if (is_response) {
        command.reset(new commands::AllowAppResponse(message));
      } else {
        command.reset(new commands::AllowAppRequest(message));
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
    case  hmi_apis::FunctionID::BasicCommunication_MixingAudioSupported: {
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
    case hmi_apis::FunctionID::UI_ShowVrHelp: {
      if (is_response) {
        command.reset(new commands::UIShowVrHelpResponse(message));
      } else {
        command.reset(new commands::UIShowVrHelpRequest(message));
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
        command.reset(new commands::UISetIconResponse(message));
      } else {
        command.reset(new commands::UISetIconRequest(message));
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
#ifdef WEB_HMI
    case hmi_apis::FunctionID::VehicleInfo_GetVehicleData: {
      if (is_response) {
        command.reset(new commands::VIGetVehicleDataResponse(message));
      } else {
        command.reset(new commands::VIGetVehicleDataRequest(message));
      }
      break;
    }
#endif // #ifdef WEB_HMI
#ifdef QT_HMI
    case hmi_apis::FunctionID::VehicleInfo_GetGpsData: {
      if (is_response) {
        command.reset(new commands::VIGetGpsDataResponse(message));
      } else {
        command.reset(new commands::VIGetGpsDataRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetSpeed: {
      if (is_response) {
        command.reset(new commands::VIGetSpeedResponse(message));
      } else {
        command.reset(new commands::VIGetSpeedRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetRpm: {
      if (is_response) {
        command.reset(new commands::VIGetRpmResponse(message));
      } else {
        command.reset(new commands::VIGetRpmRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetFuelLevel: {
      if (is_response) {
        command.reset(new commands::VIGetFuelLevelResponse(message));
      } else {
        command.reset(new commands::VIGetFuelLevelRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetFuelLevelState: {
      if (is_response) {
        command.reset(new commands::VIGetFuelLevelStateResponse(message));
      } else {
        command.reset(new commands::VIGetFuelLevelStateRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetInstantFuelConsumption: {
      if (is_response) {
        command.reset(new commands::VIGetInstantFuelConsumptionResponse(message));
      } else {
        command.reset(new commands::VIGetInstantFuelConsumptionRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetExternalTemperature: {
      if (is_response) {
        command.reset(new commands::VIGetExternalTemperatureResponse(message));
      } else {
        command.reset(new commands::VIGetExternalTemperatureRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetVin: {
      if (is_response) {
        command.reset(new commands::VIGetVinResponse(message));
      } else {
        command.reset(new commands::VIGetVinRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetPrndl: {
      if (is_response) {
        command.reset(new commands::VIGetPrndlResponse(message));
      } else {
        command.reset(new commands::VIGetPrndlRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetTirePressure: {
      if (is_response) {
        command.reset(new commands::VIGetTirePressureResponse(message));
      } else {
        command.reset(new commands::VIGetTirePressureRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetOdometer: {
      if (is_response) {
        command.reset(new commands::VIGetOdometerResponse(message));
      } else {
        command.reset(new commands::VIGetOdometerRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetBeltStatus: {
      if (is_response) {
        command.reset(new commands::VIGetBeltStatusResponse(message));
      } else {
        command.reset(new commands::VIGetBeltStatusRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetBodyInformation: {
      if (is_response) {
        command.reset(new commands::VIGetBodyInformationResponse(message));
      } else {
        command.reset(new commands::VIGetBodyInformationRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetDeviceStatus: {
      if (is_response) {
        command.reset(new commands::VIGetDeviceStatusResponse(message));
      } else {
        command.reset(new commands::VIGetDeviceStatusRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetDriverBraking: {
      if (is_response) {
        command.reset(new commands::VIGetDriverBrakingResponse(message));
      } else {
        command.reset(new commands::VIGetDriverBrakingRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetWiperStatus: {
      if (is_response) {
        command.reset(new commands::VIGetWiperStatusResponse(message));
      } else {
        command.reset(new commands::VIGetWiperStatusRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetHeadLampStatus: {
      if (is_response) {
        command.reset(new commands::VIGetHeadLampStatusResponse(message));
      } else {
        command.reset(new commands::VIGetHeadLampStatusRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetEngineTorque: {
      if (is_response) {
        command.reset(new commands::VIGetEngineTorqueResponse(message));
      } else {
        command.reset(new commands::VIGetEngineTorqueRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetAccPedalPosition: {
      if (is_response) {
        command.reset(new commands::VIGetAccPedalPositionResponse(message));
      } else {
        command.reset(new commands::VIGetAccPedalPositionRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetSteeringWheelAngle: {
      if (is_response) {
        command.reset(new commands::VIGetSteeringWheelAngleResponse(message));
      } else {
        command.reset(new commands::VIGetSteeringWheelAngleRequest(message));
      }
      break;
    }
    case hmi_apis::FunctionID::VehicleInfo_GetMyKey: {
      if (is_response) {
        command.reset(new commands::VIGetMyKeyResponse(message));
      } else {
        command.reset(new commands::VIGetMyKeyRequest(message));
      }
      break;
    }
#endif // #ifdef QT_HMI
    case hmi_apis::FunctionID::VehicleInfo_GetDTCs: {
      if (is_response) {
        command.reset(new commands::VIGetDTCsResponse(message));
      } else {
        command.reset(new commands::VIGetDTCsRequest(message));
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
    case hmi_apis::FunctionID::BasicCommunication_PlayTone: {
      command.reset(new commands::OnPlayToneNotification(message));
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
#ifdef WEB_HMI
    case hmi_apis::FunctionID::VehicleInfo_OnVehicleData: {
      command.reset(new commands::OnVIVehicleDataNotification(message));
      break;
    }
#endif // #ifdef WEB_HMI
#ifdef QT_HMI
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
      command.reset(new commands::OnVIInstantFuelConsumptionNotification(message));
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
#endif // #ifdef QT_HMI
    case hmi_apis::FunctionID::UI_ShowNotification: {
      command.reset(new commands::OnShowNotification(message));
      break;
    }
    case hmi_apis::FunctionID::Navigation_OnTBTClientState: {
      command.reset(new commands::OnNaviTBTClientStateNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::UI_OnKeyboardInput: {
      command.reset(new commands::hmi::OnUIKeyBoardInputNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::UI_OnTouchEvent: {
      command.reset(new commands::hmi::OnUITouchEventNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::UI_OnResetTimeout: {
      command.reset(new commands::hmi::OnUIResetTimeoutNotification(message));
      break;
    }
    case  hmi_apis::FunctionID::Navigation_StartStream: {
      if (is_response) {
        command.reset(new commands::NaviStartStreamResponse(message));
      } else {
        command.reset(new commands::NaviStartStreamRequest(message));
      }
      break;
    }
    case  hmi_apis::FunctionID::Navigation_StopStream: {
      if (is_response) {
        command.reset(new commands::NaviStopStreamResponse(message));
      } else {
        command.reset(new commands::NaviStopStreamRequest(message));
      }
      break;
    }
  }

  return command;
}

}  // namespace application_manager
