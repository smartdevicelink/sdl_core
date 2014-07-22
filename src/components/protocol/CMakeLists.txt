include_directories(
  ./include
)

set(SOURCES
  ./src/raw_message.cc
  ./src/service_type.cc
  ./src/message_priority.cc
  ./src/rpc_type.cc
)

add_library(ProtocolLibrary ${SOURCES})
