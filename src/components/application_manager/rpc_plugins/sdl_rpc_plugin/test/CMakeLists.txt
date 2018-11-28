project(sdl_commands_test)

create_test(${PROJECT_NAME})

file(GLOB SOURCES
          ${APPLICATION_MANAGER_DIR}/test/mock_message_helper.cc
          ${APPLICATION_MANAGER_DIR}/test/mock_application_helper.cc
          ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/*
          ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/*)

target_sources(${PROJECT_NAME} PRIVATE "${SOURCES}")

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
