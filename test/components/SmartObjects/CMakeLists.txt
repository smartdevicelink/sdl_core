include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/SmartObjects/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

# Sources are separated for each Transport Manager test
set (SOURCES_DRAFT_TEST
    ./SmartObjectDraftTest.cpp
)

# Libraries seeems to be the same for all Transport Manager test
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