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

set (SOURCES_JSONALRPCV1_TEST
    ./FormatterJsonALRPCv1Test.cpp
)

#set (SOURCES_JSONRPC2_TEST
#    ./FormatterJsonRPC2Test.cpp
#)


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

add_executable("test_FormatterJsonALRPCv1Test" ${SOURCES_JSONALRPCV1_TEST})
target_link_libraries("test_FormatterJsonALRPCv1Test" ${LIBRARIES})
add_test(test_FormatterJsonALRPCv1Test test_FormatterJsonALRPCv1Test)

#add_executable("test_FormatterJsonRPC2Test" ${SOURCES_JSONRPC2_TEST})
#target_link_libraries("test_FormatterJsonRPC2Test" ${LIBRARIES})
#add_test(test_FormatterJsonRPC2Test test_FormatterJsonRPC2Test)