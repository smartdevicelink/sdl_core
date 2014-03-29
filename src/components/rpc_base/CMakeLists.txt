include_directories(
  include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
)

set (SOURCES
 src/rpc_base/rpc_base.cc
)

set (HEADERS
  include/rpc_base/rpc_message.h
  include/rpc_base/rpc_base.h
  include/rpc_base/rpc_base_inl.h
)

add_library(rpc_base ${HEADERS} ${SOURCES})
target_link_libraries(rpc_base jsoncpp)
