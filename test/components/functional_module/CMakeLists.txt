include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/functional_module/include/
  ../../../src/components/include/
  ../../../src/components/application_manager/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    jsoncpp
    FunctionalModule
    dl
)

add_subdirectory(plugins)

create_test("test_generic_module" "./src/generic_module_test.cc" "${LIBRARIES}")
create_test("test_plugin_manager" "./src/plugin_manager_test.cc" "${LIBRARIES}")