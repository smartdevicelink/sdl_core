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
    ./src/message_priority.cc
    ./src/service_type.cc
)

add_library("ProtocolHandler" ${SOURCES})
target_link_libraries ("ProtocolHandler" log4cxx)
