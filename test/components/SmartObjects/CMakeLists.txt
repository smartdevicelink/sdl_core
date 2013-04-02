# --- SchemaItems
add_subdirectory(./SchemaItem)

include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/SmartObjects/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ../../../src/components/JSONHandler/include/
  ../../../src//thirdPartyLibs/jsoncpp/include/

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

set (SOURCES_INVALID_TEST
    ./SmartObjectInvalidTest.cpp
)

set (SOURCES_CONVERTION_TIME_TEST
    SmartObjectConvertionTimeTest.cpp
)

# Libraries are the same for all SmartObject test
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    SmartObjects
    JSONHandler
    jsoncpp
    log4cplus
)

create_test("test_SmartObject_DraftTest" ${SOURCES_DRAFT_TEST} ${LIBRARIES})
create_test("test_SmartObject_UnitTest" ${SOURCES_UNIT_TEST} ${LIBRARIES})
create_test("test_SmartObject_StressTest" ${SOURCES_STRESS_TEST} ${LIBRARIES})
create_test("test_SmartObject_InvalidTest" ${SOURCES_INVALID_TEST} ${LIBRARIES})
create_test("test_SmartObject_ConvertionTimeTest" ${SOURCES_CONVERTION_TIME_TEST} ${LIBRARIES})
create_test("test_TSharedPtrTest" ${SOURCES_TSHAREDPTR_TEST} ${LIBRARIES})
