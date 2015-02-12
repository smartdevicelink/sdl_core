include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/functional_module/include/
  ${CMAKE_SOURCE_DIR}/src/components/include/
  ${CMAKE_SOURCE_DIR}/src/components/application_manager/include
  ${CMAKE_SOURCE_DIR}/src/components/connection_handler/include
  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
  include
  plugins
)

set (LIBRARIES
  gtest
  gmock
  gmock_main
  FunctionalModule
  dl
  ApplicationManager
  jsoncpp
  Utils
)

set(SOURCES
  ./src/generic_module_test.cc
  ./src/plugin_manager_test.cc
  ./src/module_timer_test.cc
)

add_subdirectory(plugins)

create_test("function_module_test" "${SOURCES}" "${LIBRARIES}")
