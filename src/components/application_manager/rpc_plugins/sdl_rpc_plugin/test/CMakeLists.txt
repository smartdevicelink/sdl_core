# Copyright (c) 2018, Ford Motor Company All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

project(sdl_commands_test)

create_test(${PROJECT_NAME})

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  ${APPLICATION_MANAGER_DIR}/test/mock_message_helper.cc
  ${APPLICATION_MANAGER_DIR}/test/mock_application_helper.cc
  # hmi commands
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/activate_app_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/add_statistics_info_notification_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/allow_all_apps_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/allow_app_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/basic_communication_get_system_time_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/basic_communication_get_system_time_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/button_get_capabilities_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/close_popup_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/dummy_hmi_commands_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/get_system_info_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/get_system_info_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/get_urls_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/get_urls_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/hmi_notifications_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/mixing_audio_supported_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/mixing_audio_supported_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/navi_audio_start_stream_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/navi_is_ready_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/navi_is_ready_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/navi_set_video_config_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/navi_set_video_config_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/navi_start_stream_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/navi_stop_stream_requests_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/on_driver_distraction_notification_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/on_system_time_ready_notification_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/rc_get_capabilities_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/rc_is_ready_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/rc_is_ready_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/response_from_hmi_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/sdl_activate_app_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/sdl_activate_app_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/sdl_get_list_of_permisssions_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/sdl_get_status_update_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/sdl_get_user_friendly_message_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/simple_notifications_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/simple_request_from_hmi_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/simple_requests_to_hmi_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/simple_response_from_hmi_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/simple_response_to_hmi_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/tts_get_capabilities_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/tts_get_language_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/tts_get_supported_languages_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/tts_is_ready_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/ui_get_capabilities_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/ui_get_language_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/ui_get_supported_languages_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/ui_is_ready_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/ui_is_ready_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/update_device_list_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/update_sdl_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/update_sdl_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/vr_get_capabilities_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/vr_get_language_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/vr_get_supported_languages_response_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/vr_is_ready_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/vr_is_ready_response_test.cc
  # mobile commands
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/add_command_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/add_sub_menu_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/alert_maneuver_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/alert_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/change_registration_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/create_interaction_choice_set_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/delete_command_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/delete_file_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/delete_interaction_choice_set_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/delete_sub_menu_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/dial_number_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/dummy_mobile_commands_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/end_audio_pass_thru_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/get_way_points_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/list_files_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/on_button_notification_commands_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/on_command_notification_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/on_hash_change_notification_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/on_hmi_status_notification_from_mobile_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/on_hmi_status_notification_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/on_keyboard_input_notification_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/on_system_request_notification_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/on_tbt_client_state_notification_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/on_touch_event_notification_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/on_way_point_change_notification_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/perform_audio_pass_thru_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/perform_interaction_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/put_file_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/register_app_interface_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/reset_global_properties_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/scrollable_message_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/send_haptic_data_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/send_location_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/set_app_icon_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/set_display_layout_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/set_global_properties_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/set_media_clock_timer_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/show_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/simple_notification_commands_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/simple_response_commands_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/slider_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/speak_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/subscribe_button_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/subscribe_way_points_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/system_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/unregister_app_interface_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/unsubscribe_button_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/unsubscribe_way_points_request_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/update_turn_list_request_test.cc)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE components::interface sdl_rpc_plugins_static_lib)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
    ${COMPONENTS_DIR}
    ${POLICY_MOCK_INCLUDE_PATH}
    ${APPLICATION_MANAGER_DIR}/test/include
    ${APPLICATION_MANAGER_DIR}/rpc_plugins/sdl_rpc_plugin/include/sdl_rpc_plugin/commands/
  )
