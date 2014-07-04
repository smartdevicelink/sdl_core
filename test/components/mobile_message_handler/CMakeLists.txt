include_directories (
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/application_manager/include
  ../../../src/components/mobile_message_handler/include/
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
)

create_test("test_mobile_message_handler"
  "./src/mobile_message_handler_test.cc"
  "${LIBRARIES}")

if(ENABLE_LOG)
  target_link_libraries("test_mobile_message_handler" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library("test_MobileMessageHandlerTest" "./src/mobile_message_handler_test.cc")
