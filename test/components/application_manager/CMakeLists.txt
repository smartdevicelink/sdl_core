include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/application_manager/include
  ../../../src/components/SmartObjects/include
  ../../../src/components/JSONHandler/include/
  ../../../src/components/utils/include/
  ../../../src//thirdPartyLibs/jsoncpp/include/
    ${CMAKE_BINARY_DIR}/src/components/Interfaces/
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    SmartObjects
    ApplicationManager
    JSONHandler
    jsoncpp
)

set (SOURCES
  ./api_converter_v1_test.cpp
)

#add_library("test_ApplicationManagerTest" "${SOURCES}")
