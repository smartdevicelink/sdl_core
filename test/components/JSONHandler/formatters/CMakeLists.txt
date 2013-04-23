include_directories (
  ${CMAKE_SOURCE_DIR}/src/components/Utils/include
  ${CMAKE_SOURCE_DIR}/src/components/JSONHandler/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/components/SmartObjects/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/logger/include/
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

# Libraries are the same for all SmartObject test
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    SmartObjects
    log4cplus
    jsoncpp
    JSONHandler
)

create_test("test_JSONHandler_FormatterJsonALRPCv2Test" "./CFormatterTestHelper.cpp;./FormatterJsonALRPCv2Test.cpp" "${LIBRARIES}")
create_test("test_JSONHandler_FormatterJsonALRPCv1Test" "./CFormatterTestHelper.cpp;./FormatterJsonALRPCv1Test.cpp" "${LIBRARIES}")
#create_test("test_JSONHandler_FormatterJsonRPC2Test" "./FormatterJsonRPC2Test.cpp" "${LIBRARIES}")
