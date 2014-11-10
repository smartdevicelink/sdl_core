include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/functional_module/include/
  ../../../src/components/can_cooperation/include/
  ../../../src/components/include/
  ../../../src/components/application_manager/include
  ${JSONCPP_INCLUDE_DIRECTORY}
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    CANCooperation
    jsoncpp
    FunctionalModule
)

#create_test("test_can_module" "./src/can_module_test.cc" "${LIBRARIES}")
create_test("test_can_app_extension" "./src/can_app_extension_test.cc" "${LIBRARIES}")
create_test("test_can_tcp_connection" "./src/can_tcp_connection_test.cc" "${LIBRARIES}")