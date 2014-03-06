find_package(DBus)

include_directories(
  include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
  ${DBUS_INCLUDE_DIRS}
)

set (SOURCES
 src/rpc_base/dbus_message.cc
 src/rpc_base/rpc_base.cc
 src/rpc_base/rpc_message.cc
)

set (HEADERS
  include/rpc_base/dbus_message.h
  include/rpc_base/rpc_base_dbus_inl.h
  include/rpc_base/rpc_base.h
  include/rpc_base/rpc_base_inl.h
  include/rpc_base/rpc_base_json_inl.h
  include/rpc_base/rpc_message.h
)

add_library(rpc_base ${HEADERS} ${SOURCES})
target_link_libraries(rpc_base jsoncpp dbus-1)
