include_directories(
  ./include
  ../utils/include/
  ../connection_handler/include/
  ../config_profile/include/
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set(SOURCES
    ./src/protocol_handler_impl.cc
    ./src/protocol_packet.cc
    ./src/protocol_payload.cc
)

set(LIBRARIES
  ProtocolLibrary
  Utils
)

add_library(ProtocolHandler ${SOURCES})
target_link_libraries(ProtocolHandler ${LIBRARIES})
