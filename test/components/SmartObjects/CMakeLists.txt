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

create_test("test_SmartObject_DraftTest" "./SmartObjectDraftTest.cpp" "${LIBRARIES}")
create_test("test_SmartObject_UnitTest" "./SmartObjectUnitTest.cpp" "${LIBRARIES}")
create_test("test_SmartObject_StressTest" "./SmartObjectStressTest.cpp" "${LIBRARIES}")
create_test("test_SmartObject_InvalidTest" "./SmartObjectInvalidTest.cpp" "${LIBRARIES}")
create_test("test_SmartObject_ConvertionTimeTest" "./SmartObjectConvertionTimeTest.cpp" "${LIBRARIES}")
create_test("test_TSharedPtrTest" "./TSharedPtrTest.cpp" "${LIBRARIES}")
