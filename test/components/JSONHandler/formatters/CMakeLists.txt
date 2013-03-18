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

set (SOURCES_JSONALRPCV2_TEST
    ./FormatterJsonALRPCv2Test.cpp
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

add_executable("test_FormatterJsonALRPCv2Test" ${SOURCES_JSONALRPCV2_TEST})
target_link_libraries("test_FormatterJsonALRPCv2Test" ${LIBRARIES})
add_test(test_FormatterJsonALRPCv2Test test_FormatterJsonALRPCv2Test)