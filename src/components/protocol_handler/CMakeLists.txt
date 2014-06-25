include_directories(
  ./include
  ../utils/include/
  ../transport_manager/include/
  ../connection_handler/include/
  ${SecurityManagerIncludeDir}
  ../config_profile/include/
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set(SOURCES
    ./src/rpc_type.cc
    ./src/raw_message.cc
    ./src/protocol_handler_impl.cc
    ./src/protocol_packet.cc
    ./src/protocol_payload.cc
    ./src/message_priority.cc
    ./src/service_type.cc
)

set(LIBRARIES
  ${SecurityManagerLibrary}
)

add_library(ProtocolHandler ${SOURCES})
target_link_libraries(ProtocolHandler ${LIBRARIES})
