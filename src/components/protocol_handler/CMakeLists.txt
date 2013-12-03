include_directories (
  ./include
  ../utils/include/
  ../transport_manager/include/
  ../connection_handler/include/
)

set (SOURCES
    ./src/raw_message.cc
    ./src/protocol_handler_impl.cc
    ./src/protocol_packet.cc
    ./src/message_from_mobile_app_handler.cc
    ./src/messages_to_mobile_app_handler.cc
)

add_library("ProtocolHandler" ${SOURCES})
