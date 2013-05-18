# Generating interface
GenerateInterface("test_JSONHandler_v4_protocol_v2_0_revP.xml" "Gen::test::components::JSONHandler2" "sdlrpcv2")
GenerateInterface("test_JSONHandler_v4_protocol_v1_2_no_extra.xml" "Gen::test::components::JSONHandler1" "sdlrpcv1")

# --- Formatters
add_subdirectory(./formatters)

include_directories (
  ../../../src/components/utils/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/SmartObjects/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src//thirdPartyLibs/jsoncpp/include/
  ../../../src/components/JSONHandler/include/
  ../../../test/components/json_handler/include/
  ../../../test/components/
  ${CMAKE_BINARY_DIR}
)

# Generating interface
GenerateInterface("test_JSONHandler_v4_protocol_v2_0_revP.xml" "Gen::test::components::JSONHandler2" "alrpcv2")
GenerateInterface("test_JSONHandler_v4_protocol_v1_2_no_extra.xml" "Gen::test::components::JSONHandler1" "alrpcv1")

set (SOURCES
  ./src/smart_schema_draft_test.cc
  ./src/formatters/formatter_test_helper.cc
  ./src/formatters/formatter_json_alrpcv1_test.cc
  ./src/formatters/formatter_json_alrpcv2_test.cc
  ./src/formatters/formatter_json_rpc2_test.cc
)

add_library("test_JSONHandlerTest" ${SOURCES})

