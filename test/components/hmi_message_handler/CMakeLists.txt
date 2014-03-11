include_directories (
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include/
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/logger/include/
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
  ${CMAKE_SOURCE_DIR}/src/components/application_manager/include/
  ${CMAKE_SOURCE_DIR}/src/components/hmi_message_handler/include/
  ${CMAKE_SOURCE_DIR}/src/components/protocol_handler/include/
  ${CMAKE_SOURCE_DIR}/src/components/smart_objects/include/
  ${CMAKE_SOURCE_DIR}/test/components/hmi_message_handler/include/
  ${CMAKE_BINARY_DIR}/src/components/
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    Utils
    ProtocolHandler
    ApplicationManager
    SmartObjects
    jsoncpp
    HMIMessageHandler
)

set (SOURCES
   #./src/mock_subscriber.cc
   #./src/test_dbus_message_adapter.cc
)

if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTEST_HAS_PTHREAD=1")
endif()

#create_test("test_DBusMessageAdapter" "${SOURCES}" "${LIBRARIES}")
create_test("test_mqueue_adapter" "./src/test_mqueue_adapter.cc" "${LIBRARIES}")

