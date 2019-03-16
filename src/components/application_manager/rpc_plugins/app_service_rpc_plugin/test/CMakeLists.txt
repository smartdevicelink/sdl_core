include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

include_directories(
  ${GMOCK_INCLUDE_DIRECTORY}
  ${COMPONENTS_DIR}/application_manager/rpc_plugins/app_service_rpc_plugin/include
  ${COMPONENTS_DIR}/application_manager/rpc_plugins/app_service_rpc_plugin/include/app_service_rpc_plugin/commands/
  ${COMPONENTS_DIR}/application_manager/test/include/
  ${POLICY_MOCK_INCLUDE_PATH}/
  ${CMAKE_SOURCE_DIR}/src
)  


set(COMMANDS_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/commands)

file(GLOB SOURCES
  ${COMPONENTS_DIR}/application_manager/test/mock_message_helper.cc
  ${COMPONENTS_DIR}/application_manager/test/mock_application_helper.cc
  ${COMPONENTS_DIR}/application_manager/src/smart_object_keys.cc
  ${COMMANDS_TEST_DIR}/hmi/*
  ${COMMANDS_TEST_DIR}/mobile/*
  ${COMPONENTS_DIR}/application_manager/src/message.cc
  ${COMPONENTS_DIR}/application_manager/src/event_engine/*
)

set(LIBRARIES
  gmock
  Utils
  SmartObjects
  HMI_API
  MOBILE_API
  connectionHandler
  app_service_rpc_plugin
  jsoncpp
  Policy
)

create_cotired_test("app_services_commands_test" "${SOURCES}" "${LIBRARIES}" )
