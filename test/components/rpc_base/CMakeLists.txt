include_directories (
  ${CMAKE_SOURCE_DIR}/src/components/rpc_base/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    rpc_base
)

set (SOURCES
  rpc_base_json_test.cc
  rpc_base_test.cc
)

create_test("test_rpc_base" "${SOURCES}" "${LIBRARIES}")

