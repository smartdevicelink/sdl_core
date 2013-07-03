# --- GoogleTest example
#add_subdirectory(./gtest-example)

# --- components tests
add_subdirectory(./components)
add_subdirectory(./thirdPartyLibs)

if(BUILD_ONLY_TM)
add_definitions(-DBUILD_ONLY_TM)
include_directories (
  ../src/thirdPartyLibs/gmock-1.6.0/include
  ../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../src/components/transport_manager/include
  ../src/thirdPartyLibs/logger/include/
  ../src/components/utils/include/
  ../test/thirdPartyLibs/jsoncpp/include
  ../test/components/application_manager
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_BINARY_DIR}
)
else()
include_directories (
  ../src/thirdPartyLibs/gmock-1.6.0/include
  ../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../src/components/application_manager/include
  ../src/components/request_watchdog/include
  ../src/components/protocol_handler/include
  ../src/components/TransportManager/include
  ../src/components/connection_handler/include
  ../src/components/JSONHandler/include
  ../src/components/SmartObjects/include
  ../src/thirdPartyLibs/jsoncpp/include/
  ../src/thirdPartyLibs/logger/include/
  ../src/components/utils/include/
  ../test/components/request_watchdog/include/
  ../test/components/protocol_handler/include/
  ../test/components/utils/include
  ../test/components/json_handler/include
  ../test/components/SmartObjects
  ../test/components/
  ../test/thirdPartyLibs/jsoncpp/include
  ../test/components/application_manager
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_BINARY_DIR}
)
endif()

if(BUILD_ONLY_TM)
set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    test_TransportManagerTest
    jsoncpp
)
else()
set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    RequestWatchdog
    JSONHandler
    HMI_API
    SmartObjects
    ApplicationManager
    test_JSONHandler_v4_protocol_v2_0_revP
    test_JSONHandlerTest
    test_SmartObjectTest
    test_FormattersCommandsTest
    test_UtilsTest
    test_RequestWatchdogTest
    test_ProtocolHandlerTest
    test_JSONCPPTest
    v4_protocol_v2_0_revT
    jsoncpp
    rt
    Utils
)
add_executable("test_suit" "./test_suit.cc")
target_link_libraries("test_suit" ${LIBRARIES})
endif()


