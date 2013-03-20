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

set (SOURCES_UNIT_TEST
    ./SmartObjectUnitTest.cpp
)

set (SOURCES_STRESS_TEST
    ./SmartObjectStressTest.cpp
)

set (SOURCES_TSHAREDPTR_TEST
    ./TSharedPtrTest.cpp
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

add_executable("test_SmartObjectDraftTest" ${SOURCES_DRAFT_TEST})
target_link_libraries("test_SmartObjectDraftTest" ${LIBRARIES})
add_test(test_SmartObjectDraftTest test_SmartObjectDraftTest)

add_executable("test_SmartObjectUnitTest" ${SOURCES_UNIT_TEST})
target_link_libraries("test_SmartObjectUnitTest" ${LIBRARIES})
add_test(test_SmartObjectUnitTest test_SmartObjectUnitTest)

add_executable("test_SmartObjectStressTest" ${SOURCES_STRESS_TEST})
target_link_libraries("test_SmartObjectStressTest" ${LIBRARIES})
add_test(test_SmartObjectStressTest test_SmartObjectStressTest)

add_executable("test_TSharedPtrTest" ${SOURCES_TSHAREDPTR_TEST})
target_link_libraries("test_TSharedPtrTest" ${LIBRARIES})
add_test(test_TSharedPtrTest test_TSharedPtrTest)
