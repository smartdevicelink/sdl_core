include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/SmartObjects/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

# Sources are separated for each SmartObject test
set (SOURCES_DRAFT_TEST
    ./SmartObjectDraftTest.cpp
)

set (SOURCES_BASIC_TEST
    ./SmartObjectBasicTest.cpp
)

# Libraries are the same for all SmartObject test
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    SmartObjects
    log4cplus
)

add_executable("test_SmartObjectsDraftTest" ${SOURCES_DRAFT_TEST})
target_link_libraries("test_SmartObjectsDraftTest" ${LIBRARIES})
add_test(test_SmartObjectsDraftTest test_SmartObjectsDraftTest)

add_executable("test_SmartObjectBasicTest" ${SOURCES_BASIC_TEST})
target_link_libraries("test_SmartObjectBasicTest" ${LIBRARIES})
add_test(test_SmartObjectBasicTest test_SmartObjectBasicTest)

