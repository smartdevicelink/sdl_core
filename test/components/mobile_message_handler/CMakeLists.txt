include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/application_manager/include
  ../../../src/components/protocol_handler/include
  ../../../src/components/mobile_message_handler/include/
  ../../../src/components/transport_manager/include/
  ../../../src/components/utils/include/
  ../../../test/components/mobile_message_handler/include
)

set(LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    MobileMessageHandler
    ProtocolHandler
    ApplicationManager
    Utils
    rt
)

create_test("test_mobile_message_handler"
	"./src/mobile_message_handler_test.cc"
	"${LIBRARIES}")

target_link_libraries ("test_mobile_message_handler"
	log4cxx)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library("test_MobileMessageHandlerTest" "./src/mobile_message_handler_test.cc")
