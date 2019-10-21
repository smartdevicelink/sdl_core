include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)


set(APP_SERVICE_DIR "${COMPONENTS_DIR}/application_manager/rpc_plugins/app_service_rpc_plugin")
set(APP_SERVICE_SRC_DIR "${APP_SERVICE_DIR}/src")
set(APP_SERVICE_INCLUDE_DIR "${APP_SERVICE_DIR}/include/app_service_rpc_plugin")
set(APP_SERVICE_TEST_DIR "${APP_SERVICE_DIR}/test")

include_directories(
  ${GMOCK_INCLUDE_DIRECTORY}
  ${APP_SERVICE_INCLUDE_DIR}
) 

set(SOURCES ${APP_SERVICE_TEST_DIR}/app_service_plugin/app_service_app_extension_test.cc
            ${APP_SERVICE_SRC_DIR}/app_service_rpc_plugin.cc
            ${COMPONENTS_DIR}/application_manager/test/mock_message_helper.cc
)

set(LIBRARIES
  gmock
  sdl_rpc_plugin_static
  app_service_rpc_plugin_static
)

create_test("app_service_plugin_test" "${SOURCES}" "${LIBRARIES}" )
