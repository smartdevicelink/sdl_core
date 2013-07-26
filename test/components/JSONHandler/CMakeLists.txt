# Generating interface
GenerateInterface("test_JSONHandler_v4_protocol_v2_0_revP.xml" "Gen::test::components::JSONHandler2" "sdlrpcv2")
GenerateInterface("test_JSONHandler_v4_protocol_v1_2_no_extra.xml" "Gen::test::components::JSONHandler1" "sdlrpcv1")
GenerateInterface("test_json_rpc.xml" "gen::test::components::json_rpc" "jsonrpc")

# --- Formatters
add_subdirectory(./formatters)

include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/smart_objects/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ../../../src//thirdPartyLibs/jsoncpp/include/
  ../../../src/components/JSONHandler/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
  ${CMAKE_BINARY_DIR}
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    test_JSONHandler_v4_protocol_v2_0_revP
    test_json_rpc
    SmartObjects
    log4cplus
    JSONHandler
    jsoncpp
  #v4_protocol_v2_0_revP
)

create_test("test_JSONHandler_SmartSchemaDraftTest" "./SmartSchemaDraftTest.cpp" "${LIBRARIES}")
create_test("test_JSONHandler_JSONRPCFullScenario" "./test_json_rpc_full_scenario.cpp" "${LIBRARIES}")
