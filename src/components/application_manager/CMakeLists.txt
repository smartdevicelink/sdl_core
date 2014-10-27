set (COMPONENTS_DIR ${CMAKE_SOURCE_DIR}/src/components)
set (AM_SOURCE_DIR ${COMPONENTS_DIR}/application_manager)
set (AM_TEST_DIR ${AM_SOURCE_DIR}/test)
set (AM_MOCK_DIR ${AM_TEST_DIR}/mock)



include_directories (
  include/
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/formatters/include/
  ${COMPONENTS_DIR}/protocol_handler/include/
  ${COMPONENTS_DIR}/smart_objects/include/
  ${COMPONENTS_DIR}/hmi_message_handler/include
  ${COMPONENTS_DIR}/media_manager/include/
  ${COMPONENTS_DIR}/connection_handler/include/
  ${COMPONENTS_DIR}/config_profile/include/
  ${COMPONENTS_DIR}/request_watchdog/include/
  ${COMPONENTS_DIR}/resumption/include/
  ${COMPONENTS_DIR}/rpc_base/include/
  ${CMAKE_BINARY_DIR}/src/components/
  ${COMPONENTS_DIR}/include/
  ${COMPONENTS_DIR}/policy/src/policy/include/
  ${COMPONENTS_DIR}/policy/src/policy/usage_statistics/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${ENCRYPTION_INCLUDE_DIRECTORY}
  ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

file (GLOB SOURCES
  ${AM_SOURCE_DIR}/src/*
)

set (POLICIES_MANAGER
${AM_SOURCE_DIR}/src/policies/policy_handler.cc
${AM_SOURCE_DIR}/src/policies/policy_event_observer.cc
)

  include_directories(
    ${COMPONENTS_DIR}/policy/src/policy/policy_table/table_struct
  )
  list(APPEND POLICIES_MANAGER
    ${AM_SOURCE_DIR}/src/policies/pt_exchange_handler_impl.cc
    ${AM_SOURCE_DIR}/src/policies/policy_retry_sequence.cc
  )

file (GLOB EVENT_ENGINE
  ${AM_SOURCE_DIR}/src/event_engine/*
)

file (GLOB MOBILE_COMMANDS_SOURCES
  ${AM_SOURCE_DIR}/src/commands/*
  ${AM_SOURCE_DIR}/src/commands/mobile/*
)

  set (HMI_COMMANDS_SOURCES
  ${AM_SOURCE_DIR}/src/commands/hmi/request_to_hmi.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/response_from_hmi.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/request_from_hmi.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/response_to_hmi.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/notification_to_hmi.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/notification_from_hmi.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/allow_all_apps_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/allow_all_apps_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/allow_app_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/allow_app_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/update_sdl_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/update_sdl_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/activate_app_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/activate_app_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/get_system_info_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/get_system_info_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/sdl_get_list_of_permissions_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/sdl_get_list_of_permissions_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/sdl_get_user_friendly_message_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/sdl_get_user_friendly_message_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/sdl_get_status_update_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/sdl_get_status_update_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_status_update_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/sdl_activate_app_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/sdl_activate_app_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/mixing_audio_supported_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/mixing_audio_supported_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/update_app_list_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/update_app_list_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_update_device_list.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/update_device_list_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/update_device_list_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_sdl_consent_needed_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_sdl_persistence_complete_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_exit_all_applications_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_exit_application_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_start_device_discovery.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/close_popup_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/close_popup_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_app_activated_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/button_get_capabilities_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/button_get_capabilities_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_is_ready_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_is_ready_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_add_command_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_add_command_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_delete_command_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_delete_command_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_change_registration_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_change_registration_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_get_supported_languages_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_get_supported_languages_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_get_language_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_get_language_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_get_capabilities_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_get_capabilities_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_is_ready_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_is_ready_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_change_registration_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_change_registration_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_get_language_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_get_language_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_get_supported_languages_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_get_supported_languages_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_stop_speaking_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_stop_speaking_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_speak_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_speak_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_set_global_properties_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_set_global_properties_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_get_capabilities_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/tts_get_capabilities_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_perform_interaction_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vr_perform_interaction_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_add_command_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_add_command_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_delete_command_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_delete_command_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_add_submenu_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_add_submenu_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_delete_submenu_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_delete_submenu_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_get_supported_languages_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_get_supported_languages_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_get_language_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_get_language_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_get_capabilities_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_get_capabilities_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_change_registration_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_change_registration_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_show_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_show_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_alert_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_alert_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_slider_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_scrollable_message_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_scrollable_message_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_set_global_properties_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_set_global_properties_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_slider_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_is_ready_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_is_ready_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_perform_audio_pass_thru_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_perform_audio_pass_thru_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_end_audio_pass_thru_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_end_audio_pass_thru_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_perform_interaction_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_perform_interaction_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_set_icon_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_set_icon_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_set_media_clock_timer_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_set_media_clock_timer_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_show_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_show_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_is_ready_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_is_ready_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_read_did_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_read_did_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_get_dtcs_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_get_dtcs_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_diagnostic_message_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_diagnostic_message_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_get_vehicle_type_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_get_vehicle_type_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_is_ready_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_is_ready_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_alert_maneuver_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_alert_maneuver_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_update_turn_list_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_update_turn_list_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_show_constant_tbt_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_show_constant_tbt_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_send_location_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_send_location_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_ready_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_system_context_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_device_chosen_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_allow_sdl_functionality_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_app_permission_consent_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_app_registered_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_app_permission_changed_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_app_unregistered_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_driver_distraction_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_ignition_cycle_over_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_system_info_changed_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_file_removed_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_play_tone_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_tts_started_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_tts_stopped_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vr_started_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vr_stopped_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vr_command_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_ui_command_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_navi_tbt_client_state_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_app_deactivated_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_tts_language_change_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vr_language_change_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_ui_language_change_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_button_event_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_button_press_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_find_applications.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_ui_keyboard_input_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_ui_touch_event_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_ui_reset_timeout_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_start_stream_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_start_stream_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_stop_stream_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_stop_stream_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_audio_start_stream_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_audio_start_stream_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_audio_stop_stream_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/navi_audio_stop_stream_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_system_request_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_put_file_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_resume_audio_source_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_sdl_close_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_set_display_layout_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/ui_set_display_layout_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_record_start_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/add_statistics_info_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_system_error_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/basic_communication_system_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/basic_communication_system_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/sdl_policy_update.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/sdl_policy_update_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_received_policy_update.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_policy_update.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/get_urls.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/get_urls_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_device_state_changed_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_tts_reset_timeout_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_phone_call_notification.cc
)

set (HMI_COMMANDS_SOURCES_JSON
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_get_vehicle_data_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_get_vehicle_data_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_subscribe_vehicle_data_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_subscribe_vehicle_data_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_unsubscribe_vehicle_data_request.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/vi_unsubscribe_vehicle_data_response.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_vehicle_data_notification.cc
)

set (HMI_COMMANDS_SOURCES_DBUS
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_gps_data_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_speed_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_rpm_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_fuel_level_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_fuel_level_state_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_instant_fuel_consumption_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_external_temperature_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_vin_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_prndl_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_tire_pressure_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_odometer_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_belt_status_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_body_information_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_device_status_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_driver_braking_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_wiper_status_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_head_lamp_status_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_engine_torque_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_acc_pedal_position_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_steering_wheel_angle_notification.cc
  ${AM_SOURCE_DIR}/src/commands/hmi/on_vi_my_key_notification.cc
)

if (${HMI_JSON_API})
    set (HMI_COMMANDS_SOURCES ${HMI_COMMANDS_SOURCES} ${HMI_COMMANDS_SOURCES_JSON})
endif (${HMI_JSON_API})
if (${HMI_DBUS_API})
    set (HMI_COMMANDS_SOURCES ${HMI_COMMANDS_SOURCES} ${HMI_COMMANDS_SOURCES_DBUS})
endif (${HMI_DBUS_API})

SET (LIBRARIES
  UsageStatistics
  dl
  ProtocolLibrary
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "QNX")
  list(REMOVE_ITEM LIBRARIES dl)
endif()

add_library("AMEventEngine" ${EVENT_ENGINE})
target_link_libraries("AMEventEngine" ${LIBRARIES})

add_library("AMPolicyLibrary" ${POLICIES_MANAGER} )
target_link_libraries("AMPolicyLibrary" ${LIBRARIES} AMEventEngine)

add_library("AMHMICommandsLibrary" ${HMI_COMMANDS_SOURCES})
target_link_libraries("AMHMICommandsLibrary" ${LIBRARIES} AMEventEngine)

add_library("AMMobileCommandsLibrary" ${MOBILE_COMMANDS_SOURCES} )
target_link_libraries("AMMobileCommandsLibrary" ${LIBRARIES} AMEventEngine)

add_library("ApplicationManager" ${SOURCES})
target_link_libraries("ApplicationManager" ${LIBRARIES} AMHMICommandsLibrary
                                                        AMMobileCommandsLibrary
                                                        AMEventEngine
                                                        AMPolicyLibrary)

if(ENABLE_LOG)
  target_link_libraries("ApplicationManager" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
