/*
 Copyright (c) 2018, Ford Motor Company
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

#include "sdl_rpc_plugin/hmi_command_factory.h"

#include "application_manager/message.h"
#include "interfaces/HMI_API.h"

#include "sdl_rpc_plugin/commands/hmi/update_device_list_request.h"
#include "sdl_rpc_plugin/commands/hmi/update_device_list_response.h"
#include "sdl_rpc_plugin/commands/hmi/on_update_device_list.h"
#include "sdl_rpc_plugin/commands/hmi/on_start_device_discovery.h"
#include "sdl_rpc_plugin/commands/hmi/update_app_list_request.h"
#include "sdl_rpc_plugin/commands/hmi/update_app_list_response.h"
#include "sdl_rpc_plugin/commands/hmi/on_find_applications.h"
#include "sdl_rpc_plugin/commands/hmi/allow_all_apps_request.h"
#include "sdl_rpc_plugin/commands/hmi/allow_all_apps_response.h"
#include "sdl_rpc_plugin/commands/hmi/allow_app_request.h"
#include "sdl_rpc_plugin/commands/hmi/allow_app_response.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_get_list_of_permissions_request.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_get_list_of_permissions_response.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_get_user_friendly_message_request.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_get_user_friendly_message_response.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_get_status_update_request.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_get_status_update_response.h"
#include "sdl_rpc_plugin/commands/hmi/on_status_update_notification.h"
#include "sdl_rpc_plugin/commands/hmi/update_sdl_request.h"
#include "sdl_rpc_plugin/commands/hmi/update_sdl_response.h"
#include "sdl_rpc_plugin/commands/hmi/activate_app_request.h"
#include "sdl_rpc_plugin/commands/hmi/activate_app_response.h"
#include "sdl_rpc_plugin/commands/hmi/mixing_audio_supported_request.h"
#include "sdl_rpc_plugin/commands/hmi/mixing_audio_supported_response.h"
#include "sdl_rpc_plugin/commands/hmi/on_allow_sdl_functionality_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_permission_changed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_permission_consent_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_activated_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_audio_data_streaming_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_video_data_streaming_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_sdl_consent_needed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_exit_all_applications_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_exit_application_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_put_file_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_resume_audio_source_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ignition_cycle_over_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_system_info_changed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/get_system_info_request.h"
#include "sdl_rpc_plugin/commands/hmi/get_system_info_response.h"
#include "sdl_rpc_plugin/commands/hmi/close_popup_request.h"
#include "sdl_rpc_plugin/commands/hmi/close_popup_response.h"
#include "sdl_rpc_plugin/commands/hmi/button_get_capabilities_request.h"
#include "sdl_rpc_plugin/commands/hmi/button_get_capabilities_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_add_command_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_add_command_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_delete_command_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_delete_command_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_add_submenu_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_add_submenu_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_delete_submenu_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_delete_submenu_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_get_supported_languages_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_get_supported_languages_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_get_language_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_get_language_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_get_capabilities_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_get_capabilities_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_change_registration_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_change_registration_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_show_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_show_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_alert_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_alert_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_is_ready_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_is_ready_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_slider_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_slider_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_media_clock_timer_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_media_clock_timer_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_global_properties_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_global_properties_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_scrollable_message_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_scrollable_message_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_app_icon_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_app_icon_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_perform_audio_pass_thru_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_perform_audio_pass_thru_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_end_audio_pass_thru_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_end_audio_pass_thru_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_perform_interaction_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_perform_interaction_response.h"
#include "sdl_rpc_plugin/commands/hmi/vr_is_ready_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_is_ready_response.h"
#include "sdl_rpc_plugin/commands/hmi/vr_add_command_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_add_command_response.h"
#include "sdl_rpc_plugin/commands/hmi/vr_delete_command_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_delete_command_response.h"
#include "sdl_rpc_plugin/commands/hmi/vr_change_registration_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_change_registration_response.h"
#include "sdl_rpc_plugin/commands/hmi/vr_get_supported_languages_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_get_supported_languages_response.h"
#include "sdl_rpc_plugin/commands/hmi/vr_get_language_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_get_language_response.h"
#include "sdl_rpc_plugin/commands/hmi/vr_get_capabilities_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_get_capabilities_response.h"
#include "sdl_rpc_plugin/commands/hmi/tts_is_ready_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_is_ready_response.h"
#include "sdl_rpc_plugin/commands/hmi/tts_change_registration_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_change_registration_response.h"
#include "sdl_rpc_plugin/commands/hmi/tts_get_supported_languages_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_get_supported_languages_response.h"
#include "sdl_rpc_plugin/commands/hmi/tts_get_language_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_get_language_response.h"
#include "sdl_rpc_plugin/commands/hmi/tts_stop_speaking_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_stop_speaking_response.h"
#include "sdl_rpc_plugin/commands/hmi/tts_speak_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_speak_response.h"
#include "sdl_rpc_plugin/commands/hmi/tts_set_global_properties_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_set_global_properties_response.h"
#include "sdl_rpc_plugin/commands/hmi/tts_get_capabilities_request.h"
#include "sdl_rpc_plugin/commands/hmi/tts_get_capabilities_response.h"
#include "sdl_rpc_plugin/commands/hmi/vr_perform_interaction_request.h"
#include "sdl_rpc_plugin/commands/hmi/vr_perform_interaction_response.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_activate_app_request.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_activate_app_response.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_permission_changed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_event_changed_notification.h"

#ifdef EXTERNAL_PROPRIETARY_MODE
#include "sdl_rpc_plugin/commands/hmi/decrypt_certificate_request.h"
#include "sdl_rpc_plugin/commands/hmi/decrypt_certificate_response.h"
#endif  // EXTERNAL_PROPRIETARY_MODE

#include "sdl_rpc_plugin/commands/hmi/navi_is_ready_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_show_constant_tbt_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_show_constant_tbt_response.h"
#include "sdl_rpc_plugin/commands/hmi/navi_is_ready_response.h"
#include "sdl_rpc_plugin/commands/hmi/navi_alert_maneuver_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_alert_maneuver_response.h"
#include "sdl_rpc_plugin/commands/hmi/navi_update_turn_list_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_update_turn_list_response.h"
#include "sdl_rpc_plugin/commands/hmi/navi_subscribe_way_points_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_subscribe_way_points_response.h"
#include "sdl_rpc_plugin/commands/hmi/navi_unsubscribe_way_points_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_unsubscribe_way_points_response.h"
#include "sdl_rpc_plugin/commands/hmi/navi_get_way_points_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_get_way_points_response.h"
#include "sdl_rpc_plugin/commands/hmi/on_ready_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_system_time_ready_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_device_chosen_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_file_removed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_system_context_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_registered_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_unregistered_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_driver_distraction_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_tts_started_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_tts_stopped_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_vr_started_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_vr_stopped_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_vr_command_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ui_command_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_app_deactivated_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ui_language_change_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_vr_language_change_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_tts_language_change_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_navi_tbt_client_state_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_navi_way_point_change_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_button_event_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_button_press_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_button_subscription_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ui_keyboard_input_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ui_touch_event_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_ui_reset_timeout_notification.h"
#include "sdl_rpc_plugin/commands/hmi/navi_set_video_config_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_set_video_config_response.h"
#include "sdl_rpc_plugin/commands/hmi/navi_start_stream_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_start_stream_response.h"
#include "sdl_rpc_plugin/commands/hmi/navi_stop_stream_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_stop_stream_response.h"
#include "sdl_rpc_plugin/commands/hmi/navi_audio_start_stream_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_audio_start_stream_response.h"
#include "sdl_rpc_plugin/commands/hmi/navi_audio_stop_stream_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_audio_stop_stream_response.h"
#include "sdl_rpc_plugin/commands/hmi/on_system_request_notification.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_display_layout_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_set_display_layout_response.h"
#include "sdl_rpc_plugin/commands/hmi/on_sdl_close_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_sdl_persistence_complete_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_record_start_notification.h"
#include "sdl_rpc_plugin/commands/hmi/add_statistics_info_notification.h"
#include "sdl_rpc_plugin/commands/hmi/on_system_error_notification.h"
#include "sdl_rpc_plugin/commands/hmi/basic_communication_system_request.h"
#include "sdl_rpc_plugin/commands/hmi/basic_communication_system_response.h"
#include "sdl_rpc_plugin/commands/hmi/basic_communication_get_system_time_request.h"
#include "sdl_rpc_plugin/commands/hmi/basic_communication_get_system_time_response.h"
#include "sdl_rpc_plugin/commands/hmi/basic_communication_on_awake_sdl.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_policy_update.h"
#include "sdl_rpc_plugin/commands/hmi/sdl_policy_update_response.h"
#include "sdl_rpc_plugin/commands/hmi/on_received_policy_update.h"
#include "sdl_rpc_plugin/commands/hmi/on_policy_update.h"
#include "sdl_rpc_plugin/commands/hmi/get_urls.h"
#include "sdl_rpc_plugin/commands/hmi/get_urls_response.h"
#include "sdl_rpc_plugin/commands/hmi/on_device_state_changed_notification.h"
#include "sdl_rpc_plugin/commands/hmi/navi_send_location_request.h"
#include "sdl_rpc_plugin/commands/hmi/navi_send_location_response.h"
#include "sdl_rpc_plugin/commands/hmi/on_tts_reset_timeout_notification.h"
#include "sdl_rpc_plugin/commands/hmi/dial_number_request.h"
#include "sdl_rpc_plugin/commands/hmi/dial_number_response.h"
#include "sdl_rpc_plugin/commands/hmi/ui_send_haptic_data_request.h"
#include "sdl_rpc_plugin/commands/hmi/ui_send_haptic_data_response.h"

#include "sdl_rpc_plugin/commands/hmi/rc_is_ready_request.h"
#include "sdl_rpc_plugin/commands/hmi/rc_is_ready_response.h"
#include "sdl_rpc_plugin/commands/hmi/rc_get_capabilities_request.h"
#include "sdl_rpc_plugin/commands/hmi/rc_get_capabilities_response.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

HMICommandFactory::HMICommandFactory(
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : application_manager_(application_manager)
    , rpc_service_(rpc_service)
    , hmi_capabilities_(hmi_capabilities)
    , policy_handler_(policy_handler) {}

CommandSharedPtr HMICommandFactory::CreateCommand(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::commands::Command::CommandSource source) {
  const hmi_apis::FunctionID::eType function_id =
      static_cast<hmi_apis::FunctionID::eType>(
          (*message)[strings::params][strings::function_id].asInt());
  LOG4CXX_DEBUG(
      logger_, "HMICommandFactory::CreateCommand function_id: " << function_id);

  const hmi_apis::messageType::eType message_type =
      static_cast<hmi_apis::messageType::eType>(
          (*message)[strings::params][strings::message_type].asInt());

  if (hmi_apis::messageType::response == message_type) {
    LOG4CXX_DEBUG(logger_, "HMICommandFactory::CreateCommand response");
  } else if ((*message)[strings::params][strings::message_type] ==
             hmi_apis::messageType::error_response) {
    LOG4CXX_DEBUG(logger_, "HMICommandFactory::CreateCommand error response");
  } else {
    LOG4CXX_DEBUG(logger_, "HMICommandFactory::CreateCommand request");
  }

  return get_creator_factory(function_id, message_type, source).create(message);
}

bool HMICommandFactory::IsAbleToProcess(
    const int32_t function_id,
    const application_manager::commands::Command::CommandSource message_source)
    const {
  using app_mngr::commands::Command;
  return get_creator_factory(
             static_cast<hmi_apis::FunctionID::eType>(function_id),
             hmi_apis::messageType::INVALID_ENUM,
             message_source).CanBeCreated();
}

CommandCreator& HMICommandFactory::get_creator_factory(
    hmi_apis::FunctionID::eType id,
    hmi_apis::messageType::eType message_type,
    application_manager::commands::Command::CommandSource source) const {
  CommandCreatorFactory factory(
      application_manager_, rpc_service_, hmi_capabilities_, policy_handler_);

  switch (static_cast<int32_t>(id)) {
    case hmi_apis::FunctionID::BasicCommunication_OnStartDeviceDiscovery: {
      return factory.GetCreator<commands::OnStartDeviceDiscovery>();
    }
    case hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UpdateDeviceListRequest>()
                 : factory.GetCreator<commands::UpdateDeviceListResponse>();
    }
    case hmi_apis::FunctionID::BasicCommunication_ActivateApp: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::ActivateAppRequest>()
                 : factory.GetCreator<commands::ActivateAppResponse>();
    }
#ifdef EXTERNAL_PROPRIETARY_MODE
    case hmi_apis::FunctionID::BasicCommunication_DecryptCertificate: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::DecryptCertificateRequest>()
                 : factory.GetCreator<commands::DecryptCertificateResponse>();
    }
#endif  // EXTERNAL_PROPRIETARY_MODE
    case hmi_apis::FunctionID::BasicCommunication_GetSystemInfo: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::GetSystemInfoRequest>()
                 : factory.GetCreator<commands::GetSystemInfoResponse>();
    }
    case hmi_apis::FunctionID::SDL_ActivateApp: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::SDLActivateAppRequest>()
                 : factory.GetCreator<commands::SDLActivateAppResponse>();
    }
    case hmi_apis::FunctionID::BasicCommunication_PolicyUpdate: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::SDLPolicyUpdate>()
                 : factory.GetCreator<commands::SDLPolicyUpdateResponse>();
    }
    case hmi_apis::FunctionID::SDL_GetURLS: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::GetUrls>()
                 : factory.GetCreator<commands::GetUrlsResponse>();
    }
    case hmi_apis::FunctionID::SDL_OnAppPermissionChanged: {
      return factory.GetCreator<commands::OnAppPermissionChangedNotification>();
    }
    case hmi_apis::FunctionID::SDL_GetListOfPermissions: {
      return hmi_apis::messageType::request == message_type
                 ? factory
                       .GetCreator<commands::SDLGetListOfPermissionsRequest>()
                 : factory
                       .GetCreator<commands::SDLGetListOfPermissionsResponse>();
    }
    case hmi_apis::FunctionID::SDL_GetUserFriendlyMessage: {
      return hmi_apis::messageType::request == message_type
                 ? factory
                       .GetCreator<commands::SDLGetUserFriendlyMessageRequest>()
                 : factory.GetCreator<
                       commands::SDLGetUserFriendlyMessageResponse>();
    }
    case hmi_apis::FunctionID::SDL_GetStatusUpdate: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::SDLGetStatusUpdateRequest>()
                 : factory.GetCreator<commands::SDLGetStatusUpdateResponse>();
    }
    case hmi_apis::FunctionID::SDL_OnStatusUpdate: {
      return factory.GetCreator<commands::OnStatusUpdateNotification>();
    }
    case hmi_apis::FunctionID::SDL_OnAppPermissionConsent: {
      return factory.GetCreator<commands::OnAppPermissionConsentNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_MixingAudioSupported: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::MixingAudioSupportedRequest>()
                 : factory.GetCreator<commands::MixingAudioSupportedResponse>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnExitAllApplications: {
      return factory.GetCreator<commands::OnExitAllApplicationsNotification>();
    }
    case hmi_apis::FunctionID::UI_AddCommand: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIAddCommandRequest>()
                 : factory.GetCreator<commands::UIAddCommandResponse>();
    }
    case hmi_apis::FunctionID::UI_DeleteCommand: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIDeleteCommandRequest>()
                 : factory.GetCreator<commands::UIDeleteCommandResponse>();
    }
    case hmi_apis::FunctionID::UI_AddSubMenu: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIAddSubmenuRequest>()
                 : factory.GetCreator<commands::UIAddSubmenuResponse>();
    }
    case hmi_apis::FunctionID::UI_DeleteSubMenu: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIDeleteSubmenuRequest>()
                 : factory.GetCreator<commands::UIDeleteSubmenuResponse>();
    }
    case hmi_apis::FunctionID::UI_SetMediaClockTimer: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UISetMediaClockTimerRequest>()
                 : factory.GetCreator<commands::UISetMediaClockTimerResponse>();
    }
    case hmi_apis::FunctionID::UI_PerformInteraction: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIPerformInteractionRequest>()
                 : factory.GetCreator<commands::UIPerformInteractionResponse>();
    }
    case hmi_apis::FunctionID::UI_SetGlobalProperties: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UISetGlobalPropertiesRequest>()
                 : factory
                       .GetCreator<commands::UISetGlobalPropertiesResponse>();
    }
    case hmi_apis::FunctionID::UI_ScrollableMessage: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIScrollableMessageRequest>()
                 : factory.GetCreator<commands::UIScrollableMessageResponse>();
    }
    case hmi_apis::FunctionID::UI_SetAppIcon: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UISetAppIconRequest>()
                 : factory.GetCreator<commands::UISetAppIconResponse>();
    }
    case hmi_apis::FunctionID::UI_GetSupportedLanguages: {
      return hmi_apis::messageType::request == message_type
                 ? factory
                       .GetCreator<commands::UIGetSupportedLanguagesRequest>()
                 : factory
                       .GetCreator<commands::UIGetSupportedLanguagesResponse>();
    }
    case hmi_apis::FunctionID::UI_GetLanguage: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIGetLanguageRequest>()
                 : factory.GetCreator<commands::UIGetLanguageResponse>();
    }
    case hmi_apis::FunctionID::UI_GetCapabilities: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIGetCapabilitiesRequest>()
                 : factory.GetCreator<commands::UIGetCapabilitiesResponse>();
    }
    case hmi_apis::FunctionID::UI_ChangeRegistration: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIChangeRegistrationRequest>()
                 : factory.GetCreator<commands::UIChangeRegistratioResponse>();
    }
    case hmi_apis::FunctionID::UI_PerformAudioPassThru: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIPerformAudioPassThruRequest>()
                 : factory
                       .GetCreator<commands::UIPerformAudioPassThruResponse>();
    }
    case hmi_apis::FunctionID::UI_EndAudioPassThru: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIEndAudioPassThruRequest>()
                 : factory.GetCreator<commands::UIEndAudioPassThruResponse>();
    }
    case hmi_apis::FunctionID::UI_Alert: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIAlertRequest>()
                 : factory.GetCreator<commands::UIAlertResponse>();
    }
    case hmi_apis::FunctionID::VR_IsReady: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::VRIsReadyRequest>()
                 : factory.GetCreator<commands::VRIsReadyResponse>();
    }
    case hmi_apis::FunctionID::VR_AddCommand: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::VRAddCommandRequest>()
                 : factory.GetCreator<commands::VRAddCommandResponse>();
    }
    case hmi_apis::FunctionID::VR_DeleteCommand: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::VRDeleteCommandRequest>()
                 : factory.GetCreator<commands::VRDeleteCommandResponse>();
    }
    case hmi_apis::FunctionID::VR_ChangeRegistration: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::VRChangeRegistrationRequest>()
                 : factory.GetCreator<commands::VRChangeRegistrationResponse>();
    }
    case hmi_apis::FunctionID::VR_GetSupportedLanguages: {
      return hmi_apis::messageType::request == message_type
                 ? factory
                       .GetCreator<commands::VRGetSupportedLanguagesRequest>()
                 : factory
                       .GetCreator<commands::VRGetSupportedLanguagesResponse>();
    }
    case hmi_apis::FunctionID::VR_GetLanguage: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::VRGetLanguageRequest>()
                 : factory.GetCreator<commands::VRGetLanguageResponse>();
    }
    case hmi_apis::FunctionID::VR_GetCapabilities: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::VRGetCapabilitiesRequest>()
                 : factory.GetCreator<commands::VRGetCapabilitiesResponse>();
    }
    case hmi_apis::FunctionID::TTS_IsReady: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::TTSIsReadyRequest>()
                 : factory.GetCreator<commands::TTSIsReadyResponse>();
    }
    case hmi_apis::FunctionID::TTS_ChangeRegistration: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::TTSChangeRegistrationRequest>()
                 : factory.GetCreator<commands::TTSChangeRegistratioResponse>();
    }
    case hmi_apis::FunctionID::TTS_GetSupportedLanguages: {
      return hmi_apis::messageType::request == message_type
                 ? factory
                       .GetCreator<commands::TTSGetSupportedLanguagesRequest>()
                 : factory.GetCreator<
                       commands::TTSGetSupportedLanguagesResponse>();
    }
    case hmi_apis::FunctionID::TTS_StopSpeaking: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::TTSStopSpeakingRequest>()
                 : factory.GetCreator<commands::TTSStopSpeakingResponse>();
    }
    case hmi_apis::FunctionID::TTS_GetLanguage: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::TTSGetLanguageRequest>()
                 : factory.GetCreator<commands::TTSGetLanguageResponse>();
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::TTSSpeakRequest>()
                 : factory.GetCreator<commands::TTSSpeakResponse>();
    }
    case hmi_apis::FunctionID::TTS_SetGlobalProperties: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::TTSSetGlobalPropertiesRequest>()
                 : factory
                       .GetCreator<commands::TTSSetGlobalPropertiesResponse>();
    }
    case hmi_apis::FunctionID::TTS_GetCapabilities: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::TTSGetCapabilitiesRequest>()
                 : factory.GetCreator<commands::TTSGetCapabilitiesResponse>();
    }
    case hmi_apis::FunctionID::TTS_Started: {
      return factory.GetCreator<commands::OnTTSStartedNotification>();
    }
    case hmi_apis::FunctionID::TTS_Stopped: {
      return factory.GetCreator<commands::OnTTSStoppedNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppActivated: {
      return factory.GetCreator<commands::OnAppActivatedNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAwakeSDL: {
      return factory.GetCreator<commands::OnAwakeSDLNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnExitApplication: {
      return factory.GetCreator<commands::OnExitApplicationNotification>();
    }
    case hmi_apis::FunctionID::UI_Show: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIShowRequest>()
                 : factory.GetCreator<commands::UIShowResponse>();
    }
    case hmi_apis::FunctionID::UI_Slider: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UISliderRequest>()
                 : factory.GetCreator<commands::UISliderResponse>();
    }
    case hmi_apis::FunctionID::UI_ClosePopUp: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::ClosePopupRequest>()
                 : factory.GetCreator<commands::ClosePopupResponse>();
    }
    case hmi_apis::FunctionID::UI_IsReady: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UIIsReadyRequest>()
                 : factory.GetCreator<commands::UIIsReadyResponse>();
    }
    case hmi_apis::FunctionID::Navigation_IsReady: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::NaviIsReadyRequest>()
                 : factory.GetCreator<commands::NaviIsReadyResponse>();
    }
    case hmi_apis::FunctionID::Navigation_AlertManeuver: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::NaviAlertManeuverRequest>()
                 : factory.GetCreator<commands::NaviAlertManeuverResponse>();
    }
    case hmi_apis::FunctionID::Navigation_GetWayPoints: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::NaviGetWayPointsRequest>()
                 : factory.GetCreator<commands::NaviGetWayPointsResponse>();
    }
    case hmi_apis::FunctionID::Navigation_UpdateTurnList: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::NaviUpdateTurnListRequest>()
                 : factory.GetCreator<commands::NaviUpdateTurnListResponse>();
    }
    case hmi_apis::FunctionID::Navigation_ShowConstantTBT: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::NaviShowConstantTBTRequest>()
                 : factory.GetCreator<commands::NaviShowConstantTBTResponse>();
    }
    case hmi_apis::FunctionID::Navigation_SubscribeWayPoints: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::NaviSubscribeWayPointsRequest>()
                 : factory
                       .GetCreator<commands::NaviSubscribeWayPointsResponse>();
    }
    case hmi_apis::FunctionID::Navigation_UnsubscribeWayPoints: {
      return hmi_apis::messageType::request == message_type
                 ? factory
                       .GetCreator<commands::NaviUnSubscribeWayPointsRequest>()
                 : factory.GetCreator<
                       commands::NaviUnsubscribeWayPointsResponse>();
    }
    case hmi_apis::FunctionID::Buttons_GetCapabilities: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::ButtonGetCapabilitiesRequest>()
                 : factory
                       .GetCreator<commands::ButtonGetCapabilitiesResponse>();
    }
    case hmi_apis::FunctionID::SDL_OnAllowSDLFunctionality: {
      return factory
          .GetCreator<commands::OnAllowSDLFunctionalityNotification>();
    }
    case hmi_apis::FunctionID::SDL_OnSDLConsentNeeded: {
      return factory.GetCreator<commands::OnSDLConsentNeededNotification>();
    }
    case hmi_apis::FunctionID::SDL_UpdateSDL: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UpdateSDLRequest>()
                 : factory.GetCreator<commands::UpdateSDLResponse>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnIgnitionCycleOver: {
      return factory.GetCreator<commands::OnIgnitionCycleOverNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSystemInfoChanged: {
      return factory.GetCreator<commands::OnSystemInfoChangedNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnReady: {
      return factory.GetCreator<commands::OnReadyNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSystemTimeReady: {
      return factory.GetCreator<commands::OnSystemTimeReadyNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnDeviceChosen: {
      return factory.GetCreator<commands::OnDeviceChosenNotification>();
    }
    case hmi_apis::FunctionID::UI_OnSystemContext: {
      return factory.GetCreator<commands::OnSystemContextNotification>();
    }
    case hmi_apis::FunctionID::UI_OnDriverDistraction: {
      return factory
          .GetCreator<commands::hmi::OnDriverDistractionNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnUpdateDeviceList: {
      return factory.GetCreator<commands::OnUpdateDeviceList>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppRegistered: {
      return factory.GetCreator<commands::OnAppRegisteredNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered: {
      return factory.GetCreator<commands::OnAppUnregisteredNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnFindApplications: {
      return factory.GetCreator<commands::OnFindApplications>();
    }
    case hmi_apis::FunctionID::BasicCommunication_UpdateAppList: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UpdateAppListRequest>()
                 : factory.GetCreator<commands::UpdateAppListResponse>();
    }
    case hmi_apis::FunctionID::VR_Started: {
      return factory.GetCreator<commands::OnVRStartedNotification>();
    }
    case hmi_apis::FunctionID::VR_Stopped: {
      return factory.GetCreator<commands::OnVRStoppedNotification>();
    }
    case hmi_apis::FunctionID::VR_OnCommand: {
      return factory.GetCreator<commands::OnVRCommandNotification>();
    }
    case hmi_apis::FunctionID::UI_OnCommand: {
      return factory.GetCreator<commands::OnUICommandNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppDeactivated: {
      return factory.GetCreator<commands::OnAppDeactivatedNotification>();
    }
    case hmi_apis::FunctionID::UI_OnLanguageChange: {
      return factory.GetCreator<commands::OnUILanguageChangeNotification>();
    }
    case hmi_apis::FunctionID::VR_OnLanguageChange: {
      return factory.GetCreator<commands::OnVRLanguageChangeNotification>();
    }
    case hmi_apis::FunctionID::TTS_OnLanguageChange: {
      return factory.GetCreator<commands::OnTTSLanguageChangeNotification>();
    }
    case hmi_apis::FunctionID::Buttons_OnButtonEvent: {
      return factory.GetCreator<commands::hmi::OnButtonEventNotification>();
    }
    case hmi_apis::FunctionID::Buttons_OnButtonPress: {
      return factory.GetCreator<commands::hmi::OnButtonPressNotification>();
    }
    case hmi_apis::FunctionID::Buttons_OnButtonSubscription: {
      return factory
          .GetCreator<commands::hmi::OnButtonSubscriptionNotification>();
    }
    case hmi_apis::FunctionID::Navigation_OnTBTClientState: {
      return factory.GetCreator<commands::OnNaviTBTClientStateNotification>();
    }
    case hmi_apis::FunctionID::UI_OnKeyboardInput: {
      return factory.GetCreator<commands::hmi::OnUIKeyBoardInputNotification>();
    }
    case hmi_apis::FunctionID::UI_OnTouchEvent: {
      return factory.GetCreator<commands::hmi::OnUITouchEventNotification>();
    }
    case hmi_apis::FunctionID::UI_OnResetTimeout: {
      return factory.GetCreator<commands::hmi::OnUIResetTimeoutNotification>();
    }
    case hmi_apis::FunctionID::Navigation_SetVideoConfig: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::NaviSetVideoConfigRequest>()
                 : factory.GetCreator<commands::NaviSetVideoConfigResponse>();
    }
    case hmi_apis::FunctionID::Navigation_StartStream: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::NaviStartStreamRequest>()
                 : factory.GetCreator<commands::NaviStartStreamResponse>();
    }
    case hmi_apis::FunctionID::Navigation_StopStream: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::NaviStopStreamRequest>()
                 : factory.GetCreator<commands::NaviStopStreamResponse>();
    }
    case hmi_apis::FunctionID::Navigation_StartAudioStream: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::AudioStartStreamRequest>()
                 : factory.GetCreator<commands::AudioStartStreamResponse>();
    }
    case hmi_apis::FunctionID::Navigation_StopAudioStream: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::AudioStopStreamRequest>()
                 : factory.GetCreator<commands::AudioStopStreamResponse>();
    }
    case hmi_apis::FunctionID::Navigation_OnAudioDataStreaming: {
      return factory.GetCreator<commands::OnAudioDataStreamingNotification>();
    }
    case hmi_apis::FunctionID::Navigation_OnVideoDataStreaming: {
      return factory.GetCreator<commands::OnVideoDataStreamingNotification>();
    }
    case hmi_apis::FunctionID::VR_PerformInteraction: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::VRPerformInteractionRequest>()
                 : factory.GetCreator<commands::VRPerformInteractionResponse>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSystemRequest: {
      return factory.GetCreator<commands::OnSystemRequestNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnPutFile: {
      return factory.GetCreator<commands::OnPutFileNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnResumeAudioSource: {
      return factory.GetCreator<commands::OnResumeAudioSourceNotification>();
    }
    case hmi_apis::FunctionID::UI_SetDisplayLayout: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UiSetDisplayLayoutRequest>()
                 : factory.GetCreator<commands::UiSetDisplayLayoutResponse>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSDLClose: {
      return factory.GetCreator<commands::OnSDLCloseNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnSDLPersistenceComplete: {
      return factory
          .GetCreator<commands::OnSDLPersistenceCompleteNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnFileRemoved: {
      return factory.GetCreator<commands::OnFileRemovedNotification>();
    }
    case hmi_apis::FunctionID::UI_OnRecordStart: {
      return factory.GetCreator<commands::OnRecordStartdNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_SystemRequest: {
      return hmi_apis::messageType::request == message_type
                 ? factory
                       .GetCreator<commands::BasicCommunicationSystemRequest>()
                 : factory.GetCreator<
                       commands::BasicCommunicationSystemResponse>();
    }
    case hmi_apis::FunctionID::BasicCommunication_GetSystemTime: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<
                       commands::BasicCommunicationGetSystemTimeRequest>()
                 : factory.GetCreator<
                       commands::BasicCommunicationGetSystemTimeResponse>();
    }
    case hmi_apis::FunctionID::Navigation_SendLocation: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::NaviSendLocationRequest>()
                 : factory.GetCreator<commands::NaviSendLocationResponse>();
    }
    case hmi_apis::FunctionID::SDL_AddStatisticsInfo: {
      return factory.GetCreator<commands::AddStatisticsInfoNotification>();
    }
    case hmi_apis::FunctionID::SDL_OnSystemError: {
      return factory.GetCreator<commands::OnSystemErrorNotification>();
    }
    case hmi_apis::FunctionID::SDL_OnReceivedPolicyUpdate: {
      return factory.GetCreator<commands::OnReceivedPolicyUpdate>();
    }
    case hmi_apis::FunctionID::SDL_OnPolicyUpdate: {
      return factory.GetCreator<commands::OnPolicyUpdate>();
    }
    case hmi_apis::FunctionID::SDL_OnDeviceStateChanged: {
      return factory.GetCreator<commands::OnDeviceStateChangedNotification>();
    }
    case hmi_apis::FunctionID::TTS_OnResetTimeout: {
      return factory.GetCreator<commands::hmi::OnTTSResetTimeoutNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_OnEventChanged: {
      return factory.GetCreator<commands::OnEventChangedNotification>();
    }
    case hmi_apis::FunctionID::BasicCommunication_DialNumber: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::hmi::DialNumberRequest>()
                 : factory.GetCreator<commands::hmi::DialNumberResponse>();
    }
    case hmi_apis::FunctionID::Navigation_OnWayPointChange: {
      return factory.GetCreator<commands::OnNaviWayPointChangeNotification>();
    }
    case hmi_apis::FunctionID::RC_IsReady: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::RCIsReadyRequest>()
                 : factory.GetCreator<commands::RCIsReadyResponse>();
    }
    case hmi_apis::FunctionID::RC_GetCapabilities: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::RCGetCapabilitiesRequest>()
                 : factory.GetCreator<commands::RCGetCapabilitiesResponse>();
    }
    case hmi_apis::FunctionID::UI_SendHapticData: {
      return hmi_apis::messageType::request == message_type
                 ? factory.GetCreator<commands::UISendHapticDataRequest>()
                 : factory.GetCreator<commands::UISendHapticDataResponse>();
    }
    default: { return factory.GetCreator<InvalidCommand>(); }
  }
}

}  // namespace application_manager
