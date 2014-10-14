include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/functional_module/include/
  ../../../src/components/can_cooperation/include/
  ../../../src/components/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    jsoncpp
    CANCooperation
    FunctionalModule
)

create_test("test_can_module" "./src/can_module_test.cc" "${LIBRARIES}")
create_test("test_can_app_extension" "./src/can_app_extension_test.cc" "${LIBRARIES}")