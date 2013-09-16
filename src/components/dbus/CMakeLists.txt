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
  ${CMAKE_CURRENT_BINARY_DIR}/message_descriptions.cc
  ./src/dbus_message_controller.cc
  ./src/dbus_adapter.cc
)

add_library("DBus" ${SOURCES})

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/message_descriptions.cc
  COMMAND python codegen/make_message_descriptions.py --infile ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/HMI_API.xml --outfile ${CMAKE_CURRENT_BINARY_DIR}/message_descriptions.cc
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/HMI_API.xml codegen/make_message_descriptions.py
)

target_link_libraries ("DBus")
