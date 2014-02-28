include_directories (
  ./include
  ../utils/include/
  ../transport_manager/include/
  ../connection_handler/include/
  ../secure_service_manager/include/
  ../config_profile/include/
)

set (SOURCES
    ./src/rpc_type.cc
    ./src/raw_message.cc
    ./src/protocol_handler_impl.cc
    ./src/protocol_packet.cc
    ./src/protocol_payload.cc
    ./src/message_priority.cc
    ./src/service_type.cc
)

add_library("ProtocolHandler" ${SOURCES})
