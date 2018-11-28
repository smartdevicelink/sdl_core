project(rc_commands_test)

create_test(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${APPLICATION_MANAGER_DIR}/test/include
                                   ${APPLICATION_MANAGER_DIR}/include
                                   ${CMAKE_CURRENT_LIST_DIR}/include
                                   ${COMPONENTS_DIR})

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  "${COMPONENTS_DIR}/application_manager/test/mock_message_helper.cc"
  "${CMAKE_CURRENT_LIST_DIR}/commands/button_press_request_test.cc"
  "${CMAKE_CURRENT_LIST_DIR}/commands/get_interior_vehicle_data_request_test.cc"
  "${CMAKE_CURRENT_LIST_DIR}/commands/on_interior_vehicle_data_notification_test.cc"
  "${CMAKE_CURRENT_LIST_DIR}/commands/on_remote_control_settings_test.cc"
  "${CMAKE_CURRENT_LIST_DIR}/commands/rc_get_interior_vehicle_data_consent_test.cc"
  "${CMAKE_CURRENT_LIST_DIR}/commands/set_interior_vehicle_data_request_test.cc"
  )

target_link_libraries(${PROJECT_NAME}
                      PRIVATE components::interface rpc_rpc_plugin_static_lib)

project(rc_plugin_test)

create_test(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${APPLICATION_MANAGER_DIR}/test/include
                                   ${APPLICATION_MANAGER_DIR}/include
                                   ${CMAKE_CURRENT_LIST_DIR}/include
                                   ${COMPONENTS_DIR})

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/resource_allocation_manager_impl_test.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/interior_data_cache_test.cc")

target_link_libraries(${PROJECT_NAME}
                      PRIVATE components::interface rpc_rpc_plugin_static_lib)
