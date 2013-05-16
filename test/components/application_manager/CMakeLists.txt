include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/application_manager/include
  ../../../src/components/SmartObjects/include
  ../../../src/components/JSONHandler/include/
  ../../../src/components/utils/include/
  ../../../src//thirdPartyLibs/jsoncpp/include/
    ${CMAKE_BINARY_DIR}/src/components/Interfaces/
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
#    ApplicationManager
    log4cplus
    JSONHandler
    jsoncpp
    v4_protocol_v1_2_no_extra
    v4_protocol_v2_0_revT
    SmartObjects
)

create_test("test_APIVersionConverterV1Test" "./api_converter_v1_test.cpp" "${LIBRARIES}")
