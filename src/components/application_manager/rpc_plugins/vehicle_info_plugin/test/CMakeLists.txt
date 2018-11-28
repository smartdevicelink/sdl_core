project(vi_commands_test)

create_test(${PROJECT_NAME})

file(GLOB SOURCES
          ${COMPONENTS_DIR}/application_manager/test/mock_message_helper.cc
          ${COMPONENTS_DIR}/application_manager/test/mock_application_helper.cc
          ${CMAKE_CURRENT_SOURCE_DIR}/commands/hmi/*
          ${CMAKE_CURRENT_SOURCE_DIR}/commands/mobile/*)

target_sources(${PROJECT_NAME} PRIVATE "${SOURCES}")

target_link_libraries(${PROJECT_NAME}
                      PRIVATE components::interface
                              vehicle_info_plugin_static_lib)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${COMPONENTS_DIR} ${COMPONENTS_DIR}/application_manager/test/include/)
