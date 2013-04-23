include_directories (
  ./include/
  ../Utils/include/
  ../protocol_handler/include/
  ../../thirdPartyLibs/logger/include
  ../../thirdPartyLibs/logger/log4cplus-1.1.0/include
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
./src/message.cc
./src/mobile_message_handler_impl.cc
./src/messages_from_mobile_app_handler.cc
)


add_library("MobileMessageHandler"
  ${SOURCES}
)
