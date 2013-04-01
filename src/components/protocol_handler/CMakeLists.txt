include_directories (
  ./include
  ../Utils/include/
  ../TransportManager/include/
  ../../thirdPartyLibs/logger/include/
  ../../thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
    ./src/raw_message.cc
    ./src/protocol_handler_impl.cc
    ./src/protocol_packet.cc
    ./src/message_from_mobile_app_handler.cc
    ./src/messages_to_mobile_app_handler.cc
)

add_library("ProtocolHandler" ${SOURCES})