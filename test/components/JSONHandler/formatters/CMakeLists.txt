# Generating interface
GenerateInterface("test_JSONHandler_alrpcv2.xml" "Gen::test::components::JSONHandler2" "alrpcv2")

include_directories (
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
    test_JSONHandler_alrpcv2
    test_json_rpc
    SmartObjects
    log4cplus
    jsoncpp
    JSONHandler    
)

create_test("test_JSONHandler_FormatterJsonSDLRPCv2Test" "./CFormatterTestHelper.cpp;./FormatterJsonSDLRPCv2Test.cpp" "${LIBRARIES}")
create_test("test_JSONHandler_FormatterJsonSDLRPCv1Test" "./CFormatterTestHelper.cpp;./FormatterJsonSDLRPCv1Test.cpp" "${LIBRARIES}")
create_test("test_JSONHandler_FormatterJsonRPC2Test" "./CFormatterTestHelper.cpp;./formatter_json_rpc_test.cc" "${LIBRARIES}")
create_test("test_JSONHandler_MetaFormatter" "./meta_formatter_test.cc;./meta_formatter_test_helper.cc" "${LIBRARIES}")