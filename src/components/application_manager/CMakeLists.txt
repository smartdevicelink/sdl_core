# Copyright (c) 2016, Ford Motor Company
# All rights reserved.
#
# Copyright (c) 2017 Xevo Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the copyright holders nor the names of their contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/platform.cmake)
include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

set (AM_SOURCE_DIR ${COMPONENTS_DIR}/application_manager)
set (AM_TEST_DIR ${AM_SOURCE_DIR}/test)
set (AM_MOCK_DIR ${AM_TEST_DIR}/mock)

include_directories (
  include/
  ${COMPONENTS_DIR}
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/formatters/include/
  ${COMPONENTS_DIR}/protocol_handler/include/
  ${COMPONENTS_DIR}/smart_objects/include/
  ${COMPONENTS_DIR}/hmi_message_handler/include
  ${COMPONENTS_DIR}/media_manager/include
  ${COMPONENTS_DIR}/connection_handler/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/request_watchdog/include
  ${COMPONENTS_DIR}/resumption/include
  ${COMPONENTS_DIR}/rpc_base/include
  ${COMPONENTS_DIR}/interfaces
  ${POLICY_PATH}/include/
  ${POLICY_GLOBAL_INCLUDE_PATH}/
  ${CMAKE_BINARY_DIR}/src/components/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${ENCRYPTION_INCLUDE_DIRECTORY}
  ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${BSON_INCLUDE_DIRECTORY}
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  include_directories(
    ${COMPONENTS_DIR}/utils/include/utils/sqlite_wrapper/
  )
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  include_directories(
    ${COMPONENTS_DIR}/utils/include/utils/qdb_wrapper/
  )
endif()

set (MESSAGE_HELPER_SOURCE_DIR
  ${AM_SOURCE_DIR}/src/message_helper
)

set (MESSAGE_HELPER_PATHS
  ${MESSAGE_HELPER_SOURCE_DIR}
)
collect_sources(MESSAGE_HELPER_SOURCES "${MESSAGE_HELPER_PATHS}")


set (POLICIES_SOURCE_DIR
  ${AM_SOURCE_DIR}/src/policies
)

set (POLICIES_MANAGER_SOURCES
  ${POLICIES_SOURCE_DIR}/policy_handler.cc
  ${POLICIES_SOURCE_DIR}/policy_event_observer.cc
  ${POLICIES_SOURCE_DIR}/delegates/app_permission_delegate.cc
  ${POLICIES_SOURCE_DIR}/delegates/statistics_delegate.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/usage_statistics.cc
)

include_directories(
  ${POLICY_PATH}/src/policy/policy_table/table_struct
)

set (EVENT_ENGINE_SOURCE_DIR
  ${AM_SOURCE_DIR}/src/event_engine
)

set (EVENT_ENGINE_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/application_manager/event_engine
  ${EVENT_ENGINE_SOURCE_DIR}
)
collect_sources(EVENT_ENGINE_SOURCES "${EVENT_ENGINE_PATHS}")

set (COMMANDS_SOURCE_DIR
  ${AM_SOURCE_DIR}/src/commands
)
collect_sources(POLICIES_SOURCES "${POLICIES_PATHS}")

set(MOBILE_COMMANDS_EXCLUDE_PATHS
  ${COMMANDS_SOURCE_DIR}/hmi
)

set (MOBILE_COMMANDS_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/application_manager/commands
  ${CMAKE_CURRENT_SOURCE_DIR}/include/application_manager/commands/mobile
  ${COMMANDS_SOURCE_DIR}
  ${COMMANDS_SOURCE_DIR}/mobile
)
collect_sources(MOBILE_COMMANDS_SOURCES "${MOBILE_COMMANDS_PATHS}" "${MOBILE_COMMANDS_EXCLUDE_PATHS}")

set (HMI_COMMANDS_SOURCES
  ${COMMANDS_SOURCE_DIR}/hmi/request_to_hmi.cc
  ${COMMANDS_SOURCE_DIR}/hmi/response_from_hmi.cc
  ${COMMANDS_SOURCE_DIR}/hmi/request_from_hmi.cc
  ${COMMANDS_SOURCE_DIR}/hmi/response_to_hmi.cc
  ${COMMANDS_SOURCE_DIR}/hmi/notification_to_hmi.cc
  ${COMMANDS_SOURCE_DIR}/hmi/notification_from_hmi.cc
  ${COMMANDS_SOURCE_DIR}/hmi/allow_all_apps_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/allow_all_apps_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/allow_app_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/allow_app_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/update_sdl_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/update_sdl_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/activate_app_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/activate_app_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/get_system_info_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/get_system_info_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/sdl_get_list_of_permissions_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/sdl_get_list_of_permissions_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/sdl_get_user_friendly_message_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/sdl_get_user_friendly_message_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/sdl_get_status_update_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/sdl_get_status_update_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_status_update_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/sdl_activate_app_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/sdl_activate_app_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/mixing_audio_supported_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/mixing_audio_supported_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/update_app_list_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/update_app_list_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_update_device_list.cc
  ${COMMANDS_SOURCE_DIR}/hmi/update_device_list_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/update_device_list_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_audio_data_streaming_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_video_data_streaming_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_sdl_consent_needed_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_sdl_persistence_complete_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_exit_all_applications_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_exit_application_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_navi_way_point_change_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_start_device_discovery.cc
  ${COMMANDS_SOURCE_DIR}/hmi/close_popup_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/close_popup_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_app_activated_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/button_get_capabilities_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/button_get_capabilities_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_is_ready_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_is_ready_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_add_command_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_add_command_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_delete_command_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_delete_command_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_change_registration_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_change_registration_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_get_supported_languages_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_get_supported_languages_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_get_language_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_get_language_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_get_capabilities_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_get_capabilities_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_is_ready_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_is_ready_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_change_registration_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_change_registration_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_get_language_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_get_language_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_get_supported_languages_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_get_supported_languages_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_stop_speaking_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_stop_speaking_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_speak_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_speak_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_set_global_properties_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_set_global_properties_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_get_capabilities_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/tts_get_capabilities_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_perform_interaction_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vr_perform_interaction_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_add_command_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_add_command_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_delete_command_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_delete_command_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_add_submenu_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_add_submenu_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_delete_submenu_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_delete_submenu_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_get_supported_languages_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_get_supported_languages_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_get_language_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_get_language_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_get_capabilities_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_get_capabilities_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_change_registration_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_change_registration_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_show_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_show_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_alert_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_alert_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_slider_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_scrollable_message_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_scrollable_message_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_set_global_properties_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_set_global_properties_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_slider_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_is_ready_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_is_ready_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_perform_audio_pass_thru_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_perform_audio_pass_thru_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_end_audio_pass_thru_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_end_audio_pass_thru_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_perform_interaction_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_perform_interaction_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_set_app_icon_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_set_app_icon_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_set_media_clock_timer_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_set_media_clock_timer_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_show_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_show_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_is_ready_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_is_ready_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_read_did_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_read_did_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_get_dtcs_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_get_dtcs_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_diagnostic_message_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_diagnostic_message_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_get_vehicle_type_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_get_vehicle_type_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_is_ready_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_is_ready_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_alert_maneuver_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_alert_maneuver_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_update_turn_list_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_update_turn_list_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_show_constant_tbt_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_show_constant_tbt_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_send_location_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_send_location_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_subscribe_way_points_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_subscribe_way_points_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_unsubscribe_way_points_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_unsubscribe_way_points_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_ready_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_system_context_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_device_chosen_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_allow_sdl_functionality_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_app_permission_consent_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_app_registered_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_app_permission_changed_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_app_unregistered_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_driver_distraction_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_ignition_cycle_over_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_system_info_changed_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_file_removed_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_tts_started_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_tts_stopped_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vr_started_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vr_stopped_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vr_command_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_ui_command_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_navi_tbt_client_state_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_app_deactivated_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_tts_language_change_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vr_language_change_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_ui_language_change_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_button_event_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_button_press_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_button_subscription_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_find_applications.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_ui_keyboard_input_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_ui_touch_event_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_ui_reset_timeout_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_start_stream_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_start_stream_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_stop_stream_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_stop_stream_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_audio_start_stream_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_audio_start_stream_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_audio_stop_stream_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_audio_stop_stream_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_get_way_points_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_get_way_points_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_set_video_config_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/navi_set_video_config_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_system_request_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_put_file_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_resume_audio_source_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_sdl_close_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_set_display_layout_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_set_display_layout_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_record_start_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/add_statistics_info_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_system_error_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/basic_communication_system_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/basic_communication_system_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/sdl_policy_update.cc
  ${COMMANDS_SOURCE_DIR}/hmi/sdl_policy_update_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_received_policy_update.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_policy_update.cc
  ${COMMANDS_SOURCE_DIR}/hmi/get_urls.cc
  ${COMMANDS_SOURCE_DIR}/hmi/get_urls_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_device_state_changed_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_tts_reset_timeout_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/dial_number_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/dial_number_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_event_changed_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/decrypt_certificate_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/decrypt_certificate_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_set_icon_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_send_haptic_data_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/ui_send_haptic_data_response.cc
)

set (HMI_COMMANDS_SOURCES_JSON
  ${COMMANDS_SOURCE_DIR}/hmi/vi_get_vehicle_data_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_get_vehicle_data_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_subscribe_vehicle_data_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_subscribe_vehicle_data_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_unsubscribe_vehicle_data_request.cc
  ${COMMANDS_SOURCE_DIR}/hmi/vi_unsubscribe_vehicle_data_response.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_vehicle_data_notification.cc
)

set (HMI_COMMANDS_SOURCES_DBUS
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_gps_data_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_speed_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_rpm_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_fuel_level_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_fuel_level_state_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_instant_fuel_consumption_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_external_temperature_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_vin_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_prndl_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_tire_pressure_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_odometer_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_belt_status_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_body_information_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_device_status_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_driver_braking_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_wiper_status_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_head_lamp_status_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_engine_torque_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_acc_pedal_position_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_steering_wheel_angle_notification.cc
  ${COMMANDS_SOURCE_DIR}/hmi/on_vi_my_key_notification.cc
)

if (${HMI_JSON_API})
  set (HMI_COMMANDS_SOURCES ${HMI_COMMANDS_SOURCES} ${HMI_COMMANDS_SOURCES_JSON})
endif (${HMI_JSON_API})

set (HMI_COMMANDS_SOURCES ${HMI_COMMANDS_SOURCES} ${HMI_COMMANDS_SOURCES_DBUS})

set(EXCLUDE_PATHS
  ${COMMANDS_SOURCE_DIR}
  ${EVENT_ENGINE_SOURCE_DIR}
  ${MESSAGE_HELPER_SOURCE_DIR}
  ${POLICIES_SOURCE_DIR}
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")

set(LIBRARIES
  HMI_API
  MOBILE_API
  v4_protocol_v1_2_no_extra
  ProtocolLibrary
  SmartObjects
  UsageStatistics
  dl
  formatters
  dbms
  Utils
  bson -L${BSON_LIBS_DIRECTORY}
  emhashmap -L${EMHASHMAP_LIBS_DIRECTORY}
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND LIBRARIES sqlite3)
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  list(REMOVE_ITEM LIBRARIES dl)
endif()

add_library("AMEventEngine" ${EVENT_ENGINE_SOURCES})
target_link_libraries("AMEventEngine" ${LIBRARIES})

add_library("AMPolicyLibrary" ${POLICIES_MANAGER_SOURCES} )
target_link_libraries("AMPolicyLibrary" ${LIBRARIES} AMEventEngine)

add_library("AMHMICommandsLibrary" ${HMI_COMMANDS_SOURCES})
target_link_libraries("AMHMICommandsLibrary" ${LIBRARIES} AMEventEngine)

add_library("MessageHelper" ${MESSAGE_HELPER_SOURCES})
target_link_libraries("MessageHelper" ${LIBRARIES})

add_library("AMMobileCommandsLibrary" ${MOBILE_COMMANDS_SOURCES} )
target_link_libraries("AMMobileCommandsLibrary" ${LIBRARIES} AMEventEngine)

add_library("ApplicationManager" ${SOURCES})

list(APPEND LIBRARIES
  AMEventEngine
  AMPolicyLibrary
  AMHMICommandsLibrary
  AMMobileCommandsLibrary
)

if(ENABLE_LOG)
  list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

add_dependencies("ApplicationManager" libbson)
target_link_libraries("ApplicationManager" ${LIBRARIES})

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
