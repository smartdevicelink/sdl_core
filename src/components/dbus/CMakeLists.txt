include_directories (
  ./include/
  ${CMAKE_BINARY_DIR}/
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_BINARY_DIR}/src/components/smart_objects/include/
  ${CMAKE_BINARY_DIR}/src/components/utils/include/
  ${CMAKE_BINARY_DIR}/src/components/dbus/include/
  /usr/include/dbus-1.0/
  /usr/lib/i386-linux-gnu/dbus-1.0/include/
)

set (SOURCES
  ./src/schema.cc
  ./src/ford_message_descriptions.cc
  ./src/dbus_message_controller.cc
  ./src/dbus_adapter.cc
)

add_library("DBus" ${SOURCES})
target_link_libraries ("DBus")
