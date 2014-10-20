include_directories(
  ./include
  ${APR_INCLUDE_DIRECTORY}
)

set(SOURCES
  ./src/raw_message.cc
  ./src/service_type.cc
  ./src/message_priority.cc
  ./src/rpc_type.cc
)

add_library(ProtocolLibrary ${SOURCES})
target_link_libraries(ProtocolLibrary Utils)
