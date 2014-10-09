# Generating interface
GenerateInterface("test_json_rpc.xml" "gen::test::components::json_rpc" "jsonrpc")
GenerateInterface("test_JSONHandler_v4_protocol_v2_0_revP.xml" "Gen::test::components::JSONHandler2" "sdlrpcv2")
GenerateInterface("test_JSONHandler_v4_protocol_v1_2_no_extra.xml" "Gen::test::components::JSONHandler1" "sdlrpcv1")


include_directories (
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}

  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ${CMAKE_SOURCE_DIR}/src/components/smart_objects/include

  ${CMAKE_SOURCE_DIR}/test/components/json_handler/include/
  ${CMAKE_BINARY_DIR}
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main

    test_json_rpc
    test_JSONHandler_v4_protocol_v2_0_revP
    test_JSONHandler_v4_protocol_v1_2_no_extra

    jsoncpp
    formatters
    SmartObjects
)

create_test("test_JSONHandler_JSONRPCFullScenario" "./src/test_json_rpc_full_scenario.cpp" "${LIBRARIES}")
create_test("test_JSONHandler_FormatterJsonSDLRPCv2Test" "./src/formatter_test_helper.cc;./src/meta_formatter_test_helper.cc;./src/FormatterJsonSDLRPCv2Test.cpp" "${LIBRARIES}")
create_test("test_JSONHandler_FormatterJsonSDLRPCv1Test" "./src/formatter_test_helper.cc;./src/FormatterJsonSDLRPCv1Test.cpp" "${LIBRARIES}")
create_test("test_JSONHandler_FormatterJsonRPC2Test" "./src/formatter_json_rpc_test.cc;./src/formatter_test_helper.cc" "${LIBRARIES}")
create_test("test_JSONHandler_MetaFormatter" "./src/meta_formatter_test.cc;./src/meta_formatter_test_helper.cc" "${LIBRARIES}")
